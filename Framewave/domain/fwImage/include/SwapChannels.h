/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SWAPCHANNELS_H__
#define __SWAPCHANNELS_H__

#include "fe.h"
#include "Copy.h" 

namespace OPT_LEVEL
{

/////////////////////////////////////// REF FUNCTIONS ////////////////////////////////////////////

template< class TSD, CH CD>
ISV Swap(const TSD *s, TSD *d, const int *dstOrder)
    {
    for( int i = 0; i< CD; i++)
        d[i] = s[dstOrder[i]];
    }

template< class TD, CH CD>
ISV SwapI(TD *d, const int *dstOrder)
    {
    TD temp[5];
    for( int i = 0; i<CD; i++)
        temp[i] = d[i]; 

    for( int i = 0; i<CD; i++)
        d[i] = temp[dstOrder[i]];
    }

//////////////////////////////////////// SSE2 SETUP FUNCTIONS ///////////////////////////////////////

ISV SwapChannels_8u_C3R_SETUP_I(__m128i &maskr, __m128i &maskg, __m128i &maskb)
    {
    maskr = _mm_set_epi32(0x00000000,0x0000ff00,0x00ff0000,0xff0000ff);	// mask to get r's
    maskg = _mm_slli_si128(maskr, 1);
    maskb = _mm_slli_si128(maskr, 2);
    }

ISV SwapChannels_8u_C3R120_SETUP_I(__m128i &maskr, __m128i &maskgb)
    {
    maskr = _mm_set_epi32(0x00000000,0x0000ff00,0x00ff0000,0xff0000ff);	// mask to get r's
    maskgb = _mm_set_epi32(0x00000000,0xffff00ff,0xff00ffff,0x00ffff00);// mask to get g's and b's	
    }

ISV SwapChannels_8u_C3R201_SETUP_I(__m128i &maskrg, __m128i &maskb)
    {
    maskrg = _mm_set_epi32(0x00000000,0x00ffff00,0xffff00ff,0xff00ffff);// mask to get r's and g's
    maskb = _mm_set_epi32(0x00000000,0xff0000ff,0x0000ff00,0x00ff0000);	// mask to get b's
    }

ISV SwapChannels_8u_AC4R012_SETUP_I(__m128i &maska)
    {
    maska = _mm_setzero_si128();										// set to zeros
    maska = _mm_cmpeq_epi32(maska,maska);								// set to ones
    maska = _mm_slli_epi32(maska,24);									// mask to get alphas
    }

ISV SwapChannels_8u_AC4R_SETUP_I(__m128i &maskr, __m128i &maskg, __m128i &maskb, __m128i &maska)
    {
    maska = _mm_setzero_si128();										// set to zeros
    maska = _mm_cmpeq_epi32(maska,maska);								// set to ones

    maskr = _mm_srli_epi32(maska,24);									// mask to get r's	
    maskg = _mm_slli_epi32(maskr,8);									// mask to get g's
    maskb = _mm_slli_epi32(maskr,16);									// mask to get b's
    maska = _mm_slli_epi32(maska,24);									// mask to get alphas
    }

ISV SwapChannels_8u_AC4R120_SETUP_I(__m128i &maskr, __m128i &maskgb, __m128i &maska)
    {
    maska = _mm_setzero_si128();										// set to zeros
    maska = _mm_cmpeq_epi32(maska,maska);								// set to ones
    maskgb = _mm_srli_epi32(maska,16);
    maskgb = _mm_slli_epi32(maskgb,8);									// mask to get g's and b's
    maskr = _mm_srli_epi32(maska,24);									// mask to get r's
    maska = _mm_slli_epi32(maska,24);									// mask to get alphas
    }

ISV SwapChannels_8u_AC4R201_SETUP_I(__m128i &maskrg, __m128i &maskb, __m128i &maska)
    {
    maska = _mm_setzero_si128();										// set to zeros
    maska = _mm_cmpeq_epi32(maska,maska);								// set to ones
    maskrg = _mm_srli_epi32(maska,16);									// mask to get r's and g's
    maskb = _mm_srli_epi32(maska,24);
    maskb = _mm_slli_epi32(maskb,16);									// mask to get b's
    maska = _mm_slli_epi32(maska,24);									// mask to get alphas
    }

ISV SwapChannels_16u_AC4R_SETUP_I(__m128i &maska)
    {
    maska= _mm_setzero_si128();											// set to zeros
    maska = _mm_cmpeq_epi32(maska,maska);								// set to ones
    maska = _mm_slli_epi64(maska, 48);									// mask to get alphas
    }

ISV SwapChannels_32_AC4R_SETUP_I(__m128i &maska)
    {
    maska = _mm_setzero_si128();										// set to zeros
    maska = _mm_cmpeq_epi32(maska,maska);								// set to ones
    maska = _mm_slli_si128(maska, 12);									// mask to get alphas
    }

//////////////////////////////////////// SSE2 FUNCTIONS ////////////////////////////////////////

// C3R 8u
ISV SwapChannels_8u_C3R012_SSE2_I( RegFile &reg )
    {
    reg.dst[0].i = reg.src1[0].i;										// same order so just copy
    reg.dst[1].i = reg.src1[1].i; 
    reg.dst[2].i = reg.src1[2].i;				
    }

ISV SwapChannels_8u_C3R021_SSE2_I( RegFile &reg, const __m128i &maskr, const __m128i &maskg, const __m128i &maskb)
    {
    __m128i temp1, temp2;

    temp1 = _mm_and_si128(reg.src1[0].i,maskr);							// make all locations to zero except red channel
    temp2 = _mm_and_si128(reg.src1[0].i,maskg);							// get g's
    temp2 = _mm_slli_si128(temp2,1);									// shift by 1 byte
    temp1 = _mm_or_si128(temp1, temp2);									// copy
    temp2 = _mm_and_si128(reg.src1[0].i,maskb);							// get b's
    temp2 = _mm_srli_si128(temp2,1);									// shift by 1 byte
    reg.dst[0].i = _mm_or_si128(temp1, temp2);							// copy
    }

ISV SwapChannels_8u_C3R102_SSE2_I(RegFile &reg,  const __m128i &maskr, const __m128i &maskg, const __m128i &maskb)
    {
    __m128i temp1, temp2;

    temp1 = _mm_and_si128(reg.src1[0].i,maskb);							// make all locations to zero except blue channel
    temp2 = _mm_and_si128(reg.src1[0].i,maskr);							// get r's
    temp2 = _mm_slli_si128(temp2,1);									// shift by 1 byte
    temp1 = _mm_or_si128(temp1, temp2);									// copy
    temp2 = _mm_and_si128(reg.src1[0].i,maskg);							// get g's
    temp2 = _mm_srli_si128(temp2,1);									// shift by 1 byte
    reg.dst[0].i = _mm_or_si128(temp1, temp2);							// copy
    }

ISV SwapChannels_8u_C3R120_SSE2_I( RegFile &reg, const __m128i &maskr, const __m128i &maskgb )
    {
    __m128i temp1, temp2;
    temp1 = _mm_and_si128(reg.src1[0].i,maskgb);						// get g and b's data
    temp1 = _mm_srli_si128(temp1,1);									// shift by 1 byte..
    temp2 = _mm_and_si128(reg.src1[0].i,maskr);							// get r's data
    temp2 = _mm_slli_si128(temp2,2);									// shift by 2 bytes
    reg.dst[0].i = _mm_or_si128(temp1,temp2);							// copy
    }

ISV SwapChannels_8u_C3R201_SSE2_I( RegFile &reg, const __m128i &maskrg, const __m128i &maskb )
    {
    __m128i temp1, temp2;
    temp1 = _mm_and_si128(reg.src1[0].i,maskrg);						// get r and g's data
    temp1 = _mm_slli_si128(temp1,1);									// shift by 1 byte..
    temp2 = _mm_and_si128(reg.src1[0].i,maskb);							// get b's data
    temp2 = _mm_srli_si128(temp2,2);									// shift by 2 bytes
    reg.dst[0].i = _mm_or_si128(temp1,temp2);							// copy
    }

ISV SwapChannels_8u_C3R210_SSE2_I( RegFile &reg, const __m128i  &maskr, const __m128i  &maskg, const __m128i  &maskb)
    {
    __m128i temp1, temp2;
    temp1 = _mm_and_si128(reg.src1[0].i,maskg);							// make all locations to zero except green channel
    temp2 = _mm_and_si128(reg.src1[0].i,maskr);							// get r's
    temp2 = _mm_slli_si128(temp2,2);									// shift by 2 bytes
    temp1 = _mm_or_si128(temp1, temp2);									// copy
    temp2 = _mm_and_si128(reg.src1[0].i,maskb);							// get b's
    temp2 = _mm_srli_si128(temp2,2);									// shift by 2 bytes
    reg.dst[0].i = _mm_or_si128(temp1, temp2);							// copy
    }

// C3R 16u
ISV SwapChannels_16u_C3R012_SSE2_I( RegFile &reg )
    {
    reg.dst[0].i = reg.src1[0].i;										//same order so just copy
    reg.dst[1].i = reg.src1[1].i; 
    reg.dst[2].i = reg.src1[2].i;				
    }

ISV SwapChannels_16u_C3R021_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;

