#include "SentenceProcessor.h"


SentenceProcessor::SentenceProcessor() {
    registerBuiltinOperations();
    loadPlugins();
    _rdp = RawDataProcessor(operations);
    _tr = Tokenezator(operations,_rdp);
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



float SentenceProcessor::calculate(std::string input) {
    std::vector<std::string> postfix = _tr.getPostfix(input);
    
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