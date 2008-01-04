/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
//#include "fwImage.h"
#include "fwVideo.h"
#include "system.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST(101)

#define Q_BITS          15

extern SYS_FORCEALIGN_16 const int dequant_coef[6][4][4];
extern SYS_FORCEALIGN_16 const Fw16s dequant_coef16[6*4*4];
extern SYS_FORCEALIGN_16 const Fw8u QP_SCALE_CR[52];
extern const int quant_coef[6][4][4];
extern const int A[4][4];
extern const int QuantCoef[6][16];
extern const int MATR[16];
extern const Fw32s QuantIndex1[16];
extern const Fw32s InvQuantTable1[52][3];

#undef  ABS
#define ABS(a)          (((a) < 0) ? (-(a)) : (a))

#undef  MAX
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))

#undef  MIN
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))

#undef  SIGN
#define SIGN(a)         ((a) < 0 ? (-1) : (1))

#define MAX_CAVLC_LEVEL_VALUE	2063
#define absm(A) ((A)<(0) ? (-(A)):(A)) //!< abs macro, faster than procedure
int static sign(int a,int b)
{
  int x;
  x=absm(a);
  if (b >= 0)   return x;
  else		    return -x;
}
/*
Parameters
pSrcDst- Pointer to a 2x2 chroma DC block - source and destination array of size 4.
pTBlock- Pointer to a 2x2 transformed chroma DC block - source or destination array of size 4.
QPChroma- Quantization parameter for chroma, in range [0, 39].
NumLevels- Pointer to value, which contains:
	 a negative value of a number of non-zero elements in block after
		quantization (when the first quantized element in block is not equal to zero),
	 a number of non-zero elements in block after quantization (when
		the first quantized element in block is equal to zero).
		This value is calculated by the function.
Intra- Flag equal to 1 in the case of Intra macroblock, 0 otherwise	.
NeedTransform- Flag that is equal to 1 if transforming process is used. This flag is equal
		to 0 if transforming process is not used.
*/
FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformQuantChromaDC_H264_16s_C1I)(Fw16s*	pSrcDst, 
												  Fw16s*	pTBlock, 
												  Fw32s	QPChroma, 
												  Fw8s*	NumLevels, 
												  Fw8u		Intra, 
												  Fw8u 	NeedTransform)
{
	int qp_const;
	int level,coeff_ctr, count;
//	Fw16s tmp0[4];

	if(QPChroma < 0 || QPChroma > 39) return fwStsOutOfRangeErr;

    if (pSrcDst == NULL || pTBlock == NULL ) return fwStsNullPtrErr;

	int qp_per = QPChroma/6;
	int qp_rem = QPChroma%6;
	int q_bits = Q_BITS+qp_per;

    if (Intra)
       qp_const=(1<<q_bits)/3;    // intra
	 else
	   qp_const=(1<<q_bits)/6;    // inter

	if(NeedTransform)
	{
#if 0
		  //  Horizontal transform
		for (j=0;j<2;j++)
		{
//			for (i=0;i<2;i++)
//			{
//			  i1=3-i;
//			  tmp0[i]= pSrcDst[i*4+j]+pSrcDst[i*2+j];
//			  tmp0[i1]=pSrcDst[i*4+j]-pSrcDst[i1*2+j];
//			}
			pSrcDst[0*2+j]=tmp0[0]+tmp0[1];
			pSrcDst[2*2+j]=tmp0[0]-tmp0[1];
			pSrcDst[1*2+j]=tmp0[3]+tmp0[2];
			pSrcDst[3*2+j]=tmp0[3]-tmp0[2];
		}
		  // vertical
		for (i=0;i<2;i++)
		{
			for (j=0;j<2;j++)
			{
			  j1=3-j;
			  tmp0[j]= pSrcDst[i*2+j]+pSrcDst[i*2+j1];
			  tmp0[j1]=pSrcDst[i*2+j]-pSrcDst[i*2+j1];
			}
			pTBlock[i*2+0]=(tmp0[0]+tmp0[1])>>1;
			pTBlock[i*2+2]=(tmp0[0]-tmp0[1])>>1;
			pTBlock[i*2+1]=(tmp0[3]+tmp0[2])>>1;
			pTBlock[i*2+3]=(tmp0[3]-tmp0[2])>>1;
		 }
#else
		pTBlock[0] = pSrcDst[0] + pSrcDst[2] + pSrcDst[1] + pSrcDst[3];
		pTBlock[1] = pSrcDst[0] + pSrcDst[2] - pSrcDst[1] - pSrcDst[3];
		pTBlock[2] = pSrcDst[0] - pSrcDst[2] + pSrcDst[1] - pSrcDst[3];
		pTBlock[3] = pSrcDst[0] - pSrcDst[2] - pSrcDst[1] + pSrcDst[3];
#endif
	}

	// quant
	count = 0;
	for (coeff_ctr=0;coeff_ctr<4;coeff_ctr++)
	{
	    level =(abs(pTBlock[coeff_ctr]) * quant_coef[qp_rem][0][0] + 2*qp_const) >> (q_bits+1);
//		pSrcDst[i]=(Fw16s) sign(level,pTBlock[i]);
//		if(pSrcDst[i] > MAX_CAVLC_LEVEL_VALUE) return fwStsScaleRangeErr;
		pSrcDst[coeff_ctr]=(Fw16s) sign(level,pTBlock[coeff_ctr]);
		if(pSrcDst[coeff_ctr] > MAX_CAVLC_LEVEL_VALUE) return fwStsScaleRangeErr;
	}
	// after quantization
	for (coeff_ctr=0;coeff_ctr<4;coeff_ctr++)
	{
		if(pSrcDst[coeff_ctr] != 0){
			count++;
		}
	}

	if(pSrcDst[0] == 0) *NumLevels = (Fw8s)count; 
	else				*NumLevels = (Fw8s)count;

	return fwStsNoErr;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformQuantLumaDC_H264_16s_C1I)(Fw16s *pSrcDst, 
												Fw16s *pTBlock, 
												Fw32s  QP, 
												Fw8s  *NumLevels, 
												Fw8u   NeedTransform, 
												Fw16s *pScanMatrix, 
												Fw8u  *LastCoeff)
{
	int i, j, i1, j1;
	Fw16s tmp0[4];
	if(QP < 0 || QP > 51) return fwStsOutOfRangeErr;

    if (pSrcDst == NULL || pTBlock == NULL || pScanMatrix == NULL) return fwStsNullPtrErr;

	int qp_per = QP/6;
	int qp_rem = QP%6;
	int q_bits = Q_BITS+qp_per;
	int  qp_const  = (1<<q_bits)/3;

	if(NeedTransform)
	{
	  //  Horizontal transform
		for (j=0;j<4;j++)
		{
			for (i=0;i<2;i++)
			{
			  i1=3-i;
			  tmp0[i]= pSrcDst[i*4+j]+pSrcDst[i*4+j];
			  tmp0[i1]=pSrcDst[i*4+j]-pSrcDst[i1*4+j];
			}
			pSrcDst[0*4+j]=tmp0[0]+tmp0[1];
			pSrcDst[2*4+j]=tmp0[0]-tmp0[1];
			pSrcDst[1*4+j]=tmp0[3]+tmp0[2];
			pSrcDst[3*4+j]=tmp0[3]-tmp0[2];
		}
		  // vertical
		for (i=0;i<4;i++)
		{
			for (j=0;j<2;j++)
			{
			  j1=3-j;
			  tmp0[j]= pSrcDst[i*4+j]+pSrcDst[i*4+j1];
			  tmp0[j1]=pSrcDst[i*4+j]-pSrcDst[i*4+j1];
			}
			pTBlock[i*4+0]=(tmp0[0]+tmp0[1])>>1;
			pTBlock[i*4+2]=(tmp0[0]-tmp0[1])>>1;
			pTBlock[i*4+1]=(tmp0[3]+tmp0[2])>>1;
			pTBlock[i*4+3]=(tmp0[3]-tmp0[2])>>1;
		}
	}

	// quant
	int level,coeff_ctr, count;
	count = 0;
	for (coeff_ctr=0;coeff_ctr<16;coeff_ctr++)
	{
		i=pScanMatrix[coeff_ctr];
		level= (abs(pTBlock[i]) * quant_coef[qp_rem][0][0] + 2*qp_const)>>(q_bits+1);
		pSrcDst[i]=(Fw16s)sign(level,pTBlock[i]);
		if(pSrcDst[i] > MAX_CAVLC_LEVEL_VALUE) return fwStsScaleRangeErr;
	}
	// after quantization
	for (coeff_ctr=0;coeff_ctr<16;coeff_ctr++)
	{
		if(pSrcDst[coeff_ctr] != 0){
			count++;
			*LastCoeff = (Fw8u)coeff_ctr;
		}
	}

	if(pSrcDst[0] == 0) *NumLevels = (Fw8s)count; 
	else				*NumLevels = (Fw8s)count;
	return fwStsNoErr;
}
/*
Parameters
pSrcDst- Pointer to a 4x4 residual block - source and destination array of size 16.
QP- Quantization parameter for luma or for chroma, in range [0, 51] or [0, 39].
NumLevels- Pointer to value, which contains:
	• a negative value of a number of non-zero elements in block after
		quantization (when the first quantized element in block is not equal to zero),
	• a number of non-zero elements in block after quantization (when
		the first quantized element in block is equal to zero). This value is calculated by the function.
Intra- Flag equal to 1 in the case of Intra slice, 0 otherwise.
pScanMatrix- Scan matrix for coefficients in block (array of size 16).
LastCoeff- Position of the last (in order of pScanMatrix) non-zero coefficient in
block- after quantization. This value is calculated by the function.
*/
FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformQuantResidual_H264_16s_C1I)(Fw16s*	pSrcDst, 
												  Fw32s	QP,
												  Fw8s*	NumLevels,
												  Fw8u		Intra, 
												  Fw16s*	pScanMatrix, 
												  Fw8u*	LastCoeff)
{
	int i,j, i1, j1;
	Fw16s tmp0[4];

	NumLevels;

	if(QP < 0 || QP > 51) return fwStsOutOfRangeErr;
    if (pSrcDst == NULL || pScanMatrix == NULL) return fwStsNullPtrErr;

	int qp_per = QP/6;
	int qp_rem = QP%6;
	int q_bits = Q_BITS+qp_per;
	int qp_const;

    if (Intra)
       qp_const=(1<<q_bits)/3;    // intra
	 else
	   qp_const=(1<<q_bits)/6;    // inter

    //  Horizontal transform
	for (j=0;j<4;j++)
	{
		for (i=0;i<2;i++)
		{
		  i1=3-i;
		  tmp0[i] =pSrcDst[i*4+j]+pSrcDst[i1*4+j];
		  tmp0[i1]=pSrcDst[i*4+j]-pSrcDst[i1*4+j];
		}
		pSrcDst[0*4+j]=tmp0[0]+tmp0[1];
		pSrcDst[2*4+j]=tmp0[0]-tmp0[1];
		pSrcDst[1*4+j]=tmp0[3]*2+tmp0[2];
		pSrcDst[3*4+j]=tmp0[3]-tmp0[2]*2;
	}
	// vertical
	for (i=0;i<4;i++)
	{
		for (j=0;j<2;j++)
		{
		  j1=3-j;
		  tmp0[j]= pSrcDst[i*4+j]+pSrcDst[i*4+j1];
		  tmp0[j1]=pSrcDst[i*4+j]-pSrcDst[i*4+j1];
		}
		pSrcDst[i*4+0]=tmp0[0]+tmp0[1];
		pSrcDst[i*4+2]=tmp0[0]-tmp0[1];
		pSrcDst[i*4+1]=tmp0[3]*2+tmp0[2];
		pSrcDst[i*4+3]=tmp0[3]-tmp0[2]*2;
	}
	// quant
	int level,coeff_ctr, count;
	count = 0;
	for (coeff_ctr=0;coeff_ctr<16;coeff_ctr++)
	{
		i=pScanMatrix[coeff_ctr];

        level= ( abs( pSrcDst[i]) * quant_coef[qp_rem][i/4][i%4] + qp_const) >> q_bits;
		pSrcDst[i]=(Fw16s)sign(level*dequant_coef[qp_rem][i/4][i%4]<<qp_per,pSrcDst[i]);
		if(pSrcDst[i] > MAX_CAVLC_LEVEL_VALUE) return fwStsScaleRangeErr;
	}
	// after quantization
	for (coeff_ctr=0;coeff_ctr<16;coeff_ctr++)
	{
		if(pSrcDst[i] != 0){
			count++;
			*LastCoeff = (Fw8u)coeff_ctr;
		}
	}

	return fwStsNoErr;
}

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformDequantLumaDC_H264_16s_C1I)(Fw16s* pSrcDst, Fw32s QP)
{
    int i,j;
//    Fw16s tmp0[4];
    Fw16s t0, t1, t2, t3;

//	int Dispatch_Type = Dispatch::Type<DT_SSE2>();

#if 0
    Fw16s tmp1[4];
	Fw16s* tempa = (Fw16s*) fwMalloc(8*sizeof(Fw16s));
	Fw8u*  tempb = (Fw8u*) fwMalloc(16*sizeof(Fw8u));
#endif
  int qp_per = QP/6;
  int qp_rem = QP%6;
//#if 1
/*
if( Dispatch_Type ==DT_SSE2)
{
  	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5;

	//  Horizontal transform
		// movdqa  xmm0, [eax]	// xmm0 = [r1, r0]
		xmm0 = _mm_load_si128( (__m128i*)pSrcDst );
		// movdqa  xmm2, [eax+16]		// xmm2 = [r3, r2]
		xmm2 = _mm_load_si128( (__m128i*)(pSrcDst+8) );
		//	pshufd	xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm4 = xmm0;
		//	paddw		xmm0, xmm2			// xmm0 = [r1+r3|r0+r2]
		xmm0 =	_mm_add_epi16(xmm0, xmm2);			// xmm0 = [r1+r3|r0+r2]
		//	psubw		xmm4, xmm2			// xmm4 = [r1-r3|r0-r2]
		xmm4 =	_mm_sub_epi16(xmm4, xmm2);			// xmm0 = [r1+r3|r0+r2]
		// pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4			// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		// punpckhqdq	xmm3, xmm4			// xmm3 = [r1-r3|r1+r3]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		// pshufd		xmm2, xmm0, 0xe4
//		xmm2 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm2 = xmm0;
		// paddw		xmm0, xmm3			// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3			// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			
#if 0
{
_mm_storeu_si128( (__m128i*)tempa, xmm0 ); // store 16 bytes
printf("\n %3d %3d %3d %3d",tempa[0], tempa[1],tempa[2],tempa[3]);
printf("\n %3d %3d %3d %3d",tempa[4],tempa[5],tempa[6],tempa[7]);
_mm_storeu_si128( (__m128i*)tempa, xmm2 ); // store 16 bytes
printf("\n %3d %3d %3d %3d",tempa[0], tempa[1],tempa[2],tempa[3]);
printf("\n %3d %3d %3d %3d",tempa[4],tempa[5],tempa[6],tempa[7]);
getchar();
}
#endif
	// Transpose
		//	pshufd	  xmm5, xmm0, 0xe4		// xmm5 = xmm0
//		xmm5 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm5 = xmm0;
		//	pshufd	  xmm2, xmm2, 0x4e		// xmm2 = [y3 |y2]
		xmm2 = _mm_shuffle_epi32(xmm2, 0x4e);
		//  punpcklwd xmm0, xmm2			// xmm0 = [y23|y03|y22|y02|y21|y01|y20|y00]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);
		//	punpckhwd xmm5, xmm2			// xmm5 = [y33|y13|y32|y12|y31|y11|y30|y10]
		xmm5 = _mm_unpackhi_epi16(xmm5, xmm2);
		//	movdqa	  xmm1, xmm0
		xmm1 = xmm0;
		//	punpcklwd xmm0, xmm5		// xmm0 = [y31|y21|y11|y01|y30|y20|y10|y00]=[y1|y0]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm5);
		//	punpckhwd xmm1, xmm5		// xmm1 = [y33|y23|y13|y03|y32|y22|y12|y02]=[y3|y2]
		xmm1 = _mm_unpackhi_epi16(xmm1, xmm5);
		//	pshufd	  xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm4 = xmm0;

	// vertical
		//	paddw		xmm0, xmm1			// xmm0 = [r1+r3|r0+r2]
		xmm0 =	_mm_add_epi16(xmm0, xmm1);			
		//	psubw		xmm4, xmm1			// xmm4 = [r1-r3|r0-r2]
		xmm4 =	_mm_sub_epi16(xmm4, xmm1);			
		//	pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4		// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		//	punpckhqdq	xmm3, xmm4		// xmm3 = [r1-r3|r1+r3]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		//	movdqa		xmm2, xmm0
		xmm2 = xmm0;
		//	paddw		xmm0, xmm3		// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3		// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			

		xmm4 = _mm_shuffle_epi32(xmm2, 0x4e);	// xmm4 = [y3 |y2]

#if 0
	fwFree(tempa);
	fwFree(tempb);
#endif

#if 0
	{
	//  int qp_per = QP/6;
	//  int qp_rem = QP%6;
		qp_rem *= 16;
		xmm1 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem) );
		xmm3 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem+8) );

//		pmullw  xmm0, xmm1
		xmm0 = _mm_mullo_epi16(xmm0, xmm1);
//		pmullw  xmm4, xmm3
		xmm4 = _mm_mullo_epi16(xmm4, xmm3);

		xmm5  = _mm_set1_epi16(1);    // round;
//		movd    xmm7, qp_per
//		psllw   xmm6, xmm7      ; << qbits
        __m128i qp_per_x = _mm_cvtsi32_si128(qp_per);
		xmm5  = _mm_sll_epi16 (xmm5, qp_per_x);

//		pmullw xmm0, xmm6
		xmm0 = _mm_mullo_epi16(xmm0, xmm5);
//		pmullw xmm4, xmm6
		xmm4 = _mm_mullo_epi16(xmm4, xmm5);

		_mm_store_si128( (__m128i*)pSrcDst, xmm0 ); // store 16 bytes
		_mm_store_si128( (__m128i*)(pSrcDst+8), xmm4 ); // store 16 bytes
	}
#else
//  for (i=0;i<4;i++)
//    for (j=0;j<4;j++)
//	   *(pSrcDst+i*4+j) = (Fw16s)(( (*(pSrcDst+i*4+j)) * QP )+128)>>8;

	{
		xmm1  = _mm_set1_epi16((short)(QP*256));		// QP << 8

		xmm0 = _mm_mulhi_epi16(xmm0, xmm1);	// (pSrcDst*QP<<8) >> 16
		xmm4 = _mm_mulhi_epi16(xmm4, xmm1);

		_mm_store_si128( (__m128i*)pSrcDst, xmm0 ); // store 16 bytes
		_mm_store_si128( (__m128i*)(pSrcDst+8), xmm4 ); // store 16 bytes
	}
#endif
}
else
*/
{
  	  //  Horizontal transform
	 for (i=0;i<4;i++)
	{
		t0 = pSrcDst[i * 4 + 0] + pSrcDst[i * 4 + 2];
		t1 = pSrcDst[i * 4 + 0] - pSrcDst[i * 4 + 2];
		t2 = pSrcDst[i * 4 + 1] - pSrcDst[i * 4 + 3];
		t3 = pSrcDst[i * 4 + 1] + pSrcDst[i * 4 + 3];
		pSrcDst[i * 4 + 0] = t0 + t3;
		pSrcDst[i * 4 + 3] = t0 - t3;
		pSrcDst[i * 4 + 1] = t1 + t2;
		pSrcDst[i * 4 + 2] = t1 - t2;
	}
	  // vertical
	for (i=0;i<4;i++)
	{
		t0 = pSrcDst[0 * 4 + i] + pSrcDst[2 * 4 + i];
		t1 = pSrcDst[0 * 4 + i] - pSrcDst[2 * 4 + i];
		t2 = pSrcDst[1 * 4 + i] - pSrcDst[3 * 4 + i];
		t3 = pSrcDst[1 * 4 + i] + pSrcDst[3 * 4 + i];

		pSrcDst[0 * 4 + i] = t0 + t3;
		pSrcDst[3 * 4 + i] = t0 - t3;
		pSrcDst[1 * 4 + i] = t1 + t2;
		pSrcDst[2 * 4 + i] = t1 - t2;
	}

  for (i=0;i<4;i++)
    for (j=0;j<4;j++)
//	   *(pSrcDst+i*4+j) = (Fw16s)(( (*(pSrcDst+i*4+j)) * QP )+128)>>8;
	   *(pSrcDst+i*4+j) = (Fw16s)(((*(pSrcDst+i*4+j))*dequant_coef[qp_rem][i][j]<<qp_per)+2)>>2;

}

//#endif
	return fwStsNoErr;
}
#endif

FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformDequantChromaDC_H264_16s_C1I)(Fw16s* pSrcDst, Fw32s QP)
{
//	int j;
	Fw16s tmp[4];
	int qp_per = QP/6;
	int qp_rem = QP%6;
    int qbits = qp_per - 1;
#if 0
	for (j=0;j<4;j++) tmp[j]=*(pSrcDst+j);
	pSrcDst[0]=(tmp[0]+tmp[1]+tmp[2]+tmp[3]);
    pSrcDst[1]=(tmp[0]-tmp[1]+tmp[2]-tmp[3]);
    pSrcDst[2]=(tmp[0]+tmp[1]-tmp[2]-tmp[3]);
    pSrcDst[3]=(tmp[0]-tmp[1]-tmp[2]+tmp[3]);
#else
    tmp[0] = pSrcDst[0];
    tmp[1] = pSrcDst[1];
    tmp[2] = pSrcDst[2];
    tmp[3] = pSrcDst[3];
    pSrcDst[0] = tmp[0] + tmp[2] + tmp[1] + tmp[3];
    pSrcDst[1] = tmp[0] + tmp[2] - tmp[1] - tmp[3];
    pSrcDst[2] = tmp[0] - tmp[2] + tmp[1] - tmp[3];
    pSrcDst[3] = tmp[0] - tmp[2] - tmp[1] + tmp[3];
#endif

	// quantization
/*
	for (i=0;i<2;i++)
  	  for (j=0;j<2;j++)
//	   *(pSrcDst+i*2+j) = (Fw16s)((*(pSrcDst+i*2+j)) * dequant_coef[qp_rem][i][j] << qp_per) >>1;
	   *(pSrcDst+i*2+j) = (Fw16s)((*(pSrcDst+i*2+j)) * dequant_coef[qp_rem][0][0] << qp_per) >>1;
*/
    if (qbits >= 0 )
    {
        int dmf = dequant_coef[qp_rem][0][0] << qbits;

        pSrcDst[0] = pSrcDst[0] * (Fw16s)dmf;
        pSrcDst[1] = pSrcDst[1] * (Fw16s)dmf;
        pSrcDst[2] = pSrcDst[2] * (Fw16s)dmf;
        pSrcDst[3] = pSrcDst[3] * (Fw16s)dmf;
    }
    else
    {
        int dmf = dequant_coef[qp_rem][0][0];

        pSrcDst[0] = (pSrcDst[0] * (Fw16s)dmf) >> 1;
        pSrcDst[1] = (pSrcDst[1] * (Fw16s)dmf) >> 1;
        pSrcDst[2] = (pSrcDst[2] * (Fw16s)dmf) >> 1;
        pSrcDst[3] = (pSrcDst[3] * (Fw16s)dmf) >> 1;
    }


	return fwStsNoErr;
}

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidual_H264_16s_C1I)(Fw16s* pSrcDst, 
													Fw32s  step, 
													Fw16s  *pDC, 
													Fw32s  AC, 
													Fw32s  QP)
{
	int step_SHORT = step/2;
	int qp_per = (QP)/6;
	int qp_rem = (QP)%6;
//	int q_bits = Q_BITS+qp_per;
	int i, j, ii;
    int qbits = qp_per-2;

	if(*pDC)
	{
		pSrcDst[0] = *pDC;
	}
	else
	{
	    qbits=qp_per-1;
		if(pSrcDst[0] != 0)
		{
			if (qbits >= 0 )
			{
				pSrcDst[0] = pSrcDst[0]*(Fw16s)(dequant_coef[qp_rem][0][0]<<qbits);
			}
			else
			{
				pSrcDst[0] = (Fw16s)(pSrcDst[0] * dequant_coef[qp_rem][0][0]) >> 1;
			}
		}
	}

    qbits = qp_per;
	if(AC) {
//		int f = (1 << q_bits) / (is_intra ? 3 : 6);
		if (qbits >= 0)
		{
			for(ii = 1 ; ii < 16 ; ii ++)
			{
				i = ii/4; j = ii%4;
				pSrcDst[ii] = pSrcDst[ii] * (Fw16s)(dequant_coef[qp_rem][i][j]<<qbits );
			}
		}
		else
		{
			int t2  = 1<<(1 + qbits);
			for(ii = 1 ; ii < 16 ; ii ++)
			{
				i = ii/4;	j = ii%4;
				pSrcDst[ii] = pSrcDst[ii] * (Fw16s)((dequant_coef[qp_rem][i][j] + t2)>>(-qbits));
			}
		}
	}

	Fw16s s[4];

	for (i = 0; i < 4; i ++)
	{
		s[0] = pSrcDst[i*step_SHORT + 0] + pSrcDst[i*step_SHORT + 2];
		s[1] = pSrcDst[i*step_SHORT + 0] - pSrcDst[i*step_SHORT + 2];
		s[2] =(pSrcDst[i*step_SHORT + 1] >> 1) - pSrcDst[i*step_SHORT + 3];
		s[3] = pSrcDst[i*step_SHORT + 1] + (pSrcDst[i*step_SHORT + 3] >> 1);

		pSrcDst[i*step_SHORT + 0] = s[0] + s[3];
		pSrcDst[i*step_SHORT + 3] = s[0] - s[3];
		pSrcDst[i*step_SHORT + 1] = s[1] + s[2];
		pSrcDst[i*step_SHORT + 2] = s[1] - s[2];
	}

	for (i = 0; i < 4; i ++)
	{
		s[0] = pSrcDst[0*step_SHORT + i] + pSrcDst[2*step_SHORT + i];
		s[1] = pSrcDst[0*step_SHORT + i] - pSrcDst[2*step_SHORT + i];
		s[2] =(pSrcDst[1*step_SHORT + i] >> 1) - pSrcDst[3*step_SHORT + i];
		s[3] = pSrcDst[1*step_SHORT + i] + (pSrcDst[3*step_SHORT + i] >> 1);

		pSrcDst[0*step_SHORT + i] = (s[0] + s[3] + 32) >> 6;
		pSrcDst[3*step_SHORT + i] = (s[0] - s[3] + 32) >> 6;
		pSrcDst[1*step_SHORT + i] = (s[1] + s[2] + 32) >> 6;
		pSrcDst[2*step_SHORT + i] = (s[1] - s[2] + 32) >> 6;
	}

	return fwStsNoErr;
}
#else

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidual_H264_16s_C1I)(Fw16s* pSrcDst, 
													Fw32s  step, 
													Fw16s  *pDC, 
													Fw32s  AC, 
													Fw32s  QP)
{
	int step_SHORT = step/2;
	int qp_per = (QP)/6;
	int qp_rem = (QP)%6;
//	int q_bits = Q_BITS+qp_per;
	int i, j, ii;
    int qbits = qp_per-2;

	if(*pDC)
	{
		pSrcDst[0] = *pDC;
	}
	else
	{
//	    qbits=qp_per-1;
	    qbits=qp_per;
		if(pSrcDst[0] != 0)
		{
			if (qbits >= 0 )
			{
//				pSrcDst[0] *= (dequant_coef[qp_rem][0][0]<<qbits);
				pSrcDst[0] = pSrcDst[0] * (Fw16s)(dequant_coef[qp_rem][0][0]<<qbits);
			}
			else
			{
				pSrcDst[0] = (Fw16s)((pSrcDst[0] * (Fw16s)dequant_coef[qp_rem][0][0]) >> (-qbits));
			}
		}
	}
#if 1
    qbits = qp_per;
	if(AC) {
//		int f = (1 << q_bits) / (is_intra ? 3 : 6);
		if (qbits >= 0)
		{
			for(ii = 1 ; ii < 16 ; ii ++)
			{
				i = ii/4; j = ii%4;
				pSrcDst[i*step_SHORT+j] = pSrcDst[i*step_SHORT+j] * (Fw16s)(dequant_coef[qp_rem][i][j]<<qbits);
			}
		}
		else
		{
			int t2  = 1<<(1 + qbits);
			for(ii = 1 ; ii < 16 ; ii ++)
			{
				i = ii/4;	j = ii%4;
				pSrcDst[i*step_SHORT+j] = (Fw16s)((pSrcDst[i*step_SHORT+j]*dequant_coef[qp_rem][i][j] + t2)>>(-qbits));
			}
		}
	}
	Fw16s s[4];
	for (i = 0; i < 4; i ++)
	{
		s[0] = pSrcDst[i*step_SHORT + 0] + pSrcDst[i*step_SHORT + 2];
		s[1] = pSrcDst[i*step_SHORT + 0] - pSrcDst[i*step_SHORT + 2];
		s[2] =(pSrcDst[i*step_SHORT + 1] >> 1) - pSrcDst[i*step_SHORT + 3];
		s[3] = pSrcDst[i*step_SHORT + 1] + (pSrcDst[i*step_SHORT + 3] >> 1);

		pSrcDst[i*step_SHORT + 0] = s[0] + s[3];
		pSrcDst[i*step_SHORT + 3] = s[0] - s[3];
		pSrcDst[i*step_SHORT + 1] = s[1] + s[2];
		pSrcDst[i*step_SHORT + 2] = s[1] - s[2];
	}

	for (i = 0; i < 4; i ++)
	{
		s[0] = pSrcDst[0*step_SHORT + i] + pSrcDst[2*step_SHORT + i];
		s[1] = pSrcDst[0*step_SHORT + i] - pSrcDst[2*step_SHORT + i];
		s[2] =(pSrcDst[1*step_SHORT + i] >> 1) - pSrcDst[3*step_SHORT + i];
		s[3] = pSrcDst[1*step_SHORT + i] + (pSrcDst[3*step_SHORT + i] >> 1);

		pSrcDst[0*step_SHORT + i] = (s[0] + s[3] + 32) >> 6;
		pSrcDst[3*step_SHORT + i] = (s[0] - s[3] + 32) >> 6;
		pSrcDst[1*step_SHORT + i] = (s[1] + s[2] + 32) >> 6;
		pSrcDst[2*step_SHORT + i] = (s[1] - s[2] + 32) >> 6;
	}

#else

	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5;
#if 1
	Fw16s* tempa = (Fw16s*) fwMalloc(8*sizeof(Fw16s));
	Fw8u*  tempb = (Fw8u*) fwMalloc(16*sizeof(Fw8u));
#endif

    qbits = qp_per;
	if(AC) {
//		int f = (1 << q_bits) / (is_intra ? 3 : 6);
		if (qbits >= 0)
		{
//			for(ii = 1 ; ii < 16 ; ii ++)
//			{
//				i = ii/4; j = ii%4;
//				pSrcDst[i*step_SHORT+j] = pSrcDst[i*step_SHORT+j] * (Fw16s)(dequant_coef[qp_rem][i][j]<<qbits);
//			}
			{
			//  int qp_per = QP/6;
			//  int qp_rem = QP%6;
				qp_rem *= 16;
				xmm1 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem) );
				xmm3 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem+8) );

				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcDst );
				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcDst+step_SHORT) );
				xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);
				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcDst+step_SHORT*2) );
				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcDst+step_SHORT*3) );
				xmm2 = _mm_unpacklo_epi64(xmm2, xmm5);

		//		pmullw  xmm0, xmm1
				xmm0 = _mm_mullo_epi16(xmm0, xmm1);
		//		pmullw  xmm4, xmm3
				xmm2 = _mm_mullo_epi16(xmm2, xmm3);

				xmm5  = _mm_set1_epi16(1);    // round;
		//		movd    xmm7, qp_per
		//		psllw   xmm6, xmm7      ; << qbits
                __m128i qp_per_x = _mm_cvtsi32_si128(qp_per);
				xmm5  = _mm_sll_epi16 (xmm5, qp_per_x);

		//		pmullw xmm0, xmm6
				xmm0 = _mm_mullo_epi16(xmm0, xmm5);
		//		pmullw xmm4, xmm6
				xmm2 = _mm_mullo_epi16(xmm2, xmm5);
			}
		}
		else
		{
			int t2  = 1<<(1 + qbits);
//			for(ii = 1 ; ii < 16 ; ii ++)
//			{
//				i = ii/4;	j = ii%4;
//				pSrcDst[i*step_SHORT+j] = (Fw16s)((pSrcDst[i*step_SHORT+j]*dequant_coef[qp_rem][i][j] + t2)>>(-qbits));
//			}
			{
			//  int qp_per = QP/6;
			//  int qp_rem = QP%6;
				qp_rem *= 16;
				xmm1 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem) );
				xmm3 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem+8) );

				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcDst );
				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcDst+step_SHORT) );
				xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);
				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcDst+step_SHORT*2) );
				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcDst+step_SHORT*3) );
				xmm2 = _mm_unpacklo_epi64(xmm2, xmm5);

				xmm4  = _mm_set1_epi16((short)t2);    // t2;

		//		pmullw  xmm0, xmm1
				xmm0 = _mm_mullo_epi16(xmm0, xmm1);
		//		pmullw  xmm4, xmm3
				xmm2 = _mm_mullo_epi16(xmm2, xmm3);

				xmm0 =	_mm_add_epi16(xmm0, xmm4);	// +t2
				xmm2 =	_mm_add_epi16(xmm2, xmm4);	// +t2

				xmm5  = _mm_set1_epi16(1);    // round;
		//		movd    xmm7, qp_per
		//		psllw   xmm6, xmm7      ; << qbits
                __m128i qp_per_x_neg = _mm_cvtsi32_si128(-qp_per);
				xmm5  = _mm_srl_epi16 (xmm5, qp_per_x_neg); //>>(-qbits));

		//		pmullw xmm0, xmm6
				xmm0 = _mm_mullo_epi16(xmm0, xmm5);
		//		pmullw xmm4, xmm6
				xmm2 = _mm_mullo_epi16(xmm2, xmm5);
			}
		}

		xmm0 = _mm_insert_epi16 (xmm0, pSrcDst[0], 0); // DC
	}
	else
	{
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcDst );
		xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcDst+step_SHORT) );
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);
		xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcDst+step_SHORT*2) );
		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcDst+step_SHORT*3) );
		xmm2 = _mm_unpacklo_epi64(xmm2, xmm5);
	}

