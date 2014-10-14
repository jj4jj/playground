#!/bin/sh
set +x
GEN_DIR=gen
#cp xls convert
cp ../tools/xlsconverter/cpp/* ${GEN_DIR}/res/
cp ../tools/xlsconverter/meta/* res/

for dir in `ls` ;do
		if [[ -d $dir ]];then
			if [[ $dir == "res" ]];then
				continue
			fi
			for proto in $(ls ${dir}/*.proto 2> /dev/null) ;do
				if [[ -f $proto ]];then
						mkdir -p ${GEN_DIR}/${dir}
						echo "generate proto buffer file [${proto}] ..."	
						protoc --cpp_out=${GEN_DIR} ${proto}
				fi
			done
		fi
done

