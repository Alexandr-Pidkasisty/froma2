#ifndef LONGREALTEST_H
#define LONGREALTEST_H

#include <functional>               // Функции вместо операторов (std::function<bool(double, double))
#include "serialization_module.hpp" // Подключаем функции сериализации и десериализации

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

/** Основные пользовательские типы функций **/
typedef bool (LongReal::*TLogical)(const LongReal&) const;      // TLogical - тип указателя на логическую функцию-член
                                                                // класса LongReal с параметрами const LongReal&
typedef LongReal (LongReal::*TArithm)(const LongReal&) const;   // TArithm - тип указателя на арифметическую функцию-член
                                                                // класса LongReal с параметрами const LongReal&
typedef std::function<bool(double, double)> Comparator;         // Тип функционального объекта для логических операций
typedef std::function<double(double, double)> Arithmetor;       // Тип функционального объекта для арифметических операций

/** Функциональные объекты логических операций **/
equal_to<double> equals;        // Функциональный объект для проверки равенства
not_equal_to<double> noeq;      // Функциональный объект для проверки неравенства
greater<double> bigger;         // Функциональный объект для проверки свойства "больше"
less<double> lesser;            // Функциональный объект для проверки свойства "меньше"
greater_equal<double> ebigger;  // Функциональный объект для проверки свойства "больше или равно"
less_equal<double> elesser;     // Функциональный объект для проверки свойства "меньше или равно"

/** Функциональные объекты арифметических операций **/
plus<double> Plus;              // Функциональный объект арифметического сложения
minus<double> Minus;            // Функциональный объект арифметического вычитания
multiplies<double> Mult;        // Функциональный объект арифметического умножения
divides<double> Div;            // Функциональный объект арифметического деления

namespace nmLRtest {
    const size_t sZero = 0;
}   // nmLRtest

using namespace nmLRtest;

class LongRealTest {
/** Набор инструментов для тестирования класса LongReal. **/
    private:
        size_t lrlenth;     // Длина массива с данными для теста
        LongReal* data;     // Динамический массив с данными типа LongReal
        double* dcml;       // Динамический массив с копиями данных типа double
    public:
        LongRealTest() {
            lrlenth = sZero;
            data = nullptr;
            dcml = nullptr;
        }   // Default Ctor

        LongRealTest(const LongRealTest&) = delete;             // Запрет Copy/Move Ctor
        LongRealTest& operator=(const LongRealTest&) = delete;  // Запрет Copy/Move operator=

        ~LongRealTest() {
            if(data) delete[] data;
            if(dcml) delete[] dcml;
        }   // Dtor

        void Init() {
        /** Формирует массив данных для тестов **/
            lrlenth = 12;
            data = new(nothrow) LongReal[lrlenth];
            if(data) {
                *data     = 0;                      // Используем пограничные значения
                *(data+1) = numeric_limits<double>::infinity();
                *(data+2) = -numeric_limits<double>::infinity();
                *(data+3) = numeric_limits<double>::quiet_NaN();
                *(data+4) = 1;
                 (data+4)->Expchange(MaxEtype-3);
                *(data+5) = -(*(data+4));
                *(data+6) = 1;
                 (data+6)->Expchange(MinEtype);
                *(data+7) = -(*(data+6));
                for(size_t i=8; i<lrlenth; i++) {   // Генерация случайных чисел
                    *(data+i) = (((double)(rand()) - (double)(rand())) / (double)(rand()))*10;
                };
            };
        }   // Init

        void DcmlCopy() {
        /** Формирует массив данных для сравнения с тестовыми в формате double **/
            dcml = new(nothrow) double[lrlenth];   // Выделяем память массиву
            *(dcml)   = 0.0;
            *(dcml+1) = numeric_limits<double>::infinity();
            *(dcml+2) = -numeric_limits<double>::infinity();
            *(dcml+3) = numeric_limits<double>::quiet_NaN();
            if(dcml) {
                for(size_t i=4; i<lrlenth; i++)
                    *(dcml+i) = (data+i)->Get<double>();
            };
        }   // DcmlCopy

        void View() const {
        /** Метод визуального контроля внутреннего представления исходных данных для теста **/
            if(!data) {
                cout << "Данные для тестирования отсутствуют\n";
                return;
            };
            for(size_t i=sZero; i<lrlenth; i++) {
                cout << "data[" << i << "] is:" << endl;
                (data+i)->View(cout);
            }
        }   // View()

        void dataView() const {
        /** Вывод данных на экран **/
            if(!data || !dcml) {
                cout << "Данные для тестирования или копии для проверки отсутствуют\n";
                return;
            };
            for(size_t i=sZero; i<8; i++) {
                cout << setiosflags(ios::left) << "data[" << resetiosflags(ios::left) << setw(2) << i \
                << "] = " << setw(15) << (data+i)->EGet(5) << setiosflags(ios::left) << "; double data[" \
                << resetiosflags(ios::left) << setw(2) << i << "] = " << setw(15) << *(dcml+i) << endl;
            };
            for(size_t i=8; i<lrlenth; i++) {
                cout << setiosflags(ios::left) << "data[" << resetiosflags(ios::left) << setw(2) << i \
                << "] = " << setw(15) << (data+i)->Get(7) << setiosflags(ios::left) << "; double data[" \
                << resetiosflags(ios::left) << setw(2) << i << "] = " << setw(15) << *(dcml+i) << endl;
            };
        }   // dataView

