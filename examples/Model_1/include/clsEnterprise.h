/**     Программа для расчета полных и удельных переменных затрат предприятия с одним центром затрат,
        построена с использованием библиотеки "Free Operation Manager 2" (библиотека FROMA2).
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

#ifndef CLSENTERPRISE_H
#define CLSENTERPRISE_H

#include "baseproject_module.h"             // Подключаем базовый класс clsBaseProject
#include "warehouse_module.h"               // Подключаем класс склада clsStorage
#include "manufact_module.h"                // Подключаем класс производства clsManufactory
#include "common_values.hpp"                // Некоторые функции
#include "Impex_module.h"                   // Импорт исходных данных из файлов
#include "pathes.h"                         // Пути к файлам конфигурации, отчета, входных и выходных данных

namespace nmEnterprise {
    struct strSettings {            // Структура индивидуальных настроек для склада
        decimal share;              // Запас ресурсов на складе, доля от отгрузок
        PurchaseCalc calc;          // Флаг авто/ ручного расчета поступлений на склад
        bool perm;                  // Флаг разрешения на поступления и отгрузки со склада в том же периоде
        bool StF(ofstream &_outF) {
        /** Метод сериализации состояния объекта в файл **/
            if(!SEF(_outF, share)) return false;
            if(!SEF(_outF,  calc)) return false;
            if(!SEF(_outF,  perm)) return false;
            return true;
        }   // StF
        bool RfF(ifstream &_inF) {
        /** Метод десериализации состояния объекта из файла **/
            if(!DSF(_inF, share)) return false;
            if(!DSF(_inF,  calc)) return false;
            if(!DSF(_inF,  perm)) return false;
            return true;
        }   // RfF
};  // strSettings
    const size_t w1 = 25, w2 = 12, w3 = 15;     // Размеры полей в отчетах
    const size_t precis = 4;                    // Количество знаков после запятой в отчетах
    const size_t num_strings = 20;              // Количество строк, выводимых на экран методом showSKUsettings
    const unsigned int ProgressWide = 75u;      // Ширина индикатора прогресса
    enum SelectDivision{warehouse = sZero, manufactory, rowmatstock};   // Флаг выбора: СГП, производство, склад сырья
    enum ManufData{manpurchase = 11, manbalance, manshipment, recipe};  // Флаг выбора: потребность в сырье, баланс, отгрузки, рецептуры
} // namespace nmEnterprise

string DBLR_ind();
/** Функция возвращает строку символов используемого типа вещественных чисел: "DB" - для вещественных
чисел используется тип double, "LR" - для вещественных чисел используется тип LongReal. Используется
для добавления суффикса к имени выводимых файлов. **/

void strItemReplace(size_t count, strItem* dstn, strItem* src, ReportData flg);
/** Функция заменяет значения полей, выбранных флагом flg в массиве dstn значениями полей из массива
src у элементов с одинаковым индексом **/

bool inData(string &_data, const string _defdata);
/** Метод вводит строковые данные пользователя. При отсутствии данных, подставляются значения по
умолчанию. Параметры: &_data - ссылка на вводимые пользователем данные, _defdata - значение по
умолчанию. Допускается в качестве параметров &_data и _defdata использовать имя одной и той же
переменной, например inData(x, x), поскольку второй параметр передается в функцию как копия. Метод
возвращает true в случае, если значение по умолчанию было отлично от NoFileName и новое значение
равно NoFileName; в противном случае метод возвращает false. **/

template<typename T>
constexpr bool is_ch_or_size_t_or_double_v =
    std::is_same<T, char>::value || std::is_same<T, size_t>::value || std::is_same<T, decimal>::value;

template<typename T, class=std::enable_if_t<is_ch_or_size_t_or_double_v<T>>>
void inData(T &_data, const T _defdata);
/** Метод вводит данные пользователя. Предназначен для данных, определяемых условием на типы данных
is_ch_or_size_t_or_double_v. При отсутствии данных, подставляются значения по умолчанию. Параметры:
&_data - ссылка на вводимые пользователем данные, _defdata - значение по умолчанию. Допускается в
качестве параметров &_data и _defdata использовать имя одной и той же переменной, например inData(x, x),
поскольку второй параметр передается в функцию как копия. **/

