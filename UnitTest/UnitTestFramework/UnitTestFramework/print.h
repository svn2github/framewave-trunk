/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __PRINT_H__
#define __PRINT_H__

#include <emmintrin.h>
#include <iomanip>
#include <assert.h>

#pragma warning( disable: 4482 )

enum M128i_BitFormat
{
    BF_8 = 1,
    BF_16,
    BF_32,
    BF_64,
};

enum M128i_DataTypeFormat
{
    DT_UNSIGNED = 1,
    DT_SIGNED,
    DT_HEX
};

//------------------------------------------
// Overloaded output stream operator 
// and print format for __m128, __m128i, 
// and __m128d types.
//------------------------------------------

// Register Custom Format Flag
long& xmm_format_flag( std::ios_base& s ) 
{
    static int format_flag = std::ios_base::xalloc();
    return s.iword(format_flag);
}

// GET custom format flags
long get_xmm_format_flag( std::ios_base& s ) 
{
  return xmm_format_flag(s);
}

// SET custom format flags
void set_xmm_format_flag( std::ios_base& s, M128i_BitFormat bit, M128i_DataTypeFormat dt ) 
{
    long n = ((int)dt << 16) + (int)bit;
    xmm_format_flag(s) = n;
}

// RESET custom format flags
std::ostream& ResetFlag( std::ostream& os ) 
{
  set_xmm_format_flag( os, (M128i_BitFormat)0, (M128i_DataTypeFormat)0 );
  return os;
}

// Print Manipulators
#define PRINT_MANIPULATOR( manipName, bit, dt )     \
    std::ostream& manipName( std::ostream& os )   \
    {                                               \
        set_xmm_format_flag( os, bit, dt );         \
        return os;                                  \
    }

PRINT_MANIPULATOR( Format8x, M128i_BitFormat::BF_8,   M128i_DataTypeFormat::DT_HEX )
PRINT_MANIPULATOR( Format8u, M128i_BitFormat::BF_8,   M128i_DataTypeFormat::DT_UNSIGNED )
PRINT_MANIPULATOR( Format8s, M128i_BitFormat::BF_8,   M128i_DataTypeFormat::DT_SIGNED )
PRINT_MANIPULATOR( Format16x, M128i_BitFormat::BF_16, M128i_DataTypeFormat::DT_HEX )
PRINT_MANIPULATOR( Format16u, M128i_BitFormat::BF_16, M128i_DataTypeFormat::DT_UNSIGNED )
PRINT_MANIPULATOR( Format16s, M128i_BitFormat::BF_16, M128i_DataTypeFormat::DT_SIGNED )
PRINT_MANIPULATOR( Format32x, M128i_BitFormat::BF_32, M128i_DataTypeFormat::DT_HEX )
PRINT_MANIPULATOR( Format32u, M128i_BitFormat::BF_32, M128i_DataTypeFormat::DT_UNSIGNED )
PRINT_MANIPULATOR( Format32s, M128i_BitFormat::BF_32, M128i_DataTypeFormat::DT_SIGNED )
PRINT_MANIPULATOR( Format64x, M128i_BitFormat::BF_64, M128i_DataTypeFormat::DT_HEX )
PRINT_MANIPULATOR( Format64u, M128i_BitFormat::BF_64, M128i_DataTypeFormat::DT_UNSIGNED )
PRINT_MANIPULATOR( Format64s, M128i_BitFormat::BF_64, M128i_DataTypeFormat::DT_SIGNED )

std::string ConvertByteToHex( const unsigned char &val )
{
    std::string hexValue;
    const char hexValues[16] = {'0','1','2','3','4','5','6','7',
                                '8','9','a','b','c','d','e','f' };
    hexValue = "0x";
    hexValue += hexValues[val>>4];
    hexValue += hexValues[val&0x0f];
    return hexValue;
}


// Overloaded output stream operators for __mm128 type
std::ostream& operator << ( std::ostream& strm, const __m128 &var )
{
    const float *ptr = (const float *)&var + sizeof(__m128d)/sizeof(float)-1;
    strm << "{" << *(ptr--);
    for(int i=1; i<4; --ptr, ++i)
        strm << ',' << *ptr;
    strm << "}";

    return strm;
}

