#ifndef FROMA2_COMMON_VALUES_H_INCLUDED
#define FROMA2_COMMON_VALUES_H_INCLUDED

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

#include <iostream>                 // Типовые инструменты для организации ввода-вывода
#include <fstream>                  // Типовые инструменты для организации ввода-вывода в файл на диске
#include <iomanip>                  // Подключение функций и манипуляторов для потоков cout и cin
#include <algorithm>                // Функция copy_n
#include "LongReal_module.h"        // Длинная арифметика с типом LongReal
#include "serialization_module.hpp" // Методы сСкриализации и десериализации
#include "progress_module.hpp"      // Методы идикации прогресса вычислений

using namespace std;

/****************************************************************************************************/
/**                                                                                                **/
/**                       Стандарт данных для информационных потоков в модулях.                    **/
/**                                     Версия от 2025.02.13                                       **/
/**                                                                                                **/
/****************************************************************************************************/

/** Тип вещественных чисел. Псевдоним "decimal" может принимать значение "double" (встроенный в С++
    тип вещественного числа) или "LongReal". Тип LongReal - это собственный тип библиотеки FROMA2 для
    вещественных чисел повышенной точности. Изменение значения псевдонима decimal в этой строке кода
    меняет тип всех вещественных чисел во всех модулях библиотеки за исключением модуля LONGREAL:

    Варианты кода:

    typedef double decimal;     Вещественные числа представлены типом double
    typedef LongReal decimal;   Вещественные числа представлены типом LongReal  **/

typedef LongReal decimal;

/** Тип индикатора прогресса. Псевдоним "type_progress" может принимать разные значения в зависимости
    от используемого класса прогресс-индикатора. В модуле progress_module.hpp описан прогресс-индикатор для
    консольного вывода класс clsprogress_bar. Для отображения индикатора в приложениям с графическим
    интерфейсом, созданном, например, с помощью библиотеки wxWidget, можно использовать прогресс-индикатор
    класс wxProgressDialog из модуля wx/progdlg.h **/

typedef clsprogress_bar type_progress;

namespace nmBPTypes {   /** Наиболее употребимые константы, типы, функции **/

/** Наиболее употребимые константы **/
const size_t    sZero = 0,
                sOne  = 1,
                sTwo  = 2;
const decimal   dZero = 0.0;

const decimal   epsln = 1e-7;   // Бесконечно малая величина, являющаяся альтернативой нулю
                                // для сравнения вещественных чисел друг с другом и нулем

const string    EmpStr = "";    // Пустая строка
const string    stSpc  = " ";   // Пробел

/** Наиболее употребимые типы **/
enum ReportData{volume = sZero, price, value};              // Тип набора данных (в натуральном, в ценовом
                                                            // и стоимостном денежном выражении)
enum Currency{RUR = sZero, USD, EUR, CNY};                  // Тип валюты (российский рубль, доллар США, Евро,
                                                            // Китайский юань. Может дополняться другими валютами
const string CurrencyTXT[] = {"RUR", "USD", "EUR", "CNY"};  // Представление валют в виде текста. При расширении списка
                                                            // типа Currency, должен дополняться другими валютами

struct strItem {        /** Тип единичной партии ресурсов **/
	decimal volume;     // Размер партии ресурсов в натуральном измерении (например, в кг)
	decimal price;      // Цена за единицу товара (например, руб./кг)
	decimal value;      // Стоимость партии (например, руб)

    bool StF(ofstream &_outF) {
    /** Метод сериализации состояния объекта в файл **/
        if(!SEF(_outF, volume)) return false;
        if(!SEF(_outF,  price)) return false;
        if(!SEF(_outF,  value)) return false;
        return true;
	}   // StF

	bool RfF(ifstream &_inF) {
    /** Метод десериализации состояния объекта из файла **/
        if(!DSF(_inF, volume)) return false;
        if(!DSF(_inF,  price)) return false;
        if(!DSF(_inF,  value)) return false;
        return true;
	}   // RfF

	friend ostream& operator<<(ostream &stream, strItem st) {   //
	    /** Перегрузка оператора вывода информации в поток ostream **/
        stream << st.volume << ", ";
        stream << st.price << ", ";
        stream << st.value << "\n";
        return stream;
    };  // ostream &operator<<

};  // strItem

struct strNameMeas {    /** Тип описания единичной партии ресурсов **/
    string name;        // Наименование номенклатурной позиции
    string measure;     // Наименование единицы измерения номенклатурной позиции (например, кг или штук)
};  // strNameMeas

}   // namespace nmBPTypes

/****************************************************************************************************/

template<typename T>
void var_cpy(T* destin, const T* source, const size_t Num) {
/** Метод копирует данные из массива, на который ссылается указатель source в массив, на который ссылается
указатель destin. Размер обоих массивов равен Num. Если массивы имеют тривиально копируемый тип, копирование
осуществляется блоками памяти с помощью функции memcpy. В противном случае копирование поэлементное с помощью
функции copy_n. Тривиально копируемый тип (is_trivially_copyable) - это тип, хранилище которого является
непрерывным и, следовательно, его копирование подразумевает тривиальную копию блока памяти, как если бы это
выполнялось с помощью memcpy. Это справедливо для скалярных типов, тривиально копируемых классов и массивов
любых подобных типов. https://cplusplus.com/reference/type_traits/is_trivially_copyable **/
    // Если тип элементов массива - тривиально копируемый тип,
    // то копируем массивы поблочно с помощью функции memcpy
    // Иначе копируем массивы поэлементно с помощью функции std::copy_n
    (std::is_trivially_copyable<T>::value) ? memcpy(destin, source, sizeof(T)*Num) : std::copy_n(source, Num, destin);
}   // var_cpy


/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                          Константы и классы для методов визуального контроля и отчетов                              **/
/**                                                                                                                     **/
/*************************************************************************************************************************/
/** version from 2024.03.31 **/

