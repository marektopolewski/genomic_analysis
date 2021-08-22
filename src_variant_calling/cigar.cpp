#include "cigar.hpp"

Cigar::Cigar(const std::string & cigarStr)
{
    int bases = 0;
    for (const char & c : cigarStr) {
        if (c >= 'A' && c <= 'Z') {
            m_entries.emplace_back(charToOp(c), bases);
            bases = 0;
        }
        else if (c >= '0' && c <= '9') {
            bases = bases * 10 + (c - '0');
        }
    }
}

const Cigar::Entries & Cigar::getEntries()
{
    return m_entries;
}

char Cigar::opToChar(const Cigar::Op & op)
{
    if (op == Cigar::Op::Match)
        return 'M';
    else if (op == Cigar::Op::Insert)
        return 'I';
    else if (op == Cigar::Op::Delete)
        return 'D';
    else if (op == Cigar::Op::SoftClip)
        return 'S';
    else if (op == Cigar::Op::HardClip)
        return 'H';
    return '?';
}

Cigar::Op Cigar::charToOp(const char & c)
{
    if (c == 'M')
        return Cigar::Op::Match;
    else if (c == 'I')
        return Cigar::Op::Insert;
    else if (c == 'D')
        return Cigar::Op::Delete;
    else if (c == 'S')
        return Cigar::Op::SoftClip;
    else if (c == 'H')
        return Cigar::Op::HardClip;
    return Cigar::Op::Invalid;
}
