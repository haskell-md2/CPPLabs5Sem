#include <iostream>

#include "SentenceProcessor.h"

using namespace std;

int main(){
    
    string s;

    SentenceProcessor sp;

    
    while (true)
    {
        cout << "Введите выражение:" << endl;
        getline(cin, s);
        float res = sp.calculate(s);
        cout<< "Ответ: " << res << endl;
    }
    

    return 0;
}