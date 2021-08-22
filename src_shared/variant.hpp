#ifndef VARIANT_HPP
#define VARIANT_HPP

#include <string>

struct VariantEntry
{
    VariantEntry(int pos, const std::string & variant);
    int pos;
    std::string variant;
};

struct VariantEntryComparator
{
    bool operator()(const VariantEntry & lhs, const VariantEntry & rhs) const;
};

#endif // VARIANT_HPP
