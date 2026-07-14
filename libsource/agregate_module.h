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
#ifndef FROMA2_AGREGATE_MODULE_H
#define FROMA2_AGREGATE_MODULE_H

#include <common_values.hpp>    // Наиболее общие типы, переменные и функции библиотеки  FROMA2
#include <warehouse_module.h>   // Подключаем класс склада clsStorage
#include <manufact_module.h>    // Подключаем класс производства clsManufactory
#include <Impex_module.h>       // Импорт исходных данных из файлов
#include <baseproject_module.h> // Использование базового класса clsBaseProject
#include <variant>              // Использование типа variant

enum Clc_type{seq = sZero, fut, thrd};              // Сепаратор выбора типа вычислений: последовательные,
                                                    // в асинхронных потоках или синхронных
const Clc_type clc = thrd;                          // Константа с сепаратором

/** Определение типа элемента контейнера **/
typedef variant<clsStorage, clsManufactory> T_agregate;

/***************************************************************************************************************/
/**                                                                                                           **/
/**                            Структуры - посетители для типа T_agregate                                     **/
/**                                                                                                           **/
/***************************************************************************************************************/

struct Getter_exporting_data {
/** Структура с методами получения данных для экспорта в CSV-файлы. Возвращает указатель на новый массив типа
strItem с данными для последующего экспорта. **/
    private:
        strItem* (clsStorage::*fSdata)() const;     // Указатель на внутреннюю функцию класса clsStorage
        strItem* (clsManufactory::*fMdata)() const; // Указатель на внутреннюю функцию класса clsManufactory
        ChoiseData arr;                             // Сепаратор выбора данных: "purchase" - поставки, "balance"
                                                    // - остатки/незавершенное производство, "shipment" - отгрузки
    public:
        Getter_exporting_data(const ChoiseData _arr) : arr(_arr) {}     // Ctor с установкой сепаратора

        strItem* operator() (const clsStorage& obj) {           // Назначение указателю функции:
            if(arr == purchase) fSdata = &clsStorage::GetPure;  // возврата указателя на массив поступлений на склад
            else if(arr==balance) fSdata = &clsStorage::GetBal; // возврата указателя на массив остатков на складе
            else fSdata = &clsStorage::GetShip;                 // возврата указателя на массив отгрузок со склада
            return (obj.*fSdata)();                             // Получаем данные для экспорта
        }
        strItem* operator() (const clsManufactory& obj) {
            if(arr == purchase) fMdata = &clsManufactory::GetRMPurchPlan;   // возврата указателя на массив поступлений сырья
            else if(arr==balance) fMdata = &clsManufactory::GetTotalBalance;// возврата массива с незавершенным производством
            else fMdata = &clsManufactory::GetTotalProduct;     // иначе возврата массива с готовой продукцией
            return (obj.*fMdata)();                             // Получаем данные для экспорта
        }
};  // Getter_exporting_data

struct Getter_exporting_names {
/** Структура с методами получения наименований для экспорта в CSV-файлы. Возвращает указатель на новый массив
типа strNameMeas с наименованиями ресурсов и их единиц измерения для последующего экспорта. **/
    private:
        strNameMeas* (clsManufactory::*fMnames) () const;   // Указатель на внутреннюю функцию класса clsManufactory
        ChoiseData arr;                                 // Сепаратор выбора данных: "purchase" - поставки, "balance"
                                                        // - остатки/незавершенное производство, "shipment" - отгрузки
    public:
        Getter_exporting_names(const ChoiseData _arr) : arr(_arr) {}// Ctor с установкой сепаратора

        strNameMeas* operator() (const clsStorage& obj) {
            return obj.GetNameMeas();                               // Получаем данные для экспорта
        }
        strNameMeas* operator() (const clsManufactory& obj) {
            if((arr==balance) || (arr==shipment))
                fMnames = &clsManufactory::GetProductDescription;   // указываем на функцию возврата описания продуктов
            else                                                    // иначе указываем на функцию возврата указателя
                fMnames = &clsManufactory::GetRawMatDescription;    // на новый массив описания сырья и материалов
            return (obj.*fMnames)();                                // Получаем данные для экспорта
        }
};  // Getter_exporting_names

struct Getter_exporting_count {
/** Структура с методами получения числа номенклатурных позиций в массиве с наименованиями ресурсов и их
единиц измерения. Возвращает число типа size_t. **/
    private:
        const size_t (clsManufactory::*fMcount) () const;   // Указатель на функцию возврата числа ресурсов/продуктов
        ChoiseData arr;                                     // Сепаратор выбора данных: "purchase" - поставки, "balance"
                                                            // - остатки/незавершенное производство, "shipment" - отгрузки
    public:
        Getter_exporting_count(const ChoiseData _arr) : arr(_arr) {}// Ctor с установкой сепаратора

        const size_t operator() (const clsStorage& obj) {           // Получаем число позиций
            return obj.Size();
        }
        const size_t operator() (const clsManufactory& obj) {
            if((arr==balance) || (arr==shipment))
                fMcount = &clsManufactory::GetProdCount;            // указываем на функцию возврата числа продуктов
            else
                fMcount = &clsManufactory::GetRMCount;              // возврата числа позиций сырья и материалов
            return (obj.*fMcount)();
        }
};  // Getter_exporting_count

struct Calculater_back {
/** Структура с методами расчёта потребности в ресурсах для классов clsStorage и clsManufactory.
Метод расчёта выбирается в зависимости от сепаратора clc на этапе компиляции. **/
    TLack (clsStorage::*fStorageCalc)(size_t);          // Определяем тип указателя на внутреннюю функцию класса clsStorage
    void (clsManufactory::*fManufCalc_back)();          // Определяем тип указателя на внутреннюю функцию класса clsManufactory

    TLack operator()(clsStorage& obj) {                 // Оператор для варианта clsStorage
        if constexpr((clc == fut) || (clc == thrd))
            if constexpr(clc == fut) fStorageCalc = &clsStorage::Calculate_future;
            else fStorageCalc = &clsStorage::Calculate_thread;
        else fStorageCalc = &clsStorage::Calculate;
        return (obj.*fStorageCalc)(1);                 // Задаем однопроходную функцию для расчета потребности в ресурсах (1)
    }
    TLack operator() (clsManufactory& obj) {  // Оператор для варианта clsManufactory
        if constexpr((clc == fut) || (clc == thrd))
            if constexpr(clc == fut) fManufCalc_back = &clsManufactory::CalcRawMatPurchPlan_future;
            else fManufCalc_back = &clsManufactory::CalcRawMatPurchPlan_thread;
        else fManufCalc_back = &clsManufactory::CalcRawMatPurchPlan;
        (obj.*fManufCalc_back)();
        return {dZero, EmpStr}; // Для совместимости: все операторы должны возвращать результат одного и того же типа.
    }
};  // struct Calculater_back

struct Calculater_fwrd {
/** Структура с методами расчёта стоимости для классов clsStorage и clsManufactory.
Метод расчёта выбирается в зависимости от сепаратора clc на этапе компиляции. **/
    private:
    TLack (clsStorage::*fStorageCalc)(size_t);          // Определяем указатель на внутреннюю функцию класса clsStorage
    bool (clsManufactory::*fManufCalc_fwrd)();          // Определяем указатель на внутреннюю функцию класса clsManufactory

    public:
    TLack operator()(clsStorage& obj) {                 // Оператор для варианта clsStorage
        if constexpr((clc == fut) || (clc == thrd))
            if constexpr(clc == fut) fStorageCalc = &clsStorage::Calculate_future;
            else fStorageCalc = &clsStorage::Calculate_thread;
        else fStorageCalc = &clsStorage::Calculate;
        return (obj.*fStorageCalc)(2);                 // Задаем однопроходную функцию для расчета стоимостей (2)
    }
    TLack operator() (clsManufactory& obj) {
        if constexpr((clc == fut) || (clc == thrd))
            if constexpr(clc == fut) fManufCalc_fwrd = &clsManufactory::Calculate_future;
            else fManufCalc_fwrd = &clsManufactory::Calculate_thread;
        else fManufCalc_fwrd = &clsManufactory::Calculate;
        bool tmp = (obj.*fManufCalc_fwrd)();
        if(tmp) return {dZero, EmpStr}; // Для совместимости: все операторы должны возвращать результат одного
        else return {99999.99, "false"};// и того же типа. Если tmp == false, то возврат большого значения
    }
};  // struct Calculater_fwrd

