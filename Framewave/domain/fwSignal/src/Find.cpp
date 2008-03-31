/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "FwSharedCode.h"
#include "fwSignal.h"

namespace OPT_LEVEL
{

    namespace FindStr
    {
        template<typename TS,typename TF>
        int FindString(const TS *srcstr,int srclen,const TF *substr,int lenfind)
        {
            const TS *start = srcstr;
            const TS *end = srcstr + srclen;
            int index = -1;

            for(;end!=srcstr; srcstr++)
            {
                
                const TF *str = substr;
                const TF *strend  = substr + lenfind;
                const TS *src =  srcstr;
                while(strend != str && *str == *src)
                {
                    str++;
                    src++;
                }

                if(strend == str)
                {
                    index = (int)((src - lenfind) - start) ;
                    return index;
                }

            }
            return index;
        }

        template<typename TS,typename TF>
        int FindStringRev(const TS *srcstr,int srclen,const TF *substr,int lenfind)
        {
            const TS *start = srcstr;
            const TS *end = srcstr + srclen;
            int index = -1;

            for(;end!=srcstr; end--)
            {
                
                const TF *str = substr;
                const TF *strend  = substr + lenfind - 1;
                const TS *src =  end;
                while(*strend == *src && strend != str)
                {
                    strend--;
                    src--;
                }

                if(strend == str && *strend == *src)
                {
                    index = (int)((end- start) - lenfind + 1) ;
                    return index;
                }
            }

            return index;
        }
    }

    namespace FindCh
    {
        template<typename TS,typename TF>
        int FindChar(const TS *srcstr,int srclen,const TF &substr)
        {
            const TS *start = srcstr;
            const TS *end = srcstr + srclen;
            int index = -1;

            for(;end!=srcstr && substr != *srcstr ; srcstr++);

            if(substr == *srcstr)
            {
                index = (int)(srcstr - start);
            }
            return index;
        }


        template<typename TS,typename TF>
        int FindCharRev(const TS *srcstr,int srclen,const TF &substr)
        {
            const TS *start = srcstr;
            const TS *end = srcstr + srclen;
            int index = -1;

            for(;end!=srcstr && substr != *end ; end--);

            if(substr == *end)
            {
                index = (int)(end - start);
            }
            return index;
        }
    }
}

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwsFind_8u) (const Fw8u* pSrc, int len, const Fw8u* pFind, int lenFind, int* pIndex)
{
       if( FW_REF::PtrNotOK ( pSrc, pFind )) return fwStsNullPtrErr;
       if( FW_REF::SizeNotOK( len    )) return fwStsLengthErr;
       if( FW_REF::SizeNotOK(  lenFind    )) return fwStsLengthErr;
       if(lenFind < len)  return fwStsLengthErr;

       *pIndex = FindStr::FindString<Fw8u,Fw8u>(pSrc,len,pFind,lenFind);
       return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX,  fwsFind_16u)(const Fw16u* pSrc, int len, const Fw16u* pFind, int lenFind, int* pIndex)
{
       if( FW_REF::PtrNotOK ( pSrc, pFind )) return fwStsNullPtrErr;
       if( FW_REF::SizeNotOK( len    )) return fwStsLengthErr;
       if( FW_REF::SizeNotOK(  lenFind    )) return fwStsLengthErr;
       if(lenFind < len)  return fwStsLengthErr;

       *pIndex = FindStr::FindString<Fw16u,Fw16u>(pSrc,len,pFind,lenFind);
       return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX,  fwsFindRev_8u)(const Fw8u* pSrc, int len, const Fw8u* pFind, int lenFind, int* pIndex)
{
       if( FW_REF::PtrNotOK ( pSrc, pFind )) return fwStsNullPtrErr;
       if( FW_REF::SizeNotOK( len    )) return fwStsLengthErr;
       if( FW_REF::SizeNotOK(  lenFind    )) return fwStsLengthErr;
       if(lenFind < len)  return fwStsLengthErr;

       *pIndex = FindStr::FindStringRev<Fw8u,Fw8u>(pSrc,len,pFind,lenFind);
       return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX,  fwsFindRev_16u)(const Fw16u* pSrc, int len, const Fw16u* pFind, int lenFind, int* pIndex)
{
       if( FW_REF::PtrNotOK ( pSrc, pFind )) return fwStsNullPtrErr;
       if( FW_REF::SizeNotOK( len    )) return fwStsLengthErr;
       if( FW_REF::SizeNotOK(  lenFind    )) return fwStsLengthErr;
       if(lenFind < len)  return fwStsLengthErr;

       *pIndex = FindStr::FindStringRev<Fw16u,Fw16u>(pSrc,len,pFind,lenFind);
       return fwStsNoErr;
}


FwStatus PREFIX_OPT(OPT_PREFIX,  fwsFindC_8u)(const Fw8u* pSrc, int len, Fw8u valFind, int* pIndex)
{
    if( FW_REF::PtrNotOK ( pSrc )) return fwStsNullPtrErr;
    if( FW_REF::SizeNotOK( len  )) return fwStsLengthErr;
    
    *pIndex = FindCh::FindChar<Fw8u,Fw8u>(pSrc,len,valFind);
    return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX,  fwsFindC_16u)(const Fw16u* pSrc, int len, Fw16u valFind, int* pIndex)
{
    if( FW_REF::PtrNotOK ( pSrc )) return fwStsNullPtrErr;
    if( FW_REF::SizeNotOK( len  )) return fwStsLengthErr;
    
    *pIndex = FindCh::FindChar<Fw16u,Fw16u>(pSrc,len,valFind);
    return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX,  fwsFindRevC_8u)(const Fw8u* pSrc, int len, Fw8u valFind, int* pIndex)
{
    if( FW_REF::PtrNotOK ( pSrc )) return fwStsNullPtrErr;
    if( FW_REF::SizeNotOK( len  )) return fwStsLengthErr;
    
    *pIndex = FindCh::FindCharRev<Fw8u,Fw8u>(pSrc,len,valFind);
    return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX,  fwsFindRevC_16u)(const Fw16u* pSrc, int len, Fw16u valFind, int* pIndex)
{
    if( FW_REF::PtrNotOK ( pSrc )) return fwStsNullPtrErr;
    if( FW_REF::SizeNotOK( len  )) return fwStsLengthErr;
    
    *pIndex = FindCh::FindCharRev<Fw16u,Fw16u>(pSrc,len,valFind);
    return fwStsNoErr;
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR
