#include <AE/System/Logger.hpp>

namespace ae
{

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
{
    logFile.open(pathToLog, std::ios_base::out);
    if(!logFile.good())
        std::cout << "Log file does not exist";
}

Logger::~Logger()
{
    logFile.close();
}

void Logger::write(const std::string& messageType,
                   const std::string& message,
                   const std::string& fileName,
                   int errorStringNum)
{
    logFile << messageType
            << ": "
            << message << std::endl;

    if(fileName.length() > 0)
        logFile << "\tIn file: " << fileName;
    if(errorStringNum != -1)
        logFile << " | In " << errorStringNum << " line." << std::endl;
}

} // namespace ae
