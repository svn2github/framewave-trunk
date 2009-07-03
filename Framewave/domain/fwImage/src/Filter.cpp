/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"

#include <vector>

extern SYS_FORCEALIGN_16 const unsigned int alphaMasks_16[4][4];
extern SYS_FORCEALIGN_16 const unsigned int alphaMasks_32[4][4];


namespace OPT_LEVEL
{
    // @TODO: SharpenFilter is not using the same filter framework code as all other image filter functions.
    //        consider porting it to use the framework and eliminate much of code duplication.

namespace SharpenFilter
{
    template< typename DataType >
    struct Data
    {
        const DataType * pSrc;
        int srcstep;
        DataType * pDst;
        int dstStep;
        FwiSize dstRoiSize;
    };

    // reference implementation of FilterSharpen.
    // this function will calculate the result for one channel value of a single destination pixel
    template< typename SrcDstType, typename CalcType, CH chCount >
    struct REF_FN
    {
        static inline void el_fn( const SrcDstType *pSource, const int srcstep, SrcDstType *pDst )
        {
            const Fw8u *pSrc = (const Fw8u*)pSource;
            CalcType t1, t2, t3, m1, m2, m3, b1, b2, b3, tmp;
            t1 = (CalcType)*(const SrcDstType*)&pSrc[ -srcstep-chCount*sizeof(SrcDstType) ];
            t2 = (CalcType)*(const SrcDstType*)&pSrc[ -srcstep ];
            t3 = (CalcType)*(const SrcDstType*)&pSrc[ -srcstep+chCount*sizeof(SrcDstType) ];
            m1 = (CalcType)*(const SrcDstType*)&pSrc[     0  -chCount*sizeof(SrcDstType) ];
            m2 = (CalcType)*(const SrcDstType*)&pSrc[     0     ];
            m3 = (CalcType)*(const SrcDstType*)&pSrc[        +chCount*sizeof(SrcDstType) ];
            b1 = (CalcType)*(const SrcDstType*)&pSrc[ +srcstep-chCount*sizeof(SrcDstType) ];
            b2 = (CalcType)*(const SrcDstType*)&pSrc[ +srcstep ];
            b3 = (CalcType)*(const SrcDstType*)&pSrc[ +srcstep+chCount*sizeof(SrcDstType) ];

            t1 = t1 + m1 + b1;
            t2 = t2 + m2 + b2;
            t3 = t3 + m3 + b3;

            tmp = m2 * (CalcType)17;
            tmp = tmp - t1;
            tmp = tmp - t2;
            tmp = tmp - t3;

            *pDst = FW_REF::Limits<SrcDstType>::Sat( tmp/(CalcType)8 );
        }
    };

    template< typename DataType, int chCount >
    struct PERF_FN
    {
        static void SSE2( void *pData );
        static void SSE2_Unaligned( void *pData );
    };

    //  this is the reference implementation of the filterSharpen code.
    // This routine will step through all rows of the destination image
    // for each row it will step through all pixels/columns and calculate the result for each color channel
    template< CH chCount, typename DataType, typename CalcType >
    struct RefrImpl
    {
        static FwStatus REFR( const DataType * pSrc, const int srcstep, DataType * pDst, const int dstStep, FwiSize dstRoiSize )
        {
            DataType * pDataEnd = (DataType*)(((Fw8u*)pDst) + dstRoiSize.height * dstStep);
            
            for( ; pDst<pDataEnd; pDst = (DataType*)(((Fw8u*)pDst)+dstStep), pSrc = (DataType*)(((Fw8u*)pSrc)+srcstep) )            // For each horizontal line
            {
                const DataType *ps  = pSrc;
                DataType *pd  = pDst;
                const DataType *pEnd = pd + dstRoiSize.width * chCount;
        
                for( ; pd<pEnd; ++ps, ++pd )                            // For each entry in the line
                REF_FN<DataType, CalcType, chCount>::el_fn( ps, srcstep, pd );
            }
            return fwStsNoErr;
        }
    };

    // this is the AC4 version of the function above.  It will not override the channel 4 data in the destination buffer
    template< typename DataType, typename CalcType >
    struct RefrImpl< AC4, DataType, CalcType >
    {
        static FwStatus REFR( const DataType * pSrc, const int srcstep, DataType * pDst, const int dstStep, FwiSize dstRoiSize )
        {
            DataType * pDataEnd = (DataType*)(((Fw8u*)pDst) + dstRoiSize.height * dstStep);
            
            for( ; pDst<pDataEnd; pDst = (DataType*)(((Fw8u*)pDst)+dstStep), pSrc = (DataType*)(((Fw8u*)pSrc)+srcstep) )            // For each horizontal line
            {
                const DataType *ps  = pSrc;
                DataType *pd  = pDst;
                const DataType *end = pd + dstRoiSize.width * 4;
        
                for( ; pd<end; )                            // For each entry in the line
                {
                REF_FN< DataType, CalcType, C4 >::el_fn( ps, srcstep, pd );
                ++ps;
                ++pd;
                REF_FN< DataType, CalcType, C4 >::el_fn( ps, srcstep, pd );
                ++ps;
                ++pd;
                REF_FN< DataType, CalcType, C4 >::el_fn( ps, srcstep, pd );
                ps += 2;
                pd += 2;
                }
            }
            return fwStsNoErr;
        }
    };

    // this is the main body of the FilterSharpen function implementation.
    // this function will validate the parameters and call the reference or SSE2 optimized code path as appropriate.
    // for SSE2 code it will also start worker threads on multi-core hardware
    template< CH chCount, typename DataType, typename CalcType >
    static FwStatus iFilterSharpen( const DataType * pSrc, int srcstep, DataType * pDst, int dstStep, FwiSize dstRoiSize )
    {
        if( pSrc == NULL || pDst == NULL )
            return fwStsNullPtrErr;
                                    
        if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
            return fwStsSizeErr;

        if (srcstep <= 0 || dstStep <= 0)	
            return fwStsStepErr;

        switch( Dispatch::Type<DT_SSE2>() )
        {
        default:
        case DT_SSE3: 
        case DT_SSE2: 
            {
                // get the number of worker threads
                U32 tCount = ThreadPool::ThreadCount();
                std::vector< Data< DataType > > data;
                data.resize( tCount );

                U32 height = dstRoiSize.height / tCount;

                const DataType * ps = pSrc;
                    DataType * pd = pDst;

                // spin off worker threads if appropriate
                for( U32 i=0; i<tCount; ++i )
                {				
                    data[i].pSrc				= ps;
                    data[i].pDst				= pd;	
                    data[i].srcstep				= srcstep;				
                    data[i].dstStep				= dstStep;
                    data[i].dstRoiSize.width	= dstRoiSize.width;
                    if( i == 0 )
                        data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                    else
                        data[i].dstRoiSize.height = height;

                    if( ( ((size_t)ps) & (sizeof(DataType)-1) ) || ( srcstep & 0x0F ) ) // is src data aligned or the src row step size 16 bytes aligned?
                    {
                        // call the unaligned code path
                        ThreadPool::Run( PERF_FN< DataType, chCount >::SSE2_Unaligned, &data[i] );
                    }
                    else
                    {
                        // call the aligned code path
                        ThreadPool::Run( PERF_FN< DataType, chCount >::SSE2, &data[i] );
                    }

                    ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcstep );
                    pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                }

                ThreadPool::Wait();			

                return fwStsNoErr; 
            }
    
        case DT_REFR:
            // call the reference implementation
            return RefrImpl<chCount, DataType, CalcType >::REFR( pSrc, srcstep, pDst, dstStep, dstRoiSize ); 
        }	
    }

    //
    // SSE2
    //

