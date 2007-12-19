/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

// Pixel Functions

// Add


namespace OPT_LEVEL
{

template< class TS, class TD > SYS_INLINE 
void AddF( const FnDat &, const TS  *s, CH cs, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;
    
    switch( cd )
    {
    case C4:    d[3] = (TD)((d[3] + s[3]) ); 
    case C3:
    case AC4:   d[2] = (TD)((d[2] + s[2]) ); 
                d[1] = (TD)((d[1] + s[1]) ); 
    case C1:    d[0] = (TD)((d[0] + s[0]) ); 
    }
}


template< class TS1, class TS2, class TD > SYS_INLINE 
void AddF( const FnDat &, const TS1  *s1, CH cs1, const TS2  *s2, CH cs2, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
    ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;
    
    switch( cd )
    {
    case C4:    d[3] = (TD)((s1[3] + s2[3])); 
    case C3:    
    case AC4:   d[2] = (TD)((s1[2] + s2[2])); 
                d[1] = (TD)((s1[1] + s2[1])); 
    case C1:    d[0] = (TD)((s1[0] + s2[0])); 
    }
}

template< class TS, class TM, class TD > SYS_INLINE 
void AddF_m( const FnDat &, const TS  *s, CH cs, const TM  *m, CH cm, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cs) ); cs; cd; cm;
    ASSERT( 1 == ChannelCount(cm) );
    ASSERT( 1 == ChannelCount(cd) ); 

    if( *m )
        *d = (TD)( *d + *s ); 
}


template< class TS, class TD > SYS_INLINE 
void AddI( const FnDat &, const TS  *s, CH cs, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;
    
    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( d[3] + s[3] ); 
    case C3:                                                
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( d[2] + s[2] ); 
                d[1] = FW_REF::Limits<TD>::Sat( d[1] + s[1] ); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( d[0] + s[0] ); 
    }
}    

template< class TS1, class TS2, class TD > SYS_INLINE 
void AddI( const FnDat &, const TS1  *s1, CH cs1, const TS2  *s2, CH cs2, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
    ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;
    
    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( s1[3] + s2[3] ); 
    case C3:                              
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( s1[2] + s2[2] ); 
                d[1] = FW_REF::Limits<TD>::Sat( s1[1] + s2[1] ); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( s1[0] + s2[0] ); 
    }
}



template< class TS, class TD > SYS_INLINE 
void AddIS( const FnDat &data, const TS  *s, CH cs, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

    int scale = data.Get<int>(0);
    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( Scale( d[3] + s[3], scale) ); 
    case C3:                                                        
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( Scale( d[2] + s[2], scale) ); 
                d[1] = FW_REF::Limits<TD>::Sat( Scale( d[1] + s[1], scale) ); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( Scale( d[0] + s[0], scale) ); 
    }
}    

template< class TS1, class TS2, class TD > SYS_INLINE 
void AddIS( const FnDat &data, const TS1  *s1, CH cs1, const TS2  *s2, CH cs2, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
    ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

    int scale = data.Get<int>(0);
    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( Scale( (s1[3] + s2[3]), scale) ); 
    case C3:                                      
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( Scale( (s1[2] + s2[2]), scale) ); 
                d[1] = FW_REF::Limits<TD>::Sat( Scale( (s1[1] + s2[1]), scale) ); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( Scale( (s1[0] + s2[0]), scale) ); 
    }
}

// AddC

template< class TD > SYS_INLINE 
void AddCI_1( const FnDat & data, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cd) ); cd;

    TD v  = data.Get<TD >(0);
    int scale = data.Get<int>(1);            

    *d = FW_REF::Limits<TD>::Sat( Scale( *d + v, scale) );
}

template< class TS, class TD > SYS_INLINE 
void AddCI_1( const FnDat & data, const TS  *s, CH cs, TD  *d, CH cd )
{
    TD v  = data.Get<TD >(0);
    int scale = data.Get<int>(1);

    ASSERT( 1 == ChannelCount(cd) ); cd;
    ASSERT( 1 == ChannelCount(cs) ); cs;

    *d = FW_REF::Limits<TD>::Sat( Scale(*s + v, scale ) );
}

