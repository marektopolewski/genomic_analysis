#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <fstream>
#include <string>

class InFileHandler
{
public:
    InFileHandler(const std::string & path) : m_file(path) {}
    ~InFileHandler() { m_file.close(); }
    bool valid() { return m_file.is_open(); }

protected:
    std::ifstream m_file;
};

class OutFileHandler
{
public:
    OutFileHandler(const std::string & path) : m_file(path) {}
    ~OutFileHandler() { m_file.close(); }
    bool valid() { return m_file.is_open(); }

protected:
    std::ofstream m_file;
};

#endif // FILE_HANDLER_HPP