    // Fill First Dst reg
    temp1 = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(3,1,2,0));
    temp1 = _mm_shufflehi_epi16(temp1, _MM_SHUFFLE(3,2,0,1));
    temp1 = _mm_slli_si128(temp1, 2);
    temp1 = _mm_srli_si128(temp1, 2);
    temp2 = _mm_slli_si128(reg.src1[1].i, 14);
    reg.dst[0].i = _mm_or_si128(temp1, temp2);

    // Fill Second Dst reg
    temp1 = _mm_shufflelo_epi16(reg.src1[1].i, _MM_SHUFFLE(2,3,1,0));
    temp1 = _mm_shufflehi_epi16(temp1, _MM_SHUFFLE(3,1,2,0));
    temp1 = _mm_srli_si128(temp1, 2);
    temp1 = _mm_slli_si128(temp1, 2);
    temp2 = _mm_srli_si128(reg.src1[0].i, 14);
    reg.dst[1].i = _mm_or_si128(temp1, temp2);

    // Fill Third Dst reg
    temp1 = _mm_shufflelo_epi16(reg.src1[2].i, _MM_SHUFFLE(3,2,0,1));
    temp2 = _mm_srli_si128(temp1, 6);
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(3,2,0,1));
    temp2 = _mm_slli_si128(temp2, 12);
    temp2 = _mm_srli_si128(temp2, 6);
    temp1 = _mm_shufflehi_epi16(temp1, _MM_SHUFFLE(0,2,3,1));
    temp1 = _mm_slli_epi64(temp1, 16);
    temp1 = _mm_shufflelo_epi16(temp1, _MM_SHUFFLE(0,3,2,1));

    reg.dst[2].i = _mm_or_si128(temp1, temp2);
    }

