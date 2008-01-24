/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "FwImage/fwImage.h"
#include "fwdev.h"
#include "FwSharedCode_SSE2.h"
#include "../SDK/md5.h"

#pragma comment( lib, "../SDK/cryptdll.lib" )

// MINMAX - sets the min and max to val if needed
#define MINMAX(minval,val,maxval) if (val < minval) minval = val; if (val > maxval) maxval = val;

namespace OPT_LEVEL
{

enum IMAGE_TYPE
{
    FW_UNKNOWN,
    FW_RGB,
    FW_YUV,
    FW_YCbCr,
    FW_BGRC,
    FW_XYZ,
    FW_LUV,
    FW_Lab,
    FW_YCC,
    FW_HLS,
    FW_HSV,
    FW_RGB565,
    FW_RGB555,
    FW_RGB444,
    FW_BGR565,
    FW_BGR555,
    FW_BGR444,
    FW_RGB565Dither,
    FW_RGB555Dither,
    FW_RGB444Dither,
    FW_BGR565Dither,
    FW_BGR555Dither,
    FW_BGR444Dither,
    FW_YUV422,
    FW_YUV420,
    FW_YCbCr422,
    FW_YCbCr420,
    FW_CbYCr422,
    FW_CbYCr422Gamma
};

std::string GetMD5( void * data, U32 size )
{
    MD5_CTX context;

    MD5Init  ( &context );
    MD5Update( &context, (unsigned char*) data, size );
    MD5Final ( &context );

    std::ostringstream oss;

    int * tmp = (int *) context.digest;

    for( U32 i=0; i<4; ++i )
        oss << std::hex << tmp[i];

    return oss.str();
}


// FwImage

const U32 VERSION = 36; // Note: If you change this, you must also change in ImageViewer/imgViewer.php

struct ImageFile
{
    struct FileData
    {
        const U32 headerSize;
        const U32 version;
        FWType   dataType;
        IMAGE_TYPE format;
        U32 width, height, channels;
        U32 size;
        bool planar;

        FileData():version(VERSION), headerSize (sizeof(FileData)), planar (false){}
        void operator=(FileData&){}
    } data;

    BYTE *imageData;
    std::string md5; 

    const std::string & MD5(){ return md5; }
private:

    void InitMD5()
    {
        MD5_CTX context;

        MD5Init( &context );
        MD5Update( &context, imageData, data.size );
        MD5Final( &context );

        std::ostringstream oss;

        int * tmp = (int *) context.digest;

        for( U32 i=0; i<4; ++i )
            oss << std::hex << tmp[i];

        md5 = oss.str();
    }        

    std::string str()
    {
        std::ostringstream oss;

        oss << "Type:"   << DEV::Name( data.dataType ) << std::endl;
        oss << "Width:"  << data.width    << std::endl;
        oss << "Height:" << data.height   << std::endl;
        oss << "Width:"  << data.channels << std::endl;

        return oss.str();
    }

    void C1ToC3()
    {
        data.channels = 3;
        data.size*= 3;
        BYTE *mem = (BYTE*)malloc( data.size );
        BYTE *dst = mem;
        BYTE *src = imageData;
        U32 dataSize = Size( data.dataType );
        
        for( U32 i=0; i<data.width*data.height; ++i )
        {
            memcpy (dst, src, dataSize);
            memcpy (dst+dataSize  , src, dataSize);
            memcpy (dst+2*dataSize, src, dataSize);
          /*dst[0] = src[0];
            dst[1] = src[0];
            dst[2] = src[0];*/

            dst+ =3*dataSize;
            src+ =  dataSize;
        }

        free( imageData );
        imageData = mem;
    }

    void C2ToC3()
    {
        data.channels = 3;
        data.size*= 3;
        BYTE *mem = (BYTE*)malloc( data.size );
        BYTE *dst = mem;
        BYTE *src = imageData;
        U32 dataSize = Size( data.dataType );
        
        for( U32 i=0; i<data.width*data.height; ++i )
        {
            memcpy (dst, src, dataSize);
            memcpy (dst+dataSize, src+dataSize, dataSize);
            memcpy (dst+2*dataSize, src, dataSize);

            dst+ =3*dataSize;
            src+ =2*dataSize;
        }

        free( imageData );
        imageData = mem;
    }