string FullFName(string _dir, string _fname);  /** Метод возвращает полное имя файла. Параметры: _dir -
путь к файлу, который может быть добавлен к его имени, _fname - имя файла. Если имя файла уже содержит путь,
т.е. является полным, то путь не добавляется. Иначе метод возвращает полное имя файла, состоящее из пути
и имени. **/

using namespace nmEnterprise;
const string confdir = V_DIR_CONFIG;            // Макрос V_DIR_CONFIG определен в файле pathes.h
extern string cfg_file;                         // Объявление переменной из main.cpp. Имя конфигурационного файла

const string NoFileName = "nofile";             // Имя для отсутствующего файла
const string indir = V_DIR_INPUTDATA;           // Путь к импортируемым файлам

/****************************************************************************************************/
/**                                 Структура strImportConfig                                      **/
/****************************************************************************************************/

struct strImportConfig {
/** Структурный тип для сохранения/ чтения конфигурационного файла для импорта **/
    string filename_About;          // Имя файла с описанием проекта
    string filename_Shipment;       // Имя файла с отгрузками с СГП в натуральном выражении
    string filename_Production;     // Объем производства в натуральном выражении
    string filename_Purchase;       // Имя файла с закупками на ССМ с ценами закупок
    string filename_Purchase_V;     // Имя файла с закупками на ССМ с объемами закупок
    string filenameprefix_Recipes;  // Префикс для имён файлов с рецептурами
    char _ch;                       // Символ разделителя между полями в CSV_файлах
    size_t HeadCols;                // Количество столбцов с заголовками в CSV_файлах
    size_t HeadRows;                // Количество строк с заголовками в CSV_файлах
    Currency _cur;                  // Домашняя валюта проекта
    AccountingMethod _amethod;      // Принцип учета запасов
    decimal P_Share;                // Запас ресурсов на складе ССМ, доля от отгрузок
    decimal S_Share;                // Запас продуктов на складе СГП, доля от отгрузок
    bool P_indr;                    // Флаг разрешения на отгрузки с ССМ в том же периоде, что и поступления
    bool S_indr;                    // Флаг разрешения на отгрузки с СГП в том же периоде, что и поступления

    size_t PsetCount;               // Размер массива P_settings;
    size_t SsetCount;               // Размер массива S_settings;
    strSettings* P_settings;        // Указатель на массив с индивидуальными настройками ССМ размером PsetCount
    strSettings* S_settings;        // Указатель на массив с индивидуальными настройками СГП размером S_settings

    strImportConfig();                                  // Конструктор по умолчанию
    strImportConfig(const strImportConfig&);            // Конструктор копирования
    strImportConfig(strImportConfig&&);                 // Конструктор перемещения
    strImportConfig& operator=(const strImportConfig&); // Оператор присваивания копированием
    strImportConfig& operator=(strImportConfig&&);      // Оператор присваивания перемещением
    ~strImportConfig();                                 // Деструктор

    bool SaveToFile(const string _filename);    // Запись конфигурации в файл
    void swap(strImportConfig& obj) noexcept;   // Функция обмена значениями между объектами
    bool ReadFromFile(const string _filename);  // Чтение конфигурации и файла
    void Entry();                               // Ввод данных с терминала
    void Show();                                // Отображает текущую конфигурацию на экране
    void Configure();                           // Управление конфигурацией

    size_t GetPsetCount() const;                        // Возвращает размер массива P_settings
    size_t GetSsetCount() const;                        // Возвращает размер массива S_settings
    strSettings* GetPurSettings() const;                // Возвращает указатель на новую копию массива P_settings
    strSettings* GetShpSettings() const;                // Возвращает указатель на новую копию массива S_settings
    bool SetPurSettings(const strSettings* _P_settings, const size_t pcount);   // Копирует массив _P_settings в массив P_settings
    bool SetShpSettings(const strSettings* _S_settings, const size_t scount);   // Копирует массив _S_settings в массив S_settings
    void SetToAuto(const SelectDivision& _dep); /** Устанавливает поля calc массива P_settings или S_settings в
        состояние calc (авторасчет поступлений). Выбор массива определяется флагом _dep: rowmatstock - для массива
        P_settings, warehouse - для массива S_settings **/
};  // strImportConfig

