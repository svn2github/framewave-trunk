/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.

The following integer transform routines are based on ITU-H.264 doc 3/2005 and 
JVT reference code JM13.2 and is subject to the license terms below.
*/

/*
* JVT code subject to the following:
***********************************************************************
* COPYRIGHT AND WARRANTY INFORMATION
*
* Copyright 2001, International Telecommunications Union, Geneva
*
* DISCLAIMER OF WARRANTY
*
* These software programs are available to the user without any
* license fee or royalty on an "as is" basis. The ITU disclaims
* any and all warranties, whether express, implied, or
* statutory, including any implied warranties of merchantability
* or of fitness for a particular purpose.  In no event shall the
* contributor or the ITU be liable for any incidental, punitive, or
* consequential damages of any kind whatsoever arising from the
* use of these programs.
*
* This disclaimer of warranty extends to the user of these programs
* and user's customers, employees, agents, transferees, successors,
* and assigns.
*
* The ITU does not represent or warrant that the programs furnished
* hereunder are free of infringement of any third-party patents.
* Commercial implementations of ITU-T Recommendations, including
* shareware, may be subject to royalty fees to patent holders.
* Information regarding the ITU-T patent policy is available from
* the ITU Web site at http://www.itu.int.
*
* THIS IS NOT A GRANT OF PATENT RIGHTS - SEE THE ITU-T PATENT POLICY.
************************************************************************
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

// Based on ITU-H.264 doc 8.5.9
FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformQuantChromaDC_H264_16s_C1I)(Fw16s*	pSrcDst, 
												  Fw16s*	pTBlock, 
												  Fw32s	QPChroma, 
												  Fw8s*	NumLevels, 
												  Fw8u		Intra, 
												  Fw8u 	NeedTransform)
{
	int qp_const;
	int level,coeff_ctr, count;
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
		pTBlock[0] = pSrcDst[0] + pSrcDst[2] + pSrcDst[1] + pSrcDst[3];
		pTBlock[1] = pSrcDst[0] + pSrcDst[2] - pSrcDst[1] - pSrcDst[3];
		pTBlock[2] = pSrcDst[0] - pSrcDst[2] + pSrcDst[1] - pSrcDst[3];
		pTBlock[3] = pSrcDst[0] - pSrcDst[2] - pSrcDst[1] + pSrcDst[3];
	}
	// quant
	count = 0;
	for (coeff_ctr=0;coeff_ctr<4;coeff_ctr++)
	{
	    level =(abs(pTBlock[coeff_ctr]) * quant_coef[qp_rem][0][0] + 2*qp_const) >> (q_bits+1);
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
// Based on ITU-H.264 doc 8.5.8
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
// Based on ITU-H.264 doc 8.5.10
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
// Based on ITU-H.264 doc 8.5.9
FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformDequantChromaDC_H264_16s_C1I)(Fw16s* pSrcDst, Fw32s QP)
{
//	int j;
	Fw16s tmp[4];
	int qp_per = QP/6;
	int qp_rem = QP%6;
    int qbits = qp_per - 1;

    tmp[0] = pSrcDst[0];
    tmp[1] = pSrcDst[1];
    tmp[2] = pSrcDst[2];
    tmp[3] = pSrcDst[3];
    pSrcDst[0] = tmp[0] + tmp[2] + tmp[1] + tmp[3];
    pSrcDst[1] = tmp[0] + tmp[2] - tmp[1] - tmp[3];
    pSrcDst[2] = tmp[0] - tmp[2] + tmp[1] - tmp[3];
    pSrcDst[3] = tmp[0] - tmp[2] - tmp[1] + tmp[3];
	// quantization
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
// Based on ITU-H.264 doc 8.5.10
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidual_H264_16s_C1I)(Fw16s* pSrcDst, 
													Fw32s  step, 
													Fw16s  *pDC, 
													Fw32s  AC, 
													Fw32s  QP)
{
	int step_SHORT = step/2;
	int qp_per = (QP)/6;
	int qp_rem = (QP)%6;
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
				pSrcDst[0] = pSrcDst[0] * (Fw16s)(dequant_coef[qp_rem][0][0]<<qbits);
			}
			else
			{
				pSrcDst[0] = (Fw16s)((pSrcDst[0] * (Fw16s)dequant_coef[qp_rem][0][0]) >> (-qbits));
			}
		}
	}
    qbits = qp_per;
	if(AC) {
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
	return fwStsNoErr;
}

#endif


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
