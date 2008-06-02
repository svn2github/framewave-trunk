/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "UnitTestFramework.h"
#include "UnitTest.h"
#include "Util.h"
#include "DataTypes.h"

#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <string>

#define EXCEPTION_ERR 0x80000000

// removes leading and trailing white spaces
// replaces all intermediate white spaces by single desired space
// in place
static void FormatString(std::string &buffer)
{
	if(buffer.length() == 0) return;

    std::string delimiters = " \r\n\t"; // delimiter symbol array
    const char desired_space = ' ';

	// trim leading white space
	std::string::size_type  lead_non_white = buffer.find_first_not_of(delimiters);
	if(lead_non_white == std::string::npos)
	{
		buffer.resize(0);
		buffer += desired_space;
		return;
	}
	buffer.erase(0, lead_non_white);

	// trim trailing white space
	std::string::size_type trail_non_white = buffer.find_last_not_of(delimiters);
	if(trail_non_white < (buffer.length()-1)) 
		buffer.erase(trail_non_white+1);

	std::string buffer_copy;
	while(1)
	{
		std::string::size_type  lead_white = buffer.find_first_of(delimiters);
		if(lead_white == std::string::npos)
		{
			buffer_copy += buffer;
			break;
		}
		else
		{
			buffer_copy += buffer.substr(0, lead_white);
			buffer_copy += desired_space;
			buffer.erase(0, lead_white);

			std::string::size_type  lead_non_white = buffer.find_first_not_of(delimiters);
			buffer.erase(0, lead_non_white);
		}
	}

	buffer = buffer_copy;
}

static const char * CpuTypeStr(FwCpuType cpuType)
{
    switch(cpuType)
    {
    case fwCpuUnknown:     return "Unknown";
    case fwCpuSSE:         return "SSE";
    case fwCpuSSE2:        return "SSE2";
    case fwCpuSSE3:        return "SSE3";
    case fwCpuFamily10h:   return "Family10h";
    default:                EXPECT( false, "Unrecognized cpu type" );
                            return "Unknown";
    }
}

static FwCpuType CpuTypeStr(const std::string str)
{
    if(str.compare(std::string("fwCpuUnknown")) == 0)
    {
        return fwCpuUnknown;
    }
    else if(str.compare(std::string("fwCpuSSE")) == 0)
    {
        return fwCpuSSE;
    }
    else if(str.compare(std::string("fwCpuSSE2")) == 0)
    {
        return fwCpuSSE2;
    }
    else if(str.compare(std::string("fwCpuSSE3")) == 0)
    {
        return fwCpuSSE3;
    }
    else if(str.compare(std::string("fwCpuFamily10h")) == 0)
    {
        return fwCpuFamily10h;
    }
    else
    {
        EXPECT( false, "Unrecognized cpu type" );
        return fwCpuUnknown;
    }
}

class AdviceConfigFile
{
    std::string fileContent;
    std::ifstream file;

    AdviceConfigFile() : fileContent("")
    {
        const char* fileName = GetConfigFileName();

        file.open(fileName);
        if(!file.is_open())
        {
            REPORT( "Advice config file (" << fileName << ") could not be opened" );
            REPORT( "Using default advice data" );
        }
        else
        {
            std::string line;
            while(std::getline(file, line))
            {
                fileContent += line;
                fileContent += '\n';
            }
        }

    }

    static AdviceConfigFile & Inst()
    {
        static AdviceConfigFile a;
        return a;
    }
public:
    static std::string Get()
    {
        return Inst().fileContent;
    }

};

// array of string elements to initialize advice objects
static void GetStrArray(std::vector<std::string> &arr, const std::string keyStart, U32 numElements)
{
    std::string fileContent = AdviceConfigFile::Get();

    if((fileContent.length() == 0) || (fileContent.find_first_not_of(" \r\n\t") == std::string::npos))
    {
        return;
    }

    std::string::size_type loc = fileContent.find(keyStart);
    EXPECT( loc != std::string::npos, "Key starting string not found" );

    fileContent.erase(0, loc+keyStart.size());
    fileContent.erase(0, fileContent.find('\n')+1);
    while(fileContent.find_first_of('\n') > fileContent.find_first_not_of(" \r\n\t")) // non-empty line
    {
        std::string line = fileContent.substr(0, fileContent.find('\n'));
        fileContent.erase(0, fileContent.find('\n')+1);

        line.erase(0,line.find_first_of('{')+1);
        for(U32 i=0; i<numElements; i++)
        {
            std::string element;
            std::string::size_type loc;
            if(i != (numElements-1))
            {
                loc = line.find_first_of(',');
            }
            else
            {
                loc = line.find_first_of('}'); // last element
            }
            EXPECT( loc != std::string::npos, "Badly formed string" );
            element = line.substr(0, loc);
            line.erase(0, loc+1);
            FormatString(element);
            arr.push_back(element);
        }
    }
}

class AdviceBase
{
	U32 alignment_;
	U32 numThreads_;
    FwCpuType codePath_;

public:

    AdviceBase() : alignment_(0), numThreads_(1), codePath_(fwCpuUnknown)
    {}

    AdviceBase(U32 alignment, U32 numThreads, FwCpuType codePath)
    {
        alignment_ = alignment;
        numThreads_ = numThreads;
        codePath_ = codePath;
    }

	U32 Alignment()    const 
    { 
        return alignment_;
    }

    U32 NumThreads()   const 
    { 
        return numThreads_;
    }

    FwCpuType CodePath() const
    {
        return codePath_;
    }

    std::string GetDescription() const
    {
        std::stringstream desc;
        desc << " A=" << Alignment();
        desc << " T=" << NumThreads();
        desc << " CPU=" << CpuTypeStr(CodePath());
        return desc.str();
    }

    static void GetAllAdvices(std::vector<AdviceBase> &allAdvices)
    {
        std::vector<std::string> arr;
        const U32 numElements = 3;
        GetStrArray(arr, "AdviceBase", numElements);

        if(arr.size() == 0)
        {
            allAdvices.push_back(AdviceBase(0,1,fwCpuUnknown));
        }
        else
        {
            U32 numRecords = (U32)arr.size();
            for(U32 i=0; i<numRecords; i+=numElements)
            {
                U32 alignment = atoi(arr[i].c_str());
                U32 numThreads = atoi(arr[i+1].c_str());
                FwCpuType codePath = CpuTypeStr(arr[i+2].c_str());

                allAdvices.push_back(AdviceBase(alignment,numThreads,codePath));
            }
        }
    }
};

class AdviceRoiCore
{
    U32 bufferWidth_;
	U32 bufferHeight_;
	U32 roiWidth_;
    U32 roiHeight_;

public:
    AdviceRoiCore() : bufferWidth_(0), bufferHeight_(0), roiWidth_(0), roiHeight_(0)
    {}

