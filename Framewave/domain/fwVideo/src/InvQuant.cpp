/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/************************************************************/
#include "buildnum.h"
#include "fwVideo.h"
#include "FwSharedCode.h"
#include "FwSharedCode_SSE2.h"

#include <iostream>

using namespace OPT_LEVEL;

extern const unsigned int NUM_COLS;
extern const unsigned int NUM_ELEMS;

namespace OPT_LEVEL
{

FwStatus SYS_INLINE quantInvIntra_MPEG2(Fw16s *pSrcDst, int &QP, Fw16s *pQPMatrix)
{
	if(FW_REF::PtrNotOK(pSrcDst, pQPMatrix))return fwStsNullPtrErr;

	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
		{
			const bool bSrcDstIsAligned = FW_REF::IsAligned(pSrcDst,16);
			const bool bQPMatrixIsAligned = FW_REF::IsAligned(pQPMatrix,16);
			__m128i qpConst = _mm_set_epi16(CBL_LIBRARY::Limits<Fw16s>::Sat(QP), CBL_LIBRARY::Limits<Fw16s>::Sat(QP), CBL_LIBRARY::Limits<Fw16s>::Sat(QP), CBL_LIBRARY::Limits<Fw16s>::Sat(QP), CBL_LIBRARY::Limits<Fw16s>::Sat(QP), CBL_LIBRARY::Limits<Fw16s>::Sat(QP), CBL_LIBRARY::Limits<Fw16s>::Sat(QP), CBL_LIBRARY::Limits<Fw16s>::Sat(QP));
			__m128i row;
			__m128i qpRow;
            XMM128 mismatch;
            __m128i mask_negative;
            __m128i zero = _mm_setzero_si128();
            mismatch.i = _mm_setzero_si128();
            Fw16s src1 = pSrcDst[0];

            mismatch.s16[0] = pSrcDst[0] ^ 1;
            pSrcDst[0]= 0;
            
			for(unsigned int I = 0; I < NUM_COLS; I++)
			{
				row = (bSrcDstIsAligned)? _mm_load_si128(reinterpret_cast<__m128i*>(pSrcDst) + I) : _mm_loadu_si128(reinterpret_cast<__m128i*>(pSrcDst) + I);
				qpRow = (bQPMatrixIsAligned)? _mm_load_si128(reinterpret_cast<__m128i*>(pQPMatrix) + I) : _mm_loadu_si128(reinterpret_cast<__m128i*>(pQPMatrix) + I);

                mask_negative = _mm_cmplt_epi16(row, zero);

                __m128i negative_nums = _mm_and_si128(mask_negative, row);
                negative_nums = _mm_subs_epi16(zero, negative_nums);
                row = _mm_andnot_si128(mask_negative, row);
                row = _mm_or_si128(row, negative_nums);

				__m128i low = _mm_mullo_epi16(row, qpRow);
				__m128i high = _mm_mulhi_epi16(row, qpRow);
				row = _mm_unpacklo_epi16(low, high);
				__m128i tmpHigh = _mm_unpackhi_epi16(low, high);
				FW_SSE2::pack32STo16S(row, tmpHigh);

				low = _mm_mullo_epi16(row, qpConst);
				high = _mm_mulhi_epi16(row, qpConst);
				row = _mm_unpacklo_epi16(low, high);
				tmpHigh = _mm_unpackhi_epi16(low, high);
                FW_SSE2::pack32STo16S(row, tmpHigh);

                row = _mm_srli_epi16(row, 4); 
                negative_nums = _mm_and_si128(mask_negative, row);
                negative_nums = _mm_subs_epi16(zero, negative_nums);
                row = _mm_andnot_si128(mask_negative, row);
                row = _mm_or_si128(row, negative_nums);
				
                mismatch.i = _mm_xor_si128(row,mismatch.i);

				__m128i* p128i = reinterpret_cast<__m128i*>(pSrcDst);
				if(bSrcDstIsAligned)
				{
					_mm_store_si128(&p128i[I], row);
				}
				else
				{
					_mm_storeu_si128(&p128i[I], row);
				}
			}

			mismatch.s16[0] ^= mismatch.s16[1] ^  mismatch.s16[2] ^  mismatch.s16[3] ^ mismatch.s16[4];
			pSrcDst[63] ^= (mismatch.s16[0] ^ mismatch.s16[5] ^  mismatch.s16[6] ^  mismatch.s16[7]) & 1;
            pSrcDst[0] = src1;
		}
		break;
	case DT_REFR:
	default:
        {
        int mismatch;
        mismatch = pSrcDst[0] ^ 1;

		for(unsigned int I = 1; I < NUM_ELEMS; I++)
		{
            int AcCoefficient;

            if(pSrcDst[I]<0)
                {
                AcCoefficient = CBL_LIBRARY::Limits<Fw16s>::Sat((-pSrcDst[I] * QP * pQPMatrix[I])>>4);
                AcCoefficient = -AcCoefficient;
                }
            else
                AcCoefficient = CBL_LIBRARY::Limits<Fw16s>::Sat((pSrcDst[I] * QP * pQPMatrix[I])>>4);

            mismatch^= AcCoefficient;
            pSrcDst[I] = (Fw16s)AcCoefficient;
         }
        pSrcDst[63]^= mismatch&1;
        break;
        }
     }
	return fwStsNoErr;
}