namespace nmPrntSetup {

const size_t limit  = 9;                    // Предельное число столбцов для вывода отчета
const size_t sPrec  = 2;                    // Количество знаков после запятой в числовых данных (точность, разрядность)
const char chFill   = ' ';                  // Заполнитель
                                            // Константы для названий полей
const string    c_TableName = "Name",       // "Наименование"
                c_TableMeas = "Measure",    // "Ед.измерения"
                c_ByVolume  = "By volume",  // "В натуральном измерении"
                c_ByPrice   = "By price",   // "В удельном стоимостном измерении"
                c_ByValue   = "By value",   // "В стоимостном измерении"
                c_HCurrency = "RUR";        // Валюта по умолчанию для класса clsRePrint ("RUR", "CNY" и проч.)

class clsTextField {
/** Класс для установки параметров печати текстовых полей
https://stackoverflow.com/questions/9206669/making-a-table-with-printf-in-c **/
    private:
        size_t myWidth; // Ширина поля
    public:
        clsTextField( size_t width ) : myWidth( width ) {}
        friend std::ostream& operator<<( std::ostream& dest, clsTextField const& manip ) {
            dest.setf( std::ios_base::left, std::ios_base::adjustfield );
            dest.fill( chFill );
            dest.width( manip.myWidth );
            return dest;
        }   // friend std::ostream& operator<<
};  // clsTextField

class clsDataField {
/** Класс для установки параметров печати полей с числовыми данными
https://stackoverflow.com/questions/9206669/making-a-table-with-printf-in-c **/
    private:
        size_t myWidth;     // Ширина поля
        size_t precision;   // Количество знаков после запятой
    public:
        clsDataField( size_t width ) : myWidth( width ), precision(sPrec) {}                // Ctor с точностью sPrec знака после запятой
        clsDataField( size_t width, size_t prec ) : myWidth( width ), precision(prec) {}    // Ctor с установкой точности
        friend std::ostream& operator<<( std::ostream& dest, clsDataField const& manip ) {
            dest.setf( std::ios_base::right, std::ios_base::adjustfield );
            dest.setf( std::ios_base::fixed, std::ios_base::floatfield );
            dest.fill( chFill );
            dest.precision( manip.precision );
            dest.width( manip.myWidth );
            return dest;
        }   // friend std::ostream& operator<<
};  // clsDataField


}   // nmPrntSetup

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                       Методы визуального контроля                                                   **/
/**                                                                                                                     **/
/*************************************************************************************************************************/
/** version from 2024.03.31 **/