    AdviceRoiCore(U32 bufferWidth, U32 bufferHeight, U32 roiWidth, U32 roiHeight)
    {
        bufferWidth_  = bufferWidth;
        bufferHeight_ = bufferHeight;
        roiWidth_ = roiWidth;
        roiHeight_ = roiHeight;
    }

	U32 BufferWidth() const 
    { 
        return bufferWidth_;  
    }

	U32 BufferHeight() const 
    { 
        return bufferHeight_; 
    }

	U32 RoiWidth() const 
    { 
        return roiWidth_;  
    }

	U32 RoiHeight() const 
    { 
        return roiHeight_; 
    }

    FwiSize Roi() const 
    { 
        FwiSize roi = { roiWidth_, roiHeight_ };
        return roi; 
    }

    static void GetAllAdvices(std::vector<AdviceRoiCore> &allAdvices)
    {
        std::vector<std::string> arr;
        const U32 numElements = 4;
        GetStrArray(arr, "AdviceRoiCore", numElements);

        if(arr.size() == 0)
        {
            allAdvices.push_back(AdviceRoiCore(32,32,32,32));
        }
        else
        {
            U32 numRecords = (U32)arr.size();
            for(U32 i=0; i<numRecords; i+=numElements)
            {
                U32 width = atoi(arr[i].c_str());
                U32 height = atoi(arr[i+1].c_str());
                U32 roiWidth = atoi(arr[i+2].c_str());
                U32 roiHeight = atoi(arr[i+3].c_str());

                allAdvices.push_back(AdviceRoiCore(width,height,roiWidth,roiHeight));
            }
        }
    }
};

class AdviceRoiEvenWidthCore : public AdviceRoiCore
{
public:
    AdviceRoiEvenWidthCore()
    {}

    AdviceRoiEvenWidthCore(U32 bufferWidth, U32 bufferHeight, U32 roiWidth, U32 roiHeight) : AdviceRoiCore( bufferWidth, bufferHeight, roiWidth, roiHeight )
    { }

    static void GetAllAdvices(std::vector<AdviceRoiEvenWidthCore> &allAdvices)
    {
        std::vector<std::string> arr;
        const U32 numElements = 4;
        GetStrArray(arr, "AdviceRoiEvenWidthCore", numElements);

        if(arr.size() == 0)
        {
            allAdvices.push_back(AdviceRoiEvenWidthCore(32,32,32,32));
        }
        else
        {
            U32 numRecords = (U32)arr.size();
            for(U32 i=0; i<numRecords; i+=numElements)
            {
                U32 width = atoi(arr[i].c_str());
                U32 height = atoi(arr[i+1].c_str());
                U32 roiWidth = atoi(arr[i+2].c_str());
                U32 roiHeight = atoi(arr[i+3].c_str());

                allAdvices.push_back(AdviceRoiEvenWidthCore(width,height,roiWidth,roiHeight));
            }
        }
    }

};

class AdviceRoiEvenWidthEvenHeightCore : public AdviceRoiCore
{
public:
    AdviceRoiEvenWidthEvenHeightCore()
    {}

    AdviceRoiEvenWidthEvenHeightCore(U32 bufferWidth, U32 bufferHeight, U32 roiWidth, U32 roiHeight) : AdviceRoiCore( bufferWidth, bufferHeight, roiWidth, roiHeight )
    { }

    static void GetAllAdvices(std::vector<AdviceRoiEvenWidthEvenHeightCore> &allAdvices)
    {
        std::vector<std::string> arr;
        const U32 numElements = 4;
        GetStrArray(arr, "AdviceRoiEvenWidthEvenHeightCore", numElements);

        if(arr.size() == 0)
        {
            allAdvices.push_back(AdviceRoiEvenWidthEvenHeightCore(32,32,32,32));
        }
        else
        {
            U32 numRecords = (U32)arr.size();
            for(U32 i=0; i<numRecords; i+=numElements)
            {
                U32 width = atoi(arr[i].c_str());
                U32 height = atoi(arr[i+1].c_str());
                U32 roiWidth = atoi(arr[i+2].c_str());
                U32 roiHeight = atoi(arr[i+3].c_str());

                allAdvices.push_back(AdviceRoiEvenWidthEvenHeightCore(width,height,roiWidth,roiHeight));
            }
        }
    }
};


class AdviceRoi : public AdviceBase, public AdviceRoiCore
{
public:
    typedef AdviceRoiCore AdviceCore;

    AdviceRoi()
    {}

    AdviceRoi(  U32 alignment, U32 numThreads, FwCpuType codePath,
                U32 bufferWidth, U32 bufferHeight, U32 roiWidth, U32 roiHeight) :   AdviceBase(alignment, numThreads, codePath),
                                                                                    AdviceRoiCore(bufferWidth, bufferHeight, roiWidth, roiHeight)
    {}

    AdviceRoi(const AdviceBase &advBase, const AdviceRoiCore &advCore) :
                                    AdviceBase(advBase.Alignment(), advBase.NumThreads(), advBase.CodePath()),
                                    AdviceRoiCore(advCore.BufferWidth(), advCore.BufferHeight(), advCore.RoiWidth(), advCore.RoiHeight())
                                                                        
    {}

    std::string GetDescription() const
    {
        std::stringstream desc;
        desc << "size={" << BufferWidth() << "x" << BufferHeight() << "}";
        desc << " roi={" << RoiWidth() << "x" << RoiHeight() << "}";
        desc << " A=" << Alignment();
        desc << " T=" << NumThreads();
        desc << " CPU=" << CpuTypeStr(CodePath());
        return desc.str();
    }

    static void GetAllAdvices(  std::vector<AdviceBase> &allBaseAdvices,
                                std::vector<AdviceRoiCore> &allCoreAdvices)
    {
        AdviceBase::GetAllAdvices(allBaseAdvices);
        AdviceRoiCore::GetAllAdvices(allCoreAdvices);
    }
};

class AdviceRoiEvenWidth : public AdviceBase, public AdviceRoiEvenWidthCore
{
public:
    typedef AdviceRoiEvenWidthCore AdviceCore;

    AdviceRoiEvenWidth()
    {}

    AdviceRoiEvenWidth(  U32 alignment, U32 numThreads, FwCpuType codePath,
                U32 bufferWidth, U32 bufferHeight, U32 roiWidth, U32 roiHeight) :   AdviceBase(alignment, numThreads, codePath),
                                                                                    AdviceRoiEvenWidthCore(bufferWidth, bufferHeight, roiWidth, roiHeight)
    {}

    AdviceRoiEvenWidth(const AdviceBase &advBase, const AdviceRoiCore &advCore) :
                                    AdviceBase(advBase.Alignment(), advBase.NumThreads(), advBase.CodePath()),
                                    AdviceRoiEvenWidthCore(advCore.BufferWidth(), advCore.BufferHeight(), advCore.RoiWidth(), advCore.RoiHeight())
                                                                        
    {}