    void C4ToC3()
    {
        data.channels = 3;
        data.size = data.size / 4 * 3;
        BYTE *mem = (BYTE*)malloc( data.size );
        BYTE *dst = mem;
        BYTE *src = imageData;
        U32 dataSize = Size( data.dataType );

        for( U32 i=0; i<data.width*data.height; ++i )
        {
            memcpy (dst, src, dataSize*3);

          /*dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];*/

            dst + = 3*dataSize;
            src + = 4*dataSize;
        }

        free( imageData );
        imageData = mem;
    }

    //Now, only suppor C1 and C3
    void BMPToImage (BYTE *src)
    {
        for (S32 y = data.height-1; y >= 0; y--)
        {
            BYTE * dst = imageData + data.width*y*data.channels;

            if(data.channels == 3)
            {
                for(U32 x = 0; x < data.width; x++, dst += 3, src += 3 )
                {
                    dst[0] = src[2];
                    dst[1] = src[1];
                    dst[2] = src[0];
                }
            }

            if(data.channels == 1)
            {
                for(U32 x = 0; x < data.width; x++, dst++, src++)
                {
                    dst[0] = src[0];
                }
            }
        }
    }

    void GeometricMean (BYTE *dst, BYTE *src)
    {
        for (unsigned int y = 0; y < data.height; y++)
            for (unsigned int x = 0; x < data.width; x++)
            {
                // Geometric mean between pairs of values
                dst[y * (data.width * 3) + x * 3 + 0] = DEV::Geomean<A8U, A16U> ((Fw16u)src[y * (data.width * 3 * 2) + x * 3 * 2 + 0], (Fw16u)src[y * (data.width * 3 * 2) + x * 3 * 2 + 1]);
                dst[y * (data.width * 3) + x * 3 + 1] = DEV::Geomean<A8U, A16U> ((Fw16u)src[y * (data.width * 3 * 2) + x * 3 * 2 + 2], (Fw16u)src[y * (data.width * 3 * 2) + x * 3 * 2 + 3]);
                dst[y * (data.width * 3) + x * 3 + 2] = DEV::Geomean<A8U, A16U> ((Fw16u)src[y * (data.width * 3 * 2) + x * 3 * 2 + 4], (Fw16u)src[y * (data.width * 3 * 2) + x * 3 * 2 + 5]);
            }
    }

    void Flip8uImage (BYTE *dst, BYTE *Source)
    {
        for (S32 y = data.height-1; y >= 0; y--)
        {
            BYTE * src = Source + data.width*y*data.channels;
            for(U32 x = 0; x < data.width; x++, dst += data.channels, src += data.channels )
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
            }
        }
    }

