/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __3D_LUT_H__
#define __3D_LUT_H__

#include "fe.h"
#include "fwImage.h"
#include "FwSharedCode.h"

struct LUTSpec
    {
    XMM128 cubeDimensions_i;
    XMM128 cubeDimensions_f;
    XMM128 cubeDimensions_16u;

    XMM128 maxIndex_i;
    XMM128 maxIndex_f;
    XMM128 maxIndex_16u;

    XMM128 cubeMax_i;
    XMM128 cubeMax_f;
    XMM128 cubeMax_16u;

    Fw16u  cubeDimensions;
    Fw16u  maxIndex;
    Fw16u  cubeMax;
    };

namespace OPT_LEVEL
{

IS FwStatus LookUp3DSpecInitAlloc(const Fw16u cubeDimensions, const Fw16u cubeMax, FwiLUTSpec** ppLUTSpec)
    {
    *ppLUTSpec = (FwiLUTSpec*)fwMalloc(sizeof(struct LUTSpec));
    FwiLUTSpec* pLUTSpec = *ppLUTSpec;

  	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
        pLUTSpec->cubeDimensions_i.i = _mm_set1_epi32(cubeDimensions);
        pLUTSpec->cubeDimensions_f.f = _mm_cvtepi32_ps(pLUTSpec->cubeDimensions_i.i);
        pLUTSpec->cubeDimensions_16u.i = _mm_set1_epi16(cubeDimensions);

        pLUTSpec->maxIndex_i.i = _mm_set1_epi32(cubeDimensions-1);
        pLUTSpec->maxIndex_f.f = _mm_cvtepi32_ps(pLUTSpec->maxIndex_i.i);
        pLUTSpec->maxIndex_16u.i = _mm_set1_epi16(cubeDimensions-1);

        pLUTSpec->cubeMax_i.i = _mm_set1_epi32(cubeMax);
        pLUTSpec->cubeMax_f.f = _mm_cvtepi32_ps(pLUTSpec->cubeMax_i.i);
        pLUTSpec->cubeMax_16u.i = _mm_set1_epi16(cubeMax);

    case DT_REFR:
    default:

        pLUTSpec->cubeDimensions = cubeDimensions;
        pLUTSpec->maxIndex = cubeDimensions-1;
        pLUTSpec->cubeMax = cubeMax;
     }

    return fwStsNoErr;
    }

