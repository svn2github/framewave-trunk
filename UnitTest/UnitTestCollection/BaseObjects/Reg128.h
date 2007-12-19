/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __REG128_H__
#define __REG128_H__

#include "Util.h"
#include "UnitTest.h"

namespace FwUnitTest
{
	using namespace OPT_LEVEL;

	// static assert
    #define STATIC_TYPE_ASSERT(TI, TR) { TI *pTmpAssert = (TR *)0; }

	// initialization strings
    #define INIT_STR_NULL_1  "0"
    #define INIT_STR_NULL_2  "0 0"
    #define INIT_STR_NULL_4  "0 0 0 0"
    #define INIT_STR_NULL_8  "0 0 0 0 0 0 0 0"
    #define INIT_STR_NULL_16 "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"

	// Base type classes
	template <typename T> class C_BT {};

	template <> class C_BT<U8> 
	{
	public:
		typedef U8  BT;
		static const char *NullStr() { return INIT_STR_NULL_16; }
	};

	template <> class C_BT<S8> 
	{
	public:
		typedef S8  BT;
		static const char *NullStr() { return INIT_STR_NULL_16; }
	};

	template <> class C_BT<U16>
	{
	public:
		typedef U16 BT;
		static const char *NullStr() { return INIT_STR_NULL_8; }
	};

	template <> class C_BT<S16>
	{
	public:
		typedef S16 BT;
		static const char *NullStr() { return INIT_STR_NULL_8; }
	};

	template <> class C_BT<U32>
	{
	public:
		typedef U32 BT;
		static const char *NullStr() { return INIT_STR_NULL_4; }
	};

	template <> class C_BT<S32>
	{
	public:
		typedef S32 BT;
		static const char *NullStr() { return INIT_STR_NULL_4; }
	};

	template <> class C_BT<U64>
	{
	public:
		typedef U64 BT;
		static const char *NullStr() { return INIT_STR_NULL_2; }
	};

	template <> class C_BT<S64>
	{
	public:
		typedef S64 BT;
		static const char *NullStr() { return INIT_STR_NULL_2; }
	};

	template <> class C_BT<F32>
	{
	public:
		typedef F32 BT;
		static const char *NullStr() { return INIT_STR_NULL_4; }
	};

	template <> class C_BT<F64>
	{
	public:
		typedef F64 BT;
		static const char *NullStr() { return INIT_STR_NULL_2; }
	};

	// XMM128 parameter object
	template <typename T>
	class Reg128
	{
		typedef class C_BT<T> CBT;
		typedef typename CBT::BT TR;

		XMM128 *pDat, *pDatRef;
		std::string strInit;
		bool isInitialized;

		unsigned int NumElements() const
		{
			return (16/sizeof(T));
		}

		void ValidateType() const
		{
			STATIC_TYPE_ASSERT(T, TR);
		}

		void Allocate()
		{
			pDat = (XMM128 *)fwMalloc(sizeof(XMM128));
			EXPECT( pDat != 0, "fwMalloc returned null pointer" );

			pDatRef = (XMM128 *)fwMalloc(sizeof(XMM128));
			EXPECT( pDatRef != 0, "fwMalloc returned null pointer" );
		}

		void Free()
		{
			if(pDat != 0)
				fwFree(pDat);

			if(pDatRef != 0)
				fwFree(pDatRef);
		}

		void Initialize(const T *pEls)
		{
			ValidateType();
			Allocate();

			T *pTmp = reinterpret_cast<T *>(pDatRef);
			for(unsigned int i=0; i<NumElements(); i++)
			{
				pTmp[i] = pEls[i];
			}

			Copy();

			isInitialized = true;
		}

		void Initialize(const XMM128 &reg)
		{
			ValidateType();

			Allocate();

			*pDatRef = *pDat = reg;
			isInitialized = true;
		}


		void Initialize(const char *init)
		{
			ValidateType();
			strInit = init;

			StringList list = ToStringList( strInit.c_str() );
			size_t size = list.size();

			if(size != NumElements())
			{
				return;
			}

			Allocate();

			T *pTmp = reinterpret_cast<T *>(pDatRef);

			StringList::const_iterator it=list.begin();
			for( size_t i=0; it!=list.end() && i<size; ++it, ++i )
			{
				pTmp[i] = To<T>::From( it->c_str() );
			}

			Copy();

			isInitialized = true;
		}

		void Copy()
		{
			T *pTmp    = reinterpret_cast<T *>(pDat);
			const   T *pTmpRef = reinterpret_cast<T *>(pDatRef);

			for(unsigned int i=0; i<NumElements(); i++)
				*pTmp++ = *pTmpRef++;
		}

		std::string GetStrInit() const
		{
			return strInit;
		}

		void Destroy()
		{
			isInitialized = false;
			strInit = "";
			Free();
		}

	public:
		Reg128() : isInitialized(false), pDat(0), pDatRef(0)
		{
		}

		Reg128(const char *init) : isInitialized(false), pDat(0), pDatRef(0)
		{
			Initialize(init);
		}

		Reg128(const Reg128 &reg)
		{
            if(this == &reg)
                return;

            isInitialized = false;
            pDat = pDatRef = 0;
			if(reg.IsInitialized())
				Initialize(reg.GetData());
		}

		Reg128 &operator=(const Reg128 &reg)
		{
            if(this == &reg)
                return *this;

			Destroy();
			if(reg.IsInitialized())
				Initialize(reg.GetData());
		}

		~Reg128()
		{
			Destroy();
		}

		void Init(const char *init)
		{
			Destroy();
			Initialize(init);
		}

		void Init()
		{
			Destroy();
			Initialize(CBT::NullStr());
		}

		void Init(const XMM128 &reg)
		{
			Destroy();
			Initialize(reg);
		}

		void Init(const T *pEls)
		{
			Destroy();
			Initialize(pEls);

		}

		void Reset()
		{
			if(isInitialized)
				Copy();
		}

		bool IsInitialized() const
		{
			return isInitialized;
		}

        void GetData(T *pEls) const
        {
            const T *pTmp = reinterpret_cast<T *>(pDat);

			for(unsigned int i=0; i<NumElements(); i++)
			{
				*pEls++ = *pTmp++;
			}
		}

		XMM128 GetData() const
		{
			assert(isInitialized);
			return *pDat;
		}

        bool Eval(const T *pRef) const
        {
			const T *pTmp = reinterpret_cast<T *>(pDat);

			for(unsigned int i=0; i<NumElements(); i++)
				if(*pTmp++ != *pRef++)
					return false;

			return true;
        }

        bool Eval(const Reg128 &reg) const
		{
			const T *pTmp    = reinterpret_cast<T *>(pDat);
			const T *pTmpRef = reinterpret_cast<T *>(reg.pDat);

			for(unsigned int i=0; i<NumElements(); i++)
				if(*pTmp++ != *pTmpRef++)
					return false;

			return true;
		}
	};

}

#endif // __REG128_H__
