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
    Fw16u cubeDimensions[3];
    Fw16u maxIndex[3];
    Fw16u cubeMax[3];
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
	case DT_SSE2: // TODO::implement threading and SSE2 code...
    case DT_REFR:
    default:

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

    return fwStsNoErr;
    }

IS FwStatus LookUp3D_16u_C3IR(Fw16u *pSrcDst, int &srcDstStep, FwiSize &roiSize, const int *pCube, FwiLUTSpec *pLUTSpec)
    {
    if(FW_REF::PtrNotOK(pSrcDst, pCube, pLUTSpec))
        return fwStsNullPtrErr;

   	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2: // TODO::implement threading and SSE2 code...
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
