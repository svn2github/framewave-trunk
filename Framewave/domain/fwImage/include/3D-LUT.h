/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __3D_LUT_H__
#define __3D_LUT_H__

#include "fe.h"
#include "fwImage.h"

struct LUTSpec
    {
    XMM128 cubeDimensions_ix;
    XMM128 cubeDimensions_iy;
    XMM128 cubeDimensions_iz;
    XMM128 cubeDimensions_fx;
    XMM128 cubeDimensions_fy;
    XMM128 cubeDimensions_fz;

    XMM128 maxIndex_ix;
    XMM128 maxIndex_iy;
    XMM128 maxIndex_iz;
    XMM128 maxIndex_fx;
    XMM128 maxIndex_fy;
    XMM128 maxIndex_fz;

    XMM128 cubeMax_ix;
    XMM128 cubeMax_iy;
    XMM128 cubeMax_iz;
    XMM128 cubeMax_fx;
    XMM128 cubeMax_fy;
    XMM128 cubeMax_fz;

    Fw16u  cubeDimensions[3];
    Fw16u  maxIndex[3];
    Fw16u  cubeMax[3];
    };

namespace OPT_LEVEL
{

IS FwStatus LookUp3DSpecInitAlloc(const Fw16u cubeDimensions[3], const Fw16u cubeMax[3], FwiLUTSpec** ppLUTSpec)
    {
    if(FW_REF::PtrNotOK(cubeDimensions, cubeMax))
        return fwStsNullPtrErr;

    *ppLUTSpec = (FwiLUTSpec*)fwMalloc(sizeof(struct LUTSpec));

    FwiLUTSpec* pLUTSpec = *ppLUTSpec;

  	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
        pLUTSpec->cubeDimensions_ix.i = _mm_set1_epi32(cubeDimensions[0]);
        pLUTSpec->cubeDimensions_iy.i = _mm_set1_epi32(cubeDimensions[1]);
        pLUTSpec->cubeDimensions_iz.i = _mm_set1_epi32(cubeDimensions[2]);
        pLUTSpec->cubeDimensions_fx.f = _mm_cvtepi32_ps(pLUTSpec->cubeDimensions_ix.i);
        pLUTSpec->cubeDimensions_fy.f = _mm_cvtepi32_ps(pLUTSpec->cubeDimensions_iy.i);
        pLUTSpec->cubeDimensions_fz.f = _mm_cvtepi32_ps(pLUTSpec->cubeDimensions_iz.i);
        
        pLUTSpec->maxIndex_ix.i = _mm_set1_epi32(cubeDimensions[0]-1);
        pLUTSpec->maxIndex_iy.i = _mm_set1_epi32(cubeDimensions[1]-1);
        pLUTSpec->maxIndex_iz.i = _mm_set1_epi32(cubeDimensions[2]-1);
        pLUTSpec->maxIndex_fx.f = _mm_cvtepi32_ps(pLUTSpec->maxIndex_ix.i);
        pLUTSpec->maxIndex_fy.f = _mm_cvtepi32_ps(pLUTSpec->maxIndex_iy.i);
        pLUTSpec->maxIndex_fz.f = _mm_cvtepi32_ps(pLUTSpec->maxIndex_iz.i);

        pLUTSpec->cubeMax_ix.i = _mm_set1_epi32(cubeMax[0]);
        pLUTSpec->cubeMax_iy.i = _mm_set1_epi32(cubeMax[1]);
        pLUTSpec->cubeMax_iz.i = _mm_set1_epi32(cubeMax[2]);
        pLUTSpec->cubeMax_fx.f = _mm_cvtepi32_ps(pLUTSpec->cubeMax_ix.i);
        pLUTSpec->cubeMax_fy.f = _mm_cvtepi32_ps(pLUTSpec->cubeMax_iy.i);
        pLUTSpec->cubeMax_fz.f = _mm_cvtepi32_ps(pLUTSpec->cubeMax_iz.i);
        break;

    case DT_REFR:
    default:

        pLUTSpec->cubeDimensions[0] = cubeDimensions[0];
        pLUTSpec->cubeDimensions[1] = cubeDimensions[1];
        pLUTSpec->cubeDimensions[2] = cubeDimensions[2];

        pLUTSpec->maxIndex[0] = cubeDimensions[0]-1;
        pLUTSpec->maxIndex[1] = cubeDimensions[1]-1;
        pLUTSpec->maxIndex[2] = cubeDimensions[2]-1;
        
        pLUTSpec->cubeMax[0] = cubeMax[0];
        pLUTSpec->cubeMax[1] = cubeMax[1];
        pLUTSpec->cubeMax[2] = cubeMax[2];
     }