#if 1
{
_mm_storeu_si128( (__m128i*)tempa, xmm0 ); // store 16 bytes
printf("\n %3d %3d %3d %3d",tempa[0], tempa[1],tempa[2],tempa[3]);
printf("\n %3d %3d %3d %3d",tempa[4],tempa[5],tempa[6],tempa[7]);
_mm_storeu_si128( (__m128i*)tempa, xmm2 ); // store 16 bytes
printf("\n %3d %3d %3d %3d",tempa[0], tempa[1],tempa[2],tempa[3]);
printf("\n %3d %3d %3d %3d",tempa[4],tempa[5],tempa[6],tempa[7]);
}
#endif

	//  Horizontal transform
		xmm4 = xmm0;
		xmm5 = xmm2;
		xmm1 = xmm0;
		xmm3 = xmm2;

		xmm4 = _mm_srai_epi16(xmm4, 1);		// xmm4 = ([r1, r0]>>1)
		xmm5 = _mm_srai_epi16(xmm5, 1);		// xmm5 = ([r3, r2]>>1)

		xmm4 = _mm_srli_si128(xmm4, 8);		// xmm4 = ([r1]>>1)
		xmm5 = _mm_srli_si128(xmm5, 8);		// xmm5 = ([r3]>>1)

		xmm1 = _mm_unpacklo_epi64(xmm1, xmm4);	// xmm1 = ([r1]>>1), r0]	
		xmm3 = _mm_unpacklo_epi64(xmm3, xmm5);	// xmm3 = ([r3]>>1), r2]	

		//	pshufd	xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