    template<>
    struct PERF_FN< Fw8u, C1 >
    {
        // SSE2 optimized implementation for 8U 1 channel aligned data
        static void SSE2( void *pData )
        {	
            Data< Fw8u > & data = *(Data< Fw8u >*)pData;
            const Fw8u * pSrc  = (const Fw8u *)data.pSrc;	
            int      srcstep    = data.srcstep;	
            Fw8u *  pDst	    = (Fw8u *)data.pDst;
            int      dstStep    = data.dstStep;	
            FwiSize dstRoiSize = data.dstRoiSize;

            XMM128 c17;
            XMM128 zero;
            c17.i = _mm_set1_epi16( 0x0011 );
            zero.i = _mm_set1_epi16( 0x0000 );
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + sizeof(Fw8u);	// Now pSrc points at the first B in each line	
    
            Fw8u *pEnd = pDst + dstRoiSize.height * dstStep;
    
            for( ; pDst<pEnd; pDst+=dstStep, pSrc+=srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                XMM128 t0,t1, m0,m1, b0,b1;
                Fw16u mNxt, next0, next1;
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw8u, Fw16s, C1>::el_fn( ps+srcstep+1, srcstep, pd );
    
                /////////////////////////////////////////////////////////////////
                // process the aligned data (in 16 bytes chunks)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    // Load the data
                    t0.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack8UTo16U( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack8UTo16U( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack8UTo16U( b0.i, b1.i );	// Bottom line of the kernel	
                    mNxt       = (Fw16u)*(ps+1*srcstep+16);
    
                    // Add vertical columns
                    next0 = (Fw16u)*(ps+0*srcstep+16) + (Fw16u)*(ps+1*srcstep+16) + (Fw16u)*(ps+2*srcstep+16);
                    next1 = (Fw16u)*(ps+0*srcstep+17) + (Fw16u)*(ps+1*srcstep+17) + (Fw16u)*(ps+2*srcstep+17);
                    t0.i = _mm_add_epi16( t0.i, _mm_add_epi16(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;
                    t1.i = _mm_add_epi16( t1.i, _mm_add_epi16(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;
                    //next[0][0] = next[0][0] + next[0][1] + next[0][2];
                    //next[1][0] = next[1][0] + next[1][1] + next[1][2];
    
                    // Calculate / write	
                    // dst[i] =   16*src[i]   + src[i]	-  (sum of 3x3 pixels centered at i) 
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    CBL_SSE2::ShiftAndCarry_R_16s( m0.i, m1.i, mNxt );
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_mullo_epi16( m0.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
                    b1.i = _mm_mullo_epi16( m1.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
    
                    // subtract column N
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 1
                    CBL_SSE2::ShiftAndCarry_R_16s( t0.i, t1.i, next0 );
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 2
                    CBL_SSE2::ShiftAndCarry_R_16s( t0.i, t1.i, next1 );
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // make sure that there are no negative numbers and divide by 8
                    b0.i = _mm_srai_epi16( b0.i, 3 );
                    b1.i = _mm_srai_epi16( b1.i, 3 );
    
                    b0.i = _mm_packus_epi16( b0.i, b1.i );	// Saturate the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    REF_FN<Fw8u, Fw16s, C1>::el_fn( ps+srcstep+1, srcstep, pd );
            }
        }

        // SSE2 optimized implementation for 8U 1 channel unaligned data
        static void SSE2_Unaligned( void *pData )
        {
            Data< Fw8u > & data = *(Data< Fw8u >*)pData;
            const Fw8u * pSrc  = data.pSrc;
            int      srcstep    = data.srcstep;	
            Fw8u *  pDst	    = data.pDst;	
            int      dstStep    = data.dstStep;	
            FwiSize dstRoiSize = data.dstRoiSize;				
    
            XMM128 c17;
            XMM128 zero;
            c17.i = _mm_set1_epi16( 0x0011 );
            zero.i = _mm_set1_epi16( 0x0000 );
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc = (Fw8u*)(((Fw8u*)pSrc) - (srcstep + 1));	// Now pSrc points at the first B in each line	
    
            Fw8u *pEnd = pDst + dstRoiSize.height * dstStep;
    
            for( ; pDst<pEnd; pDst+=dstStep, pSrc+=srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width;
                U32 midwidth   = width & 0xFFFFFFF0;		// Bytes at end that can't be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                XMM128 t0,t1, m0,m1, b0,b1, tmp;
                int next0, next1;
                Fw16u mNxt;
    
                /////////////////////////////////////////////////////////////////
                // process the aligned data (in 16 bytes chunks)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    // Load the data
                    t0.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack8UTo16U( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack8UTo16U( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack8UTo16U( b0.i, b1.i );	// Bottom line of the kernel	
                    mNxt       = (Fw16u)*(ps+1*srcstep+16);
    
                    // Add vertical columns
                    next0 = (Fw16u)*(ps+0*srcstep+16) + (Fw16u)*(ps+1*srcstep+16) + (Fw16u)*(ps+2*srcstep+16);
                    next1 = (Fw16u)*(ps+0*srcstep+17) + (Fw16u)*(ps+1*srcstep+17) + (Fw16u)*(ps+2*srcstep+17);
                    t0.i = _mm_add_epi16( t0.i, _mm_add_epi16(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;
                    t1.i = _mm_add_epi16( t1.i, _mm_add_epi16(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;
                    //next[0][0] = next[0][0] + next[0][1] + next[0][2];
                    //next[1][0] = next[1][0] + next[1][1] + next[1][2];
    
                    // Calculate / write	
                    // dst[i] =   16*src[i]   + src[i]	-  (sum of 3x3 pixels centered at i) 
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    tmp.i = _mm_slli_si128( m1.i, 14 );
                    m0.i = _mm_srli_si128( m0.i, 2 );
                    m1.i = _mm_srli_si128( m1.i, 2 );
                    m0.i = _mm_or_si128( m0.i, tmp.i );
			        m1.i = _mm_insert_epi16( m1.i, mNxt, 7 );
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_mullo_epi16( m0.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
                    b1.i = _mm_mullo_epi16( m1.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
    
                    // subtract column N
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 1
                    tmp.i = _mm_slli_si128( t1.i, 14 );
                    t0.i = _mm_srli_si128( t0.i, 2 );
                    t1.i = _mm_srli_si128( t1.i, 2 );
                    t0.i = _mm_or_si128( t0.i, tmp.i );
                    t1.i = _mm_insert_epi16( t1.i, next0, 7 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 2
                    tmp.i = _mm_slli_si128( t1.i, 14 );
                    t0.i = _mm_srli_si128( t0.i, 2 );
                    t1.i = _mm_srli_si128( t1.i, 2 );
                    t0.i = _mm_or_si128( t0.i, tmp.i );
                    t1.i = _mm_insert_epi16( t1.i, next1, 7 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // make sure that there are no negative numbers and divide by 8
                    b0.i = _mm_srai_epi16( b0.i, 3 );
                    b1.i = _mm_srai_epi16( b1.i, 3 );
    
                    b0.i = _mm_packus_epi16( b0.i, b1.i );	// Saturate the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    REF_FN<Fw8u, Fw16s, C1>::el_fn( ps+srcstep+1, srcstep, pd );
            }
        }
    };
    
    template<>
    struct PERF_FN< Fw8u, C3 >
    {
        // SSE2 optimized implementation for 8U 3 channel aligned data
        static void SSE2( void * pData )
        {	
            Data< Fw8u > & data = *(Data< Fw8u >*)pData;
            const Fw8u * pSrc  = data.pSrc;	
            int      srcstep    = data.srcstep;	
            Fw8u *  pDst	    = data.pDst;	
            int      dstStep    = data.dstStep;	
            FwiSize dstRoiSize = data.dstRoiSize;				
    
            XMM128 c17;
            c17.i = _mm_set1_epi16( 0x0011 );
    
            // I assume that srcstep is a multiple of 16
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= (srcstep + C3);	// Now pSrc points at the first B in each line	
    
            Fw8u *pEnd = pDst + dstRoiSize.height * dstStep;
            XMM128 zero;
            zero.i = _mm_setzero_si128();
    
            U32 width      = dstRoiSize.width * C3;
            U32 startWidth = FW_REF::BytesToAlign16(pSrc);			// Bytes before alignment point.
            U32 endWidth   = (width-startWidth) & 0x0F;			// Bytes at end that can't be processed 16 wide.
            U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                
            if( width < 16 )
            {
                startWidth = 0;
                midwidth = 0;
            }
                
            for( ; pDst<pEnd; pDst+=dstStep, pSrc+=srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw8u, Fw16s, C3>::el_fn( ps+srcstep+C3, srcstep, pd );
    
                /////////////////////////////////////////////////////////////////
                // process the aligned data (in 16 bytes chunks)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {	
                    XMM128 t0,t1, m0,m1, b0,b1, tmp;
                    XMM128 next, mNxt;
                    //int next0, next1, next2, next3, next4, next5;
                    //int mNxt0, mNxt1, mNxt2;
    
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    // Load the data
                    next.d = _mm_load_sd( (double*)( ps+0*srcstep+14 ) );
                    next.i = _mm_unpacklo_epi8( next.i, zero.i );
                    mNxt.d = _mm_load_sd( (double*)( ps+1*srcstep+14 ) );
                    mNxt.i = _mm_unpacklo_epi8( mNxt.i, zero.i );
                    b0.d = _mm_load_sd( (double*)( ps+2*srcstep+14 ) );
                    b0.i = _mm_unpacklo_epi8( b0.i, zero.i );
                    next.i = _mm_add_epi16( next.i, _mm_add_epi16(mNxt.i,b0.i) );	 // t0 = t0 + m0 + b0;
    
                    t0.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack8UTo16U( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack8UTo16U( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack8UTo16U( b0.i, b1.i );	// Bottom line of the kernel
    
                    // Add vertical columns
                    t0.i = _mm_add_epi16( t0.i, _mm_add_epi16(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;
                    t1.i = _mm_add_epi16( t1.i, _mm_add_epi16(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;
    
                    // Calculate / write	
                    // dst[i] =   16*src[i]   + src[i]	-  (sum of 3x3 pixels centered at i) 
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    tmp.i = _mm_slli_si128( m1.i, 10 );
                    m0.i = _mm_srli_si128( m0.i, 6 );
                    m1.i = _mm_srli_si128( m1.i, 6 );
                    m0.i = _mm_or_si128( m0.i, tmp.i );
                    mNxt.i = _mm_srli_si128( mNxt.i, 4 );
                    mNxt.i = _mm_slli_si128( mNxt.i, 10 );
                    m1.i = _mm_or_si128( m1.i, mNxt.i );
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_mullo_epi16( m0.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
                    b1.i = _mm_mullo_epi16( m1.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
    
                    // subtract column N
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 1
                    // shift t0 and t1 to the right by 3 values
                    tmp.i = _mm_slli_si128( t1.i, 10 );
                    t0.i = _mm_srli_si128( t0.i, 6 );
                    t1.i = _mm_srli_si128( t1.i, 6 );
                    t0.i = _mm_or_si128( t0.i, tmp.i );
                    // shift in the next 3 values
                    tmp.i = _mm_srli_si128( next.i, 4 );
                    tmp.i = _mm_slli_si128( tmp.i, 10 );
                    t1.i = _mm_or_si128( t1.i, tmp.i );
                    // subtract
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 2
                    // shift t0 and t1 to the right by 3 values
                    tmp.i = _mm_slli_si128( t1.i, 10 );
                    t0.i = _mm_srli_si128( t0.i, 6 );
                    t1.i = _mm_srli_si128( t1.i, 6 );
                    t0.i = _mm_or_si128( t0.i, tmp.i );
                    // shit in the next 3 values
                    next.i = _mm_srli_si128( next.i, 10 );
                    next.i = _mm_slli_si128( next.i, 10 );
                    t1.i = _mm_or_si128( t1.i, next.i );
                    // subtract
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // divide by 8
                    b0.i = _mm_srai_epi16( b0.i, 3 );
                    b1.i = _mm_srai_epi16( b1.i, 3 );
    
                    b0.i = _mm_packus_epi16( b0.i, b1.i );	// Saturate the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    REF_FN<Fw8u, Fw16s, C3>::el_fn( ps+srcstep+C3, srcstep, pd );
            }
        }
    
        // SSE2 optimized implementation for 8U 3 channel unaligned data
        static void SSE2_Unaligned( void * pData )
        {	
            Data< Fw8u > & data = *(Data< Fw8u >*)pData;
            const Fw8u * pSrc  = data.pSrc;	
            int      srcstep    = data.srcstep;	
            Fw8u *  pDst	    = data.pDst;	
            int      dstStep    = data.dstStep;	
            FwiSize dstRoiSize = data.dstRoiSize;				
    
            XMM128 c17;
            XMM128 zero;
            c17.i = _mm_set1_epi16( 0x0011 );
            zero.i = _mm_set1_epi16( 0x0000 );
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= (srcstep + C3);	// Now pSrc points at the first B in each line	
    
            Fw8u *pEnd = pDst + dstRoiSize.height * dstStep;
    
            for( ; pDst<pEnd; pDst+=dstStep, pSrc+=srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * C3;
                U32 midwidth   = width & 0xFFFFFFF0;		// Bytes at end that can't be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the aligned data (in 16 bytes chunks)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {	
                    XMM128 t0,t1, m0,m1, b0,b1, tmp;
                    int next0, next1, next2, next3, next4, next5;
                    Fw16u mNxt0, mNxt1, mNxt2;
    
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    // Load the data
                    t0.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack8UTo16U( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack8UTo16U( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack8UTo16U( b0.i, b1.i );	// Bottom line of the kernel	
                    mNxt0 = (Fw16u)*(ps+1*srcstep+16);
                    mNxt1 = (Fw16u)*(ps+1*srcstep+17);
                    mNxt2 = (Fw16u)*(ps+1*srcstep+18);
    
                    // Add vertical columns
                    next0 = (Fw16u)*(ps+0*srcstep+16) + (Fw16u)*(ps+1*srcstep+16) + (Fw16u)*(ps+2*srcstep+16);
                    next1 = (Fw16u)*(ps+0*srcstep+17) + (Fw16u)*(ps+1*srcstep+17) + (Fw16u)*(ps+2*srcstep+17);
                    next2 = (Fw16u)*(ps+0*srcstep+18) + (Fw16u)*(ps+1*srcstep+18) + (Fw16u)*(ps+2*srcstep+18);
                    next3 = (Fw16u)*(ps+0*srcstep+19) + (Fw16u)*(ps+1*srcstep+19) + (Fw16u)*(ps+2*srcstep+19);
                    next4 = (Fw16u)*(ps+0*srcstep+20) + (Fw16u)*(ps+1*srcstep+20) + (Fw16u)*(ps+2*srcstep+20);
                    next5 = (Fw16u)*(ps+0*srcstep+21) + (Fw16u)*(ps+1*srcstep+21) + (Fw16u)*(ps+2*srcstep+21);
                    t0.i = _mm_add_epi16( t0.i, _mm_add_epi16(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;
                    t1.i = _mm_add_epi16( t1.i, _mm_add_epi16(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;
    
                    // Calculate / write	
                    // dst[i] =   16*src[i]   + src[i]	-  (sum of 3x3 pixels centered at i) 
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    tmp.i = _mm_slli_si128( m1.i, 10 );
                    m0.i = _mm_srli_si128( m0.i, 6 );
                    m1.i = _mm_srli_si128( m1.i, 6 );
                    m0.i = _mm_or_si128( m0.i, tmp.i );
			     m1.i = _mm_insert_epi16( m1.i, mNxt0, 5 );
			     m1.i = _mm_insert_epi16( m1.i, mNxt1, 6 );
			     m1.i = _mm_insert_epi16( m1.i, mNxt2, 7 );
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_mullo_epi16( m0.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
                    b1.i = _mm_mullo_epi16( m1.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
    
                    // subtract column N
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 1
                    tmp.i = _mm_slli_si128( t1.i, 10 );
                    t0.i = _mm_srli_si128( t0.i, 6 );
                    t1.i = _mm_srli_si128( t1.i, 6 );
                    t0.i = _mm_or_si128( t0.i, tmp.i );
                    t1.i = _mm_insert_epi16( t1.i, next0, 5 );
                    t1.i = _mm_insert_epi16( t1.i, next1, 6 );
                    t1.i = _mm_insert_epi16( t1.i, next2, 7 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 2
                    tmp.i = _mm_slli_si128( t1.i, 10 );
                    t0.i = _mm_srli_si128( t0.i, 6 );
                    t1.i = _mm_srli_si128( t1.i, 6 );
                    t0.i = _mm_or_si128( t0.i, tmp.i );
                    t1.i = _mm_insert_epi16( t1.i, next3, 5 );
                    t1.i = _mm_insert_epi16( t1.i, next4, 6 );
                    t1.i = _mm_insert_epi16( t1.i, next5, 7 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // make sure that there are no negative numbers and divide by 8
                    b0.i = _mm_srai_epi16( b0.i, 3 );
                    b1.i = _mm_srai_epi16( b1.i, 3 );
    
                    b0.i = _mm_packus_epi16( b0.i, b1.i );	// Saturate the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    REF_FN<Fw8u, Fw16s, C3>::el_fn( ps+srcstep+C3, srcstep, pd );
            }
        }
    };

    template<>
    struct PERF_FN< Fw8u, C4 >
    {
        // SSE2 optimized implementation for 8U 4 channel aligned data
        static void SSE2( void * pData )
        {	
            Data< Fw8u > & data = *(Data< Fw8u >*)pData;
            const Fw8u * pSrc  = data.pSrc;	
            int      srcstep    = data.srcstep;	
            Fw8u *  pDst	    = data.pDst;	
            int      dstStep    = data.dstStep;	
            FwiSize dstRoiSize = data.dstRoiSize;				
    
            XMM128 c17;
            XMM128 zero;
            c17.i = _mm_set1_epi16( 0x0011 );
            zero.i = _mm_setzero_si128();
            XMM128 next_t, next_m, next_b;
            next_t.i = _mm_setzero_si128();
            next_m.i = _mm_setzero_si128();
            next_b.i = _mm_setzero_si128();
            // I assume that srcstep is a multiple of 16
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= (srcstep + C4);	// Now pSrc points at the first B in each line	
    
            Fw8u *pEnd = pDst + dstRoiSize.height * dstStep;
    
            for( ; pDst<pEnd; pDst+=dstStep, pSrc+=srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * C4;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw8u, Fw16s, C4>::el_fn( ps+srcstep+C4, srcstep, pd );
    
                /////////////////////////////////////////////////////////////////
                // process the aligned data (in 16 bytes chunks)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {	
                    XMM128 t0,t1, m0,m1, b0,b1;
    
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    // Load the data
                    t0.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack8UTo16U( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack8UTo16U( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack8UTo16U( b0.i, b1.i );	// Bottom line of the kernel	
    
                    next_t.d = _mm_loadl_pd( next_t.d, (double*)(ps + 0*srcstep + 16) );   
                    next_t.i = _mm_unpacklo_epi8( next_t.i, zero.i );
                    next_m.d = _mm_loadl_pd( next_m.d, (double*)(ps + 1*srcstep + 16) );
                    next_m.i = _mm_unpacklo_epi8( next_m.i, zero.i );
                    next_b.d = _mm_loadl_pd( next_b.d, (double*)(ps + 2*srcstep + 16) );
                    next_b.i = _mm_unpacklo_epi8( next_b.i, zero.i );
    
                    // Add vertical columns
                    next_t.i = _mm_add_epi16( next_t.i, _mm_add_epi16(next_m.i,next_b.i) );
                    t0.i = _mm_add_epi16( t0.i, _mm_add_epi16(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;
                    t1.i = _mm_add_epi16( t1.i, _mm_add_epi16(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;
    
                    // Calculate / write	
                    // dst[i] =   16*src[i]   + src[i]	-  (sum of 3x3 pixels centered at i) 
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    m0.i = _mm_srli_si128( m0.i, 8 );
                    m0.f = _mm_movelh_ps( m0.f, m1.f );
                    m1.i = _mm_srli_si128( m1.i, 8 );
                    m1.f = _mm_movelh_ps( m1.f, next_m.f );
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_mullo_epi16( m0.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
                    b1.i = _mm_mullo_epi16( m1.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
    
                    // subtract column N
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 1
                    //t0.i = _mm_srli_si128( t0.i, 8 );
                    //t0.f = _mm_movelh_ps( t0.f, t1.f );
                    t0.d = _mm_shuffle_pd( t0.d, t1.d, 1 );
                    //t1.i = _mm_srli_si128( t1.i, 8 );
                    //t1.f = _mm_movelh_ps( t1.f, next_t.f );
                    t1.d = _mm_shuffle_pd( t1.d, next_t.d, 1 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 2
                    //t0.i = _mm_srli_si128( t0.i, 8 );
                    //t0.f = _mm_movelh_ps( t0.f, t1.f );
                    t0.d = _mm_shuffle_pd( t0.d, t1.d, 1 );
                    t1.d = _mm_shuffle_pd( t1.d, next_t.d, 3 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // make sure that there are no negative numbers and divide by 8
                    b0.i = _mm_srai_epi16( b0.i, 3 );
                    b1.i = _mm_srai_epi16( b1.i, 3 );
    
                    b0.i = _mm_packus_epi16( b0.i, b1.i );	// Saturate the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    REF_FN<Fw8u, Fw16s, C4>::el_fn( ps+srcstep+C4, srcstep, pd );
            }
        }
    
        // SSE2 optimized implementation for 8U 4 channel unaligned data
        static void SSE2_Unaligned( void * pData )
        {	
            Data< Fw8u > & data = *(Data< Fw8u >*)pData;
            const Fw8u * pSrc  = data.pSrc;	
            int      srcstep    = data.srcstep;	
            Fw8u *  pDst	    = data.pDst;	
            int      dstStep    = data.dstStep;	
            FwiSize dstRoiSize = data.dstRoiSize;				
    
            XMM128 c17;
            XMM128 zero;
            c17.i = _mm_set1_epi16( 0x0011 );
            zero.i = _mm_setzero_si128();
            XMM128 next_t, next_m, next_b;
            next_t.i = _mm_setzero_si128();
            next_m.i = _mm_setzero_si128();
            next_b.i = _mm_setzero_si128();
            // I assume that srcstep is a multiple of 16
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= (srcstep + C4);	// Now pSrc points at the first B in each line	
    
            Fw8u *pEnd = pDst + dstRoiSize.height * dstStep;
    
            for( ; pDst<pEnd; pDst+=dstStep, pSrc+=srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * C4;
                U32 midwidth   = width & (~0xF);		// Aligned bytes that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the aligned data (in 16 bytes chunks)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {	
                    XMM128 t0,t1, m0,m1, b0,b1;
    
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    // Load the data
                    t0.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack8UTo16U( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack8UTo16U( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack8UTo16U( b0.i, b1.i );	// Bottom line of the kernel	
    
                    next_t.d = _mm_loadl_pd( next_t.d, (double*)(ps + 0*srcstep + 16) );   
                    next_t.i = _mm_unpacklo_epi8( next_t.i, zero.i );
                    next_m.d = _mm_loadl_pd( next_m.d, (double*)(ps + 1*srcstep + 16) );
                    next_m.i = _mm_unpacklo_epi8( next_m.i, zero.i );
                    next_b.d = _mm_loadl_pd( next_b.d, (double*)(ps + 2*srcstep + 16) );
                    next_b.i = _mm_unpacklo_epi8( next_b.i, zero.i );
    
                    // Add vertical columns
                    next_t.i = _mm_add_epi16( next_t.i, _mm_add_epi16(next_m.i,next_b.i) );
                    t0.i = _mm_add_epi16( t0.i, _mm_add_epi16(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;
                    t1.i = _mm_add_epi16( t1.i, _mm_add_epi16(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;
    
                    // Calculate / write	
                    // dst[i] =   16*src[i]   + src[i]	-  (sum of 3x3 pixels centered at i) 
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    m0.i = _mm_srli_si128( m0.i, 8 );
                    m0.f = _mm_movelh_ps( m0.f, m1.f );
                    m1.i = _mm_srli_si128( m1.i, 8 );
                    m1.f = _mm_movelh_ps( m1.f, next_m.f );
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_mullo_epi16( m0.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
                    b1.i = _mm_mullo_epi16( m1.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
    
                    // subtract column N
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 1
                    //t0.i = _mm_srli_si128( t0.i, 8 );
                    //t0.f = _mm_movelh_ps( t0.f, t1.f );
                    t0.d = _mm_shuffle_pd( t0.d, t1.d, 1 );
                    //t1.i = _mm_srli_si128( t1.i, 8 );
                    //t1.f = _mm_movelh_ps( t1.f, next_t.f );
                    t1.d = _mm_shuffle_pd( t1.d, next_t.d, 1 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 2
                    //t0.i = _mm_srli_si128( t0.i, 8 );
                    //t0.f = _mm_movelh_ps( t0.f, t1.f );
                    t0.d = _mm_shuffle_pd( t0.d, t1.d, 1 );
                    t1.d = _mm_shuffle_pd( t1.d, next_t.d, 3 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // make sure that there are no negative numbers and divide by 8
                    b0.i = _mm_srai_epi16( b0.i, 3 );
                    b1.i = _mm_srai_epi16( b1.i, 3 );
    
                    b0.i = _mm_packus_epi16( b0.i, b1.i );	// Saturate the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    REF_FN<Fw8u, Fw16s, C4>::el_fn( ps+srcstep+C4, srcstep, pd );
            }
        }
    };

    template<>
    struct PERF_FN< Fw8u, AC4 >
    {
        // SSE2 optimized implementation for 8U 4 channel with ALPHA, aligned data
        static void SSE2( void * pData )
        {
            Data< Fw8u > & data = *(Data< Fw8u >*)pData;
            const Fw8u * pSrc  = data.pSrc;	
            int      srcstep    = data.srcstep;	
            Fw8u *  pDst	    = data.pDst;	
            int      dstStep    = data.dstStep;	
            FwiSize dstRoiSize = data.dstRoiSize;
    
            XMM128 c17;
            XMM128 zero;
            XMM128 alphaMask;
            XMM128 source;
            source.i = _mm_setzero_si128();
            c17.i = _mm_set1_epi16( 0x0011 );
            zero.i = _mm_setzero_si128();
            XMM128 next_t, next_m, next_b;
            next_t.i = _mm_setzero_si128();
            next_m.i = _mm_setzero_si128();
            next_b.i = _mm_setzero_si128();
            // I assume that srcstep is a multiple of 16
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= (srcstep + C4);	// Now pSrc points at the first B in each line	
    
            Fw8u *pEnd = pDst + dstRoiSize.height * dstStep;
    
            for( ; pDst<pEnd; pDst+=dstStep, pSrc+=srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * C4;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                int offset = 1;
                for( ; pd<pEndStart; ++pd, ++ps, ++offset ) // Process 1 at a time until we hit an alignment boundary
                {
                    if( offset & 0x3 )
                    {
                        REF_FN<Fw8u, Fw16s, C4>::el_fn( ps+srcstep+C4, srcstep, pd );
                    }
                }
    
                /////////////////////////////////////////////////////////////////
                // process the aligned data (in 16 bytes chunks)
                /////////////////////////////////////////////////////////////////
    
                static const unsigned int AlphaMasks[] = { 0x000000FF, 0xFF000000, 0x00FF0000, 0x0000FF00 };
                alphaMask.i = _mm_set1_epi32( AlphaMasks[offset & 0x3] );
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {	
                    XMM128 t0,t1, m0,m1, b0,b1;
    
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    // Load the data
                    t0.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack8UTo16U( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack8UTo16U( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack8UTo16U( b0.i, b1.i );	// Bottom line of the kernel	
                    next_t.d = _mm_loadl_pd( next_t.d, (double*)(ps + 0*srcstep + 16) );   
                    next_t.i = _mm_unpacklo_epi8( next_t.i, zero.i );
                    next_m.d = _mm_loadl_pd( next_m.d, (double*)(ps + 1*srcstep + 16) );
                    next_m.i = _mm_unpacklo_epi8( next_m.i, zero.i );
                    next_b.d = _mm_loadl_pd( next_b.d, (double*)(ps + 2*srcstep + 16) );
                    next_b.i = _mm_unpacklo_epi8( next_b.i, zero.i );
    
                    // Add vertical columns
                    next_t.i = _mm_add_epi16( next_t.i, _mm_add_epi16(next_m.i,next_b.i) );
                    t0.i = _mm_add_epi16( t0.i, _mm_add_epi16(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;
                    t1.i = _mm_add_epi16( t1.i, _mm_add_epi16(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;
    
                    // Calculate / write	
                    // dst[i] =   16*src[i]   + src[i]	-  (sum of 3x3 pixels centered at i) 
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    m0.i = _mm_srli_si128( m0.i, 8 );
                    m0.f = _mm_movelh_ps( m0.f, m1.f );
                    m1.i = _mm_srli_si128( m1.i, 8 );
                    m1.f = _mm_movelh_ps( m1.f, next_m.f );
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_mullo_epi16( m0.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
                    b1.i = _mm_mullo_epi16( m1.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
    
                    // subtract column N
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 1
                    //t0.i = _mm_srli_si128( t0.i, 8 );
                    //t0.f = _mm_movelh_ps( t0.f, t1.f );
                    t0.d = _mm_shuffle_pd( t0.d, t1.d, 1 );
                    //t1.i = _mm_srli_si128( t1.i, 8 );
                    //t1.f = _mm_movelh_ps( t1.f, next_t.f );
                    t1.d = _mm_shuffle_pd( t1.d, next_t.d, 1 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 2
                    //t0.i = _mm_srli_si128( t0.i, 8 );
                    //t0.f = _mm_movelh_ps( t0.f, t1.f );
                    t0.d = _mm_shuffle_pd( t0.d, t1.d, 1 );
                    t1.d = _mm_shuffle_pd( t1.d, next_t.d, 3 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // make sure that there are no negative numbers and divide by 8
                    b0.i = _mm_srai_epi16( b0.i, 3 );
                    b1.i = _mm_srai_epi16( b1.i, 3 );
    
                    b0.i = _mm_packus_epi16( b0.i, b1.i );	// Saturate the calculated values.
    
                    source.i = _mm_loadu_si128((__m128i*)pd);
                    b1.i = _mm_andnot_si128( alphaMask.i, b0.i );
                    source.i = _mm_and_si128( source.i, alphaMask.i );
                    b1.i = _mm_or_si128( b1.i, source.i );
    
                    _mm_storeu_si128( (__m128i*)pd, b1.i );
                }				
    
                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps, ++offset )					// Process the remainder.
                {
                    if( offset & 0x3 )
                    {
                        REF_FN<Fw8u, Fw16s, C4>::el_fn( ps+srcstep+C4, srcstep, pd );
                    }
                }
            }
        }
    
        // SSE2 optimized implementation for 8U 4 channel with ALPHA, unaligned data
        static void SSE2_Unaligned( void * pData )
        {	
            Data< Fw8u > & data = *(Data< Fw8u >*)pData;
            const Fw8u * pSrc  = data.pSrc;	
            int      srcstep    = data.srcstep;	
            Fw8u *  pDst	    = data.pDst;	
            int      dstStep    = data.dstStep;	
            FwiSize dstRoiSize = data.dstRoiSize;
    
            XMM128 c17;
            XMM128 zero;
            XMM128 alphaMask;
            XMM128 source;
            source.i = _mm_setzero_si128();
            c17.i = _mm_set1_epi16( 0x0011 );
            zero.i = _mm_setzero_si128();
            XMM128 next_t, next_m, next_b;
            next_t.i = _mm_setzero_si128();
            next_m.i = _mm_setzero_si128();
            next_b.i = _mm_setzero_si128();
            // I assume that srcstep is a multiple of 16
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= (srcstep + C4);	// Now pSrc points at the first B in each line	
    
            Fw8u *pEnd = pDst + dstRoiSize.height * dstStep;
    
            for( ; pDst<pEnd; pDst+=dstStep, pSrc+=srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * C4;
                U32 midwidth   = width & (~0x0F);		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the aligned data (in 16 bytes chunks)
                /////////////////////////////////////////////////////////////////
    
                alphaMask.i = _mm_set1_epi32( 0xFF000000 );
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {	
                    XMM128 t0,t1, m0,m1, b0,b1;
    
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    // Load the data
                    t0.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack8UTo16U( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack8UTo16U( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack8UTo16U( b0.i, b1.i );	// Bottom line of the kernel	
                    next_t.d = _mm_loadl_pd( next_t.d, (double*)(ps + 0*srcstep + 16) );   
                    next_t.i = _mm_unpacklo_epi8( next_t.i, zero.i );
                    next_m.d = _mm_loadl_pd( next_m.d, (double*)(ps + 1*srcstep + 16) );
                    next_m.i = _mm_unpacklo_epi8( next_m.i, zero.i );
                    next_b.d = _mm_loadl_pd( next_b.d, (double*)(ps + 2*srcstep + 16) );
                    next_b.i = _mm_unpacklo_epi8( next_b.i, zero.i );
    
                    // Add vertical columns
                    next_t.i = _mm_add_epi16( next_t.i, _mm_add_epi16(next_m.i,next_b.i) );
                    t0.i = _mm_add_epi16( t0.i, _mm_add_epi16(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;
                    t1.i = _mm_add_epi16( t1.i, _mm_add_epi16(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;
    
                    // Calculate / write	
                    // dst[i] =   16*src[i]   + src[i]	-  (sum of 3x3 pixels centered at i) 
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    m0.i = _mm_srli_si128( m0.i, 8 );
                    m0.f = _mm_movelh_ps( m0.f, m1.f );
                    m1.i = _mm_srli_si128( m1.i, 8 );
                    m1.f = _mm_movelh_ps( m1.f, next_m.f );
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_mullo_epi16( m0.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
                    b1.i = _mm_mullo_epi16( m1.i, c17.i ); // multiply the numbers by 17 (scalar multiplication)
    
                    // subtract column N
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 1
                    //t0.i = _mm_srli_si128( t0.i, 8 );
                    //t0.f = _mm_movelh_ps( t0.f, t1.f );
                    t0.d = _mm_shuffle_pd( t0.d, t1.d, 1 );
                    //t1.i = _mm_srli_si128( t1.i, 8 );
                    //t1.f = _mm_movelh_ps( t1.f, next_t.f );
                    t1.d = _mm_shuffle_pd( t1.d, next_t.d, 1 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // subtract column N + 2
                    //t0.i = _mm_srli_si128( t0.i, 8 );
                    //t0.f = _mm_movelh_ps( t0.f, t1.f );
                    t0.d = _mm_shuffle_pd( t0.d, t1.d, 1 );
                    t1.d = _mm_shuffle_pd( t1.d, next_t.d, 3 );
    
                    b0.i = _mm_subs_epi16( b0.i, t0.i );
                    b1.i = _mm_subs_epi16( b1.i, t1.i );
    
                    // make sure that there are no negative numbers and divide by 8
                    b0.i = _mm_srai_epi16( b0.i, 3 );
                    b1.i = _mm_srai_epi16( b1.i, 3 );
    
                    b0.i = _mm_packus_epi16( b0.i, b1.i );	// Saturate the calculated values.

                    // read original destination value
                    source.i = _mm_loadu_si128((__m128i*)pd);

                    // mask with the result
                    b1.i = _mm_andnot_si128( alphaMask.i, b0.i );
                    source.i = _mm_and_si128( source.i, alphaMask.i );
                    b1.i = _mm_or_si128( b1.i, source.i );

                    // write destination value
                    _mm_storeu_si128( (__m128i*)pd, b1.i );
                }				
    
                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                int offset = 1;
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                {
                    if( offset & 0x3 )
                    {
                        REF_FN<Fw8u, Fw16s, C4>::el_fn( ps+srcstep+C4, srcstep, pd );
                    }
                }
            }
        }
    };

    template<>
    struct PERF_FN< Fw16s, C1 >
    {
        // SSE2 optimized implementation for 16S 1 channel, aligned data
        static void SSE2( void * pData )
        {	
            Data< Fw16s > & data = *(Data< Fw16s >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C1 * sizeof(Fw16s);	// Now pSrc points at the first B in each line	
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw16s);
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;			// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                for( ; pd<pEndStart; pd += sizeof(Fw16s), ps += sizeof(Fw16s) )					// Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw16s, Fw32s, C1>::el_fn( (const Fw16s*)(ps+srcstep+sizeof(Fw16s)), srcstep, (Fw16s*)pd );
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 t0, t1, m0, m1, b0, b1, next[3];
                    int mNxt;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    t0.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack16STo32S( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    mNxt = (Fw32s)*(Fw16s*)(ps+1*srcstep+8*sizeof(Fw16s));
    
                    // Add vertical columns
                    next[0].s32[0] = *(Fw16s*)(ps+0*srcstep+8*sizeof(Fw16s)) + *(Fw16s*)(ps+1*srcstep+8*sizeof(Fw16s)) + *(Fw16s*)(ps+2*srcstep+8*sizeof(Fw16s));
                    next[0].s32[1] = *(Fw16s*)(ps+0*srcstep+9*sizeof(Fw16s)) + *(Fw16s*)(ps+1*srcstep+9*sizeof(Fw16s)) + *(Fw16s*)(ps+2*srcstep+9*sizeof(Fw16s));
                    t0.i = _mm_add_epi32( t0.i, _mm_add_epi32(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;	
                    t1.i = _mm_add_epi32( t1.i, _mm_add_epi32(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;	
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    m0.f = _mm_move_ss( m0.f, m1.f );
                    m0.i = _mm_shuffle_epi32( m0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    m1.i = _mm_srli_si128( m1.i, 4 );
                    m1.s32[3] = mNxt;
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_slli_epi32( m0.i, 4 );
                    b1.i = _mm_slli_epi32( m1.i, 4 );
                    b0.i = _mm_add_epi32( b0.i, m0.i);
                    b1.i = _mm_add_epi32( b1.i, m1.i);
    
                    // subtract column N
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 1
                    t0.f = _mm_move_ss( t0.f, t1.f );
                    t0.i = _mm_shuffle_epi32( t0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    t1.f = _mm_move_ss( t1.f, next[0].f );
                    t1.i = _mm_shuffle_epi32( t1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    next[0].i = _mm_srli_si128( next[0].i, 4 );
    
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 2
                    t0.f = _mm_move_ss( t0.f, t1.f );
                    t0.i = _mm_shuffle_epi32( t0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    t1.f = _mm_move_ss( t1.f, next[0].f );
                    t1.i = _mm_shuffle_epi32( t1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // {b0, b1} = {b0, b1} / 8
                    b0.i = _mm_srai_epi32( b0.i, 3 );
                    b1.i = _mm_srai_epi32( b1.i, 3 );
    
                    // Saturate and write the calculated values.
                    b0.i = _mm_packs_epi32( b0.i, b1.i );
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw16s), ps += sizeof(Fw16s) )					// Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw16s, Fw32s, C1>::el_fn( (const Fw16s*)(ps+srcstep+sizeof(Fw16s)), srcstep, (Fw16s*)pd );				
            }
        }
    
        // SSE2 optimized implementation for 16S 1 channel, unaligned data
        static void SSE2_Unaligned( void * pData )
        {	
            Data< Fw16s > & data = *(Data< Fw16s >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C1 * sizeof(Fw16s);	// Now pSrc points at the first B in each line	
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw16s);
                U32 midwidth   = width & 0xFFFFFFF0;		// Bytes at end that can't be processed 16 wide.
    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;    // End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;       // Enf of all the data.	
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 t0, t1, m0, m1, b0, b1, next[3];
                    int mNxt;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    t0.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack16STo32S( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    mNxt = (Fw32s)*(Fw16s*)(ps+1*srcstep+8*sizeof(Fw16s));
    
                    // Add vertical columns
                    next[0].s32[0] = *(Fw16s*)(ps+0*srcstep+8*sizeof(Fw16s)) + *(Fw16s*)(ps+1*srcstep+8*sizeof(Fw16s)) + *(Fw16s*)(ps+2*srcstep+8*sizeof(Fw16s));
                    next[0].s32[1] = *(Fw16s*)(ps+0*srcstep+9*sizeof(Fw16s)) + *(Fw16s*)(ps+1*srcstep+9*sizeof(Fw16s)) + *(Fw16s*)(ps+2*srcstep+9*sizeof(Fw16s));
                    t0.i = _mm_add_epi32( t0.i, _mm_add_epi32(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;	
                    t1.i = _mm_add_epi32( t1.i, _mm_add_epi32(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;	
    
                    // shift all the values in m0 and m1 to the left, shifting in mNxt
                    m0.f = _mm_move_ss( m0.f, m1.f );
                    m0.i = _mm_shuffle_epi32( m0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    m1.i = _mm_srli_si128( m1.i, 4 );
                    m1.s32[3] = mNxt;
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_slli_epi32( m0.i, 4 );
                    b1.i = _mm_slli_epi32( m1.i, 4 );
                    b0.i = _mm_add_epi32( b0.i, m0.i);
                    b1.i = _mm_add_epi32( b1.i, m1.i);
    
                    // subtract column N
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 1
                    t0.f = _mm_move_ss( t0.f, t1.f );
                    t0.i = _mm_shuffle_epi32( t0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    t1.f = _mm_move_ss( t1.f, next[0].f );
                    t1.i = _mm_shuffle_epi32( t1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    next[0].i = _mm_srli_si128( next[0].i, 4 );
    
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 2
                    t0.f = _mm_move_ss( t0.f, t1.f );
                    t0.i = _mm_shuffle_epi32( t0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    t1.f = _mm_move_ss( t1.f, next[0].f );
                    t1.i = _mm_shuffle_epi32( t1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // {b0, b1} = {b0, b1} / 8
                    b0.i = _mm_srai_epi32( b0.i, 3 );
                    b1.i = _mm_srai_epi32( b1.i, 3 );
    
                    // Saturate and write the calculated values.
                    b0.i = _mm_packs_epi32( b0.i, b1.i );
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw16s), ps += sizeof(Fw16s) )					// Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw16s, Fw32s, C1>::el_fn( (const Fw16s*)(ps+srcstep+sizeof(Fw16s)), srcstep, (Fw16s*)pd );				
            }
        }
    };

    template<>
    struct PERF_FN< Fw16s, C3 >
    {
        // SSE2 optimized implementation for 16S 3 channel, aligned data
        static void SSE2( void * pData )
        {
            Data< Fw16s > & data = *(Data< Fw16s >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C3 * sizeof(Fw16s);	// Now pSrc points at the first B in each line	
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw16s) * C3;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;			// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                for( ; pd<pEndStart; pd += sizeof(Fw16s), ps += sizeof(Fw16s) )					// Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw16s, Fw32s, C3>::el_fn( (const Fw16s*)(ps+srcstep+C3*sizeof(Fw16s)), srcstep, (Fw16s*)pd );
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 t0, t1, m0, m1, b0, b1, next1, next2, temp;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    t0.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack16STo32S( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
    
                    // Add vertical columns
                    next1.s32[0] = 0;
                    next1.s32[1] = (Fw32s)*(Fw16s*)(ps+0*srcstep+8*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+8*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+8*sizeof(Fw16s));
                    next1.s32[2] = (Fw32s)*(Fw16s*)(ps+0*srcstep+9*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+9*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+9*sizeof(Fw16s));
                    next1.s32[3] = (Fw32s)*(Fw16s*)(ps+0*srcstep+10*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+10*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+10*sizeof(Fw16s));
                    next2.s32[0] = 0;
                    next2.s32[1] = (Fw32s)*(Fw16s*)(ps+0*srcstep+11*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+11*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+11*sizeof(Fw16s));
                    next2.s32[2] = (Fw32s)*(Fw16s*)(ps+0*srcstep+12*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+12*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+12*sizeof(Fw16s));
                    next2.s32[3] = (Fw32s)*(Fw16s*)(ps+0*srcstep+13*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+13*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+13*sizeof(Fw16s));
                    t0.i = _mm_add_epi32( t0.i, _mm_add_epi32(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;	
                    t1.i = _mm_add_epi32( t1.i, _mm_add_epi32(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;	
    
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep + C3*sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_slli_epi32( m0.i, 4 );
                    b1.i = _mm_slli_epi32( m1.i, 4 );
                    b0.i = _mm_add_epi32( b0.i, m0.i);
                    b1.i = _mm_add_epi32( b1.i, m1.i);
    
                    // subtract column N
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 1
                    temp.i = _mm_slli_si128( t1.i, 4 );
                    t0.i = _mm_srli_si128( t0.i, 12 );
                    t0.i = _mm_or_si128( t0.i, temp.i );
                    t1.i = _mm_srli_si128( t1.i, 12 );
                    t1.i = _mm_or_si128( t1.i, next1.i );
    
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 2
                    temp.i = _mm_slli_si128( t1.i, 4 );
                    t0.i = _mm_srli_si128( t0.i, 12 );
                    t0.i = _mm_or_si128( t0.i, temp.i );
                    t1.i = _mm_srli_si128( t1.i, 12 );
                    t1.i = _mm_or_si128( t1.i, next2.i );
    
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // {b0, b1} = {b0, b1} / 8
                    b0.i = _mm_srai_epi32( b0.i, 3 );
                    b1.i = _mm_srai_epi32( b1.i, 3 );
    
                    // Saturate and write the calculated values.
                    b0.i = _mm_packs_epi32( b0.i, b1.i );
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw16s), ps += sizeof(Fw16s) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw16s, Fw32s, C3>::el_fn( (const Fw16s *)(ps+srcstep+C3*sizeof(Fw16s)), srcstep, (Fw16s*)pd );
            }
        }
    
        // SSE2 optimized implementation for 16S 3 channel, unaligned data
        static void SSE2_Unaligned( void * pData )
        {
            Data< Fw16s > & data = *(Data< Fw16s >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C3 * sizeof(Fw16s);	// Now pSrc points at the first B in each line	
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw16s) * C3;
                U32 midwidth   = width & (~0x0f);		// Aligned bytes in the middle that can be processed 16 wide.
    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;    // End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;       // Enf of all the data.	
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 t0, t1, m0, m1, b0, b1, next1, next2, temp;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    t0.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack16STo32S( t0.i, t1.i );	// Top    line of the kernel
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
    
                    // Add vertical columns
                    next1.s32[0] = 0;
                    next1.s32[1] = (Fw32s)*(Fw16s*)(ps+0*srcstep+8*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+8*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+8*sizeof(Fw16s));
                    next1.s32[2] = (Fw32s)*(Fw16s*)(ps+0*srcstep+9*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+9*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+9*sizeof(Fw16s));
                    next1.s32[3] = (Fw32s)*(Fw16s*)(ps+0*srcstep+10*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+10*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+10*sizeof(Fw16s));
                    next2.s32[0] = 0;
                    next2.s32[1] = (Fw32s)*(Fw16s*)(ps+0*srcstep+11*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+11*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+11*sizeof(Fw16s));
                    next2.s32[2] = (Fw32s)*(Fw16s*)(ps+0*srcstep+12*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+12*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+12*sizeof(Fw16s));
                    next2.s32[3] = (Fw32s)*(Fw16s*)(ps+0*srcstep+13*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+1*srcstep+13*sizeof(Fw16s)) + (Fw32s)*(Fw16s*)(ps+2*srcstep+13*sizeof(Fw16s));
                    t0.i = _mm_add_epi32( t0.i, _mm_add_epi32(m0.i,b0.i) );	 // t0 = t0 + m0 + b0;	
                    t1.i = _mm_add_epi32( t1.i, _mm_add_epi32(m1.i,b1.i) );	 // t1 = t1 + m1 + b1;	
    
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep + C3*sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
    
                    // {b0, b1} = {m0, m1} * 17
                    b0.i = _mm_slli_epi32( m0.i, 4 );
                    b1.i = _mm_slli_epi32( m1.i, 4 );
                    b0.i = _mm_add_epi32( b0.i, m0.i);
                    b1.i = _mm_add_epi32( b1.i, m1.i);
    
                    // subtract column N
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 1
                    temp.i = _mm_slli_si128( t1.i, 4 );
                    t0.i = _mm_srli_si128( t0.i, 12 );
                    t0.i = _mm_or_si128( t0.i, temp.i );
                    t1.i = _mm_srli_si128( t1.i, 12 );
                    t1.i = _mm_or_si128( t1.i, next1.i );
    
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 2
                    temp.i = _mm_slli_si128( t1.i, 4 );
                    t0.i = _mm_srli_si128( t0.i, 12 );
                    t0.i = _mm_or_si128( t0.i, temp.i );
                    t1.i = _mm_srli_si128( t1.i, 12 );
                    t1.i = _mm_or_si128( t1.i, next2.i );
    
                    b0.i = _mm_sub_epi32( b0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // {b0, b1} = {b0, b1} / 8
                    b0.i = _mm_srai_epi32( b0.i, 3 );
                    b1.i = _mm_srai_epi32( b1.i, 3 );
    
                    // Saturate and write the calculated values.
                    b0.i = _mm_packs_epi32( b0.i, b1.i );
                    _mm_storeu_si128( (__m128i*)pd, b0.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw16s), ps += sizeof(Fw16s) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw16s, Fw32s, C3>::el_fn( (const Fw16s*)(ps+srcstep+C3*sizeof(Fw16s)), srcstep, (Fw16s*)pd );
            }
        }
    };

    template<>
    struct PERF_FN< Fw16s, C4 >
    {
        // SSE2 optimized implementation for 16S 4 channel, aligned data
        static void SSE2( void * pData )
        {
            Data< Fw16s > & data = *(Data< Fw16s >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C4 * sizeof(Fw16s);	// Now pSrc points at the first B in each line	
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw16s) * C4;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;			// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                for( ; pd<pEndStart; pd += sizeof(Fw16s), ps += sizeof(Fw16s) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw16s, Fw32s, C4>::el_fn( (const Fw16s*)(ps+srcstep+C4*sizeof(Fw16s)), srcstep, (Fw16s*)pd );				
    
                XMM128 t0, t1, m0, m1, b0, b1, next0_t, next1_t;
    
                // load top row
                t0.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack16STo32S( t0.i, t1.i );	// Top    line of the kernel
                // load bottom row and add to top row
                b0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                t0.i = _mm_add_epi32( t0.i, b0.i );
                t1.i = _mm_add_epi32( t1.i, b1.i );
                // load middle row and add to top row
                m0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
                t0.i = _mm_add_epi32( t0.i, m0.i );
                t1.i = _mm_add_epi32( t1.i, m1.i );
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    // load top row
                    next0_t.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep + 8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( next0_t.i, next1_t.i );
                    // load bottom row and add to top row
                    b0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep + 8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    next0_t.i = _mm_add_epi32( next0_t.i, b0.i );
                    next1_t.i = _mm_add_epi32( next1_t.i, b1.i );
                    // load middle row and add to top row
                    b0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep + 8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    next0_t.i = _mm_add_epi32( next0_t.i, b0.i );
                    next1_t.i = _mm_add_epi32( next1_t.i, b1.i );
    
                    // at this point m1 and b0 contain the middle values of interest
                    // and we will use the m0 and b1 as temporary variables for the final result
                    // {m0, b1} = {m1, b0} * 17
                    m0.i = _mm_slli_epi32( m1.i, 4 );
                    m0.i = _mm_add_epi32( m0.i, m1.i);
                    // save the value of next middle element
                    m1.i = b1.i;
                    b1.i = _mm_slli_epi32( b0.i, 4 );
                    b1.i = _mm_add_epi32( b1.i, b0.i);
    
                    // subtract column N
                    m0.i = _mm_sub_epi32( m0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 1
                    m0.i = _mm_sub_epi32( m0.i, t1.i );
                    b1.i = _mm_sub_epi32( b1.i, next0_t.i );
    
                    // subtract column N + 2
                    m0.i = _mm_sub_epi32( m0.i, next0_t.i );
                    b1.i = _mm_sub_epi32( b1.i, next1_t.i );
    
                    // {b0, b1} = {b0, b1} / 8
                    m0.i = _mm_srai_epi32( m0.i, 3 );
                    b1.i = _mm_srai_epi32( b1.i, 3 );
    
                    // Saturate and write the calculated values.
                    m0.i = _mm_packs_epi32( m0.i, b1.i );
                    _mm_storeu_si128( (__m128i*)pd, m0.i );
    
                    t0.i = next0_t.i;
                    t1.i = next1_t.i;
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw16s), ps += sizeof(Fw16s) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw16s, Fw32s, C4>::el_fn( (const Fw16s*)(ps+srcstep+C4*sizeof(Fw16s)), srcstep, (Fw16s*)pd );
            }
        }
    
        // SSE2 optimized implementation for 16S 4 channel, unaligned data
        static void SSE2_Unaligned( void * pData )
        {
            Data< Fw16s > & data = *(Data< Fw16s >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C4 * sizeof(Fw16s);	// Now pSrc points at the first B in each line	
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw16s) * C4;
                U32 midwidth   = width & (~0xF);		// bytes that can be processed 16 wide.
    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;    // End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;       // Enf of all the data.	
    
                XMM128 t0, t1, m0, m1, b0, b1, next0_t, next1_t;
    
                // load top row
                t0.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack16STo32S( t0.i, t1.i );	// Top    line of the kernel
                // load bottom row and add to top row
                b0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                t0.i = _mm_add_epi32( t0.i, b0.i );
                t1.i = _mm_add_epi32( t1.i, b1.i );
                // load middle row and add to top row
                m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
                t0.i = _mm_add_epi32( t0.i, m0.i );
                t1.i = _mm_add_epi32( t1.i, m1.i );
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // load top row
                    next0_t.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep+8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( next0_t.i, next1_t.i );
                    // load bottom row and add to top row
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep+8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    next0_t.i = _mm_add_epi32( next0_t.i, b0.i );
                    next1_t.i = _mm_add_epi32( next1_t.i, b1.i );
                    // load middle row and add to top row
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep+8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    next0_t.i = _mm_add_epi32( next0_t.i, b0.i );
                    next1_t.i = _mm_add_epi32( next1_t.i, b1.i );
    
                    // at this point m1 and b0 contain the middle values of interest
                    // and we will use the m0 and b1 as temporary variables for the final result
                    // {m0, b1} = {m1, b0} * 17
                    m0.i = _mm_slli_epi32( m1.i, 4 );
                    m0.i = _mm_add_epi32( m0.i, m1.i);
                    // save the value of the middle element
                    m1.i = b1.i;
                    b1.i = _mm_slli_epi32( b0.i, 4 );
                    b1.i = _mm_add_epi32( b1.i, b0.i);
    
                    // subtract column N
                    m0.i = _mm_sub_epi32( m0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 1
                    m0.i = _mm_sub_epi32( m0.i, t1.i );
                    b1.i = _mm_sub_epi32( b1.i, next0_t.i );
    
                    // subtract column N + 2
                    m0.i = _mm_sub_epi32( m0.i, next0_t.i );
                    b1.i = _mm_sub_epi32( b1.i, next1_t.i );
    
                    // {b0, b1} = {b0, b1} / 8
                    m0.i = _mm_srai_epi32( m0.i, 3 );
                    b1.i = _mm_srai_epi32( b1.i, 3 );
    
                    // Saturate and write the calculated values.
                    m0.i = _mm_packs_epi32( m0.i, b1.i );
                    _mm_storeu_si128( (__m128i*)pd, m0.i );
    
                    t0.i = next0_t.i;
                    t1.i = next1_t.i;
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw16s), ps += sizeof(Fw16s) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw16s, Fw32s, C4>::el_fn( (const Fw16s*)(ps+srcstep+C4*sizeof(Fw16s)), srcstep, (Fw16s*)pd );
            }
        }
    };

    template<>
    struct PERF_FN< Fw16s, AC4 >
    {
        // SSE2 optimized implementation for 16S 4 channel with Alpha, aligned data
        static void SSE2( void * pData )
        {
            Data< Fw16s > & data = *(Data< Fw16s >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C4 * sizeof(Fw16s);	// Now pSrc points at the first B in each line	
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw16s) * C4;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;			// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                int offset = 1;
                for( ; pd<pEndStart; pd += sizeof(Fw16s), ps += sizeof(Fw16s), ++offset )   // Process 1 at a time until we hit an alignment boundary
                {
                    if( offset & 0x3 )
                    {
                        REF_FN<Fw16s, Fw32s, C4>::el_fn( (const Fw16s*)(ps+srcstep+C4*sizeof(Fw16s)), srcstep, (Fw16s*)pd );				
                    }
                }
    
// For Solaris Alignment Issue, it's Removed from here and put it into Constants.cpp
/*
				SYS_FORCEALIGN_16 static const unsigned int alphaMasks_16[4][4] = {
                    { 0x0000FFFF, 0x00000000, 0x0000FFFF, 0x00000000 },
                    { 0x00000000, 0xFFFF0000, 0x00000000, 0xFFFF0000 },
                    { 0x00000000, 0x0000FFFF, 0x00000000, 0x0000FFFF },
                    { 0xFFFF0000, 0x00000000, 0xFFFF0000, 0x00000000 },
                };
*/
                XMM128 alphaMask;
                alphaMask.i = _mm_load_si128( (__m128i*)(alphaMasks_16[offset & 0x3]) );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 t0, t1, m0, m1, b0, b1, next0_t, next1_t;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // load top row
                    t0.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack16STo32S( t0.i, t1.i );	// Top    line of the kernel
                    next0_t.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep+8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( next0_t.i, next1_t.i );
    
                    // load bottom row and add to top row
                    b0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    m0.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep+8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Bottom line of the kernel
                    t0.i = _mm_add_epi32( t0.i, b0.i );
                    t1.i = _mm_add_epi32( t1.i, b1.i );
                    next0_t.i = _mm_add_epi32( next0_t.i, m0.i );
                    next1_t.i = _mm_add_epi32( next1_t.i, m1.i );
    
                    // load middle row and add to top row
                    m0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep+8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    t0.i = _mm_add_epi32( t0.i, m0.i );
                    t1.i = _mm_add_epi32( t1.i, m1.i );
                    next0_t.i = _mm_add_epi32( next0_t.i, b0.i );
                    next1_t.i = _mm_add_epi32( next1_t.i, b1.i );
    
                    // at this point m1 and b0 contain the middle values of interest
                    // and we will use the m0 and b1 as temporary variables for the final result
                    // {m0, b1} = {m1, b0} * 17
                    m0.i = _mm_slli_epi32( m1.i, 4 );
                    b1.i = _mm_slli_epi32( b0.i, 4 );
                    m0.i = _mm_add_epi32( m0.i, m1.i);
                    b1.i = _mm_add_epi32( b1.i, b0.i);
    
                    // subtract column N
                    m0.i = _mm_sub_epi32( m0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 1
                    m0.i = _mm_sub_epi32( m0.i, t1.i );
                    b1.i = _mm_sub_epi32( b1.i, next0_t.i );
    
                    // subtract column N + 2
                    m0.i = _mm_sub_epi32( m0.i, next0_t.i );
                    b1.i = _mm_sub_epi32( b1.i, next1_t.i );
    
                    // {m0, b1} = {m0, b1} / 8
                    m0.i = _mm_srai_epi32( m0.i, 3 );
                    b1.i = _mm_srai_epi32( b1.i, 3 );
    
                    // Saturate and pack the calculated values.
                    m0.i = _mm_packs_epi32( m0.i, b1.i );

                    // load original destination value
                    b1.i = _mm_loadu_si128( (__m128i*)pd );

                    // mask the result with original
                    m0.i = _mm_andnot_si128( alphaMask.i, m0.i );
                    b1.i = _mm_and_si128( b1.i, alphaMask.i );
                    m0.i = _mm_or_si128( m0.i, b1.i );
    
                    // Write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, m0.i );
                }
    
                for( ; pd<pEnd; pd += sizeof(Fw16s), ps += sizeof(Fw16s), ++offset )   // Process 1 at a time until we hit an alignment boundary
                {
                    if( offset & 0x3 )
                    {
                        REF_FN<Fw16s, Fw32s, C4>::el_fn( (const Fw16s*)(ps+srcstep+C4*sizeof(Fw16s)), srcstep, (Fw16s*)pd );
                    }
                }
            }
        }
    
        // SSE2 optimized implementation for 16S 4 channel with Alpha, unaligned data
        static void SSE2_Unaligned( void * pData )
        {
            Data< Fw16s > & data = *(Data< Fw16s >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C4 * sizeof(Fw16s);	// Now pSrc points at the first B in each line	
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw16s) * C4;
                U32 midwidth   = width & (~0x0F);		// bytes that can be processed 16 wide.
    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;    // End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;       // Enf of all the data.	
    
                XMM128 alphaMask;
                alphaMask.i = _mm_set_epi32( 0xFFFF0000, 0x0, 0xFFFF0000, 0x0 );
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 t0, t1, m0, m1, b0, b1, next0_t, next1_t;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // load top row
                    t0.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );   CBL_SSE2::Unpack16STo32S( t0.i, t1.i );	// Top    line of the kernel
                    next0_t.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep+8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( next0_t.i, next1_t.i );
    
                    // load bottom row and add to top row
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep+8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Bottom line of the kernel
                    t0.i = _mm_add_epi32( t0.i, b0.i );
                    t1.i = _mm_add_epi32( t1.i, b1.i );
                    next0_t.i = _mm_add_epi32( next0_t.i, m0.i );
                    next1_t.i = _mm_add_epi32( next1_t.i, m1.i );
    
                    // load middle row and add to top row
                    m0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );   CBL_SSE2::Unpack16STo32S( m0.i, m1.i );	// Middle line of the kernel
                    b0.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep+8 * sizeof(Fw16s)) );   CBL_SSE2::Unpack16STo32S( b0.i, b1.i );	// Bottom line of the kernel
                    t0.i = _mm_add_epi32( t0.i, m0.i );
                    t1.i = _mm_add_epi32( t1.i, m1.i );
                    next0_t.i = _mm_add_epi32( next0_t.i, b0.i );
                    next1_t.i = _mm_add_epi32( next1_t.i, b1.i );
    
                    // at this point m1 and b0 contain the middle values of interest
                    // and we will use the m0 and b1 as temporary variables for the final result
                    // {m0, b1} = {m1, b0} * 17
                    m0.i = _mm_slli_epi32( m1.i, 4 );
                    b1.i = _mm_slli_epi32( b0.i, 4 );
                    m0.i = _mm_add_epi32( m0.i, m1.i);
                    b1.i = _mm_add_epi32( b1.i, b0.i);
    
                    // subtract column N
                    m0.i = _mm_sub_epi32( m0.i, t0.i );
                    b1.i = _mm_sub_epi32( b1.i, t1.i );
    
                    // subtract column N + 1
                    m0.i = _mm_sub_epi32( m0.i, t1.i );
                    b1.i = _mm_sub_epi32( b1.i, next0_t.i );
    
                    // subtract column N + 2
                    m0.i = _mm_sub_epi32( m0.i, next0_t.i );
                    b1.i = _mm_sub_epi32( b1.i, next1_t.i );
    
                    // {m0, b1} = {m0, b1} / 8
                    m0.i = _mm_srai_epi32( m0.i, 3 );
                    b1.i = _mm_srai_epi32( b1.i, 3 );
    
                    // Saturate and pack the calculated values.
                    m0.i = _mm_packs_epi32( m0.i, b1.i );

                    // load original destination value
                    b1.i = _mm_loadu_si128( (__m128i*)pd );

                    // mask the result with original
                    m0.i = _mm_andnot_si128( alphaMask.i, m0.i );
                    b1.i = _mm_and_si128( b1.i, alphaMask.i );
                    m0.i = _mm_or_si128( m0.i, b1.i );
    
                    // Write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, m0.i );
                }
    
                int offset = 1;
                for( ; pd<pEnd; pd += sizeof(Fw16s), ps += sizeof(Fw16s), ++offset )   // Process 1 at a time until we hit an alignment boundary
                {
                    if( offset & 0x3 )
                    {
                        REF_FN<Fw16s, Fw32s, C4>::el_fn( (const Fw16s*)(ps+srcstep+C4*sizeof(Fw16s)), srcstep, (Fw16s*)pd );
                    }
                }
            }
        }
    };

    template<>
    struct PERF_FN< Fw32f, C1 >
    {
        // SSE2 optimized implementation for 32F 1 channel, aligned data
        static void SSE2( void * pData )
        {	
            Data< Fw32f > & data = *(Data< Fw32f >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C1 * sizeof(Fw32f);	// Now pSrc points at the first B in each line	
    
            XMM128 c_17, c_8;
            c_17.f = _mm_set1_ps( 17.0 );
            c_8.f = _mm_set1_ps( 8.0 );
            XMM128 next_t, next_m, next_b;
    
            // the following 3 lines are really not necessary, but we keep them to prevent 
            // the compiler from complaining about uninitialized variables
            next_t.i = _mm_setzero_si128();
            next_b.i = _mm_setzero_si128();
            next_m.i = _mm_setzero_si128();
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw32f);
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;			// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                for( ; pd<pEndStart; pd += sizeof(Fw32f), ps += sizeof(Fw32f) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw32f, Fw32f, C1>::el_fn( (const Fw32f*)(ps+srcstep+C1*sizeof(Fw32f)), srcstep, (Fw32f*)pd );				
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 top, mid, btm;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    top.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );    // Top    line of the kernel
                    mid.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );    // Middle line of the kernel
                    btm.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );    // Bottom line of the kernel
                    next_t.d = _mm_loadl_pd( next_t.d, (double*)(ps + 0*srcstep + 4 * sizeof(Fw32s)) );
                    next_m.d = _mm_loadl_pd( next_m.d, (double*)(ps + 1*srcstep + 4 * sizeof(Fw32s)) );
                    next_b.d = _mm_loadl_pd( next_b.d, (double*)(ps + 2*srcstep + 4 * sizeof(Fw32s)) );
    
                    // Add vertical columns
                    next_t.f = _mm_add_ps( next_t.f, _mm_add_ps(next_m.f,next_b.f) );
                    top.f = _mm_add_ps( top.f, _mm_add_ps(mid.f,btm.f) );	 // top = top + mid + btm;	
    
                    // shift all the values in mid to the left
                    mid.f = _mm_move_ss( mid.f, next_m.f );
                    mid.i = _mm_shuffle_epi32( mid.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    
                    // btm = mid * 17
                    btm.f = _mm_mul_ps( mid.f, c_17.f);
    
                    // subtract column N
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // subtract column N + 1
                    top.f = _mm_move_ss( top.f, next_t.f );
                    top.i = _mm_shuffle_epi32( top.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    next_t.i = _mm_srli_si128( next_t.i, 4 );
    
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // subtract column N + 2
                    top.f = _mm_move_ss( top.f, next_t.f );
                    top.i = _mm_shuffle_epi32( top.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // btm = btm / 8
                    btm.f = _mm_div_ps( btm.f, c_8.f );
    
                    // write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, btm.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw32f), ps += sizeof(Fw32f) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw32f, Fw32f, C1>::el_fn( (const Fw32f*)(ps+srcstep+C1*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
            }
        }
    
        // SSE2 optimized implementation for 32F 1 channel, unaligned data
        static void SSE2_Unaligned( void * pData )
        {	
            Data< Fw32f > & data = *(Data< Fw32f >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C1 * sizeof(Fw32f);	// Now pSrc points at the first B in each line	
    
            XMM128 c_17, c_8;
            c_17.f = _mm_set1_ps( 17.0 );
            c_8.f = _mm_set1_ps( 8.0 );
            XMM128 next_t, next_m, next_b;
    
            // the following 3 lines are really not necessary, but we keep them to prevent 
            // the compiler from complaining about uninitialized variables
            next_t.i = _mm_setzero_si128();
            next_b.i = _mm_setzero_si128();
            next_m.i = _mm_setzero_si128();
    
            Fw8u * pEnd = (pDst + dstRoiSize.height * dstStep);
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw32f);
                U32 midwidth   = width & 0xFFFFFFF0;			// Bytes at end that can't be processed 16 wide.
    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;    // End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;       // Enf of all the data.	
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 top, mid, btm;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    top.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );    // Top    line of the kernel
                    mid.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );    // Middle line of the kernel
                    btm.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );    // Bottom line of the kernel
                    next_t.d = _mm_loadl_pd( next_t.d, (double*)(ps + 0*srcstep + 4 * sizeof(Fw32s)) );
                    next_m.d = _mm_loadl_pd( next_m.d, (double*)(ps + 1*srcstep + 4 * sizeof(Fw32s)) );
                    next_b.d = _mm_loadl_pd( next_b.d, (double*)(ps + 2*srcstep + 4 * sizeof(Fw32s)) );
    
                    // Add vertical columns
                    next_t.f = _mm_add_ps( next_t.f, _mm_add_ps(next_m.f,next_b.f) );
                    top.f = _mm_add_ps( top.f, _mm_add_ps(mid.f,btm.f) );	 // top = top + mid + btm;	
    
                    // shift all the values in mid to the left
                    mid.f = _mm_move_ss( mid.f, next_m.f );
                    mid.i = _mm_shuffle_epi32( mid.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    
                    // btm = mid * 17
                    btm.f = _mm_mul_ps( mid.f, c_17.f);
    
                    // subtract column N
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // subtract column N + 1
                    top.f = _mm_move_ss( top.f, next_t.f );
                    top.i = _mm_shuffle_epi32( top.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    next_t.i = _mm_srli_si128( next_t.i, 4 );
    
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // subtract column N + 2
                    top.f = _mm_move_ss( top.f, next_t.f );
                    top.i = _mm_shuffle_epi32( top.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // btm = btm / 8
                    btm.f = _mm_div_ps( btm.f, c_8.f );
    
                    // write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, btm.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw32f), ps += sizeof(Fw32f) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw32f, Fw32f, C1>::el_fn( (const Fw32f*)(ps+srcstep+C1*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
            }
        }
    };

    template<>
    struct PERF_FN< Fw32f, C3 >
    {
        // SSE2 optimized implementation for 32F 3 channel, aligned data
        static void SSE2( void * pData )
        {
            Data< Fw32f > & data = *(Data< Fw32f >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C3 * sizeof(Fw32f);	// Now pSrc points at the first B in each line	
    
            XMM128 c_17, c_8;
            c_17.f = _mm_set1_ps( 17.0 );
            c_8.f = _mm_set1_ps( 8.0 );
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw32f) * C3;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;			// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                for( ; pd<pEndStart; pd += sizeof(Fw32f), ps += sizeof(Fw32f) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw32f, Fw32f, C3>::el_fn( (const Fw32f*)(ps+srcstep+C3*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 top, mid, btm;
                    XMM128 next1_t, next1_m, next1_b;
                    XMM128 next2_t, next2_m, next2_b;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    top.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );    // Top    line of the kernel
                    next1_t.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep + 4 * sizeof(Fw32s)) );
                    next2_t.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep + 6 * sizeof(Fw32s)) );
    
                    mid.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );    // Middle line of the kernel
                    next1_m.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep + 4 * sizeof(Fw32s)) );
                    next2_m.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep + 6 * sizeof(Fw32s)) );
    
                    btm.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );    // Bottom line of the kernel
                    next1_b.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep + 4 * sizeof(Fw32s)) );
                    next2_b.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep + 6 * sizeof(Fw32s)) );
    
                    // Add vertical columns
                    next1_t.f = _mm_add_ps( next1_t.f, _mm_add_ps(next1_m.f,next1_b.f) );
                    next2_t.f = _mm_add_ps( next2_t.f, _mm_add_ps(next2_m.f,next2_b.f) );
                    top.f = _mm_add_ps( top.f, _mm_add_ps(mid.f,btm.f) );	 // top = top + mid + btm;
    
                    // shift all the values in mid to the left, shifting in next_m
                    mid.i = _mm_srli_si128( mid.i, 12 );
                    next1_m.i = _mm_slli_si128( next1_m.i, 4 );
                    mid.i = _mm_or_si128( mid.i, next1_m.i );
    
                    // btm = mid * 17
                    btm.f = _mm_mul_ps( mid.f, c_17.f);
    
                    // subtract column N
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // subtract column N + 1
                    top.i = _mm_srli_si128( top.i, 12 );
                    next1_t.i = _mm_slli_si128( next1_t.i, 4 );
                    top.i = _mm_or_si128( top.i, next1_t.i );
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // subtract column N + 2
                    top.i = _mm_srli_si128( top.i, 12 );
                    next2_t.f = _mm_move_ss( next2_t.f, top.f );
                    top.i = next2_t.i;
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // btm = btm / 8
                    btm.f = _mm_div_ps( btm.f, c_8.f );
    
                    // write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, btm.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw32f), ps += sizeof(Fw32f) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw32f, Fw32f, C3>::el_fn( (const Fw32f*)(ps+srcstep+C3*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
            }
        }
    
        // SSE2 optimized implementation for 32F 3 channel, unaligned data
        static void SSE2_Unaligned( void * pData )
        {	
            Data< Fw32f > & data = *(Data< Fw32f >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C3 * sizeof(Fw32f);	// Now pSrc points at the first B in each line	
    
            XMM128 c_17, c_8;
            c_17.f = _mm_set1_ps( 17.0 );
            c_8.f = _mm_set1_ps( 8.0 );
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw32f) * C3;
                U32 midwidth   = width & (~0xF);        // bytes that can be processed 16 wide.
    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;    // End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;       // Enf of all the data.	
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 top, mid, btm;
                    XMM128 next1_t, next1_m, next1_b;
                    XMM128 next2_t, next2_m, next2_b;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    top.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );    // Top    line of the kernel
                    next1_t.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep + 4 * sizeof(Fw32s)) );
                    next2_t.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep + 6 * sizeof(Fw32s)) );
    
                    mid.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );    // Middle line of the kernel
                    next1_m.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep + 4 * sizeof(Fw32s)) );
                    next2_m.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep + 6 * sizeof(Fw32s)) );
    
                    btm.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );    // Bottom line of the kernel
                    next1_b.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep + 4 * sizeof(Fw32s)) );
                    next2_b.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep + 6 * sizeof(Fw32s)) );
    
                    // Add vertical columns
                    next1_t.f = _mm_add_ps( next1_t.f, _mm_add_ps(next1_m.f,next1_b.f) );
                    next2_t.f = _mm_add_ps( next2_t.f, _mm_add_ps(next2_m.f,next2_b.f) );
                    top.f = _mm_add_ps( top.f, _mm_add_ps(mid.f,btm.f) );	 // top = top + mid + btm;
    
                    // shift all the values in mid to the left, shifting in next_m
                    mid.i = _mm_srli_si128( mid.i, 12 );
                    next1_m.i = _mm_slli_si128( next1_m.i, 4 );
                    mid.i = _mm_or_si128( mid.i, next1_m.i );
    
                    // btm = mid * 17
                    btm.f = _mm_mul_ps( mid.f, c_17.f);
    
                    // subtract column N
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // subtract column N + 1
                    top.i = _mm_srli_si128( top.i, 12 );
                    next1_t.i = _mm_slli_si128( next1_t.i, 4 );
                    top.i = _mm_or_si128( top.i, next1_t.i );
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // subtract column N + 2
                    top.i = _mm_srli_si128( top.i, 12 );
                    next2_t.f = _mm_move_ss( next2_t.f, top.f );
                    top.i = next2_t.i;
                    btm.f = _mm_sub_ps( btm.f, top.f );
    
                    // btm = btm / 8
                    btm.f = _mm_div_ps( btm.f, c_8.f );
    
                    // write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, btm.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw32f), ps += sizeof(Fw32f) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw32f, Fw32f, C3>::el_fn( (const Fw32f*)(ps+srcstep+C3*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
            }
        }
    };

    template<>
    struct PERF_FN< Fw32f, C4 >
    {
        // SSE2 optimized implementation for 32F 4 channel, aligned data
        static void SSE2( void * pData )
        {	
            Data< Fw32f > & data = *(Data< Fw32f >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C4 * sizeof(Fw32f);	// Now pSrc points at the first B in each line	
    
            XMM128 c_17, c_8;
            c_17.f = _mm_set1_ps( 17.0 );
            c_8.f = _mm_set1_ps( 8.0 );
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw32f) * C4;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;			// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                for( ; pd<pEndStart; pd += sizeof(Fw32f), ps += sizeof(Fw32f) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw32f, Fw32f, C4>::el_fn( (const Fw32f*)(ps+srcstep+C4*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 top, mid, btm;
                    XMM128 next1_t, next1_m, next1_b;
                    XMM128 next2_t, next2_m, next2_b;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    top.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );    // Top    line of the kernel
                    next1_t.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep + 4 * sizeof(Fw32s)) );
                    next2_t.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep + 8 * sizeof(Fw32s)) );
    
                    mid.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );    // Middle line of the kernel
                    next1_m.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep + 4 * sizeof(Fw32s)) );
                    next2_m.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep + 8 * sizeof(Fw32s)) );
    
                    btm.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );    // Bottom line of the kernel
                    next1_b.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep + 4 * sizeof(Fw32s)) );
                    next2_b.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep + 8 * sizeof(Fw32s)) );
    
                    // Add vertical columns
                    next1_t.f = _mm_add_ps( next1_t.f, _mm_add_ps(next1_m.f,next1_b.f) );
                    next2_t.f = _mm_add_ps( next2_t.f, _mm_add_ps(next2_m.f,next2_b.f) );
                    top.f = _mm_add_ps( top.f, _mm_add_ps(mid.f,btm.f) );	 // top = top + mid + btm;
    
                    // next1_m = next1_m * 17
                    next1_m.f = _mm_mul_ps( next1_m.f, c_17.f);
    
                    // subtract column N
                    next1_m.f = _mm_sub_ps( next1_m.f, top.f );
    
                    // subtract column N + 1
                    next1_m.f = _mm_sub_ps( next1_m.f, next1_t.f );
    
                    // subtract column N + 2
                    next1_m.f = _mm_sub_ps( next1_m.f, next2_t.f );
    
                    // btm = btm / 8
                    next1_m.f = _mm_div_ps( next1_m.f, c_8.f );
    
                    // write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, next1_m.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw32f), ps += sizeof(Fw32f) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw32f, Fw32f, C4>::el_fn( (const Fw32f*)(ps+srcstep+C4*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
            }
        }
    
        // SSE2 optimized implementation for 32F 4 channel, unaligned data
        static void SSE2_Unaligned( void * pData )
        {	
            Data< Fw32f > & data = *(Data< Fw32f >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C4 * sizeof(Fw32f);	// Now pSrc points at the first B in each line	
    
            XMM128 c_17, c_8;
            c_17.f = _mm_set1_ps( 17.0 );
            c_8.f = _mm_set1_ps( 8.0 );
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw32f) * C4;
                U32 midwidth   = width & (~0x0F);		// Aligned bytes that can be processed 16 wide.
    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;    // End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;       // Enf of all the data.	
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 top, mid, btm;
                    XMM128 next1_t, next1_m, next1_b;
                    XMM128 next2_t, next2_m, next2_b;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    top.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );    // Top    line of the kernel
                    next1_t.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep + 4 * sizeof(Fw32s)) );
                    next2_t.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep + 8 * sizeof(Fw32s)) );
    
                    mid.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );    // Middle line of the kernel
                    next1_m.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep + 4 * sizeof(Fw32s)) );
                    next2_m.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep + 8 * sizeof(Fw32s)) );
    
                    btm.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );    // Bottom line of the kernel
                    next1_b.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep + 4 * sizeof(Fw32s)) );
                    next2_b.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep + 8 * sizeof(Fw32s)) );
    
                    // Add vertical columns
                    next1_t.f = _mm_add_ps( next1_t.f, _mm_add_ps(next1_m.f,next1_b.f) );
                    next2_t.f = _mm_add_ps( next2_t.f, _mm_add_ps(next2_m.f,next2_b.f) );
                    top.f = _mm_add_ps( top.f, _mm_add_ps(mid.f,btm.f) );	 // top = top + mid + btm;
    
                    // next1_m = next1_m * 17
                    next1_m.f = _mm_mul_ps( next1_m.f, c_17.f);
    
                    // subtract column N
                    next1_m.f = _mm_sub_ps( next1_m.f, top.f );
    
                    // subtract column N + 1
                    next1_m.f = _mm_sub_ps( next1_m.f, next1_t.f );
    
                    // subtract column N + 2
                    next1_m.f = _mm_sub_ps( next1_m.f, next2_t.f );
    
                    // btm = btm / 8
                    next1_m.f = _mm_div_ps( next1_m.f, c_8.f );
    
                    // write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, next1_m.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw32f), ps += sizeof(Fw32f) )   // Process 1 at a time until we hit an alignment boundary
                    REF_FN<Fw32f, Fw32f, C4>::el_fn( (const Fw32f*)(ps+srcstep+C4*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
            }
        }
    };

    template<>
    struct PERF_FN< Fw32f, AC4 >
    {
        // SSE2 optimized implementation for 32F 4 channel with Alpha, aligned data
        static void SSE2( void * pData )
        {	
            Data< Fw32f > & data = *(Data< Fw32f >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C4 * sizeof(Fw32f);	// Now pSrc points at the first B in each line	
    
            XMM128 c_17, c_8;
            c_17.f = _mm_set1_ps( 17.0 );
            c_8.f = _mm_set1_ps( 8.0 );
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw32f) * C4;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;			// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                int offset = 1;
                for( ; pd<pEndStart; pd += sizeof(Fw32f), ps += sizeof(Fw32f), ++offset )   // Process 1 at a time until we hit an alignment boundary
                {
                    if( offset & 0x3 )
                    {
                        REF_FN<Fw32f, Fw32f, C4>::el_fn( (const Fw32f*)(((const Fw8u*)ps)+srcstep+C4*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
                    }
                }
    
// For Solaris Alignment Issue, it's Removed from here and put it into Constants.cpp
/*
				SYS_FORCEALIGN_16 static const unsigned int alphaMasks_32[4][4] = {
                    { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 },
                    { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF },
                    { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 },
                    { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 }
				};
*/  
                XMM128 alphaMask;
                alphaMask.i = _mm_load_si128( (__m128i*)(alphaMasks_32[offset & 0x3]) );
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 top, mid, btm;
                    XMM128 next1_t, next1_m, next1_b;
                    XMM128 next2_t, next2_m, next2_b;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    top.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep) );    // Top    line of the kernel
                    next1_t.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep + 4 * sizeof(Fw32s)) );
                    next2_t.i = _mm_load_si128( (__m128i*)(ps + 0*srcstep + 8 * sizeof(Fw32s)) );
    
                    mid.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep) );    // Middle line of the kernel
                    next1_m.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep + 4 * sizeof(Fw32s)) );
                    next2_m.i = _mm_load_si128( (__m128i*)(ps + 1*srcstep + 8 * sizeof(Fw32s)) );
    
                    btm.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep) );    // Bottom line of the kernel
                    next1_b.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep + 4 * sizeof(Fw32s)) );
                    next2_b.i = _mm_load_si128( (__m128i*)(ps + 2*srcstep + 8 * sizeof(Fw32s)) );
    
                    // Add vertical columns
                    next1_t.f = _mm_add_ps( next1_t.f, _mm_add_ps(next1_m.f,next1_b.f) );
                    next2_t.f = _mm_add_ps( next2_t.f, _mm_add_ps(next2_m.f,next2_b.f) );
                    top.f = _mm_add_ps( top.f, _mm_add_ps(mid.f,btm.f) );	 // top = top + mid + btm;
    
                    // save the original source data
                    mid.i = next1_m.i;
    
                    // next1_m = next1_m * 17
                    next1_m.f = _mm_mul_ps( next1_m.f, c_17.f);
    
                    // subtract column N
                    next1_m.f = _mm_sub_ps( next1_m.f, top.f );
    
                    // subtract column N + 1
                    next1_m.f = _mm_sub_ps( next1_m.f, next1_t.f );
    
                    // subtract column N + 2
                    next1_m.f = _mm_sub_ps( next1_m.f, next2_t.f );
    
                    // btm = btm / 8
                    next1_m.f = _mm_div_ps( next1_m.f, c_8.f );
    
                    // read original destionation values.
                    mid.i = _mm_loadu_si128( (__m128i*)pd );

                    // mask the alpha channel
                    mid.i = _mm_and_si128( mid.i, alphaMask.i );
                    next1_m.i = _mm_andnot_si128( alphaMask.i, next1_m.i );
                    next1_m.i = _mm_or_si128( next1_m.i, mid.i );
    
                    // write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, next1_m.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw32f), ps += sizeof(Fw32f), ++offset )   // Process 1 at a time until we hit an alignment boundary
                {
                    if( offset & 0x3 )
                    {
                        REF_FN<Fw32f, Fw32f, C4>::el_fn( (const Fw32f*)(ps+srcstep+C4*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
                    }
                }
    
            }
        }
    
        // SSE2 optimized implementation for 32F 4 channel with Alpha, unaligned data
        static void SSE2_Unaligned( void * pData )
        {
            Data< Fw32f > & data = *(Data< Fw32f >*)pData;
            const Fw8u * pSrc = (const Fw8u *)data.pSrc;
            int      srcstep    = data.srcstep;
            Fw8u * pDst	    = (Fw8u *)data.pDst;	
            int      dstStep    = data.dstStep;
            FwiSize dstRoiSize = data.dstRoiSize;
    
            // I assume that the src is aligned at the border.
            //				 the dst is aligned at the data.
    
            // I also assume that srcstep is a multiple of 16 bytes
    
            //	B B B 				//pSrc points at first S (src data)
            //  B S S    D D 		//pDst points at first D (dst data)
            //	B S S    D D 											
            pSrc -= srcstep + C4 * sizeof(Fw32f);	// Now pSrc points at the first B in each line	
    
            XMM128 c_17, c_8;
            c_17.f = _mm_set1_ps( 17.0 );
            c_8.f = _mm_set1_ps( 8.0 );
    
            Fw8u * pEnd = pDst + dstRoiSize.height * dstStep;
            for( ; pDst<pEnd; pDst += dstStep, pSrc += srcstep )			// Process vertical lines
            {
                const Fw8u *ps = pSrc;									
                    Fw8u *pd = pDst;	
    
                U32 width      = dstRoiSize.width * sizeof(Fw32f) * C4;
                U32 midwidth   = width & (~0x0f); // bytes that can be processed 16 wide.
    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;    // End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;       // Enf of all the data.	
    
                int offset = 1;
                XMM128 alphaMask;
                alphaMask.i = _mm_set_epi32( 0xFFFFFFFF, 0, 0, 0 );
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (8 words wide)
                {	
                    //	0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1		//src
                    //	  0 1 2 3 4 5 6 7 8 9 A B C D E   F			//dst
    
                    XMM128 top, mid, btm;
                    XMM128 next1_t, next1_m, next1_b;
                    XMM128 next2_t, next2_m, next2_b;
    
                    // we use the following formula:
                    // dst = ( 16 * src - sum( 9x9 matrix of pixels centered at src ) ) / 8
    
                    // Load the data
                    top.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep) );    // Top    line of the kernel
                    next1_t.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep + 4 * sizeof(Fw32s)) );
                    next2_t.i = _mm_loadu_si128( (__m128i*)(ps + 0*srcstep + 8 * sizeof(Fw32s)) );
    
                    mid.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep) );    // Middle line of the kernel
                    next1_m.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep + 4 * sizeof(Fw32s)) );
                    next2_m.i = _mm_loadu_si128( (__m128i*)(ps + 1*srcstep + 8 * sizeof(Fw32s)) );
    
                    btm.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep) );    // Bottom line of the kernel
                    next1_b.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep + 4 * sizeof(Fw32s)) );
                    next2_b.i = _mm_loadu_si128( (__m128i*)(ps + 2*srcstep + 8 * sizeof(Fw32s)) );
    
