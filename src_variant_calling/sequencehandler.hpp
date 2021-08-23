#ifndef SEQUENCE_HANDLER_HPP
#define SEQUENCE_HANDLER_HPP

#include "../src_shared/filehandler.hpp"

#include <fstream>
#include <string>

class SequenceHandler : public InFileHandler
{
public:
    SequenceHandler(const std::string & path);

    size_t getPosition();
    size_t getMapQuality();
    std::string getCigar();
    std::string getSequence();

    bool next();
    void reset();

private:
    size_t m_position;
    size_t m_mapq;
    std::string m_cigar;
    std::string m_sequence;
};

#endif // SEQUENCE_HANDLER_HPP
