#!/bin/bash
set -euo pipefail
IFS=$' \t\n'

mkdir -p cmake-build-$1
rm -rf cmake-build-$1/*

SLOW_TEST=OFF
if [ $1 == "Release" ]; then
  SLOW_TEST=ON
fi

cmake --preset $1 \
  -DBUILD_CLI=OFF \
  -DENABLE_SLOW_TEST=$SLOW_TEST \
  -DTREAT_WARNINGS_AS_ERRORS=ON -S .
cmake --build cmake-build-$1