ISV SwapChannels_16u_C3R102_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;

    // Fill First Dst reg	
    temp1 = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(3,2,0,1));
    temp2 = _mm_srli_si128(temp1, 6);
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(3,2,0,1));
    temp2 = _mm_slli_si128(temp2, 12);
    temp2 = _mm_srli_si128(temp2, 6);
    temp1 = _mm_shufflehi_epi16(temp1, _MM_SHUFFLE(0,2,3,1));
    temp1 = _mm_slli_epi64(temp1, 16);
    temp1 = _mm_shufflelo_epi16(temp1, _MM_SHUFFLE(0,3,2,1));
    reg.dst[0].i = _mm_or_si128(temp1, temp2);

    // Fill Second Dst reg
    temp1 = _mm_shufflelo_epi16(reg.src1[1].i, _MM_SHUFFLE(3,1,2,0));
    temp1 = _mm_shufflehi_epi16(temp1, _MM_SHUFFLE(3,2,0,1));
    temp1 = _mm_slli_si128(temp1, 2);
    temp1 = _mm_srli_si128(temp1, 2);
    temp2 = _mm_slli_si128(reg.src1[2].i, 14);
    reg.dst[1].i = _mm_or_si128(temp1, temp2);

    // Fill Third Dst reg
    temp1 = _mm_shufflelo_epi16(reg.src1[2].i, _MM_SHUFFLE(2,3,1,0));
    temp1 = _mm_shufflehi_epi16(temp1, _MM_SHUFFLE(3,1,2,0));
    temp1 = _mm_srli_si128(temp1, 2);
    temp1 = _mm_slli_si128(temp1, 2);
    temp2 = _mm_srli_si128(reg.src1[1].i, 14);
    reg.dst[2].i = _mm_or_si128(temp1, temp2);
    }

