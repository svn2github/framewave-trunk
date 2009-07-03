/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FE_H__
#define __FE_H__
#include "FwSharedCode_SSE2.h"

#pragma warning (disable: 4512)
#pragma warning (disable: 4328)

namespace OPT_LEVEL
{

/*   
    TODO: Needs to be reworked
    Example FE function:

    FwStatus STDCALL Add_8u_B1 ( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor )
    {
    struct Data : public fe1< Fw8u, C1 >                  // Inherit from fe<X> where X is buffer count
    {   
        FE_SSE2_REF                                       // This function provides SSE2 and REF code            

        IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
        {         
            r.dst[0].i = _mm_adds_epu8( r.dst[0].i, r.dst[0].i );      
        }      
        IV REFR( RegFile & r ) const                        // REFR Pixel function
        {         
            for( int i=0; i<16; ++i )
                r.dst[0].u8[i] += r.dst[0].u8[i];
        }      
    } data;                                             // Instance of the Data

    return B1<                                          // B<X> where X is buffer count
           Data,                                    // Name of the feDatX implementation                                                      
           VAR_STREAM                                 // FE customizing param ( VAR_NORMAL, VAR_CUSTOM_1D, VAR_STREAM )
           >::fe( data, pSrcDst, srcDstStep, roiSize );      // Run time parameters
    } 

    Compile Time constant values available for use in pixel calculation (Given in all CAPS)
    -------------------------------------------------------------

    TD1_         Data type of the destination
    TS1_        Data type of the first source
    CD_         Enum describing the channels in the destination
    CS1_        Enum describing the channels in the first source

    PIX_SSE     Number of pixels to calculate each IV xyz( RegFile & r ) function

    <DST,SRC1,SRC2,SRC3>   [ Reg Descriptors ]
     DST.ELEMENTS   Number of elements in this buffer ( 1 RGB pixel    = 3 elements )
     DST.BYTES      Number of BYTEs in this buffer    ( 1 xmm register = 16 BYTEs   )
     DST.REGCOUNT   Number of xmm registers           ( 8bit RGB may use 3 registers)

    Runtime Time data values (all lower case)
    -------------------------------------------------------------
    px_nRef      Number of pixels to process in refrence // Useful only for Custom1D.
    r            Array of source and dest XMM128 registers
*/


const static U32 DEF_BYTES_PER_THREAD = 65536;

// =========================================================
//                  MACROS
// =========================================================
#define IV  SYS_INLINE        void  
#define SV            static void
#define ISV SYS_INLINE static void
#define IS  SYS_INLINE static 
#define M_MAX( a, b )   (a>b ? a : b)
#define M_MIN( a, b )   (a<b ? a : b)
#define CH_COUNT(c)      (M_MIN(c,(CH)4))

#define PIX_C1(    T )   ((U32)16 / (U32)sizeof(T))
#define PIX_C2(    T )   ((U32)16 * (M_MAX((U32)8, (U32)sizeof(T)) / (U32)8) / ((U32)2 * (U32)sizeof(T)))
#define PIX_C3(    T )   PIX_C1(T)
#define PIX_C3_96( T )   ((U32)12 * (M_MAX((U32)4, (U32)sizeof(T)) / (U32)4) / ((U32)3 * (U32)sizeof(T)))
#define PIX_C4(    T )   ((U32)16 * (M_MAX((U32)4, (U32)sizeof(T)) / (U32)4) / ((U32)4 * (U32)sizeof(T)))

#define PIX_COUNT( T, c )               \
    ((CH_COUNT(c) == 1) ? PIX_C1(T) :   \
     (CH_COUNT(c) == 2) ? PIX_C2(T) :   \
     (CH_COUNT(c) == 3) ? PIX_C3(T) :   \
     (CH_COUNT(c) == 4) ? PIX_C4(T) :   \
     0 )

// F10H
#define FE_F10H_PFT_ABSENT                  const static PxFnType f10hPft = PFT_ABSENT;

#define FE_F10H_PFT_NORMAL                  const static PxFnType f10hPft = PFT_NORMAL;
#define FE_F10H_PFT_NORMAL_STREAM           const static PxFnType f10hPft = PFT_NORMAL_STREAM;

#define FE_F10H_PFT_EMULATE                 const static PxFnType f10hPft = PFT_NORMAL;                                     \
                                                                            IV F10H( RegFile &r ) { this->SSE2(r); }        \
                                                                            IV F10H_Init()        { this->SSE2_Init(); } 

#define FE_F10H_PFT_EMULATE_STREAM          const static PxFnType f10hPft = PFT_NORMAL_STREAM;                              \
                                                                            IV F10H( RegFile &r ) { this->SSE2(r); }        \
                                                                            IV F10H_Init()          { this->SSE2_Init(); } 

#define FE_F10H_PFT_EMULATE_CUSTOM          const static PxFnType f10hPft = PFT_EMULATE_CUSTOM;                             \
                                                                            IV F10H_Init()        { this->SSE2_Init(); } 

#define FE_F10H_PFT_EMULATE_CUSTOM_NO_FB    const static PxFnType f10hPft = PFT_EMULATE_CUSTOM_NO_FB;                       \
                                                                            IV F10H_Init()        { this->SSE2_Init(); }    

#define FE_F10H_PFT_CUSTOM                  const static PxFnType f10hPft = PFT_CUSTOM;
#define FE_F10H_PFT_CUSTOM_NO_FB            const static PxFnType f10hPft = PFT_CUSTOM_NO_FB;


// SSE2
#define FE_SSE2_PFT_ABSENT                  const static PxFnType sse2Pft = PFT_ABSENT;

#define FE_SSE2_PFT_NORMAL                  const static PxFnType sse2Pft = PFT_NORMAL;
#define FE_SSE2_PFT_NORMAL_STREAM           const static PxFnType sse2Pft = PFT_NORMAL_STREAM;

#define FE_SSE2_PFT_CUSTOM                  const static PxFnType sse2Pft = PFT_CUSTOM;
#define FE_SSE2_PFT_CUSTOM_NO_FB            const static PxFnType sse2Pft = PFT_CUSTOM_NO_FALLBACK;

// REFR
#define FE_REFR_PFT_NORMAL                  const static PxFnType refrPft = PFT_NORMAL;
#define FE_REFR_PFT_CUSTOM_NO_FB            const static PxFnType refrPft = PFT_CUSTOM_NO_FALLBACK;


#define FE_REF                                  FE_F10H_PFT_ABSENT                  \
                                                FE_SSE2_PFT_ABSENT                  \
                                                FE_REFR_PFT_NORMAL

#define FE_SSE2_REF                             FE_F10H_PFT_EMULATE                 \
                                                FE_SSE2_PFT_NORMAL                  \
                                                FE_REFR_PFT_NORMAL

#define FE_STREAM_SSE2_REF                      FE_F10H_PFT_EMULATE_STREAM          \
                                                FE_SSE2_PFT_NORMAL_STREAM           \
                                                FE_REFR_PFT_NORMAL

#define FE_CUSTOM_SSE2_REF                      FE_F10H_PFT_EMULATE_CUSTOM          \
                                                FE_SSE2_PFT_CUSTOM                  \
                                                FE_REFR_PFT_NORMAL

#define FE_CUSTOM_NO_FB_SSE2_REF                FE_F10H_PFT_EMULATE_CUSTOM_NO_FB    \
                                                FE_SSE2_PFT_CUSTOM_NO_FB            \
                                                FE_REFR_PFT_NORMAL

#define FE_CUSTOM_NO_FB_SSE2_CUSTOM_NO_FB_REF   FE_F10H_PFT_EMULATE_CUSTOM_NO_FB    \
                                                FE_SSE2_PFT_CUSTOM_NO_FB            \
                                                FE_REFR_PFT_CUSTOM_NO_FB

enum PxFnType
{
    PFT_ABSENT,                    // not present
    PFT_NORMAL,                    // present and normal
    PFT_NORMAL_STREAM,             // present and normal with streaming store
    PFT_CUSTOM,                    // present but customized with fallback
    PFT_CUSTOM_NO_FALLBACK,        // present but customized without fallback
    PFT_EMULATE_CUSTOM,            // not present but fall to next available custom
    PFT_EMULATE_CUSTOM_NO_FB       // not present but fall to next available custom with no fallback
};

enum IsAlign
{
    ALIGN_FLSE,                  // not aligned
    ALIGN_TRUE                   // aligned
};

enum IsStream
{
    STREAM_FLSE,                 // Streaming stores enabled
    STREAM_TRUE                  // Streaming stores disabled
};

enum FE_ALGORITHM
{
    ALG_1D,
    ALG_2D
};

// =========================================================
//               Pixel Function DATA 
// =========================================================

//To calculate the optimum number of threads depending on the size of the buffers
//The calculation is based on the input bytesPerThread
SYS_INLINE U32 RecommendThreads( U32 bytes, U32 bytesPerThread )
{
    //calulated max thread
    U32 maxThreads = bytes / bytesPerThread;
    //System limit of max thread based on the no of processing units
    U32 threads    = ThreadPool::ThreadCount();
    //No of threads to be used is the Minimum of the maxThread and threads caclulated above
    //if maxThread = 0(for small buffer cases) then the default is single thread
    return  M_MAX( 1, M_MIN( threads, maxThreads ));
}
// Storage for register values.
struct RegFile                                                                 
{  // Maximum of 16 registers are available in 64 bit mode and 8 registers in 32 bit mode
    XMM128 src1[4], src2[4], src3[4], src4[4];                                     
    XMM128 dst[4],  dst2[4], dst3[4], dst4[8];
};

// Described the contents of the register
template< typename T, CH c, U32 pixels >
struct RegDesc                                                                 
{   
    // Number of elements in buffer per SSE iteration.
    const static U32 ELEMENTS = pixels * CH_COUNT(c);  
    // number of BYTE units. 1 xmm register = 16 BYTE units.
    const static U32 BYTES    = ELEMENTS * sizeof(T);      
    // number of xmm registers.
    const static U32 REGCOUNT = (BYTES+15)/16;                                    
};

struct LoadStoreModules 
{   
    // Single precision floating point load/store
    //REFR, Unaligned
   template< U32 bytes, DispatchType dt, IsAlign ia, IsStream is >             
   struct MEMf
   {
        ISV Load ( XMM128 * reg, const void* src )
        { 
            reg;
            src; 
        }
        ISV Store( XMM128 * reg,       void* dst )
        { 
            reg;
            dst;
        }
   };

 
   template< U32 bytes >
   struct MEMf< bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >                      
   {

      //SSE2 Unaligned Load of specified number of bytes
      ISV Load( XMM128 * reg, const void* src )   
      {   float * mem = (float*)src;   
         switch( bytes )
         {
         case 64:   reg[3].f = _mm_loadu_ps( mem + (3 * 4) );      
         case 48:   reg[2].f = _mm_loadu_ps( mem + (2 * 4) );
         case 32:   reg[1].f = _mm_loadu_ps( mem + (1 * 4) );
         case 16:   reg[0].f = _mm_loadu_ps( mem + 0 );  
                    break;
         case 24:   CBL_OPTLEVEL::Load_192(reg[0].i, reg[1].i, (__m128i*)mem);                   
                    break;
         case 12:   CBL_OPTLEVEL::Load_96 (reg[0].i, (__m128i*)mem);
                    break;
         case  8:   CBL_OPTLEVEL::Load_64 (reg[0].i, (__m128i*)mem);
                    break;
         case  4:   CBL_OPTLEVEL::Load_32 (reg[0].i, (__m128i*)mem);
                    break;
         case  3:   CBL_OPTLEVEL::Load_24 (reg[0].i, (__m128i*)mem);
                    break;
         case  2:   CBL_OPTLEVEL::Load_16 (reg[0].i, (__m128i*)mem);
                    break;
         case  1:   CBL_OPTLEVEL::Load_8  (reg[0].i, (__m128i*)mem);
                    break;
         default:   assert(false);
         }
      }

      //SSE2 Unaligned Store of specified number of bytes
      ISV Store( XMM128 * reg, void* dst )
      {   float* mem = (float*)dst;
         switch( bytes )
         {
         case 64:   _mm_storeu_ps( mem + (3 * 4), reg[3].f );                     
         case 48:   _mm_storeu_ps( mem + (2 * 4), reg[2].f );
         case 32:   _mm_storeu_ps( mem + (1 * 4), reg[1].f );
         case 16:   _mm_storeu_ps( mem + 0, reg[0].f );
                    break;
         case 24:   CBL_OPTLEVEL::Store_192((__m128i*)mem, reg[0].i, reg[1].i);
                    break;
         case 12:   CBL_OPTLEVEL::Store_96 ((__m128i*)mem, reg[0].i);
                    break;
         case  8:   CBL_OPTLEVEL::Store_64 ((__m128i*)mem, reg[0].i);
                    break;
         case  4:   CBL_OPTLEVEL::Store_32 ((__m128i*)mem, reg[0].i);
                    break;   
         case  3:   CBL_OPTLEVEL::Store_24 ((__m128i*)mem, reg[0].i);
                    break;
         case  2:   CBL_OPTLEVEL::Store_16 ((__m128i*)mem, reg[0].i);
                    break;
         case  1:   CBL_OPTLEVEL::Store_8  ((__m128i*)mem, reg[0].i);
                    break;
         default:   assert(false);
         }
      }
   };

   template< U32 bytes >
   struct MEMf< bytes, DT_F10H, ALIGN_FLSE, STREAM_FLSE >                       
   {
        //F10H Unaligned Load falls back to SSE2 Unaligned Load
		ISV Load( XMM128 * reg, const void* src ) { MEMf<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load(reg, src);  }
        //F10H Unaligned Store falls back to SSE2 Unaligned Store
		ISV Store( XMM128 * reg, void* dst )		{ MEMf<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store(reg, dst); }
   };

   template< U32 bytes >
   struct MEMf< bytes, DT_SSE2, ALIGN_FLSE, STREAM_TRUE >                       
   {
       // SSE2, Unaligned, Stream
		ISV Load( XMM128 * reg, const void* src ) { MEMf<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load(reg, src);  }
		ISV Store( XMM128 * reg, void* dst )		{ MEMf<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store(reg, dst); }
   };

   template< U32 bytes >
   struct MEMf< bytes, DT_F10H, ALIGN_FLSE, STREAM_TRUE >                      
   {    
        // F10H, Unaligned, Stream
		ISV Load( XMM128 * reg, const void* src ) { MEMf<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load(reg, src);  }
		ISV Store( XMM128 * reg, void* dst )		{ MEMf<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store(reg, dst); }
   };

   template< U32 bytes >
   struct MEMf< bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >                       
   {
      //SSE2 Aligned Load of specified number of bytes
      ISV Load( XMM128 * reg, const void* src )                              
      {   float * mem = (float*)src;      
         switch( bytes )
         {
         case 64:   reg[3].f = _mm_load_ps( mem + (3 * 4) );                      
         case 48:   reg[2].f = _mm_load_ps( mem + (2 * 4) );
         case 32:   reg[1].f = _mm_load_ps( mem + (1 * 4) );
         case 16:   reg[0].f = _mm_load_ps( mem + 0 );                  
                    break;
         default:   
             //Partial Aligned Load falls back to Unaligned case
            MEMf< bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load( reg, mem );   
         }
      }
      //SSE2 Aligned Store of specified number of bytes
      ISV Store( XMM128 * reg, void* dst )
      {   float* mem = (float*)dst;
         switch( bytes )
         {
         case 64:   _mm_store_ps( mem + (3 * 4), reg[3].f );                     
         case 48:   _mm_store_ps( mem + (2 * 4), reg[2].f );
         case 32:   _mm_store_ps( mem + (1 * 4), reg[1].f );
         case 16:   _mm_store_ps( mem + 0, reg[0].f );                  
                    break;
         default:
             //Partial Aligned Store falls back to Unaligned case
            MEMf< bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store( reg, mem ); 
         }
      }
   };


   template< U32 bytes >
   struct MEMf< bytes, DT_F10H, ALIGN_TRUE, STREAM_FLSE >                       
   {
        //F10H Aligned Load falls back to SSE2 Aligned Load
	    ISV Load( XMM128 * reg, const void* src ) { MEMf<bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >::Load(reg, src);  }
		//F10H Aligned Store falls back to SSE2 Aligned Store
        ISV Store( XMM128 * reg, void* dst )		{ MEMf<bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >::Store(reg, dst); }
   };

   template< U32 bytes >
   struct MEMf< bytes, DT_SSE2, ALIGN_TRUE, STREAM_TRUE >                       
   {
      // SSE2, Aligned, Stream
      ISV Load ( XMM128 * reg, const void* src ){ MEMf< bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >::Load( reg, src ); }
      ISV Store( XMM128 * reg,       void* dst )
      {   float* mem = (float*)dst;
         switch( bytes )
         {
         case 64:   _mm_stream_ps( mem + (3 * 4), reg[3].f );                     
         case 48:   _mm_stream_ps( mem + (2 * 4), reg[2].f );
         case 32:   _mm_stream_ps( mem + (1 * 4), reg[1].f );
         case 16:   _mm_stream_ps( mem + 0, reg[0].f );                 
                    break;         
         default:   
            MEMf< bytes, DT_SSE2, ALIGN_FLSE, STREAM_TRUE >::Store( reg, mem ); 
         }
      }      
   };

   // F10H, Aligned, No Stream
   template< U32 bytes >
   struct MEMf< bytes, DT_F10H, ALIGN_TRUE, STREAM_TRUE >                       
   {
		ISV Load( XMM128 * reg, const void* src ) { MEMf<bytes, DT_SSE2, ALIGN_TRUE, STREAM_TRUE >::Load(reg, src);  }
		ISV Store( XMM128 * reg, void* dst )		{ MEMf<bytes, DT_SSE2, ALIGN_TRUE, STREAM_TRUE >::Store(reg, dst); }
   };
    
    
    //Double load/store

       // REFR, Unaligned
   template< U32 bytes, DispatchType dt, IsAlign ia, IsStream is >             
   struct MEMd
   {
        ISV Load ( XMM128 * reg, const void* src )
        { 
            reg;
            src; 
        }
        ISV Store( XMM128 * reg,       void* dst )
        { 
            reg;
            dst;
        }
   };

 
   template< U32 bytes >
   struct MEMd< bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >                      
   {

      //SSE2 Unaligned Load of specified number of bytes
      ISV Load( XMM128 * reg, const void* src )   
      {   double * mem = (double*)src;   
         switch( bytes )
         {
         case 64:   reg[3].d = _mm_loadu_pd( mem + (3 * 2) );      
         case 48:   reg[2].d = _mm_loadu_pd( mem + (2 * 2) );
         case 32:   reg[1].d = _mm_loadu_pd( mem + (1 * 2) );
         case 16:   reg[0].d = _mm_loadu_pd( mem + 0 );  
                    break;
         case 24:   CBL_OPTLEVEL::Load_192(reg[0].i, reg[1].i, (__m128i*)mem);                   
                    break;
         case 12:   CBL_OPTLEVEL::Load_96 (reg[0].i, (__m128i*)mem);
                    break;
         case  8:   CBL_OPTLEVEL::Load_64 (reg[0].i, (__m128i*)mem);
                    break;
         case  4:   CBL_OPTLEVEL::Load_32 (reg[0].i, (__m128i*)mem);
                    break;
         case  3:   CBL_OPTLEVEL::Load_24 (reg[0].i, (__m128i*)mem);
                    break;
         case  2:   CBL_OPTLEVEL::Load_16 (reg[0].i, (__m128i*)mem);
                    break;
         case  1:   CBL_OPTLEVEL::Load_8  (reg[0].i, (__m128i*)mem);
                    break;
         default:   assert(false);
         }
      }