    std::string GetDescription() const
    {
        std::stringstream desc;
        desc << "size={" << BufferWidth() << "x" << BufferHeight() << "}";
        desc << " roi={" << RoiWidth() << "x" << RoiHeight() << "}";
        desc << " A=" << Alignment();
        desc << " T=" << NumThreads();
        desc << " CPU=" << CpuTypeStr(CodePath());
        return desc.str();
    }

    static void GetAllAdvices(  std::vector<AdviceBase> &allBaseAdvices,
                                std::vector<AdviceRoiEvenWidthCore> &allCoreAdvices)
    {
        AdviceBase::GetAllAdvices(allBaseAdvices);
        AdviceRoiEvenWidthCore::GetAllAdvices(allCoreAdvices);
    }
};

class AdviceRoiEvenWidthEvenHeight : public AdviceBase, public AdviceRoiEvenWidthEvenHeightCore
{
public:
    typedef AdviceRoiEvenWidthEvenHeightCore AdviceCore;

    AdviceRoiEvenWidthEvenHeight()
    {}

    AdviceRoiEvenWidthEvenHeight(  U32 alignment, U32 numThreads, FwCpuType codePath,
                U32 bufferWidth, U32 bufferHeight, U32 roiWidth, U32 roiHeight) :   AdviceBase(alignment, numThreads, codePath),
                                                                                    AdviceRoiEvenWidthEvenHeightCore(bufferWidth, bufferHeight, roiWidth, roiHeight)
    {}

    AdviceRoiEvenWidthEvenHeight(const AdviceBase &advBase, const AdviceRoiCore &advCore) :
                                    AdviceBase(advBase.Alignment(), advBase.NumThreads(), advBase.CodePath()),
                                    AdviceRoiEvenWidthEvenHeightCore(advCore.BufferWidth(), advCore.BufferHeight(), advCore.RoiWidth(), advCore.RoiHeight())
                                                                        
    {}

    std::string GetDescription() const
    {
        std::stringstream desc;
        desc << "size={" << BufferWidth() << "x" << BufferHeight() << "}";
        desc << " roi={" << RoiWidth() << "x" << RoiHeight() << "}";
        desc << " A=" << Alignment();
        desc << " T=" << NumThreads();
        desc << " CPU=" << CpuTypeStr(CodePath());
        return desc.str();
    }

    static void GetAllAdvices(  std::vector<AdviceBase> &allBaseAdvices,
                                std::vector<AdviceRoiEvenWidthEvenHeightCore> &allCoreAdvices)
    {
        AdviceBase::GetAllAdvices(allBaseAdvices);
        AdviceRoiEvenWidthEvenHeightCore::GetAllAdvices(allCoreAdvices);
    }
};

class AdviceLenCore
{
	U32 bufferLength_;

public:
    AdviceLenCore() : bufferLength_(0)
    {}

    AdviceLenCore(U32 bufferLength)
    {
        bufferLength_ = bufferLength;
    }

	U32 BufferLength() const 
    { 
        return bufferLength_;  
    }

    static void GetAllAdvices(std::vector<AdviceLenCore> &allAdvices)
    {
        std::vector<std::string> arr;
        const U32 numElements = 1;
        GetStrArray(arr, "AdviceLenCore", numElements);

        if(arr.size() == 0)
        {
            allAdvices.push_back(AdviceLenCore(32));
        }
        else
        {
            U32 numRecords = (U32)arr.size();
            for(U32 i=0; i<numRecords; i+=numElements)
            {
                U32 length = atoi(arr[i].c_str());

                allAdvices.push_back(AdviceLenCore(length));
            }
        }
    }
};

class AdviceLen : public AdviceBase, public AdviceLenCore
{
public:
    typedef AdviceLenCore AdviceCore;

    AdviceLen()
    {}

    AdviceLen(  U32 alignment, U32 numThreads,  FwCpuType codePath,
                U32 bufferLength) : AdviceBase(alignment, numThreads, codePath),
                                    AdviceLenCore(bufferLength)
    {}

    AdviceLen(const AdviceBase &advBase, const AdviceLenCore &advCore) :
                                    AdviceBase(advBase.Alignment(), advBase.NumThreads(), advBase.CodePath()),
                                    AdviceLenCore(advCore.BufferLength())
                                                                        
    {}

    static void GetAllAdvices(  std::vector<AdviceBase> &allBaseAdvices,
                                std::vector<AdviceLenCore> &allCoreAdvices)
    {
        AdviceBase::GetAllAdvices(allBaseAdvices);
        AdviceLenCore::GetAllAdvices(allCoreAdvices);
    }

    std::string GetDescription() const
    {
        std::stringstream desc;
        desc << "Length=" << BufferLength();
        desc << " A=" << Alignment();
        desc << " T=" << NumThreads();
        desc << " CPU=" << CpuTypeStr(CodePath());
        return desc.str();
    }
};

// TODO: Merge Advisor<Advice> class with this
class AdvisorBase
{
    bool resetBaseCalled;
	unsigned int indexBase;

    std::vector<AdviceBase> allBaseAdvices;

	AdvisorBase() : resetBaseCalled(false), indexBase(0)
    {
        AdviceBase::GetAllAdvices(allBaseAdvices);
    }

    // reset for base advices
	void ResetBase_private()
	{
        resetBaseCalled = true;
		indexBase = -1;
	}

    // step through base advices (alignment, threads, codepath) 
    bool AdvanceBase_private()
	{
		EXPECT( resetBaseCalled, "ResetBase must be called before first call to AdvanceBase()" );

        indexBase++;
        bool isTest = (indexBase < allBaseAdvices.size());
        if(isTest)
        {
            ::fwSetNumThreads( allBaseAdvices[indexBase].NumThreads() );
            ::fwStaticInitCpu( allBaseAdvices[indexBase].CodePath() );
        }
        return isTest;
	}

	const AdviceBase & GetBase_private()
    {
        return allBaseAdvices[indexBase];
    } 

	static AdvisorBase & Inst(){ static AdvisorBase a; return a; }

public:

	static void ResetBase               (){ return Inst().ResetBase_private();      }
	static bool AdvanceBase             (){ return Inst().AdvanceBase_private();    }
	static const AdviceBase & GetBase   (){ return Inst().GetBase_private();        }
};

template <class Advice>
class Advisor
{
    bool resetBaseCalled, resetCoreCalled;
	unsigned int indexBase, indexCore;

    typedef typename Advice::AdviceCore AdviceCore;

    std::vector<AdviceBase> allBaseAdvices;
    std::vector<AdviceCore> allCoreAdvices;

	Advisor() : resetBaseCalled(false), resetCoreCalled(false), indexBase(0), indexCore(0)
    {
        Advice::GetAllAdvices(allBaseAdvices, allCoreAdvices);
    }

    // reset for base advices
	void ResetBase_private()
	{
        resetBaseCalled = true;
		indexBase = -1;
	}

    // reset for core advices
    void ResetCore_private()
	{
        resetCoreCalled = true;
		indexCore = -1;
	}