namespace nmPrntSrvs {      // Константы и функции обособлены в пространстве имен nmPrntSrvs

using namespace nmBPTypes;  // Разрешено использовать типы из пространства имен nmBPTypes
using namespace nmPrntSetup;// Разрешено использовать константы, типы и классы из пространства имен nmPrntSetup

/** Константы **/
const size_t sNames = 15;   // Установка формата вывода имен для использования в классе clsTextField
const size_t sNeas  = 12;   // Установка формата вывода ед.измерения для использования в классе clsTextField
const size_t sNumb  = 12;   // Установка формата вывода числовых данных для использования в классе clsDataField
const string CurUnt     = c_HCurrency + "/";   // Валюта на гатуральную единицу

template<typename Tdata, typename TName>
void ArrPrint(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount) {
/** Функция отображения таблиц, состоящих из первого столбца наименований и последующих столбцов с данными. Параметры:
const size_t ncount - число строк, равное числу элементов массива names[] с наименованиями строк, names[] - массив с
наименованиями строк и единицами измерения, data[] - одномерный массив, аналог двумерной матрицы размером ncount*dcount
с данными, dcount - число столбцов матрицы. Функция яляется альтернативой использованию класса clsRePrint для вывода
отчета на экран. Ширина полей фиксированная и не изменяемая. **/
    if(ncount==sZero) return;           // Проверка корректности параметров
    if(dcount==sZero) return;
    if(!names) return;
    if(!data) return;
    clsTextField name(sNames);          // Установка формата вывода имен с помощью класса clsTextField
    clsTextField meas(sNeas);           // Установка формата вывода единиц измерения
    clsDataField number(sNumb);         // Установка формата вывода числовых данных с помощью класса clsDataField
    size_t b = sZero;                   // Начальный номер столбца для вывода данных
    size_t e = limit;                   // Количество столбцов для вывода
    auto lambda = [&](){                // Лямбда-функция для вывода таблицы
        cout << name << c_TableName <<stSpc;                        // Вывод заголовков столбцов
        cout << meas << c_TableMeas <<stSpc;                        // Вывод заголовков столбцов
        for(size_t j=b; j<e; j++) {                             // Вывод заголовков столбцов (номера периодов)
            cout << number << j <<stSpc;
        };
        cout << endl;
        for(size_t i=sZero; i<ncount; i++) {                    // Цикл по строкам
            cout << name << (names+i)->name << stSpc;             // Вывод названия строки
            cout << meas << (names+i)->measure << stSpc;          // Вывод единицы измерения
            for(size_t j=b; j<e; j++) {                         // Цикл по столбцам
                cout << number << *(data+dcount*i+j) <<stSpc;     // Вывод данных столбца
            };
            cout << endl;
        };  // Цикл по строкам
    };  // lambda
    while(e < dcount) {     // Пока количество столбцов для вывода меньше общего числа столбцов, выводим по
        cout << endl;       // e-столбцов за столбцом с названияеми
        lambda();           /** Вывод таблицы числом столбцов, кратным limit **/
        b += limit;         // Изменяем начальный номер столбца
        e += limit;         // Изменяем количество столбцов для вывода
    };  // while
    cout << endl;
    e = dcount;
    lambda();               /** Вывод остатка столбцов таблицы **/
    cout << endl;
}   // ArrPrint

template<typename Tdata, typename TName>
void ArrPrint(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount, ReportData flg) {
/** Перегруженная функция отображения таблиц для отображения таблиц, получаемых из данных типа strItem. Дополнительный
параметр - flg - флаг, определяющий тип используемых данных: volume, price или value. **/
    if(ncount==sZero) return;
    if(dcount==sZero) return;
    if(!names) return;
    if(!data) return;
    clsTextField name(sNames);          // Установка формата вывода имен с помощью класса clsTextField
    clsTextField meas(sNeas);           // Установка формата вывода единиц измерения
    clsDataField number(sNumb);         // Установка формата вывода числовых данных с помощью класса clsDataField
    size_t b = sZero;                   // Начальный номер столбца для вывода данных
    size_t e = limit;                   // Количество столбцов для вывода
    string aMeas;                       // Единица измерения
    auto lambda = [&]() {
        if((flg==volume) || (flg==price))
            if(flg==volume) cout << name << c_ByVolume << stSpc; // Вывод "В натуральном измерении"
            else            cout << name << c_ByPrice  << stSpc; // Вывод "В удельном стоимостном измерении"
        else                cout << name << c_ByValue  << stSpc; // Вывод "В стоимостном измерении"
        cout << endl;
        cout << name << c_TableName <<stSpc;                   // Вывод заголовков столбцов
        cout << meas << c_TableMeas <<stSpc;                   // Вывод заголовков столбцов
        for(size_t j=b; j<e; j++) {                             // Вывод заголовков столбцов (номера периодов)
            cout << number << j <<stSpc;
        };
        cout << endl;
        // Вывод данных в таблицы
        for(size_t i=sZero; i<ncount; i++) {                    // Цикл по строкам
            cout << name << (names+i)->name << stSpc;             // Вывод названия строки
            if((flg==volume) || (flg==price))
                if(flg==volume) aMeas = (names+i)->measure;
                else            aMeas = CurUnt + (names+i)->measure;
            else                aMeas = c_HCurrency;
            cout << meas << aMeas << stSpc;                       // Вывод единицы измерения
                for(size_t j=b; j<e; j++) {                     // Цикл по столбцам
                    if((flg==volume) || (flg==price))
                        if(flg==volume) cout << number << (data+dcount*i+j)->volume << stSpc; // Вывод данных столбца
                        else            cout << number << (data+dcount*i+j)->price  << stSpc; // Вывод данных столбца
                    else                cout << number << (data+dcount*i+j)->value  << stSpc; // Вывод данных столбца
                };
            cout << endl;
        };  // Цикл по строкам
    };  // lambda
    while(e < dcount) {                 // Пока количество столбцов для вывода меньше общего числа столбцов, выводим по
        cout << endl;                   // e-столбцов за столбцом с названияеми
        lambda();
        b += limit;                                             // Изменяем начальный номер столбца
        e += limit;                                             // Изменяем количество столбцов для вывода
    };  // while
    cout << endl;
    e = dcount;
    lambda();
    cout << endl;
}   // ArrPrint with ReportData flag

template<typename Tdata, typename TName>
void ArrPrint(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount, ReportData flg,
const string& _hmcur) {
/** Перегруженная функция отображения таблиц для отображения таблиц, получаемых из данных типа strItem. Дополнительный
параметр - flg - флаг, определяющий тип используемых данных: volume, price или value; _hmcur - валюта отчета **/
    if(ncount==sZero) return;
    if(dcount==sZero) return;
    if(!names) return;
    if(!data) return;
    clsTextField name(sNames);          // Установка формата вывода имен с помощью класса clsTextField
    clsTextField meas(sNeas);           // Установка формата вывода единиц измерения
    clsDataField number(sNumb);         // Установка формата вывода числовых данных с помощью класса clsDataField
    size_t b = sZero;                   // Начальный номер столбца для вывода данных
    size_t e = limit;                   // Количество столбцов для вывода
    string aMeas;                       // Единица измерения
    auto lambda = [&]() {
        if((flg==volume) || (flg==price))
            if(flg==volume) cout << name << c_ByVolume << stSpc; // Вывод "В натуральном измерении"
            else            cout << name << c_ByPrice  << stSpc; // Вывод "В удельном стоимостном измерении"
        else                cout << name << c_ByValue  << stSpc; // Вывод "В стоимостном измерении"
        cout << endl;
        cout << name << c_TableName <<stSpc;                    // Вывод заголовков столбцов
        cout << meas << c_TableMeas <<stSpc;                    // Вывод заголовков столбцов
        for(size_t j=b; j<e; j++) {                             // Вывод заголовков столбцов (номера периодов)
            cout << number << j <<stSpc;
        };
        cout << endl;
        // Вывод данных в таблицы
        for(size_t i=sZero; i<ncount; i++) {                    // Цикл по строкам
            cout << name << (names+i)->name << stSpc;           // Вывод названия строки
            if((flg==volume) || (flg==price))
                if(flg==volume) aMeas = (names+i)->measure;
                else            aMeas = _hmcur + '/' + (names+i)->measure;
            else                aMeas = _hmcur;
            cout << meas << aMeas << stSpc;        // Вывод единицы измерения
                for(size_t j=b; j<e; j++) {                     // Цикл по столбцам
                    if((flg==volume) || (flg==price))
                        if(flg==volume) cout << number << (data+dcount*i+j)->volume << stSpc; // Вывод данных столбца
                        else            cout << number << (data+dcount*i+j)->price  << stSpc; // Вывод данных столбца
                    else                cout << number << (data+dcount*i+j)->value  << stSpc; // Вывод данных столбца
                };
            cout << endl;
        };  // Цикл по строкам
    };  // lambda
    while(e < dcount) {                 // Пока количество столбцов для вывода меньше общего числа столбцов, выводим по
        cout << endl;                   // e-столбцов за столбцом с названияеми
        lambda();
        b += limit;                                             // Изменяем начальный номер столбца
        e += limit;                                             // Изменяем количество столбцов для вывода
    };  // while
    cout << endl;
    e = dcount;
    lambda();
    cout << endl;
}   // ArrPrint with ReportData flag

}   // namespace nmPrntSrvs

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                   Константы, классы и функции для вывода отчета                                     **/
/**                                                                                                                     **/
/*************************************************************************************************************************/
/** version from 2024.08.31 **/

