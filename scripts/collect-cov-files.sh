#!/bin/bash

declare -a FILETYPES
declare -a INCLUDE_DIRS
declare -a EXCLUDE_DIR_MASKS

COVERAGE_DIR="../coverage"
FILETYPES=("c" "h" "cpp" "hpp" "gcno" "gcda")
INCLUDE_DIRS=("../src" "../test" "../build")
EXCLUDE_DIR_MASKS=("/src/msg" "/_deps")

mkdir -p $COVERAGE_DIR
for dir in "${INCLUDE_DIRS[@]}"; do
  for type in "${FILETYPES[@]}"; do
    FILES=$(find $dir -type f -name "*.$type")
    for file in $FILES; do
      INCLUDE=1
      for mex in "${EXCLUDE_DIR_MASKS[@]}"; do
        if [[ $file =~ $mex ]]; then
          INCLUDE=0
          break
        fi
      done
      if [[ $INCLUDE -eq 1 ]]; then
        FILENAME=$(basename $file)
        cp $file "$COVERAGE_DIR/$FILENAME"
      fi
    done
  done
done
