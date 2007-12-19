/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
#include "fwVideo.h"
#include "system.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST(101)

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiEncodeCoeffsCAVLC_H264_16s)(
	Fw16s *pSrc,         Fw8u AC, 
	Fw32u *pScanMatrix,  Fw8u Count, 
	Fw8u  *Traling_One,  Fw8u *Traling_One_Signs,
	Fw8u  *NumOutCoeffs, Fw8u *TotalZeros, 
	Fw16s *pLevels,      Fw8u *pRuns)
{
	int pos, coef, last_coeff;
//	Fw16s* zzBlock = (Fw16s*) fwMalloc(16*sizeof(Fw16s));
	SYS_FORCEALIGN_16 Fw16s zzBuffer[16];
	Fw16s* zzBlock = (Fw16s*) &zzBuffer[0];
	Fw16s* pS;
	int i;

    if (pSrc == NULL || pScanMatrix == NULL)
		return fwStsNullPtrErr;

	if (Count < AC || Count > 15) return fwStsOutOfRangeErr;

	// scan coefficient
	pS = zzBlock;
	for(int i=AC;i< 16; i++)
	{
		pos    = pScanMatrix[i];
		coef   = pSrc[i];
		pS[pos]= (Fw16s)coef;
	}

    // Position of the last non-zero block coefficient
    last_coeff = Count;
    (*Traling_One) = 0;
    (*Traling_One_Signs) = 0;
    (*NumOutCoeffs) = 0;
    (*TotalZeros) = 0;
    if( last_coeff >= 0 )
	{
        int b_trailing = 1;
		int idx = 0;
        /* level and run and total */
        while( last_coeff >= AC )
        {
            pLevels[idx] = zzBlock[last_coeff--];

            pRuns[idx] = 0;
            while( last_coeff >= AC && zzBlock[last_coeff] == 0 )
            {
                pRuns[idx]++;
                last_coeff--;
            }
            (*NumOutCoeffs)++;
            (*TotalZeros) = (*TotalZeros) + (Fw8u)(pRuns[idx]);
            if( b_trailing && abs( pLevels[idx] ) == 1 && (*Traling_One) < 3 )
            {
                (*Traling_One_Signs) <<= 1;
                if( pLevels[idx] < 0 ) (*Traling_One_Signs) |= 0x01;

                (*Traling_One)++;
            }
            else
            {
                b_trailing = 0;
            }
            idx++;
        }
	}

	// remove Traveling_One from Level
	for(i=0;i< (16-(*Traling_One)); i++) 
		pLevels[i] = pLevels[i+(*Traling_One)]; 
	for(;i< 16; i++) pLevels[i] = 0; 

	// except run before the first non-zero
	for(i=(*NumOutCoeffs-1);i<16; i++) pRuns[i]=0;
	

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiEncodeChromaDcCoeffsCAVLC_H264_16s)(
	Fw16s* pSrc, 
	Fw8u *Traling_One, 
	Fw8u *Traling_One_Signs, 
	Fw8u *NumOutCoeffs, 
	Fw8u *TotalZeros, Fw16s *pLevels, Fw8u *pRuns)
{
//	int pos, coef, last_coeff;
	int last_coeff;
//	Fw16s* zzBlock = (Fw16s*) fwMalloc(4*sizeof(Fw16s));
	SYS_FORCEALIGN_16 Fw16s zzBuffer[4];
	Fw16s* zzBlock = (Fw16s*) &zzBuffer[0];
//	Fw16s* pS;
	int i;

    if (pSrc == NULL) return fwStsNullPtrErr;

	// scan coefficient
	zzBlock[0] = pSrc[0];
	zzBlock[1] = pSrc[1];
	zzBlock[2] = pSrc[2];
	zzBlock[3] = pSrc[3];

	last_coeff = -1;
	for(int i=3; i>=0; i--) {
		if(zzBlock[i] != 0)	{
		    last_coeff = i;
			break;
		}
	}

    // Position of the last non-zero block coefficient
    (*Traling_One) = 0;
    (*Traling_One_Signs) = 0;
    (*NumOutCoeffs) = 0;
    (*TotalZeros) = 0;
    if( last_coeff >= 0 )
	{
        int b_trailing = 1;
		int idx = 0;
        /* level and run and total */
        while( last_coeff >= 0 )
        {
            pLevels[idx] = zzBlock[last_coeff--];

            pRuns[idx] = 0;
            while( last_coeff >= 0 && zzBlock[last_coeff] == 0 )
            {
                pRuns[idx]++;
                last_coeff--;
            }
            (*NumOutCoeffs)++;
            (*TotalZeros) = (*TotalZeros) + (Fw8u)pRuns[idx];
            if( b_trailing && abs( pLevels[idx] ) == 1 && (*Traling_One) < 3 )
            {
                (*Traling_One_Signs) <<= 1;
                if( pLevels[idx] < 0 ) (*Traling_One_Signs) |= 0x01;

                (*Traling_One)++;
            }
            else
            {
                b_trailing = 0;
            }
            idx++;
        }
	}

	// remove Traveling_One from Level
	for(i=0;i< (16-(*Traling_One)); i++) 
		pLevels[i] = pLevels[i+(*Traling_One)]; 
	for(;i< 16; i++) pLevels[i] = 0; 

	// except run before the first non-zero
	for(i=(*NumOutCoeffs-1);i<16; i++) pRuns[i]=0;

	return fwStsNoErr;
}
#endif

///////////////////////////////////////////////////////////////////////////////

#define int8_t   char
#define int16_t  short
#define uint16_t unsigned short
#define int64_t  __int64
#define uint64_t unsigned __int64
#define ptr_t unsigned int

#define BYTE   Fw8u
#define INT32  Fw32s
#define INT16  int16_t
#define UINT16 uint16_t
#define UINT32 Fw32s


typedef struct  
{
    Fw8u len;
    Fw8u uNumTrailingOnes;
    Fw8u uNumCoeff;
} vlc_coeff_token_t;

// three entry
#define VLC(a, b, c) {a, b, c}
#define VLC2(a, b, c) VLC(a, b, c), VLC(a, b, c)
#define VLC4(a, b, c) VLC2(a, b, c), VLC2(a, b, c)

