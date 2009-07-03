#!/bin/bash
#
# Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

#PATH=$PATH:../BuildTools/Python25:../../BuildTools/Python25:../../../BuildTools/Python25
#export PATH

export BASE_DIR=$1
export TARGET=$2
export PLATFORM=$3
export CONFIG=$4
LOG_FILE=$5

export BITNESS=32
export VARIANT=debug
export LIBTYPE=shared

export OSNAME=`uname -s`

if [ "$OSNAME" = "SunOS" ]; then
	export REBUILD_N=10
else
	export REBUILD_N=1
fi

if [ "$PLATFORM" = "x64" ]; then
	export BITNESS=64
fi

case $CONFIG in
debug_static)
	export LIBTYPE=static
;;
release_shared)
	export VARIANT=release	
	export LIBTYPE=shared
;;
release_static)
	export VARIANT=release	
	export LIBTYPE=static
;;
*)
;;
esac

echo -e "BASE_DIR is $BASE_DIR\n" >> $LOG_FILE
if [ "$BASE_DIR" != "" ]; then
	for dir in Framewave
	do
		echo -e "Building $dir.\n" >> $LOG_FILE
		cd $BASE_DIR/$dir
	
		mkdir -p build/include
		mkdir -p build/tmp
		mkdir -p build/bin
	
		for ((i=1; i<=REBUILD_N; i++))
		do	
			echo "scons $TARGET variant=$VARIANT bitness=$BITNESS libtype=$LIBTYPE -j4" >> $LOG_FILE
			scons $TARGET variant=$VARIANT bitness=$BITNESS libtype=$LIBTYPE -j4 >> $LOG_FILE 2>&1
			last_error=$?
			if [ "$last_error" = "0" ]; then
				break
			fi	

			not_enough_space_error=`tail -25 $LOG_FILE | grep -c 'Not enough space'`
			ran_out_of_memory_error=`tail -25 $LOG_FILE | grep -c 'Ran out of memory'`	
			if [ "$not_enough_space_error" = "0" ] && [ "$ran_out_of_memory_error" = "0" ]; then
				break
			fi	
		done
		
		if [ $last_error != "0" ]; then
			echo -e "$dir build failed.\n" >> $LOG_FILE
			exit 1
		fi
	done
else
	echo -e "Invalid BASE_DIR\n" >> $LOG_FILE
	exit 1
fi