/****************************************************************************************************/
/**             Класс clsEnterprise. Наследник класса проекта clsBaseProject                       **/
/****************************************************************************************************/

class clsEnterprise : public clsBaseProject {
/** Объединяющий класс для всего предприятия - оболочка для классов clsStorage, clsManufactory и
шин передачи данных между объектами этих классов **/
    private:
        size_t PrCount;                 // Количество периодов проекта, из них 0 -стартовый, с 1 по 59 - прогнозные
        Currency Cur;                   // Домашняя валюта проекта
        AccountingMethod Amethod;       // Принцип учета запасов
        PurchaseCalc purcalc;           // Флаг разрешающий/ запрещающий автоматический расчёт объемов закупок ресурсов
        PurchaseCalc mancalc;           // Флаг разрешающий/ запрещающий автоматический расчёт объемов производства
        decimal PurchShare;             // Запас ресурсов на складе ССМ, доля от отгрузок
        decimal ShipShare;              // Запас продуктов на складе СГП, доля от отгрузок
        bool Purch_indr;                // Флаг разрешения на отгрузки с ССМ в том же периоде, что и поступления
        bool Ship_indr;                 // Флаг разрешения на отгрузки с СГП в том же периоде, что и поступления

        size_t ProdCount;               // Полное число выпускаемых продуктов
        strItem* Shipment;              // Указатель на массив отгрузок со склада СГП; размер массива ProdCount*PrCount
        strItem* Production;            // Указатель на массив отгрузок из производства на СГП; размер массива ProdCount*PrCount
        strNameMeas* ProdNames;         // Указатель на массив названий и ед. измерения продуктов; размер массива ProdCount

        size_t RMCount;                 // Полное число наименований сырья и материалов
        strItem* Consumpt;              // Указатель на массив поступлений ресурсов в производство; размер массива RMCount*PrCount
        strItem* Purchase;              // Указатель на массив поступлений ресурсов на склад ССМ; размер массива RMCount*PrCount
        decimal* PriceBus;              // Указатель на массив стоимости ресурсов для производства; размер массива RMCount*PrCount
        strNameMeas* RMNames;           // Указатель на массив названий и ед. измерения ресурсов; размер массива RMCount

        vector<clsRecipeItem> Recipe;   // Вектор с рецептурами продуктов; размер вектора ProdCount
        clsStorage* Warehouse;          // Указатель на склад готовой продукции (СГП)
        clsManufactory* Manufactory;    // Указатель на производство
        clsStorage* RawMatStock;        // Указатель на склад сырья и материалов / ресурсов (ССМ)

        strSettings* P_settings;        // Указатель на массив с индивидуальными настройками ССМ размером RMCount
        strSettings* S_settings;        // Указатель на массив с индивидуальными настройками СГП размером ProdCount

        void EraseVector(vector<clsRecipeItem>& Recipe);        // Метод для полной очистки вектора с рецептурами

    public:
        clsEnterprise();                                        // Конструктор по умолчанию
        void swap(clsEnterprise& other) noexcept;               // Функция обмена значениями между объектами
        clsEnterprise(const clsEnterprise& other);              // Конструктор копирования
        clsEnterprise(clsEnterprise&& other);                   // Конструктор перемещения
        clsEnterprise& operator=(const clsEnterprise& other);   // Оператор присваивания копированием
        clsEnterprise& operator=(clsEnterprise&& other);        // Оператор присваивания перемещением
        virtual ~clsEnterprise();                               // Деструктор

        /** Import - методы **/
        bool Import_Data();     /** Агрегированный метод импорта. Включает методы конфигурирования импорта,
        редактирования и сохранения конфигурации в файл; а также методы импорта данных **/

        /** Создание и расчет Склада готовой продукции (СГП)  **/
        bool SetWarehouse();    /** Метод создания склада готовой продукции (СГП) и ввода в него данных. **/

        void StockEditSettings(SelectDivision stk); /** Метод редактирует индивидуальные настройки учета
        номенклатурных единиц на выбранном складе (СГП или ССМ). Для каждого SKU возможно изменение данных:
        флаг авторасчета закупок, флаг разрешения на отгрузку и закупку в одном периоде и норматив запаса
        на складе. Настройки сохраняются в объекте "склад" и конфигурационном файле. **/

