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

#endif //BUILD_NUM_AT_LEAST
#endif
