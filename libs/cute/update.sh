#!/bin/bash
# update the required dr headers

# repo url
CUTE_PREFIX="https://raw.githubusercontent.com/mathewmariani/cute_headers/master"

# headers
CUTE_H="$CUTE_PREFIX/cute_aseprite.h"

# license
CUTE_LICENSE="$CUTE_PREFIX/LICENSE"

# output directory
OUTPUT=./libs/cute

# curl all headers and utils
curl $CUTE_H > $OUTPUT/cute_aseprite.h

# license
curl $CUTE_LICENSE > $OUTPUT/LICENSE