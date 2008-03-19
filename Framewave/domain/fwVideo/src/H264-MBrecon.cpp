/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.

This implementation is based on ITU-H.264 doc 3/2005, 
JVT reference code JM13.2 and N264 (Nokia H.264/AVC codec for 3GPP) and are subject to the license terms below.
*/

/*
Nokia N264 code subject to the following:

COPYRIGHT, LICENSE AND WARRANTY INFORMATION

This software module has been originally developed by Nokia Corporation. 

Provided that a person, entity or a company willing to use the Software 
(hereinafter Licensee) comply with all the terms and conditions of this 
Statement and subject to the limitations set forth in this Statement Nokia 
grants to such Licensee a non-exclusive, sub-licensable, worldwide, 
limited license under copyrights owned by Nokia to use the Software for 
the sole purpose of creating, manufacturing, selling, marketing, or  
distributing (including the right to make modifications to the Software) 
a fully compliant decoder implementation (hereinafter "Decoder") of 
ITU-T Recommendation H.264 / ISO/IEC International Standard 14496-10 and 
an encoder implementation producing output that is decodable with the 
Decoder.

Nokia retains the ownership of copyrights to the Software. There is no 
patent nor other intellectual property right of Nokia licensed under this 
Statement (except the copyright license above). Licensee hereby assumes 
sole responsibility to secure any other intellectual property rights 
needed, if any. For example, if patent licenses  are required, it is 
their responsibility to acquire the license before utilizing the Software.

The license by Nokia is subject to that the Licensee grants to Nokia the 
non-exclusive, worldwide, royalty-free, perpetual and irrevocable 
covenant that the Licensee(s) shall not bring a suit before any court or 
administrative agency or otherwise assert a claim for infringement under 
the Licensee intellectual property rights that, but for a license, would 
be infringed by the Software against  
   (a)  Nokia or Nokia's Affiliate; or  
   (b)  other recipient of a license and covenant not to sue with respect 
        to the Software from Nokia; or 
   (c)  contractor, customer or distributor of a party listed above in a 
        or b,  
which suit or claim is related to the Software or use thereof.

The Licensee(s) further agrees to grant a reciprocal license to Nokia 
(as granted by Nokia to the Licensee(s) on the modifications made by 
Licensee(s) to the Software. 

THE SOFTWARE IS PROVIDED "AS IS" AND THE ORIGINAL DEVELOPER DISCLAIMS 
ANY AND ALL WARRANTIES WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING 
BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. THOSE INTENDING TO USE 
THE SOFTWARE ARE EXPRESSLY ADVISED THAT ITS USE MAY INFRINGE EXISTING 
PATENTS AND BE SUBJECT TO ROYALTY PAYMENTS TO PATENT OWNERS. ANYONE 
USING THE SOFTWARE ON THE BASIS OF THIS LICENSE AGREES TO OBTAIN THE 
NECESSARY PERMISSIONS FROM ANY AND ALL APPLICABLE PATENT OWNERS FOR SUCH 
USE.

IN NO EVENT SHALL THE ORIGINAL DEVELOPER BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT 
OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This copyright, license and warranty information notice must be retained 
in all copies and derivative works of the Software or substantial 
portions thereof.
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

#define D_CBP_1ST_LUMA_AC_BITPOS 1
#define COEF_POS_EOB 0x20			// end of block flag
#define COEF_POS_16BIT_COEF 0x10	// next coefficient is 16 bits flag
#define CLAMP_BIAS  256 // Bias in clamping table
#define CLIP_RANGE  (CLAMP_BIAS + 256 + CLAMP_BIAS)
#define ClampVal(x)  (ClampTbl[CLAMP_BIAS + (x)])
#define ClampTblLookup(x, y) ClampVal((x) + (y))

extern const Fw8u above_right_avail_4x4[16];

extern SYS_FORCEALIGN_16  const Fw8u ClampTbl[CLIP_RANGE];

#define         FWVC_CBP_1ST_CHROMA_DC_BITPOS 17
#define         FWVC_CBP_1ST_CHROMA_AC_BITPOS 19
#define         FWVC_CBP_CHROMA_DC (0x3<<FWVC_CBP_1ST_CHROMA_DC_BITPOS)
#define         FWVC_CBP_CHROMA_AC (0xff<<FWVC_CBP_1ST_CHROMA_AC_BITPOS)
#define         FWVC_CBP_LUMA_AC (0xffff<<FWVC_CBP_1ST_LUMA_AC_BITPOS)
#define         FWVC_CBP_1ST_LUMA_AC_BITPOS 1
#define         FWVC_CBP_LUMA_DC 1

#undef  MAX
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))
#undef  MIN
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaInterMB_H264_16s8u_P2R)(
				Fw16s **ppSrcCoeff,
				Fw8u   *pSrcDstUPlane, 
				Fw8u   *pSrcDstVPlane, 
				const Fw32u srcDstStep, 
				const Fw32u cbp4x4, 
				const Fw32s ChromaQP)
{
	SYS_FORCEALIGN_16  Fw16s ChromaDCU[4];
	SYS_FORCEALIGN_16  Fw16s ChromaDCV[4];
	SYS_FORCEALIGN_16  Fw16s ACUplane[16];
	SYS_FORCEALIGN_16  Fw16s ACVplane[16];
	int i;
	Fw8u  *pSrcDstU = pSrcDstUPlane;
	Fw8u  *pSrcDstV = pSrcDstVPlane;
	Fw16s *pACUplane = &ACUplane[0];
	Fw16s *pACVplane = &ACVplane[0];

	Fw16s *pbs16       = (Fw16s *) *ppSrcCoeff;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcCoeff == NULL || pSrcDstUPlane == NULL || pSrcDstVPlane == NULL)
		return fwStsOutOfRangeErr;
	if( ChromaQP < 0 || ChromaQP > 39) return fwStsOutOfRangeErr;

	for(i=0;i<4;i++) ChromaDCU[i] = 0 ;
	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS))) 
	{
		Fw16s *psrc;
		psrc = pbs16;	// U point
		for(i=0;i<4;i++) ChromaDCU[i] = *pbs16++ ;
		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCU[0], ChromaQP);
	}
	for(i=0;i<4;i++) ChromaDCV[i] = 0 ;
	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS+1)))	
	{
		for(i=0;i<4;i++) ChromaDCV[i] = *pbs16++ ;
		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCV[0], ChromaQP);
	}
	// U AC block
	pSrcDstU = pSrcDstUPlane;
	for	(int BlkIdx=0; BlkIdx<4; BlkIdx++)
	{
		pACUplane = &ACUplane[0];
		pSrcDstU = pSrcDstUPlane + ( (BlkIdx/2)*4*srcDstStep + (BlkIdx%2)*4 );

		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCU[BlkIdx];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+BlkIdx)))
		{
			for(i=0;i<16;i++) ACUplane[i] = *pbs16++ ;
			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 4x4
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACUplane[i] = 0;
			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 4x4
														8, &DCCoeff, 0, ChromaQP);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstU;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACUplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstU+=srcDstStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
				pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4]+pSrcDstU[0])));
				pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+1]+pSrcDstU[1])));
				pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+2]+pSrcDstU[2])));
				pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+3]+pSrcDstU[3])));

				pSrcDstU+=srcDstStep;	
			}		
		}
	}	// for BlkIdx

	// V AC block
	pSrcDstV = pSrcDstVPlane;
	for	(int BlkIdx=0; BlkIdx<4; BlkIdx++)
	{
		pACVplane = &ACVplane[0];
		pSrcDstV = pSrcDstVPlane + ( (BlkIdx/2)*4*srcDstStep + (BlkIdx%2)*4 );
		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCV[BlkIdx];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+BlkIdx+4)))
		{
			for(i=0;i<16;i++) ACVplane[i] = *pbs16++ ;
			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 16x16
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACVplane[i] = 0;
			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 16x16
														8, &DCCoeff, 0, ChromaQP);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstV;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACVplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstV+=srcDstStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
				pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4]+pSrcDstV[0])));
				pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+1]+pSrcDstV[1])));
				pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+2]+pSrcDstV[2])));
				pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+3]+pSrcDstV[3])));

				pSrcDstV+=srcDstStep;	
			}
		}
	}	// for BlkIdx
	*ppSrcCoeff = pbs16;	// U point
	return fwStsNoErr;
}

/* 
This function is based on ITU-H.264 doc 3/2005 section 8-3 and N264 (Nokia H.264/AVC codec for 3GPP) 
*/
#define  clip(min, max, val) (((val)<(min))? (min):(((val)>(max))? (max):(val)))
FwStatus static PredictIntraChroma8x8_H264_8u_C1IR(Fw8u* pSrcDst, 
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

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm7;

	if(FW_LEFT		  & availability) block_available_left = 1;
	if(FW_UPPER_LEFT & availability) block_available_up_left = 1;
	if(FW_UPPER	  & availability) block_available_up = 1;

//	int comp;
	Fw8u *recoPic;
	int S0, S1, S2, S3;
	int A, B, C, D, H, V, a, b, c;
	int i, j, tmp; 

	switch (predMode) {

	  case FW_CHROMA_DC:	// ITU-H.264 doc 8.3.2.2.4

		  recoPic = pSrcDst;
      	  S0 = S1 = S2 = S3 = 0;

		 if(block_available_up_left==1)   block_available_left= block_available_up=1;
		  if (block_available_up) {

			for( i = 0; i < 4; i++ )
			{
				S0 += pSrcDst[i   - srcdstStep];
				S1 += pSrcDst[i+4 - srcdstStep];
			}
		  }
		  if (block_available_left) {
			for( i = 0; i < 4; i++ )
			{
				S2 += pSrcDst[i     * srcdstStep - 1];
				S3 += pSrcDst[(i+4) * srcdstStep - 1];
			}
		  }
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
      
			if( Dispatch_Type == DT_SSE2)	{
				Fw32u val = (Fw32u)0x01010101;	// duplicate 4 DC value
				Fw32u vala, valb, valc, vald;	// duplicate 4 DC value
				vala = val * (Fw8u) A;
				valb = val * (Fw8u) B;
				valc = val * (Fw8u) C;
				vald = val * (Fw8u) D;

			  for (j = 0; j < 4; j++) {
				*(Fw32u*)&pSrcDst[j*srcdstStep]		= vala;	
				*(Fw32u*)&pSrcDst[j*srcdstStep+4]		= valb;	
				*(Fw32u*)&pSrcDst[(4+j)*srcdstStep]	= valc;	
				*(Fw32u*)&pSrcDst[(4+j)*srcdstStep+4]	= vald;	
			  }
			}
			else {
			  for (j = 0; j < 4; j++) {
				for (i = 0; i < 4; i++) {
				  pSrcDst[j    *srcdstStep+i  ] = (Fw8u) A;
				  pSrcDst[j    *srcdstStep+4+i] = (Fw8u) B;
				  pSrcDst[(4+j)*srcdstStep+i  ] = (Fw8u) C;
				  pSrcDst[(4+j)*srcdstStep+4+i] = (Fw8u) D;
				}
			  }
			}
		break;

	  case FW_CHROMA_VERT:	// ITU-H.264 doc 8.3.2.2.2

		/* Vertical prediction 	 */
		if (!(block_available_up)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;
			if( Dispatch_Type == DT_SSE2)	{
				xmm7 = _mm_loadl_epi64( (__m128i*)(recoPic-srcdstStep) );
				for (j = 0; j < 8; j++) {
					_mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 8 bytes
				}
			}
			else {
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

	  case FW_CHROMA_HOR:	// ITU-H.264 doc 8.3.2.2.3

		/* Horizontal prediction */
		if (!(block_available_left)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;
		if( Dispatch_Type==DT_SSE2)
		{
			for (j = 0; j < 8; j++) {
			  tmp = recoPic[-1+j*srcdstStep];
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

	  case FW_CHROMA_PLANE:	// ITU-H.264 doc 8.3.2
		/* Plane Prediction	 */
		if (!(block_available_up) || !(block_available_left) || !(block_available_up_left)) 
			return fwStsLPCCalcErr;

		recoPic = pSrcDst;

		H = V = 0;
		for( i = 0; i < 4; i++ )
		{
			H += (i+1) * ( pSrcDst[4+i-srcdstStep] -      pSrcDst[2-i-srcdstStep] );
			V += (i+1) * ( pSrcDst[-1+(i+4)*srcdstStep] - pSrcDst[-1+(2-i)*srcdstStep] );
		}
		a = 16 * ( pSrcDst[-1+7*srcdstStep] + pSrcDst[7-srcdstStep] );
		b = ( 17*H + 16 ) >> 5;
		c = ( 17*V + 16 ) >> 5;
		tmp = a -3*b -3*c + 16;
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

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaIntraMB_H264_16s8u_P2R)(
										Fw16s **ppSrcCoeff,
										Fw8u *pSrcDstUPlane, 
										Fw8u *pSrcDstVPlane, 
										const Fw32u srcDstUVStep, 
										const FwIntraChromaPredMode_H264 intra_chroma_mode, 
										const Fw32u cbp4x4, 
										const Fw32u ChromaQP, 
										const Fw8u edge_type)
{
    FwIntraChromaPredMode_H264 intra_mode = (FwIntraChromaPredMode_H264)intra_chroma_mode;
	FwStatus  res;
    Fw32s top		= (edge_type & D_TOP_EDGE);
    Fw32s left		= (edge_type & D_LEFT_EDGE);
	Fw32s top_left = (edge_type & D_TOP_LEFT_EDGE);
	Fw32s availability = (!top) | ((!left)<<1) | ((!top_left)<<5);

	Fw16s ChromaDCU[4], ChromaDCV[4];
	Fw16s ACUplane[16], ACVplane[16];
	int i;
	Fw16s *pACUplane = &ACUplane[0];
	Fw16s *pACVplane = &ACVplane[0];
	Fw8u  *pSrcDstU = pSrcDstUPlane;
	Fw8u  *pSrcDstV = pSrcDstVPlane;

	Fw16s *pbs16       = (Fw16s *) *ppSrcCoeff;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcCoeff == NULL || pSrcDstUPlane == NULL || pSrcDstVPlane == NULL)
		return fwStsOutOfRangeErr;
	if( ChromaQP < 0 || ChromaQP > 39) return fwStsOutOfRangeErr;

	availability = 0;
	if(left == 0) availability |= FW_LEFT;
	if(top == 0)  availability |= FW_UPPER;
	if(top_left == 0) availability |= FW_UPPER_LEFT;

	res = PredictIntraChroma8x8_H264_8u_C1IR(pSrcDstUPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	res = PredictIntraChroma8x8_H264_8u_C1IR(pSrcDstVPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	for(i=0;i<4;i++) ChromaDCU[i] = 0 ;
	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS)))
	{
		for(i=0;i<4;i++) ChromaDCU[i] = *pbs16++ ;
		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCU[0], ChromaQP);
	}

	for(i=0;i<4;i++) ChromaDCV[i] = 0 ;
	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS+1)))
	{
		for(i=0;i<4;i++) ChromaDCV[i] = *pbs16++ ;
		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCV[0], ChromaQP);
	}

	// U AC block
	pSrcDstU = pSrcDstUPlane;
	for	(int BlkIdx=0; BlkIdx<4; BlkIdx++)
	{
		pACUplane = &ACUplane[0];
		pSrcDstU = pSrcDstUPlane + ( (BlkIdx/2)*4*srcDstUVStep + (BlkIdx%2)*4 );

		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCU[BlkIdx];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+BlkIdx)))
		{
			for(i=0;i<16;i++) ACUplane[i] = *pbs16++ ;
			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 4x4
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACUplane[i] = 0;
			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 4x4
														8, &DCCoeff, 0, ChromaQP);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstU;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACUplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstU+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
				pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4]  +pSrcDstU[0])));
				pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+1]+pSrcDstU[1])));
				pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+2]+pSrcDstU[2])));
				pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+3]+pSrcDstU[3])));

				pSrcDstU+=srcDstUVStep;	
			}		
		}
	}	// for BlkIdx


	// V AC block
	pSrcDstV = pSrcDstVPlane;
	for	(int BlkIdx=0; BlkIdx<4; BlkIdx++)
	{
		pACVplane = &ACVplane[0];
		pSrcDstV = pSrcDstVPlane + ( (BlkIdx/2)*4*srcDstUVStep + (BlkIdx%2)*4 );

		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCV[BlkIdx];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+BlkIdx+4)))
		{
			for(i=0;i<16;i++) ACVplane[i] = *pbs16++ ;
			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 4x4
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACVplane[i] = 0;
			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 4x4
														8, &DCCoeff, 0, ChromaQP);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstV;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACVplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstV+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
				pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4]  +pSrcDstV[0])));
				pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+1]+pSrcDstV[1])));
				pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+2]+pSrcDstV[2])));
				pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+3]+pSrcDstV[3])));

				pSrcDstV+=srcDstUVStep;	
			}
		}
	}	// for BlkIdx

	*ppSrcCoeff = pbs16;	// U point

	return fwStsNoErr;
}
const Fw8u Scan_Luma4x4[16] = {0,1,4,5,2,3,6,7,8,9,12,13,10,11,14,15};
FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaInterMB_H264_16s8u_C1R)(
									Fw16s **ppSrcCoeff, 
									Fw8u  *pSrcDstYPlane, 
									const Fw32u srcDstYStep, 
									const Fw32u cbp4x4,
									const Fw32s QP)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