namespace nmRePrint {

using namespace nmBPTypes;      // Разрешено использовать типы из пространства имен nmBPTypes
using namespace nmPrntSetup;    // Разрешено использовать константы, типы и классы из пространства имен nmPrntSetup
                                // Константы для установления размеров полей
const size_t    c_n_min = 11,   // Минимальный размер поля для названия продукта/ ресурса
                c_m_min = 12,   // Минимальный размер поля для названия единицы измерения
                c_d_min =  3,   // Минимальный размер поля для числовых данных
                c_n_max = 15,   // Максимальный размер поля для названия продукта/ ресурса
                c_m_max = 15,   // Максимальный размер поля для названия единицы измерения
                c_d_max = 16;   // Максимальный размер поля для числовых данных (точность decimal 15 знаков, плюс знак разделителя)

const size_t smblcunt  = 120;   // Предельное число символов в строке

const char ch1 = ' ';
const char ch2 = '*';
const size_t uOne = 1;
const size_t uTwo = 2;
const size_t uThree = 3;

template<typename T>
size_t CalcSingleDataLenth(const T& val) {
/** Метод расчета количества знаков при форматированном выводе числа типа T. **/
    const T tOne = 1,
            tTen = 10;
    T temp = val;
    size_t i = sZero;
    while(temp > tOne) {    // Поиск числа разрядов
        temp = temp / tTen;
        i++;
    };// while
    i += 4;                 // Добавляем знак запятой, знаки дробной части после запятой и пустые поля
    return i;
}   // CalcSingleDataLenth for decimal

template<typename T=string>
size_t CalcSingleDataLenth(const string& str) {
/** Метод расчета количества заков при выводе строки **/
    int len = str.length(); // Определяем длину переменной типа string
    return len += 3;        // Добавляем пустые поля
}   // CalcSingleDataLenth for string type

template<typename T>
size_t CalcArrayDataLenth(const size_t _asize, const T _arr[], const size_t _minlimit, const size_t _maxlimit) {
/** Метод расчета максимального количества знаков у элементов массива чисел или строк. **/
    if((!_arr) || (_asize==sZero)) return sZero;        // Валидация параметров
    size_t temp = _minlimit;
    for(size_t i=sZero; i<_asize; i++) {
        temp = CalcSingleDataLenth(*(_arr+i))>temp ? CalcSingleDataLenth(*(_arr+i)): temp;
    };
    if(temp>_maxlimit) temp = _maxlimit;
    return temp;
} // CalcArrayDataLenth

template<typename Tdata, typename TName>
class clsRePrint {
/** Основной класс для форматированного вывода отчета на экран или в файл. **/
    private:
        bool calcflag;          // Флаг автоматической/ручной установки количества выводимых знаков

        size_t nwidth;          // Ширина колонки для поля с названием
        size_t mwidth;          // Ширина колонки для поля с ед.измерения
        size_t dwidth;          // Ширина колонки для числового поля
        size_t precis;          // Количество знаков после запятой

        size_t n_min;           // Минимальный размер поля для названия
        size_t m_min;           // Минимальный размер поля для ед.измерения
        size_t d_min;           // Минимальный размер для числового поля
        size_t n_max;           // Максимаьный размер поля для названия
        size_t m_max;           // Максимаьный размер поля для ед.измерения
        size_t d_max;           // Максимаьный размер для числового поля

        clsTextField* tname;    // Формат вывода имен
        clsTextField* tmeas;    // Формат вывода единиц измерения
        clsDataField* tnumb;    // Формат вывода числовых данных

        string TableName;       // Заголовок первого столбца отчета
        string TableMeas;       // Заголовок второго столбца отчета
        string ByVolume;        // Заголовок таблицы для вывода натуральных поазателей
        string ByPrice;         // Заголовок таблицы для вывода удельных стоимостных поазателей
        string ByValue;         // Заголовок таблицы для вывода полных стоимостных поазателей
        string HCurrency;       // Валюта проекта. Используется в единицах имзерения price и value типа strItem

        size_t rowcount;        // Количество строк отчета
        size_t colcount;        // Количество столбцов отчета
        const TName* rownames;  // Указатель на массив с названиями строк
        const Tdata* Tcoldata;  // Указатель на массив с данными типа Tdata

        streambuf* backup;      // Указатель на переменную типа буфер для сохранения состояния буфера вывода cout
        ofstream filestr;       // Переменная для записи в файловый поток
        ostream* out;           // Поток для вывода

    public:
        clsRePrint() {
        /** Конструктор с автоматической установкой ширины колонок отчета **/
            calcflag = true;    // Автоматическая установка количества выводимых знаков
            nwidth = sZero;     // Установка значений по умолчанию
            mwidth = sZero;
            dwidth = sZero;
            precis = sPrec;
            n_min = c_n_min;    // Установка значений по умолчанию
            m_min = c_m_min;
            d_min = c_d_min;
            n_max = c_n_max;
            m_max = c_m_max;
            d_max = c_d_max;
            tname = nullptr;
            tmeas = nullptr;
            tnumb = nullptr;
            TableName = c_TableName;
            TableMeas = c_TableMeas;
            ByVolume =  c_ByVolume;
            ByPrice  =  c_ByPrice;
            ByValue  =  c_ByValue;
            HCurrency = c_HCurrency;
            rowcount = sZero;
            colcount = sZero;
            rownames = nullptr;
            Tcoldata = nullptr;
            backup = nullptr;
            out = &std::cout;
        }   // Ctor clsRePrint

        clsRePrint(const size_t namewidth, const size_t measwidth, const size_t datawidth) {
        /** Конструктор с ручной установкой ширины колонок отчета. Параметры: namewidth - ширина поля "Наименование",
         measwidth - ширина поля "Единицы измерения", datawidth - ширина колонок с данными. **/
            calcflag = false;   // Ручная установка количества выводимых знаков
            nwidth = namewidth; // Установка введенных значений
            mwidth = measwidth;
            dwidth = datawidth;
            precis = sPrec;
            n_min = c_n_min;    // Установка значений по умолчанию
            m_min = c_m_min;
            d_min = c_d_min;
            n_max = c_n_max;
            m_max = c_m_max;
            d_max = c_d_max;
            tname = nullptr;
            tmeas = nullptr;
            tnumb = nullptr;
            TableName = c_TableName;
            TableMeas = c_TableMeas;
            ByVolume =  c_ByVolume;
            ByPrice  =  c_ByPrice;
            ByValue  =  c_ByValue;
            HCurrency = c_HCurrency;
            rowcount = sZero;
            colcount = sZero;
            rownames = nullptr;
            Tcoldata = nullptr;
            backup = nullptr;
            out = &std::cout;
        }   // Ctor clsRePrint

