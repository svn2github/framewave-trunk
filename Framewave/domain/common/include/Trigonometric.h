/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __TRIGONOMETRIC_H__
#define __TRIGONOMETRIC_H__

#include "fe.h"

// Constants

SYS_FORCEALIGN_16 extern const double atanhi_ds[];
SYS_FORCEALIGN_16 extern const double atanlo_ds[];
SYS_FORCEALIGN_16 extern const double aT_ds[];
SYS_FORCEALIGN_16 extern const int xmmDatad[];
SYS_FORCEALIGN_16 extern const float atanhi_fs[];
SYS_FORCEALIGN_16 extern const float atanlo_fs[];
SYS_FORCEALIGN_16 extern const float aT_fs[];
SYS_FORCEALIGN_16 extern const int xmmDataf[];
SYS_FORCEALIGN_16 extern const double Atan2d_constants_d[];
SYS_FORCEALIGN_16 extern const int Atan2d_constants_i[];
SYS_FORCEALIGN_16 extern const float Atan2f_constants_f[];
SYS_FORCEALIGN_16 extern const int Atan2f_constants_i[];


namespace OPT_LEVEL
{

///////////////////////////////////////// DATA TYPES FOR SETUP ///////////////////////////////////////////////////
struct Atan_DATA
{
    XMM128 constants[8];
};
///////////////////////////////////////// ATAN DOUBLE SSE2 CODE //////////////////////////////////////////////////

IS __m128d Atan_Summation_d(__m128d &x)
    {
        __m128d z,w,s1,s2;
        __m128d temp1, temp2;

        //z = x*x;
        z = _mm_mul_pd(x, x);

        //w = z*z;
        w = _mm_mul_pd(z,z);

        // break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly 

        //s1 = z*(aT[0]+w*(aT[2]+w*(aT[4]+w*(aT[6]+w*(aT[8]+w*aT[10])))));
        temp1 = _mm_load_pd(&aT_ds[20]);    //load AT[10]

        s1 = _mm_mul_pd(w, temp1);          // w *AT[10]
        temp1 = _mm_load_pd(&aT_ds[16]);    //load AT[8]

        s1 = _mm_add_pd(s1, temp1);         //(AT[8] + w *AT[10])
        s1 = _mm_mul_pd(w, s1);             //w * (AT[8] + w *AT[10])
        temp1 = _mm_load_pd(&aT_ds[12]);    //load AT[6]

        s1 = _mm_add_pd(s1, temp1);         //AT[6] + (w * (AT[8] + w *AT[10]))
        s1 = _mm_mul_pd(w, s1);             //w * (AT[6] + (w * (AT[8] + w *AT[10])))
        temp1 = _mm_load_pd(&aT_ds[8]);     //load AT[4]

        s1 = _mm_add_pd(s1, temp1);         //AT[4] + (w * (AT[6] + (w * (AT[8] + w *AT[10]))))
        s1 = _mm_mul_pd(w, s1);             //w * (AT[4] + (w * (AT[6] + (w * (AT[8] + w *AT[10])))))
        temp1 = _mm_load_pd(&aT_ds[4]);     //load AT[2]

        s1 = _mm_add_pd(s1, temp1);         //AT[2] + w * (AT[4] + w * (AT[6] + w * (AT[8] + w *AT[10])))
        s1 = _mm_mul_pd(w, s1);             //w * (AT[2] + w * (AT[4] + w * (AT[6] + w * (AT[8] + w *AT[10]))))
        temp1 = _mm_load_pd(&aT_ds[0]);     //load AT[0]

        s1 = _mm_add_pd(s1, temp1);         //AT[0] + w * (AT[2] + w * (AT[4] + w * (AT[6] + w * (AT[8] + w *AT[10]))))
        s1 = _mm_mul_pd(z, s1);             //z * ( AT[0] + w * (AT[2] + w * (AT[4] + w * (AT[6] + w * (AT[8] + w *AT[10])))))

        //s2 = w*(aT[1]+w*(aT[3]+w*(aT[5]+w*(aT[7]+w*aT[9]))));
        temp2 = _mm_load_pd(&aT_ds[18]);    //load AT[9]

        s2 = _mm_mul_pd(w, temp2);          // w *AT[9]
        temp2 = _mm_load_pd(&aT_ds[14]);    //load AT[7]

        s2 = _mm_add_pd(s2, temp2);         //(AT[7] + w *AT[9])
        s2 = _mm_mul_pd(w, s2);             //w * (AT[7] + w *AT[7])
        temp2 = _mm_load_pd(&aT_ds[10]);    //load AT[5]

        s2 = _mm_add_pd(s2, temp2);         //AT[5] + (w * (AT[7] + w *AT[9]))
        s2 = _mm_mul_pd(w, s2);             //w * (AT[5] + (w * (AT[7] + w *AT[9])))
        temp2 = _mm_load_pd(&aT_ds[6]);     //load AT[3]

        s2 = _mm_add_pd(s2, temp2);         //AT[3] + (w * (AT[5] + (w * (AT[7] + w *AT[9]))))
        s2 = _mm_mul_pd(w, s2);             //w * (AT[3] + (w * (AT[5] + (w * (AT[7] + w *AT[9])))))
        temp2 = _mm_load_pd(&aT_ds[2]);     //load AT[1]// high

        s2 = _mm_add_pd(s2, temp2);         //AT[1] + w * (AT[3] + w * (AT[5] + w * (AT[7] + w *AT[9])))
        s2 = _mm_mul_pd(w, s2);             //w * (AT[1] + w * (AT[3] + w * (AT[5] + w * (AT[7] + w *AT[9]))))

        temp2 = _mm_add_pd(s1, s2);         //(s1+s2)
        return (_mm_mul_pd(x, temp2));      //x*(s1+s2)
    }

ISV Atan_64f_SETUP_S(struct Atan_DATA &data)
    {
        ////use set ones and shifts here..
        data.constants[0].i= _mm_setzero_si128();//set to zeros
        data.constants[0].i = _mm_cmpeq_epi32(data.constants[0].i,data.constants[0].i);//set to ones
        data.constants[0].i = _mm_slli_epi64(data.constants[0].i, 32);// mask to get the higher 4 bytes in the given double number

        data.constants[1].i = _mm_set_epi32(0x7fffffff, 0x44100000, 0x7ff00000, 0x3fdc0000);
        data.constants[2].i = _mm_set_epi32(0x3e200000, 0x3ff30000, 0x3fe60000, 0x40038000);

        data.constants[3].d = _mm_set_pd(-1.0,-1.0);
        data.constants[4].d = _mm_set_pd(1.5,2.0);
        data.constants[5].d = _mm_set_pd(1.0E300,1.5707963267948966192323399573677);
        data.constants[6].i = _mm_set_epi32(0x3ff30000, 0x00000000, 0x3ff30000, 0x00000000);
        data.constants[7].i = _mm_set_epi32(0x40038000, 0x00000000, 0x40038000, 0x00000000);
    }

IS __m128i Atan_64f_SSE2(XMM128 &src, const struct Atan_DATA &data)
{
    XMM128 x,temp;
    __m128d num,den;
    __m128i ix, dst;
    __m128i mask1,mask2,mask3, mask4, mask5, mask6, mask7;
    int id;

    //make dst to zero
    dst = _mm_setzero_si128();

    //ix = higher part of input without the sign bits
    ix = _mm_srli_epi64(_mm_slli_epi64(src.i, 1), 1);
    ix = _mm_and_si128(ix, data.constants[0].i);//ix = (v,0,v,0)

    mask1 = _mm_cmplt_epi32(ix, _mm_load_si128((__m128i*)&xmmDatad[4]));//mask1 = (v,0,v,0)
    mask2 = _mm_cmplt_epi32(ix, _mm_load_si128((__m128i*)&xmmDatad)); //mask2 = (v,0,v,0)
    mask3 = _mm_andnot_si128(mask2, mask1);

    id = _mm_movemask_epi8(mask3);
    //else 3rd case
    if(id>0)
        {
            mask5 = _mm_cmplt_epi32(ix, data.constants[6].i );//mask4= (v,i,v,i)
            mask4 = _mm_andnot_si128(mask5, mask3);//mask4 = (v,0,v,0)
            id = _mm_movemask_epi8(mask4);
            //else 2nd case
            if(id>0)
                {
                    mask7 = _mm_cmplt_epi32(ix, data.constants[7].i);
                    mask6 = _mm_andnot_si128(mask7,mask4);//mask3 = (v,0,v,0)
                    id = _mm_movemask_epi8(mask6);
                    //else 2nd case
                    if(id>0)
                        {
                            //x = fabs(x);
                            x.i = _mm_slli_epi64(src.i, 1);
                            x.i = _mm_srli_epi64(x.i, 1);// make the numbers positive..

                            //x  = -1.0/x;
                            x.d = _mm_div_pd(data.constants[3].d, x.d);//temp.d, x.d);// -1.0/x
                            mask6 = _mm_shuffle_epi32(mask6, _MM_SHUFFLE(3,3,1,1));
                            //mask6 = _mm_or_si128(mask6, _mm_srli_si128(mask6, 4));

                            temp.d = Atan_Summation_d(x.d);//returns x*(s1+s2)

                            //z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
                            temp.d = _mm_sub_pd(temp.d, _mm_load_pd(&atanlo_ds[6]));//(x*(s1+s2) - atanlo[id])
                                    // subtract         // load
                            temp.d = _mm_sub_pd(temp.d, x.d);//(x*(s1+s2) - atanlo[id]) - x
                            temp.d = _mm_sub_pd(_mm_load_pd(&atanhi_ds[6]), temp.d);// atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x)
                            temp.i = _mm_or_si128(temp.i, _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 63));
                                    //change sign of result     // get the sign bits
                            dst = _mm_or_si128(dst, _mm_and_si128(mask6, temp.i));
                            if(id == 0x0000f0f0) return dst;
                        }

                    mask7 = _mm_and_si128(mask4, mask7);//mask4 = (v,0,v,0)
                    id = _mm_movemask_epi8(mask7);
                    //if (ix < 0x40038000)
                    if(id>0)
                        {
                            //x = fabs(x);
                            x.i = _mm_slli_epi64(src.i, 1);
                            x.i = _mm_srli_epi64(x.i, 1);// make the numbers positive..

                            //x  = (x-1.5)/(one1+1.5*x);
                            temp.i = _mm_shuffle_epi32(data.constants[4].i, _MM_SHUFFLE(3,2,3,2));//load 1.5
                            num = _mm_sub_pd(x.d, temp.d);// x - 1.5
                            den = _mm_mul_pd(x.d, temp.d);// 1.5 *x
                            //temp.i = _mm_shuffle_epi32(data.constants[3].i, _MM_SHUFFLE(3,2,3,2));//load 1.0
                            temp.i = _mm_srli_epi64(_mm_slli_epi64(data.constants[3].i, 1),1);//load 1.0
                            den = _mm_add_pd(den, temp.d) ;// one + 1.5 *x
                            x.d = _mm_div_pd(num, den);
                            mask7 = _mm_shuffle_epi32(mask7, _MM_SHUFFLE(3,3,1,1));//distribute the mask

                            temp.d = Atan_Summation_d(x.d);//returns x*(s1+s2)

                            //z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
                            temp.d = _mm_sub_pd(temp.d, _mm_load_pd(&atanlo_ds[4]));//(x*(s1+s2) - atanlo[id])
                                    // subtract         // load
                            temp.d = _mm_sub_pd(temp.d, x.d);//(x*(s1+s2) - atanlo[id]) - x
                            temp.d = _mm_sub_pd(_mm_load_pd(&atanhi_ds[4]), temp.d);// atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x)
                            temp.i = _mm_or_si128(temp.i, _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 63));
                                    //change sign of result     // get the sign bits
                            dst = _mm_or_si128(dst, _mm_and_si128(mask7, temp.i));

                        }
                }