/* ++ cavlc tables ++ */
static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff4_0[] = 
{
    VLC(6, 0, 2),   /* 0001 00 */
    VLC(6, 3, 3),   /* 0001 01 */
    VLC(6, 1, 2),   /* 0001 10 */
    VLC(6, 0, 1),   /* 0001 11 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff4_1[] = 
{
    VLC2(7, 3, 4),   /* 0000 000(0) */
    VLC(8, 2, 4),   /* 0000 0010 */
    VLC(8, 1, 4),   /* 0000 0011 */
    VLC2(7, 2, 3),   /* 0000 010(0) */
    VLC2(7, 1, 3),   /* 0000 011(0) */
    VLC4(6, 0, 4),   /* 0000 10(00) */
    VLC4(6, 0, 3),   /* 0000 11(00) */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff3_0[] =
{
    VLC(6, 0, 1),    /* 0000 00 */ 
    VLC(6, 1, 1),    /* 0000 01 */ 
    VLC((Fw8u)-1, (Fw8u)-1, (Fw8u)-1), /* 0000 10 */ 
    VLC(6, 0, 0),    /* 0000 11 */
    VLC(6, 0, 2),    /* 0001 00 */
    VLC(6, 1, 2),    /* 0001 01 */
    VLC(6, 2, 2),    /* 0001 10 */
    VLC((Fw8u)-1, (Fw8u)-1, (Fw8u)-1), /* 0001 11 */
    VLC(6, 0, 3),    /* 0010 00 */
    VLC(6, 1, 3),    /* 0010 01 */
    VLC(6, 2, 3),    /* 0010 10 */
    VLC(6, 3, 3),    /* 0010 11 */
    VLC(6, 0, 4),    /* 0011 00 */
    VLC(6, 1, 4),    /* 0011 01 */
    VLC(6, 2, 4),    /* 0011 10 */
    VLC(6, 3, 4),    /* 0011 11 */
    VLC(6, 0, 5),    /* 0100 00 */
    VLC(6, 1, 5),    /* 0100 01 */
    VLC(6, 2, 5),    /* 0100 10 */
    VLC(6, 3, 5),    /* 0100 11 */
    VLC(6, 0, 6),    /* 0101 00 */
    VLC(6, 1, 6),    /* 0101 01 */
    VLC(6, 2, 6),    /* 0101 10 */
    VLC(6, 3, 6),    /* 0101 11 */
    VLC(6, 0, 7),    /* 0110 00 */
    VLC(6, 1, 7),    /* 0110 01 */
    VLC(6, 2, 7),    /* 0110 10 */
    VLC(6, 3, 7),    /* 0110 11 */
    VLC(6, 0, 8),
    VLC(6, 1, 8),
    VLC(6, 2, 8),
    VLC(6, 3, 8),
    VLC(6, 0, 9),
    VLC(6, 1, 9),
    VLC(6, 2, 9),
    VLC(6, 3, 9),
    VLC(6, 0, 10),
    VLC(6, 1, 10),
    VLC(6, 2, 10),
    VLC(6, 3, 10),
    VLC(6, 0, 11),
    VLC(6, 1, 11),
    VLC(6, 2, 11),
    VLC(6, 3, 11),
    VLC(6, 0, 12),
    VLC(6, 1, 12),
    VLC(6, 2, 12),
    VLC(6, 3, 12),
    VLC(6, 0, 13),
    VLC(6, 1, 13),
    VLC(6, 2, 13),
    VLC(6, 3, 13),
    VLC(6, 0, 14),
    VLC(6, 1, 14),
    VLC(6, 2, 14),
    VLC(6, 3, 14),
    VLC(6, 0, 15),
    VLC(6, 1, 15),
    VLC(6, 2, 15),
    VLC(6, 3, 15),
    VLC(6, 0, 16),
    VLC(6, 1, 16),
    VLC(6, 2, 16),
    VLC(6, 3, 16)
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff2_0[] = 
{
    VLC(4, 3, 7),   /* 1000 */
    VLC(4, 3, 6),   /* 1001 */
    VLC(4, 3, 5),   /* 1010 */
    VLC(4, 3, 4),   /* 1011 */
    VLC(4, 3, 3),   /* 1100 */
    VLC(4, 2, 2),   /* 1101 */
    VLC(4, 1, 1),   /* 1110 */
    VLC(4, 0, 0),   /* 1111 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff2_1[] = 
{
    VLC(5, 1, 5),   /* 0100 0 */
    VLC(5, 2, 5),
    VLC(5, 1, 4),
    VLC(5, 2, 4),
    VLC(5, 1, 3),
    VLC(5, 3, 8),
    VLC(5, 2, 3),
    VLC(5, 1, 2),
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff2_2[] = 
{
    VLC(6, 0, 3),   /* 0010 00 */
    VLC(6, 2, 7),
    VLC(6, 1, 7),
    VLC(6, 0, 2),
    VLC(6, 3, 9),
    VLC(6, 2, 6),
    VLC(6, 1, 6),
    VLC(6, 0, 1),
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff2_3[] = 
{
    VLC(7, 0, 7),   /* 0001 000 */
    VLC(7, 0, 6),
    VLC(7, 2, 9),
    VLC(7, 0, 5),
    VLC(7, 3, 10),
    VLC(7, 2, 8),
    VLC(7, 1, 8),
    VLC(7, 0, 4),
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff2_4[] = 
{
    VLC(8, 3, 12),   /* 0000 1000 */
    VLC(8, 2, 11),
    VLC(8, 1, 10),
    VLC(8, 0, 9),
    VLC(8, 3, 11),
    VLC(8, 2, 10),
    VLC(8, 1, 9),
    VLC(8, 0, 8),
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff2_5[] = 
{
    VLC(9, 0, 12),   /* 0000 0100 0 */
    VLC(9, 2, 13),
    VLC(9, 1, 12),
    VLC(9, 0, 11),
    VLC(9, 3, 13),
    VLC(9, 2, 12),
    VLC(9, 1, 11),
    VLC(9, 0, 10),
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff2_6[] = 
{
    VLC((Fw8u)-1, (Fw8u)-1, (Fw8u)-1),   /* 0000 0000 00 */
    VLC(10, 0, 16),    /* 0000 0000 01 */
    VLC(10, 3, 16),    /* 0000 0000 10 */
    VLC(10, 2, 16),    /* 0000 0000 11 */
    VLC(10, 1, 16),    /* 0000 0001 00 */
    VLC(10, 0, 15),    /* 0000 0001 01 */
    VLC(10, 3, 15),    /* 0000 0001 10 */
    VLC(10, 2, 15),    /* 0000 0001 11 */
    VLC(10, 1, 15),    /* 0000 0010 00 */
    VLC(10, 0, 14),
    VLC(10, 3, 14),
    VLC(10, 2, 14),
    VLC(10, 1, 14),
    VLC(10, 0, 13),
    VLC2(9, 1, 13),    /* 0000 0011 1(0) */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff1_0[] = 
{
    VLC(4, 3, 4),  /* 0100 */
    VLC(4, 3, 3),  /* 0101 */
    VLC2(3, 2, 2), /* 011(0) */
    VLC4(2, 1, 1), /* 10 */
    VLC4(2, 0, 0), /* 11 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff1_1[] = 
{
    VLC(6, 3, 7),   /* 0001 00 */
    VLC(6, 2, 4),   /* 0001 01 */
    VLC(6, 1, 4),   /* 0001 10 */
    VLC(6, 0, 2),   /* 0001 11 */
    VLC(6, 3, 6),   /* 0010 00 */
    VLC(6, 2, 3),   /* 0010 01 */
    VLC(6, 1, 3),   /* 0010 10 */
    VLC(6, 0, 1),   /* 0010 11*/
    VLC2(5, 3, 5),   /* 0011 0(0)*/
    VLC2(5, 1, 2),   /* 0011 1(0)*/
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff1_2[] = 
{
    VLC(9, 3, 9),   /* 0000 0010 0 */
    VLC(9, 2, 7),   /* 0000 0010 1 */
    VLC(9, 1, 7),   /* 0000 0011 0 */
    VLC(9, 0, 6),   /* 0000 0011 1 */

    VLC2(8, 0, 5),   /* 0000 0100 */
    VLC2(8, 2, 6),   /* 0000 0101 */
    VLC2(8, 1, 6),   /* 0000 0110 */
    VLC2(8, 0, 4),   /* 0000 0111 */

    VLC4(7, 3, 8),    /* 0000 100 */
    VLC4(7, 2, 5),    /* 0000 101 */
    VLC4(7, 1, 5),    /* 0000 110 */
    VLC4(7, 0, 3),    /* 0000 111 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff1_3[] = 
{
    VLC(11, 3, 11),   /* 0000 0001 000 */
    VLC(11, 2, 9),    /* 0000 0001 001 */
    VLC(11, 1, 9),    /* 0000 0001 010 */
    VLC(11, 0, 8),    /* 0000 0001 011 */
    VLC(11, 3, 10),   /* 0000 0001 100 */
    VLC(11, 2, 8),    /* 0000 0001 101 */
    VLC(11, 1, 8),    /* 0000 0001 110 */
    VLC(11, 0, 7),    /* 0000 0001 111 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff1_4[] = 
{
    VLC(12, 0, 11),   /* 0000 0000 1000 */
    VLC(12, 2, 11),   /* 0000 0000 1001 */
    VLC(12, 1, 11),   /* 0000 0000 1010 */
    VLC(12, 0, 10),   /* 0000 0000 1011 */
    VLC(12, 3, 12),   /* 0000 0000 1100 */
    VLC(12, 2, 10),   /* 0000 0000 1101 */
    VLC(12, 1, 10),   /* 0000 0000 1110 */
    VLC(12, 0, 9),    /* 0000 0000 1111 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff1_5[] = 
{
    VLC(13, 3, 14),   /* 0000 0000 0100 0 */
    VLC(13, 2, 13),   /* 0000 0000 0100 1 */
    VLC(13, 1, 13),   /* 0000 0000 0101 0 */
    VLC(13, 0, 13),   /* 0000 0000 0101 1 */
    VLC(13, 3, 13),   /* 0000 0000 0110 0 */
    VLC(13, 2, 12),   /* 0000 0000 0110 1 */
    VLC(13, 1, 12),   /* 0000 0000 0111 0 */
    VLC(13, 0, 12),   /* 0000 0000 0111 1 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff1_6[] = 
{
    VLC2((Fw8u)-1, (Fw8u)-1, (Fw8u)-1),  /* 0000 0000 0000 00 */
    VLC2(13, 3, 15),   /* 0000 0000 0000 1(0) */
    VLC(14, 3, 16),   /* 0000 0000 0001 00 */
    VLC(14, 2, 16),   /* 0000 0000 0001 01 */
    VLC(14, 1, 16),   /* 0000 0000 0001 10 */
    VLC(14, 0, 16),   /* 0000 0000 0001 11 */

    VLC(14, 1, 15),   /* 0000 0000 0010 00 */
    VLC(14, 0, 15),   /* 0000 0000 0010 01 */
    VLC(14, 2, 15),   /* 0000 0000 0010 10 */
    VLC(14, 1, 14),   /* 0000 0000 0010 11 */
    VLC2(13, 2, 14),   /* 0000 0000 0011 0(0) */
    VLC2(13, 0, 14),   /* 0000 0000 0011 1(0) */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff0_0[] = 
{
    VLC2((Fw8u)-1, (Fw8u)-1, (Fw8u)-1), /* 0000 0000 0000 000(0) */
    VLC2(15, 1, 13),  /* 0000 0000 0000 001(0) */
    VLC(16, 0, 16),   /* 0000 0000 0000 0100 */
    VLC(16, 2, 16),   
    VLC(16, 1, 16),
    VLC(16, 0, 15),
    VLC(16, 3, 16),
    VLC(16, 2, 15),
    VLC(16, 1, 15),
    VLC(16, 0, 14),
    VLC(16, 3, 15),
    VLC(16, 2, 14),
    VLC(16, 1, 14),
    VLC(16, 0, 13),   /* 0000 0000 0000 1111 */
    VLC2(15, 3, 14),  /* 0000 0000 0001 000(0) */
    VLC2(15, 2, 13),
    VLC2(15, 1, 12),
    VLC2(15, 0, 12),
    VLC2(15, 3, 13),
    VLC2(15, 2, 12),
    VLC2(15, 1, 11),
    VLC2(15, 0, 11),  /* 0000 0000 0001 111(0) */
    VLC4(14, 3, 12),  /* 0000 0000 0010 00(00) */
    VLC4(14, 2, 11),
    VLC4(14, 1, 10),
    VLC4(14, 0, 10),
    VLC4(14, 3, 11),
    VLC4(14, 2, 10),
    VLC4(14, 1, 9),
    VLC4(14, 0, 9),  /* 0000 0000 0011 11(00) */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff0_1[] = 
{
    VLC(13, 0, 8),   /* 0000 0000 0100 0 */
    VLC(13, 2, 9),
    VLC(13, 1, 8),
    VLC(13, 0, 7),
    VLC(13, 3, 10),
    VLC(13, 2, 8),
    VLC(13, 1, 7),
    VLC(13, 0, 6),  /* 0000 0000 0111 1 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff0_2[] = 
{
    VLC(11, 3, 9),   /* 0000 0000 100 */
    VLC(11, 2, 7),
    VLC(11, 1, 6),
    VLC(11, 0, 5),   /* 0000 0000 111 */
    VLC2(10, 3, 8),  /* 0000 0001 00(0) */
    VLC2(10, 2, 6),
    VLC2(10, 1, 5),
    VLC2(10, 0, 4),  /* 0000 0001 11(0) */
    VLC4(9, 3, 7),  /* 0000 0010 0(0) */
    VLC4(9, 2, 5),
    VLC4(9, 1, 4),
    VLC4(9, 0, 3),  /* 0000 0011 1(0) */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff0_3[] = 
{
    VLC(8, 3, 6),   /* 0000 0100 */
    VLC(8, 2, 4),
    VLC(8, 1, 3),
    VLC(8, 0, 2),
    VLC2(7, 3, 5),  /* 0000 100 */
    VLC2(7, 2, 3),
    VLC4(6, 3, 4),  /* 0000 11 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff0_4[] = 
{
    VLC(6, 1, 2),    /* 0001 00 */
    VLC(6, 0, 1),    /* 0001 01 */
    VLC2(5, 3, 3)    /* 0001 1 */
};

static SYS_FORCEALIGN_16 const vlc_coeff_token_t coeff0_5[] = 
{
    VLC((Fw8u)-1, (Fw8u)-1, (Fw8u)-1),   /* 000 */
    VLC(3, 2, 2),      /* 001 */
    VLC2(2, 1, 1),     /* 01 */
    VLC4(1, 0, 0)      /* 1 */
};

typedef struct
{
    Fw32u bufa;
    Fw32u bufb;
    Fw32u buf;
    Fw32u pos;
    Fw32u *tail;
    Fw32u *start;
    Fw32u length;
    Fw32u initpos;
} Bitstream;


/*
AVS_INT read_bits(const AVS_BYTE * buffer, AVS_DWORD * totbitoffset, AVS_INT numbits)
{
	if (numbits == 0)
		return 0;
	AVS_INT inf;
	AVS_INT byteoffset = (*totbitoffset) >> 3;
	AVS_INT bitoffset = (*totbitoffset) & 7;
	AVS_INT bitleft = 31 - bitoffset;
	AVS_UINT data = DWORD_SWAP(*(AVS_DWORD*)(buffer + byteoffset));
	
	*totbitoffset += numbits;
	AVS_UINT tmp = data << bitoffset;
	inf = tmp >> (32 - numbits);
	return inf;
}
*/

#define BSWAP(a) { \
                unsigned int _temp0,_temp1,_temp2,_temp3,_temp4;\
                _temp1=(a & 0xFF00FF00)>>8;\
                _temp0=(a & 0x00FF00FF)<<8;\
                _temp2=_temp0+_temp1;\
                _temp3=(_temp2 & 0x0000FFFF)<<16;\
                _temp4=(_temp2 & 0xFFFF0000)>>16;\
                     a=_temp3+_temp4;\
                }
#define SWAP(type, x, y) { type* _tmp_; _tmp_ = x; x = y ; y = _tmp_;}
#define CLIP1(x) (x & ~255) ? (-x >> 31) : x
#define ABS(x) ((x) > 0 ? (x) : -(x))

/* initialise bitstream structure */

static SYS_FORCEALIGN_16 const Fw32u GetBitsMask32[32] =
{
    0x00000000, 0x00000001, 0x00000003, 0x00000007,
    0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
    0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
    0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
    0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
    0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
    0x00ffffff,	0x01ffffff, 0x03ffffff, 0x07ffffff,
    0x0fffffff,	0x1fffffff, 0x3fffffff, 0x7fffffff,
};

static void SYS_INLINE BitstreamInit(Bitstream * const bs, 
								   Fw32u	 *pBitStream, 
								   Fw32s     BitOffset)
{
	Fw32s tmp;

	bs->start = bs->tail = (Fw32u *) pBitStream;

	tmp = *(bs->start);
//		No need in sample code
//	BSWAP(tmp);
	bs->bufa = tmp;

	tmp = *(bs->start + 1);
//		No need in sample code
//	BSWAP(tmp);
	bs->bufb = tmp;

	bs->buf = 0;
	bs->pos = bs->initpos = 31 - BitOffset;
}


static Fw32u SYS_INLINE BitstreamShowBits_sse2(Bitstream * const bs, const Fw32s bits)
{
	int nbit = (bits + bs->pos) - 32;
    
	if (nbit > 0) {
		__m128i xmm0, xmm1;
		Fw32u bMask = (1 <<  bits) -1;
		xmm0 = _mm_set_epi32 ( 0, 0, bs->bufa,  bs->bufb);	// buffa + buffb
		xmm1 = _mm_set_epi32 ( 0, 0, 0,  bMask);
        __m128i nbit_x_32neg = _mm_cvtsi32_si128(32 - nbit);
		xmm0 = _mm_srl_epi64 (xmm0, nbit_x_32neg);	// (buffa+buffb) >> (32-nbit)
		xmm0 = _mm_and_si128 (xmm0, xmm1);			// (buffa+buffb) & mask
		return (Fw32u)_mm_cvtsi128_si32 (xmm0);
//		return ((bs->bufa & (0xffffffff >> bs->pos)) << nbit) | (bs->bufb >> (32-nbit));

	} else {
		return  (bs->bufa & (0xffffffff >> bs->pos)) >> (32 - bs->pos - bits);
	}
}
static Fw32u SYS_INLINE BitstreamShowBits(Bitstream * const bs, const Fw32s bits)
{
#if 1
	int bitMask = (1<<bits)-1;				// Mask out bits
	int nbit = (bits + bs->pos) - 32;

	if (nbit > 0) {
		return bitMask & (((bs->bufa & (0xffffffff >> bs->pos)) << nbit) | (bs->bufb >> (32-nbit)));
	} else {
		return  bitMask & ((bs->bufa & (0xffffffff >> bs->pos)) >> (32 - bs->pos - bits));
	}
#else
	int nbit = (bits + bs->pos) - 32;

	if (nbit > 0) {
		return ((bs->bufa & (0xffffffff >> bs->pos)) << nbit) | (bs->bufb >> (32-nbit));
	} else {
		return  (bs->bufa & (0xffffffff >> bs->pos)) >> (32 - bs->pos - bits);
	}
#endif
}
/* skip n bits forward in bitstream */

static SYS_INLINE void BitstreamSkip(Bitstream * const bs, const Fw32u bits)
{
	bs->pos += bits;

	if (bs->pos >= 32) {
		Fw32u tmp;

		bs->bufa = bs->bufb;
		tmp = *((Fw32u *) bs->tail + 2);
//		No need in sample code
//		BSWAP(tmp);

		bs->bufb = tmp;
		bs->tail++;
		bs->pos -= 32;
	}
}
/* read n bits from bitstream */
static Fw32u SYS_INLINE BitstreamGetBits_sse2(Bitstream * const bs, const Fw32u bits)
{
//	Fw32u ret = BitstreamShowBits(bs, n);
//	BitstreamSkip(bs, n);
	Fw32u ret;
	int nbit = (bits + bs->pos) - 32;

	if (nbit > 0) {
		__m128i xmm0, xmm1;
		Fw32u bMask = (1 <<  bits) -1;
		xmm0 = _mm_set_epi32 ( 0, 0, bs->bufa,  bs->bufb);	// buffa + buffb
		xmm1 = _mm_set_epi32 ( 0, 0, 0,  bMask);
        __m128i nbit_x_32neg = _mm_cvtsi32_si128(32 - nbit);
		xmm0 = _mm_srl_epi64 (xmm0, nbit_x_32neg);	// (buffa+buffb) >> (32-nbit)
		xmm0 = _mm_and_si128 (xmm0, xmm1);			// (buffa+buffb) & mask
		ret = (Fw32u)_mm_cvtsi128_si32 (xmm0);
//		return ((bs->bufa & (0xffffffff >> bs->pos)) << nbit) | (bs->bufb >> (32-nbit));
	} 
	else {
		ret = (bs->bufa & (0xffffffff >> bs->pos)) >> (32 - bs->pos - bits);
	}
	bs->pos += bits;
	if (bs->pos >= 32) {
		Fw32u tmp;
		bs->bufa = bs->bufb;
		tmp = *((Fw32u *) bs->tail + 2);
//		No need in sample code
//		BSWAP(tmp);
		bs->bufb = tmp;
		bs->tail++;
		bs->pos -= 32;
	}

	return ret;
}
static Fw32u SYS_INLINE BitstreamGetBits(Bitstream * const bs, const Fw32u n)
{
	Fw32u ret = BitstreamShowBits(bs, n);
	BitstreamSkip(bs, n);
	return ret;
}

/* 2 > nC >= 0 */
void  static H264dec_mb_read_coff_token_t0(Bitstream * const tbs, Fw32u &uNumTrailingOnes, Fw32u &uNumCoeff, int Dispatch_Type)
{
    Fw32s code;
    const vlc_coeff_token_t* table;

	if( Dispatch_Type == DT_SSE2)
	    code = BitstreamShowBits_sse2(tbs, 16);
	else
	    code = BitstreamShowBits(tbs, 16);

//printf("\n H264dec_mb_read_coff_token_t0: code=%d",code);

    if (code >= 8192)
    {
        table = coeff0_5;
        code >>= 13;
    }
    else if (code >= 4096)
    {
        table = coeff0_4;
        code = (code >> 10) - 4;
    }
    else if (code >= 1024)
    {
        table = coeff0_3;
        code = (code >> 8) - 4;
    }
    else if (code >= 128)
    {
        table = coeff0_2;
        code = (code >> 5) - 4;
    }
    else if (code >= 64)
    {
        table = coeff0_1;
        code = (code >> 3) - 8;
    }
    else
    {
        table = coeff0_0;
    }

    uNumTrailingOnes = table[code].uNumTrailingOnes;
    uNumCoeff        = table[code].uNumCoeff;
    BitstreamSkip(tbs, table[code].len);

//printf("\n H264dec_mb_read_coff_token_t0: code=%d, uNumTrailingOnes=%d, uNumCoeff=%d len=%d",
//										code, uNumTrailingOnes, uNumCoeff, table[code].len);

}
/* 8 > nC >= 4 */
void  static H264dec_mb_read_coff_token_t2(Bitstream * const tbs, Fw32u &uNumTrailingOnes, Fw32u &uNumCoeff, int Dispatch_Type)
{
    Fw32s code;
    const vlc_coeff_token_t* table;

//    code = eg_show(t->bs, 10);
	if( Dispatch_Type == DT_SSE2)
	    code = BitstreamShowBits_sse2(tbs, 10);
	else
	    code = BitstreamShowBits(tbs, 10);

    if (code >= 512)
    {
        table = coeff2_0;
        code = (code >> 6) - 8;
    }
    else if (code >= 256)
    {
        table = coeff2_1;
        code = (code >> 5) - 8;
    }
    else if (code >= 128)
    {
        table = coeff2_2;
        code = (code >> 4) - 8;
    }
    else if (code >= 64)
    {
        table = coeff2_3;
        code = (code >> 3) - 8;
    }
    else if (code >= 32)
    {
        table = coeff2_4;
        code = (code >> 2) - 8;
    }
    else if (code >= 16)
    {
        table = coeff2_5;
        code = (code >> 1) - 8;
    }
    else
    {
        table = coeff2_6;
    }

//    *uNumTrailingOnes = table[code].uNumTrailingOnes;
 //   *uNumCoeff = table[code].uNumCoeff;
 //   eg_read_skip(t->bs, table[code].len);
    uNumTrailingOnes = table[code].uNumTrailingOnes;
    uNumCoeff        = table[code].uNumCoeff;
    BitstreamSkip(tbs, table[code].len);
}

/* 4 > nC >= 2 */
void  static H264dec_mb_read_coff_token_t1(Bitstream * const tbs, Fw32u &uNumTrailingOnes, Fw32u &uNumCoeff, int Dispatch_Type)
{
    Fw32s code;
    const vlc_coeff_token_t* table;

//    code = eg_show(t->bs, 14);
	if( Dispatch_Type == DT_SSE2)
	    code = BitstreamShowBits_sse2(tbs, 14);
	else
	    code = BitstreamShowBits(tbs, 14);

    if (code >= 4096)
    {
        table = coeff1_0;
        code = (code >> 10) - 4;
    }
    else if (code >= 1024)
    {
        table = coeff1_1;
        code = (code >> 8) - 4;
    }
    else if (code >= 128)
    {
        table = coeff1_2;
        code = (code >> 5) - 4;
    }
    else if (code >= 64)
    {
        table = coeff1_3;
        code = (code >> 3) - 8;
    }
    else if (code >= 32)
    {
        table = coeff1_4;
        code = (code >> 2) - 8;
    }
    else if (code >= 16)
    {
        table = coeff1_5;
        code = (code >> 1) - 8;
    }
    else
    {
        table = coeff1_6;
    }

//    *uNumTrailingOnes = table[code].uNumTrailingOnes;
//    *uNumCoeff = table[code].uNumCoeff;
//    eg_read_skip(t->bs, table[code].len);
    uNumTrailingOnes = table[code].uNumTrailingOnes;
    uNumCoeff        = table[code].uNumCoeff;
    BitstreamSkip(tbs, table[code].len);

}

/* nC >= 8 */
void  static H264dec_mb_read_coff_token_t3(Bitstream * const tbs, Fw32u &uNumTrailingOnes, Fw32u &uNumCoeff, int Dispatch_Type)
{
    Fw32s code;

//  code = eg_read_direct(t->bs, 6);
	if( Dispatch_Type == DT_SSE2)
	    code = BitstreamShowBits_sse2(tbs, 6);
	else
	    code = BitstreamShowBits(tbs, 6);

    uNumTrailingOnes = coeff3_0[code].uNumTrailingOnes;
    uNumCoeff        = coeff3_0[code].uNumCoeff;

    BitstreamSkip(tbs, 6);	/// works for ffmpeg
}

#if 1
/* nC == -1 */
void  static H264dec_mb_read_coff_token_t4(Bitstream * const tbs, Fw32u &uNumTrailingOnes, Fw32u &uNumCoeff, int Dispatch_Type)
{
    Fw32s code;

//    code = eg_show(t->bs, 8);
	if( Dispatch_Type == DT_SSE2)
	    code = BitstreamShowBits_sse2(tbs, 8);
	else
	    code = BitstreamShowBits(tbs, 8);

    if (code >= 16)
    {
        if (code >= 128)
        {
            /* 1 */
            uNumTrailingOnes = 1;
            uNumCoeff = 1;
//            eg_read_skip(t->bs, 1);
		    BitstreamSkip(tbs, 1);
        }
        else if (code >= 64)
        {
            /* 01 */
            uNumTrailingOnes = 0;
            uNumCoeff = 0;
//            eg_read_skip(t->bs, 2);
		    BitstreamSkip(tbs, 2);
        }
        else if (code >= 32)
        {
            /* 001 */
            uNumTrailingOnes = 2;
            uNumCoeff = 2;
//            eg_read_skip(t->bs, 3);
		    BitstreamSkip(tbs, 3);
        }
        else
        {
            code = (code >> 2) - 4;

            uNumTrailingOnes = coeff4_0[code].uNumTrailingOnes;
            uNumCoeff = coeff4_0[code].uNumCoeff;
//            eg_read_skip(t->bs, 6);
			BitstreamSkip(tbs, 6);
        }
    }
    else
    {
        uNumTrailingOnes = coeff4_1[code].uNumTrailingOnes;
        uNumCoeff = coeff4_1[code].uNumCoeff;
//        eg_read_skip(t->bs, coeff4_1[code].len);
	    BitstreamSkip(tbs, coeff4_1[code].len);
    }
}
#endif

typedef void (*H264dec_mb_read_coff_token_t)(Bitstream * const tbs, Fw32u &uNumTrailingOnes, Fw32u &uNumCoeff, int Dispatch_Type);
static const H264dec_mb_read_coff_token_t read_coeff[4] = 
{
    H264dec_mb_read_coff_token_t0, 
	H264dec_mb_read_coff_token_t1,
    H264dec_mb_read_coff_token_t2, 
	H264dec_mb_read_coff_token_t3
};


#if 1

// two entry
#undef VLC
#undef VLC2
#undef VLC4
#define VLC(a, b) {a, b}
#define VLC2(a, b) VLC(a, b), VLC(a, b)
#define VLC4(a, b) VLC2(a, b), VLC2(a, b)
#define VLC8(a, b) VLC4(a, b), VLC4(a, b)

typedef struct  
{
    Fw8u num;
    Fw8u len;
} zero_count_t;

static const zero_count_t total_zero_table1_0[] = 
{
    VLC((Fw8u)-1, (Fw8u)-1),
    VLC(15, 9), /* 0000 0000 1 */
    VLC(14, 9),
    VLC(13, 9), /* 0000 0001 1 */
    VLC2(12, 8),/* 0000 0010 */
    VLC2(11, 8),/* 0000 0011 */
    VLC4(10, 7),/* 0000 010 */
    VLC4(9, 7), /* 0000 011 */
    VLC8(8, 6), /* 0000 10 */
    VLC8(7, 6), /* 0000 11 */
};

static const zero_count_t total_zero_table1_1[] = 
{
    VLC2((Fw8u)-1, (Fw8u)-1),
    VLC(6, 5), /* 0001 0 */
    VLC(5, 5), /* 0001 1 */
    VLC2(4, 4),/* 0010 */
    VLC2(3, 4),/* 0011 */
    VLC4(2, 3),/* 010 */
    VLC4(1, 3),/* 011 */
    VLC8(0, 1), /*1 */
    VLC8(0, 1), /*1 */
};

static const zero_count_t total_zero_table2_0[] = 
{
    VLC(14, 6), /* 0000 00 */
    VLC(13, 6),
    VLC(12, 6),
    VLC(11, 6),
    VLC2(10, 5),/* 0001 0 */
    VLC2(9, 5),
};

static const zero_count_t total_zero_table2_1[] = 
{
    VLC2((Fw8u)-1, (Fw8u)-1),
    VLC(8, 4), /* 0010 */
    VLC(7, 4), /* 0011 */
    VLC(6, 4),
    VLC(5, 4),
    VLC2(4, 3),/* 011 */
    VLC2(3, 3),/* 100 */
    VLC2(2, 3), /*101 */
    VLC2(1, 3), /*110 */
    VLC2(0, 3), /*111 */
};

static const zero_count_t total_zero_table3_0[] = 
{
    VLC(13, 6), /* 0000 00 */
    VLC(11, 6),
    VLC2(12, 5),/* 0000 1 */
    VLC2(10, 5),/* 0001 0 */
    VLC2(9, 5), /* 0001 1 */
};

static const zero_count_t total_zero_table3_1[] = 
{
    VLC2((Fw8u)-1, (Fw8u)-1),
    VLC(8, 4), /* 0010 */
    VLC(5, 4), /* 0011 */
    VLC(4, 4),
    VLC(0, 4),
    VLC2(7, 3),/* 011 */
    VLC2(6, 3),/* 100 */
    VLC2(3, 3), /*101 */
    VLC2(2, 3), /*110 */
    VLC2(1, 3), /*111 */
};

static const zero_count_t total_zero_table6_0[] = 
{
    VLC(10, 6), /* 0000 00 */
    VLC(0, 6),
    VLC2(1, 5),/* 0000 1 */
    VLC4(8, 4),/* 0000 1 */
};

static const zero_count_t total_zero_table6_1[] = 
{
    VLC((Fw8u)-1, (Fw8u)-1),
    VLC(9, 3), /* 001 */
    VLC(7, 3), /* 010 */
    VLC(6, 3),
    VLC(5, 3),
    VLC(4, 3),
    VLC(3, 3),
    VLC(2, 3)
};

static const zero_count_t total_zero_table7_0[] = 
{
    VLC(9, 6), /* 0000 00 */
    VLC(0, 6),
    VLC2(1, 5),/* 0000 1 */
    VLC4(7, 4),/* 0001 */
};

static const zero_count_t total_zero_table7_1[] = 
{
    VLC((Fw8u)-1, (Fw8u)-1),
    VLC(8, 3), /* 001 */
    VLC(6, 3), /* 010 */
    VLC(4, 3),
    VLC(3, 3),
    VLC(2, 3),
    VLC2(5, 2)
};

static const zero_count_t total_zero_table8_0[] = 
{
    VLC(8, 6), /* 0000 00 */
    VLC(0, 6),
    VLC2(2, 5),/* 0000 1 */
    VLC4(1, 4),/* 0001 */
};

static const zero_count_t total_zero_table8_1[] = 
{
    VLC((Fw8u)-1, (Fw8u)-1),
    VLC(7, 3), /* 001 */
    VLC(6, 3), /* 010 */
    VLC(3, 3),
    VLC2(5, 2),
    VLC2(4, 2)
};

static const zero_count_t total_zero_table9_0[] = 
{
    VLC(1, 6), /* 0000 00 */
    VLC(0, 6),
    VLC2(7, 5),/* 0000 1 */
    VLC4(2, 4),/* 0001 */
};

static const zero_count_t total_zero_table9_1[] = 
{
    VLC((Fw8u)-1, (Fw8u)-1),
    VLC(5, 3), /* 001 */
    VLC2(6, 2), /* 01 */
    VLC2(4, 2),
    VLC2(3, 2),
};

static const zero_count_t total_zero_table4_0[] = 
{
    VLC(12, 5), /* 0000 0 */
    VLC(11, 5),
    VLC(10, 5), /* 0000 1 */
    VLC(0, 5),  /* 0001 1 */
    VLC2(9, 4), /* 0010 */
    VLC2(7, 4),
    VLC2(3, 4),
    VLC2(2, 4), /* 0101 */
    VLC4(8, 3), /* 011 */
};

static const zero_count_t total_zero_table4_1[] = 
{
    VLC(6, 3),   /* 100 */
    VLC(5, 3),   /* 101 */
    VLC(4, 3),   /* 110 */
    VLC(1, 3)    /* 111 */
};

static const zero_count_t total_zero_table5_0[] = 
{
    VLC(11, 5),  /* 0000 0 */
    VLC(9, 5),
    VLC2(10, 4), /* 0000 1 */
    VLC2(8, 4),  /* 0010 */
    VLC2(2, 4),
    VLC2(1, 4),
    VLC2(0, 4),
    VLC4(7, 3)
};

static const zero_count_t total_zero_table5_1[] = 
{
    VLC(6, 3), /* 100 */
    VLC(5, 3),
    VLC(4, 3),
    VLC(3, 3)
};

static const zero_count_t total_zero_table10_0[] = 
{
    VLC(1, 5), /* 0000 0 */
    VLC(0, 5),
    VLC2(6, 4), /* 0000 1 */
};

static const zero_count_t total_zero_table10_1[] = 
{
    VLC((Fw8u)-1, (Fw8u)-1),
    VLC(2, 3), /* 001 */
    VLC2(5, 2), /* 01 */
    VLC2(4, 2),
    VLC2(3, 2),
};

static const zero_count_t total_zero_table11_0[] = 
{
    VLC(0, 4), /* 0000 */
    VLC(1, 4),
    VLC2(2, 3), /* 010 */
    VLC2(3, 3),
    VLC2(5, 3),
    VLC8(4, 1)
};

static const zero_count_t total_zero_table12_0[] = 
{
    VLC(0, 4), /* 0000 */
    VLC(1, 4),
    VLC2(4, 3), /* 010 */
    VLC4(2, 2),
    VLC8(3, 1)
};

static const zero_count_t total_zero_table13_0[] = 
{
    VLC(0, 3), /* 000 */
    VLC(1, 3),
    VLC2(3, 2), /* 01 */
    VLC4(2, 1),
};

static const zero_count_t total_zero_table14_0[] = 
{
    VLC(0, 2), 
    VLC(1, 2),
    VLC2(2, 1),
};

static const zero_count_t total_zero_table_chroma[3][8] = 
{
    {
        VLC(3, 3), 
        VLC(2, 3),
        VLC2(1, 2),
        VLC4(0, 1)
    },
    {
        VLC2(2, 2),
        VLC2(1, 2),
        VLC4(0, 1)
    },
    {
        VLC4(1, 1),
        VLC4(0, 1)
    }
};
#endif



Fw8u  static H264dec_mb_read_total_zero1(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 9);
    if (code >= 32)
    {
        code >>= 4;
        total_zero = total_zero_table1_1[code].num;
        BitstreamSkip(tbs, total_zero_table1_1[code].len);
    }
    else
    {
        total_zero = total_zero_table1_0[code].num;
        BitstreamSkip(tbs, total_zero_table1_0[code].len);
    }

    return total_zero;
}


Fw8u  static H264dec_mb_read_total_zero2(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 6);
    if (code >= 8)
    {
        code >>= 2;
        total_zero = total_zero_table2_1[code].num;
        BitstreamSkip(tbs, total_zero_table2_1[code].len);
    }
    else
    {
        total_zero = total_zero_table2_0[code].num;
        BitstreamSkip(tbs, total_zero_table2_0[code].len);
    }

    return total_zero;
}



Fw8u  static H264dec_mb_read_total_zero3(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 6);
    if (code >= 8)
    {
        code >>= 2;
        total_zero = total_zero_table3_1[code].num;
        BitstreamSkip(tbs, total_zero_table3_1[code].len);
    }
    else
    {
        total_zero = total_zero_table3_0[code].num;
        BitstreamSkip(tbs, total_zero_table3_0[code].len);
    }

    return total_zero;
}

Fw8u  static H264dec_mb_read_total_zero6(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 6);
    if (code >= 8)
    {
        code >>= 3;
        total_zero = total_zero_table6_1[code].num;
        BitstreamSkip(tbs, total_zero_table6_1[code].len);
    }
    else
    {
        total_zero = total_zero_table6_0[code].num;
        BitstreamSkip(tbs, total_zero_table6_0[code].len);
    }

    return total_zero;
}

Fw8u static 
H264dec_mb_read_total_zero7(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 6);
    if (code >= 8)
    {
        code >>= 3;
        total_zero = total_zero_table7_1[code].num;
        BitstreamSkip(tbs, total_zero_table7_1[code].len);
    }
    else
    {
        total_zero = total_zero_table7_0[code].num;
        BitstreamSkip(tbs, total_zero_table7_0[code].len);
    }

    return total_zero;
}

Fw8u static 
H264dec_mb_read_total_zero8(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 6);
    if (code >= 8)
    {
        code >>= 3;
        total_zero = total_zero_table8_1[code].num;
        BitstreamSkip(tbs, total_zero_table8_1[code].len);
    }
    else
    {
        total_zero = total_zero_table8_0[code].num;
        BitstreamSkip(tbs, total_zero_table8_0[code].len);
    }

    return total_zero;
}

Fw8u static 
H264dec_mb_read_total_zero9(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 6);
    if (code >= 8)
    {
        code >>= 3;
        total_zero = total_zero_table9_1[code].num;
        BitstreamSkip(tbs, total_zero_table9_1[code].len);
    }
    else
    {
        total_zero = total_zero_table9_0[code].num;
        BitstreamSkip(tbs, total_zero_table9_0[code].len);
    }

    return total_zero;
}

Fw8u static H264dec_mb_read_total_zero4(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 5);
    if (code >= 16)
    {
        code = (code >> 2) - 4;
        total_zero = total_zero_table4_1[code].num;
        BitstreamSkip(tbs, total_zero_table4_1[code].len);
    }
    else
    {
        total_zero = total_zero_table4_0[code].num;
        BitstreamSkip(tbs, total_zero_table4_0[code].len);
    }

    return total_zero;
}

Fw8u static H264dec_mb_read_total_zero5(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 5);
    if (code >= 16)
    {
        code = (code >> 2) - 4;
        total_zero = total_zero_table5_1[code].num;
        BitstreamSkip(tbs, total_zero_table5_1[code].len);
    }
    else
    {
        total_zero = total_zero_table5_0[code].num;
        BitstreamSkip(tbs, total_zero_table5_0[code].len);
    }

    return total_zero;
}

Fw8u static H264dec_mb_read_total_zero10(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 5);
    if (code >= 4)
    {
        code >>= 2;
        total_zero = total_zero_table10_1[code].num;
        BitstreamSkip(tbs, total_zero_table10_1[code].len);
    }
    else
    {
        total_zero = total_zero_table10_0[code].num;
        BitstreamSkip(tbs, total_zero_table10_0[code].len);
    }

    return total_zero;
}


Fw8u static H264dec_mb_read_total_zero11(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 4);
    total_zero = total_zero_table11_0[code].num;
    BitstreamSkip(tbs, total_zero_table11_0[code].len);

    return total_zero;
}