    // step through base advices (alignment, threads, codepath) 
    bool AdvanceBase_private()
	{
		EXPECT( resetBaseCalled, "ResetBase must be called before first call to AdvanceBase()" );

        indexBase++;
        bool isTest = (indexBase < allBaseAdvices.size());
        if(isTest)
        {
            ::fwSetNumThreads( allBaseAdvices[indexBase].NumThreads() );
            ::fwStaticInitCpu( allBaseAdvices[indexBase].CodePath() );
        }
        return isTest;
	}

    // step through core advices (sizes)
	bool AdvanceCore_private()
	{
		EXPECT( resetCoreCalled, "ResetCore must be called before first call to AdvanceCore()" );

        indexCore++;
        bool isTest = (indexCore < allCoreAdvices.size());
        return isTest;
	}

	const Advice Get_private()
    {
        return Advice(allBaseAdvices[indexBase], allCoreAdvices[indexCore]);
    } 

    const AdviceCore & GetCore_private()
    {
        return allCoreAdvices[indexCore];
    } 

	static Advisor & Inst(){ static Advisor a; return a; }

public:

	static void ResetBase    (){ return Inst().ResetBase_private();   }
    static void ResetCore    (){ return Inst().ResetCore_private();   }
	static bool AdvanceBase  (){ return Inst().AdvanceBase_private(); }
    static bool AdvanceCore  (){ return Inst().AdvanceCore_private(); }
	static const Advice Get  (){ return Inst().Get_private(); }
    static const AdviceCore & GetCore(){ return Inst().GetCore_private(); }

};

template<typename T, int> class PlanarBuffer;
//
// Buffer
//

const char bufferPrefix[]  = "BufferStartHere";
const char bufferPostfix[] = "BufferEndsHere!";

// this class provides some basic functionality for non-complex data types
template< typename T >
class MemOpsBase
{
public:
    // perform buffer initialization with string values from the input list
    // ptr is pointer to the first element in the buffer
    // count is the total count of elements in the buffer
    // input is a list of values (in text format)
    static size_t InitializeData( T * ptr, size_t count, const StringList & input )
    {
		StringList::const_iterator it=input.begin();
		size_t i;
		for( i = 0; it != input.end() && i < count; ++it, ++i )
		{
			ptr[i] = To<T>::From( *it );
		}
        return i;
    }

    // compare two values
    // v1, v2 the values to compare
    // errorMargine - the magnitude of the error allowed
    // returns true if ABS(v1-v2) <= errorMargin, false otherwise
    static bool Compare( const T & v1, const T & v2, const T & errorMargin )
    {
        if( v1 == v2 )
        {
            return true;
        }
        if( v1 < v2 )
        {
            if( (v2 - v1) <= errorMargin )
                return true;
            else
                return false;
        }
        else
        {
            if( (v1 - v2) <= errorMargin )
                return true;
            else
                return false;
        }
        return false;
    }

    // returns a zero initialized value of the specified type
    // returns 0
    static T Zero()
    {
        return static_cast<T>(0);
    }
};

// this class provides some basic functionality for non-complex data types
template< typename T >
class MemComplexOpsBase
{
public:
    // perform buffer initialization for complex data types
    // buffer is initialized with string values from the input list.
    // - ptr is pointer to the first element in the buffer
    // - count is the total count of elements in the buffer
    // - input is a list of values (in text format).  For each element of the value, two 
    //   values from the input list will be used.  Odd elements of the list will be 
    //   used as the real value, even elements of the list will be used as the imageinary value.
    // If the list has an odd number of elements, the last element will be ignored.
    static size_t InitializeData( T * ptr, size_t count, const StringList & input )
    {
		StringList::const_iterator it=input.begin();
		size_t i = 0;
		for( i=0; it!=input.end() && i<count; ++it )
		{
            const std::string im = *it;
            ++it;
            if( it==input.end() )
                break;
            const std::string re = *it;
			ptr[i] = To<T>::From( im, re );
            ++i;
		}

        return i;
    }

    // compare two values
    // v1, v2 the values to compare
    // errorMargin - the magnitude of the error allowed
    // returns true if ( (ABS(v1.i-v2.i) <= errorMargin.i) AND (ABS(v1.r-v2.r) <= errorMargin.r) ), 
    //         false otherwise
    static bool Compare( const T & v1, const T & v2, const T & errorMargin )
    {
        if( ( v1.re < v2.re ) && ( (v2.re - v1.re) > errorMargin.re ) )
        {
            return false;
        }
        if( ( v1.im < v2.im ) && ( (v2.im - v1.im) > errorMargin.im ) )
        {
            return false;
        }
        if( ( v2.re < v1.re ) && ( (v1.re - v2.re) > errorMargin.re ) )
        {
            return false;
        }
        if( ( v2.im < v1.im ) && ( (v1.im - v2.im) > errorMargin.im ) )
        {
            return false;
        }
        return true;
    }

    // returns a zero initialized value of the specified type
    // returns { 0, 0 }
    static T Zero()
    {
        T r = { 0 };
        return r;
    }
};

template< typename T >
class MemOps : public MemOpsBase< T >
{
};

template<> class MemOps<Fw16sc> : public MemComplexOpsBase<Fw16sc> {};
template<> class MemOps<Fw32sc> : public MemComplexOpsBase<Fw32sc> {};
template<> class MemOps<Fw64sc> : public MemComplexOpsBase<Fw64sc> {};
template<> class MemOps<Fw32fc> : public MemComplexOpsBase<Fw32fc> {};
template<> class MemOps<Fw64fc> : public MemComplexOpsBase<Fw64fc> {};

// 
// base buffer class
//
template< typename T >
class Buffer 
{
    friend class PlanarBuffer< T, 1 >;
    friend class PlanarBuffer< T, 2 >;
    friend class PlanarBuffer< T, 3 >;
    friend class PlanarBuffer< T, 4 >;

private:

    size_t allocBytes; // the size of the entire memory allocated

protected:

    U32 alignment; // user requested alignment (in bytes)
	size_t count; // the size of the buffer in number of elements, as requested by the user
	size_t bufflen;
	size_t bytes; // the size of the buffer in bytes, as requested by the user
	char *ptr0; // this is the address we obtain from malloc call
	char *ptr1; // this is the 16 bytes aligned address
	T *ptr;     // this is the address with user specified alignment

private:

    void Create()
    {
        bytes = count * sizeof(T);

        //add extra bytes to test buffer overrun and alignment
		size_t totalBytes = bytes + sizeof(bufferPrefix) + sizeof(bufferPostfix) + alignment;
        allocBytes = totalBytes + 16;
		ptr0 = (char*) ::malloc( allocBytes );
		EXPECT( ptr0 != 0, "Malloc returned null pointer" );

        // ensure that the base address is 16 bytes aligned
        size_t address = (size_t)ptr0 + sizeof(bufferPrefix);
        if( address % 16 )
        {
            address = address + 16 - ( address % 16 );
        }
		ptr1 = (char*)address;

		//byte alignement
		ptr = reinterpret_cast<T*>( ptr1 + alignment );

        ::memset( ptr0, 0, allocBytes );
        ::memcpy( ptr0, bufferPrefix, sizeof(bufferPrefix) );
        ::memcpy( ptr1 + alignment + bytes, bufferPostfix, sizeof(bufferPostfix) );
    }