                    // Add vertical columns
                    next1_t.f = _mm_add_ps( next1_t.f, _mm_add_ps(next1_m.f,next1_b.f) );
                    next2_t.f = _mm_add_ps( next2_t.f, _mm_add_ps(next2_m.f,next2_b.f) );
                    top.f = _mm_add_ps( top.f, _mm_add_ps(mid.f,btm.f) );	 // top = top + mid + btm;
    
                    // save the original source data
                    mid.i = next1_m.i;
    
                    // next1_m = next1_m * 17
                    next1_m.f = _mm_mul_ps( next1_m.f, c_17.f);
    
                    // subtract column N
                    next1_m.f = _mm_sub_ps( next1_m.f, top.f );
    
                    // subtract column N + 1
                    next1_m.f = _mm_sub_ps( next1_m.f, next1_t.f );
    
                    // subtract column N + 2
                    next1_m.f = _mm_sub_ps( next1_m.f, next2_t.f );
    
                    // btm = btm / 8
                    next1_m.f = _mm_div_ps( next1_m.f, c_8.f );
    
                    // read original destionation values.
                    mid.i = _mm_loadu_si128( (__m128i*)pd );

                    // mask the alpha channel
                    mid.i = _mm_and_si128( mid.i, alphaMask.i );
                    next1_m.i = _mm_andnot_si128( alphaMask.i, next1_m.i );
                    next1_m.i = _mm_or_si128( next1_m.i, mid.i );
    
