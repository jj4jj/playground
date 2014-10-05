#!/bin/sh
mkdir -p meta
protoc --cpp_out=meta meta.proto
