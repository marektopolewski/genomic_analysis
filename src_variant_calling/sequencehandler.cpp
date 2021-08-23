#include "sequencehandler.hpp"

#include "constants.hpp"

SequenceHandler::SequenceHandler(const std::string & path)
    : InFileHandler(path)
{
    reset();
}

size_t SequenceHandler::getPosition()
{
    return m_position;
}

size_t SequenceHandler::getMapQuality()
{
    return m_mapq;
}

std::string SequenceHandler::getCigar()
{
    return m_cigar;
}

std::string SequenceHandler::getSequence()
{
    return m_sequence;
}

bool SequenceHandler::next()
{
    reset();

    std::string line;
    if(!std::getline(m_file, line))
        return false;

    int column = 0, currentPos = 0;
    int entryPos = line.find('\t');
    do {
        switch (column) {
        case SAM_COLUMN_POSITION:
            m_position = std::stoi(line.substr(currentPos, entryPos - currentPos));
            break;
        case SAM_COLUMN_MAPQ:
            m_mapq = std::stoi(line.substr(currentPos, entryPos - currentPos));
            break;
        case SAM_COLUMN_CIGAR:
            m_cigar = line.substr(currentPos, entryPos - currentPos);
            break;
        case SAM_COLUMN_SEQUENCE:
            m_sequence = line.substr(currentPos, entryPos - currentPos);
            break;
        default:
            break;
        }
        currentPos = entryPos + 1;
        entryPos = line.find('\t', currentPos);
        ++column;
    } while (entryPos != -1 && column <= SAM_COLUMN_SEQUENCE);

    return m_position != -1 && !m_cigar.empty() && m_mapq != -1 && !m_sequence.empty();
}

void SequenceHandler::reset()
{
    m_position = -1;
    m_cigar = "";
    m_mapq = -1;
    m_sequence = "";
}