    return fwStsNoErr;
    }

ISV LookUp3D_fourPixel(XMM128 &x, XMM128 &y,XMM128 &z,const int *pCube, FwiLUTSpec *pLUTSpec)
    {
    x;y;z;pCube;pLUTSpec;

    XMM128 fx,fy,fz;
    XMM128 z1,z2,z3,z4,z5,z6,z7;
    XMM128 y1,y2,y3,y4,y5,y6,y7;
    XMM128 x1,x2,x3,x4,x5,x6,x7;

    XMM128 ix0,iy0,iz0,ix1,iy1,iz1;
    XMM128 fraction_x,fraction_y,fraction_z;
    XMM128 index1,index2,index3,index4,index5,index6,index7,index8;
    XMM128 vertex1_0,vertex2_0,vertex3_0,vertex4_0,vertex5_0,vertex6_0,vertex7_0,vertex8_0;
    XMM128 vertex1_1,vertex2_1,vertex3_1,vertex4_1,vertex5_1,vertex6_1,vertex7_1,vertex8_1;
    XMM128 vertex1_2,vertex2_2,vertex3_2,vertex4_2,vertex5_2,vertex6_2,vertex7_2,vertex8_2;

    // check if overflow
    //if(x>pLUTSpec->cubeMax[0]) x = pLUTSpec->cubeMax[0];
    XMM128 temp;
    temp.i = _mm_cmpgt_epi32(x.i, pLUTSpec->cubeMax_ix.i);
    int mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        x.i = _mm_andnot_si128(temp.i, x.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_ix.i);
        x.i = _mm_or_si128(x.i, temp.i);
        }

    //if(y>pLUTSpec->cubeMax[1]) y = pLUTSpec->cubeMax[1];
    temp.i = _mm_cmpgt_epi32(y.i, pLUTSpec->cubeMax_iy.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        y.i = _mm_andnot_si128(temp.i, y.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_iy.i);
        y.i = _mm_or_si128(y.i, temp.i);
        }
    //if(z>pLUTSpec->cubeMax[2]) z = pLUTSpec->cubeMax[2];
    temp.i = _mm_cmpgt_epi32(z.i, pLUTSpec->cubeMax_iz.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        z.i = _mm_andnot_si128(temp.i, z.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_iz.i);
        z.i = _mm_or_si128(y.i, temp.i);
        }
    // map the input pixel value onto the cube
    //fx = ((float)x/pLUTSpec->cubeMax[0]) * pLUTSpec->maxIndex[0];
    fx.f = _mm_cvtepi32_ps(x.i);
    fx.f = _mm_div_ps(fx.f, pLUTSpec->cubeMax_fx.f);
    fx.f = _mm_mul_ps(fx.f, pLUTSpec->maxIndex_fx.f);
    //fy = ((float)y/pLUTSpec->cubeMax[1]) * pLUTSpec->maxIndex[1];
    fy.f = _mm_cvtepi32_ps(y.i);
    fy.f = _mm_div_ps(fy.f, pLUTSpec->cubeMax_fy.f);
    fy.f = _mm_mul_ps(fy.f, pLUTSpec->maxIndex_fy.f);
    //fz = ((float)z/pLUTSpec->cubeMax[2]) * pLUTSpec->maxIndex[2];
    fz.f = _mm_cvtepi32_ps(z.i);
    fz.f = _mm_div_ps(fz.f, pLUTSpec->cubeMax_fz.f);
    fz.f = _mm_mul_ps(fz.f, pLUTSpec->maxIndex_fz.f);

    // get the indices as integers
    //ix0 = (int)fx;
    ix0.i = _mm_cvtps_epi32(fx.f);
    //iy0 = (int)fy;
    iy0.i = _mm_cvtps_epi32(fy.f);
    //iz0 = (int)fz;
    iz0.i = _mm_cvtps_epi32(fz.f);

    //ix1 = (ix0 == pLUTSpec->maxIndex[0]) ? ix0 : ix0+1;
    ix1.i = _mm_setzero_si128();
    ix1.i = _mm_cmpeq_epi32(ix1.i,ix1.i);
    ix1.i = _mm_srli_epi32(ix1.i, 31);

    iy1.i = _mm_add_epi32(ix1.i, iy0.i);         // incement iy0 and iz0 also by 1
    iz1.i = _mm_add_epi32(ix1.i, iz0.i);

    ix1.i = _mm_add_epi32(ix1.i, ix0.i);
    temp.i = _mm_cmpeq_epi32(ix0.i, pLUTSpec->maxIndex_ix.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        ix1.i = _mm_andnot_si128(temp.i, ix1.i);
        temp.i = _mm_and_si128(temp.i, ix0.i);
        ix1.i = _mm_or_si128(ix1.i, temp.i);
        }

    //iy1 = (iy0 == pLUTSpec->maxIndex[1]) ? iy0 : iy0+1;
    temp.i = _mm_cmpeq_epi32(iy0.i, pLUTSpec->maxIndex_iy.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        iy1.i = _mm_andnot_si128(temp.i, iy1.i);
        temp.i = _mm_and_si128(temp.i, iy0.i);
        iy1.i = _mm_or_si128(iy1.i, temp.i);
        }

    //iz1 = (iz0 == pLUTSpec->maxIndex[2]) ? iz0 : iz0+1;
    temp.i = _mm_cmpeq_epi32(iz0.i, pLUTSpec->maxIndex_iz.i);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        iz1.i = _mm_andnot_si128(temp.i, iz1.i);
        temp.i = _mm_and_si128(temp.i, iz0.i);
        iz1.i = _mm_or_si128(iz1.i, temp.i);
        }

    // get the fractional parts
    //fraction_x   = fx - ix0;
    ix0.f = _mm_cvtepi32_ps(ix0.i);
    fraction_x.f = _mm_sub_ps(fx.f, ix0.f);
    //fraction_y   = fy - iy0;
    iy0.f = _mm_cvtepi32_ps(iy0.i);
    fraction_y.f = _mm_sub_ps(fy.f, iy0.f);
    //fraction_z   = fz - iz0;
    iz0.f = _mm_cvtepi32_ps(iz0.i);
    fraction_z.f = _mm_sub_ps(fz.f, iz0.f);

    // get the 8 vertices surrounding the given input point
    //int temp1 = ix0*pLUTSpec->cubeDimensions[1];
    XMM128 temp1,temp2,temp3,temp4,three;
    temp1.f = _mm_mul_ps(ix0.f, pLUTSpec->cubeDimensions_fy.f);
    //int temp2 = ix1*pLUTSpec->cubeDimensions[1];
    ix1.f = _mm_cvtepi32_ps(ix1.i);
    temp2.f = _mm_mul_ps(ix1.f, pLUTSpec->cubeDimensions_fy.f);
    //int temp3 = pLUTSpec->cubeDimensions[2] * 3;
    three.f = _mm_set1_ps(3.0);
    temp3.f = _mm_mul_ps(pLUTSpec->cubeDimensions_fz.f, three.f);
    //int temp4 = (iz1 - iz0)*3;
    iz1.f = _mm_cvtepi32_ps(iz1.i);
    //iz0.f = _mm_cvtepi32_ps(iz0.i); //iz0 is already converted
    temp4.f = _mm_sub_ps(iz1.f, iz0.f);
    temp4.f = _mm_mul_ps(temp4.f, three.f);

    //index1 = ((temp1 + iy0)*pLUTSpec->cubeDimensions[2] + iz0)*3;
    index1.f = _mm_add_ps(temp1.f, iy0.f);
    index1.f = _mm_mul_ps(index1.f, pLUTSpec->cubeDimensions_fz.f);
    index1.f = _mm_add_ps(index1.f, iz0.f);
    index1.f = _mm_mul_ps(index1.f, three.f);    
    //index2 = index1 + (temp2 - temp1) * temp3;
    index2.f = _mm_sub_ps(temp2.f, temp1.f);
    index2.f = _mm_mul_ps(index2.f, temp3.f);
    index4.f = index2.f;                        // fill the temporary value here
    index2.f = _mm_add_ps(index2.f, index1.f);
    //index3  = index1 + (iy1 - iy0) * temp3;
    iy1.f    = _mm_cvtepi32_ps(iy1.i);
    index3.f = _mm_sub_ps(iy1.f, iy0.f);
    index3.f = _mm_mul_ps(index3.f, temp3.f);
    index3.f = _mm_add_ps(index3.f, index1.f);
    //index4  = index3 + (temp2 - temp1) * temp3; == index3+index2-index1
    index4.f = _mm_add_ps(index4.f, index3.f); 

    index1.i = _mm_cvtps_epi32(index1.f);
    index2.i = _mm_cvtps_epi32(index2.f);
    index3.i = _mm_cvtps_epi32(index3.f);
    index4.i = _mm_cvtps_epi32(index4.f);
    ///index5  = index1 + temp4;
    temp4.i = _mm_cvtps_epi32(temp4.f);
    index5.i = _mm_add_epi32(index1.i, temp4.i);
    //index6  = index2 + temp4;
    index6.i = _mm_add_epi32(index2.i, temp4.i);
    //index7  = index3 + temp4;
    index7.i = _mm_add_epi32(index3.i, temp4.i);
    //index8  = index4 + temp4;
    index8.i = _mm_add_epi32(index4.i, temp4.i);

    //vertex1 = &pCube[index1];
    vertex1_0.f = _mm_set_ps((float)pCube[index1.u32[3]],(float)pCube[index1.u32[2]],(float)pCube[index1.u32[1]],(float)pCube[index1.u32[0]]);
    vertex1_1.f = _mm_set_ps((float)pCube[index1.u32[3]+1],(float)pCube[index1.u32[2]+1],(float)pCube[index1.u32[1]+1],(float)pCube[index1.u32[0]+1]);
    vertex1_2.f = _mm_set_ps((float)pCube[index1.u32[3]+2],(float)pCube[index1.u32[2]+2],(float)pCube[index1.u32[1]+2],(float)pCube[index1.u32[0]+2]);
    //vertex2 = &pCube[index2];
    vertex2_0.f = _mm_set_ps((float)pCube[index2.u32[3]],(float)pCube[index2.u32[2]],(float)pCube[index2.u32[1]],(float)pCube[index2.u32[0]]);
    vertex2_1.f = _mm_set_ps((float)pCube[index2.u32[3]+1],(float)pCube[index2.u32[2]+1],(float)pCube[index2.u32[1]+1],(float)pCube[index2.u32[0]+1]);
    vertex2_2.f = _mm_set_ps((float)pCube[index2.u32[3]+2],(float)pCube[index2.u32[2]+2],(float)pCube[index2.u32[1]+2],(float)pCube[index2.u32[0]+2]);
    //vertex3 = &pCube[index3];
    vertex3_0.f = _mm_set_ps((float)pCube[index3.u32[3]],(float)pCube[index3.u32[2]],(float)pCube[index3.u32[1]],(float)pCube[index3.u32[0]]);
    vertex3_1.f = _mm_set_ps((float)pCube[index3.u32[3]+1],(float)pCube[index3.u32[2]+1],(float)pCube[index3.u32[1]+1],(float)pCube[index3.u32[0]+1]);
    vertex3_2.f = _mm_set_ps((float)pCube[index3.u32[3]+2],(float)pCube[index3.u32[2]+2],(float)pCube[index3.u32[1]+2],(float)pCube[index3.u32[0]+2]);
    //vertex4 = &pCube[index4];
    vertex4_0.f = _mm_set_ps((float)pCube[index4.u32[3]],(float)pCube[index4.u32[2]],(float)pCube[index4.u32[1]],(float)pCube[index4.u32[0]]);
    vertex4_1.f = _mm_set_ps((float)pCube[index4.u32[3]+1],(float)pCube[index4.u32[2]+1],(float)pCube[index4.u32[1]+1],(float)pCube[index4.u32[0]+1]);
    vertex4_2.f = _mm_set_ps((float)pCube[index4.u32[3]+2],(float)pCube[index4.u32[2]+2],(float)pCube[index4.u32[1]+2],(float)pCube[index4.u32[0]+2]);
    //vertex5 = &pCube[index5];
    vertex5_0.f = _mm_set_ps((float)pCube[index5.u32[3]],(float)pCube[index5.u32[2]],(float)pCube[index5.u32[1]],(float)pCube[index5.u32[0]]);
    vertex5_1.f = _mm_set_ps((float)pCube[index5.u32[3]+1],(float)pCube[index5.u32[2]+1],(float)pCube[index5.u32[1]+1],(float)pCube[index5.u32[0]+1]);
    vertex5_2.f = _mm_set_ps((float)pCube[index5.u32[3]+2],(float)pCube[index5.u32[2]+2],(float)pCube[index5.u32[1]+2],(float)pCube[index5.u32[0]+2]);
    //vertex6 = &pCube[index6];
    vertex6_0.f = _mm_set_ps((float)pCube[index6.u32[3]],(float)pCube[index6.u32[2]],(float)pCube[index6.u32[1]],(float)pCube[index6.u32[0]]);
    vertex6_1.f = _mm_set_ps((float)pCube[index6.u32[3]+1],(float)pCube[index6.u32[2]+1],(float)pCube[index6.u32[1]+1],(float)pCube[index6.u32[0]+1]);
    vertex6_2.f = _mm_set_ps((float)pCube[index6.u32[3]+2],(float)pCube[index6.u32[2]+2],(float)pCube[index6.u32[1]+2],(float)pCube[index6.u32[0]+2]);
    //vertex7 = &pCube[index7];
    vertex7_0.f = _mm_set_ps((float)pCube[index7.u32[3]],(float)pCube[index7.u32[2]],(float)pCube[index7.u32[1]],(float)pCube[index7.u32[0]]);
    vertex7_1.f = _mm_set_ps((float)pCube[index7.u32[3]+1],(float)pCube[index7.u32[2]+1],(float)pCube[index7.u32[1]+1],(float)pCube[index7.u32[0]+1]);
    vertex7_2.f = _mm_set_ps((float)pCube[index7.u32[3]+2],(float)pCube[index7.u32[2]+2],(float)pCube[index7.u32[1]+2],(float)pCube[index7.u32[0]+2]);
    //vertex8 = &pCube[index8];
    vertex8_0.f = _mm_set_ps((float)pCube[index8.u32[3]],(float)pCube[index8.u32[2]],(float)pCube[index8.u32[1]],(float)pCube[index8.u32[0]]);
    vertex8_1.f = _mm_set_ps((float)pCube[index8.u32[3]+1],(float)pCube[index8.u32[2]+1],(float)pCube[index8.u32[1]+1],(float)pCube[index8.u32[0]+1]);
    vertex8_2.f = _mm_set_ps((float)pCube[index8.u32[3]+2],(float)pCube[index8.u32[2]+2],(float)pCube[index8.u32[1]+2],(float)pCube[index8.u32[0]+2]);

    // interpolate for x,y and z
    //x1 = vertex1[0] + fraction_x * (vertex2[0] - vertex1[0]);
    x1.f = _mm_sub_ps(vertex2_0.f, vertex1_0.f);
    x1.f = _mm_mul_ps(x1.f, fraction_x.f);
    x1.f = _mm_add_ps(x1.f, vertex1_0.f);
    //y1 = vertex1[1] + fraction_x * (vertex2[1] - vertex1[1]);
    y1.f = _mm_sub_ps(vertex2_1.f, vertex1_1.f);
    y1.f = _mm_mul_ps(y1.f, fraction_x.f);
    y1.f = _mm_add_ps(y1.f, vertex1_1.f);
    //z1 = vertex1[2] + fraction_x * (vertex2[2] - vertex1[2]);
    z1.f = _mm_sub_ps(vertex2_2.f, vertex1_2.f);
    z1.f = _mm_mul_ps(z1.f, fraction_x.f);
    z1.f = _mm_add_ps(z1.f, vertex1_2.f);
    //x2 = vertex3[0] + fraction_x * (vertex4[0] - vertex3[0]);
    x2.f = _mm_sub_ps(vertex4_0.f, vertex3_0.f);
    x2.f = _mm_mul_ps(x2.f, fraction_x.f);
    x2.f = _mm_add_ps(x2.f, vertex3_0.f);
    //y2 = vertex3[1] + fraction_x * (vertex4[1] - vertex3[1]);
    y2.f = _mm_sub_ps(vertex4_1.f, vertex3_1.f);
    y2.f = _mm_mul_ps(y2.f, fraction_x.f);
    y2.f = _mm_add_ps(y2.f, vertex3_1.f);
    //z2 = vertex3[2] + fraction_x * (vertex4[2] - vertex3[2]);
    z2.f = _mm_sub_ps(vertex4_2.f, vertex3_2.f);
    z2.f = _mm_mul_ps(z2.f, fraction_x.f);
    z2.f = _mm_add_ps(z2.f, vertex3_2.f);
    //x3 = vertex5[0] + fraction_x * (vertex6[0] - vertex5[0]);
    x3.f = _mm_sub_ps(vertex6_0.f, vertex5_0.f);
    x3.f = _mm_mul_ps(x3.f, fraction_x.f);
    x3.f = _mm_add_ps(x3.f, vertex5_0.f);
    //y3 = vertex5[1] + fraction_x * (vertex6[1] - vertex5[1]);
    y3.f = _mm_sub_ps(vertex6_1.f, vertex5_1.f);
    y3.f = _mm_mul_ps(y3.f, fraction_x.f);
    y3.f = _mm_add_ps(y3.f, vertex5_1.f);
    //z3 = vertex5[2] + fraction_x * (vertex6[2] - vertex5[2]);
    z3.f = _mm_sub_ps(vertex6_2.f, vertex5_2.f);
    z3.f = _mm_mul_ps(z3.f, fraction_x.f);
    z3.f = _mm_add_ps(z3.f, vertex5_2.f);
    //x4 = vertex7[0] + fraction_x * (vertex8[0] - vertex7[0]);
    x4.f = _mm_sub_ps(vertex8_0.f, vertex7_0.f);
    x4.f = _mm_mul_ps(x4.f, fraction_x.f);
    x4.f = _mm_add_ps(x4.f, vertex7_0.f);
    //y4 = vertex7[1] + fraction_x * (vertex8[1] - vertex7[1]);
    y4.f = _mm_sub_ps(vertex8_1.f, vertex7_1.f);
    y4.f = _mm_mul_ps(y4.f, fraction_x.f);
    y4.f = _mm_add_ps(y4.f, vertex7_1.f);
    //z4 = vertex7[2] + fraction_x * (vertex8[2] - vertex7[2]);
    z4.f = _mm_sub_ps(vertex8_2.f, vertex7_2.f);
    z4.f = _mm_mul_ps(z4.f, fraction_x.f);
    z4.f = _mm_add_ps(z4.f, vertex7_2.f);
    //x5 = x1 + fraction_y * (x2 - x1);
    x5.f = _mm_sub_ps(x2.f, x1.f);
    x5.f = _mm_mul_ps(x5.f, fraction_y.f);
    x5.f = _mm_add_ps(x5.f, x1.f);
    //y5 = y1 + fraction_y * (y2 - y1);
    y5.f = _mm_sub_ps(y2.f, y1.f);
    y5.f = _mm_mul_ps(y5.f, fraction_y.f);
    y5.f = _mm_add_ps(y5.f, y1.f);
    //z5 = z1 + fraction_y * (z2 - z1);
    z5.f = _mm_sub_ps(z2.f, z1.f);
    z5.f = _mm_mul_ps(z5.f, fraction_y.f);
    z5.f = _mm_add_ps(z5.f, z1.f);
    //x6 = x3 + fraction_y * (x4 - x3);
    x6.f = _mm_sub_ps(x4.f, x3.f);
    x6.f = _mm_mul_ps(x6.f, fraction_y.f);
    x6.f = _mm_add_ps(x6.f, x3.f);
    //y6 = y3 + fraction_y * (y4 - y3);
    y6.f = _mm_sub_ps(y4.f, y3.f);
    y6.f = _mm_mul_ps(y6.f, fraction_y.f);
    y6.f = _mm_add_ps(y6.f, y3.f);
    //z6 = z3 + fraction_y * (z4 - z3);
    z6.f = _mm_sub_ps(z4.f, z3.f);
    z6.f = _mm_mul_ps(z6.f, fraction_y.f);
    z6.f = _mm_add_ps(z6.f, z3.f);
    //x7 = x5 + fraction_z * (x6 - x5);
    x7.f = _mm_sub_ps(x6.f, x5.f);
    x7.f = _mm_mul_ps(x7.f, fraction_z.f);
    x7.f = _mm_add_ps(x7.f, x5.f);
    //y7 = y5 + fraction_z * (y6 - y5);
    y7.f = _mm_sub_ps(y6.f, y5.f);
    y7.f = _mm_mul_ps(y7.f, fraction_z.f);
    y7.f = _mm_add_ps(y7.f, y5.f);
    //z7 = z5 + fraction_z * (z6 - z5);
    z7.f = _mm_sub_ps(z6.f, z5.f);
    z7.f = _mm_mul_ps(z7.f, fraction_z.f);
    z7.f = _mm_add_ps(z7.f, z5.f);

    //x  = (x7>pLUTSpec->cubeMax[0]) ? pLUTSpec->cubeMax[0] : (Fw16u)x7;
    x.i = _mm_cvtps_epi32(x7.f);
    temp.f = _mm_cmpgt_ps(x7.f, pLUTSpec->cubeMax_fx.f);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        x.i  = _mm_andnot_si128(temp.i, x.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_fx.i);
        x.i = _mm_or_si128(x.i, temp.i);
        }
    //y  = (y7>pLUTSpec->cubeMax[1]) ? pLUTSpec->cubeMax[1] : (Fw16u)y7;
    y.i = _mm_cvtps_epi32(y7.f);
    temp.f = _mm_cmpgt_ps(y7.f, pLUTSpec->cubeMax_fy.f);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        y.i  = _mm_andnot_si128(temp.i, y.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_fy.i);
        y.i = _mm_or_si128(y.i, temp.i);
        }
    //z  = (z7>pLUTSpec->cubeMax[2]) ? pLUTSpec->cubeMax[2] : (Fw16u)z7;
    z.i = _mm_cvtps_epi32(z7.f);
    temp.f = _mm_cmpgt_ps(z7.f, pLUTSpec->cubeMax_fz.f);
    mask = _mm_movemask_epi8(temp.i);
    if(mask)
        {
        z.i  = _mm_andnot_si128(temp.i, z.i);
        temp.i = _mm_and_si128(temp.i, pLUTSpec->cubeMax_fz.i);
        z.i = _mm_or_si128(z.i, temp.i);
        }
   }

