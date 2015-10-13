#!/bin/bash

if [ $# -ne 1 ]; then
  echo "usage: ./run.sh [task-directory-name]";
  echo
  echo "task directory name must contain files: "
  echo "checker.cc, and dirs tp/ and broken_tp/"
  exit;
fi

dir=$1

[ ! -f $dir/checker.cc ] && echo "$dir/checker.cc is not a file." && exit
[ ! -d $dir/tp ] && echo "$dir/tp is not a dir." && exit
[ ! -d $dir/broken_tp ] && echo "$dir/broken_tp is not a dir." && exit

g++ -Wall -Wextra $dir/checker.cc -ochecker

for i in $dir/tp/*in*; do
  in=$i;
  out=${i/in/out};
  ./checker $in $out $out > verdict
  if [ $? -ne 0 ]; then
    echo "Checker returned non zero exit code.";
    exit;
  fi
  if [ $(head -n1 verdict) -ne 1 ]; then
    echo "Checker thinks that official solution is not correct: ";
    echo $in $out
    cat verdict
    exit;
  fi
  echo $in $out is good.
done

for i in $dir/broken_tp/*in*; do
  in=$i;
  out=${i/in/out};
  uout=${i/in/uout};
  ./checker $in $out $uout > verdict
  if [ $? -ne 0 ]; then
    echo "Checker returned non zero exit code.";
    exit;
  fi
  if [ $(head -n1 verdict) -eq 1 ]; then
    echo "Checker thinks that broken solution is correct: ";
    echo $in $out $uout
    cat verdict
    exit;
  fi
  echo $in $out $uout is broken as it should be.
done

echo "All good.";
rm checker
rm verdict
