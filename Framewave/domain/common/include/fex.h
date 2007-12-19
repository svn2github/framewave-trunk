/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FEX_H__
#define __FEX_H__

#include "fe.h"

#define THREAD_SW_OFF 536870910 //switch off thread 
#define XMMREGSIZE 16           // 16 bytes

namespace OPT_LEVEL
{


#define FEX_F10H_PFT_EMULATE		const static PxFnType f10hPft = PFT_NORMAL;									\
																	IV F10H( RegFile &r ) { this->SSE2(r); }			\
																	IV F10H_Init()		  { this->SSE2_Init(); }		\
																	template<class FE>							\
																	IV F10H_Post( FE &f ) { this->template SSE2_Post<FE>(f); } 

#define FEX_F10H_PFT_EMULATE_STREAM	const static PxFnType f10hPft = PFT_NORMAL_STREAM; 							\
																	IV F10H( RegFile &r ) { this->SSE2(r); }			\
																	IV F10H_Init()		  { this->SSE2_Init(); }		\
																	template<class FE>							\
																	IV F10H_Post( FE &f ) { this->template SSE2_Post<FE>(f); }


#define FEX_SSE2_REF				FEX_F10H_PFT_EMULATE		\
									FE_SSE2_PFT_NORMAL			\
									FE_REFR_PFT_NORMAL

#define FEX_STREAM_SSE2_REF			FEX_F10H_PFT_EMULATE_STREAM	\
									FE_SSE2_PFT_NORMAL_STREAM	\
									FE_REFR_PFT_NORMAL

#define FEX_CUSTOM_SSE2_REF			FE_F10H_PFT_CUSTOM			\
									FE_SSE2_PFT_CUSTOM			\
									FE_REFR_PFT_NORMAL



struct feBaseSt : public feBase
{
	IV F10H( RegFile &r ) const { SSE2(r); }
	IV SSE2( RegFile & )  const { assert(false); }

	IV F10H_Init() { SSE2_Init(); }
	IV SSE2_Init() {}
    IV REFR_Init() {}           // Initialization function for reference path. Will be called for each thread.

    template<class FE>
    IV F10H_Post(FE &f) { SSE2_Post(f); }       // Cleanup function for f10h path. will be called after the execution of all the threads.
    template<class FE>
    IV SSE2_Post(FE &) {}       // Cleanup function for sse2 path. will be called after the execution of all the threads.
    template<class FE>          // Cleanup function for ref path. will be called after the execution of all the threads.
    IV REFR_Post(FE &) {}
};


template< typename TS1, CH cs1>
struct fe1St : public feBaseSt
{
   typedef TS1 TS1_;
   const static CH CS1_      = cs1;
   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TS1_,CS1_ ),(U32)16 );

   class SRC1: public RegDesc< TS1_, CS1_, nPIX_SSE > {};
   
   IV REFR(                              const TS1_ *)         { assert(false); }


};


template< typename TS1, CH cs1, 
          typename TS2, CH cs2 >
struct fe2St : public feBaseSt
{   
   typedef TS1 TS1_;
   typedef TS2 TS2_;

   const static CH CS1_= cs1;
   const static CH CS2_= cs2;

   
   const static U32 nPIX_SSE = M_MIN( PIX_COUNT( TS1_,CS1_ ), 
                                      PIX_COUNT( TS2_,CS2_ ));

   class SRC1: public RegDesc< TS1_, CS1_, nPIX_SSE > {};   
   class SRC2: public RegDesc< TS2_, CS2_, nPIX_SSE > {};
      
   IV REFR(                              const TS1_ *, const TS2_ *)        { assert(false); }

};

namespace PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)
{
   template< class FE >                                             // Common data / initialization routines
   class B0St
   {
   protected:
      mutable FE &fe;                                               // Algorithm instance
      const U32 width;   
      const S32 s1Step, s2Step, s3Step,s4Step; 
      U32 threads;   
  
      SYS_INLINE
      B0St( FE & fe_, ASZ roi_, U32 bytesPerThread_, U32 s1Stp, U32 s2Stp, U32 s3Stp,U32 s4Stp)
       :fe     ( fe_        )
       ,width  ( roi_.width )
       ,s1Step ( s1Stp      )
       ,s2Step ( s2Stp      )
       ,s3Step ( s3Stp      )
       ,s4Step ( s4Stp      )
       ,threads( RecommendThreads( s1Step*roi_.height, bytesPerThread_ ) )
      {}

