#!/bin/sh
mkdir -p meta
protoc --cpp_out=meta meta.proto
protoc --cpp_out=meta test.proto