            mask5 = _mm_and_si128(mask5,mask3);//mask5= (v,0,v,0)
            id = _mm_movemask_epi8(mask5);
            //if (ix < 0x3ff30000)
            if (id >0)
                {
                    //__m128i mask6,mask7;
                    mask6 = _mm_cmplt_epi32(ix, _mm_shuffle_epi32(data.constants[2].i, _MM_SHUFFLE(1,2,1,0)));//mask6 = (v,i,v,i)
                    mask7 = _mm_and_si128(mask6, mask5);
                    mask6 = _mm_andnot_si128(mask6, mask5);
                    id = _mm_movemask_epi8(mask6);
                    //else
                    if(id>0)
                        {
                            //x = fabs(x);
                            x.i = _mm_slli_epi64(src.i, 1);
                            x.i = _mm_srli_epi64(x.i, 1);// make the numbers positive..

                            //x  = (x-one1)/(x+one1);
                            //temp.i = _mm_shuffle_epi32(data.constants[3].i, _MM_SHUFFLE(3,2,3,2));//load one
                            temp.i = _mm_srli_epi64(_mm_slli_epi64(data.constants[3].i, 1),1);//load 1.0
                            num = _mm_sub_pd(x.d, temp.d);// x - one..
                            den = _mm_add_pd(x.d, temp.d);//x + one
                            x.d = _mm_div_pd(num, den);   //(x-one)/(x+one)
                            mask6 = _mm_shuffle_epi32(mask6, _MM_SHUFFLE(3,3,1,1));//distribute the mask

                            temp.d = Atan_Summation_d(x.d);//returns x*(s1+s2)

                            //z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
                            temp.d = _mm_sub_pd(temp.d, _mm_load_pd(&atanlo_ds[2]));//(x*(s1+s2) - atanlo[id])
                                    // subtract         // load
                            temp.d = _mm_sub_pd(temp.d, x.d);//(x*(s1+s2) - atanlo[id]) - x
                            temp.d = _mm_sub_pd(_mm_load_pd(&atanhi_ds[2]), temp.d);// atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x)
                            temp.i = _mm_or_si128(temp.i, _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 63));
                                    //change sign of result     // get the sign bits
                            dst = _mm_or_si128(dst, _mm_and_si128(mask6, temp.i));

                        }

