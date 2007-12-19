/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
#include "fwVideo.h"
#include "system.h"
#include "system.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST(101)

#define Q_BITS          15



FwStatus PREFIX_OPT(OPT_PREFIX, fwiPredictIntra_4x4_H264_8u_C1IR)(Fw8u* pSrcDst, 
											Fw32s srcdstStep,
											FwIntra4x4PredMode_H264 predMode, 
											Fw32s availability)
{
	if(pSrcDst == NULL) return fwStsNullPtrErr;
	if(srcdstStep < 4) return fwStsStepErr;
	if(predMode < 0 || predMode > 8) return fwStsOutOfRangeErr;

	int block_available_up=0;
	int block_available_left=0;
	int block_available_up_left=0;
	int block_available_up_right=0;

	if(FW_LEFT & availability)			block_available_left = 1;
	if(FW_UPPER & availability)		block_available_up = 1;
	if(FW_UPPER_RIGHT & availability)	block_available_up_right = 1;
	if(FW_UPPER_LEFT & availability && block_available_left && block_available_up)	
		block_available_up_left = 1;

//	  int i, j;
	  Fw32u X,A,B,C,D,E,F,G,H,I,J,K,L;
	  Fw32u dc;


	  /*
	   * Copy reconstruction pixels used for prediction
	   */

	  if (block_available_up_left) /* up-left */
		X = pSrcDst[-srcdstStep-1];
	  else
		X = 128;

	  if (block_available_up) { /* up */
		A = pSrcDst[-srcdstStep+0];
		B = pSrcDst[-srcdstStep+1];
		C = pSrcDst[-srcdstStep+2];
		D = pSrcDst[-srcdstStep+3];
		if (block_available_up_right) { /* up-right */
		  E = pSrcDst[-srcdstStep+4];
		  F = pSrcDst[-srcdstStep+5];
		  G = pSrcDst[-srcdstStep+6];
		  H = pSrcDst[-srcdstStep+7];
		}
		else
		  E = F = G = H = D;
	  }
	  else
		A = B = C = D = E = F = G = H = 128;

	  if (block_available_left) { /* left */
		I = pSrcDst[0*srcdstStep-1];
		J = pSrcDst[1*srcdstStep-1];
		K = pSrcDst[2*srcdstStep-1];
		L = pSrcDst[3*srcdstStep-1];
	  }
	  else
		I = J = K = L = 128;

	  switch (predMode) {

		  case FW_4x4_DC:

			/* DC PREDICTION */ 
			if (block_available_up && block_available_left) /* up & left */
			  dc = (A+B+C+D+I+J+K+L+4)>>3;
			else if (block_available_up) /* up */
			  dc = (A+B+C+D+2)>>2;
			else if (block_available_left) /* left */
			  dc = (I+J+K+L+2)>>2;
			else
			  dc = 128;
#if 0
			for (j = 0; j < 4; j++) {
			  pSrcDst[j*srcdstStep+0] = (Fw8u) dc;
			  pSrcDst[j*srcdstStep+1] = (Fw8u) dc;
			  pSrcDst[j*srcdstStep+2] = (Fw8u) dc;
			  pSrcDst[j*srcdstStep+3] = (Fw8u) dc;
			}
#else
			{
				Fw32u val = (Fw32u)dc*0x01010101;					// duplicate 4 DC value
				*(Fw32u*)&pSrcDst[0*srcdstStep] =
				*(Fw32u*)&pSrcDst[1*srcdstStep] =
				*(Fw32u*)&pSrcDst[2*srcdstStep] =
				*(Fw32u*)&pSrcDst[3*srcdstStep] = val;	
			}
#endif
      
			break;

		  case FW_4x4_VERT:

			/* VERTICAL PREDICTION */
#if 0  
			for (j = 0; j < 4; j++) {
			  pSrcDst[j*srcdstStep+0] = (Fw8u) A;
			  pSrcDst[j*srcdstStep+1] = (Fw8u) B;
			  pSrcDst[j*srcdstStep+2] = (Fw8u) C;
			  pSrcDst[j*srcdstStep+3] = (Fw8u) D;
			}
#else
			{
				Fw32u val = A + (B<<8) + (C<<16) + (D<<24);
				*(Fw32u*)&pSrcDst[0*srcdstStep] =
				*(Fw32u*)&pSrcDst[1*srcdstStep] =
				*(Fw32u*)&pSrcDst[2*srcdstStep] =
				*(Fw32u*)&pSrcDst[3*srcdstStep] = val;	
			}
#endif
			break;

		  case FW_4x4_HOR:

			/* HORIZONTAL PREDICTION */
#if 0
			for (i = 0; i < 4; i++) {
			  pSrcDst[0*srcdstStep+i] = (Fw8u) I;
			  pSrcDst[1*srcdstStep+i] = (Fw8u) J;
			  pSrcDst[2*srcdstStep+i] = (Fw8u) K;
			  pSrcDst[3*srcdstStep+i] = (Fw8u) L;
			}
#else
			{
				*(Fw32u*)&pSrcDst[0*srcdstStep] =(Fw32u)I*0x01010101;
				*(Fw32u*)&pSrcDst[1*srcdstStep] =(Fw32u)J*0x01010101;
				*(Fw32u*)&pSrcDst[2*srcdstStep] =(Fw32u)K*0x01010101;
				*(Fw32u*)&pSrcDst[3*srcdstStep] =(Fw32u)L*0x01010101;
			}
#endif
			break;

		  case FW_4x4_DIAG_DR:

			/* DIAGONAL PREDICTION */
#if 0
				pSrcDst[3*srcdstStep+0] = (Fw8u) ((L+2*K+J+2)>>2);
				pSrcDst[2*srcdstStep+0] = 
				pSrcDst[3*srcdstStep+1] = (Fw8u) ((K+2*J+I+2)>>2);
				pSrcDst[1*srcdstStep+0] = 
				pSrcDst[2*srcdstStep+1] = 
				pSrcDst[3*srcdstStep+2] = (Fw8u) ((J+2*I+X+2)>>2);
				pSrcDst[0*srcdstStep+0] = 
				pSrcDst[1*srcdstStep+1] = 
				pSrcDst[2*srcdstStep+2] = 
				pSrcDst[3*srcdstStep+3] = (Fw8u) ((I+2*X+A+2)>>2);
				pSrcDst[0*srcdstStep+1] = 
				pSrcDst[1*srcdstStep+2] = 
				pSrcDst[2*srcdstStep+3] = (Fw8u) ((X+2*A+B+2)>>2);
				pSrcDst[0*srcdstStep+2] = 
				pSrcDst[1*srcdstStep+3] = (Fw8u) ((A+2*B+C+2)>>2);
				pSrcDst[0*srcdstStep+3] = (Fw8u) ((B+2*C+D+2)>>2);
#else
			{
				Fw32u value;
				value = K+J+2;
				pSrcDst[3*srcdstStep+0] = (Fw8u) ((L+K+value)>>2);
				pSrcDst[2*srcdstStep+0] = 
				pSrcDst[3*srcdstStep+1] = (Fw8u) ((value+J+I)>>2);
				value = I+X+2;
				pSrcDst[1*srcdstStep+0] = 
				pSrcDst[2*srcdstStep+1] = 
				pSrcDst[3*srcdstStep+2] = (Fw8u) ((J+I+value)>>2);
				pSrcDst[0*srcdstStep+0] = 
				pSrcDst[1*srcdstStep+1] = 
				pSrcDst[2*srcdstStep+2] = 
				pSrcDst[3*srcdstStep+3] = (Fw8u) ((value+X+A)>>2);
				value = A+B+2;
				pSrcDst[0*srcdstStep+1] = 
				pSrcDst[1*srcdstStep+2] = 
				pSrcDst[2*srcdstStep+3] = (Fw8u) ((X+A+value)>>2);
				pSrcDst[0*srcdstStep+2] = 
				pSrcDst[1*srcdstStep+3] = (Fw8u) ((value+B+C)>>2);
				pSrcDst[0*srcdstStep+3] = (Fw8u) ((B+2*C+D+2)>>2);
			}
#endif
			break;

		  case FW_4x4_DIAG_DL:
#if 0
			pSrcDst[0*srcdstStep+0] = (Fw8u) ((A+2*B+C+2)>>2);
			pSrcDst[0*srcdstStep+1] = 
			pSrcDst[1*srcdstStep+0] = (Fw8u) ((B+2*C+D+2)>>2);
			pSrcDst[0*srcdstStep+2] =
			pSrcDst[1*srcdstStep+1] =
			pSrcDst[2*srcdstStep+0] = (Fw8u) ((C+2*D+E+2)>>2);
			pSrcDst[0*srcdstStep+3] = 
			pSrcDst[1*srcdstStep+2] = 
			pSrcDst[2*srcdstStep+1] = 
			pSrcDst[3*srcdstStep+0] = (Fw8u) ((D+2*E+F+2)>>2);
			pSrcDst[1*srcdstStep+3] = 
			pSrcDst[2*srcdstStep+2] = 
			pSrcDst[3*srcdstStep+1] = (Fw8u) ((E+2*F+G+2)>>2);
			pSrcDst[2*srcdstStep+3] = 
			pSrcDst[3*srcdstStep+2] = (Fw8u) ((F+2*G+H+2)>>2);
			pSrcDst[3*srcdstStep+3] = (Fw8u) ((G+3*H+2)>>2);
#else
			{
				Fw32u value;
				value = B+C+2;
				pSrcDst[0*srcdstStep+0] = (Fw8u) ((A+B+value)>>2);
				pSrcDst[0*srcdstStep+1] = 
				pSrcDst[1*srcdstStep+0] = (Fw8u) ((value+C+D)>>2);
				value = D+E+2;
				pSrcDst[0*srcdstStep+2] =
				pSrcDst[1*srcdstStep+1] =
				pSrcDst[2*srcdstStep+0] = (Fw8u) ((C+D+value)>>2);
				pSrcDst[0*srcdstStep+3] = 
				pSrcDst[1*srcdstStep+2] = 
				pSrcDst[2*srcdstStep+1] = 
				pSrcDst[3*srcdstStep+0] = (Fw8u) ((value+E+F)>>2);
				value = F+G+2;
				pSrcDst[1*srcdstStep+3] = 
				pSrcDst[2*srcdstStep+2] = 
				pSrcDst[3*srcdstStep+1] = (Fw8u) ((E+F+value)>>2);
				pSrcDst[2*srcdstStep+3] = 
				pSrcDst[3*srcdstStep+2] = (Fw8u) ((value+G+H)>>2);
				pSrcDst[3*srcdstStep+3] = (Fw8u) ((G+3*H+2)>>2);			
			}
#endif
			break;

		  case FW_4x4_VR:/* diagonal prediction -22.5 deg to horizontal plane */
#if 0
			pSrcDst[0*srcdstStep+0] = 
			pSrcDst[2*srcdstStep+1] = (Fw8u) ((X+A+1)>>1);
			pSrcDst[0*srcdstStep+1] = 
			pSrcDst[2*srcdstStep+2] = (Fw8u) ((A+B+1)>>1);
			pSrcDst[0*srcdstStep+2] = 
			pSrcDst[2*srcdstStep+3] = (Fw8u) ((B+C+1)>>1);
			pSrcDst[0*srcdstStep+3] = (Fw8u) ((C+D+1)>>1);
			pSrcDst[1*srcdstStep+0] = 
			pSrcDst[3*srcdstStep+1] = (Fw8u) ((I+2*X+A+2)>>2);
			pSrcDst[1*srcdstStep+1] = 
			pSrcDst[3*srcdstStep+2] = (Fw8u) ((X+2*A+B+2)>>2);
			pSrcDst[1*srcdstStep+2] = 
			pSrcDst[3*srcdstStep+3] = (Fw8u) ((A+2*B+C+2)>>2);
			pSrcDst[1*srcdstStep+3] = (Fw8u) ((B+2*C+D+2)>>2);
			pSrcDst[2*srcdstStep+0] = (Fw8u) ((X+2*I+J+2)>>2);
			pSrcDst[3*srcdstStep+0] = (Fw8u) ((I+2*J+K+2)>>2);
#else
			{
				Fw32u X_A_1, A_B_1, B_C_1, C_D_1;
				X_A_1 = X+A+1;
				A_B_1 = A+B+1;
				B_C_1 = B+C+1;
				C_D_1 = C+D+1;
				pSrcDst[0*srcdstStep+0] = 
				pSrcDst[2*srcdstStep+1] = (Fw8u) ((X_A_1)>>1);
				pSrcDst[0*srcdstStep+1] = 
				pSrcDst[2*srcdstStep+2] = (Fw8u) ((A_B_1)>>1);
				pSrcDst[0*srcdstStep+2] = 
				pSrcDst[2*srcdstStep+3] = (Fw8u) ((B_C_1)>>1);
				pSrcDst[0*srcdstStep+3] = (Fw8u) ((C_D_1)>>1);
				pSrcDst[1*srcdstStep+0] = 
				pSrcDst[3*srcdstStep+1] = (Fw8u) ((I+2*X+A+2)>>2);
				pSrcDst[1*srcdstStep+1] = 
				pSrcDst[3*srcdstStep+2] = (Fw8u) ((X_A_1+A_B_1)>>2);
				pSrcDst[1*srcdstStep+2] = 
				pSrcDst[3*srcdstStep+3] = (Fw8u) ((A+2*B+C+2)>>2);
				pSrcDst[1*srcdstStep+3] = (Fw8u) ((B_C_1+C_D_1)>>2);
				pSrcDst[2*srcdstStep+0] = (Fw8u) ((X+2*I+J+2)>>2);
				pSrcDst[3*srcdstStep+0] = (Fw8u) ((I+2*J+K+2)>>2);	
			}
#endif
			break;

		  case FW_4x4_VL:/* diagonal prediction -22.5 deg to horizontal plane */
#if 0
			pSrcDst[0*srcdstStep+0] = (Fw8u) ((A+B+1)>>1);
			pSrcDst[0*srcdstStep+1] = 
			pSrcDst[2*srcdstStep+0] = (Fw8u) ((B+C+1)>>1);
			pSrcDst[0*srcdstStep+2] = 
			pSrcDst[2*srcdstStep+1] = (Fw8u) ((C+D+1)>>1);
			pSrcDst[0*srcdstStep+3] = 
			pSrcDst[2*srcdstStep+2] = (Fw8u) ((D+E+1)>>1);
			pSrcDst[2*srcdstStep+3] = (Fw8u) ((E+F+1)>>1);
			pSrcDst[1*srcdstStep+0] = (Fw8u) ((A+2*B+C+2)>>2);
			pSrcDst[1*srcdstStep+1] = 
			pSrcDst[3*srcdstStep+0] = (Fw8u) ((B+2*C+D+2)>>2);
			pSrcDst[1*srcdstStep+2] = 
			pSrcDst[3*srcdstStep+1] = (Fw8u) ((C+2*D+E+2)>>2);
			pSrcDst[1*srcdstStep+3] = 
			pSrcDst[3*srcdstStep+2] = (Fw8u) ((D+2*E+F+2)>>2);
			pSrcDst[3*srcdstStep+3] = (Fw8u) ((E+2*F+G+2)>>2);
#else
			{
				Fw32u X_A_1, A_B_1, B_C_1, C_D_1, D_E_1, E_F_1;
				X_A_1 = X+A+1;
				A_B_1 = A+B+1;
				B_C_1 = B+C+1;
				C_D_1 = C+D+1;
				D_E_1 = D+E+1;
				E_F_1 = E+F+1;
				pSrcDst[0*srcdstStep+0] = (Fw8u) ((A_B_1)>>1);
				pSrcDst[0*srcdstStep+1] = 
				pSrcDst[2*srcdstStep+0] = (Fw8u) ((B_C_1)>>1);
				pSrcDst[0*srcdstStep+2] = 
				pSrcDst[2*srcdstStep+1] = (Fw8u) ((C_D_1)>>1);
				pSrcDst[0*srcdstStep+3] = 
				pSrcDst[2*srcdstStep+2] = (Fw8u) ((D+E+1)>>1);
				pSrcDst[2*srcdstStep+3] = (Fw8u) ((E+F+1)>>1);
				pSrcDst[1*srcdstStep+0] = (Fw8u) ((A_B_1+B_C_1)>>2);
				pSrcDst[1*srcdstStep+1] = 
				pSrcDst[3*srcdstStep+0] = (Fw8u) ((B_C_1+C_D_1)>>2);
				pSrcDst[1*srcdstStep+2] = 
				pSrcDst[3*srcdstStep+1] = (Fw8u) ((C_D_1+D_E_1)>>2);
				pSrcDst[1*srcdstStep+3] = 
				pSrcDst[3*srcdstStep+2] = (Fw8u) ((D_E_1+E_F_1)>>2);
				pSrcDst[3*srcdstStep+3] = (Fw8u) ((E_F_1+F+G+1)>>2);			
			}
#endif
			break;

		  case FW_4x4_HU:/* diagonal prediction -22.5 deg to horizontal plane */
#if 0
			pSrcDst[0*srcdstStep+0] = (Fw8u) ((I+J+1)>>1);
			pSrcDst[0*srcdstStep+1] = (Fw8u) ((I+2*J+K+2)>>2);
			pSrcDst[0*srcdstStep+2] = 
			pSrcDst[1*srcdstStep+0] = (Fw8u) ((J+K+1)>>1);
			pSrcDst[0*srcdstStep+3] = 
			pSrcDst[1*srcdstStep+1] = (Fw8u) ((J+2*K+L+2)>>2);
			pSrcDst[1*srcdstStep+2] = 
			pSrcDst[2*srcdstStep+0] = (Fw8u) ((K+L+1)>>1);
			pSrcDst[1*srcdstStep+3] = 
			pSrcDst[2*srcdstStep+1] = (Fw8u) ((K+2*L+L+2)>>2);
			pSrcDst[2*srcdstStep+3] = 
			pSrcDst[2*srcdstStep+2] = 
			pSrcDst[3*srcdstStep+0] = 
			pSrcDst[3*srcdstStep+1] = 
			pSrcDst[3*srcdstStep+2] = 
			pSrcDst[3*srcdstStep+3] = (Fw8u) L;
#else
			{
				Fw32u I_J_1, J_K_1, K_L_1;
				I_J_1 = I+J+1;
				J_K_1 = J+K+1;
				K_L_1 = K+L+1;
				pSrcDst[0*srcdstStep+0] = (Fw8u) ((I_J_1)>>1);
				pSrcDst[0*srcdstStep+1] = (Fw8u) ((I_J_1+J_K_1)>>2);
				pSrcDst[0*srcdstStep+2] = 
				pSrcDst[1*srcdstStep+0] = (Fw8u) ((J_K_1)>>1);
				pSrcDst[0*srcdstStep+3] = 
				pSrcDst[1*srcdstStep+1] = (Fw8u) ((J_K_1+K_L_1)>>2);
				pSrcDst[1*srcdstStep+2] = 
				pSrcDst[2*srcdstStep+0] = (Fw8u) ((K_L_1)>>1);
				pSrcDst[1*srcdstStep+3] = 
				pSrcDst[2*srcdstStep+1] = (Fw8u) ((K_L_1+2*L+1)>>2);
				pSrcDst[2*srcdstStep+3] = 
				pSrcDst[2*srcdstStep+2] = 
				pSrcDst[3*srcdstStep+0] = 
				pSrcDst[3*srcdstStep+1] = 
				pSrcDst[3*srcdstStep+2] = 
				pSrcDst[3*srcdstStep+3] = (Fw8u) L;
			}
#endif
			break;

		  case FW_4x4_HD:/* diagonal prediction -22.5 deg to horizontal plane */
#if 0
			pSrcDst[0*srcdstStep+0] = 
			pSrcDst[1*srcdstStep+2] = (Fw8u) ((X+I+1)>>1);
			pSrcDst[0*srcdstStep+1] = 
			pSrcDst[1*srcdstStep+3] = (Fw8u) ((I+2*X+A+2)>>2);
			pSrcDst[0*srcdstStep+2] = (Fw8u) ((X+2*A+B+2)>>2);
			pSrcDst[0*srcdstStep+3] = (Fw8u) ((A+2*B+C+2)>>2);
			pSrcDst[1*srcdstStep+0] = 
			pSrcDst[2*srcdstStep+2] = (Fw8u) ((I+J+1)>>1);
			pSrcDst[1*srcdstStep+1] = 
			pSrcDst[2*srcdstStep+3] = (Fw8u) ((X+2*I+J+2)>>2);
			pSrcDst[2*srcdstStep+0] = 
			pSrcDst[3*srcdstStep+2] = (Fw8u) ((J+K+1)>>1);
			pSrcDst[2*srcdstStep+1] = 
			pSrcDst[3*srcdstStep+3] = (Fw8u) ((I+2*J+K+2)>>2);
			pSrcDst[3*srcdstStep+0] = (Fw8u) ((K+L+1)>>1);
			pSrcDst[3*srcdstStep+1] = (Fw8u) ((J+2*K+L+2)>>2);
#else
			{
				Fw32u X_I_1, I_J_1, J_K_1, K_L_1;
				X_I_1 = X+I+1;
				I_J_1 = I+J+1;
				J_K_1 = J+K+1;
				K_L_1 = K+L+1;
				pSrcDst[0*srcdstStep+0] = 
				pSrcDst[1*srcdstStep+2] = (Fw8u) ((X_I_1)>>1);
				pSrcDst[0*srcdstStep+1] = 
				pSrcDst[1*srcdstStep+3] = (Fw8u) ((I+2*X+A+2)>>2);
				pSrcDst[0*srcdstStep+2] = (Fw8u) ((X+2*A+B+2)>>2);
				pSrcDst[0*srcdstStep+3] = (Fw8u) ((A+2*B+C+2)>>2);
				pSrcDst[1*srcdstStep+0] = 
				pSrcDst[2*srcdstStep+2] = (Fw8u) ((I_J_1)>>1);
				pSrcDst[1*srcdstStep+1] = 
				pSrcDst[2*srcdstStep+3] = (Fw8u) ((X_I_1+I_J_1)>>2);
				pSrcDst[2*srcdstStep+0] = 
				pSrcDst[3*srcdstStep+2] = (Fw8u) ((J_K_1)>>1);
				pSrcDst[2*srcdstStep+1] = 
				pSrcDst[3*srcdstStep+3] = (Fw8u) ((I_J_1+J_K_1)>>2);
				pSrcDst[3*srcdstStep+0] = (Fw8u) ((K_L_1)>>1);
				pSrcDst[3*srcdstStep+1] = (Fw8u) ((J_K_1+K_L_1)>>2);
			}
#endif
			break;

		  default:
			break;
	  }

	return fwStsNoErr;

}


