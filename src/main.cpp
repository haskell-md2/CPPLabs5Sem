#include <iostream>
#include "core/headers/CalculatorModule.h"


int main() {
    
    // if (std::setlocale(LC_ALL, "ru_RU.UTF-8") == nullptr &&
    //     std::setlocale(LC_ALL, "ru_RU.utf8") == nullptr &&
    //     std::setlocale(LC_ALL, "Russian_Russia.1251") == nullptr &&
    //     std::setlocale(LC_ALL, "rus") == nullptr) {
    //     std::cout << "Failed to set Russian locale. Enjoy the abracadabra! AZAZA!!1!" << std::endl;
    // }

    std::string s;
    

    CalculatorModule clac;
    
    while (true) {
        std::cout << "Введите выражение:" << std::endl;
        getline(std::cin, s);
        
        if (s == "exit" || s == "quit") {
            break;
        }
        
        try {
            float res = clac.calculate(s);
            std::cout << "Ответ: " << res << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}