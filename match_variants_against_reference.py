import pandas as pd

CHROMOSOMES = ["5", "20", "X"]

# Read the reference variants (selected chromosomes only)
# Source: https://www.nature.com/articles/s42003-019-0487-2#MOESM10
print("Loading reference variants... ", end="")
df_ref = pd.read_csv("reference_variants.csv", sep=";", header=None,
                     names=["chr", "pos", "ref", "alt", "gene"])
print("done.")

# Read called variants files
df_chrs = dict()
for chr in CHROMOSOMES:
    print(f"Loading chromosome {chr} variants... ", end="")
    df_chrs[chr] = pd.read_csv(f"output/variant_counts.chr{chr}.csv",
                               sep=",", header=None, usecols=[0,1,2],
                               names=["pos", "ref", "alt"], index_col="pos")
    print("done.")
print()

# Count reference (major) and alternative (minor) allele mismatches
ref_mismatches = 0
alt_mismatches = 0
missing = 0
for _, row in df_ref.iterrows():
    chr, pos, ref, alt, gene = row
    if chr not in CHROMOSOMES:
        continue

    # Find corresponding record in respective variant dataframe
    if pos in df_chrs[chr].index:
        vrow = df_chrs[chr].loc[pos]

        # If only one variant found
        if isinstance(vrow.ref, str):
            if vrow.ref is not ref:
                print(f"ERR expected ref {ref} but got {vrow.ref}. " +
                      f"Chr {chr} at position {pos}")
                ref_mismatches += 1
            elif vrow.alt is not alt:
                print(f"ERR expected alt {alt} but got {vrow.alt}. " +
                      f"Chr {chr} at position {pos}")
                alt_mismatches += 1

        # If mutiple variants found
        else:
            ref_matched = False
            for vref in vrow.ref:
                if ref is vref:
                    ref_matched = True
                    break
            if not ref_matched:
                print(f"ERR expected ref {ref} chr {chr} at position {pos}, " +
                      f"none matches:\n{vrow.ref}")
                ref_mismatches += 1
            else:
                alt_matched = False
                for valt in vrow.alt:
                    if alt is valt:
                        alt_matched = True
                        break
                if not alt_matched:
                    print(f"ERR expected alt {alt} chr {chr} at position {pos}, " +
                          f"none matches:\n{vrow.alt}")
                    alt_mismatches += 1
    else:
        print(f"ERR record {chr, pos, ref, alt, gene} not found")
        missing += 1


# Print results
size = len(df_ref)
mismatches = ref_mismatches + alt_mismatches + missing
print()
print(f"Refs mismatched {ref_mismatches}")
print(f"Alts mismatched {alt_mismatches}")
print(f"Records missing {missing}")
print(f"Total matched {size - mismatches} out of {size}, " +
      f"percentage: { (size - mismatches) / size * 100 }")

# Refs matched 180 out of 184, percentage: 97.83
# Alts matched 167 out of 184, percentage: 90.76
# Total matched 163 out of 184, percentage: 88.59
