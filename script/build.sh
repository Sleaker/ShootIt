#!/bin/bash

# Build directories
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
WORKSPACE=${SCRIPT_DIR}/..
SRC=${WORKSPACE}/src
BIN=${WORKSPACE}/bin
EXTERNAL_DIR=${WORKSPACE}/external
LOG=${WORKSPACE}/build.log
DEBUG=1


# Compiler Options
COMPILER=clang-7
OPTS="-v -Wall"
[ $DEBUG -ne 0 ] && OPTS="$OPTS -g"
LINKER="-lSDL2 -lSDL2main -lSDL2_image"

INCLUDES="-I$SRC"
# Get the external header directories
echo "Checking for external headers to include"
for f in $(ls -1 ${EXTERNAL_DIR}); do
    [ -d "${EXTERNAL_DIR}/$f" ] && INCLUDES="$INCLUDES -I${EXTERNAL_DIR}/$f"
done

echo "Cleaning directories"
rm -vrf $BIN $LOG
mkdir -p $BIN

$COMPILER $OPTS $INCLUDES $LINKER -o $BIN/shootit $SRC/*.c 2>&1 | tee -a $LOG
[ ${PIPESTATUS[0]} -ne 0 ] && echo "Failed Build" | tee -a $LOG && exit 1

chmod +x $BIN/shootit
