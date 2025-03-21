#!/bin/bash
shopt -s expand_aliases
alias gcc="gcc-14"
mkdir bin
gcc src/brain.c -o bin/brain -std=c23
