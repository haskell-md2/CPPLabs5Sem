#pragma once

#include <iostream> //to del

#include <string>
#include <vector>

using namespace std;

class SentenceProcessor{

    private:
        vector<string> _postfix;
        
        vector<string> _getPostfix(string input);

        vector<string> _split(string input);

        bool isDig(char c);
        bool isOperation(char c);

        int priority(char s);

    public:
        
        SentenceProcessor();

        float calculate(string input);
};