Fw8u  static  H264dec_mb_read_total_zero12(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 4);
    total_zero = total_zero_table12_0[code].num;
    BitstreamSkip(tbs, total_zero_table12_0[code].len);

    return total_zero;
}

Fw8u  static H264dec_mb_read_total_zero13(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 3);
    total_zero = total_zero_table13_0[code].num;
    BitstreamSkip(tbs, total_zero_table13_0[code].len);

    return total_zero;
}

Fw8u  static H264dec_mb_read_total_zero14(Bitstream * const tbs)
{
    Fw8u total_zero;
    Fw32s code;

    code = BitstreamShowBits(tbs, 2);
    total_zero = total_zero_table14_0[code].num;
    BitstreamSkip(tbs, total_zero_table14_0[code].len);

    return total_zero;
}

Fw8u  static H264dec_mb_read_total_zero15(Bitstream * const tbs)
{
//	return (Fw8u)BitstreamGetBits(tbs, 1);
	Fw32u ret = (tbs->bufa & (0x80000000 >> tbs->pos));
	tbs->pos += 1;
	if (tbs->pos >= 32) {
		Fw32u tmp;
		tbs->bufa = tbs->bufb;
		tmp = *((Fw32u *) tbs->tail + 2);
//		No need in sample code
//		BSWAP(tmp);
		tbs->bufb = tmp;
		tbs->tail++;
		tbs->pos -= 32;
	}

	if(ret == 0) return 0;
	else		 return 1;
//    return eg_read_direct1(tbs);
}

