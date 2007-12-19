/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

// Macros used to express FW function headers, and arguments.


#ifndef __FNTYPES_H__
#define __FNTYPES_H__

// 11
#define ARGS_11C              TS value  , TD *pSrcDst,                 int len
#define PARAM_11C                value  ,     pSrcDst,                     len
#define ARGS_11CR             TS value  , TD *pSrcDst, int srcDstStep, FwiSize roiSize
#define PARAM_11CR               value  ,     pSrcDst,     srcDstStep,          roiSize
#define ARGS_11KR       const TS value[], TD *pSrcDst, int srcDstStep, FwiSize roiSize
#define PARAM_11KR               value  ,     pSrcDst,     srcDstStep,          roiSize
#define ARGS_11R                          TD *pSrcDst, int srcDstStep, FwiSize roiSize
#define PARAM_11R                             pSrcDst,     srcDstStep,          roiSize

#define ARGS_11CS             TS value  , TD *pSrcDst,                 int len,          int scaleFactor
#define PARAM_11CS               value  ,     pSrcDst,                     len,              scaleFactor
#define ARGS_11KS       const TS value[], TD *pSrcDst,                 int len,          int scaleFactor
#define PARAM_11KS               value  ,     pSrcDst,                     len,              scaleFactor
#define ARGS_11S                          TD *pSrcDst,                 int len,          int scaleFactor
#define PARAM_11S                             pSrcDst,                     len,              scaleFactor
#define ARGS_11CRS            TS value  , TD *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor
#define PARAM_11CRS              value  ,     pSrcDst,     srcDstStep,          roiSize,     scaleFactor
#define ARGS_11RS                         TD *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor
#define PARAM_11RS                            pSrcDst,     srcDstStep,          roiSize,     scaleFactor
#define ARGS_11KRS      const TS value[], TD *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor
#define PARAM_11KRS              value  ,     pSrcDst,     srcDstStep,          roiSize,     scaleFactor

#define FN_11C   FwStatus (*fn_11C  )( ARGS_11C   );
#define FN_11R   FwStatus (*fn_11R  )( ARGS_11R   );
#define FN_11CR  FwStatus (*fn_11CR )( ARGS_11CR  );
#define FN_11CS  FwStatus (*fn_11CS )( ARGS_11CS  );
#define FN_11KR  FwStatus (*fn_11KR )( ARGS_11KR  );
#define FN_11RS  FwStatus (*fn_11RS )( ARGS_11RS  );
#define FN_11CRS FwStatus (*fn_11CRS)( ARGS_11CRS );
#define FN_11KRS FwStatus (*fn_11KRS)( ARGS_11KRS );

// 21
#define ARGS_21         const TS *pSrc,                                TD *pDst, int len
#define PARAM_21                  pSrc,                                    pDst,     len
#define ARGS_21C        const TS *pSrc,                    TS value  , TD *pDst, int len
#define PARAM_21C                 pSrc,                       value  ,     pDst,     len 
#define ARGS_21CR       const TS *pSrc, int srcStep,       TS value  , TD *pDst, int dstStep, FwiSize roiSize
#define PARAM_21CR                pSrc,     srcStep,          value  ,     pDst,     dstStep,          roiSize
#define ARGS_21KR       const TS *pSrc, int srcStep, const TS value[], TD *pDst, int dstStep, FwiSize roiSize
#define PARAM_21KR                pSrc,     srcStep,          value  ,     pDst,     dstStep,          roiSize
#define ARGS_21R        const TS *pSrc, int srcStep,                   TD *pDst, int dstStep, FwiSize roiSize
#define PARAM_21R                 pSrc,     srcStep,                       pDst,     dstStep,          roiSize
#define ARGS_21MR       const TS *pSrc, int srcStep,                   TD *pDst, int dstStep, FwiSize roiSize
#define PARAM_21MR                pSrc,     srcStep,                       pDst,     dstStep,          roiSize