//	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	Fw16s *pbs16 = (Fw16s *) *ppSrcCoeff;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int BlkIdx, i;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(BlkIdx=0; BlkIdx<16; BlkIdx++) //, uCBPMask <<= 1)
	{
		uCBPMask = (1<<(BlkIdx+1));

		if( cbp4x4 & uCBPMask )
		{
			//1.Integer inverse transformation, and shift for each 4x4 block based on Figure 6-6 of [JVTG050] in accordance 
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;

			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 8, &DCCoeff, 1, QP);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block based on 8-247 of [JVTG050].
			pSrcDstYPlane2 = pSrcDstYPlane + (Scan_Luma4x4[BlkIdx]%4)*4 + 
											 (Scan_Luma4x4[BlkIdx]/4)*4*srcDstYStep;

			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
	}
	*ppSrcCoeff = pbs16;	// U point

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntraMB_H264_16s8u_C1R)(
									Fw16s **ppSrcCoeff, 
									Fw8u  *pSrcDstYPlane, 
									Fw32s srcDstYStep, 
									const FwIntra4x4PredMode_H264 *pMBIntraTypes, 
									const Fw32u cbp4x4, 
									const Fw32u QP, 
									const Fw8u  edgeType)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	FwIntra4x4PredMode_H264 intra_mode;
    Fw32s	top, left, right, top_left, top_right;
	Fw32s     availability;
	FwStatus  res;

	Fw16s *pbs16 = (Fw16s *) *ppSrcCoeff;
	Fw32s Luma4x4_idx;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int BlkIdx, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);
	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(BlkIdx=0; BlkIdx<16; BlkIdx++, uCBPMask <<= 1)
	{
		Luma4x4_idx = Scan_Luma4x4[BlkIdx];
        intra_mode  = pMBIntraTypes[BlkIdx];

		// ITU H.264 Doc 2005 Figure 8-6 – luma4x4BlkIdx for 16x16 MacroBlock
		//			0  1  4  5
		//			2  3  6  7
		//			8  9 12 13
		//		   10 11 14 15
        top			=	(BlkIdx==0 || BlkIdx==1 || BlkIdx==4 || BlkIdx==5) && (edgeType & D_TOP_EDGE);
        left		=	(BlkIdx==0 || BlkIdx==2 || BlkIdx==8 || BlkIdx==10)&& (edgeType & D_LEFT_EDGE);
		right		=	(BlkIdx==5 || BlkIdx==7 || BlkIdx==13|| BlkIdx==15) && (edgeType & D_RIGHT_EDGE);
		top_left	=	(top & left);
		top_right	=	(top & right) | (BlkIdx==3||BlkIdx==7||BlkIdx==11||BlkIdx==13||BlkIdx==15);

		availability = (!top) | ((!left)<<1) | ((!top_left)<<5) | ((!top_right)<<6);

		pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
										 (Luma4x4_idx/4)*4*srcDstYStep;

		res = fwiPredictIntra_4x4_H264_8u_C1IR ( pSrcDstYPlane2, srcDstYStep, intra_mode,availability );

		pSrcCoeff = ppSrcCoeff[BlkIdx];
		if( cbp4x4 & uCBPMask )
		{
			//1. Integer inverse transformation, and shift for each 4x4 block according to Figure 6-6 of [JVTG050] 
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;
			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 8, &DCCoeff, 1, QP);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block based on 8-247 of [JVTG050].
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + (Luma4x4_idx/4)*4*srcDstYStep;
			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);	// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);		// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
	}
	*ppSrcCoeff = pbs16;	// U point
	return fwStsNoErr;
}

