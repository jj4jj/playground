#!/bin/sh
set +x
GEN_DIR=gen
for dir in `ls` ;do
		if [[ -d $dir ]];then
			for proto in $(ls ${dir}/*.proto 2> /dev/null) ;do
				if [[ -f $proto ]];then
						mkdir -p ${GEN_DIR}/${dir}
						echo "generate proto buffer file [${proto}] ..."	
						protoc --cpp_out=${GEN_DIR} ${proto}
				fi
			done
		fi
done

