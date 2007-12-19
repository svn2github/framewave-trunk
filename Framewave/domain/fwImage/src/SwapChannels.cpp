/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "SwapChannelsDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

// C3R
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_8u_C3R)(cA8U *s,int sStep, A8U *d, int dStep, ASZ roi,const int dstOrder[3])
    {
    int i;
    i = dstOrder[0]*100 + dstOrder[1]*10 + dstOrder[2];//determine the pattern -- pattern = 12   if dstOrder[]= 0,1,2

    switch(i)
        {
        case  12:{
            SwapChannels_8u_C3R012 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3R012> (dat, s, sStep, d , dStep, roi);
            }
        case  21:{
            SwapChannels_8u_C3R021 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3R021> (dat, s, sStep, d , dStep, roi);
            }
        case 102:{	
            SwapChannels_8u_C3R102 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3R102> (dat, s, sStep, d , dStep, roi);
            }
        case 120:{	
            SwapChannels_8u_C3R120 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3R120> (dat, s, sStep, d , dStep, roi);	
            }	
        case 201:{	
            SwapChannels_8u_C3R201 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3R201> (dat, s, sStep, d , dStep, roi);
            }	
        case 210:{	
            SwapChannels_8u_C3R210 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3R210> (dat, s, sStep, d , dStep, roi);
            }	
        default :   return fwStsChannelOrderErr;
        }
    }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_16u_C3R)(cA16U *s,int sStep, A16U *d, int dStep, ASZ roi,const int dstOrder[3])
    {
    int i;
    i = dstOrder[0]*100 + dstOrder[1]*10 + dstOrder[2];//determine the pattern -- pattern = 12   if dstOrder[]= 0,1,2

    switch(i)
        {
        case  12:{	
            SwapChannels_16u_C3R012 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_C3R012> (dat, s, sStep, d , dStep, roi);
            }	
        case  21:{	
            SwapChannels_16u_C3R021 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_C3R021> (dat, s, sStep, d , dStep, roi);
            }	
        case 102:{	
            SwapChannels_16u_C3R102 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_C3R102> (dat, s, sStep, d , dStep, roi);
            }	
        case 120:{	
            SwapChannels_16u_C3R120 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_C3R120> (dat, s, sStep, d , dStep, roi);
            }	
        case 201:{	
            SwapChannels_16u_C3R201 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_C3R201> (dat, s, sStep, d , dStep, roi);
            }	
        case 210:{   
            SwapChannels_16u_C3R210 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_C3R210> (dat, s, sStep, d , dStep, roi);
            }	
        default :	return fwStsChannelOrderErr;
        }
    }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_32s_C3R)(cA32S *s,int sStep, A32S *d, int dStep, ASZ roi,const int dstOrder[3])
    {
    int i;
    i = dstOrder[0]*100 + dstOrder[1]*10 + dstOrder[2];//determine the pattern -- pattern = 12   if dstOrder[]= 0,1,2

    switch(i)
        {
        case  12:{	
            SwapChannels_32_C3R012<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R012<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case  21:{	
            SwapChannels_32_C3R021<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R021<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case 102:{	
            SwapChannels_32_C3R102<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R102<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case 120:{	
            SwapChannels_32_C3R120<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R120<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case 201:{	
            SwapChannels_32_C3R201<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R201<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case 210:{	
            SwapChannels_32_C3R210<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R210<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        default :   return fwStsChannelOrderErr;
        }			
    }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_32f_C3R)(cA32F *s,int sStep, A32F *d, int dStep, ASZ roi,const int dstOrder[3])
    {
    int i;
    i = dstOrder[0]*100 + dstOrder[1]*10 + dstOrder[2];//determine the pattern -- pattern = 12   if dstOrder[]= 0,1,2

    switch(i)
        {
        case  12:{	
            SwapChannels_32_C3R012<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R012<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case  21:{	
            SwapChannels_32_C3R021<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R021<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case 102:{	
            SwapChannels_32_C3R102<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R102<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case 120:{	
            SwapChannels_32_C3R120<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R120<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case 201:{	
            SwapChannels_32_C3R201<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R201<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case 210:{	
            SwapChannels_32_C3R210<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_C3R210<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        default :	return fwStsChannelOrderErr;

        }			
    }

// AC4R
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_8u_AC4R)(cA8U *s,int sStep, A8U *d, int dStep, ASZ roi,const int dstOrder[3])
    {
    int i;
    i = dstOrder[0]*100 + dstOrder[1]*10 + dstOrder[2];//determine the pattern -- pattern = 12   if dstOrder[]= 0,1,2

    switch(i)
        {
        case  12:{	
            SwapChannels_8u_AC4R012 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_AC4R012> (dat, s, sStep, d , dStep, roi);
            }	
        case  21:{	
            SwapChannels_8u_AC4R021 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_AC4R021> (dat, s, sStep, d , dStep, roi);
            }	
        case 102:{	
            SwapChannels_8u_AC4R102 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_AC4R102> (dat, s, sStep, d , dStep, roi);
            }	
        case 120:{	
            SwapChannels_8u_AC4R120 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_AC4R120> (dat, s, sStep, d , dStep, roi);
            }	
        case 201:{	
            SwapChannels_8u_AC4R201 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_AC4R201> (dat, s, sStep, d , dStep, roi);
            }	
        case 210:{	
            SwapChannels_8u_AC4R210 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_AC4R210> (dat, s, sStep, d , dStep, roi);
            }	
        default :   return  fwStsChannelOrderErr;

        }			
    }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_16u_AC4R )(cA16U *s,int sStep, A16U *d, int dStep, ASZ roi,const int dstOrder[3])
    {
    int i;
    i = dstOrder[0]*100 + dstOrder[1]*10 + dstOrder[2];//determine the pattern -- pattern = 12   if dstOrder[]= 0,1,2

    switch(i)
        {
        case  12:{	
            SwapChannels_16u_AC4R012 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_AC4R012> (dat, s, sStep, d , dStep, roi);
            }	
        case  21:{	
            SwapChannels_16u_AC4R021 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_AC4R021> (dat, s, sStep, d , dStep, roi);
            }	
        case 102:{	
            SwapChannels_16u_AC4R102 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_AC4R102> (dat, s, sStep, d , dStep, roi);
            }	
        case 120:{	
            SwapChannels_16u_AC4R120 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_AC4R120> (dat, s, sStep, d , dStep, roi);
            }	
        case 201:{	
            SwapChannels_16u_AC4R201 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_AC4R201> (dat, s, sStep, d , dStep, roi);
            }	
        case 210:{	
            SwapChannels_16u_AC4R210 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_16u_AC4R210> (dat, s, sStep, d , dStep, roi);
            }	
        default :   return fwStsChannelOrderErr;

        }			
    }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_32s_AC4R)(cA32S *s,int sStep, A32S *d, int dStep, ASZ roi,const int dstOrder[3])
    {
    int i;
    i = dstOrder[0]*100 + dstOrder[1]*10 + dstOrder[2];//determine the pattern -- pattern = 12   if dstOrder[]= 0,1,2

    switch(i)
        {
        case  12:{	
            SwapChannels_32_AC4R012<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R012<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case  21:{	
            SwapChannels_32_AC4R021<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R021<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case 102:{	
            SwapChannels_32_AC4R102<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R102<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case 120:{	
            SwapChannels_32_AC4R120<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R120<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case 201:{	
            SwapChannels_32_AC4R201<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R201<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        case 210:{	
            SwapChannels_32_AC4R210<A32S> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R210<A32S> > (dat, s, sStep, d , dStep, roi);
            }	
        default :   return fwStsChannelOrderErr;

        }			
    }

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_32f_AC4R)(cA32F *s,int sStep, A32F *d, int dStep, ASZ roi,const int dstOrder[3])
    {
    int i;
    i = dstOrder[0]*100 + dstOrder[1]*10 + dstOrder[2];//determine the pattern -- pattern = 12   if dstOrder[]= 0,1,2

    switch(i)
        {
        case  12:{	
            SwapChannels_32_AC4R012<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R012<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case  21:{	
            SwapChannels_32_AC4R021<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R021<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case 102:{	
            SwapChannels_32_AC4R102<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R102<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case 120:{	
            SwapChannels_32_AC4R120<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R120<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case 201:{	
            SwapChannels_32_AC4R201<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R201<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        case 210:{	
            SwapChannels_32_AC4R210<A32F> dat(dstOrder);
            return OPT_LEVEL::fe< SwapChannels_32_AC4R210<A32F> > (dat, s, sStep, d , dStep, roi);
            }	
        default	:   return fwStsChannelOrderErr;

        }			
    }

// C3IR
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_8u_C3IR)(A8U *pSrcDst, int srcDstStep,ASZ roi, const int dstOrder[3])
    {
    int i;
    i = dstOrder[0]*100 + dstOrder[1]*10 + dstOrder[2];//determine the pattern -- pattern = 12   if dstOrder[]= 0,1,2

    switch(i)
        {
        case  12:	return fwStsNoErr;//The order is same so just return..
        case  21:{	
            SwapChannels_8u_C3IR021 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3IR021> (dat, pSrcDst , srcDstStep, roi);
            }	
        case 102:{	
            SwapChannels_8u_C3IR102 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3IR102> (dat, pSrcDst , srcDstStep, roi);
            }	
        case 120:{	
            SwapChannels_8u_C3IR120 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3IR120> (dat, pSrcDst , srcDstStep, roi);
            }	
        case 201:{	
            SwapChannels_8u_C3IR201 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3IR201> (dat, pSrcDst , srcDstStep, roi);
            }	
        case 210:{	
            SwapChannels_8u_C3IR210 dat(dstOrder);
            return OPT_LEVEL::fe<SwapChannels_8u_C3IR210> (dat, pSrcDst , srcDstStep, roi);
            }	
        default :   return fwStsChannelOrderErr;

        }			
    }


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