/* This macro clips value val to the range of [min, max] */
#define  clip(min, max, val) (((val)<(min))? (min):(((val)>(max))? (max):(val)))
FwStatus PREFIX_OPT(OPT_PREFIX, fwiPredictIntra_16x16_H264_8u_C1IR)(Fw8u* pSrcDst, 
											  Fw32s srcdstStep, 
											  FwIntra16x16PredMode_H264 predMode, 
											  Fw32s availability)
{
	if(pSrcDst == NULL) return fwStsNullPtrErr;
	if(srcdstStep < 4) return fwStsStepErr;
	if(predMode < 0 || predMode > 3) return fwStsOutOfRangeErr;

	int block_available_up=0;
	int block_available_left=0;
	int block_available_up_left=0;
	int block_available_up_right=0;

	if(FW_LEFT & availability)			block_available_left = 1;
	if(FW_UPPER & availability)		block_available_up = 1;
	if(FW_UPPER_RIGHT & availability)	block_available_up_right = 1;
	if(FW_UPPER_LEFT & availability && block_available_left && block_available_up)	
		block_available_up_left = 1;

	  int dc;
	  int H, V;
	  int a, b, c;
	  int i, j;
	  int tmp, tmp2;
	  Fw8u *recoPtr, *recoPtr2;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;

	  switch (predMode) {

	  case FW_16X16_DC:		/* DC PREDICTION */

		if (block_available_up && block_available_left) {
		  recoPtr = &pSrcDst[srcdstStep];
		  for (dc = 0, i = 0; i < 16; i++) {
			dc += pSrcDst[1+i] + (*recoPtr);
			recoPtr += srcdstStep;
		  }
		  dc = (dc+16)>>5;
		}
		else if (block_available_up) {
		  recoPtr = &pSrcDst[srcdstStep];
		  for (dc = 0, i = 0; i < 16; i++) {
			dc += (*recoPtr);
			recoPtr += srcdstStep;
		  }
		  dc = (dc+8)>>4;
		}
		else if (block_available_left) {
		  for (dc = 0, i = 0; i < 16; i++)
			dc += pSrcDst[1+i];
		  dc = (dc+8)>>4;
		}
		else
		  dc = 128;

		if( Dispatch_Type==DT_SSE2)
		{
			xmm7  = _mm_set1_epi8((Fw8u)dc); 
			for (j = 0; j < 16; j++) {
				_mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
			}
		}
		else 
		{
			for (j = 0; j < 16; j++) {
			  for (i = 0; i < 16; i+=4) {
				pSrcDst[j*srcdstStep+i+0] = (Fw8u) dc;
				pSrcDst[j*srcdstStep+i+1] = (Fw8u) dc;
				pSrcDst[j*srcdstStep+i+2] = (Fw8u) dc;
				pSrcDst[j*srcdstStep+i+3] = (Fw8u) dc;
			  }
			}
		}
		break;

	  case FW_16X16_VERT:			/* VERTICAL PREDICTION */

		if (!(block_available_left)) return fwStsLPCCalcErr;

#if 1
		if( Dispatch_Type==DT_SSE2)
		{
			xmm7 = _mm_load_si128( (__m128i*)(pSrcDst-srcdstStep) );
			for (j = 0; j < 16; j++) {
				_mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
			}
		}
		else 
		{
			Fw32u tmpa= ((Fw32u*)(pSrcDst-srcdstStep))[0];
			Fw32u tmpb= ((Fw32u*)(pSrcDst-srcdstStep))[1];
			Fw32u tmpc= ((Fw32u*)(pSrcDst-srcdstStep))[2];
			Fw32u tmpd= ((Fw32u*)(pSrcDst-srcdstStep))[3];

			for(i=0; i<16; i++){
				((Fw32u*)(pSrcDst+i*srcdstStep))[0]= tmpa;
				((Fw32u*)(pSrcDst+i*srcdstStep))[1]= tmpb;
				((Fw32u*)(pSrcDst+i*srcdstStep))[2]= tmpc;
				((Fw32u*)(pSrcDst+i*srcdstStep))[3]= tmpd;
			}	
		}
#else
		if( Dispatch_Type==DT_SSE2)
		{
//			for (i = 0; i < 16; i++) {
//			  tmp = pSrcDst[1+i];
			  xmm7 = _mm_load_si128( (__m128i*)(pSrcDst+1) );
/*
			  for (j = 0; j < 16; j+=4) {
				pSrcDst[(j+0)*srcdstStep+i] = (Fw8u) tmp;
				pSrcDst[(j+1)*srcdstStep+i] = (Fw8u) tmp;
				pSrcDst[(j+2)*srcdstStep+i] = (Fw8u) tmp;
				pSrcDst[(j+3)*srcdstStep+i] = (Fw8u) tmp;
			  }
			}
*/
			for (j = 0; j < 16; j++) {
				_mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
			}
		}
		else 
		{
			for (i = 0; i < 16; i++) {
			  tmp = pSrcDst[1+i];
			  for (j = 0; j < 16; j+=4) {
				pSrcDst[(j+0)*srcdstStep+i] = (Fw8u) tmp;
				pSrcDst[(j+1)*srcdstStep+i] = (Fw8u) tmp;
				pSrcDst[(j+2)*srcdstStep+i] = (Fw8u) tmp;
				pSrcDst[(j+3)*srcdstStep+i] = (Fw8u) tmp;
			  }
			}
		}
#endif
		break;

	  case FW_16X16_HOR:		/* HORIZONTAL PREDICTION */

		if (!(block_available_up)) return fwStsLPCCalcErr;

		if( Dispatch_Type==DT_SSE2)
		{
			recoPtr = &pSrcDst[srcdstStep];
			for (j = 0; j < 16; j++) {
			  tmp = *recoPtr;
			  recoPtr += srcdstStep;
	  		  xmm7  = _mm_set1_epi8((Fw8u)tmp); 
			  _mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
			}
		}
		else
		{
			recoPtr = &pSrcDst[srcdstStep];
			for (j = 0; j < 16; j++) {
			  tmp = *recoPtr;
			  recoPtr += srcdstStep;
			  for (i = 0; i < 16; i+=4) {
				pSrcDst[j*srcdstStep+i+0] = (Fw8u) tmp;
				pSrcDst[j*srcdstStep+i+1] = (Fw8u) tmp;
				pSrcDst[j*srcdstStep+i+2] = (Fw8u) tmp;
				pSrcDst[j*srcdstStep+i+3] = (Fw8u) tmp;
			  }
			}
		}
		break;

	  case FW_16X16_PLANE:		/* PLANE PREDICTION */

		if (!(block_available_up) || !(block_available_left) || !(block_available_up_left)) 
			return fwStsLPCCalcErr;

		recoPtr  = &pSrcDst[9*srcdstStep];
		recoPtr2 = &pSrcDst[7*srcdstStep];
		for (H = 0, V = 0, i = 1; i <= 8; i++) {
		  H += (i * (pSrcDst[8+i] - pSrcDst[8-i]));
		  V += (i * ((*recoPtr) - (*recoPtr2)));
		  recoPtr  += srcdstStep;
		  recoPtr2 -= srcdstStep;
		}
		a = 16*(pSrcDst[16*srcdstStep] + pSrcDst[16]);
		b = (int)((5*((Fw32s)H)+32)>>6);
		c = (int)((5*((Fw32s)V)+32)>>6);

		if( Dispatch_Type==DT_SSE2)
		{
			SYS_FORCEALIGN_16 const Fw16s c_b[16] = {-7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8};
			{
				xmm6  = _mm_load_si128( (__m128i*)(c_b) );	//[-7, 0]
				xmm7  = _mm_load_si128( (__m128i*)(c_b+8) );
				xmm5  = _mm_set1_epi16((Fw16s)b);
//				tmp = a+c*(0-7)+16;
				xmm3  = _mm_set1_epi16((Fw16s)(a+16-7*c));	 // a+16-7*c
				xmm1  = xmm5;	//b
				xmm1  = _mm_mullo_epi16(xmm1, xmm6); //b*[-7,0]
				xmm2  = xmm5;	//b
				xmm2  = _mm_mullo_epi16(xmm2, xmm7); //b*[1, 8]

				xmm1  =	_mm_add_epi16(xmm1, xmm3);	//a+b*[-7,0]+16-7*c
				xmm2  =	_mm_add_epi16(xmm2, xmm3);	//a+b*[1, 8]+16-7*c

				xmm4  = _mm_set1_epi16((Fw16s)(c));	// c
/*
				for (j = 0; j < 16; j++, tmp+=c) {
				  tmp2 = tmp + b*(0-7);
				  for (i = 0; i < 16; i++, tmp2+=b) {
					pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,tmp2>>5);
				  }
				}
*/
				for (j = 0; j < 16; j++) {
					xmm0 = xmm1;
					xmm7 = xmm2;
					xmm0 = _mm_srai_epi16(xmm0, 5);		//>>5
					xmm7 = _mm_srai_epi16(xmm7, 5);		//>>5
					xmm0 = _mm_unpacklo_epi64(xmm0, xmm7);	//clip(0,255,tmp2>>5);
					_mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm0 ); // store 16 bytes
					xmm1  =	_mm_add_epi16(xmm1, xmm4);	//+c
					xmm2  =	_mm_add_epi16(xmm2, xmm4);	//+c
				}
			}
		}
		else
		{
			tmp = a + c*(0-7) + 16;
			for (j = 0; j < 16; j++, tmp+=c) {
			  tmp2 = tmp + b*(0-7);
			  for (i = 0; i < 16; i++, tmp2+=b) {
				pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,tmp2>>5);
			  }
			}
		}
		break;
	  }

	return fwStsNoErr;

