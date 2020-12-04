#!/bin/bash
set -u

# ./test.sh <input file> <expected status code>
main() {
  # Compile code
  ./teal < $1 > tmp.ll

  # Set return code to $status
  lli tmp.ll
  local status="$?"

  # Clean up
  rm tmp.ll

  # Check the status
  if [ "$status" = "$2" ]; then
    echo -e "$1: \033[32mOK\033[m" 1>&2
    exit 0
  else
    echo -e "$1: \033[31mERROR\033[m (expected $2 but got $status)" 1>&2
    exit 1
  fi
}

main $@
