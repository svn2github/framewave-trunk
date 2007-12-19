/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FILTER_H__
#define __FILTER_H__

/**************************************************************************************************
*** FILTER FUNCTIONS DESIGN OVERVIEW ***

All filter functions operate on a kernel.  Most commonly kernel sizes are 3x3 and 5x5 but many
kernel functions support other sizes.  Some filter functions support arbitrary size kernels.

The fact that a filter function operates on a kernel of size MxN means that for each destination
it needs to use MxN corresponding source pixels as input.  Because of this general requirement 
the filter functions could not easily use the foreach framework, and instead use their own.

Each filter function is implemented in terms of three fundamental class templates:
1. divide-and-conquer class - divides the work between multiple worker threads and selects 
   appropriate execution path (REF, SSE2, etc.)
2. processor class - process the image row by row, column by column, loads the data and passes 
   it to the operator class for processing
3. operator class - performs the calculations specific to each filter function.


The DivideAndConquer class template implements one static function: Run()
There are many implementations of the DivideAndConquer class.  The main difference between them 
is the type and number of arguments used by the Run() function.
The name of the class usually indicates the parameter types and number used.

DivideAndConquer_pSrc_sStep_pDst_dStep_roi
DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor
DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor
DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor
DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor
DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor
DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor
DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customhorizontalkernel_size_anchor_divisor
DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customverticalkernel_size_anchor_divisor
DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
DivideAndConquer_pSrc_sStep_ppDst_roi_kernel32f_size_anchor_borderType_borderValue_buffer
DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
DivideAndConquer_ppSrc_pDst_dStep_roi_kernel32f_size_anchor_border_borderValue_buffer
There are also some special variants: 
DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor - copies the source data to a 
    separate buffer and then call the corresponding out-of-place variant of the DivideAndConquer class.
    Used by the FilterBox in place variant.
DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor - allocates a buffer used for temporary 
    calculations.  Used by the FilterSumWindowRow and FilterSumWindowColumn functions.
DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer - replicates the border.  Used
    by the Filter[Min/Max]BorderReplicate functions.
DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer - performs one of several types of border 
    replication as indicated by the caller.  Used by some fixed filter functions (ex. FilterScharr, 
    FilterSobel.)
DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep - take two images as inputs.

The DivideAndConquer class template takes several template parameters, most of these parameters are other 
tempalte classes.  For example one of the parameters will be the processor class to use, and the operator 
class to be used by the operator class.
The DivideAndConquer::Run function needs to pass most/all parameters passed by the caller to the processor 
class (and later operator class).  Since the initial parameters vary depending on the function, they will
be passed by pointer to a special DataType struct (all parameters are members of this struct)  The DataType 
is also a template parameter to the DivideAndConquer class template.
All DivideAndConquer classes also take template parameters that indicate the source and destination data type 
(ex. U8, S16, F32, etc.), intermediate calculations data type as well as the number of channels in the 
source/destination images.

The Processor class typically provides several functions, one for each code path supported, ex.:
void REFR( void *p ) - reference implementation.  p is a pointer to the appropriate DataType object.
void SSE2( void *p ) - SSE2 implementation.  p is a pointer to the appropriate DataType object.
void SSE2_Unaligned( void *p ) - SSE2 implementation.  Source data is unaligned.  p is a pointer to the 
                                 appropriate DataType object.

The DivideAndConquer::Run function will automatically call one of these functions based on the hardware 
architecture and data aligment.
The REFR function will call the Operator::Calculate function for each pixel of the destination image, 
typically in the row-by-row, column-by-column order (left to right, top to bottom).  Some variants of the 
processor class will iterate over destination pixels in a different order, ex. "zig-zag" (left to right, 
one down, right to left, one down, etc.) to better optimized performance for a specific algorithm.

The default implementations for SSE2 and SSE2_Unaligned methods simply call the REFR function.  It is 
assumed that the SSE2 and further optimized implementations will derive from the specific Processor/Operator
class and override the SSE2, SSE2_Unaligned, etc. methods.  The main reason for this assumption is that SSE
optimzied functions do not usually process the data on pixel-by-pixel basis but instead load data and process 
it for multiple pixels at the same time in parallel.  They typically have their own, implementation specific
code to iterate over image rows and columns.

Notes about the data alignment issues:  
The Processor::SSE2 function is expected to handle misaligned data by
calling the reference code for the misaligned pixels at the beginning and end of each row of data.  However, 
since a kernel typically consists of multiple rows of data, it is possible that while data in one row is aligned, 
the data in another row might be misaligned.  In such cases it will be impossible to have all source data fully 
aligned for any destination pixel in the image (this typically occurs if the source image srcStep is not a 
multiple of 16).  Another possibility is if the source data type is larger than byte (ex. single precision 
floating point - 4 bytes), and the first value is not aligned according to the data type size (ex. not 4 
bytes aligned).  In such scenario the source data will always be misaligned.  The DivideAndConquer function tests 
for such conditions and if necessary will call unaligned function ex. SSE2_Unaligned() instead of SSE2().

Just like the DivideAndConquer class template, the processor template also takes several template arguments that 
include the source and destination data type, intermediate calculations data type, number of chnnels, 
operator class, DataType class, etc.  The name of the processor template class usually indicates the parameters used.

It also needs to be noted that the processor classes often have specialized version for 4 channel alpha case.  The 
number of channels passed as template argument is an integer in the 1-5 range.  1 - single channel, 2 - unusued, 
3 - three channels, 4 - four channels, 5 - four channels with alpha (4th channel destination data is unmodified).

TProcessor_pSrc_sStep_pDst_dStep_roi
TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor
TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor 
TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor 
TProcessorWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor 
TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor 
TProcessorAllChannelsWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor 
TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer
TProcessor_ZigZagRDRD 
TProcessor_ZigZagDRDR 
TProcessor_ZigZagRDLD 
TProcessor_ZigZagDRUR 
TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor 
TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep 
TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor 
TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer 

Operator class typically provides just a single function called Calculate used by the reference path.
The function is specific to each filter.  Ex. the FilterBox operator class template has a Calculate 
function that calculates the average ofvalue of all the values within the kernel around the specific
point.
Some operator classes provide more than one function, ex. the operator classes used by the ZigZag 
type processor class templates will also have multiple Calculate functions called CalculateRight, 
CalcualteLeft, CalculateDown, CalculateUp that will be used dependning on the "zig-zag phase" of the 
algorithm (ex. if the code is iterating pixels left to right, it will call CalculateRight, if it is 
iterating pixels top to bottom, it will call CalculateDown).
The operator class template also takes several template parameters that include source, destination 
and intermediate data types and the number of channels (the class is often specialized for the 4 channel alpha 
scenario).

**************************************************************************************************/

