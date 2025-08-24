#!/usr/bin/bash
EXEC="bat2c"
CC="gcc"
CC_OPTS="-Wall -Wextra -pedantic"

function printHelp() {
	echo "BAT2C: BASIC Auto Transpiler To C"
	echo "Usage: ./build.sh <file.bas>"
}

if [ $# -eq 0 ]; then
	printHelp
else
	if [ -f $1 ]; then
		OUT_FILE=$(basename -s .bas $1)
		$(./${EXEC} $1)
		mv out.c ${OUT_FILE}.c
		$(${CC} ${OUT_FILE}.c ${CC_OPTS} -o ${OUT_FILE})
	else
		echo "File '$1' not found"
		exit 1
	fi
fi