ISV LookUp3D_eightPixel(XMM128 &x, XMM128 &y,XMM128 &z,const Fw16u *pCube, FwiLUTSpec *pLUTSpec)
    {
    x;y;z;pCube;pLUTSpec;

    XMM128 fx,fy,fz,fxhi,fyhi,fzhi;
    XMM128 z1,z2,z3,z4,z5,z6,z7,z1hi,z2hi,z3hi,z4hi,z5hi,z6hi,z7hi;
    XMM128 y1,y2,y3,y4,y5,y6,y7,y1hi,y2hi,y3hi,y4hi,y5hi,y6hi,y7hi;
    XMM128 x1,x2,x3,x4,x5,x6,x7,x1hi,x2hi,x3hi,x4hi,x5hi,x6hi,x7hi;
    XMM128 ix0,iy0,iz0,ix1,iy1,iz1,ixhi0,iyhi0,izhi0,ixhi1,iyhi1,izhi1;
    XMM128 fraction_x,fraction_y,fraction_z,fraction_xhi,fraction_yhi,fraction_zhi;
    XMM128 index1,index2,index3,index4,index5,index6,index7,index8;
    XMM128 index1hi,index2hi,index3hi,index4hi,index5hi,index6hi,index7hi,index8hi;
    XMM128 vertex1_0,vertex2_0,vertex3_0,vertex4_0,vertex5_0,vertex6_0,vertex7_0,vertex8_0;
    XMM128 vertex1hi_0,vertex2hi_0,vertex3hi_0,vertex4hi_0,vertex5hi_0,vertex6hi_0,vertex7hi_0,vertex8hi_0;
    XMM128 vertex1_1,vertex2_1,vertex3_1,vertex4_1,vertex5_1,vertex6_1,vertex7_1,vertex8_1;
    XMM128 vertex1hi_1,vertex2hi_1,vertex3hi_1,vertex4hi_1,vertex5hi_1,vertex6hi_1,vertex7hi_1,vertex8hi_1;
    XMM128 vertex1_2,vertex2_2,vertex3_2,vertex4_2,vertex5_2,vertex6_2,vertex7_2,vertex8_2;
    XMM128 vertex1hi_2,vertex2hi_2,vertex3hi_2,vertex4hi_2,vertex5hi_2,vertex6hi_2,vertex7hi_2,vertex8hi_2;

    // check if overflow
    //if(x>pLUTSpec->cubeMax) x = pLUTSpec->cubeMax;
    XMM128 temp;
    temp.i = _mm_cmpgt_epi16(x.i, pLUTSpec->cubeMax_16u.i);
    int mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        x.i = _mm_andnot_si128(temp.i, x.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_16u.i);
        x.i = _mm_or_si128(x.i, temp.i);
        }

    //if(y>pLUTSpec->cubeMax) y = pLUTSpec->cubeMax;
    temp.i = _mm_cmpgt_epi16(y.i, pLUTSpec->cubeMax_16u.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        y.i = _mm_andnot_si128(temp.i, y.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_16u.i);
        y.i = _mm_or_si128(y.i, temp.i);
        }
    //if(z>pLUTSpec->cubeMax) z = pLUTSpec->cubeMax;
    temp.i = _mm_cmpgt_epi16(z.i, pLUTSpec->cubeMax_16u.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        z.i = _mm_andnot_si128(temp.i, z.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_16u.i);
        z.i = _mm_or_si128(z.i, temp.i);
        }
    
    // map the input pixel value onto the cube
    XMM128 zero,xhi,yhi,zhi;
    zero.i  = _mm_setzero_si128(); // x,y and z contain 8 elements upto this point

    xhi.i = _mm_unpackhi_epi16(x.i, zero.i); // splitting x,y and z to 2 registers each
    yhi.i = _mm_unpackhi_epi16(y.i, zero.i);
    zhi.i = _mm_unpackhi_epi16(z.i, zero.i);

    x.i   = _mm_unpacklo_epi16(x.i, zero.i);
    y.i   = _mm_unpacklo_epi16(y.i, zero.i);
    z.i   = _mm_unpacklo_epi16(z.i, zero.i);


    //temp = (float)pLUTSpec->maxIndex/pLUTSpec->cubeMax;    
    temp.f = _mm_div_ps(pLUTSpec->maxIndex_f.f, pLUTSpec->cubeMax_f.f);
    //fx   = x * temp;
    fxhi.f = _mm_cvtepi32_ps(xhi.i);
    fxhi.f = _mm_mul_ps(fxhi.f, temp.f);
    fx.f   = _mm_cvtepi32_ps(x.i);
    fx.f   = _mm_mul_ps(fx.f, temp.f);
    //fy   = y * temp;
    fyhi.f = _mm_cvtepi32_ps(yhi.i);
    fyhi.f = _mm_mul_ps(fyhi.f, temp.f);
    fy.f   = _mm_cvtepi32_ps(y.i);
    fy.f   = _mm_mul_ps(fy.f, temp.f);
    //fz   = z * temp;
    fzhi.f = _mm_cvtepi32_ps(zhi.i);
    fzhi.f = _mm_mul_ps(fzhi.f, temp.f);
    fz.f   = _mm_cvtepi32_ps(z.i);
    fz.f   = _mm_mul_ps(fz.f, temp.f);

    // get the indices as integers
    //ix0   = (int)fx;
    ixhi0.i = _mm_cvttps_epi32(fxhi.f);
    ix0.i   = _mm_cvttps_epi32(fx.f);
    //iy0   = (int)fy;
    iyhi0.i   = _mm_cvttps_epi32(fyhi.f);
    iy0.i   = _mm_cvttps_epi32(fy.f);
    //iz0   = (int)fz;
    izhi0.i = _mm_cvttps_epi32(fzhi.f);
    iz0.i   = _mm_cvttps_epi32(fz.f);

    // join ix0 and ixhi0, iy0 and iyhi0, iz0 and izhi0 // from here ix0,iy0,iz0,ix1,iy1,iz1 are seen 16u data
    ix0.i = _mm_packs_epi32(ix0.i, ixhi0.i);
    iy0.i = _mm_packs_epi32(iy0.i, iyhi0.i);
    iz0.i = _mm_packs_epi32(iz0.i, izhi0.i);

    //ix1 = (ix0 == pLUTSpec->maxIndex) ? ix0 : ix0+1;
    ix1.i = _mm_setzero_si128();
    ix1.i = _mm_cmpeq_epi16(ix1.i,ix1.i);
    ix1.i = _mm_srli_epi16(ix1.i, 15);           // ix1 is all 1's now

    // use ix1 to increment iy0, iz0, ix0 by 1
    iy1.i = _mm_add_epi16(ix1.i, iy0.i); 
    iz1.i = _mm_add_epi16(ix1.i, iz0.i);
    ix1.i = _mm_add_epi16(ix1.i, ix0.i);

    //ix1 = (ix0 == pLUTSpec->maxIndex) ? ix0 : ix0+1;
    temp.i = _mm_cmpeq_epi16(ix0.i, pLUTSpec->maxIndex_16u.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        ix1.i = _mm_andnot_si128(temp.i, ix1.i);
        temp.i = _mm_and_si128(temp.i, ix0.i);
        ix1.i = _mm_or_si128(ix1.i, temp.i);
        }

    //iy1 = (iy0 == pLUTSpec->maxIndex) ? iy0 : iy0+1;
    temp.i = _mm_cmpeq_epi16(iy0.i, pLUTSpec->maxIndex_16u.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        iy1.i = _mm_andnot_si128(temp.i, iy1.i);
        temp.i = _mm_and_si128(temp.i, iy0.i);
        iy1.i = _mm_or_si128(iy1.i, temp.i);
        }

    //iz1 = (iz0 == pLUTSpec->maxIndex) ? iz0 : iz0+1;
    temp.i = _mm_cmpeq_epi16(iz0.i, pLUTSpec->maxIndex_16u.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        iz1.i = _mm_andnot_si128(temp.i, iz1.i);
        temp.i = _mm_and_si128(temp.i, iz0.i);
        iz1.i = _mm_or_si128(iz1.i, temp.i);
        }

    // unjoin ix0 and ixhi0, iy0 and iyhi0, iz0 and izhi0 // from here ix0,iy0,iz0,ix1,iy1,iz1 are seen 32u data
    ixhi0.i = _mm_unpackhi_epi16(ix0.i, zero.i);
    iyhi0.i = _mm_unpackhi_epi16(iy0.i, zero.i);
    izhi0.i = _mm_unpackhi_epi16(iz0.i, zero.i);
    ix0.i   = _mm_unpacklo_epi16(ix0.i, zero.i);
    iy0.i   = _mm_unpacklo_epi16(iy0.i, zero.i);
    iz0.i   = _mm_unpacklo_epi16(iz0.i, zero.i);

    ixhi1.i = _mm_unpackhi_epi16(ix1.i, zero.i);
    iyhi1.i = _mm_unpackhi_epi16(iy1.i, zero.i);
    izhi1.i = _mm_unpackhi_epi16(iz1.i, zero.i);
    ix1.i   = _mm_unpacklo_epi16(ix1.i, zero.i);
    iy1.i   = _mm_unpacklo_epi16(iy1.i, zero.i);
    iz1.i   = _mm_unpacklo_epi16(iz1.i, zero.i);

    // get the fractional parts
    //fraction_x   = fx - ix0;
    ixhi0.f = _mm_cvtepi32_ps(ixhi0.i);
    fraction_xhi.f = _mm_sub_ps(fxhi.f, ixhi0.f);
    ix0.f = _mm_cvtepi32_ps(ix0.i);
    fraction_x.f = _mm_sub_ps(fx.f, ix0.f);
    //fraction_y   = fy - iy0;
    iyhi0.f = _mm_cvtepi32_ps(iyhi0.i);
    fraction_yhi.f = _mm_sub_ps(fyhi.f, iyhi0.f);
    iy0.f = _mm_cvtepi32_ps(iy0.i);
    fraction_y.f = _mm_sub_ps(fy.f, iy0.f);
    //fraction_z   = fz - iz0;
    izhi0.f = _mm_cvtepi32_ps(izhi0.i);
    fraction_zhi.f = _mm_sub_ps(fzhi.f, izhi0.f);
    iz0.f = _mm_cvtepi32_ps(iz0.i);
    fraction_z.f = _mm_sub_ps(fz.f, iz0.f);

    // get the 8 vertices surrounding the given input point
    //int temp1 = ix0*pLUTSpec->cubeDimensions;
    XMM128 temp1,temp2,temp3,temp4,three,temp1hi,temp2hi,temp4hi;
    temp1hi.f = _mm_mul_ps(ixhi0.f, pLUTSpec->cubeDimensions_f.f);
    temp1.f = _mm_mul_ps(ix0.f, pLUTSpec->cubeDimensions_f.f);
    //int temp2 = ix1*pLUTSpec->cubeDimensions;
    ixhi1.f = _mm_cvtepi32_ps(ixhi1.i);
    temp2hi.f = _mm_mul_ps(ixhi1.f, pLUTSpec->cubeDimensions_f.f);
    ix1.f = _mm_cvtepi32_ps(ix1.i);
    temp2.f = _mm_mul_ps(ix1.f, pLUTSpec->cubeDimensions_f.f);
    //int temp3 = pLUTSpec->cubeDimensions * 3;
    three.f = _mm_set1_ps(3.0);
    temp3.f = _mm_mul_ps(pLUTSpec->cubeDimensions_f.f, three.f); // use the same temp3 for hi also
    //int temp4 = (iz1 - iz0)*3;
    izhi1.f = _mm_cvtepi32_ps(izhi1.i);
    iz1.f = _mm_cvtepi32_ps(iz1.i);
    //iz0.f = _mm_cvtepi32_ps(iz0.i); //iz0 is already converted
    temp4hi.f = _mm_sub_ps(izhi1.f, izhi0.f);
    temp4hi.f = _mm_mul_ps(temp4hi.f, three.f);
    temp4.f = _mm_sub_ps(iz1.f, iz0.f);
    temp4.f = _mm_mul_ps(temp4.f, three.f);

    //index1 = ((temp1 + iy0)*pLUTSpec->cubeDimensions[2] + iz0)*3;
    index1hi.f = _mm_add_ps(temp1hi.f, iyhi0.f);
    index1hi.f = _mm_mul_ps(index1hi.f, pLUTSpec->cubeDimensions_f.f);
    index1hi.f = _mm_add_ps(index1hi.f, izhi0.f);
    index1hi.f = _mm_mul_ps(index1hi.f, three.f);    

    index1.f = _mm_add_ps(temp1.f, iy0.f);
    index1.f = _mm_mul_ps(index1.f, pLUTSpec->cubeDimensions_f.f);
    index1.f = _mm_add_ps(index1.f, iz0.f);
    index1.f = _mm_mul_ps(index1.f, three.f);    
    //index2 = index1 + (temp2 - temp1) * temp3;
    index2hi.f = _mm_sub_ps(temp2hi.f, temp1hi.f);
    index2hi.f = _mm_mul_ps(index2hi.f, temp3.f);
    index4hi.f = index2hi.f;                        // fill the temporary value here
    index2hi.f = _mm_add_ps(index2hi.f, index1hi.f);

    index2.f = _mm_sub_ps(temp2.f, temp1.f);
    index2.f = _mm_mul_ps(index2.f, temp3.f);
    index4.f = index2.f;                        // fill the temporary value here
    index2.f = _mm_add_ps(index2.f, index1.f);
    //index3  = index1 + (iy1 - iy0) * temp3;
    iyhi1.f    = _mm_cvtepi32_ps(iyhi1.i);
    index3hi.f = _mm_sub_ps(iyhi1.f, iyhi0.f);
    index3hi.f = _mm_mul_ps(index3hi.f, temp3.f);
    index3hi.f = _mm_add_ps(index3hi.f, index1hi.f);

    iy1.f    = _mm_cvtepi32_ps(iy1.i);
    index3.f = _mm_sub_ps(iy1.f, iy0.f);
    index3.f = _mm_mul_ps(index3.f, temp3.f);
    index3.f = _mm_add_ps(index3.f, index1.f);
    //index4  = index3 + (temp2 - temp1) * temp3; == index3+index2-index1
    index4hi.f = _mm_add_ps(index4hi.f, index3hi.f); 
    index4.f = _mm_add_ps(index4.f, index3.f); 

    index1hi.i = _mm_cvttps_epi32(index1hi.f);
    index2hi.i = _mm_cvttps_epi32(index2hi.f);
    index3hi.i = _mm_cvttps_epi32(index3hi.f);
    index4hi.i = _mm_cvttps_epi32(index4hi.f);

    index1.i = _mm_cvttps_epi32(index1.f);
    index2.i = _mm_cvttps_epi32(index2.f);
    index3.i = _mm_cvttps_epi32(index3.f);
    index4.i = _mm_cvttps_epi32(index4.f);
    ///index5  = index1 + temp4;
    temp4hi.i = _mm_cvttps_epi32(temp4hi.f);
    index5hi.i = _mm_add_epi32(index1hi.i, temp4hi.i);
    temp4.i = _mm_cvttps_epi32(temp4.f);
    index5.i = _mm_add_epi32(index1.i, temp4.i);
    //index6  = index2 + temp4;
    index6hi.i = _mm_add_epi32(index2hi.i, temp4hi.i);
    index6.i = _mm_add_epi32(index2.i, temp4.i);
    //index7  = index3 + temp4;
    index7hi.i = _mm_add_epi32(index3hi.i, temp4hi.i);
    index7.i = _mm_add_epi32(index3.i, temp4.i);
    //index8  = index4 + temp4;
    index8hi.i = _mm_add_epi32(index4hi.i, temp4hi.i);
    index8.i = _mm_add_epi32(index4.i, temp4.i);

    //vertex1 = &pCube[index1];
    vertex1hi_0.i = _mm_set_epi32(pCube[index1hi.u32[3]],pCube[index1hi.u32[2]],pCube[index1hi.u32[1]],pCube[index1hi.u32[0]]);
    vertex1hi_1.i = _mm_set_epi32(pCube[index1hi.u32[3]+1],pCube[index1hi.u32[2]+1],pCube[index1hi.u32[1]+1],pCube[index1hi.u32[0]+1]);
    vertex1hi_2.i = _mm_set_epi32(pCube[index1hi.u32[3]+2],pCube[index1hi.u32[2]+2],pCube[index1hi.u32[1]+2],pCube[index1hi.u32[0]+2]);
    
    vertex1hi_0.f = _mm_cvtepi32_ps(vertex1hi_0.i);
    vertex1hi_1.f = _mm_cvtepi32_ps(vertex1hi_1.i);
    vertex1hi_2.f = _mm_cvtepi32_ps(vertex1hi_2.i);

    vertex1_0.i = _mm_set_epi32(pCube[index1.u32[3]],pCube[index1.u32[2]],pCube[index1.u32[1]],pCube[index1.u32[0]]);
    vertex1_1.i = _mm_set_epi32(pCube[index1.u32[3]+1],pCube[index1.u32[2]+1],pCube[index1.u32[1]+1],pCube[index1.u32[0]+1]);
    vertex1_2.i = _mm_set_epi32(pCube[index1.u32[3]+2],pCube[index1.u32[2]+2],pCube[index1.u32[1]+2],pCube[index1.u32[0]+2]);
    
    vertex1_0.f = _mm_cvtepi32_ps(vertex1_0.i);
    vertex1_1.f = _mm_cvtepi32_ps(vertex1_1.i);
    vertex1_2.f = _mm_cvtepi32_ps(vertex1_2.i);
    //vertex2 = &pCube[index2];
    vertex2hi_0.i = _mm_set_epi32(pCube[index2hi.u32[3]],pCube[index2hi.u32[2]],pCube[index2hi.u32[1]],pCube[index2hi.u32[0]]);
    vertex2hi_1.i = _mm_set_epi32(pCube[index2hi.u32[3]+1],pCube[index2hi.u32[2]+1],pCube[index2hi.u32[1]+1],pCube[index2hi.u32[0]+1]);
    vertex2hi_2.i = _mm_set_epi32(pCube[index2hi.u32[3]+2],pCube[index2hi.u32[2]+2],pCube[index2hi.u32[1]+2],pCube[index2hi.u32[0]+2]);

    vertex2hi_0.f = _mm_cvtepi32_ps(vertex2hi_0.i);
    vertex2hi_1.f = _mm_cvtepi32_ps(vertex2hi_1.i);
    vertex2hi_2.f = _mm_cvtepi32_ps(vertex2hi_2.i);

    vertex2_0.i = _mm_set_epi32(pCube[index2.u32[3]],pCube[index2.u32[2]],pCube[index2.u32[1]],pCube[index2.u32[0]]);
    vertex2_1.i = _mm_set_epi32(pCube[index2.u32[3]+1],pCube[index2.u32[2]+1],pCube[index2.u32[1]+1],pCube[index2.u32[0]+1]);
    vertex2_2.i = _mm_set_epi32(pCube[index2.u32[3]+2],pCube[index2.u32[2]+2],pCube[index2.u32[1]+2],pCube[index2.u32[0]+2]);

    vertex2_0.f = _mm_cvtepi32_ps(vertex2_0.i);
    vertex2_1.f = _mm_cvtepi32_ps(vertex2_1.i);
    vertex2_2.f = _mm_cvtepi32_ps(vertex2_2.i);
    //vertex3 = &pCube[index3];
    vertex3hi_0.i = _mm_set_epi32(pCube[index3hi.u32[3]],pCube[index3hi.u32[2]],pCube[index3hi.u32[1]],pCube[index3hi.u32[0]]);
    vertex3hi_1.i = _mm_set_epi32(pCube[index3hi.u32[3]+1],pCube[index3hi.u32[2]+1],pCube[index3hi.u32[1]+1],pCube[index3hi.u32[0]+1]);
    vertex3hi_2.i = _mm_set_epi32(pCube[index3hi.u32[3]+2],pCube[index3hi.u32[2]+2],pCube[index3hi.u32[1]+2],pCube[index3hi.u32[0]+2]);

    vertex3hi_0.f = _mm_cvtepi32_ps(vertex3hi_0.i);
    vertex3hi_1.f = _mm_cvtepi32_ps(vertex3hi_1.i);
    vertex3hi_2.f = _mm_cvtepi32_ps(vertex3hi_2.i);

    vertex3_0.i = _mm_set_epi32(pCube[index3.u32[3]],pCube[index3.u32[2]],pCube[index3.u32[1]],pCube[index3.u32[0]]);
    vertex3_1.i = _mm_set_epi32(pCube[index3.u32[3]+1],pCube[index3.u32[2]+1],pCube[index3.u32[1]+1],pCube[index3.u32[0]+1]);
    vertex3_2.i = _mm_set_epi32(pCube[index3.u32[3]+2],pCube[index3.u32[2]+2],pCube[index3.u32[1]+2],pCube[index3.u32[0]+2]);

    vertex3_0.f = _mm_cvtepi32_ps(vertex3_0.i);
    vertex3_1.f = _mm_cvtepi32_ps(vertex3_1.i);
    vertex3_2.f = _mm_cvtepi32_ps(vertex3_2.i);
    //vertex4 = &pCube[index4];
    vertex4hi_0.i = _mm_set_epi32(pCube[index4hi.u32[3]],pCube[index4hi.u32[2]],pCube[index4hi.u32[1]],pCube[index4hi.u32[0]]);
    vertex4hi_1.i = _mm_set_epi32(pCube[index4hi.u32[3]+1],pCube[index4hi.u32[2]+1],pCube[index4hi.u32[1]+1],pCube[index4hi.u32[0]+1]);
    vertex4hi_2.i = _mm_set_epi32(pCube[index4hi.u32[3]+2],pCube[index4hi.u32[2]+2],pCube[index4hi.u32[1]+2],pCube[index4hi.u32[0]+2]);

    vertex4hi_0.f = _mm_cvtepi32_ps(vertex4hi_0.i);
    vertex4hi_1.f = _mm_cvtepi32_ps(vertex4hi_1.i);
    vertex4hi_2.f = _mm_cvtepi32_ps(vertex4hi_2.i);

    vertex4_0.i = _mm_set_epi32(pCube[index4.u32[3]],pCube[index4.u32[2]],pCube[index4.u32[1]],pCube[index4.u32[0]]);
    vertex4_1.i = _mm_set_epi32(pCube[index4.u32[3]+1],pCube[index4.u32[2]+1],pCube[index4.u32[1]+1],pCube[index4.u32[0]+1]);
    vertex4_2.i = _mm_set_epi32(pCube[index4.u32[3]+2],pCube[index4.u32[2]+2],pCube[index4.u32[1]+2],pCube[index4.u32[0]+2]);

    vertex4_0.f = _mm_cvtepi32_ps(vertex4_0.i);
    vertex4_1.f = _mm_cvtepi32_ps(vertex4_1.i);
    vertex4_2.f = _mm_cvtepi32_ps(vertex4_2.i);
    //vertex5 = &pCube[index5];
    vertex5hi_0.i = _mm_set_epi32(pCube[index5hi.u32[3]],pCube[index5hi.u32[2]],pCube[index5hi.u32[1]],pCube[index5hi.u32[0]]);
    vertex5hi_1.i = _mm_set_epi32(pCube[index5hi.u32[3]+1],pCube[index5hi.u32[2]+1],pCube[index5hi.u32[1]+1],pCube[index5hi.u32[0]+1]);
    vertex5hi_2.i = _mm_set_epi32(pCube[index5hi.u32[3]+2],pCube[index5hi.u32[2]+2],pCube[index5hi.u32[1]+2],pCube[index5hi.u32[0]+2]);

    vertex5hi_0.f = _mm_cvtepi32_ps(vertex5hi_0.i);
    vertex5hi_1.f = _mm_cvtepi32_ps(vertex5hi_1.i);
    vertex5hi_2.f = _mm_cvtepi32_ps(vertex5hi_2.i);

    vertex5_0.i = _mm_set_epi32(pCube[index5.u32[3]],pCube[index5.u32[2]],pCube[index5.u32[1]],pCube[index5.u32[0]]);
    vertex5_1.i = _mm_set_epi32(pCube[index5.u32[3]+1],pCube[index5.u32[2]+1],pCube[index5.u32[1]+1],pCube[index5.u32[0]+1]);
    vertex5_2.i = _mm_set_epi32(pCube[index5.u32[3]+2],pCube[index5.u32[2]+2],pCube[index5.u32[1]+2],pCube[index5.u32[0]+2]);

    vertex5_0.f = _mm_cvtepi32_ps(vertex5_0.i);
    vertex5_1.f = _mm_cvtepi32_ps(vertex5_1.i);
    vertex5_2.f = _mm_cvtepi32_ps(vertex5_2.i);
    //vertex6 = &pCube[index6];
    vertex6hi_0.i = _mm_set_epi32(pCube[index6hi.u32[3]],pCube[index6hi.u32[2]],pCube[index6hi.u32[1]],pCube[index6hi.u32[0]]);
    vertex6hi_1.i = _mm_set_epi32(pCube[index6hi.u32[3]+1],pCube[index6hi.u32[2]+1],pCube[index6hi.u32[1]+1],pCube[index6hi.u32[0]+1]);
    vertex6hi_2.i = _mm_set_epi32(pCube[index6hi.u32[3]+2],pCube[index6hi.u32[2]+2],pCube[index6hi.u32[1]+2],pCube[index6hi.u32[0]+2]);

    vertex6hi_0.f = _mm_cvtepi32_ps(vertex6hi_0.i);
    vertex6hi_1.f = _mm_cvtepi32_ps(vertex6hi_1.i);
    vertex6hi_2.f = _mm_cvtepi32_ps(vertex6hi_2.i);

    vertex6_0.i = _mm_set_epi32(pCube[index6.u32[3]],pCube[index6.u32[2]],pCube[index6.u32[1]],pCube[index6.u32[0]]);
    vertex6_1.i = _mm_set_epi32(pCube[index6.u32[3]+1],pCube[index6.u32[2]+1],pCube[index6.u32[1]+1],pCube[index6.u32[0]+1]);
    vertex6_2.i = _mm_set_epi32(pCube[index6.u32[3]+2],pCube[index6.u32[2]+2],pCube[index6.u32[1]+2],pCube[index6.u32[0]+2]);

    vertex6_0.f = _mm_cvtepi32_ps(vertex6_0.i);
    vertex6_1.f = _mm_cvtepi32_ps(vertex6_1.i);
    vertex6_2.f = _mm_cvtepi32_ps(vertex6_2.i);
    //vertex7 = &pCube[index7];
    vertex7hi_0.i = _mm_set_epi32(pCube[index7hi.u32[3]],pCube[index7hi.u32[2]],pCube[index7hi.u32[1]],pCube[index7hi.u32[0]]);
    vertex7hi_1.i = _mm_set_epi32(pCube[index7hi.u32[3]+1],pCube[index7hi.u32[2]+1],pCube[index7hi.u32[1]+1],pCube[index7hi.u32[0]+1]);
    vertex7hi_2.i = _mm_set_epi32(pCube[index7hi.u32[3]+2],pCube[index7hi.u32[2]+2],pCube[index7hi.u32[1]+2],pCube[index7hi.u32[0]+2]);

    vertex7hi_0.f = _mm_cvtepi32_ps(vertex7hi_0.i);
    vertex7hi_1.f = _mm_cvtepi32_ps(vertex7hi_1.i);
    vertex7hi_2.f = _mm_cvtepi32_ps(vertex7hi_2.i);

    vertex7_0.i = _mm_set_epi32(pCube[index7.u32[3]],pCube[index7.u32[2]],pCube[index7.u32[1]],pCube[index7.u32[0]]);
    vertex7_1.i = _mm_set_epi32(pCube[index7.u32[3]+1],pCube[index7.u32[2]+1],pCube[index7.u32[1]+1],pCube[index7.u32[0]+1]);
    vertex7_2.i = _mm_set_epi32(pCube[index7.u32[3]+2],pCube[index7.u32[2]+2],pCube[index7.u32[1]+2],pCube[index7.u32[0]+2]);

    vertex7_0.f = _mm_cvtepi32_ps(vertex7_0.i);
    vertex7_1.f = _mm_cvtepi32_ps(vertex7_1.i);
    vertex7_2.f = _mm_cvtepi32_ps(vertex7_2.i);

    //vertex8 = &pCube[index8];
    vertex8hi_0.i = _mm_set_epi32(pCube[index8hi.u32[3]],pCube[index8hi.u32[2]],pCube[index8hi.u32[1]],pCube[index8hi.u32[0]]);
    vertex8hi_1.i = _mm_set_epi32(pCube[index8hi.u32[3]+1],pCube[index8hi.u32[2]+1],pCube[index8hi.u32[1]+1],pCube[index8hi.u32[0]+1]);
    vertex8hi_2.i = _mm_set_epi32(pCube[index8hi.u32[3]+2],pCube[index8hi.u32[2]+2],pCube[index8hi.u32[1]+2],pCube[index8hi.u32[0]+2]);

    vertex8hi_0.f = _mm_cvtepi32_ps(vertex8hi_0.i);
    vertex8hi_1.f = _mm_cvtepi32_ps(vertex8hi_1.i);
    vertex8hi_2.f = _mm_cvtepi32_ps(vertex8hi_2.i);

    vertex8_0.i = _mm_set_epi32(pCube[index8.u32[3]],pCube[index8.u32[2]],pCube[index8.u32[1]],pCube[index8.u32[0]]);
    vertex8_1.i = _mm_set_epi32(pCube[index8.u32[3]+1],pCube[index8.u32[2]+1],pCube[index8.u32[1]+1],pCube[index8.u32[0]+1]);
    vertex8_2.i = _mm_set_epi32(pCube[index8.u32[3]+2],pCube[index8.u32[2]+2],pCube[index8.u32[1]+2],pCube[index8.u32[0]+2]);

    vertex8_0.f = _mm_cvtepi32_ps(vertex8_0.i);
    vertex8_1.f = _mm_cvtepi32_ps(vertex8_1.i);
    vertex8_2.f = _mm_cvtepi32_ps(vertex8_2.i);

    // interpolate for x,y and z
    //x1 = vertex1[0] + fraction_x * (vertex2[0] - vertex1[0]);
    x1hi.f = _mm_sub_ps(vertex2hi_0.f, vertex1hi_0.f);
    x1hi.f = _mm_mul_ps(x1hi.f, fraction_xhi.f);
    x1hi.f = _mm_add_ps(x1hi.f, vertex1hi_0.f);

    x1.f = _mm_sub_ps(vertex2_0.f, vertex1_0.f);
    x1.f = _mm_mul_ps(x1.f, fraction_x.f);
    x1.f = _mm_add_ps(x1.f, vertex1_0.f);
    //y1 = vertex1[1] + fraction_x * (vertex2[1] - vertex1[1]);
    y1hi.f = _mm_sub_ps(vertex2hi_1.f, vertex1hi_1.f);
    y1hi.f = _mm_mul_ps(y1hi.f, fraction_xhi.f);
    y1hi.f = _mm_add_ps(y1hi.f, vertex1hi_1.f);

    y1.f = _mm_sub_ps(vertex2_1.f, vertex1_1.f);
    y1.f = _mm_mul_ps(y1.f, fraction_x.f);
    y1.f = _mm_add_ps(y1.f, vertex1_1.f);
    //z1 = vertex1[2] + fraction_x * (vertex2[2] - vertex1[2]);
    z1hi.f = _mm_sub_ps(vertex2hi_2.f, vertex1hi_2.f);
    z1hi.f = _mm_mul_ps(z1hi.f, fraction_xhi.f);
    z1hi.f = _mm_add_ps(z1hi.f, vertex1hi_2.f);

    z1.f = _mm_sub_ps(vertex2_2.f, vertex1_2.f);
    z1.f = _mm_mul_ps(z1.f, fraction_x.f);
    z1.f = _mm_add_ps(z1.f, vertex1_2.f);
    //x2 = vertex3[0] + fraction_x * (vertex4[0] - vertex3[0]);
    x2hi.f = _mm_sub_ps(vertex4hi_0.f, vertex3hi_0.f);
    x2hi.f = _mm_mul_ps(x2hi.f, fraction_xhi.f);
    x2hi.f = _mm_add_ps(x2hi.f, vertex3hi_0.f);

    x2.f = _mm_sub_ps(vertex4_0.f, vertex3_0.f);
    x2.f = _mm_mul_ps(x2.f, fraction_x.f);
    x2.f = _mm_add_ps(x2.f, vertex3_0.f);
    //y2 = vertex3[1] + fraction_x * (vertex4[1] - vertex3[1]);
    y2hi.f = _mm_sub_ps(vertex4hi_1.f, vertex3hi_1.f);
    y2hi.f = _mm_mul_ps(y2hi.f, fraction_xhi.f);
    y2hi.f = _mm_add_ps(y2hi.f, vertex3hi_1.f);

    y2.f = _mm_sub_ps(vertex4_1.f, vertex3_1.f);
    y2.f = _mm_mul_ps(y2.f, fraction_x.f);
    y2.f = _mm_add_ps(y2.f, vertex3_1.f);
    //z2 = vertex3[2] + fraction_x * (vertex4[2] - vertex3[2]);
    z2hi.f = _mm_sub_ps(vertex4hi_2.f, vertex3hi_2.f);
    z2hi.f = _mm_mul_ps(z2hi.f, fraction_xhi.f);
    z2hi.f = _mm_add_ps(z2hi.f, vertex3hi_2.f);

    z2.f = _mm_sub_ps(vertex4_2.f, vertex3_2.f);
    z2.f = _mm_mul_ps(z2.f, fraction_x.f);
    z2.f = _mm_add_ps(z2.f, vertex3_2.f);
    //x3 = vertex5[0] + fraction_x * (vertex6[0] - vertex5[0]);
    x3hi.f = _mm_sub_ps(vertex6hi_0.f, vertex5hi_0.f);
    x3hi.f = _mm_mul_ps(x3hi.f, fraction_xhi.f);
    x3hi.f = _mm_add_ps(x3hi.f, vertex5hi_0.f);

    x3.f = _mm_sub_ps(vertex6_0.f, vertex5_0.f);
    x3.f = _mm_mul_ps(x3.f, fraction_x.f);
    x3.f = _mm_add_ps(x3.f, vertex5_0.f);
    //y3 = vertex5[1] + fraction_x * (vertex6[1] - vertex5[1]);
    y3hi.f = _mm_sub_ps(vertex6hi_1.f, vertex5hi_1.f);
    y3hi.f = _mm_mul_ps(y3hi.f, fraction_xhi.f);
    y3hi.f = _mm_add_ps(y3hi.f, vertex5hi_1.f);

    y3.f = _mm_sub_ps(vertex6_1.f, vertex5_1.f);
    y3.f = _mm_mul_ps(y3.f, fraction_x.f);
    y3.f = _mm_add_ps(y3.f, vertex5_1.f);
    //z3 = vertex5[2] + fraction_x * (vertex6[2] - vertex5[2]);
    z3hi.f = _mm_sub_ps(vertex6hi_2.f, vertex5hi_2.f);
    z3hi.f = _mm_mul_ps(z3hi.f, fraction_xhi.f);
    z3hi.f = _mm_add_ps(z3hi.f, vertex5hi_2.f);

    z3.f = _mm_sub_ps(vertex6_2.f, vertex5_2.f);
    z3.f = _mm_mul_ps(z3.f, fraction_x.f);
    z3.f = _mm_add_ps(z3.f, vertex5_2.f);
    //x4 = vertex7[0] + fraction_x * (vertex8[0] - vertex7[0]);
    x4hi.f = _mm_sub_ps(vertex8hi_0.f, vertex7hi_0.f);
    x4hi.f = _mm_mul_ps(x4hi.f, fraction_xhi.f);
    x4hi.f = _mm_add_ps(x4hi.f, vertex7hi_0.f);

    x4.f = _mm_sub_ps(vertex8_0.f, vertex7_0.f);
    x4.f = _mm_mul_ps(x4.f, fraction_x.f);
    x4.f = _mm_add_ps(x4.f, vertex7_0.f);
    //y4 = vertex7[1] + fraction_x * (vertex8[1] - vertex7[1]);
    y4hi.f = _mm_sub_ps(vertex8hi_1.f, vertex7hi_1.f);
    y4hi.f = _mm_mul_ps(y4hi.f, fraction_xhi.f);
    y4hi.f = _mm_add_ps(y4hi.f, vertex7hi_1.f);

    y4.f = _mm_sub_ps(vertex8_1.f, vertex7_1.f);
    y4.f = _mm_mul_ps(y4.f, fraction_x.f);
    y4.f = _mm_add_ps(y4.f, vertex7_1.f);
    //z4 = vertex7[2] + fraction_x * (vertex8[2] - vertex7[2]);
    z4hi.f = _mm_sub_ps(vertex8hi_2.f, vertex7hi_2.f);
    z4hi.f = _mm_mul_ps(z4hi.f, fraction_xhi.f);
    z4hi.f = _mm_add_ps(z4hi.f, vertex7hi_2.f);

    z4.f = _mm_sub_ps(vertex8_2.f, vertex7_2.f);
    z4.f = _mm_mul_ps(z4.f, fraction_x.f);
    z4.f = _mm_add_ps(z4.f, vertex7_2.f);
    //x5 = x1 + fraction_y * (x2 - x1);
    x5hi.f = _mm_sub_ps(x2hi.f, x1hi.f);
    x5hi.f = _mm_mul_ps(x5hi.f, fraction_yhi.f);
    x5hi.f = _mm_add_ps(x5hi.f, x1hi.f);

    x5.f = _mm_sub_ps(x2.f, x1.f);
    x5.f = _mm_mul_ps(x5.f, fraction_y.f);
    x5.f = _mm_add_ps(x5.f, x1.f);
    //y5 = y1 + fraction_y * (y2 - y1);
    y5hi.f = _mm_sub_ps(y2hi.f, y1hi.f);
    y5hi.f = _mm_mul_ps(y5hi.f, fraction_yhi.f);
    y5hi.f = _mm_add_ps(y5hi.f, y1hi.f);

    y5.f = _mm_sub_ps(y2.f, y1.f);
    y5.f = _mm_mul_ps(y5.f, fraction_y.f);
    y5.f = _mm_add_ps(y5.f, y1.f);
    //z5 = z1 + fraction_y * (z2 - z1);
    z5hi.f = _mm_sub_ps(z2hi.f, z1hi.f);
    z5hi.f = _mm_mul_ps(z5hi.f, fraction_yhi.f);
    z5hi.f = _mm_add_ps(z5hi.f, z1hi.f);

    z5.f = _mm_sub_ps(z2.f, z1.f);
    z5.f = _mm_mul_ps(z5.f, fraction_y.f);
    z5.f = _mm_add_ps(z5.f, z1.f);
    //x6 = x3 + fraction_y * (x4 - x3);
    x6hi.f = _mm_sub_ps(x4hi.f, x3hi.f);
    x6hi.f = _mm_mul_ps(x6hi.f, fraction_yhi.f);
    x6hi.f = _mm_add_ps(x6hi.f, x3hi.f);

    x6.f = _mm_sub_ps(x4.f, x3.f);
    x6.f = _mm_mul_ps(x6.f, fraction_y.f);
    x6.f = _mm_add_ps(x6.f, x3.f);
    //y6 = y3 + fraction_y * (y4 - y3);
    y6hi.f = _mm_sub_ps(y4hi.f, y3hi.f);
    y6hi.f = _mm_mul_ps(y6hi.f, fraction_yhi.f);
    y6hi.f = _mm_add_ps(y6hi.f, y3hi.f);

    y6.f = _mm_sub_ps(y4.f, y3.f);
    y6.f = _mm_mul_ps(y6.f, fraction_y.f);
    y6.f = _mm_add_ps(y6.f, y3.f);
    //z6 = z3 + fraction_y * (z4 - z3);
    z6hi.f = _mm_sub_ps(z4hi.f, z3hi.f);
    z6hi.f = _mm_mul_ps(z6hi.f, fraction_yhi.f);
    z6hi.f = _mm_add_ps(z6hi.f, z3hi.f);

    z6.f = _mm_sub_ps(z4.f, z3.f);
    z6.f = _mm_mul_ps(z6.f, fraction_y.f);
    z6.f = _mm_add_ps(z6.f, z3.f);
    //x7 = x5 + fraction_z * (x6 - x5);
    x7hi.f = _mm_sub_ps(x6hi.f, x5hi.f);
    x7hi.f = _mm_mul_ps(x7hi.f, fraction_zhi.f);
    x7hi.f = _mm_add_ps(x7hi.f, x5hi.f);

    x7.f = _mm_sub_ps(x6.f, x5.f);
    x7.f = _mm_mul_ps(x7.f, fraction_z.f);
    x7.f = _mm_add_ps(x7.f, x5.f);
    //y7 = y5 + fraction_z * (y6 - y5);
    y7hi.f = _mm_sub_ps(y6hi.f, y5hi.f);
    y7hi.f = _mm_mul_ps(y7hi.f, fraction_zhi.f);
    y7hi.f = _mm_add_ps(y7hi.f, y5hi.f);

    y7.f = _mm_sub_ps(y6.f, y5.f);
    y7.f = _mm_mul_ps(y7.f, fraction_z.f);
    y7.f = _mm_add_ps(y7.f, y5.f);
    //z7 = z5 + fraction_z * (z6 - z5);
    z7hi.f = _mm_sub_ps(z6hi.f, z5hi.f);
    z7hi.f = _mm_mul_ps(z7hi.f, fraction_zhi.f);
    z7hi.f = _mm_add_ps(z7hi.f, z5hi.f);

    z7.f = _mm_sub_ps(z6.f, z5.f);
    z7.f = _mm_mul_ps(z7.f, fraction_z.f);
    z7.f = _mm_add_ps(z7.f, z5.f);

    //x  = (x7>pLUTSpec->cubeMax) ? pLUTSpec->cubeMax : (Fw16u)x7;
    xhi.i = _mm_cvttps_epi32(x7hi.f);    
    x.i = _mm_cvttps_epi32(x7.f);
    x.i = _mm_packs_epi32(x.i, xhi.i);
    temp.i = _mm_cmpgt_epi16(x.i, pLUTSpec->cubeMax_16u.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        x.i  = _mm_andnot_si128(temp.i, x.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_16u.i);
        x.i = _mm_or_si128(x.i, temp.i);
        }
    //y  = (y7>pLUTSpec->cubeMax) ? pLUTSpec->cubeMax : (Fw16u)y7;
    yhi.i = _mm_cvttps_epi32(y7hi.f);
    y.i = _mm_cvttps_epi32(y7.f);
    y.i = _mm_packs_epi32(y.i, yhi.i);
    temp.i = _mm_cmpgt_epi16(y.i, pLUTSpec->cubeMax_16u.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        y.i  = _mm_andnot_si128(temp.i, y.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_16u.i);
        y.i = _mm_or_si128(y.i, temp.i);
        }
    //z  = (z7>pLUTSpec->cubeMax) ? pLUTSpec->cubeMax : (Fw16u)z7;
    zhi.i = _mm_cvttps_epi32(z7hi.f);
    z.i = _mm_cvttps_epi32(z7.f);
    z.i = _mm_packs_epi32(z.i, zhi.i);
    temp.i = _mm_cmpgt_epi16(z.i, pLUTSpec->cubeMax_16u.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        z.i  = _mm_andnot_si128(temp.i, z.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_16u.i);
        z.i = _mm_or_si128(z.i, temp.i);
        }
   }

