#include "analyser.hpp"

#include <iostream>
#include <string>

#define BATCH_SIZE 100

int main(int argc, char * argv[])
{
    // Parse arguments
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " <META> <CHR> <OUT>\n";
        std::cerr << "where:\n";
        std::cerr << "    <META> path to metadata file containing VCF file list\n";
        std::cerr << "    <CHR>  chromosome to analyse\n";
        std::cerr << "    <RBEG> initial position of region of interest\n";
        std::cerr << "    <REND> final position of region of interest\n";
        std::cerr << "    <OUT>  path to the output file\n";
        std::cerr << "For more information contact: marek.topolewski20@imperial.ac.uk\n";
        return -1;
    }
    auto metadataPath = argv[1];
    auto chromosome = argv[2];
    auto regionStartPosition = std::stoi(argv[3]);
    auto regionEndPosition = std::stoi(argv[4]);
    auto outputPath = argv[5];

    Analyser analyser(outputPath, chromosome, BATCH_SIZE, regionStartPosition, regionEndPosition);
    if (!analyser.valid()) {
        std::cerr << "Could not open the output file.\n";
        return -1;
    }

    if (!analyser.openVcfFiles(metadataPath)) {
        std::cerr << "Could not open file containing metadata or one of VCF files.\n";
        return -1;
    }

    std::cout << "Starting statistical analysis on chromosome " << chromosome
              << " from " << analyser.sampleCount() << " samples(s).\n";
    analyser.start();
    std::cout << "Done.\n";

    return 0;
}
