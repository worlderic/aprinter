#!/usr/bin/env bash
#
# Copyright (c) 2017 Ambroz Bizjak
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

set -e

PREPROCESSED=$1
OUTPUT_HEADER=$2

(
    echo "#include <stdint.h>"

    # Transform the results into variable definitions.
    # Only lines which match the expected pattern will be kept,
    # this implicitly excludes registers which not available.
    val_pattern='\(\*\(volatile uint(8|16|32)_t \*\)\(([^;]+)\)\)'
    match_pattern='APRINTER_AVR_REG_([^:]*):'"$val_pattern"';'
    replace='static uint16_t const APrinter_AVR_\1_ADDR = \3;'
    sed -E 's/'"$match_pattern"'/'"$replace"'/g;tx;d;:x' "$PREPROCESSED"
) > "$OUTPUT_HEADER"
