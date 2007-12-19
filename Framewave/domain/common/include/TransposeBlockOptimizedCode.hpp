/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/****************************************************************/
#if DEBUG_TRANSPOSE_BLOCK
    std::cout << pSrc << std::endl;
#endif
#if OREAD_FROM_REGS
    xmm0 = *(__m128i*)&_mm_load_pd((double*)&xmm8);
    xmm1 = *(__m128i*)&_mm_load_pd((double*)&xmm9);
    xmm2 = *(__m128i*)&_mm_load_pd((double*)&xmm10);
    xmm3 = *(__m128i*)&_mm_load_pd((double*)&xmm11);
    xmm4 = *(__m128i*)&_mm_load_pd((double*)&xmm12);
    xmm5 = *(__m128i*)&_mm_load_pd((double*)&xmm13);
    xmm6 = *(__m128i*)&_mm_load_pd((double*)&xmm14);
    xmm7 = *(__m128i*)&_mm_load_pd((double*)&xmm15);
#else
    xmm0 = _mm_load_si128((__m128i *)(((char*)&pSrc[0][0]) + 0*NUM_COLS*sizeof(Fw16s))); 
    xmm1 = _mm_load_si128((__m128i *)(((char*)&pSrc[0][0]) + 1*NUM_COLS*sizeof(Fw16s))); 
    xmm2 = _mm_load_si128((__m128i *)(((char*)&pSrc[0][0]) + 2*NUM_COLS*sizeof(Fw16s))); 
    xmm3 = _mm_load_si128((__m128i *)(((char*)&pSrc[0][0]) + 3*NUM_COLS*sizeof(Fw16s))); 
    xmm4 = _mm_load_si128((__m128i *)(((char*)&pSrc[0][0]) + 4*NUM_COLS*sizeof(Fw16s))); 
    xmm5 = _mm_load_si128((__m128i *)(((char*)&pSrc[0][0]) + 5*NUM_COLS*sizeof(Fw16s))); 
    xmm6 = _mm_load_si128((__m128i *)(((char*)&pSrc[0][0]) + 6*NUM_COLS*sizeof(Fw16s))); 
    xmm7 = _mm_load_si128((__m128i *)(((char*)&pSrc[0][0]) + 7*NUM_COLS*sizeof(Fw16s))); 

#endif

#if DEBUG_TRANSPOSE_BLOCK
    std::cout << "Starting loading" << std::endl;
    std::cout << xmm0 << std::endl;
    std::cout << xmm1 << std::endl;
    std::cout << xmm2 << std::endl;
    std::cout << xmm3 << std::endl;
    std::cout << xmm4 << std::endl;
    std::cout << xmm5 << std::endl;
    std::cout << xmm6 << std::endl;
    std::cout << xmm7 << std::endl;
    std::cout << "Done loading" << std::endl;
#endif
    
    //step 1
    xmm8 = _mm_unpacklo_epi64(xmm0, xmm4);
    xmm9 = _mm_unpackhi_epi64(xmm0, xmm4);
    xmm0 = _mm_load_si128(&xmm8);
    xmm4 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi64(xmm1, xmm5);
    xmm9 = _mm_unpackhi_epi64(xmm1, xmm5);    
    xmm1 = _mm_load_si128(&xmm8);
    xmm5 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi64(xmm2, xmm6);
    xmm9 = _mm_unpackhi_epi64(xmm2, xmm6);    
    xmm2 = _mm_load_si128(&xmm8);
    xmm6 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi64(xmm3, xmm7);
    xmm9 = _mm_unpackhi_epi64(xmm3, xmm7);
    xmm3 = _mm_load_si128(&xmm8);
    xmm7 = _mm_load_si128(&xmm9);


#if DEBUG_TRANSPOSE_BLOCK
    std::cout << "Output of first step, start" << std::endl;
    std::cout << xmm0 << std::endl;
    std::cout << xmm1 << std::endl;
    std::cout << xmm2 << std::endl;
    std::cout << xmm3 << std::endl;
    std::cout << xmm4 << std::endl;
    std::cout << xmm5 << std::endl;
    std::cout << xmm6 << std::endl;
    std::cout << xmm7 << std::endl;
    std::cout << "Output of first step, stop" << std::endl;
