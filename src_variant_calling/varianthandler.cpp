#include "varianthandler.hpp"

#include "constants.hpp"

#define BATCH_SIZE 100

VariantHandler::VariantHandler(const std::string & path)
    : OutFileHandler(path)
{}

void VariantHandler::call(size_t readPos,
                          const std::string & prefix,
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
                if (ref[refPos + i] == alt[altPos + i] || ref[refPos + i] == WILDCARD_NUCLEOTIDE
                    || alt[altPos + i] == WILDCARD_NUCLEOTIDE)
                    continue;
                save(readPos + refPos + i, ref.substr(refPos + i, 1), alt.substr(altPos + i, 1));
            }
            refPos += basesLeft;
            altPos += basesLeft;
            break;
        case Cigar::Op::Insert:
            if (refPos == 0)
                save(readPos + refPos, prefix, prefix + alt.substr(altPos, basesLeft));
            else
                save(readPos + refPos, ref.substr(refPos - 1, 1), alt.substr(altPos - 1, basesLeft + 1));
            altPos += basesLeft;
            break;
        case Cigar::Op::Delete:
        if (refPos == 0)
                save(readPos + refPos, prefix + ref.substr(refPos, basesLeft), prefix);
            else
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

    // Check if should flush to disk
    if (++m_iterSinceFlush >= BATCH_SIZE)
        flush(readPos);
}

void VariantHandler::save(size_t pos, const std::string & ref, const std::string & alt)
{
    std::string variant;
    variant += ref;
    variant += ",";
    variant += alt;
    m_set.emplace(pos, std::move(variant));
}

void VariantHandler::flush(size_t lastPos)
{
    m_iterSinceFlush = 0;
    auto entryIt = m_set.begin();
    for (; entryIt != m_set.end(); ++entryIt) {
        if (entryIt->pos + SEQ_READ_SIZE >= lastPos)
            break;
        write(*entryIt);
    }
    m_set.erase(m_set.begin(), entryIt);
}

void VariantHandler::write(const VariantEntry & entry)
{
    m_file << entry.pos << "," << entry.variant << "\n";
}