                    id = _mm_movemask_epi8(mask7);
                    //if (ix < 0x3fe60000)
                    if(id>0)
                        {
                            //x = fabs(x);
                            x.i = _mm_slli_epi64(src.i, 1);
                            x.i = _mm_srli_epi64(x.i, 1);// make the numbers positive..

                            //x = (2.0*x-one1)/(2.0+x);
                            temp.i = _mm_shuffle_epi32(data.constants[4].i, _MM_SHUFFLE(1,0,1,0));//load 2.0
                            den = _mm_add_pd(temp.d, x.d);//2.0 + x
                            num  = _mm_mul_pd(x.d, temp.d);//2 *x
                            //temp.i = _mm_shuffle_epi32(data.constants[3].i, _MM_SHUFFLE(3,2,3,2));//load 1.0
                            temp.i = _mm_srli_epi64(_mm_slli_epi64(data.constants[3].i, 1),1);//load 1.0
                            num  = _mm_sub_pd(num, temp.d);//2*x -1.0
                            x.d = _mm_div_pd(num, den);//(2.0*x-one1)/(2.0+x)
                            mask7 = _mm_shuffle_epi32(mask7, _MM_SHUFFLE(3,3,1,1));//distribute the mask

                            temp.d = Atan_Summation_d(x.d);//returns x*(s1+s2)

                              //z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
                            temp.d = _mm_sub_pd(temp.d, _mm_load_pd(&atanlo_ds[0]));//(x*(s1+s2) - atanlo[id])
                                    // subtract         // load
                            temp.d = _mm_sub_pd(temp.d, x.d);//(x*(s1+s2) - atanlo[id]) - x
                            temp.d = _mm_sub_pd(_mm_load_pd(&atanhi_ds[0]), temp.d);// atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x)
                            temp.i = _mm_or_si128(temp.i, _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 63));
                                    //change sign of result     // get the sign bits
                            dst = _mm_or_si128(dst, _mm_and_si128(mask7, temp.i));

                        }

                }
        }

    mask1 = _mm_andnot_si128(mask1, data.constants[0].i);
    id =  _mm_movemask_epi8(mask1);//get the mask as integer
    //if(ix>=0x44100000)
    if(id>0)
        {
              //__m128i mask4;
            temp.i = _mm_shuffle_epi32(data.constants[1].i, _MM_SHUFFLE(1,2,1,0));//get 7ff00000 to correct places
            mask4 = _mm_or_si128(_mm_cmpgt_epi32(ix,temp.i),_mm_and_si128(_mm_cmpeq_epi32(ix, temp.i),_mm_slli_si128(_mm_andnot_si128(_mm_cmpeq_epi32(src.i, _mm_setzero_si128()),_mm_shuffle_epi32(data.constants[0].i, _MM_SHUFFLE(2,3,2,1))),4)));
               //or both    //cmp ix > 7ff00000       //and both    //cmp ix = 7ff00000       //shift the mask //negate it      //check lx = 0
            //mask2 = (v,0,v,0)
            id =  _mm_movemask_epi8(mask4);//get the mask as integer
            //if(ix>0x7ff00000||(ix==0x7ff00000&&(__LO(x)!=0)))
            if(id>0)
                {
                    //__m128i mask5;
                    mask5 = _mm_and_si128(mask4,mask1);// form the correct mask
                    mask5 = _mm_shuffle_epi32(mask5, _MM_SHUFFLE(3,3,1,1));//distribute the mask
                    //reg.dst[0].f64[i] = x+x;
                    temp.d = _mm_add_pd(src.d, src.d);
                    dst = _mm_or_si128(dst,_mm_and_si128(temp.i, mask5));
                    //retain prev. dst        // retain only masked    //x + x
                }

            mask4 = _mm_andnot_si128(mask4, mask1);//mask2 = (v,0,v,0)
            id =  _mm_movemask_epi8(mask4);//get the mask as integer
            //if(hx>0) or <= 0
            if(id>0)
                {
                    //reg.dst[0].f64[i] = atanhi[3]+atanlo[3];
                    temp.i = _mm_srli_epi64(src.i, 63);
                    temp.i = _mm_slli_epi64(temp.i, 63);// get the sign bits in hx
                    mask4 = _mm_shuffle_epi32(mask4, _MM_SHUFFLE(3,3,1,1));//distribute the mask
                    dst = _mm_or_si128(dst, _mm_and_si128(_mm_or_si128(temp.i, _mm_shuffle_epi32(data.constants[5].i, _MM_SHUFFLE(1,0,1,0))), mask4));
                    //retain prev. dst    //retain only masked //retain signs of hi // tanhi[3] + tanlo[3]
                }
        }

    mask2 = _mm_andnot_si128(mask1, mask2);
    id =  _mm_movemask_epi8(mask2);//get the mask as integer
    //if (ix < 0x3fdc0000)
    if(id>0)
        {
            mask3 = _mm_and_si128(_mm_cmplt_epi32(ix, _mm_shuffle_epi32(data.constants[2].i, _MM_SHUFFLE(3,2,3,0))), data.constants[0].i);// mask3= (v,0,v,0)
            id =  _mm_movemask_epi8(mask3);//get the mask as integer
            //if (ix < 0x3e200000)
            if(id>0)
                {
                    temp.i = _mm_shuffle_epi32(data.constants[5].i, _MM_SHUFFLE(3,2,3,2));
                    x.i = _mm_srli_epi64(_mm_slli_epi64(data.constants[3].i, 1),1);//load 1.0
                    temp.d = _mm_cmpgt_pd(_mm_add_pd(temp.d, src.d),x.d);
                            //huge + x           // get huge to place     // get one to place
                    id = _mm_movemask_epi8(temp.i);
                    //if(huge1+x>one1)
                    if(id>0)
                        {
                            //__m128i mask4;
                            mask4 = _mm_and_si128(mask3,mask2);
                            mask4 = _mm_and_si128(mask4,temp.i);
                            mask4 = _mm_shuffle_epi32(mask4, _MM_SHUFFLE(3,3,1,1));// distribute the mask
                            dst = _mm_or_si128(dst, _mm_and_si128(src.i, mask4));
                        }
                }
            mask3 = _mm_andnot_si128(mask3, mask2);//returns x*(s1+s2)
            mask3 = _mm_shuffle_epi32(mask3, _MM_SHUFFLE(3,3,1,1)); //distribute the mask
            temp.d = Atan_Summation_d(src.d);
            temp.d = _mm_sub_pd(src.d, temp.d);//x - x*(s1+s2)
            dst = _mm_or_si128(dst, _mm_and_si128(temp.i, mask3));
        }
    return dst;
}

ISV Atan_64f_A53_SSE2_S(RegFile &reg, const struct Atan_DATA &data)
{
    reg.dst[0].i = Atan_64f_SSE2(reg.src1[0], data);
}
ISV Atan_64f_inplace_SSE2_S(RegFile &reg, const struct Atan_DATA &data)
{
    reg.dst[0].i = Atan_64f_SSE2(reg.dst[0], data);
}

///////////////////////////////////////// ATAN FLOAT SSE2 CODE ////////////////////////////////////////////