    void Create( const char * init, U32 count_, U32 alignment_ )
    {
        count = count_;
        alignment = alignment_;

        StringList input;
        if( init != NULL )
        {
            input = ToStringList( init ); 
		    count = count_ ? count_ : input.size();
			if(0 != input.size())
				bufflen = input.size();
			else
				bufflen = count;

        }
        EXPECT( count > 0, "The buffer cannot be zero size!" );
		
        Create();

        // initialize the data using the initialization string.
        // The function will return the count of elements initialized.
        size_t i = MemOps<T>::InitializeData( ptr, count, input );

		//replicate data for bigger buffers
		//Will not replicate empty string since data has been initialized to 0
		if ((i!=0) && (i<count)) {
			size_t validlen = i;
			for (;i<count;i++) 
			{
				ptr[i] = ptr[i%validlen];
			}
		}
    }

    void Destroy()
    {
        if(!ptr0)
        {
            ::free( ptr0 );
        }
    }

    void Create(const Buffer &buffer)
    {
        alignment = buffer.alignment;       // copy alignment
        count = buffer.count;               // copy count
        Create();                           // create new buffer
        ::memcpy(ptr, buffer.ptr, bytes);   // copy data
    }

public:

    Buffer() : count(0), alignment(0), bytes(0), ptr0(0), ptr1(0), ptr(0)
    {}

	Buffer( const char * init, U32 count_=0, U32 alignment_=0 )
	{
        Create(init, count_, alignment_);
	}

    void Reset( const char * init, U32 count_=0, U32 alignment_=0 )
    {
        Destroy();
        Create(init, count_, alignment_);
    }

    Buffer(const Buffer &buffer)
    {
        if( this == &buffer )
            return;

        Create(buffer);
    }

    Buffer &operator=(const Buffer &buffer)
    {
        if( this == &buffer )
            return *this;

        Destroy();
        Create(buffer);
        return *this;
    }

	~Buffer()
	{
		Destroy();
	}

	size_t Count() const 
    { 
        return count;
    }

	size_t BuffLen() const 
    { 
        return bufflen;
    }

	T & operator[]( int i ) 
    { 
        return ptr[i]; 
    }
	const T & operator[] ( int i ) const 
    { 
        return ptr[i]; 
    }
	
	T * Ptr() 
    { 
        return ptr;
    }
	const T * Ptr() const 
    { 
        return ptr;
    }

	std::string ToStr( size_t start, size_t count )
	{
		std::ostringstream oss;
		size_t end = start+count;//std::min< size_t >( start+count, count );

		for( size_t i=start; i<end; ++i )
		{
			oss << std::setw( 4 ) << ::ToStr( ptr[i] ) << " ";
		} 

		return oss.str();
	}

    bool Eval(std::stringstream &sStream, const Buffer<T> &buffer, T errorMargin = MemOps<T>::Zero()) const
    {
        if(Count() != buffer.Count())
        {
            return false;
        }

		U32 i;
	    const T *pDat = Ptr();
	    const T *pDatRef = buffer.Ptr();

		for( i=0; i < Count(); ++i )
		{
            if( !MemOps<T>::Compare( pDat[i], pDatRef[i], errorMargin ) )
			{
				sStream << std::endl << "Buffer mismatch at index: " << i << std::endl;
                sStream << "Actual: " << ::ToStr(pDat[i]) << std::endl;
				sStream << "Expected: " << ::ToStr(pDatRef[i]) << std::endl;
                return false;
			}
		}

        if( !TestIntegrity() )
        {
            sStream << std::endl << "Memory corruption detected around the buffer!" << std::endl;
            return false;
		}
        return true;
    }

    // will return false if the beginning or the end of the buffer is corrupted, true otherwise
    bool TestIntegrity() const
    {
        return ( ::memcmp( ptr0, bufferPrefix, sizeof(bufferPrefix) ) == 0 ) &&
               ( ::memcmp( ptr1 + alignment + bytes, bufferPostfix, sizeof(bufferPostfix) ) == 0 );
    }
};

//
// channel (interleaved) buffer
//
template< typename T, int channels >
class ChannelBuffer : public Buffer< T >
{
    U32 width;
    U32 height;

public:
    ChannelBuffer() : width(0), height(0)
    {}

    void Reset( const char * init, U32 width_, U32 height_, U32 alignment = 0 )
    {
        Buffer<T>::Reset( init, width_ * height_ * channels, alignment );
        width = width_;
        height = height_;
    }

    ChannelBuffer( const char * init, U32 width_, U32 height_, U32 alignment = 0 )
        : Buffer<T>( init, width_ * height_ * channels, alignment )
    {
        width = width_;
        height = height_;
    }

    ChannelBuffer(const ChannelBuffer &buffer) : Buffer<T>(buffer)
    {
        if( this == &buffer )
            return;

        width = buffer.width;
        height = buffer.height;
    }

    ChannelBuffer &operator=(const ChannelBuffer &buffer)
    {
        if( this == &buffer )
            return *this;

        Buffer<T>::operator=(buffer);
        return *this;
    }

    bool Eval(std::stringstream &sStream, const ChannelBuffer<T,channels> &buffer, T errorMargin = MemOps<T>::Zero()) const
    {
        return Buffer<T>::Eval(sStream, buffer, errorMargin);
    }

    U32 StepBytes()  const 
    {
        return width * sizeof(T) * channels;
    }

    U32 SizeBytes() const
    {
        return width * height * channels * sizeof(T);
    }
};

//
// Planar Buffer
//
template< typename T, int planes >
class PlanarBuffer 
{
    Buffer<T> *pPlanes;
    T **pPlanesArray;
    U32 width[planes];
    U32 height[planes];
    U32 alignment[planes];

    void Common(const U32 *pWidth, const U32 *pHeight)
    {
        EXPECT( planes > 1, "The planes count must be higher than 1" );

        for( int n = 0; n < planes; n++ )
        {
            width[n]  = pWidth[n];
            height[n] = pHeight[n];
        }

        pPlanes = (Buffer<T>*)malloc( sizeof(Buffer<T>) * planes );
        pPlanesArray = new T*[planes];
    }

    void Create(const char ** ppInit, const U32 *pWidth, const U32 *pHeight, const U32 *pAlignment = 0 )
    {
        EXPECT( ppInit != NULL, "Must provide an initialization strings array" );

        Common(pWidth, pHeight);

        for( int n = 0; n < planes; n++ )
        {
            const char * pInit = NULL;
            if( ppInit != NULL )
            {
                pInit = ppInit[n];
            }
            U32 alignment = 0;
            if( pAlignment != NULL )
                alignment = pAlignment[n];
            Buffer<T> * pBuffer = new(pPlanes + n) Buffer<T>( pInit, width[n] * height[n], alignment );
            pPlanesArray[n] = pBuffer->ptr;
        }
    }

