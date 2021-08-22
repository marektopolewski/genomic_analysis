#ifndef CIGAR_HPP
#define CIGAR_HPP

#include <string>
#include <vector>

class Cigar
{
public:
    enum class Op {
        Invalid = -1,
        Match,
        Insert,
        Delete,
        SoftClip,
        HardClip
    };

    Cigar(const std::string & cigarStr);

    using Entries = std::vector<std::pair<Op, int>>;
    const Entries & getEntries();

    static char opToChar(const Op & op);
    static Op charToOp(const char & c);

private:
    Entries m_entries;
    bool m_hasVariant;
};

#endif // CIGAR_HPP