template< class TD > SYS_INLINE 
void AddCI( const FnDat & data, TD  *d, CH cd )
{
    TD *v      = data.Get<TD*>(0);
    int scale  = data.Get<int>(1);

    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( Scale( d[3] + v[3], scale )); 
    case C3:
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( Scale( d[2] + v[2], scale )); 
                d[1] = FW_REF::Limits<TD>::Sat( Scale( d[1] + v[1], scale )); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( Scale( d[0] + v[0], scale )); 
    }
}

template< class TS, class TD > SYS_INLINE 
void AddCI( const FnDat & data, const TS  *s, CH cs, TD  *d, CH cd )
{
    TD *v      = data.Get<TD*>(0);
    int scale  = data.Get<int>(1);

    ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( Scale( s[3] + v[3], scale )); 
    case C3:
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( Scale( s[2] + v[2], scale )); 
                d[1] = FW_REF::Limits<TD>::Sat( Scale( s[1] + v[1], scale )); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( Scale( s[0] + v[0], scale )); 
    }
}

template< class TD > SYS_INLINE 
void AddCF_1( const FnDat & data, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cd) ); cd;

    d[0] = (TD)((d[0] + data.Get<TD >(0)) );
}

template< class TS, class TD > SYS_INLINE 
void AddCF_1( const FnDat & data, const TS  *s, CH cs, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cd) ); cd;
    ASSERT( 1 == ChannelCount(cs) ); cs;

    d[0] = (TD)((s[0] +  data.Get<TD >(0)) );
}

template< class TD > SYS_INLINE 
void AddCF( const FnDat & data, TD  *d, CH cd )
{
    TD *v  = data.Get<TD*>(0);        

    switch( cd )
    {
    case C4:    d[3] = (TD)((d[3] + v[3]) ); 
    case C3:
    case AC4:   d[2] = (TD)((d[2] + v[2]) ); 
                d[1] = (TD)((d[1] + v[1]) ); 
    case C1:    d[0] = (TD)((d[0] + v[0]) ); 
    }
}

template< class TS, class TD > SYS_INLINE 
void AddCF( const FnDat & data, const TS  *s, CH cs, TD  *d, CH cd )
{
    TD *v  = data.Get<TD*>(0);    

    ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

    switch( cd )
    {
    case C4:    d[3] = (TD)((s[3] + v[3]) ); 
    case C3:
    case AC4:   d[2] = (TD)((s[2] + v[2]) ); 
                d[1] = (TD)((s[1] + v[1]) ); 
    case C1:    d[0] = (TD)((s[0] + v[0]) ); 
    }
}    

//    AddProduct

template< class TS1, class TS2, class TD > SYS_INLINE 
void AddProduct( const FnDat & , const TS1  *s1, CH cs1, const TS2 *s2, CH cs2, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs1) == ChannelCount(cs2) ); cs1; cs2;
    ASSERT( ChannelCount(cs2) == ChannelCount(cd)  ); 

    switch( cd )
    {
    case C4:    d[3] += (TD)( s1[3]  *s2[3] ); 
    case C3:
    case AC4:   d[2] += (TD)( s1[2]  *s2[2] ); 
                d[1] += (TD)( s1[1]  *s2[1] ); 
    case C1:    d[0] += (TD)( s1[0]  *s2[0] ); 
    }
}

template< class TS1, class TS2, class TM, class TD > SYS_INLINE 
void AddProduct_m( const FnDat &, const TS1  *s1, CH cs1, const TS2 *s2, CH cs2, const TM *m, CH cm, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cs1) ); cs1; cs2; cm; cd;
    ASSERT( 1 == ChannelCount(cs2) );
    ASSERT( ChannelCount(cm ) == 1 ); 
    ASSERT( ChannelCount(cd ) == 1 ); 

    if( *m )
        *d + = (TD)( *s1 * *s2 ); 
}

// AddSquare

template< class TS, class TD > SYS_INLINE 
void AddSquare( const FnDat & , const TS  *s, CH cs, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cs) ); cs; cd;
    ASSERT( 1 == ChannelCount(cd) ); 

    *d = *d + (*s * *s);
}

template< class TS, class TM, class TD > SYS_INLINE 
void AddSquare_m( const FnDat & , const TS  *s, CH cs, const TM  *m, CH cm,  TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cs) ); cs; cd; cm;
    ASSERT( 1 == ChannelCount(cm) ); 
    ASSERT( 1 == ChannelCount(cd) ); 

    if( *m )
        *d = *d + (*s * *s);
}