ISV LookUp3D_onePixel(Fw16u &x,Fw16u &y,Fw16u &z, const Fw16u *pCube, FwiLUTSpec *pLUTSpec)
    {

    float fx,fy,fz;
    float z1,z2,z3,z4,z5,z6,z7;
    float y1,y2,y3,y4,y5,y6,y7;
    float x1,x2,x3,x4,x5,x6,x7;
    int   ix0,iy0,iz0,ix1,iy1,iz1;
    float fraction_x,fraction_y,fraction_z;
    int   index1,index2,index3,index4,index5,index6,index7,index8;
    const Fw16u *vertex1,*vertex2,*vertex3,*vertex4,*vertex5,*vertex6,*vertex7,*vertex8;

    // check if overflow
    if(x>pLUTSpec->cubeMax) x = pLUTSpec->cubeMax;
    if(y>pLUTSpec->cubeMax) y = pLUTSpec->cubeMax;
    if(z>pLUTSpec->cubeMax) z = pLUTSpec->cubeMax;

    // map the input pixel value onto the cube
    float temp;
    temp = (float)pLUTSpec->maxIndex/pLUTSpec->cubeMax;
    fx = x * temp;
    fy = y * temp;
    fz = z * temp;

    // get the indices as integers
    ix0 = (int)fx;
    iy0 = (int)fy;
    iz0 = (int)fz;

    ix1 = (ix0 == pLUTSpec->maxIndex) ? ix0 : ix0+1;
    iy1 = (iy0 == pLUTSpec->maxIndex) ? iy0 : iy0+1;
    iz1 = (iz0 == pLUTSpec->maxIndex) ? iz0 : iz0+1;

    // get the fractional parts
    fraction_x   = fx - ix0;
    fraction_y   = fy - iy0;
    fraction_z   = fz - iz0;
    
    // get the 8 vertices surrounding the given input point
    int temp1 = ix0*pLUTSpec->cubeDimensions;
    int temp2 = ix1*pLUTSpec->cubeDimensions;
    int temp3 = pLUTSpec->cubeDimensions * 3;
    int temp4 = (iz1 - iz0)*3;

    index1  = ((temp1 + iy0)*pLUTSpec->cubeDimensions + iz0)*3;
    index2  = index1 + (temp2 - temp1) * temp3;
    index3  = index1 + (iy1 - iy0) * temp3;
    index4  = index3 + (temp2 - temp1) * temp3;
    index5  = index1 + temp4;
    index6  = index2 + temp4;
    index7  = index3 + temp4;
    index8  = index4 + temp4;

    vertex1 = &pCube[index1];
    vertex2 = &pCube[index2];
    vertex3 = &pCube[index3];
    vertex4 = &pCube[index4];
    vertex5 = &pCube[index5];
    vertex6 = &pCube[index6];
    vertex7 = &pCube[index7];
    vertex8 = &pCube[index8];

    // interpolate for x,y and z
    x1 = vertex1[0] + fraction_x * (vertex2[0] - vertex1[0]);
    y1 = vertex1[1] + fraction_x * (vertex2[1] - vertex1[1]);
    z1 = vertex1[2] + fraction_x * (vertex2[2] - vertex1[2]);

    x2 = vertex3[0] + fraction_x * (vertex4[0] - vertex3[0]);
    y2 = vertex3[1] + fraction_x * (vertex4[1] - vertex3[1]);
    z2 = vertex3[2] + fraction_x * (vertex4[2] - vertex3[2]);

    x3 = vertex5[0] + fraction_x * (vertex6[0] - vertex5[0]);
    y3 = vertex5[1] + fraction_x * (vertex6[1] - vertex5[1]);
    z3 = vertex5[2] + fraction_x * (vertex6[2] - vertex5[2]);

    x4 = vertex7[0] + fraction_x * (vertex8[0] - vertex7[0]);
    y4 = vertex7[1] + fraction_x * (vertex8[1] - vertex7[1]);
    z4 = vertex7[2] + fraction_x * (vertex8[2] - vertex7[2]);

    x5 = x1 + fraction_y * (x2 - x1);
    y5 = y1 + fraction_y * (y2 - y1);
    z5 = z1 + fraction_y * (z2 - z1);

    x6 = x3 + fraction_y * (x4 - x3);
    y6 = y3 + fraction_y * (y4 - y3);
    z6 = z3 + fraction_y * (z4 - z3);

    x7 = x5 + fraction_z * (x6 - x5);
    y7 = y5 + fraction_z * (y6 - y5);
    z7 = z5 + fraction_z * (z6 - z5);

    x  = (x7>pLUTSpec->cubeMax) ? pLUTSpec->cubeMax : (Fw16u)x7;
    y  = (y7>pLUTSpec->cubeMax) ? pLUTSpec->cubeMax : (Fw16u)y7;
    z  = (z7>pLUTSpec->cubeMax) ? pLUTSpec->cubeMax : (Fw16u)z7;
   }

