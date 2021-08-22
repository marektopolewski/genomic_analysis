#include "referencehandler.hpp"

ReferenceHandler::ReferenceHandler(const std::string & path)
    : InFileHandler(path)
{
    if (valid()) {
        read(SEQ_READ_SIZE);
        m_sequence = m_buffer;
    }
}

std::string ReferenceHandler::getSequence()
{
    return m_sequence;
}

void ReferenceHandler::seek(const size_t & pos)
{
    auto readStartPos = pos;
    auto readEndPos = readStartPos + SEQ_READ_SIZE - 1;

    // Case 1: Read is the same as current reference
    if (readStartPos == m_startPos && readEndPos == m_endPos)
        return;

    // Case 2: Read overlaps current reference
    else if (readStartPos > m_startPos && readEndPos > m_endPos && readStartPos < m_endPos) {
        auto trim = readStartPos - m_startPos;
        m_sequence = m_sequence.substr(trim, SEQ_READ_SIZE - trim);
        read(trim);
        m_sequence += m_buffer;
    }

    // Case 3: Read is after the reference (no overlap)
    else if (readStartPos > m_startPos && readEndPos > m_endPos) {
        auto skip = readStartPos - m_endPos - 1;
        m_file.seekg(static_cast<long>(m_file.tellg()) + skip);
        read(SEQ_READ_SIZE);
        m_sequence = m_buffer;
    }

    // Case INVALID
    else
        assert(false && "Read could not be aligned given current reference position");

    m_startPos = readStartPos;
    m_endPos = readEndPos;
}

void ReferenceHandler::read(const size_t & size)
{
    memset(m_buffer, 0, SEQ_READ_SIZE + 1);
    m_file.read(m_buffer, size);
}