#if 1
Fw8u  static H264dec_mb_read_total_zero_chroma(Bitstream * const tbs, Fw32u total_coeff)
{
    Fw8u total_zero;
    Fw32s code;

//    code = eg_show(t->bs, 3);
    code = BitstreamShowBits(tbs, 3);
	code &= 7; // 3 bit only

//printf("\n H264dec_mb_read_total_zero_chroma-1: tbs=(%d-%ld) code=%d #coeff= %d", 
//													tbs->pos, tbs->tail, code, total_coeff);

    total_zero = total_zero_table_chroma[total_coeff-1][code].num;

//printf("\n H264dec_mb_read_total_zero_chroma-2: #coeff=%d code=%d #zero=%d",
//					total_coeff, code, total_zero);

//    eg_read_skip(t->bs, total_zero_table_chroma[total_coeff - 1][code].len);
    BitstreamSkip(tbs, total_zero_table_chroma[total_coeff-1][code].len);

    return total_zero;
}
#endif

typedef Fw8u (*H264dec_mb_read_total_zero_t)(Bitstream * const tbs);
static H264dec_mb_read_total_zero_t total_zero_f[] =
{
    H264dec_mb_read_total_zero1, H264dec_mb_read_total_zero2, 
	H264dec_mb_read_total_zero3, H264dec_mb_read_total_zero4,
    H264dec_mb_read_total_zero5, H264dec_mb_read_total_zero6, 
	H264dec_mb_read_total_zero7, H264dec_mb_read_total_zero8,
    H264dec_mb_read_total_zero9, H264dec_mb_read_total_zero10, 
	H264dec_mb_read_total_zero11, H264dec_mb_read_total_zero12,
    H264dec_mb_read_total_zero13, H264dec_mb_read_total_zero14, 
	H264dec_mb_read_total_zero15
};

