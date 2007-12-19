/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef _AAN_IDCT_H
#define _AAN_IDCT_H

/*****************************************************/
#define DEBUG_TRANSPOSE_BLOCK 0

/*****************************************************/
const unsigned int NUM_ROWS = 8;
const unsigned int NUM_COLS = 8;

/*****************************************************/
typedef Fw16s	(*Pointer8x8Block16s)[NUM_ROWS][NUM_COLS];
typedef Fw8u	(*Pointer8x8Block8u)[NUM_ROWS][NUM_COLS];
typedef Fw16s	(*Pointer8x8Block16s16s)[NUM_ROWS][2*NUM_COLS];
typedef Fw8u	(*Pointer8x8Block8u8u)[NUM_ROWS][2*NUM_COLS];
typedef Fw16s	(*Pointer8x8Block16s16sOneEmptyRow)[2*NUM_ROWS][2*NUM_COLS];
typedef Fw8u	(*Pointer8x8Block8u8uOneEmptyRow)[2*NUM_ROWS][2*NUM_COLS];

#endif
