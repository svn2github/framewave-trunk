/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#define IS_SYSTEM_CPP
#include "fwBase.h"
#include "system.h"

// Initialize baseData pointer here so it'll always be initially zero
// no matter where fwBase.library is loaded from and won't be re-initialized
// when system.h is included from the various FW libraries
void * x_baseDataPtr = 0;

// Windows
#if defined( FW_WIN )

    #pragma intrinsic(__cpuid)
    #pragma intrinsic(__rdtsc)

    static inline __int64 get_rdtsc_sys()
    {
        return __rdtsc();
    }

    void *AlignedMalloc ( size_t length )
    {
        return _aligned_malloc( length, 32 );
    }
    void AlignedFree ( void *ptr )
    {
        _aligned_free( ptr );
    }

// Linux
#elif defined( FW_LIN ) || defined( FW_MAC )

    // CPUID
    #if defined( FW_SYS64 )

        #define __cpuid(CPUInfo, InfoType)    __asm__ __volatile__("    pushq %%rbx;                      \
                                                                    xorq %%rax, %%rax;                    \
                                                                    movl %%esi, %%eax;                    \
                                                                    cpuid;                                \
                                                                    movl %%eax, 0x0(%%rdi);               \
                                                                    movl %%ebx, 0x4(%%rdi);               \
                                                                    movl %%ecx, 0x8(%%rdi);               \
                                                                    movl %%edx, 0xc(%%rdi);               \
                                                                    popq %%rbx;"                          \
                                                                    : : "D" (CPUInfo), "S" (InfoType)     \
                                                                    : "%rax", "%rcx", "%rdx" )

    #elif defined( FW_SYS32 )

        #define __cpuid(CPUInfo, InfoType)    __asm__ __volatile__("    pushl %%ebx;                      \
                                                                    xorl %%eax, %%eax;                    \
                                                                    movl %%esi, %%eax;                    \
                                                                    cpuid;                                \
                                                                    movl %%eax, 0x0(%%edi);               \
                                                                    movl %%ebx, 0x4(%%edi);               \
                                                                    movl %%ecx, 0x8(%%edi);               \
                                                                    movl %%edx, 0xc(%%edi);               \
                                                                    popl %%ebx;"                          \
                                                                    : : "D" (CPUInfo), "S" (InfoType)     \
                                                                    : "%eax", "%ecx", "%edx" )

    #endif

    // RDTSC
    #define rdtsc(low,high) __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

    static inline unsigned long long get_rdtsc_sys()
    {
        unsigned long low, high;
        rdtsc(low, high);
        return ((unsigned long long)high << 32) | (unsigned long long)low;
    }

    #if defined( FW_MAC )
        void *AlignedMalloc ( size_t length )
        {
            void *memptr = malloc ( length ); // malloc in Mac OS X gives 16 byte aligned memory
            return memptr;
        }
    #else
        void *AlignedMalloc ( size_t length )
        {
            void *memptr;
            posix_memalign ( &memptr, 32, length );
            return memptr;
        }
    #endif

    void AlignedFree ( void *ptr )
    {
        free( ptr );
    }

