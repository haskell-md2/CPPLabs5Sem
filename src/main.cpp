#include <iostream>
#include "SentenceProcessor.h"


using namespace std;


int main() {
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