    void ImageToBMP (BYTE *dst)
    {
        // Assumes Image is already 3-channel now
        FwiSize tempRoi = {data.width, data.height};
        BYTE *tempDst, *tempDst2;
        float min, max;
        switch (data.dataType)
        {
        case FW_NONE:
            return;

        case FW_8U:

        case FW_8S:
            Flip8uImage (dst, imageData);
            return;

        case FW_16U:
            tempDst = new BYTE[data.width*data.height*data.channels];
            fwiScale_16u8u_C3R ( (const Fw16u *)imageData, data.width*2*data.channels, tempDst, data.width*data.channels, tempRoi, fwAlgHintNone );
            Flip8uImage (dst, tempDst);
            delete []tempDst;
            return;

        case FW_16S:
            tempDst = new BYTE[data.width*data.height*data.channels];
            fwiScale_16s8u_C3R ( (const Fw16s *)imageData, data.width*2*data.channels, tempDst, data.width*data.channels, tempRoi, fwAlgHintNone );
            Flip8uImage (dst, tempDst);
            delete []tempDst;
            return;

        case FW_16SC:
            tempDst  = new BYTE[data.width*data.height*data.channels*2];
            tempDst2 = new BYTE[data.width*data.height*data.channels];
            tempRoi.width *= 2;
            fwiScale_16s8u_C3R ( (const Fw16s *)imageData, data.width*4*data.channels, tempDst, data.width*2*data.channels, tempRoi, fwAlgHintNone );
            GeometricMean (tempDst2, tempDst);
            Flip8uImage (dst, tempDst2);
            delete []tempDst;
            delete []tempDst2;
            return;

        case FW_32U:
            tempDst = new BYTE[data.width*data.height*data.channels];
            for (unsigned int y = 0; y < data.height; y++)
                for (unsigned int x = 0; x < data.width; x++)
                {
                    tempDst[y * (data.width * 3) + x * 3 + 0] = FW_REF::Normalize<U8,U8>::Unscale ( FW_REF::Normalize<U32,U32>::Scale ( ((Fw32u *)imageData)[y * (data.width * 3) + x * 3 + 0] ));
                    tempDst[y * (data.width * 3) + x * 3 + 1] = FW_REF::Normalize<U8,U8>::Unscale ( FW_REF::Normalize<U32,U32>::Scale ( ((Fw32u *)imageData)[y * (data.width * 3) + x * 3 + 1] ));
                    tempDst[y * (data.width * 3) + x * 3 + 2] = FW_REF::Normalize<U8,U8>::Unscale ( FW_REF::Normalize<U32,U32>::Scale ( ((Fw32u *)imageData)[y * (data.width * 3) + x * 3 + 2] ));
                }
            Flip8uImage (dst, tempDst);
            delete []tempDst;
            return;

        case FW_32S:
            tempDst = new BYTE[data.width*data.height*data.channels];
            fwiScale_32s8u_C3R ( (const Fw32s *)imageData, data.width*4*data.channels, tempDst, data.width*data.channels, tempRoi, fwAlgHintNone );
            Flip8uImage (dst, tempDst);
            delete []tempDst;
            return;

        case FW_32SC:
            tempDst  = new BYTE[data.width*data.height*data.channels*2];
            tempDst2 = new BYTE[data.width*data.height*data.channels];
            tempRoi.width *= 2;
            fwiScale_32s8u_C3R ( (const Fw32s *)imageData, data.width*8*data.channels, tempDst, data.width*2*data.channels, tempRoi, fwAlgHintNone );
            GeometricMean (tempDst2, tempDst);
            Flip8uImage (dst, tempDst2);
            delete []tempDst;
            delete []tempDst2;
            return;

        case FW_32F:
            min = max = ((const Fw32f *)imageData)[0];
            for (unsigned int y = 0; y < data.height; y++)
                for (unsigned int x = 0; x < data.width; x++)
                {
                    float temp = ((const Fw32f *)imageData)[y * (data.width * 3) + x * 3 + 0];
                        MINMAX(min,temp,max)
                          temp = ((const Fw32f *)imageData)[y * (data.width * 3) + x * 3 + 1];
                        MINMAX(min,temp,max)
                          temp = ((const Fw32f *)imageData)[y * (data.width * 3) + x * 3 + 2];
                        MINMAX(min,temp,max)
                }
            tempDst = new BYTE[data.width*data.height*data.channels];
            fwiScale_32f8u_C3R ( (const Fw32f *)imageData, data.width*4*data.channels, tempDst, data.width*data.channels, tempRoi, min, max );
            Flip8uImage (dst, tempDst);
            delete []tempDst;
            return;

        case FW_32FC:
            min = max = ((const Fw32f *)imageData)[0];
            for (unsigned int y = 0; y < data.height; y++)
                for (unsigned int x = 0; x < data.width*2; x++)
                {
                    float temp = ((const Fw32f *)imageData)[y * (data.width * 3) + x * 3 + 0];
                        MINMAX(min,temp,max)
                          temp = ((const Fw32f *)imageData)[y * (data.width * 3) + x * 3 + 1];
                        MINMAX(min,temp,max)
                          temp = ((const Fw32f *)imageData)[y * (data.width * 3) + x * 3 + 2];
                        MINMAX(min,temp,max)
                }
            tempDst  = new BYTE[data.width*data.height*data.channels*2];
            tempDst2 = new BYTE[data.width*data.height*data.channels];
            tempRoi.width *= 2;
            fwiScale_32f8u_C3R ( (const Fw32f *)imageData, data.width*8*data.channels, tempDst, data.width*2*data.channels, tempRoi, min, max );
            GeometricMean (tempDst2, tempDst);
            Flip8uImage (dst, tempDst2);
            delete []tempDst;
            delete []tempDst2;
            return;

        case FW_64F:
            /*min = max = ((const Fw32f *)imageData)[0];
            for (unsigned int y = 0; y < data.height; y++)
                for (unsigned int x = 0; x < data.width; x++)
                {
                    float temp = ((const Fw32f *)imageData)[y * (data.width * 3) + x * 3 + 0];
                        MINMAX(min,temp,max)
                          temp = ((const Fw32f *)imageData)[y * (data.width * 3) + x * 3 + 1];
                        MINMAX(min,temp,max)
                          temp = ((const Fw32f *)imageData)[y * (data.width * 3) + x * 3 + 2];
                        MINMAX(min,temp,max)
                }
            tempDst = new BYTE[data.width*data.height*data.channels];
            for (unsigned int y = 0; y < data.height; y++)
                for (unsigned int x = 0; x < data.width; x++)
                {
                    tempDst[y * (data.width * 3) + x * 3 + 0] = ((const Fw64f *)imageData)[y * (data.width * 3) + x * 3 + 0];
                        MINMAX(min,temp,max)
                    temp = ((const Fw64f *)imageData)[y * (data.width * 3) + x * 3 + 1];
                        MINMAX(min,temp,max)
                    temp = ((const Fw64f *)imageData)[y * (data.width * 3) + x * 3 + 2];
                        MINMAX(min,temp,max)
                }
            Flip8uImage (dst, tempDst);
            delete []tempDst;*/
            return;

        case FW_64FC:
            return;
        }

        Flip8uImage (dst, imageData);
    }