IS __m128 Atan_Summation_f(__m128 &x)
    {
        __m128 z,w,s1,s2;
        __m128 temp1, temp2;

        //z = x*x;
        z = _mm_mul_ps(x, x);

        //w = z*z;
        w = _mm_mul_ps(z,z);

        // break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly 

        //s1 = z*(aT[0]+w*(aT[2]+w*(aT[4]+w*(aT[6]+w*(aT[8]+w*aT[10])))));
        temp1 = _mm_load_ps(&aT_fs[40]);    //load AT[10]

        s1 = _mm_mul_ps(w, temp1);          // w *AT[10]
        temp1 = _mm_load_ps(&aT_fs[32]);    //load AT[8]

        s1 = _mm_add_ps(s1, temp1);         //(AT[8] + w *AT[10])
        s1 = _mm_mul_ps(w, s1);             //w * (AT[8] + w *AT[10])
        temp1 = _mm_load_ps(&aT_fs[24]);    //load AT[6]

        s1 = _mm_add_ps(s1, temp1);         //AT[6] + (w * (AT[8] + w *AT[10]))
        s1 = _mm_mul_ps(w, s1);             //w * (AT[6] + (w * (AT[8] + w *AT[10])))
        temp1 = _mm_load_ps(&aT_fs[16]);    //load AT[4]

        s1 = _mm_add_ps(s1, temp1);         //AT[4] + (w * (AT[6] + (w * (AT[8] + w *AT[10]))))
        s1 = _mm_mul_ps(w, s1);             //w * (AT[4] + (w * (AT[6] + (w * (AT[8] + w *AT[10])))))
        temp1 = _mm_load_ps(&aT_fs[8]);     //load AT[2]

        s1 = _mm_add_ps(s1, temp1);         //AT[2] + w * (AT[4] + w * (AT[6] + w * (AT[8] + w *AT[10])))
        s1 = _mm_mul_ps(w, s1);             //w * (AT[2] + w * (AT[4] + w * (AT[6] + w * (AT[8] + w *AT[10]))))
        temp1 = _mm_load_ps(&aT_fs[0]);     //load AT[0]

        s1 = _mm_add_ps(s1, temp1);         //AT[0] + w * (AT[2] + w * (AT[4] + w * (AT[6] + w * (AT[8] + w *AT[10]))))
        s1 = _mm_mul_ps(z, s1);             //z * ( AT[0] + w * (AT[2] + w * (AT[4] + w * (AT[6] + w * (AT[8] + w *AT[10])))))

        //s2 = w*(aT[1]+w*(aT[3]+w*(aT[5]+w*(aT[7]+w*aT[9]))));
        temp2 = _mm_load_ps(&aT_fs[36]);    //load AT[9]

        s2 = _mm_mul_ps(w, temp2);          // w *AT[9]
        temp2 = _mm_load_ps(&aT_fs[28]);    //load AT[7]

        s2 = _mm_add_ps(s2, temp2);         //(AT[7] + w *AT[9])
        s2 = _mm_mul_ps(w, s2);             //w * (AT[7] + w *AT[7])
        temp2 = _mm_load_ps(&aT_fs[20]);    //load AT[5]

        s2 = _mm_add_ps(s2, temp2);         //AT[5] + (w * (AT[7] + w *AT[9]))
        s2 = _mm_mul_ps(w, s2);             //w * (AT[5] + (w * (AT[7] + w *AT[9])))
        temp2 = _mm_load_ps(&aT_fs[12]);    //load AT[3]

        s2 = _mm_add_ps(s2, temp2);         //AT[3] + (w * (AT[5] + (w * (AT[7] + w *AT[9]))))
        s2 = _mm_mul_ps(w, s2);             //w * (AT[3] + (w * (AT[5] + (w * (AT[7] + w *AT[9])))))
        temp2 = _mm_load_ps(&aT_fs[4]);     //load AT[1]// high

        s2 = _mm_add_ps(s2, temp2);         //AT[1] + w * (AT[3] + w * (AT[5] + w * (AT[7] + w *AT[9])))
        s2 = _mm_mul_ps(w, s2);             //w * (AT[1] + w * (AT[3] + w * (AT[5] + w * (AT[7] + w *AT[9]))))

        temp2 = _mm_add_ps(s1, s2);         //(s1+s2)
        return (_mm_mul_ps(x, temp2));      //x*(s1+s2)

    }

ISV Atan_32f_SETUP_S( struct Atan_DATA &data)
{
    //use set ones and shifts here..
    data.constants[0].i = _mm_setzero_si128();//set to zeros
    data.constants[0].i = _mm_cmpeq_epi32(data.constants[0].i,data.constants[0].i);//set to ones
    //data.constants[0].i = _mm_slli_epi64(data.constants[0].i, 32);// mask to get the higher 4 bytes in the given double number

    data.constants[1].i = _mm_set_epi32(0x7fffffff, 0x50800000, 0x7f800000, 0x3ee00000);
    data.constants[2].i = _mm_set_epi32(0x31000000, 0x3f980000, 0x3f300000, 0x401c0000);

    data.constants[3].f = _mm_set_ps(-1.0,-1.0,-1.0,-1.0);
    data.constants[4].f = _mm_set_ps(1.5,1.5,2.0,2.0);
    data.constants[5].f = _mm_set_ps(1.0E30F,1.0E30F,1.570796326797894159F,1.570796326797894159F);
    data.constants[6].i = _mm_set_epi32(0x3f980000, 0x3f980000, 0x3f980000, 0x3f980000);
    data.constants[7].i = _mm_set_epi32(0x401c0000, 0x401c0000, 0x401c0000, 0x401c0000);
}

