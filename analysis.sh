#!/bin/sh

# compile variant calling algorithm
cd src_analysis/
./compile.sh
cd ../
if [ ! -f "bin/analysis" ]; then
    echo "Compilation failed, exiting.."
    exit 1
fi

# setup output location
if [ -d "output" ]; then
    printf "Output file exists, are you sure you want to clear it? (y/N) "
    read ALLOW_CLEAR
    if [[ "$ALLOW_CLEAR" != "y" ]] && [[ "$ALLOW_CLEAR" != "Y" ]]; then
        echo "Exiting..."
        exit 1
    fi
    rm -rf output/* > /dev/null 2>&1
else
    mkdir -p output
fi

METADATA_FILE="metadata.csv"
OUTPUT_FILE_PREFIX="output/variant_counts."
OUTPUT_FILE_SUFFIX=".csv"

echo "Starting analysis..."

./bin/analysis $METADATA_FILE 5 $OUTPUT_FILE_PREFIX"chr5"$OUTPUT_FILE_SUFFIX
./bin/analysis $METADATA_FILE 20 $OUTPUT_FILE_PREFIX"chr20"$OUTPUT_FILE_SUFFIX
./bin/analysis $METADATA_FILE X $OUTPUT_FILE_PREFIX"chrX"$OUTPUT_FILE_SUFFIX

echo "Done."