ISV SwapChannels_16u_C3R120_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;

    // Fill First Dst reg
    temp1 = _mm_slli_si128(reg.src1[0].i, 4);
    temp1 = _mm_srli_si128(temp1, 10);
    temp1 = _mm_shufflelo_epi16(temp1, _MM_SHUFFLE(3,0,2,1));
    temp1 = _mm_slli_si128(temp1, 6);
    temp2 = _mm_slli_si128(reg.src1[1].i, 14);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(0,3,2,1));
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(3,3,1,0));
    temp2 = _mm_shuffle_epi32(temp2, _MM_SHUFFLE(3,0,1,2));
    temp2 = _mm_srli_si128(temp2, 6);
    temp2 = _mm_slli_si128(temp2, 8);
    temp2 = _mm_srli_si128(temp2, 2);
    temp2 = _mm_shuffle_epi32(temp2, _MM_SHUFFLE(3,0,1,2));
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(2,3,1,0));
    reg.dst[0].i = _mm_or_si128(temp1, temp2);

    // Fill Second Dst reg
    temp1 = _mm_slli_si128(reg.src1[0].i, 2);
    temp1 = _mm_srli_si128(temp1, 14);
    temp2 = _mm_slli_si128(reg.src1[2].i, 14);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shufflelo_epi16(reg.src1[1].i, _MM_SHUFFLE(1,3,2,0));
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(0,2,1,3));
    temp2 = _mm_srli_epi64(temp2, 16);
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(2,1,0,3));
    reg.dst[1].i = _mm_or_si128(temp1, temp2);

    // Fill Third Dst reg
    temp1 = _mm_slli_si128(reg.src1[2].i, 6);
    temp1 = _mm_srli_si128(temp1, 10);
    temp1 = _mm_shufflelo_epi16(temp1, _MM_SHUFFLE(3,0,2,1));
    temp1 = _mm_slli_si128(temp1, 4);
    temp2 = _mm_srli_si128(reg.src1[1].i, 14);
    temp2 = _mm_slli_si128(temp2, 2);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shufflelo_epi16(reg.src1[2].i, _MM_SHUFFLE(1,2,1,0));
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(1,3,2,0));
    temp2 = _mm_srli_epi64(temp2, 16);
    temp2 = _mm_slli_epi64(temp2, 16);
    temp2 = _mm_srli_si128(temp2, 6);
    temp2 = _mm_slli_si128(temp2, 6);
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(0,2,1,3));
    reg.dst[2].i = _mm_or_si128(temp1, temp2);	
    }

ISV SwapChannels_16u_C3R201_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;

    // Fill First Dst reg
    temp1 = _mm_slli_si128(reg.src1[0].i, 4);
    temp1 = _mm_srli_si128(temp1, 10);
    temp1 = _mm_shufflelo_epi16(temp1, _MM_SHUFFLE(3,1,0,2));
    temp1 = _mm_slli_si128(temp1, 6);
    temp2 = _mm_slli_si128(reg.src1[1].i, 14);
    temp2 = _mm_srli_si128(temp2, 2);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(3,1,0,2));
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(3,1,0,2));
    temp2 = _mm_slli_epi64(temp2, 16);
    temp2 = _mm_srli_epi64(temp2, 16);
    temp2 = _mm_slli_si128(temp2, 6);
    temp2 = _mm_srli_si128(temp2, 6);
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(0,3,2,1));
    reg.dst[0].i = _mm_or_si128(temp1, temp2);

    // Fill Second Dst reg
    temp1 = _mm_srli_si128(reg.src1[0].i, 14);
    temp2 = _mm_srli_si128(reg.src1[2].i, 2);
    temp2 = _mm_slli_si128(temp2, 14);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shufflelo_epi16(reg.src1[1].i, _MM_SHUFFLE(2,1,3,0));
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(1,0,2,3));
    temp2 = _mm_srli_epi64(temp2, 16);
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(2,1,0,3));
    reg.dst[1].i = _mm_or_si128(temp1, temp2);

    // Fill Third Dst reg
    temp1 = _mm_slli_si128(reg.src1[2].i, 6);
    temp1 = _mm_srli_si128(temp1, 10);
    temp1 = _mm_shufflelo_epi16(temp1, _MM_SHUFFLE(3,1,0,2));
    temp1 = _mm_slli_si128(temp1, 4);
    temp2 = _mm_srli_si128(reg.src1[1].i, 14);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shufflelo_epi16(reg.src1[2].i, _MM_SHUFFLE(0,2,1,0));
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(2,1,3,0));
    temp2 = _mm_srli_epi64(temp2, 16);
    temp2 = _mm_slli_epi64(temp2, 16);
    temp2 = _mm_srli_si128(temp2, 6);
    temp2 = _mm_slli_si128(temp2, 6);
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(2,1,3,0));
    reg.dst[2].i = _mm_or_si128(temp1, temp2);	
    }