        clsRePrint(const string& filename, ios_base::openmode mode) {
        /** Конструктор с автоматической установкой ширины колонок отчета для перенаправления вывода в файл.
        Параметры: filename - имя файла, ios_base::openmode mode - флаг режима работы с файлом: app -
        данные добавляются в конец существующего файла, trunc - содержимое файла стирается перед записью. **/
            calcflag = true;    // Автоматическая установка количества выводимых знаков
            nwidth = sZero;     // Установка значений по умолчанию
            mwidth = sZero;
            dwidth = sZero;
            precis = sPrec;
            n_min = c_n_min;    // Установка значений по умолчанию
            m_min = c_m_min;
            d_min = c_d_min;
            n_max = c_n_max;
            m_max = c_m_max;
            d_max = c_d_max;
            tname = nullptr;
            tmeas = nullptr;
            tnumb = nullptr;
            TableName = c_TableName;
            TableMeas = c_TableMeas;
            ByVolume =  c_ByVolume;
            ByPrice  =  c_ByPrice;
            ByValue  =  c_ByValue;
            HCurrency = c_HCurrency;
            rowcount = sZero;
            colcount = sZero;
            rownames = nullptr;
            Tcoldata = nullptr;
            streambuf* psbuf;               // Вспомогательный буфер для файла
            filestr.open(filename, mode);   // Связываем буферную переменную с файлом для вывода и открываем файл для записи
            backup = cout.rdbuf();          // Запоминаем состояние текущего буфера cout в переменной backup
            psbuf = filestr.rdbuf();        // Получаем доступ к буферу файла
            cout.rdbuf(psbuf);              // Перенаправляем поток из cout в файл: теперь все будет в файл
            out = &std::cout;
        }   // Ctor clsRePrint

        clsRePrint(const size_t namewidth, const size_t measwidth, const size_t datawidth, \
            const string& filename, ios_base::openmode mode) {
        /** Конструктор с ручной установкой ширины колонок отчета для перенаправления вывода в файл. Параметры:
        namewidth - ширина поля "НАименование", measwidth - ширина поля "Единицы измерения", datawidth - ширина
        колонок с данными; filename - имя файла, ios_base::openmode mode - флаг режима работы с файлом: app -
        данные добавляются в конец существующего файла, trunc - содержимое файла стирается перед записью. **/
            calcflag = false;   // Ручная установка количества выводимых знаков
            nwidth = namewidth; // Установка введенных значений
            mwidth = measwidth;
            dwidth = datawidth;
            precis = sPrec;
            n_min = c_n_min;    // Установка значений по умолчанию
            m_min = c_m_min;
            d_min = c_d_min;
            n_max = c_n_max;
            m_max = c_m_max;
            d_max = c_d_max;
            tname = nullptr;
            tmeas = nullptr;
            tnumb = nullptr;
            TableName = c_TableName;
            TableMeas = c_TableMeas;
            ByVolume =  c_ByVolume;
            ByPrice  =  c_ByPrice;
            ByValue  =  c_ByValue;
            HCurrency = c_HCurrency;
            rowcount = sZero;
            colcount = sZero;
            rownames = nullptr;
            Tcoldata = nullptr;
            streambuf* psbuf;               // Вспомогательный буфер для файла
            filestr.open(filename, mode);   // Связываем буферную переменную с файлом для вывода и открываем файл для записи
            backup = cout.rdbuf();          // Запоминаем состояние текущего буфера в переменной backup
            psbuf = filestr.rdbuf();        // Получаем доступ к буферу файла
            cout.rdbuf(psbuf);              // Перенаправляем поток из cout в файл: теперь все будет в файл
            out = &std::cout;
        }   // Ctor clsRePrint

        ~clsRePrint() {
            if(tname) delete tname;         // Усли переменные существуют, то удаляем их
            if(tmeas) delete tmeas;
            if(tnumb) delete tnumb;
            rownames = nullptr;
            Tcoldata = nullptr;
            if(backup) {
                cout.rdbuf(backup);         // Восстанавливаем состояние буфера cout
                backup = nullptr;           // Обнуляем переменную
                filestr.close();            // Закрываем файл
            };
        }   // Dtor clsRePrint

        void ResetReport() {
        /** Метод сбрасывает форматы вывода имен, очищает указатели на входные массивы, возвращает перенаправление
        вывода в буфер cout и закрывает файл **/
            if(tname) { delete tname; tname = nullptr; };
            if(tmeas) { delete tmeas; tmeas = nullptr; };
            if(tnumb) { delete tnumb; tnumb = nullptr; };
            rownames = nullptr;
            Tcoldata = nullptr;
            precis = sPrec;
            if(backup) {                    // Если есть сохраненный буфер вывода в поток cout, то:
                cout.rdbuf(backup);         // Восстанавливаем состояние буфера cout
                backup = nullptr;           // Сбрасываем указатель
                filestr.close();            // Закрываем файл
            };
            out = &std::cout;
        }   // ResetReport

        void SetStream(ostream& val) {
        /** Установка потока для вывода **/
            out = &val;
        }   // SetStream

        void SetLimits(size_t _nmin, size_t _mnin, size_t _dmin, size_t _nmax, size_t _mmax, size_t _dmax) {
        /** Метод устанавливает минимальные и максимальные пределы для полей. **/
            n_min = _nmin;
            m_min = _mnin;
            d_min = _dmin;
            n_max = _nmax;
            m_max = _mmax;
            d_max = _dmax;
        }   // SetLimits

        void SetHeadings(const string& _tname, const string& _tmeasure, const string& _byvol, const string& _byprice, const string& _byvalue) {
        /** Метод устанавливает заголовки таблицы **/
            TableName = _tname;
            TableMeas = _tmeasure;
            ByVolume = _byvol;
            ByPrice  = _byprice;
            ByValue  = _byvalue;
        }   // SetHeadings

        void SetCurrency(Currency val) {
        /** Метод устанавливает наименование валюты, использующееся в выводе данных из price- и value- полей
        массива данных типа strItem. **/
            HCurrency = *(nmBPTypes::CurrencyTXT+val);
        }   // SetCurrency

        void ResetToAuto() {
        /** Метод переводит флаг установки количества выводимых знаков в автоматический режим. **/
            calcflag = true;    // Устанавливаем флаг в автоматический режим расчета ширины полей выводимых таблиц
            nwidth = sZero;     // Обнуляем поля
            mwidth = sZero;
            dwidth = sZero;
            precis = sPrec;
        }   // ResetToAuto

        void SetToManual(const size_t namewidth, const size_t measwidth, const size_t datawidth, size_t prec) {
        /** Метод переводит флаг установки количества выводимых знаков в ручной режим и вводит значения полей.
        Параметры: namewidth - ширина поля "Наименование", measwidth - ширина поля "Единицы измерения", datawidth
        - ширина колонок с данными, prec - количество знаков после запятой. **/
            calcflag = false;   // Ручная установка количества выводимых знаков
            nwidth = namewidth; // Присваиваем введенные значения полей
            mwidth = measwidth;
            dwidth = datawidth;
            precis = prec;
        }   // SetToManual

