/**     Этот файл является частью библиотеки программного обеспечения для экономического
        моделирования, финансового анализа и планирования операционной деятельности предприятия
        "Free Operation Manager 2" (библиотека FROMA2).
**/

/****************************************************************************************************/
/****************************************************************************************************/
/*** 																						      ***/
/***   Copyright © 2025 Пидкасистый Александр Павлович										      ***/
/*** 																						      ***/
/***   Данная лицензия разрешает лицам, получившим копию данного программного обеспечения и	      ***/
/***   сопутствующей документации (далее — Программное обеспечение), безвозмездно использовать    ***/
/***   Программное обеспечение без ограничений, включая неограниченное право на использование,    ***/
/***   копирование, изменение, слияние, публикацию, распространение, сублицензирование и/или      ***/
/***   продажу копий Программного обеспечения, а также лицам, которым предоставляется данное      ***/
/***   Программное обеспечение, при соблюдении следующих условий:								  ***/
/*** 																					      	  ***/
/***   Указанное выше уведомление об авторском праве и данные условия должны быть включены во	  ***/
/***   все копии или значимые части данного Программного обеспечения.							  ***/
/*** 																						      ***/
/***   ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ «КАК ЕСТЬ», БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, ЯВНО   ***/
/***   ВЫРАЖЕННЫХ ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ ГАРАНТИИ ТОВАРНОЙ ПРИГОДНОСТИ, СООТВЕТСТВИЯ ПО	  ***/
/***   ЕГО КОНКРЕТНОМУ НАЗНАЧЕНИЮ И ОТСУТСТВИЯ НАРУШЕНИЙ, НО НЕ ОГРАНИЧИВАЯСЬ ИМИ. НИ В КАКОМ	  ***/
/***   СЛУЧАЕ АВТОРЫ ИЛИ ПРАВООБЛАДАТЕЛИ НЕ НЕСУТ ОТВЕТСТВЕННОСТИ ПО КАКИМ-ЛИБО ИСКАМ, ЗА УЩЕРБ   ***/
/***   ИЛИ ПО ИНЫМ ТРЕБОВАНИЯМ, В ТОМ ЧИСЛЕ, ПРИ ДЕЙСТВИИ КОНТРАКТА, ДЕЛИКТЕ ИЛИ ИНОЙ СИТУАЦИИ,   ***/
/***   ВОЗНИКШИМ ИЗ-ЗА ИСПОЛЬЗОВАНИЯ ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ ИЛИ ИНЫХ ДЕЙСТВИЙ С ПРОГРАММНЫМ     ***/
/***   ОБЕСПЕЧЕНИЕМ.																			  ***/
/*** 																						      ***/
/****************************************************************************************************/
/****************************************************************************************************/

#ifndef FROMA2_LONGREAL_H
#define FROMA2_LONGREAL_H

#include <iostream>         // Ввод и вывод на экран
#include <fstream>          // Ввод и вывод в файл
#include <string>           // Работа со строками
//#include <limits>         // Предельные значения фундаментальных типов
#include <sstream>          // Потоковый класс для работы со строками
#include <iomanip>          // std::setprecision
#include <cmath>            // Математические функции и проверка на NaN
#include "serialization_module.hpp" // Подключаем функции сериализации и десериализации

using namespace std;

/** При изменении типов дотаточно поменять значения справа от знака равно **/
using Stype = bool;                 // Тип знака числа (1 для отрицательных и 0 для положительных)
using Dtype = short int;            // Тип цифры мантиссы числа (каждая цифра имеет этот тип). Обязательно тип со знаком!
using Etype = int;                  // Тип числа экспоненты

/** Константы **/
//const Etype MinEtype = numeric_limits<Etype>::min()/2+1;  // Предельная граница для минимального значения экспоненты
//const Etype MaxEtype = numeric_limits<Etype>::max()/2-1;  // Предельная граница для максимального значения экспоненты
const Etype MinEtype = -32768;      // Текущая граница для минимального значения экспоненты
const Etype MaxEtype =  32768;      // Текущая граница для максимального значения экспоненты
const size_t manDigits = 64;        // Максимальное количество цифр после запятой

enum Scale{NANO=-9, MICRO=-6, MILL=-3, KILO = 3, MEGA=6, GIGA=9}; // Для повышенной читаемости параметра в методе Expchange

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                               class LongReal                                                        **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