ISV SwapChannels_16u_C3R210_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;
    // Fill First Dst reg
    temp1 = _mm_slli_si128(reg.src1[0].i, 4);
    temp1 = _mm_srli_si128(temp1, 10);
    temp1 = _mm_shufflelo_epi16(temp1, _MM_SHUFFLE(3,0,1,2));
    temp1 = _mm_slli_si128(temp1, 6);
    temp2 = _mm_slli_si128(reg.src1[1].i, 14);
    temp2 = _mm_srli_si128(temp2, 2);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(3,0,1,2));
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(2,1,0,3));
    temp2 = _mm_slli_epi64(temp2, 16);
    temp2 = _mm_srli_epi64(temp2, 16);
    temp2 = _mm_slli_si128(temp2, 6);
    temp2 = _mm_srli_si128(temp2, 6);
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(0,3,2,1));
    reg.dst[0].i = _mm_or_si128(temp1, temp2);

    // Fill Second Dst reg
    temp1 = _mm_slli_si128(reg.src1[0].i, 2);
    temp1 = _mm_srli_si128(temp1, 14);
    temp2 = _mm_srli_si128(reg.src1[2].i, 2);
    temp2 = _mm_slli_si128(temp2, 14);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shufflelo_epi16(reg.src1[1].i, _MM_SHUFFLE(1,2,3,0));
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(0,1,2,3));
    temp2 = _mm_srli_epi64(temp2, 16);
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(2,1,0,3));
    reg.dst[1].i = _mm_or_si128(temp1, temp2);

    // Fill Third Dst reg
    temp1 = _mm_slli_si128(reg.src1[2].i, 6);
    temp1 = _mm_srli_si128(temp1, 10);
    temp1 = _mm_shufflelo_epi16(temp1, _MM_SHUFFLE(3,0,1,2));
    temp1 = _mm_slli_si128(temp1, 4);
    temp2 = _mm_srli_si128(reg.src1[1].i, 14);
    temp2 = _mm_slli_si128(temp2, 2);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shufflelo_epi16(reg.src1[2].i, _MM_SHUFFLE(0,2,1,0));
    temp2 = _mm_shufflehi_epi16(temp2, _MM_SHUFFLE(1,2,3,0));
    temp2 = _mm_srli_epi64(temp2, 16);
    temp2 = _mm_slli_epi64(temp2, 16);
    temp2 = _mm_srli_si128(temp2, 6);
    temp2 = _mm_slli_si128(temp2, 6);
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(0,2,1,3));
    reg.dst[2].i = _mm_or_si128(temp1, temp2);	
    }

// C3R 32s and 32f
ISV SwapChannels_32_C3R012_SSE2_I( RegFile &reg )
    {
    reg.dst[0].i = reg.src1[0].i;												// same order so just copy
    reg.dst[1].i = reg.src1[1].i; 
    reg.dst[2].i = reg.src1[2].i;
    }

ISV SwapChannels_32_C3R021_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;

    // Fill first Dst reg
    reg.dst[0].i = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,1,2,0));

    // Fill second Dst reg
    temp1 = _mm_shuffle_epi32(reg.src1[1].i, _MM_SHUFFLE(3,2,0,1));
    temp1 = _mm_slli_si128(temp1, 4);											// Make the last value to zero
    temp1 = _mm_srli_si128(temp1, 4);
    temp2 = _mm_slli_si128(reg.src1[2].i, 12);									// Fill the last value from the 3rd src reg
    reg.dst[1].i = _mm_or_si128(temp1, temp2);

    // Fill Third Dst reg
    temp1 = _mm_shuffle_epi32(reg.src1[2].i, _MM_SHUFFLE(2,3,1,0));
    temp1 = _mm_srli_si128(temp1, 4);											// Make the first value to zero
    temp1 = _mm_slli_si128(temp1, 4);
    temp2 = _mm_srli_si128(reg.src1[1].i, 12);									// Fill the first value from the 2nd src reg
    reg.dst[2].i = _mm_or_si128(temp1, temp2);
    }

ISV SwapChannels_32_C3R102_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;

    // Fill first Dst reg
    temp1 = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,2,0,1));
    temp1 = _mm_slli_si128(temp1, 4);											// Make the last value to zero
    temp1 = _mm_srli_si128(temp1, 4);
    temp2 = _mm_slli_si128(reg.src1[1].i, 12);									// Fill the last value from the 2nd src reg
    reg.dst[0].i = _mm_or_si128(temp1, temp2);

    // Fill second Dst reg	
    temp1 = _mm_shuffle_epi32(reg.src1[1].i, _MM_SHUFFLE(2,3,1,0));
    temp1 = _mm_srli_si128(temp1, 4);											// Make the first value to zero
    temp1 = _mm_slli_si128(temp1, 4);
    temp2 = _mm_srli_si128(reg.src1[0].i, 12);									// Fill the first value from the 1st src reg
    reg.dst[1].i = _mm_or_si128(temp1, temp2);

    // Fill Third Dst reg
    reg.dst[2].i = _mm_shuffle_epi32(reg.src1[2].i, _MM_SHUFFLE(3,1,2,0));
    }