#endif

    //step 2
    xmm8 = _mm_unpacklo_epi16(xmm0, xmm1);
    xmm9 = _mm_unpackhi_epi16(xmm0, xmm1);
    xmm0 = _mm_load_si128(&xmm8);
    xmm1 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi16(xmm2, xmm3);
    xmm9 = _mm_unpackhi_epi16(xmm2, xmm3);
    xmm2 = _mm_load_si128(&xmm8);
    xmm3 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi16(xmm4, xmm5);
    xmm9 = _mm_unpackhi_epi16(xmm4, xmm5);
    xmm4 = _mm_load_si128(&xmm8);
    xmm5 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi16(xmm6, xmm7);
    xmm9 = _mm_unpackhi_epi16(xmm6, xmm7);
    xmm6 = _mm_load_si128(&xmm8);
    xmm7 = _mm_load_si128(&xmm9);

#if DEBUG_TRANSPOSE_BLOCK
    std::cout << "Output of second step, start" << std::endl;
    std::cout << xmm0 << std::endl;
    std::cout << xmm1 << std::endl;
    std::cout << xmm2 << std::endl;
    std::cout << xmm3 << std::endl;
    std::cout << xmm4 << std::endl;
    std::cout << xmm5 << std::endl;
    std::cout << xmm6 << std::endl;
    std::cout << xmm7 << std::endl;
    std::cout << "Output of second step, stop" << std::endl;
#endif

    //step 3
    xmm8 = _mm_unpacklo_epi32(xmm0, xmm2);
    xmm9 = _mm_unpackhi_epi32(xmm0, xmm2);
    xmm0 = _mm_load_si128(&xmm8);
    xmm2 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi32(xmm1, xmm3);
    xmm9 = _mm_unpackhi_epi32(xmm1, xmm3);
    xmm1 = _mm_load_si128(&xmm8);
    xmm3 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi32(xmm4, xmm6);
    xmm9 = _mm_unpackhi_epi32(xmm4, xmm6);
    xmm4 = _mm_load_si128(&xmm8);
    xmm6 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi32(xmm5, xmm7);
    xmm9 = _mm_unpackhi_epi32(xmm5, xmm7);
    xmm5 = _mm_load_si128(&xmm8);
    xmm7 = _mm_load_si128(&xmm9);

#if DEBUG_TRANSPOSE_BLOCK
    std::cout << "Output of third step, start" << std::endl;
    std::cout << xmm0 << std::endl;
    std::cout << xmm1 << std::endl;
    std::cout << xmm2 << std::endl;
    std::cout << xmm3 << std::endl;
    std::cout << xmm4 << std::endl;
    std::cout << xmm5 << std::endl;
    std::cout << xmm6 << std::endl;
    std::cout << xmm7 << std::endl;
    std::cout << "Output of third step, stop" << std::endl;
#endif
    
    //step 4
    xmm8 = _mm_unpacklo_epi64(xmm0, xmm1);
    xmm9 = _mm_unpackhi_epi64(xmm0, xmm1);
    xmm0 = _mm_load_si128(&xmm8);
    xmm1 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi64(xmm2, xmm3);
    xmm9 = _mm_unpackhi_epi64(xmm2, xmm3);    
    xmm2 = _mm_load_si128(&xmm8);
    xmm3 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi64(xmm4, xmm5);
    xmm9 = _mm_unpackhi_epi64(xmm4, xmm5);    
    xmm4 = _mm_load_si128(&xmm8);
    xmm5 = _mm_load_si128(&xmm9);

    xmm8 = _mm_unpacklo_epi64(xmm6, xmm7);
    xmm9 = _mm_unpackhi_epi64(xmm6, xmm7);    
    xmm6 = _mm_load_si128(&xmm8);
    xmm7 = _mm_load_si128(&xmm9);

#if DEBUG_TRANSPOSE_BLOCK
    std::cout << "Output of fourth step, start" << std::endl;
    std::cout << xmm0 << std::endl;
    std::cout << xmm1 << std::endl;
    std::cout << xmm2 << std::endl;
    std::cout << xmm3 << std::endl;
    std::cout << xmm4 << std::endl;
    std::cout << xmm5 << std::endl;
    std::cout << xmm6 << std::endl;
    std::cout << xmm7 << std::endl;
    std::cout << "Output of fourth step, stop" << std::endl;
#endif