struct Setter_ship {
/** Структура с методами ввода объемов отгрузок для классов clsStorage и clsManufactory. Объемы отгрузок в
натуральном выражении вводятся в конструкторе. **/
    private:
        strItem* buffer;        // Указатель на массив для хранения параметров вызываемых функций

    public:
        Setter_ship(strItem* &_buffer) : buffer(nullptr) {
             std::swap(_buffer, buffer);
        }   // Ctor with Move-parametrs

        ~Setter_ship() {
            if(buffer) delete[] buffer;
        }   // Dtor
        bool operator() (clsStorage& obj) {
            return obj.SetShipment(move(buffer));
        }
        bool operator() (clsManufactory& obj) {
            return obj.SetProdPlan(move(buffer));
        }
};  // Setter_ship

struct Getter_ship {
/** Структура с методами вывода объемов и стоимостных показателей отгрузок для классов clsStorage и clsManufactory **/
    public:
        strItem* operator() (const clsStorage& obj) {
            return obj.GetShip();
        }
        strItem* operator() (const clsManufactory& obj) {
            return obj.GetTotalProduct();
        }
};  // Getter_ship

struct Setter_purch {
/** Структура с методами ввода цен и объемов поставок для классов clsStorage и clsManufactory.
Цены вводятся в конструкторе. **/
    private:
        strItem* buffer;        // Указатель на массив для хранения параметров вызываемых функций

    public:
        Setter_purch(strItem* &_buffer) : buffer(nullptr) {
            std::swap(_buffer, buffer);
        }   // Ctor with Move-parametrs

        ~Setter_purch() {
            if(buffer) delete[] buffer;
        }   // Dtor
        bool operator() (clsStorage& obj) {
            return obj.SetPurchase(move(buffer));
        }
        bool operator() (clsManufactory& obj) {
            return obj.SetRawMatPrice(move(buffer));
        }
};  // Setter_purch

struct Getter_purch {
/** Структура с методами вывода объемов поставок в натуральном выражении для классов clsStorage
и clsManufactory **/
    public:
        strItem* operator() (clsStorage& obj) {
            return obj.GetPure();
        }
        strItem* operator() (clsManufactory& obj) {
            return obj.GetRMPurchPlan();
        }
};  // Getter_purch

struct Getter_NameMeas {
/** Структура с методами возврата указателя на вновь создаваемые массивы с наименованиями ресурсов и ед.измерения для
классов clsStorage и clsManufactory **/
    strNameMeas* operator() (const clsStorage& obj) {
        return obj.GetNameMeas();
    }

    strNameMeas* operator() (const clsManufactory& obj) {
        return obj.GetRawMatDescription();
    }
};  // Getter_NameMeas

struct Setter_progress_message {
    string Message;     // Сообщение индикатора прогресса

    void operator() (clsStorage& obj) {
        obj.Set_progress_message(move(Message));
    }

    void operator() (clsManufactory& obj) {
        obj.Set_progress_message(move(Message));
    }
};  // Setter_progress_message

/***************************************************************************************************************/
/**                                                                                                           **/
/**                                    Пространство имен nmAgregate                                           **/
/**                                                                                                           **/
/***************************************************************************************************************/

namespace nmAgregate {
    const size_t w1 = 25, w2 = 12, w3 = 15;     // Размеры полей в отчетах
    const size_t precis = 4;                    // Количество знаков после запятой в отчетах
    const string rTableName  = "Наименование";
    const string rTableMeas  = "Ед.измерения";
    const string rByVolume   = "В натуральном измерении";
    const string rByPrice    = "В удельном стоимостном измерении";
    const string rByValue    = "В стоимостном измерении";
    const string aPurchase   = "поступления";
    const string aBalance    = "остатки";
    const string aShipment   = "отгрузки";
};


/***************************************************************************************************************/
/**                                                                                                           **/
/**                                             clsAgregate                                                   **/
/**                                                                                                           **/
/***************************************************************************************************************/

