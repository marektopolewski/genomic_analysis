#include "variant.hpp"

VariantEntry::VariantEntry(int pos, const std::string & variant)
    : pos(pos)
    , variant(variant)
{}

bool VariantEntryComparator::operator()(const VariantEntry & lhs, const VariantEntry & rhs) const {
    if (lhs.pos == rhs.pos)
        return lhs.variant < rhs.variant;
    return lhs.pos < rhs.pos;
}
