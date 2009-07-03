#!/bin/csh 
#
# Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#
set FW_BIN_PATH=../../../Framewave/build/bin
set FW_INC_PATH=../../../Framewave/build/include/
set CONFIG="debug"
set BITNESS=32
set CLEAN=''

foreach arg ($*)
	switch ($arg)
		case "release":
			set CONFIG="release"
			breaksw
		case "debug":
			set CONFIG="debug"
			breaksw
		case "32":
			set BITNESS=32
			breaksw
		case "64":
			set BITNESS=64
			breaksw
		case "clean":
			set CLEAN=clean
			breaksw
		case "all":
			set CONFIG="debug release"
			set BITNESS="32 64"
			breaksw
	endsw
end

set PWD=`pwd`
set LIST=`find UnitTestFramework/ | grep -i 'makefile$'`
set LIST=$LIST" "`find UnitTestCollection/ | grep -i 'makefile$'`
set OSNAME=`uname -s`

if( $?FW_INC_PATH == 0 ) then 
	set FW_INC_PATH=$PWD/FWBuildBits
endif

if( $?FW_BIN_PATH == 0 ) then 
	set FW_BIN_PATH=$PWD/FWBuildBits
endif

foreach config ($CONFIG)
	foreach bitness ($BITNESS)
		echo "*************************************************"
		echo $config $bitness
		echo "*************************************************"
		set OUTDIR=$PWD/bin/$config\_$bitness/
		set TMPDIR=$PWD/tmp/$config\_$bitness/

		set FW_BIN_PATH_CONFIG=$FW_BIN_PATH/$config\_shared_$bitness\

		echo "Creating Symbolic Links"
		if( "$OSNAME" == "SunOS" || "$OSNAME" == "Linux" ) then
			foreach binary (`ls $FW_BIN_PATH_CONFIG/*.so.1.1.0`)
				ln -sf $binary `expr $binary : '\(.*so\)'`
				ln -sf $binary `expr $binary : '\(.*so.1\)'`
			end
		else if( "$OSNAME" == "Darwin" ) then
			foreach binary( `ls $FW_BIN_PATH_CONFIG/*.dylib`)
				if( "`expr $binary : '.*\(\.1.dylib\)'`" != ".1.dylib" ) then
					ln -sf $binary `expr $binary : '\(.*\).dylib'`.1.dylib 
				endif
			end
		endif
		echo -------------------------------------------------
		
		set UTFRAMEWORK_BIN_PATH=$OUTDIR

		set MAKEFILE_ARGS="OUTDIR=$OUTDIR BITNESS=$bitness CONFIG_TYPE=$config"
		set MAKEFILE_ARGS="$MAKEFILE_ARGS FW_INC_PATH=$FW_INC_PATH FW_BIN_PATH=$FW_BIN_PATH_CONFIG"
		set MAKEFILE_ARGS="$MAKEFILE_ARGS UNIT_TEST_FRAMEWORK_LIB_PATH=$UTFRAMEWORK_BIN_PATH"
		set MAKEFILE_ARGS="$MAKEFILE_ARGS $CLEAN"

		foreach make_file ($LIST)
			set PROJECT_DIR=`dirname $make_file`
			set PROJECT_NAME=`basename $PROJECT_DIR`
			set SRC_DIR="$PWD/$PROJECT_DIR"
			set TMPDIR_PROJ="$TMPDIR/$PROJECT_NAME"
			set MAKEFILE_ARGS_PROJ="TMPDIR=$TMPDIR_PROJ $MAKEFILE_ARGS"

			echo "--$PROJECT_NAME--"
			pushd $SRC_DIR								# Store the present directory
			echo make $MAKEFILE_ARGS_PROJ SRC_DIR=$SRC_DIR
			make $MAKEFILE_ARGS_PROJ SRC_DIR=$SRC_DIR	# Execute make file
			popd										# Restore directory
			echo -------------------------------------------------
		end
	end
end