    bool EndsWith( const std::string & haystack, const std::string & needle )
    {
        return haystack.rfind( needle ) == haystack.length() - needle.length();
    }

    void Load( const std::string & file )
    {
        FILE * stream;
        if( fopen_s( &stream, file.c_str(), "rb" ) )
        {
            std::cout << "Could not open for READ: " << file << std::endl;
            exit(-1);
        }

        if( EndsWith(file,".bmp") )
        {
            BITMAPFILEHEADER bmpFileHead;
            BITMAPINFOHEADER bmpInfoHead;

            fread( &bmpFileHead, 1, sizeof(BITMAPFILEHEADER), stream );
            fread( &bmpInfoHead, 1, sizeof(BITMAPINFOHEADER), stream );

            data.dataType   = FW_8U;
            data.width  = bmpInfoHead.biWidth;
            data.height = bmpInfoHead.biHeight;
            data.size   = bmpFileHead.bfSize - bmpFileHead.bfOffBits;
            //data.channels = 3;

            if( 0 != data.width % 4 )
            {
                std::cout << "ImageFile requires files with width divisible by 4" << std::endl;
                exit( -1 );
            }

            imageData = (BYTE*) malloc( data.size );

            fseek( stream, bmpFileHead.bfOffBits, 0 );
            fread( imageData, 1, data.size, stream );
            
          //if( bmpInfoHead.biBitCount == 8 )
          //    C1ToC3();
            if( bmpInfoHead.biBitCount == 8 )
                data.channels = 1;
            if( bmpInfoHead.biBitCount == 16)
                data.channels = 2;
            if( bmpInfoHead.biBitCount == 24)
                data.channels = 3;
            if( bmpInfoHead.biBitCount == 32)
                data.channels = 4;

            BYTE *tempBMP = new BYTE[data.size];
          //memcpy (tempBMP, imageData, data.width*data.height*3);
            memcpy (tempBMP, imageData, data.width*data.height*data.channels);
            BMPToImage (tempBMP);
            delete []tempBMP;
        }
        else if( EndsWith(file,".fw")  )
        {
            fread( &data, 1, sizeof(FileData), stream );

            if( data.version != VERSION )
            {
                std::cout << "Invalid file version. Expected:" << VERSION << " Found:" << data.version << std::endl;
                exit(-1);
            }

            imageData = (BYTE*) malloc( data.size );
            fread( imageData, 1, data.size, stream );        
        }
        else 
        {
            std::cout << "Unknown file type" << std::endl;
            exit(-1);
        }

        fclose( stream );
    }

public:

    U32 GetSize()
    {
        return data.size;
    }

