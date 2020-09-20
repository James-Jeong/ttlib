#!/bin/sh

make clean && make
cd samples/mylib && make clean && make && cd ../../
cd samples/strlib && make clean && make

