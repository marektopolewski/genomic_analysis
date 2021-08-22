#!/bin/sh

# download reference data

REF_5_FILE="data/chr5/REF_CHR_5.fasta"
REF_20_FILE="data/chr20/REF_CHR_20.fasta"
REF_X_FILE="data/chrX/REF_CHR_X.fasta"

if [ ! -f "$REF_5_FILE" ]; then
    wget "https://www.ncbi.nlm.nih.gov/sviewer/viewer.cgi?tool=portal&save=file&log$=seqview&db=nuccore&report=fasta&id=355387287&" -O "$REF_5_FILE.temp"
    sed '1d' "$REF_5_FILE.temp" | tr -d '\n' > $REF_5_FILE
    rm "$REF_5_FILE.temp"
fi

if [ ! -f "$REF_20_FILE" ]; then
    wget "https://www.ncbi.nlm.nih.gov/sviewer/viewer.cgi?tool=portal&save=file&log$=seqview&db=nuccore&report=fasta&id=355387272&" -O "$REF_20_FILE.temp"
    sed '1d' "$REF_20_FILE.temp" | tr -d '\n' > $REF_20_FILE
    rm "$REF_20_FILE.temp"
fi

if [ ! -f "$REF_X_FILE" ]; then
    wget "https://www.ncbi.nlm.nih.gov/sviewer/viewer.cgi?tool=portal&save=file&log$=seqview&db=nuccore&report=fasta&id=355387253&" -O "$REF_X_FILE.temp"
    sed '1d' "$REF_X_FILE.temp" | tr -d '\n' > $REF_X_FILE
    rm "$REF_X_FILE.temp"
fi