      //SSE2 Unaligned Store of specified number of bytes
      ISV Store( XMM128 * reg, void* dst )
      {   double* mem = (double*)dst;
         switch( bytes )
         {
         case 64:   _mm_storeu_pd( mem + (3 * 2), reg[3].d );                     
         case 48:   _mm_storeu_pd( mem + (2 * 2), reg[2].d );
         case 32:   _mm_storeu_pd( mem + (1 * 2), reg[1].d );
         case 16:   _mm_storeu_pd( mem + 0, reg[0].d );
                    break;
         case 24:   CBL_OPTLEVEL::Store_192((__m128i*)mem, reg[0].i, reg[1].i);
                    break;
         case 12:   CBL_OPTLEVEL::Store_96 ((__m128i*)mem, reg[0].i);
                    break;
         case  8:   CBL_OPTLEVEL::Store_64 ((__m128i*)mem, reg[0].i);
                    break;
         case  4:   CBL_OPTLEVEL::Store_32 ((__m128i*)mem, reg[0].i);
                    break;   
         case  3:   CBL_OPTLEVEL::Store_24 ((__m128i*)mem, reg[0].i);
                    break;
         case  2:   CBL_OPTLEVEL::Store_16 ((__m128i*)mem, reg[0].i);
                    break;
         case  1:   CBL_OPTLEVEL::Store_8  ((__m128i*)mem, reg[0].i);
                    break;
         default:   assert(false);
         }
      }
   };

   template< U32 bytes >
   struct MEMd< bytes, DT_F10H, ALIGN_FLSE, STREAM_FLSE >                       
   {
        //F10H Unaligned Load falls back to SSE2 Unaligned Load
		ISV Load( XMM128 * reg, const void* src ) { MEMd<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load(reg, src);  }
        //F10H Unaligned Store falls back to SSE2 Unaligned Store
		ISV Store( XMM128 * reg, void* dst )		{ MEMd<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store(reg, dst); }
   };

   template< U32 bytes >
   struct MEMd< bytes, DT_SSE2, ALIGN_FLSE, STREAM_TRUE >                       
   {
       // SSE2, Unaligned, Stream
		ISV Load( XMM128 * reg, const void* src ) { MEMd<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load(reg, src);  }
		ISV Store( XMM128 * reg, void* dst )		{ MEMd<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store(reg, dst); }
   };

   template< U32 bytes >
   struct MEMd< bytes, DT_F10H, ALIGN_FLSE, STREAM_TRUE >                      
   {    
        // F10H, Unaligned, Stream
		ISV Load( XMM128 * reg, const void* src ) { MEMd<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load(reg, src);  }
		ISV Store( XMM128 * reg, void* dst )		{ MEMd<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store(reg, dst); }
   };

   template< U32 bytes >
   struct MEMd< bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >                       
   {
      //SSE2 Aligned Load of specified number of bytes
      ISV Load( XMM128 * reg, const void* src )                              
      {   double * mem = (double*)src;      
         switch( bytes )
         {
         case 64:   reg[3].d = _mm_load_pd( mem + (3 * 2) );                      
         case 48:   reg[2].d = _mm_load_pd( mem + (2 * 2) );
         case 32:   reg[1].d = _mm_load_pd( mem + (1 * 2) );
         case 16:   reg[0].d = _mm_load_pd( mem + 0 );                  
                    break;
         default:   
             //Partial Aligned Load falls back to Unaligned case
            MEMd< bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load( reg, mem );   
         }
      }
      //SSE2 Aligned Store of specified number of bytes
      ISV Store( XMM128 * reg, void* dst )
      {   double* mem = (double*)dst;
         switch( bytes )
         {
         case 64:   _mm_store_pd( mem + (3 * 2), reg[3].d );                     
         case 48:   _mm_store_pd( mem + (2 * 2), reg[2].d );
         case 32:   _mm_store_pd( mem + (1 * 2), reg[1].d );
         case 16:   _mm_store_pd( mem + 0, reg[0].d );                  
                    break;
         default:
             //Partial Aligned Store falls back to Unaligned case
            MEMd< bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store( reg, mem ); 
         }
      }
   };


   template< U32 bytes >
   struct MEMd< bytes, DT_F10H, ALIGN_TRUE, STREAM_FLSE >                       
   {
        //F10H Aligned Load falls back to SSE2 Aligned Load
	    ISV Load( XMM128 * reg, const void* src ) { MEMd<bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >::Load(reg, src);  }
		//F10H Aligned Store falls back to SSE2 Aligned Store
        ISV Store( XMM128 * reg, void* dst )		{ MEMd<bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >::Store(reg, dst); }
   };

   template< U32 bytes >
   struct MEMd< bytes, DT_SSE2, ALIGN_TRUE, STREAM_TRUE >                       
   {
      // SSE2, Aligned, Stream
      ISV Load ( XMM128 * reg, const void* src ){ MEMd< bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >::Load( reg, src ); }
      ISV Store( XMM128 * reg,       void* dst )
      {   double* mem = (double*)dst;
         switch( bytes )
         {
         case 64:   _mm_stream_pd( mem + (3 * 2), reg[3].d );                     
         case 48:   _mm_stream_pd( mem + (2 * 2), reg[2].d );
         case 32:   _mm_stream_pd( mem + (1 * 2), reg[1].d );
         case 16:   _mm_stream_pd( mem + 0, reg[0].d );                 
                    break;         
         default:   
            MEMd< bytes, DT_SSE2, ALIGN_FLSE, STREAM_TRUE >::Store( reg, mem ); 
         }
      }      
   };

   // F10H, Aligned, No Stream
   template< U32 bytes >
   struct MEMd< bytes, DT_F10H, ALIGN_TRUE, STREAM_TRUE >                       
   {
		ISV Load( XMM128 * reg, const void* src ) { MEMd<bytes, DT_SSE2, ALIGN_TRUE, STREAM_TRUE >::Load(reg, src);  }
		ISV Store( XMM128 * reg, void* dst )		{ MEMd<bytes, DT_SSE2, ALIGN_TRUE, STREAM_TRUE >::Store(reg, dst); }
   };

    // REFR, Unaligned
    template< U32 bytes, DispatchType dt, IsAlign ia, IsStream is >             
    struct MEM
    {
        ISV Load ( XMM128 * reg, const void* src )
        { 
            reg;
            src; 
        }
        ISV Store( XMM128 * reg,       void* dst )
        { 
            reg;
            dst;
        }
    };

 
    template< U32 bytes >
    struct MEM< bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >                      
    {

        //SSE2 Unaligned Load of specified number of bytes
        ISV Load( XMM128 * reg, const void* src )   
        {
            __m128i * mem = (__m128i*)src;   
            switch( bytes )
            {
            case 64:    reg[3].i = _mm_loadu_si128( mem + 3 );      
            case 48:    reg[2].i = _mm_loadu_si128( mem + 2 );
            case 32:    reg[1].i = _mm_loadu_si128( mem + 1 );
            case 16:    reg[0].i = _mm_loadu_si128( mem + 0 );  
                        break;
            case 24:    CBL_OPTLEVEL::Load_192(reg[0].i, reg[1].i, mem);
                        break;
            case 12:    CBL_OPTLEVEL::Load_96 (reg[0].i, mem);
                        break;
            case  8:    CBL_OPTLEVEL::Load_64 (reg[0].i, mem);
                        break;
            case  4:    CBL_OPTLEVEL::Load_32 (reg[0].i, mem);
                        break;
            case  3:    CBL_OPTLEVEL::Load_24 (reg[0].i, mem);
                        break;
            case  2:    CBL_OPTLEVEL::Load_16 (reg[0].i, mem);
                        break;
            case  1:    CBL_OPTLEVEL::Load_8  (reg[0].i, mem);
                        break;
            default:   assert(false);
            }
        }

        //SSE2 Unaligned Store of specified number of bytes
        ISV Store( XMM128 * reg, void* dst )
        {
            __m128i* mem = (__m128i*)dst;
            switch( bytes )
            {
            case 64:    _mm_storeu_si128( mem + 3, reg[3].i );                     
            case 48:    _mm_storeu_si128( mem + 2, reg[2].i );
            case 32:    _mm_storeu_si128( mem + 1, reg[1].i );
            case 16:    _mm_storeu_si128( mem + 0, reg[0].i );
                        break;
            case 24:    CBL_OPTLEVEL::Store_192(mem, reg[0].i, reg[1].i);
                        break;
            case 12:    CBL_OPTLEVEL::Store_96 (mem, reg[0].i);
                        break;
            case  8:    CBL_OPTLEVEL::Store_64 (mem, reg[0].i);
                        break;
            case  4:    CBL_OPTLEVEL::Store_32 (mem, reg[0].i);
                        break;   
            case  3:    CBL_OPTLEVEL::Store_24 (mem, reg[0].i);
                        break;
            case  2:    CBL_OPTLEVEL::Store_16 (mem, reg[0].i);
                        break;
            case  1:    CBL_OPTLEVEL::Store_8  (mem, reg[0].i);
                        break;
            default:    assert(false);
            }
        }
    };

    template< U32 bytes >
    struct MEM< bytes, DT_F10H, ALIGN_FLSE, STREAM_FLSE >                       
    {
        //F10H Unaligned Load falls back to SSE2 Unaligned Load
        ISV Load( XMM128 * reg, const void* src ) { MEM<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load(reg, src);  }
        //F10H Unaligned Store falls back to SSE2 Unaligned Store
        ISV Store( XMM128 * reg, void* dst )        { MEM<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store(reg, dst); }
    };

    template< U32 bytes >
    struct MEM< bytes, DT_SSE2, ALIGN_FLSE, STREAM_TRUE >                       
    {
        // SSE2, Unaligned, Stream
        ISV Load( XMM128 * reg, const void* src ) { MEM<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load(reg, src);  }
        ISV Store( XMM128 * reg, void* dst )        { MEM<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store(reg, dst); }
    };

    template< U32 bytes >
    struct MEM< bytes, DT_F10H, ALIGN_FLSE, STREAM_TRUE >                      
    {    
        // F10H, Unaligned, Stream
        ISV Load( XMM128 * reg, const void* src ) { MEM<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load(reg, src);  }
        ISV Store( XMM128 * reg, void* dst )        { MEM<bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store(reg, dst); }
    };

    template< U32 bytes >
    struct MEM< bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >                       
    {
        //SSE2 Aligned Load of specified number of bytes
        ISV Load( XMM128 * reg, const void* src )                              
        {
            __m128i * mem = (__m128i*)src;      
            switch( bytes )
            {
            case 64:    reg[3].i = _mm_load_si128( mem + 3 );                      
            case 48:    reg[2].i = _mm_load_si128( mem + 2 );
            case 32:    reg[1].i = _mm_load_si128( mem + 1 );
            case 16:    reg[0].i = _mm_load_si128( mem + 0 );                  
                        break;
            default:   
                        //Partial Aligned Load falls back to Unaligned case
                        MEM< bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Load( reg, mem );   
            }
        }

        //SSE2 Aligned Store of specified number of bytes
        ISV Store( XMM128 * reg, void* dst )
        {
            __m128i* mem = (__m128i*)dst;
            switch( bytes )
            {
            case 64:    _mm_store_si128( mem + 3, reg[3].i );                     
            case 48:    _mm_store_si128( mem + 2, reg[2].i );
            case 32:    _mm_store_si128( mem + 1, reg[1].i );
            case 16:    _mm_store_si128( mem + 0, reg[0].i );                  
                        break;
            default:
                        //Partial Aligned Store falls back to Unaligned case
                        MEM< bytes, DT_SSE2, ALIGN_FLSE, STREAM_FLSE >::Store( reg, mem ); 
            }
        }
    };


    template< U32 bytes >
    struct MEM< bytes, DT_F10H, ALIGN_TRUE, STREAM_FLSE >                       
    {
        //F10H Aligned Load falls back to SSE2 Aligned Load
        ISV Load( XMM128 * reg, const void* src ) { MEM<bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >::Load(reg, src);  }
        //F10H Aligned Store falls back to SSE2 Aligned Store
        ISV Store( XMM128 * reg, void* dst )        { MEM<bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >::Store(reg, dst); }
    };

    template< U32 bytes >
    struct MEM< bytes, DT_SSE2, ALIGN_TRUE, STREAM_TRUE >                       
    {
        // SSE2, Aligned, Stream
        ISV Load ( XMM128 * reg, const void* src ){ MEM< bytes, DT_SSE2, ALIGN_TRUE, STREAM_FLSE >::Load( reg, src ); }
        ISV Store( XMM128 * reg,       void* dst )
        {
            __m128i* mem = (__m128i*)dst;
            switch( bytes )
            {
            case 64:    _mm_stream_si128( mem + 3, reg[3].i );                     
            case 48:    _mm_stream_si128( mem + 2, reg[2].i );
            case 32:    _mm_stream_si128( mem + 1, reg[1].i );
            case 16:    _mm_stream_si128( mem + 0, reg[0].i );                 
                        break;         
            default:   
            MEM< bytes, DT_SSE2, ALIGN_FLSE, STREAM_TRUE >::Store( reg, mem ); 
            }
        }      
    };

    // F10H, Aligned, No Stream
    template< U32 bytes >
    struct MEM< bytes, DT_F10H, ALIGN_TRUE, STREAM_TRUE >                       
    {
        ISV Load ( XMM128 * reg, const void* src )   { MEM<bytes, DT_SSE2, ALIGN_TRUE, STREAM_TRUE >::Load (reg, src);  }
        ISV Store( XMM128 * reg,       void* dst )   { MEM<bytes, DT_SSE2, ALIGN_TRUE, STREAM_TRUE >::Store(reg, dst); }
    };

    template< U32 bytes, DispatchType dt, IsAlign ia, IsStream is >
    ISV LOAD(XMM128 * reg, const void* src)    
    {
        MEM< bytes,DISPATCH,ia,is >::Load(reg,src);
    }

    template< U32 bytes, DispatchType dt, IsAlign ia, IsStream is >
    ISV STORE(XMM128 * reg,  void* src)    
    {
        MEM< bytes,DISPATCH,ia,is >::Store(reg,src);
    }

    template< U32 bytes, DispatchType dt, IsAlign ia, IsStream is >
    ISV LOAD(XMM128 * reg, const Fw64f* src)    
    {
        MEMd< bytes,DISPATCH,ia,is >::Load(reg,src);
    }

    template< U32 bytes, DispatchType dt, IsAlign ia, IsStream is >
    ISV STORE(XMM128 * reg,  Fw64f* src)    
    {
        MEMd< bytes,DISPATCH,ia,is >::Store(reg,src);
    }

    template< U32 bytes, DispatchType dt, IsAlign ia, IsStream is >
    ISV LOAD(XMM128 * reg, const Fw32f* src)    
    {
        MEMf< bytes,DISPATCH,ia,is >::Load(reg,src);
    }

    template< U32 bytes, DispatchType dt, IsAlign ia, IsStream is >
    ISV STORE(XMM128 * reg,  Fw32f* src)    
    {
        MEMf< bytes,DISPATCH,ia,is >::Store(reg,src);
    }

    template<class T>
    IV  IncrementBuffers(T **ptr,  int count)
    {
        *ptr += count;
    }

    template<class T1, class T2>
    IV  IncrementBuffers(T1 **ptr1, int count1,
                         T2 **ptr2, int count2)
    {
        *ptr1 += count1;
        *ptr2 += count2;
    }
    template<class T1, class T2,class T3>
    IV  IncrementBuffers(T1 **ptr1, int count1,
                         T2 **ptr2, int count2,
                         T3 **ptr3, int count3)
    {
        *ptr1 += count1;
        *ptr2 += count2;
        *ptr3 += count3;
    }

    template<class T1, class T2,class T3,class T4>
    IV  IncrementBuffers(T1 **ptr1, int count1,
                         T2 **ptr2, int count2,
                         T3 **ptr3, int count3,
                         T4 **ptr4, int count4)
    {
        *ptr1 += count1;
        *ptr2 += count2;
        *ptr3 += count3;
        *ptr4 += count4;
    }

    template<class T1, class T2,class T3,class T4,class T5>
    IV  IncrementBuffers(T1 **ptr1, int count1,
                         T2 **ptr2, int count2,
                         T3 **ptr3, int count3,
                         T4 **ptr4, int count4,
                         T5 **ptr5, int count5)
    {
        *ptr1 += count1;
        *ptr2 += count2;
        *ptr3 += count3;
        *ptr4 += count4;
        *ptr5 += count5;
    }


    template<class T1, class T2,class T3,class T4,class T5, class T6>
    IV  IncrementBuffers(T1 **ptr1, int count1,
                         T2 **ptr2, int count2,
                         T3 **ptr3, int count3,
                         T4 **ptr4, int count4,
                         T5 **ptr5, int count5,
                         T6 **ptr6, int count6)
    {
        *ptr1 += count1;
        *ptr2 += count2;
        *ptr3 += count3;
        *ptr4 += count4;
        *ptr5 += count5;
        *ptr6 += count6;
    }

    template<class T1, class T2,class T3,class T4,class T5, class T6,class T7, class T8>
    IV  IncrementBuffers(T1 **ptr1, int count1,
                         T2 **ptr2, int count2,
                         T3 **ptr3, int count3,
                         T4 **ptr4, int count4,
                         T5 **ptr5, int count5,
                         T6 **ptr6, int count6,
                         T7 **ptr7, int count7,
                         T8 **ptr8, int count8)
    {
        *ptr1 += count1;
        *ptr2 += count2;
        *ptr3 += count3;
        *ptr4 += count4;
        *ptr5 += count5;
        *ptr6 += count6;
        *ptr7 += count7;
        *ptr8 += count8;
    }
};


struct feBase : public LoadStoreModules
{
    IV F10H_Init()              { SSE2_Init();  }
    IV F10H( RegFile &r ) const { SSE2(r);      }