namespace OPT_LEVEL
{
template< class T >
static U32 BytesToAlign( const T * a, size_t alignment )	// Number of bytes needed to process before a becomes aligned
{
	size_t addr = (size_t) a;
	return (U32)(alignment - (addr % alignment));
}

namespace FilterCommon
{
    // basic data structure containing source and destination buffer pointers, step size and roisize.
    template< typename SrcDataType, typename DstDataType >
    struct Data
    {
        const SrcDataType *pSrc;
        int                srcStep;
        DstDataType       *pDst;
        int                dstStep;
        FwiSize           dstRoiSize;
    };

    // this data structure also contains 2D mask and kernel information
    template< typename SrcDataType, typename DstDataType >
    struct DataWithMask : public Data< SrcDataType, DstDataType >
    {
        typedef FwiSize MaskType;
        typedef FwiPoint AnchorType;
        MaskType    maskSize;
        AnchorType  anchor;
    };

    // this data structure contains 1D mask and kernel information.
    // this will be used by functions where the kernel is a single row or column.
    template< typename SrcDataType, typename DstDataType >
    struct DataWithLinearMask : public Data< SrcDataType, DstDataType >
    {
        typedef int MaskType;
        typedef int AnchorType;
        MaskType    maskSize;
        AnchorType  anchor;
    };

    // this data structure also contains a pointer to buffer with kernel specific data 
    // and a divisor used by some functions
    template< typename SrcDataType, typename DstDataType >
    struct DataWithKernel : public Data< SrcDataType, DstDataType >
    {
        typedef FwiSize KernelType;
        typedef FwiPoint AnchorType;
        void*       pKernel;
        KernelType  kernelSize;
        AnchorType  anchor;
        int         divisor;
    };

    // this data structure also contains a pointer to buffer with kernel specific data 
    // and a divisor used by some functions, as well as additional buffer pointer that 
    // will be used for temporary calculations by some functions
    template< typename SrcDataType, typename DstDataType >
    struct DataWithKernelAndBuffer : public DataWithKernel< SrcDataType, DstDataType >
    {
        void *pBufferObj;
    };