    void Create(const PlanarBuffer &buffer)
    {
        Common(buffer.width, buffer.height);

        for( int n = 0; n < planes; n++ )
        {
            Buffer<T> * pBuffer = new(pPlanes + n) Buffer<T>( buffer[n] );
            pPlanesArray[n] = pBuffer->ptr;
        }
    }

    void Destroy()
    {
        for( int n = 0; n < planes; n++ )
        {
            pPlanes[n].~Buffer<T>();
        }
        delete [] pPlanesArray;
        if(!pPlanes)
        {
            ::free(pPlanes);
        }
    }

public:    

	PlanarBuffer( const char * init, U32 width_, U32 height_, U32 alignment = 0 )
	{
        char *ppInit[planes];
        U32 pAlignment[planes];
        U32 pWidth[planes];
        U32 pHeight[planes];

        for(int n = 0; n < planes; n++ )
        {
            ppInit[n] = init;
            pAlignment[n] = alignment;
            pWidth[n] = width_;
            pHeight[n] = height_;
        }

        Create(ppInit, pWidth, pHeight, pAlignment);
	}

	PlanarBuffer( const char ** ppInit, U32 width_, U32 height_, U32 *pAlignment = 0 )
	{
        U32 pWidth[planes];
        U32 pHeight[planes];

        for(int n = 0; n < planes; n++ )
        {
            pWidth[n] = width_;
            pHeight[n] = height_;
        }

        Create(ppInit, pWidth, pHeight, pAlignment);
	}

	PlanarBuffer( const char ** ppInit, U32 *pWidth, U32 *pHeight, U32 *pAlignment = 0 )
	{
        Create(ppInit, pWidth, pHeight, pAlignment);
	}

    PlanarBuffer(const PlanarBuffer &buffer)
    {
        if( this == &buffer )
            return;

        Create(buffer);
    }

    PlanarBuffer &operator=(const PlanarBuffer &buffer)
    {
        if( this == &buffer )
            return *this;

        Destroy();
        Create(buffer);
        return *this;
    }

	~PlanarBuffer()
	{
        Destroy();
	}

    T** Ptr() { return pPlanesArray; }
    T** Ptr() const { return pPlanesArray; }
    // T **pPlanesArray;

	Buffer<T> & operator[]( int i ){ return pPlanes[i]; }
	const Buffer<T> & operator[]( int i ) const { return pPlanes[i]; }

    U32 StepBytes(int plane = 0)  const 
    {
        return width[plane] * sizeof(T);
    }

    U32 Size(int plane = 0) const
    {
        return width[plane] * height[plane];
    }
};

//
// Collections
//

class Timer
{
    double wallTime;
    Fw64u cycleTime;
    TimerWall wall;
    TimerCycle cycle;

    bool isStartCalled, isStopCalled;

public:
    Timer() : wallTime(0), cycleTime(0), isStartCalled(false), isStopCalled(false)
    {}

    void Start()
    {
        isStartCalled = true;
        wall.Start();
        cycle.Start(); // TODO: overhead of stacking 2 timer starts?
    }

    void Stop()
    {
        cycleTime = cycle.SampleElapsed();
        wallTime = wall.SampleElapsed();
        isStopCalled = true;
    }

    std::string GetDescription() const
    {
        EXPECT( isStartCalled && isStopCalled , "Timer's Start and/or Stop functions are not called" );
        std::stringstream desc;

        desc << " WallTime= " << wallTime;
        desc << " Cycles= " << cycleTime;
        return desc.str();
    }
};

class FunctionObjectBase : public UnitTestBase
{
	FwStatus func_stat;
    unsigned int testCaseNum;

protected:
    Timer timer;

public:
    FunctionObjectBase( UnitTestCatalogBase & parent, const char *name )
        : UnitTestBase( parent, name ), testCaseNum( 0 )
    {}

    virtual void ResetFnParams( const AdviceBase &adv ) = 0;
    virtual bool Eval() = 0;
    virtual void RunAll() = 0;
    virtual FwStatus CallFn() = 0;

protected:
    void ExecuteTest( const FwStatus expectedReturn )
    {
        testCaseNum++;

        unsigned testNum = 0;
        AdvisorBase::ResetBase();
        while( AdvisorBase::AdvanceBase() )
        {
		    const AdviceBase &adv = AdvisorBase::GetBase();
            std::ostringstream oss; 
            oss << "Test " << ++testNum;

            ResetFnParams( adv );

            timer.Start();
            try
            {
                func_stat = CallFn();
            }
            catch( ... )
            {
                func_stat = (FwStatus)EXCEPTION_ERR;
            }
            timer.Stop();

            std::string msg = oss.str() + std::string(" ");
            msg += adv.GetDescription();
            msg += timer.GetDescription();

            if( func_stat == EXCEPTION_ERR )
            {
                ReportException ( oss.str().c_str(), msg.c_str() );
            }
            else
            {
                bool isPass;
                if(func_stat == expectedReturn)
                {
                    isPass = Eval();
                }
                else
                {
                    isPass = false;
                }

                ReportStatus( isPass, oss.str().c_str(), msg.c_str() );
            }
        }
	}
};

template <typename TD>
class DstLenBase : public UnitTestBase
{
	FwStatus func_stat;
    unsigned int testCaseNum;

protected:
    Timer timer;

public:
	DstLenBase( UnitTestCatalogBase & parent, const char *name )
        : UnitTestBase( parent, name ), testCaseNum( 0 )
	{
    }

    virtual void RunAll() = 0;

protected:
    void ExecuteTest( const char * dstInit, const char * dstExpected, TD errorMargin, const FwStatus expectedReturn )
    {
        testCaseNum++;
        if( dstInit == NULL )
            dstInit = "";

        bool isExpectedAvailable = true;
        if( dstExpected == NULL )
        {
            isExpectedAvailable = false;
            dstExpected = "";
        }

        unsigned testNum = 0;
		Advisor<AdviceLen>::ResetCore();
		while( Advisor<AdviceLen>::AdvanceCore() )
		{
            const AdviceLenCore & advCore = Advisor<AdviceLen>::GetCore();

            Buffer< TD > dstCore( dstInit, advCore.BufferLength() );
            Buffer< TD > expected( dstExpected, advCore.BufferLength() );

            bool isGoldSame = true;
            bool isFirst = true;
            Advisor<AdviceLen>::ResetBase();
            while( Advisor<AdviceLen>::AdvanceBase() )
            {
			    const AdviceLen & adv = Advisor<AdviceLen>::Get();
                std::ostringstream oss; 
                oss << "Test " << ++testNum;

                Buffer< TD > dst( dstInit, adv.BufferLength(), adv.Alignment() );

			    CallFn( adv, func_stat, dst );

                std::string msg = oss.str() + std::string(" ") + adv.GetDescription() + timer.GetDescription();

                if( func_stat == EXCEPTION_ERR )
                {
                    ReportException ( oss.str().c_str(), msg.c_str() );
                }
                else
                {
                    bool isExpectedSame = true;
                    if(isExpectedAvailable)
                    {
                        isExpectedSame = Eval( adv, dst, expected, expectedReturn, errorMargin );
                    }
                    else
                    {
                        if(isFirst)
                        {
                            dstCore = dst;
                        }
                        else
                        {
                            isGoldSame = Eval( adv, dst, dstCore, expectedReturn, errorMargin );
                        }
                    }

                    ReportStatus( isGoldSame && isExpectedSame, oss.str().c_str(), msg.c_str() );
                }
                isFirst = false;
            }
		}
	}
	