//		xmm4 = xmm0;
		//	paddw		xmm0, xmm2			// xmm0 = [r1+r3>>1|r0+r2]
		xmm3 =	_mm_add_epi16(xmm3, xmm0);			// xmm3 = [r1+r3>>1|r0+r2] = [s3 s0]
		//	psubw		xmm4, xmm2			// xmm4 = [r1>>1-r3|r0-r2] 
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);			// xmm1 = [r1>>1-r3|r0-r2] = [s2 s1]
		xmm0 = xmm3;
		xmm4 = xmm1;

		// pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4			// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		// punpckhqdq	xmm3, xmm4			// xmm3 = [r1>>1-r3|r1+r3>>1]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		// pshufd		xmm2, xmm0, 0xe4
//		xmm2 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm2 = xmm0;

		// paddw		xmm0, xmm3			// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3			// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			
#if 0
{
_mm_storeu_si128( (__m128i*)tempa, xmm0 ); // store 16 bytes
printf("\n %3d %3d %3d %3d",tempa[0], tempa[1],tempa[2],tempa[3]);
printf("\n %3d %3d %3d %3d",tempa[4],tempa[5],tempa[6],tempa[7]);
_mm_storeu_si128( (__m128i*)tempa, xmm2 ); // store 16 bytes
printf("\n %3d %3d %3d %3d",tempa[0], tempa[1],tempa[2],tempa[3]);
printf("\n %3d %3d %3d %3d",tempa[4],tempa[5],tempa[6],tempa[7]);
}
#endif

	// Transpose
		//	pshufd	  xmm5, xmm0, 0xe4		// xmm5 = xmm0