FwStatus SYS_INLINE quantInv_MPEG2(Fw16s *pSrcDst, int &QP, Fw16s *pQPMatrix)
{
	if(FW_REF::PtrNotOK(pSrcDst, pQPMatrix))return fwStsNullPtrErr;

	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
		{
			const bool bSrcDstIsAligned = FW_REF::IsAligned(pSrcDst,16);
			const bool bQPMatrixIsAligned = FW_REF::IsAligned(pQPMatrix,16);
			__m128i qpConst = _mm_set1_epi16(CBL_LIBRARY::Limits<Fw16s>::Sat(QP));
			__m128i row;
			__m128i qpRow;
            __m128i mask_zero, mask_negative;
			static const __m128i one = _mm_set1_epi16(1);
            static const __m128i zero = _mm_setzero_si128();

			XMM128 mismatch;
			mismatch.i= _mm_set_epi16(0,0,0,0,0,0,0,1);
			for(unsigned int I = 0; I < NUM_COLS; I++)
			{
				row = (bSrcDstIsAligned)? _mm_load_si128(reinterpret_cast<__m128i*>(pSrcDst) + I) : _mm_loadu_si128(reinterpret_cast<__m128i*>(pSrcDst) + I);
				qpRow = (bQPMatrixIsAligned)? _mm_load_si128(reinterpret_cast<__m128i*>(pQPMatrix) + I) : _mm_loadu_si128(reinterpret_cast<__m128i*>(pQPMatrix) + I);
				
                mask_zero = _mm_cmpeq_epi16(row, zero);
                mask_negative = _mm_cmplt_epi16(row, zero);

                __m128i negative_nums = _mm_and_si128(mask_negative, row);
                negative_nums = _mm_subs_epi16(zero, negative_nums);
                row = _mm_andnot_si128(mask_negative, row);
                row = _mm_or_si128(row, negative_nums);

				row = _mm_adds_epi16(row,row);
				row = _mm_adds_epi16(row,one);

				__m128i low = _mm_mullo_epi16(row, qpRow);
				__m128i high = _mm_mulhi_epi16(row, qpRow);
				row = _mm_unpacklo_epi16(low, high);
				__m128i tmpHigh = _mm_unpackhi_epi16(low, high);
				FW_SSE2::pack32STo16S(row, tmpHigh);

				low = _mm_mullo_epi16(row, qpConst);
				high = _mm_mulhi_epi16(row, qpConst);
				row = _mm_unpacklo_epi16(low, high);
				tmpHigh = _mm_unpackhi_epi16(low, high);
                FW_SSE2::pack32STo16S(row, tmpHigh);

                row = _mm_srli_epi16(row, 5); 
                negative_nums = _mm_and_si128(mask_negative, row);
                negative_nums = _mm_subs_epi16(zero, negative_nums);
                row = _mm_andnot_si128(mask_negative, row);
                row = _mm_or_si128(row, negative_nums);

                row = _mm_andnot_si128(mask_zero, row);
				mismatch.i = _mm_xor_si128(row,mismatch.i);

				__m128i* p128i = reinterpret_cast<__m128i*>(pSrcDst);
				if(bSrcDstIsAligned)
				{
					_mm_store_si128(&p128i[I], row);
				}
				else
				{
					_mm_storeu_si128(&p128i[I], row);
				}
			}

            mismatch.s16[0] ^= mismatch.s16[1] ^  mismatch.s16[2] ^  mismatch.s16[3] ^ mismatch.s16[4];
			pSrcDst[63] ^= (mismatch.s16[0] ^ mismatch.s16[5] ^  mismatch.s16[6] ^  mismatch.s16[7]) & 1;

		}
		break;
	case DT_REFR:
	default:
        {
        int mismatch = 1;

        for(unsigned int I = 0; I < NUM_ELEMS; I++)
		{
            int coefficient;
            
            if(pSrcDst[I]==0)
                coefficient = 0;
            else if(pSrcDst[I]<0)
                {
                coefficient = CBL_LIBRARY::Limits<Fw16s>::Sat((( 2*(-pSrcDst[I]) + 1) * QP * pQPMatrix[I])>>5);
                coefficient = -coefficient;
                }
            else
                coefficient = CBL_LIBRARY::Limits<Fw16s>::Sat(((2 * pSrcDst[I] + 1) * QP * pQPMatrix[I])>>5);

            mismatch^= coefficient;
            pSrcDst[I] = (Fw16s)coefficient;
         }
        pSrcDst[63]^= mismatch&1;
        break;
        }
     }
	return fwStsNoErr;

}
} // end namespace OPT_LEVEL
/************************************************************/
FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiQuantInvIntra_MPEG2_16s_C1I)(Fw16s *pSrcDst, int QP, Fw16s *pQPMatrix)
{
	return quantInvIntra_MPEG2(pSrcDst, QP, pQPMatrix);
}

FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiQuantInv_MPEG2_16s_C1I)(Fw16s *pSrcDst, int QP, Fw16s *pQPMatrix)
{
	return quantInv_MPEG2(pSrcDst, QP, pQPMatrix);
}




// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
