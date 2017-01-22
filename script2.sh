#!/bin/bash
#Daniela Becerra Gonzalez
#use this script in the same level as the linux repo
#and pass linux as the first argument
#e.g. ./script2.sh linux
cd $1
# b) print number of files per directory
echo "Analyzing the linux kernel repository..."
shopt -s nullglob
for dir in ./*/;do
  files=$(find $dir -type f | wc -l)
  echo "$dir : $files files"
done
echo
echo "simple horizontal bar graph"
declare -i freq
for dir in ./*/;do
  freq=$(find $dir -type f | wc -l)
  freq=$((freq/220))
  echo $(for i in $(seq 1 $freq); do printf "#"; done)
done
echo
# c) print number of files according to their types
declare -i classified_files=0
declare -i other_files=0
types=("README" "Kconfig" "Kbuild" "Makefiles" ".c" ".h" ".pl")
patterns=("README" "Kconfig" "Kbuild" "Makefile" "\.c$" "\.h$" "\.pl$")
for i in {0..6}
do
  type_files=$(find . -type f | grep ${patterns[$i]} | wc -l)
  classified_files=$((classified_files + type_files))
  echo "Number of ${types[$i]} files = $type_files "
done
total_files=$(find . -type f |  wc -l)
other_files=$((total_files - classified_files))
echo "Number of others files = $other_files"
echo
echo "Total number of files = $total_files"
# d) how many times is the library linux/module.h
#in the .c and .h files
echo
mkdir ../C_FILES
mkdir ../H_FILES
cfiles=$(find . -type f | grep "\.c$" )
hfiles=$(find . -type f | grep "\.h$" )
declare -i includes=0
echo "Go grab a coffe, the next step will take some minutes"
for i in ${cfiles[@]}
do
  cp $i ../C_FILES/
  if grep "#include <linux/module.h>" $i > ../whatever; then
    includes=$((includes + 1))
  fi
done

for j in ${hfiles[@]}
do
  cp $j ../H_FILES/
  if grep "#include <linux/module.h>" $i >> ../whatever; then
    includes=$((includes + 1))
  fi
done
rm ../whatever
echo "The linux/module.h library is used $includes times"
more_c=$(find ../C_FILES -type f | grep "\.c$" | wc -l)
more_h=$(find ../H_FILES -type f | grep "\.h$" | wc -l)
echo "Counting c files again =  $more_c"
echo "Counting h files again = $more_h"
