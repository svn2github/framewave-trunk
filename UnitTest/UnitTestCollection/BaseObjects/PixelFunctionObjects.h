/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __PIXEL_FUNCTION_OBJECTS_H__
#define __PIXEL_FUNCTION_OBJECTS_H__

#include "Reg128.h"

namespace FwUnitTest
{
    #define MAX_NUM_REGS 4

	template <typename T, CH C, unsigned int NUM_PX>
	class PixelBase
	{
	public:
		const static unsigned int NUM_PIXELS = NUM_PX;
		const static unsigned int NUM_REGS = ((NUM_PIXELS*CH_COUNT(C)*sizeof(T))+15)/16;

	private:
		Reg128<T> dat[NUM_REGS];
		bool isInitialized;
		bool isStr;
		const char *initStr[MAX_NUM_REGS];

		void Initialize()
		{
			for(unsigned int i=0; i<NUM_REGS; i++)
				dat[i].Init();

			isInitialized = true;
		}

		void Initialize(const char *init[MAX_NUM_REGS])
		{
			for(unsigned int i=0; i<NUM_REGS; i++)
			{
				if(std::string(init[i]).compare(std::string("")))
				{
					dat[i].Init( init[i] );
				}
				else
				{
					assert(false);
				}
			}

			for(unsigned int i=0; i<MAX_NUM_REGS; i++)
				initStr[i] = init[i];

			isStr = true;
			isInitialized = true;
		}

	public:
		PixelBase() : isStr(false), isInitialized(false) {}

		PixelBase(const char *init[MAX_NUM_REGS]) : isStr(false), isInitialized(false)
		{
			Initialize(init);
		}

		void Init()
		{
			Initialize();
		}

		void Reset()
		{
			assert(isInitialized);
			if(isStr)
				Initialize(initStr);
			else
				Initialize();
		}

		void Init(const char *init[MAX_NUM_REGS])
		{
			Initialize(init);
		}

		Reg128<T> Dat(unsigned int i) const
		{
			assert(i<=NUM_REGS);
			return dat[i];
		}

		void SetRegs(const XMM128 *regs)
		{
			assert(isInitialized);
			for(unsigned int i=0; i<NUM_REGS; i++)
			{
				dat[i].Init(regs[i]);
			}
		}

		void SetElements(const T *pEls)
		{
			assert(isInitialized);
			for(unsigned int i=0; i<NUM_REGS; i++)
			{
				dat[i].Init(pEls);
                pEls += (16/sizeof(T));
			}
		}

		void GetRegs(XMM128 *regs) const
		{
			assert(isInitialized);
			for(unsigned int i=0; i<NUM_REGS; i++)
			{
				regs[i] = dat[i].GetData();
			}
		}

		void GetElements(T *pEls) const
		{
			assert(isInitialized);
			for(unsigned int i=0; i<NUM_REGS; i++)
			{
				dat[i].GetData(pEls);
                pEls += (16/sizeof(T));
			}
		}

		bool Eval(XMM128 *regs) const
		{
			assert(isInitialized);
			for(unsigned int i=0; i<NUM_REGS; i++)
			{
				Reg128<T> ref;
				ref.Init(regs[i]);
				if(!dat[i].Eval(ref))
					return false;
			}

			return true;
		}

        bool Eval(const T *pEls) const
        {
            assert(isInitialized);
            for(unsigned int i=0; i<NUM_REGS; i++)
            {
				if(!dat[i].Eval(pEls))
                    return false;

                pEls += (16/sizeof(T));
            }

            return true;
        }
	};

	template <typename T, CH C, unsigned int NUM_PX>
	class PixelBaseMod : public PixelBase<T,C,NUM_PX>
	{
        typedef PixelBase<T,C,NUM_PX> Base;

        Reg128<T> datRef[Base::NUM_REGS];
		bool isInitialized;

		void Initialize(const char *initRef[MAX_NUM_REGS])
		{
			for(unsigned int i=0; i<Base::NUM_REGS; i++)
			{
				if(std::string(initRef[i]).compare(std::string("")))
				{
                    datRef[i].Init( initRef[i] );
				}
				else
				{
					assert(false);
				}
			}

			isInitialized = true;
		}

	public:
		PixelBaseMod() : Base(), isInitialized(false) {}

		PixelBaseMod(const char *init[MAX_NUM_REGS], const char *initRef[MAX_NUM_REGS]) : Base(init), isInitialized(false)
		{
			Initialize(initRef);
		}

		void Init(const char *initRef[MAX_NUM_REGS])
		{
			Base::Init();
			Initialize(initRef);
		}

