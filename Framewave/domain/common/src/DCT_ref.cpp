/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//************************************************************************* 
//This file include DCT functions from Linear Transform Chapter
//	fwiDCT8x8Fwd_16s_C1
//	fwiDCT8x8Fwd_16s_C1I
//	fwiDCT8x8Inv_16s_C1
//	fwiDCT8x8Inv_16s_C1I
//************************************************************************* 
#if 0
#include "fwdev.h"
#include "system.h"

#if BUILD_NUM_AT_LEAST( 102 )


//------------------------------------------------------------------------
//Start internal function for DCT
#define PI 3.14159265358979323846
//static float c[8][8]; /* transform coefficients */
void init_idct(float c[8][8])
{
	int i, j;
	float s;

	for (i=0; i<8; i++) {
		s = (float)((i==0) ? sqrt(0.125) : 0.5);

		for (j=0; j<8; j++)
			c[i][j] = s * (float)(cos((PI/8.0)*i*(j+0.5)));
	}

}

void idct(float c[8][8], const Fw16s* pSrc, Fw16s* pDst)
{
	int i, j, k;
	float partial_product;
	float tmp[64];

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
		{
			partial_product = 0.0;

			for (k=0; k<8; k++)
				partial_product+= c[k][j]*pSrc[8*i+k];

			tmp[8*i+j] = partial_product;
		}

		/* Transpose operation is integrated into address mapping by switching 
		loop order of i and j */

		for (j=0; j<8; j++)
			for (i=0; i<8; i++)
			{
				partial_product = 0.0;

				for (k=0; k<8; k++)
					partial_product+= c[k][i]*tmp[8*k+j];

				//v = CBL_LIBRARY::Limits<int>::Sat(floor(partial_product+0.5));
				//pDst[8*i+j] = (Fw16s) CBL_LIBRARY::Limits<Fw16s>::Sat(v);
				pDst[8*i+j] = (Fw16s) CBL_LIBRARY::Limits<Fw16s>::Sat(floor(partial_product+0.5));
			}
}

//-----------------------------------------------------------------------------
#define F (float)

#define W1 F 0.70710678118654752438 // cos(pi*4/16)
#define W2 F 0.54119610014619698435 // cos(pi*6/16)sqrt(2)
#define W5 F 0.38268343236508977170 // cos(pi*6/16)
#define W4 F 1.30656296487637652774 // cos(pi*2/16)sqrt(2)

static float SCALE[64]=
{
	F 1.000000, F 0.720960, F 0.765367, F 0.850430, F 1.000000, F 1.272759, F 1.847759, F 3.624510,
	F 0.720960, F 0.519783, F 0.551799, F 0.613126, F 0.720960, F 0.917608, F 1.332160, F 2.613126,
	F 0.765367, F 0.551799, F 0.585786, F 0.650891, F 0.765367, F 0.974127, F 1.414214, F 2.774080,
	F 0.850430, F 0.613126, F 0.650891, F 0.723231, F 0.850430, F 1.082392, F 1.571390, F 3.082392,
	F 1.000000, F 0.720960, F 0.765367, F 0.850430, F 1.000000, F 1.272759, F 1.847759, F 3.624510,
	F 1.272759, F 0.917608, F 0.974127, F 1.082392, F 1.272759, F 1.619914, F 2.351751, F 4.613126,
	F 1.847759, F 1.332160, F 1.414214, F 1.571390, F 1.847759, F 2.351751, F 3.414214, F 6.697221,
	F 3.624510, F 2.613126, F 2.774080, F 3.082392, F 3.624510, F 4.613126, F 6.697221, F 13.137072
};

