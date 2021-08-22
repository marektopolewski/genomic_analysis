#ifndef VARIANT_HANDLER_HPP
#define VARIANT_HANDLER_HPP

#include "cigar.hpp"
#include "filehandler.hpp"

#include <set>
#include <string>

struct VariantEntry
{
    VariantEntry(int pos, const std::string & variant) : pos(pos), variant(variant) {}
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

class VariantHandler : public OutFileHandler
{
public:
    VariantHandler(const std::string & path);
    void call(size_t readPos, const std::string & prefix, const std::string & ref,
              const std::string & alt, const Cigar::Entries & cigarEntries);
protected:
    virtual void write(const VariantEntry & entry);
    void flush(size_t lastPos);

private:
    void save(size_t pos, const std::string & ref, const std::string & alt);

    std::set<VariantEntry, VariantEntryComparator> m_set;
    int m_iterSinceFlush = 0;
};

#endif // VARIANT_HANDLER_HPP
