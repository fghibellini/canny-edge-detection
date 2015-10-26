#!/bin/bash

if [[ $# == 2 ]]; then
    convert "$1" -compress lossless -depth 8 "$2"
else
    printf "usage:\t$0 <src> <dst>\n"
fi
