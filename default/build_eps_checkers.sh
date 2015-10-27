#!/bin/bash

# This script takes checker.double1_1.cc and checker.double1_n.cc and creates
# checker.double?_1.cc and checker.double?_n.cc files.

MAX_EPS=6

if [ ! -f checker.double1_1.cc ]; then
  echo "Run from a directory where checker.double1_1.cc exists.";
  exit 1
fi

function change_EPS_constant {
  sed --in-place "s/1e-1/1e-$2/g" $1
}

for ((eps_value=2; $eps_value <= $MAX_EPS; eps_value+=1)); do
  cp checker.double1_1.cc checker.double${eps_value}_1.cc
  cp checker.double1_n.cc checker.double${eps_value}_n.cc
  change_EPS_constant checker.double${eps_value}_1.cc $eps_value
  change_EPS_constant checker.double${eps_value}_n.cc $eps_value
done
