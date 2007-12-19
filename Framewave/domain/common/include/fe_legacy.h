/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FE_LEGACY_H__
#define __FE_LEGACY_H__
#include "fe.h"

#pragma warning( push )
#pragma warning( disable: 4100 )

//=========================================================
//                LEGACY BASE
//=========================================================

namespace OPT_LEVEL
{

template< class TD, CH cd >
struct Legacy_B1_BASE : public feDat1< TD, cd >
{
    FnDat dat;
    Legacy_B1_BASE( FnDat & dat_ ):dat(dat_){}
    
    template< DispatchType dt, MemAlign ma >
    IV Custom1D( const TD *d, int count ) const{}
};

template< class TS, CH cs,
          class TD, CH cd >
struct Legacy_B2_BASE : public feDat2< TS, cs, TD, cd >
{
    FnDat dat;
    Legacy_B2_BASE( FnDat & dat_ ):dat(dat_){}

    template< DispatchType dt, MemAlign ma >
    IV Custom1D ( const TS *s, const TD *d, int count ) const{}
};

template< class TS1, CH cs1,
          class TS2, CH cs2,
          class TD , CH cd >
struct Legacy_B3_BASE : public feDat3< TS1, cs1, TS2, cs2, TD, cd >
{
    FnDat dat;
    Legacy_B3_BASE( FnDat & dat_ ):dat(dat_){}
    
    template< DispatchType dt, MemAlign ma >
    IV Custom1D (const TS1 *s1, const TS2 *s2, TD *d,int count) const{}
    
};

template< class TS1, CH cs1,
          class TS2, CH cs2,
          class TS3, CH cs3,
          class TD , CH cd >
struct Legacy_B4_BASE : public feDat4< TS1, cs1, TS2, cs2, TS3, cs3, TD, cd >
{        
    FnDat dat;
    Legacy_B4_BASE( FnDat & dat_ ):dat(dat_){}
    
