#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>

#define BEGIN_TEST MESSAGE_test("Running " + std::string(__FUNCTION__) + ':');\
                   int TEST_RESULT = true;

#define END_TEST CHECK_TEST_RESULT_test(TEST_RESULT);

#define ERROR(message) ERROR_test(__FILE__, __LINE__, __FUNCTION__, (message));\
                       TEST_RESULT = false;

#define MESSAGE(message) MESSAGE_test(message)


// For create test, you need inherit this class Test and
// override beginTests() method, in which call methods that implement tests.
// Then, in function in main.cpp add include file and edit initialize list.
//
// Methods that implement tests should look like this:
//
// void <method_name>()
// {
//     BEGIN_TEST
//     ...
//     END_TEST
// }


class Test
{
protected:
    // Put message to std::cout
    void MESSAGE_test(const std::string &message)
	{
	    std::cout << '\n' << message << std::endl;
	}
    
    // Put error fileName, line, functionName and message to std::cout 
    void ERROR_test(const std::string &fileName,
		    int line,
		    const std ::string &functionName,
		    const std::string &message)
	{
	    std::cout << "\nERROR" 
		      << "\n\tFile: " << fileName
		      << "\n\tin function: " << functionName << "()"
		      << " line: " << line
		      << "\n\t\t" << message << std::endl;
	}

    // Check and put test status to std::cout
    void CHECK_TEST_RESULT_test(bool expression)
	{
	    if(expression) {
		std::cout << "Test complete successfully" << std::endl;
	    } else {
		std::cout << "Test failed\n" << std::endl;
		std::exit(0);
	    }
	}    

public:
    virtual ~Test() {}
    virtual void beginTests() {}
};





#endif //TEST_HPP