class LongReal {
/** Класс для хранения и совершения арифметических операций с длинными вещественными числами. **/
    private:
        size_t NumCount;                // Количество цифр в числе равно количеству элементов массива digits
        Stype sign;                     // Знак числа (1 для отрицательных и 0 для положительных)
        Dtype* digits;                  // Указатель на динамический массив цифр длинной NumCount для хранения мантиссы числа
        Etype exponent;                 // Экспонента числа
        /** каждое число представляется в виде sign * 0.d1d2...dn * 10^exponent **/

        inline void setzero();                                             // Ввод числа ноль
        inline void setposinf();                                           // Ввод положительной бесконечности
        inline void setneginf();                                           // Ввод отрицательной бесконечности
        inline void setNaN();                                              // Ввод значения NaN
        void normalize();                                           // Нормализация числа
        inline Dtype* cut(const Dtype _digits[], size_t Num);       // Возвращает обрезанную копию заданного массива
        inline void Resize(const size_t _n);                        // Обрезание внутреннего массива до актуального размера
        void init(const string& s);                                 // Инициализация числа из строки
        stringstream getstream() const;                             // Вывод хранимого числа в поток stringstream
        inline bool ovrflw(const Etype& E1, const Etype& E2) const; // Контроль переполнения
        LongReal incrE(const Etype& _E) const;                      // Меняет экспоненту без изменения числа
        void incrD(const size_t& _N);       // Увеличивает длину массива digits до значения _N за счет правых нулей

    public:
        /** Конструкторы, деструктор, метод обмена **/
        LongReal();                         // Конструктор по-умолчанию
        LongReal(const string& value);      // Конструктор с инициализацией числа из строки
        LongReal(const double& value);      // Конструктор с инициализацией числа из числа типа double
        LongReal(const LongReal& obj);      // Конструктор копирования
        LongReal(LongReal&& obj);           // Конструктор перемещения
        ~LongReal();                        // Деструктор
        void swap(LongReal& obj) noexcept;  // Функция обмена значениями между объектами

        /** Перегрузка операторов присваивания **/
        LongReal& operator=(const LongReal &obj);       // Перегрузка оператора присваивания копированием объекта LongReal
        LongReal& operator=(const double &value);       // Перегрузка оператора присваивания копированием объекта double
        LongReal& operator=(const string &value);       // Перегрузка оператора присваивания копированием объекта string
        LongReal& operator=(LongReal &&obj);            // Перегрузка оператора присваивания перемещением

        /** Методы Get **/
        size_t Size() const;                    // Метод возвращает размер объекта в байтах
        template <typename T>
        T Get() const;                          // Возвращает число в форме string, long double, double или float
        string Get(size_t _n) const;            // Возвращает число в форме string с заданным количеством знаков после точки
        string EGet(size_t n) const;            // Возвращает число в форме string в виде .ddddEn (мантисса со степенью)

        /** Методы масштабирования числа в кило-, мега-, милли-, микро- и т.п. числа **/
        LongReal& Expchange(const Etype _exch); // Метод изменяет экспоненту числа

        /** Перегрузка логических и арифметических операторов **/
        bool isZero() const;                            // Метод возвращает true, если число равно +/-нулю
        bool isposInf() const;                          // Метод возвращает true, если число равно Inf
        bool isnegInf() const;                          // Метод возвращает true, если число равно -Inf
        bool isNaN() const;                             // Проверка числа на NaN (неопределенность)
        bool operator==(const LongReal& x) const;       // Оператор сравнения равно
        bool operator!=(const LongReal& x) const;       // Оператор сравнения не равно
        LongReal operator*(const LongReal& x) const;    // Оператор умножения
        LongReal operator-() const;                     // Унарный минус
        bool operator>(const LongReal& x) const;        // Оператор больше
        bool operator<(const LongReal& x) const;        // Оператор меньше
        bool operator>=(const LongReal& x) const;       // Оператор больше или равно
        bool operator<=(const LongReal& x) const;       // Оператор меньше или равно
        LongReal operator+(const LongReal& x) const;    // Оператор сложения
        LongReal operator-(const LongReal& x) const;    // Оператор вычитания
        LongReal& operator-=(const LongReal& x);        // Оператор декремента на величину x
        LongReal& operator+=(const LongReal& x);        // Оператор инкремента на величину x
        LongReal inverse() const;                       // Вычисление обратного числа (1/x)
        LongReal operator/(const LongReal& x) const;    // Перегрузка оператора деления
        friend LongReal fabs(const LongReal& x);        // Взятие числа по модулю