    IV SSE2_Init()              {               }
    IV SSE2( RegFile & )  const { assert(false);}
};

//TD1-Type of destination1(some variants of fe has more than 1 destinations), cd-No of channels in destination
template< typename TD1, CH cd >
struct fe1 : public feBase
{      
   typedef TD1 TD1_;   
   const static CH CD_      = cd;
   //Number of Pixels to Load in each SSE Iteration
   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TD1_,CD_ ),(U32)16 );
   // Describe the content of the register
   class DST : public RegDesc< TD1_, CD_, nPIX_SSE > {};                          

   //Custom1D routines can be derived to give added flexibility for the developer to include their own 1D loop
   template<IsAlign ia>  IV Custom1D_f10h(TD1_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_sse2(TD1_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_refr(TD1_ *,  U32   )  const { assert(false); } 
   IV REFR(                                    TD1_ * )            const { assert(false); }
};

//TS1-Type of source1, cs1-No of channels in source1
//TD1-Type of destination1(some variants have more than 1 destinations), cd-No of channels in destination
//fe2 - suffix 2 stands for 2 buffer case
template< typename TS1, CH cs1, 
          typename TD1,  CH cd >            
struct fe2 : public feBase
{
   typedef TS1 TS1_;
   typedef TD1  TD1_;   
   const static CH  CS1_     = cs1;
   const static CH  CD_      = cd;
   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TS1_,CS1_ ), 
                                      PIX_COUNT( TD1_, CD_  ) );

   class DST:  public RegDesc< TD1_,  CD_,  nPIX_SSE > {};         
   class SRC1: public RegDesc< TS1_, CS1_, nPIX_SSE > {};
   
   template<IsAlign ia>  IV Custom1D_f10h(const TS1_ *, TD1_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_sse2(const TS1_ *, TD1_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_refr(const TS1_ *,   TD1_ *,  U32   )    const { assert(false); }
   IV REFR(                                  const TS1_ *,   TD1_ *)            const { assert(false); }
};


template< typename TS1, CH cs1, 
          typename TS2, CH cs2, 
          typename TD1,  CH cd  >         
struct fe3 : public feBase
{   
   typedef TS1 TS1_;
   typedef TS2 TS2_;
   typedef TD1  TD1_;      
   const static CH CS1_= cs1;
   const static CH CS2_= cs2;
   const static CH CD_ = cd;
   
   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TS1_,CS1_ ), 
                               M_MIN( PIX_COUNT( TS2_,CS2_ ),                            
                                      PIX_COUNT( TD1_, CD_  )));   

   class DST:  public RegDesc< TD1_,  CD_,  nPIX_SSE > {};         
   class SRC1: public RegDesc< TS1_, CS1_, nPIX_SSE > {};   
   class SRC2: public RegDesc< TS2_, CS2_, nPIX_SSE > {};
      
   template<IsAlign ia>  IV Custom1D_f10h(const TS1_ *, const TS2_ *, TD1_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_sse2(const TS1_ *, const TS2_ *, TD1_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_refr(const TS1_ *,   const TS2_ *,   TD1_ *,  U32   )    const { assert(false); }
   IV REFR(                                  const TS1_ *,   const TS2_ *,   TD1_ *)            const { assert(false); }
};


template< typename TS1, CH cs1, 
          typename TS2, CH cs2, 
          typename TS3, CH cs3,
          typename TD1,  CH cd  >         
struct fe4 : public feBase
{
   typedef TS1 TS1_;
   typedef TS2 TS2_;
   typedef TS3 TS3_;
   typedef TD1  TD1_;      
   const static CH CS1_= cs1;
   const static CH CS2_= cs2;
   const static CH CS3_= cs3;
   const static CH CD_ = cd;

   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TD1, CD_  ), 
                               M_MIN( PIX_COUNT( TS1,CS1_ ),
                               M_MIN( PIX_COUNT( TS2,CS2_ ),
                                      PIX_COUNT( TS3,CS3_ ))));

   class DST:  public RegDesc< TD1,  CD_ , nPIX_SSE > {};         
   class SRC1: public RegDesc< TS1, CS1_, nPIX_SSE > {};   
   class SRC2: public RegDesc< TS2, CS2_, nPIX_SSE > {};   
   class SRC3: public RegDesc< TS3, CS3_, nPIX_SSE > {};
   
   template<IsAlign ia>  IV Custom1D_f10h(const TS1_ *, const TS2_ *, const TS3_ *, TD1 *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_sse2(const TS1_ *, const TS2_ *, const TS3_ *, TD1 *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_refr(const TS1_ *,   const TS2_ *,   const TS3_ *,   TD1 *,  U32   )    const { assert(false); }   
   IV REFR(                                  const TS1_ *, const TS2_ *, const TS3_ *, TD1_ *)            const { assert(false); }

};


template< typename TS1, CH cs1, 
          typename TD1, CH cd1, 
          typename TD2, CH cd2,
          typename TD3,  CH cd3  >         
struct fe1S3D : public feBase
{
   typedef TS1 TS1_;
   typedef TD1 TD1_;
   typedef TD2 TD2_;
   typedef TD3  TD3_;      
   const static CH CS1_= cs1;
   const static CH CD1_= cd1;
   const static CH CD2_= cd2;
   const static CH CD3_ = cd3;

   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TD1, CD1_  ), 
                               M_MIN( PIX_COUNT( TD2,CD2_ ),
                               M_MIN( PIX_COUNT( TD3,CD3_ ),
                                      PIX_COUNT( TS1,CS1_ ))));

   class DST1:  public RegDesc< TD1,CD1_ , nPIX_SSE > {};         
   class DST2: public RegDesc< TD2, CD2_, nPIX_SSE > {};   
   class DST3: public RegDesc< TD3, CD3_, nPIX_SSE > {};   
   class SRC1: public RegDesc< TS1, CS1_, nPIX_SSE > {};
   
   template<IsAlign ia>  IV Custom1D_f10h(const TS1_ *, TD1_ *, TD2_ *, TD3_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_sse2(const TS1_ *, TD1_ *, TD2_ *, TD3_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_refr(const TS1_ *,  TD1_ *,  TD2_ *, TD3_ *, U32 )    const { assert(false); }   
   IV REFR(                                  const TS1_ *,  TD1_ *,  TD2_ *, TD3_ *)        const { assert(false); }

};


template< typename TS1, CH cs1, 
          typename TS2, CH cs2, 
          typename TS3, CH cs3,
          typename TS4, CH cs4,
          typename TD1,  CH cd  >         
struct fe4S1D : public feBase
{
   typedef TS1 TS1_;
   typedef TS2 TS2_;
   typedef TS3 TS3_;
   typedef TS4 TS4_;
   typedef TD1  TD1_;      
   const static CH CS1_= cs1;
   const static CH CS2_= cs2;
   const static CH CS3_= cs3;
   const static CH CS4_= cs4;
   const static CH CD_ = cd;

   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TD1, CD_  ), 
                               M_MIN( PIX_COUNT( TS1,CS1_ ),
                               M_MIN( PIX_COUNT( TS2,CS2_ ),
                                M_MIN( PIX_COUNT( TS3,CS3_ ),
                                      PIX_COUNT( TS4,CS4_ )))));

   class DST:  public RegDesc< TD1,  CD_ , nPIX_SSE > {};         
   class SRC1: public RegDesc< TS1, CS1_, nPIX_SSE > {};   
   class SRC2: public RegDesc< TS2, CS2_, nPIX_SSE > {};   
   class SRC3: public RegDesc< TS3, CS3_, nPIX_SSE > {};
   class SRC4: public RegDesc< TS4, CS4_, nPIX_SSE > {};
   
   template<IsAlign ia>  IV Custom1D_f10h(const TS1_ *, const TS2_ *, const TS3_ *, const TS4_ *, TD1 *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_sse2(const TS1_ *, const TS2_ *, const TS3_ *, const TS4_ *, TD1 *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_refr(const TS1_ *, const TS2_ *, const TS3_ *, const TS4_ *, TD1 *, U32 )    const { assert(false); }   
   IV REFR(                                  const TS1_ *, const TS2_ *, const TS3_ *, const TS4_ *, TD1_ *)        const { assert(false); }

};


//This is a special case, with 1 source and 4 destination
template< typename TS1, CH cs1, 
          typename TD1, CH cd1, 
          typename TD2, CH cd2,
          typename TD3,  CH cd3,        
          typename TD4,  CH cd4  >    
struct fe1S4D : public feBase
{
   typedef TS1 TS1_;
   typedef TD1 TD1_;
   typedef TD2 TD2_;
   typedef TD3  TD3_;      
   typedef TD4  TD4_;   
   const static CH CS1_= cs1;
   const static CH CD1_= cd1;
   const static CH CD2_= cd2;
   const static CH CD3_ = cd3;
   const static CH CD4_ = cd4;

   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TD1, CD1_  ), 
                               M_MIN( PIX_COUNT( TD2,CD2_ ),
                               M_MIN( PIX_COUNT( TD3,CD3_ ),
                               M_MIN( PIX_COUNT( TD4,CD4_ ),
                                      PIX_COUNT( TS1,CS1_ )))));

   class DST1:  public RegDesc< TD1,CD1_ , nPIX_SSE > {};         
   class DST2: public RegDesc< TD2, CD2_, nPIX_SSE > {};   
   class DST3: public RegDesc< TD3, CD3_, nPIX_SSE > {};   
   class DST4: public RegDesc< TD4, CD4_, nPIX_SSE > {};   
   class SRC1: public RegDesc< TS1, CS1_, nPIX_SSE > {};
   
   template<IsAlign ia> IV Custom1D_f10h(const TS1_ *, TD1_ *,  TD2_ *, TD3_ *,TD4_ *, U32 )  const { assert(false); }
   template<IsAlign ia> IV Custom1D_sse2(const TS1_ *, TD1_ *,  TD2_ *, TD3_ *,TD4_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_refr(const TS1_ *,  TD1_ *,  TD2_ *, TD3_ *,TD4_ *, U32 )    const { assert(false); }   
   IV REFR(                                  const TS1_ *,  TD1_ *,  TD2_ *, TD3_ *,TD4_ *)        const { assert(false); }

};

//This is a special case, with 3 source and 3 destination
template<    typename TS1, CH cs1, 
            typename TS2, CH cs2, 
            typename TS3, CH cs3, 
            typename TD1, CH cd1,
            typename TD2, CH cd2,
            typename TD3,  CH cd3 >         
struct fe3S3D : public feBase
{
   typedef TS1 TS1_;
   typedef TS2 TS2_;
   typedef TS3 TS3_;
   typedef TD1 TD1_;
   typedef TD2 TD2_;
   typedef TD3  TD3_;      
   
    const static CH CS1_= cs1;
    const static CH CS2_= cs2;
    const static CH CS3_= cs3;
    const static CH CD1_= cd1;
    const static CH CD2_= cd2;
    const static CH CD3_ = cd3;

   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TD1, CD1_  ), 
                               M_MIN( PIX_COUNT( TD2,CD2_ ),
                               M_MIN( PIX_COUNT( TD3,CD3_ ),
                               M_MIN( PIX_COUNT( TS1,CS1_ ),
                                M_MIN( PIX_COUNT( TS2,CS2_ ),
                                      PIX_COUNT( TS3,CS3_ ))))));

   class DST1:  public RegDesc< TD1,CD1_ , nPIX_SSE > {};         
   class DST2: public RegDesc< TD2, CD2_, nPIX_SSE > {};   
   class DST3: public RegDesc< TD3, CD3_, nPIX_SSE > {};   

   class SRC1: public RegDesc< TS1, CS1_, nPIX_SSE > {};
   class SRC2: public RegDesc< TS2, CS2_, nPIX_SSE > {};
   class SRC3: public RegDesc< TS3, CS3_, nPIX_SSE > {};
   
   template<IsAlign ia>  IV Custom1D_f10h(const TS1_ *,const TS2_ *,const TS3_ *, TD1_ *, TD2_ *, TD3_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_sse2(const TS1_ *,const TS2_ *,const TS3_ *, TD1_ *, TD2_ *, TD3_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_refr(const TS1_ *,const TS2_ *,const TS3_ *,   TD1_ *,  TD2_ *, TD3_ *, U32 )    const { assert(false); }   
   IV REFR(                                  const TS1_ *,const TS2_ *,const TS3_ *,   TD1_ *,  TD2_ *, TD3_ *)        const { assert(false); }

};
//This is a special case, with 4 source and 4 destination
template<    typename TS1, CH cs1, 
            typename TS2, CH cs2, 
            typename TS3, CH cs3, 
            typename TS4, CH cs4, 
            typename TD1, CH cd1,
            typename TD2, CH cd2,
            typename TD3,  CH cd3,
            typename TD4,  CH cd4>      
struct fe4S4D : public feBase
{
   typedef TS1 TS1_;
   typedef TS2 TS2_;
   typedef TS3 TS3_;
   typedef TS4 TS4_;
   typedef TD1 TD1_;
   typedef TD2 TD2_;
   typedef TD3  TD3_;      
   typedef TD4  TD4_;      

    const static CH CS1_= cs1;
    const static CH CS2_= cs2;
    const static CH CS3_= cs3;
    const static CH CS4_= cs4;
    const static CH CD1_= cd1;
    const static CH CD2_= cd2;
    const static CH CD3_ = cd3;
    const static CH CD4_ = cd4;

   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TD1, CD1_  ), 
                               M_MIN( PIX_COUNT( TD2,CD2_ ),
                               M_MIN( PIX_COUNT( TD3,CD3_ ),
                               M_MIN( PIX_COUNT( TD4,CD4_ ),
                               M_MIN( PIX_COUNT( TS1,CS1_ ),
                                M_MIN( PIX_COUNT( TS2,CS2_ ),
                                M_MIN( PIX_COUNT( TS3,CS3_ ),
                                      PIX_COUNT( TS4,CS4_ ))))))));

   class DST1:  public RegDesc< TD1,CD1_ , nPIX_SSE > {};         
   class DST2: public RegDesc< TD2, CD2_, nPIX_SSE > {};   
   class DST3: public RegDesc< TD3, CD3_, nPIX_SSE > {};
   class DST4: public RegDesc< TD4, CD4_, nPIX_SSE > {};

   class SRC1: public RegDesc< TS1, CS1_, nPIX_SSE > {};
   class SRC2: public RegDesc< TS2, CS2_, nPIX_SSE > {};
   class SRC3: public RegDesc< TS3, CS3_, nPIX_SSE > {};
   class SRC4: public RegDesc< TS4, CS4_, nPIX_SSE > {};
   
   template<IsAlign ia>  IV Custom1D_f10h(const TS1_ *,const TS2_ *,const TS3_ *,const TS4_ *, TD1_ *, TD2_ *, TD3_ *,TD4_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_sse2(const TS1_ *,const TS2_ *,const TS3_ *,const TS4_ *, TD1_ *, TD2_ *, TD3_ *,TD4_ *, U32 )  const { assert(false); }
   template<IsAlign ia>  IV Custom1D_refr(const TS1_ *,const TS2_ *,const TS3_ *,const TS4_ *,   TD1_ *,  TD2_ *, TD3_ *,TD4_ *, U32 )    const { assert(false); }   
   IV REFR(                                  const TS1_ *,const TS2_ *,const TS3_ *,const TS4_ *,   TD1_ *,  TD2_ *, TD3_ *,TD4_ *)        const { assert(false); }

};

//_96 suffix is used for cases where 96 bits of data loaded per iteration for C3 case
//Partial registers are loaded to accomodate complete pixel data
//For all datatypes, loading 128 bits of C3 data results in loading partial pixel in the MSB
//The other alternate to 96 bit load is to load 3 complete 128 bit registers
//For performance reasons 96 bit load is preferred over 3 registers full of data for some cases
template< typename TS1, CH cs1, 
typename TS2, CH cs2, 
typename TD1,  CH cd  >         
struct fe3_96 : public fe3<TS1, cs1, TS2, cs2, TD1, cd>
{   
    typedef TS1 TS1_;
    typedef TS2 TS2_;
    typedef TD1  TD1_;   

    const static CH CS1_= cs1;
    const static CH CS2_= cs2;
    const static CH CD_ = cd;   

    const static U32 nPIX_SSE = PIX_C3_96( TS1 );   
    class DST:  public RegDesc< TD1_,  CD_,  nPIX_SSE > {};  
    class SRC1: public RegDesc< TS1_, CS1_, nPIX_SSE > {};   
    class SRC2: public RegDesc< TS2_, CS2_, nPIX_SSE > {};
};



template<    typename TS1, CH cs1, 
typename TD1,  CH cd  >  
struct fe2_96 : public fe2<TS1, cs1, TD1, cd>
{   
    typedef TS1 TS1_;
    typedef TD1  TD1_;      
    const static CH CS1_= cs1;
    const static CH CD_ = cd;

    const static U32 nPIX_SSE = PIX_C3_96( TS1 ); 

    class DST:  public RegDesc< TD1_,  CD_,  nPIX_SSE > {};         
    class SRC1: public RegDesc< TS1_, CS1_, nPIX_SSE > {};   
};

template< typename TD1,  CH cd  >  
struct fe1_96 : public fe1<TD1, cd>
{   
    typedef TD1  TD1_;      
    const static CH CD_ = cd;

    const static U32 nPIX_SSE = PIX_C3_96( TD1 ); 

    class DST:  public RegDesc< TD1_,  CD_,  nPIX_SSE > {};         
};


namespace PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)
{
   // =========================================================
   //                  LOAD / STORE
   // =========================================================



   // =========================================================
   //            ForEach 
   // =========================================================

    template< class Bx, IsAlign ia >
    struct FeThread
    {
        typedef struct Bx::ThreadData    ThData;

        SV fe_1Dt( void *p )
        {
            ThData *pThData = (ThData *)p;
            Bx::template fe_1D<ia>( *pThData );
        }

        SV fe_1DT( ThData* thData )
        {
            for(U32 i=0; i<(thData->pThis->Threads() - 1); ++i)
               ThreadPool::Run( fe_1Dt, (thData + i) );
            fe_1Dt(thData + (thData->pThis->Threads() -1));
            ThreadPool::Wait();
        }

        SV fe_2Dt( void *p )
        {
            ThData *pThData = (ThData *)p;
            Bx::template fe_2D<ia>( *pThData );
        }

        SV fe_2DT( ThData* thData )
        {
            for(U32 i=0; i<(thData->pThis->Threads() - 1); ++i)
               ThreadPool::Run( fe_2Dt, (thData + i) );
            fe_2Dt(thData + (thData->pThis->Threads() -1));
            ThreadPool::Wait();   
        }
    };

    // Common data / initialization routines
   template< class FE >                                                        
   class B0
   {
   protected:
      // Algorithm instance
      mutable FE &fe;                                                                 
      const U32 width;   
      const S32 s1Step, s2Step, s3Step,s4Step; 
      const S32 d1Step,d2Step,d3Step,d4Step;
      U32 threads;   
  
      SYS_INLINE
      B0( FE & fe_, ASZ roi_, U32 bytesPerThread_, U32 s1Stp, U32 s2Stp, U32 s3Stp,U32 s4Stp,
                                                    U32 d1Stp, U32 d2Stp, U32 d3Stp,U32 d4Stp)
       :fe     ( fe_        )        
       ,width  ( roi_.width )
       ,d1Step  ( d1Stp     )
       ,d2Step    ( d2Stp        )
       ,d3Step    ( d3Stp        )
       ,d4Step    ( d4Stp        )
       ,s1Step ( s1Stp      )
       ,s2Step ( s2Stp      )
       ,s3Step ( s3Stp      )
       ,s4Step ( s4Stp      )
       ,threads( RecommendThreads( d1Step*roi_.height, bytesPerThread_ ) )
      {}

      SYS_INLINE
      B0( FE & fe_, int len, U32 bytesPerThread_ )
       :fe     ( fe_ )        
       ,width  ( len )
       ,d1Step  ( 1     )
       ,d2Step    ( 1        )
       ,d3Step    ( 1        )
       ,d4Step    ( 1        )
       ,s1Step ( 1      )
       ,s2Step ( 1      )
       ,s3Step ( 1      )
       ,s4Step ( 1      )
       ,threads( RecommendThreads( len*sizeof(typename FE::TD1_), bytesPerThread_ ) )
      {}

        ISV GetDispatch( DispatchType &dt )
        {
#if   defined(OPT_F10H) || defined(OPT_SSE3)
            if      ((FE::f10hPft != PFT_ABSENT) && (FE::f10hPft != PFT_EMULATE_CUSTOM)&& (FE::f10hPft != PFT_EMULATE_CUSTOM_NO_FB)) dt = DT_F10H;
            else if ((FE::sse2Pft != PFT_ABSENT) && (FE::sse2Pft != PFT_EMULATE_CUSTOM)) dt = DT_SSE2; 
            else if ((FE::refrPft != PFT_ABSENT) && (FE::refrPft != PFT_EMULATE_CUSTOM)) dt = DT_REFR; 
            else                                                                       { assert(false); 
                                                                                         dt = DT_REFR; }
#elif defined(OPT_SSE2)
            if      ((FE::sse2Pft != PFT_ABSENT) && (FE::sse2Pft != PFT_EMULATE_CUSTOM)) dt = DT_SSE2; 
            else if ((FE::refrPft != PFT_ABSENT) && (FE::refrPft != PFT_EMULATE_CUSTOM)) dt = DT_REFR; 
            else                                                                       { assert(false); 
                                                                                         dt = DT_REFR; }
#elif defined(OPT_REFR)
            if      ((FE::refrPft != PFT_ABSENT) && (FE::refrPft != PFT_EMULATE_CUSTOM)) dt = DT_REFR; 
            else                                                                       { assert(false); 
                                                                                         dt = DT_REFR; }
#else
                                                                                         assert(false);
                                                                                         dt = DT_REFR; 
#endif
        }


        //Returns the pixel funtion type, normal, stream or custom type
        IS PxFnType GetPft()
        {
#if   defined(OPT_F10H) || defined (OPT_SSE3)
            return FE::f10hPft;
#elif defined(OPT_SSE2)
            return FE::sse2Pft;
#elif defined(OPT_REFR)
            return FE::refrPft;
#else
            assert(false); 
            return FE::refrPft;
#endif
        }


      //Call the appropriate FeThread routine
      //ALG_1D - 1D for signals
      //ALG_2D - 2D for images
      template< class Bn, IsAlign ia >
      SYS_INLINE void BnRunAlg( typename Bn::ThreadData *td )
      {
          switch( Bn::alg_ )
          {
          case ALG_1D:    FeThread< Bn, ia >::fe_1DT( td ); break;
          case ALG_2D:    FeThread< Bn, ia >::fe_2DT( td ); break;
          default:        assert(false);
          }
      }

      template< class Bn, IsAlign ia >
      SYS_INLINE void BnRun( typename Bn::ThreadData *td )
      {
          FeInit();
          BnRunAlg< Bn, ia >( td );
      }

      //Calling appropriate initialization function. This initialization routine contains code for any one
      //time computations or to initialize variables which might be required to operate on the input buffers
      SYS_INLINE void FeInit( ) const
      {

// Depending on current dispatch path, run the correct init function
#if   defined (OPT_F10H) || defined (OPT_SSE3)
          fe.F10H_Init();
#elif defined (OPT_SSE2)
          fe.SSE2_Init();
#endif
      }

   public:
       SYS_INLINE U32 Threads() const { return threads; }
   };

   // -------- ForEach 1 Buffer --------
   template< class FE, FE_ALGORITHM alg >
   class B1 : public B0< FE >
   {
      friend class B0< FE>;
      const static FE_ALGORITHM alg_ = alg;
      typedef B1<FE,alg>        This;
      typedef typename FE::TD1_  TD1;
              
   public:
      struct ThreadData{                                                       // Dynamic thread data
         const This *pThis;
               TD1   *pDst;
         U32 size;
         U32 widthTh;
         U32 px_nRef;
       };   
      //fe_1D_Ref walks through the buffer and calling reference code on per pixel basis
      ISV fe_1D_Ref( const FE &fe, TD1 *pDst, U32 count )
      {
          const TD1 * end = pDst + (count * CH_COUNT(FE::CD_));
          for( ;pDst<end; pDst += CH_COUNT(FE::CD_) )
              fe.REFR( pDst );
      }

      //fe_1D_Custom calls the appropriate Custom_1D routine based on the dispatch type
      template< IsAlign ia, bool fallBack >
      ISV fe_1D_Custom( const ThreadData &t )
      {
          U32 count;
          if( fallBack )
              count = t.widthTh;
          else
              count = t.widthTh + t.px_nRef;

          DispatchType dt; B0<FE>::GetDispatch(dt);
          switch( dt )
          {
          case DT_F10H:
          case DT_SSE3:     t.pThis->fe.template Custom1D_f10h<ia>(t.pDst, count );
                            break;
          case DT_SSE2:     t.pThis->fe.template Custom1D_sse2<ia>(t.pDst, count );
                            break;
          case DT_REFR:     t.pThis->fe.template Custom1D_refr<ia>(t.pDst, count );
                            break;
          default:          assert(false);
          }

          if( fallBack )
              if( t.px_nRef )
                  fe_1D_Ref( t.pThis->fe, t.pDst + (t.widthTh) * CH_COUNT(FE::CD_), t.px_nRef );
      }
     
      //fe_1D_Normal walks through the buffer, In each iteration, loading "BYTES" no of bytes from source and 
      //destination(In this particular case only destination) and calling the optimized routine, 
      //either F10H or SSE2 path based on the dispatch type, store the resulting data in the 
      //destination register back to the destination buffer, increment the buffers and continue 
      //with the next iteration until the end of the buffer
      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
        fe_1D_Ref( t.pThis->fe, t.pDst, t.widthTh + t.px_nRef );
        return;
#else
        TD1 *pDst = t.pDst;
        FE fec = t.pThis->fe;

        for( TD1 * end = pDst + (t.widthTh) * CH_COUNT(FE::CD_);
            pDst <end;)
        {
            RegFile r;                                                         
            fec.template LOAD< FE::DST::BYTES,DISPATCH,ia,is >( r.dst, pDst );          
#if   defined(OPT_F10H) || defined(OPT_SSE3)
            fec.F10H( r );        
#elif   defined(OPT_SSE2)
            fec.SSE2( r );        
#else
            assert(false);  
#endif
            fec.template STORE< FE::DST::BYTES,DISPATCH,ia,is >( r.dst, pDst );    
            fec.IncrementBuffers(&pDst,FE::DST::ELEMENTS);
        }    
        t.pThis->fe = fec;

        if( t.px_nRef )
        fe_1D_Ref( t.pThis->fe, pDst, t.px_nRef );
#endif
      }

      //fe_1D_Pft checks for the funtion type to be called and call the appropriate one
      template< IsAlign ia >
      ISV fe_1D_Pft( const ThreadData & t )
      {
          switch( B0<FE>::GetPft() )
          {
          case PFT_NORMAL:                                fe_1D_Normal    < ia, STREAM_FLSE    >( t );            
                                                        break;
          case PFT_NORMAL_STREAM:                        fe_1D_Normal    < ia, STREAM_TRUE    >( t );            
                                                        break;
          case PFT_EMULATE_CUSTOM:
          case PFT_CUSTOM:                                fe_1D_Custom    < ia, true            >( t );            
                                                        break;
          case PFT_EMULATE_CUSTOM_NO_FB:
          case PFT_CUSTOM_NO_FALLBACK:                    fe_1D_Custom    < ia, false            >( t );            
                                                        break;
          default:                    assert(false);
          }
      }

      template< IsAlign ia >
      SV fe_1D( const ThreadData & t )
      {
         fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
          for( TD1 *end = FW_REF::Offset( t.pDst, (T_OFFSET)(t.size) * (t.pThis->d1Step) );
              t.pDst!=end; 
              FW_REF::Inc(t.pDst, t.pThis->d1Step) 
            )
            fe_1D_Pft<ia>( t );
      }

   //This routine splits the 1D buffers to be passed to each thread
   //1D splitting is used for Signal routines
   private:
      IV SplitData_1D( ThreadData* td, TD1 * pDst, int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // len per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this; 
            td[i].pDst    = FW_REF::Offset(pDst, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD1)*(T_OFFSET)CH_COUNT(FE::CD_)*(T_OFFSET)(lenTh));
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }
         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      }

     //This routine splits the 1D buffers to be passed to each thread
     //2D splitting is used for Image routines
      IV SplitData_2D( ThreadData* td, TD1 * pDst, int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this; 
            td[i].pDst    = FW_REF::Offset(pDst, (T_OFFSET)(i)*this->d1Step*(T_OFFSET)(heightTh));
            td[i].size    = heightTh;                                                              // 2D size=height
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size    = height - ((this->threads)-1)*heightTh;                    // Last thread
      }

      //This is the main routine operating on the entire buffer which calls the SplitData and checks for
      //alignment of the buffers and the resulting ThreadData from the split and alignment information 
      //is passed on to the BnRun routine in the base class
      IV Run( TD1 *pDst, int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData) );
         bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pDst,0            ); 
                        break;
         case ALG_2D:    SplitData_2D( td, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pDst,this->d1Step    ); 
                        break;
         default:        assert(false);
         }

         isAligned    ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );
      }

   public:  
      
      B1( FE & fe, TD1 *pDst, int len, U32 bytesPerThread )  //1D
       :B0< FE >   ( fe, len, bytesPerThread )        
      {  
         STATIC_ASSERT( ALG_1D == alg );
         Run( pDst, len );             
      }

      
      B1( FE & fe, TD1 *pDst, int d1Step, ASZ roi, U32 bytesPerThread )
       :B0< FE >( fe, roi, bytesPerThread,0,0,0,0,d1Step,0,0,0)  
      {  
         STATIC_ASSERT( ALG_2D == alg );
         Run( pDst, roi.height );       
      }           
   };

   // -------- ForEach 2 Buffers --------
   template< class FE, FE_ALGORITHM alg >
   class B2 : public B0< FE >
   {    
      friend class B0<FE>;
      const static FE_ALGORITHM alg_ = alg;
      typedef B2<FE,alg>        This;
      typedef typename FE::TS1_ TS1;
      typedef typename FE::TD1_  TD1;

   public:
      struct ThreadData{                                                       // Dynamic thread data 
         const This *pThis;
         const TS1  *pSrc1;
               TD1   *pDst; 
         U32 size;
         U32 widthTh;
         U32 px_nRef;
       }; 

      ISV fe_1D_Ref( const FE &fe, const TS1 *pSrc1, TD1 *pDst, U32 count )
      {
          const TD1 * end = pDst + (count * CH_COUNT(FE::CD_));
          for( ;pDst<end; pDst  += CH_COUNT(FE::CD_),
                       pSrc1 += CH_COUNT(FE::CS1_) )
               fe.REFR( pSrc1, pDst );
      }

      template< IsAlign ia, bool fallBack >
      ISV fe_1D_Custom( const ThreadData &t )
      {
          U32 count;
          if( fallBack )
              count = t.widthTh;
          else
              count = t.widthTh + t.px_nRef;

          DispatchType dt; B0<FE>::GetDispatch(dt);
          switch( dt )
          {
          case DT_F10H:
          case DT_SSE3:     t.pThis->fe.template Custom1D_f10h<ia>(t.pSrc1, t.pDst, count ); 
                            break;
          case DT_SSE2:     t.pThis->fe.template Custom1D_sse2<ia>(t.pSrc1, t.pDst, count ); 
                            break;
          case DT_REFR:     t.pThis->fe.template Custom1D_refr<ia>(t.pSrc1, t.pDst, count ); 
                            break;
          default:          assert(false);
          }

          if( fallBack )
              if( t.px_nRef )
                  fe_1D_Ref( t.pThis->fe,    t.pSrc1 + (t.widthTh) * CH_COUNT(FE::CS1_),
                                            t.pDst  + (t.widthTh) * CH_COUNT(FE::CD_), t.px_nRef );
      }

      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
        fe_1D_Ref( t.pThis->fe, t.pSrc1, t.pDst, t.widthTh + t.px_nRef );
        return;
#else

         const TS1    *pSrc1 = t.pSrc1;
         TD1            *pDst  = t.pDst;

            FE fec = t.pThis->fe;

        for( TD1 * end = pDst + (t.widthTh) * CH_COUNT(FE::CD_);
            pDst <end;)
        {
            RegFile r;                                                         
            fec.template LOAD<FE::DST::BYTES,DISPATCH,ia,is >( r.dst , pDst  ); 
            fec.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, pSrc1 );        
#if   defined(OPT_F10H) || defined(OPT_SSE3)
            fec.F10H( r );        
#elif   defined(OPT_SSE2)
            fec.SSE2( r );        
#else
            assert(false);  
#endif
            fec.template STORE< FE::DST::BYTES,DISPATCH,ia,is >( r.dst, pDst );
            fec.IncrementBuffers(&pSrc1,FE::SRC1::ELEMENTS,&pDst,FE::DST::ELEMENTS );
        }
 

        t.pThis->fe = fec;
         if( t.px_nRef )
            fe_1D_Ref( t.pThis->fe, pSrc1, pDst, t.px_nRef );
#endif
      }

      template< IsAlign ia > 
      ISV fe_1D_Pft( const ThreadData & t )
      {
          switch( B0<FE>::GetPft() )
          {
          case PFT_NORMAL:                                fe_1D_Normal    < ia, STREAM_FLSE    >( t );                
                                                        break;
          case PFT_NORMAL_STREAM:                        fe_1D_Normal    < ia, STREAM_TRUE    >( t );                
                                                        break;
          case PFT_EMULATE_CUSTOM:
          case PFT_CUSTOM:                                fe_1D_Custom    < ia, true            >( t );                
                                                        break;
          case PFT_EMULATE_CUSTOM_NO_FB:
          case PFT_CUSTOM_NO_FALLBACK:                    fe_1D_Custom    < ia, false            >( t );                
                                                        break;
          default:                    assert(false);
          }
      }

      template< IsAlign ia > 
      SV fe_1D( const ThreadData & t )
      {
          fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
         for( TD1 *end = FW_REF::Offset(t.pDst,(T_OFFSET)(t.size)*(t.pThis->d1Step));
              t.pDst!=end; 
              FW_REF::Inc(t.pSrc1,t.pThis->s1Step,t.pDst,t.pThis->d1Step) 
            ) 
            fe_1D_Pft<ia>( t );  
      }

   private:
      IV SplitData_1D( ThreadData *td, const TS1 *pSrc1, TD1 * pDst, int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this; 
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS1) *(T_OFFSET)CH_COUNT(FE::CS1_)*(T_OFFSET)(lenTh));
            td[i].pDst    = FW_REF::Offset(pDst , (T_OFFSET)(i)*(T_OFFSET)sizeof(TD1) *(T_OFFSET)CH_COUNT(FE::CD_) *(T_OFFSET)(lenTh));
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }

         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      } 

      IV SplitData_2D( ThreadData *td, const TS1 *pSrc1, TD1 * pDst, int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this; 
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(this->s1Step) *(T_OFFSET)(heightTh));
             td[i].pDst    = FW_REF::Offset(pDst,  (T_OFFSET)(i)*(this->d1Step) *(T_OFFSET)(heightTh));
            td[i].size    = heightTh;
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
      } 

      IV Run( const TS1* pSrc1, TD1 *pDst, int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));
            bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, pSrc1, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,0,                pDst,0                ); 
                        break;
         case ALG_2D:    SplitData_2D( td, pSrc1, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,this->s1Step,    pDst,this->d1Step    ); 
                        break;
         default:        assert(false);
         }

         isAligned    ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );
      }

   public:  
      
      B2( FE & fe, const TS1* pSrc1, TD1 *pDst, int len, U32 bytesPerThread )  
       :B0< FE >( fe, len, bytesPerThread )        
      {  
         STATIC_ASSERT( ALG_1D == alg );
         Run( pSrc1, pDst, len );             
      }  

      
      B2( FE & fe, const TS1* pSrc1, int s1Step, 
                         TD1 *pDst, int d1Step, ASZ roi, U32 bytesPerThread )
       :B0< FE >( fe, roi, bytesPerThread,s1Step,0,0,0,d1Step,0,0,0 )         
      {   
         STATIC_ASSERT( ALG_2D == alg );
         Run( pSrc1, pDst, roi.height );     
      }       
   };

   // -------- ForEach 3 Buffers --------
   template< class FE, FE_ALGORITHM alg >
   class B3 : public B0< FE >
   {     
      friend class B0<FE>;
      const static FE_ALGORITHM alg_ = alg;
      typedef B3<FE,alg>        This;
      typedef typename FE::TS1_ TS1;
      typedef typename FE::TS2_ TS2;
      typedef typename FE::TD1_  TD1;
        
   public:
      struct ThreadData{                                                       // Dynamic thread data 
         const This *pThis;
         const TS1  *pSrc1;
         const TS2  *pSrc2;
               TD1   *pDst; 
         U32 size;
         U32 widthTh;
         U32 px_nRef;
       };

      ISV fe_1D_Ref( const FE &fe, const TS1 *pSrc1, const TS2 *pSrc2, TD1 *pDst, U32 count )
      {
          const TD1 * end = pDst + (count * CH_COUNT(FE::CD_));
          for( ; pDst<end; pDst  += CH_COUNT(FE::CD_ ), 
                        pSrc1 += CH_COUNT(FE::CS1_),
                        pSrc2 += CH_COUNT(FE::CS2_))
                fe.REFR( pSrc1, pSrc2, pDst );
      }

      template< IsAlign ia, bool fallBack >
      ISV fe_1D_Custom( const ThreadData &t )
      {
          U32 count;
          if( fallBack )
              count = t.widthTh;
          else
              count = t.widthTh + t.px_nRef;

          DispatchType dt; B0<FE>::GetDispatch(dt);
          switch( dt )
          {
          case DT_F10H:
          case DT_SSE3:     t.pThis->fe.template Custom1D_f10h<ia>(t.pSrc1, t.pSrc2, t.pDst, count ); 
                            break;
          case DT_SSE2:     t.pThis->fe.template Custom1D_sse2<ia>(t.pSrc1, t.pSrc2, t.pDst, count ); 
                            break;
          case DT_REFR:     t.pThis->fe.template Custom1D_refr<ia>(t.pSrc1, t.pSrc2, t.pDst, count ); 
                            break;
          default:          assert(false);
          }
          
          if( fallBack )
              if( t.px_nRef )
                  fe_1D_Ref( t.pThis->fe,    t.pSrc1 + (t.widthTh) * CH_COUNT(FE::CS1_),
                                            t.pSrc2 + (t.widthTh) * CH_COUNT(FE::CS2_),
                                            t.pDst  + (t.widthTh) * CH_COUNT(FE::CD_), t.px_nRef );
      }

      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
        fe_1D_Ref( t.pThis->fe, t.pSrc1, t.pSrc2, t.pDst, t.widthTh + t.px_nRef );
        return;
#else

         const TS1    *pSrc1 = t.pSrc1;
         const TS2    *pSrc2 = t.pSrc2;
         TD1            *pDst  = t.pDst;
        FE fec = t.pThis->fe;

        for( typename FE::TD1_ * end = pDst + (t.widthTh) * CH_COUNT(FE::CD_);
            pDst <end;)
        {
            RegFile r;                                                         
            fec.template LOAD< FE::DST::BYTES,DISPATCH,ia,is >( r.dst , pDst  ); 
            fec.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, pSrc1 ); 
            fec.template LOAD< FE::SRC2::BYTES,DISPATCH,ia,is >( r.src2, pSrc2 );      
#if   defined(OPT_F10H) || defined(OPT_SSE3)
            fec.F10H( r );        
#elif   defined(OPT_SSE2)
            fec.SSE2( r );        
#else
            assert(false);  
#endif
            fec.template STORE< FE::DST::BYTES,DISPATCH,ia,is >( r.dst, pDst );
            fec.IncrementBuffers(&pSrc1,FE::SRC1::ELEMENTS,&pSrc2,FE::SRC2::ELEMENTS,&pDst,FE::DST::ELEMENTS);
        }

        t.pThis->fe = fec;

         if( t.px_nRef )
            fe_1D_Ref( t.pThis->fe, pSrc1, pSrc2, pDst, t.px_nRef );
