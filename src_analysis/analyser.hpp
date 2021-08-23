#ifndef ANALYSER_HPP
#define ANALYSER_HPP

#include "../src_shared/filehandler.hpp"
#include "../src_shared/variant.hpp"

#include <fstream>
#include <vector>
#include <map>

class Analyser : public OutFileHandler
{
public:
    Analyser(const std::string & outputPath,
             const std::string & chromosome,
             size_t batchSize,
             size_t regionStart = 0,
             size_t regionEnd = INT_MAX);

    bool openVcfFiles(const std::string & metadataPath);
    void start();
    size_t sampleCount();

protected:
    virtual void write(const VariantEntry & entry, size_t count);
    std::vector<std::ifstream> m_vcfFiles;

private:
    bool batchRead(size_t vcfFileIt);
    void flushWrite();

    std::string m_chromosome;
    size_t m_batchSize;
    size_t m_regionStart;
    size_t m_regionEnd;

    std::map<VariantEntry, size_t, VariantEntryComparator> m_counts;
    size_t m_minPos;
};

#endif // ANALYSER_HPP