IS __m128i Atan_32f_SSE2(XMM128 &src, const struct Atan_DATA &data)
{
XMM128 x,temp;
__m128 num,den;
__m128i ix,dst;
__m128i mask1,mask2,mask3, mask4, mask5, mask6, mask7;
int id;

//make dst to zero
dst = _mm_setzero_si128();

//ix = higher part of input without the sign bits
ix = _mm_srli_epi32(_mm_slli_epi32(src.i, 1), 1);
//ix = _mm_and_si128(ix, data.constants[0].i);//ix = (v,0,v,0)

mask1 = _mm_cmplt_epi32(ix, _mm_load_si128((__m128i*)&xmmDataf[4]));//mask1 = (v,v,v,v)
mask2 = _mm_cmplt_epi32(ix, _mm_load_si128((__m128i*)&xmmDataf));   //mask2 = (v,v,v,v)
mask3 = _mm_andnot_si128(mask2, mask1);

id = _mm_movemask_epi8(mask3);
//else 3rd case
if(id>0)
{
    mask5 = _mm_cmplt_epi32(ix, data.constants[6].i );//mask4= (v,v,v,v)
    mask4 = _mm_andnot_si128(mask5, mask3);           //mask4 = (v,v,v,v)
    id = _mm_movemask_epi8(mask4);
    //else 2nd case
    if(id>0)
        {
            mask7 = _mm_cmplt_epi32(ix, data.constants[7].i);
            mask6 = _mm_andnot_si128(mask7,mask4);  //mask3 = (v,v,v,v)
            id = _mm_movemask_epi8(mask6);
            //else 2nd case
            if(id>0)
                {
                    //x = fabs(x);
                    x.i = _mm_slli_epi32(src.i, 1);
                    x.i = _mm_srli_epi32(x.i, 1);   // make the numbers positive..

                    //x  = -1.0/x;
                    x.f = _mm_div_ps(data.constants[3].f, x.f);//temp.d, x.d);// -1.0/x
                    //mask6 = _mm_shuffle_epi32(mask6, _MM_SHUFFLE(3,3,1,1));
                    //mask6 = _mm_or_si128(mask6, _mm_srli_si128(mask6, 4));

                    temp.f = Atan_Summation_f(x.f); //returns x*(s1+s2)

                    //z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
                    temp.f = _mm_sub_ps(temp.f, _mm_load_ps(&atanlo_fs[12]));//(x*(s1+s2) - atanlo[id])
                            // subtract         // load
                    temp.f = _mm_sub_ps(temp.f, x.f);//(x*(s1+s2) - atanlo[id]) - x
                    temp.f = _mm_sub_ps(_mm_load_ps(&atanhi_fs[12]), temp.f);// atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x)

                    temp.i = _mm_or_si128(temp.i, _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31));
                            //change sign of result     // get the sign bits
                    dst = _mm_or_si128(dst, _mm_and_si128(mask6, temp.i));

                    if(id == 0x0000ffff) return dst;
                }

            mask7 = _mm_and_si128(mask4, mask7);//mask4 = (v,v,v,v)
            id = _mm_movemask_epi8(mask7);
            //if (ix < 0x40038000)
            if(id>0)
                {
                    //x = fabs(x);
                    x.i = _mm_slli_epi32(src.i, 1);
                    x.i = _mm_srli_epi32(x.i, 1);// make the numbers positive..

                    //x  = (x-1.5)/(one1+1.5*x);
                    temp.i = _mm_shuffle_epi32(data.constants[4].i, _MM_SHUFFLE(3,2,3,2));//load 1.5
                    num = _mm_sub_ps(x.f, temp.f);// x - 1.5
                    den = _mm_mul_ps(x.f, temp.f);// 1.5 *x
                    //temp.i = _mm_shuffle_epi32(data.constants[3].i, _MM_SHUFFLE(3,2,3,2));//load 1.0
                    temp.i = _mm_srli_epi32(_mm_slli_epi32(data.constants[3].i, 1),1);//load 1.0
                    den = _mm_add_ps(den, temp.f);// one + 1.5 *x
                    x.f = _mm_div_ps(num, den);
                    //mask7 = _mm_shuffle_epi32(mask7, _MM_SHUFFLE(3,3,1,1));//distribute the mask

                    temp.f = Atan_Summation_f(x.f); //returns x*(s1+s2)

                    //z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
                    temp.f = _mm_sub_ps(temp.f, _mm_load_ps(&atanlo_fs[8]));//(x*(s1+s2) - atanlo[id])
                            // subtract         // load
                    temp.f = _mm_sub_ps(temp.f, x.f);//(x*(s1+s2) - atanlo[id]) - x
                    temp.f = _mm_sub_ps(_mm_load_ps(&atanhi_fs[8]), temp.f);// atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x)

                    temp.i = _mm_or_si128(temp.i, _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31));
                            //change sign of result     // get the sign bits
                    dst = _mm_or_si128(dst, _mm_and_si128(mask7, temp.i));
                }
        }

    mask5 = _mm_and_si128(mask5,mask3);//mask5= (v,v,v,v)
    id = _mm_movemask_epi8(mask5);
    //if (ix < 0x3ff30000)
    if (id >0)
        {
            //__m128i mask6,mask7;
            mask6 = _mm_cmplt_epi32(ix, _mm_shuffle_epi32(data.constants[2].i, _MM_SHUFFLE(1,1,1,1)));//mask6 = (v,v,v,v)
            mask7 = _mm_and_si128(mask6, mask5);
            mask6 = _mm_andnot_si128(mask6, mask5);
            id = _mm_movemask_epi8(mask6);
            //else
            if(id>0)
                {
                    //x = fabs(x);
                    x.i = _mm_slli_epi32(src.i, 1);
                    x.i = _mm_srli_epi32(x.i, 1);// make the numbers positive..

                    //x  = (x-one1)/(x+one1);
                    //temp.i = _mm_shuffle_epi32(data.constants[3].i, _MM_SHUFFLE(3,2,3,2));//load one
                    temp.i = _mm_srli_epi32(_mm_slli_epi32(data.constants[3].i, 1),1);//load 1.0
                    num = _mm_sub_ps(x.f, temp.f);// x - one..
                    den = _mm_add_ps(x.f, temp.f);//x + one
                    x.f = _mm_div_ps(num, den);//(x-one)/(x+one)
                    //mask6 = _mm_shuffle_epi32(mask6, _MM_SHUFFLE(3,3,1,1));//distribute the mask

                    temp.f = Atan_Summation_f(x.f); //returns x*(s1+s2)

                    //z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
                    temp.f = _mm_sub_ps(temp.f, _mm_load_ps(&atanlo_fs[4]));//(x*(s1+s2) - atanlo[id])
                            // subtract         // load
                    temp.f = _mm_sub_ps(temp.f, x.f);//(x*(s1+s2) - atanlo[id]) - x
                    temp.f = _mm_sub_ps(_mm_load_ps(&atanhi_fs[4]), temp.f);// atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x)

                    temp.i = _mm_or_si128(temp.i, _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31));
                            //change sign of result     // get the sign bits
                    dst = _mm_or_si128(dst, _mm_and_si128(mask6, temp.i));
                }

            id = _mm_movemask_epi8(mask7);
            //if (ix < 0x3fe60000)
            if(id>0)
                {
                    //x = fabs(x);
                    x.i = _mm_slli_epi32(src.i, 1);
                    x.i = _mm_srli_epi32(x.i, 1);// make the numbers positive..

                    //x = (2.0*x-one1)/(2.0+x);
                    temp.i = _mm_shuffle_epi32(data.constants[4].i, _MM_SHUFFLE(1,0,1,0));//load 2.0
                    den = _mm_add_ps(temp.f, x.f);//2.0 + x
                    num  = _mm_mul_ps(x.f, temp.f);//2 *x
                    //temp.i = _mm_shuffle_epi32(data.constants[3].i, _MM_SHUFFLE(3,2,3,2));//load 1.0
                    temp.i = _mm_srli_epi32(_mm_slli_epi32(data.constants[3].i, 1),1);//load 1.0
                    num  = _mm_sub_ps(num, temp.f);//2*x -1.0
                    x.f = _mm_div_ps(num, den);//(2.0*x-one1)/(2.0+x)
                    //mask7 = _mm_shuffle_epi32(mask7, _MM_SHUFFLE(3,3,1,1));//distribute the mask

                    temp.f = Atan_Summation_f(x.f); //returns x*(s1+s2)

                    //z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
                    temp.f = _mm_sub_ps(temp.f, _mm_load_ps(&atanlo_fs[0]));//(x*(s1+s2) - atanlo[id])
                            // subtract         // load
                    temp.f = _mm_sub_ps(temp.f, x.f);//(x*(s1+s2) - atanlo[id]) - x
                    temp.f = _mm_sub_ps(_mm_load_ps(&atanhi_fs[0]), temp.f);// atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x)

                    temp.i = _mm_or_si128(temp.i, _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31));
                            //change sign of result     // get the sign bits
                    dst = _mm_or_si128(dst, _mm_and_si128(mask7, temp.i));
                }

        }
}

mask1 = _mm_andnot_si128(mask1, data.constants[0].i);
id =  _mm_movemask_epi8(mask1);//get the mask as integer
//if(ix>=0x44100000)
if(id>0)
{
   // __m128i mask4;
    temp.i = _mm_shuffle_epi32(data.constants[1].i, _MM_SHUFFLE(1,1,1,1));//get 7ff00000 to correct places
    //mask4 = _mm_or_si128(_mm_cmpgt_epi32(ix,temp.i),_mm_and_si128(_mm_cmpeq_epi32(ix, temp.i),_mm_slli_si128(_mm_andnot_si128(_mm_cmpeq_epi32(reg.src1[0].i, _mm_setzero_si128()),_mm_shuffle_epi32(data.constants[0].i, _MM_SHUFFLE(2,3,2,1))),4)));
            //or both    //cmp ix > 7ff00000       //and both    //cmp ix = 7ff00000       //shift the mask //negate it      //check lx = 0
            //mask2 = (v,0,v,0)
    mask4 = _mm_cmpgt_epi32(ix,temp.i);
    id =  _mm_movemask_epi8(mask4);//get the mask as integer
    //if(ix>0x7ff00000||(ix==0x7ff00000&&(__LO(x)!=0)))
    if(id>0)
        {
        //__m128i mask5;
        mask5 = _mm_and_si128(mask4,mask1);// form the correct mask
        //mask5 = _mm_shuffle_epi32(mask5, _MM_SHUFFLE(3,3,1,1));//distribute the mask
        //reg.dst[0].f64[i] = x+x;
        temp.f = _mm_add_ps(src.f, src.f);
        dst = _mm_or_si128(dst,_mm_and_si128(temp.i, mask5));
              //retain prev. dst        // retain only masked    //x + x
        }
    mask4 = _mm_andnot_si128(mask4, mask1);//mask2 = (v,v,v,v)
    id =  _mm_movemask_epi8(mask4);//get the mask as integer
    //if(hx>0) or <= 0
    if(id>0)
        {
        //reg.dst[0].f64[i] = atanhi[3]+atanlo[3];
        temp.i = _mm_srli_epi32(src.i, 31);
        temp.i = _mm_slli_epi32(temp.i, 31);// get the sign bits in hx
        //mask4 = _mm_shuffle_epi32(mask4, _MM_SHUFFLE(3,3,1,1));//distribute the mask
        dst = _mm_or_si128(dst, _mm_and_si128(_mm_or_si128(temp.i, _mm_shuffle_epi32(data.constants[5].i, _MM_SHUFFLE(1,0,1,0))), mask4));
              //retain prev. dst    //retain only masked //retain signs of hi // tanhi[3] + tanlo[3]
        }
}

