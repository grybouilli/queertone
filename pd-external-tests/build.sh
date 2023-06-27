#!/bin/bash

PATH_TO_PD_EXTERNALS=~/Documents/Pd/externals
ext_name=$1

if [ -z $1 ]
then
    echo Please enter external name.
    exit
fi

cat Makefile > temp.out
sed -i "s/mylib/$ext_name/g" Makefile
make
cat temp.out > Makefile
rm temp.out

mkdir -p $PATH_TO_PD_EXTERNALS/$ext_name && cp $ext_name.pd_linux $PATH_TO_PD_EXTERNALS/$ext_name

rm -f *.o *.pd_linux