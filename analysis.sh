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

TP53_START_POS="32561406"
TP53_END_POS="32565149"
SETD2_START_POS="41675539"
SETD2_END_POS="41800697"
DMD_START_POS="26290903"
DMD_END_POS="28444635"

echo "Starting analysis..."

./bin/analysis $METADATA_FILE 5 $TP53_START_POS $TP53_END_POS $OUTPUT_FILE_PREFIX"chr5"$OUTPUT_FILE_SUFFIX
./bin/analysis $METADATA_FILE 20 $SETD2_START_POS $SETD2_END_POS $OUTPUT_FILE_PREFIX"chr20"$OUTPUT_FILE_SUFFIX
./bin/analysis $METADATA_FILE X $DMD_START_POS $DMD_END_POS $OUTPUT_FILE_PREFIX"chrX"$OUTPUT_FILE_SUFFIX

echo "Done."