mask2 = _mm_andnot_si128(mask1, mask2);
id =  _mm_movemask_epi8(mask2);//get the mask as integer
//if (ix < 0x3fdc0000)
if(id>0)
{
    //mask3 = _mm_and_si128(_mm_cmplt_epi32(ix, _mm_shuffle_epi32(data.constants[2].i, _MM_SHUFFLE(3,2,3,0))), data.constants[0].i);// mask3= (v,0,v,0)
    mask3 = _mm_cmplt_epi32(ix, _mm_shuffle_epi32(data.constants[2].i, _MM_SHUFFLE(3,3,3,3)));// mask3= (v,0,v,0)
    id    =  _mm_movemask_epi8(mask3);//get the mask as integer
    //if (ix < 0x3e200000)
    if(id>0)
        {
            temp.i = _mm_shuffle_epi32(data.constants[5].i, _MM_SHUFFLE(3,2,3,2));
            x.i    = _mm_srli_epi32(_mm_slli_epi32(data.constants[3].i, 1),1);//load 1.0
            temp.f = _mm_cmpgt_ps(_mm_add_ps(temp.f, src.f),x.f);
                     //huge + x           // get huge to place          // get one to place
            id     = _mm_movemask_epi8(temp.i);
            //if(huge1+x>one1)
            if(id>0)
                {
                //__m128i mask4;
                mask4 = _mm_and_si128(mask3,mask2);
                mask4 = _mm_and_si128(mask4,temp.i);
                //mask4 = _mm_shuffle_epi32(mask4, _MM_SHUFFLE(3,3,1,1));// distribute the mask
                dst   = _mm_or_si128(dst, _mm_and_si128(src.i, mask4));
                }
        }
    mask3  = _mm_andnot_si128(mask3, mask2);
    temp.f = Atan_Summation_f(src.f); //returns x*(s1+s2)
    temp.f = _mm_sub_ps(src.f, temp.f);//x - x*(s1+s2)
    dst    = _mm_or_si128(dst, _mm_and_si128(temp.i, mask3));
}
return dst;
}

ISV Atan_32f_A24_SSE2_S(RegFile &reg, const struct Atan_DATA &data)
{
    reg.dst[0].i = Atan_32f_SSE2(reg.src1[0], data);
}
ISV Atan_32f_inplace_SSE2_S(RegFile &reg, const struct Atan_DATA &data)
{
    reg.dst[0].i = Atan_32f_SSE2(reg.dst[0], data);
}

///////////////////////////////////////// ATAN2 DOUBLE SSE2 CODE ////////////////////////////////////////////

