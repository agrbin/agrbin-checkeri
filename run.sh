#!/bin/bash

if [ $# -ne 1 ]; then
  echo "usage: ./run.sh [task-directory-name]";
  echo
  echo "task directory name must contain files: "
  echo "checker.cc, and dirs tp/ and broken_tp/"
  echo "have environment variable VERBOSE=true to see more details."
  exit;
fi

dir=$1

[ ! -f $dir/checker.cc ] && echo "$dir/checker.cc is not a file." && exit
[ ! -d $dir/tp ] && echo "$dir/tp is not a dir." && exit
[ ! -d $dir/broken_tp ] && echo "$dir/broken_tp is not a dir." && exit

if g++ -Wall -Wextra $dir/checker.cc -ochecker; then
  echo
else
  echo Compilation failed.
  exit 1
fi

for i in $dir/tp/*in*; do
  in=$i;
  out=${i/in/out};
  ./checker $in $out $out > verdict
  if [ "$VERBOSE" = "2" ]; then
    echo $in $out $out
    cat verdict
    echo
  fi
  if [ $? -ne 0 ]; then
    echo "Checker returned non zero exit code for $in.";
    exit;
  fi
  if [ $(head -n1 verdict) != "1" ]; then
    echo "Checker thinks that official solution is not correct: ";
    echo $in $out
    cat verdict
    exit;
  fi
  echo $in $out is good.
  [ "$VERBOSE" = "1" ] && cat verdict && echo
  rm verdict
done

for i in $dir/broken_tp/*in*; do
  in=$i;
  out=${i/in/out};
  uout=${i/in/uout};
  ./checker $in $out $uout > verdict 2> /dev/null
  if [ "$VERBOSE" = "2" ]; then
    echo $in $out $uout
    cat verdict
    echo
  fi
  if [ "$(head -n1 verdict)" = "1" ]; then
    echo "Checker thinks that broken solution is correct: ";
    echo $in $out $uout
    cat verdict
    exit;
  fi
  echo $in $out $uout is broken as it should be.
  [ "$VERBOSE" = "1" ] && cat verdict && echo
  rm verdict
done

echo "All good.";
rm checker
