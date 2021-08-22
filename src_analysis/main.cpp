#include <fstream>
#include <vector>
#include <iostream>

int main() {

    auto chromosome = 5;
    auto filePrefix = "variant_test/chr" + std::to_string(chromosome);

    std::ifstream metaListFile("metadata.csv");
    if (!metaListFile.is_open()) {
        std::cerr << "Could not open file containing metadata.\n";
        return -1;
    }

    std::vector<std::ifstream> vcfFiles;
    int delimPos;
    std::string metaLine, fileName;
    while(std::getline(metaListFile, metaLine)) {
        delimPos = metaLine.find('\t');
        fileName = metaLine.substr(delimPos + 1, metaLine.length() - delimPos - 4);
        fileName += "chr" + std::to_string(chromosome) + ".vcf";
        vcfFiles.emplace_back(fileName);
    }

    std::cout << "Starting statistical analysis on chromosome " << chromosome
              << " from " << vcfFiles.size() << " file(s).\n";


    bool hasRead;
    std::string vcfLine;
    while(!vcfFiles.empty()) {

        for (auto vcfIt = vcfFiles.begin(); vcfIt != vcfFiles.end();) {

            if (!std::getline(*vcfIt, vcfLine)) {
                std::cout << "Removing\n";
                auto vcfToErase = vcfIt;
                ++vcfIt;
                assert(vcfToErase != vcfIt);
                vcfFiles.erase(vcfToErase);
                continue;
            }
            std::cout << ""
            ++vcfIt;
        }

    }

    std::cout << "Done.\n";

}