IS __m128i Atan2_64f_SSE2(XMM128 &y, XMM128 &x)
{
    //double z;
    XMM128 z;
    z.i = _mm_setzero_si128();
    XMM128 temp_m128;

    //int k,m,ix,iy;
    __m128i temp;
    __m128i k,ix,iy,signx,signy;
    __m128i mask1, mask2, mask3, mask4, mask5;

    //union 
    //{
    //    double d;
    //    int i[2];// higher part or lower part depends upon little ndian or big endian
    //    unsigned int ui[2];
    //}hx,hy;

    //hx.d = x;
    //ix = hx.i[1]&0x7fffffff;
    ix = _mm_slli_epi64(x.i, 1);
    ix = _mm_srli_epi64(ix, 1); // ix = (v2,lx2, v1, lx1)
    //ix = _mm_srli_epi64(ix, 33);
    //ix = _mm_slli_epi64(ix, 32);// ix = (v, 0, v, 0)
    
    //hy.d = y;
    //iy = hy.i[1]&0x7fffffff;
    iy = _mm_slli_epi64(y.i, 1);
    iy = _mm_srli_epi64(iy, 1);     //iy = (v2,ly2, v1, ly1)
    //iy = _mm_srli_epi64(iy, 33);
    //iy = _mm_slli_epi64(iy, 32);  // iy = (v, 0, v, 0)

    //m = ((hy.i[1]>>31)&1)|((hx.i[1]>>30)&2);    // 2*sign(x)+sign(y) 
 
    signx = _mm_srai_epi32(x.i, 31);    // get the signs of x and y
    signx = _mm_shuffle_epi32(signx, _MM_SHUFFLE(3,3,1,1));//distribute the sign signx = (s2,s2,s1,s1)
    signy = _mm_srai_epi32(y.i, 31);
    signy = _mm_shuffle_epi32(signy, _MM_SHUFFLE(3,3,1,1));
  
    // when y = 0 
    //if((iy|hy.ui[0])==0) {
    //    switch(m) {
    //    case 0:
    //    case 1: return y;      // atan(+-0,+anything)=+-0 
    //    case 2: return pi_d; // pi_d+tiny_d; // atan(+0,-anything) = pi 
    //    case 3: return -pi_d;//-pi_d-tiny_d; // atan(-0,-anything) =-pi 
    //    }
    //}

    //Check if y==0
    mask2 = _mm_shuffle_epi32(iy, _MM_SHUFFLE(2,2,0,0));// ly
    mask1 = _mm_shuffle_epi32(iy, _MM_SHUFFLE(3,3,1,1));// hy with + ive sign
    mask1 = _mm_or_si128(mask1, mask2);
    mask1 = _mm_cmpeq_epi32(mask1, _mm_setzero_si128());// mask1 = v2,v2,v1,v1)
    if(_mm_movemask_epi8(mask1))
        {
            mask2 = _mm_andnot_si128(signx, mask1);     // check if x is positive mask2 = (v2,v2,v1,v1)
            temp  = _mm_and_si128(y.i, mask2);          // get y
            z.i   = _mm_or_si128(z.i, temp);
            mask2 = _mm_and_si128(signx, mask1);        // for x is - ive mask2 = (v2,v2,v1,v1)
            if(_mm_movemask_epi8(mask2))
                {
                    temp  = _mm_load_si128((__m128i*)&Atan2d_constants_d[0]);
                    temp  = _mm_or_si128(temp, _mm_slli_epi64(signy, 63));
                    temp  = _mm_and_si128(temp, mask2);
                    z.i   = _mm_or_si128(z.i, temp);
                }
        }

    // when x is INF 
    //if(ix==0x7ff00000) {
    //    if(iy==0x7ff00000) {
    //    switch(m) {
    //        case 0: return  pi_o_4_d;  //pi_o_4_d+tiny_d;/* atan(+INF,+INF) 
    //        case 1: return -pi_o_4_d;  //-pi_o_4_d-tiny_d;/* atan(-INF,+INF)
    //        case 2: return  pi3_o_4_d; //(float)3.0*pi_o_4_d+tiny_d;/*atan(+INF,-INF)
    //        case 3: return -pi3_o_4_d; //(float)-3.0*pi_o_4_d-tiny_d;/*atan(-INF,-INF)
    //    }
    //    } else {
    //    switch(m) {
    //        case 2: return  pi_d;//pi_d+tiny_d  ;    // atan(+...,-INF) 
    //        case 3: return -pi_d;//-pi_d-tiny_d  ;   // atan(-...,-INF) 
    //    }
    //    }
    //}
    temp  = _mm_load_si128((__m128i*)&Atan2d_constants_i[0]);
    mask2 = _mm_cmpeq_epi32(ix, temp); // check if x = INF
    mask2 = _mm_shuffle_epi32(mask2, _MM_SHUFFLE(3,3,1,1));//distribute the mask mask2 = (v2,v2,v1,v1)
    if(_mm_movemask_epi8(mask2))
        {
            mask3 = _mm_cmpeq_epi32(iy, temp);      // check if y = INF
            mask3 = _mm_shuffle_epi32(mask3, _MM_SHUFFLE(3,3,1,1));// distribute the mask
            mask4 = _mm_and_si128(mask3, mask2);    // mask4 = (v2,v2,v1,v1)
            mask4 = _mm_and_si128(mask4, mask1);    // mask4 indicates that we entered second if
            mask5 = _mm_andnot_si128(signx, mask4); // check if x is positive mask5 =(v2,v2,v1,v1)
            if(_mm_movemask_epi8(mask5))
                {
                    temp  = _mm_load_si128((__m128i*)&Atan2d_constants_d[2]);
                    temp  = _mm_or_si128(temp, _mm_slli_epi64(signy, 63));
                    temp  = _mm_and_si128(temp, mask5);
                    z.i   = _mm_or_si128(z.i, temp);
                }
            mask5 = _mm_and_si128(signx, mask4);    // check if x is - ive mask5 =(v2,v2,v1,v1)
            if(_mm_movemask_epi8(mask5))
                {
                    temp  = _mm_load_si128((__m128i*)&Atan2d_constants_d[4]);
                    temp  = _mm_or_si128(temp, _mm_slli_epi64(signy, 63));
                    temp  = _mm_and_si128(temp, mask5);
                    z.i   = _mm_or_si128(z.i, temp);
                }
            mask3 = _mm_andnot_si128(mask3, mask2); // mask3 =(v2,v2,v1,v1)
            mask3 = _mm_and_si128(mask3, mask1);    // mask3 indicates x is INF and y!=INF
            if(_mm_movemask_epi8(mask3))
                {
                    temp  = _mm_load_si128((__m128i*)&Atan2d_constants_d[0]);
                    temp  = _mm_or_si128(temp, _mm_slli_epi64(signy, 63));
                    temp  = _mm_and_si128(temp, mask3);
                    z.i   = _mm_or_si128(z.i, temp);
                }
        }

    // compute y/x 
    //k = (iy-ix)>>20;
    k = _mm_sub_epi32(iy, ix);
    k = _mm_srai_epi32(k, 20); // k= (v2, invalid, v1,invalid)

    //if(hx.i[1]<0&&k<-60) z=0.0;     // |y|/x < -2**60 
    mask3 = _mm_cmplt_epi32(x.i, _mm_setzero_si128()); // mask3 = (v2,invalid,v1, invalid)
    mask4 = _mm_cmplt_epi32(k, _mm_load_si128((__m128i*)&Atan2d_constants_i[4]));// mask4 = (v2,invalid,v1, invalid)
    mask3 = _mm_and_si128(mask3, mask4);
    mask3 = _mm_shuffle_epi32(mask3, _MM_SHUFFLE(3,3,1,1)); // mask3 = (v2,v2,v1,v1)
    if(_mm_movemask_epi8(mask3))
        {
            mask4 = _mm_andnot_si128(mask2, mask3);
            mask4 = _mm_andnot_si128(mask1, mask4); // mask4 = (v2,v2,v1,v1)
            temp  = _mm_load_si128((__m128i*)&Atan2d_constants_d[10]);
            temp  = _mm_and_si128(temp, mask4);
            z.i   = _mm_or_si128(z.i, temp);
        }

    //else z= Atan_64f(fabs(y/x));    // safe to do y/x 
    mask4 = _mm_andnot_si128(mask3, _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128()));
    mask3 = _mm_andnot_si128(mask2, mask4);
    mask3 = _mm_andnot_si128(mask1, mask3);// mask3 = (v2,v2,v1,v1)
    if(_mm_movemask_epi8(mask3))
        {
            temp_m128.d = _mm_div_pd(y.d, x.d);
            temp_m128.i = _mm_slli_epi64(temp_m128.i, 1);
            temp_m128.i = _mm_srli_epi64(temp_m128.i, 1);
            Atan_DATA data;
            Atan_64f_SETUP_S(data);
            temp_m128.i = Atan_64f_SSE2(temp_m128, data);
            temp_m128.i = _mm_and_si128(temp_m128.i, mask3);
            z.i         = _mm_or_si128(z.i, temp_m128.i);
        }

    //switch (m) {
    //    case 0: return  z; // atan(+,+) 
    //    case 1: return -z; // atan(-,+) 
    //           
    //    case 2: return  pi_hi_d-(z-pi_lo_d); // atan(+,-) 
    //    default: // case 3 
    //    return  (z-pi_lo_d)-pi_hi_d; // atan(-,-) 
    //}
    mask2 = _mm_andnot_si128(mask2, _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128()));
    mask1 = _mm_andnot_si128(mask1, mask2);// mask1 indicates we are inside switch
    mask2 = _mm_andnot_si128(signx, mask1);//check x is positive and we are in last switch statement
    if(_mm_movemask_epi8(mask2))
        {
            temp = _mm_or_si128(z.i, _mm_slli_epi64(signy, 63));
            temp = _mm_and_si128(temp, mask2);
            z.i  = _mm_or_si128(z.i, temp);
        }
    mask2 = _mm_and_si128(mask1, signx);// x is negative and we are inside last switch
    if(_mm_movemask_epi8(mask2))
        {
        mask3 = _mm_andnot_si128(signy, mask2);//mask3 indicates y is positive,x is negative and we are inside switch
        if(_mm_movemask_epi8(mask3))
            {
            temp_m128.d = _mm_sub_pd(z.d, _mm_load_pd(&Atan2d_constants_d[8]));// z - pi_lo
            temp_m128.d = _mm_sub_pd(_mm_load_pd(&Atan2d_constants_d[6]), temp_m128.d);
            temp_m128.i = _mm_and_si128(temp_m128.i, mask3);
            z.i         = _mm_andnot_si128(mask3, z.i);// make the places of z to copy temp_m128i as zero
            z.i         = _mm_or_si128(z.i, temp_m128.i);
            }
        mask3 = _mm_and_si128(signy, mask2);
        if(_mm_movemask_epi8(mask3))
            {
            temp_m128.d = _mm_sub_pd(z.d, _mm_load_pd(&Atan2d_constants_d[8]));// z - pi_lo
            temp_m128.d = _mm_sub_pd(temp_m128.d, _mm_load_pd(&Atan2d_constants_d[6]));
            temp_m128.i = _mm_and_si128(temp_m128.i, mask3);
            z.i         = _mm_andnot_si128(mask3, z.i);// make the places of z to copy temp_m128i as zero
            z.i         = _mm_or_si128(z.i, temp_m128.i);
            }
        }
    return z.i;
}

ISV Atan2_64f_A53_SSE2_S(RegFile &reg)
{
    reg.dst[0].i = Atan2_64f_SSE2(reg.src1[0], reg.src2[0]);
}

///////////////////////////////////////// ATAN2 FLOAT SSE2 CODE ////////////////////////////////////////////

