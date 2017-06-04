#include <AE/System/Log.hpp>

namespace ae
{

Log& Log::getInstance()
{
    static Log instance;
    return instance;
}

Log::Log()
{
    logFile.open(pathToLog, std::ios_base::out);
    if(!logFile.good())
        std::cout << "Log file does not exist";
}

Log::~Log()
{
    logFile.close();
}

void Log::write(const std::string& _messageType, const std::string& _fileName,
                   const std::string& _errorStringNum, const std::string& _message)
{
    logFile << _messageType << " | "
            << _fileName << " | "
            <<_errorStringNum << " | "
            << _message << std::endl;
}

} // namespace ae
