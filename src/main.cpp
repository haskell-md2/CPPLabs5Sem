#include <iostream>
#include "SentenceProcessor.h"


using namespace std;


int main() {
    
    if (std::setlocale(LC_ALL, "ru_RU.UTF-8") == nullptr &&
        std::setlocale(LC_ALL, "ru_RU.utf8") == nullptr &&
        std::setlocale(LC_ALL, "Russian_Russia.1251") == nullptr &&
        std::setlocale(LC_ALL, "rus") == nullptr) {
        std::cout << "Failed to set Russian locale. Enjoy the abracadabra! AZAZA!!1!" << std::endl;
    }

    string s;
    

    SentenceProcessor sp;
    
    while (true) {
        cout << "Введите выражение:" << endl;
        getline(cin, s);
        
        if (s == "exit" || s == "quit") {
            break;
        }
        
        try {
            float res = sp.calculate(s);
            cout << "Ответ: " << res << endl;
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
    }

    return 0;
}