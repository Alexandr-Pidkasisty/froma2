#include <iostream>             // Подключаем потоки ostream
#include <LongReal_module.h>    // Подключаем длинную арифметику
#include "common_values.hpp"    // Общие типы и методы библиотеки FROMA2>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");                                   // Установка русского языка для вывода

    const size_t n = 31;                                            // Количество знаков после запятой
    const string strPi    = "3.1415926535897932384626433832795";
    string strE     = "2.7182818284590452353602874713527";
    const LongReal lrPi   = strPi;
    LongReal lrE    = strE;
    const double dPi      = 3.1415926535897932384626433832795;
    float fEe       = 2.7182818284590452353602874713527;
    const char Space = '\t';
    const char* Header_os = "********************** Вывод в поток типа ostream ***********************\n";
    char* Header_lr = const_cast<char*>("********************** Вывод в поток типа lrstream **********************\n");

    cout << Header_os
         << '\n'
         << "Строковое представление чисел Пи и Е"
         << '\n'
         << strPi
         << Space
         << strE
         <<'\n'
         << "\nВывод чисел Пи и Е в формате чисел с плавающей точкой (Пи - double,\nЕ - float)"
         << " с точностью " << n << " знаков после запятой"
         << '\n'
         << fixed
         << setprecision(n)
         << dPi
         << Space
         << fEe
         << endl;

    lrstream los(n);
    los.pos = &cout;
    los  << '\n'
         << Header_lr
         << '\n'
         << "Строковое представление чисел Пи и Е"
         << '\n'
         << strPi.c_str()   // Если вводить в поток lrstream объект типа string, то происходит инициализация
         << Space           // числа типа LongReal строкой, далее в поток lrstream вводится число типа LongReal.
         << strE.c_str()    // В операторе << это число конвертируется в double и далее выодится в поток los.pos
         <<'\n'
         << "\nВ поток введен объект типа string\t"
         << strE
         << '\n'
         << "\nВывод чисел Пи и Е в формате LongReal"
         << '\n'
         << lrPi
         << Space
         << lrE
         << '\n'
         << "\nВывод чисел Пи и Е в формате чисел с плавающей точкой"
         << '\n'
         << dPi
         << Space
         << fEe
         << endl;

return 0;
}