ISV LookUp3D_onePixel(Fw16u &x,Fw16u &y,Fw16u &z, const int *pCube, FwiLUTSpec *pLUTSpec)
    {

    float fx,fy,fz;
    float z1,z2,z3,z4,z5,z6,z7;
    float y1,y2,y3,y4,y5,y6,y7;
    float x1,x2,x3,x4,x5,x6,x7;
    int   ix0,iy0,iz0,ix1,iy1,iz1;
    float fraction_x,fraction_y,fraction_z;
    int   index1,index2,index3,index4,index5,index6,index7,index8;
    const int   *vertex1,*vertex2,*vertex3,*vertex4,*vertex5,*vertex6,*vertex7,*vertex8;

    // check if overflow
    if(x>pLUTSpec->cubeMax[0]) x = pLUTSpec->cubeMax[0];
    if(y>pLUTSpec->cubeMax[1]) y = pLUTSpec->cubeMax[1];
    if(z>pLUTSpec->cubeMax[2]) z = pLUTSpec->cubeMax[2];

    // map the input pixel value onto the cube
    fx = ((float)x/pLUTSpec->cubeMax[0]) * pLUTSpec->maxIndex[0];
    fy = ((float)y/pLUTSpec->cubeMax[1]) * pLUTSpec->maxIndex[1];
    fz = ((float)z/pLUTSpec->cubeMax[2]) * pLUTSpec->maxIndex[2];

    // get the indices as integers
    ix0 = (int)fx;
    iy0 = (int)fy;
    iz0 = (int)fz;

    ix1 = (ix0 == pLUTSpec->maxIndex[0]) ? ix0 : ix0+1;
    iy1 = (iy0 == pLUTSpec->maxIndex[1]) ? iy0 : iy0+1;
    iz1 = (iz0 == pLUTSpec->maxIndex[2]) ? iz0 : iz0+1;

    // get the fractional parts
    fraction_x   = fx - ix0;
    fraction_y   = fy - iy0;
    fraction_z   = fz - iz0;
    
    // get the 8 vertices surrounding the given input point
    int temp1 = ix0*pLUTSpec->cubeDimensions[1];
    int temp2 = ix1*pLUTSpec->cubeDimensions[1];
    int temp3 = pLUTSpec->cubeDimensions[2] * 3;
    int temp4 = (iz1 - iz0)*3;

    index1  = ((temp1 + iy0)*pLUTSpec->cubeDimensions[2] + iz0)*3;
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

    x  = (x7>pLUTSpec->cubeMax[0]) ? pLUTSpec->cubeMax[0] : (Fw16u)x7;
    y  = (y7>pLUTSpec->cubeMax[1]) ? pLUTSpec->cubeMax[1] : (Fw16u)y7;
    z  = (z7>pLUTSpec->cubeMax[2]) ? pLUTSpec->cubeMax[2] : (Fw16u)z7;
   }