    // this data structure also contains a pointer to buffer with kernel specific data 
    // and a divisor used by some functions.  It assumes that the kernel is a single row or column.
    template< typename SrcDataType, typename DstDataType >
    struct DataWithLinearKernel : public Data< SrcDataType, DstDataType >
    {
        typedef int KernelType;
        typedef int AnchorType;
        void*       pKernel;
        KernelType  kernelSize;
        AnchorType  anchor;
        int         divisor;
    };

    // this data structure also contains a pointer to buffer with kernel specific data 
    // and a divisor used by some functions.  It assumes that the kernel is a single row or column.
    // It also has additional buffer pointer that will be used for temporary calculations 
    // by some functions
    template< typename SrcDataType, typename DstDataType >
    struct DataWithLinearKernelAndBuffer : public DataWithLinearKernel< SrcDataType, DstDataType >
    {
        void *pBufferObj;
    };

    // this data structure contains mask size and anchor information, and a pointer to a buffer
    // for temporary calculations
    template< typename SrcDataType, typename DstDataType >
    struct DataWithBuffer : public DataWithMask< SrcDataType, DstDataType >
    {
        void *pBufferObj;
    };

    // this data structure contains mask size and anchor information, and a pointer to a buffer
    // for temporary calculations.  It assumes that the mask is a single row or column
    template< typename SrcDataType, typename DstDataType >
    struct DataWithLinearMaskAndBuffer : public DataWithLinearMask< SrcDataType, DstDataType >
    {
        void *pBufferObj;
    };

    // this function tests if the given mask is valid (non-negative)
    template< typename MaskType >
    SYS_INLINE bool IsValidMask( const MaskType & maskSize )
    {
        if( maskSize <= 0 )
            return false;

        return true;
    }

    // this function tests if the given mask is valid (non-negative).
    // this is a 2D specialized version of the function above.
    template<>
    SYS_INLINE bool IsValidMask( const FwiSize & maskSize )
    {
        if (maskSize.height <= 0 || maskSize.width <= 0 )
            return false;

        return true;
    }

    // this function tests if the given anchor is valid (non-negative)
    template< typename MaskType, typename AnchorType >
    SYS_INLINE bool IsValidAnchor( const MaskType & maskSize, const AnchorType & anchor )
    {
        if( anchor < 0 || anchor >= maskSize )
            return false;

        return true;
    }

    // this function tests if the given anchor is valid (non-negative).
    // this is a 2D specialized version of the function above.
    template<>
    SYS_INLINE bool IsValidAnchor( const FwiSize & maskSize, const FwiPoint & anchor )
    {
        if( anchor.x < 0 || anchor.x >= maskSize.width ||
            anchor.y < 0 || anchor.y >= maskSize.height )
            return false;

        return true;
    }
}

} // namespace OPT_LEVEL


#include "BordersReplication.h"

namespace OPT_LEVEL
{
namespace FilterCommon
{
    ////////////////////////////////////////////////////////////////////////////////////
    // the class template below has function Run() that will divide the work between the 
    // available worker and user threads and will call reference or SSEx optimized code 
    // depending on system capabilites
    ////////////////////////////////////////////////////////////////////////////////////

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class Processor,
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType
    >
    struct DivideAndConquer_pSrc_sStep_pDst_dStep_roi
    {
        typedef DataType< SrcType, DstType > DataClass;

        // this is the main function that will start worker threads and call the appropriate code path
        // based on the hardware capabilities
        static FwStatus Run( const SrcType * pSrc, int srcStep, DstType * pDst, int dstStep, FwiSize dstRoiSize )
        {
            // first validate the input parameters

            if( pSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;
                                        
            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            // based on the hardware capabilities we will call SSE2 or reference implementation
            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    // break the workload between the worker threads
                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    // each worker thread will process a different part of the data
                    // setup data structures for each thread and queue the jobs
                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) || ( srcStep & 0x0F ) ) // is src data aligned or the src row step size 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcStep );
                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                    }

                    // wait for worker thread to complete
                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pSrc;
                    data.srcStep = srcStep;
                    data.pDst = pDst;
                    data.dstStep = dstStep;
                    data.dstRoiSize = dstRoiSize;

                    // call the reference implementation of the function
                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class Processor,
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType
    >
    struct DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::MaskType MaskType;
        typedef typename DataClass::AnchorType AnchorType;

