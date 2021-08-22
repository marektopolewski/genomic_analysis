#include <fstream>
#include <vector>
#include <iostream>
#include <map>

#define BATCH_SIZE 100

struct VariantEntry
{
    int pos;
    std::string variant;
};

struct VariantEntryComparator
{
    bool operator()(const VariantEntry & lhs, const VariantEntry & rhs) const {
        if (lhs.pos == rhs.pos)
            return lhs.variant < rhs.variant;
        return lhs.pos < rhs.pos;
    }
};

int main() {

    auto chromosome = 5;
    auto filePrefix = "variant/chr" + std::to_string(chromosome);

    std::ifstream metaListFile("metadata.csv");
    if (!metaListFile.is_open()) {
        std::cerr << "Could not open file containing metadata.\n";
        return -1;
    }

    std::vector<std::ifstream> vcfFiles;
    // int delimPos;
    // std::string metaLine, fileName;
    // while(std::getline(metaListFile, metaLine)) {
    //     delimPos = metaLine.find('\t');
    //     fileName = metaLine.substr(delimPos + 1, metaLine.length() - delimPos - 4);
    //     fileName += "chr" + std::to_string(chromosome) + ".vcf";
    //     vcfFiles.emplace_back(fileName);
    // }
    metaListFile.close();

    vcfFiles.emplace_back("variant_test/test.1.vcf");
    vcfFiles.emplace_back("variant_test/test.2.vcf");
    vcfFiles.emplace_back("variant_test/test.3.vcf");

    std::ofstream outputFile("analysis_output.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Could not open file for writing output.\n";
        return -1;
    }

    std::cout << "Starting statistical analysis on chromosome " << chromosome
              << " from " << vcfFiles.size() << " file(s).\n";

    std::map<VariantEntry, int, VariantEntryComparator> variantCounts;
    int minPos;
    auto batchRead = [&vcfFiles, &variantCounts, &minPos](size_t vcfIt) {
        int it = 0, delimPos1, delimPos2;
        std::string variantStr;
        while (it < BATCH_SIZE) {

            if (!std::getline(vcfFiles[vcfIt], variantStr))
                return true;

            auto delimPos = variantStr.find('\t', 0);
            auto pos = std::stoi(variantStr.substr(0, delimPos));
            auto variant = variantStr.substr(delimPos + 1, variantStr.length() - delimPos - 1);

            if (minPos > pos)
                minPos = pos;

            ++variantCounts[{ pos, variant }];
            ++it;
        }
        return false;
    };

    auto flushWrite = [&variantCounts, &minPos, &outputFile]() {
        auto variantIt = variantCounts.begin();
        for (; variantIt != variantCounts.end(); ++variantIt) {
            if (variantIt->first.pos >= minPos)
                break;
            outputFile << variantIt->first.pos << "\t" << variantIt->first.variant
                       << "\t" << variantIt->second << "\n";
        }
        variantCounts.erase(variantCounts.begin(), variantIt);
    };

    bool outputUpdated = true;
    std::vector<bool> vcfFilesComplete(vcfFiles.size(), false);

    while(outputUpdated) {
        std::cout << "------------------ NEW ITERATION ------------------\n";
        outputUpdated = false;
        minPos = INT_MAX;

        for (int i = 0; i < vcfFiles.size(); ++i) {
            if (vcfFilesComplete[i])
                continue;
            vcfFilesComplete[i] = batchRead(i);
            outputUpdated = outputUpdated || !vcfFilesComplete[i];
        }
        flushWrite();

        std::cout << "Stats:\n";
        for (auto v : variantCounts) {
            std::cout << "\t" << v.first.pos << "\t" << v.first.variant << "\tcount="  << v.second << "\n";
        }
        std::cout << "End.\n";

    }

    // Close VCF files
    for (auto & vcfFile : vcfFiles)
        vcfFile.close();

    // Flush remaining variants
    minPos = INT_MAX;
    flushWrite();
    outputFile.close();

    std::cout << "Done.\n";

}