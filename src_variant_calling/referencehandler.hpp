#ifndef REFERENCE_HANDLER_HPP
#define REFERENCE_HANDLER_HPP

#include "constants.hpp"
#include "../src_shared/filehandler.hpp"

#include <fstream>
#include <string>

class ReferenceHandler : public InFileHandler
{
public:
    ReferenceHandler(const std::string & path);

    std::string getPrefix();
    std::string getSequence();

    void seek(const size_t & pos);

private:
    void read(const size_t & size);
    void readPrefix();

    char m_buffer[SEQ_READ_SIZE + 1];
    char m_prefixBuffer[2];

    size_t m_startPos = 1;
    size_t m_endPos = SEQ_READ_SIZE;
    std::string m_prefix;
    std::string m_sequence;
};

#endif // REFERENCE_HANDLER_HPP
