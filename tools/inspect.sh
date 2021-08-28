#!/bin/sh

cd $(dirname "$(readlink -f "$0")")

ADDR=$(cat ../log.txt | grep pc | cut -d"=" -f6 | cut -d":" -f2 | cut -d" " -f1 | head -n 1)

if [ -z "$ADDR" ]; then
	echo No crash found
	exit 1
fi

echo $ADDR
r2 -s 0x$ADDR ../kernel/kernel.elf