                    // write the calculated values.
                    _mm_storeu_si128( (__m128i*)pd, next1_m.i );
                }				
    
                for( ; pd<pEnd; pd += sizeof(Fw32f), ps += sizeof(Fw32f), ++offset )   // Process 1 at a time until we hit an alignment boundary
                {
                    if( offset & 0x3 )
                    {
                        REF_FN<Fw32f, Fw32f, C4>::el_fn((const Fw32f*)(ps+srcstep+C4*sizeof(Fw32f)), srcstep, (Fw32f*)pd );
                    }
                }
            }
        }
    };
};	// namespace SharpenFilter
}

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_8u_C1R)( const Fw8u * pSrc, int srcstep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<C1,Fw8u,Fw16s>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_16s_C1R)( const Fw16s * pSrc, int srcstep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<C1,Fw16s,Fw32s>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_32f_C1R)( const Fw32f * pSrc, int srcstep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<C1,Fw32f,Fw32f>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_8u_C3R)( const Fw8u * pSrc, int srcstep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
  	return SharpenFilter::iFilterSharpen<C3,Fw8u,Fw16s>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_16s_C3R)( const Fw16s * pSrc, int srcstep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<C3,Fw16s,Fw32s>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_32f_C3R)( const Fw32f * pSrc, int srcstep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<C3,Fw32f,Fw32f>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_8u_C4R)( const Fw8u * pSrc, int srcstep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<C4,Fw8u,Fw16s>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_16s_C4R)( const Fw16s * pSrc, int srcstep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<C4,Fw16s,Fw32s>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_32f_C4R)( const Fw32f * pSrc, int srcstep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<C4,Fw32f,Fw32f>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_8u_AC4R)( const Fw8u * pSrc, int srcstep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<AC4,Fw8u,Fw16s>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_16s_AC4R)( const Fw16s * pSrc, int srcstep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<AC4,Fw16s,Fw32s>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_32f_AC4R)( const Fw32f * pSrc, int srcstep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return SharpenFilter::iFilterSharpen<AC4,Fw32f,Fw32f>( pSrc, srcstep, pDst, dstStep, dstRoiSize );
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