//    Addweighted

template< class TS, class TD > SYS_INLINE 
void AddWeighted( const FnDat & dat, const TS  *s, CH cs, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cs) ); cs; cd;
    ASSERT( 1 == ChannelCount(cd) ); 

    A32F alpha = dat.Get< A32F >(0);

    *d = (*d *(1-alpha)) + (*s  *alpha);        
}

template< class TS, class TM, class TD > SYS_INLINE 
void AddWeighted_m( const FnDat & dat, const TS  *s, CH cs, const TM *m, CH cm, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cs) ); cs; cd; cm;
    ASSERT( 1 == ChannelCount(cm) );
    ASSERT( 1 == ChannelCount(cd) ); 

    A32F alpha = dat.Get< A32F >(0);

    if( *m )
        *d = (*d *(1-alpha)) + (*s  *alpha);        
}

// Sub

template< class TS, class TD > SYS_INLINE 
void SubF( const FnDat &, const TS  *s, CH cs, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;
    
    switch( cd )
    {
    case C4:    d[3] = (TD)((d[3] - s[3]) ); 
    case C3:
    case AC4:   d[2] = (TD)((d[2] - s[2]) ); 
                d[1] = (TD)((d[1] - s[1]) ); 
    case C1:    d[0] = (TD)((d[0] - s[0]) ); 
    }
}

template< class TS1, class TS2, class TD > SYS_INLINE 
void SubF( const FnDat &, const TS1  *s1, CH cs1, const TS2  *s2, CH cs2, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
    ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;
    
    switch( cd )
    {
    case C4:    d[3] = (TD)((s1[3] - s2[3])); 
    case C3:    
    case AC4:   d[2] = (TD)((s1[2] - s2[2])); 
                d[1] = (TD)((s1[1] - s2[1])); 
    case C1:    d[0] = (TD)((s1[0] - s2[0])); 
    }
}


template< class TS, class TM, class TD > SYS_INLINE 
void SubF_m( const FnDat &, const TS  *s, CH cs, const TM  *m, CH cm, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cs) ); cs; cd; cm;
    ASSERT( 1 == ChannelCount(cm) );
    ASSERT( 1 == ChannelCount(cd) ); 

    if( *m )
        *d = (TD)( *d - *s ); 
}


template< class TS, class TD > SYS_INLINE 
void SubI( const FnDat &data, const TS  *s, CH cs, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

    int scale = data.Get<int>(0);
    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( Scale( d[3] - s[3], scale) ); 
    case C3:                                                        
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( Scale( d[2] - s[2], scale) ); 
                d[1] = FW_REF::Limits<TD>::Sat( Scale( d[1] - s[1], scale) ); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( Scale( d[0] - s[0], scale) ); 
    }
}    

template< class TS1, class TS2, class TD > SYS_INLINE 
void SubI( const FnDat &data, const TS1  *s1, CH cs1, const TS2  *s2, CH cs2, TD  *d, CH cd )
{
    ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
    ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

    int scale = data.Get<int>(0);
    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( Scale( (s1[3] - s2[3]), scale) ); 
    case C3:                                      
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( Scale( (s1[2] - s2[2]), scale) ); 
                d[1] = FW_REF::Limits<TD>::Sat( Scale( (s1[1] - s2[1]), scale) ); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( Scale( (s1[0] - s2[0]), scale) ); 
    }
}

// SubC

template< class TD > SYS_INLINE 
void SubCI_1( const FnDat & data, TD  *d, CH cd )
{
    TD v      = data.Get<TD >(0);
    int scale = data.Get<int>(1);

    ASSERT( 1 == ChannelCount(cd) ); cd;

    d[0] = FW_REF::Limits<TD>::Sat( Scale( d[0] - v, scale) );
}

template< class TS, class TD > SYS_INLINE 
void SubCI_1( const FnDat & data, const TS  *s, CH cs, TD  *d, CH cd )
{
    TD v      = data.Get<TD >(0);
    int scale = data.Get<int>(1);

    ASSERT( 1 == ChannelCount(cd) ); cd;
    ASSERT( 1 == ChannelCount(cs) ); cs;

    d[0] = FW_REF::Limits<TD>::Sat( Scale(s[0] - v, scale ) );
}

