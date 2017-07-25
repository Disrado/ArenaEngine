#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <functional>

#define BEGIN_TEST Message("Running " + std::string(__FUNCTION__) + ':');

class Test
{
private:
    bool testResult = true;
    
public:
    virtual ~Test() {}

    bool getTestResult();
    virtual void beginTests() {}
    
protected:
    virtual void Message(const std::string &fileName);
    
    virtual void Error(const std::string &fileName,
	 	       const std::string &functionName,
		       const std::string &message);

    virtual void Check(bool expression);
};


#endif //TEST_HPP
