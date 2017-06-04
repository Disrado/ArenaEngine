#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>

const std::string pathToLog = "../../log.txt";

namespace ae
{
    
class Log
{
private:
    std::fstream logFile;

    Log();
    ~Log();
    
public:
    static Log& getInstance();

    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    void write(const std::string& _messageType,
               const std::string& _fileName,
               const std::string& _errorStringNum,
               const std::string& _message);
};

} // namespace ae


#endif //LOGGER_HPP