template< class TD > SYS_INLINE 
void SubCI( const FnDat & data, TD  *d, CH cd )
{
    TD *v      = data.Get<TD*>(0);
    int scale  = data.Get<int>(1);

    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( Scale( d[3] - v[3], scale )); 
    case C3:
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( Scale( d[2] - v[2], scale )); 
                d[1] = FW_REF::Limits<TD>::Sat( Scale( d[1] - v[1], scale )); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( Scale( d[0] - v[0], scale )); 
    }
}

template< class TS, class TD > SYS_INLINE 
void SubCI( const FnDat & data, const TS  *s, CH cs, TD  *d, CH cd )
{
    TD *v      = data.Get<TD*>(0);
    int scale  = data.Get<int>(1);

    ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

    switch( cd )
    {
    case C4:    d[3] = FW_REF::Limits<TD>::Sat( Scale( s[3] - v[3], scale )); 
    case C3:
    case AC4:   d[2] = FW_REF::Limits<TD>::Sat( Scale( s[2] - v[2], scale )); 
                d[1] = FW_REF::Limits<TD>::Sat( Scale( s[1] - v[1], scale )); 
    case C1:    d[0] = FW_REF::Limits<TD>::Sat( Scale( s[0] - v[0], scale )); 
    }
}

template< class TD > SYS_INLINE 
void SubCF_1( const FnDat & data, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cd) ); cd;

    d[0] = (TD)((d[0] - data.Get<TD >(0)) );
}

template< class TS, class TD > SYS_INLINE 
void SubCF_1( const FnDat & data, const TS  *s, CH cs, TD  *d, CH cd )
{
    ASSERT( 1 == ChannelCount(cd) ); cd;
    ASSERT( 1 == ChannelCount(cs) ); cs;

    d[0] = (TD)((s[0] -  data.Get<TD >(0)) );
}

template< class TD > SYS_INLINE 
void SubCF( const FnDat & data, TD  *d, CH cd )
{
    TD *v  = data.Get<TD*>(0);        

    switch( cd )
    {
    case C4:    d[3] = (TD)((d[3] - v[3]) ); 
    case C3:
    case AC4:   d[2] = (TD)((d[2] - v[2]) ); 
                d[1] = (TD)((d[1] - v[1]) ); 
    case C1:    d[0] = (TD)((d[0] - v[0]) ); 
    }
}

template< class TS, class TD > SYS_INLINE 
void SubCF( const FnDat & data, const TS  *s, CH cs, TD  *d, CH cd )
{
    TD *v  = data.Get<TD*>(0);    

    ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

    switch( cd )
    {
    case C4:    d[3] = (TD)((s[3] - v[3]) ); 
    case C3:
    case AC4:   d[2] = (TD)((s[2] - v[2]) ); 
                d[1] = (TD)((s[1] - v[1]) ); 
    case C1:    d[0] = (TD)((s[0] - v[0]) ); 
    }
}    


// SSE2

SYS_INLINE void Add_8u32f( const FnDat &, RegDescriptor &reg )    
{    XMM128 temp;
    CBL_SSE2::Unpack8UTo16U(reg.src1[0],temp);
    CBL_SSE2::Unpack16STo32S(reg.src1[0].i,temp.i);
    reg.src1[0].f = _mm_cvtepi32_ps (reg.src1[0].i);
    reg.dst[0].f  = ::_mm_add_ps( reg.dst[0].f, reg.src1[0].f );    
}

SYS_INLINE void Add_8s32f( const FnDat &, RegDescriptor &reg )    
{    XMM128 temp;
    CBL_SSE2::Unpack8STo16S(reg.src1[0],temp);
    CBL_SSE2::Unpack16STo32S(reg.src1[0].i,temp.i);
    reg.src1[0].f = _mm_cvtepi32_ps (reg.src1[0].i);
    reg.dst[0].f  = ::_mm_add_ps( reg.dst[0].f, reg.src1[0].f );    
}

SYS_INLINE void Add_16u32f( const FnDat &, RegDescriptor &reg )
{    XMM128 temp;
    CBL_SSE2::Unpack16STo32S(reg.src1[0].i,temp.i);
    reg.src1[0].f = _mm_cvtepi32_ps (reg.src1[0].i);
    reg.dst[0].f  = ::_mm_add_ps( reg.dst[0].f, reg.src1[0].f );    
}

