#!/bin/bash
# update the required dr headers

# repo url
DR_PREFIX="https://raw.githubusercontent.com/mackron/dr_libs/master"

# headers
DR_VORBIS_H="$DR_PREFIX/dr_wav.h"

# license
DR_LICENSE="$DR_PREFIX/LICENSE"

# output directory
OUTPUT=./libs/dr

# curl all headers and utils
curl $DR_VORBIS_H > $OUTPUT/dr_wav.h

# license
curl $DR_LICENSE > $OUTPUT/LICENSE