        bool StockCalculate(const SelectDivision& _dep, size_t thr); /** Метод рассчитывает требуемый объем
        закупок/поступлений на склад для тех SKU, у которых выставлен разрешающий такой расчет флаг. Выбор
        склада (СГП или ССМ) осуществляется значением параметра _dep: "warehouse" - СГП, любое другое значение
        - ССМ. Значением переменной thr устанавливается метод вычисления: "1" - TLack clsStorage::Calculate_future,
        "2" - TLack clsStorage::Calculate_thread, иначе TLack clsStorage::Calculate. **/

        /** Создание и расчет производства **/
        bool SetManufactory(); /** Метод создания производства и ввода параметров **/

        void ManufCalculateIn(size_t thr); /** Метод рассчитывает объем потребления сырья и материалов в производстве
        в натуральном выражении для всего плана выпуска всех продуктов. Значением переменной thr устанавливается метод
        вычисления: "1" - void clsManufactory::CalcRawMatPurchPlan_future,
        "2" - void clsManufactory::CalcRawMatPurchPlan_thread, иначе void clsManufactory::CalcRawMatPurchPlan. **/

        bool ManufCalculateOut(size_t thr); /** Метод рассчитывает объем, удельную и полную себестоимость
        незавершенного производства и готовой продукции для всех продуктов, выпускаемых на протяжении всего проекта.
        Значением переменной thr устанавливается метод вычисления: "1" - bool clsManufactory::Calculate_future(),
        "2" - bool clsManufactory::Calculate_thread(), иначе bool clsManufactory::Calculate(). **/

        bool MWCostTransmition(); /** Метод передает учетную себестоимость произведенной продукции из производства
        на склад готовой продукции **/

        /** Создание  и расчет склада сырья и материалов (ССМ) **/
        bool SetRawMatStock();  /** Метод создания склада сырья и материалов (ССМ) и ввода параметров. **/

        bool RMCostTransmition();   /** Метод передает учетную себестоимость сырья и материалов со склада сырья
        и материалов в производство **/

        /** Методы для визуального контроля и отчетов **/
        void ReportView(const SelectDivision& _rep, const int _arr, const ReportData flg) const; /** Функция выводит
        выбранный отчет. Параметры: _rep - выбранное подразделение (warehouse - СГП, manufactory - производство,
        rowmatstock- ССМ), _arr -  тип данных (purchase - массив поступлений. balance - массив остатков, shipment -
        массив отгрузок). Параметр flg - тип выводимой информации: volume - в натуральном, value - в стоимостном,
        price - в ценовом измерении **/

        void StockSettingsView(const SelectDivision& _rep); /** Функция выводит индивидуальные настройки склада.
        Параметры: _rep - выбранное подразделение (warehouse - СГП, rowmatstock или любой другой - ССМ) **/

        /** Export - методы **/

        bool Export_Data(string filename, const SelectDivision& _dep, const ChoiseData& _arr, \
        const ReportData& flg) const; /** Метод записывает массив поставок, остатков или отгрузок со склада готовой
        продукции (СГП), склада сырья и материалов (ССМ) или с Производства в csv-файл с именем filename. Параметры:
        _dep - флаг выбора подразделения: "warehouse" - СГП, "rowmatstock" - ССМ, "manufactory" - Производство; _arr
        - выбор данных: "purchase" - поставки, "balance" - остатки/незавершенное производство, "shipment" - отгрузки;
        flg - тип выводимой в файл информации: volume - в натуральном, value - в стоимостном, price - в ценовом
        измерении. В качестве разделителя между полями используется символ _ch по умолчанию (';'). **/

    protected:
        /** Заголовки таблиц в отчетах **/
        const string rTableName  = "Наименование";
        const string rTableMeas  = "Ед.измерения";
        const string rByVolume   = "В натуральном измерении";
        const string rByPrice    = "В удельном стоимостном измерении";
        const string rByValue    = "В стоимостном измерении";
        const string dWarehouse  = "Склад готовой продукции: ";
        const string dManufactory= "Производство: ";
        const string dRawMatStock= "Склад сырья и материалов: ";
        const string aPurchase   = "поступления";
        const string aBalance    = "остатки";
        const string aShipment   = "отгрузки";
        const string aRecipe1    = "Рецептура продукта: ";
        const string aRecipe2    = "Расход на ";

