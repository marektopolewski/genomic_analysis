#!/bin/sh

# setup downoald location
# if [ -d "data" ]; then
#     printf "Directory exists, are you sure you want to clear it? (y/N) "
#     read ALLOW_CLEAR
#     if [[ "$ALLOW_CLEAR" != "y" ]] && [[ "$ALLOW_CLEAR" != "Y" ]]; then
#         echo "Exiting..."
#         exit 1
#     fi
#     rm -rf data/*
# else
#     mkdir -p data
# fi
# cd data/
# mkdir chr5 chr20 chrX
# cd ../

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
    # curl -X GET \
    #     -H "Authorization: Bearer $OAUTH2_TOKEN" \
    #     -o "data/$BAM_FILE" \
    #     "https://storage.googleapis.com/storage/v1/b/$BUCKET_NAME/o/$DIRECTORY%2F$BAM_FILE?alt=media"
    cd data/

    # convert BAM to SAM and subsample
    SAM_5_FILE="chr5/$(echo "$BAM_FILE" | sed "s/.bam/.chr5.sam/")"
    SAM_20_FILE="chr20/$(echo "$BAM_FILE" | sed "s/.bam/.chr20.sam/")"
    SAM_X_FILE="chrX/$(echo "$BAM_FILE" | sed "s/.bam/.chrX.sam/")"
    samtools index $BAM_FILE
    samtools view $BAM_FILE 5 > $SAM_5_FILE
    samtools view $BAM_FILE 20 > $SAM_20_FILE
    samtools view $BAM_FILE X > $SAM_X_FILE

    # remove original BAM file
    rm $BAM_FILE "$BAM_file.bai"
    cd ../

done < "metadata.csv"

