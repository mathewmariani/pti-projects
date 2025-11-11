#!/bin/bash
# update the required pti headers

# repo url
BRANCH="debug-platform"  # specify your branch here
PTI_PREFIX="https://raw.githubusercontent.com/mathewmariani/pti/$BRANCH"

# files
PTI_H="$PTI_PREFIX/src/pti.h"
PTI_LICENSE="$PTI_PREFIX/LICENSE"

# platforms
PLATFORM_GL_CPP="$PTI_PREFIX/tests/platform_gl.cpp"
PLATFORM_GL_C="$PTI_PREFIX/tests/platform_gl.c"
PLATFORM_SOKOL_C="$PTI_PREFIX/tests/platform_sokol.c"

# output directory
OUTPUT=./libs/pti

# curl all headers and utils
curl $PTI_H > $OUTPUT/pti.h
curl $PTI_LICENSE > $OUTPUT/LICENSE

curl $PLATFORM_GL_CPP > $OUTPUT/platform_gl.cpp
curl $PLATFORM_GL_C > $OUTPUT/platform_gl.c
curl $PLATFORM_SOKOL_C > $OUTPUT/platform_sokol.c