	virtual bool Eval(const AdviceLen & adv, 
                      const Buffer< TD >& dst,
                      const Buffer< TD >& expected,
                      const FwStatus expectedReturn,
                      TD errorMargin )
	{
		if (func_stat != expectedReturn) {
			Log() << "Status Error, Expected Status: " << expectedReturn << 
				", Returned Status: " << func_stat << std::endl;
			return false;
		}
		if (func_stat==fwStsNullPtrErr) return true;

		U32 i;
	    const TD *pDst = dst.Ptr();
	    const TD *pDstExp = expected.Ptr();

		for( i=0; i < dst.Count(); ++i )
		{
            if( !MemOps<TD>::Compare( dst[i], pDstExp[i], errorMargin ) )
			{
				Log() << std::endl << "Buffer mismatch at index: " << i << std::endl;
                Log() << "Actual: " << ::ToStr(dst[i]) << std::endl;
				Log() << "Expected: " << ::ToStr(pDstExp[i]) << std::endl;
                return false;
			}
		}

        if( !dst.TestIntegrity() )
        {
            Log() << std::endl << "Memory corruption detected around the destination buffer!" << std::endl;
            return false;
		}
        return true;
	}

    virtual void CallFn( const AdviceLen & adv, 
                         FwStatus & stat, 
                         Buffer< TD >& dst ) = 0;

    virtual unsigned int GetTestCaseNum()
    {
        return testCaseNum;
    }
};

template< typename TD, U32 dstChannels, class Advice >
class DstRoiBase : public UnitTestBase
{
	FwStatus func_stat;
    unsigned int testCaseNum;

protected:
    Timer timer;

public:
	DstRoiBase( UnitTestCatalogBase & parent, const char *name )
        : UnitTestBase( parent, name ), testCaseNum( 0 )
	{
    }

    virtual void RunAll() = 0;

protected:
    void ExecuteTest( const char * dstInit, const char * dstExpected, TD errorMargin, const FwStatus expectedReturn )
    {
        testCaseNum++;
        if( dstInit == NULL )
            dstInit = "";

        bool isExpectedAvailable = true;
        if( dstExpected == NULL )
        {
            isExpectedAvailable = false;
            dstExpected = "";
        }

        unsigned int testNum = 0;
		Advisor<Advice>::ResetCore();
		while( Advisor<Advice>::AdvanceCore() )
		{
            const AdviceRoiCore & advCore = Advisor<Advice>::GetCore();

            ChannelBuffer< TD, dstChannels > dstCore( dstInit, advCore.BufferWidth(), advCore.BufferHeight() );
            ChannelBuffer< TD, dstChannels > expected( dstExpected, advCore.BufferWidth(), advCore.BufferHeight() );
            PrepareExpectedBuffer( dstCore, expected, advCore );

            bool isGoldSame = true;
            bool isFirst = true;
            Advisor<Advice>::ResetBase();
            while( Advisor<Advice>::AdvanceBase() )
            {
			    const Advice & adv = Advisor<Advice>::Get();
                std::ostringstream oss; 
                oss << "Test " << ++testNum;

                ChannelBuffer< TD, dstChannels > dst( dstInit, adv.BufferWidth(), adv.BufferHeight(), adv.Alignment() );
			    
                CallFn( adv, func_stat, dst );

                std::string msg = oss.str() + std::string(" ") + adv.GetDescription() + timer.GetDescription();

                if( func_stat == EXCEPTION_ERR )
                {
                    ReportException ( oss.str().c_str(), msg.c_str() );
                }
                else
                {
                    bool isExpectedSame = true;
                    if(isExpectedAvailable)
                    {
                        isExpectedSame = Eval( adv, dst, expected, expectedReturn, errorMargin );
                    }
                    else
                    {
                        if(isFirst)
                        {
                            dstCore = dst;
                        }
                        else
                        {
                             isGoldSame = Eval( adv, dst, dstCore, expectedReturn, errorMargin );
                        }
                    }

                    ReportStatus( isGoldSame && isExpectedSame, oss.str().c_str(), msg.c_str() );
                }
                isFirst = false;
            }
		}
	}
	
    void PrepareExpectedBuffer( const ChannelBuffer< TD, dstChannels > & dst, ChannelBuffer< TD, dstChannels > & expected, const AdviceRoiCore & adv )
    {
	    const TD *pDst = dst.Ptr();
	    TD *pDstExp = expected.Ptr();
	    U32 bufferWidth = adv.BufferWidth();
	    U32 bufferHeight = adv.BufferHeight();
	    U32 roiWidth = adv.Roi().width;
	    U32 roiHeight = adv.Roi().height;

        // initialize the area to the right of the roi
	    for( U32 i = 0; i < roiHeight; i++ )
	    {
		    ::memcpy( pDstExp + (bufferWidth * i + roiWidth) * dstChannels,
                      pDst + (bufferWidth * i + roiWidth) * dstChannels,
                      sizeof(TD) * (bufferWidth - roiWidth) * dstChannels );
	    }
	    ::memcpy( pDstExp + (roiHeight * bufferWidth * dstChannels),
                  pDst + (roiHeight * bufferWidth * dstChannels),
		          sizeof(TD) * (bufferHeight - roiHeight) * bufferWidth * dstChannels);
    }

	virtual bool Eval(const Advice & adv, 
                      const ChannelBuffer< TD, dstChannels >& dst,
                      const ChannelBuffer< TD, dstChannels >& expected,
                      const FwStatus expectedReturn,
                      TD errorMargin )
	{
		if (func_stat != expectedReturn) {
			Log() << "Status Error, Expected Status: " << expectedReturn << 
				", Returned Status: " << func_stat << std::endl;
			return false;
		}
		if (func_stat==fwStsNullPtrErr) return true;

		U32 i;
	    const TD *pDst = dst.Ptr();
	    const TD *pDstExp = expected.Ptr();

		for( i=0; i < expected.BuffLen(); ++i )
		{
            if( !MemOps<TD>::Compare( dst[i], pDstExp[i], errorMargin ) )
			{
				Log() << std::endl << "Buffer mismatch at index: " << i << std::endl;
                Log() << "Actual: " << ::ToStr(dst[i]) << std::endl;
				Log() << "Expected: " << ::ToStr(pDstExp[i]) << std::endl;
                return false;
			}
		}

        if( !dst.TestIntegrity() )
        {
            Log() << std::endl << "Memory corruption detected around the destination buffer!" << std::endl;
            return false;
		}
        return true;
	}

