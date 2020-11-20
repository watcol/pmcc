#!/bin/bash
set -u

# assert <input file> <expected status code>
assert() {
  # Compile code
  ./teal < $1 > tmp.s
  cc -o tmp tmp.s

  # Set return code to $status
  ./tmp
  local status="$?"

  # Check the status
  if [ "$status" = "$2" ]; then
    echo -e "$1: \033[32mOK\033[m" 1>&2
    echo 0
  else
    echo -e "$1: \033[31mERROR\033[m (expected $2 but got $status)" 1>&2
    echo 1
  fi

  # Clean up
  rm tmp tmp.s
}

main() {
  # return code
  local ok=0

  # List tests ...
  #
  # test `assert <file> <expected>` -eq 0
  # ok=$?

  exit $ok
}

main
