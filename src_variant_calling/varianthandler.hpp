#ifndef VARIANT_HANDLER_HPP
#define VARIANT_HANDLER_HPP

#include "cigar.hpp"
#include "filehandler.hpp"

#include <unordered_set>
#include <string>

// struct Variant
// {
//     Variant(size_t pos, std::string ref, std::string alt);
//     size_t pos;
//     std::string ref;
//     std::string alt;
//     friend std::ostream & operator<<(std::ostream & os, const Variant & snp);
//     friend bool operator==(const Variant & lhs, const Variant & rhs);
// };
// struct VariantHash
// {
//     size_t operator()(const Variant & snp) const;
// };

class VariantHandler : public OutFileHandler
{
public:
    VariantHandler(const std::string & path);
    void call(size_t readPos, const std::string & prefix, const std::string & ref,
              const std::string & alt, const Cigar::Entries & cigarEntries);
    virtual void save(size_t pos, const std::string & ref, const std::string & alt);

private:
    // std::unordered_set<Variant, VariantHash> m_set;
};

#endif // VARIANT_HANDLER_HPP
