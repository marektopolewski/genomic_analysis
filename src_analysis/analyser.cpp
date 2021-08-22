#include "analyser.hpp"

Analyser::Analyser(const std::string & outputPath,
                   const std::string & chromosome,
                   size_t batchSize,
                   double significanceRatio)
    : OutFileHandler(outputPath)
    , m_chromosome(chromosome)
    , m_batchSize(batchSize)
    , m_significanceRatio(significanceRatio)
{}

bool Analyser::openVcfFiles(const std::string & metadataPath)
{
    std::ifstream metaListFile("metadata.csv");
    if (!metaListFile.is_open())
        return false;

    int delimPos;
    std::string metaLine, fileName;
    while (std::getline(metaListFile, metaLine)) {
        delimPos = metaLine.find('\t');
        fileName = metaLine.substr(delimPos + 1, metaLine.length() - delimPos - 4);
        fileName += "chr" + m_chromosome + ".vcf";
        m_vcfFiles.emplace_back("variants/chr" + m_chromosome + "/" + fileName);
        if (!m_vcfFiles.back().is_open())
            return false;
    }
    metaListFile.close();
    return true;
}

void Analyser::start()
{
    bool outputUpdated = true;
    std::vector<bool> vcfFilesComplete(m_vcfFiles.size(), false);

    // Read files sequentially in batches until no lines remain
    while(outputUpdated) {
        outputUpdated = false;
        m_minPos = INT_MAX;
        for (int i = 0; i < m_vcfFiles.size(); ++i) {
            if (vcfFilesComplete[i])
                continue;
            vcfFilesComplete[i] = batchRead(i);
            outputUpdated = outputUpdated || !vcfFilesComplete[i];
        }
        flushWrite();
    }

    // Close VCF files
    for (auto & vcfFile : m_vcfFiles)
        vcfFile.close();

    // Flush remaining variants
    m_minPos = INT_MAX;
    flushWrite();
}

size_t Analyser::sampleCount()
{
    return m_vcfFiles.size();
}

void Analyser::write(const VariantEntry & entry, size_t count)
{
    m_file << entry.pos << "\t" << entry.variant << "\t" << count << "\n";
}


bool Analyser::batchRead(size_t vcfFileIt)
{
    int it = 0, delimPos1, delimPos2;
    std::string variantStr;

    // Read a batch of lines and iteratively update variant count
    while (it < m_batchSize) {
        if (!std::getline(m_vcfFiles[vcfFileIt], variantStr))
            return true;

        // Parse string and update minimal position currently in memory
        auto delimPos = variantStr.find('\t', 0);
        auto pos = std::stoi(variantStr.substr(0, delimPos));
        auto variant = variantStr.substr(delimPos + 1, variantStr.length() - delimPos - 1);
        if (m_minPos > pos)
            m_minPos = pos;

        // Record variant
        ++m_counts[{ pos, variant }];
        ++it;
    }
    return false;
}

void Analyser::flushWrite()
{
    auto it = m_counts.begin();
    for (; it != m_counts.end(); ++it) {
        // Stop if reached a position that may overlap
        if (it->first.pos >= m_minPos)
            break;

        // Ignore if insufficiently frequent variant
        if (it->second < m_vcfFiles.size() * m_significanceRatio)
            continue;

        // Write to disk
        write(it->first, it->second);
    }

    m_counts.erase(m_counts.begin(), it);
};
