@echo off

REM check arguments: %1=source, %2=XSL, %3=result
if a%3 == a goto :out_trap

set prog_dir=..\
if not a%prog_dir% == a..\ goto :env_trap
if not exist %prog_dir%saxon9.jar goto :check_su
goto :do_it

:check_su
set prog_dir=..\app\
if not a%prog_dir% == a..\app\ goto :env_trap
if not exist %prog_dir%saxon9.jar goto :check_saxon
goto :do_it

:check_saxon
set prog_dir=%saxon%
if not a%prog_dir% == a%saxon% goto :env_trap
if not exist %prog_dir%saxon9.jar goto :jar_trap

:do_it
echo Processing XSLT 2.0...
java -cp "%prog_dir%;%prog_dir%saxon9.jar" net.sf.saxon.Transform -o %3 %1 %2
goto :finis

:jar_trap
if not exist ..\%prog_dir%saxon9.jar echo The "saxon9.jar" is not in "..\", "..\app" or "%prog_dir%"
goto :finis

:env_trap
echo More environment space for SET variables.
goto :finis

:out_trap
echo No output, check arguments.
goto :finis

:finis
set prog_dir=