IS FwStatus LookUp3D_16u_C3R(const Fw16u *pSrc, int &srcStep, Fw16u *pDst, int &dstStep, FwiSize &roiSize, const Fw16u *pCube, FwiLUTSpec *pLUTSpec)
    {
    if(FW_REF::PtrNotOK(pSrc,pDst, pCube, pLUTSpec))
        return fwStsNullPtrErr;

   	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2: // TODO::implement threading 
        {
        XMM128 x,y,z;
        int height = roiSize.height;
        int width  = roiSize.width;
        int step  = srcStep/2;dstStep;
        int pixnsse = width>>3;      // width/8
        int pixnref = width - (pixnsse<<3); // width - pixnsse*8

        const Fw16u *src = pSrc;
        Fw16u *dst = pDst;

        for(int i=0;i<height;i++)
            {
            const Fw16u *tempSrc = src;
            Fw16u *tempDst    = dst;
            bool srcAligned = FW_REF::IsAligned(tempSrc,16);
            bool dstAligned = FW_REF::IsAligned(tempDst,16);

            for(int j=0;j<pixnsse;j++)
                {
                if(srcAligned)
                    {
                    x.i = _mm_load_si128((__m128i*)(&(tempSrc[0])));
                    y.i = _mm_load_si128((__m128i*)(&(tempSrc[8])));
                    z.i = _mm_load_si128((__m128i*)(&(tempSrc[16])));
                    }
                else
                    {
                    x.i = _mm_loadu_si128((__m128i*)(&(tempSrc[0])));
                    y.i = _mm_loadu_si128((__m128i*)(&(tempSrc[8])));
                    z.i = _mm_loadu_si128((__m128i*)(&(tempSrc[16])));
                    }
                
                 // convert c3 to p3 16 bit
                 ssp_convert_3c_3p_epi16(&x.i, &y.i, &z.i);

                 LookUp3D_eightPixel(x,y,z,pCube,pLUTSpec);

                 // convert p3 to c3 16 bit
                 ssp_convert_3p_3c_epi16(&x.i, &y.i, &z.i);
                 
                 if(dstAligned)
                     {
                     _mm_store_si128((__m128i*)(&(tempDst[0])), x.i);
                     _mm_store_si128((__m128i*)(&(tempDst[8])), y.i);
                     _mm_store_si128((__m128i*)(&(tempDst[16])), z.i);
                     }
                 else
                     {
                     _mm_storeu_si128((__m128i*)(&(tempDst[0])), x.i);
                     _mm_storeu_si128((__m128i*)(&(tempDst[8])), y.i);
                     _mm_storeu_si128((__m128i*)(&(tempDst[16])), z.i);
                     }

                 tempSrc += 24;
                 tempDst += 24;
                }

            if(pixnref)
                {
                int temp = (8 - pixnref) * 3;
                tempSrc -= temp;
                tempDst -= temp;
         
                bool srcAligned = FW_REF::IsAligned(tempSrc,16);
                bool dstAligned = FW_REF::IsAligned(tempDst,16);

                if(srcAligned)
                    {
                    x.i = _mm_load_si128((__m128i*)(&(tempSrc[0])));
                    y.i = _mm_load_si128((__m128i*)(&(tempSrc[8])));
                    z.i = _mm_load_si128((__m128i*)(&(tempSrc[16])));
                    }
                else
                    {
                    x.i = _mm_loadu_si128((__m128i*)(&(tempSrc[0])));
                    y.i = _mm_loadu_si128((__m128i*)(&(tempSrc[8])));
                    z.i = _mm_loadu_si128((__m128i*)(&(tempSrc[16])));
                    }
                
                 // convert c3 to p3 16 bit
                 ssp_convert_3c_3p_epi16(&x.i, &y.i, &z.i);

                 LookUp3D_eightPixel(x,y,z,pCube,pLUTSpec);

                 // convert p3 to c3 16 bit
                 ssp_convert_3p_3c_epi16(&x.i, &y.i, &z.i);

                 if(dstAligned)
                     {
                     _mm_store_si128((__m128i*)(&(tempDst[0])), x.i);
                     _mm_store_si128((__m128i*)(&(tempDst[8])), y.i);
                     _mm_store_si128((__m128i*)(&(tempDst[16])), z.i);
                     }
                 else
                     {
                     _mm_storeu_si128((__m128i*)(&(tempDst[0])), x.i);
                     _mm_storeu_si128((__m128i*)(&(tempDst[8])), y.i);
                     _mm_storeu_si128((__m128i*)(&(tempDst[16])), z.i);
                     }

                }
            src+= step;
            dst+= step;
            }
        break;
        }

    case DT_REFR:
    default:
        {
        Fw16u x,y,z;

        int height = roiSize.height;
        int width  = roiSize.width;
        int step  = srcStep/2;dstStep;
        
        const Fw16u *src = pSrc;
        Fw16u *dst = pDst;
        

        for(int i=0;i<height;i++)
            {
            const Fw16u *tempSrc = src;
            Fw16u *tempDst = dst;

            for(int j=0;j<width;j++)
                {
                 x = tempSrc[0];
                 y = tempSrc[1];
                 z = tempSrc[2];

                 LookUp3D_onePixel(x,y,z, pCube, pLUTSpec);

                 tempDst[0] = x;
                 tempDst[1] = y;
                 tempDst[2] = z;

                 tempSrc+= 3;
                 tempDst+= 3;
                }
            src+= step;
            dst+= step;
            }
        }
        }
    return fwStsNoErr;
    }