static const Fw8u prefix_table0[] = 
{
    (Fw8u)-1,
    (Fw8u)3,
    (Fw8u)2, (Fw8u)2,
    (Fw8u)1, (Fw8u)1, (Fw8u)1, (Fw8u)1,
    (Fw8u)0, (Fw8u)0, (Fw8u)0, (Fw8u)0, (Fw8u)0, (Fw8u)0, (Fw8u)0, (Fw8u)0
};

static const Fw8u prefix_table1[] = 
{
    (Fw8u)-1,
    (Fw8u)7,
    (Fw8u)6, (Fw8u)6,
    (Fw8u)5, (Fw8u)5, (Fw8u)5, (Fw8u)5,
    (Fw8u)4, (Fw8u)4, (Fw8u)4, (Fw8u)4, (Fw8u)4, (Fw8u)4, (Fw8u)4, (Fw8u)4
};

static const Fw8u prefix_table2[] =
{
    (Fw8u)-1,
    (Fw8u)11,
    (Fw8u)10, (Fw8u)10,
    (Fw8u)9, (Fw8u)9, (Fw8u)9, (Fw8u)9,
    (Fw8u)8, (Fw8u)8, (Fw8u)8, (Fw8u)8, (Fw8u)8, (Fw8u)8, (Fw8u)8, (Fw8u)8
};