#define ARGS_21S        const TS *pSrc,                                TD *pDst, int len    ,                   int scaleFactor
#define PARAM_21S                 pSrc,                                    pDst,     len    ,                       scaleFactor
#define ARGS_21CS       const TS *pSrc,                    TS value  , TD *pDst, int len    ,                   int scaleFactor
#define PARAM_21CS                pSrc,                       value  ,     pDst,     len    ,                       scaleFactor
#define ARGS_21KS       const TS *pSrc,              const TS value[], TD *pDst, int len    ,                   int scaleFactor
#define PARAM_21KS                pSrc,                       value  ,     pDst,     len    ,                       scaleFactor
#define ARGS_21CRS      const TS *pSrc, int srcStep,       TS value  , TD *pDst, int dstStep, FwiSize roiSize, int scaleFactor
#define PARAM_21CRS               pSrc,     srcStep,          value  ,     pDst,     dstStep,          roiSize,     scaleFactor
#define ARGS_21KRS      const TS *pSrc, int srcStep, const TS value[], TD *pDst, int dstStep, FwiSize roiSize, int scaleFactor
#define PARAM_21KRS               pSrc,     srcStep,          value  ,     pDst,     dstStep,          roiSize,     scaleFactor
#define ARGS_21RS       const TS *pSrc, int srcStep,                   TD *pDst, int dstStep, FwiSize roiSize, int scaleFactor
#define PARAM_21RS                pSrc,     srcStep,                       pDst,     dstStep,          roiSize,     scaleFactor

// For Color Model Conversion
#define ARGS_21P        const TS *pSrc,              TD *pDst, int len
#define PARAM_21P                 pSrc,                  pDst,     len
#define ARGS_21RP       const TS *pSrc, int srcStep, TD *pDst, int dstStep, FwiSize roiSize
#define PARAM_21RP                pSrc,     srcStep,     pDst,     dstStep,          roiSize
#define ARGS_21SP       const TS *pSrc,              TD *pDst, int len    ,  int scaleFactor
#define PARAM_21SP                pSrc,                  pDst,     len    ,      scaleFactor

// Scale
#define ARGS_21_X1      const TS * pSrc, int srcStep, TD * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint 
#define PARAM_21_X1                pSrc,     srcStep,      pDst,     dstStep,          roiSize,                  hint


#define FN_21RP  FwStatus (*fn_21RP )( ARGS_21RP  );
#define FN_21R   FwStatus (*fn_21R  )( ARGS_21R   );
#define FN_21C   FwStatus (*fn_21C  )( ARGS_21C   );
#define FN_21CS  FwStatus (*fn_21CS )( ARGS_21CS  );
#define FN_21CR  FwStatus (*fn_21CR )( ARGS_21CR  );
#define FN_21KR  FwStatus (*fn_21KR )( ARGS_21KR  );
#define FN_21RS  FwStatus (*fn_21RS )( ARGS_21RS  );
#define FN_21CRS FwStatus (*fn_21CRS)( ARGS_21CRS );
#define FN_21KRS FwStatus (*fn_21KRS)( ARGS_21KRS );
#define FN_21_X1 FwStatus (*fn_21_X1)( ARGS_21_X1 );

// 22
#define ARGS_22         const TS *pSrc,              TD *pSrcDst, int len
#define PARAM_22                  pSrc,                  pSrcDst,     len
#define ARGS_22R        const TS *pSrc, int srcStep, TD *pSrcDst, int srcDstStep, FwiSize roiSize
#define PARAM_22R                 pSrc,     srcStep,     pSrcDst,     srcDstStep,          roiSize
#define ARGS_22S        const TS *pSrc,              TD *pSrcDst, int len       ,                   int scaleFactor
#define PARAM_22S                 pSrc,                  pSrcDst,     len       ,                       scaleFactor
#define ARGS_22RS       const TS *pSrc, int srcStep, TD *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor
#define PARAM_22RS                pSrc,     srcStep,     pSrcDst,     srcDstStep,          roiSize,     scaleFactor
#define ARGS_22A        const TS *pSrc,              TD *pSrcDst, int len       ,                   Fw32f alpha
#define PARAM_22A                 pSrc,                  pSrcDst,     len       ,                          alpha
#define ARGS_22RA       const TS *pSrc, int srcStep, TD *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha
#define PARAM_22RA                pSrc,     srcStep,     pSrcDst,     srcDstStep,          roiSize,        alpha

