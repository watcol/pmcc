#!/bin/bash
set -u

main() {
  local dir=`pwd`
  _tester arith.c 42
  _tester logic.c 1
  _tester var.c 42
  _tester flow.c 2
  _tester fibo.c 55
}

# ./tester <tealc> <input file> <expected status code>
_tester() {
  # Set return code to $status
  ./tealc ../examples/$1
  local status="$?"

  # Check the status
  if [ "$status" = "$2" ]; then
    echo -e "$1: \033[32mOK\033[m" 1>&2
    return 0
  else
    echo -e "$1: \033[31mERROR\033[m (expected $2 but got $status)" 1>&2
    return 1
  fi
}

main
