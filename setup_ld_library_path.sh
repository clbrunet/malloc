#!/bin/bash

export LD_LIBRARY_PATH="$(cd -- $(dirname $0) &> /dev/null; pwd -P)${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
