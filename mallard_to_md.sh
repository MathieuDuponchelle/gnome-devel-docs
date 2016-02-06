#!/usr/bin/env bash

rm -rf markdown_files
mkdir markdown_files

for FILE in $1/*page
do
	/home/meh/devel/pandoc/dist/build/pandoc/pandoc -s -f mallard -t markdown+yaml_metadata_block $FILE | sed '/\.\.\./q' > "markdown_files/$(basename ${FILE%.page}.yaml)"
	/home/meh/devel/pandoc/dist/build/pandoc/pandoc -s -f mallard -t commonmark $FILE -o "markdown_files/$(basename ${FILE%.page}.markdown)"
done
