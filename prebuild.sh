#!/bin/sh
set +x
ROOT_DIR=`pwd`
echo "current dir is $ROOT_DIR"
PROTO_DIR=${ROOT_DIR}/proto
echo ${PROTO_DIR}
cd ${PROTO_DIR} && sh generate.sh
