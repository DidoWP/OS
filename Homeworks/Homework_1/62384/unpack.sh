#!/bin/bash

if [[ $# -ne 4 ]]; then
	exit 1
fi

if [[ ! -f $1 || -d "$2" || -d "$3" || -f "$4" ]]; then
	exit 2
fi

tempUnzip=$(mktemp -d) # temp directory for unziped moodle file
tempHomes=$(mktemp) # file to store result from find comand

mkdir $2 # directory for finished homeworks
mkdir $3 # directory for unfinished homeworks
touch $4 # result file

unzip -q -d "$tempUnzip" "$1"

find $tempUnzip -mindepth 2 -maxdepth 2  > $tempHomes 

while read line
do
	fn=$(echo $line | cut -d"/" -f 4 | cut -d"-" -f 1)
	gri=1
	grf=1
	nd=1
	grd=1
	
	correctFormat="XZ"
	allFormats="XZ;Zip;gzip;bzip2;POSIX;RAR;"
	archiveFormat=$(file -b "$line" | cut -d" " -f 1)
	archiveName=$(echo "$line" | cut -d"/" -f 5)
	
	if [[ "$archiveFormat" = "$correctFormat" ]]; then # correct archive format
		grf=0
	fi

	if [[ "$archiveName" = "$fn.tar.xz" ]]; then # correct archive name
		gri=0
	fi
	
	tempDir=$(mktemp -d)

	if [[ $(awk -v a="$archiveFormat" '$0 == a {print 1}' RS=';' <<< $allFormats) = 1 ]]; then
		
		if [[ $(tar -xf "$line" -C "$tempDir") || $(unzip "$line" -d "$tempDir") || $(unrar x "$line" "$tempDir") ]]; then
		
			if [[ $(find "$tempDir" -mindepth 1 -type d | wc -l) -eq 1 ]]; then
				nd=0
			
				if [[ $(find "$tempDir" -mindepth 1 -type d -printf '%f') = "$fn" ]]; then
					grd=0
				fi
			fi

			mkdir "$2"/"$fn"
			cp $(find "$tempDir" -mindepth 2 -maxdepth 2) "$2"/"$fn"/
			echo $fn $gri $grf $nd $grd >> "$4"
		fi
		
	else
		mkdir "$3"/"$fn"
		cp "$line" "$3"/"$fn"
	fi

	rm -r "$tempDir"

done < $tempHomes

rm -r "$tempUnzip"
rm "$tempHomes"

exit 0