class clsAgregate : public clsBaseProject {
/** Класс для объединения объектов типа clsStorage и clsManufactory в последовательную цепочку на базе вектора
типа variant. Наилучший способ использования данного класса - наследование в класс с конкретным числом элементов
вектора CostChain (конкретным числом подразделений преддприятия). **/
    private:

    public:
        /** Конструктор по умолчанию clsAgregate() перенесён в секцию private, чтобы его смог использовать
        только класс - наследник. **/

        void swap(clsAgregate& other) noexcept;
        /** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/

        clsAgregate(const clsAgregate& other);
        /** Конструктор копирования **/

        clsAgregate(clsAgregate&& other);
        /** Конструктор перемещения **/

        virtual ~clsAgregate();         // Деструктор

        clsAgregate& operator=(const clsAgregate& rhs);
        /** Перегрузка оператора присваивания копированием. Реализовано в идеоме
        КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/

        clsAgregate& operator=(clsAgregate&& rhs);
        /** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
        (move-and-swap idiom) **/

        /** Get - методы **/

        size_t ProjectCount();  /** Метод возвращает количество периодов проекта **/

        size_t Counts(const ChoiseData _arr, const size_t _count); /** Метод возвращает число ресурсов
        выбранного подразделения. Параметры: _arr - сепаратор выбора данных: "purchase" - поставки, "balance"
        - остатки/незавершенное производство, "shipment" - отгрузки; _count - индекс выбранного подразделения:
        0 - первое подразделение, принимающее ресурсы (элемент контейнера CostChain с индексом ноль);
        подразделение, отгружающее готовую продукцию будет иметь самый большой номер (элемент контейнера
        CostChain с максимальным индексом). **/

        strNameMeas* GetNameMeas(const ChoiseData _arr, const size_t _count) const; /** Метод возвращает указатель
        на новый массив типа strNameMeas с названием ресурсов и единицами измерения. Число элементов массива
        равно Counts(_arr, _count). Параметры: _arr - сепаратор выбора данных: "purchase" - поставки, "balance"
        - остатки/незавершенное производство, "shipment" - отгрузки; _count - индекс выбранного подразделения:
        0 - первое подразделение, принимающее ресурсы (элемент контейнера CostChain с индексом ноль);
        подразделение, отгружающее готовую продукцию будет иметь самый большой номер (элемент контейнера
        CostChain с максимальным индексом).**/

        bool Export_Data(const string filename, const size_t _dep, const ChoiseData _arr, const ReportData& flg) const;
        /** Метод записывает массив поставок, остатков или отгрузок выбранного подразделения в csv-файл с именем filename.
        Параметры: _dep - индекс подразделения в цепочке подразделений CostChain; _arr - выбор данных: "purchase" - поставки,
        "balance" - остатки/незавершенное производство, "shipment" - отгрузки; flg - тип выводимой в файл информации: volume
        - в натуральном, value - в стоимостном, price - в ценовом измерении. В качестве разделителя между полями используется
        символ _ch по умолчанию (';'). **/

        bool Detail_report(const size_t _dep, const string _depName[], const ChoiseData _arr, const ReportData& flg) const;
        /** Функция выводит выбранный отчет. Параметры: _dep - индекс подразделения в цепочке подразделений CostChain;
        _depName - указатель на массив с названиями подразделений (может указывать на nullptr; в этом случае вместо названий
        подразделений в отчёт будут выведены их номера); _arr - выбор данных: "purchase" - поставки, "balance" - остатки/
        незавершенное производство, "shipment" - отгрузки; flg - тип выводимой в файл информации: volume - в натуральном,
        value - в стоимостном, price - в ценовом измерении. Отчет выводится в файл с именем, содержащимся в переменной
        clsBaseProject::RName. **/

        /** Set - методы **/

        void Set_progress_shell(clsProgress_shell<type_progress>* val);
        /** Функция присваивает указателю pshell каждого объекта из контейнера CostChain адрес объекта
        val - оболочки для индикатора прогресса. **/

        void Set_share(const size_t index, const decimal _share[]);
        /** Функция устанавливает нормативы запасов на складе. Параметры: index - индекс элемента контейнера CostChain,
        в котором находится объект типа clsStorage, _share - указатель на массив с нормативами запасов ресурсов для каждого
        индивидуального склада. Запас выражается в долях от объемов отгрузки. **/

