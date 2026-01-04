#!/bin/bash
# update the required stb headers

# repo url
STB_PREFIX="https://raw.githubusercontent.com/nothings/stb/master"

# headers
STB_VORBIS_C="$STB_PREFIX/stb_vorbis.c"

# license
STB_LICENSE="$STB_PREFIX/LICENSE"

# output directory
OUTPUT=./libs/stb

# curl all headers and utils
curl $STB_VORBIS_C > $OUTPUT/stb_vorbis.c

# license
curl $STB_LICENSE > $OUTPUT/LICENSE