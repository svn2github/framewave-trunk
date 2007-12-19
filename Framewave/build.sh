#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#
#!/bin/bash

#PATH=$PATH:../BuildTools/Python25:../../BuildTools/Python25:../../../BuildTools/Python25
#export PATH

export BASE_DIR=$1
export TARGET=$2
export PLATFORM=$3
export CONFIG=$4

export BITNESS=32
export VARIANT=debug
export LIBTYPE=shared

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

echo scons $TARGET variant=$VARIANT bitness=$BITNESS libtype=$LIBTYPE -j4

if [ "$BASE_DIR" != "" ]; then
	cd $BASE_DIR
fi

mkdir -p build/include
mkdir -p build/tmp
mkdir -p build/bin

scons $TARGET variant=$VARIANT bitness=$BITNESS libtype=$LIBTYPE -j4