      SYS_INLINE
      B0St( FE & fe_, int len, U32 bytesPerThread_ )
       :fe     ( fe_ )
       ,width  ( len )
       ,s1Step ( 1   )
       ,s2Step ( 1   )
       ,s3Step ( 1   )
       ,s4Step ( 1   )
       ,threads( RecommendThreads( len*sizeof(typename FE::TS1_), bytesPerThread_ ) )
      {}

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
		  case ALG_1D:	FeThread< Bn, ia >::fe_1DT( td ); break;
		  case ALG_2D:	FeThread< Bn, ia >::fe_2DT( td ); break;
		  default:		assert(false);
		  }
	  }


	  template< class Bn, IsAlign ia >
	  SYS_INLINE void BnRun( typename Bn::ThreadData *td )
	  {
		  BnRunAlg< Bn, ia >( td );
          FePost<Bn>(td);                     // Call the post function when all the threads have completed execution.
      }

      template<class Bn>
      SYS_INLINE void FeInit( const typename Bn::ThreadData &td ) const
      {
#if   defined (OPT_F10H) || defined (OPT_SSE3)
            td.fe.F10H_Init();
#elif defined (OPT_SSE2)
            td.fe.SSE2_Init();
#elif defined (OPT_REFR)
            td.fe.REFR_Init();
#endif
      }


		template<class Bn>
		SYS_INLINE void FePost( typename Bn::ThreadData *td) const        // call the post function for each thread.
		{
#if   defined (OPT_F10H) || defined (OPT_SSE3)
			for(U32 i = 0 ; i < td->pThis->threads; i++) fe.template F10H_Post<FE>(td[i].fe);
#elif defined (OPT_SSE2)
			for(U32 i = 0 ; i < td->pThis->threads; i++) fe.template SSE2_Post<FE>(td[i].fe);
#elif defined (OPT_REFR)
			for(U32 i = 0 ; i < td->pThis->threads; i++) fe.template REFR_Post<FE>(td[i].fe);
#endif
		}

   public:
       SYS_INLINE U32 Threads() const { return threads; }
   };


   // -------- ForEach 1 Buffers --------
   template< class FE, FE_ALGORITHM alg >
   class B1St : public B0St< FE >
   {    
      friend class B0St<FE>;
      const static FE_ALGORITHM alg_ = alg;
      typedef B1St<FE,alg>        This;
      typedef typename FE::TS1_ TS1;


   public:
      struct ThreadData                                     // Dynamic thread data
      {
         const This *pThis;
         const TS1  *s1;
         mutable FE fe;
         U32 size;
         U32 widthTh;
         U32 px_nRef;
       }; 

      ISV fe_1D_Ref( FE &fe, const TS1 *s1, U32 count)
      {
          const TS1 * end = s1 + (count * CH_COUNT(FE::CS1_));
          for( ;s1<end; s1 += CH_COUNT(FE::CS1_) )
               fe.REFR( s1 );
      }

      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
		fe_1D_Ref( t.fe, t.s1, t.widthTh + t.px_nRef);
		return;
#else
         const TS1 *s1 = t.s1;

        for( const TS1 * end = s1 + (t.widthTh) * CH_COUNT(FE::CS1_);
                s1 <end;)
            {
                RegFile r;
                t.fe.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, s1 );

#if   defined(OPT_F10H) || defined(OPT_SSE3)
				t.fe.F10H( r );
#elif   defined(OPT_SSE2)
				t.fe.SSE2( r );
#else
			assert(false);  
#endif
                t.fe.IncrementBuffers(&s1,FE::SRC1::ELEMENTS);
            }
 

         if( t.px_nRef )
            fe_1D_Ref( t.fe, s1, t.px_nRef);
