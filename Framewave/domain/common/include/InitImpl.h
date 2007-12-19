/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __INITIMPL_H__
#define __INITIMPL_H__

#include "fwdev.h"
#include "FwSharedCode_SSE2.h"
#include "fe.h"


namespace OPT_LEVEL
{

namespace Common
{
    namespace InitConst
    {
        namespace C1
        {
            
            template<class TS, class TD>
            struct ValC : public fe2< TS, OPT_LEVEL::C1, TD, OPT_LEVEL::C1>
            {
                TS valC;
                XMM128 val;

                ValC(TS valC_)
                {
                    valC = valC_;
                }
            };

            namespace To8u
            {
                ISV From8u(Fw8u val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x16_8u(reg.i, val);
                }
            }
            namespace To16s
            {
                ISV From8u(Fw8u val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x8_16s(reg.i, (Fw16s) val);
                }
                ISV From16s(Fw16s val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x8_16s(reg.i, (Fw16s) val);
                }
                ISV From16sc(Fw16sc val, XMM128 &reg)
                {
                    FW_SSE2::Load1(reg, val);
                }
            }
            namespace To16u
            {
                ISV From8u(Fw8u val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x8_16u(reg.i, (Fw16u) val);
                }
            }
            namespace To32s
            {
                ISV From8u(Fw8u val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x4_32s(reg.i, (Fw32s) val);
                }
                ISV From16s(Fw16s val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x4_32s(reg.i, (Fw32s) val);
                }
                ISV From16u(Fw16u val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x4_32s(reg.i, (Fw32s) val);
                }
            }
            namespace To32sc
            {
                ISV From16sc(Fw16sc val, XMM128 &reg)
                {
                    Fw32sc val32sc;
                    val32sc.re = val.re; val32sc.im = val.im;

                    FW_SSE2::Load1(reg, val32sc);
                }
                ISV From32sc(Fw32sc val, XMM128 &reg)
                {
                    FW_SSE2::Load1(reg, val);
                }
            }
            namespace To32f
            {
                ISV From8u(Fw8u val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x4_32f(reg.f, (Fw32f) val);
                }
                ISV From16s(Fw16s val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x4_32f(reg.f, (Fw32f) val);
                }
                ISV From16u(Fw16u val, XMM128 &reg)
                {
                    CBL_SSE2::Load_1x4_32f(reg.f, (Fw32f) val);
                }
            }
        }

        namespace C3
        {
            template<class TS, class TD>
            struct ValC : public fe2< TS, OPT_LEVEL::C3, TD, OPT_LEVEL::C3>
            {
                TD valC[3];
                XMM128 val0, val1, val2;

                ValC(const TD valC_[3])
                {
                    valC[0] = valC_[0];
                    valC[1] = valC_[1];
                    valC[2] = valC_[2];
                }
            };

            namespace To8u
            {
                ISV From8u(Fw8u val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw8u valArray[3] = {val[0], val[1], val[2]};

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 2);
                }
            }

            namespace To16s
            {
                ISV From8u(Fw8u val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw16s valArray[3] = {val[0], val[1], val[2]};

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 2);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 1);
                }

                ISV From16s(Fw16s val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    FW_SSE2::Load2(reg0, val, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, val, OPT_LEVEL::C3, 2);
                    FW_SSE2::Load2(reg2, val, OPT_LEVEL::C3, 1);
                }
            }