static const Fw8u prefix_table3[] = 
{
    (Fw8u)-1,
    (Fw8u)15,
    (Fw8u)14, (Fw8u)14,
    (Fw8u)13, (Fw8u)13, (Fw8u)13, (Fw8u)13,
    (Fw8u)12, (Fw8u)12, (Fw8u)12, (Fw8u)12, (Fw8u)12, (Fw8u)12, (Fw8u)12, (Fw8u)12
};
Fw8u  static H264dec_mb_read_level_prefix(Bitstream * const tbs)
{
    Fw8u prefix;
    Fw32s code;

    code = BitstreamShowBits(tbs, 16);
    if (code >= 4096)   {
        prefix = prefix_table0[code >> 12];
    }
    else if (code >= 256)   {
        prefix = prefix_table1[code >> 8];
    }
    else if (code >= 16)   {
        prefix = prefix_table2[code >> 4];
    }
    else   {
        prefix = prefix_table3[code];
    }

    BitstreamSkip(tbs, prefix + 1);

    return prefix;
}
Fw8u  static H264dec_mb_read_level_prefix_sse2(Bitstream * const tbs)
{
    Fw8u prefix;
    Fw32s code;

    code = BitstreamShowBits_sse2(tbs, 16);
    if (code >= 4096)   {
        prefix = prefix_table0[code >> 12];
    }
    else if (code >= 256)   {
        prefix = prefix_table1[code >> 8];
    }
    else if (code >= 16)   {
        prefix = prefix_table2[code >> 4];
    }
    else   {
        prefix = prefix_table3[code];
    }
    BitstreamSkip(tbs, prefix + 1);

    return prefix;
}
static const zero_count_t run_before_table_0[7][8] = 
{
    {
        VLC4(1, 1),
        VLC4(0, 1)
    },
    {
        VLC2(2, 2),
        VLC2(1, 2),
        VLC4(0, 1)
    },
    {
        VLC2(3, 2),
        VLC2(2, 2),
        VLC2(1, 2),
        VLC2(0, 2)
    },
    {
        VLC(4, 3),
        VLC(3, 3),
        VLC2(2, 2),
        VLC2(1, 2),
        VLC2(0, 2)
    },
    {
        VLC(5, 3),
        VLC(4, 3),
        VLC(3, 3),
        VLC(2, 3),
        VLC2(1, 2),
        VLC2(0, 2),
    },
    {
        VLC(1, 3),
        VLC(2, 3),
        VLC(4, 3),
        VLC(3, 3),
        VLC(6, 3),
        VLC(5, 3),
        VLC2(0, 2)
    },
    {
        VLC((Fw8u)-1, (Fw8u)-1),
        VLC(6, 3),
        VLC(5, 3),
        VLC(4, 3),
        VLC(3, 3),
        VLC(2, 3),
        VLC(1, 3),
        VLC(0, 3)
    }
};
static const Fw8u run_before_table_1[] =
{
    (Fw8u)-1,
    (Fw8u)10,
    (Fw8u)9, (Fw8u)9,
    (Fw8u)8, (Fw8u)8, (Fw8u)8, (Fw8u)8,
    (Fw8u)7, (Fw8u)7, (Fw8u)7, (Fw8u)7, (Fw8u)7, (Fw8u)7, (Fw8u)7,(Fw8u) 7
};

static const Fw8u run_before_table_2[] =
{
    (Fw8u)-1,
    (Fw8u)14,
    (Fw8u)13, (Fw8u)13,
    (Fw8u)12, (Fw8u)12, (Fw8u)12, (Fw8u)12,
    (Fw8u)11, (Fw8u)11, (Fw8u)11, (Fw8u)11, (Fw8u)11, (Fw8u)11, (Fw8u)11, (Fw8u)11
};

