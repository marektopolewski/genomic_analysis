#!/bin/sh

print_help () {
    echo "Usage: callvariants.h <META> <SEQLEN>"
    echo "  <META>    path to metadata file with list of samples"
    echo "  <SEQLEN>  integer representing length of a single SAM read"
    echo "For more information contact: marek.topoleski20@imperial.ac.uk"
}

# Parse metadata argument
if [[ -z "$1" ]] || [[ ! -f $1 ]]; then
    echo "No metadata list file supplied"
    print_help
    exit 1
fi
METADATA_CSV=$1

# Parse sequence length argument
if [[ -z "$2" ]] || [[ $2 -le 0 ]]; then
    echo "Invalid sequence length provided"
    print_help
    exit 1
fi
SEQ_LEN=$2

# compile variant calling algorithm
cd src_variant_calling/
./compile.sh
cd ../
if [ ! -f "bin/variantcall" ]; then
    echo "Compilation failed, exiting.."
    exit 1
fi

# setup output location
if [ -d "variants" ]; then
    printf "Directory exists, are you sure you want to clear it? (y/N) "
    read ALLOW_CLEAR
    if [[ "$ALLOW_CLEAR" != "y" ]] && [[ "$ALLOW_CLEAR" != "Y" ]]; then
        echo "Exiting..."
        exit 1
    fi
    rm -rf variants/*
else
    mkdir -p variants
fi
cd variants/
mkdir chr5 chr20 chrX
cd ../

# setup reference data paths
REF_5_FILE="data/chr5/REF_CHR_5.fasta"
REF_20_FILE="data/chr20/REF_CHR_20.fasta"
REF_X_FILE="data/chrX/REF_CHR_X.fasta"

while IFS= read -r line
do
    # get original file name
    BAM_FILE=$(echo "$line" | awk '{print $2}')

    # print info message
    echo "Calling file $BAM_FILE"

    # call chromosome 5
    printf "    chromosome 5  ... "
    SAM_5_FILE="data/chr5/$(echo "$BAM_FILE" | sed "s/.bam/.chr5.sam/")"
    VCF_5_FILE="variants/chr5/$(echo "$BAM_FILE" | sed "s/.bam/.chr5.vcf/")"
    ./bin/variantcall $SAM_5_FILE $REF_5_FILE $SEQ_LEN $VCF_5_FILE
    echo "done."

    # call chromosome 20
    printf "    chromosome 20 ... "
    SAM_20_FILE="data/chr20/$(echo "$BAM_FILE" | sed "s/.bam/.chr20.sam/")"
    VCF_20_FILE="variants/chr20/$(echo "$BAM_FILE" | sed "s/.bam/.chr20.vcf/")"
    ./bin/variantcall $SAM_20_FILE $REF_20_FILE $SEQ_LEN $VCF_20_FILE
    echo "done."

    # call chromosome X
    printf "    chromosome X  ... "
    SAM_X_FILE="data/chrX/$(echo "$BAM_FILE" | sed "s/.bam/.chrX.sam/")"
    VCF_X_FILE="variants/chrX/$(echo "$BAM_FILE" | sed "s/.bam/.chrX.vcf/")"
    ./bin/variantcall $SAM_X_FILE $REF_X_FILE $SEQ_LEN $VCF_X_FILE
    echo "done."

done < $METADATA_CSV

