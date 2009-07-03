/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COMMONIMPL_H__
#define __COMMONIMPL_H__

#include "fwdev.h"
#include "FwSharedCode_SSE2.h"
#include "fe.h"


namespace OPT_LEVEL
{

namespace Common
{
    namespace AC4
    {
        namespace Init
        {
            ISV For8(XMM128 &reg)
            {
                reg.i = _mm_cmpeq_epi32(reg.i, reg.i);
                reg.i = _mm_srli_epi32(reg.i, 8);
            }

            ISV For16(XMM128 &reg)
            {
                reg.i = _mm_cmpeq_epi32(reg.i, reg.i);
                reg.i = _mm_srli_epi64 (reg.i, 16);
            }

            ISV For32(XMM128 &reg)
            {
                reg.i = _mm_cmpeq_epi32(reg.i, reg.i);
                reg.i = _mm_srli_si128(reg.i, 4);
            }

            ISV For64(XMM128 &reg)
            {
                reg.i = _mm_cmpeq_epi32(reg.i, reg.i);
                reg.i = _mm_srli_si128(reg.i, 8);
            }
        }

        namespace Apply
        {
            ISV MaskI(XMM128 &dst, const XMM128 &temp, const XMM128 &mask)
            {
                dst.i = _mm_or_si128( _mm_and_si128    (mask.i, temp.i),
                                      _mm_andnot_si128 (mask.i, dst.i ) );
            }
            ISV MaskF(XMM128 &dst, const XMM128 &temp, const XMM128 &mask)
            {
                dst.f = _mm_or_ps   ( _mm_and_ps       (mask.f, temp.f),
                                      _mm_andnot_ps    (mask.f, dst.f ) );
            }
            ISV MaskD(XMM128 &dst, const XMM128 &temp, const XMM128 &mask)
            {
                dst.d = _mm_or_pd   ( _mm_and_pd       (mask.d, temp.d),
                                      _mm_andnot_pd    (mask.d, dst.d ) );
            }
        }
    }
}


} // namespace OPT_LEVEL


#endif // __COMMONIMPL_H__