#endif
      }

      template< IsAlign ia > 
      ISV fe_1D_Pft( const ThreadData & t )
      {
		  switch( B0St<FE>::GetPft() )
		  {
		  case PFT_NORMAL:								fe_1D_Normal	< ia, STREAM_FLSE	>( t ); break;
		  case PFT_NORMAL_STREAM:						fe_1D_Normal	< ia, STREAM_TRUE	>( t );	break;
          default:                  assert(false);
          }
      }

      template< IsAlign ia > 
      SV fe_1D( const ThreadData & t )
      {
          t.pThis->template FeInit<B1St>(t);
          fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
         t.pThis->template FeInit<B1St>(t);
         for( const TS1 *end = FW_REF::Offset(t.s1,(T_OFFSET)t.size*(t.pThis->s1Step));
              t.s1!=end; 
              FW_REF::Inc(t.s1,t.pThis->s1Step) 
            ) 
            fe_1D_Pft<ia>( t );
      }

   private:
      IV SplitData_1D( ThreadData *td, const TS1 *s1, int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this; 
            td[i].fe      = this->fe;
            td[i].s1      = FW_REF::Offset(s1, (T_OFFSET)i*(T_OFFSET)sizeof(TS1)*(T_OFFSET)CH_COUNT(FE::CS1_)*(T_OFFSET)lenTh);
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }

         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      } 

      IV SplitData_2D( ThreadData *td, const TS1 *s1, int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef  = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this; 
            td[i].s1      = FW_REF::Offset(s1, (T_OFFSET)i*(this->s1Step)*(T_OFFSET)heightTh);
            td[i].size    = heightTh;
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
      } 

      IV Run( const TS1* s1, int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));
            bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, s1 , size ); isAligned = FW_REF::IsAligned( s1,0);            break;
         case ALG_2D:    SplitData_2D( td, s1 , size ); isAligned = FW_REF::IsAligned( s1,this->s1Step); break;
         default:        assert(false);
         }

         isAligned  ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );
         
      }

   public:  
      
      B1St( FE & fe, const TS1* s1,int len, U32 bytesPerThread )
       :B0St< FE >( fe, len, bytesPerThread )
      {  
         STATIC_ASSERT( ALG_1D == alg );
         Run( s1,len );
      }

      
      B1St( FE & fe, const TS1* s1, int s1Step,ASZ roi, U32 bytesPerThread )
       :B0St< FE >( fe, roi, bytesPerThread,s1Step,0,0,0)
      {   
         STATIC_ASSERT( ALG_2D == alg );
         Run( s1,roi.height );
      }
   };

  // -------- ForEach 2 Buffers --------
   template< class FE, FE_ALGORITHM alg >
   class B2St : public B0St< FE >
   {     
      friend class B0St<FE>;
      const static FE_ALGORITHM alg_ = alg;
      typedef B2St<FE,alg>        This;
      typedef typename FE::TS1_ TS1;
      typedef typename FE::TS2_ TS2;

        
   public:
      struct ThreadData                                     // Dynamic thread data
      { 
         const This *pThis;
         const TS1  *s1;
         const TS2  *s2;
         mutable FE fe;
         U32 size;
         U32 widthTh;
         U32 px_nRef;
       };

      ISV fe_1D_Ref(FE &fe, const TS1 *s1, const TS2 *s2, U32 count)
      {
          const TS1 * end = s1 + (count * CH_COUNT(FE::CS1_));
          for( ; s1<end;s1 += CH_COUNT(FE::CS1_),
                        s2 += CH_COUNT(FE::CS2_))
                fe.REFR( s1, s2);
      }

      template< IsAlign ia, IsStream is >
      ISV fe_1D_Normal( const ThreadData &t )
      {
#if defined(OPT_REFR)
		fe_1D_Ref( t.fe, t.s1, t.s2, t.widthTh + t.px_nRef);
		return;
#else

         const TS1  *s1 = t.s1;
         const TS2  *s2 = t.s2;

        for( const typename FE::TS1_ * end = s1 + (t.widthTh) * CH_COUNT(FE::CS1_);
            s1 <end;)
        {
                RegFile r;
                t.fe.template LOAD< FE::SRC1::BYTES,DISPATCH,ia,is >( r.src1, s1 );
                t.fe.template LOAD< FE::SRC2::BYTES,DISPATCH,ia,is >( r.src2, s2 );

#if   defined(OPT_F10H) || defined(OPT_SSE3)
				t.fe.F10H( r );
#elif   defined(OPT_SSE2)
				t.fe.SSE2( r );
#else
			assert(false);  
#endif
                t.fe.IncrementBuffers(&s1,FE::SRC1::ELEMENTS,&s2,FE::SRC2::ELEMENTS);
        }


         if( t.px_nRef )
            fe_1D_Ref( t.fe, s1, s2,t.px_nRef);
#endif
      }

      template< IsAlign ia > 
      ISV fe_1D_Pft( const ThreadData & t )
      {
		  switch( B0St<FE>::GetPft() )
		  {
		  case PFT_NORMAL:								fe_1D_Normal	< ia, STREAM_FLSE	>( t ); break;
		  case PFT_NORMAL_STREAM:						fe_1D_Normal	< ia, STREAM_TRUE	>( t );	break;
          default:                  assert(false);
          }
      }

      template< IsAlign ia > 
      SV fe_1D( const ThreadData & t )
      {
          t.pThis->template FeInit<B2St>(t);
          fe_1D_Pft<ia>( t );
      }

      template< IsAlign ia > 
      SV fe_2D( ThreadData & t )
      {
         t.pThis->template FeInit<B2St>(t);
         for( const TS1 *end = FW_REF::Offset(t.s1,(T_OFFSET)t.size*(t.pThis->s1Step));
              t.s1!=end; 
              FW_REF::Inc(t.s1,t.pThis->s1Step, t.s2,t.pThis->s2Step) 
            )
            fe_1D_Pft<ia>( t );
      }        

   private:
      IV SplitData_1D( ThreadData *td, const TS1 *s1, const TS2 *s2, int len )
      {
         U32 lenTh = (len / (this->fe.nPIX_SSE * this->threads)) * this->fe.nPIX_SSE;              // size per thread
         U32 px_nRef = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].fe      = this->fe;
            td[i].s1      = FW_REF::Offset(s1, (T_OFFSET)i*(T_OFFSET)sizeof(TS1)*(T_OFFSET)CH_COUNT(FE::CS1_)*(T_OFFSET)lenTh);
            td[i].s2      = FW_REF::Offset(s2, (T_OFFSET)i*(T_OFFSET)sizeof(TS2)*(T_OFFSET)CH_COUNT(FE::CS2_)*(T_OFFSET)lenTh);
            td[i].size    = lenTh;                                                                 // 1D size=len
            td[i].widthTh = lenTh;
            td[i].px_nRef = 0;
         }
         td[(this->threads)-1].size    = len - ((this->threads)-1)*lenTh;                          // Last thread
         td[(this->threads)-1].widthTh = len - ((this->threads)-1)*lenTh - px_nRef;
         td[(this->threads)-1].px_nRef = px_nRef;
      } 

      IV SplitData_2D( ThreadData *td, const TS1 *s1, const TS2 *s2, int height )
      {
         U32 heightTh = height / (this->threads);                                                  // height per thread
         U32 px_nRef  = this->width % this->fe.nPIX_SSE;
         for(U32 i=0; i<(this->threads); ++i)
         {
            td[i].pThis   = this;
            td[i].s1      = FW_REF::Offset(s1, (T_OFFSET)i*(this->s1Step)*(T_OFFSET)heightTh);
            td[i].s2      = FW_REF::Offset(s2, (T_OFFSET)i*(this->s2Step)*(T_OFFSET)heightTh);
            td[i].size    = heightTh;
            td[i].widthTh = this->width - px_nRef;
            td[i].px_nRef = px_nRef;
         }
         td[(this->threads)-1].size = height - ((this->threads)-1)*heightTh;                       // Last thread
      }   

      IV Run( const TS1* s1, const TS2* s2, int size )
      {
         ThreadData * const td = (ThreadData*)alloca( (this->threads) * sizeof(ThreadData));
         bool isAligned = false;

         switch( alg )
         {
         case ALG_1D:    SplitData_1D( td, s1, s2,size ); isAligned = FW_REF::IsAligned( s1,0,            s2,0);            break;
         case ALG_2D:    SplitData_2D( td, s1, s2,size ); isAligned = FW_REF::IsAligned( s1,this->s1Step, s2,this->s2Step); break;
         default:        assert(false);
         }

         isAligned  ? this->template BnRun< This, ALIGN_TRUE >( td )
                    : this->template BnRun< This, ALIGN_FLSE >( td );

      }

   public:  
      
      B2St( FE & fe, const TS1* s1, 
                     const TS2* s2, int len, U32 bytesPerThread )
       :B0St< FE >( fe, len, bytesPerThread )
      {   
         STATIC_ASSERT( ALG_1D == alg );
         Run( s1, s2,len );
      }

      
      B2St( FE & fe, const TS1* s1, int s1Step, 
                     const TS2* s2, int s2Step,ASZ roi, U32 bytesPerThread )
       :B0St< FE >( fe, roi, bytesPerThread,s1Step, s2Step,0,0)
      {   
         STATIC_ASSERT( ALG_2D == alg );
         Run( s1, s2,roi.height );
      }
   };

   }

template< class FE >
    SYS_INLINE FwStatus fex( FE & fe, const typename FE::TS1_ *s1, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
       if( FW_REF::PtrNotOK ( s1  )) return fwStsNullPtrErr;
       if( FW_REF::SizeNotOK( len )) return fwStsSizeErr;

       PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B1St<FE, ALG_1D>( fe, s1, len, bytesPerThread );
       return fwStsNoErr;
    }

    template< class FE >
    SYS_INLINE FwStatus fex( FE & fe, const typename FE::TS1_ *s1, 
                                       const typename FE::TS2_ *s2, int len, U32 bytesPerThread=DEF_BYTES_PER_THREAD )
    {
       if( FW_REF::PtrNotOK ( s1, s2 )) return fwStsNullPtrErr;
       if( FW_REF::SizeNotOK( len    )) return fwStsSizeErr;

       PREFIX_OPT(OPT_PREFIX, FE_PRIVATE)::B2St<FE, ALG_1D>( fe, s1, s2, len, bytesPerThread );
       return fwStsNoErr;
    }


}
#endif // __FEX_H__

