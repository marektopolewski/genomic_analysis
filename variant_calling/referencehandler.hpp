#ifndef REFERENCE_HANDLER_HPP
#define REFERENCE_HANDLER_HPP

#include "constants.hpp"
#include "filehandler.hpp"

#include <fstream>
#include <string>

class ReferenceHandler : public InFileHandler
{
public:
    ReferenceHandler(const std::string & path);

    std::string getSequence();
    void seek(const size_t & pos);

private:
    void read(const size_t & size);

    char m_buffer[SEQ_READ_SIZE + 1];
    size_t m_startPos = 1;
    size_t m_endPos = SEQ_READ_SIZE;
    std::string m_sequence;
};

#endif // REFERENCE_HANDLER_HPP