#endif
      }

      template< IsAlign ia > 
      ISV fe_1D_Pft( const ThreadData & t )
      {
          switch( B0<FE>::GetPft() )
          {
          case PFT_NORMAL:                                fe_1D_Normal    < ia, STREAM_FLSE    >( t );                        
                                                        break;
          case PFT_NORMAL_STREAM:                        fe_1D_Normal    < ia, STREAM_TRUE    >( t );                        
                                                        break;
          case PFT_EMULATE_CUSTOM:
          case PFT_CUSTOM:                                fe_1D_Custom    < ia, true            >( t );                        
                                                        break;
          case PFT_EMULATE_CUSTOM_NO_FB:
          case PFT_CUSTOM_NO_FALLBACK:                    fe_1D_Custom    < ia, false            >( t );                        
                                                        break;
          default:                    assert(false);
          }
      }

      template< IsAlign ia > 
      SV fe_1D( const ThreadData & t )
      {
          fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
         for( TD1 *end = FW_REF::Offset(t.pDst,(T_OFFSET)(t.size)*(t.pThis->d1Step));
              t.pDst!=end; 
              FW_REF::Inc(t.pSrc1,t.pThis->s1Step, t.pSrc2,t.pThis->s2Step, t.pDst,t.pThis->d1Step) 
            )
            fe_1D_Pft<ia>( t );
      }        

   private:
      IV SplitData_1D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, TD1 * pDst, int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS1) *(T_OFFSET)CH_COUNT(FE::CS1_)*(T_OFFSET)(lenTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS2) *(T_OFFSET)CH_COUNT(FE::CS2_)*(T_OFFSET)(lenTh));
            td[i].pDst    = FW_REF::Offset(pDst,  (T_OFFSET)(i)*(T_OFFSET)sizeof(TD1) *(T_OFFSET)CH_COUNT(FE::CD_) *(T_OFFSET)(lenTh));
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }
         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      } 

      IV SplitData_2D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, TD1 * pDst, int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(this->s1Step)*(T_OFFSET)(heightTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)*(this->s2Step)*(T_OFFSET)(heightTh));
            td[i].pDst    = FW_REF::Offset(pDst,  (T_OFFSET)(i)*(this->d1Step)*(T_OFFSET)(heightTh));
            td[i].size    = heightTh;
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
      }   

      IV Run( const TS1* pSrc1, const TS2* pSrc2, TD1 *pDst, int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));           
         bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, pSrc1, pSrc2, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,0,                pSrc2,0,                pDst,0                ); 
                        break;
         case ALG_2D:    SplitData_2D( td, pSrc1, pSrc2, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,this->s1Step,    pSrc2,this->s2Step,    pDst,this->d1Step    ); 
                        break;
         default:        assert(false);
         }

         isAligned    ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );
      }

   public:  
      
      B3( FE & fe, const TS1* pSrc1, 
                   const TS2* pSrc2, 
                         TD1 * pDst, int len, U32 bytesPerThread )
       :B0< FE >( fe, len, bytesPerThread )
      {   
         STATIC_ASSERT( ALG_1D == alg );
         Run( pSrc1, pSrc2, pDst, len );         
      }

      
      B3( FE & fe, const TS1* pSrc1, int s1Step, 
                   const TS2* pSrc2, int s2Step, 
                         TD1 * pDst,  int d1Step, ASZ roi, U32 bytesPerThread )
       :B0< FE >( fe, roi, bytesPerThread,s1Step, s2Step,0,0,d1Step,0,0,0)
      {   
         STATIC_ASSERT( ALG_2D == alg );
         Run( pSrc1, pSrc2, pDst, roi.height );         
      }        
   };

   // -------- ForEach 4 Buffers --------
   template< class FE, FE_ALGORITHM alg >
   class B4 : public B0< FE >
   {     
      friend class B0<FE>;
      const static FE_ALGORITHM alg_ = alg;
      typedef B4<FE,alg>        This;
      typedef typename FE::TS1_ TS1;
      typedef typename FE::TS2_ TS2;
      typedef typename FE::TS3_ TS3;
      typedef typename FE::TD1_  TD1;
          
   public:
      struct ThreadData{                                                       // Dynamic thread data
         const This *pThis;
         const TS1  *pSrc1;
         const TS2  *pSrc2;
         const TS3  *pSrc3;
               TD1   *pDst; 
         U32 size;
         U32 widthTh;
         U32 px_nRef;
       };

      ISV fe_1D_Ref( const FE &fe, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3, TD1 *pDst, U32 count )
      {
          const TD1 * end = pDst + (count * CH_COUNT(FE::CD_));
          for( ; pDst<end; pDst  += CH_COUNT(FE::CD_ ), 
                        pSrc1 += CH_COUNT(FE::CS1_),
                        pSrc2 += CH_COUNT(FE::CS2_),
                        pSrc3 += CH_COUNT(FE::CS3_) )
                fe.REFR( pSrc1, pSrc2, pSrc3, pDst );
      }

      template< IsAlign ia, bool fallBack >
      ISV fe_1D_Custom( const ThreadData &t )
      {
          U32 count;
          if( fallBack )
              count = t.widthTh;
          else
              count = t.widthTh + t.px_nRef;

          DispatchType dt; B0<FE>::GetDispatch(dt);
          switch( dt )
          {
          case DT_F10H:
          case DT_SSE3:     t.pThis->fe.template Custom1D_f10h<ia>(t.pSrc1, t.pSrc2, t.pSrc3, t.pDst, count ); 
                            break;
          case DT_SSE2:     t.pThis->fe.template Custom1D_sse2<ia>(t.pSrc1, t.pSrc2, t.pSrc3, t.pDst, count ); 
                            break;
          case DT_REFR:     t.pThis->fe.template Custom1D_refr<ia>(t.pSrc1, t.pSrc2, t.pSrc3, t.pDst, count ); 
                            break;
          default:          assert(false);
          }

          if( fallBack )
              if( t.px_nRef )
                  fe_1D_Ref( t.pThis->fe,    t.pSrc1 + (t.widthTh) * CH_COUNT(FE::CS1_),
                                            t.pSrc2 + (t.widthTh) * CH_COUNT(FE::CS2_),
                                            t.pSrc3 + (t.widthTh) * CH_COUNT(FE::CS3_),
                                            t.pDst  + (t.widthTh) * CH_COUNT(FE::CD_), t.px_nRef );
      }

      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
        fe_1D_Ref( t.pThis->fe, t.pSrc1, t.pSrc2, t.pSrc3, t.pDst, t.widthTh + t.px_nRef );
        return;
#else

         const TS1    *pSrc1 = t.pSrc1;
         const TS2    *pSrc2 = t.pSrc2;
         const TS3    *pSrc3 = t.pSrc3;
         TD1            *pDst  = t.pDst;

          FE fec = t.pThis->fe;

        for( typename FE::TD1_ * end = pDst + (t.widthTh) * CH_COUNT(FE::CD_);
            pDst <end;)
        {
            RegFile r;                                                         
            fec.template LOAD< FE::DST::BYTES,DISPATCH,ia,is >( r.dst , pDst  ); 
            fec.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, pSrc1 ); 
            fec.template LOAD< FE::SRC2::BYTES,DISPATCH,ia,is >( r.src2, pSrc2 );
            fec.template LOAD< FE::SRC3::BYTES,DISPATCH,ia,is >( r.src3, pSrc3 );   
#if   defined(OPT_F10H) || defined(OPT_SSE3)
            fec.F10H( r );        
#elif   defined(OPT_SSE2)
            fec.SSE2( r );        
#else
            assert(false);  
#endif
            fec.template STORE< FE::DST::BYTES,DISPATCH,ia,is >( r.dst, pDst );
            fec.IncrementBuffers(&pSrc1,FE::SRC1::ELEMENTS,&pSrc2,FE::SRC2::ELEMENTS,&pSrc3,FE::SRC3::ELEMENTS,&pDst,FE::DST::ELEMENTS);
        }

        t.pThis->fe = fec;

         if( t.px_nRef )
            fe_1D_Ref( t.pThis->fe, pSrc1, pSrc2, pSrc3, pDst, t.px_nRef );
#endif
      }

      template< IsAlign ia > 
      ISV fe_1D_Pft( const ThreadData & t )
      {
          switch( B0<FE>::GetPft() )
          {
          case PFT_NORMAL:                                fe_1D_Normal    < ia, STREAM_FLSE    >( t );                            
                                                        break;
          case PFT_NORMAL_STREAM:                        fe_1D_Normal    < ia, STREAM_TRUE    >( t );                            
                                                        break;
          case PFT_EMULATE_CUSTOM:
          case PFT_CUSTOM:                                fe_1D_Custom    < ia, true            >( t );                            
                                                        break;
          case PFT_EMULATE_CUSTOM_NO_FB:
          case PFT_CUSTOM_NO_FALLBACK:                    fe_1D_Custom    < ia, false            >( t );                            
                                                        break;
          default:                    assert(false);
          }
      }

      template< IsAlign ia > 
      SV fe_1D( const ThreadData & t )
      {
          fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
         for( TD1 *end = FW_REF::Offset(t.pDst,(T_OFFSET)(t.size)*(t.pThis->d1Step));
              t.pDst!=end; 
              FW_REF::Inc(t.pSrc1,t.pThis->s1Step, t.pSrc2,t.pThis->s2Step, t.pSrc3,t.pThis->s3Step, t.pDst,t.pThis->d1Step) 
            ) 
            fe_1D_Pft<ia>( t );
      }  

   private:
      IV SplitData_1D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3, TD1 * pDst, int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS1) *(T_OFFSET)CH_COUNT(FE::CS1_)*(T_OFFSET)(lenTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS2) *(T_OFFSET)CH_COUNT(FE::CS2_)*(T_OFFSET)(lenTh));
            td[i].pSrc3   = FW_REF::Offset(pSrc3, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS3) *(T_OFFSET)CH_COUNT(FE::CS3_)*(T_OFFSET)(lenTh));
            td[i].pDst    = FW_REF::Offset(pDst , (T_OFFSET)(i)*(T_OFFSET)sizeof(TD1) *(T_OFFSET)CH_COUNT(FE::CD_) *(T_OFFSET)(lenTh));
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }
         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      }

      IV SplitData_2D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3, TD1 * pDst, int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(this->s1Step)*(T_OFFSET)(heightTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)*(this->s2Step)*(T_OFFSET)(heightTh));
            td[i].pSrc3   = FW_REF::Offset(pSrc3, (T_OFFSET)(i)*(this->s3Step)*(T_OFFSET)(heightTh));
            td[i].pDst    = FW_REF::Offset(pDst , (T_OFFSET)(i)*(this->d1Step)*(T_OFFSET)(heightTh));
            td[i].size    = heightTh;
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
      } 

      IV Run( const TS1* pSrc1, const TS2* pSrc2, const TS3 *pSrc3, TD1 *pDst, int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));             
         bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, pSrc1, pSrc2, pSrc3, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,0,            pSrc2,0,                pSrc3,0,                pDst,0                ); 
                        break;
         case ALG_2D:    SplitData_2D( td, pSrc1, pSrc2, pSrc3, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,this->s1Step, pSrc2,this->s2Step,    pSrc3,this->s3Step,    pDst,this->d1Step    ); 
                        break;
         default:        assert(false);
         }
         
         isAligned    ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );
      }  
      
   public:  
      
      B4( FE & fe, const TS1* pSrc1, const TS2* pSrc2, 
                   const TS3* pSrc3,        TD1 * pDst, int len, U32 bytesPerThread )
       :B0< FE >( fe, len, bytesPerThread  )         
      {  
         STATIC_ASSERT( ALG_1D == alg );
         Run( pSrc1, pSrc2, pSrc3, pDst, len );              
      }

      
      B4( FE & fe, const TS1* pSrc1,int s1Step, const TS2* pSrc2,int s2Step, 
                   const TS3* pSrc3,int s3Step,       TD1 * pDst, int d1Step, ASZ roi, U32 bytesPerThread )
       :B0< FE >( fe, roi, bytesPerThread,s1Step, s2Step, s3Step,0,d1Step,0,0,0)         
      {  
         STATIC_ASSERT( ALG_2D == alg );
         Run( pSrc1, pSrc2, pSrc3, pDst, roi.height );              
      }       
   };


//----- Four source buffers and 1 destination buffer------------
   template< class FE, FE_ALGORITHM alg >
   class B4S1D : public B0< FE >
   {     
      friend class B0<FE>;
      const static FE_ALGORITHM alg_ = alg;
      typedef B4S1D<FE,alg>        This;
      typedef typename FE::TS1_ TS1;
      typedef typename FE::TS2_ TS2;
      typedef typename FE::TS3_ TS3;
      typedef typename FE::TS4_ TS4;
      typedef typename FE::TD1_  TD1;
          
   public:
      struct ThreadData{                                                       // Dynamic thread data
         const This *pThis;
         const TS1  *pSrc1;
         const TS2  *pSrc2;
         const TS3  *pSrc3;
         const TS4  *pSrc4;
               TD1   *pDst; 
         U32 size;
         U32 widthTh;
         U32 px_nRef;
       };

      ISV fe_1D_Ref( const FE &fe, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3,const TS4 *pSrc4, TD1 *pDst, U32 count )
      {
          const TD1 * end = pDst + (count * CH_COUNT(FE::CD_));
          for( ; pDst<end; pDst  += CH_COUNT(FE::CD_ ), 
                        pSrc1 += CH_COUNT(FE::CS1_),
                        pSrc2 += CH_COUNT(FE::CS2_),
                        pSrc3 += CH_COUNT(FE::CS3_),
                        pSrc4 += CH_COUNT(FE::CS4_))
                fe.REFR( pSrc1, pSrc2, pSrc3,pSrc4, pDst );
      }

      template< IsAlign ia, bool fallBack >
      ISV fe_1D_Custom( const ThreadData &t )
      {
          U32 count;
          if( fallBack )
              count = t.widthTh;
          else
              count = t.widthTh + t.px_nRef;

          DispatchType dt; B0<FE>::GetDispatch(dt);
          switch( dt )
          {
          case DT_F10H:
          case DT_SSE3:     t.pThis->fe.template Custom1D_f10h<ia>(t.pSrc1, t.pSrc2, t.pSrc3,t.pSrc4, t.pDst, count ); 
                            break;
          case DT_SSE2:     t.pThis->fe.template Custom1D_sse2<ia>(t.pSrc1, t.pSrc2, t.pSrc3,t.pSrc4, t.pDst, count ); 
                            break;
          case DT_REFR:     t.pThis->fe.template Custom1D_refr<ia>(t.pSrc1, t.pSrc2, t.pSrc3,t.pSrc4, t.pDst, count );
                            break;
          default:          assert(false);
          }

          if( fallBack )
              if( t.px_nRef )
                  fe_1D_Ref( t.pThis->fe,    t.pSrc1 + (t.widthTh) * CH_COUNT(FE::CS1_),
                                            t.pSrc2 + (t.widthTh) * CH_COUNT(FE::CS2_),
                                            t.pSrc3 + (t.widthTh) * CH_COUNT(FE::CS3_),
                                            t.pSrc4 + (t.widthTh) * CH_COUNT(FE::CS4_),
                                            t.pDst  + (t.widthTh) * CH_COUNT(FE::CD_), t.px_nRef );
      }

      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
        fe_1D_Ref( t.pThis->fe, t.pSrc1, t.pSrc2, t.pSrc3,t.pSrc4, t.pDst, t.widthTh + t.px_nRef );
        return;
#else

         const TS1    *pSrc1 = t.pSrc1;
         const TS2    *pSrc2 = t.pSrc2;
         const TS3    *pSrc3 = t.pSrc3;
         const TS4    *pSrc4 = t.pSrc4;
         TD1            *pDst  = t.pDst;

          FE fec = t.pThis->fe;

        for( typename FE::TD1_ * end = pDst + (t.widthTh) * CH_COUNT(FE::CD_);
            pDst <end;)
        {
            RegFile r;                                                         
            fec.template LOAD< FE::DST::BYTES,DISPATCH,ia,is >( r.dst , pDst  ); 
            fec.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, pSrc1 ); 
            fec.template LOAD< FE::SRC2::BYTES,DISPATCH,ia,is >( r.src2, pSrc2 );
            fec.template LOAD< FE::SRC3::BYTES,DISPATCH,ia,is >( r.src3, pSrc3 );   
            fec.template LOAD< FE::SRC4::BYTES,DISPATCH,ia,is >( r.src4, pSrc4 );   
#if   defined(OPT_F10H) || defined(OPT_SSE3)
            fec.F10H( r );        
#elif   defined(OPT_SSE2)
            fec.SSE2( r );        
#else
            assert(false);  
#endif
            fec.template STORE< FE::DST::BYTES,DISPATCH,ia,is >( r.dst, pDst );
            fec.IncrementBuffers(&pSrc1,FE::SRC1::ELEMENTS,&pSrc2,FE::SRC2::ELEMENTS,&pSrc3,FE::SRC3::ELEMENTS,&pSrc4,FE::SRC4::ELEMENTS,&pDst,FE::DST::ELEMENTS);
        }

         if( t.px_nRef )
            fe_1D_Ref( t.pThis->fe, pSrc1, pSrc2, pSrc3,pSrc4, pDst, t.px_nRef );
#endif
      }

      template< IsAlign ia > 
      ISV fe_1D_Pft( const ThreadData & t )
      {
          switch( B0<FE>::GetPft() )
          {
          case PFT_NORMAL:                                fe_1D_Normal    < ia, STREAM_FLSE    >( t );                            
                                                        break;
          case PFT_NORMAL_STREAM:                        fe_1D_Normal    < ia, STREAM_TRUE    >( t );        
                                                        break;
          case PFT_EMULATE_CUSTOM:
          case PFT_CUSTOM:                                fe_1D_Custom    < ia, true            >( t );    
                                                        break;
          case PFT_EMULATE_CUSTOM_NO_FB:
          case PFT_CUSTOM_NO_FALLBACK:                    fe_1D_Custom    < ia, false            >( t );    
                                                        break;
          default:                    assert(false);
          }
      }

      template< IsAlign ia > 
      SV fe_1D( const ThreadData & t )
      {
          fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
         for( TD1 *end = FW_REF::Offset(t.pDst,(T_OFFSET)(t.size)*(t.pThis->d1Step));
              t.pDst!=end; 
              FW_REF::Inc(t.pSrc1,t.pThis->s1Step, t.pSrc2,t.pThis->s2Step, t.pSrc3,t.pThis->s3Step,t.pSrc4,t.pThis->s4Step, t.pDst,t.pThis->d1Step) 
            ) 
            fe_1D_Pft<ia>( t );
      }  

   private:
      IV SplitData_1D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3,const TS4 *pSrc4, TD1 * pDst, int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS1) *(T_OFFSET)CH_COUNT(FE::CS1_)*(T_OFFSET)(lenTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS2) *(T_OFFSET)CH_COUNT(FE::CS2_)*(T_OFFSET)(lenTh));
            td[i].pSrc3   = FW_REF::Offset(pSrc3, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS3) *(T_OFFSET)CH_COUNT(FE::CS3_)*(T_OFFSET)(lenTh));
            td[i].pSrc4   = FW_REF::Offset(pSrc4, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS3) *(T_OFFSET)CH_COUNT(FE::CS4_)*(T_OFFSET)(lenTh));
            td[i].pDst    = FW_REF::Offset(pDst , (T_OFFSET)(i)*(T_OFFSET)sizeof(TD1) *(T_OFFSET)CH_COUNT(FE::CD_ )*(T_OFFSET)(lenTh));
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }
         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      }

      IV SplitData_2D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3,const TS4 *pSrc4, TD1 * pDst, int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(this->s1Step)*(T_OFFSET)(heightTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)*(this->s2Step)*(T_OFFSET)(heightTh));
            td[i].pSrc3   = FW_REF::Offset(pSrc3, (T_OFFSET)(i)*(this->s3Step)*(T_OFFSET)(heightTh));
            td[i].pSrc4   = FW_REF::Offset(pSrc4, (T_OFFSET)(i)*(this->s4Step)*(T_OFFSET)(heightTh));
            td[i].pDst    = FW_REF::Offset(pDst,  (T_OFFSET)(i)*(this->d1Step)*(T_OFFSET)(heightTh));
            td[i].size    = heightTh;
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
      } 

      IV Run( const TS1* pSrc1, const TS2* pSrc2, const TS3 *pSrc3, const TS3 *pSrc4,TD1 *pDst, int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));             
         bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, pSrc1, pSrc2, pSrc3,pSrc4, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,0,            pSrc2,0,                pSrc3,0,pSrc4,0,                pDst,0                ); 
                        break;
         case ALG_2D:    SplitData_2D( td, pSrc1, pSrc2, pSrc3,pSrc4, pDst, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,this->s1Step, pSrc2,this->s2Step,    pSrc3,this->s3Step,pSrc4,this->s4Step,    pDst,this->d1Step    ); 
                        break;
         default:        assert(false);
         }
         
         isAligned    ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );
      }  
      
   public:  
      
      B4S1D( FE & fe, const TS1* pSrc1, const TS2* pSrc2, 
                   const TS3* pSrc3, const TS4* pSrc4,       TD1 * pDst, int len, U32 bytesPerThread )
       :B0< FE >( fe, len, bytesPerThread  )         
      {  
         STATIC_ASSERT( ALG_1D == alg );
         Run( pSrc1, pSrc2, pSrc3,pSrc4, pDst, len );              
      }

      
      B4S1D( FE & fe, const TS1* pSrc1,int s1Step, const TS2* pSrc2,int s2Step, 
                   const TS3* pSrc3,int s3Step, const TS4* pSrc4,int s4Step,       TD1 * pDst, int d1Step, ASZ roi, U32 bytesPerThread )
       :B0< FE >( fe, roi, bytesPerThread,s1Step, s2Step, s3Step,s4Step,d1Step,0,0,0)         
      {  
         STATIC_ASSERT( ALG_2D == alg );
         Run( pSrc1, pSrc2, pSrc3,pSrc4, pDst, roi.height );              
      }       
   };


