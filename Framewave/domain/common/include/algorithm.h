/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "fwdev.h"
#include "FwSharedCode_SSE2.h"
#include "fnTypes.h"

#define EL_FN_HEAD template< class TS, class TD >    SYS_INLINE

#define EL_FN  void (*el_fn )(    TD*, const     TS*, const     TS*, const     U8*, void *, void* )
#define EL_FN2 void (*el_fn2)(XMM128*, const XMM128*, const XMM128*, const XMM128*, void *, void* )

namespace OPT_LEVEL
{

SYS_INLINE void NoSSE2( XMM128 *, const XMM128 *, const XMM128 *, const XMM128 *, void *, void * )
{
    //ASSERT( false );
}

//TODO: ADD DIVBYZERO ERROR FOR DIVISION (THINK WHETHER TO ADD NE WCASE FOR DIVISION
// chids Did this But in some cases like fwiDiv_xyz the execution is continued after the error is returned but in Divc, execution is stopped
// I think we dont have to bother, the calling program will take care. But if we return errorcode, we dont give chance to continue.
//TODO: Implement other flavors, of ForEach_SSE2.


// ===================================================================================
//    BEGIN: External Entry points for Algorithm.h
//    Use only these functions to call into Algorithm.h
// ===================================================================================

#define DISPATCH_FOREACHX( FN,SEL,CS,CD,EL,EL2) FwStatus FN( ARGS_##SEL ){ return Apply_##SEL<TS,TD,CS,CD,EL,EL2>( PARAM_##SEL ); }
#define DISPATCH_FOREACH(  FN,SEL,CH,   EL    ) DISPATCH_FOREACHX( FN, SEL, CH, CH, EL, NoSSE2 )

// ===================================================================================
//        END: External Entry points for Algorithm.h
// ===================================================================================


// ===================================================================================
//        Macros
// ===================================================================================

#define FUNCTION( NAME, ALG, CS, CD, DEV, SSE2 ) \
    FwStatus NAME( ARGS_##ALG ){ return Apply_##ALG<TS,TD,CS,CD,DEV,SSE2>( PARAM_##ALG ); }

#define TEMPLATE_ARGS_ND template< class TS, class TD, CH chSrc, CH chDst,                    EL_FN, EL_FN2 >
#define TEMPLATE_ARGS    template< class TS, class TD, CH chSrc, CH chDst, DispatchType disp, EL_FN, EL_FN2 >

#define DEF_FOREACH( SEL ) TEMPLATE_ARGS SYS_INLINE FwStatus forEach_##SEL( ARGS_##SEL )

#define DEF_BASE( SEL )      TEMPLATE_ARGS    struct base_##SEL
#define DEF_BASE_SSE2( SEL ) TEMPLATE_ARGS_ND struct base_##SEL<TS, TD, chSrc, chDst, DT_SSE2, el_fn, el_fn2 >

#define CALL_BASE(      SEL ) base_##SEL   < TS, TD, chSrc, chDst, disp,    el_fn, el_fn2 >::forEach
#define CALL_BASE_REFR( SEL ) base_##SEL   < TS, TD, chSrc, chDst, DT_REFR, el_fn, el_fn2 >::forEach

#define CALL_FOREACH( SEL ) forEach_##SEL< TS, TD, chSrc, chDst, disp, el_fn, el_fn2 >


// ===================================================================================
//                forEach()
// ===================================================================================
//
//    Step through each el in a region of interest. Call 'el_fn' for each element. 
//
//    Functions come in 2 basic flavors:
//
//    1. BASE() functions provide the base 1 dimensional operation. This is the function that should be 
//       optimized. 
//
//    2. FOREACH() functions provide minimum translation necessary to redirect to BASE functions. 
//       For example, ROI (2D) functions, step vertically through the buffer, calling
//       the 1D base function for the horizontal data. 
//
//       FOREACH() functions come in 3 types:
//       a. base redirection. Pass through to BASE() functions
//       b. non scaled variants. Call the scaled functions with 0 as the scale factor.
//       c. scaled ROI. Step vertically through the data, call base functions for the horizontal. 
//
//       len            - number of elements in the vector.
//       ROI            - size in pixels.
//       src/dstStep    - step in bytes through the image.
//
//       NOTE: All sizes passed are calculated on the dst image.
//
//       NAMING KEY:
//        
//       C - Takes constant value
//       K - Takes multichannel constant value
//       M - Takes mask parameter
//       R - Takes ROI parameter
//       S - Takes a scale parameter.
//       A - Takes an alpha value.


//  ROI                    - number of pixels
//  srcStep/ dstStep    - number of bytes


// ============================================
//    11    - 1 buffer, 1 source
// ============================================

DEF_BASE( 11S )
{
    static SYS_INLINE FwStatus forEach( ARGS_11S )
    {
        el_fn2;
        TD *end = pSrcDst + len;

        for( ;pSrcDst<end; pSrcDst+=ChannelCount(chDst) )                                // Modify the scanline
        {
            switch( chDst )
            {
            
            case C1:    el_fn( (pSrcDst+0), 0, 0, 0,&scaleFactor,NULL );
            case C4:    el_fn( (pSrcDst+3), 0, 0, 0,&scaleFactor,NULL );
            case C3:
            case AC4:
                        el_fn( (pSrcDst+2), 0, 0, 0,&scaleFactor,NULL );
                        el_fn( (pSrcDst+1), 0, 0, 0,&scaleFactor,NULL );
                        el_fn( (pSrcDst+0), 0, 0, 0,&scaleFactor,NULL );
            }
        }
  
        return fwStsNoErr;
    }
};


DEF_BASE( 11CS )
{
    static SYS_INLINE FwStatus forEach( ARGS_11CS )
    {
        el_fn2;
        TD *end = pSrcDst + len;

        for( ;pSrcDst<end; pSrcDst+=ChannelCount(chDst) )                               // Modify the scanline
        {
            switch( chDst )
            {
            
            case C4:    el_fn( (pSrcDst+3), 0, &value, 0,&scaleFactor,0 );              // All fall through!!!
            case C3:     
            case AC4:    
                        el_fn( (pSrcDst+2), 0, &value, 0,&scaleFactor,0 );
                        el_fn( (pSrcDst+1), 0, &value, 0,&scaleFactor,0 );
                        el_fn( (pSrcDst+0), 0, &value, 0,&scaleFactor,0 );
            }
        }
  
        return fwStsNoErr;
    }
};

DEF_BASE( 11KS )
{    
    static SYS_INLINE FwStatus forEach( ARGS_11KS)
    {
        STATIC_ASSERT( (chSrc==chDst) );
        STATIC_ASSERT( (chSrc == C3)||(chSrc==AC4)||(chSrc==C4) );
        el_fn2;
        TD *end = pSrcDst + len;

        for( ;pSrcDst<end; pSrcDst+=ChannelCount(chDst) )                               // Modify the scanline
        {
            switch( chDst )
            {
            
            case C4:    el_fn( (pSrcDst+3), (pSrcDst+3), &value[3], 0,&scaleFactor,0 ); // All fall through!!!
            case C3:     
            case AC4:    
                        el_fn( (pSrcDst+2), (pSrcDst+2), &value[2], 0,&scaleFactor,0 );
                        el_fn( (pSrcDst+1), (pSrcDst+1), &value[1], 0,&scaleFactor,0 );
                        el_fn( (pSrcDst+0), (pSrcDst+0), &value[0], 0,&scaleFactor,0 );
            }
        }

        return fwStsNoErr;
    }
};

DEF_BASE_SSE2( 11CS )
{
    static SYS_INLINE FwStatus forEach( ARGS_11CS )
    {
        U32 width = 16 / sizeof(TS);        // Number of length units to consume in each 128 bit execution.

        int lenRef = len % width;           // Count of units to process in reference
        int lenSSE = len - lenRef;          // Count of units to process in SSE.
        TD *endSSE = pSrcDst + lenSSE;      // One past last unit in SSE.

        XMM128 sd, v;

        SSE2::Load1( v, value );

          for( ;pSrcDst!=endSSE; pSrcDst+=width ){
                sd.i  = _mm_loadu_si128( (__m128i*)pSrcDst );        // Load the source into XMM Register
                el_fn2( &sd, &sd, &v,0,&scaleFactor,0);
                _mm_storeu_si128( (__m128i*)pSrcDst, sd.i );
            }
    
        return CALL_BASE_REFR(11CS)( value, pSrcDst, lenRef, scaleFactor );
    }
};

DEF_BASE_SSE2( 11KS )
{
    static SYS_INLINE FwStatus forEach( ARGS_11KS )
    {
        U32 width = 16 / sizeof(TS);        // Number of length units to consume in each 128 bit execution.

        int lenRef = len % width;           // Count of units to process in reference
        int lenSSE = len - lenRef;          // Count of units to process in SSE.
        TD *endSSE = pSrcDst + lenSSE;      // One past last unit in SSE.
        int channelIndex = 0;
        XMM128 sd, v;

        for( ;pSrcDst!=endSSE; pSrcDst+=width ){
                sd.i  = _mm_loadu_si128( (__m128i*)pSrcDst );        // Load the source into XMM Register
                SSE2::Load2( v, value, chSrc,channelIndex);
                channelIndex++;
                el_fn2( &sd, &sd, &v,0,&scaleFactor,0);
                _mm_storeu_si128( (__m128i*)pSrcDst, sd.i );
            }
    
        return CALL_BASE_REFR(11KS)( value, pSrcDst,lenRef, scaleFactor );
     }
};


DEF_FOREACH( 11S   ){ return CALL_BASE(    11S  )( PARAM_11S    ); }
DEF_FOREACH( 11CS  ){ return CALL_BASE(    11CS )( PARAM_11CS   ); }
DEF_FOREACH( 11KS  ){ return CALL_BASE(    11KS )( PARAM_11KS   ); }
DEF_FOREACH( 11C   ){ return CALL_FOREACH( 11CS )( PARAM_11C, 0 ); }

DEF_FOREACH( 11CRS )
{
    TD *end = DEV::Offset( pSrcDst, roiSize.height * srcDstStep );

    for( ;pSrcDst!=end; DEV::Inc( pSrcDst, srcDstStep) )
        CALL_FOREACH( 11CS )( value, pSrcDst, roiSize.width*ChannelCount(chDst), scaleFactor) ;

    return fwStsNoErr;
}

DEF_FOREACH( 11RS )
{
    TD *end = DEV::Offset( pSrcDst, roiSize.height * srcDstStep );

    for( ;pSrcDst<end; DEV::Inc( pSrcDst, srcDstStep ) )
        CALL_FOREACH( 11S )( pSrcDst, roiSize.width*ChannelCount(chDst), scaleFactor );
    return fwStsNoErr;
}

DEF_FOREACH( 11KRS )
{    
    TD *end = DEV::Offset( pSrcDst, roiSize.height * srcDstStep );

    for( ;pSrcDst<end; DEV::Inc( pSrcDst, srcDstStep ) )
        CALL_FOREACH( 11KS )( value, pSrcDst, roiSize.width*ChannelCount(chDst), scaleFactor );

    return fwStsNoErr;
}

DEF_FOREACH( 11CR ){ return CALL_FOREACH( 11CRS )( PARAM_11CR,  0 ); } // Must be after 11CRS  (GCC requirement)
DEF_FOREACH( 11KR ){ return CALL_FOREACH( 11KRS )( PARAM_11KR,  0 ); } // Must be after 11KRS (GCC requirement)
DEF_FOREACH( 11R  ){ return CALL_FOREACH( 11RS  )( PARAM_11R,   0 ); } 


//============================================
//    21    - 2 buffers, 1 source
//============================================

// For Color Model Conversion
DEF_BASE( 21SP )
{
    static SYS_INLINE FwStatus forEach( ARGS_21SP )            // Can Use ARGS_21SP
    {
        el_fn2;
        STATIC_ASSERT((chSrc == C3 && chDst == AC4) || 
                      (chSrc == C1 && chDst == C1 ) ||
                      (chSrc == C3 && chDst == C3 ) ||
                      (chSrc == C4 && chDst == C4 ) || 
                      (chSrc == AC4 && chDst == AC4)  );        //TODO: Support other channel sizes 
        TD *end = pDst + len;        

        for( ;pDst!=end; pDst+=ChannelCount(chDst), pSrc+=ChannelCount(chSrc) )
        {
            switch( MERGE(chSrc,chDst) )
            {         
                case MERGE( C3, AC4 ):   // !!! What's THIS merge?
                      el_fn((pDst+0), (pSrc+0), 0, 0, &scaleFactor,0 );
                      el_fn((pDst+1), (pSrc+1), 0, 0, &scaleFactor,0 );
                      el_fn((pDst+2), (pSrc+2), 0, 0, &scaleFactor,0 );
                      *(pDst+3) = 0;
                break;
                case MERGE( C1, C1 ):
                      el_fn( (pDst+0), (pSrc+0), 0, 0,&scaleFactor,0 );
                    break;

                case MERGE( C4, C4):           // All fall through!!!
                case MERGE( C3, C3):
                    el_fn( (pDst+0), (pSrc+0), 0, 0,&scaleFactor,0 );
                    break;
                case MERGE( AC4,AC4):
                    el_fn( (pDst+0), (pSrc+0), 0, 0,&scaleFactor,0 );
                    *(pDst+3) = (TD)*(pSrc+3); // Copy Alpha Channel
                    break;
            }
        }
        return fwStsNoErr; 
    }    
};

DEF_BASE_SSE2( 21SP )
{
    static SYS_INLINE FwStatus forEach( ARGS_21SP )        //SSE2
    {            
        if( chSrc==C3 && chDst == AC4 )
        {
            if( IS_ALIGNED16_2( (size_t)pSrc, (size_t)pDst ) )    
            {
                int lenOdd = len % 64;
                int len64  = len - lenOdd;
                TD *end64  = pDst + len64;

                XMM128 a, b, c, d, tmp;

                // 3 byte wide input, 4 byte wide output.
                for( ;pDst<end64; pDst+=64, pSrc+=48 )                  // Read 48 bytes, write 64
                {    
                    a.i   = _mm_load_si128( (__m128i*)(pSrc +  0) );    // a   = (15-0 )
                    b.i   = _mm_load_si128( (__m128i*)(pSrc + 16) );    // b   = (31-16)
                    c.i   = _mm_load_si128( (__m128i*)(pSrc + 32) );    // c   = (47-32)

                    el_fn2( &d, &a, &a, 0,&scaleFactor,0 );             // d   = (11-0)

                    a.i   = _mm_srli_si128( a.i, 12  );                 // a   = 0000 0000 0000 (15-12)
                    tmp.i = _mm_slli_si128( b.i, 4   );                 // tmp = (27-16) 0000 // Save b
                    a.i   = _mm_or_si128  ( a.i, tmp.i);                // a   = (27-12)
                    el_fn2( &a, &a, &a, 0, &scaleFactor, 0  );          // a   = (23-12)

                    b.i   = _mm_srli_si128( b.i, 8  );                  // b   = 0000 0000 (31-24)
                    tmp.i = _mm_slli_si128( c.i, 8  );                  // tmp = (39-32) 0000 0000 // Save c
                    b.i   = _mm_or_si128( b.i, tmp.i );                 // b   = (39-24)
                    el_fn2(&b, &b, &b, 0, &scaleFactor, 0  );           // b   = (24-35)
                                        
                    c.i   = _mm_srli_si128( c.i, 4  );                  // c   = 0000 (47-36)
                    el_fn2(&c, &c, &c, 0, &scaleFactor, 0  );           // c   = (47-36)

                    _mm_store_si128( (__m128i*)(pDst+ 0), d.i );
                    _mm_store_si128( (__m128i*)(pDst+16), a.i );
                    _mm_store_si128( (__m128i*)(pDst+32), b.i );
                    _mm_store_si128( (__m128i*)(pDst+48), c.i );
                }

                return CALL_BASE_REFR( 21SP )( pSrc, pDst, lenOdd, scaleFactor );
            }
            else
            {
                int lenOdd = len % 16;
                int len16  = len - lenOdd;
                TD *end16  = pDst + len16;

                for( ;pDst<end16; pDst+=16, pSrc+=12 )        // Read 12 bytes, write 16
                {
                    XMM128 tmp;
                    tmp.i = _mm_loadu_si128( (__m128i*)pSrc );
                    el_fn2( &tmp, &tmp, &tmp, 0,&scaleFactor,0  );
                    _mm_storeu_si128( (__m128i*)pDst, tmp.i );
                }

                return CALL_BASE_REFR( 21SP )( pSrc, pDst, lenOdd, scaleFactor );
            }
        }
            else
                return CALL_BASE_REFR( 21SP )( PARAM_21S );
    }
};


DEF_BASE( 21S )
{
    static SYS_INLINE FwStatus forEach( ARGS_21S )           // Default
    {
        el_fn2;
        STATIC_ASSERT((chSrc == C3 && chDst == AC4) || 
                      (chSrc == C1 && chDst == C1 ) ||
                      (chSrc == C3 && chDst == C3 ) ||
                      (chSrc == C4 && chDst == C4 ) || 
                      (chSrc == AC4 && chDst == AC4)  );      //TODO: Support other channel sizes
        TD *end = pDst + len;

        for( ;pDst!=end; pDst+=ChannelCount(chDst), pSrc+=ChannelCount(chSrc) )
        {
            switch( MERGE(chSrc,chDst) )
            {         
                case MERGE( C3, AC4 ):
                      el_fn((pDst+0), (pSrc+0), 0, 0, &scaleFactor,0 );
                      el_fn((pDst+1), (pSrc+1), 0, 0, &scaleFactor,0 );
                      el_fn((pDst+2), (pSrc+2), 0, 0, &scaleFactor,0 );
                      *(pDst+3) = 0;
                break;
                case MERGE( C1, C1 ):   el_fn( (pDst+0), (pSrc+0), 0, 0,&scaleFactor,0 );
                    break;

                case MERGE( C4, C4):    el_fn( (pDst+3), (pSrc+3), 0, 0,&scaleFactor,0 );        // All fall through!!!
                case MERGE( C3, C3):
                case MERGE( AC4,AC4):
                                        el_fn( (pDst+2), (pSrc+2), 0, 0,&scaleFactor,0 );
                                        el_fn( (pDst+1), (pSrc+1), 0, 0,&scaleFactor,0 );
                                        el_fn( (pDst+0), (pSrc+0), 0, 0,&scaleFactor,0 );

            }
        }
        return fwStsNoErr; 
    }    
};

DEF_BASE_SSE2( 21S )
{
    static SYS_INLINE FwStatus forEach( ARGS_21S )        //SSE2
    {            
        if( chSrc==C3 && chDst == AC4 )
        {                
            if( IS_ALIGNED16_2( (size_t)pSrc, (size_t)pDst ) )
            {
                int lenOdd = len % 64;
                int len64  = len - lenOdd;        
                TD *end64   = pDst + len64;    

                XMM128 a, b, c, d, tmp;

                // 3 byte wide input, 4 byte wide output.
                for( ;pDst<end64; pDst+=64, pSrc+=48 )                  // Read 48 bytes, write 64
                {    
                    a.i   = _mm_load_si128( (__m128i*)(pSrc +  0) );    // a    = (15-0 )
                    b.i   = _mm_load_si128( (__m128i*)(pSrc + 16) );    // b    = (31-16)
                    c.i   = _mm_load_si128( (__m128i*)(pSrc + 32) );    // c    = (47-32)

                    el_fn2( &d, &a, &a, 0,&scaleFactor,0 );             // d    = (11-0)

                    a.i   = _mm_srli_si128( a.i, 12  );                 // a    = 0000 0000 0000 (15-12)
                    tmp.i = _mm_slli_si128( b.i, 4   );                 // tmp  = (27-16) 0000 // Save b
                    a.i   = _mm_or_si128  ( a.i, tmp.i);                // a    = (27-12)
                    el_fn2( &a, &a, &a, 0, &scaleFactor, 0  );          // a    = (23-12)

                    b.i   = _mm_srli_si128( b.i, 8  );                  // b    = 0000 0000 (31-24)
                    tmp.i = _mm_slli_si128( c.i, 8  );                  // tmp  = (39-32) 0000 0000 // Save c
                    b.i   = _mm_or_si128( b.i, tmp.i );                 // b    = (39-24)
                    el_fn2(&b, &b, &b, 0, &scaleFactor, 0  );           // b    = (24-35)
                                        
                    c.i   = _mm_srli_si128( c.i, 4  );                  // c    = 0000 (47-36)
                    el_fn2(&c, &c, &c, 0, &scaleFactor, 0  );           // c    = (47-36)

                    _mm_store_si128( (__m128i*)(pDst+ 0), d.i );
                    _mm_store_si128( (__m128i*)(pDst+16), a.i );
                    _mm_store_si128( (__m128i*)(pDst+32), b.i );
                    _mm_store_si128( (__m128i*)(pDst+48), c .i);
                }

                return CALL_BASE_REFR( 21S )( pSrc, pDst, lenOdd, scaleFactor );
            }
        }
            else
            {
                int lenOdd = len % 16;
                int len16  = len - lenOdd;
                TD *end16  = pDst + len16;

                for( ;pDst<end16; pDst+=16, pSrc+=12 )        // Read 12 bytes, write 16
                {
                    XMM128 tmp;
                    tmp.i = _mm_loadu_si128( (__m128i*)pSrc );
                    el_fn2( &tmp, &tmp, &tmp, 0,&scaleFactor,0 );
                    _mm_storeu_si128( (__m128i*)pDst, tmp.i );
                }

                return CALL_BASE_REFR( 21S )( pSrc, pDst, lenOdd, scaleFactor );
            }
        
            return CALL_BASE_REFR( 21S )( PARAM_21S );
    }
};

DEF_BASE( 21CS )
{
    static SYS_INLINE FwStatus forEach( ARGS_21CS )
    {
        el_fn2;
        STATIC_ASSERT( chSrc == C1 && chDst == C1 );    //TODO: Support other channel sizes
            
        TD *end = pDst + len;    
        
        for( ;pDst<end; ++pDst, ++pSrc )
            el_fn( pDst, pSrc, &value, 0,&scaleFactor,0 );

        return fwStsNoErr;
    }

};



DEF_BASE_SSE2( 21CS )
{
    static SYS_INLINE FwStatus forEach( ARGS_21CS )
    {
        STATIC_ASSERT( sizeof(TS) == sizeof(TD) );

        U32 width = 16 / sizeof(TS);    // Number of length units to consume in each 128 bit execution.

        int lenRef = len % width;       // Count of units to process in reference
        int lenSSE = len - lenRef;      // Count of units to process in SSE.
        TD *endSSE = pDst + lenSSE;     // One past last unit in SSE.

        XMM128 d, s, v;

        SSE2::Load1( v, value );

        if( IS_ALIGNED16_2( (size_t)pDst, (size_t)pSrc ))
        {
            for( ;pDst!=endSSE; pSrc+=width, pDst+=width )
            {
                s.i  = _mm_load_si128( (__m128i*)pSrc );        // Load the source into XMM Register
                el_fn2( &d, &s, &v,0,&scaleFactor,0);
                _mm_store_si128( (__m128i*)pDst, d.i );
            }
        }
        else
        {
            for( ;pDst!=endSSE; pSrc+=width, pDst+=width )
            {
                s.i  = _mm_loadu_si128( (__m128i*)pSrc );       // Load the source into XMM Register
                el_fn2( &d, &s, &v,0,&scaleFactor,0);
                _mm_storeu_si128( (__m128i*)pDst, d.i );
            }
        }
        return CALL_BASE_REFR(21CS)( pSrc, value, pDst, lenRef, scaleFactor );
     }
};


DEF_BASE( 21KS )
{
    static SYS_INLINE FwStatus forEach( ARGS_21KS)
    {
        el_fn2;
        STATIC_ASSERT( (chSrc==chDst) );
        STATIC_ASSERT( (chSrc == C3)||(chSrc==AC4)||(chSrc==C4) );
        
        //TD *end = pDst + len;
        TD *end = DEV::Offset( pDst, len );
        
        for( ;pDst<end; pDst+=ChannelCount(chDst), pSrc+=ChannelCount(chSrc) )  // Modify the scanline
        {
            switch( chSrc )
            {
            case C4:    el_fn( (pDst+3), (pSrc+3), value+3, 0,&scaleFactor,0 );
            case C3:    
            case AC4:   el_fn( (pDst+2), (pSrc+2), value+2, 0,&scaleFactor,0 );
                        el_fn( (pDst+1), (pSrc+1), value+1, 0,&scaleFactor,0 );
                        el_fn( (pDst+0), (pSrc+0), value+0, 0,&scaleFactor,0 );
            }            
        }    
        return fwStsNoErr;
    }
};

DEF_BASE_SSE2( 21KS )
{
    static SYS_INLINE FwStatus forEach( ARGS_21KS )
    {
        STATIC_ASSERT( sizeof(TS) == sizeof(TD) );

        U32 width = 16 / sizeof(TS);    // Number of length units to consume in each 128 bit execution.

        int lenRef = len % width;       // Count of units to process in reference
        int lenSSE = len - lenRef;      // Count of units to process in SSE.
        TD *endSSE = pDst + lenSSE;     // One past last unit in SSE.
        int channelIndex = 0;
        XMM128 d, s, v;

        if( IS_ALIGNED16_2( (size_t)pDst, (size_t) pSrc ))
        {
            for( ;pDst!=endSSE; pSrc+=width, pDst+=width )
            {
                s.i  = _mm_load_si128( (__m128i*)pSrc );        // Load the source into XMM Register
                SSE2::Load2( v, value, chSrc,channelIndex);
                channelIndex++;
                el_fn2( &d, &s, &v,0,&scaleFactor,0);
                _mm_store_si128( (__m128i*)pDst, d.i );
            }
        }
        else
        {
            for( ;pDst!=endSSE; pSrc+=width, pDst+=width )
            {
                s.i  = _mm_loadu_si128( (__m128i*)pSrc );       // Load the source into XMM Register
                SSE2::Load2( v, value, chSrc,channelIndex);
                channelIndex++;
                el_fn2( &d, &s, &v,0,&scaleFactor,0);
                _mm_storeu_si128( (__m128i*)pDst, d.i );
            }
        }
        return CALL_BASE_REFR(21KS)( pSrc, value, pDst, lenRef, scaleFactor );
     }
};


// For Color Model Conversion
DEF_FOREACH( 21P  ){ return CALL_BASE   ( 21SP )( PARAM_21, 0  ); }
DEF_FOREACH( 21SP ){ return CALL_BASE   ( 21SP )( PARAM_21SP   ); }

DEF_FOREACH( 21S  ){ return CALL_BASE   ( 21S  )( PARAM_21S    ); }
DEF_FOREACH( 21CS ){ return CALL_BASE   ( 21CS )( PARAM_21CS   ); }
DEF_FOREACH( 21KS ){ return CALL_BASE   ( 21KS )( PARAM_21KS   ); }
DEF_FOREACH( 21   ){ return CALL_FOREACH( 21S  )( PARAM_21 , 0 ); }
DEF_FOREACH( 21C  ){ return CALL_FOREACH( 21CS )( PARAM_21C, 0 ); }

DEF_FOREACH( 21CRS )
{
    TD *end = DEV::Offset( pDst, roiSize.height * dstStep );
    int len = roiSize.width * ChannelCount(chDst);

    for( ;pDst<end; DEV::Inc( pDst, dstStep), DEV::Inc( pSrc, srcStep )  )      // For each scanline
        CALL_FOREACH( 21CS )( pSrc, value, pDst, len, scaleFactor);
    return fwStsNoErr;    
}

DEF_FOREACH( 21KRS )
{
    TD *end =DEV::Offset( pDst, roiSize.height * dstStep );
    int len = roiSize.width * ChannelCount(chDst);

    for( ;pDst<end; DEV::Inc( pDst, dstStep), DEV::Inc( pSrc, srcStep ) )       // For each scanline
        CALL_FOREACH( 21KS )( pSrc, value, pDst, len, scaleFactor); 
    return fwStsNoErr;
}

DEF_FOREACH( 21R )
{
    TD *end = DEV::Offset( pDst, roiSize.height * dstStep );
    int len = roiSize.width * ChannelCount(chDst);

    for( ;pDst!=end; DEV::Inc( pDst, dstStep), DEV::Inc( pSrc, srcStep ) )
        CALL_FOREACH( 21 )( pSrc, pDst, len );
    return fwStsNoErr;
}

DEF_FOREACH( 21MR )
{  
    TD *end = DEV::Offset( pDst, roiSize.height * dstStep );
    int len = roiSize.width * ChannelCount(chDst);

    for( ;pDst<end; DEV::Inc( pDst, dstStep ), DEV::Inc( pSrc, srcStep ) )      // For each scanline
        CALL_FOREACH( 21 )( pSrc, pDst,len ); 
    return fwStsNoErr;
}    


DEF_FOREACH( 21RS )
{
    TD *end = DEV::Offset( pDst, roiSize.height * dstStep );
    int len = roiSize.width * ChannelCount(chDst);

    for( ;pDst<end; DEV::Inc( pDst, dstStep ), DEV::Inc( pSrc, srcStep ) )      // For each scanline
        CALL_FOREACH( 21S )( pSrc, pDst,len,scaleFactor ); 
    return fwStsNoErr;
}

DEF_FOREACH( 21CR  ){ return CALL_FOREACH( 21CRS )( PARAM_21CR, 0 ); }
DEF_FOREACH( 21KR  ){ return CALL_FOREACH( 21KRS )( PARAM_21KR, 0 ); }

DEF_FOREACH( 21RP )        // For Color Model Conversions and 3-channel Color!
{
    TD *end = DEV::Offset( pDst, roiSize.height * dstStep );
    int len = roiSize.width * ChannelCount(chDst);

    for( ;pDst!=end; DEV::Inc( pDst, dstStep), DEV::Inc( pSrc, srcStep ) )
        CALL_FOREACH( 21P )( pSrc, pDst, len );
    return fwStsNoErr;
}

//============================================
//    22    - 2 buffers, 2 sources
//============================================
    
DEF_BASE( 22S )
{
    static SYS_INLINE FwStatus forEach( ARGS_22S)
    {
        el_fn2;
        STATIC_ASSERT( chSrc == chDst );

        //TD *end = pSrcDst + len;
        TD *end = DEV::Offset( pSrcDst,len );
            
        for( ;pSrcDst<end; pSrcDst+=ChannelCount(chSrc), pSrc+=ChannelCount(chSrc) )// Modify the scanline
        {
            switch( chSrc )
            {
            case C4:    el_fn( (pSrcDst+3), (pSrc+3), 0, 0,&scaleFactor,0 );        // All fall through!!!
            case C3:
            case AC4:   el_fn( (pSrcDst+2), (pSrc+2), 0, 0,&scaleFactor,0 );
                        el_fn( (pSrcDst+1), (pSrc+1), 0, 0,&scaleFactor,0 );
            case C1:    el_fn( (pSrcDst+0), (pSrc+0), 0, 0,&scaleFactor,0 );
            }
        }
        return fwStsNoErr;
    }
};

    
DEF_BASE_SSE2( 22S )
{
    static SYS_INLINE FwStatus forEach( ARGS_22S )
    {
        //STATIC_ASSERT( sizeof(TS) == sizeof(TD) );

        U32 width = 16 / sizeof(TS);    // Number of length units to consume in each 128 bit execution.

        int lenRef = len % width;       // Count of units to process in reference
        int lenSSE = len - lenRef;      // Count of units to process in SSE.
        TD *endSSE = pSrcDst + lenSSE;  // One past last unit in SSE.

        XMM128 d, s1, scl ;
        //SSE2::Load1(scl, scaleFactor );

        if( IS_ALIGNED16_2( (size_t)pSrcDst, (size_t)pSrc) )
        {
            for( ;pSrcDst!=endSSE; pSrc+=width, pSrcDst+=width )
            {
                s1.i  = _mm_load_si128( (__m128i*)pSrc );       // Load the source into XMM Register
                d.i  = _mm_load_si128( (__m128i*)pSrcDst );
                el_fn2( &d, &s1, 0, 0,&scl,0);
                _mm_store_si128( (__m128i*)pSrcDst, d.i );
            }
        }
        else{
            for( ;pSrcDst!=endSSE; pSrc+=width, pSrcDst+=width )
            {
                s1.i  = _mm_loadu_si128( (__m128i*)pSrc );      // Load the source into XMM Register
                d.i  = _mm_loadu_si128( (__m128i*)pSrcDst );
                el_fn2( &d, &s1, 0, 0,&scl,0);
                _mm_storeu_si128( (__m128i*)pSrcDst, d.i );
            }
        }
        return CALL_BASE_REFR(22S)( pSrc, pSrcDst, lenRef, scaleFactor );
    }  

}; // Def base

DEF_BASE( 22A )
{
    static SYS_INLINE FwStatus forEach( ARGS_22A)
    {
        el_fn2;
        STATIC_ASSERT( chSrc == chDst );
        STATIC_ASSERT( chSrc == C1 );
        //TD *end = pSrcDst + len;
        TD *end = DEV::Offset( pSrcDst,len );

        for( ;pSrcDst<end; pSrcDst++, pSrc++)                   // Modify the scanline
            el_fn( pSrcDst,pSrc, 0, 0, &alpha, 0 );

        return fwStsNoErr;
    }
};

DEF_FOREACH( 22S  ){ return CALL_BASE   ( 22S )( PARAM_22S   ); }
DEF_FOREACH( 22   ){ return CALL_FOREACH( 22S )( PARAM_22, 0 ); }
DEF_FOREACH( 22RS )
{
    TD *end = DEV::Offset( pSrcDst, roiSize.height * srcDstStep );

    for( ;pSrcDst<end; DEV::Inc( pSrcDst, srcDstStep ), DEV::Inc( pSrc, srcStep ) ) // For each scanline
        CALL_FOREACH( 22S )( pSrc, pSrcDst, roiSize.width * ChannelCount(chDst), scaleFactor );
    return fwStsNoErr;
}

        
DEF_FOREACH( 22R  ){ return CALL_FOREACH( 22RS )( PARAM_22R,0); }
DEF_FOREACH( 22A  ){ return CALL_BASE(    22A  )( PARAM_22A  ); }
DEF_FOREACH( 22RA )
{
    TD *end = DEV::Offset( pSrcDst, roiSize.height * srcDstStep );

    for( ;pSrcDst<end; DEV::Inc( pSrcDst, srcDstStep ), DEV::Inc( pSrc, srcStep ) ) // For each scanline
        CALL_FOREACH( 22A )( pSrc, pSrcDst, roiSize.width , alpha );
    return fwStsNoErr;
}


//============================================
//    32    - 3 buffers, 2 source
//============================================

DEF_BASE( 32S )
{
    static SYS_INLINE FwStatus forEach( ARGS_32S )
    {
        el_fn2;    
        STATIC_ASSERT( chSrc == chDst );
        TD *end = pDst + len;

        for( ;pDst<end; pDst+=ChannelCount(chDst), pSrc1+=ChannelCount(chDst), pSrc2+=ChannelCount(chDst) ) // Modify the scanline
        {
            switch( chDst )
            {
            case C4:     el_fn( (pDst+3), (pSrc1+3), (pSrc2+3),0,&scaleFactor,0 );  // All fall through!!!
            case C3:
            case AC4:    el_fn( (pDst+2), (pSrc1+2), (pSrc2+2),0,&scaleFactor,0 );
                         el_fn( (pDst+1), (pSrc1+1), (pSrc2+1),0,&scaleFactor,0 );
            case C1:     el_fn( (pDst+0), (pSrc1+0), (pSrc2+0),0,&scaleFactor,0 );
            }
        }    
        return fwStsNoErr;
    }

};
    
DEF_BASE_SSE2( 32S )
{
    static SYS_INLINE FwStatus forEach( ARGS_32S )
    {
        STATIC_ASSERT( sizeof(TS) == sizeof(TD) );

        U32 width = 16 / sizeof(TS);    // Number of length units to consume in each 128 bit execution.

        int lenRef = len % width;       // Count of units to process in reference
        int lenSSE = len - lenRef;      // Count of units to process in SSE.
        TD *endSSE = pDst + lenSSE;     // One past last unit in SSE.

        XMM128 d, s1, s2, scl;
//        SSE2::Load1(scl, scaleFactor );

        if( IS_ALIGNED16_3( (size_t)pDst, (size_t)pSrc1, (size_t)pSrc2 ) )
        {
            for( ;pDst!=endSSE; pSrc1+=width, pSrc2+=width, pDst+=width )
            {
                s1.i  = _mm_load_si128( (__m128i*)pSrc1 );        // Load the source into XMM Register
                s2.i  = _mm_load_si128( (__m128i*)pSrc2 );
                el_fn2( &d, &s1, &s2,0,&scl,0);
                _mm_store_si128( (__m128i*)pDst, d.i );
            }
        }
        else{
            for( ;pDst!=endSSE; pSrc1+=width, pSrc2+=width, pDst+=width )
            {
                s1.i  = _mm_loadu_si128( (__m128i*)pSrc1 );        // Load the source into XMM Register
                s2.i  = _mm_loadu_si128( (__m128i*)pSrc2 );
                el_fn2( &d, &s1, &s2,0,&scl,0);
                _mm_storeu_si128( (__m128i*)pDst, d.i );
            }
        }
        return CALL_BASE_REFR(32S)( pSrc1, pSrc2, pDst, lenRef, scaleFactor );
    }  

}; // Def base



DEF_FOREACH( 32S  ){ return CALL_BASE( 32S )( PARAM_32S    ); }
DEF_FOREACH( 32   ){ return CALL_BASE( 32S )( PARAM_32,  0 ); }
DEF_FOREACH( 32RS )
{ 
    TD *end = DEV::Offset( pDst, roiSize.height * dstStep );

    for( ;pDst<end; DEV::Inc(pDst,dstStep), DEV::Inc(pSrc1,src1Step), DEV::Inc(pSrc2,src2Step) )        // For each scanline
        CALL_FOREACH( 32S )( pSrc1, pSrc2, pDst, roiSize.width * ChannelCount(chDst), scaleFactor);
    return fwStsNoErr;
}

DEF_FOREACH( 32R  ){ return CALL_FOREACH( 32RS )( PARAM_32R, 0); }


//============================================
//    33    - 3 buffers, 3 source
//============================================

DEF_BASE(  33 )
{
    static SYS_INLINE FwStatus forEach( ARGS_33 )
    {
        el_fn2;
        STATIC_ASSERT( chSrc == chDst );
        STATIC_ASSERT( chSrc == C1    );
        TD *end = pSrcDst + len;

        for( ;pSrcDst<end; pSrcDst++, pSrc1++,pSrc2++)      // Modify the scanline
            el_fn( pSrcDst, pSrc1, pSrc2,0,0,0);            //*pMask
        return fwStsNoErr;
    }
};

DEF_BASE( 33M )
{
    static SYS_INLINE FwStatus forEach( ARGS_33M )
    {
        el_fn2;    
        STATIC_ASSERT( chSrc == chDst );
        STATIC_ASSERT( chSrc == C1    );
        TD *end = pSrcDst + len;
        
        for( ;pSrcDst<end; pSrcDst++, pSrc++, pMask++)      // Modify the scanline
        {
            el_fn( pSrcDst,pSrc,0,pMask,0,0); 
        }    
        return fwStsNoErr;
    }
};

DEF_BASE( 33MA )
{
    static SYS_INLINE FwStatus forEach( ARGS_33MA )
    {
        el_fn2;    
        STATIC_ASSERT( chSrc == chDst );
        STATIC_ASSERT( chSrc == C1    );
        TD *end = pSrcDst + len;
        
        for( ;pSrcDst<end; pSrcDst++, pSrc++, pMask++)      // Modify the scanline
            el_fn( pSrcDst,pSrc,0,pMask,&alpha,NULL); 

        return fwStsNoErr;
    }
};


DEF_FOREACH(  33  ){ return CALL_BASE( 33   )( PARAM_33   ); }
DEF_FOREACH(  33M ){ return CALL_BASE( 33M  )( PARAM_33M  ); }
DEF_FOREACH( 33MA ){ return CALL_BASE( 33MA )( PARAM_33MA ); }
DEF_FOREACH( 33R  )
{
    TD *end = DEV::Offset( pSrcDst, roiSize.height * srcDstStep );
    for( ;pSrcDst<end; DEV::Inc( pSrcDst, srcDstStep ), DEV::Inc( pSrc1, src1Step ), DEV::Inc( pSrc2, src2Step ) )  // For each scanline
        CALL_FOREACH( 33 )( pSrc1, pSrc2, pSrcDst, roiSize.width );
    return fwStsNoErr;
}

DEF_FOREACH( 33MR )
{
    TD *end = DEV::Offset( pSrcDst, roiSize.height * srcDstStep );
    for( ;pSrcDst<end; DEV::Inc( pSrcDst, srcDstStep ), DEV::Inc( pSrc, srcStep ), DEV::Inc( pMask, maskStep) )     // For each scanline
        CALL_FOREACH( 33M )( pSrc, pMask, pSrcDst, roiSize.width );
    return fwStsNoErr;
}

DEF_FOREACH( 33MRA )
{
    TD *end = DEV::Offset( pSrcDst, roiSize.height * srcDstStep );

    for( ;pSrcDst<end; DEV::Inc( pSrcDst, srcDstStep), DEV::Inc( pSrc, srcStep), DEV::Inc( pMask, maskStep) )       // For each scanline
        CALL_FOREACH( 33MA )( pSrc, pMask, pSrcDst, roiSize.width, alpha );
    return fwStsNoErr;
}



//============================================
//    44    - 4 buffers, 4 sources
//============================================

DEF_BASE( 44M )
{
    static SYS_INLINE FwStatus forEach( ARGS_44M )
    {
        el_fn2;
        STATIC_ASSERT( chSrc == chDst );
        STATIC_ASSERT( chSrc == C1 );
        TD *end = pSrcDst + len;

        for( ;pSrcDst<end; pSrcDst++, pSrc1++, pSrc2++,pMask++)     // Modify the scanline
            el_fn( pSrcDst, pSrc1, pSrc2,pMask,0,0);
        return fwStsNoErr;
    }
};
DEF_FOREACH( 44M  ){ return CALL_BASE( 44M )( PARAM_44M ); }
DEF_FOREACH( 44MR )
{
    TD *end = DEV::Offset( pSrcDst, roiSize.height * srcDstStep );
    for( ;pSrcDst<end; DEV::Inc( pSrcDst, srcDstStep), DEV::Inc(pSrc1,src1Step), DEV::Inc(pSrc2,src2Step), DEV::Inc(pMask,maskStep))    // For each scanline
             CALL_FOREACH( 44M )( pSrc1, pSrc2,pMask,pSrcDst, roiSize.width );
    return fwStsNoErr;
}


// =======================================
//            Argument checking.
// =======================================    
//
//    These macros check for valid parameters in calls to all functions.
//
//    They are implemented as macros in order so that the calling function
//    can benefit from an immediate return without checking the return value. 
#define CHECK_11C                                                                \
    if( pSrcDst==0 )                                    return fwStsNullPtrErr; \
    if( len    < 0 )                                    return fwStsSizeErr;

#define CHECK_11R                                                                \
    if( pSrcDst==0 )                                                             \
    if( roiSize.height<=0 || roiSize.width<=0 )         return fwStsSizeErr;

#define CHECK_11CR                                                               \
    if( pSrcDst       ==0 )                             return fwStsNullPtrErr; \
    if( srcDstStep    <=0 )                             return fwStsStepErr;    \
    if( roiSize.height<=0 || roiSize.width<=0 )         return fwStsSizeErr;

#define CHECK_21                                                                 \
    if( pSrc==0 || pDst==0 )                            return fwStsNullPtrErr; \
    if( len < 0 )                                       return fwStsSizeErr;

#define CHECK_21R                                                                \
    if( pDst   ==0 || pSrc   ==0 )                      return fwStsNullPtrErr; \
    if( dstStep<=0 || srcStep<=0 )                      return fwStsStepErr;    \
    if( roiSize.height<=0 || roiSize.width <=0 )        return fwStsSizeErr;

#define CHECK_22R                                                                \
    if( pSrc   ==0 || pSrcDst   ==0 )                   return fwStsNullPtrErr; \
    if( srcStep<=0 || srcDstStep<=0 )                   return fwStsStepErr;    \
    if( roiSize.height<=0 || roiSize.width<=0 )         return fwStsSizeErr;

#define CHECK_32                                                                 \
    if( pSrc1==0 || pSrc2==0 || pDst==0 )               return fwStsNullPtrErr; \
    if( len  < 0 )                                      return fwStsSizeErr;

#define CHECK_32R                                                                \
    if( pSrc1   ==0 || pSrc2   ==0 || pDst   ==0 )      return fwStsNullPtrErr; \
    if( src1Step<=0 || src2Step<=0 || dstStep<=0 )      return fwStsStepErr;    \
    if( roiSize.height<= 0 || roiSize.width<=0  )       return fwStsSizeErr;

#define CHECK_33R                                                                \
    if( pSrc1   ==0 || pSrc2   ==0 || pSrcDst   ==0 )   return fwStsNullPtrErr; \
    if( src1Step<=0 || src2Step<=0 || srcDstStep<=0 )   return fwStsStepErr;    \
    if( roiSize.height<= 0 || roiSize.width<=0  )       return fwStsSizeErr;

#define CHECK_44MR                                                               \
    CHECK_33R                                                                    \
    if(!pMask) return fwStsNullPtrErr;                                          \

#define CHECK_33MR                                                               \
    CHECK_22R                                                                    \
    if(!pMask) return fwStsNullPtrErr;

#define CHECK_33MRA                                                              \
    CHECK_33MR                                                                   \
    if(alpha<0) return fwStsSizeErr;

//
//    Since not all parameters are checked, not all flavors need a unique checking routine.
//    These macros resolve the redundancies.
//

#define CHECK_11RS     CHECK_11R
#define CHECK_11CS     CHECK_11C
#define CHECK_11CRS    CHECK_11CR
#define CHECK_11KR     CHECK_11CR
#define CHECK_11KRS    CHECK_11CR
#define CHECK_11KR     CHECK_11CR

#define CHECK_21C      CHECK_21
#define CHECK_21CS     CHECK_21
#define CHECK_21S      CHECK_21
#define CHECK_21CR     CHECK_21R
#define CHECK_21CRS    CHECK_21R
#define CHECK_21KR     CHECK_21R
#define CHECK_21KRS    CHECK_21R
#define CHECK_21RS     CHECK_21R
#define CHECK_21MR     CHECK_21R

#define CHECK_21SP CHECK_21
#define CHECK_21RP CHECK_21R    // For Color Model Conversion

#define CHECK_22RS        CHECK_22R
#define CHECK_22RA      CHECK_22R

#define CHECK_32RS        CHECK_32R    

// =======================================
//                    Apply
// =======================================
//
//    1. Calls the appropriate parameter check macro.
//    2. Dispatches the appropriate version of forEach().
//


#define DECLARE_APPLY( SEL )                                                                         \
TEMPLATE_ARGS_ND                                                                                     \
SYS_INLINE FwStatus Apply_##SEL( ARGS_##SEL)                                                        \
{                                                                                                    \
        CHECK_##SEL                                                                                  \
        switch( Dispatch::Type<DT_SSE2>() )                                                          \
        {                                                                                            \
        case DT_SSE3: return forEach_##SEL<TS,TD,chSrc,chDst,DT_SSE2, el_fn, el_fn2>( PARAM_##SEL ); \
        case DT_SSE2: return forEach_##SEL<TS,TD,chSrc,chDst,DT_SSE2, el_fn, el_fn2>( PARAM_##SEL ); \
        default:      return forEach_##SEL<TS,TD,chSrc,chDst,DT_REFR, el_fn, el_fn2>( PARAM_##SEL ); \
    }                                                                                                \
}

//------------------------
//    Apply Declarations
// ------------------------
    

DECLARE_APPLY( 11R  ); DECLARE_APPLY( 11RS  );
DECLARE_APPLY( 11C  ); DECLARE_APPLY( 11CS  );
DECLARE_APPLY( 11CR ); DECLARE_APPLY( 11CRS );
DECLARE_APPLY( 11KR ); DECLARE_APPLY( 11KRS );

DECLARE_APPLY( 21S  );
DECLARE_APPLY( 21C  ); DECLARE_APPLY( 21CS  );
DECLARE_APPLY( 21CR ); DECLARE_APPLY( 21CRS );
DECLARE_APPLY( 21KR ); DECLARE_APPLY( 21KRS );
DECLARE_APPLY( 21R  ); DECLARE_APPLY( 21RS  );
DECLARE_APPLY( 21MR );

DECLARE_APPLY( 21SP );        // For Color Model Conversion
DECLARE_APPLY( 21RP );        // For Color Model Conversion

DECLARE_APPLY( 22R  ); DECLARE_APPLY( 22RS );
DECLARE_APPLY( 22RA );

DECLARE_APPLY( 32   );
DECLARE_APPLY( 32R  ); DECLARE_APPLY( 32RS );

DECLARE_APPLY( 33R   );
DECLARE_APPLY( 33MR  );
DECLARE_APPLY( 33MRA );

DECLARE_APPLY( 44MR  );


} // namespace OPT_LEVEL

#endif //__ALGORITHM_H_
