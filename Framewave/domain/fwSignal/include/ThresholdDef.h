/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __THRESHOLD_SIG_DEF__
#define __THRESHOLD_SIG_DEF__ 

#include "ThresholdImp.h"
#include "fe.h"

namespace OPT_LEVEL
{

template<class TD,class TL,class TV>
struct Threshold1 :public fe1< TD, C1 >   
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


template<class TS,class TD,class TL,class TV>
struct Threshold2 :public fe2< TS, C1, TD, C1 >   
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

namespace DEF_TRES
{
	namespace LT
		{
			namespace VAL 
			{
				struct Threshold_LTVal_16s: public Threshold2<Fw16s,Fw16s,Fw16s,Fw16s>
				{
					Threshold_LTVal_16s(Fw16s l,Fw16s v): Threshold2<Fw16s,Fw16s,Fw16s,Fw16s>(l,v){}

					FE_SSE2_REF 
					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_16s_setup_S(mLevel.i,mValue.i,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].i = THRES::SSE2_LT::VAL::sThresholdLTVal_16s(r.src1[0].i,mLevel.i,mValue.i);
					}      
					IV REFR(const Fw16s *s,Fw16s *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdLTVal<Fw16s,Fw16s>(s,d,level,value);
					}  
				};

				struct Threshold_LTVal_16s_I: public Threshold1<Fw16s,Fw16s,Fw16s>
				{
					Threshold_LTVal_16s_I(Fw16s l,Fw16s v): Threshold1<Fw16s,Fw16s,Fw16s>(l,v){}

					FE_SSE2_REF 
					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_16s_setup_S(mLevel.i,mValue.i,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].i = THRES::SSE2_LT::VAL::sThresholdLTVal_16s(r.dst[0].i,mLevel.i,mValue.i);
					}      
					IV REFR(Fw16s *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdLTVal<Fw16s>(d,level,value);
					}  
				};



				struct Threshold_LTVal_32f :public Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>
				{	        

						Threshold_LTVal_32f(Fw32f l,Fw32f v):Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>(l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_32f_setup_S(mLevel.f,mValue.f,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].f = THRES::SSE2_LT::VAL::sThresholdLTVal_32f(r.src1[0].f,mLevel.f,mValue.f);
						}      
						IV REFR(const Fw32f *s,Fw32f *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal<Fw32f,Fw32f>(s,d,level,value);
						}    
				};

				struct Threshold_LTVal_32f_I :public Threshold1<Fw32f,Fw32f,Fw32f>
				{	        

						Threshold_LTVal_32f_I(Fw32f l,Fw32f v):Threshold1<Fw32f,Fw32f,Fw32f>(l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_32f_setup_S(mLevel.f,mValue.f,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].f = THRES::SSE2_LT::VAL::sThresholdLTVal_32f( r.dst[0].f,mLevel.f,mValue.f);
						}      
						IV REFR(Fw32f *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal<Fw32f>(d,level,value);
						}    
				};


				struct Threshold_LTVal_64f : public Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>
				{	        
						
						Threshold_LTVal_64f(Fw64f l,Fw64f v):Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>(l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_64f_setup_S(mLevel.d,mValue.d,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].d = THRES::SSE2_LT::VAL::sThresholdLTVal_64f(r.src1[0].d,mLevel.d,mValue.d);
						}      
						IV REFR(const Fw64f *s,Fw64f *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal<Fw64f,Fw64f>(s,d,level,value);
						}    
				};


				struct Threshold_LTVal_64f_I :  public Threshold1<Fw64f,Fw64f,Fw64f>
				{	        
						
						Threshold_LTVal_64f_I(Fw64f l,Fw64f v):Threshold1<Fw64f,Fw64f,Fw64f>(l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_64f_setup_S(mLevel.d,mValue.d,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].d = THRES::SSE2_LT::VAL::sThresholdLTVal_64f(r.dst[0].d,mLevel.d,mValue.d);
						}      
						IV REFR(Fw64f *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal<Fw64f>(d,level,value);
						}    
				};

				struct Threshold_LTVal_16sc : public Threshold2<Fw16sc,Fw16sc,Fw16s,Fw16sc>         
				{	        
						
						Threshold_LTVal_16sc(Fw16s l,Fw16sc v):Threshold2<Fw16sc,Fw16sc,Fw16s,Fw16sc>(l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_16sc_setup_S(mLevel.i,mValue.i,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].i = THRES::SSE2_LT::VAL::sThresholdLTVal_16sc(r.src1[0].i,mLevel.i,mValue.i);
						}      
						IV REFR(const Fw16sc *s,Fw16sc *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal16sc(s,d,level,value);
						}    
				};


				struct Threshold_LTVal_16sc_I :public Threshold1<Fw16sc,Fw16s,Fw16sc>         
				{	        
						
						Threshold_LTVal_16sc_I(Fw16s l,Fw16sc v):Threshold1<Fw16sc,Fw16s,Fw16sc> (l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_16sc_setup_S(mLevel.i,mValue.i,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].i = THRES::SSE2_LT::VAL::sThresholdLTVal_16sc(r.dst[0].i,mLevel.i,mValue.i);
						}      
						IV REFR(Fw16sc *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal16sc(d,level,value);
						}    
				};

				struct Threshold_LTVal_32fc :public Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>         
				{	        

					
						Threshold_LTVal_32fc(Fw32f l,Fw32fc v):Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>(l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_32fc_setup_S(mLevel.f,mValue.f,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].f = THRES::SSE2_LT::VAL::sThresholdLTVal_32fc(r.src1[0].f,mLevel.f,mValue.f);
						}      
						IV REFR(const Fw32fc *s,Fw32fc *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal32fc(s,d,level,value);
						}    
				};


				struct Threshold_LTVal_32fc_I : public Threshold1<Fw32fc,Fw32f,Fw32fc>         
				{	        

					
						Threshold_LTVal_32fc_I(Fw32f l,Fw32fc v):Threshold1<Fw32fc,Fw32f,Fw32fc>(l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_32fc_setup_S(mLevel.f,mValue.f,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].f = THRES::SSE2_LT::VAL::sThresholdLTVal_32fc(r.dst[0].f,mLevel.f,mValue.f);
						}      
						IV REFR(Fw32fc *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal32fc(d,level,value);
						}    
				};

				struct Threshold_LTVal_64fc :public Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>       
				{	        
						
						Threshold_LTVal_64fc(Fw64f l,Fw64fc v):Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>(l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_64fc_setup_S(mLevel.d,mValue.d,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].d = THRES::SSE2_LT::VAL::sThresholdLTVal_64fc(r.src1[0].d,mLevel.d,mValue.d);
						}      
						IV REFR(const Fw64fc *s,Fw64fc *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal64fc(s,d,level,value);
						}    
				};


				struct Threshold_LTVal_64fc_I :public Threshold1<Fw64fc,Fw64f,Fw64fc>       
				{	        
						
						Threshold_LTVal_64fc_I(Fw64f l,Fw64fc v):Threshold1<Fw64fc,Fw64f,Fw64fc> (l,v){}

						FE_SSE2_REF                                                 