//----- Three source buffers and Three destination buffers------------
   template< class FE, FE_ALGORITHM alg >
   class B3S3D : public B0< FE >
   {     
        friend class B0<FE>;
        const static FE_ALGORITHM alg_ = alg;
        typedef B3S3D<FE,alg>        This;
        typedef typename FE::TS1_ TS1;
        typedef typename FE::TS2_ TS2;
        typedef typename FE::TS3_ TS3;
        typedef typename FE::TD1_  TD1;
        typedef typename FE::TD2_  TD2;
        typedef typename FE::TD3_  TD3;
          
   public:
      struct ThreadData{                                                       // Dynamic thread data
         const This *pThis;
         const TS1  *pSrc1;
         const TS2  *pSrc2;
         const TS3  *pSrc3;
         TD1   *pDst1; 
         TD2   *pDst2; 
         TD3   *pDst3; 
         U32 size;
         U32 widthTh;
         U32 px_nRef;
       };

      ISV fe_1D_Ref( const FE &fe, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3,TD1 *pDst1,TD2 *pDst2,TD3 *pDst3, U32 count )
      {
          const TD1 * end = pDst1 + (count * CH_COUNT(FE::CD1_));
          for( ; pDst1<end; pDst1  += CH_COUNT(FE::CD1_ ), 
                        pDst2  += CH_COUNT(FE::CD2_ ), 
                        pDst3  += CH_COUNT(FE::CD3_ ), 
                        pSrc1 += CH_COUNT(FE::CS1_),
                        pSrc2 += CH_COUNT(FE::CS2_),
                        pSrc3 += CH_COUNT(FE::CS3_))
                fe.REFR( pSrc1, pSrc2, pSrc3,pDst1,pDst2,pDst3 );
      }

      template< IsAlign ia, bool fallBack >
      ISV fe_1D_Custom( const ThreadData &t )
      {
          U32 count;
          if( fallBack )
              count = t.widthTh;
          else
              count = t.widthTh + t.px_nRef;

          DispatchType dt; B0<FE>::GetDispatch(dt);
          switch( dt )
          {
          case DT_F10H:
          case DT_SSE3:     t.pThis->fe.template Custom1D_f10h<ia>(t.pSrc1, t.pSrc2, t.pSrc3, t.pDst1,t.pDst2,t.pDst3, count ); 
                            break;
          case DT_SSE2:     t.pThis->fe.template Custom1D_sse2<ia>(t.pSrc1, t.pSrc2, t.pSrc3, t.pDst1,t.pDst2,t.pDst3, count ); 
                            break;
          case DT_REFR:     t.pThis->fe.template Custom1D_refr<ia>(t.pSrc1, t.pSrc2, t.pSrc3, t.pDst1,t.pDst2,t.pDst3, count ); 
                            break;
          default:          assert(false);
          }

          if( fallBack )
              if( t.px_nRef )
                  fe_1D_Ref( t.pThis->fe,    t.pSrc1 + (t.widthTh) * CH_COUNT(FE::CS1_),
                                            t.pSrc2 + (t.widthTh) * CH_COUNT(FE::CS2_),
                                            t.pSrc3 + (t.widthTh) * CH_COUNT(FE::CS3_),
                                            t.pDst1  + (t.widthTh) * CH_COUNT(FE::CD1_), 
                                            t.pDst2  + (t.widthTh) * CH_COUNT(FE::CD2_), 
                                            t.pDst3  + (t.widthTh) * CH_COUNT(FE::CD3_),t.px_nRef );
      }

      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
        fe_1D_Ref( t.pThis->fe, t.pSrc1, t.pSrc2, t.pSrc3,t.pDst1,t.pDst2,t.pDst3, t.widthTh + t.px_nRef );
        return;
#else

         const TS1    *pSrc1 = t.pSrc1;
         const TS2    *pSrc2 = t.pSrc2;
         const TS3    *pSrc3 = t.pSrc3;
         TD1        *pDst1  = t.pDst1;
         TD2        *pDst2  = t.pDst2;
         TD3        *pDst3  = t.pDst3;

          FE fec = t.pThis->fe;

        for( typename FE::TD1_ * end = pDst1 + (t.widthTh) * CH_COUNT(FE::CD1_);
            pDst1 <end;)
        {
            RegFile r;                                                         
            fec.template LOAD< FE::DST1:: BYTES,DISPATCH,ia,is >( r.dst , pDst1  ); 
            fec.template LOAD< FE::DST2:: BYTES,DISPATCH,ia,is >( r.dst2 , pDst2  ); 
            fec.template LOAD< FE::DST3:: BYTES,DISPATCH,ia,is >( r.dst3 , pDst3  ); 
            fec.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, pSrc1 ); 
            fec.template LOAD< FE::SRC2::BYTES,DISPATCH,ia,is >( r.src2, pSrc2 );
            fec.template LOAD< FE::SRC3::BYTES,DISPATCH,ia,is >( r.src3, pSrc3 );   
#if   defined(OPT_F10H) || defined(OPT_SSE3)
            fec.F10H( r );        
#elif   defined(OPT_SSE2)
            fec.SSE2( r );        
#else
            assert(false);  
#endif
            fec.template STORE< FE::DST1::BYTES,DISPATCH,ia,is >( r.dst, pDst1 );
            fec.template STORE< FE::DST2:: BYTES,DISPATCH,ia,is >( r.dst2 , pDst2  ); 
            fec.template STORE< FE::DST3:: BYTES,DISPATCH,ia,is >( r.dst3 , pDst3  ); 
            fec.IncrementBuffers(&pSrc1,FE::SRC1::ELEMENTS,&pSrc2,FE::SRC2::ELEMENTS,&pSrc3,FE::SRC3::ELEMENTS,&pDst1,FE::DST1::ELEMENTS,&pDst2,FE::DST2::ELEMENTS,&pDst3,FE::DST3::ELEMENTS);
        }

         if( t.px_nRef )
            fe_1D_Ref( t.pThis->fe, pSrc1, pSrc2, pSrc3,pDst1,pDst2,pDst3, t.px_nRef );
#endif
      }

      template< IsAlign ia > 
      ISV fe_1D_Pft( const ThreadData & t )
      {
          switch( B0<FE>::GetPft() )
          {
          case PFT_NORMAL:                                fe_1D_Normal    < ia, STREAM_FLSE    >( t );                            
                                                        break;
          case PFT_NORMAL_STREAM:                        fe_1D_Normal    < ia, STREAM_TRUE    >( t );                            
                                                        break;
          case PFT_EMULATE_CUSTOM:
          case PFT_CUSTOM:                                fe_1D_Custom    < ia, true            >( t );                            
                                                        break;
          case PFT_EMULATE_CUSTOM_NO_FB:
          case PFT_CUSTOM_NO_FALLBACK:                    fe_1D_Custom    < ia, false            >( t );    
                                                        break;
          default:                    assert(false);
          }
      }

      template< IsAlign ia > 
      SV fe_1D( const ThreadData & t )
      {
          fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
        for( TD1 *end = FW_REF::Offset(t.pDst1,(T_OFFSET)(t.size)*(t.pThis->d1Step));
              t.pDst1!=end; 
              FW_REF::Inc(t.pSrc1,t.pThis->s1Step, t.pSrc2,t.pThis->s2Step, t.pSrc3,t.pThis->s3Step,t.pDst1,t.pThis->d1Step,t.pDst2,t.pThis->d2Step,t.pDst3,t.pThis->d3Step) 
            ) 
            fe_1D_Pft<ia>( t );
      }  

   private:
      IV SplitData_1D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3,TD1 * pDst1,TD2 * pDst2,TD3 * pDst3, int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS1) *(T_OFFSET)CH_COUNT(FE::CS1_) *(T_OFFSET)(lenTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS2) *(T_OFFSET)CH_COUNT(FE::CS2_) *(T_OFFSET)(lenTh));
            td[i].pSrc3   = FW_REF::Offset(pSrc3, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS3) *(T_OFFSET)CH_COUNT(FE::CS3_) *(T_OFFSET)(lenTh));
            td[i].pDst1   = FW_REF::Offset(pDst1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD1) *(T_OFFSET)CH_COUNT(FE::CD1_) *(T_OFFSET)(lenTh));
            td[i].pDst2   = FW_REF::Offset(pDst2, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD2) *(T_OFFSET)CH_COUNT(FE::CD2_) *(T_OFFSET)(lenTh));
            td[i].pDst3   = FW_REF::Offset(pDst3, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD3) *(T_OFFSET)CH_COUNT(FE::CD3_) *(T_OFFSET)(lenTh));
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }
         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      }

      IV SplitData_2D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3,TD1 * pDst1,TD2 * pDst2,TD3 * pDst3, int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(this->s1Step)*(T_OFFSET)(heightTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)*(this->s2Step)*(T_OFFSET)(heightTh));
            td[i].pSrc3   = FW_REF::Offset(pSrc3, (T_OFFSET)(i)*(this->s3Step)*(T_OFFSET)(heightTh));
            td[i].pDst1   = FW_REF::Offset(pDst1, (T_OFFSET)(i)*(this->d1Step)*(T_OFFSET)(heightTh));
            td[i].pDst2   = FW_REF::Offset(pDst2, (T_OFFSET)(i)*(this->d2Step)*(T_OFFSET)(heightTh));
            td[i].pDst3   = FW_REF::Offset(pDst3, (T_OFFSET)(i)*(this->d3Step)*(T_OFFSET)(heightTh));
            td[i].size    = heightTh;
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
      } 

      IV Run( const TS1* pSrc1, const TS2* pSrc2, const TS3 *pSrc3,TD1 *pDst1,TD2 *pDst2, TD3 *pDst3,int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));             
         bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, pSrc1, pSrc2, pSrc3,pDst1,pDst2,pDst3, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,0,            pSrc2,0,                pSrc3,0,pDst1,0,pDst2,0,pDst3,0                ); 
                        break;
         case ALG_2D:    SplitData_2D( td, pSrc1, pSrc2, pSrc3,pDst1,pDst2,pDst3, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,this->s1Step, pSrc2,this->s2Step,    pSrc3,this->s3Step,pDst1,this->d1Step,pDst2,this->d2Step,pDst3,this->d3Step    ); 
                        break;
         default:        assert(false);
         }
         
         isAligned    ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );
      }  
      
   public:  
      
      B3S3D( FE & fe, const TS1* pSrc1, const TS2* pSrc2, 
                   const TS3* pSrc3,TD1 * pDst1,TD2 * pDst2,TD3 * pDst3, int len, U32 bytesPerThread )
       :B0< FE >( fe, len, bytesPerThread  )         
      {  
         STATIC_ASSERT( ALG_1D == alg );
         Run( pSrc1, pSrc2, pSrc3,pDst1,pDst2,pDst3, len );              
      }

      
      B3S3D( FE & fe, const TS1* pSrc1,int s1Step, const TS2* pSrc2,int s2Step, 
                   const TS3* pSrc3,int s3Step, TD1 * pDst1, int d1Step,TD2 * pDst2, int d2Step,TD3 * pDst3, int d3Step, ASZ roi, U32 bytesPerThread )
       :B0< FE >( fe, roi, bytesPerThread,s1Step, s2Step, s3Step,0,d1Step,d2Step,d3Step,0)         
      {  
         STATIC_ASSERT( ALG_2D == alg );
         Run( pSrc1, pSrc2, pSrc3,pDst1,pDst2,pDst3, roi.height );              
      }       
   };

//----- Four source buffers and Four destination buffers------------
   template< class FE, FE_ALGORITHM alg >
   class B4S4D : public B0< FE >
   {     
        friend class B0<FE>;
        const static FE_ALGORITHM alg_ = alg;
        typedef B4S4D<FE,alg>        This;
        typedef typename FE::TS1_ TS1;
        typedef typename FE::TS2_ TS2;
        typedef typename FE::TS3_ TS3;
        typedef typename FE::TS4_ TS4;

        typedef typename FE::TD1_  TD1;
        typedef typename FE::TD2_  TD2;
        typedef typename FE::TD3_  TD3;
        typedef typename FE::TD4_  TD4;

   public:
      struct ThreadData{                                                       // Dynamic thread data
         const This *pThis;
         const TS1  *pSrc1;
         const TS2  *pSrc2;
         const TS3  *pSrc3;
         const TS4  *pSrc4;
         TD1   *pDst1; 
         TD2   *pDst2; 
         TD3   *pDst3; 
         TD4   *pDst4; 
         U32 size;
         U32 widthTh;
         U32 px_nRef;
       };

      ISV fe_1D_Ref( const FE &fe, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3, const TS4 *pSrc4,TD1 *pDst1,TD2 *pDst2,TD3 *pDst3,TD4 *pDst4, U32 count )
      {
          const TD1 * end = pDst1 + (count * CH_COUNT(FE::CD1_));
          for( ; pDst1<end; pDst1  += CH_COUNT(FE::CD1_ ), 
                        pDst2  += CH_COUNT(FE::CD2_ ), 
                        pDst3  += CH_COUNT(FE::CD3_ ), 
                        pDst4  += CH_COUNT(FE::CD4_ ), 
                        pSrc1 += CH_COUNT(FE::CS1_),
                        pSrc2 += CH_COUNT(FE::CS2_),
                        pSrc3 += CH_COUNT(FE::CS3_),
                        pSrc4 += CH_COUNT(FE::CS4_))
                fe.REFR( pSrc1, pSrc2, pSrc3,pSrc4,pDst1,pDst2,pDst3,pDst4 );
      }

      template< IsAlign ia, bool fallBack >
      ISV fe_1D_Custom( const ThreadData &t )
      {
          U32 count;
          if( fallBack )
              count = t.widthTh;
          else
              count = t.widthTh + t.px_nRef;

          DispatchType dt; B0<FE>::GetDispatch(dt);
          switch( dt )
          {
          case DT_F10H:
          case DT_SSE3:     t.pThis->fe.template Custom1D_f10h<ia>(t.pSrc1, t.pSrc2, t.pSrc3,t.pSrc4, t.pDst1,t.pDst2,t.pDst3,t.pDst4, count ); 
                            break;
          case DT_SSE2:     t.pThis->fe.template Custom1D_sse2<ia>(t.pSrc1, t.pSrc2, t.pSrc3,t.pSrc4, t.pDst1,t.pDst2,t.pDst3,t.pDst4, count ); 
                            break;
          case DT_REFR:     t.pThis->fe.template Custom1D_refr<ia>(t.pSrc1, t.pSrc2, t.pSrc3,t.pSrc4, t.pDst1,t.pDst2,t.pDst3,t.pDst4, count ); 
                            break;
          default:          assert(false);
          }

          if( fallBack )
              if( t.px_nRef )
                  fe_1D_Ref( t.pThis->fe,    t.pSrc1 + (t.widthTh) * CH_COUNT(FE::CS1_),
                                            t.pSrc2 + (t.widthTh) * CH_COUNT(FE::CS2_),
                                            t.pSrc3 + (t.widthTh) * CH_COUNT(FE::CS3_),
                                            t.pSrc4 + (t.widthTh) * CH_COUNT(FE::CS4_),
                                            t.pDst1  + (t.widthTh) * CH_COUNT(FE::CD1_), 
                                            t.pDst2  + (t.widthTh) * CH_COUNT(FE::CD2_), 
                                            t.pDst3  + (t.widthTh) * CH_COUNT(FE::CD3_),
                                            t.pDst4  + (t.widthTh) * CH_COUNT(FE::CD4_),t.px_nRef );
      }

      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
        fe_1D_Ref( t.pThis->fe, t.pSrc1, t.pSrc2, t.pSrc3,t.pSrc4,t.pDst1,t.pDst2,t.pDst3,t.pDst4, t.widthTh + t.px_nRef );
        return;
#else

         const TS1    *pSrc1 = t.pSrc1;
         const TS2    *pSrc2 = t.pSrc2;
         const TS3    *pSrc3 = t.pSrc3;
         const TS4    *pSrc4 = t.pSrc4;

         TD1        *pDst1  = t.pDst1;
         TD2        *pDst2  = t.pDst2;
         TD3        *pDst3  = t.pDst3;
         TD4        *pDst4  = t.pDst4;

          FE fec = t.pThis->fe;

        for( typename FE::TD1_ * end = pDst1 + (t.widthTh) * CH_COUNT(FE::CD1_);
            pDst1 <end;)
        {
            RegFile r;                                                         
            fec.template LOAD< FE::DST1:: BYTES,DISPATCH,ia,is >( r.dst , pDst1  ); 
            fec.template LOAD< FE::DST2:: BYTES,DISPATCH,ia,is >( r.dst2 , pDst2  ); 
            fec.template LOAD< FE::DST3:: BYTES,DISPATCH,ia,is >( r.dst3 , pDst3  ); 
            fec.template LOAD< FE::DST4:: BYTES,DISPATCH,ia,is >( r.dst4 , pDst4  ); 
            fec.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, pSrc1 ); 
            fec.template LOAD< FE::SRC2::BYTES,DISPATCH,ia,is >( r.src2, pSrc2 );
            fec.template LOAD< FE::SRC3::BYTES,DISPATCH,ia,is >( r.src3, pSrc3 );   
            fec.template LOAD< FE::SRC4::BYTES,DISPATCH,ia,is >( r.src4, pSrc4 );   
#if   defined(OPT_F10H) || defined(OPT_SSE3)
            fec.F10H( r );        
#elif   defined(OPT_SSE2)
            fec.SSE2( r );        
#else
            assert(false);  
#endif
            fec.template STORE< FE::DST1::BYTES,DISPATCH,ia,is >( r.dst, pDst1 );
            fec.template STORE< FE::DST2:: BYTES,DISPATCH,ia,is >( r.dst2 , pDst2  ); 
            fec.template STORE< FE::DST3:: BYTES,DISPATCH,ia,is >( r.dst3 , pDst3  ); 
            fec.template STORE< FE::DST4:: BYTES,DISPATCH,ia,is >( r.dst4 , pDst4  ); 
            fec.IncrementBuffers(&pSrc1,FE::SRC1::ELEMENTS,&pSrc2,FE::SRC2::ELEMENTS,&pSrc3,FE::SRC3::ELEMENTS,&pSrc4,FE::SRC4::ELEMENTS,&pDst1,FE::DST1::ELEMENTS,&pDst2,FE::DST2::ELEMENTS,&pDst3,FE::DST3::ELEMENTS,&pDst4,FE::DST4::ELEMENTS);
        }

         if( t.px_nRef )
            fe_1D_Ref( t.pThis->fe, pSrc1, pSrc2, pSrc3,pSrc4,pDst1,pDst2,pDst3,pDst4, t.px_nRef );