//		xmm5 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm5 = xmm0;
		//	pshufd	  xmm2, xmm2, 0x4e		// xmm2 = [y3 |y2]
		xmm2 = _mm_shuffle_epi32(xmm2, 0x4e);
		//  punpcklwd xmm0, xmm2			// xmm0 = [y23|y03|y22|y02|y21|y01|y20|y00]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);
		//	punpckhwd xmm5, xmm2			// xmm5 = [y33|y13|y32|y12|y31|y11|y30|y10]
		xmm5 = _mm_unpackhi_epi16(xmm5, xmm2);
		//	movdqa	  xmm1, xmm0
		xmm1 = xmm0;
		//	punpcklwd xmm0, xmm5		// xmm0 = [y31|y21|y11|y01|y30|y20|y10|y00]=[y1|y0]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm5);
		//	punpckhwd xmm1, xmm5		// xmm1 = [y33|y23|y13|y03|y32|y22|y12|y02]=[y3|y2]
		xmm1 = _mm_unpackhi_epi16(xmm1, xmm5);

	// vertical
		xmm2 = xmm1;

		xmm4 = xmm0;
		xmm5 = xmm2;
		xmm1 = xmm0;
		xmm3 = xmm2;
		xmm4 = _mm_srai_epi16(xmm4, 1);		// xmm4 = ([r1, r0]>>1)
		xmm5 = _mm_srai_epi16(xmm5, 1);		// xmm5 = ([r3, r2]>>1)
		xmm4 = _mm_srli_si128(xmm4, 8);		// xmm4 = ([r1]>>1)
		xmm5 = _mm_srli_si128(xmm5, 8);		// xmm5 = ([r3]>>1)
		xmm1 = _mm_unpacklo_epi64(xmm1, xmm4);	// xmm1 = ([r1]>>1), r0]	
		xmm3 = _mm_unpacklo_epi64(xmm3, xmm5);	// xmm3 = ([r3]>>1), r2]	

		//	pshufd	xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
//		xmm4 = xmm0;
		//	paddw		xmm0, xmm2			// xmm0 = [r1+r3>>1|r0+r2]
		xmm3 =	_mm_add_epi16(xmm3, xmm0);			// xmm3 = [r1+r3>>1|r0+r2] = [s3 s0]
		//	psubw		xmm4, xmm2			// xmm4 = [r1>>1-r3|r0-r2] 
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);			// xmm1 = [r1>>1-r3|r0-r2] = [s2 s1]
		xmm0 = xmm3;
		xmm4 = xmm1;

		//	pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4		// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		//	punpckhqdq	xmm3, xmm4		// xmm3 = [r1-r3|r1+r3]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		//	movdqa		xmm2, xmm0
		xmm2 = xmm0;
		//	paddw		xmm0, xmm3		// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3		// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			

		xmm4 = _mm_shuffle_epi32(xmm2, 0x4e);	// xmm4 = [y3 |y2]

		xmm5  = _mm_set1_epi16((short)32);    // round;

		xmm0 =	_mm_add_epi16(xmm0, xmm5);			
		xmm4 =	_mm_add_epi16(xmm4, xmm5);			

		xmm0 = _mm_srai_epi16(xmm0, 6);		// xmm4 = ([r1, r0]>>1)
		xmm4 = _mm_srai_epi16(xmm4, 6);		// xmm4 = ([r1, r0]>>1)

		_mm_store_si128( (__m128i*)pSrcDst, xmm0 ); // store 16 bytes
		_mm_store_si128( (__m128i*)(pSrcDst+8), xmm4 ); // store 16 bytes

#if 1
	fwFree(tempa);
	fwFree(tempb);
