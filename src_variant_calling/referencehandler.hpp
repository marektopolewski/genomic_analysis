#ifndef REFERENCE_HANDLER_HPP
#define REFERENCE_HANDLER_HPP

#include "constants.hpp"
#include "../src_shared/filehandler.hpp"

#include <fstream>
#include <string>

#define SHORT_SEQ_LEN 82
#define LONG_SEQ_LEN 151
class ReferenceHandler : public InFileHandler
{
public:
    ReferenceHandler(const std::string & path, int seqLen);

    std::string getPrefix();
    std::string getSequence();

    void seek(const size_t & pos);

private:
    void read(const size_t & size);
    void readPrefix();

    char * m_buffer;
    char m_prefixBuffer[2];

    size_t m_startPos;
    size_t m_endPos;
    std::string m_prefix;
    std::string m_sequence;

    int m_seqLen;
};

#endif // REFERENCE_HANDLER_HPP