        void SetPrecision(size_t _prec) {
        /** Метод устанавливает количество знаков после запятой при выводе в отчет вещественных чисел **/
            precis = _prec;
        }   // SetPrecision

        bool SetReport(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount) {
        /** Метод вводит данные для отчета и рассчитывает ширину колонок. Параметры: ncount - число элементов массива
        names и число строк отчета; names - массив с именами и единицами измерения строк (наиболее подходящий тип - это
        тип strNameMeas); data - массив с данными; dcount - размерность массива data и число столбцов отчета. **/
            if( (ncount==sZero) || (!names) || (dcount==sZero) || (!data) ) return false;   // Валидация параметров
            rowcount = ncount;                              // Размер массива наименований строк
            colcount = dcount;                              // Размер массива данных для столбцов
            rownames = names;                               // Указательна массив наименований строк
            Tcoldata = data;                                // Указатель на массив данных для столбцов
            if(calcflag) {                                  // Если расчет автоматический, то
                string* ntemp = new(nothrow) string[rowcount];  // Выделяем память вспомогательному массиву наименований
                if(!ntemp) return false;                        // Если память не выдлена, то выход с false
                string* mtemp = new(nothrow) string[rowcount];  // Выделяем память вспомогательному массиву с единицами измерений
                if(!mtemp) { delete[] ntemp; return false; };   // Если память не выдлена, то выход с false
                for(size_t i=sZero; i<rowcount; i++) {          // Формируем временные массивы для анализа
                    *(ntemp+i) = (rownames+i)->name;            // Заполняем вспомогательные масивы наименованиями строк
                    *(mtemp+i) = (rownames+i)->measure;         // и единицами измерений
                };
                nwidth = CalcArrayDataLenth(rowcount, ntemp, n_min, n_max); // Определяем ширину поля для столбца с наименованием
                mwidth = CalcArrayDataLenth(rowcount, mtemp, m_min, m_max); // Определяем ширину поля для столбца с ед.измерения
                dwidth = CalcArrayDataLenth(colcount, Tcoldata, d_min, d_max); // Определяем ширину поля для данных
                delete[] ntemp;
                delete[] mtemp;
            };
            if(tname) delete tname;                             // Если объекты существуют, то удаляем их
            if(tmeas) delete tmeas;
            if(tnumb) delete tnumb;
            tname = new(nothrow) clsTextField(nwidth);          // Ширина поля для вывода первой текстовой колонки
            if(!tname) { return false; };
            tmeas = new(nothrow) clsTextField(mwidth);          // Ширина поля для вывода второй текстовой колонки
            if(!tmeas) { delete tname; return false; };
            tnumb = new(nothrow) clsDataField(dwidth, precis);  // Ширина поля для вывода колонки с цифрами
            if(!tnumb) { delete tname; delete tmeas; return false; };
            return true;
        }   // SetReport

        bool SetReport(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount, ReportData flg) {
        /** Метод вводит данные для отчета и рассчитывает ширину колонок для данных, выбранных флагом flg. Параметры:
        ncount - число элементов массива names и число строк отчета; names - массив с именами и единицами измерения строк;
        data - массив с данными с тремя полями; dcount - размерность массива data и число столбцов отчета **/
            if( (ncount==sZero) || (!names) || (dcount==sZero) || (!data) ) return false;   // Валидация параметров
            rowcount = ncount;
            colcount = dcount;
            rownames = names;
            Tcoldata = data;
            if(calcflag) {
                string* ntemp = new(nothrow) string[rowcount];
                if(!ntemp) return false;
                string* mtemp = new(nothrow) string[rowcount];
                if(!mtemp) { delete[] ntemp; return false; };
                decimal* dtemp = new(nothrow) decimal[colcount];
                if(!dtemp) { delete[] ntemp; delete[] mtemp; return false; };
                for(size_t i=sZero; i<rowcount; i++) {            // Формируем временные массивы для анализа
                    *(ntemp+i) = (rownames+i)->name;
                    *(mtemp+i) = (rownames+i)->measure;
                };
                nwidth = CalcArrayDataLenth(rowcount, ntemp, n_min, n_max); // Определяем ширину поля
                mwidth = CalcArrayDataLenth(rowcount, mtemp, m_min, m_max); // Определяем ширину поля
                switch(flg) {
                case volume:                                // Формируем временные массивы для анализа в зависимости от флага flg
                    for(size_t i=sZero; i<colcount; i++) {
                        *(dtemp+i) = (Tcoldata+i)->volume;
                    } break;
                case price:
                    for(size_t i=sZero; i<colcount; i++) {
                        *(dtemp+i) = (Tcoldata+i)->price;
                    } break;
                case value:
                    for(size_t i=sZero; i<colcount; i++) {
                        *(dtemp+i) = (Tcoldata+i)->value;
                    } break;
                };
                dwidth = CalcArrayDataLenth(colcount, dtemp, d_min, d_max); // Определяем ширину поля
                delete[] ntemp;
                delete[] mtemp;
                delete[] dtemp;
            };
            if(tname) delete tname;                             // Если объекты существуют, то удаляем их
            if(tmeas) delete tmeas;
            if(tnumb) delete tnumb;
            tname = new(nothrow) clsTextField(nwidth);          // Ширина поля для вывода первой текстовой колонки
            if(!tname)  return false;
            tmeas = new(nothrow) clsTextField(mwidth);          // Ширина поля для вывода второй текстовой колонки
            if(!tmeas) { delete[] tname; return false; };
            tnumb = new(nothrow) clsDataField(dwidth, precis);  // Ширина поля для вывода колонки с цифрами
            if(!tnumb) { delete[] tname; delete[] tmeas; return false; };
            return true;
        }   // SetReport

