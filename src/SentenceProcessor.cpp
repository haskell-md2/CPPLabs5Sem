#include "SentenceProcessor.h"


SentenceProcessor::SentenceProcessor() {
    registerBuiltinOperations();
    loadPlugins();
}

void SentenceProcessor::registerBuiltinOperations() {
    registerOperation(std::make_unique<Plus>());
    registerOperation(std::make_unique<Minus>());
    registerOperation(std::make_unique<Mult>());
    registerOperation(std::make_unique<Div>());
    registerOperation(std::make_unique<UnarMinus>());
}

void SentenceProcessor::registerOperation(std::unique_ptr<IOperation> operation) {
    std::string name = operation->getName();
    operations[name] = std::move(operation);
}

void SentenceProcessor::loadPlugins() {
    auto pluginOperations = pluginManager.loadOperations();
    for (auto& operation : pluginOperations) {
        if (operation) {
            registerOperation(std::move(operation));
        }
    }
}

std::vector<std::string> SentenceProcessor::_split(std::string input) {
    std::vector<std::string> result;
    std::string current;
    
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
        else if (isalpha(c)) {
            current += c;
            if (i + 1 == input.length() || !isalpha(input[i + 1])) {
                result.push_back(current);
                current.clear();
            }
        }
        else {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
            result.push_back(std::string(1, c));
        }
    }
    
    if (!current.empty()) {
        result.push_back(current);
    }
    
    return result;
}

bool SentenceProcessor::isDig(char c) {
    return isdigit(c) || c == '.';
}

bool SentenceProcessor::isFunction(const std::string& token) {
    return operations.find(token) != operations.end();
}

int SentenceProcessor::priority(const std::string& s) {

    auto it = operations.find(s);
    if (it != operations.end()) {
        return it->second->getPriority(); 
    }
    return 0; 
}


std::vector<std::string> SentenceProcessor::_getPostfix(std::string input) {
    std::vector<std::string> stack;
    std::vector<std::string> result;
    std::vector<std::string> tokens = _split(input);
    
    for (size_t i = 0; i < tokens.size(); i++) {
        const std::string& s = tokens[i];
        char first_c = s.at(0);
        
        if (isDig(first_c) || (s.length() > 1 && isalpha(first_c))) {
            result.push_back(s);
        }
        else if (first_c == '(') {
            stack.push_back(s);
        }
        else if (first_c == ')') {
            while (!stack.empty() && stack.back() != "(") {
                result.push_back(stack.back());
                stack.pop_back();
            }
            if (!stack.empty() && stack.back() == "(") {
                stack.pop_back();
            }
        }
        else if (s == "-") {
            bool isUnary = (i == 0) || 
                          (i > 0 && (tokens[i-1] == "(" || 
                           operations.find(tokens[i-1]) != operations.end()));
            
            if (isUnary) {
                std::string unarMinus = "-_unar";
                while (!stack.empty() && stack.back() != "(" && 
                       priority(stack.back()) >= priority(unarMinus)) {
                    result.push_back(stack.back());
                    stack.pop_back();
                }
                stack.push_back(unarMinus);
            } else {
                while (!stack.empty() && stack.back() != "(" && 
                       priority(stack.back()) >= priority(s)) {
                    result.push_back(stack.back());
                    stack.pop_back();
                }
                stack.push_back(s);
            }
        }
        else if (isFunction(s)) {
            if (stack.empty()) {
                stack.push_back(s);
            }
            else {
                std::string prev_in_stack = stack.back();
                int new_priority = priority(s);
                int prev_priority = priority(prev_in_stack);
                
                while (!stack.empty() && stack.back() != "(" && 
                      prev_priority >= new_priority) {
                    result.push_back(stack.back());
                    stack.pop_back();
                    
                    if (!stack.empty()) {
                        prev_in_stack = stack.back();
                        prev_priority = priority(prev_in_stack);
                    }
                }
                stack.push_back(s);
            }
        }
        else {
            throw std::invalid_argument("Чуждый калькулятору символ : " + s);
        }
    }
    
    while (!stack.empty()) {
        if (stack.back() != "(") { 
            result.push_back(stack.back());
        }
        stack.pop_back();
    }

    return result;
}

float SentenceProcessor::calculate(std::string input) {

    std::vector<float> nums;

    for (const std::string& s : _getPostfix(input)) {


        if (isDig(s.at(0))) {
            nums.push_back(std::stof(s));
        }
        else {
  
            auto it = operations.find(s);
            if (it != operations.end()) {
                IOperation* operation = it->second.get();
                size_t argCount = operation->getArgumentCount();
                

                if (nums.size() < argCount) {
                    
                    throw std::invalid_argument("Недостаточно операндов: " + s);

                }
                
                std::vector<float> args;
                for (size_t i = 0; i < argCount; ++i) {
                    args.insert(args.begin(), nums.back());
                    nums.pop_back();
                }
                
                float result = operation->execute(args);
                nums.push_back(result);
            }
            else {
                throw std::invalid_argument("Неизвестная операция: " + s);
            }
        }
    }

    if (nums.size() != 1) {
        throw std::invalid_argument("Невалидное выражение");
    }

    return nums.at(0);
}