        // this is the main function that will start worker threads and call the appropriate code path
        // based on the hardware capabilities
        static FwStatus Run( const SrcType * pSrc, int srcStep, DstType * pDst, int dstStep, FwiSize dstRoiSize, MaskType maskSize, AnchorType anchor )
        {
            // first validate the input parameters

            if( pSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;
                                        
            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            if( !IsValidMask( maskSize ) )
                return fwStsMaskSizeErr;

            if( !IsValidAnchor( maskSize, anchor ) )
                return fwStsAnchorErr;

            // based on the hardware capabilities we will call SSE2 or reference implementation
            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    // each worker thread will process a different part of the data
                    // setup data structures for each thread and queue the jobs
                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].maskSize            = maskSize;
                        data[i].anchor	            = anchor;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) || ( srcStep & 0x0F ) ) // is src data aligned or the src row step size 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcStep );
                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                    }

                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pSrc;
                    data.srcStep = srcStep;
                    data.pDst = pDst;
                    data.dstStep = dstStep;
                    data.dstRoiSize = dstRoiSize;
                    data.maskSize = maskSize;
                    data.anchor	= anchor;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class Processor,
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType
    >
    struct DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::KernelType KernelType;
        typedef typename DataClass::AnchorType AnchorType;

        // this is the main function that will start worker threads and call the appropriate code path
        // based on the hardware capabilities
        static FwStatus Run( const SrcType * pSrc, int srcStep, 
                                           DstType * pDst, int dstStep, 
                                     FwiSize dstRoiSize, const Fw32s * pKernel,
                                     KernelType kernelSize, AnchorType anchor,
                                     int divisor )
        {
            // first validate the input parameters

            if( pSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;
                                        
            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            if( !IsValidMask( kernelSize ) )
                return fwStsMaskSizeErr;

            if( !IsValidAnchor( kernelSize, anchor ) )
                return fwStsAnchorErr;

            if( divisor == 0 )
                return fwStsDivisorErr;

            // based on the hardware capabilities we will call SSE2 or reference implementation
            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    // each worker thread will process a different part of the data
                    // setup data structures for each thread and queue the jobs
                    for( U32 i=0; i<tCount; ++i )
                    {
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].pKernel             = (void*)pKernel;
                        data[i].kernelSize          = kernelSize;
                        data[i].anchor	            = anchor;
                        data[i].divisor	            = divisor;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) || ( srcStep & 0x0F ) ) // is src data aligned or the src row step size 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcStep );
                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                    }

                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pSrc;
                    data.srcStep = srcStep;
                    data.pDst = pDst;
                    data.dstStep = dstStep;
                    data.dstRoiSize = dstRoiSize;
                    data.pKernel = (void*)pKernel;
                    data.kernelSize = kernelSize;
                    data.anchor = anchor;
                    data.divisor = divisor;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class Processor,
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType
    >
    struct DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::KernelType KernelType;
        typedef typename DataClass::AnchorType AnchorType;

        // this is the main function that will start worker threads and call the appropriate code path
        // based on the hardware capabilities
        static FwStatus Run( const SrcType * pSrc, int srcStep, 
                                           DstType * pDst, int dstStep, 
                                     FwiSize dstRoiSize, const Fw32s * pKernel,
                                     KernelType kernelSize, AnchorType anchor,
                                     int divisor )
        {
            // first validate the input parameters

            if( pSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;
                                        
            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            if( !IsValidMask( kernelSize ) )
                return fwStsMaskSizeErr;

            if( !IsValidAnchor( kernelSize, anchor ) )
                return fwStsAnchorErr;

            if( divisor == 0 )
                return fwStsDivisorErr;

            // based on the hardware capabilities we will call SSE2 or reference implementation
            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    // each worker thread will process a different part of the data
                    // setup data structures for each thread and queue the jobs
                    for( U32 i=0; i<tCount; ++i )
                    {
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].pKernel             = (void*)pKernel;
                        data[i].kernelSize          = kernelSize;
                        data[i].anchor	            = anchor;
                        data[i].divisor	            = divisor;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) ) // is src data 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcStep );
                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                    }

                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pSrc;
                    data.srcStep = srcStep;
                    data.pDst = pDst;
                    data.dstStep = dstStep;
                    data.dstRoiSize = dstRoiSize;
                    data.pKernel = (void*)pKernel;
                    data.kernelSize = kernelSize;
                    data.anchor = anchor;
                    data.divisor = divisor;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class Processor,
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType
    >
    struct DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::KernelType KernelType;
        typedef typename DataClass::AnchorType AnchorType;

        // this is the main function that will start worker threads and call the appropriate code path
        // based on the hardware capabilities
        static FwStatus Run( const SrcType * pSrc, int srcStep, 
                                           DstType * pDst, int dstStep, 
                                     FwiSize dstRoiSize, const Fw32f * pKernel,
                                     KernelType kernelSize, AnchorType anchor )
        {
            // first validate the input parameters

            if( pSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;
                                        
            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            if( !IsValidMask( kernelSize ) )
                return fwStsMaskSizeErr;

            if( !IsValidAnchor( kernelSize, anchor ) )
                return fwStsAnchorErr;

            // based on the hardware capabilities we will call SSE2 or reference implementation
            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    // each worker thread will process a different part of the data
                    // setup data structures for each thread and queue the jobs
                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].pKernel             = (void*)pKernel;
                        data[i].kernelSize          = kernelSize;
                        data[i].anchor	            = anchor;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) || ( srcStep & 0x0F ) ) // is src data aligned or the src row step size 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcStep );
                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                    }

                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pSrc;
                    data.srcStep = srcStep;
                    data.pDst = pDst;
                    data.dstStep = dstStep;
                    data.dstRoiSize = dstRoiSize;
                    data.pKernel = (void*)pKernel;
                    data.kernelSize = kernelSize;
                    data.anchor = anchor;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    // Some FW Filter functions are operating on in-place data (i.e. they write the results directly into the source buffer).
    // The image filter functions that we implemented so far assume that the source and destination are two separate buffers.
    // In fact, there is no easy way to perform the calculations on an InOut buffer since the filter functions have to deal with 
    // neighbourhood pixels.
    // The ROICopier class below implements code to create a copy of the a buffer ROI region.
    // Functions that operate on in-place data can create a new, temporary destination buffer,
    // perform all computations storing the result into the temporary buffer, and then use the 
    // ROICopier::Copy function to copy the result from the temporary destination buffer into the 
    // SrcDst buffer.
    template< CH chCount, typename SrcDstType >
    struct ROICopier
    {
        static SYS_INLINE void Copy( SrcDstType * pDst, int dstStep, const SrcDstType * pSrc, int srcStep, FwiSize dstRoiSize )
        {
            SrcDstType *pDstEnd = (SrcDstType*)(((Fw8u*)pDst) + dstStep * dstRoiSize.height );
            while( pDst < pDstEnd )
            {
                memcpy( pDst, pSrc, dstRoiSize.width * chCount * sizeof(SrcDstType) );
                pDst = (SrcDstType*)(((Fw8u*)pDst) + dstStep );
                pSrc = (const SrcDstType*)(((const Fw8u*)pSrc) + srcStep );
            }
        }
    };

    // This is the 4 channel with Alpha specialized version of the class/function above.
    template< typename SrcDstType >
    struct ROICopier< AC4, SrcDstType >
    {
        static SYS_INLINE void Copy( SrcDstType * pDst, int dstStep, const SrcDstType * pSrc, int srcStep, FwiSize dstRoiSize )
        {
            SrcDstType *pDstEnd = (SrcDstType*)(((Fw8u*)pDst) + dstStep * dstRoiSize.height );
            while( pDst < pDstEnd )
            {
                const SrcDstType *pSource = pSrc;
                SrcDstType *pElement = pDst;
                SrcDstType *pLastElement = pDst + dstRoiSize.width * C4;
                while( pElement < pLastElement )
                {
                    *(pElement+0) = *(pSource+0);
                    *(pElement+1) = *(pSource+1);
                    *(pElement+2) = *(pSource+2);
                    pElement += C4;
                    pSource += C4;
                }
                pDst = (SrcDstType*)(((Fw8u*)pDst) + dstStep );
                pSrc = (const SrcDstType*)(((Fw8u*)pSrc) + srcStep );
            }
        }
    };

    // the DivideAndConquer class tempalte below will allocate a temporary buffer for destination data
    // call the out-of-place variant of the function passing the temporary buffer as the destination,
    // and copy the destination data back into the srcdst memory location.
    template< CH chCount, 
              typename SrcDstType, 
              typename CalcType, 
              template< CH _chCount_, 
                        typename _SrcType_,
                        typename _DstType_, 
                        typename _CalcType_, 
                        template< CH, typename, typename, typename > class _Operator_,
                        template< typename, typename > class _DataType_
                      > class Processor,
              template< CH, 
                        typename _SrcType_,
                        typename _DstType_, 
                        typename _CalcType_ 
                      > class Operator,
              template< typename _SrcType_, typename _DstType_ > class DataType
    >
    struct DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor : 
        DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< chCount, SrcDstType, SrcDstType, CalcType,
                                                                Processor, Operator, DataType >
    {
        typedef DataType< SrcDstType, SrcDstType > DataClass;
        typedef typename DataClass::MaskType MaskType;
        typedef typename DataClass::AnchorType AnchorType;

        // this is the main function that will create the temporary destination location, and then 
        // indirectly start worker threads and call the appropriate code path based on the 
        // hardware capabilities
        static FwStatus Run( SrcDstType * pSrcDst, int srcDstStep, FwiSize dstRoiSize, 
                                MaskType maskSize, AnchorType anchor )
        {
            // first validate the input parameters

            if( pSrcDst == NULL )
                return fwStsNullPtrErr;   
            if (dstRoiSize.height <= 0)
                return fwStsSizeErr;
            if (srcDstStep <= 0)	
                return fwStsStepErr;

            // allocate the temporary destination buffer
            SrcDstType * pDst = (SrcDstType * )malloc( dstRoiSize.height * srcDstStep );
            if( pDst == NULL )
                return fwStsMemAllocErr;

            // call the out-of-place version of the function
            FwStatus status = FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor
                < chCount, SrcDstType, SrcDstType, CalcType, Processor, Operator, DataType >
            ::Run( pSrcDst, srcDstStep, pDst, srcDstStep, dstRoiSize, maskSize, anchor );

            // copy the result back to the pSrcDst location
            ROICopier<chCount, SrcDstType>::Copy( pSrcDst, srcDstStep, pDst, srcDstStep, dstRoiSize );
            free( pDst );
            return status;
        } // Run
    };

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class Processor,
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType
    >
    struct DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::MaskType MaskType;
        typedef typename DataClass::AnchorType AnchorType;

        // this is the main function that will start worker threads and call the appropriate code path
        // based on the hardware capabilities
        static FwStatus Run( const SrcType * pSrc, int srcStep, DstType * pDst, int dstStep, FwiSize dstRoiSize, MaskType maskSize, AnchorType anchor )
        {
            // first validate the input parameters

            if( pSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;
                                        
            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            if( !IsValidMask( maskSize ) )
                return fwStsMaskSizeErr;

            if( !IsValidAnchor( maskSize, anchor ) )
                return fwStsAnchorErr;

            // based on the hardware capabilities we will call SSE2 or reference implementation
            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    int nTempBufferLength = Operator< chCount, SrcType, DstType, CalcType >::GetMinimumBufferSize( maskSize );
                    Fw8u * pTempBuffer = (Fw8u*)malloc( nTempBufferLength * tCount );
                    if( pTempBuffer == NULL )
                        return fwStsMemAllocErr;

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    // each worker thread will process a different part of the data
                    // setup data structures for each thread and queue the jobs
                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].maskSize            = maskSize;
                        data[i].anchor	            = anchor;
                        data[i].pBufferObj          = pTempBuffer + i * nTempBufferLength;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) || ( srcStep & 0x0F ) ) // is src data aligned or the src row step size 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcStep );
                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                    }

                    ThreadPool::Wait();
                    free( pTempBuffer );
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    int nTempBufferLength = Operator< chCount, SrcType, DstType, CalcType >::GetMinimumBufferSize( maskSize );
                    Fw8u * pTempBuffer = (Fw8u*)malloc( nTempBufferLength );
                    if( pTempBuffer == NULL )
                        return fwStsMemAllocErr;

                    DataClass data;
                    data.pSrc = pSrc;
                    data.srcStep = srcStep;
                    data.pDst = pDst;
                    data.dstStep = dstStep;
                    data.dstRoiSize = dstRoiSize;
                    data.maskSize = maskSize;
                    data.anchor	= anchor;
                    data.pBufferObj = pTempBuffer;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );

                    free( pTempBuffer );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH _chCount_, 
                        typename _SrcType_, 
                        typename _DstType_, 
                        typename _CalcType_, 
                        template< CH, 
                                  typename _SrcType_, 
                                  typename _DstType_, 
                                  typename _CalcType_,
                                  template< CH _chCount_,
                                            typename _SrcType_ > class _BorderType_
                                > class _Operator_,
                        template< typename, typename > class _DataType_,
                        template< CH _chCount_,
                                  typename _SrcType_ > class _BorderType_
                      > class Processor,
              template< CH _chCount_, 
                        typename _SrcType_, 
                        typename _DstType_, 
                        typename _CalcType_,
                        template< CH _chCount_,
                                  typename _SrcType_ > class _BorderType_
                      > class Operator,
              template< typename _SrcType_,
                        typename _DstType_
                      > class DataType,
              template< CH _chCount_, 
                        typename _SrcType_
                      > class BorderType
    >
    struct DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::MaskType MaskType;
        typedef typename DataClass::AnchorType AnchorType;

        // this is the main function that will start worker threads and call the appropriate code path
        // based on the hardware capabilities
        static FwStatus Run( const SrcType * pSrc, int srcStep, DstType * pDst, int dstStep, FwiSize dstRoiSize, 
                                MaskType maskSize, AnchorType anchor, Fw8u * pBuffer )
        {
            // first validate the input parameters

            if( pSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;

            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            if( !IsValidMask( maskSize ) )
                return fwStsMaskSizeErr;

            if( !IsValidAnchor( maskSize, anchor ) )
                return fwStsAnchorErr;

            // based on the hardware capabilities we will call SSE2 or reference implementation
            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    U32 tCount = ThreadPool::ThreadCount();

                    BorderType< chCount, SrcType > bufferObjects[MAX_THREADS];
                    DataClass data[MAX_THREADS];

                    int nThreadBufferSize;
                    BorderType< chCount, SrcType >::GetBufferSize( dstRoiSize.width, maskSize, &nThreadBufferSize );

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;
                    int srcIndex = 0;

                    // each worker thread will process a different part of the data
                    // setup data structures for each thread and queue the jobs
                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;
                        data[i].maskSize            = maskSize;
                        data[i].anchor	            = anchor;
                        data[i].pBufferObj          = &bufferObjects[i];
                        bufferObjects[i].Initialize( ps, srcStep, dstRoiSize.height, srcIndex,
                                                     data[i].dstRoiSize, 
                                                     maskSize, anchor,
                                                     pBuffer + i * nThreadBufferSize );

                        srcIndex += data[i].dstRoiSize.height;
                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcStep );
                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                    }

                    if( ( ((size_t)pSrc) & (sizeof(SrcType)-1) ) || ( srcStep & 0x0F ) ) // is src data aligned or the src row step size 16 bytes aligned?
                    {
                        for( U32 t = 0; t < tCount; t++ )
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType,
                                                        BorderType >::SSE2_Unaligned, &data[t] );
                        }
                    }
                    else
                    {
                        for( U32 t = 0; t < tCount; t++ )
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType,
                                                        BorderType >::SSE2, &data[t] );
                        }
                    }

                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pSrc;
                    data.srcStep = srcStep;
                    data.pDst = pDst;
                    data.dstStep = dstStep;
                    data.dstRoiSize = dstRoiSize;
                    data.maskSize = maskSize;
                    data.anchor	= anchor;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType, BorderType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        }

        static FwStatus GetBufferSize( int roiWidth, FwiSize maskSize, int* pBufferSize )
        {
            FwStatus status = BorderType< chCount, SrcType >::GetBufferSize( roiWidth, maskSize, pBufferSize );
            if( status == fwStsNoErr )
            {
                (*pBufferSize) *= MAX_THREADS;
            }
            return status;
        }
    };

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class Processor,
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType,
              int nBorderWidth
    >
    struct DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer
    {
        typedef DataType< SrcType, DstType > DataClass;

        // this is the main function that will (indirectly) start worker threads and call the 
        // appropriate code path based on the hardware capabilities
        static FwStatus Run( const SrcType * pSrc, int srcStep, DstType * pDst, int dstStep, 
                                     FwiSize dstRoiSize, FwiBorderType borderType, SrcType borderValue[chCount], 
                                     Fw8u * pBuffer )
        {
            // validate the parameters
            if( pSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;

            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            // determine the steps size of the image (with borders) in the temporary buffer
            int newSrcStep = BordersReplication::BorderReplicator< chCount, SrcType, nBorderWidth >::AlignedStepSize( dstRoiSize.width );
            // copy the source image into the temporary buffer
            BordersReplication::ImageDuplicator< chCount, SrcType, nBorderWidth >::DuplicateImage( (SrcType *)pBuffer, newSrcStep, pSrc, srcStep, dstRoiSize );

            // create borders around the image in the temporary buffer
            SrcType * pNewSrc = NULL;
            FwStatus sts = BordersReplication::BorderReplicator< chCount, SrcType, nBorderWidth >
                ::CreateBorder( (SrcType*)pBuffer, dstRoiSize, borderType, borderValue, &pNewSrc );
            if( sts != fwStsNoErr )
                return sts;

            // process the source data in the temporary buffer
            return DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
                chCount,
                SrcType,
                DstType,
                CalcType,
                Processor,
                Operator, DataType >
            ::Run( pNewSrc, newSrcStep, pDst, dstStep, dstRoiSize );
        }
    };

    // some functions take a kernel matrix as one of the input parameters.
    // The src pixel values are then multiplied by kernel values, added together and optionally scaled.
    // Depending on the dimenstions of the kernel matrix and the values inside, it might be possible
    // to perform all these calculations using a 16/32/64/etc bit variable to store intermediate results.
    // the two functions below are used to determine if the intermediate results can be stored in 16 bits accuracy 
    // (16 bit signed integer), 24 bits accuracy (single precision floating point number), 32 bits accuracy
    // (32 bit signed integer) or higher (64-bit double precision floating point number).

    enum ArithmeticsType { arithmetics_16bit, arithmetics_24bit, arithmetics_32bit, arithmetics_64bit };

    // This function performs the analysis for source image containing 8 bit unsigned data
    SYS_INLINE ArithmeticsType KernelAnalysis8bit( const Fw32s * pKernel, int kernelSize, int divisor )
    {
        Fw64s minSum = 0;
        Fw64s maxSum = 0;

        // Assume the worst case scenario where each source pixel has a value of max or min.
        // The loop below we calculate the total sum of products of all pixels and kernel values.
        // We calculate one total sum for the scenario where all pixels are min, and one for the 
        // scenario where all pixels are positive
        for( int n = 0; n < kernelSize; n++ )
        {
            Fw32s val = pKernel[n];

            if( val < 0 )
                minSum = minSum - val;
            else
                maxSum = maxSum + val;
        }

        // depending on the total sum value, we now decide what data type is needed to perform the 
        // intermediate calculations
        if( ( minSum * 255 ) <= ( 1 << 15 ) && ( maxSum * 255 ) < ( 1 << 15 ) )
        {
            if( -32768 <= divisor && divisor < 32768 )
            {
                // we can use 16 bit arithmetics
                return arithmetics_16bit;
            }
            else
            {
                // we can use 24 bit arithmetics
                return arithmetics_24bit;
            }
        }
        else if( ( minSum * 255 ) <= ( 1 << 23 ) && ( maxSum * 255 ) < ( 1 << 23 ) )
        {
            // we can use 24 bit arithmetics (ex. single precision floating point numbers)
            return arithmetics_24bit;
        }
        else if( ( minSum * 255 ) <= ( 1 << 31 ) && ( maxSum * 255 ) < ( 1 << 31 ) )
        {
            // we can use 32 bit arithmetics
            return arithmetics_32bit;
        }
        else
        {
            // we have to use 64 bit arithmetics
            return arithmetics_64bit;
        }
    }

    // This function performs the analysis for source image containing 16 bit signed data
    SYS_INLINE ArithmeticsType KernelAnalysis16bit( const Fw32s * pKernel, int kernelSize, int /*divisor*/ )
    {
        Fw64s minSum = 0;
        Fw64s maxSum = 0;

        // Assume the worst case scenario where each source pixel has a value of max or min.
        // The loop below we calculate the total sum of products of all pixels and kernel values.
        // We calculate one total sum for the scenario where all pixels are min, and one for the 
        // scenario where all pixels are positive
        for( int n = 0; n < kernelSize; n++ )
        {
            Fw32s val = pKernel[n];

            if( val < 0 )
                minSum = minSum - val;
            else
                maxSum = maxSum + val;
        }

        // depending on the total sum value, we now decide what data type is needed to perform the 
        // intermediate calculations
        if( ( minSum * 32768 ) <= ( 1 << 23 ) && ( maxSum * 32768 ) < ( 1 << 23 ) )
        {
            // we can use 24 bit arithmetics (ex. single precision floating point numbers)
            return arithmetics_24bit;
        }
        else if( ( minSum * 32768 ) <= ( 1 << 31 ) && ( maxSum * 32768 ) < ( 1 << 31 ) )
        {
            // we can use 32 bit arithmetics
            return arithmetics_32bit;
        }
        else
        {
            // we have to use 64 bit arithmetics
            return arithmetics_64bit;
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace FiltersCommon

#include "Filter_ProcessorTemplates.h"

} // namespace OPT_LEVEL

#endif // __FILTER_H__