SYS_INLINE void Add32f_SSE2( const FnDat &, RegDescriptor &reg )    
{    
    switch( reg.dst.RegCount() )
    {
    case 4: reg.dst[3].f = ::_mm_add_ps( reg.src1[3].f, reg.src2[3].f );
    case 3: reg.dst[2].f = ::_mm_add_ps( reg.src1[2].f, reg.src2[2].f );
    case 2: reg.dst[1].f = ::_mm_add_ps( reg.src1[1].f, reg.src2[1].f );
    case 1: reg.dst[0].f = ::_mm_add_ps( reg.src1[0].f, reg.src2[0].f );
    }    
}

SYS_INLINE void Add32fI_SSE2( const FnDat &, RegDescriptor &reg )    
{    
    switch( reg.dst.RegCount() )
    {
    case 4: reg.dst[3].f = ::_mm_add_ps( reg.src1[3].f, reg.dst[3].f );
    case 3: reg.dst[2].f = ::_mm_add_ps( reg.src1[2].f, reg.dst[2].f );
    case 2: reg.dst[1].f = ::_mm_add_ps( reg.src1[1].f, reg.dst[1].f );
    case 1: reg.dst[0].f = ::_mm_add_ps( reg.src1[0].f, reg.dst[0].f );
    }    
}

SYS_INLINE void Sub32f_SSE2( const FnDat &, RegDescriptor &reg )    
{    
    switch( reg.dst.RegCount() )
    {
    case 4: reg.dst[3].f = ::_mm_sub_ps( reg.src1[3].f, reg.src2[3].f );
    case 3: reg.dst[2].f = ::_mm_sub_ps( reg.src1[2].f, reg.src2[2].f );
    case 2: reg.dst[1].f = ::_mm_sub_ps( reg.src1[1].f, reg.src2[1].f );
    case 1: reg.dst[0].f = ::_mm_sub_ps( reg.src1[0].f, reg.src2[0].f );
    }    
}

SYS_INLINE void Sub32fI_SSE2( const FnDat &, RegDescriptor &reg )    
{    
    switch( reg.dst.RegCount() )
    {
    case 4: reg.dst[3].f = ::_mm_sub_ps( reg.src1[3].f, reg.dst[3].f );
    case 3: reg.dst[2].f = ::_mm_sub_ps( reg.src1[2].f, reg.dst[2].f );
    case 2: reg.dst[1].f = ::_mm_sub_ps( reg.src1[1].f, reg.dst[1].f );
    case 1: reg.dst[0].f = ::_mm_sub_ps( reg.src1[0].f, reg.dst[0].f );
    }    
}

SYS_INLINE void Add_16s( const FnDat &, RegDescriptor &reg )    
{    
    switch( reg.dst.RegCount() )
    {
    case 4: reg.dst[3].i = ::_mm_add_epi16( reg.dst[3].i, reg.src1[3].i );
    case 3: reg.dst[2].i = ::_mm_add_epi16( reg.dst[2].i, reg.src1[2].i );
    case 2: reg.dst[1].i = ::_mm_add_epi16( reg.dst[1].i, reg.src1[1].i );
    case 1: reg.dst[0].i = ::_mm_add_epi16( reg.dst[0].i, reg.src1[0].i );
    }    
}

SYS_INLINE void Add1_Sat_16s_2S1D( const FnDat &, RegDescriptor &reg )    
{    
    ASSERT( 1 == reg.dst.RegCount() );
    reg.dst[0].i = ::_mm_adds_epi16( reg.src1[0].i, reg.src2[0].i );        
}

SYS_INLINE void Add8uI_SSE2( const FnDat &, RegDescriptor &reg )    
{    
    switch( reg.dst.RegCount() )
    {
    case 4: reg.dst[3].i = ::_mm_add_epi8( reg.dst[3].i, reg.src1[3].i );
    case 3: reg.dst[2].i = ::_mm_add_epi8( reg.dst[2].i, reg.src1[2].i );
    case 2: reg.dst[1].i = ::_mm_add_epi8( reg.dst[1].i, reg.src1[1].i );
    case 1: reg.dst[0].i = ::_mm_add_epi8( reg.dst[0].i, reg.src1[0].i );
    }    
}

