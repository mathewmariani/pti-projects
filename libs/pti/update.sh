#!/bin/bash
# update the required pti headers

# repo url
PTI_PREFIX="https://raw.githubusercontent.com/mathewmariani/pti/main"

# files
PTI_H="$PTI_PREFIX/src/pti.h"
PTI_LICENSE="$PTI_PREFIX/LICENSE"

# output directory
OUTPUT=./libs/pti

# curl all headers and utils
curl $PTI_H > $OUTPUT/pti.h
curl $PTI_LICENSE > $OUTPUT/LICENSE