#!/bin/sh

# build directory tree
sed \
	-e 's/$ //' \
	-e 's/cd \//mkdir tmp; cd tmp/' \
	-e 's/cd \([^.]\+\)/cd \1/' \
	-e 's/^\([0-9]*\) \(.*\)/truncate -s\1 \2/' \
	-e 's/^ls//' \
	-e 's/^dir/mkdir/' \
	input \
| sh

# for each directory
for dir in `find tmp -type d`; do \
	# find all files, sum them
	find $dir -type f \
		| xargs du -b \
		| cut -f1 \
		| paste -sd+ \
		| bc;
#then take each dir < 100000 and sum all those
done | grep "^[0-9]\{1,5\}$" | paste -sd+ | bc

USED=`find tmp/ -type f|xargs du -b|cut -f1|paste -sd+|bc`
NEEDED=$((USED-40000000))

for dir in `find tmp -type d`; do \
	DIRSIZE=`find $dir -type f \
		| xargs du -b \
		| cut -f1 \
		| paste -sd+ \
		| bc`
	echo $((DIRSIZE - NEEDED)) : $DIRSIZE
done | grep "^[0-9]" | sort -n | head -n1 | sed 's/.* : //'

rm -rf tmp
