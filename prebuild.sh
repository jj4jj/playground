#!/bin/bash
ROOT_DIR=`pwd`
echo "current dir is $ROOT_DIR"
PROTO_DIR=`pwd`/proto
cd `pwd`/proto && sh generate.sh
cd ..