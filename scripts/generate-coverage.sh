#!/bin/bash

pushd .
cd $(dirname $0)

declare -a FILETYPES
declare -a INCLUDE_DIRS
declare -a EXCLUDE_DIR_MASKS

COVERAGE_DIR="../coverage"
COVERAGE_FILE="_coverage.info"
FILETYPES=("c" "h" "cpp" "hpp" "gcno" "gcda")
INCLUDE_DIRS=("../src" "../test" "../build")
EXCLUDE_DIR_MASKS=("/src/msg" "/_deps")

FUNCTION_FILTER="^FN[A-Z]{0,2}:[0-9]+.*$"

COVERAGE_FILE_PATH="$COVERAGE_DIR/$COVERAGE_FILE"

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

lcov -c --directory $COVERAGE_DIR --output-file $COVERAGE_FILE_PATH
lcov --remove $COVERAGE_FILE_PATH -o $COVERAGE_FILE_PATH "*_deps*" "/usr/*" "*test/*" "*src/msg*"
#lcov --list "$COVERAGE_DIR/_coverage.info"

echo "Remove function coverage"
declare -a FILTERED_LINES
while read -r line; do
  if ! [[ $line =~ $FUNCTION_FILTER ]]; then
    #echo "$line"
    FILTERED_LINES+=($line)
  fi
done < $COVERAGE_FILE_PATH

echo "" > $COVERAGE_FILE_PATH
for line in "${FILTERED_LINES[@]}"; do
  echo "$line" >> "$COVERAGE_FILE_PATH"
done

lcov --summary "$COVERAGE_FILE_PATH"

if [[ $1 == "html" ]]; then
  genhtml "$COVERAGE_FILE_PATH" --output-directory "$COVERAGE_DIR/html"
fi

popd