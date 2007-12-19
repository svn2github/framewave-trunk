/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __RESIZE_H
#define __RESIZE_H

#define	FW_WEIGHT	8
#define MAX_CHANNEL 4
#define	FW_WEIGHT_6 6

namespace OPT_LEVEL
{
	//Internal structure for Nearest Neighbor interpolation for unsigned short pixel value
	struct FW_Resize_16u_NN
	{
		const Fw16u *pSrc;
		int srcStep; 
		Fw16u *pDst;
		int dstStep; 
		int resizeWidth;
		int *px_NN; 
		int *py_NN;
		int yBegin;
		int yEnd;
		int xBegin;
		int xEnd;
		int channel;
	}	;

	//Internal structure for Nearest Neighbor interpolation for float pixel value
	struct FW_Resize_32f_NN
	{
		const Fw32f *pSrc;
		int srcStep; 
		Fw32f *pDst;
		int dstStep; 
		int resizeWidth;
		int *px_NN; 
		int *py_NN;
		int yBegin;
		int yEnd;
		int xBegin;
		int xEnd;
		int channel;
	};

	//Internal structure for Nearest Neighbor interpolation for unsigned char pixel value
	struct FW_Resize_8u_NN
	{
		const Fw8u *pSrc;
		int srcStep; 
		Fw8u *pDst;
		int dstStep; 
		int resizeWidth;
		int *px_NN; 
		int *py_NN;
		int yBegin;
		int yEnd;
		int xBegin;
		int xEnd;
		int channel;
	};

	//Internal structure for recording Linear interpolation with unsigned short coefficient
	struct Linear_Array {
		int floor;
		int ceil;
		Fw16u ifraction;
		Fw16u ione_Minus_Val;
	} ;

	//Internal structure for recording Linear interpolation with double coefficient
	struct Linear_Array_Double{
		int floor;
		int ceil;
		double ifraction;
		double ione_Minus_Val;
	};

	//Internal structure for recording Linear interpolation with float coefficient
	struct Linear_Array_Float{
		int floor;
		int ceil;
		float ifraction;
		float ione_Minus_Val;
	};

	//Internal structure for Linear interpolation for unsigned char pixel value
	struct FW_Resize_8u_Linear
	{
		const Fw8u *pSrc;
		Fw8u *pDst;
		int dstStep; 
		int resizeWidth;
		Linear_Array *pX_Array_Value;
		Linear_Array *pY_Array_Value; 
		//FwiSize dstRoiSize;
		int yBegin;
		int yEnd;
		int xBegin;
		int xEnd;
		Fw16u *pIfx;
		Fw16u *pIofx;
		int channel;
	};

	//Internal structure for Linear interpolation for unsigned short pixel value
	struct FW_Resize_16u_Linear
	{
		const Fw16u *pSrc;
		Fw16u *pDst;
		int dstStep; 
		int resizeWidth;
		Linear_Array_Float *pX_Array_Value;
		Linear_Array_Float *pY_Array_Value; 
		//FwiSize dstRoiSize;
		int yBegin;
		int yEnd;
		int xBegin;
		int xEnd;
		Fw32f *pIfx;
		Fw32f *pIofx;
		int channel;
	};

	//Internal structure for Linear interpolation for float pixel value
	struct FW_Resize_32f_Linear
	{
		const Fw32f *pSrc;
		Fw32f *pDst;
		int dstStep; 
		int resizeWidth;
		Linear_Array_Float *pX_Array_Value;
		Linear_Array_Float *pY_Array_Value; 
		//FwiSize dstRoiSize;
		int yBegin;
		int yEnd;
		int xBegin;
		int xEnd;
		Fw32f *pIfx;
		Fw32f *pIofx;
		int channel;
	};

	//Linear_8u
	namespace{

		static void My_FW_Resize_8u_LN_Thread (void *param);

		static void My_FW_Resize_8u_LN_Op1(const Fw8u *pSrc, Fw8u *pDst, int dstStep, 
			int resizeWidth, Linear_Array *pX_Array_Value, Linear_Array *pY_Array_Value,
			int xBegin, int xEnd, int yBegin, int yEnd, Fw16u *pIfx, Fw16u *pIofx);

		static void My_FW_Resize_8u_LN_Op(const Fw8u *pSrc, Fw8u *pDst, int dstStep, 
			int resizeWidth, Linear_Array *pX_Array_Value, Linear_Array *pY_Array_Value,
			int xBegin, int xEnd, int yBegin, int yEnd, Fw16u *pIfx, Fw16u *pIofx, int channel);


		static void My_FW_Resize_8u_LN_Op_AC4(const Fw8u *pSrc, Fw8u *pDst, int dstStep, 
			int resizeWidth, 
			Linear_Array *pX_Array_Value, Linear_Array *pY_Array_Value, 
			int xBegin, int xEnd, int yBegin, int yEnd, Fw16u *pIfx, Fw16u *pIofx, int channel);

		//internal function for mutithreading unsigned char data, Linear interpolation
		void My_FW_Resize_8u_LN_Thread (void *param)
		{
			FW_Resize_8u_Linear *par = (FW_Resize_8u_Linear *) param;

			if (par->channel == 1) {
				//Special handling for 1 channel data, since we do not need to loop for channels
				My_FW_Resize_8u_LN_Op1 (par->pSrc, par->pDst, par->dstStep, par->resizeWidth, 
					par->pX_Array_Value, par->pY_Array_Value, par->xBegin, par->xEnd, par->yBegin, par->yEnd, 
					par->pIfx, par->pIofx);
			} else if(par->channel == 5) {
				//channel 5 is for AC4, which is 4 channel.
				//AC4 Alpha channel data will not be touched
				My_FW_Resize_8u_LN_Op_AC4 (par->pSrc, par->pDst, par->dstStep, par->resizeWidth, 
					par->pX_Array_Value, par->pY_Array_Value, par->xBegin, par->xEnd, par->yBegin, par->yEnd, 
					par->pIfx, par->pIofx, 4);
			}
			else {
				// 3 or 4 channels remaining, but channel count is right
				My_FW_Resize_8u_LN_Op (par->pSrc, par->pDst, par->dstStep, par->resizeWidth, 
					par->pX_Array_Value, par->pY_Array_Value, par->xBegin, par->xEnd, par->yBegin, par->yEnd, 
					par->pIfx, par->pIofx, par->channel);
			}

			return;
		}
		
		//internal function for Row handling for 8u data, Linear mode
		static void FW_Resize_8u_LN_row_REF(Fw8u *pSrc_FloorY, Fw8u *pSrc_CeilY, Fw8u *pDst, 
			int xstart, int xend, Linear_Array *pX_Array_Value, 
			Fw16u *pIfx, Fw16u *pIofx, 
			int ifraction_y, int ione_minus_y, int channel)
		{
			int k, x;
			unsigned short half_FW_WEIGHT = 1<<(FW_WEIGHT-1);
			int ceil_X, floor_X;
			unsigned char p1, p2, p3, p4, t1, t2;

			for (x=xstart; x < xend; x++){			
				ceil_X =pX_Array_Value[x].ceil;
				floor_X=pX_Array_Value[x].floor;

				for (k=0;k<channel;k++) {
					p1 = *(pSrc_FloorY + floor_X*channel+k);
					p2 = *(pSrc_FloorY + ceil_X*channel +k);
					p3 = *(pSrc_CeilY  + floor_X*channel+k);
					p4 = *(pSrc_CeilY  + ceil_X*channel +k);

					//half_FW_WEIGHT for rounding
					//The result should be in the range of unsigned char; it needs conversion
					//but not saturation
					t1 = (unsigned char)((*(pIofx+x*channel+k) * p1 + *(pIfx+x*channel+k) * p2 
						+ half_FW_WEIGHT) >> FW_WEIGHT);
					t2 = (unsigned char)((*(pIofx+x*channel+k) * p3 + *(pIfx+x*channel+k) * p4 
						+ half_FW_WEIGHT) >> FW_WEIGHT);

					*(pDst + x*channel+k) = 
						(unsigned char)((ione_minus_y*t1 + ifraction_y*t2 
						+ half_FW_WEIGHT) >> FW_WEIGHT);
				}
			}

			return;
		}

		//Reference code for 8u resizeCenter function with Linear interpolation, fall on int y case
		static void FW_Resize_8u_LN_introw_REF(Fw8u *pSrc, Fw8u *pDst, int xstart, int xend, 
			Linear_Array *pX_Array_Value, Fw16u *pIfx, Fw16u *pIofx,
			int channel)
		{
			int k, x;
			unsigned short half_FW_WEIGHT = 1<<(FW_WEIGHT-1);
			int ceil_X, floor_X;
			unsigned char p1, p2;

			for (x=xstart; x < xend; x++){			
				ceil_X =pX_Array_Value[x].ceil;
				floor_X=pX_Array_Value[x].floor;

				// resize
				for (k=0;k<channel;k++) {
					p1 = *(pSrc + floor_X*channel+k);
					p2 = *(pSrc + ceil_X *channel+k);

					//pIofx and pIfx value has been shifted by FW_WEIGHT, but no sturation is needed
					// + half_FW_WEIGHT for rounding
					*(pDst + x*channel+k) = (unsigned char)((*(pIofx+x*channel+k) * p1 + 
						*(pIfx+x*channel+k) * p2 + half_FW_WEIGHT) >> FW_WEIGHT);
				}
			}

			return;
		}

		//internal function for Row handling for 8u data, Linear mode
		//Special handle for AC4
		//Reference code
		static void FW_Resize_8u_LN_row_REF_AC4(Fw8u *pSrc_FloorY, Fw8u *pSrc_CeilY, Fw8u *pDst, 
			int xstart, int xend, Linear_Array *pX_Array_Value, 
			Fw16u *pIfx, Fw16u *pIofx, 
			int ifraction_y, int ione_minus_y, int channel)
		{
			int k, x;
			unsigned short half_FW_WEIGHT = 1<<(FW_WEIGHT-1);
			int ceil_X, floor_X;
			unsigned char p1, p2, p3, p4, t1, t2;

			for (x=xstart; x < xend; x++){			
				ceil_X =pX_Array_Value[x].ceil;
				floor_X=pX_Array_Value[x].floor;

				// resize
				for (k=0;k<3;k++) {
					p1 = *(pSrc_FloorY + floor_X*channel+k);
					p2 = *(pSrc_FloorY + ceil_X*channel +k);
					p3 = *(pSrc_CeilY  + floor_X*channel+k);
					p4 = *(pSrc_CeilY  + ceil_X*channel +k);

					//pIofx and pIfx value has been shifted by FW_WEIGHT, but no sturation is needed
					// + half_FW_WEIGHT for rounding
					t1 = (unsigned char)((*(pIofx+x*channel+k) * p1 + *(pIfx+x*channel+k) * p2 
						+ half_FW_WEIGHT) >> FW_WEIGHT);
					t2 = (unsigned char)((*(pIofx+x*channel+k) * p3 + *(pIfx+x*channel+k) * p4 
						+ half_FW_WEIGHT) >> FW_WEIGHT);

					*(pDst + x*channel+k) = 
						(unsigned char)((ione_minus_y*t1 + ifraction_y*t2 
						+ half_FW_WEIGHT) >> FW_WEIGHT);
				}
			}

			return;
		}

		//Reference code for 8u resizeCenter function with Linear interpolation, fall on int y case for AC4 type
		static void FW_Resize_8u_LN_introw_REF_AC4(Fw8u *pSrc, Fw8u *pDst, int xstart, int xend, 
			Linear_Array *pX_Array_Value, Fw16u *pIfx, Fw16u *pIofx,
			int channel)
		{
			int k, x;
			unsigned short half_FW_WEIGHT = 1<<(FW_WEIGHT-1);
			int ceil_X, floor_X;
			unsigned char p1, p2;

			for (x=xstart; x < xend; x++){			
				ceil_X =pX_Array_Value[x].ceil;
				floor_X=pX_Array_Value[x].floor;

				// resize
				for (k=0;k<3;k++) {
					p1 = *(pSrc + floor_X*channel+k);
					p2 = *(pSrc + ceil_X *channel+k);

					//pIofx and pIfx value has been shifted by FW_WEIGHT, but no sturation is needed
					// + half_FW_WEIGHT for rounding
					*(pDst + x*channel+k) = (unsigned char)((*(pIofx+x*channel+k) * p1 + 
						*(pIfx+x*channel+k) * p2 + half_FW_WEIGHT) >> FW_WEIGHT);
				}
			}

			return;
		}

