#include "../src/operations/PlusOperation.h"
#include "../src/operations/PlusOperation.h"
#include "../src/operations/MinusOperation.h"
#include "../src/operations/MultOperation.h"
#include "../src/operations/DivOperation.h"
#include "../src/operations/UnarMinusOperation.h"
#include "../src/core/headers/CalculatorModule.h"

#include "../src/core/headers/Tokenezator.h"
#include "../src/core/headers/RawDataProcessor.h"

#include <vector>
#include <string>
#include "assert.h"
#include <iostream>


void testOperations() {
    Plus plusOp;
    assert(plusOp.getName() == "+");
    assert(plusOp.getArgumentCount() == 2);
    assert(plusOp.execute({2, 3}) == 5);

    Minus minusOp;
    assert(minusOp.getName() == "-");
    assert(minusOp.getArgumentCount() == 2);
    assert(minusOp.execute({5, 3}) == 2);

    Mult multOp;
    assert(multOp.getName() == "*");
    assert(multOp.getArgumentCount() == 2);
    assert(multOp.execute({4, 2}) == 8);

    Div divOp;
    assert(divOp.getName() == "/");
    assert(divOp.getArgumentCount() == 2);
    assert(divOp.execute({8, 4}) == 2);

    UnarMinus unarMinusOp;
    assert(unarMinusOp.getName() == "-_unar");
    assert(unarMinusOp.getArgumentCount() == 1);
    assert(unarMinusOp.execute({3}) == -3);


    std::cout << "Операции регистрируются корректно" << std::endl;
}

void testCalculatorModule() {
    CalculatorModule calculator;

    assert(calculator.calculate("2 + 3") == 5);
    assert(calculator.calculate("5 - 2") == 3);
    assert(calculator.calculate("4 * 2") == 8);
    assert(calculator.calculate("8 / 4") == 2);
    assert(calculator.calculate("- 3") == -3);

    std::cout << "Одиночные стандартные операции работают корректно" << std::endl;
}

void testTokenezator() {
    CalculatorModule calculator; 
    Tokenezator tokenizer = Tokenezator(calculator.getOperations(), calculator.getRawDataProcessor());

    std::vector<std::string> postfix = tokenizer.getPostfix("2 + 3");
    assert(postfix.size() == 3);
    assert(postfix[0] == "2");
    assert(postfix[1] == "3");
    assert(postfix[2] == "+");

    std::cout << "Преобразоваитель в обратную польскую нотацицю работает корректно" << std::endl;
}

void testRawDataProcessor() {
    CalculatorModule calculator;
    RawDataProcessor processor = calculator.getRawDataProcessor();

    assert(processor.isDig('5') == true);
    assert(processor.isDig('a') == false);

    std::vector<std::string> tokens = processor.split("12 + 34");
    assert(tokens.size() == 3);
    assert(tokens[0] == "12");
    assert(tokens[1] == "+");
    assert(tokens[2] == "34");

    assert(processor.isFunction("+") == true);
    assert(processor.isFunction("12") == false);

    std::cout << "Числа и операции парсятся корректно" << std::endl;
}


int main() {
    testOperations();
    testCalculatorModule();
    testTokenezator();
    testRawDataProcessor();

    return 0;
}