#include <Test.hpp>

bool Test::getTestResult()
{
    return testResult;
}

void Test::Message(const std::string &message)
{
    std::cout << '\n' << message << std::endl;
}

void Test::Error(const std::string &fileName,
		 const std::string &functionName,
		 const std::string &message)
{
    std::cout << "\nERROR" 
	      << "\n\tFile: " << fileName
	      << "\n\tin function: " << functionName << "()"
	      << "\n\t\t" << message << std::endl;
}

void Test::Check(bool expression)
{
    if(expression)
	std::cout << "Test complete successfully" << std::endl;
    else {
	std::cout << "Test failed" << std::endl;
	testResult = false;
	std::exit(0);
    }
}