						IV SSE2_Init() 
						{
							THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_64fc_setup_S(mLevel.d,mValue.d,level,value);
						}

						IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
						{         
						  r.dst[0].d = THRES::SSE2_LT::VAL::sThresholdLTVal_64fc(r.dst[0].d ,mLevel.d,mValue.d);
						}      
						IV REFR( Fw64fc *d) const                        // REFR Pixel function
						{         
							THREF::ThresholdLTVal64fc( d,level,value);
						}    
				};

			}
			struct Threshold_LT_16s : public Threshold2<Fw16s,Fw16s,Fw16s,Fw16s>
			{
				Threshold_LT_16s(Fw16s l): Threshold2<Fw16s,Fw16s,Fw16s,Fw16s>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_16s_setup_S(mLevel.i ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].i = THRES::SSE2_LT::sThresholdLT_16s_sse2_S(r.src1[0].i, mLevel.i);
				}      
				IV REFR(const Fw16s *s,Fw16s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT<Fw16s,Fw16s>(s,d,level);
				} 	
			};


			struct Threshold_LT_16s_I : public Threshold1<Fw16s,Fw16s,Fw16s>
			{
				Threshold_LT_16s_I(Fw16s l): Threshold1<Fw16s,Fw16s,Fw16s>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_16s_setup_S(mLevel.i ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].i = THRES::SSE2_LT::sThresholdLT_16s_sse2_S(r.dst[0].i, mLevel.i);
				}      
				IV REFR(Fw16s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT<Fw16s>(d,level);
				} 	
			};


			struct Threshold_LT_32s : public Threshold2<Fw32s,Fw32s,Fw32s,Fw32s>
			{
				Threshold_LT_32s(Fw32s l): Threshold2<Fw32s,Fw32s,Fw32s,Fw32s>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_32s_setup_S(mLevel.i ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].i = THRES::SSE2_LT::sThresholdLT_32s_sse2_S(r.src1[0].i, mLevel.i);
				}      
				IV REFR(const Fw32s *s,Fw32s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT<Fw32s,Fw32s>(s,d,level);
				} 	
			};

			struct Threshold_LT_32s_I : public Threshold1<Fw32s,Fw32s,Fw32s>
			{
				Threshold_LT_32s_I(Fw32s l): Threshold1<Fw32s,Fw32s,Fw32s>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_32s_setup_S(mLevel.i ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].i = THRES::SSE2_LT::sThresholdLT_32s_sse2_S(r.dst[0].i, mLevel.i);
				}      
				IV REFR(Fw32s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT<Fw32s>(d,level);
				} 	
			};


			struct Threshold_LT_32f : public Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>
			{
				Threshold_LT_32f(Fw32f l): Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_32f_setup_S(mLevel.f ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].f = THRES::SSE2_LT::sThresholdLT_32f_sse2_S(r.src1[0].f, mLevel.f);
				}      
				IV REFR(const Fw32f *s,Fw32f *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT<Fw32f,Fw32f>(s,d,level);
				} 	
			};

			struct Threshold_LT_32f_I : public Threshold1<Fw32f,Fw32f,Fw32f>
			{
				Threshold_LT_32f_I(Fw32f l): Threshold1<Fw32f,Fw32f,Fw32f>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_32f_setup_S(mLevel.f ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].f = THRES::SSE2_LT::sThresholdLT_32f_sse2_S(r.dst[0].f, mLevel.f);
				}      
				IV REFR(Fw32f *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT<Fw32f>(d,level);
				} 	
			};


			struct Threshold_LT_64f : public Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>
			{
				Threshold_LT_64f(Fw64f l): Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_64f_setup_S(mLevel.d ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].d = THRES::SSE2_LT::sThresholdLT_64f_sse2_S(r.src1[0].d, mLevel.d);
				}      
				IV REFR(const Fw64f *s,Fw64f *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT<Fw64f,Fw64f>(s,d,level);
				} 	
			};


			struct Threshold_LT_64f_I : public Threshold1<Fw64f,Fw64f,Fw64f>
			{
				Threshold_LT_64f_I(Fw64f l): Threshold1<Fw64f,Fw64f,Fw64f>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_64f_setup_S(mLevel.d ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].d = THRES::SSE2_LT::sThresholdLT_64f_sse2_S(r.dst[0].d , mLevel.d);
				}      
				IV REFR(Fw64f *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT<Fw64f>(d,level);
				} 
			};


			struct Threshold_LT_32fc : public Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>
			{
				Threshold_LT_32fc(Fw32f l): Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_32fc_setup_S(mLevel.d ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].f = THRES::SSE2_LT::sThresholdLT_32fc_sse2_S(r.src1[0].f, mLevel.d);
				}      
				IV REFR(const Fw32fc *s,Fw32fc *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT32fc(s,d,level);
				} 	
			};


			struct Threshold_LT_32fc_I : public Threshold1<Fw32fc,Fw32f,Fw32fc>
			{
				Threshold_LT_32fc_I(Fw32f l): Threshold1<Fw32fc,Fw32f,Fw32fc>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_32fc_setup_S(mLevel.d ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].f = THRES::SSE2_LT::sThresholdLT_32fc_sse2_S(r.dst[0].f, mLevel.d);
				}      
				IV REFR(Fw32fc *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT32fc(d,level);
				} 	
			};

			struct Threshold_LT_64fc : public Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>
			{
				Threshold_LT_64fc(Fw64f l): Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_64fc_setup_S(mLevel.d ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].d = THRES::SSE2_LT::sThresholdLT_64fc_sse2_S(r.src1[0].d, mLevel.d);
				}      
				IV REFR(const Fw64fc *s,Fw64fc *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT64fc(s,d,level);
				} 	
			};


			struct Threshold_LT_64fc_I : public Threshold1<Fw64fc,Fw64f,Fw64fc>
			{
				Threshold_LT_64fc_I(Fw64f l): Threshold1<Fw64fc,Fw64f,Fw64fc>(l){}
				FE_SSE2_REF 
				
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_64fc_setup_S(mLevel.d ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].d = THRES::SSE2_LT::sThresholdLT_64fc_sse2_S(r.dst[0].d, mLevel.d);
				}      
				IV REFR(Fw64fc *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT64fc(d,level);
				} 	
			};

			struct Threshold_LT_16sc : public Threshold2<Fw16sc,Fw16sc,Fw16s,Fw16sc>
			{
				Threshold_LT_16sc(Fw16s l): Threshold2<Fw16sc,Fw16sc,Fw16s,Fw16sc>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_16sc_setup_S(mLevel.f ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].i = THRES::SSE2_LT::sThresholdLT_16sc_sse2_S(r.src1[0].i, mLevel.f);
				}      
				IV REFR(const Fw16sc *s,Fw16sc *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT16sc(s,d,level);
				} 	
			};


			struct Threshold_LT_16sc_I : public Threshold1<Fw16sc,Fw16s,Fw16sc>
			{
				Threshold_LT_16sc_I(Fw16s l): Threshold1<Fw16sc,Fw16s,Fw16sc>(l){}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_SETUP::sThreshold_16sc_setup_S(mLevel.f ,level );
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].i = THRES::SSE2_LT::sThresholdLT_16sc_sse2_S(r.dst[0].i, mLevel.f);
				}      
				IV REFR( Fw16sc *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLT16sc( d,level);
				} 	
			};
	}


	namespace GT
	{
		namespace VAL
		{
			struct Threshold_GTVal_16s: public Threshold2<Fw16s,Fw16s,Fw16s,Fw16s>
			{
				Threshold_GTVal_16s(Fw16s l,Fw16s v): Threshold2<Fw16s,Fw16s,Fw16s,Fw16s>(l,v){}

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_16s_setup_S(mLevel.i,mValue.i,level,value);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].i = THRES::SSE2_GT::VAL::sThresholdGTVal_16s(r.src1[0].i,mLevel.i,mValue.i);
				}      
				IV REFR(const Fw16s *s,Fw16s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdGTVal<Fw16s,Fw16s>(s,d,level,value);
				}  
			};

			struct Threshold_GTVal_16s_I: public Threshold1<Fw16s,Fw16s,Fw16s>
			{
				Threshold_GTVal_16s_I(Fw16s l,Fw16s v): Threshold1<Fw16s,Fw16s,Fw16s>(l,v){}

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_16s_setup_S(mLevel.i,mValue.i,level,value);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
					r.dst[0].i = THRES::SSE2_GT::VAL::sThresholdGTVal_16s(r.dst[0].i,mLevel.i,mValue.i);
				}      
				IV REFR(Fw16s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdGTVal<Fw16s>(d,level,value);
				}  
			};



			struct Threshold_GTVal_32f :public Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>
			{	        

					Threshold_GTVal_32f(Fw32f l,Fw32f v):Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>(l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_32f_setup_S(mLevel.f,mValue.f,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].f = THRES::SSE2_GT::VAL::sThresholdGTVal_32f(r.src1[0].f,mLevel.f,mValue.f);
					}      
					IV REFR(const Fw32f *s,Fw32f *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal<Fw32f,Fw32f>(s,d,level,value);
					}    
			};

			struct Threshold_GTVal_32f_I :public Threshold1<Fw32f,Fw32f,Fw32f>
			{	        

					Threshold_GTVal_32f_I(Fw32f l,Fw32f v):Threshold1<Fw32f,Fw32f,Fw32f>(l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_32f_setup_S(mLevel.f,mValue.f,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].f = THRES::SSE2_GT::VAL::sThresholdGTVal_32f( r.dst[0].f,mLevel.f,mValue.f);
					}      
					IV REFR(Fw32f *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal<Fw32f>(d,level,value);
					}    
			};


			struct Threshold_GTVal_64f : public Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>
			{	        
					
					Threshold_GTVal_64f(Fw64f l,Fw64f v):Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>(l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_64f_setup_S(mLevel.d,mValue.d,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].d = THRES::SSE2_GT::VAL::sThresholdGTVal_64f(r.src1[0].d,mLevel.d,mValue.d);
					}      
					IV REFR(const Fw64f *s,Fw64f *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal<Fw64f,Fw64f>(s,d,level,value);
					}    
			};


			struct Threshold_GTVal_64f_I : public Threshold1<Fw64f,Fw64f,Fw64f>
			{	        
					
					Threshold_GTVal_64f_I(Fw64f l,Fw64f v):Threshold1<Fw64f,Fw64f,Fw64f>(l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_64f_setup_S(mLevel.d,mValue.d,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].d = THRES::SSE2_GT::VAL::sThresholdGTVal_64f(r.dst[0].d,mLevel.d,mValue.d);
					}      
					IV REFR(Fw64f *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal<Fw64f>(d,level,value);
					}    
			};

			struct Threshold_GTVal_16sc :public Threshold2<Fw16sc,Fw16sc,Fw16s,Fw16sc>         
			{	        
					
					Threshold_GTVal_16sc(Fw16s l,Fw16sc v):Threshold2<Fw16sc,Fw16sc,Fw16s,Fw16sc>(l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_16sc_setup_S(mLevel.i,mValue.i,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].i = THRES::SSE2_GT::VAL::sThresholdGTVal_16sc(r.src1[0].i,mLevel.i,mValue.i);
					}      
					IV REFR(const Fw16sc *s,Fw16sc *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal16sc(s,d,level,value);
					}    
			};


			struct Threshold_GTVal_16sc_I :public Threshold1<Fw16sc,Fw16s,Fw16sc>         
			{	        
					
					Threshold_GTVal_16sc_I(Fw16s l,Fw16sc v):Threshold1<Fw16sc,Fw16s,Fw16sc>(l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_16sc_setup_S(mLevel.i,mValue.i,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].i = THRES::SSE2_GT::VAL::sThresholdGTVal_16sc(r.dst[0].i,mLevel.i,mValue.i);
					}      
					IV REFR(Fw16sc *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal16sc(d,level,value);
					}    
			};

			struct Threshold_GTVal_32fc : public Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>         
			{	        

				
					Threshold_GTVal_32fc(Fw32f l,Fw32fc v):Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>(l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_32fc_setup_S(mLevel.f,mValue.f,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].f = THRES::SSE2_GT::VAL::sThresholdGTVal_32fc(r.src1[0].f,mLevel.f,mValue.f);
					}      
					IV REFR(const Fw32fc *s,Fw32fc *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal32fc(s,d,level,value);
					}    
			};


			struct Threshold_GTVal_32fc_I :public Threshold1<Fw32fc,Fw32f,Fw32fc>         
			{	        

				
					Threshold_GTVal_32fc_I(Fw32f l,Fw32fc v):Threshold1<Fw32fc,Fw32f,Fw32fc>(l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_32fc_setup_S(mLevel.f,mValue.f,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].f = THRES::SSE2_GT::VAL::sThresholdGTVal_32fc(r.dst[0].f,mLevel.f,mValue.f);
					}      
					IV REFR(Fw32fc *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal32fc(d,level,value);
					}    
			};

			struct Threshold_GTVal_64fc : public Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>       
			{	        
					
					Threshold_GTVal_64fc(Fw64f l,Fw64fc v):Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc> (l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_64fc_setup_S(mLevel.d,mValue.d,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].d = THRES::SSE2_GT::VAL::sThresholdGTVal_64fc(r.src1[0].d,mLevel.d,mValue.d);
					}      
					IV REFR(const Fw64fc *s,Fw64fc *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal64fc(s,d,level,value);
					}    
			};


			struct Threshold_GTVal_64fc_I : public Threshold1<Fw64fc,Fw64f,Fw64fc>       
			{	        
					
					Threshold_GTVal_64fc_I(Fw64f l,Fw64fc v):Threshold1<Fw64fc,Fw64f,Fw64fc>(l,v){}

					FE_SSE2_REF                                                 

					IV SSE2_Init() 
					{
						THRES::SSE2_LT::VAL::SSE2_SETUP::sThresholdLTVal_64fc_setup_S(mLevel.d,mValue.d,level,value);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
					  r.dst[0].d = THRES::SSE2_GT::VAL::sThresholdGTVal_64fc(r.dst[0].d ,mLevel.d,mValue.d);
					}      
					IV REFR( Fw64fc *d) const                        // REFR Pixel function
					{         
						THREF::ThresholdGTVal64fc( d,level,value);
					}    
			};
		}
		struct Threshold_GT_16s : public Threshold2<Fw16s,Fw16s,Fw16s,Fw16s>
		{
			Threshold_GT_16s(Fw16s l): Threshold2<Fw16s,Fw16s,Fw16s,Fw16s>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_16s_setup_S(mLevel.i ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = THRES::SSE2_GT::sThresholdGT_16s_sse2_S(r.src1[0].i, mLevel.i);
			}      
			IV REFR(const Fw16s *s,Fw16s *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT<Fw16s,Fw16s>(s,d,level);
			} 	
		};


		struct Threshold_GT_16s_I : public Threshold1<Fw16s,Fw16s,Fw16s>
		{
			Threshold_GT_16s_I(Fw16s l): Threshold1<Fw16s,Fw16s,Fw16s>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_16s_setup_S(mLevel.i ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = THRES::SSE2_GT::sThresholdGT_16s_sse2_S(r.dst[0].i, mLevel.i);
			}      
			IV REFR(Fw16s *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT<Fw16s>(d,level);
			} 	
		};


		struct Threshold_GT_32s : public Threshold2<Fw32s,Fw32s,Fw32s,Fw32s>
		{
			Threshold_GT_32s(Fw32s l): Threshold2<Fw32s,Fw32s,Fw32s,Fw32s>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_32s_setup_S(mLevel.i ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = THRES::SSE2_GT::sThresholdGT_32s_sse2_S(r.src1[0].i, mLevel.i);
			}      
			IV REFR(const Fw32s *s,Fw32s *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT<Fw32s,Fw32s>(s,d,level);
			} 	
		};

		struct Threshold_GT_32s_I : public Threshold1<Fw32s,Fw32s,Fw32s>
		{
			Threshold_GT_32s_I(Fw32s l): Threshold1<Fw32s,Fw32s,Fw32s>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_32s_setup_S(mLevel.i ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = THRES::SSE2_GT::sThresholdGT_32s_sse2_S(r.dst[0].i, mLevel.i);
			}      
			IV REFR(Fw32s *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT<Fw32s>(d,level);
			} 	
		};


		struct Threshold_GT_32f : public Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>
		{
			Threshold_GT_32f(Fw32f l): Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_32f_setup_S(mLevel.f ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = THRES::SSE2_GT::sThresholdGT_32f_sse2_S(r.src1[0].f, mLevel.f);
			}      
			IV REFR(const Fw32f *s,Fw32f *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT<Fw32f,Fw32f>(s,d,level);
			} 	
		};

		struct Threshold_GT_32f_I : public Threshold1<Fw32f,Fw32f,Fw32f>
		{
			Threshold_GT_32f_I(Fw32f l): Threshold1<Fw32f,Fw32f,Fw32f>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_32f_setup_S(mLevel.f ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = THRES::SSE2_GT::sThresholdGT_32f_sse2_S(r.dst[0].f, mLevel.f);
			}      
			IV REFR(Fw32f *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT<Fw32f>(d,level);
			} 	
		};


		struct Threshold_GT_64f : public Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>
		{
			Threshold_GT_64f(Fw64f l): Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_64f_setup_S(mLevel.d ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].d = THRES::SSE2_GT::sThresholdGT_64f_sse2_S(r.src1[0].d, mLevel.d);
			}      
			IV REFR(const Fw64f *s,Fw64f *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT<Fw64f,Fw64f>(s,d,level);
			} 	
		};


		struct Threshold_GT_64f_I : public Threshold1<Fw64f,Fw64f,Fw64f>
		{
			Threshold_GT_64f_I(Fw64f l): Threshold1<Fw64f,Fw64f,Fw64f>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_64f_setup_S(mLevel.d ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].d = THRES::SSE2_GT::sThresholdGT_64f_sse2_S(r.dst[0].d , mLevel.d);
			}      
			IV REFR(Fw64f *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT<Fw64f>(d,level);
			} 
		};

		struct  Threshold_GT_32fc :public fe2< Fw32fc, C1, Fw32fc, C1 >   
		{
			XMM128 mLevel;
			XMM128 Indf;
			XMM128 Inff;
			
			Fw32f level;
			
			Threshold_GT_32fc(Fw32f l)
			{
				level = l;
			}

			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThresholdGT_32fc_setup_S(mLevel.d ,Indf.f,Inff.f,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = THRES::SSE2_GT::sThresholdGT_32fc_sse2_S(r.src1[0].f, mLevel.d,Inff.f,Indf.f);
			}      
			IV REFR(const Fw32fc *s,Fw32fc *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT32fc (s,d,level);
			} 	
		};

		struct  Threshold_GT_32fc_I :public fe1< Fw32fc, C1  >   
		{
			XMM128 mLevel;
			XMM128 Indf;
			XMM128 Inff;
			
			Fw32f level;
			
			Threshold_GT_32fc_I(Fw32f l)
			{
				level = l;
			}

			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThresholdGT_32fc_setup_S(mLevel.d ,Indf.f,Inff.f,level);
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].f = THRES::SSE2_GT::sThresholdGT_32fc_sse2_S(r.dst[0].f, mLevel.d,Inff.f,Indf.f);
			}      
			IV REFR(Fw32fc *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT32fc (d,level);
			} 	
		};


		struct Threshold_GT_64fc : public Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>
		{
			Threshold_GT_64fc(Fw64f l): Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_64fc_setup_S(mLevel.d ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].d = THRES::SSE2_GT::sThresholdGT_64fc_sse2_S(r.src1[0].d, mLevel.d);
			}      
			IV REFR(const Fw64fc *s,Fw64fc *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT64fc (s,d,level);
			} 	
		};


		struct Threshold_GT_64fc_I : public Threshold1<Fw64fc,Fw64f,Fw64fc>
		{
			Threshold_GT_64fc_I(Fw64f l): Threshold1<Fw64fc,Fw64f,Fw64fc>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_64fc_setup_S(mLevel.d ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].d = THRES::SSE2_GT::sThresholdGT_64fc_sse2_S(r.dst[0].d, mLevel.d);
			}      
			IV REFR( Fw64fc *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT64fc (d,level);
			} 	
		};


		struct Threshold_GT_16sc : public Threshold2<Fw16sc,Fw16sc,Fw16s,Fw16sc>
		{
			Threshold_GT_16sc(Fw16s l): Threshold2<Fw16sc,Fw16sc,Fw16s,Fw16sc>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_16sc_setup_S(mLevel.f ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = THRES::SSE2_GT::sThresholdGT_16sc_sse2_S(r.src1[0].i, mLevel.f);
			}      
			IV REFR(const Fw16sc *s,Fw16sc *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT16sc (s,d,level);
			} 	
		};


		struct Threshold_GT_16sc_I : public Threshold1<Fw16sc,Fw16s,Fw16sc>
		{
			Threshold_GT_16sc_I(Fw16s l): Threshold1<Fw16sc,Fw16s,Fw16sc>(l){}
			FE_SSE2_REF 
			IV SSE2_Init() 
			{
				THRES::SSE2_SETUP::sThreshold_16sc_setup_S(mLevel.f ,level );
			}

			IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
			{         
				r.dst[0].i = THRES::SSE2_GT::sThresholdGT_16sc_sse2_S(r.dst[0].i , mLevel.f);
			}      
			IV REFR( Fw16sc *d) const                        // REFR Pixel function
			{         
				THREF::ThresholdGT16sc ( d,level);
			} 	
		};
	}

	namespace LTVALGTVAL
	{

		template< class TD,class TL,class TV>
		struct ThresholdVal1:  public Threshold1< TD,TL,TV>    
		{
			XMM128 mLevelGt;
			XMM128 mValueGt;
			TL levelGt;
			TV valueGt;
			ThresholdVal1(TL llt,TV vlt,TL lgt,TV vgt):Threshold1<TD,TL,TV>(llt,vlt)
			{
				levelGt = lgt;
				valueGt = vgt;
			}
		};

		template<class TS, class TD,class TL,class TV>
		struct ThresholdVal2:  public Threshold2<TS,TD,TL,TV>    
		{
			XMM128 mLevelGt;
			XMM128 mValueGt;
			TL levelGt;
			TV valueGt;
			ThresholdVal2(TL llt,TV vlt,TL lgt,TV vgt):Threshold2<TS,TD,TL,TV>(llt,vlt)
			{
				levelGt = lgt;
				valueGt = vgt;
			}
		};

		struct Threshold_LTValGTVal_16s: public ThresholdVal2<Fw16s,Fw16s,Fw16s,Fw16s>       
		{	        
					
				Threshold_LTValGTVal_16s(Fw16s llt,Fw16s vlt,Fw16s lgt,Fw16s vgt):ThresholdVal2<Fw16s,Fw16s,Fw16s,Fw16s>(llt,vlt,lgt,vgt){}

				FE_SSE2_REF                                                 

				IV SSE2_Init() 
				{
					THRES::SSE2_LTVALGTVAL::SSE2_SETUP::sThresholdLTValGTVal_16s_setup_S(mLevel.i,mValue.i,mLevelGt.i,mValueGt.i,	\
																  level,value,levelGt,valueGt);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
				  r.dst[0].i = THRES::SSE2_LTVALGTVAL::sThresholdLTValGTVal_16s_sse2_S(r.src1[0].i,mLevel.i,mValue.i,mLevelGt.i,mValueGt.i);
				}      
				IV REFR(const Fw16s *s,Fw16s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLTGT<Fw16s,Fw16s>(s,d,level,value,levelGt,valueGt);
				}    
		};


		struct Threshold_LTValGTVal_16s_I: public ThresholdVal1<Fw16s,Fw16s,Fw16s>       
		{	        
					
				Threshold_LTValGTVal_16s_I(Fw16s llt,Fw16s vlt,Fw16s lgt,Fw16s vgt):ThresholdVal1<Fw16s,Fw16s,Fw16s> (llt,vlt,lgt,vgt){}

				FE_SSE2_REF                                                 

				IV SSE2_Init() 
				{
					THRES::SSE2_LTVALGTVAL::SSE2_SETUP::sThresholdLTValGTVal_16s_setup_S(mLevel.i,mValue.i,mLevelGt.i,mValueGt.i,	\
																  level,value,levelGt,valueGt);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
				  r.dst[0].i = THRES::SSE2_LTVALGTVAL::sThresholdLTValGTVal_16s_sse2_S(r.dst[0].i,mLevel.i,mValue.i,mLevelGt.i,mValueGt.i);
				}      
				IV REFR(Fw16s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLTGT<Fw16s>(d,level,value,levelGt,valueGt);
				}    
		};


		struct Threshold_LTValGTVal_32s: public ThresholdVal2<Fw32s,Fw32s,Fw32s,Fw32s>       
		{	        
					
				Threshold_LTValGTVal_32s(Fw32s llt,Fw32s vlt,Fw32s lgt,Fw32s vgt):ThresholdVal2<Fw32s,Fw32s,Fw32s,Fw32s>(llt,vlt,lgt,vgt){}

				FE_SSE2_REF                                                 

				IV SSE2_Init() 
				{
					THRES::SSE2_LTVALGTVAL::SSE2_SETUP::sThresholdLTValGTVal_32s_setup_S(mLevel.i,mValue.i,mLevelGt.i,mValueGt.i,	\
																  level,value,levelGt,valueGt);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
				  r.dst[0].i = THRES::SSE2_LTVALGTVAL::sThresholdLTValGTVal_32s_sse2_S(r.src1[0].i,mLevel.i,mValue.i,mLevelGt.i,mValueGt.i);
				}      
				IV REFR(const Fw32s *s,Fw32s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLTGT<Fw32s,Fw32s>(s,d,level,value,levelGt,valueGt);
				}    
		};

		struct Threshold_LTValGTVal_32s_I: public ThresholdVal1<Fw32s,Fw32s,Fw32s>       
		{	        
					
				Threshold_LTValGTVal_32s_I(Fw32s llt,Fw32s vlt,Fw32s lgt,Fw32s vgt):ThresholdVal1<Fw32s,Fw32s,Fw32s>(llt,vlt,lgt,vgt){}

				FE_SSE2_REF                                                 

				IV SSE2_Init() 
				{
					THRES::SSE2_LTVALGTVAL::SSE2_SETUP::sThresholdLTValGTVal_32s_setup_S(mLevel.i,mValue.i,mLevelGt.i,mValueGt.i,	\
																  level,value,levelGt,valueGt);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
				  r.dst[0].i = THRES::SSE2_LTVALGTVAL::sThresholdLTValGTVal_32s_sse2_S(r.dst[0].i,mLevel.i,mValue.i,mLevelGt.i,mValueGt.i);
				}      
				IV REFR( Fw32s *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLTGT<Fw32s>(d,level,value,levelGt,valueGt);
				}    
		};

		struct Threshold_LTValGTVal_32f: public ThresholdVal2<Fw32f,Fw32f,Fw32f,Fw32f>       
		{	        
					
				Threshold_LTValGTVal_32f(Fw32f llt,Fw32f vlt,Fw32f lgt,Fw32f vgt):ThresholdVal2<Fw32f,Fw32f,Fw32f,Fw32f> (llt,vlt,lgt,vgt){}

				FE_SSE2_REF                                                 

				IV SSE2_Init() 
				{
					THRES::SSE2_LTVALGTVAL::SSE2_SETUP::sThresholdLTValGTVal_32f_setup_S(mLevel.f,mValue.f,mLevelGt.f,mValueGt.f,	\
																  level,value,levelGt,valueGt);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
				  r.dst[0].f = THRES::SSE2_LTVALGTVAL::sThresholdLTValGTVal_32f_sse2_S(r.src1[0].f,mLevel.f,mValue.f,mLevelGt.f,mValueGt.f);
				}      
				IV REFR(const Fw32f *s,Fw32f *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLTGT<Fw32f,Fw32f>(s,d,level,value,levelGt,valueGt);
				}    
		};

		struct Threshold_LTValGTVal_32f_I: public ThresholdVal1<Fw32f,Fw32f,Fw32f>       
		{	        
					
				Threshold_LTValGTVal_32f_I(Fw32f llt,Fw32f vlt,Fw32f lgt,Fw32f vgt):ThresholdVal1<Fw32f,Fw32f,Fw32f>(llt,vlt,lgt,vgt){}

				FE_SSE2_REF                                                 

				IV SSE2_Init() 
				{
					THRES::SSE2_LTVALGTVAL::SSE2_SETUP::sThresholdLTValGTVal_32f_setup_S(mLevel.f,mValue.f,mLevelGt.f,mValueGt.f,	\
																  level,value,levelGt,valueGt);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
				  r.dst[0].f = THRES::SSE2_LTVALGTVAL::sThresholdLTValGTVal_32f_sse2_S(r.dst[0].f,mLevel.f,mValue.f,mLevelGt.f,mValueGt.f);
				}      
				IV REFR( Fw32f *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLTGT<Fw32f>(d,level,value,levelGt,valueGt);
				}    
		};


		struct Threshold_LTValGTVal_64f: public ThresholdVal2<Fw64f,Fw64f,Fw64f,Fw64f>       
		{	        
					
				Threshold_LTValGTVal_64f(Fw64f llt,Fw64f vlt,Fw64f lgt,Fw64f vgt):ThresholdVal2<Fw64f,Fw64f,Fw64f,Fw64f>(llt,vlt,lgt,vgt){}

				FE_SSE2_REF                                                 

				IV SSE2_Init() 
				{
					THRES::SSE2_LTVALGTVAL::SSE2_SETUP::sThresholdLTValGTVal_64f_setup_S(mLevel.d,mValue.d,mLevelGt.d,mValueGt.d,	\
																  level,value,levelGt,valueGt);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
				  r.dst[0].d = THRES::SSE2_LTVALGTVAL::sThresholdLTValGTVal_64f_sse2_S(r.src1[0].d,mLevel.d,mValue.d,mLevelGt.d,mValueGt.d);
				}      
				IV REFR(const Fw64f *s,Fw64f *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLTGT<Fw64f,Fw64f>(s,d,level,value,levelGt,valueGt);
				}    
		};


		struct Threshold_LTValGTVal_64f_I: public ThresholdVal1<Fw64f,Fw64f,Fw64f>       
		{	        

				Threshold_LTValGTVal_64f_I(Fw64f llt,Fw64f vlt,Fw64f lgt,Fw64f vgt):ThresholdVal1<Fw64f,Fw64f,Fw64f>(llt,vlt,lgt,vgt){}

				FE_SSE2_REF                                                 

				IV SSE2_Init() 
				{
					THRES::SSE2_LTVALGTVAL::SSE2_SETUP::sThresholdLTValGTVal_64f_setup_S(mLevel.d,mValue.d,mLevelGt.d,mValueGt.d,	\
																  level,value,levelGt,valueGt);
				}

				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{         
				  r.dst[0].d = THRES::SSE2_LTVALGTVAL::sThresholdLTValGTVal_64f_sse2_S(r.dst[0].d ,mLevel.d,mValue.d,mLevelGt.d,mValueGt.d);
				}      
				IV REFR( Fw64f *d) const                        // REFR Pixel function
				{         
					THREF::ThresholdLTGT< Fw64f>( d,level,value,levelGt,valueGt);
				}    
		};

	}
	namespace INV
	{
		struct Threshold_LTInv_32f: public Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>
		{
					
					XMM128 mPmax;
					XMM128 m_level;
					
					Threshold_LTInv_32f(Fw32f l): Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_32f_setup_S(mLevel.f,level,mPmax.i,m_level.f);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].f =THRES::INV::sThreshold_LTInv_32f_sse2_S(r.src1[0].f, mLevel.f,m_level.f,mPmax.f);
					}      
					IV REFR(const Fw32f *s,Fw32f *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv(s,d,level);
					} 	
		};

		struct Threshold_LTInv_32f_I: public Threshold1<Fw32f,Fw32f,Fw32f>
		{
					
					XMM128 mPmax;
					XMM128 m_level;
					
					Threshold_LTInv_32f_I(Fw32f l): Threshold1<Fw32f,Fw32f,Fw32f>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_32f_setup_S(mLevel.f,level,mPmax.i,m_level.f);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].f =THRES::INV::sThreshold_LTInv_32f_sse2_S(r.dst[0].f, mLevel.f,m_level.f,mPmax.f);
					}      
					IV REFR(Fw32f *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv(d,level);
					} 	
		};

		struct Threshold_LTInv_32f_LZero: public Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>
		{
					
					XMM128 mInf;
					FwStatus status;
					
					Threshold_LTInv_32f_LZero(Fw32f l): Threshold2<Fw32f,Fw32f,Fw32f,Fw32f>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_32f_LZero_setup_S(mInf.f);
						status = fwStsNoErr;
					}

					IV SSE2( RegFile & r ) //const                        // SSE2 Pixel function
					{         
						r.dst[0].f =THRES::INV::sThreshold_LTInv_32f_LZero_sse2_S(r.src1[0].f,mInf.f, status);
					}      
					IV REFR(const Fw32f *s, Fw32f *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv32fLZero(s,d);
					} 	
		};

		struct Threshold_LTInv_32f_LZero_I: public Threshold1<Fw32f,Fw32f,Fw32f>
		{
					
					XMM128 mInf;
					FwStatus status;

					
					Threshold_LTInv_32f_LZero_I(Fw32f l): Threshold1<Fw32f,Fw32f,Fw32f>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_32f_LZero_setup_S(mInf.f);
						status = fwStsNoErr;
					}

					IV SSE2( RegFile & r ) //const                        // SSE2 Pixel function
					{         
						r.dst[0].f =THRES::INV::sThreshold_LTInv_32f_LZero_sse2_S(r.dst[0].f,mInf.f, status);
					}      
					IV REFR(Fw32f *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv32fLZero(d);
					} 	
		};


		struct Threshold_LTInv_32fc: public Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>
		{
					
					XMM128 mPmax;
					XMM128 m_level;
					
					Threshold_LTInv_32fc(Fw32f l): Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_32fc_setup_S(mLevel.f,level,mPmax.i,m_level.f);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].f =THRES::INV::sThreshold_LTInv_32fc_sse2_S(r.src1[0].f, mLevel.f,m_level.f,mPmax.f);
					}      
					IV REFR(const Fw32fc *s,Fw32fc *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv32fc(s,d,level);
					} 	
		};

		struct Threshold_LTInv_32fc_I: public Threshold1<Fw32fc,Fw32f,Fw32fc>
		{
					
					XMM128 mPmax;
					XMM128 m_level;
					
					Threshold_LTInv_32fc_I(Fw32f l): Threshold1<Fw32fc,Fw32f,Fw32fc>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_32fc_setup_S(mLevel.f,level,mPmax.i,m_level.f);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].f =THRES::INV::sThreshold_LTInv_32fc_sse2_S(r.dst[0].f, mLevel.f,m_level.f,mPmax.f);
					}      
					IV REFR(Fw32fc *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv32fc(d,level);
					} 	
		};


		struct Threshold_LTInv_32fc_LZero: public Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>
		{
					
					XMM128 mInf;
					XMM128 mPmax;
					FwStatus status;
					
					Threshold_LTInv_32fc_LZero(Fw32f l): Threshold2<Fw32fc,Fw32fc,Fw32f,Fw32fc>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_32fc_LZero_setup_S(mInf.f,mPmax.i);
						status = fwStsNoErr;
					}

					IV SSE2( RegFile & r )                         // SSE2 Pixel function
					{         
						r.dst[0].f =THRES::INV::sThreshold_LTInv_32fc_LZero_sse2_S(r.src1[0].f,mInf.f,mPmax.f, status);
					}      
					IV REFR(const Fw32fc *s, Fw32fc *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv32fcLZero(s,d);
					} 	
		};

		struct Threshold_LTInv_32fc_LZero_I: public Threshold1<Fw32fc,Fw32f,Fw32fc>
		{
					
					XMM128 mInf;
					XMM128 mPmax;
					FwStatus status;

					
					Threshold_LTInv_32fc_LZero_I(Fw32f l): Threshold1<Fw32fc,Fw32f,Fw32fc>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_32fc_LZero_setup_S(mInf.f,mPmax.i);
						status = fwStsNoErr;
					}

					IV SSE2( RegFile & r )                        // SSE2 Pixel function
					{         
						r.dst[0].f =THRES::INV::sThreshold_LTInv_32fc_LZero_sse2_S(r.dst[0].f,mInf.f,mPmax.f, status);
					}      
					IV REFR(Fw32fc *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv32fcLZero(d);
					} 	
		};

		struct Threshold_LTInv_64f: public Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>
		{
					
					XMM128 mPmax;
					XMM128 m_level;
					
					Threshold_LTInv_64f(Fw64f l): Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_64f_setup_S(mLevel.d,level,mPmax.i,m_level.d);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].d =THRES::INV::sThreshold_LTInv_64f_sse2_S(r.src1[0].d, mLevel.d,m_level.d,mPmax.d);
					}      
					IV REFR(const Fw64f *s,Fw64f *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv(s,d,level);
					} 	
		};

		struct Threshold_LTInv_64f_I: public Threshold1<Fw64f,Fw64f,Fw64f>
		{
					
					XMM128 mPmax;
					XMM128 m_level;
					
					Threshold_LTInv_64f_I(Fw64f l): Threshold1<Fw64f,Fw64f,Fw64f>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_64f_setup_S(mLevel.d,level,mPmax.i,m_level.d);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].d =THRES::INV::sThreshold_LTInv_64f_sse2_S(r.dst[0].d, mLevel.d,m_level.d,mPmax.d);
					}      
					IV REFR(Fw64f *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv(d,level);
					} 	
		};

		struct Threshold_LTInv_64f_LZero: public Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>
		{
					
					XMM128 mInf;
					XMM128 mOne;
					FwStatus status;
					
					Threshold_LTInv_64f_LZero(Fw64f l): Threshold2<Fw64f,Fw64f,Fw64f,Fw64f>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_64f_LZero_setup_S(mInf.d,mOne.d);
						status = fwStsNoErr;
					}

					IV SSE2( RegFile & r )                         // SSE2 Pixel function
					{         
						r.dst[0].d =THRES::INV::sThreshold_LTInv_64f_LZero_sse2_S(r.src1[0].d,mInf.d,mOne.d, status);
					}      
					IV REFR(const Fw64f *s, Fw64f *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv64fLZero(s,d);
					} 	
		};

		struct Threshold_LTInv_64f_LZero_I: public Threshold1<Fw64f,Fw64f,Fw64f>
		{
					
					XMM128 mInf;
					XMM128 mOne;
					FwStatus status;
					
					Threshold_LTInv_64f_LZero_I(Fw64f l): Threshold1<Fw64f,Fw64f,Fw64f>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_64f_LZero_setup_S(mInf.d,mOne.d);
						status = fwStsNoErr;
					}

					IV SSE2( RegFile & r )                         // SSE2 Pixel function
					{         
						r.dst[0].d =THRES::INV::sThreshold_LTInv_64f_LZero_sse2_S(r.dst[0].d,mInf.d,mOne.d, status);
					}      
					IV REFR(Fw64f *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv64fLZero(d);
					} 	
		};



		struct Threshold_LTInv_64fc: public Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>
		{
					
					XMM128 mPmax;
					XMM128 m_level;
					
					Threshold_LTInv_64fc(Fw64f l): Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_64fc_setup_S(mLevel.d,level,mPmax.i,m_level.d);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].d =THRES::INV::sThreshold_LTInv_64fc_sse2_S(r.src1[0].d, mLevel.d,m_level.d,mPmax.d);
					}      
					IV REFR(const Fw64fc *s,Fw64fc *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv64fc(s,d,level);
					} 	
		};

		struct Threshold_LTInv_64fc_I: public Threshold1<Fw64fc,Fw64f,Fw64fc>
		{
					
					XMM128 mPmax;
					XMM128 m_level;
					
					Threshold_LTInv_64fc_I(Fw64f l): Threshold1<Fw64fc,Fw64f,Fw64fc>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_64fc_setup_S(mLevel.d,level,mPmax.i,m_level.d);
					}

					IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
					{         
						r.dst[0].d =THRES::INV::sThreshold_LTInv_64fc_sse2_S(r.dst[0].d, mLevel.d,m_level.d,mPmax.d);
					}      
					IV REFR(Fw64fc *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv64fc(d,level);
					} 	
		};

		struct Threshold_LTInv_64fc_LZero: public Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>
		{
					
					XMM128 mInf;
					XMM128 mPmax;
					FwStatus status;
					
					Threshold_LTInv_64fc_LZero(Fw64f l): Threshold2<Fw64fc,Fw64fc,Fw64f,Fw64fc>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_64fc_LZero_setup_S(mInf.d,mPmax.i);
						status = fwStsNoErr;
					}

					IV SSE2( RegFile & r )                        // SSE2 Pixel function
					{         
						r.dst[0].d =THRES::INV::sThreshold_LTInv_64fc_LZero_sse2_S(r.src1[0].d,mInf.d,mPmax.d, status);
					}      
					IV REFR(const Fw64fc *s, Fw64fc *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv64fcLZero(s,d);
					} 	
		};

		struct Threshold_LTInv_64fc_LZero_I: public Threshold1<Fw64fc,Fw64f,Fw64fc>
		{
					
					XMM128 mInf;
					XMM128 mPmax;
					FwStatus status;
					
					Threshold_LTInv_64fc_LZero_I(Fw64f l): Threshold1<Fw64fc,Fw64f,Fw64fc>(l){}
					FE_SSE2_REF 

					IV SSE2_Init() 
					{
						THRES::INV::SSE2_SETUP::sThreshold_LTInv_64fc_LZero_setup_S(mInf.d,mPmax.i);
						status = fwStsNoErr;
					}

					IV SSE2( RegFile & r )                        // SSE2 Pixel function
					{         
						r.dst[0].d =THRES::INV::sThreshold_LTInv_64fc_LZero_sse2_S(r.dst[0].d,mInf.d,mPmax.d, status);
					}      
					IV REFR(Fw64fc *d) const                        // REFR Pixel function
					{         
						THREF::Threshold_LTInv64fcLZero(d);
					} 	
		};
	

	}
}