/*
Return Values
fwStsNoErr Indicates no error.
fwStsNullPtrErr Indicates an error condition if at least one of the specified pointers is
NULL.
fwStsStepErr Indicates an error condition if srcdstStep value is less than 16.
fwStsOutOfRangeErr Indicates an error condition if predMode value falls outside [0,3].
fwStsLPCCalcErr Indicates an error condition if predMode is not allowed for this block.

*/
}


#if 1
FwStatus PREFIX_OPT(OPT_PREFIX, fwiPredictIntraChroma8x8_H264_8u_C1IR)(Fw8u* pSrcDst, 
												 Fw32s srcdstStep, 
												 FwIntraChromaPredMode_H264 predMode, 
												 Fw32s availability)

{
	if(pSrcDst == NULL) return fwStsNullPtrErr;
	if(srcdstStep < 4) return fwStsStepErr;
	if(predMode < 0 || predMode > 3) return fwStsOutOfRangeErr;

	int block_available_up=0;
	int block_available_left=0;
	int block_available_up_left=0;

	if(FW_LEFT		  & availability) block_available_left = 1;
	if(FW_UPPER_LEFT & availability) block_available_up_left = 1;
	if(FW_UPPER	  & availability) block_available_up = 1;

//	int comp;
	Fw8u *recoPic;
	int S0, S1, S2, S3;
	int A, B, C, D;
	int H, V, a, b, c;
	int i, j;
	int tmp;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm1, xmm3, xmm4, xmm5, xmm6, xmm7;

	switch (predMode) {

	  case FW_CHROMA_DC:
		  recoPic = pSrcDst;
      	  S0 = S1 = S2 = S3 = 0;

		 if(block_available_up_left==1)   block_available_left= block_available_up=1;
		  if (block_available_up) {
#if 0
			for (i = 0; i < 4; i++) {
			  S2 += recoPic[(1+i)*srcdstStep];
			  S3 += recoPic[(5+i)*srcdstStep];
			}
#else
			for( i = 0; i < 4; i++ )
			{
				S0 += pSrcDst[i   - srcdstStep];
				S1 += pSrcDst[i+4 - srcdstStep];
			}
#endif
		  }
		  if (block_available_left) {
#if 0
			for (i = 0; i < 4; i++) {
			  S0 += recoPic[1+i];
			  S1 += recoPic[5+i];
			}
#else
			for( i = 0; i < 4; i++ )
			{
				S2 += pSrcDst[i     * srcdstStep - 1];
				S3 += pSrcDst[(i+4) * srcdstStep - 1];
			}
#endif
		  }
//printf("\n A=%d B=%d C=%d D=%d", A,B,C,D);

		  if (block_available_up && block_available_left) {
			A = (S0 + S2 + 4)>>3;
			B = (S1 + 2)>>2;
			C = (S3 + 2)>>2;
			D = (S1 + S3 + 4)>>3;
		  }
		  else if (block_available_up) {
			A = B = (S0 + 2)>>2;
			C = D = (S1 + 2)>>2;
		  }
		  else if (block_available_left) {
			A = C = (S2 + 2)>>2;
			B = D = (S3 + 2)>>2;
		  }
		  else
			A = B = C = D = 128;
      
#if 1
			{
//				Fw32u val = (Fw32u)0x01010101;					// duplicate 4 DC value
				Fw32u A_4 =(Fw32u)A*0x01010101;
				Fw32u B_4 =(Fw32u)B*0x01010101;
				Fw32u C_4 =(Fw32u)C*0x01010101;
				Fw32u D_4 =(Fw32u)D*0x01010101;
				for (j = 0; j < 4; j++) {	
					*(Fw32u*)&pSrcDst[j*srcdstStep] = A_4;
					*(Fw32u*)&pSrcDst[j*srcdstStep+4] = B_4;
					*(Fw32u*)&pSrcDst[(4+j)*srcdstStep] = C_4;
					*(Fw32u*)&pSrcDst[(4+j)*srcdstStep+4] = D_4;
				}
			}
#else
		  for (j = 0; j < 4; j++) {
			for (i = 0; i < 4; i++) {
			  pSrcDst[j*srcdstStep+i  ] = (Fw8u) A;
			  pSrcDst[j*srcdstStep+4+i] = (Fw8u) B;
			  pSrcDst[(4+j)*srcdstStep+i  ] = (Fw8u) C;
			  pSrcDst[(4+j)*srcdstStep+4+i] = (Fw8u) D;
			}
		  }
#endif
		break;

	  case FW_CHROMA_VERT:

		/*
		 * Vertical prediction
		 */
//printf("\n PredictIntraChroma8x8_H264_8u_C1IR------: FW_CHROMA_VERT: T=%d", block_available_up); 

//		if (!(block_available_left)) return fwStsLPCCalcErr;
		if (!(block_available_up)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;
		if( Dispatch_Type==DT_SSE2)
		{
			xmm7 = _mm_loadl_epi64( (__m128i*)(recoPic-srcdstStep) );
			for (j = 0; j < 8; j++) {
				_mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
			}
		}
		else
		{
			for (i = 0; i < 8; i++) {
				tmp = recoPic[-srcdstStep+i];
				for (j = 0; j < 8; j+=4) {
				  pSrcDst[(j+0)*srcdstStep+i] = (Fw8u) tmp;
				  pSrcDst[(j+1)*srcdstStep+i] = (Fw8u) tmp;
				  pSrcDst[(j+2)*srcdstStep+i] = (Fw8u) tmp;
				  pSrcDst[(j+3)*srcdstStep+i] = (Fw8u) tmp;
				}
			}
		}
		break;

	  case FW_CHROMA_HOR:

		/*
		 * Horizontal prediction
		 */

//		if (!(block_available_up)) return fwStsLPCCalcErr;
		if (!(block_available_left)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;
		if( Dispatch_Type==DT_SSE2)
		{
			recoPic = &pSrcDst[-1];
			for (j = 0; j < 8; j++) {
			  tmp = *recoPic;
			  recoPic += srcdstStep;
	  		  xmm7  = _mm_set1_epi8((Fw8u)tmp); 
			  _mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
			}
		}
		else
		{
			for (j = 0; j < 8; j++) {
				tmp = recoPic[-1+j*srcdstStep];
				for (i = 0; i < 8; i+=4) {
				  pSrcDst[j*srcdstStep+i+0] = (Fw8u) tmp;
				  pSrcDst[j*srcdstStep+i+1] = (Fw8u) tmp;
				  pSrcDst[j*srcdstStep+i+2] = (Fw8u) tmp;
				  pSrcDst[j*srcdstStep+i+3] = (Fw8u) tmp;
				}
			}
		}
		break;

	  case FW_CHROMA_PLANE:

		/*
		 * Plane Prediction
		 */


		if (!(block_available_up) || !(block_available_left) || !(block_available_up_left)) 
			return fwStsLPCCalcErr;

		recoPic = pSrcDst;

		H = V = 0;
/*
		for (i=1; i<=4; i++) {
			H += i*(recoPic[4+i] - recoPic[4-i]);
			V += i*(recoPic[(4+i)*srcdstStep] - recoPic[(4-i)*srcdstStep]);
		}
*/
		for( i = 0; i < 4; i++ )
		{
			H += (i+1) * ( pSrcDst[4+i-srcdstStep] -      pSrcDst[2-i-srcdstStep] );
			V += (i+1) * ( pSrcDst[-1+(i+4)*srcdstStep] - pSrcDst[-1+(2-i)*srcdstStep] );
		}

/*
		a = 16*(recoPic[8*srcdstStep] + recoPic[8]);
		b = (int)((17*((Fw32s)H)+16)>>5);
		c = (int)((17*((Fw32s)V)+16)>>5);
		tmp = a + c*(0-3) + 16;
*/
		a = 16 * ( pSrcDst[-1+7*srcdstStep] + pSrcDst[7-srcdstStep] );
		b = ( 17*H + 16 ) >> 5;
		c = ( 17*V + 16 ) >> 5;
		tmp = a -3*b -3*c + 16;
/*
		for (j = 0; j < 8; j++, tmp+=c) {
			tmp2 = tmp + b*(0-3);
			for (i = 0; i < 8; i++, tmp2+=b) {
			  pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,tmp2>>5);
			}        
		}
*/
		if( Dispatch_Type==DT_SSE2)
		{
			SYS_FORCEALIGN_16 const Fw16s c_b[8] = {0, 1, 2, 3, 4, 5, 6, 7};
			{
				xmm6  = _mm_load_si128( (__m128i*)(c_b) );	//[0, 7]
				xmm5  = _mm_set1_epi16((Fw16s)b);	// b
				xmm4  = _mm_set1_epi16((Fw16s)c);	// c
				xmm3  = _mm_set1_epi16((Fw16s)tmp);	// tmp
				xmm7  =	_mm_set1_epi16(0);				// 0

				xmm5  = _mm_mullo_epi16(xmm5, xmm6); //b*[0, 7]
				xmm3  =	_mm_add_epi16(xmm3, xmm5);	//tmp+b*[0, 7]

/*
			for(j=0; j<8; j++, tmp+=c)			{
				for(i=0; i<8; i++ )				{
					int pix = (tmp+b*i) >> 5;
					pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,pix);
				}
			}
*/
				for (j = 0; j < 8; j++) {
					xmm1 = xmm3;
					xmm1 = _mm_srai_epi16(xmm1, 5);		//>>5
					xmm1 = _mm_unpacklo_epi64(xmm1, xmm7);	//clip(0,255,tmp2>>5);
				    _mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm1 ); // store 8 bytes

					xmm3  =	_mm_add_epi16(xmm3, xmm4);	//+c
				}
			}
		}
		else
		{
			for(j=0; j<8; j++, tmp+=c)
			{
				for(i=0; i<8; i++ )
				{
					int pix;
					pix = (tmp+b*i) >> 5;
					pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,pix);
				}
			}
		}
		break;
	  default:
		  break;
	  }
	return fwStsNoErr;
}

