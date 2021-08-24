#!/bin/sh

# setup downoald location
if [ -d "data" ]; then
    printf "Directory exists, are you sure you want to clear it? (y/N) "
    read ALLOW_CLEAR
    if [[ "$ALLOW_CLEAR" != "y" ]] && [[ "$ALLOW_CLEAR" != "Y" ]]; then
        echo "Exiting..."
        exit 1
    fi
    rm -rf data/*
else
    mkdir -p data
fi
cd data/
mkdir chr5 chr20 chrX
cd ../

# Regions of interest as per gene locations
TP53_START_POS="32561406"
TP53_END_POS="32565149"
SETD2_START_POS="41675539"
SETD2_END_POS="41800697"
DMD_START_POS="26290903"
DMD_END_POS="28444635"

while IFS= read -r line
do
    # get file path
    DIRECTORY=$(echo "$line" | awk '{print $1}')
    BAM_FILE=$(echo "$line" | awk '{print $2}')

    # print info message
    echo "\n--------------------------------------------------------------------------------"
    echo "Handling file $BAM_FILE"

    # download BAM file
    gsutil cp \
        "gs://gwas-os-test-3/$DIRECTORY/$BAM_FILE" \
        "data/$BAM_FILE"
    cd data/

    # convert BAM to SAM and subsample
    SAM_5_FILE="chr5/$(echo "$BAM_FILE" | sed "s/.bam/.chr5.sam/")"
    SAM_20_FILE="chr20/$(echo "$BAM_FILE" | sed "s/.bam/.chr20.sam/")"
    SAM_X_FILE="chrX/$(echo "$BAM_FILE" | sed "s/.bam/.chrX.sam/")"
    samtools index $BAM_FILE
    samtools view $BAM_FILE 5:$TP53_START_POS-$TP53_END_POS > $SAM_5_FILE
    samtools view $BAM_FILE 20:$SETD2_START_POS-$SETD2_END_POS > $SAM_20_FILE
    samtools view $BAM_FILE X:$DMD_START_POS-$DMD_END_POS > $SAM_X_FILE

    # remove original BAM file
    rm $BAM_FILE "$BAM_FILE.bai"
    cd ../

done < "metadata.csv"