        void Print() {
        /** Метод выводит на экран данные, введенные с помощью функции SetReport **/
            if(rowcount==sZero) return;           // Проверка корректности параметров
            if(colcount==sZero) return;
            if(!rownames) return;
            if(!Tcoldata) return;
            TName* tmpnames = new(nothrow) TName[rowcount]; // Создаем вспомогательный массив, строки в котором можно урезать
            if(!tmpnames) return;                           // если память не выделена, то выходим из функции
            for(size_t i=sZero; i<rowcount; i++) {
                *(tmpnames+i) = *(rownames+i);              // Копируем во вспомогательный массив данные из массива с названиями
                (tmpnames+i)->name.resize(nwidth);          // Урезаем строку с названиями
                (tmpnames+i)->measure.resize(mwidth);       // Урезаем строку с единицами измерения
            };
            TableName.resize(nwidth);                       // Урезаем строки заголовков: наименование продукта
            TableMeas.resize(mwidth);                       // Урезаем строки заголовков: единица измерения
            const size_t limit = dwidth!=sZero?(smblcunt-nwidth-mwidth)/dwidth:sZero;    // Предельное число столбцов для вывода отчета
            size_t b = sZero;                   // Начальный номер столбца для вывода данных
            size_t e = limit;                   // Количество столбцов для вывода
            /** Лямбда функция **/
            auto lambda = [this, &b, &e, &tmpnames](){      // Лябда-функция
                *out << *tname << TableName <<stSpc;        // Вывод заголовков столбцов
                *out << *tmeas << TableMeas <<stSpc;        // Вывод заголовков столбцов
                for(size_t j=b; j<e; j++) {                 // Вывод заголовков столбцов (номера периодов)
                    *out << *tnumb << j <<stSpc;
                };
                *out << endl;
                // Вывод данных в таблицы
                for(size_t i=sZero; i<rowcount; i++) {                      // Цикл по строкам
                    *out << *tname << (tmpnames+i)->name << stSpc;          // Вывод названия строки
                    *out << *tmeas << (tmpnames+i)->measure << stSpc;       // Вывод единицы измерения
                    for(size_t j=b; j<e; j++) {                             // Цикл по столбцам
                        *out << *tnumb << *(Tcoldata+colcount*i+j) <<stSpc; // Вывод данных столбца
                    };
                    *out << endl;
                };  // Цикл по строкам

            };  // lambda
            while(e < colcount) {               // Пока количество столбцов для вывода меньше общего числа столбцов, выводим по
                *out << endl;                   // e-столбцов за столбцом с названияеми
                lambda();                       // Вывод таблицы числом столбцов, кратным limit
                b += limit;                     // Изменяем начальный номер столбца
                e += limit;                     // Изменяем количество столбцов для вывода
            };
            *out << endl;
            e = colcount;
            lambda();                           // Вывод остатка столбцов таблицы
            *out << endl;
            delete[] tmpnames;                  // Удаляем вспомогательный массив
        }   // Print

        void Print(ReportData flg) {
        /** Метод выводит на экран данные, введенные с помощью функции SetReport. Параметры: flg - выбор данных для массива
        типа strItem **/
            if(rowcount==sZero) return;           // Проверка корректности параметров
            if(colcount==sZero) return;
            if(!rownames) return;
            if(!Tcoldata) return;
            TName* tmpnames = new(nothrow) TName[rowcount];
            if(!tmpnames) return;
            for(size_t i=sZero; i<rowcount; i++) {
                *(tmpnames+i) = *(rownames+i);
                (tmpnames+i)->name.resize(nwidth);
                (tmpnames+i)->measure.resize(mwidth);
            };
            TableName.resize(nwidth);
            TableMeas.resize(mwidth);
            const size_t limit = dwidth!=sZero?(smblcunt-nwidth-mwidth)/dwidth:sZero;    // Предельное число столбцов для вывода отчета
            size_t b = sZero;                   // Начальный номер столбца для вывода данных
            size_t e = limit;                   // Количество столбцов для вывода
            string aMeas;                       // Единица измерения
            /** Лямбда функция **/
            auto lambda = [this, &b, &e, &flg, &aMeas, &tmpnames]() {  // Лябда-функция
                if((flg==volume) || (flg==price))
                    if(flg==volume) *out << *tname << ByVolume << stSpc; // Вывод "В натуральном измерении"
                    else            *out << *tname << ByPrice  << stSpc; // Вывод "В удельном стоимостном измерении"
                else                *out << *tname << ByValue  << stSpc; // Вывод "В стоимостном измерении"
                *out << endl;
                *out << *tname << TableName <<stSpc;                 // Вывод заголовков столбцов
                *out << *tmeas << TableMeas <<stSpc;                 // Вывод заголовков столбцов
                for(size_t j=b; j<e; j++) {                          // Вывод заголовков столбцов (номера периодов)
                    *out << *tnumb << j <<stSpc;
                };
                *out << endl;
                // Вывод данных в таблицы
                for(size_t i=sZero; i<rowcount; i++) {               // Цикл по строкам
                    *out << *tname << (tmpnames+i)->name << stSpc;   // Вывод названия строки
                    if((flg==volume) || (flg==price))
                        if(flg==volume) aMeas = (tmpnames+i)->measure;
                        else            aMeas = HCurrency + "/" + (tmpnames+i)->measure;
                    else                aMeas = HCurrency;
                    aMeas.resize(mwidth);                            // Выравниваем длину строки, чтобы таблица не поехала
                    *out << *tmeas << aMeas << stSpc;                // Вывод единицы измерения
                    for(size_t j=b; j<e; j++) {                      // Цикл по столбцам
                        if((flg==volume) || (flg==price))
                            if(flg==volume) *out << *tnumb << (Tcoldata+colcount*i+j)->volume << stSpc; // Вывод данных столбца
                            else            *out << *tnumb << (Tcoldata+colcount*i+j)->price  << stSpc; // Вывод данных столбца
                        else                *out << *tnumb << (Tcoldata+colcount*i+j)->value  << stSpc; // Вывод данных столбца
                    };
                    *out << endl;
                };  // Цикл по строкам
            };  // lambda
            while(e < colcount) {               // Пока количество столбцов для вывода меньше общего числа столбцов, выводим по
                *out << endl;                   // e-столбцов за столбцом с названияеми
                lambda();                       // Вывод таблицы числом столбцов, кратным limit
                b += limit;                     // Изменяем начальный номер столбца
                e += limit;                     // Изменяем количество столбцов для вывода
            };
            *out << endl;
            e = colcount;
            lambda();                           // Вывод остатка столбцов таблицы
            *out << endl;
            delete[] tmpnames;
        }   // Print