Fw8u  static H264dec_mb_read_run_before(Bitstream * const tbs, Fw8u zero_left)
{
    Fw32s code;
    Fw8u run_before;

    assert(zero_left != 255);

//    code = eg_show(t->bs, 3);
    code = BitstreamShowBits(tbs, 3);

    if (zero_left <= 6)
    {
        run_before = run_before_table_0[zero_left - 1][code].num;
//        eg_read_skip(t->bs, run_before_table_0[zero_left - 1][code].len);
	    BitstreamSkip(tbs, run_before_table_0[zero_left - 1][code].len);

    }
    else
    {
//        eg_read_skip(t->bs, 3);
	    BitstreamSkip(tbs, 3);

        if (code > 0)
        {
            run_before = run_before_table_0[6][code].num;
        }
        else
        {
//          code = eg_show(t->bs, 4);
			code = BitstreamShowBits(tbs, 4);
            if (code > 0)
            {
                run_before = run_before_table_1[code];
//              eg_read_skip(t->bs, run_before - 6);
			    BitstreamSkip(tbs, run_before - 6);
            }
            else
            {
//              eg_read_skip(t->bs, 4);
			    BitstreamSkip(tbs, 4);
//              code = eg_show(t->bs, 4);
				code = BitstreamShowBits(tbs, 4);
                run_before = run_before_table_2[code];
//              eg_read_skip(t->bs, run_before - 10);
			    BitstreamSkip(tbs, run_before - 10);

            }
        }
    }
//    assert(run_before >= 0 && run_before <= 14);

    return run_before;
}
Fw8u static H264dec_mb_read_run_before_sse2(Bitstream * const tbs, Fw8u zero_left)
{
    Fw32s code;
    Fw8u run_before;

    assert(zero_left != 255);

//    code = eg_show(t->bs, 3);
    code = BitstreamShowBits_sse2(tbs, 3);

    if (zero_left <= 6)
    {
        run_before = run_before_table_0[zero_left - 1][code].num;
//        eg_read_skip(t->bs, run_before_table_0[zero_left - 1][code].len);
	    BitstreamSkip(tbs, run_before_table_0[zero_left - 1][code].len);

    }
    else
    {
//        eg_read_skip(t->bs, 3);
	    BitstreamSkip(tbs, 3);

        if (code > 0)
        {
            run_before = run_before_table_0[6][code].num;
        }
        else
        {
//          code = eg_show(t->bs, 4);
			code = BitstreamShowBits_sse2(tbs, 4);
            if (code > 0)
            {
                run_before = run_before_table_1[code];
//              eg_read_skip(t->bs, run_before - 6);
			    BitstreamSkip(tbs, run_before - 6);
            }
            else
            {
//              eg_read_skip(t->bs, 4);
			    BitstreamSkip(tbs, 4);
//              code = eg_show(t->bs, 4);
				code = BitstreamShowBits_sse2(tbs, 4);
                run_before = run_before_table_2[code];
//              eg_read_skip(t->bs, run_before - 10);
			    BitstreamSkip(tbs, run_before - 10);

            }
        }
    }
//    assert(run_before >= 0 && run_before <= 14);

    return run_before;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDecodeCAVLCCoeffs_H264_1u16s)(
	Fw32u **ppBitStream, 
	Fw32s *pBitOffset, 
	Fw16s *pNumCoeff, 
	Fw16s **ppDstCoeffs, 
	Fw32u uVLCSelect, 
	Fw16s uMaxNumCoeff, 
	const Fw32s **ppTblCoeffToken,
	const Fw32s **ppTblTotalZeros, 
	const Fw32s **ppTblRunBefore, 
	const Fw32s  *pScanMatrix)
{
	ppTblCoeffToken;
	ppTblTotalZeros; 
	ppTblRunBefore;

	Fw32u uNumCoeff=0;
	Fw32u uNumTrailingOnes=0;
//	Fw32u TrOneSigns;		// return sign bits (1==neg) in low 3 bits
//	Fw32u uTotalZeros=0;

//	Fw8u uRunBeforeBuf[16];		// buffer to return up to 16

//	FwStatus ps = fwStsNoErr;

//	Fw16s CoeffBuf[16];		// buffer to return up to 16
	Fw16s *pDstCoeffs = (Fw16s *) *ppDstCoeffs;		// return coeffs

//	Fw8u  *pbs       = (Fw8u *) *ppBitStream;
	Fw32u *pbs32       = (Fw32u *) *ppBitStream;
//	Fw32u bitOffset =  *pBitOffset ;

//	Fw16s DstCoeffsBeforeScan[16];		// buffer to return up to 16
	int i, j;


Bitstream bs;
Bitstream * const tbs = &bs;
SYS_FORCEALIGN_16 Fw16s level[16];
SYS_FORCEALIGN_16 Fw8u run[16];
Fw32s zero_left = 0;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();

		int uVLCIndex = 3;

		// Use one of 3 luma tables
		if (uVLCSelect < 2)
				uVLCIndex = 0;
		else if (uVLCSelect < 4)
				uVLCIndex = 1;
		else if (uVLCSelect < 8)
				uVLCIndex = 2;
		else
				uVLCIndex = 3;

	BitstreamInit(tbs, pbs32, *pBitOffset);

//printf("\n  uVLCIndex= %d pos=%d", uVLCIndex, tbs->pos);

//	H264dec_mb_read_coff_token_t0(tbs, uNumTrailingOnes, uNumCoeff);
	read_coeff[uVLCIndex](tbs, uNumTrailingOnes, uNumCoeff, Dispatch_Type);

//printf("\n after H264dec_mb_read_coff_token_t0: %d %d %d", tbs->pos, uNumCoeff, uNumTrailingOnes);

    for(i=0; i<16; i++)pDstCoeffs[i] = 0;

    if (uNumCoeff > 0)
    {
        Fw8u suffix_length = 0;
        Fw32s level_code;


        if (uNumCoeff > 10 && uNumTrailingOnes < 3) suffix_length = 1;

//printf("\n LEVEL: ");
		if( Dispatch_Type == DT_SSE2)
		{
			for(i = 0 ; i < (int)uNumTrailingOnes ; i ++){
				 level[i] = (Fw16s)(1 - 2 * BitstreamGetBits_sse2(tbs, 1));
			}
			for( ; i < (int)uNumCoeff ; i ++)
			{
				Fw32u level_suffixsize;
				Fw32u level_suffix;
				Fw8u  level_prefix = H264dec_mb_read_level_prefix_sse2(tbs);

				level_suffixsize = suffix_length;
				if (suffix_length == 0 && level_prefix == 14)
					level_suffixsize = 4;
				else if (level_prefix == 15)
					level_suffixsize = 12;
				if (level_suffixsize > 0)
					level_suffix = BitstreamGetBits_sse2(tbs, level_suffixsize);
				else
					level_suffix = 0;
				level_code = (level_prefix << suffix_length) + level_suffix;
				if (level_prefix == 15 && suffix_length == 0)	{
					level_code += 15;
				}
				if (i == (int)uNumTrailingOnes && (int)uNumTrailingOnes < 3)	{
					level_code += 2;
				}
				if (level_code % 2 == 0)	{
					level[i] = (Fw16s)((level_code + 2) >> 1);
				}
				else	{
					level[i] = (Fw16s)((-level_code - 1) >> 1);
				}

				if (suffix_length == 0) suffix_length = 1;

				if (ABS(level[i]) > (3 << (suffix_length - 1)) && suffix_length < 6)	{
					suffix_length ++;
				}
			}
		}
		else
		{
			for(i = 0 ; i < (int)uNumTrailingOnes ; i ++)
			{
	//            level[i] = 1 - 2 * eg_read_direct1(t->bs);
				 level[i] = (Fw16s)(1 - 2 * BitstreamGetBits(tbs, 1));
	//printf(" %d", level[i]);
			}		
	//printf("\n LEVEL: ");
			for( ; i < (int)uNumCoeff ; i ++)
			{
				Fw32u level_suffixsize;
				Fw32u level_suffix;
				Fw8u  level_prefix = H264dec_mb_read_level_prefix(tbs);

				level_suffixsize = suffix_length;
				if (suffix_length == 0 && level_prefix == 14)
					level_suffixsize = 4;
				else if (level_prefix == 15)
					level_suffixsize = 12;
				if (level_suffixsize > 0)
					level_suffix = BitstreamGetBits(tbs, level_suffixsize);
				else
					level_suffix = 0;
				level_code = (level_prefix << suffix_length) + level_suffix;
				if (level_prefix == 15 && suffix_length == 0)	{
					level_code += 15;
				}
				if (i == (int)uNumTrailingOnes && (int)uNumTrailingOnes < 3)	{
					level_code += 2;
				}
				if (level_code % 2 == 0)	{
					level[i] = (Fw16s)((level_code + 2) >> 1);
				}
				else	{
					level[i] = (Fw16s)((-level_code - 1) >> 1);
				}

				if (suffix_length == 0) suffix_length = 1;

				if (ABS(level[i]) > (3 << (suffix_length - 1)) && suffix_length < 6)	{
					suffix_length ++;
				}
	//printf(" %d", level[i]);
			}
		}

//printf("\n uNumCoeff = %d uNumCoeff=%d", uNumCoeff, uNumCoeff);

        if ((int)uNumCoeff < (int)uMaxNumCoeff)
        {
//            if(idx != BLOCK_INDEX_CHROMA_DC)
                zero_left = total_zero_f[uNumCoeff - 1](tbs);
//            else
//              zero_left = H264dec_mb_read_total_zero_chroma(t, uNumCoeff);
//printf("\n zero_left= %d", zero_left);

        }

		if( Dispatch_Type == DT_SSE2)
		{
			for(i = 0 ; i < (int)uNumCoeff - 1 ; i ++)
			{
				if (zero_left > 0)         {
					run[i] = (Fw8u)H264dec_mb_read_run_before_sse2(tbs, (Fw8u)zero_left);
				}
				else         {
					run[i] = 0;
				}
				zero_left -= run[i];
			}
		}
		else
		{
			for(i = 0 ; i < (int)uNumCoeff - 1 ; i ++)
			{
				if (zero_left > 0)         {
					run[i] = (Fw8u)H264dec_mb_read_run_before(tbs, (Fw8u)zero_left);
				}
				else         {
					run[i] = 0;
				}
				zero_left -= run[i];
			}
		}
        run[uNumCoeff - 1] = (Fw8u)zero_left;

        j = -1;
		if(uMaxNumCoeff != 15)
		{
			for(i = uNumCoeff - 1 ; i >= 0 ; i --)
			{
				j +=run[i] + 1;
	//            z[j] = level[i];
				pDstCoeffs[ pScanMatrix[j] ] = level[i];
			}
		}
		else
		{
			for(i = uNumCoeff - 1 ; i >= 0 ; i --)
			{
				j +=run[i] + 1;
	//            z[j] = level[i];
				pDstCoeffs[ pScanMatrix[j+1] ] = level[i];
			}
//			pDstCoeffs[0]=DstCoeffsDC;		// DC coeffs
		}
		*ppDstCoeffs += 16; // point to next block only if number of coeff > 0
	}

	*ppBitStream  = tbs->tail;
	*pBitOffset   = 31-tbs->pos;
	*pNumCoeff    = (Fw16s)uNumCoeff;

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDecodeCAVLCChromaDcCoeffs_H264_1u16s)(
	Fw32u **ppBitStream, 
	Fw32s *pBitOffset, 
	Fw16s *pNumCoeff, 
	Fw16s **ppDstCoeffs, 
	const Fw32s *pTblCoeffToken, 
	const Fw32s **ppTblTotalZerosCR, 
	const Fw32s **ppTblRunBefore)
{
	pTblCoeffToken;
	ppTblTotalZerosCR; 
	ppTblRunBefore;

	Fw32u uNumCoeff=0;
	Fw32u uNumTrailingOnes=0;
//	Fw32u TrOneSigns;		// return sign bits (1==neg) in low 3 bits
//	Fw32u uTotalZeros=0;

//	Fw8u uRunBeforeBuf[16];		// buffer to return up to 16

//	FwStatus ps = fwStsNoErr;

//	Fw16s CoeffBuf[16];		// buffer to return up to 16
	Fw16s *pDstCoeffs = (Fw16s *) *ppDstCoeffs;		// return coeffs

//	Fw8u *pbs       = (Fw8u *) *ppBitStream;
//	Fw32u bitOffset =  *pBitOffset ;

//	Fw16s DstCoeffsBeforeScan[16];		// buffer to return up to 16
	int i, j;

Fw32u *pbs32 = (Fw32u *) *ppBitStream;
Bitstream bs;
Bitstream * const tbs = &bs;
SYS_FORCEALIGN_16 Fw16s level[16];
SYS_FORCEALIGN_16 Fw8u run[16];
Fw32s zero_left = 0;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();


	BitstreamInit(tbs, pbs32, *pBitOffset);

//printf("\n BLOCK_INDEX_CHROMA_DC: %d-%ld %d-%ld", *pBitOffset, *ppBitStream, tbs->pos, tbs->tail);


//  H264dec_mb_read_coff_token_t4(t, &uNumTrailingOnes, &total_coeff);
    H264dec_mb_read_coff_token_t4(tbs, uNumTrailingOnes, uNumCoeff, Dispatch_Type);

//printf("\n BLOCK_INDEX_CHROMA_DC: pos=%d uNumTrailingOnes=%d uNumCoeff=%d", tbs->pos, uNumTrailingOnes, uNumCoeff);

    for(i=0; i<4; i++)pDstCoeffs[i] = 0;

    if (uNumCoeff > 0)
    {

        Fw8u suffix_length = 0;
        Fw32s level_code;

        if (uNumCoeff > 10 && uNumTrailingOnes < 3) suffix_length = 1;

		if( Dispatch_Type == DT_SSE2)
		{
			for(i = 0 ; i < (int)uNumTrailingOnes ; i ++)		{
				 level[i] = (Fw16s)(1 - 2 * BitstreamGetBits_sse2(tbs, 1));
			}
			for( ; i < (int)uNumCoeff ; i ++)
			{
				Fw32u level_suffixsize;
				Fw32u level_suffix;
				Fw8u  level_prefix = H264dec_mb_read_level_prefix_sse2(tbs);

				level_suffixsize = suffix_length;
				if (suffix_length == 0 && level_prefix == 14)
					level_suffixsize = 4;
				else if (level_prefix == 15)
					level_suffixsize = 12;
				if (level_suffixsize > 0)
					level_suffix = BitstreamGetBits_sse2(tbs, level_suffixsize);
				else
					level_suffix = 0;
				level_code = (level_prefix << suffix_length) + level_suffix;
				if (level_prefix == 15 && suffix_length == 0)	{
					level_code += 15;
				}
				if (i == (int)uNumTrailingOnes && (int)uNumTrailingOnes < 3)	{
					level_code += 2;
				}
				if (level_code % 2 == 0)				{
					level[i] = (Fw16s)((level_code + 2) >> 1);
				}
				else				{
					level[i] = (Fw16s)((-level_code - 1) >> 1);
				}

				if (suffix_length == 0) suffix_length = 1;

				if (ABS(level[i]) > (3 << (suffix_length - 1)) && suffix_length < 6)	{
					suffix_length ++;
				}
	//printf(" %d", level[i]);
			}
		}
		else
		{
	//printf("\n LEVEL: ");
			for(i = 0 ; i < (int)uNumTrailingOnes ; i ++)
			{
	//            level[i] = 1 - 2 * eg_read_direct1(t->bs);
				 level[i] = (Fw16s)(1 - 2 * BitstreamGetBits(tbs, 1));
	//printf(" %d", level[i]);
			}
	//printf("\n DC-LEVEL: ");
			for( ; i < (int)uNumCoeff ; i ++)
			{
				Fw32u level_suffixsize;
				Fw32u level_suffix;
				Fw8u  level_prefix = H264dec_mb_read_level_prefix(tbs);

				level_suffixsize = suffix_length;
				if (suffix_length == 0 && level_prefix == 14)
					level_suffixsize = 4;
				else if (level_prefix == 15)
					level_suffixsize = 12;
				if (level_suffixsize > 0)
					level_suffix = BitstreamGetBits(tbs, level_suffixsize);
				else
					level_suffix = 0;
				level_code = (level_prefix << suffix_length) + level_suffix;
				if (level_prefix == 15 && suffix_length == 0)				{
					level_code += 15;
				}
				if (i == (int)uNumTrailingOnes && (int)uNumTrailingOnes < 3)	{
					level_code += 2;
				}
				if (level_code % 2 == 0)				{
					level[i] = (Fw16s)((level_code + 2) >> 1);
				}
				else				{
					level[i] = (Fw16s)((-level_code - 1) >> 1);
				}

				if (suffix_length == 0) suffix_length = 1;

				if (ABS(level[i]) > (3 << (suffix_length - 1)) && suffix_length < 6)	{
					suffix_length ++;
				}
	//printf(" %d", level[i]);
			}
		}




//printf("\n uNumCoeff = %d uNumCoeff=%d", uNumCoeff, uNumCoeff);

		Fw32u uMaxNumCoeff = 4;

        if (uNumCoeff < uMaxNumCoeff)
        {
//            if(idx != BLOCK_INDEX_CHROMA_DC)
//                zero_left = total_zero_f[uNumCoeff - 1](tbs);
//            else
//              zero_left = H264dec_mb_read_total_zero_chroma(t, uNumCoeff);
//printf("\n start H264dec_mb_read_total_zero_chroma: tbs=(%d-%ld) uNumCoeff= %d", tbs->pos, tbs->tail, uNumCoeff);

                zero_left = H264dec_mb_read_total_zero_chroma(tbs, uNumCoeff);

//printf("\n zero_left= %d", zero_left);
        }

		if( Dispatch_Type == DT_SSE2)
		{
			for(i = 0 ; i < (int)uNumCoeff - 1 ; i ++)
			{
				if (zero_left > 0)
				{
					run[i] = H264dec_mb_read_run_before_sse2(tbs, (Fw8u)zero_left);
				}
				else
				{
					run[i] = 0;
				}
				zero_left -= (Fw8u)run[i];
			}
		}
		else
		{
			for(i = 0 ; i < (int)uNumCoeff - 1 ; i ++)
			{
				if (zero_left > 0)
				{
					run[i] = H264dec_mb_read_run_before(tbs, (Fw8u)zero_left);
				}
				else
				{
					run[i] = 0;
				}
				zero_left -= (Fw8u)run[i];
			}
		}

        run[uNumCoeff - 1] = (Fw8u)zero_left;

        j = -1;
        for(i = uNumCoeff - 1 ; i >= 0 ; i --)
        {
            j +=run[i] + 1;
//            z[j] = level[i];
			pDstCoeffs[j] = level[i];
        }

		*ppDstCoeffs += 4; // point to next block only if number of coeff > 0
	}

	*ppBitStream  = tbs->tail;
	*pBitOffset   = 31-tbs->pos;
	*pNumCoeff    = (Fw16s)uNumCoeff;

	return fwStsNoErr;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiDecodeExpGolombOne_H264_1u16s)(
	Fw32u **ppBitStream, 
	Fw32s *pBitOffset, 
	Fw16s *pDst, 
	Fw8u isSigned)
{
  register int inf;
//  Fw8u *pbs       = (Fw8u *) *ppBitStream;
//  Fw32u bitOffset =  *pBitOffset ;

//  long byteoffset;    // byte from start of buffer
//  int bitoffset;      // bit from start of byte
//  int ctr_bit=0;      // control bit for current bit posision
  int bitcounter=1;
  int uNumZeroBits;
  int info_bit;

  if (ppBitStream == NULL || pDst == NULL) 	return fwStsNullPtrErr;

Fw32u *pbs32 = (Fw32u *) *ppBitStream;
Bitstream bs;
Bitstream * const tbs = &bs;
int Dispatch_Type = Dispatch::Type<DT_SSE2>();

BitstreamInit(tbs, pbs32, *pBitOffset);

/*
  byteoffset= totbitoffset/8;
  bitoffset= 7-(totbitoffset%8);
  ctr_bit = (buffer[byteoffset] & (0x01<<bitoffset));   // set up control bit
  len=1;
  while (ctr_bit==0)
  {                 // find leading 1 bit
    len++;
    bitoffset-=1;           
    bitcounter++;
    if (bitoffset<0)
    {                 // finish with current byte ?
      bitoffset=bitoffset+8;
      byteoffset++;
    }
    ctr_bit=buffer[byteoffset] & (0x01<<(bitoffset));
  }
*/

  bitcounter;

	uNumZeroBits = 0;
//	while (My_Get1Bit(pbs, bitOffset) == 0) uNumZeroBits++;
	if( Dispatch_Type == DT_SSE2)
		while (BitstreamGetBits(tbs, 1) == 0) uNumZeroBits++;
	else
		while (BitstreamGetBits_sse2(tbs, 1) == 0) uNumZeroBits++;

//	printf("\n uNumZeroBits=%d", uNumZeroBits);

    // make infoword
  inf=0;                          // shortest possible code is 1, then info is always 0
/*
  for(info_bit=0;(info_bit<(len-1)); info_bit++)
  {
    bitcounter++;
    bitoffset-=1;
    if (bitoffset<0)
    {                 // finished with current byte ?
      bitoffset=bitoffset+8;
      byteoffset++;
    }
    if (byteoffset > bytecount)
    {
      return -1;
    }
    inf=(inf<<1);
    if(buffer[byteoffset] & (0x01<<(bitoffset)))
      inf |=1;
  }
*/
  if(uNumZeroBits == 0)   *pDst = 0;
  else
  {
	if( Dispatch_Type == DT_SSE2)
	{
	  for(info_bit=0;info_bit<uNumZeroBits; info_bit++)
	  {
	      inf=(inf<<1);
//		  inf |= My_Get1Bit(pbs, bitOffset);
		  inf |= BitstreamGetBits_sse2(tbs, 1);
	  }
	}
	else
	{
	  for(info_bit=0;info_bit<uNumZeroBits; info_bit++)
	  {
	      inf=(inf<<1);
//		  inf |= My_Get1Bit(pbs, bitOffset);
		  inf |= BitstreamGetBits(tbs, 1);
	  }
	}
  }

  int code_num = (1<<uNumZeroBits) + inf - 1;

// printf("\n code_num=%d", code_num);

  if(isSigned)
  {
	  if(code_num % 2)	*pDst = (Fw16s)(((code_num+1)/2));		
	  else 				*pDst = (Fw16s)-(code_num/2);		
  }
  else	
	  *pDst = (Fw16s)code_num;

///  *ppBitStream = (Fw32u *)pbs;
//  *pBitOffset = bitOffset ;
	*ppBitStream  = tbs->tail;
	*pBitOffset   = 31-tbs->pos;

  return fwStsNoErr;
}

#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
