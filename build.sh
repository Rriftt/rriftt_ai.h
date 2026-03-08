#!/bin/bash

CC=gcc
CFLAGS="-std=c23 -Wall -Wextra -Wpedantic"
LDFLAGS="-lm"
TEST_SRCS=tests/*.c
EXAMPLE_SRCS=examples/*.c

echo "================================"
echo "  Building and Executing Tests  " 
echo "================================"
for TEST_SRC in $TEST_SRCS; do
	TEST_EXE=${TEST_SRC/.c/}
	echo "Building  $TEST_EXE ..."
	$CC $CFLAGS $LDFLAGS -o $TEST_EXE $TEST_SRC
	echo "Executing $TEST_EXE ..."
	./$TEST_EXE > /dev/null
	if [ $? -ne 0 ]; then
		echo "$TEST_EXE failed with exit code $?"
		rm $TEST_EXE
		exit
	fi
	rm $TEST_EXE
done

echo ""

echo "================================"
echo "  Building Examples             "
echo "================================"
for EXAMPLE_SRC in $EXAMPLE_SRCS; do
	EXAMPLE_EXE=${EXAMPLE_SRC/.c/}
	echo "Building $EXAMPLE_EXE ..."
	$CC $CFLAGS $LDFLAGS -o $EXAMPLE_EXE $EXAMPLE_SRC
	rm $EXAMPLE_EXE
done