#endif
      }

      template< IsAlign ia > 
      ISV fe_1D_Pft( const ThreadData & t )
      {
          switch( B0<FE>::GetPft() )
          {
          case PFT_NORMAL:                                fe_1D_Normal    < ia, STREAM_FLSE    >( t );                            
                                                        break;
          case PFT_NORMAL_STREAM:                        fe_1D_Normal    < ia, STREAM_TRUE    >( t );                            
                                                        break;
          case PFT_EMULATE_CUSTOM:
          case PFT_CUSTOM:                                fe_1D_Custom    < ia, true            >( t );                
                                                        break;
          case PFT_EMULATE_CUSTOM_NO_FB:
          case PFT_CUSTOM_NO_FALLBACK:                    fe_1D_Custom    < ia, false            >( t );    
                                                        break;
          default:                    assert(false);
          }
      }

      template< IsAlign ia > 
      SV fe_1D( const ThreadData & t )
      {
          fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
         for( TD1 *end = FW_REF::Offset(t.pDst1,(T_OFFSET)(t.size)*(t.pThis->d1Step));
              t.pDst1!=end; 
              FW_REF::Inc(t.pSrc1,t.pThis->s1Step, t.pSrc2,t.pThis->s2Step, t.pSrc3,t.pThis->s3Step,t.pSrc4,t.pThis->s4Step,t.pDst1,t.pThis->d1Step,t.pDst2,t.pThis->d2Step,t.pDst3,t.pThis->d3Step,t.pDst4,t.pThis->d4Step) 
            ) 
            fe_1D_Pft<ia>( t );
      }  

   private:
      IV SplitData_1D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3,const TS4 *pSrc4,TD1 * pDst1,TD2 * pDst2,TD3 * pDst3,TD4 * pDst4, int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS1) *(T_OFFSET)CH_COUNT(FE::CS1_) *(T_OFFSET)(lenTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS2) *(T_OFFSET)CH_COUNT(FE::CS2_) *(T_OFFSET)(lenTh));
            td[i].pSrc3   = FW_REF::Offset(pSrc3, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS3) *(T_OFFSET)CH_COUNT(FE::CS3_) *(T_OFFSET)(lenTh));
            td[i].pSrc4   = FW_REF::Offset(pSrc4, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS4) *(T_OFFSET)CH_COUNT(FE::CS4_) *(T_OFFSET)(lenTh));
            td[i].pDst1   = FW_REF::Offset(pDst1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD1) *(T_OFFSET)CH_COUNT(FE::CD1_) *(T_OFFSET)(lenTh));
            td[i].pDst2   = FW_REF::Offset(pDst2, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD2) *(T_OFFSET)CH_COUNT(FE::CD2_) *(T_OFFSET)(lenTh));
            td[i].pDst3   = FW_REF::Offset(pDst3, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD3) *(T_OFFSET)CH_COUNT(FE::CD3_) *(T_OFFSET)(lenTh));
            td[i].pDst4   = FW_REF::Offset(pDst4, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD3) *(T_OFFSET)CH_COUNT(FE::CD4_) *(T_OFFSET)(lenTh));
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }
         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      }

      IV SplitData_2D( ThreadData *td, const TS1 *pSrc1, const TS2 *pSrc2, const TS3 *pSrc3, const TS4 *pSrc4,TD1 * pDst1,TD2 * pDst2,TD3 * pDst3,TD4 * pDst4, int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)* (this->s1Step)*(T_OFFSET)(heightTh));
            td[i].pSrc2   = FW_REF::Offset(pSrc2, (T_OFFSET)(i)* (this->s2Step)*(T_OFFSET)(heightTh));
            td[i].pSrc3   = FW_REF::Offset(pSrc3, (T_OFFSET)(i)* (this->s3Step)*(T_OFFSET)(heightTh));
            td[i].pSrc4   = FW_REF::Offset(pSrc4, (T_OFFSET)(i)* (this->s4Step)*(T_OFFSET)(heightTh));
            td[i].pDst1   = FW_REF::Offset(pDst1, (T_OFFSET)(i)* (this->d1Step)*(T_OFFSET)(heightTh));
            td[i].pDst2   = FW_REF::Offset(pDst2, (T_OFFSET)(i)* (this->d2Step)*(T_OFFSET)(heightTh));
            td[i].pDst3   = FW_REF::Offset(pDst3, (T_OFFSET)(i)* (this->d3Step)*(T_OFFSET)(heightTh));
            td[i].pDst4   = FW_REF::Offset(pDst4, (T_OFFSET)(i)* (this->d4Step)*(T_OFFSET)(heightTh));
            td[i].size    = heightTh;
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
      } 

      IV Run( const TS1* pSrc1, const TS2* pSrc2, const TS3 *pSrc3,const TS4 *pSrc4,TD1 *pDst1,TD2 *pDst2, TD3 *pDst3,TD4 *pDst4,int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));             
         bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, pSrc1, pSrc2, pSrc3,pSrc4,pDst1,pDst2,pDst3,pDst4, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,0,            pSrc2,0,                pSrc3,0,pSrc4,0,pDst1,0,pDst2,0,pDst3,0,pDst4,0                ); 
                        break;
         case ALG_2D:    SplitData_2D( td, pSrc1, pSrc2, pSrc3,pSrc4,pDst1,pDst2,pDst3,pDst4, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,this->s1Step, pSrc2,this->s2Step,    pSrc3,this->s3Step,pSrc4,this->s4Step,pDst1,this->d1Step,pDst2,this->d2Step,pDst3,this->d3Step,pDst4,this->d4Step    ); 
                        break;
         default:        assert(false);
         }
         
         isAligned    ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );
      }  
      
   public:  
      
      B4S4D( FE & fe, const TS1* pSrc1, const TS2* pSrc2, 
                   const TS3* pSrc3,const TS4* pSrc4,TD1 * pDst1,TD2 * pDst2,TD3 * pDst3, TD4 * pDst4,int len, U32 bytesPerThread )
       :B0< FE >( fe, len, bytesPerThread  )         
      {  
         STATIC_ASSERT( ALG_1D == alg );
         Run( pSrc1, pSrc2, pSrc3,pSrc4,pDst1,pDst2,pDst3,pDst4, len );              
      }

      
      B4S4D( FE & fe, const TS1* pSrc1,int s1Step, const TS2* pSrc2,int s2Step, 
                   const TS3* pSrc3,int s3Step, const TS4* pSrc4,int s4Step, TD1 * pDst1, int d1Step,TD2 * pDst2, int d2Step,TD3 * pDst3,int d3Step,TD4 * pDst4,int d4Step, ASZ roi, U32 bytesPerThread )
       :B0< FE >( fe, roi, bytesPerThread,s1Step, s2Step, s3Step,s4Step,d1Step,d2Step,d3Step,d4Step)         
      {  
         STATIC_ASSERT( ALG_2D == alg );
         Run( pSrc1, pSrc2, pSrc3,pSrc4,pDst1,pDst2,pDst3,pDst4, roi.height );              
      }       
   };


//----- One source buffer and Three destination buffers------------
   template< class FE, FE_ALGORITHM alg >
   class B1S3D : public B0< FE >
   {     
      friend class B0<FE>;
      const static FE_ALGORITHM alg_ = alg;
      typedef B1S3D<FE,alg>        This;
      typedef typename FE::TS1_ TS1;
      typedef typename FE::TD1_ TD1;
      typedef typename FE::TD2_ TD2;
      typedef typename FE::TD3_  TD3;
          
   public:
      struct ThreadData{                                                       // Dynamic thread data
         const This *pThis;
         const TS1  *pSrc1;
         TD1  *pDst1;
         TD2  *pDst2;
         TD3  *pDst3; 

         U32 size;
         U32 widthTh;
         U32 px_nRef;
       };

      ISV fe_1D_Ref( const FE &fe, const TS1 *pSrc1,  TD1 *pDst1,  TD2 *pDst2, TD3 *pDst3, U32 count )
      {
          const TD1 * end = pDst1 + (count * CH_COUNT(FE::CD1_));
          for( ; pDst1<end; pDst1  += CH_COUNT(FE::CD1_ ), 
                        pDst2 += CH_COUNT(FE::CD2_),
                        pDst3 += CH_COUNT(FE::CD3_),
                        pSrc1 += CH_COUNT(FE::CS1_) )
                fe.REFR( pSrc1, pDst1, pDst2, pDst3 );
      }

      template< IsAlign ia, bool fallBack >
      ISV fe_1D_Custom( const ThreadData &t )
      {
          U32 count;
          if( fallBack )
              count = t.widthTh;
          else
              count = t.widthTh + t.px_nRef;

          DispatchType dt; B0<FE>::GetDispatch(dt);
          switch( dt )
          {
          case DT_F10H:
          case DT_SSE3:     t.pThis->fe.template Custom1D_f10h<ia>(t.pSrc1, t.pDst1, t.pDst2, t.pDst3, count ); 
                            break;
          case DT_SSE2:     t.pThis->fe.template Custom1D_sse2<ia>(t.pSrc1, t.pDst1, t.pDst2, t.pDst3, count ); 
                            break;
          case DT_REFR:     t.pThis->fe.template Custom1D_refr<ia>(t.pSrc1, t.pDst1, t.pDst2, t.pDst3, count );
                            break;
          default:          assert(false);
          }

          if( fallBack )
              if( t.px_nRef )
                  fe_1D_Ref( t.pThis->fe,    t.pSrc1 + (t.widthTh) * CH_COUNT(FE::CS1_),
                                            t.pDst1 + (t.widthTh) * CH_COUNT(FE::CD1_),
                                            t.pDst2 + (t.widthTh) * CH_COUNT(FE::CD2_),
                                            t.pDst3  + (t.widthTh) * CH_COUNT(FE::CD3_), t.px_nRef );
      }

      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
        fe_1D_Ref( t.pThis->fe, t.pSrc1, t.pDst1, t.pDst2, t.pDst3, t.widthTh + t.px_nRef );
        return;
#else

         const TS1    *pSrc1 = t.pSrc1;
         TD1    *pDst1 = t.pDst1;
         TD2    *pDst2 = t.pDst2;
         TD3    *pDst3  = t.pDst3;

          FE fec = t.pThis->fe;

        for( typename FE::TD1_ * end = pDst1 + (t.widthTh) * CH_COUNT(FE::CD1_);
            pDst1 <end;)
        {
            RegFile r;                                                         
            fec.template LOAD< FE::DST1:: BYTES,DISPATCH,ia,is >( r.dst , pDst1); 
            fec.template LOAD< FE::DST2::BYTES,DISPATCH,ia,is >( r.dst2, pDst2 ); 
            fec.template LOAD< FE::DST3::BYTES,DISPATCH,ia,is >( r.dst3, pDst3 );
            fec.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, pSrc1 );   
#if   defined(OPT_F10H) || defined(OPT_SSE3)
            fec.F10H( r );        
#elif   defined(OPT_SSE2)
            fec.SSE2( r );        
#else
            assert(false);  
#endif
            fec.template STORE< FE::DST1:: BYTES,DISPATCH,ia,is >( r.dst , pDst1); 
            fec.template STORE< FE::DST2::BYTES,DISPATCH,ia,is >( r.dst2, pDst2 ); 
            fec.template STORE< FE::DST3::BYTES,DISPATCH,ia,is >( r.dst3, pDst3 );
            fec.IncrementBuffers(&pSrc1,FE::SRC1::ELEMENTS,&pDst1,FE::DST1::ELEMENTS,&pDst2,FE::DST2::ELEMENTS,&pDst3,FE::DST3::ELEMENTS);
        }
        
        t.pThis->fe = fec;

         if( t.px_nRef )
            fe_1D_Ref( t.pThis->fe, pSrc1, pDst1, pDst2, pDst3, t.px_nRef );
#endif
      }

      template< IsAlign ia > 
      ISV fe_1D_Pft( const ThreadData & t )
      {
          switch( B0<FE>::GetPft() )
          {
          case PFT_NORMAL:                                fe_1D_Normal    < ia, STREAM_FLSE    >( t );                            
                                                        break;
          case PFT_NORMAL_STREAM:                        fe_1D_Normal    < ia, STREAM_TRUE    >( t );                            
                                                        break;
          case PFT_EMULATE_CUSTOM:
          case PFT_CUSTOM:                                fe_1D_Custom    < ia, true            >( t );    
                                                        break;
          case PFT_EMULATE_CUSTOM_NO_FB:
          case PFT_CUSTOM_NO_FALLBACK:                    fe_1D_Custom    < ia, false            >( t );
                                                        break;
          default:                    assert(false);
          }
      }

      template< IsAlign ia > 
      SV fe_1D( const ThreadData & t )
      {
          fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
         for( TD1 *end = FW_REF::Offset(t.pDst1,(T_OFFSET)(t.size)*(t.pThis->d1Step));
              t.pDst1!=end; 
              FW_REF::Inc(t.pSrc1,t.pThis->s1Step, t.pDst1,t.pThis->d1Step, t.pDst2,t.pThis->d2Step, t.pDst3,t.pThis->d3Step) 
            ) 
            fe_1D_Pft<ia>( t );
      }  

   private:
      IV SplitData_1D( ThreadData *td,  const TS1 *pSrc1,  TD1 *pDst1,  TD2 *pDst2, TD3 * pDst3,int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS1) *(T_OFFSET)CH_COUNT(FE::CS1_) *(T_OFFSET)(lenTh));
            td[i].pDst1   = FW_REF::Offset(pDst1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD1) *(T_OFFSET)CH_COUNT(FE::CD1_) *(T_OFFSET)(lenTh));
            td[i].pDst2   = FW_REF::Offset(pDst2, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD2) *(T_OFFSET)CH_COUNT(FE::CD2_) *(T_OFFSET)(lenTh));
            td[i].pDst3   = FW_REF::Offset(pDst3, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD3) *(T_OFFSET)CH_COUNT(FE::CD3_) *(T_OFFSET)(lenTh));
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }
         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      }

      IV SplitData_2D( ThreadData *td,  const TS1 *pSrc1,  TD1 *pDst1,  TD2 *pDst2, TD3 * pDst3,int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(this->s1Step)*(T_OFFSET)(heightTh));
            td[i].pDst1   = FW_REF::Offset(pDst1, (T_OFFSET)(i)*(this->d1Step)*(T_OFFSET)(heightTh));
            td[i].pDst2   = FW_REF::Offset(pDst2, (T_OFFSET)(i)*(this->d2Step)*(T_OFFSET)(heightTh));
            td[i].pDst3   = FW_REF::Offset(pDst3, (T_OFFSET)(i)*(this->d3Step)*(T_OFFSET)(heightTh));
            td[i].size    = heightTh;
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
      } 

      IV Run( const TS1* pSrc1, TD1* pDst1,TD2 *pDst2, TD3 *pDst3, int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));             
         bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, pSrc1, pDst1, pDst2, pDst3, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,0,            pDst1,0,                pDst2,0,                pDst3,0                ); 
                        break;
         case ALG_2D:    SplitData_2D( td, pSrc1, pDst1, pDst2, pDst3, size ); 
                        isAligned = FW_REF::IsAligned( pSrc1,this->s1Step, pDst1,this->s1Step,    pDst2,this->s2Step,    pDst3,this->d3Step    ); 
                        break;
         default:        assert(false);
         }
         
         isAligned    ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );
      }  
      
   public:  
      
      B1S3D( FE & fe, const TS1* pSrc1,  TD1* pDst1, 
                    TD2* pDst2,        TD3 * pDst3, int len, U32 bytesPerThread )
       :B0< FE >( fe, len, bytesPerThread  )         
      {  
         STATIC_ASSERT( ALG_1D == alg );
         Run( pSrc1, pDst1, pDst2, pDst3, len );              
      }

      
      B1S3D( FE & fe, const TS1* pSrc1,int s1Step,  TD1 * pDst1,int d1Step, 
                    TD2 * pDst2,int d2Step,       TD3 * pDst3, int d3Step, ASZ roi, U32 bytesPerThread )
       :B0< FE >( fe, roi, bytesPerThread,s1Step, 0, 0,0,d1Step,d2Step,d3Step,0)         
      {  
         STATIC_ASSERT( ALG_2D == alg );
         Run( pSrc1, pDst1, pDst2, pDst3, roi.height );              
      }       
   };

    // ----- One source buffer and Four destination buffers -----
    template< class FE, FE_ALGORITHM alg >
    class B1S4D : public B0< FE >
    {     
        friend class B0<FE>;
        const static FE_ALGORITHM alg_ = alg;
        typedef B1S4D<FE,alg>        This;
        typedef typename FE::TS1_ TS1;
        typedef typename FE::TD1_ TD1;
        typedef typename FE::TD2_ TD2;
        typedef typename FE::TD3_  TD3;
        typedef typename FE::TD4_  TD4;

    public:
        struct ThreadData{                                                       // Dynamic thread data
            const This *pThis;
            const TS1  *pSrc1;
            TD1  *pDst1;
            TD2  *pDst2;
            TD3  *pDst3; 
            TD4  *pDst4; 

            U32 size;
            U32 widthTh;
            U32 px_nRef;
        };

        ISV fe_1D_Ref( const FE &fe, const TS1 *pSrc1, TD1 *pDst1, TD2 *pDst2, TD3 *pDst3, TD4 *pDst4, U32 count )
        {
            const TD1 * end = pDst1 + (count * CH_COUNT(FE::CD1_));
            for( ; pDst1<end; pDst1 += CH_COUNT(FE::CD1_ ), 
                              pDst2 += CH_COUNT(FE::CD2_),
                              pDst3 += CH_COUNT(FE::CD3_),
                              pDst4 += CH_COUNT(FE::CD4_),
                              pSrc1 += CH_COUNT(FE::CS1_) )
                fe.REFR( pSrc1, pDst1, pDst2, pDst3,pDst4 );
        }

        template< IsAlign ia, bool fallBack >
        ISV fe_1D_Custom( const ThreadData &t )
        {
            U32 count;
            if( fallBack )
              count = t.widthTh;
            else
                count = t.widthTh + t.px_nRef;

            DispatchType dt; B0<FE>::GetDispatch(dt);
            switch( dt )
            {
            case DT_F10H:
            case DT_SSE3:   t.pThis->fe.template Custom1D_f10h<ia>( t.pSrc1, t.pDst1, t.pDst2, t.pDst3, t.pDst4, count );
                            break;                                                                    
            case DT_SSE2:   t.pThis->fe.template Custom1D_sse2<ia>( t.pSrc1, t.pDst1, t.pDst2, t.pDst3, t.pDst4, count );
                            break;                                                                    
            case DT_REFR:   t.pThis->fe.template Custom1D_refr<ia>( t.pSrc1, t.pDst1, t.pDst2, t.pDst3, t.pDst4, count );
                            break;
            default:        assert(false);
            }

            if( fallBack )
              if( t.px_nRef )
                  fe_1D_Ref( t.pThis->fe,   t.pSrc1 + (t.widthTh) * CH_COUNT(FE::CS1_),
                                            t.pDst1 + (t.widthTh) * CH_COUNT(FE::CD1_),
                                            t.pDst2 + (t.widthTh) * CH_COUNT(FE::CD2_),
                                            t.pDst3 + (t.widthTh) * CH_COUNT(FE::CD3_), 
                                            t.pDst4 + (t.widthTh) * CH_COUNT(FE::CD4_), t.px_nRef );
        }

        template< IsAlign ia, IsStream is >
        ISV fe_1D_Normal( const ThreadData &t )
        {
#if defined(OPT_REFR)
            fe_1D_Ref( t.pThis->fe, t.pSrc1, t.pDst1, t.pDst2, t.pDst3,t.pDst4, t.widthTh + t.px_nRef );
            return;
#else

            const TS1    *pSrc1 = t.pSrc1;
            TD1    *pDst1 = t.pDst1;
            TD2    *pDst2 = t.pDst2;
            TD3    *pDst3  = t.pDst3;
            TD4    *pDst4  = t.pDst4;

            FE fec = t.pThis->fe;

            for( typename FE::TD1_ * end = pDst1 + (t.widthTh) * CH_COUNT(FE::CD1_);
                pDst1 <end;)
            {
                RegFile r;                                                         
                fec.template LOAD< FE::DST1:: BYTES,DISPATCH,ia,is >( r.dst , pDst1); 
                fec.template LOAD< FE::DST2::BYTES,DISPATCH,ia,is >( r.dst2, pDst2 ); 
                fec.template LOAD< FE::DST3::BYTES,DISPATCH,ia,is >( r.dst3, pDst3 );
                fec.template LOAD< FE::DST4::BYTES,DISPATCH,ia,is >( r.dst4, pDst4 );
                fec.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, pSrc1 );   
#if   defined(OPT_F10H) || defined(OPT_SSE3)
                fec.F10H( r );        
#elif   defined(OPT_SSE2)
                fec.SSE2( r );        
#else
                assert(false);  
#endif
                fec.template STORE< FE::DST1:: BYTES,DISPATCH,ia,is >( r.dst , pDst1); 
                fec.template STORE< FE::DST2::BYTES,DISPATCH,ia,is >( r.dst2, pDst2 ); 
                fec.template STORE< FE::DST3::BYTES,DISPATCH,ia,is >( r.dst3, pDst3 );
                fec.template STORE< FE::DST4::BYTES,DISPATCH,ia,is >( r.dst4, pDst4 );
                fec.IncrementBuffers(&pSrc1,FE::SRC1::ELEMENTS,&pDst1,FE::DST1::ELEMENTS,&pDst2,FE::DST2::ELEMENTS,&pDst3,FE::DST3::ELEMENTS,&pDst4,FE::DST4::ELEMENTS);
            }

            t.pThis->fe = fec;

            if( t.px_nRef )
                fe_1D_Ref( t.pThis->fe, pSrc1, pDst1, pDst2, pDst3,pDst4, t.px_nRef );
#endif
        }

        template< IsAlign ia > 
        ISV fe_1D_Pft( const ThreadData & t )
        {
            switch( B0<FE>::GetPft() )
            {
            case PFT_NORMAL:                fe_1D_Normal    < ia, STREAM_FLSE    >( t );                            
                                            break;
            case PFT_NORMAL_STREAM:         fe_1D_Normal    < ia, STREAM_TRUE    >( t );                            
                                            break;
            case PFT_EMULATE_CUSTOM:
            case PFT_CUSTOM:                fe_1D_Custom    < ia, true            >( t );
                                            break;

            case PFT_EMULATE_CUSTOM_NO_FB:
            case PFT_CUSTOM_NO_FALLBACK:    fe_1D_Custom    < ia, false            >( t );    
                                            break;
            default:                        assert(false);
            }
        }

        template< IsAlign ia > 
        SV fe_1D( const ThreadData & t )
        {
            fe_1D_Pft<ia>( t );
        }

        template< IsAlign ia > 
        SV fe_2D( ThreadData & t )
        {
            for(TD1 *end = FW_REF::Offset(t.pDst1,(T_OFFSET)(t.size)*(t.pThis->d1Step));
                                                                            t.pDst1!=end;
                                                    FW_REF::Inc(t.pSrc1,t.pThis->s1Step,
                                                                 t.pDst1,t.pThis->d1Step,
                                                                 t.pDst2,t.pThis->d2Step,
                                                                 t.pDst3,t.pThis->d3Step,
                                                                 t.pDst4,t.pThis->d4Step))
                fe_1D_Pft<ia>( t );
        }  

   private:
        IV SplitData_1D( ThreadData *td,  const TS1 *pSrc1,  TD1 *pDst1,  TD2 *pDst2, TD3 * pDst3, TD4 * pDst4,int len )
        {
            U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
            U32 px_nRef = this->width % this->fe.nPIX_SSE;
            for(U32 i=0; i<(this->threads); ++i)
            {
                td[i].pThis   = this;
                td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TS1) *(T_OFFSET)CH_COUNT(FE::CS1_) *(T_OFFSET)(lenTh));
                td[i].pDst1   = FW_REF::Offset(pDst1, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD1) *(T_OFFSET)CH_COUNT(FE::CD1_) *(T_OFFSET)(lenTh));
                td[i].pDst2   = FW_REF::Offset(pDst2, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD2) *(T_OFFSET)CH_COUNT(FE::CD2_) *(T_OFFSET)(lenTh));
                td[i].pDst3   = FW_REF::Offset(pDst3, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD3) *(T_OFFSET)CH_COUNT(FE::CD3_) *(T_OFFSET)(lenTh));
                td[i].pDst4   = FW_REF::Offset(pDst4, (T_OFFSET)(i)*(T_OFFSET)sizeof(TD4) *(T_OFFSET)CH_COUNT(FE::CD4_) *(T_OFFSET)(lenTh));
                td[i].size    = lenTh;                                                                 // 1D size=len
                td[i].widthTh = lenTh;
                td[i].px_nRef = 0;
            }
            td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
            td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
            td[(this->threads)-1].px_nRef = px_nRef;
        }

        IV SplitData_2D( ThreadData *td,  const TS1 *pSrc1,  TD1 *pDst1,  TD2 *pDst2, TD3 * pDst3, TD4 * pDst4, int height )
        {
            U32 heightTh = height / (this->threads);                                                  // height per thread
            U32 px_nRef = this->width % this->fe.nPIX_SSE;
            for(U32 i=0; i<(this->threads); ++i)
            {
                td[i].pThis   = this;
                td[i].pSrc1   = FW_REF::Offset(pSrc1, (T_OFFSET)(i)* (this->s1Step)*(T_OFFSET)(heightTh));
                td[i].pDst1   = FW_REF::Offset(pDst1, (T_OFFSET)(i)* (this->d1Step)*(T_OFFSET)(heightTh));
                td[i].pDst2   = FW_REF::Offset(pDst2, (T_OFFSET)(i)* (this->d2Step)*(T_OFFSET)(heightTh));
                td[i].pDst3   = FW_REF::Offset(pDst3, (T_OFFSET)(i)* (this->d3Step)*(T_OFFSET)(heightTh));
                td[i].pDst4   = FW_REF::Offset(pDst4, (T_OFFSET)(i)* (this->d3Step)*(T_OFFSET)(heightTh));
                td[i].size    = heightTh;
                td[i].widthTh = this->width - px_nRef;
                td[i].px_nRef = px_nRef;
            }
            td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
        } 

        IV Run( const TS1* pSrc1, TD1* pDst1,TD2 *pDst2, TD3 *pDst3,TD4 *pDst4, int size )
        {
            ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));             
            bool isAligned = false;

            switch( alg )
            {
            case ALG_1D:    SplitData_1D( td, pSrc1, pDst1, pDst2, pDst3,pDst4, size ); 
                            isAligned = FW_REF::IsAligned( pSrc1,0, pDst1,0, pDst2,0, pDst3,0, pDst4,0 ); 
                            break;
            case ALG_2D:    SplitData_2D( td, pSrc1, pDst1, pDst2, pDst3,pDst4, size ); 
                            isAligned = FW_REF::IsAligned( pSrc1, this->s1Step,
                                                            pDst1, this->s1Step,
                                                            pDst2, this->s2Step,
                                                            pDst3, this->d3Step,
                                                            pDst4, this->d4Step );
                            break;
            default:        assert(false);
            }
         
            isAligned   ? this->template BnRun< This, ALIGN_TRUE >( td )
                        : this->template BnRun< This, ALIGN_FLSE >( td );
        }  
      
    public:  
        B1S4D( FE & fe, const TS1* pSrc1,  TD1* pDst1, 
                                           TD2* pDst2,
                                           TD3 * pDst3,
                                           TD4 * pDst4, int len, U32 bytesPerThread )
            :B0< FE >( fe, len, bytesPerThread  )         
        {  
            STATIC_ASSERT( ALG_1D == alg );
            Run( pSrc1, pDst1, pDst2, pDst3,pDst4, len );              
        }
        B1S4D( FE & fe, const TS1* pSrc1,int s1Step,  TD1 * pDst1,int d1Step, 
                                                      TD2 * pDst2,int d2Step,
                                                      TD3 * pDst3, int d3Step,
                                                      TD4 * pDst4, int d4Step, ASZ roi, U32 bytesPerThread )
            :B0< FE >( fe, roi, bytesPerThread,s1Step, 0, 0,0,d1Step,d2Step,d3Step,d4Step)         
        {  
            STATIC_ASSERT( ALG_2D == alg );
            Run( pSrc1, pDst1, pDst2, pDst3,pDst4, roi.height );              
        }       
    };
}; // namespace FE_PRIVATE