IS FwStatus LookUp3D_16u_C3IR(Fw16u *pSrcDst, int &srcDstStep, FwiSize &roiSize, const Fw16u *pCube, FwiLUTSpec *pLUTSpec)
    {
    if(FW_REF::PtrNotOK(pSrcDst, pCube, pLUTSpec))
        return fwStsNullPtrErr;

   	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2: // TODO::implement threading ...
        {
        XMM128 x,y,z;
        int height = roiSize.height;
        int width  = roiSize.width;
        int step  = srcDstStep/2;
        int pixnsse = width>>3;      // width/8
        int pixnref = width - (pixnsse<<3); // width - pixnsse*8

        Fw16u *srcDst = pSrcDst;        

        for(int i=0;i<height;i++)
            {
            Fw16u *tempSrcDst = srcDst;
            bool srcDstAligned = FW_REF::IsAligned(tempSrcDst,16);

            for(int j=0;j<pixnsse;j++)
                {
                if(srcDstAligned)
                    {
                    x.i = _mm_load_si128((__m128i*)(&(tempSrcDst[0])));
                    y.i = _mm_load_si128((__m128i*)(&(tempSrcDst[8])));
                    z.i = _mm_load_si128((__m128i*)(&(tempSrcDst[16])));
                    }
                else
                    {
                    x.i = _mm_loadu_si128((__m128i*)(&(tempSrcDst[0])));
                    y.i = _mm_loadu_si128((__m128i*)(&(tempSrcDst[8])));
                    z.i = _mm_loadu_si128((__m128i*)(&(tempSrcDst[16])));
                    }

                 // convert c3 to p3 16 bit
                 ssp_convert_3c_3p_epi16(&x.i, &y.i, &z.i);

                 LookUp3D_eightPixel(x,y,z,pCube,pLUTSpec);

                 // convert p3 to c3 16 bit
                 ssp_convert_3p_3c_epi16(&x.i, &y.i, &z.i);

                if(srcDstAligned)
                    {
                    _mm_store_si128((__m128i*)(&(tempSrcDst[0])), x.i);
                    _mm_store_si128((__m128i*)(&(tempSrcDst[8])), y.i);
                    _mm_store_si128((__m128i*)(&(tempSrcDst[16])), z.i);
                    }
                else
                    {
                    _mm_storeu_si128((__m128i*)(&(tempSrcDst[0])), x.i);
                    _mm_storeu_si128((__m128i*)(&(tempSrcDst[8])), y.i);
                    _mm_storeu_si128((__m128i*)(&(tempSrcDst[16])), z.i);
                    }

                 tempSrcDst += 24;
                }

            for(int k=0;k<pixnref;k++)
                {
                 Fw16u x,y,z;
                 x = tempSrcDst[0];
                 y = tempSrcDst[1];
                 z = tempSrcDst[2];

                 LookUp3D_onePixel(x,y,z, pCube, pLUTSpec);

                 tempSrcDst[0] = x;
                 tempSrcDst[1] = y;
                 tempSrcDst[2] = z;

                 tempSrcDst+= 3;
                }

            srcDst+= step;
            }
        break;
        }

    case DT_REFR:
    default:

        Fw16u x,y,z;

        int height = roiSize.height;
        int width  = roiSize.width;
        int step  = srcDstStep/2;
        
        Fw16u *srcDst = pSrcDst;

        for(int i=0;i<height;i++)
            {
            Fw16u *tempSrcDst = srcDst;

            for(int j=0;j<width;j++)
                {
                 x = tempSrcDst[0];
                 y = tempSrcDst[1];
                 z = tempSrcDst[2];

                 LookUp3D_onePixel(x,y,z, pCube, pLUTSpec);

                 tempSrcDst[0] = x;
                 tempSrcDst[1] = y;
                 tempSrcDst[2] = z;

                 tempSrcDst+= 3;
                }
            srcDst+= step;
            }
     }

    return fwStsNoErr;
    }

IS FwStatus LookUp3DSpecFree(FwiLUTSpec* pLUTSpec)
    {
    if(FW_REF::PtrNotOK(pLUTSpec))
        return fwStsNullPtrErr;
    fwFree(pLUTSpec);
    return fwStsNoErr;
    }

} // end namespace OPT_LEVEL
#endif