IS FwStatus LookUp3D_16u_C3R(const Fw16u *pSrc, int &srcStep, Fw16u *pDst, int &dstStep, FwiSize &roiSize, const int *pCube, FwiLUTSpec *pLUTSpec)
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
        int pixnsse = width>>2;      // width/4
        int pixnref = width - (pixnsse<<2); // width - pixnsse*4

        const Fw16u *src = pSrc;
        Fw16u *dst = pDst;

        for(int i=0;i<height;i++)
            {
            const Fw16u *tempSrc = src;
            Fw16u *tempDst    = dst;

            for(int j=0;j<pixnsse;j++)
                {
                 x.i = _mm_set_epi32(tempSrc[9],tempSrc[6],tempSrc[3],tempSrc[0]);
                 y.i = _mm_set_epi32(tempSrc[10],tempSrc[7],tempSrc[4],tempSrc[1]);
                 z.i = _mm_set_epi32(tempSrc[11],tempSrc[8],tempSrc[5],tempSrc[2]);

                 LookUp3D_fourPixel(x,y,z,pCube,pLUTSpec);

                 x.i = _mm_set_epi16((Fw16u)y.u32[2],(Fw16u)x.u32[2],(Fw16u)z.u32[1],(Fw16u)y.u32[1],(Fw16u)x.u32[1],(Fw16u)z.u32[0],(Fw16u)y.u32[0],(Fw16u)x.u32[0]);
                 y.i = _mm_set_epi16(0,0,0,0,(Fw16u)z.u32[3],(Fw16u)y.u32[3],(Fw16u)x.u32[3],(Fw16u)z.u32[2]);
                 _mm_storeu_si128((__m128i*)tempDst, x.i);
                 tempDst += 8;
                 _mm_storel_epi64((__m128i*)tempDst, y.i);
                 tempDst += 4;
                 tempSrc += 12;
                }

            for(int k=0;k<pixnref;k++)
                {
                 Fw16u x,y,z;
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

IS FwStatus LookUp3D_16u_C3IR(Fw16u *pSrcDst, int &srcDstStep, FwiSize &roiSize, const int *pCube, FwiLUTSpec *pLUTSpec)
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
        int pixnsse = width>>2;      // width/4
        int pixnref = width - (pixnsse<<2); // width - pixnsse*4

        Fw16u *srcDst = pSrcDst;        

        for(int i=0;i<height;i++)
            {
            Fw16u *tempSrcDst = srcDst;

            for(int j=0;j<pixnsse;j++)
                {
                 x.i = _mm_set_epi32(tempSrcDst[9],tempSrcDst[6],tempSrcDst[3],tempSrcDst[0]);
                 y.i = _mm_set_epi32(tempSrcDst[10],tempSrcDst[7],tempSrcDst[4],tempSrcDst[1]);
                 z.i = _mm_set_epi32(tempSrcDst[11],tempSrcDst[8],tempSrcDst[5],tempSrcDst[2]);

                 LookUp3D_fourPixel(x,y,z,pCube,pLUTSpec);

                 x.i = _mm_set_epi16((Fw16u)y.u32[2],(Fw16u)x.u32[2],(Fw16u)z.u32[1],(Fw16u)y.u32[1],(Fw16u)x.u32[1],(Fw16u)z.u32[0],(Fw16u)y.u32[0],(Fw16u)x.u32[0]);
                 y.i = _mm_set_epi16(0,0,0,0,(Fw16u)z.u32[3],(Fw16u)y.u32[3],(Fw16u)x.u32[3],(Fw16u)z.u32[2]);
                 _mm_storeu_si128((__m128i*)tempSrcDst, x.i);
                 tempSrcDst += 8;
                 _mm_storel_epi64((__m128i*)tempSrcDst, y.i);
                 tempSrcDst += 4;
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

} // end namespace OPT_LEVEL
#endif
