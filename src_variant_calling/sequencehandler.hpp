#ifndef SEQUENCE_HANDLER_HPP
#define SEQUENCE_HANDLER_HPP

#include "filehandler.hpp"

#include <fstream>
#include <string>

class SequenceHandler : public InFileHandler
{
public:
    SequenceHandler(const std::string & path);

    std::string getSequence();
    size_t getPosition();
    std::string getCigar();

    bool next();
    void reset();

private:
    int m_position;
    std::string m_cigar;
    std::string m_sequence;
};

#endif // SEQUENCE_HANDLER_HPP
