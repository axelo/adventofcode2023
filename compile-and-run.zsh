#!/bin/zsh

set -euo pipefail

DAY="$1"

if [[ -z ${2-} ]]; then
    INPUT="${1:0:-1}.input"
else
    INPUT=$2
fi

FLAGS="-Ofast \
    -Wunused-but-set-variable \
    -Wunused-parameter \
    -Wunused-macros"

if [[ -v D ]]; then
    FLAGS="-O0 \
        -Wno-unused-function \
        -Wno-unused-variable"
fi

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
    ${=FLAGS} \
    -std=c17 \
    --debug \
    -o "./build/$DAY" \
    "$DAY.c"

"./build/$DAY" < "$INPUT"