void C_faandct(const Fw16s* pSrc, Fw16s* pDst, float round)
{
	float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	float tmp10, tmp11, tmp12, tmp13;
	float temp[64];
	int i;


	for (i=0; i<8*8; i+=8) {
		tmp0= F (pSrc[0 + i] + pSrc[7 + i]);
		tmp7= F (pSrc[0 + i] - pSrc[7 + i]);
		tmp1= F (pSrc[1 + i] + pSrc[6 + i]);
		tmp6= F (pSrc[1 + i] - pSrc[6 + i]);
		tmp2= F (pSrc[2 + i] + pSrc[5 + i]);
		tmp5= F (pSrc[2 + i] - pSrc[5 + i]);
		tmp3= F (pSrc[3 + i] + pSrc[4 + i]);
		tmp4= F (pSrc[3 + i] - pSrc[4 + i]);

		tmp10= tmp0 + tmp3;
		tmp13= tmp0 - tmp3;
		tmp11= tmp1 + tmp2;
		tmp12= tmp1 - tmp2;

		temp[0 + i]= tmp10 + tmp11;
		temp[4 + i]= tmp10 - tmp11;

		tmp12= (tmp12 + tmp13)*W1;
		temp[2 + i]= tmp13 + tmp12;
		temp[6 + i]= tmp13 - tmp12;

		tmp10= tmp4 + tmp5;
		tmp11= tmp5 + tmp6;
		tmp12= tmp6 + tmp7;

		tmp1= (tmp10 - tmp12) * W5;
		tmp2= tmp10*W2 + tmp1;
		tmp4= tmp12*W4 + tmp1;
		tmp3= tmp11*W1;

		tmp11= tmp7 + tmp3;
		tmp13= tmp7 - tmp3;

		temp[5 + i]= tmp13 + tmp2;
		temp[3 + i]= tmp13 - tmp2;
		temp[1 + i]= tmp11 + tmp4;
		temp[7 + i]= tmp11 - tmp4;
	}    

	for (i=0; i<8; i++) {
		tmp0= temp[8*0 + i] + temp[8*7 + i];
		tmp7= temp[8*0 + i] - temp[8*7 + i];
		tmp1= temp[8*1 + i] + temp[8*6 + i];
		tmp6= temp[8*1 + i] - temp[8*6 + i];
		tmp2= temp[8*2 + i] + temp[8*5 + i];
		tmp5= temp[8*2 + i] - temp[8*5 + i];
		tmp3= temp[8*3 + i] + temp[8*4 + i];
		tmp4= temp[8*3 + i] - temp[8*4 + i];

		tmp10= tmp0 + tmp3;
		tmp13= tmp0 - tmp3;
		tmp11= tmp1 + tmp2;
		tmp12= tmp1 - tmp2;

		pDst[8*0 + i]= (short) floor(SCALE[8*0 + i]/8.0 * (tmp10 + tmp11) + round);
		pDst[8*4 + i]= (short) floor(SCALE[8*4 + i]/8.0 * (tmp10 - tmp11) + round);

		tmp12= (tmp12 + tmp13)* W1;
		pDst[8*2 + i]= (short) floor(SCALE[8*2 + i]/8.0 * (tmp13 + tmp12) + round);
		pDst[8*6 + i]= (short) floor(SCALE[8*6 + i]/8.0 * (tmp13 - tmp12) + round);

		tmp10= tmp4 + tmp5;
		tmp11= tmp5 + tmp6;
		tmp12= tmp6 + tmp7;

		tmp1= (tmp10 - tmp12) * W5;
		tmp2= tmp10*W2 + tmp1;
		tmp4= tmp12*W4 + tmp1;
		tmp3= tmp11*W1;

		tmp11= tmp7 + tmp3;
		tmp13= tmp7 - tmp3;

		pDst[8*5 + i]= (short)floor(SCALE[8*5 + i]/8.0 * (tmp13 + tmp2) + round);
		pDst[8*3 + i]= (short)floor(SCALE[8*3 + i]/8.0 * (tmp13 - tmp2) + round);
		pDst[8*1 + i]= (short)floor(SCALE[8*1 + i]/8.0 * (tmp11 + tmp4) + round);
		pDst[8*7 + i]= (short)floor(SCALE[8*7 + i]/8.0 * (tmp11 - tmp4) + round);
	}
}


#endif //BUILD_NUM_AT_LEAST
#endif
