#include "SentenceProcessor.h"


SentenceProcessor::SentenceProcessor() {
    registerBuiltinOperations();
    loadPlugins();
    _rdp = RawDataProcessor(operations);
}

void SentenceProcessor::registerBuiltinOperations() {
    registerOperation(std::make_unique<Plus>());
    registerOperation(std::make_unique<Minus>());
    registerOperation(std::make_unique<Mult>());
    registerOperation(std::make_unique<Div>());
    registerOperation(std::make_unique<UnarMinus>());
}

void SentenceProcessor::registerOperation(std::shared_ptr<IOperation> operation) {
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



int SentenceProcessor::priority(const std::string& s) {

    auto it = operations.find(s);
    if (it != operations.end()) {
        return it->second->getPriority(); 
    }
    return 0; 
}


std::vector<std::string> SentenceProcessor::_getPostfix(std::string input) {
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

float SentenceProcessor::calculate(std::string input) {
    std::vector<std::string> postfix = _getPostfix(input);
    
    std::stack<float> values;

    try {
        for (const std::string& token : postfix) {
            try {
                if (_rdp.isDig(token[0])) {

                    values.push(std::stof(token));
                } 
                else if (_rdp.isFunction(token)) {

                    auto it = operations.find(token);
                    if (it == operations.end()) {
                        throw std::invalid_argument("Неизвестная операция: " + token);
                    }

                    auto& operation = it->second;
                    size_t argCount = operation->getArgumentCount();

                    if (values.size() < argCount) {
                        throw std::invalid_argument("Недостаточно аргументов для операции: " + token);
                    }

                    std::vector<float> args;
                    for (size_t i = 0; i < argCount; i++) {
                        args.push_back(values.top());
                        values.pop();
                    }
                    std::reverse(args.begin(), args.end());

                    float result;
                    try {
                        result = operation->execute(args);
                    } catch (const std::domain_error& e) {
                        throw std::domain_error(token + ": " + e.what());
                    } catch (const std::exception& e) {
                        throw std::runtime_error(token + " ошибка исполнения операции: " + e.what());
                    }

                    values.push(result);
                } 
                else {
                    throw std::invalid_argument("Невалидный токен в начале выражения: " + token);
                }
            } catch (const std::exception& e) {
                throw std::invalid_argument(std::string("Токен '") + token + "': " + e.what());
            }
        }

        if (values.size() != 1) {
            throw std::invalid_argument("Некорректное кол-во значений");
        }

        return values.top();
    } 
    catch (const std::exception& e) {
        throw;
    }

}