            namespace To16sc
            {
                ISV From16sc(Fw16sc val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw16sc valArray[3];
                    valArray[0].re = val[0].re; valArray[0].im = val[0].im;
                    valArray[1].re = val[1].re; valArray[1].im = val[1].im;
                    valArray[2].re = val[2].re; valArray[2].im = val[2].im;


                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 2);
                }
            }

            namespace To16u
            {
                ISV From8u(Fw8u val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw16u valArray[3] = {val[0], val[1], val[2]};

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 2);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 1);
                }
            }

            namespace To32s
            {
                ISV From8u(Fw8u val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw32s valArray[3] = {val[0], val[1], val[2]};

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 2);
                }
                ISV From16s(Fw16s val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw32s valArray[3] = {val[0], val[1], val[2]};

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 2);
                }
                ISV From16u(Fw16u val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw32s valArray[3] = {val[0], val[1], val[2]};

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 2);
                }
            }

            namespace To32sc
            {
                ISV From16sc(Fw16sc val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw32sc valArray[3];
                    valArray[0].re = val[0].re; valArray[0].im = val[0].im;
                    valArray[1].re = val[1].re; valArray[1].im = val[1].im;
                    valArray[2].re = val[2].re; valArray[2].im = val[2].im;

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 2);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 1);
                }
                ISV From32sc(Fw32sc val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    FW_SSE2::Load2(reg0, val, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, val, OPT_LEVEL::C3, 2);
                    FW_SSE2::Load2(reg2, val, OPT_LEVEL::C3, 1);
                }
            }

            namespace To32f
            {
                ISV From8u(Fw8u val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw32f valArray[3] = { (float) val[0], (float) val[1], (float) val[2]};

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 2);
                }
                ISV From16s(Fw16s val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw32f valArray[3] = { (float) val[0], (float) val[1], (float) val[2]};

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 2);
                }
                ISV From16u(Fw16u val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    Fw32f valArray[3] = { (float) val[0], (float) val[1], (float) val[2]};

                    FW_SSE2::Load2(reg0, valArray, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, valArray, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(reg2, valArray, OPT_LEVEL::C3, 2);
                }

                ISV From32f(Fw32f val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    FW_SSE2::Load2(reg0, val, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, val, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(reg2, val, OPT_LEVEL::C3, 2);
                }
            }

            namespace To32fc
            {
                ISV From32fc(Fw32fc val[], XMM128 &reg0, XMM128 &reg1, XMM128 &reg2)
                {
                    FW_SSE2::Load2(reg0, val, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(reg1, val, OPT_LEVEL::C3, 2);
                    FW_SSE2::Load2(reg2, val, OPT_LEVEL::C3, 1);
                }
            }
        }

        namespace AC4
        {
            template<class TS, class TD>
            struct ValC : public fe2< TS, OPT_LEVEL::AC4, TD, OPT_LEVEL::AC4>
            {
                TD valC[3];
                XMM128 val;

                ValC(const TD valC_[3])
                {
                    valC[0] = valC_[0];
                    valC[1] = valC_[1];
                    valC[2] = valC_[2];
                }
            };

            namespace To8u
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw8u valArray[3] = {val[0], val[1], val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }
            }

            namespace To16s
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw16s valArray[3] = {val[0], val[1], val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }
                ISV From16s(Fw16s val[], XMM128 &reg)
                {
                    FW_SSE2::Load2(reg, val, OPT_LEVEL::AC4, 0);
                }
            }

            namespace To16sc
            {
                ISV From16sc(Fw16sc val[], XMM128 &reg)
                {
                    Fw16sc valArray[3] = {val[0], val[1], val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }
            }

            namespace To16u
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw16u valArray[3] = {val[0], val[1], val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }
            }

            namespace To32s
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw32s valArray[3] = {val[0], val[1], val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }
                ISV From16s(Fw16s val[], XMM128 &reg)
                {
                    Fw32s valArray[3] = {val[0], val[1], val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }
                ISV From16u(Fw16u val[], XMM128 &reg)
                {
                    Fw32s valArray[3] = {val[0], val[1], val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }
            }

            namespace To32sc
            {
                ISV From16sc(Fw16sc val[], XMM128 &reg0, XMM128 &reg1)
                {
                    reg0.s32[0] = val[0].re;
                    reg0.s32[1] = val[0].im;
                    reg0.s32[2] = val[1].re;
                    reg0.s32[3] = val[1].im;

                    reg1.s32[0] = val[2].re;
                    reg1.s32[1] = val[2].im;
                    reg1.s32[2] = 0;
                    reg1.s32[3] = 0;
                }
                ISV From32sc(Fw32sc val[], XMM128 &reg0, XMM128 &reg1)
                {
                    reg0.s32[0] = val[0].re;
                    reg0.s32[1] = val[0].im;
                    reg0.s32[2] = val[1].re;
                    reg0.s32[3] = val[1].im;

                    reg1.s32[0] = val[2].re;
                    reg1.s32[1] = val[2].im;
                    reg1.s32[2] = 0;
                    reg1.s32[3] = 0;
                }
            }

            namespace To32f
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw32f valArray[3] = { (float) val[0], (float) val[1], (float) val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }
                ISV From16s(Fw16s val[], XMM128 &reg)
                {
                    Fw32f valArray[3] = { (float) val[0], (float) val[1], (float) val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }
                ISV From16u(Fw16u val[], XMM128 &reg)
                {
                    Fw32f valArray[3] = { (float) val[0], (float) val[1], (float) val[2]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::AC4, 0);
                }

                ISV From32f(Fw32f val[], XMM128 &reg)
                {
                    FW_SSE2::Load2(reg, val, OPT_LEVEL::AC4, 0);
                }
            }

            namespace To32fc
            {
                ISV From32fc(Fw32fc val[], XMM128 &reg0, XMM128 &reg1)
                {
                    reg0.f32[0] = val[0].re;
                    reg0.f32[1] = val[0].im;
                    reg0.f32[2] = val[1].re;
                    reg0.f32[3] = val[1].im;

                    reg1.f32[0] = val[2].re;
                    reg1.f32[1] = val[2].im;
                    reg1.f32[2] = 0;
                    reg1.f32[3] = 0;
                }
            }

        }

        namespace C4
        {
            template<class TS, class TD>
            struct ValC : public fe2< TS, OPT_LEVEL::C4, TD, OPT_LEVEL::C4>
            {
                TD valC[4];
                XMM128 val;

                ValC(const TD valC_[4])
                {
                    valC[0] = valC_[0];
                    valC[1] = valC_[1];
                    valC[2] = valC_[2];
                    valC[3] = valC_[3];
                }
            };

            namespace To8u
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw8u valArray[4] = {val[0], val[1], val[2], val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
            }

            namespace To16s
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw16s valArray[4] = {val[0], val[1], val[2], val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
                ISV From16s(Fw16s val[], XMM128 &reg)
                {
                    FW_SSE2::Load2(reg, val, OPT_LEVEL::C4, 0);
                }
            }

            namespace To16sc
            {
                ISV From16sc(Fw16sc val[], XMM128 &reg)
                {
                    Fw16sc valArray[4] = {val[0], val[1], val[2], val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
            }

            namespace To16u
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw16u valArray[4] = {val[0], val[1], val[2], val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
            }

            namespace To32s
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw32s valArray[4] = {val[0], val[1], val[2], val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
                ISV From16s(Fw16s val[], XMM128 &reg)
                {
                    Fw32s valArray[4] = {val[0], val[1], val[2], val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
                ISV From16u(Fw16u val[], XMM128 &reg)
                {
                    Fw32s valArray[4] = {val[0], val[1], val[2], val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
            }

            namespace To32f
            {
                ISV From8u(Fw8u val[], XMM128 &reg)
                {
                    Fw32f valArray[4] = { (float) val[0], (float) val[1], (float) val[2], (float) val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
                ISV From16s(Fw16s val[], XMM128 &reg)
                {
                    Fw32f valArray[4] = { (float) val[0], (float) val[1], (float) val[2], (float) val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
                ISV From16u(Fw16u val[], XMM128 &reg)
                {
                    Fw32f valArray[4] = { (float) val[0], (float) val[1], (float) val[2], (float) val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }

                ISV From32f(Fw32f val[], XMM128 &reg)
                {
                    FW_SSE2::Load2(reg, val, OPT_LEVEL::C4, 0);
                }
            }

            namespace To32fc
            {
                ISV From32fc(Fw32fc val[], XMM128 &reg)
                {
                    Fw32fc valArray[4] = {val[0], val[1], val[2], val[3]};
                    FW_SSE2::Load2(reg, valArray, OPT_LEVEL::C4, 0);
                }
            }
        }
    }

    namespace InitScale
    {
        ISV To16s(int scale, XMM128 &reg)
        {
            Fw32f scaleF = (Fw32f) pow(2.0, -scale);
            CBL_SSE2::Load_1x8_16s(reg.i, (short) scaleF);
        }

        ISV To32f(int scale, XMM128 &reg)
        {
            Fw32f scaleF = (Fw32f) pow(2.0, -scale);
            CBL_SSE2::Load_1x4_32f(reg.f, scaleF);
        }

        ISV To64f(int scale, XMM128 &reg)
        {
            Fw64f scaleD = (Fw64f) pow(2.0, -scale);
            CBL_SSE2::Load_1x2_64f(reg.d, scaleD);
        }
    }


    namespace InitMisc
    {
        namespace Min
        {
            ISV Of32s(XMM128 &reg)
            {
                reg.i = _mm_cmpeq_epi32(reg.i, reg.i);
                reg.i = _mm_slli_epi32(reg.i, 31);
            }
        }

        namespace Max
        {
            ISV Of8u(XMM128 &reg)
            {
                reg.i = _mm_cmpeq_epi32(reg.i, reg.i);
            }
            ISV Of32s(XMM128 &reg)
            {
                reg.i = _mm_cmpeq_epi32(reg.i, reg.i);
                reg.i = _mm_srli_epi32(reg.i, 1);
            }
            ISV Of64s(XMM128 &reg)
            {
                reg.i = _mm_cmpeq_epi32(reg.i, reg.i);
                reg.i = _mm_srli_epi64(reg.i, 1);
            }
        }

        namespace Constants
        {
            ISV negamask32f(XMM128 &reg)
            {
                reg.i = _mm_set_epi32(0x00000000, 0x80000000, 0x00000000, 0x80000000);
            }

            ISV negbmask32f(XMM128 &reg)
            {
                reg.i = _mm_set_epi32(0x80000000, 0x00000000, 0x80000000, 0x00000000);
            }


            ISV negbmask64f(XMM128 &reg)
            {
                reg.i = _mm_set_epi32(0x80000000, 0x00000000, 0x00000000, 0x00000000);
            }

            ISV negbmask16s(XMM128 &reg)
            {
                reg.i = _mm_cmpeq_epi32(reg.i, reg.i);
                reg.i = _mm_slli_epi32(reg.i, 16);
            }

            ISV half32f(XMM128 &reg)
            {
                reg.f = _mm_set1_ps(.5);
            }

            ISV half64f(XMM128 &reg)
            {
                reg.d = _mm_set1_pd(.5);
            }
        }
    }
}


} // namespace OPT_LEVEL

#endif // __INITIMPL_H__