        void Header0(const string& val) {
        /** Метод выводит заголовок 0-го уровня для повышения читаемости тестов. Num - ширина заголовка
        в символах, val - строка заголовка **/
            size_t Num = smblcunt+uThree;
            size_t len = val.length();
            size_t field1, field2;
            field1 = (Num - len) / uTwo;
            field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
            auto SLIne = [&Num, this]() {
                *out << "**";
                for(size_t i=sZero; i<Num; i++)
                    *out << ch2;
                *out << "**\n";
            };
            auto SBord = [&Num, this]() {
                *out << "**";
                for(size_t i=sZero; i<Num; i++)
                    *out << ch1;
                *out << "**\n";
            };
            SLIne();
            SLIne();
            SBord();
            *out << "**";
            for(size_t i=sZero; i<field1; i++)
                *out << ch1;
            *out << val;
            for(size_t i=sZero; i<field2; i++)
                *out << ch1;
            *out << "**\n";
            SBord();
            SLIne();
            SLIne();
        }   // Header0

        void Header1(const string& val) {
        /** Метод выводит заголовок 1-го уровня для повышения читаемости тестов. Num - ширина заголовка
        в символах, val - строка заголовка **/
            size_t Num = smblcunt+uThree;
            size_t len = val.length();
            size_t field1, field2;
            field1 = (Num - len) / uTwo;
            field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
            auto SLIne = [&Num, this]() {
                *out << "**";
                for(size_t i=sZero; i<Num; i++)
                    *out << ch2;
                *out << "**\n";
            };
            SLIne();
            *out << "**";
            for(size_t i=sZero; i<field1; i++)
                *out << ch1;
            *out << val;
            for(size_t i=sZero; i<field2; i++)
                *out << ch1;
            *out << "**\n";
            SLIne();
        }   // Header1

        void Header2(const string& val) {
        /** Метод выводит заголовок 2-го уровня для повышения читаемости тестов. Num - ширина заголовка
        в символах, val - строка заголовка **/
            size_t Num = smblcunt+uThree;
            size_t len = val.length();
            size_t field1, field2;
            field1 = (Num - len) / uTwo;
            field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
            *out << "**";
            for(size_t i=sZero; i<field1-sOne; i++)
                *out << ch2;
            *out << ' ' << val << ch1;
            for(size_t i=sZero; i<field2-sOne; i++)
                *out << ch2;
            *out << "**\n";
        }   // Header2

};  // clsRePrint

template<typename T=void>
void PrintHeader0(std::ostream& out, const size_t& Num, const string& val) {
/** Метод выводит заголовок уровня 0 в поток os для повышения читаемости тестов. out - выходной поток,
Num - ширина заголовка в символах, val - строка заголовка **/
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / uTwo;
    field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
    auto SLIne = [&Num, &out]() {
        out << "**";
        for(size_t i=sZero; i<Num; i++)
            out << ch2;
        out << "**\n";
        };
    auto SBord = [&Num, &out]() {
        out << "**";
        for(size_t i=sZero; i<Num; i++)
            out << ch1;
        out << "**\n";
        };
    SLIne();
    SLIne();
    SBord();
    out << "**";
    for(size_t i=sZero; i<field1; i++)
        out << ch1;
    out << val;
    for(size_t i=sZero; i<field2; i++)
        out << ch1;
    out << "**\n";
    SBord();
    SLIne();
    SLIne();
}   // PrintHeader0

template<typename T=void>
void PrintHeader1(std::ostream& out, const size_t& Num, const string& val) {
/** Метод выводит заголовок уровня 1 в поток os для повышения читаемости тестов. out - выходной поток,
Num - ширина заголовка в символах, val - строка заголовка **/
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / uTwo;
    field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
    auto SLIne = [&Num, &out]() {
        out << "**";
        for(size_t i=sZero; i<Num; i++)
            out << ch2;
        out << "**\n";
        };
    SLIne();
    out << "**";
    for(size_t i=sZero; i<field1; i++)
        out << ch1;
    out << val;
    for(size_t i=sZero; i<field2; i++)
        out << ch1;
    out << "**\n";
    SLIne();
}   // PrintHeader1

template<typename T=void>
void PrintHeader2(std::ostream& out, const size_t& Num, const string& val) {
/** Метод выводит заголовок уровня 2 в поток os для повышения читаемости тестов. out - выходной поток,
Num - ширина заголовка в символах, val - строка заголовка **/
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / uTwo;
    field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
    out << "**";
    for(size_t i=sZero; i<field1-sOne; i++)
        out << ch2;
    out << ' ' << val << ch1;
    for(size_t i=sZero; i<field2-sOne; i++)
        out << ch2;
    out << "**\n";
}   // PrintHeader2

template<typename T=void>
void PrintHeader(const size_t& Num, const string& val) {
/** Метод выводит заголовок для повышения читаемости тестов. Num - ширина заголовка в символах,
val - строка заголовка **/
    const char ch1 = ' ';
    const char ch2 = '*';
    const size_t sTwo = 2;
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / sTwo;
    field2 = (Num - len) % sTwo == sZero? field1 : field1+sOne;
    auto SLIne = [&Num]() {
        cout << "**";
        for(size_t i=sZero; i<Num; i++)
            cout << ch2;
        cout << "**\n";
    };
    SLIne();
    cout << "**";
    for(size_t i=sZero; i<field1; i++)
        cout << ch1;
    cout << val;
    for(size_t i=sZero; i<field2; i++)
        cout << ch1;
    cout << "**\n";
    SLIne();
};  // PrintHeader

template<typename T=void>
void PrintUnderHeader(const size_t& Num, const string& val) {
/** Метод выводит подзаголовок для повышения читаемости тестов. Num - ширина заголовка в символах,
val - строка заголовка **/
    const char ch1 = ' ';
    const char ch2 = '*';
    const size_t sTwo = 2;
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / sTwo;
    field2 = (Num - len) % sTwo == sZero? field1 : field1+sOne;
    cout << "**";
    for(size_t i=sZero; i<field1-sOne; i++)
        cout << ch2;
    cout << ' ' << val << ch1;
    for(size_t i=sZero; i<field2-sOne; i++)
        cout << ch2;
    cout << "**\n";
}   //

}   // nmRePrint


#endif // FROMA2_COMMON_VALUES_H_INCLUDED