/* 
This function is based on ITU-H.264 doc 3/2005 section 8-3 and N264 (Nokia H.264/AVC codec for 3GPP) 
*/
FwStatus static PredictIntraChroma4x8_H264_8u_C1IR(Fw8u* pSrcDst, 
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
	Fw8u *recoPic;
	int S0, S1, S2, S3;
	int A, B, C, D;
	int H, V, a, b, c;
	int i, j;
	int tmp, tmp2;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm1, xmm3, xmm4, xmm5, xmm6, xmm7;

	switch (predMode) {

	  case FW_CHROMA_DC:

		  recoPic = pSrcDst;
      	  S0 = S1 = S2 = S3 = 0;
      
		  if (block_available_up) {
			for (i = 0; i < 2; i++) {
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
      	{
			Fw32u A_4 =(Fw32u)A*0x01010101;
			Fw32u B_4 =(Fw32u)B*0x01010101;
			Fw32u C_4 =(Fw32u)C*0x01010101;
			Fw32u D_4 =(Fw32u)D*0x01010101;
			*(Fw32u*)&pSrcDst[0*srcdstStep]   = A_4; 
			*(Fw32u*)&pSrcDst[0*srcdstStep+4] = B_4;
			*(Fw32u*)&pSrcDst[1*srcdstStep]   = A_4; 
			*(Fw32u*)&pSrcDst[1*srcdstStep+4] = B_4;
			*(Fw32u*)&pSrcDst[2*srcdstStep]   = C_4;
			*(Fw32u*)&pSrcDst[2*srcdstStep+4] = D_4;	
			*(Fw32u*)&pSrcDst[3*srcdstStep]   = C_4;
			*(Fw32u*)&pSrcDst[3*srcdstStep+4] = D_4;	
		}
		break;

	  case FW_CHROMA_VERT:
		/* Vertical prediction */
		if (!(block_available_left)) return fwStsLPCCalcErr;
		recoPic = pSrcDst;
		if( Dispatch_Type==DT_SSE2)
		{
			xmm7 = _mm_loadl_epi64( (__m128i*)(recoPic-srcdstStep) );
			_mm_storel_epi64( (__m128i*)(pSrcDst), xmm7 );				// store 8 bytes
			_mm_storel_epi64( (__m128i*)(pSrcDst+1*srcdstStep), xmm7 ); // store 8 bytes
			_mm_storel_epi64( (__m128i*)(pSrcDst+2*srcdstStep), xmm7 ); // store 8 bytes
			_mm_storel_epi64( (__m128i*)(pSrcDst+3*srcdstStep), xmm7 ); // store 8 bytes
		}
		else
		{
			for (i = 0; i < 8; i++) {	// correction: for (i = 0; i < 4; i++) {
				tmp = recoPic[-srcdstStep+i];  // tmp = recoPic[1+i];
				for (j = 0; j < 4; j+=4) { // for (j = 0; j < 8; j+=4) 
				  pSrcDst[(j+0)*srcdstStep+i] = (Fw8u) tmp;
				  pSrcDst[(j+1)*srcdstStep+i] = (Fw8u) tmp;
				  pSrcDst[(j+2)*srcdstStep+i] = (Fw8u) tmp;
				  pSrcDst[(j+3)*srcdstStep+i] = (Fw8u) tmp;
				}
			}
		}

		break;

	  case FW_CHROMA_HOR:
		/* Horizontal prediction */
		if (!(block_available_up)) return fwStsLPCCalcErr;
		recoPic = pSrcDst;
		if( Dispatch_Type==DT_SSE2)
		{
			recoPic = &pSrcDst[-1];
			for (j = 0; j < 4; j++) {
			  tmp = *recoPic;
			  recoPic += srcdstStep;
	  		  xmm7  = _mm_set1_epi8((Fw8u)tmp); 
			  _mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
			}
		}
		else
		{
			for (j = 0; j < 4; j++) {
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
		/*	Plane Prediction */
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
				for (j = 0; j < 4; j++) {
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
			for (j = 0; j < 4; j++, tmp+=c) {
				tmp2 = tmp + b*(0-3);
				for (i = 0; i < 8; i++, tmp2+=b) {
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
fwStsOutOfRangeErr Indicates an error condition if predMode value falls outside [0,3].
fwStsStepErr Indicates an error condition if srcdstStep value is less than 8.
fwStsLPCCalcErr Indicates an error condition if predMode is not allowed for this block.
*/
}

FwStatus static TransformDequantChromaDC_idc_1(Fw16s* pSrcDst, Fw32s QP, Fw16s LevelScale)
{
	int qp_per = QP/6;

	Fw16s tmp0=*(pSrcDst);
	Fw16s tmp1=*(pSrcDst+1);
	Fw16s tmp2=*(pSrcDst+2);
	Fw16s tmp3=*(pSrcDst+3);

	pSrcDst[0]=(tmp0+tmp1+tmp2+tmp3);
    pSrcDst[1]=(tmp0-tmp1+tmp2-tmp3);
    pSrcDst[2]=(tmp0+tmp1-tmp2-tmp3);
    pSrcDst[3]=(tmp0-tmp1-tmp2+tmp3);

	// quantization
	//dcCij = ((fij* LevelScale(QPc%6,0,0))<<(QP'/6)) >> 5 with i,j = 0,1
	pSrcDst[0]=(Fw16s)( ( pSrcDst[0] * LevelScale)<<qp_per)>>5;
	pSrcDst[1]=(Fw16s)( ( pSrcDst[1] * LevelScale)<<qp_per)>>5;
	pSrcDst[2]=(Fw16s)( ( pSrcDst[2] * LevelScale)<<qp_per)>>5;
	pSrcDst[3]=(Fw16s)( ( pSrcDst[3] * LevelScale)<<qp_per)>>5;
	return fwStsNoErr;
}
FwStatus static DequantTransformResidual_idc_1(Fw16s* pSrcDst, Fw32s  QP, Fw16s *LevelScale)
{
	int qp_per = (QP)/6;
	int i;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm2, xmm4, xmm3, xmm5;

	if( QP < 24)
	{
		int qp_add = 1<<(3-qp_per);
		int qp_shft = 4-qp_per;
        __m128i qp_shft_x = _mm_cvtsi32_si128(qp_shft);
		Fw16s pSrcDst0 = pSrcDst[0];

		if( Dispatch_Type==DT_SSE2)
		{
				xmm0 = _mm_load_si128( (__m128i*)pSrcDst );
				xmm1 = _mm_load_si128( (__m128i*)LevelScale );
				xmm2  = _mm_set1_epi16((Fw16s)qp_add);	// qp_add
				xmm0  = _mm_mullo_epi16(xmm0, xmm1); // pSrcDst[i]*LevelScale[i]
				xmm0  =	_mm_add_epi16(xmm0, xmm2);	//  pSrcDst[i]*LevelScale[i] +qp_add
                xmm0 = _mm_sra_epi16(xmm0, qp_shft_x);
				_mm_store_si128( (__m128i*)(pSrcDst), xmm0); // store 8 short

				xmm3 = _mm_load_si128( (__m128i*)pSrcDst+8 );
				xmm4 = _mm_load_si128( (__m128i*)LevelScale+8 );
				xmm3  = _mm_mullo_epi16(xmm3, xmm4); // pSrcDst[i]*LevelScale[i]
				xmm3  =	_mm_add_epi16(xmm3, xmm2);	//  pSrcDst[i]*LevelScale[i] +qp_add
				xmm3 = _mm_sra_epi16(xmm3, qp_shft_x);
				_mm_store_si128( (__m128i*)(pSrcDst+8), xmm3); // store 8 short
		}
		else
		{
			for(int i=1;i<16;i++)
				pSrcDst[i]=(Fw16s)( (pSrcDst[i]*LevelScale[i]+qp_add ) >> qp_shft);
		}

		pSrcDst[0] = pSrcDst0;
	}
	else
	{
		int qp_shft = qp_per-4;
        __m128i qp_shft_x = _mm_cvtsi32_si128(qp_shft);
		Fw16s pSrcDst0 = pSrcDst[0];
		if( Dispatch_Type==DT_SSE2)
		{
				xmm0 = _mm_load_si128( (__m128i*)pSrcDst );
				xmm1 = _mm_load_si128( (__m128i*)LevelScale );
				xmm0  = _mm_mullo_epi16(xmm0, xmm1); // pSrcDst[i]*LevelScale[i]
				xmm0 = _mm_sll_epi16(xmm0, qp_shft_x);
				_mm_store_si128( (__m128i*)(pSrcDst), xmm0); // store 8 short

				xmm3 = _mm_load_si128( (__m128i*)pSrcDst+8 );
				xmm4 = _mm_load_si128( (__m128i*)LevelScale+8 );
				xmm3  = _mm_mullo_epi16(xmm3, xmm4); // pSrcDst[i]*LevelScale[i]
				xmm3 = _mm_sll_epi16(xmm3, qp_shft_x);
				_mm_store_si128( (__m128i*)(pSrcDst+8), xmm3); // store 8 short
		}
		else
		{
			for(int i=1;i<16;i++)
				pSrcDst[i]=(Fw16s)( (pSrcDst[i]*LevelScale[i]) << qp_shft);
		}
		pSrcDst[0] = pSrcDst0;
	}

	if( Dispatch_Type==DT_SSE2)	
	{
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

		xmm3 =	_mm_add_epi16(xmm3, xmm0);			// xmm3 = [r1+r3>>1|r0+r2] = [s3 s0]
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);			// xmm1 = [r1>>1-r3|r0-r2] = [s2 s1]
		xmm0 = xmm3;
		xmm4 = xmm1;

		xmm3 = xmm0;
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		xmm2 = xmm0;

		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			

	// Transpose
		xmm5 = xmm0;
		xmm2 = _mm_shuffle_epi32(xmm2, 0x4e);
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);
		xmm5 = _mm_unpackhi_epi16(xmm5, xmm2);
		xmm1 = xmm0;
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm5);
		xmm1 = _mm_unpackhi_epi16(xmm1, xmm5);

	// vertical
		xmm2 = xmm1;

		xmm4 = xmm0;
		xmm5 = xmm2;
		xmm1 = xmm0;
		xmm3 = xmm2;
		xmm4 = _mm_srai_epi16(xmm4, 1);		
		xmm5 = _mm_srai_epi16(xmm5, 1);	
		xmm4 = _mm_srli_si128(xmm4, 8);	
		xmm5 = _mm_srli_si128(xmm5, 8);		
		xmm1 = _mm_unpacklo_epi64(xmm1, xmm4);	
		xmm3 = _mm_unpacklo_epi64(xmm3, xmm5);	

		xmm3 =	_mm_add_epi16(xmm3, xmm0);	
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);	
		xmm0 = xmm3;
		xmm4 = xmm1;

		xmm3 = xmm0;
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		xmm2 = xmm0;
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			
		xmm4 = _mm_shuffle_epi32(xmm2, 0x4e);
		xmm5  = _mm_set1_epi16((short)32);    
		xmm0 =	_mm_add_epi16(xmm0, xmm5);			
		xmm4 =	_mm_add_epi16(xmm4, xmm5);			
		xmm0 = _mm_srai_epi16(xmm0, 6);		
		xmm4 = _mm_srai_epi16(xmm4, 6);		
		_mm_store_si128( (__m128i*)pSrcDst, xmm0 ); // store 16 bytes
		_mm_store_si128( (__m128i*)(pSrcDst+8), xmm4 ); // store 16 bytes
	}
	else
	{
		Fw16s s[4];
		for (i = 0; i < 4; i ++)
		{
			s[0] = pSrcDst[i*4 + 0] + pSrcDst[i*4 + 2];
			s[1] = pSrcDst[i*4 + 0] - pSrcDst[i*4 + 2];
			s[2] =(pSrcDst[i*4 + 1] >> 1) - pSrcDst[i*4 + 3];
			s[3] = pSrcDst[i*4 + 1] + (pSrcDst[i*4 + 3] >> 1);

			pSrcDst[i*4 + 0] = s[0] + s[3];
			pSrcDst[i*4 + 3] = s[0] - s[3];
			pSrcDst[i*4 + 1] = s[1] + s[2];
			pSrcDst[i*4 + 2] = s[1] - s[2];
		}

		for (i = 0; i < 4; i ++)
		{
			s[0] = pSrcDst[0*4 + i] + pSrcDst[2*4 + i];
			s[1] = pSrcDst[0*4 + i] - pSrcDst[2*4 + i];
			s[2] =(pSrcDst[1*4 + i] >> 1) - pSrcDst[3*4 + i];
			s[3] = pSrcDst[1*4 + i] + (pSrcDst[3*4 + i] >> 1);

			pSrcDst[0*4 + i] = (s[0] + s[3] + 32) >> 6;
			pSrcDst[3*4 + i] = (s[0] - s[3] + 32) >> 6;
			pSrcDst[1*4 + i] = (s[1] + s[2] + 32) >> 6;
			pSrcDst[2*4 + i] = (s[1] - s[2] + 32) >> 6;
		}
	}
	return fwStsNoErr;
}

typedef enum
{
    FW_8x8_VERT     = 0,
    FW_8x8_HOR      = 1,
    FW_8x8_DC       = 2,
    FW_8x8_DIAG_DL  = 3,
    FW_8x8_DIAG_DR  = 4,
    FW_8x8_VR       = 5,
    FW_8x8_HD       = 6,
    FW_8x8_VL       = 7,
    FW_8x8_HU       = 8

} fwintra8x8PredMode_H264;

/*
* This function is based on JVT reference code JM13.2, transform8x8.c
*/
 // Predictor array index definitions
#define P_Z (PredPel[0])
#define P_A (PredPel[1])
#define P_B (PredPel[2])
#define P_C (PredPel[3])
#define P_D (PredPel[4])
#define P_E (PredPel[5])
#define P_F (PredPel[6])
#define P_G (PredPel[7])
#define P_H (PredPel[8])
#define P_I (PredPel[9])
#define P_J (PredPel[10])
#define P_K (PredPel[11])
#define P_L (PredPel[12])
#define P_M (PredPel[13])
#define P_N (PredPel[14])
#define P_O (PredPel[15])
#define P_P (PredPel[16])
#define P_Q (PredPel[17])
#define P_R (PredPel[18])
#define P_S (PredPel[19])
#define P_T (PredPel[20])
#define P_U (PredPel[21])
#define P_V (PredPel[22])
#define P_W (PredPel[23])
#define P_X (PredPel[24])
void static LowPassForIntra8x8Pred(int *PredPel, int block_up_left, int block_up, int block_left)
{
  int i;
  int LoopArray[25];
 

  for(i = 0; i < 25; i++)
     LoopArray[i] = PredPel[i] ;

 	if(block_up)
	{
		if(block_up_left) 
		{
			LoopArray[1] = ((&P_Z)[0] + ((&P_Z)[1]<<1) + (&P_Z)[2] + 2)>>2;
		}
		else
			LoopArray[1] = ((&P_Z)[1] + ((&P_Z)[1]<<1) + (&P_Z)[2] + 2)>>2; 


		for(i = 2; i <16; i++)
		{
			LoopArray[i] = ((&P_Z)[i-1] + ((&P_Z)[i]<<1) + (&P_Z)[i+1] + 2)>>2;
		}
		LoopArray[16] = (P_P + (P_P<<1) + P_O + 2)>>2;
	}

	if(block_up_left) 
	{
		
		if(block_up && block_left)
		{
				LoopArray[0] = (P_Q + (P_Z<<1) + P_A +2)>>2;
		}
		else
		{
			if(block_up)
				LoopArray[0] = (P_Z + (P_Z<<1) + P_A +2)>>2;
			else
				if(block_left)
					LoopArray[0] = (P_Z + (P_Z<<1) + P_Q +2)>>2;
		}

	}

	if(block_left)
	{
		if(block_up_left)
			LoopArray[17] = (P_Z + (P_Q<<1) + P_R + 2)>>2; 
		else
			LoopArray[17] = (P_Q + (P_Q<<1) + P_R + 2)>>2;

		for(i = 18; i <24; i++)
		{
			LoopArray[i] = ((&P_Z)[i-1] + ((&P_Z)[i]<<1) + (&P_Z)[i+1] + 2)>>2;
		}
		LoopArray[24] = (P_W + (P_X<<1) + P_X + 2)>>2;
	}

  for(i = 0; i < 25; i++)
    PredPel[i] = LoopArray[i];
}

#define PRED8x8_LOWPASS(m1, m2, m3, m4, m5)	\
{	\
    m5 = m2;\
	m2 = _mm_avg_epu8(m2, m3);\
	m3 = _mm_xor_si128 (m3, m5);\
    m1 = m4;\
	m3 = _mm_and_si128(m3, _mm_set1_epi8(1) );\
	m2 = _mm_subs_epu8(m2, m3);\
	m1 = _mm_avg_epu8(m1, m2);\
}

/*
 * This function is based on ITU-H.264 doc 3/2005 section 8-3 and JVT reference code JM13.2 
 */
FwStatus static iPredictIntra_8x8_H264_8u_C1IR(Fw8u* pSrcDst, 
											Fw32s srcdstStep,
											fwintra8x8PredMode_H264 predMode, 
											Fw32s availability)
{
	if(pSrcDst == NULL) return fwStsNullPtrErr;
	if(srcdstStep < 4) return fwStsStepErr;
	if(predMode < 0 || predMode > 8) return fwStsOutOfRangeErr;

  int i,j;
  int s0;
  SYS_FORCEALIGN_16  int PredPel[25];  // array of predictor pels

  int block_available_up=0;
  int block_available_left=0;
  int block_available_up_left=0;
  int block_available_up_right=0;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm7;

	if(FW_LEFT & availability)			block_available_left = 1;
	if(FW_UPPER_LEFT & availability)	block_available_up_left = 1;
	if(FW_UPPER & availability)		block_available_up = 1;
	if(FW_UPPER_RIGHT & availability)	block_available_up_right = 1;


	// ITU-H.264 doc 8.3.2.2
  if (block_available_up)
  {
    P_A = pSrcDst[1];
    P_B = pSrcDst[2];
    P_C = pSrcDst[3];
    P_D = pSrcDst[4];
    P_E = pSrcDst[5];
    P_F = pSrcDst[6];
    P_G = pSrcDst[7];
    P_H = pSrcDst[8];
  }
  else
  {
    P_A = P_B = P_C = P_D = P_E = P_F = P_G = P_H = 128;
  }

  if (block_available_up_right)
  {
    P_I = pSrcDst[9+1];
    P_J = pSrcDst[9+2];
    P_K = pSrcDst[9+3];
    P_L = pSrcDst[9+4];
    P_M = pSrcDst[9+5];
    P_N = pSrcDst[9+6];
    P_O = pSrcDst[9+7];
    P_P = pSrcDst[9+8];

  }
  else
  {
    P_I = P_J = P_K = P_L = P_M = P_N = P_O = P_P = P_H;
  }

  if (block_available_left)
  {
    P_Q = pSrcDst[srcdstStep*1];
    P_R = pSrcDst[srcdstStep*2];
    P_S = pSrcDst[srcdstStep*3];
    P_T = pSrcDst[srcdstStep*4];
    P_U = pSrcDst[srcdstStep*5];
    P_V = pSrcDst[srcdstStep*6];
    P_W = pSrcDst[srcdstStep*7];
    P_X = pSrcDst[srcdstStep*8];
  }
  else
  {
    P_Q = P_R = P_S = P_T = P_U = P_V = P_W = P_X = 128;
  }

  if (block_available_up_left)
  {
    P_Z = pSrcDst[0];
  }
  else
  {
    P_Z = 128;
  }
  
  LowPassForIntra8x8Pred(&(P_Z), block_available_up_left, block_available_up, block_available_left);

//pSrcDst[x][y]
  switch(predMode)
  {
  case FW_8x8_DC:
    s0 = 0;
    if (block_available_up && block_available_left)
    {   
      // no edge
      s0 = (P_A + P_B + P_C + P_D + P_E + P_F + P_G + P_H + P_Q + P_R + P_S + P_T + P_U + P_V + P_W + P_X + 8) >> 4;
    }
    else if (!block_available_up && block_available_left)
    {
      // upper edge
      s0 = (P_Q + P_R + P_S + P_T + P_U + P_V + P_W + P_X + 4) >> 3;             
    }
    else if (block_available_up && !block_available_left)
    {
      // left edge
      s0 = (P_A + P_B + P_C + P_D + P_E + P_F + P_G + P_H + 4) >> 3;             
    }
    else //if (!block_available_up && !block_available_left)
    {
      // top left corner, nothing to predict from
      s0 = 128; //img->dc_pred_value;                           
    }

	if( Dispatch_Type==DT_SSE2)
	{
		xmm7  = _mm_set1_epi8((Fw8u)s0); 
		for (j = 0; j < 8; j++) {
			_mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
		}
	}
	else 
	{
		for(i=0; i <8; i++)
		  for(j=0; j <8; j++)
			pSrcDst[i*srcdstStep+ j] = (Fw8u) s0;// pSrcDst[i*srcdstStep + j] = s0;
	}
    break;

  case FW_8x8_VERT:

	if( Dispatch_Type==DT_SSE2)
	{
		xmm7 = _mm_loadl_epi64( (__m128i*)(&PredPel[1]) );
		for (j = 0; j < 8; j++) {
			_mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
		}
	}
	else 
	{
		for (i=0; i < 8; i++) //    for (i=0; i < 2*BLOCK_SIZE; i++)
		{
		  pSrcDst[i*srcdstStep + 0] = 
		  pSrcDst[i*srcdstStep + 1] = 
		  pSrcDst[i*srcdstStep + 2] = 
		  pSrcDst[i*srcdstStep + 3] = 
		  pSrcDst[i*srcdstStep + 4] = 
		  pSrcDst[i*srcdstStep + 5] = 
		  pSrcDst[i*srcdstStep + 6] = 
		  pSrcDst[i*srcdstStep + 7] = (Fw8u) (&P_A)[i];
		}
	}
    break;
  case FW_8x8_HOR:
	if( Dispatch_Type==DT_SSE2)
	{
		Fw8u tmp;
		for (j = 0; j < 8; j++) {
		  tmp = (Fw8u) (&P_A)[j];
  		  xmm7  = _mm_set1_epi8((Fw8u)tmp); 
		  _mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
		}
	}
	else
	{
		for (j=0; j < 8; j++)
		{
		  pSrcDst[0*srcdstStep + j]  = 
		  pSrcDst[1*srcdstStep + j]  = 
		  pSrcDst[2*srcdstStep + j]  = 
		  pSrcDst[3*srcdstStep + j]  = 
		  pSrcDst[4*srcdstStep + j]  = 
		  pSrcDst[5*srcdstStep + j]  = 
		  pSrcDst[6*srcdstStep + j]  = 
		  pSrcDst[7*srcdstStep + j]  = (Fw8u) (&P_Q)[j];
		}
	}
    break;

  case FW_8x8_DIAG_DL:
    // Mode DIAG_DOWN_LEFT_PRED
	if( Dispatch_Type==DT_SSE2)
	{	
		xmm3 = _mm_loadu_si128 ((__m128i*)(&PredPel[1])  );
		xmm2 = _mm_loadu_si128 ((__m128i*)(&PredPel[2])  );
		xmm1 = xmm3;
		xmm1 = _mm_slli_si128(xmm1, 1);
		PRED8x8_LOWPASS( xmm0, xmm1, xmm2, xmm3, xmm4);

		for(i=0;i<8;i++)
		{
			xmm0 = _mm_srli_si128(xmm0, 1);
			_mm_storel_epi64( (__m128i*)(pSrcDst+i*srcdstStep), xmm0 );
		}
	}
	else
	{
		pSrcDst[0*srcdstStep + 0] = (Fw8u) ((P_A + P_C + 2*(P_B) + 2) >> 2);
		pSrcDst[0*srcdstStep + 1] = 
		pSrcDst[1*srcdstStep + 0] = (Fw8u) ((P_B + P_D + 2*(P_C) + 2) >> 2);
		pSrcDst[0*srcdstStep + 2] =
		pSrcDst[1*srcdstStep + 1] =
		pSrcDst[2*srcdstStep + 0] = (Fw8u) ((P_C + P_E + 2*(P_D) + 2) >> 2);
		pSrcDst[0*srcdstStep + 3] = 
		pSrcDst[1*srcdstStep + 2] = 
		pSrcDst[2*srcdstStep + 1] = 
		pSrcDst[3*srcdstStep + 0] = (Fw8u) ((P_D + P_F + 2*(P_E) + 2) >> 2);
		pSrcDst[0*srcdstStep + 4] = 
		pSrcDst[1*srcdstStep + 3] = 
		pSrcDst[2*srcdstStep + 2] = 
		pSrcDst[3*srcdstStep + 1] = 
		pSrcDst[4*srcdstStep + 0] = (Fw8u) ((P_E + P_G + 2*(P_F) + 2) >> 2);
		pSrcDst[0*srcdstStep + 5] = 
		pSrcDst[1*srcdstStep + 4] = 
		pSrcDst[2*srcdstStep + 3] = 
		pSrcDst[3*srcdstStep + 2] = 
		pSrcDst[4*srcdstStep + 1] = 
		pSrcDst[5*srcdstStep + 0] = (Fw8u) ((P_F + P_H + 2*(P_G) + 2) >> 2);
		pSrcDst[0*srcdstStep + 6] = 
		pSrcDst[1*srcdstStep + 5] = 
		pSrcDst[2*srcdstStep + 4] = 
		pSrcDst[3*srcdstStep + 3] = 
		pSrcDst[4*srcdstStep + 2] = 
		pSrcDst[5*srcdstStep + 1] = 
		pSrcDst[6*srcdstStep + 0] = (Fw8u) ((P_G + P_I + 2*(P_H) + 2) >> 2);
		pSrcDst[0*srcdstStep + 7] = 
		pSrcDst[1*srcdstStep + 6] = 
		pSrcDst[2*srcdstStep + 5] = 
		pSrcDst[3*srcdstStep + 4] = 
		pSrcDst[4*srcdstStep + 3] = 
		pSrcDst[5*srcdstStep + 2] = 
		pSrcDst[6*srcdstStep + 1] = 
		pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_H + P_J + 2*(P_I) + 2) >> 2);
		pSrcDst[1*srcdstStep + 7] = 
		pSrcDst[2*srcdstStep + 6] = 
		pSrcDst[3*srcdstStep + 5] = 
		pSrcDst[4*srcdstStep + 4] = 
		pSrcDst[5*srcdstStep + 3] = 
		pSrcDst[6*srcdstStep + 2] = 
		pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_I + P_K + 2*(P_J) + 2) >> 2);
		pSrcDst[2*srcdstStep + 7] = 
		pSrcDst[3*srcdstStep + 6] = 
		pSrcDst[4*srcdstStep + 5] = 
		pSrcDst[5*srcdstStep + 4] = 
		pSrcDst[6*srcdstStep + 3] = 
		pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_J + P_L + 2*(P_K) + 2) >> 2);
		pSrcDst[3*srcdstStep + 7] = 
		pSrcDst[4*srcdstStep + 6] = 
		pSrcDst[5*srcdstStep + 5] = 
		pSrcDst[6*srcdstStep + 4] = 
		pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_K + P_M + 2*(P_L) + 2) >> 2);
		pSrcDst[4*srcdstStep + 7] = 
		pSrcDst[5*srcdstStep + 6] = 
		pSrcDst[6*srcdstStep + 5] = 
		pSrcDst[7*srcdstStep + 4] = (Fw8u) ((P_L + P_N + 2*(P_M) + 2) >> 2);
		pSrcDst[5*srcdstStep + 7] = 
		pSrcDst[6*srcdstStep + 6] = 
		pSrcDst[7*srcdstStep + 5] = (Fw8u) ((P_M + P_O + 2*(P_N) + 2) >> 2);
		pSrcDst[6*srcdstStep + 7] = 
		pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_N + P_P + 2*(P_O) + 2) >> 2);
		pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_O + 3*(P_P) + 2) >> 2);
	}
    break;

  case FW_8x8_VL:
	if( Dispatch_Type==DT_SSE2)
	{	
		xmm4 = _mm_loadu_si128 ((__m128i*)(&PredPel[1])  );
		xmm2 = xmm4;
		xmm1 = xmm4;
		xmm3 = xmm4;
		xmm2 = _mm_srli_si128(xmm2, 1);
		xmm1 = _mm_slli_si128 (xmm1, 1);
		xmm3 = _mm_avg_epu8(xmm3, xmm2);
		PRED8x8_LOWPASS( xmm0, xmm1, xmm2, xmm4, xmm5);

		xmm1 = xmm0;
		_mm_unpacklo_epi8 (xmm0, xmm3);	// LO
		_mm_unpackhi_epi8 (xmm1, xmm3);	// HI

		for(i=0;i<4;i++)
		{
			_mm_storel_epi64( (__m128i*)(pSrcDst+i*srcdstStep), xmm0 );
			xmm0 = _mm_srli_si128(xmm0, 2);
		}
		for(;i<8;i++)
		{
			_mm_storel_epi64( (__m128i*)(pSrcDst+i*srcdstStep), xmm1 );
			xmm1 = _mm_srli_si128(xmm1, 2);
		}
	}
	else
	{
		pSrcDst[0*srcdstStep + 0] = (Fw8u) ((P_A + P_B + 1) >> 1);
		pSrcDst[1*srcdstStep + 0] = 
		pSrcDst[0*srcdstStep + 2] = (Fw8u) ((P_B + P_C + 1) >> 1);
		pSrcDst[2*srcdstStep + 0] = 
		pSrcDst[1*srcdstStep + 2] = 
		pSrcDst[0*srcdstStep + 4] = (Fw8u) ((P_C + P_D + 1) >> 1);
		pSrcDst[3*srcdstStep + 0] = 
		pSrcDst[2*srcdstStep + 2] = 
		pSrcDst[1*srcdstStep + 4] = 
		pSrcDst[0*srcdstStep + 6] = (Fw8u) ((P_D + P_E + 1) >> 1);
		pSrcDst[4*srcdstStep + 0] = 
		pSrcDst[3*srcdstStep + 2] = 
		pSrcDst[2*srcdstStep + 4] = 
		pSrcDst[1*srcdstStep + 6] = (Fw8u) ((P_E + P_F + 1) >> 1);
		pSrcDst[5*srcdstStep + 0] = 
		pSrcDst[4*srcdstStep + 2] = 
		pSrcDst[3*srcdstStep + 4] = 
		pSrcDst[2*srcdstStep + 6] = (Fw8u) ((P_F + P_G + 1) >> 1);
		pSrcDst[6*srcdstStep + 0] = 
		pSrcDst[5*srcdstStep + 2] = 
		pSrcDst[4*srcdstStep + 4] = 
		pSrcDst[3*srcdstStep + 6] = (Fw8u) ((P_G + P_H + 1) >> 1);
		pSrcDst[7*srcdstStep + 0] = 
		pSrcDst[6*srcdstStep + 2] = 
		pSrcDst[5*srcdstStep + 4] = 
		pSrcDst[4*srcdstStep + 6] = (Fw8u) ((P_H + P_I + 1) >> 1);
		pSrcDst[7*srcdstStep + 2] = 
		pSrcDst[6*srcdstStep + 4] = 
		pSrcDst[5*srcdstStep + 6] = (Fw8u) ((P_I + P_J + 1) >> 1);
		pSrcDst[7*srcdstStep + 4] = 
		pSrcDst[6*srcdstStep + 6] = (Fw8u) ((P_J + P_K + 1) >> 1);
		pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_K + P_L + 1) >> 1);
		pSrcDst[0*srcdstStep + 1] = (Fw8u) ((P_A + P_C + 2*P_B + 2) >> 2);
		pSrcDst[1*srcdstStep + 1] = 
		pSrcDst[0*srcdstStep + 3] = (Fw8u) ((P_B + P_D + 2*P_C + 2) >> 2);
		pSrcDst[2*srcdstStep + 1] = 
		pSrcDst[1*srcdstStep + 3] = 
		pSrcDst[0*srcdstStep + 5] = (Fw8u) ((P_C + P_E + 2*P_D + 2) >> 2);
		pSrcDst[3*srcdstStep + 1] = 
		pSrcDst[2*srcdstStep + 3] = 
		pSrcDst[1*srcdstStep + 5] = 
		pSrcDst[0*srcdstStep + 7] = (Fw8u) ((P_D + P_F + 2*P_E + 2) >> 2);
		pSrcDst[4*srcdstStep + 1] = 
		pSrcDst[3*srcdstStep + 3] = 
		pSrcDst[2*srcdstStep + 5] = 
		pSrcDst[1*srcdstStep + 7] = (Fw8u) ((P_E + P_G + 2*P_F + 2) >> 2);
		pSrcDst[5*srcdstStep + 1] = 
		pSrcDst[4*srcdstStep + 3] = 
		pSrcDst[3*srcdstStep + 5] = 
		pSrcDst[2*srcdstStep + 7] = (Fw8u) ((P_F + P_H + 2*P_G + 2) >> 2);
		pSrcDst[6*srcdstStep + 1] = 
		pSrcDst[5*srcdstStep + 3] = 
		pSrcDst[4*srcdstStep + 5] = 
		pSrcDst[3*srcdstStep + 7] = (Fw8u) ((P_G + P_I + 2*P_H + 2) >> 2);
		pSrcDst[7*srcdstStep + 1] = 
		pSrcDst[6*srcdstStep + 3] = 
		pSrcDst[5*srcdstStep + 5] = 
		pSrcDst[4*srcdstStep + 7] = (Fw8u) ((P_H + P_J + 2*P_I + 2) >> 2);
		pSrcDst[7*srcdstStep + 3] = 
		pSrcDst[6*srcdstStep + 5] = 
		pSrcDst[5*srcdstStep + 7] = (Fw8u) ((P_I + P_K + 2*P_J + 2) >> 2);
		pSrcDst[7*srcdstStep + 5] = 
		pSrcDst[6*srcdstStep + 7] = (Fw8u) ((P_J + P_L + 2*P_K + 2) >> 2);
		pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_K + P_M + 2*P_L + 2) >> 2);
	}
	break;
  case FW_8x8_DIAG_DR:
    // Mode DIAG_DOWN_RIGHT_PRED
	if( Dispatch_Type==DT_SSE2) 
	{	
	    SYS_FORCEALIGN_16  int PredPel2[25];  // array of predictor pels
		int y=0;
		for(i=8;i>=0;i--, y++) 	PredPel2[y] = PredPel[i];
		for(i=17;i<=24;i++, y++)PredPel2[y] = PredPel[i];

		xmm3 = _mm_loadu_si128 ((__m128i*)(&PredPel2[0])  );
		xmm1 = _mm_loadu_si128 ((__m128i*)(&PredPel2[1])  );
		xmm2 = xmm3;
		xmm2 = _mm_srli_si128(xmm2, 1);
		PRED8x8_LOWPASS( xmm0, xmm1, xmm2, xmm3, xmm4);
		xmm1 = xmm0;
		xmm1 = _mm_srli_si128(xmm1, 1);

		for(y=7, i=0;i<3;i++, y-=2)
		{
			_mm_storel_epi64( (__m128i*)(pSrcDst+y*srcdstStep), xmm0 );
			_mm_storel_epi64( (__m128i*)(pSrcDst+(y-1)*srcdstStep), xmm1 );
			xmm0 = _mm_srli_si128(xmm0, 2);
			xmm1 = _mm_srli_si128(xmm1, 2);
		}
		_mm_storel_epi64( (__m128i*)(pSrcDst+srcdstStep), xmm0 );
		_mm_storel_epi64( (__m128i*)(pSrcDst), xmm1 );

	}
	else
	{
		pSrcDst[0*srcdstStep + 7] = (Fw8u) ((P_X + P_V + 2*(P_W) + 2) >> 2);
		pSrcDst[0*srcdstStep + 6] = 
		pSrcDst[1*srcdstStep + 7] = (Fw8u) ((P_W + P_U + 2*(P_V) + 2) >> 2);
		pSrcDst[0*srcdstStep + 5] = 
		pSrcDst[1*srcdstStep + 6] = 
		pSrcDst[2*srcdstStep + 7] = (Fw8u) ((P_V + P_T + 2*(P_U) + 2) >> 2);
		pSrcDst[0*srcdstStep + 4] = 
		pSrcDst[1*srcdstStep + 5] = 
		pSrcDst[2*srcdstStep + 6] = 
		pSrcDst[3*srcdstStep + 7] = (Fw8u) ((P_U + P_S + 2*(P_T) + 2) >> 2);
		pSrcDst[0*srcdstStep + 3] = 
		pSrcDst[1*srcdstStep + 4] = 
		pSrcDst[2*srcdstStep + 5] = 
		pSrcDst[3*srcdstStep + 6] = 
		pSrcDst[4*srcdstStep + 7] = (Fw8u) ((P_T + P_R + 2*(P_S) + 2) >> 2);
		pSrcDst[0*srcdstStep + 2] = 
		pSrcDst[1*srcdstStep + 3] = 
		pSrcDst[2*srcdstStep + 4] = 
		pSrcDst[3*srcdstStep + 5] = 
		pSrcDst[4*srcdstStep + 6] = 
		pSrcDst[5*srcdstStep + 7] = (Fw8u) ((P_S + P_Q + 2*(P_R) + 2) >> 2);
		pSrcDst[0*srcdstStep + 1] = 
		pSrcDst[1*srcdstStep + 2] = 
		pSrcDst[2*srcdstStep + 3] = 
		pSrcDst[3*srcdstStep + 4] = 
		pSrcDst[4*srcdstStep + 5] = 
		pSrcDst[5*srcdstStep + 6] = 
		pSrcDst[6*srcdstStep + 7] = (Fw8u) ((P_R + P_Z + 2*(P_Q) + 2) >> 2);
		pSrcDst[0*srcdstStep + 0] = 
		pSrcDst[1*srcdstStep + 1] = 
		pSrcDst[2*srcdstStep + 2] = 
		pSrcDst[3*srcdstStep + 3] = 
		pSrcDst[4*srcdstStep + 4] = 
		pSrcDst[5*srcdstStep + 5] = 
		pSrcDst[6*srcdstStep + 6] = 
		pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_Q + P_A + 2*(P_Z) + 2) >> 2);
		pSrcDst[1*srcdstStep + 0] = 
		pSrcDst[2*srcdstStep + 1] = 
		pSrcDst[3*srcdstStep + 2] = 
		pSrcDst[4*srcdstStep + 3] = 
		pSrcDst[5*srcdstStep + 4] = 
		pSrcDst[6*srcdstStep + 5] = 
		pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_Z + P_B + 2*(P_A) + 2) >> 2);
		pSrcDst[2*srcdstStep + 0] = 
		pSrcDst[3*srcdstStep + 1] = 
		pSrcDst[4*srcdstStep + 2] = 
		pSrcDst[5*srcdstStep + 3] = 
		pSrcDst[6*srcdstStep + 4] = 
		pSrcDst[7*srcdstStep + 5] = (Fw8u) ((P_A + P_C + 2*(P_B) + 2) >> 2);
		pSrcDst[3*srcdstStep + 0] = 
		pSrcDst[4*srcdstStep + 1] = 
		pSrcDst[5*srcdstStep + 2] = 
		pSrcDst[6*srcdstStep + 3] = 
		pSrcDst[7*srcdstStep + 4] = (Fw8u) ((P_B + P_D + 2*(P_C) + 2) >> 2);
		pSrcDst[4*srcdstStep + 0] = 
		pSrcDst[5*srcdstStep + 1] = 
		pSrcDst[6*srcdstStep + 2] = 
		pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_C + P_E + 2*(P_D) + 2) >> 2);
		pSrcDst[5*srcdstStep + 0] = 
		pSrcDst[6*srcdstStep + 1] = 
		pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_D + P_F + 2*(P_E) + 2) >> 2);
		pSrcDst[6*srcdstStep + 0] = 
		pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_E + P_G + 2*(P_F) + 2) >> 2);
		pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_F + P_H + 2*(P_G) + 2) >> 2);
	}
    break;

  case  FW_8x8_VR:/* diagonal prediction -22.5 deg to horizontal plane */
	{
		pSrcDst[0*srcdstStep + 0] = 
		pSrcDst[1*srcdstStep + 2] = 
		pSrcDst[2*srcdstStep + 4] = 
		pSrcDst[3*srcdstStep + 6] = (Fw8u) ((P_Z + P_A + 1) >> 1);
		pSrcDst[1*srcdstStep + 0] = 
		pSrcDst[2*srcdstStep + 2] = 
		pSrcDst[3*srcdstStep + 4] = 
		pSrcDst[4*srcdstStep + 6] = (Fw8u) ((P_A + P_B + 1) >> 1);
		pSrcDst[2*srcdstStep + 0] = 
		pSrcDst[3*srcdstStep + 2] = 
		pSrcDst[4*srcdstStep + 4] = 
		pSrcDst[5*srcdstStep + 6] = (Fw8u) ((P_B + P_C + 1) >> 1);
		pSrcDst[3*srcdstStep + 0] = 
		pSrcDst[4*srcdstStep + 2] = 
		pSrcDst[5*srcdstStep + 4] = 
		pSrcDst[6*srcdstStep + 6] = (Fw8u) ((P_C + P_D + 1) >> 1);
		pSrcDst[4*srcdstStep + 0] = 
		pSrcDst[5*srcdstStep + 2] = 
		pSrcDst[6*srcdstStep + 4] = 
		pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_D + P_E + 1) >> 1);
		pSrcDst[5*srcdstStep + 0] = 
		pSrcDst[6*srcdstStep + 2] = 
		pSrcDst[7*srcdstStep + 4] = (Fw8u) ((P_E + P_F + 1) >> 1);
		pSrcDst[6*srcdstStep + 0] = 
		pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_F + P_G + 1) >> 1);
		pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_G + P_H + 1) >> 1);
		pSrcDst[0*srcdstStep + 1] = 
		pSrcDst[1*srcdstStep + 3] = 
		pSrcDst[2*srcdstStep + 5] = 
		pSrcDst[3*srcdstStep + 7] = (Fw8u) ((P_Q + P_A + 2*P_Z + 2) >> 2);
		pSrcDst[1*srcdstStep + 1] = 
		pSrcDst[2*srcdstStep + 3] = 
		pSrcDst[3*srcdstStep + 5] = 
		pSrcDst[4*srcdstStep + 7] = (Fw8u) ((P_Z + P_B + 2*P_A + 2) >> 2);
		pSrcDst[2*srcdstStep + 1] = 
		pSrcDst[3*srcdstStep + 3] = 
		pSrcDst[4*srcdstStep + 5] = 
		pSrcDst[5*srcdstStep + 7] = (Fw8u) ((P_A + P_C + 2*P_B + 2) >> 2);
		pSrcDst[3*srcdstStep + 1] = 
		pSrcDst[4*srcdstStep + 3] = 
		pSrcDst[5*srcdstStep + 5] = 
		pSrcDst[6*srcdstStep + 7] = (Fw8u) ((P_B + P_D + 2*P_C + 2) >> 2);
		pSrcDst[4*srcdstStep + 1] = 
		pSrcDst[5*srcdstStep + 3] = 
		pSrcDst[6*srcdstStep + 5] = 
		pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_C + P_E + 2*P_D + 2) >> 2);
		pSrcDst[5*srcdstStep + 1] = 
		pSrcDst[6*srcdstStep + 3] = 
		pSrcDst[7*srcdstStep + 5] = (Fw8u) ((P_D + P_F + 2*P_E + 2) >> 2);
		pSrcDst[6*srcdstStep + 1] = 
		pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_E + P_G + 2*P_F + 2) >> 2);
		pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_F + P_H + 2*P_G + 2) >> 2);
		pSrcDst[0*srcdstStep + 2] =
		pSrcDst[1*srcdstStep + 4] =
		pSrcDst[2*srcdstStep + 6] = (Fw8u) ((P_R + P_Z + 2*P_Q + 2) >> 2);
		pSrcDst[0*srcdstStep + 3] =
		pSrcDst[1*srcdstStep + 5] =
		pSrcDst[2*srcdstStep + 7] = (Fw8u) ((P_S + P_Q + 2*P_R + 2) >> 2);
		pSrcDst[0*srcdstStep + 4] =
		pSrcDst[1*srcdstStep + 6] = (Fw8u) ((P_T + P_R + 2*P_S + 2) >> 2);
		pSrcDst[0*srcdstStep + 5] =
		pSrcDst[1*srcdstStep + 7] = (Fw8u) ((P_U + P_S + 2*P_T + 2) >> 2);
		pSrcDst[0*srcdstStep + 6] = (Fw8u) ((P_V + P_T + 2*P_U + 2) >> 2);
		pSrcDst[0*srcdstStep + 7] = (Fw8u) ((P_W + P_U + 2*P_V + 2) >> 2);
	}
    break;

  case  FW_8x8_HD:/* diagonal prediction -22.5 deg to horizontal plane */
   
    pSrcDst[0*srcdstStep + 0] = 
    pSrcDst[2*srcdstStep + 1] = 
    pSrcDst[4*srcdstStep + 2] = 
    pSrcDst[6*srcdstStep + 3] = (Fw8u) ((P_Q + P_Z + 1) >> 1);
    pSrcDst[0*srcdstStep + 1] = 
    pSrcDst[2*srcdstStep + 2] = 
    pSrcDst[4*srcdstStep + 3] = 
    pSrcDst[6*srcdstStep + 4] = (Fw8u) ((P_R + P_Q + 1) >> 1);
    pSrcDst[0*srcdstStep + 2] = 
    pSrcDst[2*srcdstStep + 3] = 
    pSrcDst[4*srcdstStep + 4] = 
    pSrcDst[6*srcdstStep + 5] = (Fw8u) ((P_S + P_R + 1) >> 1);
    pSrcDst[0*srcdstStep + 3] = 
    pSrcDst[2*srcdstStep + 4] = 
    pSrcDst[4*srcdstStep + 5] = 
    pSrcDst[6*srcdstStep + 6] = (Fw8u) ((P_T + P_S + 1) >> 1);
    pSrcDst[0*srcdstStep + 4] = 
    pSrcDst[2*srcdstStep + 5] = 
    pSrcDst[4*srcdstStep + 6] = 
    pSrcDst[6*srcdstStep + 7] = (Fw8u) ((P_U + P_T + 1) >> 1);
    pSrcDst[0*srcdstStep + 5] = 
    pSrcDst[2*srcdstStep + 6] = 
    pSrcDst[4*srcdstStep + 7] = (Fw8u) ((P_V + P_U + 1) >> 1);
    pSrcDst[0*srcdstStep + 6] = 
    pSrcDst[2*srcdstStep + 7] = (Fw8u) ((P_W + P_V + 1) >> 1);
    pSrcDst[0*srcdstStep + 7] = (Fw8u) ((P_X + P_W + 1) >> 1);
    pSrcDst[1*srcdstStep + 0] =
    pSrcDst[3*srcdstStep + 1] =
    pSrcDst[5*srcdstStep + 2] =
    pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_Q + P_A + 2*P_Z + 2) >> 2);
    pSrcDst[1*srcdstStep + 1] =
    pSrcDst[3*srcdstStep + 2] =
    pSrcDst[5*srcdstStep + 3] =
    pSrcDst[7*srcdstStep + 4] = (Fw8u) ((P_Z + P_R + 2*P_Q + 2) >> 2);
    pSrcDst[1*srcdstStep + 2] =
    pSrcDst[3*srcdstStep + 3] =
    pSrcDst[5*srcdstStep + 4] =
    pSrcDst[7*srcdstStep + 5] = (Fw8u) ((P_Q + P_S + 2*P_R + 2) >> 2);
    pSrcDst[1*srcdstStep + 3] =
    pSrcDst[3*srcdstStep + 4] =
    pSrcDst[5*srcdstStep + 5] =
    pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_R + P_T + 2*P_S + 2) >> 2);
    pSrcDst[1*srcdstStep + 4] =
    pSrcDst[3*srcdstStep + 5] =
    pSrcDst[5*srcdstStep + 6] =
    pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_S + P_U + 2*P_T + 2) >> 2);
    pSrcDst[1*srcdstStep + 5] =
    pSrcDst[3*srcdstStep + 6] =
    pSrcDst[5*srcdstStep + 7] = (Fw8u) ((P_T + P_V + 2*P_U + 2) >> 2);
    pSrcDst[1*srcdstStep + 6] =
    pSrcDst[3*srcdstStep + 7] = (Fw8u) ((P_U + P_W + 2*P_V + 2) >> 2);
    pSrcDst[1*srcdstStep + 7] = (Fw8u) ((P_V + P_X + 2*P_W + 2) >> 2);
    pSrcDst[2*srcdstStep + 0] = 
    pSrcDst[4*srcdstStep + 1] = 
    pSrcDst[6*srcdstStep + 2] = (Fw8u) ((P_Z + P_B + 2*P_A + 2) >> 2);
    pSrcDst[3*srcdstStep + 0] = 
    pSrcDst[5*srcdstStep + 1] = 
    pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_A + P_C + 2*P_B + 2) >> 2);
    pSrcDst[4*srcdstStep + 0] = 
    pSrcDst[6*srcdstStep + 1] = (Fw8u) ((P_B + P_D + 2*P_C + 2) >> 2);
    pSrcDst[5*srcdstStep + 0] = 
    pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_C + P_E + 2*P_D + 2) >> 2);
    pSrcDst[6*srcdstStep + 0] = (Fw8u) ((P_D + P_F + 2*P_E + 2) >> 2);
    pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_E + P_G + 2*P_F + 2) >> 2);
    break;

  case  FW_8x8_HU:/* diagonal prediction -22.5 deg to horizontal plane */

    pSrcDst[0*srcdstStep + 0] = (Fw8u) ((P_Q + P_R + 1) >> 1);
    pSrcDst[0*srcdstStep + 1] =
    pSrcDst[2*srcdstStep + 0] = (Fw8u) ((P_R + P_S + 1) >> 1);
    pSrcDst[0*srcdstStep + 2] =
    pSrcDst[2*srcdstStep + 1] =
    pSrcDst[4*srcdstStep + 0] = (Fw8u) ((P_S + P_T + 1) >> 1);
    pSrcDst[0*srcdstStep + 3] =
    pSrcDst[2*srcdstStep + 2] =
    pSrcDst[4*srcdstStep + 1] =
    pSrcDst[6*srcdstStep + 0] = (Fw8u) ((P_T + P_U + 1) >> 1);
    pSrcDst[0*srcdstStep + 4] =
    pSrcDst[2*srcdstStep + 3] =
    pSrcDst[4*srcdstStep + 2] =
    pSrcDst[6*srcdstStep + 1] = (Fw8u) ((P_U + P_V + 1) >> 1);
    pSrcDst[0*srcdstStep + 5] =
    pSrcDst[2*srcdstStep + 4] =
    pSrcDst[4*srcdstStep + 3] =
    pSrcDst[6*srcdstStep + 2] = (Fw8u) ((P_V + P_W + 1) >> 1);
    pSrcDst[0*srcdstStep + 6] =
    pSrcDst[2*srcdstStep + 5] =
    pSrcDst[4*srcdstStep + 4] =
    pSrcDst[6*srcdstStep + 3] = (Fw8u) ((P_W + P_X + 1) >> 1);
    pSrcDst[6*srcdstStep + 4] =
    pSrcDst[7*srcdstStep + 4] =
    pSrcDst[4*srcdstStep + 5] =
    pSrcDst[5*srcdstStep + 5] =
    pSrcDst[6*srcdstStep + 5] =
    pSrcDst[7*srcdstStep + 5] =
    pSrcDst[2*srcdstStep + 6] =
    pSrcDst[3*srcdstStep + 6] =
    pSrcDst[4*srcdstStep + 6] =
    pSrcDst[5*srcdstStep + 6] =
    pSrcDst[6*srcdstStep + 6] =
    pSrcDst[7*srcdstStep + 6] =
    pSrcDst[0*srcdstStep + 7] =
    pSrcDst[1*srcdstStep + 7] =
    pSrcDst[2*srcdstStep + 7] =
    pSrcDst[3*srcdstStep + 7] =
    pSrcDst[4*srcdstStep + 7] =
    pSrcDst[5*srcdstStep + 7] =
    pSrcDst[6*srcdstStep + 7] =
    pSrcDst[7*srcdstStep + 7] = (Fw8u) P_X;
    pSrcDst[1*srcdstStep + 6] =
    pSrcDst[3*srcdstStep + 5] =
    pSrcDst[5*srcdstStep + 4] =
    pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_W + 3*P_X + 2) >> 2);
    pSrcDst[1*srcdstStep + 5] =
      pSrcDst[3*srcdstStep + 4] =
      pSrcDst[5*srcdstStep + 3] =
      pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_X + P_V + 2*P_W + 2) >> 2);
    pSrcDst[1*srcdstStep + 4] =
      pSrcDst[3*srcdstStep + 3] =
      pSrcDst[5*srcdstStep + 2] =
      pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_W + P_U + 2*P_V + 2) >> 2);
    pSrcDst[1*srcdstStep + 3] =
      pSrcDst[3*srcdstStep + 2] =
      pSrcDst[5*srcdstStep + 1] =
      pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_V + P_T + 2*P_U + 2) >> 2);
    pSrcDst[1*srcdstStep + 2] =
      pSrcDst[3*srcdstStep + 1] =
      pSrcDst[5*srcdstStep + 0] = (Fw8u) ((P_U + P_S + 2*P_T + 2) >> 2);
    pSrcDst[1*srcdstStep + 1] =
      pSrcDst[3*srcdstStep + 0] = (Fw8u) ((P_T + P_R + 2*P_S + 2) >> 2);
    pSrcDst[1*srcdstStep + 0] = (Fw8u) ((P_S + P_Q + 2*P_R + 2) >> 2);
    break;
    
  default:
    break;
  }
  return fwStsNoErr;
}



#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