#endif
#endif
	return fwStsNoErr;
}
#endif

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidualAndAdd_H264_16s_C1I)(const  Fw8u  *pPred,
																 Fw16s *pSrcDst, 
																 Fw16s *pDC, 
																 Fw8u  *pDst, 
																 Fw32s  PredStep, 
																 Fw32s  DstStep,
																 Fw32s  QP, 
																 Fw32s  AC)
{
	int qp_per = (QP)/6;
	int qp_rem = (QP)%6;
	int q_bits    = Q_BITS+qp_per;
//	int i, j;
	int PredStep_SHORT = PredStep/2;
	int DstStep_SHORT = DstStep/2;

	if(pDC)
	{
		pSrcDst[0] = *pDC;
	}
	else
	{
		int f = (1 << q_bits) / 6;

		if (pSrcDst[0] > 0)
			pSrcDst[0] = (Fw16s)((pSrcDst[0] * quant_coef[qp_rem][0][0] + f) >> q_bits);
		else
			pSrcDst[0] = (Fw16s)-((-(pSrcDst[0] * quant_coef[qp_rem][0][0]) + f) >> q_bits);
	}
	
#if 0
	if(AC) {
//		int f = (1 << q_bits) / (is_intra ? 3 : 6);
		int f = (1 << q_bits) / 6;
		
		for(int ii=1;ii<16;ii++)
		{
			i = ii/4;
			j = ii%4;
			if (pSrcDst[i*4+j] > 0)
				pSrcDst[i*4+j]=(Fw16s)((pSrcDst[i*4+j]*quant_coef[qp_rem][i][j]+f) >> q_bits);
			else
				pSrcDst[i*4+j]=(Fw16s)-((-(pSrcDst[i*4+j]*quant_coef[qp_rem][i][j])+f) >> q_bits);
		}
	}

	Fw16s a[4];

   // horizontal
	for (i=0;i<4;i++)
	{	
		a[0] = pSrcDst[i*4+0] + pSrcDst[i*4+2];
		a[1] = pSrcDst[i*4+0] - pSrcDst[i*4+2];
		a[2] = (pSrcDst[i*4+1]>>1) - pSrcDst[i*4+3];
		a[3] = pSrcDst[i*4+1]+(pSrcDst[i*4+3]>>1);

		pSrcDst[i*4+0] = a[0] + a[3];
		pSrcDst[i*4+1] = a[1] + a[2];
		pSrcDst[i*4+2] = a[1] - a[2];
		pSrcDst[i*4+3] = a[0] - a[3];
	}

	// vertical
	for (i=0;i<4;i++)
	{	
		a[0] = pSrcDst[0*4+i] + pSrcDst[2*4+i];
		a[1] = pSrcDst[0*4+i] - pSrcDst[2*4+i];
		a[2] = (pSrcDst[1*4+i]>>1) - pSrcDst[3*4+i];
		a[3] = pSrcDst[1*4+i]+(pSrcDst[3*4+i]>>1);

		pSrcDst[0*4+i] = MAX(0,MIN(255,(a[0] + a[3] + 32) >> 6));
		pSrcDst[1*4+i] = MAX(0,MIN(255,(a[1] + a[2] + 32) >> 6));
		pSrcDst[2*4+i] = MAX(0,MIN(255,(a[1] - a[2] + 32) >> 6));
		pSrcDst[3*4+i] = MAX(0,MIN(255,(a[0] - a[3] + 32) >> 6));		

		pDst[0*DstStep_SHORT+i] = (Fw8u)MAX(0,MIN(255,(pSrcDst[0*4+i]+pPred[0*PredStep_SHORT+i])));
		pDst[1*DstStep_SHORT+i] = (Fw8u)MAX(0,MIN(255,(pSrcDst[1*4+i]+pPred[1*PredStep_SHORT+i])));
		pDst[2*DstStep_SHORT+i] = (Fw8u)MAX(0,MIN(255,(pSrcDst[2*4+i]+pPred[2*PredStep_SHORT+i])));
		pDst[3*DstStep_SHORT+i] = (Fw8u)MAX(0,MIN(255,(pSrcDst[3*4+i]+pPred[3*PredStep_SHORT+i])));

	}		
#else
	int *a;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5;
#if 1
	Fw16s* tempa = (Fw16s*) fwMalloc(8*sizeof(Fw16s));
	Fw8u*  tempb = (Fw8u*) fwMalloc(16*sizeof(Fw8u));
#endif

    q_bits = qp_per;
	if(AC) {
//		int f = (1 << q_bits) / (is_intra ? 3 : 6);
		if (q_bits >= 0)
		{
//			for(ii = 1 ; ii < 16 ; ii ++)
//			{
//				i = ii/4; j = ii%4;
//				pSrcDst[i*step_SHORT+j] = pSrcDst[i*step_SHORT+j] * (Fw16s)(dequant_coef[qp_rem][i][j]<<qbits);
//			}
			{
			//  int qp_per = QP/6;
			//  int qp_rem = QP%6;
				qp_rem *= 16;
				xmm1 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem) );
				xmm3 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem+8) );

				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcDst );
				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4) );
				xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);
				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*2) );
				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*3) );
				xmm2 = _mm_unpacklo_epi64(xmm2, xmm5);

		//		pmullw  xmm0, xmm1
				xmm0 = _mm_mullo_epi16(xmm0, xmm1);
		//		pmullw  xmm4, xmm3
				xmm2 = _mm_mullo_epi16(xmm2, xmm3);

				xmm5  = _mm_set1_epi16(1);    // round;
		//		movd    xmm7, qp_per
		//		psllw   xmm6, xmm7      ; << qbits
                __m128i qp_per_x = _mm_cvtsi32_si128(qp_per);
				xmm5  = _mm_sll_epi16 (xmm5, qp_per_x);

		//		pmullw xmm0, xmm6
				xmm0 = _mm_mullo_epi16(xmm0, xmm5);
		//		pmullw xmm4, xmm6
				xmm2 = _mm_mullo_epi16(xmm2, xmm5);
			}
		}
		else
		{
			int t2  = 1<<(1 + q_bits);
//			for(ii = 1 ; ii < 16 ; ii ++)
//			{
//				i = ii/4;	j = ii%4;
//				pSrcDst[i*step_SHORT+j] = (Fw16s)((pSrcDst[i*step_SHORT+j]*dequant_coef[qp_rem][i][j] + t2)>>(-qbits));
//			}
			{
			//  int qp_per = QP/6;
			//  int qp_rem = QP%6;
				qp_rem *= 16;
				xmm1 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem) );
				xmm3 = _mm_load_si128( (__m128i*)(dequant_coef16+qp_rem+8) );

				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcDst );
				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4) );
				xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);
				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*2) );
				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*3) );
				xmm2 = _mm_unpacklo_epi64(xmm2, xmm5);

				xmm4  = _mm_set1_epi16((short)t2);    // t2;

		//		pmullw  xmm0, xmm1
				xmm0 = _mm_mullo_epi16(xmm0, xmm1);
		//		pmullw  xmm4, xmm3
				xmm2 = _mm_mullo_epi16(xmm2, xmm3);

				xmm0 =	_mm_add_epi16(xmm0, xmm4);	// +t2
				xmm2 =	_mm_add_epi16(xmm2, xmm4);	// +t2

				xmm5  = _mm_set1_epi16(1);    // round;
		//		movd    xmm7, qp_per
		//		psllw   xmm6, xmm7      ; << qbits
                __m128i qp_per_x_neg = _mm_cvtsi32_si128(-qp_per);
				xmm5  = _mm_srl_epi16 (xmm5, qp_per_x_neg); //>>(-qbits));

		//		pmullw xmm0, xmm6
				xmm0 = _mm_mullo_epi16(xmm0, xmm5);
		//		pmullw xmm4, xmm6
				xmm2 = _mm_mullo_epi16(xmm2, xmm5);
			}
		}

		xmm0 = _mm_insert_epi16 (xmm0, pSrcDst[0], 0); // DC
	}
	else
	{
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcDst );
		xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4) );
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);
		xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*2) );
		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*3) );
		xmm2 = _mm_unpacklo_epi64(xmm2, xmm5);
	}


	//  Horizontal transform
		xmm4 = xmm0;
		xmm5 = xmm2;
		xmm1 = xmm0;
		xmm3 = xmm2;

		xmm4 = _mm_srai_epi16(xmm4, 1);		// xmm4 = ([r1, r0]>>1)
		xmm5 = _mm_srai_epi16(xmm5, 1);		// xmm5 = ([r3, r2]>>1)

		xmm4 = _mm_srli_si128(xmm4, 8);		// xmm4 = ([r1]>>1)
		xmm5 = _mm_srli_si128(xmm5, 8);		// xmm5 = ([r3]>>1)

		xmm1 = _mm_unpacklo_epi64(xmm1, xmm4);	// xmm1 = ([r1]>>1), r0]	
		xmm3 = _mm_unpacklo_epi64(xmm3, xmm5);	// xmm3 = ([r3]>>1), r2]	

		//	pshufd	xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
//		xmm4 = xmm0;
		//	paddw		xmm0, xmm2			// xmm0 = [r1+r3>>1|r0+r2]
		xmm3 =	_mm_add_epi16(xmm3, xmm0);			// xmm3 = [r1+r3>>1|r0+r2] = [s3 s0]
		//	psubw		xmm4, xmm2			// xmm4 = [r1>>1-r3|r0-r2] 
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);			// xmm1 = [r1>>1-r3|r0-r2] = [s2 s1]
		xmm0 = xmm3;
		xmm4 = xmm1;

		// pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4			// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		// punpckhqdq	xmm3, xmm4			// xmm3 = [r1>>1-r3|r1+r3>>1]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		// pshufd		xmm2, xmm0, 0xe4
//		xmm2 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm2 = xmm0;

		// paddw		xmm0, xmm3			// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3			// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			
#if 0
{
_mm_storeu_si128( (__m128i*)tempa, xmm0 ); // store 16 bytes
printf("\n %3d %3d %3d %3d",tempa[0], tempa[1],tempa[2],tempa[3]);
printf("\n %3d %3d %3d %3d",tempa[4],tempa[5],tempa[6],tempa[7]);
_mm_storeu_si128( (__m128i*)tempa, xmm2 ); // store 16 bytes
printf("\n %3d %3d %3d %3d",tempa[0], tempa[1],tempa[2],tempa[3]);
printf("\n %3d %3d %3d %3d",tempa[4],tempa[5],tempa[6],tempa[7]);
}
#endif

	// Transpose
		//	pshufd	  xmm5, xmm0, 0xe4		// xmm5 = xmm0
//		xmm5 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm5 = xmm0;
		//	pshufd	  xmm2, xmm2, 0x4e		// xmm2 = [y3 |y2]
		xmm2 = _mm_shuffle_epi32(xmm2, 0x4e);
		//  punpcklwd xmm0, xmm2			// xmm0 = [y23|y03|y22|y02|y21|y01|y20|y00]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);
		//	punpckhwd xmm5, xmm2			// xmm5 = [y33|y13|y32|y12|y31|y11|y30|y10]
		xmm5 = _mm_unpackhi_epi16(xmm5, xmm2);
		//	movdqa	  xmm1, xmm0
		xmm1 = xmm0;
		//	punpcklwd xmm0, xmm5		// xmm0 = [y31|y21|y11|y01|y30|y20|y10|y00]=[y1|y0]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm5);
		//	punpckhwd xmm1, xmm5		// xmm1 = [y33|y23|y13|y03|y32|y22|y12|y02]=[y3|y2]
		xmm1 = _mm_unpackhi_epi16(xmm1, xmm5);

	// vertical
		xmm2 = xmm1;

		xmm4 = xmm0;
		xmm5 = xmm2;
		xmm1 = xmm0;
		xmm3 = xmm2;
		xmm4 = _mm_srai_epi16(xmm4, 1);		// xmm4 = ([r1, r0]>>1)
		xmm5 = _mm_srai_epi16(xmm5, 1);		// xmm5 = ([r3, r2]>>1)
		xmm4 = _mm_srli_si128(xmm4, 8);		// xmm4 = ([r1]>>1)
		xmm5 = _mm_srli_si128(xmm5, 8);		// xmm5 = ([r3]>>1)
		xmm1 = _mm_unpacklo_epi64(xmm1, xmm4);	// xmm1 = ([r1]>>1), r0]	
		xmm3 = _mm_unpacklo_epi64(xmm3, xmm5);	// xmm3 = ([r3]>>1), r2]	

		//	pshufd	xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
