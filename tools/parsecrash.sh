#!/bin/sh

cd $(dirname "$(readlink -f "$0")")

ADDR=$(cat ../log.txt | grep RIP=ffffffff8 | cut -d" " -f1 | cut -d"=" -f2)

if [ -z "$ADDR" ]; then
	echo No crash found
	exit 1
fi

echo s 0x$ADDR
addr2line -e ../kernel/kernel.sym $ADDR