    virtual void CallFn( const Advice & adv, 
                         FwStatus & stat, 
                         ChannelBuffer< TD, dstChannels >& dst ) = 0;

    virtual unsigned int GetTestCaseNum()
    {
        return testCaseNum;
    }
};

template< typename TD, int dstPlanes, class Advice >
class PDstRoiBase : public UnitTestBase
{
    //@TODO: add support for different alignment on each plane

    unsigned int testCaseNum;
	FwStatus func_stat;

protected:
    Timer timer;

public:
	PDstRoiBase( UnitTestCatalogBase & parent, const char *name )
        : UnitTestBase( parent, name ), testCaseNum( 0 )
	{
    }

    virtual void RunAll() = 0;

protected:
    void ExecuteTest( const char ** pDstInit, const char ** pDstExpected, TD errorMargin, const FwStatus expectedReturn )
    {
        testCaseNum++;
        U32 alignment[dstPlanes];
        const char * dstInit[dstPlanes];
        const char * dstExpected[dstPlanes];
        for( int plane = 0; plane < dstPlanes; plane++ )
        {
            if( pDstInit && pDstInit[plane] )
            {
                dstInit[plane] = pDstInit[plane];
            }
            else
            {
                dstInit[plane] = "";
            }

            if( pDstExpected && pDstExpected[plane] )
            {
                dstExpected[plane] = pDstExpected[plane];
            }
            else
            {
                dstExpected[plane] = "";
            }
        }

        bool isExpectedAvailable = true;
        if( pDstExpected == NULL )
        {
            isExpectedAvailable = false;
        }

        unsigned int testNum = 0;
        Advisor<Advice>::ResetCore();
		while( Advisor<Advice>::AdvanceCore() )
		{
            const AdviceRoiCore & advCore = Advisor<Advice>::GetCore();

			PlanarBuffer< TD, dstPlanes > dstCore( dstInit, advCore.BufferWidth(), advCore.BufferHeight() );
			PlanarBuffer< TD, dstPlanes > expected( dstExpected, advCore.BufferWidth(), advCore.BufferHeight() );
            PrepareExpectedBuffer( dstCore, expected, advCore );

            bool isGoldSame = true;
            bool isFirst = true;
            Advisor<Advice>::ResetBase();
            while( Advisor<Advice>::AdvanceBase() )
            {
			    const Advice & adv = Advisor<Advice>::Get();
                std::ostringstream oss; 
                oss << "Test " << ++testNum;

                for( int plane = 0; plane < dstPlanes; plane++ )
                {
                    alignment[plane] = adv.Alignment();
                }

			    PlanarBuffer< TD, dstPlanes > dst( dstInit, adv.BufferWidth(), adv.BufferHeight(), alignment );

			    CallFn( adv, func_stat, dst );

                std::string msg = oss.str() + std::string(" ") + adv.GetDescription() + timer.GetDescription();

                if( func_stat == EXCEPTION_ERR )
                {
                    ReportException ( oss.str().c_str(), msg.c_str() );
                }
                else
                {
                    bool isExpectedSame = true;
                    if(isExpectedAvailable)
                    {
                        isExpectedSame = Eval( adv, dst, expected, expectedReturn, errorMargin );
                    }
                    else
                    {
                        if(isFirst)
                        {
                            dstCore = dst;
                        }
                        else
                        {
                            isGoldSame = Eval( adv, dst, dstCore, expectedReturn, errorMargin );
                        }
                    }
                
                    ReportStatus( isGoldSame && isExpectedSame, oss.str().c_str(), msg.c_str() );
                }
                isFirst = false;
            }
		}
	}
	// @TODO: use StepSize in the calculations below
    void PrepareExpectedBuffer( const PlanarBuffer< TD, dstPlanes > & dst, PlanarBuffer< TD, dstPlanes > & expected, const AdviceRoiCore & adv )
    {
        //Initialize the outside area of ROI with the data from the destination
        for( int plane = 0; plane < dstPlanes; plane++ )
        {
		    const TD *pDst = dst[plane].Ptr();
		    TD *pDstExp = expected[plane].Ptr();
		    U32 bufferWidth = adv.BufferWidth();
		    U32 bufferHeight = adv.BufferHeight();
		    U32 roiWidth = adv.Roi().width;
		    U32 roiHeight = adv.Roi().height;
		    for( U32 i = 0; i < roiHeight; i++ )
		    {
			    ::memcpy( pDstExp + (bufferWidth * i + roiWidth),
                          pDst + (bufferWidth * i + roiWidth),
                          sizeof(TD) * (bufferWidth - roiWidth) );
		    }
		    ::memcpy( pDstExp + (roiHeight * bufferWidth),
                      pDst + (roiHeight * bufferWidth),
			          sizeof(TD) * (bufferHeight - roiHeight) * bufferWidth );
        }
    }

	virtual bool Eval(const Advice & adv, 
                      const PlanarBuffer< TD, dstPlanes > & dst, 
                      const PlanarBuffer< TD, dstPlanes > & expected, 
                      const FwStatus expectedReturn,
                      TD errorMargin )
	{
		if (func_stat != expectedReturn) {
			Log() << "Status Error, Expected Status: " << expectedReturn << 
				", Returned Status: " << func_stat << std::endl;
			return false;
		}
		if (func_stat==fwStsNullPtrErr) return true;

        // compare the destination to expected
        for( int plane = 0; plane < dstPlanes; plane++ )
        {
		    for( U32 i = 0; i < dst.Size(); ++i )
		    {
                if( !MemOps<TD>::Compare( dst[plane][i], expected[plane][i], errorMargin ) )
			    {
				    Log() << std::endl << "Buffer mismatch at index: " << i << std::endl;
				    Log() << "Actual: " << ::ToStr(dst[plane][i]) << std::endl;
				    Log() << "Expected: " << ::ToStr(expected[plane][i]) << std::endl;
                    return false;
			    }
		    }
            if( !dst[plane].TestIntegrity() )
            {
                Log() << std::endl << "Memory corruption detected around the destination buffer!" << std::endl;
                return false;
            }
	    }
        return true;
    }

    virtual void CallFn( const Advice & adv, 
                         FwStatus & stat, 
                         PlanarBuffer< TD, dstPlanes > & d ) = 0;

    virtual unsigned int GetTestCaseNum()
    {
        return testCaseNum;
    }
};

#endif // __OBJECT_H__