namespace DEF_MAG
{
	struct Magnitude_32f : public fe3<Fw32f,C1,Fw32f,C1,Fw32f,C1>
	{
		
		FE_SSE2_REF 

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			r.dst[0].f =MAG::Magnitude_32f_sse2_S(r.src1[0].f,r.src2[0].f);
		}      
		IV REFR(const Fw32f *s1,const Fw32f *s2,Fw32f *d) const                        // REFR Pixel function
		{         
			MAGREF::Magnitude32f(s1,s2,d);
		} 
	};

	struct Magnitude_32fc : public fe2<Fw32fc,C1,Fw32f,C1>
	{

        const static U32 nPIX_SSE = 2 * 2; // Load two registers
        class SRC1: public RegDesc< Fw32fc, C1, nPIX_SSE > {};
        class DST:  public RegDesc< Fw32f,  C1, nPIX_SSE > {}; 
    		
		FE_SSE2_REF 

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
            __m128 mulval =  _mm_mul_ps(r.src1[0].f, r.src1[0].f);
            __m128 mulval2 =  _mm_mul_ps(r.src1[1].f, r.src1[1].f);
            __m128 loval = _mm_unpacklo_ps(mulval,mulval2);
            __m128 hival = _mm_unpackhi_ps(mulval,mulval2);
            
            __m128 loval1 = _mm_unpacklo_ps(loval,hival);
            __m128 hival1 = _mm_unpackhi_ps(loval,hival);

            loval1    = _mm_add_ps(loval1, hival1); 
            r.dst[0].f =  _mm_sqrt_ps(loval1);
           
		}      

		IV REFR(const Fw32fc *s,Fw32f *d) const                        // REFR Pixel function
		{         
			MAGREF::Magnitude32fc(s,d);
		} 
	};

	struct Magnitude_64f : public fe3<Fw64f,C1,Fw64f,C1,Fw64f,C1>
	{
		XMM128 mInf;
		XMM128 mInd;
		
		FE_SSE2_REF 

		IV SSE2_Init() 
		{
			MAG::SSE2_SETUP::Magnitude_64f_setup_S(mInf.d,mInd.d);
		}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			r.dst[0].d =MAG::Magnitude_64f_sse2_S(r.src1[0].d,r.src2[0].d,mInf.d,mInd.d);
		}      
		IV REFR(const Fw64f *s1,const Fw64f *s2,Fw64f *d) const                        // REFR Pixel function
		{         
			MAGREF::Magnitude64f(s1,s2,d);
		} 
	};


	struct Magnitude_64fc : public fe2<Fw64fc,C1,Fw64f,C1>
	{
		XMM128 mInf;
		XMM128 mInd;
		
		FE_SSE2_REF 

		IV SSE2_Init() 
		{
			MAG::SSE2_SETUP::Magnitude_64f_setup_S(mInf.d,mInd.d);
		}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			r.dst[0].d =MAG::Magnitude_64fc_sse2_S(r.src1[0].d,mInf.d,mInd.d);
		}      
		IV REFR(const Fw64fc *s,Fw64f *d) const                        // REFR Pixel function
		{         
			MAGREF::Magnitude64fc(s,d);
		} 
	};


	struct Magnitude_16s32f : public fe3<Fw16s,C1,Fw16s,C1,Fw32f,C1>
	{
	
		FE_SSE2_REF 

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			r.dst[0].f =MAG::Magnitude_16s32f_sse2_S(r.src1[0].i,r.src2[0].i);
		}      
		IV REFR(const Fw16s *s1,const Fw16s *s2,Fw32f *d) const                        // REFR Pixel function
		{         
			MAGREF::Magnitude16s32f(s1,s2,d);
		} 
	};

	struct Magnitude_16sc32f : public fe2<Fw16sc,C1,Fw32f,C1>
	{
		XMM128 mNmax;
		
		FE_SSE2_REF 

		IV SSE2_Init() 
		{
			MAG::SSE2_SETUP::Magnitude_16sc32f_setup_S(mNmax.i);
		}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			r.dst[0].f =MAG::Magnitude_16sc32f_sse2_S(r.src1[0].i,mNmax.i);
		}      
		IV REFR(const Fw16sc *s,Fw32f *d) const                        // REFR Pixel function
		{         
			MAGREF::Magnitude16sc32f(s,d);
		} 
	};

	struct Magnitude_16s : public fe3<Fw16s,C1,Fw16s,C1,Fw16s,C1>
	{
		XMM128 mScale;
		int scale;
		
		Magnitude_16s(int scaleval)
		{
			scale = scaleval;
		}

		FE_SSE2_REF 

		IV SSE2_Init() 
		{
			MAG::SSE2_SETUP::Magnitude_16s_setup_S(mScale.f,scale);
		}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			r.dst[0].i =MAG::Magnitude_16s_sse2_S(r.src1[0].i,r.src2[0].i,mScale.f);
		}      
		IV REFR(const Fw16s *s1,const Fw16s *s2,Fw16s *d) const                        // REFR Pixel function
		{         
			MAGREF::Magnitude16s(s1,s2,d,scale);
		} 
	};

	struct Magnitude_16sc : public fe2<Fw16sc,C1,Fw16s,C1>
	{
		XMM128 mScale;
		int scale;
		
		Magnitude_16sc(int scaleval)
		{
			scale = scaleval;
		}

		FE_SSE2_REF 

		IV SSE2_Init() 
		{
			MAG::SSE2_SETUP::Magnitude_16s_setup_S(mScale.f,scale);
		}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			r.dst[0].i =MAG::Magnitude_16sc_sse2_S(r.src1[0].i,mScale.f);
		}      
		IV REFR(const Fw16sc *s,Fw16s *d) const                        // REFR Pixel function
		{         
			MAGREF::Magnitude16sc16s(s,d,scale);
		} 
	};

	struct Magnitude_32sc : public fe2<Fw32sc,C1,Fw32s,C1>
	{
		XMM128 mScale;
		int scale;
		
		Magnitude_32sc(int scaleval)
		{
			scale = scaleval;
		}

		FE_SSE2_REF 

		IV SSE2_Init() 
		{
			MAG::SSE2_SETUP::Magnitude_32s_setup_S(mScale.d,scale);
		}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			r.dst[0].i =MAG::Magnitude_32sc_sse2_S(r.src1[0].i,mScale.d);
		}      
		IV REFR(const Fw32sc *s,Fw32s *d) const                        // REFR Pixel function
		{         
			MAGREF::Magnitude32sc32s(s,d,scale);
		} 
	};

}


template<class TS,class TD>
struct Threshold_LTVal_L_ZERO :  public fe2< TS, C1, TD, C1 >           
{	        

		FE_SSE2_REF                                                 

		IV SSE2_Init() {}

		IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
		{         
			 COPY::copy_128si_sse2_S(r.src1[0].i,r.dst[0].i);
		}      
		IV REFR(const TS *s,TS *d) const                        // REFR Pixel function
		{         
			THREF::CopyS(s,d);
		}    
};

} // namespace OPT_LEVEL

#endif