    /*void WriteMD5( const std::string & file )
    {
        std::ofstream f( file.c_str() );
        f << MD5();
    }*/

    void Write( const std::string & file )
    {
        FILE * stream;        
        if( fopen_s( &stream, file.c_str(), "wb" ) )
        {
            std::cout << "Could not open for WRITE: " << file << ":" << WinErr() << std::endl;
            exit(-1);
        }

        if( EndsWith(file,".fw")  )
        {
            fwrite( &data,     1, sizeof(FileData), stream );    // dump the header.
            fwrite( imageData, 1, data.size,        stream );    // dump the image.            
        }
        else if( EndsWith(file,".bmp")  )
        {
            switch( data.channels )
            {
            case 1: C1ToC3(); break;
            case 2: C2ToC3(); break;
            case 4: C4ToC3(); break;
            }        

            BITMAPFILEHEADER bmpFileHead;
            BITMAPINFOHEADER bmpInfoHead;        

            U32 headerSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

            memset( &bmpFileHead, 0, sizeof(BITMAPFILEHEADER) );
            memset( &bmpInfoHead, 0, sizeof(BITMAPINFOHEADER) );        

            bmpFileHead.bfType     = 19778;                                 //"BM"
            bmpFileHead.bfSize     = headerSize + data.height*data.width*3; //data.size;            
            bmpFileHead.bfOffBits  = headerSize;

            bmpInfoHead.biSize     = sizeof(BITMAPINFOHEADER);
            bmpInfoHead.biWidth    = data.width;
            bmpInfoHead.biHeight   = data.height;
            bmpInfoHead.biPlanes   = 1;
            bmpInfoHead.biBitCount = 24;                

            fwrite( &bmpFileHead, 1, sizeof(BITMAPFILEHEADER), stream );
            fwrite( &bmpInfoHead, 1, sizeof(BITMAPINFOHEADER), stream );
            BYTE *tempBMP = new BYTE[data.height*data.width*3];
            ImageToBMP ( tempBMP );
            fwrite( tempBMP, 1, data.height*data.width*3/*data.size*/, stream );
            delete []tempBMP;
        }
        else
        {
            std::cout << "Unknown file type" << std::endl;
            exit(-1);
        }

        fclose( stream );
    }

    void operator=( const ImageFile &){}


    ImageFile( const std::string & file )    
    {
        Load( file );    
        InitMD5();
    }

    ImageFile( FWType dataType_, U32 width_, U32 height_, U32 channels_, 
               IMAGE_TYPE format_=FW_RGB, bool planar_=false )
    {
        data.dataType  = dataType_;
        data.format    = format_  ;
        data.width     = width_   ;
        data.height    = height_  ;
        data.channels  = channels_;
        data.planar    = planar_  ;

        data.size = Size( data.dataType ) * data.width * data.height * data.channels;
        imageData = (BYTE*)malloc( data.size );
        InitMD5();
    }

    ImageFile( FWType dataType_, U32 width_, U32 height_, U32 channels_, const void * mem, 
               IMAGE_TYPE format_=FW_RGB, bool planar_=false )
    {
        data.dataType  = dataType_;
        data.format    = format_  ;
        data.width     = width_   ;
        data.height    = height_  ;
        data.channels  = channels_;
        data.planar    = planar_  ;

        data.size = Size( data.dataType ) * data.width * data.height * data.channels;
        imageData = (BYTE*)malloc( data.size );
        memcpy( imageData, mem, data.size );    
        InitMD5();
    }

    void Subtract( const ImageFile & a )    // bitmap file
    {
        for( U32 i=0; i<data.size; ++i )
            imageData[ i ] = (BYTE) abs( imageData[ i ] - a.imageData[i]);

    }

    //ImageFile( const ImageFile & a, const ImageFile & b )    // Create an image from the diff.     TODO: various typed diffs
    //{    
    //    data.dataType  = a.data.dataType;
    //    data.format    = a.data.format;
    //    data.width     = a.data.width;
    //    data.height    = a.data.height;
    //    data.channels  = a.data.channels;
    //    data.planar    = a.data.planar;
    //    data.size      = a.data.size;