        void Compare(string zn, TLogical f) {
        /** Функция тестирования сравнения чисел типа LongReal между собой. Метод сравнивает попарно все числа
        из массива с данными data. Параметры: string zn - строка с выводимыми символами сравнения (==, !=, >,
        <, >=, <=,), f - указатель типа TLogical на функцию-член класса LongReal, обеспечивающую требуемое
        сравнение (все функции типа bool, имеющие параметром const LongReal& x). ВНИМАНИЕ!!! Метод не проверяет
        соответствие функции и символа операции! **/
            cout << setiosflags(ios::right) << setw(14) << "LongReal " << setw(17) << "LongReal "\
            << setw(13) << "Результат" << resetiosflags(ios::right) << endl;
            for(size_t i=sZero; i<lrlenth; i++) {           // Перебор всех чисел по i
                for(size_t j=sZero; j<lrlenth; j++) {       // перебор всех чисел по j
                    cout << setw(13) << (data+i)->EGet(5) << zn << setw(13) << (data+j)->EGet(5) \
                    << " --> " << boolalpha << ((data+i)->*f)(*(data+j)) << endl;
                    // Вместо операции (*(data+i) == *(data+j)) и аналогичных используем синтаксис
                    // ((data+i)->*f)(*(data+j)), где f - указатель на используемую функцию, *(data+j) - параметр
                }
            }
        }   // Compare

        void TwiceCompare(string zn, TLogical f, Comparator comp) {
        /** Функция тестирования сравнения чисел типа LongReal между собой. Метод сравнивает попарно все числа
        из массива с данными data. Параметры: string zn - строка с выводимыми символами сравнения (==, !=, >,
        <, >=, <=,), f - указатель типа TLogical на функцию-член класса LongReal, обеспечивающую требуемое
        сравнение (все функции типа bool, имеющие параметром const LongReal& x). ВНИМАНИЕ!!! Метод не проверяет
        соответствие функции и символа операции! В отличие от предыдущего метода, производит также сравнения чисел
        типа double из массива копий dcml. **/
            cout << setiosflags(ios::right) << setw(14) << "LongReal " << setw(17) << "LongReal "\
            << setw(13) << "Результат" <<  setw(14) << "double" << resetiosflags(ios::right) << endl;
            for(size_t i=sZero; i<lrlenth; i++) {           // Перебор всех чисел по i
                for(size_t j=sZero; j<lrlenth; j++) {       // перебор всех чисел по j
                    cout << setw(15) << (data+i)->EGet(5) << zn << setw(15) << (data+j)->EGet(5) \
                    << " --> " << boolalpha << ((data+i)->*f)(*(data+j)) << setw(13) << comp(*(dcml+i), *(dcml+j)) << endl;
                    // Вместо операции (*(data+i) == *(data+j)) и аналогичных используем синтаксис
                    // ((data+i)->*f)(*(data+j)), где f - указатель на используемую функцию, *(data+j) - параметр
                }
            }
        }   // TwiceCompare

        void Arithmetic(string zn, TArithm f) {
        /** Метод тестирования арифметических операторов класса LongReal. Метод производит арифметические
        операции над парами чисел из массива с данными data. Параметры: string zn - строка с выводимыми символами
        (+, -, *, /), f - указатель типа TArithm на функцию-член класса LongReal, обеспечивающую требуемое
        арифметическое действие. ВНИМАНИЕ!!! Метод не проверяет соответствие функции и символа операции! **/
            for(size_t i=sZero; i<lrlenth; i++) {           // Перебор всех чисел по i
                for(size_t j=sZero; j<lrlenth; j++) {       // перебор всех чисел по j
                    cout << setw(13) << (data+i)->EGet(5) << zn << setw(13) << (data+j)->EGet(5) \
                    << " = " << (((data+i)->*f)(*(data+j))).EGet(5) << endl;
                }
            }
        }   // Arithmetic

        void TwiceArithmetic(string zn, TArithm f, Arithmetor math) {
        /** Метод тестирования арифметических операторов класса LongReal. Метод производит арифметические
        операции над парами чисел из массива с данными data. Параметры: string zn - строка с выводимыми символами
        (+, -, *, /), f - указатель типа TArithm на функцию-член класса LongReal, обеспечивающую требуемое
        арифметическое действие. ВНИМАНИЕ!!! Метод не проверяет соответствие функции и символа операции!
        В отличие от предыдущего метода, производит также операции над числами типа double из массива копий dcml.**/
            cout << setiosflags(ios::right) << setw(14) << "LongReal " << setw(16) << "LongReal "\
            << setw(16) << "Результат" <<  setw(18) << "double" << resetiosflags(ios::right) << endl;
            for(size_t i=sZero; i<lrlenth; i++) {           // Перебор всех чисел по i
                for(size_t j=sZero; j<lrlenth; j++) {       // перебор всех чисел по j
                    cout << setw(15) << (data+i)->EGet(5) << zn << setw(15) << (data+j)->EGet(5) \
                    << " = " << setw(14) << ((data+i)->*f)(*(data+j)).EGet(5) << setw(14) << math(*(dcml+i), *(dcml+j)) << endl;
                }
            }
        }   // TwiceArithmetic
};
#endif // LONGREALTEST_H
