#!/bin/bash
#
# Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
# This software is subject to the Apache v2.0 License.
#

NUM_ARGS=$#
E_NUM_ARGS=2

PROG_NAME=$0
AR_NAME=$1
OUT_DIR=$2

UNDB="_"
DOT="."

# identify current OS and set utility paths
OSNAME=`uname -s`
if [ "$OSNAME" = "SunOS" ]; then
	AR=/opt/csw/bin/gar
	GREP=/usr/local/bin/grep
	GAWK=/opt/csw/bin/gawk
else
	AR=/usr/bin/ar
	GREP=/usr/bin/grep
	GAWK=/usr/bin/gawk
fi

Usage()
{
	echo -e "Usage: $PROG_NAME 'archive name' 'output_folder'"
	echo -e "Example: $PROG_NAME libTest.a archive_members"
}

# check input
if [ "$NUM_ARGS" != "$E_NUM_ARGS" ]; then
	Usage	
	exit 1
fi

# create output folder
if [ -d $2 ]; then
	echo -e "$OUT_DIR already exists"
else
	echo -e "$OUT_DIR does not exist, creating ..."
	mkdir -p $OUT_DIR	
fi

# write names of members to file
AR_MEMBERS_FILE=".tmp_ar_members_file"
$AR t $AR_NAME > $OUT_DIR/$AR_MEMBERS_FILE

# extract members
if [ "$OSNAME" = "Darwin" ]; then

	sed -i '' '/SYMDEF/d' $OUT_DIR/$AR_MEMBERS_FILE
	while read line
	do
		$AR -x $AR_NAME $line
		mv $line $OUT_DIR/
	done < $OUT_DIR/$AR_MEMBERS_FILE

else

	for member in `$AR t $AR_NAME`
	do
		X=`$GREP -c -x $member $OUT_DIR/$AR_MEMBERS_FILE`
		I=1
		while [ $I -le $X ]
		do
			$AR -x -N $I $AR_NAME $member
			member_bname=`echo $member | $GAWK -F. '{print $1}'`
			member_sname=`echo $member | $GAWK -F. '{print $2}'`
			mv $member $OUT_DIR/$member_bname$UNDB$I$DOT$member_sname
			I=$((I+1))
		done
	done

fi

exit