ISV SwapChannels_32_C3R120_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;

    // Fill First dst reg
    temp1 = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,0,2,1));
    temp1 = _mm_slli_si128(temp1, 4);
    temp1 = _mm_srli_si128(temp1, 4);
    temp2 = _mm_slli_si128(reg.src1[1].i, 12);
    reg.dst[0].i = _mm_or_si128(temp1, temp2);

    // Fill Second dst reg
    temp1 = _mm_slli_si128(reg.src1[2].i, 12);
    temp2 = _mm_srli_si128(reg.src1[0].i, 12);
    temp2 = _mm_slli_si128(temp2, 4);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_srli_epi64(reg.src1[1].i, 32);
    reg.dst[1].i = _mm_or_si128(temp2, temp1);

    // Fill Third dst reg
    temp1 = _mm_shuffle_epi32(reg.src1[2].i, _MM_SHUFFLE(1,3,2,0));
    temp1 = _mm_srli_si128(temp1, 4);
    temp1 = _mm_slli_si128(temp1, 4);
    temp2 = _mm_slli_si128(reg.src1[1].i, 4);
    temp2 = _mm_srli_si128(temp2, 12);
    reg.dst[2].i = _mm_or_si128(temp1, temp2);
    }

ISV SwapChannels_32_C3R201_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;

    // Fill First Dst reg
    temp1 = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,1,0,2));
    temp1 = _mm_slli_si128(temp1, 4);
    temp1 = _mm_srli_si128(temp1, 4);
    temp2 = _mm_srli_si128(reg.src1[1].i, 4);
    temp2 = _mm_slli_si128(temp2, 12);
    reg.dst[0].i = _mm_or_si128(temp1, temp2);

    // Fill Second Dst reg
    temp1 = _mm_srli_si128(reg.src1[0].i, 12);
    temp2 = _mm_slli_si128(reg.src1[2].i, 12);
    temp2 = _mm_srli_si128(temp2, 4);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_slli_epi64(reg.src1[1].i, 32);
    reg.dst[1].i = _mm_or_si128(temp1, temp2);

    // Fill Third Dst reg
    temp1 = _mm_shuffle_epi32(reg.src1[2].i, _MM_SHUFFLE(2,1,3,0));
    temp1 = _mm_srli_si128(temp1, 4);
    temp1 = _mm_slli_si128(temp1, 4);
    temp2 = _mm_srli_si128(reg.src1[1].i, 12);
    reg.dst[2].i = _mm_or_si128(temp1, temp2);
    }

ISV SwapChannels_32_C3R210_SSE2_I( RegFile &reg )
    {
    __m128i temp1, temp2;

    // Fill First Dst reg
    temp1 = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,0,1,2));
    temp1 = _mm_slli_si128(temp1, 4);
    temp1 = _mm_srli_si128(temp1, 4);
    temp2 = _mm_srli_si128(reg.src1[1].i, 4);
    temp2 = _mm_slli_si128(temp2, 12);
    reg.dst[0].i = _mm_or_si128(temp1, temp2);

    // Fill Second Dst reg
    temp1 = _mm_srli_si128(reg.src1[0].i, 12);
    temp1 = _mm_slli_si128(temp1, 4);
    temp2 = _mm_slli_si128(reg.src1[2].i, 12);
    temp2 = _mm_srli_si128(temp2, 4);
    temp1 = _mm_or_si128(temp1, temp2);
    temp2 = _mm_shuffle_epi32(reg.src1[1].i, _MM_SHUFFLE(2,3,1,0));
    temp2 = _mm_slli_epi64(temp2, 32);
    temp2 = _mm_shufflelo_epi16(temp2, _MM_SHUFFLE(1,0,3,2));
    reg.dst[1].i = _mm_or_si128(temp1, temp2);

    // Fill Third Dst reg
    temp1 = _mm_shuffle_epi32(reg.src1[2].i, _MM_SHUFFLE(1,2,3,0));
    temp1 = _mm_srli_si128(temp1, 4);
    temp1 = _mm_slli_si128(temp1, 4);
    temp2 = _mm_slli_si128(reg.src1[1].i, 4);
    temp2 = _mm_srli_si128(temp2, 12);
    reg.dst[2].i = _mm_or_si128(temp1, temp2);
    }

// AC4R 8u
ISV SwapChannels_8u_AC4R012_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    Copy_With_Mask(reg.src1[0].i, reg.dst[0].i, maska);
    }