//		xmm4 = xmm0;
		//	paddw		xmm0, xmm2			// xmm0 = [r1+r3>>1|r0+r2]
		xmm3 =	_mm_add_epi16(xmm3, xmm0);			// xmm3 = [r1+r3>>1|r0+r2] = [s3 s0]
		//	psubw		xmm4, xmm2			// xmm4 = [r1>>1-r3|r0-r2] 
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);			// xmm1 = [r1>>1-r3|r0-r2] = [s2 s1]
		xmm0 = xmm3;
		xmm4 = xmm1;

		//	pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4		// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		//	punpckhqdq	xmm3, xmm4		// xmm3 = [r1-r3|r1+r3]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		//	movdqa		xmm2, xmm0
		xmm2 = xmm0;
		//	paddw		xmm0, xmm3		// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3		// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			

		xmm4 = _mm_shuffle_epi32(xmm2, 0x4e);	// xmm4 = [y3 |y2]

		xmm5  = _mm_set1_epi16((short)32);    // round;

		xmm0 =	_mm_add_epi16(xmm0, xmm5);			
		xmm4 =	_mm_add_epi16(xmm4, xmm5);			

		xmm0 = _mm_srai_epi16(xmm0, 6);		// xmm4 = ([r1, r0]>>1)
		xmm4 = _mm_srai_epi16(xmm4, 6);		// xmm4 = ([r1, r0]>>1)

		_mm_store_si128( (__m128i*)pSrcDst, xmm0 ); // store 16 bytes
		_mm_store_si128( (__m128i*)(pSrcDst+8), xmm4 ); // store 16 bytes

/////////////////****************************************************8
//		pDst[0*DstStep_SHORT+i] = (Fw8u)MAX(0,MIN(255,(pSrcDst[0*4+i]+pPred[0*PredStep_SHORT+i])));
//		pDst[1*DstStep_SHORT+i] = (Fw8u)MAX(0,MIN(255,(pSrcDst[1*4+i]+pPred[1*PredStep_SHORT+i])));
//		pDst[2*DstStep_SHORT+i] = (Fw8u)MAX(0,MIN(255,(pSrcDst[2*4+i]+pPred[2*PredStep_SHORT+i])));
//		pDst[3*DstStep_SHORT+i] = (Fw8u)MAX(0,MIN(255,(pSrcDst[3*4+i]+pPred[3*PredStep_SHORT+i])));
		xmm1 = _mm_load_si128( (__m128i*)(pPred) );
		xmm2 = _mm_load_si128( (__m128i*)(pPred+2*PredStep_SHORT) );

		xmm0 =	_mm_add_epi16(xmm0, xmm1);			
		xmm4 =	_mm_add_epi16(xmm4, xmm2);			

		xmm0 = _mm_packus_epi16 (xmm0, xmm4);	  // 16bit to 8bit
//		_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
		a    = (int *)pDst;
		*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
		xmm0 = _mm_srli_si128(xmm0, 2);
		a    += DstStep_SHORT;
		*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
		xmm0 = _mm_srli_si128(xmm0, 2);
		a    += DstStep_SHORT;
		*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
		xmm0 = _mm_srli_si128(xmm0, 2);
		a    += DstStep_SHORT;
		*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src

#if 1
	fwFree(tempa);
	fwFree(tempb);
#endif

#endif

	return fwStsNoErr;
}
#endif

#if 0

FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformPrediction_H264_8u16s_C1)(Fw8u *pSrc, Fw32s step, Fw16s *pDst)
{
//	Fw8u  *pRec;
//	Fw32s i, j;
	Fw32s i;
	Fw16s s[4];
#if 0
//	Fw16s* prow[4];
  	prow[0] = pDst;
	prow[1] = pDst + 4;
	prow[2] = pDst + 8;
	prow[3] = pDst + 12;
	pRec = pSrc;	
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			prow[i][j] = pRec[j];
		}
		pRec += step;
	}	
	//horizontal transform for one block
	for (i=0; i < 4; i++)
	{
		a[0]=prow[i][0]+prow[i][3];
		a[1]=prow[i][1]+prow[i][2];
		a[2]=prow[i][1]-prow[i][2];
		a[3]=prow[i][0]-prow[i][3];
						
		prow[i][0]= (a[0]  + a[1]);
		prow[i][1]= a[3]*2 + a[2];
		prow[i][2]= (a[0]  - a[1]);
		prow[i][3]= a[3]   - a[2]*2;
	}
	//vertical transform for one block
	for(j = 0; j < 4; j++)
	{
		a[0] = prow[0][j] + prow[3][j];
		a[1] = prow[1][j] + prow[2][j];
		a[2] = prow[1][j] - prow[2][j];
		a[3] = prow[0][j] - prow[3][j];	
		
		prow[0][j]=(a[0]   + a[1]);
		prow[1][j]= a[3]*2 + a[2];
		prow[2][j]=(a[0]   - a[1]);		
		prow[3][j]= a[3]   - a[2]*2;
	}
#else
    for (i = 0; i < 4; i ++)
    {
        s[0] = pSrc[i * step + 0] + pSrc[i * step + 2];
        s[1] = pSrc[i * step + 0] - pSrc[i * step + 2];
        s[2] = (pSrc[i * step + 1] >> 1) - pSrc[i * step + 3];
        s[3] = pSrc[i * step + 1] + (pSrc[i * step + 3] >> 1);

        pDst[i * 4 + 0] = s[0] + s[3];
        pDst[i * 4 + 3] = s[0] - s[3];
        pDst[i * 4 + 1] = s[1] + s[2];
        pDst[i * 4 + 2] = s[1] - s[2];
    }

    for (i = 0; i < 4; i ++)
    {
        s[0] = pDst[0 * 4 + i] + pDst[2 * 4 + i];
        s[1] = pDst[0 * 4 + i] - pDst[2 * 4 + i];
        s[2] = (pDst[1 * 4 + i] >> 1) - pDst[3 * 4 + i];
        s[3] = pDst[1 * 4 + i] + (pDst[3 * 4 + i] >> 1);

        pDst[0 * 4 + i] = (s[0] + s[3] + 32) >> 6;
        pDst[3 * 4 + i] = (s[0] - s[3] + 32) >> 6;
        pDst[1 * 4 + i] = (s[1] + s[2] + 32) >> 6;
        pDst[2 * 4 + i] = (s[1] - s[2] + 32) >> 6;
    }
#endif
	return fwStsNoErr;
}
#endif

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidual_SISP_H264_16s_C1I)(	Fw16s* pSrcDst,
															Fw16s* pPredictBlock, 
															Fw16s* pDC, 
															Fw32s  AC, 
															Fw32s  qp, 
															Fw32s  qs, Fw32s Switch)
{
//	Fw16s* prow[4];
//	Fw16s  s[4];
	Fw32s  i, ilev;

	Fw32s qp1_sp = qs/6;
	Fw32s qp2_sp = qs%6;
	Fw32s bits_sp = 15 + qp1_sp;
	Fw32s bits2_sp = (1<<bits_sp)/2;  //sp_pred

	if(Switch)
	{
		qp = qs;
	}
	
	if(pDC)
	{
		pSrcDst[0] = *pDC;
	}
	else
	{
		ilev=((pSrcDst[0]*(InvQuantTable1[qp][QuantIndex1[0]])*MATR[0]) >>6)
							+ pPredictBlock[0] ;
		ilev > 0 ?
		pSrcDst[0] = (Fw16s)(((abs(ilev)* QuantCoef[qp2_sp][0]+ bits2_sp)
			>> bits_sp)*(InvQuantTable1[qs][QuantIndex1[0]])) :
		
		pSrcDst[0] = (Fw16s)(-((abs(ilev)* QuantCoef[qp2_sp][0]+ bits2_sp)
			>> bits_sp)*(InvQuantTable1[qs][QuantIndex1[0]]));
	}
	
	if(AC)
	{
		for (i=1;i<16;i++)
		{
			ilev=((pSrcDst[i]*(InvQuantTable1[qp][QuantIndex1[i]])*MATR[i]) >>6)
								+ pPredictBlock[i] ;
			ilev > 0 ?
			pSrcDst[i] = (Fw16s)(((abs(ilev)* QuantCoef[qp2_sp][i]+ bits2_sp)
				>> bits_sp)*(InvQuantTable1[qs][QuantIndex1[i]])) :
			
			pSrcDst[i] = (Fw16s)(-((abs(ilev)* QuantCoef[qp2_sp][i]+ bits2_sp)
				>> bits_sp)*(InvQuantTable1[qs][QuantIndex1[i]]));

		}
	}
	else
	{
		for (i=1;i<16;i++)
		{
			ilev = pPredictBlock[i];
			ilev > 0 ?
			pSrcDst[i] = (Fw16s)(((abs(ilev)* QuantCoef[qp2_sp][i]+ bits2_sp)
				>> bits_sp)*(InvQuantTable1[qs][QuantIndex1[i]])) :
			
			pSrcDst[i] = (Fw16s)(-((abs(ilev)* QuantCoef[qp2_sp][i]+ bits2_sp)
				>> bits_sp)*(InvQuantTable1[qs][QuantIndex1[i]]));

		}
	}
#if 1
	Fw16s  s[4];

    for (i = 0; i < 4; i ++)
    {
        s[0] = pSrcDst[i * 4 + 0] + pSrcDst[i * 4 + 2];
        s[1] = pSrcDst[i * 4 + 0] - pSrcDst[i * 4 + 2];
        s[2] = (pSrcDst[i * 4 + 1] >> 1) - pSrcDst[i * 4 + 3];
        s[3] = pSrcDst[i * 4 + 1] + (pSrcDst[i * 4 + 3] >> 1);

        pSrcDst[i * 4 + 0] = s[0] + s[3];
        pSrcDst[i * 4 + 3] = s[0] - s[3];
        pSrcDst[i * 4 + 1] = s[1] + s[2];
        pSrcDst[i * 4 + 2] = s[1] - s[2];
    }

    for (i = 0; i < 4; i ++)
    {
        s[0] = pSrcDst[0 * 4 + i] + pSrcDst[2 * 4 + i];
        s[1] = pSrcDst[0 * 4 + i] - pSrcDst[2 * 4 + i];
        s[2] = (pSrcDst[1 * 4 + i] >> 1) - pSrcDst[3 * 4 + i];
        s[3] = pSrcDst[1 * 4 + i] + (pSrcDst[3 * 4 + i] >> 1);

        pSrcDst[0 * 4 + i] = (s[0] + s[3] + 32) >> 6;
        pSrcDst[3 * 4 + i] = (s[0] - s[3] + 32) >> 6;
        pSrcDst[1 * 4 + i] = (s[1] + s[2] + 32) >> 6;
        pSrcDst[2 * 4 + i] = (s[1] - s[2] + 32) >> 6;
    }
#else
		// problem with 64bit compiler

		__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5;

		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcDst );
		xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4) );
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);
		xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*2) );
		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*3) );
		xmm2 = _mm_unpacklo_epi64(xmm2, xmm5);


	//  Horizontal transform
		xmm4 = xmm0;
		xmm5 = xmm2;
		xmm1 = xmm0;
		xmm3 = xmm2;

		xmm4 = _mm_srai_epi16(xmm4, 1);		// xmm4 = ([r1, r0]>>1)
		xmm5 = _mm_srai_epi16(xmm5, 1);		// xmm5 = ([r3, r2]>>1)

		xmm4 = _mm_srli_si128(xmm4, 8);		// xmm4 = ([r1]>>1)
		xmm5 = _mm_srli_si128(xmm5, 8);		// xmm5 = ([r3]>>1)

		xmm1 = _mm_unpacklo_epi64(xmm1, xmm4);	// xmm1 = ([r1]>>1), r0]	
		xmm3 = _mm_unpacklo_epi64(xmm3, xmm5);	// xmm3 = ([r3]>>1), r2]	

		//	pshufd	xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