IS __m128i Atan2_32f_SSE2(XMM128 &y, XMM128 &x)
{
    //float z;
    XMM128 z;
    z.i = _mm_setzero_si128();
    XMM128 temp_m128;

    __m128i temp;

    //int k,m,hx,hy,ix,iy;
    __m128i k,ix,iy,signx,signy;
    __m128i mask1, mask2, mask3, mask4, mask5;

    //hx = *(int*)&x;
    //ix = hx&0x7fffffff;
    ix = _mm_slli_epi32(x.i, 1);
    ix = _mm_srli_epi32(ix, 1);
    
    //hy = *(int*)&y;
    //iy = hy&0x7fffffff;
    iy = _mm_slli_epi32(y.i, 1);
    iy = _mm_srli_epi32(iy, 1);

    //m = ((hy>>31)&1)|((hx>>30)&2); // 2*sign(x)+sign(y) 
    signx = _mm_srai_epi32(x.i, 31); // get the signs of x and y
    signy = _mm_srai_epi32(y.i, 31);

    // when y = 0 
    //if(iy<0x00800000L) {
    //    switch(m) {
    //    case 0:
    //    case 1: return y;    // atan(+-0,+anything)=+-0 
    //    case 2: return pi_f; // pi_f+tiny_f;/* atan(+0,-anything) = pi
    //    case 3: return -pi_f;//-pi_f-tiny_f;/* atan(-0,-anything) =-pi
    //    }
    //}

    //Check if y==0
    mask1 = _mm_cmplt_epi32(iy, _mm_load_si128((__m128i*)&Atan2f_constants_i[0]));
    if(_mm_movemask_epi8(mask1))
        {
            mask2 = _mm_andnot_si128(signx, mask1); // check if x is positive
            temp  = _mm_and_si128(y.i, mask2);
            z.i   = _mm_or_si128(z.i, temp);
            mask2 = _mm_and_si128(signx, mask1);
            if(_mm_movemask_epi8(mask2))
                {
                    temp = _mm_load_si128((__m128i*)&Atan2f_constants_f[0]);
                    temp = _mm_or_si128(temp, _mm_slli_epi32(signy, 31));
                    temp = _mm_and_si128(temp, mask2);
                    z.i  = _mm_or_si128(z.i, temp);
                }
        }

    // when x is INF 
    //if(ix==0x7f800000L) {
    //    if(iy==0x7f800000L) {
    //    switch(m) {
    //        case 0: return  pi_o_4_f; //pi_o_4_f+tiny_f; // atan(+INF,+INF)
    //        case 1: return -pi_o_4_f; //-pi_o_4_f-tiny_f; // atan(-INF,+INF) 
    //        case 2: return  pi3_o_4_f;//(float)3.0*pi_o_4_f+tiny_f; // atan(+INF,-INF)
    //        case 3: return -pi3_o_4_f;//(float)-3.0*pi_o_4_f-tiny_f; // atan(-INF,-INF)
    //    }
    //    } else {
    //    switch(m) {
    //        case 2: return  pi_f;//pi_f+tiny_f  ; atan(+...,-INF)
    //        case 3: return -pi_f;//-pi_f-tiny_f  ; atan(-...,-INF)
    //    }
    //    }
    //}
    temp  = _mm_load_si128((__m128i*)&Atan2f_constants_i[4]);
    mask2 = _mm_cmpeq_epi32(ix, temp); // check if x = INF
    if(_mm_movemask_epi8(mask2))
        {
            mask3 = _mm_cmpeq_epi32(iy, temp);// check if y = INF
            mask4 = _mm_and_si128(mask3, mask2);
            mask4 = _mm_and_si128(mask4, mask1);// mask4 indicates that we entered second if
            mask5 = _mm_andnot_si128(signx, mask4);// check if x is positive
            if(_mm_movemask_epi8(mask5))
                {
                    temp = _mm_load_si128((__m128i*)&Atan2f_constants_f[4]);
                    temp = _mm_or_si128(temp, _mm_slli_epi32(signy, 31));
                    temp = _mm_and_si128(temp, mask5);
                    z.i  = _mm_or_si128(z.i, temp);
                }
            mask5 = _mm_and_si128(signx, mask4);// check if x is - ive
            if(_mm_movemask_epi8(mask5))
                {
                    temp = _mm_load_si128((__m128i*)&Atan2f_constants_f[8]);
                    temp = _mm_or_si128(temp, _mm_slli_epi32(signy, 31));
                    temp = _mm_and_si128(temp, mask5);
                    z.i  = _mm_or_si128(z.i, temp);
                }
            mask3 = _mm_andnot_si128(mask3, mask2);
            mask3 = _mm_and_si128(mask3, mask1);// mask3 indicates x is INF and y!=INF
            if(_mm_movemask_epi8(mask3))
                {
                    temp = _mm_load_si128((__m128i*)&Atan2f_constants_f[0]);
                    temp = _mm_or_si128(temp, _mm_slli_epi32(signy, 31));
                    temp = _mm_and_si128(temp, mask3);
                    z.i  = _mm_or_si128(z.i, temp);
                }
        }

    // compute y/x 
    //k = (iy-ix)>>23;
    k = _mm_sub_epi32(iy, ix);
    k = _mm_srai_epi32(k, 23);
    //if(hx<0&&k<-60) z=0.0;     // |y|/x < -2**60 
    mask3 = _mm_cmplt_epi32(x.i, _mm_setzero_si128());
    mask4 = _mm_cmplt_epi32(k, _mm_load_si128((__m128i*)&Atan2f_constants_i[8]));
    mask3 = _mm_and_si128(mask3, mask4);
    if(_mm_movemask_epi8(mask3))
        {
            mask4 = _mm_andnot_si128(mask2, mask3);
            mask4 = _mm_andnot_si128(mask1, mask4);
            temp  = _mm_load_si128((__m128i*)&Atan2f_constants_f[20]);
            temp  = _mm_and_si128(temp, mask4);
            z.i   = _mm_or_si128(z.i, temp);
        }
    //else z= Atan_32f(fabsf(y/x));    // safe to do y/x 
    mask4 = _mm_andnot_si128(mask3, _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128()));
    mask3 = _mm_andnot_si128(mask2, mask4);
    mask3 = _mm_andnot_si128(mask1, mask3);
    if(_mm_movemask_epi8(mask3))
        {
            temp_m128.f = _mm_div_ps(y.f, x.f);
            temp_m128.i = _mm_slli_epi32(temp_m128.i, 1);
            temp_m128.i = _mm_srli_epi32(temp_m128.i, 1);
            Atan_DATA data;
            Atan_32f_SETUP_S(data);
            temp_m128.i = Atan_32f_SSE2(temp_m128, data);
            temp_m128.i = _mm_and_si128(temp_m128.i, mask3);
            z.i         = _mm_or_si128(z.i, temp_m128.i);
        }
    
    //switch (m) {
    //    case 0: return  z; // atan(+,+) 
    //    case 1: return -z; // atan(-,+) 

    //    case 2: return  pi_f-(z-pi_lo_f); // atan(+,-) 
    //    default: // case 3 
    //    return  (z-pi_lo_f)-pi_f; // atan(-,-) 
    //}
    mask2 = _mm_andnot_si128(mask2, _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128()));
    mask1 = _mm_andnot_si128(mask1, mask2);// mask1 indicates we are inside switch
    mask2 = _mm_andnot_si128(signx, mask1);//check x is positive and we are in last switch statement
    if(_mm_movemask_epi8(mask2))
        {
            temp = _mm_or_si128(z.i, _mm_slli_epi32(signy, 31));
            temp = _mm_and_si128(temp, mask2);
            z.i  = _mm_or_si128(z.i, temp);
        }
    mask2 = _mm_and_si128(mask1, signx);// x is negative and we are inside last switch
    if(_mm_movemask_epi8(mask2))
        {
        mask3 = _mm_andnot_si128(signy, mask2);//mask3 indicates y is positive,x is negative and we are inside switch
        if(_mm_movemask_epi8(mask3))
            {
            temp_m128.f = _mm_sub_ps(z.f, _mm_load_ps(&Atan2f_constants_f[16]));// z - pi_lo
            temp_m128.f = _mm_sub_ps(_mm_load_ps(&Atan2f_constants_f[12]), temp_m128.f);
            temp_m128.i = _mm_and_si128(temp_m128.i, mask3);
            z.i         = _mm_andnot_si128(mask3, z.i);// make the places of z to copy temp_m128i as zero
            z.i         = _mm_or_si128(z.i, temp_m128.i);
            }
        mask3 = _mm_and_si128(signy, mask2);
        if(_mm_movemask_epi8(mask3))
            {
            temp_m128.f = _mm_sub_ps(z.f, _mm_load_ps(&Atan2f_constants_f[16]));// z - pi_lo
            temp_m128.f = _mm_sub_ps(temp_m128.f, _mm_load_ps(&Atan2f_constants_f[12]));
            temp_m128.i = _mm_and_si128(temp_m128.i, mask3);
            z.i         = _mm_andnot_si128(mask3, z.i);// make the places of z to copy temp_m128i as zero
            z.i         = _mm_or_si128(z.i, temp_m128.i);
            }
        }
    return z.i;
}
ISV Atan2_32f_A24_SSE2_S(RegFile &reg)
{
    reg.dst[0].i = Atan2_32f_SSE2(reg.src1[0], reg.src2[0]);
}


} // OPT_LEVEL

#endif