ISV SwapChannels_8u_AC4R021_SSE2_I( RegFile &reg, const __m128i &maskr, const __m128i &maskg, const __m128i &maskb, const __m128i &maska)
    {
    __m128i temp;

    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,maska);							// make dst to zeros and retain only alphas
    temp = _mm_and_si128( reg.src1[0].i, maskr);								// get the data of r channel
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);							// copy
    temp = _mm_and_si128( reg.src1[0].i, maskg);								// get the data of g channel
    temp = _mm_slli_si128(temp, 1);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);							// copy
    temp = _mm_and_si128( reg.src1[0].i, maskb);								// get the data of b channel
    temp = _mm_srli_si128(temp, 1);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);							// copy					    
    }

ISV SwapChannels_8u_AC4R102_SSE2_I( RegFile &reg,  const __m128i &maskr, const __m128i &maskg, const __m128i &maskb, const __m128i &maska)
    {
    __m128i temp;

    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,maska);							// make dst to zeros and retain only alphas
    temp = _mm_and_si128( reg.src1[0].i, maskr);								// get the data of r channel
    temp = _mm_slli_si128(temp, 1);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);							// copy
    temp = _mm_and_si128( reg.src1[0].i, maskg);								// get the data of g channel
    temp = _mm_srli_si128(temp, 1);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);							// copy
    temp = _mm_and_si128( reg.src1[0].i, maskb);								// get the data of b channel
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);							// copy
    }

ISV SwapChannels_8u_AC4R120_SSE2_I( RegFile &reg, const __m128i &maskr, const __m128i &maskgb, const __m128i &maska )
    {
    __m128i temp;
    reg.dst[0].i = _mm_and_si128(maska,reg.dst[0].i);							// make dst to zeros and retain only alphas
    temp = _mm_and_si128( reg.src1[0].i, maskgb);								// get the data of g,b channel
    temp = _mm_srli_si128(temp, 1);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);							// copy
    temp = _mm_and_si128( reg.src1[0].i, maskr);								// get the data of r channel
    temp = _mm_slli_si128(temp, 2);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);					
    }

ISV SwapChannels_8u_AC4R201_SSE2_I( RegFile &reg, const __m128i &maskrg, const __m128i &maskb, const __m128i &maska)
    {
    __m128i temp;
    reg.dst[0].i = _mm_and_si128(maska,reg.dst[0].i);							// make dst to zeros and retain only alphas
    temp = _mm_and_si128( reg.src1[0].i, maskrg);								// get the data of r,g channel
    temp = _mm_slli_si128(temp, 1);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);							// copy
    temp = _mm_and_si128( reg.src1[0].i, maskb);								// get the data of b channel
    temp = _mm_srli_si128(temp, 2);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);					
    }

ISV SwapChannels_8u_AC4R210_SSE2_I( RegFile &reg, const __m128i &maskr, const __m128i &maskg, const __m128i &maskb, const __m128i &maska )
    {
    __m128i temp;
    reg.dst[0].i = _mm_and_si128(reg.dst[0].i,maska);							// make dst to zeros and retain only alphas
    temp = _mm_and_si128( reg.src1[0].i, maskr);								// get the data of r channel
    temp = _mm_slli_si128(temp, 2);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);
    temp = _mm_and_si128( reg.src1[0].i, maskg);								// get the data of g channel
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);
    temp = _mm_and_si128( reg.src1[0].i, maskb);								// get the data of b channel
    temp = _mm_srli_si128(temp, 2);												// shift 
    reg.dst[0].i =  _mm_or_si128(reg.dst[0].i, temp);							// copy
    }

// AC4R 16u
ISV SwapChannels_16u_AC4R012_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    Copy_With_Mask(reg.src1[0].i, reg.dst[0].i, maska);
    }

ISV SwapChannels_16u_AC4R021_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(3,1,2,0));
    temp = _mm_shufflehi_epi16(temp, _MM_SHUFFLE(3,1,2,0));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

ISV SwapChannels_16u_AC4R102_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(3,2,0,1));
    temp = _mm_shufflehi_epi16(temp, _MM_SHUFFLE(3,2,0,1));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

ISV SwapChannels_16u_AC4R120_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(3,0,2,1));
    temp = _mm_shufflehi_epi16(temp, _MM_SHUFFLE(3,0,2,1));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

ISV SwapChannels_16u_AC4R201_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(3,1,0,2));
    temp = _mm_shufflehi_epi16(temp, _MM_SHUFFLE(3,1,0,2));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

ISV SwapChannels_16u_AC4R210_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shufflelo_epi16(reg.src1[0].i, _MM_SHUFFLE(3,0,1,2));
    temp = _mm_shufflehi_epi16(temp, _MM_SHUFFLE(3,0,1,2));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