		void Init(const char *init[MAX_NUM_REGS], const char *initRef[MAX_NUM_REGS])
		{
			Base::Init(init);
			Initialize(initRef);
		}

		bool Eval()
		{
			for(unsigned int i=0; i<Base::NUM_REGS; i++)
			{
                assert(Base::Dat(i).IsInitialized());
                if(!Base::Dat(i).Eval(datRef[i]))
					return false;
			}

			return true;
		}
	};

    class PixelFuncBase
    {
    protected:

        bool RunREFR()
        {
            if(!InitializeREFRData())
                return false;

            CallREFR();
            CopyREFRResults();
            return EvalREFR();
        }

        bool RunSSE2()
        {
            InitializeSSE2Regs();
            CallSSE2();
            CopySSE2Results();
            return EvalSSE2();
        }

        virtual void InitializeSSE2Regs() = 0;
        virtual void CallSSE2() = 0;
        virtual void CopySSE2Results() = 0;
        virtual bool EvalSSE2() = 0;

        virtual bool InitializeREFRData() = 0;
        virtual void CallREFR() = 0;
        virtual void CopyREFRResults() = 0;
        virtual bool EvalREFR() = 0;

        void VerifyPxFnObject(const void *pObj)
        {
            EXPECT( pObj != 0, "Pixel Function object is not created, try calling 'InitializePxFnObj' function" );
        }

    public:

		bool Run()
		{
			bool passSSE2, passREFR;

			passSSE2 = RunSSE2();
			passREFR = RunREFR();
            
			return (passSSE2 && passREFR);
		}
    };

	template <class PXFN>
    class PixelFunc1SrcDst_Base : public PixelFuncBase
	{
    protected:

		typedef typename PXFN::TD1_ TD1;
		const static CH CD1 = PXFN::CD_;

		const static unsigned int NUM_PIXELS = PXFN::nPIX_SSE;

        PXFN *pObj;
		PixelBaseMod<TD1,CD1,NUM_PIXELS> dst1;

        RegFile reg;
        TD1 *pDst1;

		const static unsigned int EXPECT_NUM_INIT_STR = 2;

        void DestroyPxFnObj()
        {
            if(pObj != 0)
                fwFree(pObj);
        }

		void Initialize(const char *init[EXPECT_NUM_INIT_STR][MAX_NUM_REGS])
		{
			dst1.Init(init[0],init[1]);
		}

        bool InitializeREFRData()
        {
            dst1.Reset();

            pDst1 = (TD1 *)fwMalloc(16*dst1.NUM_REGS);
            if(pDst1 == 0)
            {
                return false;
            }

			dst1.GetElements(pDst1);

            return true;
        }

        void Destroy()
        {
            DestroyPxFnObj();
            if(pDst1 != 0)
                fwFree(pDst1);
        }

        void CallREFR()
        {
            VerifyPxFnObject(pObj);

            TD1 *pDst1_ = pDst1;
            for(unsigned int i=0; i<NUM_PIXELS; i++)
            {
                pObj->REFR(pDst1_);
                pDst1_ += CH_COUNT(CD1);
            }
        }

        void CopyREFRResults()
        {
			dst1.SetElements(pDst1);
        }

        bool EvalREFR()
        {
            return dst1.Eval();
        }

        void InitializeSSE2Regs()
        {
            dst1.Reset();
            dst1.GetRegs(reg.dst);
        }

        void CallSSE2()
        {
            VerifyPxFnObject(pObj);

            pObj->SSE2_Init();
            pObj->SSE2(reg);
        }

        void CopySSE2Results()
        {
			dst1.SetRegs(reg.dst);
        }

        bool EvalSSE2()
        {
            return dst1.Eval();
        }

	public:

        PixelFunc1SrcDst_Base(const char *init[EXPECT_NUM_INIT_STR][MAX_NUM_REGS]) : pObj(0), pDst1(0)
		{
			Initialize(init);
		}

        ~PixelFunc1SrcDst_Base()
        {
            Destroy();
        }
	};

	template <class PXFN>
    class PixelFunc1SrcDst : public PixelFunc1SrcDst_Base<PXFN>
	{
        typedef PixelFunc1SrcDst_Base<PXFN> Base;

    protected:
        void InitializePxFnObj()
        {
            void *pTmp = fwMalloc(sizeof(PXFN));
            Base::pObj = new(pTmp) PXFN;
        }

    public:
        PixelFunc1SrcDst(const char *init[Base::EXPECT_NUM_INIT_STR][MAX_NUM_REGS], typename Base::TD1 val) : Base(init)
        {
            InitializePxFnObj();
        }
    };