		//for C1 only, no channel loop
		void My_FW_Resize_8u_LN_Op1(const Fw8u *pSrc, Fw8u *pDst, int dstStep, 
			int resizeWidth, 
			Linear_Array *pX_Array_Value, Linear_Array *pY_Array_Value, 
			int xBegin, int xEnd, int yBegin, int yEnd, Fw16u *pIfx, Fw16u *pIofx)
		{
			int y;

			//fwMalloc allocated 32-byte alligned data
			Fw8u *pRow1 = (Fw8u*) fwMalloc(2*resizeWidth*sizeof(Fw8u));
			Fw8u *pRow2 = (Fw8u*) fwMalloc(2*resizeWidth*sizeof(Fw8u));

			Fw8u * pSrc_FloorY, *pSrc_CeilY;
			int x, xx, ceil_X, floor_X;
			int ifraction_y, ione_minus_y;
			Fw8u *prev_Src_CeilY = 0;
			Fw8u *prev_Src_FloorY = 0;
			Fw8u *pexchange;
			int yPos;
			short half_FW_WEIGHT = 1<<(FW_WEIGHT-1);
			__m128i rxmm0, rxmm1, rxmm2, rxmm3, rxmm4, rxmm5, rxmm6, rxmm7, rxmm8;
			XMM128 pp1={0}, pp2={0}, pp3={0}, pp4={0};

			rxmm8 = _mm_set1_epi16(half_FW_WEIGHT);
			if (resizeWidth < 8)
			{
				for (y = yBegin; y < yEnd; y++)
				{
					pSrc_CeilY   = (Fw8u*)pSrc + pY_Array_Value[y].ceil;
					pSrc_FloorY  = (Fw8u*)pSrc + pY_Array_Value[y].floor;

					ifraction_y = pY_Array_Value[y].ifraction;
					ione_minus_y = pY_Array_Value[y].ione_Minus_Val;
					yPos = y*dstStep;

					// use reference code for smaller case
					FW_Resize_8u_LN_row_REF(pSrc_FloorY, pSrc_CeilY, pDst+yPos, xBegin, xEnd, 
						pX_Array_Value, pIfx, pIofx, ifraction_y, ione_minus_y, 1);
				}
				return;
			}

			//To calculate the pixel value
			for (y = yBegin; y < yEnd; y++) {
				pSrc_CeilY   = (Fw8u*)pSrc + pY_Array_Value[y].ceil;
				pSrc_FloorY  = (Fw8u*)pSrc + pY_Array_Value[y].floor;

				ifraction_y = pY_Array_Value[y].ifraction;
				ione_minus_y = pY_Array_Value[y].ione_Minus_Val;
				yPos = y*dstStep;

				//The remainder case are for resizeWidth >=8
				rxmm0  = _mm_set1_epi16((short)ione_minus_y);                         
				rxmm7  = _mm_set1_epi16((short)ifraction_y);

				if (ifraction_y == 0) {//one y row case
					if (prev_Src_CeilY == pSrc_FloorY){
						pexchange = pRow1;	// swap line arrays, pRow1 for floor, pRow2 for ceil
						pRow1	 = pRow2;
						pRow2     = pexchange;

						for (x=xBegin; x <= xEnd-8; x+=8) {
							//process 8 pixels in one loop
							rxmm1 = _mm_loadu_si128((__m128i *)(pRow1 + x*2));

							//Copy data to pRow2 for next cycle usage
							_mm_storeu_si128((__m128i *)(pRow2 + x*2), rxmm1);

							rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// convert to 8bit

							_mm_storel_epi64((__m128i *)(pDst + x + yPos), rxmm1);
						}
					} else {

						for (x=xBegin; x <= xEnd-8; x+=8)	{
							// process 8 pixels in parallel						
							for (xx = 0; xx < 8; xx++)	{
								pp1.u8[xx]  = *(pSrc_FloorY + pX_Array_Value[xx+x].floor);
								pp2.u8[xx]  = *(pSrc_FloorY + pX_Array_Value[xx+x].ceil);
							}
							rxmm1 = _mm_load_si128(&pp1.i);
							rxmm2 = _mm_load_si128(&pp2.i);

							rxmm5 = _mm_loadu_si128((__m128i*)(pIfx+x));	//ifraction_x
							rxmm6 = _mm_loadu_si128((__m128i*)(pIofx+x));	//ione_minus_x

							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm6);	// ione_minus_x * p1 
							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm5);	// ifraction_x  * p2 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm2);
							rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
							rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

							_mm_storeu_si128((__m128i *)(pRow1 + x*2), rxmm1);
							//Copy data to pRow2 for next cycle usage
							_mm_storeu_si128((__m128i *)(pRow2 + x*2), rxmm1);

							rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

							_mm_storel_epi64((__m128i *)(pDst + x + yPos), rxmm1);
						}
					}

					//The remainder part use reference code
					if (x<xEnd) {
						FW_Resize_8u_LN_introw_REF(pSrc_FloorY, pDst+yPos, x, xEnd, 
							pX_Array_Value, pIfx, pIofx, 1);
					}

				} else {//ifraction_y != 0, two y rows needed, general case

					if (prev_Src_CeilY == pSrc_CeilY && prev_Src_FloorY == pSrc_FloorY) {

						for (x=xBegin; x <= xEnd-8; x+=8)	{
							//process 8 pixels in parallel	
							rxmm1 = _mm_loadu_si128((__m128i*)(pRow1 + x*2));
							rxmm3 = _mm_loadu_si128((__m128i*)(pRow2 + x*2));

							//*(pDst + x + y*dstStep) = (unsigned char)((ione_minus_y *t1 
							//         + ifraction_y * t2) >> FW_WEIGHT);
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm0);	// ione_minus_y * t1 
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm7);	// ifraction_y  * t2
							rxmm1 = _mm_add_epi16(rxmm1, rxmm3);
							rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
							rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);
							rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