        /** Import - методы **/
        virtual bool Import_About(const string filename);   /** Метод читает информацию из файла с описанием
        проекта и формирует поля Title и Descript класса clsBaseProject. **/

        virtual bool ImportSingleArray(const string filename, const char _ch, size_t hcols, size_t hrows,\
        ReportData flg, strItem* &_data, strNameMeas* &_names, size_t& ColCount, size_t& RowCount);
        /** Метод читает информацию из файла с именем filename и разделителями между полями ch и заполняет поля:
        RowCount - число номенклатурных позиций (ресурсов или продуктов), ColCount - число периодов проекта,
        names - ссылка на указатель на массив с наименованиями номенклатурных позиций и единиц их измерения,
        _data - ссылка на указатель на формируемый массив, flg - флаг, определяющий тип импортируемых данных:
        "volume" - объемы в натуральном выражении, "price" - цены, "value" - стоимость. **/

        virtual bool Import_Recipes(const string filename, const char _ch, size_t hcols, size_t hrows); /** Метод
        читает информацию из файлов с именами, содержащими вначале filename и заканчивающимися на _i, где i- номер
        рецептуры. В качестве разделителя используется символ _ch. Метод заполняет вектор рецептур Recipe. **/

        /** Методы редактирования **/

        virtual bool SKUEdt(clsStorage* stock, const size_t num);
        /**  Метод редактирования введенной ранее информации: редактирование номенклатурной позиции (SKU).
        Параметры: stock - указатель на конкернтый склад (Warehouse или RawMatStock. **/

        /** Методы для визуального контроля и отчетов **/
        virtual void reportstream(ostream& os) const;   /** Метод выводит информацию о проекте в поток os.
        Метод переопределяет одноименный метод класса clsBaseProject **/

        virtual void showSKUsettings(ostream& os, clsStorage* stock) const; /** Метод вывода в выходной поток
        настроек учета на складе для каждого SKU: номер, название и ед. измерения SKU, флаг авторасчета закупок,
        флаг разрешения на отгрузку и закупку в одном периоде и норматив запаса на складе, Параметры: os - поток
        для вывода, stock - указатель на конкернтый склад(Warehouse или RawMatStock. **/

        virtual bool Report_Storage(clsStorage* obj, const int _arr, const ReportData flg) const;   /** Метод для
        вывода на экран отчетов по Объекту типа clsStorage (Warehouse или RawMatStock). Параметр obj - экземпляр
        класса clsStorage. Доступные массивы: _arr = purchase, массив поступлений на склад; _arr = balance, массив
        остатков на складе; _arr = shipment, массив отгрузок со склада. Параметр flg - тип выводимой информации:
        volume - в натуральном, value - в стоимостном, price - в ценовом измерении. Если значение _arr не относится
        к перечисляемому типу из модуля warehouse_module.h enum ChoiseData{purchase = sZero, balance, shipment},
        то отчет не выводится: другие значения _arr предназначены для других методов. Используется в методе
        Dev_Report_Storage **/

        virtual bool Report_Storage_to_dev(clsStorage* obj, const int _arr, const ReportData flg) const; /** Метод
        направляет отчет, созданный методом Report_Storage на выбранное устройство: "пустое" устройство, экран или
        файл (определяется содержанием переменной Rdevice). Использует метод Report_Storage. Параметры совпадают с
        параметрами метода Report_Storage. **/

        virtual void Report_Recipe() const; /** Метод для вывода в отчет рецептур всех продуктов **/

        virtual bool Report_Manufactory(const int _arr, const ReportData flg) const; /** Метод для вывода на экран
        отчетов по производсту. Доступные массивы: _arr = manpurchase, массив  потребности в сырье и материалах;
        _arr = manbalance, массив c балансами незавершенного производства для всех продуктов; _arr = manshipment,
        массив отгрузок на СГП; _arr = recipe, рецептуры продуктов. flg - тип выводимой информации: volume - в
        натуральном, value - в стоимостном, price - в ценовом измерении. Если значение _arr не относится к
        перечисляемому типу из данного модуля enum ManufData{manpurchase = 11, manbalance, manshipment, recipe},
        то отчет не выводится **/

};  // clsEnterprise

#endif // CLSENTERPRISE_H
