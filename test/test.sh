#!/bin/bash
function test {
  echo $2 | bin/goro $1 | grep -q $3;
  echo $1: $2 "->" $3
  if [ $? -ne 0 ]; then
    echo "                                                              failed" 
  else
    echo "                                                              passed"
  fi
}

cmake -DCMAKE_BUILD_TYPE:STRING=Debug .
make all \
&& test "--tictactoe" "a1\na2\nc1\nb3\nc3\n" "Nobody" \
&& test "--tictactoe" "a1\na2\nb3\n" "Second"