// Solaris
#elif defined( FW_SOL )

    #if    defined( FW_SYS64 )

        static void rdtsc64(unsigned long *pLow, unsigned long *pHigh)
        {
            asm("rdtsc");                // rdtsc
            asm("movq %rax, (%rdi)");    // save rax, rdi - first argument - pLow
            asm("movq %rdx, (%rsi)");    // save rdx, rsi - second argument - pHigh
        }
        
        static void cpuid64(int *pCPUInfo, unsigned int InfoType)
        {
            asm("pushq %rbx");           // save rbx
            asm("xorq %rax, %rax");      // set rax to 0
            asm("movl %esi, %eax");      // second argument - InfoType
            asm("cpuid");                // cpuid
            asm("movl %eax,   (%rdi)");  // save eax, first argument - pCPUInfo
            asm("movl %ebx,  4(%rdi)");  // save ebx
            asm("movl %ecx,  8(%rdi)");  // save ecx
            asm("movl %edx, 12(%rdi)");  // save edx
            asm("popq %rbx");            // restore rbx
        }

        #define rdtsc rdtsc64
        #define __cpuid cpuid64

    #elif    defined( FW_SYS32 )

        static void rdtsc32(unsigned long *pLow, unsigned long *pHigh)
        {
            asm("rdtsc");                // rdtsc
            asm("movl 8(%ebp), %ecx");   // first argument - low
            asm("movl %eax, (%ecx)");    // store eax
            asm("movl 12(%ebp), %ecx");  // second argument - high
            asm("movl %edx, (%ecx)");    // store edx
        }

        static void cpuid32(int *pCPUInfo, unsigned int InfoType)
        {
            asm("pushl %ebx");           // save ebx
            asm("pushl %edi");           // save edi
            asm("xorl %eax, %eax");      // set eax to 0
            asm("movl 12(%ebp), %eax");  // second argument - InfoType
            asm("movl 8(%ebp), %edi");   // first argument - pCPUInfo
            asm("cpuid");                // cpuid
            asm("movl %eax,   (%edi)");  // store eax
            asm("movl %ebx,  4(%edi)");  // store ebx
            asm("movl %ecx,  8(%edi)");  // store ecx
            asm("movl %edx, 12(%edi)");  // store edx
            asm("popl %edi");            // restore edi
            asm("popl %ebx");            // restore ebx
        }
        
        #define rdtsc rdtsc32
        #define __cpuid cpuid32

    #endif

    static inline unsigned long long get_rdtsc_sys()
    {
        unsigned long low, high;
        rdtsc(&low, &high);
        return ((unsigned long long)high << 32) | (unsigned long long)low;
    }

    void *AlignedMalloc ( size_t length )
    {
        void *memptr = memalign ( 32, length );
        return memptr;
    }
    void AlignedFree ( void *ptr )
    {
        free( ptr );
    }

#endif

Fw64u get_rdtsc()
{
    return get_rdtsc_sys();
}

//-----------------------------------
//  Bit Manipulation
//-----------------------------------

//
// Data
//
const unsigned CPUID_CODE_FEATURES   = 1;
const unsigned CPUID_INDEX_SSE4A     = 2;
const unsigned CPUID_INDEX_SSE3      = 2;
const unsigned CPUID_INDEX_FEATURES  = 3;
const unsigned CPUID_BIT_SSE3        = 0;
const unsigned CPUID_BIT_SSE         = 25;
const unsigned CPUID_BIT_SSE2        = 26;
const unsigned CPUID_BIT_SSE4A       = 6;

//Returns mask of chosen bit. (Used for 32/64 bit type safety on the shift)
template<class T> T inline ChooseBit( unsigned bit ){ T t = 1; return (T)(t << bit);}


template<class T> inline bool IsSet   ( const T & t, unsigned bit ){ return 0 != (t &   ChooseBit<T>(bit)); }
template<class T> inline void SetBit  (       T & t, unsigned bit ){              t |=  ChooseBit<T>(bit);  }
template<class T> inline void ClearBit(       T & t, unsigned bit ){              t &= ~ChooseBit<T>(bit);  }

// Return the MSB position for the type
template< class T >    unsigned MSBPos(){ return (sizeof(T)*8) - 1; }

FwCpuType GetCpuType()
{
    int out[4];
    __cpuid( out, CPUID_CODE_FEATURES );    
    
    if( IsSet( out[CPUID_INDEX_SSE4A   ], CPUID_BIT_SSE4A)) return fwCpuFamily10h;
    if( IsSet( out[CPUID_INDEX_SSE3    ], CPUID_BIT_SSE3 )) return fwCpuSSE3;
    if( IsSet( out[CPUID_INDEX_FEATURES], CPUID_BIT_SSE2 )) return fwCpuSSE2;
    // removed check for fwCpuSSE since that code should be defaulting to reference code anyway

    return fwCpuUnknown;
}
