#!/usr/bin/bash
# tools/mk_html.sh
# - Tool to convert .md files to .html c files in the /www folder

MDFILES=$(ls *.md)

for mdfile in ${MDFILES[*]}
do
  FNAME=$(echo $mdfile | sed 's/.md$//')
  Markdown.pl $mdfile | sed 's/\.md/\.html/' > www/${FNAME}.html
done

ls www/*.html

