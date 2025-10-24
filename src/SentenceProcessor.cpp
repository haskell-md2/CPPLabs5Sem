#include "SentenceProcessor.h"

SentenceProcessor::SentenceProcessor(){

}

vector<string> SentenceProcessor::_split(string input){
    vector<string> result;
    string current;
    
    for (size_t i = 0; i < input.length(); i++) {
        char c = input[i];

        if (c == ' ') continue;
        
        if (isdigit(c) || c == '.') {
            current += c;
            if (i + 1 == input.length() || (!isdigit(input[i + 1]) && input[i + 1] != '.')) {
                result.push_back(current);
                current.clear();
            }
        }

        else if (isOperation(c) || c == '(' || c == ')') {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
            result.push_back(string(1, c));
        }else{
            //throw exceptions!
        }
    }
    
    if (!current.empty()) {
        result.push_back(current);
    }
    
    return result;
}


bool SentenceProcessor::isDig(char c){
    return isdigit(c) || c == '.';
}

bool SentenceProcessor::isOperation(char c){
    return c == '+' || c == '-' || c == '*' || c == '/';
}


int SentenceProcessor::priority(char s){

    switch(s){
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }

}


vector<string> SentenceProcessor::_getPostfix(string input){

    vector<char> stack;
    vector<string> result;
    vector<string> tokens = _split(input);
    
    for(string s : tokens){
        char first_c = s.at(0);
        
        if(isDig(first_c)){
            result.push_back(s);
        }
        else if(first_c == '('){

            stack.push_back(first_c);
        }
        else if(first_c == ')'){

            while(!stack.empty() && stack.back() != '('){
                result.push_back(std::string(1, stack.back()));
                stack.pop_back();
            }
            if(!stack.empty() && stack.back() == '('){
                stack.pop_back();
            }
        }
        else if(isOperation(first_c)){
            if(stack.empty()){
                stack.push_back(first_c);
            }
            else{
                char prev_in_stack = stack.back();
                int new_priority = priority(first_c);
                int prev_priority = priority(prev_in_stack);
                
                while(!stack.empty() && stack.back() != '(' && 
                      prev_priority >= new_priority){
                    
                    result.push_back(std::string(1, stack.back()));
                    stack.pop_back();
                    
                    if(!stack.empty()){
                        prev_in_stack = stack.back();
                        prev_priority = priority(prev_in_stack);
                    }
                }
                stack.push_back(first_c);
            }
        }
    }
    

    while(!stack.empty()){
        if(stack.back() != '('){ 
            result.push_back(std::string(1, stack.back()));
        }
        stack.pop_back();
    }

    return result;
}


float SentenceProcessor::calculate(string input){

    vector<float> nums;

    for(string s : _getPostfix(input)){
        if(isDig(s.at(0))){
            nums.push_back(stof(s));
        }else{

            char c = s.at(0);
            float a = nums.back();
            nums.pop_back();
            float b = nums.back();
            nums.pop_back();

            float res = 0;
            switch (c)
            {
            case '+':
                res = a + b;
                break;
            case '-':
                res = b - a;
                break;
            case '*':
                res = a * b;
                break;
            case '/':
                res = b / a;
                break;

            default:
                break;
            }

            nums.push_back(res);
        }
    }

    return nums.at(0);
}