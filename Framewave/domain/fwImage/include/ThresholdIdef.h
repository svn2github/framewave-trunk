/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __THRESHOLD_IMG_DEF__
#define __THRESHOLD_IMG_DEF__ 

#include "ThresholdIimp.h"
#include "fe.h"

namespace OPT_LEVEL
{

template<class TD,CH chd,class TL,class TV>
struct Threshold1 :public fe1< TD, chd >   
{
		TV value;
		TL level;
		XMM128  mLevel;
  	    XMM128  mValue;

		Threshold1(TL l)
		{
			level = l;
		}
		Threshold1(TL l,TV v)
		{
			level = l;
			value = v;
		}
};


template<class TS,CH chs,class TD,CH chd,class TL,class TV>
struct Threshold2 :public fe2< TS, chs, TD, chd >   
{
		TV value;
		TL level;
		XMM128  mLevel;
  	    XMM128  mValue;
		Threshold2(TL l)
		{
			level = l;
		}
		Threshold2(TL l,TV v)
		{
			level = l;
			value = v;
		}
};


struct Threshold_8u_C1R_LT: public Threshold2<Fw8u,C1,Fw8u,C1,Fw8u,Fw8u>
{
			
			Threshold_8u_C1R_LT(Fw8u l): Threshold2<Fw8u,C1,Fw8u,C1,Fw8u,Fw8u>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_c1(mLevel.i,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_8u_c1_lt(r.src1[0].i,mLevel.i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_1_lt(s,C1,d,C1,level);
			} 	
};

struct Threshold_8u_C1R_GT: public Threshold2<Fw8u,C1,Fw8u,C1,Fw8u,Fw8u>
{
			
			Threshold_8u_C1R_GT(Fw8u l): Threshold2<Fw8u,C1,Fw8u,C1,Fw8u,Fw8u>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_c1(mLevel.i,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_8u_c1_gt(r.src1[0].i,mLevel.i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_1_gt(s,C1,d,C1,level);
			} 	
};




struct Threshold_16s_C1R_LT: public Threshold2<Fw16s,C1,Fw16s,C1,Fw16s,Fw16s>
{
			
			Threshold_16s_C1R_LT(Fw16s l): Threshold2<Fw16s,C1,Fw16s,C1,Fw16s,Fw16s>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_c1(mLevel.i,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_16s_c1_lt(r.src1[0].i,mLevel.i);
			}      
			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_1_lt(s,C1,d,C1,level);
			} 	
};

struct Threshold_16s_C1R_GT: public Threshold2<Fw16s,C1,Fw16s,C1,Fw16s,Fw16s>
{
			
			Threshold_16s_C1R_GT(Fw16s l): Threshold2<Fw16s,C1,Fw16s,C1,Fw16s,Fw16s>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_c1(mLevel.i,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_16s_c1_gt(r.src1[0].i,mLevel.i);
			}      
			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_1_gt(s,C1,d,C1,level);
			} 	
};


struct Threshold_32f_C1R_LT: public Threshold2<Fw32f,C1,Fw32f,C1,Fw32f,Fw32f>
{
			
			Threshold_32f_C1R_LT(Fw32f l): Threshold2<Fw32f,C1,Fw32f,C1,Fw32f,Fw32f>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_c1(mLevel.f,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = Th_sse2_32f_c1_lt(r.src1[0].f,mLevel.f);
			}      
			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_1_lt(s,C1,d,C1,level);
			} 	
};

struct Threshold_32f_C1R_GT: public Threshold2<Fw32f,C1,Fw32f,C1,Fw32f,Fw32f>
{
			
			Threshold_32f_C1R_GT(Fw32f l): Threshold2<Fw32f,C1,Fw32f,C1,Fw32f,Fw32f>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_c1(mLevel.f,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = Th_sse2_32f_c1_gt(r.src1[0].f,mLevel.f);
			}      
			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_1_gt(s,C1,d,C1,level);
			} 	
};


struct Threshold_8u_C3R_LT: public fe2< Fw8u, C3, Fw8u, C3 >   
{
			XMM128 mLevel[3];
			Fw8u level[3];