#define FN_22R  FwStatus (*fn_22R )( ARGS_22R  );
#define FN_22RS FwStatus (*fn_22RS)( ARGS_22RS );
#define FN_22RA FwStatus (*fn_22RA)( ARGS_22RA );

// 32
#define ARGS_32         const TS *pSrc1,               const TS *pSrc2,               TD *pDst,              int len
#define PARAM_32                  pSrc1,                         pSrc2,                   pDst,                  len
#define ARGS_32R        const TS *pSrc1, int src1Step, const TS *pSrc2, int src2Step, TD *pDst, int dstStep, FwiSize roiSize
#define PARAM_32R                 pSrc1,     src1Step,           pSrc2,     src2Step,     pDst,     dstStep,          roiSize
#define ARGS_32S        const TS *pSrc1,               const TS *pSrc2,               TD *pDst,              int len,          int scaleFactor
#define PARAM_32S                 pSrc1,                         pSrc2,                   pDst,                  len,              scaleFactor
#define ARGS_32RS       const TS *pSrc1, int src1Step, const TS *pSrc2, int src2Step, TD *pDst, int dstStep, FwiSize roiSize, int scaleFactor
#define PARAM_32RS                pSrc1,     src1Step,           pSrc2,     src2Step,     pDst,     dstStep,          roiSize,     scaleFactor


#define FN_32   FwStatus (*fn_32  )( ARGS_32   );
#define FN_32R  FwStatus (*fn_32R )( ARGS_32R  );
#define FN_32RS FwStatus (*fn_32RS)( ARGS_32RS );

// 33
#define ARGS_33R        const TS *pSrc1, int src1Step, const TS *pSrc2, int src2Step,    TD *pSrcDst, int srcDstStep, FwiSize roiSize
#define PARAM_33R                 pSrc1,     src1Step,           pSrc2,     src2Step,        pSrcDst,     srcDstStep,          roiSize
#define ARGS_33         const TS *pSrc1,               const TS *pSrc2,                  TD *pSrcDst, int len
#define PARAM_33                  pSrc1,                         pSrc2,                      pSrcDst,     len

#define ARGS_33MR       const TS *pSrc , int srcStep , const Fw8u *pMask, int maskStep, TD *pSrcDst, int srcDstStep, FwiSize roiSize
#define PARAM_33MR                pSrc ,     srcStep ,              pMask,     maskStep,     pSrcDst,     srcDstStep,          roiSize
#define ARGS_33M        const TS *pSrc ,               const Fw8u *pMask,               TD *pSrcDst, int len
#define PARAM_33M                 pSrc ,                            pMask,                   pSrcDst,     len
#define ARGS_33MA       const TS *pSrc ,               const Fw8u *pMask,               TD *pSrcDst, int len,                           Fw32f alpha
#define PARAM_33MA                pSrc ,                            pMask,                   pSrcDst,     len,                                  alpha
#define ARGS_33MRA      const TS *pSrc ,   int srcStep,const Fw8u *pMask, int maskStep, TD *pSrcDst, int srcDstStep, FwiSize roiSize,  Fw32f alpha
#define PARAM_33MRA               pSrc ,       srcStep,             pMask,     maskStep,     pSrcDst,     srcDstStep,          roiSize,         alpha

#define FN_33R   FwStatus (*fn_33R  )( ARGS_33R   );
#define FN_33MR  FwStatus (*fn_33MR )( ARGS_33MR  );
#define FN_33MRA FwStatus (*fn_33MRA)( ARGS_33MRA );

// 44
#define ARGS_44MR       const TS *pSrc1, int src1Step, const TS *pSrc2, int src2Step,  const Fw8u *pMask, int maskStep, TD *pSrcDst, int srcDstStep, FwiSize roiSize 
#define PARAM_44MR                pSrc1,     src1Step,           pSrc2,     src2Step,               pMask,     maskStep,     pSrcDst,     srcDstStep,          roiSize 
#define ARGS_44M        const TS *pSrc1,               const TS *pSrc2,                const Fw8u *pMask,               TD *pSrcDst, int len
#define PARAM_44M                 pSrc1,                         pSrc2,                             pMask,                   pSrcDst,     len

#define FN_44MR FwStatus (*fn_44MR)( ARGS_44MR );

#endif //__FNTYPES_H__