// Overloaded output stream operators for __m128d type
std::ostream& operator << ( std::ostream& strm, const __m128d &var )
{
    const long double *ptr = (const long double *)&var;
    strm << "{" << *(ptr+1) << ',' << *ptr << "}";
    return strm;
}

// Overloaded output stream operators for __mm128i type
std::ostream& operator << ( std::ostream& strm, const __m128i &var )
{
    long flag = get_xmm_format_flag(strm);
    int bit = (int)(flag & 0xFF);
    int dt  = (int)(flag >> 16 );

    std::ios::fmtflags oldFlags = strm.flags();

    strm << '{';
    switch( bit )
    {
        case M128i_BitFormat::BF_8:
        {
            const char *ptr = (const char*)&var + sizeof(__m128i)/sizeof(char) - 1;
            switch( dt )
            {
                case M128i_DataTypeFormat::DT_UNSIGNED:
                        strm << (unsigned int)(*(ptr--));
                        for(int i=1; i<16; --ptr, ++i)
                            strm << ',' << (unsigned int)(*ptr);
                        break;    
                case M128i_DataTypeFormat::DT_HEX:
                        strm << ConvertByteToHex(*(ptr--));
                        for(int i=1; i<16; --ptr, ++i)
                            strm << ',' << ConvertByteToHex(*ptr);
                        break;
                case M128i_DataTypeFormat::DT_SIGNED:
                    strm << (int)(*(ptr--));
                    for(int i=1; i<16; --ptr, ++i)
                        strm << ',' << (int)(*ptr);
                    break;
                default:
                    assert(false);
            }
        } break;
        case M128i_BitFormat::BF_16:
        {   
            const short *ptr = (const short*)&var + sizeof(__m128i)/sizeof(short) - 1;
            switch( dt )
            {
                case M128i_DataTypeFormat::DT_HEX:
                    strm.flags(std::ios::hex | std::ios::showbase );
                case M128i_DataTypeFormat::DT_SIGNED:
                    strm << *(ptr--);
                    for(int i=1; i<8; --ptr, ++i)
                        strm << ',' << (*ptr);
                    break;
                case M128i_DataTypeFormat::DT_UNSIGNED:
                    strm << (unsigned short)(*(ptr--));
                    for(int i=1; i<8; --ptr, ++i)
                        strm << ',' << (unsigned short)(*ptr);
                    break;
                
                default:
                    assert(false);
            }
        } break;
        case M128i_BitFormat::BF_32:
        {    
            const int *ptr = (const int*)&var + sizeof(__m128i)/sizeof(int) - 1;
            switch( dt )
            {
                case M128i_DataTypeFormat::DT_HEX:
                    strm.flags(std::ios::hex | std::ios::showbase );
                case M128i_DataTypeFormat::DT_SIGNED:
                    strm << *(ptr--);
                    for(int i=1; i<4; --ptr, ++i)
                        strm << ',' << (*ptr);
                    break;
                case M128i_DataTypeFormat::DT_UNSIGNED:
                    strm << (unsigned int)(*(ptr--));
                    for(int i=1; i<4; --ptr, ++i)
                        strm << ',' << (unsigned int)(*ptr);
                    break;
                default:
                    assert(false);
            }
        } break;
        case M128i_BitFormat::BF_64:
        {   
            const long long *ptr = (const long long*)&var;
            switch( dt )
            {
                case M128i_DataTypeFormat::DT_UNSIGNED:
                    strm << (unsigned long long)(*(ptr+1)) << ',' << (unsigned long long)(*ptr);
                    break;    
                case M128i_DataTypeFormat::DT_HEX:
                    strm.flags(std::ios::hex | std::ios::showbase );
                case M128i_DataTypeFormat::DT_SIGNED:
                    strm << *(ptr+1) << ',' << (*ptr);
                    break;
                default:
                    assert(false);
            }
        } break;
        default:
        {
            const long long *ptr = (const long long*)&var;
            strm.flags(std::ios::hex );
            strm << "0x" << *(ptr+1) << *(ptr);
        }
    }
    strm << '}';

    strm.flags( oldFlags );
    return strm;
}


#pragma warning( default: 4482 )

#endif // __PRINT_H__
