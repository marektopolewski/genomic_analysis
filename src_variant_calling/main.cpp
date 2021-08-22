#include "cigar.hpp"
#include "sequencehandler.hpp"
#include "referencehandler.hpp"
#include "varianthandler.hpp"

#include <iostream>

int main (int argc, char * argv[])
{
    // Parse arguments
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <SAM> <REF> <OUT>\n";
        std::cerr << "where:\n";
        std::cerr << "    <SAM> path to SAM file containing read sequences\n";
        std::cerr << "    <REF> path to the reference chromosome sequence\n";
        std::cerr << "    <OUT> path to the output file\n";
        std::cerr << "For more information contact: marek.topolewski20@imperial.ac.uk\n";
        return -1;
    }
    auto seq_path = argv[1];
    auto ref_path = argv[2];
    auto out_path = argv[3];

    // Init object for parsing the read sequences (SAM)
    SequenceHandler seqHandler(seq_path); // "SRR8691809.5.sam"
    if (!seqHandler.valid()) {
        std::cerr << "Could not open the SAM file with sequence reads.\n";
        return -1;
    }

    // Init object for parsing the reference sequence
    ReferenceHandler refHandler(ref_path); // "chr5_ref_sequence_oneline.fasta"
    if (!refHandler.valid()) {
        std::cerr << "Could not open the reference genome FASTA file.\n";
        return -1;
    }

    // Init object for variant calling
    VariantHandler variantHandler(out_path); // ""
    if (!variantHandler.valid())  {
        std::cerr << "Could not open the output file.\n";
        return -1;
    }

    // Iteratively call variants
    while (seqHandler.next()) {
        Cigar cigar{seqHandler.getCigar()};
        refHandler.seek(seqHandler.getPosition());
        auto refSeq = refHandler.getSequence();
        auto samSeq = seqHandler.getSequence();
        variantHandler.call(seqHandler.getPosition(), refSeq, samSeq, cigar.getEntries());
    }

    return 0;
}