SYS_INLINE void Add8u_Scale(XMM128& dst, int& scale)
{    
    XMM128 temp;
    CBL_SSE2::Unpack8UTo16U(dst, temp);
    dst.i  = _mm_srli_epi16(dst.i, scale);
    temp.i = _mm_srli_epi16(temp.i, scale);
    FW_SSE2::Pack16Uto8(dst, temp);
}

SYS_INLINE void AddC8u(XMM128& src, XMM128& dst, XMM128& value, XMM128& scale)
{    
    XMM128 srcTemp1, srcTemp2;
    XMM128 srcTemp1_1,srcTemp2_1;
    srcTemp1 = src;
    CBL_SSE2::Unpack8UTo16U(srcTemp1,srcTemp2);
    srcTemp1.i = _mm_add_epi16(srcTemp1.i, value.i);
    srcTemp2.i = _mm_add_epi16(srcTemp2.i, value.i);


    CBL_SSE2::Unpack16UTo32U(srcTemp1.i, srcTemp1_1.i);
    CBL_SSE2::Unpack16UTo32U(srcTemp2.i, srcTemp2_1.i);

    srcTemp1.f    = _mm_cvtepi32_ps (srcTemp1.i  );
    srcTemp1_1.f  = _mm_cvtepi32_ps (srcTemp1_1.i);
    srcTemp2.f    = _mm_cvtepi32_ps (srcTemp2.i  );
    srcTemp2_1.f  = _mm_cvtepi32_ps (srcTemp2_1.i);

    srcTemp1.f    = _mm_mul_ps (srcTemp1.f  , scale.f);
    srcTemp1_1.f  = _mm_mul_ps (srcTemp1_1.f, scale.f);
    srcTemp2.f    = _mm_mul_ps (srcTemp2.f  , scale.f);
    srcTemp2_1.f  = _mm_mul_ps (srcTemp2_1.f, scale.f);

    srcTemp1.i    = _mm_cvtps_epi32 (srcTemp1.f  );
    srcTemp1_1.i  = _mm_cvtps_epi32 (srcTemp1_1.f);
    srcTemp2.i    = _mm_cvtps_epi32 (srcTemp2.f  );
    srcTemp2_1.i  = _mm_cvtps_epi32 (srcTemp2_1.f);

    FW_SSE2::pack32STo16S(srcTemp1.i, srcTemp1_1.i);
    FW_SSE2::pack32STo16S(srcTemp2.i, srcTemp2_1.i);

    FW_SSE2::pack16STo8U(srcTemp1.i,srcTemp2.i);
    dst = srcTemp1;
}


SYS_INLINE void AddC8uI_SSE2( const FnDat &data, RegDescriptor &reg )    
{    //int scale  = data.Get<int>(1);
    XMM128 value = data.Get<XMM128>(2);
    XMM128 scale = data.Get<XMM128>(3);
    AddC8u(reg.dst[0], reg.dst[0], value, scale);
}

SYS_INLINE void AddC8u_SSE2( const FnDat &data, RegDescriptor &reg )    
{    //int scale  = data.Get<int>(1);
    XMM128 value = data.Get<XMM128>(2);
    XMM128 scale = data.Get<XMM128>(3);
    AddC8u(reg.src1[0], reg.dst[0], value, scale);
}
//AddC8uC3_SSE2
SYS_INLINE void AddC8uC3_SSE2( const FnDat &data, RegDescriptor &reg )    
{    //XMM128 *value = data.Get<XMM128*>(2);
    XMM128 *value = (XMM128*)data.data[2];
    XMM128 scale  = data.Get<XMM128>(3);
    AddC8u(reg.src1[0], reg.dst[0], value[0], scale);
    AddC8u(reg.src1[1], reg.dst[1], value[1], scale);
    AddC8u(reg.src1[2], reg.dst[2], value[2], scale);
}

SYS_INLINE void AddC8uC3I_SSE2( const FnDat &data, RegDescriptor &reg )    
{   XMM128 *value = (XMM128*)data.data[2];
    XMM128 scale  = data.Get<XMM128>(3);
    AddC8u(reg.dst[0], reg.dst[0], value[0], scale);
    AddC8u(reg.dst[1], reg.dst[1], value[1], scale);
    AddC8u(reg.dst[2], reg.dst[2], value[2], scale);
}


} // namespace OPT_LEVEL

