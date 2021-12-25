#!/usr/bin/bash
# tools/mk_txt.sh
# - Tool to create .txt files from .c files in the /txt folder

CFILES=$(ls *.c)

for cfile in ${CFILES[*]}
do
   cp $cfile www/txt/${cfile}.txt
done

ls txt/*.txt