    template< DispatchType dt, MemAlign ma >
    IV Custom1D (const TS1 *s1, const TS2 *s2,
                 const TS3 *s3,       TD  *d , int count ) const{}
};

//=========================================================
//                fe_legacy Entry points
//=========================================================

template< RegSetting rs, class TD, CH cd,    
          void (*pix_ref )( const FnDat &, TD*, CH ),
          void (&pix_sse )( const FnDat &, RegDescriptor & ),
          void (&pix_sse_setup) ( FnDat & ) >

SYS_INLINE static FwStatus 
fe_legacy( TD  *d, int dStep, FwiSize &roi, FnDat& dat=g_dat, unsigned int min_bytes_per_thread=DEF_BYTES_PER_THREAD)
{
    struct Tfe1 : public Legacy_B1_BASE< TD, cd >
    {   PX_DEF_SSE3
        Tfe1( FnDat & dat_ ):Legacy_B1_BASE(dat_){}
        IV SSE2_Init(                   )       {                             pix_sse_setup( dat    ); }
        IV SSE2     ( RegDescriptor & r ) const { r.dstRegCount=dst.regCount; pix_sse      ( dat, r ); }
        IV REFR     ( RegDescriptor & r ) const
        {
                  TD *d   = (TD*) r.dst ;
            const TD *end = d + px_ref_count  *CH_COUNT(cd);
            for( ; d<end; d+=CH_COUNT(cd)) 
                pix_ref( dat, d,cd );
        }        
    } tfe1( dat );

    return B1<  Tfe1, VAR_NORMAL, TD, cd  >::fe( tfe1, d, dStep, roi, min_bytes_per_thread );
}

// 2 Buffer
template< RegSetting rs, class TS, CH cs,
                         class TD, CH cd,
          void (*pix_ref )( const FnDat &, const TS*,CH, TD*,CH ),
          void (&pix_sse )( const FnDat &, RegDescriptor & ),
          void (&pix_sse_setup) ( FnDat & ) >
SYS_INLINE static FwStatus 
fe_legacy( const TS *s, int sStep, 
                 TD *d, int dStep, FwiSize &roi, FnDat& dat=g_dat, unsigned int min_bytes_per_thread=DEF_BYTES_PER_THREAD)
{
    struct Tfe2 : public Legacy_B2_BASE< TS,cs, TD,cd >
    {   PX_DEF_SSE3
        Tfe2( FnDat & dat_ ):Legacy_B2_BASE(dat_){}
        IV SSE2_Init(                   )      {                             pix_sse_setup( dat    ); }
        IV SSE2     ( RegDescriptor & r ) const{ r.dstRegCount=dst.regCount; pix_sse      ( dat, r ); }
        IV REFR     ( RegDescriptor & r ) const
        {
            const TS *s   = (TS*) r.src1;
                  TD *d   = (TD*) r.dst ;
            const TD *end = d + px_ref_count  *CH_COUNT(cd);
            for( ; d<end; d+=CH_COUNT(cd), 
                          s+=CH_COUNT(cs)) 
                pix_ref( dat, s,cs, d,cd );
        }        
    } tfe2( dat );

    return B2< Tfe2, VAR_NORMAL, TS, cs, TD, cd >::fe( tfe2, s, sStep, d, dStep, roi, min_bytes_per_thread );
}


// 3 Buffer
template< RegSetting rs, class TS1, CH cs1,
                         class TS2, CH cs2,
                         class TD , CH cd ,
          void (*pix_ref )( const FnDat &, const TS1*,CH, const TS2*,CH, TD*,CH ),
          void (&pix_sse )( const FnDat &, RegDescriptor & ),
          void (&pix_sse_setup) ( FnDat & ) >
SYS_INLINE static FwStatus 
fe_legacy( const TS1  *s1, int s1Step, 
           const TS2  *s2, int s2Step,
                 TD   *d , int dStep ,FwiSize &roi,FnDat& dat=g_dat,unsigned int min_bytes_per_thread=DEF_BYTES_PER_THREAD)
{
    struct Tfe3 : public Legacy_B3_BASE< TS1,cs1, TS2,cs2, TD,cd >
    {    PX_DEF_SSE3
        Tfe3( FnDat & dat_ ):Legacy_B3_BASE(dat_){}    
        IV SSE2_Init(                   )       {                             pix_sse_setup( dat    ); }
        IV SSE2     ( RegDescriptor & r ) const { r.dstRegCount=dst.regCount; pix_sse      ( dat, r ); }
        IV REFR     ( RegDescriptor & r ) const
        {
            const TS1 *s1  = (TS1*) r.src1;
                  TS2 *s2  = (TS2*) r.src2;
                  TD  *d   = (TD *) r.dst ;
            const TD  *end = d + px_ref_count  *CH_COUNT(cd);
            for( ; d<end; d +=CH_COUNT(cd ), 
                          s1+=CH_COUNT(cs1),
                          s2+=CH_COUNT(cs2)) 
                pix_ref( dat, s1,cs1, s2,cs2, d,cd );
        }    
    } tfe3( dat );

    return B3< Tfe3, VAR_NORMAL, TS1, cs1, TS2, cs2, TD, cd
             >::fe( tfe3, s1, s1Step, s2, s2Step, d, dStep, roi, min_bytes_per_thread );
}


// 4 Buffer
template< RegSetting rs, class TS1, CH cs1,
                         class TS2, CH cs2,
                         class TS3, CH cs3,
                         class TD , CH cd ,
          void (*pix_ref )( const FnDat &, const TS1*,CH, const TS2*,CH, const TS3*,CH, TD*,CH ),
          void (&pix_sse )( const FnDat &dat, RegDescriptor &reg ),
          void (&pix_sse_setup) ( FnDat &dat ) >
SYS_INLINE static FwStatus 
fe_legacy( const TS1  *s1, int s1Step, 
           const TS2  *s2, int s2Step,
           const TS3  *s3, int s3Step,
                 TD   *d , int dStep ,FwiSize &roi,FnDat& dat=g_dat,unsigned int min_bytes_per_thread=DEF_BYTES_PER_THREAD)
{
    struct Tfe4 : public Legacy_B4_BASE< TS1,cs1, TS2,cs2, TS3,cs3, TD,cd >
    {   PX_DEF_SSE3
        Tfe4( FnDat & dat_ ):Legacy_B4_BASE(dat_){}
        IV SSE2_Init(                   )       {                             pix_sse_setup( dat    ); }
        IV SSE2     ( RegDescriptor & r ) const { r.dstRegCount=dst.regCount; pix_sse      ( dat, r ); }
        IV REFR     ( RegDescriptor & r ) const
        {
            const TS1 *s1  = (TS1*) r.src1;
                  TS2 *s2  = (TS2*) r.src2;
                  TS3 *s3  = (TS3*) r.src3;
                  TD  *d   = (TD *) r.dst ;
            const TD  *end = d + px_ref_count  *CH_COUNT(cd);
            for( ; d<end; d +=CH_COUNT(cd ), 
                          s1+=CH_COUNT(cs1),
                          s2+=CH_COUNT(cs2),
                          s3+=CH_COUNT(cs3)) 
                pix_ref( dat, s1,cs1, s2,cs2, s3,cs3, d,cd );
        }    
    }tfe4( dat );

    return B4< Tfe4, VAR_NORMAL, TS1, cs1, TS2, cs2, TS3, cs3, TD, cd 
             >::fe( tfe4, s1, s1Step, s2, s2Step, s3, s3Step, d, dStep, roi, min_bytes_per_thread );
}


} // namespace OPT_LEVEL

#pragma warning( pop )

#endif // __FE_LEGACY_H__