        void Set_permission(const size_t index, const bool _perm[]);
        /** Функция устанавливает разрешение/запрет на отгрузку и закупку на складе в одном и том же периоде. Параметры:
        index - индекс элемента контейнера CostChain, _perm - указатель на массив с флагами разрешения. **/

        void Set_autopurchase(const size_t index, const PurchaseCalc _pcalc[]);
        /** Функция устанавливает флаг авторасчета/ ручного расчета закупок на складе. Параметры: index - индекс элемента
        контейнера CostChain, _pcalc - указатель на массив с флагами авторасчета. **/

        bool Set_ship_volume(const strItem _Shipment[]);
        /** Метод ввода объемов отгрузки в подразделение с самым большим индексом. Параметры: _Shipment
        - указатель на вводимый массив с отгрузками. Метод возвращает True при удачном копировании входного массива
        во внутренний массив Shipment.**/

        bool Set_pur_price(const strItem _Purchase[]);
        /** Метод ввода цен поставок в подразделение с нулевым индексом. Параметры: _Purchase - указатель на вводимый
        массив с поставками. Метод возвращает True при удачном копировании входного массива во внутренний массив Purchase. **/

        bool Set_pur_volume(const strItem _Pur_vol[]);
        /** Метод ввода объемов поставок в подразделение типа clsStorage с самым низким индексом. Используется в случае, когда
        хотя бы для одного SKU установлен флаг ручного ввода объемов поставок (флаг "nocalc" - без автоматического расчета).
        ВНИМАНИЕ!!! Метод вызывается ДО вызова функции RCalculate, примерно рядом с вызовом метода Set_ship_volume. **/

        virtual void Reset() override;
        /** Метод сбрасывает всю информацию и приводит объект к состоянию по умолчанию **/

        /** View- методы **/

        void ViewSettings() const;
        /** Функция выводит на экран информацию о настройках для расчета. **/

        /** Расчётные методы **/

        bool RCalculate(const string msg[]);
        /** Метод рассчитывает объем ресурсов в натуральном выражении, поступающих в подразделение с нулевым индексом.
        Если расчёты завершились удачно (метод вернул true), в массиве Purchase будут содержаться расчётные данные.
        Параметры: msg - указатель на массив с наименованиями индикаторов прогресса. **/

        bool FCalculate(const string msg[]);
        /** Метод рассчитывает удельную и полную стоимости ресурсов, отгружаемых из подразделения с максимальным индексом.
        Если расчёты завершились удачно (метод вернул true), в массиве Shipment будут содержаться расчётные данные.
        Параметры: msg - указатель на массив с наименованиями индикаторов прогресса. **/


    protected:
        size_t PrCount;                 // Количество периодов проекта
        vector<T_agregate> CostChain;   // Контейнер типа variant для объединения объектов типа clsStorage и clsManufactory
        strItem* Purchase;              // Указатель на массив с ценами и объемами закупок; одномерный аналог матрицы
        strItem* Shipment;              // Указатель на буферный массив. До начала вычислений содержит объемы отгрузок;
                                        // после всех вычислений - объемы и стоимостные данные; одномерный аналог матрицы
        size_t sh_size;                 // Динамическое число строк массива Shipment (число столбцов равно PrCount)

        clsAgregate();
        /** Конструктор по умолчанию. Чтобы запретить создание экземпляров данного класса, конструктор перенесен
        в секцию protected. В наследуемых классах конструкторы необходимо объявлять в секции public. **/

        virtual void reportstream(ostream& os) const override;
        /** Метод выводит отчет в поток os. Используется невиртуальным методом clsBaseProject::Report **/

        /** Методы сериализации и десериализации **/

        virtual bool StF(ofstream &_outF) override;
        /** Метод имплементации записи в файловую переменную текущего экземпляра класса (запись в файл, метод
        сериализации). Параметры: &_outF - ссылка на экземпляр класса ofstream для записи данных **/

        virtual bool RfF(ifstream &_inF) override;
        /** Метод имплементации чтения из файловой переменной экземпляра класса (чтение из файла, метод
        десериализации). Параметры: &_inF - ссылка на экземпляр класса ifstream для чтения данных. **/
};

#endif // FROMA2_AGREGATE_MODULE_H
