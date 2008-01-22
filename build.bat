::
:: Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
:: This software is subject to the Apache v2.0 License.
::


set BASE_DIR=%1
set TARGET=%2
set PLATFORM=%3
set CONFIG=%4

set BITNESS=32
set VARIANT=debug
set LIBTYPE=shared
set PASS=TRUE


if "%PLATFORM%" NEQ "x64" GOTO CONFIG
set BITNESS=64
call "%VS80COMNTOOLS%..\..\VC\vcvarsall.bat" amd64

:CONFIG
if "%CONFIG%" == "debug_static" set LIBTYPE=static
if "%CONFIG%" == "release_static" set VARIANT=release
if "%CONFIG%" == "release_static" set LIBTYPE=static
if "%CONFIG%" == "release_shared" set VARIANT=release
if "%CONFIG%" == "release_exe" set VARIANT=release

echo BaseDir[%BASE_DIR%] Target[%TARGET%] Platform[%PLATFORM%] Variant[%VARIANT%]


if "%SERVER%"=="TRUE" goto SERVER_ALL
if "%BASE_DIR%"=="ALL" goto CLIENT_ALL
goto CLIENT_ONE


:CLIENT_ONE
::--------------------------------
SET SCONS=SCONS_CLIENT

SET RETURN=END
cd %BASE_DIR%
GOTO RUN
::--------------------------------


:CLIENT_ALL
::--------------------------------
SET SCONS=SCONS_CLIENT

SET RETURN=CLIENT_ALL_1
cd Framewave
GOTO RUN
:CLIENT_ALL_1
cd ..


SET RETURN=END
cd Test
GOTO RUN
::--------------------------------


:SERVER_ALL
::--------------------------------
SET SCONS=SCONS_SERVER

SET RETURN=SERVER_ALL_1
cd Framewave
GOTO RUN
:SERVER_ALL_1
cd ..

SET RETURN=SERVER_ALL_2
cd UnitTest
GOTO RUN
:SERVER_ALL_2
cd ..

echo PASS: %PASS% 		>> %LOG%
if "%PASS%"=="FALSE" exit
echo del ..\..\%CLIENT_ERROR%	>> %LOG%
del ..\%CLIENT_ERROR%		>> %LOG%
exit
::--------------------------------


:RUN
::--------------------------------
mkdir build\include
mkdir build\tmp
mkdir build\bin

goto %SCONS%
::--------------------------------


:SCONS_SERVER
::--------------------------------
echo scons %TARGET% variant=%VARIANT% bitness=%BITNESS% libtype=%LIBTYPE% -j8 >> %LOG%
call scons %TARGET% variant=%VARIANT% bitness=%BITNESS% libtype=%LIBTYPE% -j8 >> %LOG%

echo errorlevel: %errorlevel% 	>> %LOG%
if errorlevel 1 set PASS=FALSE
GOTO %RETURN%
::--------------------------------


:SCONS_CLIENT
::--------------------------------
echo start /b /BelowNormal scons %TARGET% variant=%VARIANT% bitness=%BITNESS% libtype=%LIBTYPE% -j4 
     start /b /BelowNormal scons %TARGET% variant=%VARIANT% bitness=%BITNESS% libtype=%LIBTYPE% -j4 
GOTO %RETURN%
::--------------------------------


:END




