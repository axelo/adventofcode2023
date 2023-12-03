#!/bin/zsh

set -euo pipefail

DAY="$1"
INPUT="$2"

mkdir -p build

set -x

clang \
    -fsanitize=undefined,integer,nullability \
    -ferror-limit=4 \
    -Werror \
    -Wall \
    -Wpedantic \
    -Wconversion \
    -Wsign-compare \
    -Wswitch-enum \
    -Wno-gnu-binary-literal \
    -Wunused-variable \
    -Wunused-function \
    -Wunused-but-set-variable \
    -Wunused-parameter \
    -std=c17 \
    --debug \
    -o "./build/$DAY" \
    "$DAY.c"

"./build/$DAY" < "$INPUT"