#else

FwStatus PREFIX_OPT(OPT_PREFIX, fwiPredictIntraChroma8x8_H264_8u_C1IR)(Fw8u* pSrcDst, 
												 Fw32s srcdstStep, 
												 FwIntraChromaPredMode_H264 predMode, 
												 Fw32s availability)
{
	if(pSrcDst == NULL) return fwStsNullPtrErr;
	if(srcdstStep < 4) return fwStsStepErr;
	if(predMode < 0 || predMode > 3) return fwStsOutOfRangeErr;

	int block_available_up=0;
	int block_available_left=0;
	int block_available_up_left=0;

	if(FW_LEFT		  & availability) block_available_left = 1;
	if(FW_UPPER_LEFT & availability) block_available_up_left = 1;
	if(FW_UPPER	  & availability) block_available_up = 1;

//	int comp;
	Fw8u *recoPic;
	int S0, S1, S2, S3;
	int A, B, C, D;
	int H, V, a, b, c;
	int i, j;
	int tmp, tmp2;

	switch (predMode) {

	  case FW_CHROMA_DC:

		  recoPic = pSrcDst;
      	  S0 = S1 = S2 = S3 = 0;
      
		  if (block_available_up) {
			for (i = 0; i < 4; i++) {
			  S2 += recoPic[(1+i)*srcdstStep];
			  S3 += recoPic[(5+i)*srcdstStep];
			}
		  }
		  if (block_available_left) {
			for (i = 0; i < 4; i++) {
			  S0 += recoPic[1+i];
			  S1 += recoPic[5+i];
			}
		  }

		  if (block_available_up && block_available_left) {
			A = (S0 + S2 + 4)>>3;
			B = (S1 + 2)>>2;
			C = (S3 + 2)>>2;
			D = (S1 + S3 + 4)>>3;
		  }
		  else if (block_available_up) {
			A = B = (S2 + 2)>>2;
			C = D = (S3 + 2)>>2;
		  }
		  else if (block_available_left) {
			A = C = (S0 + 2)>>2;
			B = D = (S1 + 2)>>2;
		  }
		  else
			A = B = C = D = 128;
      
		  for (j = 0; j < 4; j++) {
			for (i = 0; i < 4; i++) {
			  pSrcDst[j  *srcdstStep+i  ] = (Fw8u) A;
			  pSrcDst[j  *srcdstStep+4+i] = (Fw8u) B;
			  pSrcDst[(4+j)*srcdstStep+i  ] = (Fw8u) C;
			  pSrcDst[(4+j)*srcdstStep+4+i] = (Fw8u) D;
			}
		  }
		break;

	  case FW_CHROMA_VERT:

		/*
		 * Vertical prediction
		 */

		if (!(block_available_left)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;

		for (i = 0; i < 8; i++) {
			tmp = recoPic[1+i];
			for (j = 0; j < 8; j+=4) {
			  pSrcDst[(j+0)*srcdstStep+i] = (Fw8u) tmp;
			  pSrcDst[(j+1)*srcdstStep+i] = (Fw8u) tmp;
			  pSrcDst[(j+2)*srcdstStep+i] = (Fw8u) tmp;
			  pSrcDst[(j+3)*srcdstStep+i] = (Fw8u) tmp;
			}
		}

		break;

	  case FW_CHROMA_HOR:

		/*
		 * Horizontal prediction
		 */

		if (!(block_available_up)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;

		for (j = 0; j < 8; j++) {
			tmp = recoPic[(1+j)*srcdstStep];
			for (i = 0; i < 8; i+=4) {
			  pSrcDst[j*srcdstStep+i+0] = (Fw8u) tmp;
			  pSrcDst[j*srcdstStep+i+1] = (Fw8u) tmp;
			  pSrcDst[j*srcdstStep+i+2] = (Fw8u) tmp;
			  pSrcDst[j*srcdstStep+i+3] = (Fw8u) tmp;
			}
		}

		break;

	  case FW_CHROMA_PLANE:

		/*
		 * Plane Prediction
		 */

		if (!(block_available_up) || !(block_available_left) || !(block_available_up_left)) 
			return fwStsLPCCalcErr;

		recoPic = pSrcDst;

		for (H = V = 0, i = 1; i <= 4; i++) {
			H += i*(recoPic[4+i] - recoPic[4-i]);
			V += i*(recoPic[(4+i)*srcdstStep] - recoPic[(4-i)*srcdstStep]);
		}

		a = 16*(recoPic[8*srcdstStep] + recoPic[8]);
		b = (int)((17*((Fw32s)H)+16)>>5);
		c = (int)((17*((Fw32s)V)+16)>>5);

		tmp = a + c*(0-3) + 16;
		for (j = 0; j < 8; j++, tmp+=c) {
			tmp2 = tmp + b*(0-3);
			for (i = 0; i < 8; i++, tmp2+=b) {
			  pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,tmp2>>5);
			}        
		}

		break;
	  }

	return fwStsNoErr;

/*
Return Values
fwStsNoErr Indicates no error.
fwStsNullPtrErr Indicates an error condition if at least one of the specified pointers is
NULL.
fwStsOutOfRangeErr Indicates an error condition if predMode value falls outside [0,3].
fwStsStepErr Indicates an error condition if srcdstStep value is less than 8.
fwStsLPCCalcErr Indicates an error condition if predMode is not allowed for this block.
*/
}

#endif

#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