// AC4R 32s and 32f
ISV SwapChannels_32_AC4R012_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    Copy_With_Mask(reg.src1[0].i, reg.dst[0].i, maska );
    }

ISV SwapChannels_32_AC4R021_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,1,2,0));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

ISV SwapChannels_32_AC4R102_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,2,0,1));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

ISV SwapChannels_32_AC4R120_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,0,2,1));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

ISV SwapChannels_32_AC4R201_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,1,0,2));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

ISV SwapChannels_32_AC4R210_SSE2_I( RegFile &reg, const __m128i &maska )
    {
    __m128i temp;
    temp = _mm_shuffle_epi32(reg.src1[0].i, _MM_SHUFFLE(3,0,1,2));
    Copy_With_Mask(temp, reg.dst[0].i, maska);
    }

// C3IR 8u
ISV SwapChannels_8u_C3IR021_SSE2_I( RegFile &reg, const __m128i &maskr, const __m128i &maskg, const __m128i &maskb )
    {	
    __m128i temp1, temp2;
    temp1 = _mm_and_si128(reg.dst[0].i,maskr);									// make all locations to zero except red channel
    temp2 = _mm_and_si128(reg.dst[0].i,maskg);									// get g's
    temp2 = _mm_slli_si128(temp2,1);											// shift by 1 byte
    temp1 = _mm_or_si128(temp1, temp2);											// copy
    temp2 = _mm_and_si128(reg.dst[0].i,maskb);									// get b's
    temp2 = _mm_srli_si128(temp2,1);											// shift by 1 byte
    reg.dst[0].i = _mm_or_si128(temp1, temp2);									// copy
    }

ISV SwapChannels_8u_C3IR102_SSE2_I( RegFile &reg, const __m128i &maskr, const __m128i &maskg, const __m128i &maskb )
    {
    __m128i temp1, temp2;
    temp1 = _mm_and_si128(reg.dst[0].i,maskb);									// make all locations to zero except blue channel
    temp2 = _mm_and_si128(reg.dst[0].i,maskr);									// get r's
    temp2 = _mm_slli_si128(temp2,1);											// shift by 1 byte
    temp1 = _mm_or_si128(temp1, temp2);											// copy
    temp2 = _mm_and_si128(reg.dst[0].i,maskg);									// get g's
    temp2 = _mm_srli_si128(temp2,1);											// shift by 1 byte
    reg.dst[0].i = _mm_or_si128(temp1,temp2);									// copy
    }

ISV SwapChannels_8u_C3IR120_SSE2_I( RegFile &reg, const __m128i &maskr, const __m128i &maskgb )
    {
    __m128i temp1, temp2;
    temp1 = _mm_and_si128(reg.dst[0].i, maskgb);								// get g and b's data
    temp1 = _mm_srli_si128(temp1, 1);											// shift by 1 byte..
    temp2 = _mm_and_si128(reg.dst[0].i, maskr);									// get r's data
    temp2 = _mm_slli_si128(temp2,2);											// shift by 2 bytes
    reg.dst[0].i = _mm_or_si128(temp2,temp1);									// copy
    }

ISV SwapChannels_8u_C3IR201_SSE2_I( RegFile &reg, const __m128i &maskrg, const __m128i &maskb )
    {
    __m128i temp1, temp2;
    temp1 = _mm_and_si128(reg.dst[0].i,maskrg);									// get r and g's data
    temp1 = _mm_slli_si128(temp1,1);											// shift by 1 byte..
    temp2 = _mm_and_si128(reg.dst[0].i,maskb);									// get b's data
    temp2 = _mm_srli_si128(temp2,2);											// shift by 2 bytes
    reg.dst[0].i = _mm_or_si128(temp2, temp1);									// copy
    }

ISV SwapChannels_8u_C3IR210_SSE2_I( RegFile &reg, const __m128i &maskr, const __m128i &maskg, const __m128i &maskb )
    {
    __m128i temp1, temp2;
    temp1 = _mm_and_si128(reg.dst[0].i, maskg);									// make all locations to zero except green channel
    temp2 = _mm_and_si128(reg.dst[0].i, maskr);									// get r's
    temp2 = _mm_slli_si128(temp2, 2);											// shift by 2 bytes
    temp1 = _mm_or_si128(temp1, temp2);											// copy
    temp2 = _mm_and_si128(reg.dst[0].i, maskb);									// get b's
    temp2 = _mm_srli_si128(temp2,2);											// shift by 2 bytes
    reg.dst[0].i = _mm_or_si128(temp2, temp1);									// copy
    }
} // OPT_LEVEL

#endif