//		xmm4 = xmm0;
		//	paddw		xmm0, xmm2			// xmm0 = [r1+r3>>1|r0+r2]
		xmm3 =	_mm_add_epi16(xmm3, xmm0);			// xmm3 = [r1+r3>>1|r0+r2] = [s3 s0]
		//	psubw		xmm4, xmm2			// xmm4 = [r1>>1-r3|r0-r2] 
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);			// xmm1 = [r1>>1-r3|r0-r2] = [s2 s1]
		xmm0 = xmm3;
		xmm4 = xmm1;

		// pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4			// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		// punpckhqdq	xmm3, xmm4			// xmm3 = [r1>>1-r3|r1+r3>>1]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		// pshufd		xmm2, xmm0, 0xe4
//		xmm2 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm2 = xmm0;

		// paddw		xmm0, xmm3			// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3			// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			

	// Transpose
		//	pshufd	  xmm5, xmm0, 0xe4		// xmm5 = xmm0
//		xmm5 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm5 = xmm0;
		//	pshufd	  xmm2, xmm2, 0x4e		// xmm2 = [y3 |y2]
		xmm2 = _mm_shuffle_epi32(xmm2, 0x4e);
		//  punpcklwd xmm0, xmm2			// xmm0 = [y23|y03|y22|y02|y21|y01|y20|y00]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);
		//	punpckhwd xmm5, xmm2			// xmm5 = [y33|y13|y32|y12|y31|y11|y30|y10]
		xmm5 = _mm_unpackhi_epi16(xmm5, xmm2);
		//	movdqa	  xmm1, xmm0
		xmm1 = xmm0;
		//	punpcklwd xmm0, xmm5		// xmm0 = [y31|y21|y11|y01|y30|y20|y10|y00]=[y1|y0]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm5);
		//	punpckhwd xmm1, xmm5		// xmm1 = [y33|y23|y13|y03|y32|y22|y12|y02]=[y3|y2]
		xmm1 = _mm_unpackhi_epi16(xmm1, xmm5);

	// vertical
		xmm2 = xmm1;

		xmm4 = xmm0;
		xmm5 = xmm2;
		xmm1 = xmm0;
		xmm3 = xmm2;
		xmm4 = _mm_srai_epi16(xmm4, 1);		// xmm4 = ([r1, r0]>>1)
		xmm5 = _mm_srai_epi16(xmm5, 1);		// xmm5 = ([r3, r2]>>1)
		xmm4 = _mm_srli_si128(xmm4, 8);		// xmm4 = ([r1]>>1)
		xmm5 = _mm_srli_si128(xmm5, 8);		// xmm5 = ([r3]>>1)
		xmm1 = _mm_unpacklo_epi64(xmm1, xmm4);	// xmm1 = ([r1]>>1), r0]	
		xmm3 = _mm_unpacklo_epi64(xmm3, xmm5);	// xmm3 = ([r3]>>1), r2]	

		//	pshufd	xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
//		xmm4 = xmm0;
		//	paddw		xmm0, xmm2			// xmm0 = [r1+r3>>1|r0+r2]
		xmm3 =	_mm_add_epi16(xmm3, xmm0);			// xmm3 = [r1+r3>>1|r0+r2] = [s3 s0]
		//	psubw		xmm4, xmm2			// xmm4 = [r1>>1-r3|r0-r2] 
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);			// xmm1 = [r1>>1-r3|r0-r2] = [s2 s1]
		xmm0 = xmm3;
		xmm4 = xmm1;

		//	pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4		// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		//	punpckhqdq	xmm3, xmm4		// xmm3 = [r1-r3|r1+r3]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		//	movdqa		xmm2, xmm0
		xmm2 = xmm0;
		//	paddw		xmm0, xmm3		// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3		// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			

		xmm4 = _mm_shuffle_epi32(xmm2, 0x4e);	// xmm4 = [y3 |y2]

		xmm5  = _mm_set1_epi16((short)32);    // round;

		xmm0 =	_mm_add_epi16(xmm0, xmm5);			
		xmm4 =	_mm_add_epi16(xmm4, xmm5);			

		xmm0 = _mm_srai_epi16(xmm0, 6);		// xmm4 = ([r1, r0]>>1)
		xmm4 = _mm_srai_epi16(xmm4, 6);		// xmm4 = ([r1, r0]>>1)

		_mm_store_si128( (__m128i*)pSrcDst, xmm0 ); // store 16 bytes
		_mm_store_si128( (__m128i*)(pSrcDst+8), xmm4 ); // store 16 bytes

#endif
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformDequantChromaDC_SISP_H264_16s_C1I)(Fw16s* pSrcDst,
														Fw16s*  pDCPredict, 
														Fw32s   qp, 
														Fw32s   qs, 
														Fw32s   Switch)
{
//	Fw16s* prow[4];
	Fw16s  a[4];
	Fw32s  i, ilev;

	Fw32s qp1_sp = qs/6;
	Fw32s qp2_sp = qs%6;
	Fw32s bits_sp = 15 + qp1_sp;
	Fw32s bits2_sp = (1<<bits_sp)/2;  //sp_pred

	if(Switch)
	{
		qp = qs;
	}

    a[0]=(pDCPredict[0]+pDCPredict[1]+pDCPredict[2]+pDCPredict[3]);
    a[1]=(pDCPredict[0]-pDCPredict[1]+pDCPredict[2]-pDCPredict[3]);
    a[2]=(pDCPredict[0]+pDCPredict[1]-pDCPredict[2]-pDCPredict[3]);
    a[3]=(pDCPredict[0]-pDCPredict[1]-pDCPredict[2]+pDCPredict[3]);

    for (i=0; i < 4; i++)
	{
		ilev=((pSrcDst[i]*(InvQuantTable1[qp][QuantIndex1[0]])*MATR[0]) >>5)+a[i];
		ilev > 0 ?
		a[i] = (Fw16s)(((abs(ilev)* QuantCoef[qp2_sp][0]+ 2 * bits2_sp)
			>> (bits_sp+1))*InvQuantTable1[qs][QuantIndex1[0]]) :
		
		a[i] = (Fw16s)(-((abs(ilev)* QuantCoef[qp2_sp][0]+ 2 * bits2_sp)
			>> (bits_sp+1))*InvQuantTable1[qs][QuantIndex1[0]]);
	}
	
    pSrcDst[0]=(a[0] + a[1] + a[2] + a[3])>>1;
    pSrcDst[1]=(a[0] - a[1] + a[2] - a[3])>>1;
    pSrcDst[2]=(a[0] + a[1] - a[2] - a[3])>>1;
    pSrcDst[3]=(a[0] - a[1] - a[2] + a[3])>>1;

	return fwStsNoErr;
}
#endif
#endif


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