			Threshold_8u_C3R_LT(const Fw8u l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_8u_c3_lt(r.src1[0].i,r.src1[1].i,r.src1[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(s,d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_8u_C3R_GT: public fe2< Fw8u, C3, Fw8u, C3 >   
{
			XMM128 mLevel[3];
			Fw8u level[3];

			Threshold_8u_C3R_GT(const Fw8u l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_8u_c3_gt(r.src1[0].i,r.src1[1].i,r.src1[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(s,d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_16s_C3R_LT: public fe2< Fw16s, C3, Fw16s, C3 >   
{
			XMM128 mLevel[3];
			Fw16s level[3];

			Threshold_16s_C3R_LT(const Fw16s l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_16s_c3_lt(r.src1[0].i,r.src1[1].i,r.src1[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i);
			}      

			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(s,d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_16s_C3R_GT: public fe2< Fw16s, C3, Fw16s, C3 >   
{
			XMM128 mLevel[3];
			Fw16s level[3];

			Threshold_16s_C3R_GT(const Fw16s l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_16s_c3_gt(r.src1[0].i,r.src1[1].i,r.src1[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i);
			}      

			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(s,d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_32f_C3R_LT: public fe2< Fw32f, C3, Fw32f, C3 >   
{
			XMM128 mLevel[3];
			Fw32f level[3];

			Threshold_32f_C3R_LT(const Fw32f l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_c3(mLevel[0].f,mLevel[1].f,mLevel[2].f,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_32f_c3_lt(r.src1[0].f,r.src1[1].f,r.src1[2].f,r.dst[0].f,r.dst[1].f,r.dst[2].f,mLevel[0].f,mLevel[1].f,mLevel[2].f);
			}      

			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(s,d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_32f_C3R_GT: public fe2< Fw32f, C3, Fw32f, C3 >   
{
			XMM128 mLevel[3];
			Fw32f level[3];

			Threshold_32f_C3R_GT(const Fw32f l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_c3(mLevel[0].f,mLevel[1].f,mLevel[2].f,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_32f_c3_gt(r.src1[0].f,r.src1[1].f,r.src1[2].f,r.dst[0].f,r.dst[1].f,r.dst[2].f,mLevel[0].f,mLevel[1].f,mLevel[2].f);
			}      

			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(s,d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_8u_AC4R_LT: public fe2< Fw8u, AC4, Fw8u, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw8u level[3];

			Threshold_8u_AC4R_LT(const Fw8u l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_ac4(mMask.i,mLevel.i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_8u_ac4_lt(r.src1[0].i,r.dst[0].i,mLevel.i,mMask.i);
			}      

			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(s,d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_8u_AC4R_GT: public fe2< Fw8u, AC4, Fw8u, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw8u level[3];

			Threshold_8u_AC4R_GT(const Fw8u l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_ac4(mMask.i,mLevel.i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_8u_ac4_gt(r.src1[0].i,r.dst[0].i,mLevel.i,mMask.i);
			}      

			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(s,d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_16s_AC4R_LT: public fe2< Fw16s, AC4, Fw16s, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw16s level[3];

			Threshold_16s_AC4R_LT(const Fw16s l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_ac4(mMask.i,mLevel.i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_16s_ac4_lt(r.src1[0].i,r.dst[0].i,mLevel.i,mMask.i);
			}      

			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(s,d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_16s_AC4R_GT: public fe2< Fw16s, AC4, Fw16s, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw16s level[3];

			Threshold_16s_AC4R_GT(const Fw16s l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_ac4(mMask.i,mLevel.i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_16s_ac4_gt(r.src1[0].i,r.dst[0].i,mLevel.i,mMask.i);
			}      

			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(s,d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_32f_AC4R_LT: public fe2< Fw32f, AC4, Fw32f, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw32f level[3];

			Threshold_32f_AC4R_LT(const Fw32f l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_ac4(mMask.i,mLevel.f,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = Th_sse2_32f_ac4_lt(r.src1[0].f,r.dst[0].f,mLevel.f,mMask.f);
			}      

			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(s,d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_32f_AC4R_GT: public fe2< Fw32f, AC4, Fw32f, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw32f level[3];

			Threshold_32f_AC4R_GT(const Fw32f l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_ac4(mMask.i,mLevel.f,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = Th_sse2_32f_ac4_gt(r.src1[0].f,r.dst[0].f,mLevel.f,mMask.f);
			}      

			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(s,d,level[0],level[1],level[2]);
			} 	
};



// in place 
struct Threshold_8u_C1IR_LT: public Threshold1<Fw8u,C1,Fw8u,Fw8u>
{
			
			Threshold_8u_C1IR_LT(Fw8u l): Threshold1<Fw8u,C1,Fw8u,Fw8u>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_c1(mLevel.i,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_8u_c1_lt(r.dst[0].i,mLevel.i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_1_lt(d,C1,level);
			} 	
};

struct Threshold_8u_C1IR_GT: public Threshold1<Fw8u,C1,Fw8u,Fw8u>
{
			
			Threshold_8u_C1IR_GT(Fw8u l): Threshold1<Fw8u,C1,Fw8u,Fw8u>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_c1(mLevel.i,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_8u_c1_gt(r.dst[0].i,mLevel.i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_1_gt(d,C1,level);
			} 	
};




struct Threshold_16s_C1IR_LT: public Threshold1<Fw16s,C1,Fw16s,Fw16s>
{
			
			Threshold_16s_C1IR_LT(Fw16s l): Threshold1<Fw16s,C1,Fw16s,Fw16s>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_c1(mLevel.i,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_16s_c1_lt(r.dst[0].i,mLevel.i);
			}      
			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_1_lt(d,C1,level);
			} 	
};

struct Threshold_16s_C1IR_GT: public Threshold1<Fw16s,C1,Fw16s,Fw16s>
{
			
			Threshold_16s_C1IR_GT(Fw16s l): Threshold1<Fw16s,C1,Fw16s,Fw16s>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_c1(mLevel.i,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_16s_c1_gt(r.dst[0].i,mLevel.i);
			}      
			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_1_gt(d,C1,level);
			} 	
};


struct Threshold_32f_C1IR_LT: public Threshold1<Fw32f,C1,Fw32f,Fw32f>
{
			
			Threshold_32f_C1IR_LT(Fw32f l): Threshold1<Fw32f,C1,Fw32f,Fw32f>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_c1(mLevel.f,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = Th_sse2_32f_c1_lt(r.dst[0].f,mLevel.f);
			}      
			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_1_lt(d,C1,level);
			} 	
};

struct Threshold_32f_C1IR_GT: public Threshold1<Fw32f,C1,Fw32f,Fw32f>
{
			
			Threshold_32f_C1IR_GT(Fw32f l): Threshold1<Fw32f,C1,Fw32f,Fw32f>(l){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_c1(mLevel.f,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = Th_sse2_32f_c1_gt(r.dst[0].f,mLevel.f);
			}      
			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_1_gt(d,C1,level);
			} 	
};


struct Threshold_8u_C3IR_LT: public fe1< Fw8u, C3>   
{
			XMM128 mLevel[3];
			Fw8u level[3];

			Threshold_8u_C3IR_LT(const Fw8u l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_8u_c3_lt(r.dst[0].i,r.dst[1].i,r.dst[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_8u_C3IR_GT: public fe1< Fw8u, C3>   
{
			XMM128 mLevel[3];
			Fw8u level[3];

			Threshold_8u_C3IR_GT(const Fw8u l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_8u_c3_gt(r.dst[0].i,r.dst[1].i,r.dst[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_16s_C3IR_LT: public fe1< Fw16s, C3>   
{
			XMM128 mLevel[3];
			Fw16s level[3];

			Threshold_16s_C3IR_LT(const Fw16s l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_16s_c3_lt(r.dst[0].i,r.dst[1].i,r.dst[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i);
			}      

			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_16s_C3IR_GT: public fe1< Fw16s, C3>   
{
			XMM128 mLevel[3];
			Fw16s level[3];

			Threshold_16s_C3IR_GT(const Fw16s l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_16s_c3_gt(r.dst[0].i,r.dst[1].i,r.dst[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i);
			}      

			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_32f_C3IR_LT: public fe1< Fw32f, C3 >   
{
			XMM128 mLevel[3];
			Fw32f level[3];

			Threshold_32f_C3IR_LT(const Fw32f l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_c3(mLevel[0].f,mLevel[1].f,mLevel[2].f,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_32f_c3_lt(r.dst[0].f,r.dst[1].f,r.dst[2].f,r.dst[0].f,r.dst[1].f,r.dst[2].f,mLevel[0].f,mLevel[1].f,mLevel[2].f);
			}      

			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_32f_C3IR_GT: public fe1< Fw32f, C3>   
{
			XMM128 mLevel[3];
			Fw32f level[3];

			Threshold_32f_C3IR_GT(const Fw32f l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_c3(mLevel[0].f,mLevel[1].f,mLevel[2].f,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				Th_sse2_32f_c3_gt(r.dst[0].f,r.dst[1].f,r.dst[2].f,r.dst[0].f,r.dst[1].f,r.dst[2].f,mLevel[0].f,mLevel[1].f,mLevel[2].f);
			}      

			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_8u_AC4IR_LT: public fe1< Fw8u, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw8u level[3];

			Threshold_8u_AC4IR_LT(const Fw8u l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_ac4(mMask.i,mLevel.i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_8u_ac4_lt(r.dst[0].i,r.dst[0].i,mLevel.i,mMask.i);
			}      

			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_8u_AC4IR_GT: public fe1< Fw8u, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw8u level[3];

			Threshold_8u_AC4IR_GT(const Fw8u l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_8u_ac4(mMask.i,mLevel.i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_8u_ac4_gt(r.dst[0].i,r.dst[0].i,mLevel.i,mMask.i);
			}      

			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_16s_AC4IR_LT: public fe1< Fw16s, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw16s level[3];

			Threshold_16s_AC4IR_LT(const Fw16s l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_ac4(mMask.i,mLevel.i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_16s_ac4_lt(r.dst[0].i,r.dst[0].i,mLevel.i,mMask.i);
			}      

			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_16s_AC4IR_GT: public fe1< Fw16s, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw16s level[3];

			Threshold_16s_AC4IR_GT(const Fw16s l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_16s_ac4(mMask.i,mLevel.i,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = Th_sse2_16s_ac4_gt(r.dst[0].i,r.dst[0].i,mLevel.i,mMask.i);
			}      

			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(d,level[0],level[1],level[2]);
			} 	
};


struct Threshold_32f_AC4IR_LT: public fe1< Fw32f, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw32f level[3];

			Threshold_32f_AC4IR_LT(const Fw32f l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_ac4(mMask.i,mLevel.f,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = Th_sse2_32f_ac4_lt(r.dst[0].f,r.dst[0].f,mLevel.f,mMask.f);
			}      

			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_lt(d,level[0],level[1],level[2]);
			} 	
};

struct Threshold_32f_AC4IR_GT: public fe1< Fw32f, AC4 >   
{
			XMM128 mLevel;
			XMM128 mMask;
			Fw32f level[3];

			Threshold_32f_AC4IR_GT(const Fw32f l[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				Th_setup_32f_ac4(mMask.i,mLevel.f,level[0],level[1],level[2]);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = Th_sse2_32f_ac4_gt(r.dst[0].f,r.dst[0].f,mLevel.f,mMask.f);
			}      

			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_gt(d,level[0],level[1],level[2]);
			} 	
};


// threshold val


struct Threshold_Val_8u_C1R_LT: public Threshold2<Fw8u,C1,Fw8u,C1,Fw8u,Fw8u>
{
			
			Threshold_Val_8u_C1R_LT(Fw8u l,Fw8u v): Threshold2<Fw8u,C1,Fw8u,C1,Fw8u,Fw8u>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c1(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_c1_lt(r.src1[0].i,mLevel.i,mValue.i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_8u_C1R_GT: public Threshold2<Fw8u,C1,Fw8u,C1,Fw8u,Fw8u>
{
			
			Threshold_Val_8u_C1R_GT(Fw8u l,Fw8u v): Threshold2<Fw8u,C1,Fw8u,C1,Fw8u,Fw8u>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c1(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_c1_gt(r.src1[0].i,mLevel.i,mValue.i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_gt(s,d,level,value);
			} 	
};




struct Threshold_Val_16s_C1R_LT: public Threshold2<Fw16s,C1,Fw16s,C1,Fw16s,Fw16s>
{
			
			Threshold_Val_16s_C1R_LT(Fw16s l,Fw16s v): Threshold2<Fw16s,C1,Fw16s,C1,Fw16s,Fw16s>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c1(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_c1_lt(r.src1[0].i,mLevel.i,mValue.i);
			}      
			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_16s_C1R_GT: public Threshold2<Fw16s,C1,Fw16s,C1,Fw16s,Fw16s>
{
			
			Threshold_Val_16s_C1R_GT(Fw16s l,Fw16s v): Threshold2<Fw16s,C1,Fw16s,C1,Fw16s,Fw16s>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c1(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_c1_gt(r.src1[0].i,mLevel.i,mValue.i);
			}      
			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_gt(s,d,level,value);
			} 	
};


struct Threshold_Val_32f_C1R_LT: public Threshold2<Fw32f,C1,Fw32f,C1,Fw32f,Fw32f>
{
			
			Threshold_Val_32f_C1R_LT(Fw32f  l,Fw32f  v): Threshold2<Fw32f,C1,Fw32f,C1,Fw32f,Fw32f>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c1(mLevel.f,mValue.f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_c1_lt(r.src1[0].f,mLevel.f,mValue.f);
			}      
			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_32f_C1R_GT: public Threshold2<Fw32f,C1,Fw32f,C1,Fw32f,Fw32f>
{
			
			Threshold_Val_32f_C1R_GT(Fw32f  l,Fw32f  v): Threshold2<Fw32f,C1,Fw32f,C1,Fw32f,Fw32f>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c1(mLevel.f,mValue.f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_c1_gt(r.src1[0].f,mLevel.f,mValue.f);
			}      
			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_gt(s,d,level,value);
			} 	
};


struct Threshold_Val_8u_C3R_LT: public fe2< Fw8u, C3, Fw8u, C3 >   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw8u level[3], value[3];

			Threshold_Val_8u_C3R_LT(const Fw8u l[3],const Fw8u v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_8u_c3_lt(r.src1[0].i,r.src1[1].i,r.src1[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(s,d,level,value);
			} 	
};


struct Threshold_Val_8u_C3R_GT: public fe2< Fw8u, C3, Fw8u, C3 >   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw8u level[3], value[3];

			Threshold_Val_8u_C3R_GT(const Fw8u l[3],const Fw8u  v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_8u_c3_gt(r.src1[0].i,r.src1[1].i,r.src1[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(s,d,level,value);
			} 	
};


struct Threshold_Val_16s_C3R_LT: public fe2< Fw16s, C3, Fw16s, C3 >   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw16s level[3], value[3];

			Threshold_Val_16s_C3R_LT(const Fw16s l[3],const Fw16s  v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_16s_c3_lt(r.src1[0].i,r.src1[1].i,r.src1[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i);
			}      

			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(s,d,level,value);
			} 	
};


struct Threshold_Val_16s_C3R_GT: public fe2< Fw16s, C3, Fw16s, C3 >   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw16s level[3], value[3];

			Threshold_Val_16s_C3R_GT(const Fw16s l[3],const Fw16s v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_16s_c3_gt(r.src1[0].i,r.src1[1].i,r.src1[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i);
			}      

			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(s,d,level,value);
			} 	
};


struct Threshold_Val_32f_C3R_LT: public fe2< Fw32f, C3, Fw32f, C3 >   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw32f  level[3],  value[3];

			Threshold_Val_32f_C3R_LT(const Fw32f  l[3],const Fw32f   v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c3(mLevel[0].f,mLevel[1].f,mLevel[2].f,mValue[0].f,mValue[1].f,mValue[2].f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_32f_c3_lt(r.src1[0].f,r.src1[1].f,r.src1[2].f,r.dst[0].f,r.dst[1].f,r.dst[2].f,mLevel[0].f,mLevel[1].f,mLevel[2].f,mValue[0].f,mValue[1].f,mValue[2].f);
			}      

			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_32f_C3R_GT: public fe2< Fw32f, C3, Fw32f, C3 >   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw32f  level[3],  value[3];

			Threshold_Val_32f_C3R_GT(const Fw32f  l[3],const Fw32f  v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c3(mLevel[0].f,mLevel[1].f,mLevel[2].f,mValue[0].f,mValue[1].f,mValue[2].f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_32f_c3_gt(r.src1[0].f,r.src1[1].f,r.src1[2].f,r.dst[0].f,r.dst[1].f,r.dst[2].f,mLevel[0].f,mLevel[1].f,mLevel[2].f,mValue[0].f,mValue[1].f,mValue[2].f);
			}      

			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(s,d,level,value);
			} 	
};


struct Threshold_Val_8u_AC4R_LT: public fe2< Fw8u, AC4, Fw8u, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw8u level[3], value[3];

			Threshold_Val_8u_AC4R_LT(const Fw8u l[3],const Fw8u v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_ac4(mLevel.i,mValue.i,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_ac4_lt(r.src1[0].i,r.dst[0].i,mLevel.i,mValue.i,mMask.i);
			}      

			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_8u_AC4R_GT: public fe2< Fw8u, AC4, Fw8u, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw8u level[3], value[3];

			Threshold_Val_8u_AC4R_GT(const Fw8u l[3],const Fw8u v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_ac4(mLevel.i,mValue.i,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_ac4_gt(r.src1[0].i,r.dst[0].i,mLevel.i,mValue.i,mMask.i);
			}      

			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(s,d,level,value);
			} 	
};

struct Threshold_Val_16s_AC4R_LT: public fe2< Fw16s, AC4, Fw16s, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw16s level[3], value[3];

			Threshold_Val_16s_AC4R_LT(const Fw16s l[3],const Fw16s v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_ac4(mLevel.i,mValue.i,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_ac4_lt(r.src1[0].i,r.dst[0].i,mLevel.i,mValue.i,mMask.i);
			}      

			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_16s_AC4R_GT: public fe2< Fw16s, AC4, Fw16s, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw16s level[3], value[3];

			Threshold_Val_16s_AC4R_GT(const Fw16s l[3],const Fw16s v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_ac4(mLevel.i,mValue.i,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_ac4_gt(r.src1[0].i,r.dst[0].i,mLevel.i,mValue.i,mMask.i);
			}      

			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(s,d,level,value);
			} 	
};


struct Threshold_Val_32f_AC4R_LT: public fe2< Fw32f, AC4, Fw32f, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw32f  level[3],  value[3];

			Threshold_Val_32f_AC4R_LT(const Fw32f  l[3],const Fw32f  v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_ac4(mLevel.f,mValue.f,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_ac4_lt(r.src1[0].f,r.dst[0].f,mLevel.f,mValue.f,mMask.f);
			}      

			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_32f_AC4R_GT: public fe2< Fw32f, AC4, Fw32f, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw32f  level[3],  value[3];

			Threshold_Val_32f_AC4R_GT(const Fw32f  l[3],const Fw32f  v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_ac4(mLevel.f,mValue.f,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_ac4_gt(r.src1[0].f,r.dst[0].f,mLevel.f,mValue.f,mMask.f);
			}      

			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(s,d,level,value);
			} 	
};



// in place 
struct Threshold_Val_8u_C1IR_LT: public Threshold1<Fw8u,C1,Fw8u,Fw8u>
{
			
			Threshold_Val_8u_C1IR_LT(Fw8u l,Fw8u v): Threshold1<Fw8u,C1,Fw8u,Fw8u>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c1(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_c1_lt(r.dst[0].i,mLevel.i,mValue.i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_lt(d,level,value);
			} 	
};

struct Threshold_Val_8u_C1IR_GT: public Threshold1<Fw8u,C1,Fw8u,Fw8u>
{
			
			Threshold_Val_8u_C1IR_GT(Fw8u l,Fw8u v): Threshold1<Fw8u,C1,Fw8u,Fw8u>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c1(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_c1_gt(r.dst[0].i,mLevel.i,mValue.i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_gt(d,level,value);
			} 	
};




struct Threshold_Val_16s_C1IR_LT: public Threshold1<Fw16s,C1,Fw16s,Fw16s>
{
			
			Threshold_Val_16s_C1IR_LT(Fw16s l,Fw16s v): Threshold1<Fw16s,C1,Fw16s,Fw16s>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c1(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_c1_lt(r.dst[0].i,mLevel.i,mValue.i);
			}      
			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_lt(d,level,value);
			} 	
};

struct Threshold_Val_16s_C1IR_GT: public Threshold1<Fw16s,C1,Fw16s,Fw16s>
{
			
			Threshold_Val_16s_C1IR_GT(Fw16s l,Fw16s v): Threshold1<Fw16s,C1,Fw16s,Fw16s>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c1(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_c1_gt(r.dst[0].i,mLevel.i,mValue.i);
			}      
			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_gt(d,level,value);
			} 	
};


struct Threshold_Val_32f_C1IR_LT: public Threshold1<Fw32f,C1,Fw32f,Fw32f>
{
			
			Threshold_Val_32f_C1IR_LT(Fw32f  l,Fw32f  v): Threshold1<Fw32f,C1,Fw32f,Fw32f>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c1(mLevel.f,mValue.f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_c1_lt(r.dst[0].f,mLevel.f,mValue.f);
			}      
			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_lt(d,level,value);
			} 	
};

struct Threshold_Val_32f_C1IR_GT: public Threshold1<Fw32f,C1,Fw32f,Fw32f>
{
			
			Threshold_Val_32f_C1IR_GT(Fw32f  l,Fw32f  v): Threshold1<Fw32f,C1,Fw32f,Fw32f>(l,v){}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c1(mLevel.f,mValue.f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_c1_gt(r.dst[0].f,mLevel.f,mValue.f);
			}      
			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_1_gt(d,level,value);
			} 	
};


struct Threshold_Val_8u_C3IR_LT: public fe1< Fw8u, C3>   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw8u level[3], value[3];

			Threshold_Val_8u_C3IR_LT(const Fw8u l[3],const Fw8u v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_8u_c3_lt(r.dst[0].i,r.dst[1].i,r.dst[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(d,level,value);
			} 	
};


struct Threshold_Val_8u_C3IR_GT: public fe1< Fw8u, C3>   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw8u level[3], value[3];

			Threshold_Val_8u_C3IR_GT(const Fw8u l[3],const Fw8u v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_8u_c3_gt(r.dst[0].i,r.dst[1].i,r.dst[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(d,level,value);
			} 	
};


struct Threshold_Val_16s_C3IR_LT: public fe1< Fw16s, C3>   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw16s level[3], value[3];

			Threshold_Val_16s_C3IR_LT(const Fw16s l[3],const Fw16s v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_16s_c3_lt(r.dst[0].i,r.dst[1].i,r.dst[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i);
			}      

			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(d,level,value);
			} 	
};


struct Threshold_Val_16s_C3IR_GT: public fe1< Fw16s, C3>   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw16s level[3], value[3];

			Threshold_Val_16s_C3IR_GT(const Fw16s l[3],const Fw16s v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c3(mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_16s_c3_gt(r.dst[0].i,r.dst[1].i,r.dst[2].i,r.dst[0].i,r.dst[1].i,r.dst[2].i,mLevel[0].i,mLevel[1].i,mLevel[2].i,mValue[0].i,mValue[1].i,mValue[2].i);
			}      

			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(d,level,value);
			} 	
};


struct Threshold_Val_32f_C3IR_LT: public fe1< Fw32f, C3 >   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw32f  level[3],  value[3];

			Threshold_Val_32f_C3IR_LT(const Fw32f  l[3],const Fw32f  v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c3(mLevel[0].f,mLevel[1].f,mLevel[2].f,mValue[0].f,mValue[1].f,mValue[2].f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_32f_c3_lt(r.dst[0].f,r.dst[1].f,r.dst[2].f,r.dst[0].f,r.dst[1].f,r.dst[2].f,mLevel[0].f,mLevel[1].f,mLevel[2].f,mValue[0].f,mValue[1].f,mValue[2].f);
			}      

			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(d,level,value);
			} 	
};

struct Threshold_Val_32f_C3IR_GT: public fe1< Fw32f, C3>   
{
			XMM128 mLevel[3];XMM128 mValue[3];
			Fw32f  level[3],  value[3];

			Threshold_Val_32f_C3IR_GT(const Fw32f  l[3],const Fw32f  v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c3(mLevel[0].f,mLevel[1].f,mLevel[2].f,mValue[0].f,mValue[1].f,mValue[2].f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				ThVal_sse2_32f_c3_gt(r.dst[0].f,r.dst[1].f,r.dst[2].f,r.dst[0].f,r.dst[1].f,r.dst[2].f,mLevel[0].f,mLevel[1].f,mLevel[2].f,mValue[0].f,mValue[1].f,mValue[2].f);
			}      

			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(d,level,value);
			} 	
};


struct Threshold_Val_8u_AC4IR_LT: public fe1< Fw8u, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw8u level[3], value[3];

			Threshold_Val_8u_AC4IR_LT(const Fw8u l[3],const Fw8u v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_ac4(mLevel.i,mValue.i,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_ac4_lt(r.dst[0].i,r.dst[0].i,mLevel.i,mValue.i,mMask.i);
			}      

			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(d,level,value);
			} 	
};

struct Threshold_Val_8u_AC4IR_GT: public fe1< Fw8u, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw8u level[3], value[3];

			Threshold_Val_8u_AC4IR_GT(const Fw8u l[3],const Fw8u v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_ac4(mLevel.i,mValue.i,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_ac4_gt(r.dst[0].i,r.dst[0].i,mLevel.i,mValue.i,mMask.i);
			}      

			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(d,level,value);
			} 	
};

struct Threshold_Val_16s_AC4IR_LT: public fe1< Fw16s, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw16s level[3], value[3];

			Threshold_Val_16s_AC4IR_LT(const Fw16s l[3],const Fw16s v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_ac4(mLevel.i,mValue.i,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_ac4_lt(r.dst[0].i,r.dst[0].i,mLevel.i,mValue.i,mMask.i);
			}      

			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(d,level,value);
			} 	
};

struct Threshold_Val_16s_AC4IR_GT: public fe1< Fw16s, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw16s level[3], value[3];

			Threshold_Val_16s_AC4IR_GT(const Fw16s l[3],const Fw16s v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_ac4(mLevel.i,mValue.i,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_ac4_gt(r.dst[0].i,r.dst[0].i,mLevel.i,mValue.i,mMask.i);
			}      

			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(d,level,value);
			} 	
};


struct Threshold_Val_32f_AC4IR_LT: public fe1< Fw32f, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw32f  level[3],  value[3];

			Threshold_Val_32f_AC4IR_LT(const Fw32f  l[3], const Fw32f  v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_ac4(mLevel.f,mValue.f,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_ac4_lt(r.dst[0].f,r.dst[0].f,mLevel.f,mValue.f,mMask.f);
			}      

			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_lt(d,level,value);
			} 	
};

struct Threshold_Val_32f_AC4IR_GT: public fe1< Fw32f, AC4 >   
{
			XMM128 mLevel;
			XMM128 mValue;
			XMM128 mMask;
			Fw32f  level[3],  value[3];

			Threshold_Val_32f_AC4IR_GT(const Fw32f  l[3],const Fw32f   v[3])
			{
				level[0]=l[0];level[1]=l[1];level[2]=l[2];value[0]=v[0];value[1]=v[1];value[2]=v[2];
			}
			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_ac4(mLevel.f,mValue.f,mMask.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_ac4_gt(r.dst[0].f,r.dst[0].f,mLevel.f,mValue.f,mMask.f);
			}      

			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_gt(d,level,value);
			} 	
};


//////C4


struct Threshold_Val_8u_C4R_LT: public fe2<Fw8u,C4,Fw8u,C4>
{
			XMM128 mLevel;
			XMM128 mValue;
			Fw8u level[4], value[4];
			
			Threshold_Val_8u_C4R_LT(const Fw8u l[4],const Fw8u v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c4(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_c1_lt(r.src1[0].i,mLevel.i,mValue.i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_8u_C4R_GT: public fe2<Fw8u,C4,Fw8u,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw8u level[4], value[4];
			
			Threshold_Val_8u_C4R_GT(const Fw8u l[4],const Fw8u v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c4(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_c1_gt(r.src1[0].i,mLevel.i,mValue.i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_gt(s,d,level,value);
			} 	
};




struct Threshold_Val_16s_C4R_LT: public fe2<Fw16s,C4,Fw16s,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw16s level[4], value[4];
			
			Threshold_Val_16s_C4R_LT(const Fw16s l[4],const Fw16s v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c4(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_c1_lt(r.src1[0].i,mLevel.i,mValue.i);
			}      
			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_16s_C4R_GT: public fe2<Fw16s,C4,Fw16s,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw16s level[4], value[4];
			
			Threshold_Val_16s_C4R_GT(const Fw16s l[4],const Fw16s v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c4(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_c1_gt(r.src1[0].i,mLevel.i,mValue.i);
			}      
			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_gt(s,d,level,value);
			} 	
};


struct Threshold_Val_32f_C4R_LT: public fe2<Fw32f,C4,Fw32f,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw32f level[4], value[4];
			
			Threshold_Val_32f_C4R_LT(const Fw32f l[4],const Fw32f v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c4(mLevel.f,mValue.f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_c1_lt(r.src1[0].f,mLevel.f,mValue.f);
			}      
			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_lt(s,d,level,value);
			} 	
};

struct Threshold_Val_32f_C4R_GT: public fe2<Fw32f,C4,Fw32f,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw32f level[4], value[4];
			
			Threshold_Val_32f_C4R_GT(const Fw32f l[4],const Fw32f v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c4(mLevel.f,mValue.f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_c1_gt(r.src1[0].f,mLevel.f,mValue.f);
			}      
			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_gt(s,d,level,value);
			} 	
};


//// C4 In place

struct Threshold_Val_8u_C4IR_LT: public fe1<Fw8u,C4>
{
			XMM128 mLevel;
			XMM128 mValue;
			Fw8u level[4], value[4];
			
			Threshold_Val_8u_C4IR_LT(const Fw8u l[4],const Fw8u v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c4(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_c1_lt(r.dst[0].i,mLevel.i,mValue.i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_lt(d,level,value);
			} 	
};

struct Threshold_Val_8u_C4IR_GT: public fe1<Fw8u,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw8u level[4], value[4];
			
			Threshold_Val_8u_C4IR_GT(const Fw8u l[4],const Fw8u v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_8u_c4(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_8u_c1_gt(r.dst[0].i,mLevel.i,mValue.i);
			}      
			IV REFR( Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_gt(d,level,value);
			} 	
};




struct Threshold_Val_16s_C4IR_LT: public fe1<Fw16s,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw16s level[4], value[4];
			
			Threshold_Val_16s_C4IR_LT(const Fw16s l[4],const Fw16s v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c4(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_c1_lt(r.dst[0].i,mLevel.i,mValue.i);
			}      
			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_lt(d,level,value);
			} 	
};

struct Threshold_Val_16s_C4IR_GT: public fe1<Fw16s,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw16s level[4], value[4];
			
			Threshold_Val_16s_C4IR_GT(const Fw16s l[4],const Fw16s v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_16s_c4(mLevel.i,mValue.i,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThVal_sse2_16s_c1_gt(r.dst[0].i,mLevel.i,mValue.i);
			}      
			IV REFR( Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_gt(d,level,value);
			} 	
};


struct Threshold_Val_32f_C4IR_LT: public fe1<Fw32f,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw32f level[4], value[4];
			
			Threshold_Val_32f_C4IR_LT(const Fw32f l[4],const Fw32f v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c4(mLevel.f,mValue.f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_c1_lt(r.dst[0].f,mLevel.f,mValue.f);
			}      
			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_lt(d,level,value);
			} 	
};

struct Threshold_Val_32f_C4IR_GT: public fe1<Fw32f,C4>
{
			
			XMM128 mLevel;
			XMM128 mValue;
			Fw32f level[4], value[4];
			
			Threshold_Val_32f_C4IR_GT(const Fw32f l[4],const Fw32f v[4])
			{
				for(int i=0;i<4;i++)
				{
					level[i] = l[i];
					value[i] = v[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThVal_setup_32f_c4(mLevel.f,mValue.f,level,value);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThVal_sse2_32f_c1_gt(r.dst[0].f,mLevel.f,mValue.f);
			}      
			IV REFR( Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_Val_c4_gt(d,level,value);
			} 	
};


struct Threshold_LTValGTVal_8u_C1R: public fe2<Fw8u,C1,Fw8u,C1>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			Fw8u levelLt, valueLt,levelGt,valueGt;
			
			Threshold_LTValGTVal_8u_C1R(const Fw8u llt,const Fw8u vlt,const Fw8u lgt,const Fw8u vgt)
			{
				levelLt = llt;
				valueLt = vlt;
				levelGt = lgt;
				valueGt = vgt;
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_8u_c1(mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i,levelLt,valueLt,levelGt,valueGt);

			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThLTGT_sse2_8u(r.src1[0].i,mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i);
			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal_1(s,d,levelLt,valueLt,levelGt,valueGt);
			} 	
};

struct Threshold_LTValGTVal_8u_C1IR: public fe1<Fw8u,C1>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			Fw8u levelLt, valueLt,levelGt,valueGt;
			
			Threshold_LTValGTVal_8u_C1IR(const Fw8u llt,const Fw8u vlt,const Fw8u lgt,const Fw8u vgt)
			{
				levelLt = llt;
				valueLt = vlt;
				levelGt = lgt;
				valueGt = vgt;
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_8u_c1(mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i,levelLt,valueLt,levelGt,valueGt);

			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThLTGT_sse2_8u(r.dst[0].i,mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i);
			}      
			IV REFR(Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal_1(d,levelLt,valueLt,levelGt,valueGt);
			} 	
};


struct Threshold_LTValGTVal_16s_C1R: public fe2<Fw16s,C1,Fw16s,C1>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			Fw16s levelLt, valueLt,levelGt,valueGt;
			
			Threshold_LTValGTVal_16s_C1R(const Fw16s llt,const Fw16s vlt,const Fw16s lgt,const Fw16s vgt)
			{
				levelLt = llt;
				valueLt = vlt;
				levelGt = lgt;
				valueGt = vgt;
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_16s_c1(mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i,levelLt,valueLt,levelGt,valueGt);

			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThLTGT_sse2_16s(r.src1[0].i,mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i);
			}      
			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal_1(s,d,levelLt,valueLt,levelGt,valueGt);
			} 	
};

struct Threshold_LTValGTVal_16s_C1IR: public fe1<Fw16s,C1>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			Fw16s levelLt, valueLt,levelGt,valueGt;
			
			Threshold_LTValGTVal_16s_C1IR(const Fw16s llt,const Fw16s vlt,const Fw16s lgt,const Fw16s vgt)
			{
				levelLt = llt;
				valueLt = vlt;
				levelGt = lgt;
				valueGt = vgt;
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_16s_c1(mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i,levelLt,valueLt,levelGt,valueGt);

			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThLTGT_sse2_16s(r.dst[0].i,mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i);
			}      
			IV REFR(Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal_1(d,levelLt,valueLt,levelGt,valueGt);
			} 	
};


struct Threshold_LTValGTVal_32f_C1R: public fe2<Fw32f,C1,Fw32f,C1>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			Fw32f levelLt, valueLt,levelGt,valueGt;
			
			Threshold_LTValGTVal_32f_C1R(const Fw32f llt,const Fw32f vlt,const Fw32f lgt,const Fw32f vgt)
			{
				levelLt = llt;
				valueLt = vlt;
				levelGt = lgt;
				valueGt = vgt;
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_32f_c1(mLevelLt.f,mValueLt.f,mLevelGt.f,mValueGt.f,levelLt,valueLt,levelGt,valueGt);

			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThLTGT_sse2_32f(r.src1[0].f,mLevelLt.f,mValueLt.f,mLevelGt.f,mValueGt.f);
			}      
			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal_1(s,d,levelLt,valueLt,levelGt,valueGt);
			} 	
};

struct Threshold_LTValGTVal_32f_C1IR: public fe1<Fw32f,C1>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			Fw32f levelLt, valueLt,levelGt,valueGt;
			
			Threshold_LTValGTVal_32f_C1IR(const Fw32f llt,const Fw32f vlt,const Fw32f lgt,const Fw32f vgt)
			{
				levelLt = llt;
				valueLt = vlt;
				levelGt = lgt;
				valueGt = vgt;
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_32f_c1(mLevelLt.f,mValueLt.f,mLevelGt.f,mValueGt.f,levelLt,valueLt,levelGt,valueGt);

			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThLTGT_sse2_32f(r.dst[0].f,mLevelLt.f,mValueLt.f,mLevelGt.f,mValueGt.f);
			}      
			IV REFR(Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal_1(d,levelLt,valueLt,levelGt,valueGt);
			} 	
};






struct Threshold_LTValGTVal_8u_AC4R: public fe2<Fw8u,AC4,Fw8u,AC4>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			XMM128 mask;
			Fw8u levelLt[3], valueLt[3],levelGt[3],valueGt[3];
			
			Threshold_LTValGTVal_8u_AC4R(const Fw8u llt[3],const Fw8u vlt[3],const Fw8u lgt[3],const Fw8u vgt[3])
			{
				for (int i = 0; i<3 ;i++)
				{
					levelLt[i] = llt[i];
					valueLt[i] = vlt[i];
					levelGt[i] = lgt[i];
					valueGt[i] = vgt[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_8u_ac4(mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i,mask.i,levelLt,valueLt,levelGt,valueGt);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThLTGT_sse2_8u_ac4(r.src1[0].i,r.dst[0].i,mask.i,mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i);

			}      
			IV REFR(const Fw8u *s, Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal(s,d,levelLt,valueLt,levelGt,valueGt);
			} 	
};

struct Threshold_LTValGTVal_8u_AC4IR: public fe1<Fw8u,AC4>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			XMM128 mask;
			Fw8u levelLt[3], valueLt[3],levelGt[3],valueGt[3];
			
			Threshold_LTValGTVal_8u_AC4IR(const Fw8u llt[3],const Fw8u vlt[3],const Fw8u lgt[3],const Fw8u vgt[3])
			{
				for (int i = 0; i<3 ;i++)
				{
					levelLt[i] = llt[i];
					valueLt[i] = vlt[i];
					levelGt[i] = lgt[i];
					valueGt[i] = vgt[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_8u_ac4(mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i,mask.i,levelLt,valueLt,levelGt,valueGt);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThLTGT_sse2_8u_ac4(r.dst[0].i,r.dst[0].i,mask.i,mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i);
			}      
			IV REFR(Fw8u *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal(d,levelLt,valueLt,levelGt,valueGt);
			} 		
};


struct Threshold_LTValGTVal_16s_AC4R: public fe2<Fw16s,AC4,Fw16s,AC4>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			XMM128 mask;
			Fw16s levelLt[3], valueLt[3],levelGt[3],valueGt[3];
			
			Threshold_LTValGTVal_16s_AC4R(const Fw16s llt[3],const Fw16s vlt[3],const Fw16s lgt[3],const Fw16s vgt[3])
			{
				for (int i = 0; i<3 ;i++)
				{
					levelLt[i] = llt[i];
					valueLt[i] = vlt[i];
					levelGt[i] = lgt[i];
					valueGt[i] = vgt[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_16s_ac4(mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i,mask.i,levelLt,valueLt,levelGt,valueGt);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThLTGT_sse2_16s_ac4(r.src1[0].i,r.dst[0].i,mask.i,mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i);

			}      
			IV REFR(const Fw16s *s, Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal(s,d,levelLt,valueLt,levelGt,valueGt);
			} 	
};


struct Threshold_LTValGTVal_16s_AC4IR: public fe1<Fw16s,AC4>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			XMM128 mask;
			Fw16s levelLt[3], valueLt[3],levelGt[3],valueGt[3];
			
			Threshold_LTValGTVal_16s_AC4IR(const Fw16s llt[3],const Fw16s vlt[3],const Fw16s lgt[3],const Fw16s vgt[3])
			{
				for (int i = 0; i<3 ;i++)
				{
					levelLt[i] = llt[i];
					valueLt[i] = vlt[i];
					levelGt[i] = lgt[i];
					valueGt[i] = vgt[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_16s_ac4(mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i,mask.i,levelLt,valueLt,levelGt,valueGt);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = ThLTGT_sse2_16s_ac4(r.dst[0].i,r.dst[0].i,mask.i,mLevelLt.i,mValueLt.i,mLevelGt.i,mValueGt.i);

			}      
			IV REFR(Fw16s *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal(d,levelLt,valueLt,levelGt,valueGt);
			} 	
};


struct Threshold_LTValGTVal_32f_AC4R: public fe2<Fw32f,AC4,Fw32f,AC4>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			XMM128 mask;
			Fw32f levelLt[3], valueLt[3],levelGt[3],valueGt[3];
			
			Threshold_LTValGTVal_32f_AC4R(const Fw32f llt[3],const Fw32f vlt[3],const Fw32f lgt[3],const Fw32f vgt[3])
			{
				for (int i = 0; i<3 ;i++)
				{
					levelLt[i] = llt[i];
					valueLt[i] = vlt[i];
					levelGt[i] = lgt[i];
					valueGt[i] = vgt[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_32f_ac4(mLevelLt.f,mValueLt.f,mLevelGt.f,mValueGt.f,mask.i,levelLt,valueLt,levelGt,valueGt);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThLTGT_sse2_32f_ac4(r.src1[0].f,r.dst[0].f,mask.f,mLevelLt.f,mValueLt.f,mLevelGt.f,mValueGt.f);

			}      
			IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal(s,d,levelLt,valueLt,levelGt,valueGt);
			} 	
};


struct Threshold_LTValGTVal_32f_AC4IR: public fe1<Fw32f,AC4>
{
			
			XMM128 mLevelLt,mLevelGt;
			XMM128 mValueLt,mValueGt;
			XMM128 mask;
			Fw32f levelLt[3], valueLt[3],levelGt[3],valueGt[3];
			
			Threshold_LTValGTVal_32f_AC4IR(const Fw32f llt[3],const Fw32f vlt[3],const Fw32f lgt[3],const Fw32f vgt[3])
			{
				for (int i = 0; i<3 ;i++)
				{
					levelLt[i] = llt[i];
					valueLt[i] = vlt[i];
					levelGt[i] = lgt[i];
					valueGt[i] = vgt[i];
				}
			}

			FE_SSE2_REF 

			IV SSE2_Init() 
			{
				ThLTGT_setup_32f_ac4(mLevelLt.f,mValueLt.f,mLevelGt.f,mValueGt.f,mask.i,levelLt,valueLt,levelGt,valueGt);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = ThLTGT_sse2_32f_ac4(r.dst[0].f,r.dst[0].f,mask.f,mLevelLt.f,mValueLt.f,mLevelGt.f,mValueGt.f);

			}      
			IV REFR(Fw32f *d) const                        // REFR Pixel function
			{         
				 Threshold_LTValGTVal(d,levelLt,valueLt,levelGt,valueGt);
			} 	
};


// To be modified once align-96 is supported in fe.h

struct Threshold_LTValGTVal_8u_C3R: public fe2_96<Fw8u,C3,Fw8u,C3>
{
	XMM128 mThlt;
	XMM128 mThgt;
	XMM128 mVlt;
	XMM128 mVgt;
	Fw8u tl[3],tg[3],vl[3],vg[3];
	
	Threshold_LTValGTVal_8u_C3R(const Fw8u thl[3],const Fw8u vll[3],const Fw8u thg[3],const Fw8u vlg[3])
	{
		for(int i=0; i<3;i++)
		{
			tl[i]= thl[i];
			vl[i] = vll[i];
			tg[i] = thg[i];
			vg[i] = vlg[i];
		}
	}

	FE_SSE2_REF 

	IV SSE2_Init() 
	{
		ThLTGT_setup_8u_c3(mThlt.i,mVlt.i,mThgt.i,mVgt.i,tl,vl,tg,vg); 		
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		r.dst[0].i = ThLTGT_sse2_8u(r.src1[0].i,mThlt.i,mVlt.i,mThgt.i,mVgt.i);
	}      
	IV REFR(const Fw8u *s,Fw8u *d) const                        // REFR Pixel function
	{         
		 Threshold_LTValGTVal(s,d,tl,vl,tg,vg);
	} 		
};

struct Threshold_LTValGTVal_16s_C3R: public fe2_96<Fw16s,C3,Fw16s,C3>
{
	XMM128 mThlt;
	XMM128 mThgt;
	XMM128 mVlt;
	XMM128 mVgt;
	Fw16s tl[3],tg[3],vl[3],vg[3];
	
	Threshold_LTValGTVal_16s_C3R(const Fw16s thl[3],const Fw16s vll[3],const Fw16s thg[3],const Fw16s vlg[3])
	{
		for(int i=0; i<3;i++)
		{
			tl[i]= thl[i];
			vl[i] = vll[i];
			tg[i] = thg[i];
			vg[i] = vlg[i];
		}
	}

	FE_SSE2_REF 

	IV SSE2_Init() 
	{
		ThLTGT_setup_16s_c3(mThlt.i,mVlt.i,mThgt.i,mVgt.i,tl,vl,tg,vg); 		
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		r.dst[0].i = ThLTGT_sse2_16s(r.src1[0].i,mThlt.i,mVlt.i,mThgt.i,mVgt.i);
	}      
	IV REFR(const Fw16s *s,Fw16s *d) const                        // REFR Pixel function
	{         
		 Threshold_LTValGTVal(s,d,tl,vl,tg,vg);
	} 		
};

struct Threshold_LTValGTVal_32f_C3R: public fe2_96<Fw32f,C3,Fw32f,C3>
{
	XMM128 mThlt;
	XMM128 mThgt;
	XMM128 mVlt;
	XMM128 mVgt;
	Fw32f tl[3],tg[3],vl[3],vg[3];
	
	Threshold_LTValGTVal_32f_C3R(const Fw32f thl[3],const Fw32f vll[3],const Fw32f thg[3],const Fw32f vlg[3])
	{
		for(int i=0; i<3;i++)
		{
			tl[i]= thl[i];
			vl[i] = vll[i];
			tg[i] = thg[i];
			vg[i] = vlg[i];
		}
	}

	FE_SSE2_REF 

	IV SSE2_Init() 
	{
		ThLTGT_setup_32f_c3(mThlt.f,mVlt.f,mThgt.f,mVgt.f,tl,vl,tg,vg); 		
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		r.dst[0].f = ThLTGT_sse2_32f(r.src1[0].f,mThlt.f,mVlt.f,mThgt.f,mVgt.f);
	}      
	IV REFR(const Fw32f *s,Fw32f *d) const                        // REFR Pixel function
	{         
		 Threshold_LTValGTVal(s,d,tl,vl,tg,vg);
	} 		
};

struct Threshold_LTValGTVal_8u_C3IR: public fe1_96<Fw8u,C3>
{
	XMM128 mThlt;
	XMM128 mThgt;
	XMM128 mVlt;
	XMM128 mVgt;
	Fw8u tl[3],tg[3],vl[3],vg[3];
	
	Threshold_LTValGTVal_8u_C3IR(const Fw8u thl[3],const Fw8u vll[3],const Fw8u thg[3],const Fw8u vlg[3])
	{
		for(int i=0; i<3;i++)
		{
			tl[i]= thl[i];
			vl[i] = vll[i];
			tg[i] = thg[i];
			vg[i] = vlg[i];
		}
	}

	FE_SSE2_REF 

	IV SSE2_Init() 
	{
		ThLTGT_setup_8u_c3(mThlt.i,mVlt.i,mThgt.i,mVgt.i,tl,vl,tg,vg); 		
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		r.dst[0].i = ThLTGT_sse2_8u(r.dst[0].i,mThlt.i,mVlt.i,mThgt.i,mVgt.i);
	}      
	IV REFR(Fw8u *d) const                        // REFR Pixel function
	{         
		 Threshold_LTValGTVal(d,tl,vl,tg,vg);
	} 		
};

struct Threshold_LTValGTVal_16s_C3IR: public fe1_96<Fw16s,C3>
{
	XMM128 mThlt;
	XMM128 mThgt;
	XMM128 mVlt;
	XMM128 mVgt;
	Fw16s tl[3],tg[3],vl[3],vg[3];
	
	Threshold_LTValGTVal_16s_C3IR(const Fw16s thl[3],const Fw16s vll[3],const Fw16s thg[3],const Fw16s vlg[3])
	{
		for(int i=0; i<3;i++)
		{
			tl[i]= thl[i];
			vl[i] = vll[i];
			tg[i] = thg[i];
			vg[i] = vlg[i];
		}
	}

	FE_SSE2_REF 

	IV SSE2_Init() 
	{
		ThLTGT_setup_16s_c3(mThlt.i,mVlt.i,mThgt.i,mVgt.i,tl,vl,tg,vg); 		
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		r.dst[0].i = ThLTGT_sse2_16s(r.dst[0].i,mThlt.i,mVlt.i,mThgt.i,mVgt.i);
	}      
	IV REFR(Fw16s *d) const                        // REFR Pixel function
	{         
		 Threshold_LTValGTVal(d,tl,vl,tg,vg);
	} 		
};

struct Threshold_LTValGTVal_32f_C3IR: public fe1_96<Fw32f,C3>
{
	XMM128 mThlt;
	XMM128 mThgt;
	XMM128 mVlt;
	XMM128 mVgt;
	Fw32f tl[3],tg[3],vl[3],vg[3];
	
	Threshold_LTValGTVal_32f_C3IR(const Fw32f thl[3],const Fw32f vll[3],const Fw32f thg[3],const Fw32f vlg[3])
	{
		for(int i=0; i<3;i++)
		{
			tl[i]= thl[i];
			vl[i] = vll[i];
			tg[i] = thg[i];
			vg[i] = vlg[i];
		}
	}

	FE_SSE2_REF 

	IV SSE2_Init() 
	{
		ThLTGT_setup_32f_c3(mThlt.f,mVlt.f,mThgt.f,mVgt.f,tl,vl,tg,vg); 		
	}

	IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
	{         
		r.dst[0].f = ThLTGT_sse2_32f(r.dst[0].f,mThlt.f,mVlt.f,mThgt.f,mVgt.f);
	}      
	IV REFR(Fw32f *d) const                        // REFR Pixel function
	{         
		 Threshold_LTValGTVal(d,tl,vl,tg,vg);
	} 		
};

} // OPT_LEVEL

#endif

