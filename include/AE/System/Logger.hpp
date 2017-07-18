#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <AE/System/Export.hpp>
#include <iostream>
#include <fstream>

const std::string pathToLog = "../../log.txt";

namespace ae
{

class SFML_SYSTEM_API Logger
{
private:
    std::fstream logFile;

    Logger();
    ~Logger();
    
public:
    static Logger& getInstance();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void write(const std::string& messageType,
               const std::string& message,
               const std::string& fileName = "",
               int errorStringNum = -1);
};

} // namespace ae


#endif //LOGGER_HPP