// =========================================================
//            External Interface - 1D
// =========================================================



    template< class FE >
    SYS_INLINE FwStatus fe( FE & fe, typename FE::TD1_ *pDst, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
        if( FW_REF::PtrNotOK ( pDst   )) return fwStsNullPtrErr;    
        if( FW_REF::SizeNotOK( len    )) return fwStsSizeErr;
    
        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B1<FE, ALG_1D>( fe, pDst, len, bytesPerThread );
        return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fe( FE & fe, const typename FE::TS1_ *pSrc1, 
                                            typename FE::TD1_  *pDst, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
        if( FW_REF::PtrNotOK ( pSrc1, pDst )) return fwStsNullPtrErr;    
        if( FW_REF::SizeNotOK( len     )) return fwStsSizeErr;
   
        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B2<FE, ALG_1D>( fe, pSrc1, pDst, len, bytesPerThread );
        return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fe( FE & fe, const typename FE::TS1_ *pSrc1, 
                                      const typename FE::TS2_ *pSrc2,
                                            typename FE::TD1_  *pDst, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
        if( FW_REF::PtrNotOK ( pSrc1,pSrc2,pDst )) return fwStsNullPtrErr;    
        if( FW_REF::SizeNotOK( len        )) return fwStsSizeErr;
 
        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B3<FE, ALG_1D>( fe, pSrc1, pSrc2, pDst, len, bytesPerThread );
        return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fe( FE & fe, const typename FE::TS1_ *pSrc1, 
                                      const typename FE::TS2_ *pSrc2,
                                      const typename FE::TS3_ *pSrc3,
                                            typename FE::TD1_  *pDst, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
        if( FW_REF::PtrNotOK ( pSrc1,pSrc2,pSrc3,pDst )) return fwStsNullPtrErr;    
        if( FW_REF::SizeNotOK( len           )) return fwStsSizeErr;
   
        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B4<FE, ALG_1D>( fe, pSrc1, pSrc2, pSrc3, pDst, len, bytesPerThread );
        return fwStsNoErr;
    }


    template< class FE >
    SYS_INLINE FwStatus fec1S3D( FE & fe, const typename FE::TS1_ *pSrc1, 
                                                 typename FE::TD1_  *pDst1,
                                                 typename FE::TD2_  *pDst2,
                                                 typename FE::TD3_  *pDst3, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
        if( FW_REF::PtrNotOK ( pSrc1, pDst1,pDst2,pDst3 )) return fwStsNullPtrErr;    
        if( FW_REF::SizeNotOK( len     )) return fwStsSizeErr;
 
        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B1S3D<FE, ALG_1D>( fe, pSrc1, pDst1,pDst2,pDst3, len, bytesPerThread );
        return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fec1S4D( FE & fe, const typename FE::TS1_ *pSrc1, 
                                                 typename FE::TD1_  *pDst1,
                                                 typename FE::TD2_  *pDst2,
                                                 typename FE::TD3_  *pDst3,
                                                 typename FE::TD4_  *pDst4, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
        if( FW_REF::PtrNotOK ( pSrc1, pDst1,pDst2,pDst3,pDst4 )) return fwStsNullPtrErr;    
        if( FW_REF::SizeNotOK( len     )) return fwStsSizeErr;
 
        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B1S4D<FE, ALG_1D>( fe, pSrc1, pDst1,pDst2,pDst3,pDst4, len, bytesPerThread );
        return fwStsNoErr;
    }


    template< class FE >
    SYS_INLINE FwStatus fec4S1D( FE & fe, const typename FE::TS1_ *pSrc1, 
                                           const typename FE::TS2_ *pSrc2,
                                           const typename FE::TS3_ *pSrc3,
                                           const typename FE::TS4_ *pSrc4,
                                                 typename FE::TD1_  *pDst, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
        if( FW_REF::PtrNotOK ( pSrc1,pSrc2,pSrc3,pSrc4,pDst )) return fwStsNullPtrErr;    
        if( FW_REF::SizeNotOK( len           )) return fwStsSizeErr;
 
        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B4S1D<FE, ALG_1D>( fe, pSrc1, pSrc2, pSrc3,pSrc4, pDst, len, bytesPerThread );
        return fwStsNoErr;
    }


    template< class FE >
    SYS_INLINE FwStatus fec3S3D( FE & fe, const typename FE::TS1_ *pSrc1, 
                                           const typename FE::TS2_ *pSrc2, 
                                           const typename FE::TS3_ *pSrc3, 
                                                 typename FE::TD1_  *pDst1,
                                                 typename FE::TD2_  *pDst2,
                                                 typename FE::TD3_  *pDst3, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
        if( FW_REF::PtrNotOK ( pSrc1,pSrc2,pSrc3, pDst1,pDst2,pDst3 )) return fwStsNullPtrErr;    
        if( FW_REF::SizeNotOK( len     )) return fwStsSizeErr;
 
        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B3S3D<FE, ALG_1D>( fe, pSrc1,pSrc2,pSrc3, pDst1,pDst2,pDst3, len, bytesPerThread );
        return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fec4S4D( FE & fe, const typename FE::TS1_ *pSrc1, 
                                           const typename FE::TS2_ *pSrc2, 
                                           const typename FE::TS3_ *pSrc3, 
                                           const typename FE::TS4_ *pSrc4, 
                                                 typename FE::TD1_  *pDst1,
                                                 typename FE::TD2_  *pDst2,
                                                 typename FE::TD3_  *pDst3,
                                                 typename FE::TD4_  *pDst4, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
        if( FW_REF::PtrNotOK ( pSrc1,pSrc2,pSrc3,pSrc4, pDst1,pDst2,pDst3,pDst4 )) return fwStsNullPtrErr;    
        if( FW_REF::SizeNotOK( len     )) return fwStsSizeErr;
 
        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B4S4D<FE, ALG_1D>( fe, pSrc1,pSrc2,pSrc3,pSrc4, pDst1,pDst2,pDst3,pDst4, len, bytesPerThread );
        return fwStsNoErr;
     }

    // =========================================================
    //            External Interface - 2D
    // =========================================================

    template< class FE >
    SYS_INLINE FwStatus fe( FE & fe, typename FE::TD1_ *pDst, int d1Step, ASZ roi, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
         if( FW_REF::PtrNotOK ( pDst     ) ) return fwStsNullPtrErr;   
         if( FW_REF::StepZeroNotOK( d1Step ) ) return fwStsStepErr;
         if( FW_REF::RoiNotOK ( roi   ) ) return fwStsSizeErr;

         PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B1<FE, ALG_2D>( fe, pDst, d1Step, roi, bytesPerThread );
         return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fe( FE & fe, const typename FE::TS1_ *pSrc1, int s1Step, 
                                            typename FE::TD1_  *pDst,  int d1Step, ASZ roi, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {   
        if( FW_REF::PtrNotOK ( pSrc1    ,pDst     ) ) return fwStsNullPtrErr;   
        if( FW_REF::StepZeroNotOK( s1Step,d1Step ) ) return fwStsStepErr;
        if( FW_REF::RoiNotOK ( roi          ) ) return fwStsSizeErr;   

        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B2<FE, ALG_2D>( fe, pSrc1, s1Step, pDst, d1Step, roi, bytesPerThread );
        return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fe( FE & fe, const typename FE::TS1_ *pSrc1, int s1Step, 
                                      const typename FE::TS2_ *pSrc2, int s2Step, 
                                            typename FE::TD1_   *pDst, int d1Step, ASZ roi, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {    
        if( FW_REF::PtrNotOK ( pSrc1,    pSrc2,    pDst     ) ) return fwStsNullPtrErr;   
        if( FW_REF::StepZeroNotOK( s1Step,s2Step,d1Step ) ) return fwStsStepErr;
        if( FW_REF::RoiNotOK ( roi                 ) ) return fwStsSizeErr;

        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B3<FE, ALG_2D>( fe, pSrc1,s1Step, pSrc2,s2Step, pDst,d1Step, roi, bytesPerThread );
        return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fe( FE & fe, const typename FE::TS1_ *pSrc1, int s1Step, 
                                      const typename FE::TS2_ *pSrc2, int s2Step, 
                                      const typename FE::TS3_ *pSrc3, int s3Step, 
                                            typename FE::TD1_   *pDst, int d1Step, ASZ roi, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {    
        if( FW_REF::PtrNotOK ( pSrc1,    pSrc2,    pSrc3,    pDst     ) ) return fwStsNullPtrErr;   
        if( FW_REF::StepZeroNotOK( s1Step,s2Step,s3Step,d1Step ) ) return fwStsStepErr;
        if( FW_REF::RoiNotOK ( roi                        ) ) return fwStsSizeErr;

        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B4<FE, ALG_2D>( fe, pSrc1,s1Step, pSrc2,s2Step, pSrc3,s3Step, pDst,d1Step, roi, bytesPerThread );
        return fwStsNoErr;
    }


    template< class FE >
    SYS_INLINE FwStatus fec1S3D( FE & fe, const typename FE::TS1_ *pSrc1, int s1Step, 
                                      typename FE::TD1_ *pDst1, int d1Step, 
                                      typename FE::TD2_ *pDst2, int d2Step, 
                                       typename FE::TD3_ *pDst3, int d3Step, ASZ roi, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {    
        if( FW_REF::PtrNotOK ( pSrc1,    pDst1,    pDst2,    pDst3     ) ) return fwStsNullPtrErr;   
        if( FW_REF::StepZeroNotOK( s1Step,d1Step,d2Step,d3Step ) ) return fwStsStepErr;
        if( FW_REF::RoiNotOK ( roi                        ) ) return fwStsSizeErr;

        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B1S3D<FE, ALG_2D>( fe, pSrc1,s1Step, pDst1,d1Step, pDst2,d2Step, pDst3,d3Step, roi, bytesPerThread );
        return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fec1S4D( FE & fe, const typename FE::TS1_ *pSrc1, int s1Step, 
                                                 typename FE::TD1_ *pDst1, int d1Step, 
                                                 typename FE::TD2_ *pDst2, int d2Step, 
                                                 typename FE::TD3_ *pDst3, int d3Step, 
                                                 typename FE::TD4_ *pDst4, int d4Step,ASZ roi, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {    
        if( FW_REF::PtrNotOK ( pSrc1,    pDst1,    pDst2,    pDst3 ,pDst4    ) ) return fwStsNullPtrErr;   
        if( FW_REF::StepZeroNotOK( s1Step,d1Step,d2Step,d3Step,d4Step ) ) return fwStsStepErr;
        if( FW_REF::RoiNotOK ( roi                        ) ) return fwStsSizeErr;

        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B1S4D<FE, ALG_2D>( fe, pSrc1,s1Step, pDst1,d1Step, pDst2,d2Step, pDst3,d3Step,pDst4,d4Step,  roi, bytesPerThread );
        return fwStsNoErr;
    }


    template< class FE >
    SYS_INLINE FwStatus fec4S1D( FE & fe, const typename FE::TS1_ *pSrc1, int s1Step, 
                                           const typename FE::TS2_ *pSrc2, int s2Step, 
                                           const typename FE::TS3_ *pSrc3, int s3Step, 
                                           const typename FE::TS4_ *pSrc4, int s4Step, 
                                                 typename FE::TD1_   *pDst, int d1Step, ASZ roi, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {    
        if( FW_REF::PtrNotOK ( pSrc1,    pSrc2,    pSrc3,pSrc4,    pDst     ) ) return fwStsNullPtrErr;   
        if( FW_REF::StepZeroNotOK( s1Step,s2Step,s3Step,s4Step,d1Step ) ) return fwStsStepErr;
        if( FW_REF::RoiNotOK ( roi                        ) ) return fwStsSizeErr;

        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B4S1D<FE, ALG_2D>( fe, pSrc1,s1Step, pSrc2,s2Step, pSrc3,s3Step,pSrc4,s4Step, pDst,d1Step, roi, bytesPerThread );
        return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fec3S3D( FE & fe, const typename FE::TS1_ *pSrc1, int s1Step, 
                                           const typename FE::TS2_ *pSrc2, int s2Step, 
                                           const typename FE::TS3_ *pSrc3, int s3Step, 
                                                 typename FE::TD1_ *pDst1, int d1Step, 
                                                 typename FE::TD2_ *pDst2, int d2Step, 
                                                 typename FE::TD3_ *pDst3, int d3Step, ASZ roi, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {    
        if( FW_REF::PtrNotOK ( pSrc1,pSrc2,pSrc3,    pDst1,    pDst2,    pDst3     ) ) return fwStsNullPtrErr;   
        if( FW_REF::StepZeroNotOK( s1Step,s2Step,s3Step,d1Step,d2Step,d3Step ) ) return fwStsStepErr;
        if( FW_REF::RoiNotOK ( roi                        ) ) return fwStsSizeErr;

        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B3S3D<FE, ALG_2D>( fe, pSrc1,s1Step,pSrc2,s2Step,pSrc3,s3Step, pDst1,d1Step, pDst2,d2Step, pDst3,d3Step, roi, bytesPerThread );
        return fwStsNoErr;
    }


    template< class FE >
    SYS_INLINE FwStatus fec4S4D( FE & fe, const typename FE::TS1_ *pSrc1, int s1Step, 
                                           const typename FE::TS2_ *pSrc2, int s2Step, 
                                           const typename FE::TS3_ *pSrc3, int s3Step, 
                                           const typename FE::TS4_ *pSrc4, int s4Step, 
                                                 typename FE::TD1_ *pDst1, int d1Step, 
                                                 typename FE::TD2_ *pDst2, int d2Step, 
                                                 typename FE::TD3_ *pDst3, int d3Step,
                                                 typename FE::TD4_ *pDst4, int d4Step,ASZ roi, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {    
        if( FW_REF::PtrNotOK ( pSrc1,pSrc2,pSrc3,pSrc4,    pDst1,    pDst2,    pDst3,pDst4     ) ) return fwStsNullPtrErr;   
        if( FW_REF::StepZeroNotOK( s1Step,s2Step,s3Step,s4Step,d1Step,d2Step,d3Step,d4Step ) ) return fwStsStepErr;
        if( FW_REF::RoiNotOK ( roi                        ) ) return fwStsSizeErr;

        PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B4S4D<FE, ALG_2D>( fe, pSrc1,s1Step,pSrc2,s2Step,pSrc3,s3Step,pSrc4,s4Step, pDst1,d1Step, pDst2,d2Step, pDst3,d3Step,pDst4,d4Step, roi, bytesPerThread );
        return fwStsNoErr;
    }
} // namespace OPT_LEVEL

//#include "fe_fix.h"

#endif // __FE_H__

