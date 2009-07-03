/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.

The following Deblock filter code is based on JVT-G050r1 document 27 May 2003 
*/

#ifndef __H264_DEBLOCK_H__
#define __H264_DEBLOCK_H__

#include "fe.h"

namespace OPT_LEVEL
{
IS int clip3(int x, int y,int z)
    {
    if(z<x) return x;
    else if(z>y) return y;
    else return z;
    }

IS int clip1(int x)
    {
    return clip3(0,255,x);
    }

// This function follows the section 8.7.2.3 of H.264 standard JVT G050
ISV filter_luma_BS_LT_4(Fw8u *pSrcDstTemp, Fw32s step, Fw8u &alpha, Fw8u &beta, Fw8u &tc0, Fw32s dirFactor)
    {

    Fw8u ap,aq,tc;
    Fw8u p0,p1,p2,q0,q1,q2;
    int delta;

    for(int k=0;k<4;k++) // process all the 4 rows (or columns) in the current 4*4 block
        {
        p2 = pSrcDstTemp[-3*dirFactor]; // select the pixels at the edge for this row (or column)
        p1 = pSrcDstTemp[-2*dirFactor];
        p0 = pSrcDstTemp[-dirFactor];
        q0 = pSrcDstTemp[0];
        q1 = pSrcDstTemp[dirFactor];
        q2 = pSrcDstTemp[2*dirFactor];

        if( abs( p0 - q0 ) < alpha &&  abs( p1 - p0 ) < beta && abs( q1 - q0 ) < beta )
            {
            ap = (Fw8u)abs(p2 - p0);
            aq = (Fw8u)abs(q2 - q0);
            tc = tc0;

            if(ap < beta)
                {
                pSrcDstTemp[-2*dirFactor] = (Fw8u)(p1 + clip3( -tc0, tc0, ( p2 + ( ( p0 + q0 + 1 ) >> 1 ) - ( p1 << 1 ) ) >> 1 ));
                tc += 1;
                }
            if(aq < beta)
                {
                pSrcDstTemp[dirFactor] = (Fw8u)(q1 + clip3( -tc0, tc0, ( q2 + ( ( p0 + q0 + 1 ) >> 1 ) - ( q1 << 1 ) ) >> 1 ));
                tc +=1;
                }
            delta = clip3( -tc, tc, ( ( ( ( q0 - p0 ) << 2 ) + ( p1 - q1 ) + 4 ) >> 3 ) );
            pSrcDstTemp[-dirFactor]  = (Fw8u)clip1( p0 + delta );
            pSrcDstTemp[0]   = (Fw8u)clip1( q0 - delta );
            }
        pSrcDstTemp += step; // move to next row (or column) in the current 4*4 block
        }
    }

// This function follows the section 8.7.2.4 of H.264 standard JVT G050
ISV filter_luma_BS_EQ_4(Fw8u *pSrcDstTemp, Fw32s step, Fw8u &alpha, Fw8u &beta, Fw32s dirFactor)
    {

    Fw8u ap,aq;
    Fw8u p0,p1,p2,p3,q0,q1,q2,q3;

    for(int k=0;k<4;k++) // process all the 4 rows (or columns) in the current 4*4 block
        {
        p3 = pSrcDstTemp[-4*dirFactor]; // select the pixels at the edge for this row (or column)
        p2 = pSrcDstTemp[-3*dirFactor];
        p1 = pSrcDstTemp[-2*dirFactor];
        p0 = pSrcDstTemp[-dirFactor];
        q0 = pSrcDstTemp[0];
        q1 = pSrcDstTemp[dirFactor];
        q2 = pSrcDstTemp[2*dirFactor];
        q3 = pSrcDstTemp[3*dirFactor];

        if( abs( p0 - q0 ) < alpha &&  abs( p1 - p0 ) < beta && abs( q1 - q0 ) < beta )
            {
            ap = (Fw8u)abs(p2 - p0);
            aq = (Fw8u)abs(q2 - q0);

            if(abs( p0 - q0 ) < ( ( alpha >> 2 ) + 2 ))
                {
                if(ap < beta)
                    {
                    pSrcDstTemp[-dirFactor] = ( p2 + 2*p1 + 2*p0 + 2*q0 + q1 + 4 ) >> 3;
                    pSrcDstTemp[-2*dirFactor] = ( p2 + p1 + p0 + q0 + 2 ) >> 2;
                    pSrcDstTemp[-3*dirFactor] = ( 2*p3 + 3*p2 + p1 + p0 + q0 + 4 ) >> 3;
                    }
                else
                    pSrcDstTemp[-dirFactor] = ( 2*p1 + p0 + q1 + 2 ) >> 2;

                if(aq < beta)
                    {
                    pSrcDstTemp[0] = ( p1 + 2*p0 + 2*q0 + 2*q1 + q2 + 4 ) >> 3;
                    pSrcDstTemp[dirFactor] = ( p0 + q0 + q1 + q2 + 2 ) >> 2;
                    pSrcDstTemp[2*dirFactor] = ( 2*q3 + 3*q2 + q1 + q0 + p0 + 4 ) >> 3;
                    }
                else
                    pSrcDstTemp[0] = ( 2*q1 + q0 + p1 + 2 ) >> 2;
                }
            else
                {
                pSrcDstTemp[-dirFactor] = ( 2*p1 + p0 + q1 + 2 ) >> 2;
                pSrcDstTemp[0] = ( 2*q1 + q0 + p1 + 2 ) >> 2;
                }
            }
        pSrcDstTemp += step; // move to next row (or column) in the current 4*4 block
        }
    }

IS FwStatus filterDeblockingLuma_VerEdge_H264_8u_C1IR( Fw8u *pSrcDst, Fw32s  srcDstStep, Fw8u *pAlpha, Fw8u *pBeta, Fw8u *pThresholds, Fw8u *pBS)
    {
     if(FW_REF::PtrNotOK(pSrcDst, pAlpha, pBeta, pThresholds, pBS))
         return fwStsNullPtrErr;

     Fw8u* pSrcDstTemp;
     Fw8u alpha,beta,tc0,bs;

     for(int i=0;i<4;i++)     // process the 4 vertical edges
         {
         (i==0) ? alpha = pAlpha[0], beta = pBeta[0] : alpha = pAlpha[1], beta = pBeta[1]; 
         pSrcDstTemp = pSrcDst;         

         for(int j=0;j<4;j++) // process the four 4*4 blocks along one edge 
             {
             tc0 = pThresholds[0];
             bs =  pBS[0];

             switch(bs)
                 {
                 case 0:      // no filtering when bs == 0
                     pSrcDstTemp += 4*srcDstStep; // we need not modify the 4*4 block
                     break;

                 case 4:      // strong filter when bs == 4
                     filter_luma_BS_EQ_4(pSrcDstTemp,srcDstStep,alpha,beta,1);
                     break;

                default:      // weak filter when bs = 1,2 or 3
                     filter_luma_BS_LT_4(pSrcDstTemp,srcDstStep,alpha,beta,tc0,1);
                 }            // end switch
             pBS += 1;        // move to BS of next 4*4 block(vertical order)
             pThresholds += 1;// move to threshold of next 4*4 block(vertical order)
             }
         pSrcDst+=4;          // move to next vertical boundary
         }
     
     return fwStsNoErr;
    }

IS FwStatus filterDeblockingLuma_HorEdge_H264_8u_C1IR( Fw8u *pSrcDst, Fw32s srcDstStep,Fw8u *pAlpha, Fw8u *pBeta, Fw8u *pThresholds, Fw8u *pBS)
    {
     if(FW_REF::PtrNotOK(pSrcDst, pAlpha, pBeta, pThresholds, pBS))
         return fwStsNullPtrErr;

     Fw8u* pSrcDstTemp;
     Fw8u alpha,beta,tc0,bs;

     for(int i=0;i<4;i++)      // process the 4 horizontal edges
         {
         (i==0) ? alpha = pAlpha[0], beta = pBeta[0] : alpha = pAlpha[1], beta = pBeta[1]; 
         pSrcDstTemp = pSrcDst;         

         for(int j=0;j<4;j++)  // process the four 4*4 blocks along one edge 
             {
             tc0 = pThresholds[0];
             bs =  pBS[0];

             switch(bs)
                 {
                 case 0:       // no filtering when bs == 0
                     pSrcDstTemp += 4; // we need not modify the 4*4 block
                     break;

                 case 4:       // filter when bs == 4                      
                     filter_luma_BS_EQ_4(pSrcDstTemp,1,alpha,beta,srcDstStep);
                     break;

                default:       // weak filter when bs = 1,2 or 3
                     filter_luma_BS_LT_4(pSrcDstTemp,1,alpha,beta,tc0,srcDstStep);
                 }             // end switch
             pBS += 1;         // move to BS of next 4*4 block(horizontal order)
             pThresholds += 1; // move to threshold of next 4*4 block(horizontal order)
             }
         pSrcDst+=4*srcDstStep;// move to next horizontal boundary
         }
    return fwStsNoErr;
    }

// This function follows the section 8.7.2.3 of H.264 standard JVT G050
ISV filter_chroma_BS_LT_4(Fw8u *pSrcDstTemp, Fw32s step, Fw8u &alpha, Fw8u &beta, Fw8u &tc0, Fw32s dirFactor)
    {

    Fw8u tc;
    Fw8u p0,p1,q0,q1;
    int delta;
    
    tc = tc0+1;
    for(int k=0;k<2;k++) // process the 2 rows (or columns) in the current 2*2 block
        {
        p1 = pSrcDstTemp[-2*dirFactor];// select the pixels at the edge for this row (or column)
        p0 = pSrcDstTemp[-dirFactor];
        q0 = pSrcDstTemp[0];
        q1 = pSrcDstTemp[dirFactor];

        if( abs( p0 - q0 ) < alpha &&  abs( p1 - p0 ) < beta && abs( q1 - q0 ) < beta )
            {
            delta = clip3( -tc, tc, ( ( ( ( q0 - p0 ) << 2 ) + ( p1 - q1 ) + 4 ) >> 3 ) );
            pSrcDstTemp[-dirFactor]  = (Fw8u)clip1( p0 + delta );
            pSrcDstTemp[0]   = (Fw8u)clip1( q0 - delta );
            }
        pSrcDstTemp += step; // move to next row (or column) in the current 4*4 block
        }
    }

// This function follows the section 8.7.2.4 of H.264 standard JVT G050
ISV filter_chroma_BS_EQ_4(Fw8u *pSrcDstTemp, Fw32s step, Fw8u &alpha, Fw8u &beta, Fw32s dirFactor)
    {

    Fw8u p0,p1,q0,q1;

    for(int k=0;k<2;k++) // process the 2 rows (or columns) in the current 2*2 block
        {
        p1 = pSrcDstTemp[-2*dirFactor];// select the pixels at the edge for this row (or column)
        p0 = pSrcDstTemp[-dirFactor];
        q0 = pSrcDstTemp[0];
        q1 = pSrcDstTemp[dirFactor];

        if( abs( p0 - q0 ) < alpha &&  abs( p1 - p0 ) < beta && abs( q1 - q0 ) < beta )
            {
             pSrcDstTemp[-dirFactor] = ( 2*p1 + p0 + q1 + 2 ) >> 2;
             pSrcDstTemp[0] = ( 2*q1 + q0 + p1 + 2 ) >> 2;
            }
        pSrcDstTemp += step; // move to next row (or column) in the current 4*4 block
        }
    }

IS FwStatus filterDeblockingChroma_HorEdge_H264_8u_C1IR( Fw8u *pSrcDst, Fw32s srcDstStep, Fw8u *pAlpha, Fw8u *pBeta, Fw8u *pThresholds, Fw8u *pBS)
    {
     if(FW_REF::PtrNotOK(pSrcDst, pAlpha, pBeta, pThresholds, pBS))
         return fwStsNullPtrErr;

     Fw8u* pSrcDstTemp;
     Fw8u alpha,beta,tc0,bs;

     for(int i=0;i<2;i++)      // process the 2 horizontal edges
         {
         (i==0) ? alpha = pAlpha[0], beta = pBeta[0] : alpha = pAlpha[1], beta = pBeta[1]; 
         pSrcDstTemp = pSrcDst;         

         for(int j=0;j<4;j++)  // process the four 2*2 blocks along one edge 
             {
             tc0 = pThresholds[0];
             bs =  pBS[0];

             switch(bs)
                 {
                 case 0:       // no filtering when bs == 0
                     pSrcDstTemp += 2; // we need not modify the 2*2 block
                     break;

                 case 4:       // filter when bs == 4                      
                     filter_chroma_BS_EQ_4(pSrcDstTemp,1,alpha,beta,srcDstStep);
                     break;

                default:       // weak filter when bs = 1,2 or 3
                     filter_chroma_BS_LT_4(pSrcDstTemp,1,alpha,beta,tc0,srcDstStep);
                 }             // end switch
             pBS += 1;         // move to BS of next 2*2 block(horizontal order)
             pThresholds += 1; // move to threshold of next 2*2 block(horizontal order)
             }
         pSrcDst+=4*srcDstStep;// move to next horizontal boundary
         pBS +=4;             // since pBS is given for all 2*2 blocks unlike pThresholds
         }
    return fwStsNoErr;
    }

IS FwStatus filterDeblockingChroma_VerEdge_H264_8u_C1IR( Fw8u *pSrcDst, Fw32s srcDstStep, Fw8u *pAlpha, Fw8u *pBeta, Fw8u *pThresholds, Fw8u *pBS)
    {
     if(FW_REF::PtrNotOK(pSrcDst, pAlpha, pBeta, pThresholds, pBS))
         return fwStsNullPtrErr;

     Fw8u* pSrcDstTemp;
     Fw8u alpha,beta,tc0,bs;

     for(int i=0;i<2;i++)     // process the 2 vertical edges
         {
         (i==0) ? alpha = pAlpha[0], beta = pBeta[0] : alpha = pAlpha[1], beta = pBeta[1]; 
         pSrcDstTemp = pSrcDst;         

         for(int j=0;j<4;j++) // process the four 2*2 blocks along one edge 
             {
             tc0 = pThresholds[0];
             bs =  pBS[0];

             switch(bs)
                 {
                 case 0:      // no filtering when bs == 0
                     pSrcDstTemp += 2*srcDstStep; // we need not modify the 2*2 block
                     break;

                 case 4:      // strong filter when bs == 4
                     filter_chroma_BS_EQ_4(pSrcDstTemp,srcDstStep,alpha,beta,1);
                     break;

                default:      // weak filter when bs = 1,2 or 3
                     filter_chroma_BS_LT_4(pSrcDstTemp,srcDstStep,alpha,beta,tc0,1);
                 }            // end switch
             pBS += 1;        // move to BS of next 4*4 block(vertical order)
             pThresholds += 1;// move to threshold of next 4*4 block(vertical order)
             }
         pSrcDst+=4;          // move to next vertical boundary
         pBS +=4;             // since pBS is given for all 2*2 blocks unlike pThresholds
         }
     
     return fwStsNoErr;
    }

} // end namespace OPT_LEVEL
#endif
