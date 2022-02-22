#!/bin/bash

DIR=$(cd -- $(dirname $0) &> /dev/null; pwd -P)

export LD_LIBRARY_PATH="${DIR}${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export LD_PRELOAD="${DIR}/libft_malloc.so"

$@
