#ifndef VARIANT_HANDLER_HPP
#define VARIANT_HANDLER_HPP

#include "cigar.hpp"
#include "../src_shared/filehandler.hpp"
#include "../src_shared/variant.hpp"

#include <set>
#include <string>

class VariantHandler : public OutFileHandler
{
public:
    VariantHandler(const std::string & path);
    void call(size_t readPos, const std::string & prefix, const std::string & ref,
              const std::string & alt, const Cigar::Entries & cigarEntries);
    void forceFlush() { flush(INT_MAX); }

protected:
    virtual void write(const VariantEntry & entry);
    void flush(size_t lastPos);

private:
    void save(size_t pos, const std::string & ref, const std::string & alt);

    std::set<VariantEntry, VariantEntryComparator> m_set;
    int m_iterSinceFlush = 0;
};

#endif // VARIANT_HANDLER_HPP