    //    if( a.data.dataType != b.data.dataType ){ std::cout << "Datatypes differ"; exit(-1); }
    //    if( a.data.size     != b.data.size     ){ std::cout << "Sizes differ";     exit(-1); }

    //    imageData = (BYTE*)malloc( data.size );        

    //    for( U32 i=0; i<data.size; ++i )
    //        imageData[ i ] =  (BYTE) abs( a.imageData[i] - b.imageData[i]);
    //}

    ~ImageFile()
    {
        free( imageData );
    }
};

//
// Acessors
//

SYS_INLINE
void Delete( const std::string & file )
{
    ::DeleteFile( file.c_str() );
}

//#include <shellapi.h>
//SYS_INLINE
//void RemoveDir( std::string dir )
//{
//    //std::string param = "/C rmdir /s /q " + dir; 
//    std::string param = "/C del " + dir + "\\*.png"; 
//    ::ShellExecute( NULL, "open", "cmd.exe", param.c_str(), "C:\\WINDOWS\\system32", NULL );
//
//    ::RemoveDirectory( dir.c_str() );
//}

void DelFile( std::string file ){ ::DeleteFile( file.c_str()); }
void DelDir ( std::string dir  )
{ 
    ::RemoveDirectory( dir.c_str() );
  //::RemoveDir ( dir.c_str());  
} 

SYS_INLINE
void UpdateImage(    bool writeOnPass, bool pass, std::string dir,     
                     const std::string &fnName, U32 test, const std::string & pathStr,
                     const std::string &dll1, const void * mem1, 
                     const std::string &dll2, const void * mem2,
                     FWType type,
                     U32 width, 
                     U32 height, 
                     U32 channels )
{    
    if( dir.empty() )
        return;

    ImageFile a( type, width, height, channels, mem1 );
    ImageFile b( type, width, height, channels, mem2 );        

    dir += "\\" + fnName;
    std::ostringstream file1; file1 << dir << "\\" << dll1 << "(" << test << ")_" << pathStr;
    std::ostringstream file2; file2 << dir << "\\" << dll2 << "(" << test << ")_" << pathStr;
    std::ostringstream diff ; diff  << dir << "\\diff(" << test << ")_" << pathStr << ".bmp";    

    if( pass && !writeOnPass )
    {
        DelFile( file1.str() + ".fw" );    DelFile( file2.str() + ".fw" );
        DelFile( file1.str() + ".bmp" );    DelFile( file2.str() + ".bmp" );    
        DelFile( diff.str() );

        DelDir( dir );    // Remove only if empty
        return;
    }

    /*BOOL success = */CreateDirectory (dir.c_str(), 0);

    /*if( 183 != GetLastError() )
        EXIT_IF( !success, "Cannot Create Directory: " << dir << " " << WinErr ())    */


    a.Write( file1.str() + ".fw" ); a.Write( file1.str() + ".bmp" );
    b.Write( file2.str() + ".fw" ); b.Write( file2.str() + ".bmp" );    

    ImageFile c( file1.str() + ".bmp" );
    ImageFile d( file2.str() + ".bmp" );

    c.Subtract( d );        
    c.Write(  diff.str() );     
}

SYS_INLINE
void UpdateImage(    bool writeOnPass, bool pass, std::string dir,     
                     const std::string &fnName, U32 test, const std::string & pathStr,
                     const std::string &dll1, const void * mem1,
                     FWType type,
                     U32 width, 
                     U32 height, 
                     U32 channels )
{
    if( dir.empty() )
        return;

    ImageFile a( type, width, height, channels, mem1 );    

    dir += "\\" + fnName;
    std::ostringstream file1; file1 << dir << "\\" << dll1 << "(" << test << ")_" << pathStr;    

    if( pass && !writeOnPass )
    {
        DelFile( file1.str() + ".fw" );
        DelFile( file1.str() + ".bmp" );        

        DelDir( dir );    // Remove only if empty
        return;
    }

    BOOL success = CreateDirectory (dir.c_str(), 0);

    if( 183 != GetLastError() )
        EXIT_IF( !success, "Cannot Create Directory: " << dir << " " << WinErr ());

    a.Write( file1.str() + ".fw" ); 
    a.Write( file1.str() + ".bmp" );    
}


} // namespace OPT_LEVEL

