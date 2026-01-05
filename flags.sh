#!/usr/bin/env bash

readonly none=0x00
readonly solid=0x01
readonly semi=0x02

readonly spikeT=0x03
readonly spikeR=0x04
readonly spikeB=0x05
readonly spikeL=0x06

readonly slopeRB=0x07
readonly slopeRT=0x08
readonly slopeLT=0x09
readonly slopeLB=0x0A

FLAGS=(
  $none $solid $solid $solid $solid $solid $solid $solid $solid $solid
  $solid $solid $solid $solid $solid $solid $solid $solid $solid $solid
  $solid $solid $solid $solid $none $none $none $semi $semi $semi
  $semi $slopeRB $slopeRT $slopeLT $slopeLB $none $none $none $semi $semi
  $semi $semi $spikeT $spikeR $solid $solid $none $none $none $none
  $none $none $none $spikeB $spikeL $none $none $none $none $none
  $none
)

: > flags.bin
for b in "${FLAGS[@]}"; do
  printf -v esc '\\x%02x' "$((b))"
  printf '%b' "$esc"
done >> flags.bin