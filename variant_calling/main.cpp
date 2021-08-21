#include "cigar.hpp"
#include "sequencehandler.hpp"
#include "referencehandler.hpp"
#include "varianthandler.hpp"

#include <iostream>

int main ()
{
    SequenceHandler seqHandler("SRR8691809.5.sam");
    if (!seqHandler.valid()) {
        std::cerr << "Could not open the sequence reads SAM file.\n";
        return -1;
    }

    ReferenceHandler refHandler("chr5_ref_sequence_oneline.fasta");
    if (!refHandler.valid()) {
        std::cerr << "Could not open the reference genome FASTA file.\n";
        return -1;
    }

    VariantHandler variantHandler("SRR8691809.5.vcf");
    if (!variantHandler.valid())  {
        std::cerr << "Could not open the output file.\n";
        return -1;
    }

    while (seqHandler.next()) {
        Cigar cigar{seqHandler.getCigar()};
        refHandler.seek(seqHandler.getPosition());
        auto refSeq = refHandler.getSequence();
        auto samSeq = seqHandler.getSequence();
        variantHandler.call(seqHandler.getPosition(), refSeq, samSeq, cigar.getEntries());
    }

    return 0;
}