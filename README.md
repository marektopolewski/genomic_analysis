# BAM Variant Caller and Genomic Analysis

Simple pipeline for BAM variant calling and statistical analysis.

## Pipeline

#### Preprocessing
First fetch and preprocess (split into individual chromosomes and convert to SAM) the required
BAM files from Google Cloud Storage (GCS) using the following script:
```
gcloud auth login
./preprocessing.sh
```
GsUtil is required, for installation help follow: https://cloud.google.com/storage/docs/gsutil_install
The output SAM files will be places in `data/` directory and in respective chromosome subdirectories.

Subsequently, the reference genome has to be downloaded. Desired chromosomes can be fetched separately
using this command:
```
./fetchreference.sh
```

#### Variant calling
To call variants for each sample and chromosome against the reference sequences use:
```
./callvariants
```
This will produce a series of VCF files in the `vairants/` directory.

#### Analysis
Lastly, the statistical analysis is launched using the following program:
```
./analysis
```
The resulting CSV files are present under the `output/` directory.

## Summary
To execute the entire pipeline with a single command use:
```
./preprocessing.sh && ./fetchreference.sh && ./callvariants.sh && ./analysis.sh
```