        /** Перегрузка операторов ввода и вывода **/
        friend ostream& operator<<(ostream& os, const LongReal& value);     // Оператор вывода информации в поток ostream
        friend stringstream& operator>>(stringstream& ss, LongReal& value); // Оператор ввода информации из потока stringstream
        friend istream& operator>>(istream& is, LongReal& value);           // Оператор ввода информации из потока cin

        /** Методы сериализации и десериализации **/
        bool StF(ofstream &_outF) const;                // Запись экземпляра класса в файловую переменную
        bool RfF(ifstream &_inF);                       // Чтение из файловой переменной в экземпляр класса
        bool SaveToFile(const string _filename) const;  // Запись экземпляра класса в файл
        bool ReadFromFile(const string _filename);      // Чтение из файла в экземпляр класса
        friend bool SEF(ofstream &_outF, LongReal& x);                  // Перегрузка методов SEF
        friend bool SEF(ofstream &_outF, LongReal x[], size_t Cnt);     // Перегрузка методов SEF
        friend bool DSF(ifstream &_inF,  LongReal& x);                  // Перегрузка методов DSF
        friend bool DSF(ifstream &_inF,  LongReal x[], size_t Cnt);     // Перегрузка методов DSF

        /** Методы визуального контроля **/
        void View(ostream& os) const;           // Метод визуального контроля числа с горизонтальным выводом массива
        void ViewM(ostream& os) const;          // Метод визуального контроля числа с вертикальным выводом массива

    protected:

};  // LongReal

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                               class lrstream                                                        **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

template<typename T>    // Проверка типа на соответствие float, double и long double
constexpr bool is_std_real_v =
    std::is_same<T, float>::value || std::is_same<T, double>::value || std::is_same<T, long double>::value;

struct lrstream {
/** Тип, реализующий "псевдопоток" для вывода в поток типа ostream чисел типа LongReal, float, double или
long double с одним и тем же манипулятором. Манипулятор ограничивает количество знаков после запятой у
вещественных чисел указанных типов. Может применяться в методах, где используется псевдоним decimal, при
этом манипулятор будет единственным для всех указанных типов. Преимущество данного подхода в том, что не
требуется промежуточная конвертация чисел LongReal в double и используется единый манипулятор для обоих типов. **/
    ostream *pos = &cout;                       // Поток для реального вывода чисел и символов
    const size_t N;                             // Количество знаков после запятой у вещественных чисел
    lrstream(const size_t w);                   // Констркутор
//    lrstream(ostream& oss, const size_t w);     // Констркутор

    lrstream& operator<<(const LongReal& val);  // Оператор вывода в поток lrstream вещественных чисел типа LongReal

    template<typename T, class=std::enable_if_t<is_std_real_v<T>>>
    lrstream& operator<<(const T& val);         // Оператор вывода в поток lrstream вещественных чисел встроенных типов

    lrstream& operator<<(const char val);       // Оператор вывода в поток lrstream символов (для вывода разделителей)

    ostream& operator<<(ostream&(*f)(ostream&));/** Оператор ввода в поток манипулятора для выхода из потока lrstream
        в поток ostream. В качестве аргумента может использоваться функция lr_exit или стандартные манипуляторы
        std::endl, std::flush и другие output-манипуляторы из заголовочного файла <ostream>
        (https://en.cppreference.com/w/cpp/io/manip.html) **/

//    void set_stream(ostream& oss);              // Метод установки потока для вывода типа ostream

};  // struct lrstream

lrstream lr_precision(const size_t w);
    /** Манипулятор для чисел типа LongReal, float, double и long double. Устанавливает количество
    выводимых знаков после запятой. **/

lrstream operator <<(ostream& os, lrstream&& m);
    /** Оператор ввода объекта типа lrstream в поток ostream. Позволяет подменить поток ostream
    потоком lrstream и принимать в поток lrstream последующие данные. **/

ostream& lr_exit(ostream&);
    /** Манипулятор для выхода из потока lrstream и возвращения в поток ostream. Вместо манипулятора
    lr_exit могут использоваться манипуляторы std::endl, std::flush и другие output-манипуляторы из
    заголовочного файла <ostream> (https://en.cppreference.com/w/cpp/io/manip.html)**/

#endif // FROMA2_LONGREAL_H
