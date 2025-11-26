#include "headers/RawDataProcessor.h"

std::vector<std::string> RawDataProcessor::split(std::string input) {
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

bool RawDataProcessor::isDig(char c) {
    return isdigit(c) || c == '.';
}

bool RawDataProcessor::isFunction(const std::string& token) {
    return _operations.find(token) != _operations.end();
}
