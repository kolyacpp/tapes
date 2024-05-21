#!/bin/bash
set -euo pipefail
IFS=$' \t\n'

cmake-build-$1/tests/tapes/tapes_tests_correctness
cmake-build-$1/tests/tapes/tapes_tests_random
