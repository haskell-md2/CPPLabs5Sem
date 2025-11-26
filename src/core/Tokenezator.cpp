#include "headers/Tokenezator.h"

int Tokenezator::priority(const std::string& s) {

    auto it = _operations.find(s);
    if (it != _operations.end()) {
        return it->second->getPriority(); 
    }
    return 0; 
}


std::vector<std::string> Tokenezator::getPostfix(std::string input) {
    std::vector<std::string> tokens = _rdp.split(input);
    std::vector<std::string> output;
    std::stack<std::string> operators;

    for (const auto& token : tokens) {
        try {
            if (_rdp.isDig(token[0])) {

                size_t pos;
                std::stof(token, &pos);
                if (pos != token.length()) {
                    throw std::invalid_argument("Невалидное число: " + token);
                }
                output.push_back(token);
            } 
            else if (token == "(") {
                operators.push(token);
            } 
            else if (token == ")") {

                bool foundOpen = false;
                while (!operators.empty()) {
                    std::string op = operators.top();
                    operators.pop();
                    if (op == "(") {
                        foundOpen = true;
                        break;
                    }
                    output.push_back(op);
                }
                if (!foundOpen) {
                    throw std::invalid_argument("Беда со скобками");
                }
            } 
            else if (_rdp.isFunction(token)) {
                while (!operators.empty() && operators.top() != "(" 
                       && priority(operators.top()) >= priority(token)) {
                    output.push_back(operators.top());
                    operators.pop();
                }
                operators.push(token);
            } 
            else {
                throw std::invalid_argument("Неизвестный токен: " + token);
            }
        } catch (const std::exception& e) {
            throw std::invalid_argument(std::string("Ошибка парсинга: ") + e.what());
        }
    }

    while (!operators.empty()) {
        if (operators.top() == "(") {
            throw std::invalid_argument("Не хватает закрывающей скобки");
        }
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}