	template <class PXFN>
    class PixelFunc1SrcDst_val : public PixelFunc1SrcDst_Base<PXFN>
	{
        typedef PixelFunc1SrcDst_Base<PXFN> Base;
        typename Base::TD1 val_;

    protected:
        void InitializePxFnObj()
        {
            void *pTmp = fwMalloc(sizeof(PXFN));
            Base::pObj = new(pTmp) PXFN(val_);
        }

    public:
        PixelFunc1SrcDst_val(const char *init[Base::EXPECT_NUM_INIT_STR][MAX_NUM_REGS], typename Base::TD1 val) : Base(init), val_(val)
        {
            InitializePxFnObj();
        }
    };

	template <class PXFN>
    class PixelFunc1Src1Dst_Base : public PixelFuncBase
	{
    protected:
		typedef typename PXFN::TS1_ TS1;
		typedef typename PXFN::TD1_ TD1;
		const static CH CS1 = PXFN::CS1_;
		const static CH CD1 = PXFN::CD_;

		const static unsigned int NUM_PIXELS = PXFN::nPIX_SSE;

        PXFN *pObj;
		PixelBase<TS1,CS1,NUM_PIXELS> src1;
		PixelBaseMod<TD1,CD1,NUM_PIXELS> dst1;

        RegFile reg;
        TS1 *pSrc1;
        TD1 *pDst1;

		const static unsigned int EXPECT_NUM_INIT_STR = 2;

        void DestroyPxFnObj()
        {
            if(pObj != 0)
                fwFree(pObj);
        }

		void Initialize(const char *init[EXPECT_NUM_INIT_STR][MAX_NUM_REGS])
		{
			src1.Init(init[0]);
			dst1.Init(init[1]);
		}

        bool InitializeREFRData()
        {
            dst1.Reset();

            pSrc1 = (TS1 *)fwMalloc(16*src1.NUM_REGS);
            if(pSrc1 == 0)
            {
                return false;
            }
            pDst1 = (TD1 *)fwMalloc(16*dst1.NUM_REGS);
            if(pDst1 == 0)
            {
                fwFree(pSrc1);
                return false;
            }

            src1.GetElements(pSrc1);
			dst1.GetElements(pDst1);

            return true;
        }

        void Destroy()
        {
            DestroyPxFnObj();
            if(pSrc1 != 0)
                fwFree(pSrc1);
            if(pDst1 != 0)
                fwFree(pDst1);
        }

        void CallREFR()
        {
            TS1 *pSrc1_ = pSrc1;
            TD1 *pDst1_ = pDst1;
            for(unsigned int i=0; i<NUM_PIXELS; i++)
            {
                pObj->REFR(pSrc1_,pDst1_);
                pSrc1_ += CH_COUNT(CS1);
                pDst1_ += CH_COUNT(CD1);
            }
        }

        void CopyREFRResults()
        {
			dst1.SetElements(pDst1);
        }

        bool EvalREFR()
        {
            assert(src1.Eval(pSrc1));
            return dst1.Eval();
        }

        void InitializeSSE2Regs()
        {
            dst1.Reset();
            src1.GetRegs(reg.src1);
            dst1.GetRegs(reg.dst);
        }

        void CallSSE2()
        {
            pObj->SSE2_Init();
            pObj->SSE2(reg);
        }

        void CopySSE2Results()
        {
			dst1.SetRegs(reg.dst);
        }

        bool EvalSSE2()
        {
			assert(src1.Eval(reg.src1));
            return dst1.Eval();
        }

	public:

        PixelFunc1Src1Dst_Base(const char *init[2][MAX_NUM_REGS]) : pObj(0), pSrc1(0), pDst1(0)
		{
			Initialize(init);
		}

        ~PixelFunc1Src1Dst_Base()
        {
            Destroy();
        }
	};

	template <class PXFN>
    class PixelFunc1Src1Dst : public PixelFunc1Src1Dst_Base<PXFN>
	{
        typedef PixelFunc1Src1Dst_Base<PXFN> Base;

    protected:
        void InitializePxFnObj()
        {
            void *pTmp = fwMalloc(sizeof(PXFN));
            Base::pObj = new(pTmp) PXFN;
        }

    public:
        PixelFunc1Src1Dst(const char *init[Base::EXPECT_NUM_INIT_STR][MAX_NUM_REGS]) : Base(init)
        {
            InitializePxFnObj();
        }
    };
}

#endif // __PIXEL_FUNCTION_OBJECTS_H__