							_mm_storel_epi64((__m128i *)(pDst + x + yPos), rxmm1);
						}

					} else if (prev_Src_CeilY == pSrc_FloorY) {
						//easier case, pcurrent floor = previous ceiling
						//We use previously calculated pRow2 data --> pRow1, then generate new pRow2
						pexchange = pRow1;	// swap line arrays, pRow1 for floor, pRow2 for ceil
						pRow1	 = pRow2;
						pRow2     = pexchange;

						for (x=xBegin; x <= xEnd-8; x+=8)	{
							for (xx = 0; xx < 8; xx++)	{
								// process 8 pixels in parallel
								ceil_X		 = pX_Array_Value[xx+x].ceil;
								floor_X		 = pX_Array_Value[xx+x].floor;

								pp3.u8 [xx] = *(pSrc_CeilY + floor_X);
								pp4.u8 [xx] = *(pSrc_CeilY + ceil_X);
							}

							//pRow1 is ready
							rxmm1 = _mm_loadu_si128((__m128i *)(pRow1 + x*2));

							rxmm3 = _mm_load_si128(&pp3.i);
							rxmm4 = _mm_load_si128(&pp4.i);
							rxmm5 = _mm_loadu_si128((__m128i*)(pIfx+x));	// ifraction_x
							rxmm6 = _mm_loadu_si128((__m128i*)(pIofx+x));	// ione_minus_x

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// ione_minus_x * p1 
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm5);	// ifraction_x  * p2
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);            
							rxmm3 = _mm_add_epi16(rxmm3, rxmm8);  
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT);

							_mm_storeu_si128((__m128i *)(pRow2 + x*2), rxmm3);

							rxmm1 = _mm_mullo_epi16 (rxmm0, rxmm1);	// ione_minus_y * t1 
							rxmm3 = _mm_mullo_epi16 (rxmm7, rxmm3);	// ifraction_y  * t2
							rxmm1 = _mm_add_epi16(rxmm1, rxmm3);
							rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
							rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

							rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

							_mm_storel_epi64((__m128i *)(pDst + x + yPos), rxmm1);
						}
					} else {
						// Different ceiling and floor
						// We need to generate new pRow1 and pRow2

						for (x=xBegin; x <= xEnd-8; x+=8)	{	
							//Process 8 pixels by at the same time
							for (xx=0;xx<8;xx++) {
								pp1.u8[xx] = *(pSrc_FloorY + pX_Array_Value[xx+x].floor);
								pp2.u8[xx] = *(pSrc_FloorY + pX_Array_Value[xx+x].ceil);
								pp3.u8[xx] = *(pSrc_CeilY + pX_Array_Value[xx+x].floor);
								pp4.u8[xx] = *(pSrc_CeilY + pX_Array_Value[xx+x].ceil);
							}

							__m128i iofx_tmp, ifx_tmp;
							iofx_tmp = _mm_loadu_si128((__m128i*)(pIofx+x));
							ifx_tmp = _mm_loadu_si128((__m128i*)(pIfx+x));

							rxmm1 = _mm_mullo_epi16 (pp1.i, iofx_tmp);	// ione_minus_x * p1 
							rxmm2 = _mm_mullo_epi16 (pp2.i, ifx_tmp);	// ifraction_x  * p2 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm2);
							rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
							rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

							_mm_storeu_si128((__m128i *)(pRow1 + x*2), rxmm1);

							rxmm3 = _mm_mullo_epi16 (pp3.i, iofx_tmp);	// ione_minus_x * p1 
							rxmm4 = _mm_mullo_epi16 (pp4.i, ifx_tmp);	// ifraction_x  * p2
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);            
							rxmm3 = _mm_add_epi16(rxmm3, rxmm8);
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT);

							_mm_storeu_si128((__m128i *)(pRow2 + x*2), rxmm3);

							rxmm1 = _mm_mullo_epi16 (rxmm0, rxmm1);	// ione_minus_y * t1 
							rxmm3 = _mm_mullo_epi16 (rxmm7, rxmm3);	// ifraction_y  * t2
							rxmm1 = _mm_add_epi16(rxmm1, rxmm3);
							rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
							rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

							rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

							_mm_storel_epi64((__m128i *)(pDst + x + yPos), rxmm1);
						}
					}

					//The remainder part use reference code
					if (x<xEnd) {
						FW_Resize_8u_LN_row_REF(pSrc_FloorY, pSrc_CeilY, pDst+yPos, x, xEnd, 
							pX_Array_Value, pIfx, pIofx, ifraction_y, ione_minus_y, 1);
					}
				}

				prev_Src_FloorY = pSrc_FloorY;
				prev_Src_CeilY  = pSrc_CeilY;
			}

			//free allocated memory by fwMalloc
			fwFree(pRow1);
			fwFree(pRow2);

			return;
		}

		//internal function for resize, 8u data, Linear interpolation, special for threaded unit
		void My_FW_Resize_8u_LN_Op(const Fw8u *pSrc, Fw8u *pDst, int dstStep, 
			int resizeWidth, 
			Linear_Array *pX_Array_Value, Linear_Array *pY_Array_Value, 
			int xBegin, int xEnd, int yBegin, int yEnd, Fw16u *pIfx, Fw16u *pIofx, int channel)
		{
			int y;

			//fwMalloc allocated 32-byte alligned data
			Fw8u *pRow1 = (Fw8u*) fwMalloc(2*channel*resizeWidth*sizeof(Fw8u));
			Fw8u *pRow2 = (Fw8u*) fwMalloc(2*channel*resizeWidth*sizeof(Fw8u));

			Fw8u * pSrc_FloorY, *pSrc_CeilY;
			int x, xx, ceil_X, floor_X;
			int ifraction_y, ione_minus_y;
			Fw8u *prev_Src_CeilY = 0;
			Fw8u *prev_Src_FloorY = 0;
			Fw8u *pexchange;
			int yPos, k;
			unsigned short half_FW_WEIGHT = 1<<(FW_WEIGHT-1);
			__m128i rxmm0, rxmm1, rxmm2, rxmm3, rxmm4, rxmm5, rxmm6, rxmm7, rxmm8;
			_MM_ALIGN16 unsigned short pp1[8*MAX_CHANNEL], pp2[8*MAX_CHANNEL], pp3[8*MAX_CHANNEL], 
				pp4[8*MAX_CHANNEL];

			rxmm8 = _mm_set1_epi16(half_FW_WEIGHT);

			if (resizeWidth < 8)
			{
				for (y = yBegin; y < yEnd; y++)
				{
					pSrc_CeilY   = (Fw8u*)pSrc + pY_Array_Value[y].ceil;
					pSrc_FloorY  = (Fw8u*)pSrc + pY_Array_Value[y].floor;

					ifraction_y = pY_Array_Value[y].ifraction;
					ione_minus_y = pY_Array_Value[y].ione_Minus_Val;
					yPos = y*dstStep;

					// use reference code for smaller case
					FW_Resize_8u_LN_row_REF(pSrc_FloorY, pSrc_CeilY, pDst+yPos, xBegin, xEnd, 
						pX_Array_Value, pIfx, pIofx, ifraction_y, ione_minus_y, channel);
				}
				return;
			}

			//To calculate the pixel value
			for (y = yBegin; y < yEnd; y++) {
				pSrc_CeilY   = (Fw8u*)pSrc + pY_Array_Value[y].ceil;
				pSrc_FloorY  = (Fw8u*)pSrc + pY_Array_Value[y].floor;

				ifraction_y = pY_Array_Value[y].ifraction;
				ione_minus_y = pY_Array_Value[y].ione_Minus_Val;
				yPos = y*dstStep;

				//The remainder case are for resizeWidth >=8
				rxmm0  = _mm_set1_epi16((short)ione_minus_y);                         
				rxmm7  = _mm_set1_epi16((short)ifraction_y);

				if (ifraction_y == 0) {//one y row case
					if (prev_Src_CeilY == pSrc_FloorY){
						pexchange = pRow1;	// swap line arrays, pRow1 for floor, pRow2 for ceil
						pRow1	 = pRow2;
						pRow2     = pexchange;

						for (x=xBegin; x<=xEnd-8; x+=8) {
							//process 8 pixels in one loop
							for (k=0;k<channel;k++) {
								rxmm1 = _mm_loadu_si128((__m128i *)(pRow1 + x*2*channel+k*16));

								//Copy data to pRow2 for next cycle usage
								_mm_storeu_si128((__m128i *)(pRow2 + x*2*channel+k*16), rxmm1);

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								_mm_storel_epi64((__m128i *)(pDst + x*channel + 8*k + yPos), rxmm1);
							}
						}
					} else {

						for (x=xBegin; x<=xEnd-8; x+=8)	{
							// process 8 pixels in parallel		
							if(channel == 4 )
							{
								for (xx = 0; xx < 8; xx = xx+2)	{

									ceil_X		 = pX_Array_Value[xx+x].ceil;
									floor_X		 = pX_Array_Value[xx+x].floor;
									__m128i temp1, temp2, temp;

									temp1 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + floor_X*channel)));
									temp2 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + ceil_X*channel)));

									ceil_X		 = pX_Array_Value[xx+x+1].ceil;
									floor_X		 = pX_Array_Value[xx+x+1].floor;

									temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + floor_X*channel)));
									temp = _mm_slli_si128(temp, 4);
									temp1 = _mm_or_si128(temp1, temp);

									temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + ceil_X*channel)));
									temp = _mm_slli_si128(temp, 4);
									temp2 = _mm_or_si128(temp2, temp);

									temp1 = _mm_unpacklo_epi8(temp1, _mm_setzero_si128());
									temp2 = _mm_unpacklo_epi8(temp2, _mm_setzero_si128());

									_mm_store_si128((__m128i *)((pp1+xx*channel)), temp1);
									_mm_store_si128((__m128i *)((pp2+xx*channel)), temp2);
								}
							}
							else
							{
								for (xx = 0; xx < 8; xx++)	{
									ceil_X		 = pX_Array_Value[xx+x].ceil;
									floor_X		 = pX_Array_Value[xx+x].floor;

									for (k=0;k<channel;k++) {
										pp1[xx*channel+k]  = *(pSrc_FloorY + floor_X*channel + k);
										pp2[xx*channel+k]  = *(pSrc_FloorY + ceil_X*channel + k);
									}
								}
							}

							for (k=0;k<channel; k++) {
								rxmm1 = _mm_load_si128((__m128i*)&pp1[k*8]);
								rxmm2 = _mm_load_si128((__m128i*)&pp2[k*8]);

								rxmm5 = _mm_loadu_si128((__m128i*)(pIfx+x*channel+k*8));	// ifraction_x
								rxmm6 = _mm_loadu_si128((__m128i*)(pIofx+x*channel+k*8));	// ione_minus_x

								rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm6);	// ione_minus_x * p1 
								rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm5);	// ifraction_x  * p2 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm2);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

								_mm_storeu_si128((__m128i *)(pRow1 + x*2*channel+16*k), rxmm1);
								//Copy data to pRow2 for next cycle usage
								_mm_storeu_si128((__m128i *)(pRow2 + x*2*channel+k*16), rxmm1);

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								_mm_storel_epi64((__m128i *)(pDst + x*channel + 8*k + yPos), rxmm1);
							}
						}
					}

					//The remainder part use reference code
					if (x<xEnd) {
						FW_Resize_8u_LN_introw_REF(pSrc_FloorY, pDst+yPos, x, xEnd, 
							pX_Array_Value, pIfx, pIofx, channel);
					}

				} else {//ifraction_y != 0, two y rows needed, general case

					if (prev_Src_CeilY == pSrc_CeilY && prev_Src_FloorY == pSrc_FloorY) {

						// for (x=0; x <= resizeWidth-8; x+=8)
						for (x=xBegin; x<=xEnd-8; x+=8)	{
							//process 8 pixels in parallel	
							for (k=0;k<channel;k++) {
								rxmm1 = _mm_loadu_si128((__m128i*)(pRow1 + x*2*channel+k*16));
								rxmm3 = _mm_loadu_si128((__m128i*)(pRow2 + x*2*channel+k*16));

								rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm0);	// ione_minus_y * t1 
								rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm7);	// ifraction_y  * t2
								rxmm1 = _mm_add_epi16(rxmm1, rxmm3);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);
								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								_mm_storel_epi64((__m128i *)(pDst + x*channel + k*8 + yPos), rxmm1);
							}
						}

					} else if (prev_Src_CeilY == pSrc_FloorY) {
						//easier case, pcurrent floor = previous ceiling
						//We use previously calculated pRow2 data --> pRow1, then generate new pRow2
						pexchange = pRow1;	// swap line arrays, pRow1 for floor, pRow2 for ceil
						pRow1	 = pRow2;
						pRow2     = pexchange;

						for (x=xBegin; x<=xEnd-8; x+=8)	
						{
							if(channel == 4 )
							{
								for (xx = 0; xx < 8; xx = xx+2)	{

									ceil_X		 = pX_Array_Value[xx+x].ceil;
									floor_X		 = pX_Array_Value[xx+x].floor;
									__m128i temp1, temp2, temp;

									temp1 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + floor_X*channel)));
									temp2 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + ceil_X*channel)));

									ceil_X		 = pX_Array_Value[xx+x+1].ceil;
									floor_X		 = pX_Array_Value[xx+x+1].floor;

									temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + floor_X*channel)));
									temp = _mm_slli_si128(temp, 4);
									temp1 = _mm_or_si128(temp1, temp);

									temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + ceil_X*channel)));
									temp = _mm_slli_si128(temp, 4);
									temp2 = _mm_or_si128(temp2, temp);

									temp1 = _mm_unpacklo_epi8(temp1, _mm_setzero_si128());
									temp2 = _mm_unpacklo_epi8(temp2, _mm_setzero_si128());

									_mm_store_si128((__m128i *)((pp3+xx*channel)), temp1);
									_mm_store_si128((__m128i *)((pp4+xx*channel)), temp2);
								}
							}
							else
							{
								for (xx = 0; xx < 8; xx++)	{
									// process 8 pixels in parallel
									ceil_X		 = pX_Array_Value[xx+x].ceil;
									floor_X		 = pX_Array_Value[xx+x].floor;

									for (k=0;k<channel;k++) {
										pp3[xx*channel+k]  = *(pSrc_CeilY + floor_X*channel + k);
										pp4[xx*channel+k]  = *(pSrc_CeilY + ceil_X*channel + k);
									}
								}
							}

							for (k=0;k<channel; k++) {
								//pRow1 is ready
								rxmm1 = _mm_loadu_si128((__m128i *)(pRow1 + x*2*channel + k*16));

								rxmm3 = _mm_load_si128((__m128i*)&pp3[k*8]);
								rxmm4 = _mm_load_si128((__m128i*)&pp4[k*8]);
								rxmm5 = _mm_loadu_si128((__m128i*)(pIfx+x*channel+k*8));	// ifraction_x
								rxmm6 = _mm_loadu_si128((__m128i*)(pIofx+x*channel+k*8));	// ione_minus_x

								rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// ione_minus_x * p1 
								rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm5);	// ifraction_x  * p2
								rxmm3 = _mm_add_epi16(rxmm3, rxmm4);            
								rxmm3 = _mm_add_epi16(rxmm3, rxmm8); 
								rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT);

								_mm_storeu_si128((__m128i *)(pRow2 + x*2*channel + k*16), rxmm3);

								rxmm1 = _mm_mullo_epi16 (rxmm0, rxmm1);	// ione_minus_y * t1 
								rxmm3 = _mm_mullo_epi16 (rxmm7, rxmm3);	// ifraction_y  * t2
								rxmm1 = _mm_add_epi16(rxmm1, rxmm3);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								_mm_storel_epi64((__m128i *)(pDst + x*channel + k*8 + yPos), rxmm1);
							}
						}
					} else {
						for (x=xBegin; x<=xEnd-8; x+=8)	
						{	
							if(channel == 4 )
							{
								for (xx = 0; xx < 8; xx = xx+2)	{

									ceil_X		 = pX_Array_Value[xx+x].ceil;
									floor_X		 = pX_Array_Value[xx+x].floor;
									__m128i temp1, temp2, temp;

									temp1 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + floor_X*channel)));
									temp2 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + ceil_X*channel)));

									ceil_X		 = pX_Array_Value[xx+x+1].ceil;
									floor_X		 = pX_Array_Value[xx+x+1].floor;

									temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + floor_X*channel)));
									temp = _mm_slli_si128(temp, 4);
									temp1 = _mm_or_si128(temp1, temp);

									temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + ceil_X*channel)));
									temp = _mm_slli_si128(temp, 4);
									temp2 = _mm_or_si128(temp2, temp);

									temp1 = _mm_unpacklo_epi8(temp1, _mm_setzero_si128());
									temp2 = _mm_unpacklo_epi8(temp2, _mm_setzero_si128());

									_mm_store_si128((__m128i *)((pp1+xx*channel)), temp1);
									_mm_store_si128((__m128i *)((pp2+xx*channel)), temp2);

									ceil_X		 = pX_Array_Value[xx+x].ceil;
									floor_X		 = pX_Array_Value[xx+x].floor;

									temp1 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + floor_X*channel)));
									temp2 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + ceil_X*channel)));

									ceil_X		 = pX_Array_Value[xx+x+1].ceil;
									floor_X		 = pX_Array_Value[xx+x+1].floor;

									temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + floor_X*channel)));
									temp = _mm_slli_si128(temp, 4);
									temp1 = _mm_or_si128(temp1, temp);

									temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + ceil_X*channel)));
									temp = _mm_slli_si128(temp, 4);
									temp2 = _mm_or_si128(temp2, temp);

									temp1 = _mm_unpacklo_epi8(temp1, _mm_setzero_si128());
									temp2 = _mm_unpacklo_epi8(temp2, _mm_setzero_si128());

									_mm_store_si128((__m128i *)((pp3+xx*channel)), temp1);
									_mm_store_si128((__m128i *)((pp4+xx*channel)), temp2);
								}
							}
							else
							{
								for (xx=0;xx<8;xx++) {
									ceil_X		 = pX_Array_Value[xx+x].ceil;
									floor_X		 = pX_Array_Value[xx+x].floor;

									for (k=0;k<channel;k++) {
										pp1[xx*channel+k]  = *(pSrc_FloorY + floor_X*channel + k);
										pp2[xx*channel+k]  = *(pSrc_FloorY + ceil_X*channel + k);
										pp3 [xx*channel+k] = *(pSrc_CeilY + floor_X*channel + k);
										pp4 [xx*channel+k] = *(pSrc_CeilY + ceil_X*channel + k);
									}
								}
							}

							for (k=0;k<channel; k++) {

								__m128i iofx_tmp, ifx_tmp;
								iofx_tmp = _mm_loadu_si128((__m128i*)(pIofx+x*channel+k*8));
								ifx_tmp = _mm_loadu_si128((__m128i*)(pIfx+x*channel+k*8));

								rxmm1 = _mm_mullo_epi16 (*((__m128i*)&pp1[k*8]), iofx_tmp);	// ione_minus_x * p1 
								rxmm2 = _mm_mullo_epi16 (*((__m128i*)&pp2[k*8]), ifx_tmp);	// ifraction_x  * p2 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm2);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

								_mm_storeu_si128((__m128i *)(pRow1 + x*2*channel + k*16), rxmm1);

								rxmm3 = _mm_mullo_epi16 (*((__m128i*)&pp3[k*8]), iofx_tmp);	// ione_minus_x * p1 
								rxmm4 = _mm_mullo_epi16 (*((__m128i*)&pp4[k*8]), ifx_tmp);	// ifraction_x  * p2
								rxmm3 = _mm_add_epi16(rxmm3, rxmm4);            
								rxmm3 = _mm_add_epi16(rxmm3, rxmm8); 
								rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT);

								_mm_storeu_si128((__m128i *)(pRow2 + x*2*channel + k*16), rxmm3);

								rxmm1 = _mm_mullo_epi16 (rxmm0, rxmm1);	// ione_minus_y * t1 
								rxmm3 = _mm_mullo_epi16 (rxmm7, rxmm3);	// ifraction_y  * t2
								rxmm1 = _mm_add_epi16(rxmm1, rxmm3);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								_mm_storel_epi64((__m128i *)(pDst + x*channel + k*8 + yPos), rxmm1);
							}
						}
					}

					//The remainder part use reference code
					if (x<xEnd) {
						FW_Resize_8u_LN_row_REF(pSrc_FloorY, pSrc_CeilY, pDst+yPos, x, xEnd, 
							pX_Array_Value, pIfx, pIofx, ifraction_y, ione_minus_y, channel);
					}
				}

				prev_Src_FloorY = pSrc_FloorY;
				prev_Src_CeilY  = pSrc_CeilY;
			}

			//free allocated memory by fwMalloc
			fwFree(pRow1);
			fwFree(pRow2);

			return;
		}

		//internal function for resize, 8u data with AC$, Linear interpolation, single thread unit
		void My_FW_Resize_8u_LN_Op_AC4(const Fw8u *pSrc, Fw8u *pDst, int dstStep, 
			int resizeWidth, 
			Linear_Array *pX_Array_Value, Linear_Array *pY_Array_Value, 
			int xBegin, int xEnd, int yBegin, int yEnd, Fw16u *pIfx, Fw16u *pIofx, int channel)
		{
			int y;

			//fwMalloc allocated 32-byte alligned data
			Fw8u *pRow1 = (Fw8u*) fwMalloc(2*channel*resizeWidth*sizeof(Fw8u));
			Fw8u *pRow2 = (Fw8u*) fwMalloc(2*channel*resizeWidth*sizeof(Fw8u));

			Fw8u * pSrc_FloorY, *pSrc_CeilY;
			int x, xx, ceil_X, floor_X;
			int ifraction_y, ione_minus_y;
			Fw8u *prev_Src_CeilY = 0;
			Fw8u *prev_Src_FloorY = 0;
			Fw8u *pexchange;
			int yPos, k;
			unsigned short half_FW_WEIGHT = 1<<(FW_WEIGHT-1);
			__m128i rxmm0, rxmm1, rxmm2, rxmm3, rxmm4, rxmm5, rxmm6, rxmm7, rxmm8;
			_MM_ALIGN16 unsigned short pp1[8*MAX_CHANNEL], pp2[8*MAX_CHANNEL], pp3[8*MAX_CHANNEL], 
				pp4[8*MAX_CHANNEL];
			__m128i mask = _mm_set_epi32(0, 0, 0x00FFFFFF, 0x00FFFFFF);

			rxmm8 = _mm_set1_epi16(half_FW_WEIGHT);

			//To calculate the pixel value
			for (y = yBegin; y < yEnd; y++) {
				pSrc_CeilY   = (Fw8u*)pSrc + pY_Array_Value[y].ceil;
				pSrc_FloorY  = (Fw8u*)pSrc + pY_Array_Value[y].floor;

				ifraction_y = pY_Array_Value[y].ifraction;
				ione_minus_y = pY_Array_Value[y].ione_Minus_Val;
				yPos = y*dstStep;

				if (resizeWidth < 8) { // use reference code for smaller case
					FW_Resize_8u_LN_row_REF_AC4(pSrc_FloorY, pSrc_CeilY, pDst+yPos, xBegin, xEnd, 
						pX_Array_Value, pIfx, pIofx, ifraction_y, ione_minus_y, channel);
					continue;
				}

				//The remainder case are for resizeWidth >=8
				rxmm0  = _mm_set1_epi16((short)ione_minus_y);                         
				rxmm7  = _mm_set1_epi16((short)ifraction_y);

				if (ifraction_y == 0) {//one y row case
					if (prev_Src_CeilY == pSrc_FloorY){
						pexchange = pRow1;	// swap line arrays, pRow1 for floor, pRow2 for ceil
						pRow1	 = pRow2;
						pRow2     = pexchange;

						//debug
						//count[0]++;

						for (x=xBegin; x<=xEnd-8; x+=8) {
							//process 8 pixels in one loop
							for (k=0;k<channel;k++) {
								rxmm1 = _mm_loadu_si128((__m128i *)(pRow1 + x*2*channel+k*16));

								//Copy data to pRow2 for next cycle usage
								_mm_storeu_si128((__m128i *)(pRow2 + x*2*channel+k*16), rxmm1);

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								__m128i temp = _mm_loadl_epi64((__m128i *)(pDst + x*channel + 8*k + yPos));
								temp = _mm_andnot_si128(temp, mask);
								rxmm1 = _mm_and_si128(rxmm1, mask);
								rxmm1 = _mm_or_si128(rxmm1, temp);
								_mm_storel_epi64((__m128i *)(pDst + x*channel + 8*k + yPos), rxmm1);
							}
						}
					} else {
						for (x=xBegin; x<=xEnd-8; x+=8)	{
							// process 8 pixels in parallel		
							for (xx = 0; xx < 8; xx = xx+2)	{

								ceil_X		 = pX_Array_Value[xx+x].ceil;
								floor_X		 = pX_Array_Value[xx+x].floor;
								__m128i temp1, temp2, temp;

								temp1 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + floor_X*channel)));
								temp2 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + ceil_X*channel)));

								ceil_X		 = pX_Array_Value[xx+x+1].ceil;
								floor_X		 = pX_Array_Value[xx+x+1].floor;

								temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + floor_X*channel)));
								temp = _mm_slli_si128(temp, 4);
								temp1 = _mm_or_si128(temp1, temp);

								temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + ceil_X*channel)));
								temp = _mm_slli_si128(temp, 4);
								temp2 = _mm_or_si128(temp2, temp);

								temp1 = _mm_unpacklo_epi8(temp1, _mm_setzero_si128());
								temp2 = _mm_unpacklo_epi8(temp2, _mm_setzero_si128());

								_mm_store_si128((__m128i *)((pp1+xx*channel)), temp1);
								_mm_store_si128((__m128i *)((pp2+xx*channel)), temp2);
							}

							for (k=0;k<channel; k++) {
								rxmm1 = _mm_load_si128((__m128i*)&pp1[k*8]);
								rxmm2 = _mm_load_si128((__m128i*)&pp2[k*8]);

								rxmm5 = _mm_loadu_si128((__m128i*)(pIfx+x*channel+k*8));	// ifraction_x
								rxmm6 = _mm_loadu_si128((__m128i*)(pIofx+x*channel+k*8));	// ione_minus_x

								// t1 = (unsigned char)((ione_minus_x * p1 + ifraction_x * p2) >> FW_WEIGHT)
								rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm6);	// ione_minus_x * p1 
								rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm5);	// ifraction_x  * p2 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm2);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

								_mm_storeu_si128((__m128i *)(pRow1 + x*2*channel+16*k), rxmm1);
								//Copy data to pRow2 for next cycle usage
								_mm_storeu_si128((__m128i *)(pRow2 + x*2*channel+k*16), rxmm1);

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								__m128i temp = _mm_loadl_epi64((__m128i *)(pDst + x*channel + 8*k + yPos));
								temp = _mm_andnot_si128(temp, mask);
								rxmm1 = _mm_and_si128(rxmm1, mask);
								rxmm1 = _mm_or_si128(rxmm1, temp);
								_mm_storel_epi64((__m128i *)(pDst + x*channel + 8*k + yPos), rxmm1);
							}
						}
					}

					//The remainder part use reference code
					if (x<xEnd) {
						FW_Resize_8u_LN_introw_REF_AC4(pSrc_FloorY, pDst+yPos, x, xEnd, 
							pX_Array_Value, pIfx, pIofx, channel);
					}

				} else {//ifraction_y != 0, two y rows needed, general case
					if (prev_Src_CeilY == pSrc_CeilY && prev_Src_FloorY == pSrc_FloorY) {

						// for (x=0; x <= resizeWidth-8; x+=8)
						for (x=xBegin; x<=xEnd-8; x+=8)	{
							//process 8 pixels in parallel	
							for (k=0;k<channel;k++) {
								rxmm1 = _mm_loadu_si128((__m128i*)(pRow1 + x*2*channel+k*16));
								rxmm3 = _mm_loadu_si128((__m128i*)(pRow2 + x*2*channel+k*16));

								//*(pDst + x + y*dstStep) = (unsigned char)((ione_minus_y *t1 
								//         + ifraction_y * t2) >> FW_WEIGHT);
								rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm0);	// ione_minus_y * t1 
								rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm7);	// ifraction_y  * t2
								rxmm1 = _mm_add_epi16(rxmm1, rxmm3);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);
								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								__m128i temp = _mm_loadl_epi64((__m128i *)(pDst + x*channel + 8*k + yPos));
								temp = _mm_andnot_si128(temp, mask);
								rxmm1 = _mm_and_si128(rxmm1, mask);
								rxmm1 = _mm_or_si128(rxmm1, temp);

								_mm_storel_epi64((__m128i *)(pDst + x*channel + k*8 + yPos), rxmm1);
							}
						}

					} else if (prev_Src_CeilY == pSrc_FloorY) {
						//easier case, pcurrent floor = previous ceiling
						//We use previously calculated pRow2 data --> pRow1, then generate new pRow2
						pexchange = pRow1;	// swap line arrays, pRow1 for floor, pRow2 for ceil
						pRow1	 = pRow2;
						pRow2     = pexchange;

						for (x=xBegin; x<=xEnd-8; x+=8)	
						{
							for (xx = 0; xx < 8; xx = xx+2)	{

								ceil_X		 = pX_Array_Value[xx+x].ceil;
								floor_X		 = pX_Array_Value[xx+x].floor;
								__m128i temp1, temp2, temp;

								temp1 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + floor_X*channel)));
								temp2 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + ceil_X*channel)));

								ceil_X		 = pX_Array_Value[xx+x+1].ceil;
								floor_X		 = pX_Array_Value[xx+x+1].floor;

								temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + floor_X*channel)));
								temp = _mm_slli_si128(temp, 4);
								temp1 = _mm_or_si128(temp1, temp);

								temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + ceil_X*channel)));
								temp = _mm_slli_si128(temp, 4);
								temp2 = _mm_or_si128(temp2, temp);

								temp1 = _mm_unpacklo_epi8(temp1, _mm_setzero_si128());
								temp2 = _mm_unpacklo_epi8(temp2, _mm_setzero_si128());

								_mm_store_si128((__m128i *)((pp3+xx*channel)), temp1);
								_mm_store_si128((__m128i *)((pp4+xx*channel)), temp2);
							}

							for (k=0;k<channel; k++) {
								//pRow1 is ready
								rxmm1 = _mm_loadu_si128((__m128i *)(pRow1 + x*2*channel + k*16));

								rxmm3 = _mm_load_si128((__m128i*)&pp3[k*8]);
								rxmm4 = _mm_load_si128((__m128i*)&pp4[k*8]);
								rxmm5 = _mm_loadu_si128((__m128i*)(pIfx+x*channel+k*8));	// ifraction_x
								rxmm6 = _mm_loadu_si128((__m128i*)(pIofx+x*channel+k*8));	// ione_minus_x

								// t2 = (unsigned char)((ione_minus_x * p3 + 
								//              ifraction_x * p4) >> FW_WEIGHT);
								rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// ione_minus_x * p1 
								rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm5);	// ifraction_x  * p2
								rxmm3 = _mm_add_epi16(rxmm3, rxmm4);            
								rxmm3 = _mm_add_epi16(rxmm3, rxmm8); 
								rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT);

								_mm_storeu_si128((__m128i *)(pRow2 + x*2*channel + k*16), rxmm3);

								// *(pDst + x + y*dstStep) = (unsigned char)((ione_minus_y *t1 +
								//              ifraction_y * t2) >> FW_WEIGHT);*/
								rxmm1 = _mm_mullo_epi16 (rxmm0, rxmm1);	// ione_minus_y * t1 
								rxmm3 = _mm_mullo_epi16 (rxmm7, rxmm3);	// ifraction_y  * t2
								rxmm1 = _mm_add_epi16(rxmm1, rxmm3);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								__m128i temp = _mm_loadl_epi64((__m128i *)(pDst + x*channel + 8*k + yPos));
								temp = _mm_andnot_si128(temp, mask);
								rxmm1 = _mm_and_si128(rxmm1, mask);
								rxmm1 = _mm_or_si128(rxmm1, temp);
								_mm_storel_epi64((__m128i *)(pDst + x*channel + k*8 + yPos), rxmm1);
							}
						}
					} else {
						for (x=xBegin; x<=xEnd-8; x+=8)	
						{	
							for (xx = 0; xx < 8; xx = xx+2)	{

								ceil_X		 = pX_Array_Value[xx+x].ceil;
								floor_X		 = pX_Array_Value[xx+x].floor;
								__m128i temp1, temp2, temp;

								temp1 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + floor_X*channel)));
								temp2 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + ceil_X*channel)));

								ceil_X		 = pX_Array_Value[xx+x+1].ceil;
								floor_X		 = pX_Array_Value[xx+x+1].floor;

								temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + floor_X*channel)));
								temp = _mm_slli_si128(temp, 4);
								temp1 = _mm_or_si128(temp1, temp);

								temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_FloorY + ceil_X*channel)));
								temp = _mm_slli_si128(temp, 4);
								temp2 = _mm_or_si128(temp2, temp);

								temp1 = _mm_unpacklo_epi8(temp1, _mm_setzero_si128());
								temp2 = _mm_unpacklo_epi8(temp2, _mm_setzero_si128());

								_mm_store_si128((__m128i *)((pp1+xx*channel)), temp1);
								_mm_store_si128((__m128i *)((pp2+xx*channel)), temp2);

								ceil_X		 = pX_Array_Value[xx+x].ceil;
								floor_X		 = pX_Array_Value[xx+x].floor;

								temp1 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + floor_X*channel)));
								temp2 = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + ceil_X*channel)));

								ceil_X		 = pX_Array_Value[xx+x+1].ceil;
								floor_X		 = pX_Array_Value[xx+x+1].floor;

								temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + floor_X*channel)));
								temp = _mm_slli_si128(temp, 4);
								temp1 = _mm_or_si128(temp1, temp);

								temp = _mm_cvtsi32_si128( *((Fw32u*)(pSrc_CeilY + ceil_X*channel)));
								temp = _mm_slli_si128(temp, 4);
								temp2 = _mm_or_si128(temp2, temp);

								temp1 = _mm_unpacklo_epi8(temp1, _mm_setzero_si128());
								temp2 = _mm_unpacklo_epi8(temp2, _mm_setzero_si128());

								_mm_store_si128((__m128i *)((pp3+xx*channel)), temp1);
								_mm_store_si128((__m128i *)((pp4+xx*channel)), temp2);
							}

							for (k=0;k<channel; k++) {

								__m128i iofx_tmp, ifx_tmp;
								iofx_tmp = _mm_loadu_si128((__m128i*)(pIofx+x*channel+k*8));
								ifx_tmp = _mm_loadu_si128((__m128i*)(pIfx+x*channel+k*8));

								rxmm1 = _mm_mullo_epi16 (*((__m128i*)&pp1[k*8]), iofx_tmp);	// ione_minus_x * p1 
								rxmm2 = _mm_mullo_epi16 (*((__m128i*)&pp2[k*8]), ifx_tmp);	// ifraction_x  * p2 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm2);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

								_mm_storeu_si128((__m128i *)(pRow1 + x*2*channel + k*16), rxmm1);

								rxmm3 = _mm_mullo_epi16 (*((__m128i*)&pp3[k*8]), iofx_tmp);	// ione_minus_x * p1 
								rxmm4 = _mm_mullo_epi16 (*((__m128i*)&pp4[k*8]), ifx_tmp);	// ifraction_x  * p2
								rxmm3 = _mm_add_epi16(rxmm3, rxmm4);            
								rxmm3 = _mm_add_epi16(rxmm3, rxmm8); 
								rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT);

								_mm_storeu_si128((__m128i *)(pRow2 + x*2*channel + k*16), rxmm3);

								rxmm1 = _mm_mullo_epi16 (rxmm0, rxmm1);	// ione_minus_y * t1 
								rxmm3 = _mm_mullo_epi16 (rxmm7, rxmm3);	// ifraction_y  * t2
								rxmm1 = _mm_add_epi16(rxmm1, rxmm3);
								rxmm1 = _mm_add_epi16(rxmm1, rxmm8);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								__m128i temp = _mm_loadl_epi64((__m128i *)(pDst + x*channel + 8*k + yPos));
								temp = _mm_andnot_si128(temp, mask);
								rxmm1 = _mm_and_si128(rxmm1, mask);
								rxmm1 = _mm_or_si128(rxmm1, temp);

								_mm_storel_epi64((__m128i *)(pDst + x*channel + k*8 + yPos), rxmm1);
							}
						}
					}

					//The remainder part use reference code
					if (x<xEnd) {
						FW_Resize_8u_LN_row_REF_AC4(pSrc_FloorY, pSrc_CeilY, pDst+yPos, x, xEnd, 
							pX_Array_Value, pIfx, pIofx, ifraction_y, ione_minus_y, channel);
					}
				}

				prev_Src_FloorY = pSrc_FloorY;
				prev_Src_CeilY  = pSrc_CeilY;
			}

			//free allocated memory by fwMalloc
			fwFree(pRow1);
			fwFree(pRow2);

			return;
		}

	}

	//Linear_16u
	namespace{
		//internal function for resize, unsigned short data with Linear interpolation
		static void My_FW_Resize_16u_LN_Op(const Fw16u *pSrc, Fw16u *pDst, int dstStep, 
			int /*resizeWidth*/, 
			Linear_Array_Float *pX_Array_Value, Linear_Array_Float *pY_Array_Value, 
			int xbegin, int xend, int ybegin, int yend, Fw32f *pIfx, Fw32f *pIofx, int channel)
		{
			int channel1, y, x, k;

			//channel 5 is for AC4, which is 4 channel.
			//AC4 Alpha channel data will not be touched
			channel1 = (channel == 5)? 4 :  channel;

			if(channel == 1)
			{
				//Pre-calculate the y coefficient.
				for (y = ybegin; y < yend; y++) {
					Fw16u *pSrc_CeilY  = (Fw16u*)pSrc;
					Fw16u *pSrc_FloorY = (Fw16u*)pSrc;
					pSrc_CeilY  += pY_Array_Value[y].ceil;
					pSrc_FloorY += pY_Array_Value[y].floor;

					float fractY = pY_Array_Value[y].ifraction;
					float one_Minus_Y = pY_Array_Value[y].ione_Minus_Val;

					XMM128 p1Reg, p2Reg, p3Reg, p4Reg, ifxReg, iofxReg, dst, t1, t2, ifyReg, iofyReg;
					ifyReg.i = iofyReg.i = p1Reg.i = p2Reg.i = p3Reg.i = p4Reg.i = _mm_setzero_si128();
					for( int i = 0 ; i < 4 ; i++)
					{
						ifyReg.f32[i] = fractY;
						iofyReg.f32[i] = one_Minus_Y;
					}
					for (x=xbegin; x <= xend-4; x = x+4){			

						for(int i = 0;i < 4; i++)
						{
							p1Reg.f32[i] =  *(pSrc_FloorY + pX_Array_Value[x+i].floor);
							p2Reg.f32[i] =  *(pSrc_FloorY + pX_Array_Value[x+i].ceil);
							p3Reg.f32[i] =  *(pSrc_CeilY + pX_Array_Value[x+i].floor);
							p4Reg.f32[i] =  *(pSrc_CeilY + pX_Array_Value[x+i].ceil);
						}
						ifxReg.i = _mm_loadu_si128((__m128i*)(&pIfx[x]));
						iofxReg.i = _mm_loadu_si128((__m128i*)(&pIofx[x]));

						p1Reg.f = _mm_mul_ps(p1Reg.f, iofxReg.f);
						p2Reg.f = _mm_mul_ps(p2Reg.f, ifxReg.f);
						t1.f = _mm_add_ps(p1Reg.f, p2Reg.f);

						p3Reg.f = _mm_mul_ps(p3Reg.f, iofxReg.f);
						p4Reg.f = _mm_mul_ps(p4Reg.f, ifxReg.f);
						t2.f = _mm_add_ps(p3Reg.f, p4Reg.f);

						t1.f = _mm_mul_ps(t1.f, iofyReg.f);
						t2.f = _mm_mul_ps(t2.f, ifyReg.f);

						dst.f = _mm_add_ps(t1.f, t2.f);
						dst.i = _mm_cvtps_epi32(dst.f);

						dst.i = _mm_shufflelo_epi16(dst.i, _MM_SHUFFLE(0,0,2,0));
						dst.i = _mm_shufflehi_epi16(dst.i, _MM_SHUFFLE(0,0,2,0));

						dst.i = _mm_shuffle_epi32(dst.i, _MM_SHUFFLE(0,0,2,0));
						_mm_storel_epi64((__m128i *)(pDst + y*dstStep + x), dst.i);
					}

					for (; x < xend; x++){			
						int ceil_X =pX_Array_Value[x].ceil;
						int floor_X=pX_Array_Value[x].floor;
						float fraction_X  = pX_Array_Value[x].ifraction;
						float one_Minus_X = pX_Array_Value[x].ione_Minus_Val;

						// resize
						Fw16u p1 = *(pSrc_FloorY + floor_X);
						Fw16u p2 = *(pSrc_FloorY + ceil_X);
						Fw16u p3 = *(pSrc_CeilY  + floor_X);
						Fw16u p4 = *(pSrc_CeilY  + ceil_X);

						//line y
						Fw32f t1 = one_Minus_X * p1 + fraction_X * p2;
						//line y+1
						Fw32f t2 = one_Minus_X * p3 + fraction_X * p4;

						//saturate the result for corresponding data type
						*(pDst + y*dstStep + x) = 
							FW_REF::Limits<Fw16u>::Sat(one_Minus_Y*t1 + fractY*t2 + 0.5);

					}
				}
			}
			else if(channel == 3 || channel == 5)
			{
				//channel 5 is for AC4, which is 4 channel.
				//AC4 Alpha channel data will not be touched

				//Pre-calculate the y coefficient.
				for (y = ybegin; y < yend; y++) {
					Fw16u *pSrc_CeilY  = (Fw16u*)pSrc;
					Fw16u *pSrc_FloorY = (Fw16u*)pSrc;
					pSrc_CeilY  += pY_Array_Value[y].ceil;
					pSrc_FloorY += pY_Array_Value[y].floor;

					float fractY = pY_Array_Value[y].ifraction;
					float one_Minus_Y = pY_Array_Value[y].ione_Minus_Val;

					XMM128 p1Reg, p2Reg, p3Reg, p4Reg, ifxReg, iofxReg, dst, t1, t2, ifyReg, iofyReg;
					ifyReg.i = iofyReg.i = p1Reg.i = p2Reg.i = p3Reg.i = p4Reg.i = _mm_setzero_si128();
					for( int i = 0 ; i < 4 ; i++)
					{
						ifyReg.f32[i] = fractY;
						iofyReg.f32[i] = one_Minus_Y;
					}
					for (x=xbegin; x < xend-1; x++){			

						p1Reg.i = _mm_loadl_epi64((__m128i*)(pSrc_FloorY + pX_Array_Value[x].floor * channel1));
						p2Reg.i = _mm_loadl_epi64((__m128i*)(pSrc_FloorY + pX_Array_Value[x].ceil * channel1));
						p3Reg.i = _mm_loadl_epi64((__m128i*)(pSrc_CeilY + pX_Array_Value[x].floor * channel1));
						p4Reg.i = _mm_loadl_epi64((__m128i*)(pSrc_CeilY + pX_Array_Value[x].ceil * channel1));

						p1Reg.i = _mm_unpacklo_epi16(p1Reg.i, _mm_setzero_si128());
						p2Reg.i = _mm_unpacklo_epi16(p2Reg.i, _mm_setzero_si128());
						p3Reg.i = _mm_unpacklo_epi16(p3Reg.i, _mm_setzero_si128());
						p4Reg.i = _mm_unpacklo_epi16(p4Reg.i, _mm_setzero_si128());

						p1Reg.f = _mm_cvtepi32_ps(p1Reg.i);
						p2Reg.f = _mm_cvtepi32_ps(p2Reg.i);
						p3Reg.f = _mm_cvtepi32_ps(p3Reg.i);
						p4Reg.f = _mm_cvtepi32_ps(p4Reg.i);

						ifxReg.i = _mm_loadu_si128((__m128i*)(&pIfx[x*channel1]));
						iofxReg.i = _mm_loadu_si128((__m128i*)(&pIofx[x*channel1]));

						p1Reg.f = _mm_mul_ps(p1Reg.f, iofxReg.f);
						p2Reg.f = _mm_mul_ps(p2Reg.f, ifxReg.f);
						t1.f = _mm_add_ps(p1Reg.f, p2Reg.f);

						p3Reg.f = _mm_mul_ps(p3Reg.f, iofxReg.f);
						p4Reg.f = _mm_mul_ps(p4Reg.f, ifxReg.f);
						t2.f = _mm_add_ps(p3Reg.f, p4Reg.f);

						t1.f = _mm_mul_ps(t1.f, iofyReg.f);
						t2.f = _mm_mul_ps(t2.f, ifyReg.f);

						dst.f = _mm_add_ps(t1.f, t2.f);
						dst.i = _mm_cvtps_epi32(dst.f);

						dst.i = _mm_shufflelo_epi16(dst.i, _MM_SHUFFLE(0,0,2,0));
						dst.i = _mm_shufflehi_epi16(dst.i, _MM_SHUFFLE(0,0,2,0));

						dst.i = _mm_shuffle_epi32(dst.i, _MM_SHUFFLE(0,0,2,0));

						*((Fw32u*)(pDst + y*dstStep + x*channel1)) = dst.u32[0];
						*(pDst + y*dstStep + x*channel1+2) = dst.u16[4];					
					}

					for (; x < xend; x++){			
						int ceil_X =pX_Array_Value[x].ceil;
						int floor_X=pX_Array_Value[x].floor;
						float fraction_X  = pX_Array_Value[x].ifraction;
						float one_Minus_X = pX_Array_Value[x].ione_Minus_Val;

						// resize
						for(k=0; k<3 ; k++)
						{
							Fw16u p1 = *(pSrc_FloorY + floor_X * channel1 + k);
							Fw16u p2 = *(pSrc_FloorY + ceil_X * channel1 + k);
							Fw16u p3 = *(pSrc_CeilY  + floor_X * channel1 + k);
							Fw16u p4 = *(pSrc_CeilY  + ceil_X * channel1 + k);

							//line y
							Fw32f t1 = one_Minus_X * p1 + fraction_X * p2;
							//line y+1
							Fw32f t2 = one_Minus_X * p3 + fraction_X * p4;

							//saturate the result for corresponding data type
							*(pDst + y*dstStep + x * channel1 + k) = 
								FW_REF::Limits<Fw16u>::Sat(one_Minus_Y*t1 + fractY*t2 + 0.5);
						}					
					}
				}
			}

			else if(channel == 4)
			{
				//Pre-calculate the y coefficient.
				for (y = ybegin; y < yend; y++) {
					Fw16u *pSrc_CeilY  = (Fw16u*)pSrc;
					Fw16u *pSrc_FloorY = (Fw16u*)pSrc;
					pSrc_CeilY  += pY_Array_Value[y].ceil;
					pSrc_FloorY += pY_Array_Value[y].floor;

					float fractY = pY_Array_Value[y].ifraction;
					float one_Minus_Y = pY_Array_Value[y].ione_Minus_Val;

					XMM128 p1Reg, p2Reg, p3Reg, p4Reg, ifxReg, iofxReg, dst, t1, t2, ifyReg, iofyReg;
					ifyReg.i = iofyReg.i = p1Reg.i = p2Reg.i = p3Reg.i = p4Reg.i = _mm_setzero_si128();
					for( int i = 0 ; i < 4 ; i++)
					{
						ifyReg.f32[i] = fractY;
						iofyReg.f32[i] = one_Minus_Y;
					}
					for (x=xbegin; x < xend; x++){			

						p1Reg.i = _mm_loadl_epi64((__m128i*)(pSrc_FloorY + pX_Array_Value[x].floor * channel));
						p2Reg.i = _mm_loadl_epi64((__m128i*)(pSrc_FloorY + pX_Array_Value[x].ceil * channel));
						p3Reg.i = _mm_loadl_epi64((__m128i*)(pSrc_CeilY + pX_Array_Value[x].floor * channel));
						p4Reg.i = _mm_loadl_epi64((__m128i*)(pSrc_CeilY + pX_Array_Value[x].ceil * channel));

						p1Reg.i = _mm_unpacklo_epi16(p1Reg.i, _mm_setzero_si128());
						p2Reg.i = _mm_unpacklo_epi16(p2Reg.i, _mm_setzero_si128());
						p3Reg.i = _mm_unpacklo_epi16(p3Reg.i, _mm_setzero_si128());
						p4Reg.i = _mm_unpacklo_epi16(p4Reg.i, _mm_setzero_si128());

						p1Reg.f = _mm_cvtepi32_ps(p1Reg.i);
						p2Reg.f = _mm_cvtepi32_ps(p2Reg.i);
						p3Reg.f = _mm_cvtepi32_ps(p3Reg.i);
						p4Reg.f = _mm_cvtepi32_ps(p4Reg.i);

						ifxReg.i = _mm_loadu_si128((__m128i*)(&pIfx[x*channel]));
						iofxReg.i = _mm_loadu_si128((__m128i*)(&pIofx[x*channel]));

						p1Reg.f = _mm_mul_ps(p1Reg.f, iofxReg.f);
						p2Reg.f = _mm_mul_ps(p2Reg.f, ifxReg.f);
						t1.f = _mm_add_ps(p1Reg.f, p2Reg.f);

						p3Reg.f = _mm_mul_ps(p3Reg.f, iofxReg.f);
						p4Reg.f = _mm_mul_ps(p4Reg.f, ifxReg.f);
						t2.f = _mm_add_ps(p3Reg.f, p4Reg.f);

						t1.f = _mm_mul_ps(t1.f, iofyReg.f);
						t2.f = _mm_mul_ps(t2.f, ifyReg.f);

						dst.f = _mm_add_ps(t1.f, t2.f);
						dst.i = _mm_cvtps_epi32(dst.f);

						dst.i = _mm_shufflelo_epi16(dst.i, _MM_SHUFFLE(0,0,2,0));
						dst.i = _mm_shufflehi_epi16(dst.i, _MM_SHUFFLE(0,0,2,0));

						dst.i = _mm_shuffle_epi32(dst.i, _MM_SHUFFLE(0,0,2,0));
						_mm_storel_epi64((__m128i *)(pDst + y*dstStep + x*channel), dst.i);

					}
				}
			}	
		}

		//internal function for mutithreading unsigned short data, Linear interpolation
		static void My_FW_Resize_16u_LN_Thread (void *param)
		{
			FW_Resize_16u_Linear *par = (FW_Resize_16u_Linear *) param;

			My_FW_Resize_16u_LN_Op (par->pSrc, par->pDst, par->dstStep, par->resizeWidth, 
				par->pX_Array_Value, par->pY_Array_Value, par->xBegin, par->xEnd, par->yBegin, par->yEnd, 
				par->pIfx, par->pIofx, par->channel);

			return;
		}
	}

	//32f linear interpolation
	namespace{

		static void My_FW_Resize_32f_LN_Op(const Fw32f *pSrc, Fw32f *pDst, int dstStep, 
			int resizeWidth, 
			Linear_Array_Float *pX_Array_Value, Linear_Array_Float *pY_Array_Value, 
			int xbegin, int xend, int ybegin, int yend, Fw32f *pIfx, Fw32f *pIofx, int channel)
		{
			int channel1, y, x, k;

			//channel 5 is for AC4, which is 4 channel.
			//AC4 Alpha channel data will not be touched
			channel1 = (channel == 5)? 4 :  channel;

			if(channel == 1)
			{
				//Pre-calculate the y coefficient.
				Fw32f *prev_CeilY = 0;
				//use fwMalloc instead of malloc for aligned address
				Fw32f *prev_Ceil_Row = (Fw32f*)fwMalloc(resizeWidth*sizeof(Fw32f));
				for (y = ybegin; y < yend; y++) {
					Fw32f *pSrc_CeilY  = (Fw32f*)pSrc;
					Fw32f *pSrc_FloorY = (Fw32f*)pSrc;
					pSrc_CeilY  += pY_Array_Value[y].ceil;
					pSrc_FloorY += pY_Array_Value[y].floor;

					float fractY = pY_Array_Value[y].ifraction;
					float one_Minus_Y = pY_Array_Value[y].ione_Minus_Val;


					XMM128 p1Reg, p2Reg, p3Reg, p4Reg, ifxReg, iofxReg, dst, t1, t2, ifyReg, iofyReg;
					ifyReg.i = iofyReg.i = p1Reg.i = p2Reg.i = p3Reg.i = p4Reg.i = _mm_setzero_si128();
					for( int i = 0 ; i < 4 ; i++)
					{
						ifyReg.f32[i] = fractY;
						iofyReg.f32[i] = one_Minus_Y;
					}
					if(prev_CeilY == pSrc_FloorY)
					{
						for (x=xbegin; x <= xend-4; x = x+4){			

							for(int i = 0;i < 4; i++)
							{

								p3Reg.f32[i] =  *(pSrc_CeilY + pX_Array_Value[x+i].floor);
								p4Reg.f32[i] =  *(pSrc_CeilY + pX_Array_Value[x+i].ceil);
							}

							ifxReg.i = _mm_load_si128((__m128i*)(&pIfx[(x-xbegin)]));
							iofxReg.i = _mm_load_si128((__m128i*)(&pIofx[(x-xbegin)]));


							t1.i = _mm_load_si128((__m128i*)(prev_Ceil_Row + x - xbegin));

							p3Reg.f = _mm_mul_ps(p3Reg.f, iofxReg.f);
							p4Reg.f = _mm_mul_ps(p4Reg.f, ifxReg.f);
							t2.f = _mm_add_ps(p3Reg.f, p4Reg.f);


							_mm_store_si128((__m128i*)(prev_Ceil_Row + x - xbegin), t2.i);

							t1.f = _mm_mul_ps(t1.f, iofyReg.f);
							t2.f = _mm_mul_ps(t2.f, ifyReg.f);

							dst.f = _mm_add_ps(t1.f, t2.f);

							_mm_storeu_si128((__m128i *)(pDst + y*dstStep + x), dst.i);
							prev_CeilY = pSrc_CeilY;

						}

					}
					else
					{
						for (x=xbegin; x <= xend-4; x = x+4){			

							for(int i = 0;i < 4; i++)
							{
								p1Reg.f32[i] =  *(pSrc_FloorY + pX_Array_Value[x+i].floor);
								p2Reg.f32[i] =  *(pSrc_FloorY + pX_Array_Value[x+i].ceil);
								p3Reg.f32[i] =  *(pSrc_CeilY + pX_Array_Value[x+i].floor);
								p4Reg.f32[i] =  *(pSrc_CeilY + pX_Array_Value[x+i].ceil);
							}

							ifxReg.i = _mm_load_si128((__m128i*)(&pIfx[(x-xbegin)]));
							iofxReg.i = _mm_load_si128((__m128i*)(&pIofx[(x-xbegin)]));

							p1Reg.f = _mm_mul_ps(p1Reg.f, iofxReg.f);
							p2Reg.f = _mm_mul_ps(p2Reg.f, ifxReg.f);
							t1.f = _mm_add_ps(p1Reg.f, p2Reg.f);

							p3Reg.f = _mm_mul_ps(p3Reg.f, iofxReg.f);
							p4Reg.f = _mm_mul_ps(p4Reg.f, ifxReg.f);
							t2.f = _mm_add_ps(p3Reg.f, p4Reg.f);

							_mm_store_si128((__m128i*)(prev_Ceil_Row + x - xbegin), t2.i);

							t1.f = _mm_mul_ps(t1.f, iofyReg.f);
							t2.f = _mm_mul_ps(t2.f, ifyReg.f);

							dst.f = _mm_add_ps(t1.f, t2.f);

							_mm_storeu_si128((__m128i *)(pDst + y*dstStep + x), dst.i);
							prev_CeilY = pSrc_CeilY;

						}

					}

					for (; x < xend; x++){			
						int ceil_X =pX_Array_Value[x].ceil;
						int floor_X=pX_Array_Value[x].floor;
						float fraction_X  = pX_Array_Value[x].ifraction;
						float one_Minus_X = pX_Array_Value[x].ione_Minus_Val;

						// resize

						Fw32f p1 = *(pSrc_FloorY + floor_X);
						Fw32f p2 = *(pSrc_FloorY + ceil_X);
						Fw32f p3 = *(pSrc_CeilY  + floor_X);
						Fw32f p4 = *(pSrc_CeilY  + ceil_X);

						//line y
						Fw32f t1 = one_Minus_X * p1 + fraction_X * p2;
						//line y+1
						Fw32f t2 = one_Minus_X * p3 + fraction_X * p4;

						//saturate the result for corresponding data type
						*(pDst + y*dstStep + x) = 
							FW_REF::Limits<Fw32f>::Sat(one_Minus_Y*t1 + fractY*t2);

					}
				}

				//free allocated memory by fwMalloc
				fwFree(prev_Ceil_Row);
			}
			else if(channel == 3 || channel == 5)
			{
				//channel 5 is for AC4, which is 4 channel.
				//AC4 Alpha channel data will not be touched

				//Pre-calculate the y coefficient.
				for (y = ybegin; y < yend; y++) {
					Fw32f *pSrc_CeilY  = (Fw32f*)pSrc;
					Fw32f *pSrc_FloorY = (Fw32f*)pSrc;
					pSrc_CeilY  += pY_Array_Value[y].ceil;
					pSrc_FloorY += pY_Array_Value[y].floor;

					float fractY = pY_Array_Value[y].ifraction;
					float one_Minus_Y = pY_Array_Value[y].ione_Minus_Val;


					XMM128 p1Reg, p2Reg, p3Reg, p4Reg, ifxReg, iofxReg, dst, t1, t2, ifyReg, iofyReg;
					ifyReg.i = iofyReg.i = p1Reg.i = p2Reg.i = p3Reg.i = p4Reg.i = _mm_setzero_si128();
					for( int i = 0 ; i < 4 ; i++)
					{
						ifyReg.f32[i] = fractY;
						iofyReg.f32[i] = one_Minus_Y;
					}
					for (x=xbegin; x < xend-1; x++){			


						p1Reg.i = _mm_loadu_si128((__m128i*)(pSrc_FloorY + pX_Array_Value[x].floor * channel1));
						p2Reg.i = _mm_loadu_si128((__m128i*)(pSrc_FloorY + pX_Array_Value[x].ceil * channel1));
						p3Reg.i = _mm_loadu_si128((__m128i*)(pSrc_CeilY + pX_Array_Value[x].floor * channel1));
						p4Reg.i = _mm_loadu_si128((__m128i*)(pSrc_CeilY + pX_Array_Value[x].ceil * channel1));



						ifxReg.i = _mm_loadu_si128((__m128i*)(&pIfx[(x-xbegin)*channel1]));
						iofxReg.i = _mm_loadu_si128((__m128i*)(&pIofx[(x-xbegin)*channel1]));


						p1Reg.f = _mm_mul_ps(p1Reg.f, iofxReg.f);
						p2Reg.f = _mm_mul_ps(p2Reg.f, ifxReg.f);
						t1.f = _mm_add_ps(p1Reg.f, p2Reg.f);

						p3Reg.f = _mm_mul_ps(p3Reg.f, iofxReg.f);
						p4Reg.f = _mm_mul_ps(p4Reg.f, ifxReg.f);
						t2.f = _mm_add_ps(p3Reg.f, p4Reg.f);

						t1.f = _mm_mul_ps(t1.f, iofyReg.f);
						t2.f = _mm_mul_ps(t2.f, ifyReg.f);

						dst.f = _mm_add_ps(t1.f, t2.f);


						_mm_storel_epi64((__m128i *)(pDst + y*dstStep + x*channel1), dst.i);
						*(pDst + y*dstStep + x*channel1+2) = dst.f32[2];
					}

					for (; x < xend; x++){			
						int ceil_X =pX_Array_Value[x].ceil;
						int floor_X=pX_Array_Value[x].floor;
						float fraction_X  = pX_Array_Value[x].ifraction;
						float one_Minus_X = pX_Array_Value[x].ione_Minus_Val;

						// resize
						for(k=0; k<3 ; k++)
						{

							Fw32f p1 = *(pSrc_FloorY + floor_X * channel1 + k);
							Fw32f p2 = *(pSrc_FloorY + ceil_X * channel1 + k);
							Fw32f p3 = *(pSrc_CeilY  + floor_X * channel1 + k);
							Fw32f p4 = *(pSrc_CeilY  + ceil_X * channel1 + k);

							//line y
							Fw32f t1 = one_Minus_X * p1 + fraction_X * p2;
							//line y+1
							Fw32f t2 = one_Minus_X * p3 + fraction_X * p4;

							//saturate the result for corresponding data type
							*(pDst + y*dstStep + x * channel1 + k) = 
								FW_REF::Limits<Fw32f>::Sat(one_Minus_Y*t1 + fractY*t2 + 0.5);
						}

					}
				}
			}

			else if(channel == 4)
			{
				//Pre-calculate the y coefficient.
				for (y = ybegin; y < yend; y++) {
					Fw32f *pSrc_CeilY  = (Fw32f*)pSrc;
					Fw32f *pSrc_FloorY = (Fw32f*)pSrc;
					pSrc_CeilY  += pY_Array_Value[y].ceil;
					pSrc_FloorY += pY_Array_Value[y].floor;

					float fractY = pY_Array_Value[y].ifraction;
					float one_Minus_Y = pY_Array_Value[y].ione_Minus_Val;


					XMM128 p1Reg, p2Reg, p3Reg, p4Reg, ifxReg, iofxReg, dst, t1, t2, ifyReg, iofyReg;
					ifyReg.i = iofyReg.i = p1Reg.i = p2Reg.i = p3Reg.i = p4Reg.i = _mm_setzero_si128();
					for( int i = 0 ; i < 4 ; i++)
					{
						ifyReg.f32[i] = fractY;
						iofyReg.f32[i] = one_Minus_Y;
					}
					for (x=xbegin; x < xend; x++){			
						p1Reg.i = _mm_loadu_si128((__m128i*)(pSrc_FloorY + pX_Array_Value[x].floor * channel));
						p2Reg.i = _mm_loadu_si128((__m128i*)(pSrc_FloorY + pX_Array_Value[x].ceil * channel));
						p3Reg.i = _mm_loadu_si128((__m128i*)(pSrc_CeilY + pX_Array_Value[x].floor * channel));
						p4Reg.i = _mm_loadu_si128((__m128i*)(pSrc_CeilY + pX_Array_Value[x].ceil * channel));

						ifxReg.i = _mm_load_si128((__m128i*)(&pIfx[(x-xbegin)*channel]));
						iofxReg.i = _mm_load_si128((__m128i*)(&pIofx[(x-xbegin)*channel]));

						p1Reg.f = _mm_mul_ps(p1Reg.f, iofxReg.f);
						p2Reg.f = _mm_mul_ps(p2Reg.f, ifxReg.f);
						t1.f = _mm_add_ps(p1Reg.f, p2Reg.f);

						p3Reg.f = _mm_mul_ps(p3Reg.f, iofxReg.f);
						p4Reg.f = _mm_mul_ps(p4Reg.f, ifxReg.f);
						t2.f = _mm_add_ps(p3Reg.f, p4Reg.f);

						t1.f = _mm_mul_ps(t1.f, iofyReg.f);
						t2.f = _mm_mul_ps(t2.f, ifyReg.f);

						dst.f = _mm_add_ps(t1.f, t2.f);
						_mm_storeu_si128((__m128i *)(pDst + y*dstStep + x*channel), dst.i);

					}
				}
			}
		}

		//internal function for mutithreading float data, Linear interpolation
		static void My_FW_Resize_32f_LN_Thread (void *param)
		{
			FW_Resize_32f_Linear *par = (FW_Resize_32f_Linear *) param;

			My_FW_Resize_32f_LN_Op (par->pSrc, par->pDst, par->dstStep, par->resizeWidth, 
				par->pX_Array_Value, par->pY_Array_Value, par->xBegin, par->xEnd, par->yBegin, par->yEnd, 
				par->pIfx, par->pIofx, par->channel);

			return;
		}
	}//end namespace

	static void My_FW_Resize_8u_NN_Thread (void *param);

	static void My_FW_Resize_8u_NN_Op1(const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, 
		int resizeWidth, int *px_NN, int *py_NN, int xBegin, int xEnd, int yBegin, int yEnd);

	static void My_FW_Resize_8u_NN_Op(const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, 
		int resizeWidth, int *px_NN, int *py_NN, int xBegin, int xEnd,
		int yBegin, int yEnd, int channel);

	//internal function for threading 8u data, NN interpolation
	void My_FW_Resize_8u_NN_Thread (void *param)
	{
		FW_Resize_8u_NN *par = (FW_Resize_8u_NN *) param;

		if (par->channel == 1) {
			//Special handling for 1 channel data, since we do not need to loop for channels
			My_FW_Resize_8u_NN_Op1 (par->pSrc, par->srcStep, par->pDst, par->dstStep, par->resizeWidth, 
				par->px_NN, par->py_NN, par->xBegin, par->xEnd, par->yBegin, par->yEnd);
		} else {
			My_FW_Resize_8u_NN_Op (par->pSrc, par->srcStep, par->pDst, par->dstStep, par->resizeWidth, 
				par->px_NN, par->py_NN, par->xBegin, par->xEnd, par->yBegin, par->yEnd, par->channel);
		}

		return;
	}

	//internal function for handling 8u 1 channel with Nearear Neighbor interpolcation
	//Special handling for 1 channel data, since we do not need to loop for channels
	void My_FW_Resize_8u_NN_Op1(const Fw8u *pSrc, int , Fw8u *pDst, int dstStep, 
		int /*resizeWidth*/, int *px_NN, int *py_NN, int xBegin, int xEnd, int yBegin, int yEnd)
	{
		//fwMalloc allocated 32-byte alligned data
		Fw8u *pSrc_y, *pDst_y;
		//_MM_ALIGN16 Fw8u pp[16];
		XMM128 pp={0};
		int x, y, xx;

		//To calculate the pixel value
		for (y = yBegin; y < yEnd; y++) {

			pSrc_y = (Fw8u*)pSrc+py_NN[y];
			pDst_y = pDst + y*dstStep;


			for (x=xBegin ; x <= xEnd-16; x+=16)	{
				// process 16 pixels in parallel						
				for (xx = 0; xx < 16; xx++)	{
					pp.u8[xx] = *(pSrc_y+px_NN[xx+x]);
				}

				_mm_storeu_si128((__m128i *)(pDst_y + x), pp.i);
			}

			//The remainder part use reference code
			if (x<xEnd) {
				for (;x<xEnd;x++) {
					*(pDst+y * dstStep+x) = *(pSrc_y+px_NN[x]);
				}
			}
		}

		return;
	}

	void My_FW_Resize_8u_NN_Op(const Fw8u *pSrc, int , Fw8u *pDst, int dstStep, 
		int /*resizeWidth*/, int *px_NN, int *py_NN, 
		int xbegin, int xend,
		int ybegin, int yend, int channel)
	{
		int x,y;

		int channel1;
		channel1 = channel;

		if(channel == 1)
		{
			for (y=ybegin; y<yend;y++) 
			{
				for (x=xbegin;x<xend;x++)
				{
					*(pDst+y * dstStep+x * channel) = 
						*(pSrc+py_NN[y] +px_NN[x] * channel);
				}
			}
		}
		else if(channel == 3)
		{
			Fw32u result32;
			Fw16u *pRes16 = (Fw16u *) (&result32);
			Fw8u *pRes8 = ((Fw8u *) (&result32)) + 2;
			XMM128 dst[4];
			for (y=ybegin; y<yend;y++) 
			{
				for (x=xbegin;x<=xend-16;x=x+16)
				{
					dst[0].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x]*channel)));
					dst[0].i = _mm_slli_si128(dst[0].i, 13);
					dst[0].i = _mm_srli_si128(dst[0].i, 13);


					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+1]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 10);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+2]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 7);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+3]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 4);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+4]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 1);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+5]*channel)));
					dst[2].i = _mm_slli_si128(dst[1].i, 15);

					dst[0].i = _mm_or_si128(dst[0].i, dst[2].i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel), dst[0].i);

					//2 reg
					dst[0].i = _mm_slli_si128(dst[1].i, 13);
					dst[0].i = _mm_srli_si128(dst[0].i, 14);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+6]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 11);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+7]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 8);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+8]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 5);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+9]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 2);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+10]*channel)));
					dst[2].i = _mm_slli_si128(dst[1].i, 14);

					dst[0].i = _mm_or_si128(dst[0].i, dst[2].i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel+16), dst[0].i);

					//3 reg
					dst[0].i = _mm_slli_si128(dst[1].i, 13);
					dst[0].i = _mm_srli_si128(dst[0].i, 15);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+11]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 12);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+12]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 9);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+13]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 6);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+14]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);
					dst[1].i = _mm_srli_si128(dst[1].i, 3);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+15]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 13);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel+32), dst[0].i);
				}

				if (x<xend) {
					for (;x<xend;x++) {
						result32 = *((Fw32u *)(pSrc+py_NN[y]+px_NN[x]*channel));
						*((Fw16u*)(pDst+y * dstStep+x * channel)) = *pRes16;
						*(pDst+y * dstStep+x * channel + 2) = *pRes8;
					}
				}
			}
		}
		else if(channel == 5)
		{
			//channel 5 is for AC4, which is 4 channel.
			//AC4 Alpha channel data will not be touched
			if (channel == 5) { channel1 = 3, channel =4;}

			Fw32u result32;
			Fw16u *pRes16 = (Fw16u *) (&result32);
			Fw8u *pRes8 = ((Fw8u *) (&result32)) + 2;

			XMM128 dst[2], src, mask[2];
			mask[0].i = _mm_set_epi32(0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000);
			mask[1].i = _mm_set_epi32(0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF);

			for (y=ybegin; y<yend;y++) 
			{
				for (x=xbegin;x<=xend-4;x=x+4)
				{
					dst[0].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x]*channel)));

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+1]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 4);
					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+2]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 8);
					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+3]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 12);
					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					src.i = _mm_loadu_si128((__m128i *)(pDst + y * dstStep+ x * channel));

					dst[0].i = _mm_and_si128(dst[0].i, mask[1].i);
					src.i = _mm_and_si128(src.i, mask[0].i);
					dst[0].i = _mm_or_si128(dst[0].i, src.i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel), dst[0].i);
				}

				if (x<xend) {
					for (;x<xend;x++) {
						result32 = *((Fw32u *)(pSrc+py_NN[y]+px_NN[x]*channel));
						*((Fw16u*)(pDst+y * dstStep+x * channel)) = *pRes16;
						*(pDst+y * dstStep+x * channel + 2) = *pRes8;
					}
				}
			}
		}

		else if(channel == 4)
		{
			XMM128 dst[2];
			for (y=ybegin; y<yend;y++) 
			{
				for (x=xbegin;x<=xend-4;x=x+4)
				{
					dst[0].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x]*channel)));

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+1]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 4);
					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+2]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 8);
					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+3]*channel)));
					dst[1].i = _mm_slli_si128(dst[1].i, 12);
					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel), dst[0].i);
				}

				if (x<xend) {
					for (;x<xend;x++) {
						*((Fw32u*)(pDst+y * dstStep+x * channel)) = *((Fw32u *)(pSrc+py_NN[y]+px_NN[x]*channel));
					}
				}
			}
		}

		return;
	}

	//internal function for Row handling for 8u data, NN mode
	static void FW_Resize_8u_NN_row_REF(Fw8u *pSrc_y, Fw8u *pDst_y, int xstart, int xend, 
		int *px_NN, int channel)
	{
		int x;

		//pSrc_y, pDst_y positions are pre-calculated
		if (channel == 1) {
			for (x=xstart; x < xend; x++){
				*(pDst_y + x) = *(pSrc_y + px_NN[x]);
			}
		}
		else if (channel == 4) {
			Fw32u *pDst32 = (Fw32u *)pDst_y;
			Fw32u *pSrc32 = (Fw32u *)pSrc_y;

			for (x=xstart; x < xend; x++){			
				//memcpy(pDst_y+x*channel, pSrc_y+px_NN[x]*channel, channel);
				*(pDst32+x) = *(pSrc32+px_NN[x]);
			}
		}
		else {	//c3 and AC4
			if (channel==5) channel=4;
			//Fw16u *pDst16 = (Fw16u *)pDst_y;
			Fw32u result32;
			Fw16u *pRes16 = (Fw16u *) (&result32);
			//The third element
			Fw8u *pRes8 = ((Fw8u *) (&result32)) + 2;

			for (x=xstart; x < xend; x++){
				//*(pDst_y + x*channel)     = *(pSrc_y + px_NN[x]*channel);
				//*(pDst_y + x*channel + 1) = *(pSrc_y + px_NN[x]*channel + 1);
				//*(pDst_y + x*channel + 2) = *(pSrc_y + px_NN[x]*channel + 2);
				result32 = * ((Fw32u *)(pSrc_y+px_NN[x]*channel));
				*((Fw16u *)(pDst_y + x*channel)) = *pRes16;
				*(pDst_y + x*channel + 2) = *pRes8;
			}
		}

		return;
	}


	//Resize_16u
	static void My_FW_Resize_16u_NN_Op1(const Fw16u *pSrc, int , Fw16u *pDst, int dstStep, 
		int /*resizeWidth*/, int *px_NN, int *py_NN, int xBegin, int xEnd, int yBegin, int yEnd)
	{
		//fwMalloc allocated 32-byte alligned data
		Fw16u *pSrc_y, *pDst_y;
		//_MM_ALIGN16 Fw16u pp[8];
		XMM128 pp={0};
		int x, y, xx;

		//To calculate the pixel value
		for (y = yBegin; y < yEnd; y++) {

			pSrc_y = (Fw16u*)pSrc+py_NN[y];
			pDst_y = pDst + y*dstStep;

			for (x=xBegin ; x <= xEnd-8; x+=8)	{			
				for (xx = 0; xx < 8; xx++)	{
					pp.u16[xx] = *(pSrc_y+px_NN[xx+x]);
				}

				_mm_storeu_si128((__m128i *)(pDst_y + x), pp.i);
			}

			//The remainder part use reference code
			if (x<xEnd) {
				for (;x<xEnd;x++) {
					*(pDst+y * dstStep+x) = *(pSrc_y+px_NN[x]);
				}
			}
		}

		return;
	}


	static void My_FW_Resize_16u_NN_Op(const Fw16u *pSrc, int , Fw16u *pDst, int dstStep, 
		int /*resizeWidth*/, int *px_NN, int *py_NN, 
		int xbegin, int xend,
		int ybegin, int yend, int channel)
	{
		int x,y;

		int channel1;
		channel1 = channel;

		if(channel == 3)
		{
			XMM128 dst[4];
			for (y=ybegin; y<yend;y++) 
			{
				for (x=xbegin;x<=xend-8;x=x+8)
				{
					dst[0].i = _mm_loadl_epi64 ((__m128i *)(pSrc+py_NN[y]+px_NN[x]*channel));
					dst[0].i = _mm_slli_si128(dst[0].i, 10);
					dst[0].i = _mm_srli_si128(dst[0].i, 10);

					dst[1].i = _mm_loadl_epi64 ((__m128i *)(pSrc+py_NN[y]+px_NN[x+1]*channel));
					dst[1].i = _mm_slli_si128(dst[1].i, 10);
					dst[1].i = _mm_srli_si128(dst[1].i, 4);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_loadl_epi64 ((__m128i *)(pSrc+py_NN[y]+px_NN[x+2]*channel));
					dst[1].i = _mm_slli_si128(dst[1].i, 12);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel), dst[0].i);

					dst[0].i = _mm_loadl_epi64 ((__m128i *)(pSrc+py_NN[y]+px_NN[x+2]*channel+2));
					dst[0].i = _mm_slli_si128(dst[0].i, 10);
					dst[0].i = _mm_srli_si128(dst[0].i, 10);

					dst[1].i = _mm_loadl_epi64 ((__m128i *)(pSrc+py_NN[y]+px_NN[x+3]*channel+2));
					dst[1].i = _mm_slli_si128(dst[1].i, 10);
					dst[1].i = _mm_srli_si128(dst[1].i, 4);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+4]*channel+2)));
					dst[1].i = _mm_slli_si128(dst[1].i, 12);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel+8), dst[0].i);

					//3 reg
					dst[0].i = _mm_loadl_epi64 ((__m128i *)(pSrc+py_NN[y]+px_NN[x+5]*channel+1));
					dst[0].i = _mm_slli_si128(dst[0].i, 10);
					dst[0].i = _mm_srli_si128(dst[0].i, 10);

					dst[1].i = _mm_loadl_epi64 ((__m128i *)(pSrc+py_NN[y]+px_NN[x+6]*channel+1));
					dst[1].i = _mm_slli_si128(dst[1].i, 10);
					dst[1].i = _mm_srli_si128(dst[1].i, 4);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					dst[1].i = _mm_cvtsi32_si128(*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+7]*channel+1)));
					dst[1].i = _mm_slli_si128(dst[1].i, 12);

					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);			

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel+16), dst[0].i);
				}

				if (x<xend) {
					for (;x<xend;x++) {
						for(int i=0;i<3;i++)
							*(pDst+y * dstStep+x * channel +i) = *((pSrc+py_NN[y]+px_NN[x]*channel+i));
					}
				}
			}
		}		
		return;
	}

	//internal function for mutithreading unsigned short data, Nearest Neighbor interpolation
	static void My_FW_Resize_16u_NN_Thread (void *param)
	{
		FW_Resize_16u_NN *par = (FW_Resize_16u_NN *) param;

		if (par->channel == 1) {
			//Special handling for 1 channel data, since we do not need to loop for channels
			My_FW_Resize_16u_NN_Op1 (par->pSrc, par->srcStep, par->pDst, par->dstStep, par->resizeWidth, 
				par->px_NN, par->py_NN, par->xBegin, par->xEnd, par->yBegin, par->yEnd);
		} else {
			My_FW_Resize_16u_NN_Op (par->pSrc, par->srcStep, par->pDst, par->dstStep, par->resizeWidth, 
				par->px_NN, par->py_NN, par->xBegin, par->xEnd, par->yBegin, par->yEnd, par->channel);
		}

		return;
	}

	//Resize_32f
	static void My_FW_Resize_32f_NN_Op1(const Fw32f *pSrc, int , Fw32f *pDst, int dstStep, 
		int /*resizeWidth*/, int *px_NN, int *py_NN, int xBegin, int xEnd, int yBegin, int yEnd)
	{
		//fwMalloc allocated 32-byte alligned data
		Fw32f *pSrc_y, *pDst_y;
		//_MM_ALIGN16 Fw32f pp[4];
		XMM128 pp={0};
		int x, y, xx;

		//To calculate the pixel value
		for (y = yBegin; y < yEnd; y++) {

			pSrc_y = (Fw32f*)pSrc+py_NN[y];
			pDst_y = pDst + y*dstStep;

			for (x=xBegin ; x <= xEnd-4; x+=4)	{
				// process 16 pixels in parallel						
				for (xx = 0; xx < 4; xx++)	{
					pp.f32[xx] = *(pSrc_y+px_NN[xx+x]);
				}

				_mm_storeu_si128((__m128i *)(pDst_y + x), pp.i);
			}

			//The remainder part use reference code
			if (x<xEnd) {
				for (;x<xEnd;x++) {
					*(pDst+y * dstStep+x) = *(pSrc_y+px_NN[x]);
				}
			}
		}

		return;
	}


	static void My_FW_Resize_32f_NN_Op(const Fw32f *pSrc, int , Fw32f *pDst, int dstStep, 
		int /*resizeWidth*/, int *px_NN, int *py_NN, 
		int xbegin, int xend,
		int ybegin, int yend, int channel)
	{
		int x,y;

		int channel1;
		channel1 = channel;

		if(channel == 3)
		{
			XMM128 dst[4];
			for (y=ybegin; y<yend;y++) 
			{
				for (x=xbegin;x<=xend-4;x=x+4)
				{
					dst[0].i = _mm_loadu_si128 ((__m128i *)(pSrc+py_NN[y]+px_NN[x]*channel));
					dst[0].i = _mm_slli_si128(dst[0].i, 4);
					dst[0].i = _mm_srli_si128(dst[0].i, 4);

					dst[1].i = _mm_loadu_si128 ((__m128i *)(pSrc+py_NN[y]+px_NN[x+1]*channel));
					dst[2].i = _mm_slli_si128(dst[1].i, 12);

					dst[0].i = _mm_or_si128(dst[0].i, dst[2].i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel), dst[0].i);

					//2 reg
					dst[0].i = _mm_loadu_si128 ((__m128i *)(pSrc+py_NN[y]+px_NN[x+2]*channel));
					dst[2].i = _mm_slli_si128(dst[0].i, 8);

					dst[1].i = _mm_slli_si128(dst[1].i, 4);
					dst[1].i = _mm_srli_si128(dst[1].i, 8);

					dst[1].i = _mm_or_si128(dst[1].i, dst[2].i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel+4), dst[1].i);

					//3 reg
					dst[2].i = _mm_loadl_epi64 ((__m128i *)(pSrc+py_NN[y]+px_NN[x+3]*channel));
					dst[2].i = _mm_slli_si128(dst[2].i, 8);
					dst[2].i = _mm_srli_si128(dst[2].i, 4);

					dst[0].i = _mm_slli_si128(dst[0].i, 4);
					dst[0].i = _mm_srli_si128(dst[0].i, 12);

					dst[0].i = _mm_or_si128(dst[0].i, dst[2].i);

					dst[1].i = _mm_cvtsi32_si128 (*((Fw32s *)(pSrc+py_NN[y]+px_NN[x+3]*channel+2)));

					dst[1].i = _mm_slli_si128(dst[1].i, 12);
					dst[0].i = _mm_or_si128(dst[0].i, dst[1].i);

					_mm_storeu_si128((__m128i *)(pDst + y * dstStep+ x * channel+8), dst[0].i);
				}

				if (x<xend) {
					for (;x<xend;x++) {
						for(int i=0;i<3;i++)
							*(pDst+y * dstStep+x * channel +i) = *((pSrc+py_NN[y]+px_NN[x]*channel+i));
					}
				}
			}
		}

		return;
	}

	//internal function for mutithreading float data, Nearest Neighbor interpolation
	static void My_FW_Resize_32f_NN_Thread (void *param)
	{
		FW_Resize_32f_NN *par = (FW_Resize_32f_NN *) param;

		if (par->channel == 1) {
			//Special handling for 1 channel data, since we do not need to loop for channels
			My_FW_Resize_32f_NN_Op1 (par->pSrc, par->srcStep, par->pDst, par->dstStep, par->resizeWidth, 
				par->px_NN, par->py_NN, par->xBegin, par->xEnd, par->yBegin, par->yEnd);
		} else {
			My_FW_Resize_32f_NN_Op (par->pSrc, par->srcStep, par->pDst, par->dstStep, par->resizeWidth, 
				par->px_NN, par->py_NN, par->xBegin, par->xEnd, par->yBegin, par->yEnd, par->channel);
		}

		return;
	}

}
#endif //__RESIZE_H
