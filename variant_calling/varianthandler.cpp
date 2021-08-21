#include "varianthandler.hpp"

#include "constants.hpp"

Variant::Variant(size_t pos, std::string ref, std::string alt)
    : pos(pos)
    , ref(ref)
    , alt(alt)
{}

inline std::ostream & operator<<(std::ostream & os, const Variant & snp)
{
    return os << "c." << snp.pos << " " << snp.ref << ">" << snp.alt;
}

inline bool operator==(const Variant & lhs, const Variant & rhs)
{
    return lhs.pos == rhs.pos && lhs.ref == rhs.ref && lhs.alt == rhs.alt;
}


size_t VariantHash::operator()(const Variant & snp) const
{
    return ((std::hash<size_t>()(snp.pos) ^ (std::hash<std::string>()(snp.ref) << 1)) >> 1)
        ^ (std::hash<std::string>()(snp.alt) << 1);
}


VariantHandler::VariantHandler(const std::string & path)
    : OutFileHandler(path)
{}

void VariantHandler::call(
    size_t readPos,
    const std::string & ref,
    const std::string & alt,
    const Cigar::Entries & cigarEntries)
{
    int refPos = 0, altPos = 0;
    for (const auto & cigarEntry : cigarEntries) {
        auto basesLeft = std::min(SEQ_READ_SIZE - std::max(refPos, altPos), cigarEntry.second);
        switch (cigarEntry.first) {
        case Cigar::Op::Match:
            for (int i = 0; i < basesLeft; ++i) {
                if (ref[refPos + i] == alt[altPos + i])
                    continue;
                save(readPos + refPos + i, ref.substr(refPos + i, 1), alt.substr(altPos + i, 1));
            }
            refPos += basesLeft;
            altPos += basesLeft;
            break;
        case Cigar::Op::Insert:
            save(readPos + refPos, ref.substr(refPos - 1, 1), alt.substr(altPos - 1, basesLeft + 1));
            altPos += basesLeft;
            break;
        case Cigar::Op::Delete:
            save(readPos + refPos, ref.substr(refPos - 1, basesLeft + 1), alt.substr(altPos - 1, 1));
            refPos += basesLeft;
            break;
        case Cigar::Op::SoftClip:
            altPos += basesLeft;
            break;
        case Cigar::Op::HardClip:
            break;

        default:
            assert(false && "Unhandled CIGAR operation");
            break;
        }
    }
}

void VariantHandler::save(size_t pos, const std::string & ref, const std::string & alt)
{
    // m_set.emplace(pos, ref, alt);
    m_file << pos << '\t' << ref << '\t' << alt << '\n';
}
