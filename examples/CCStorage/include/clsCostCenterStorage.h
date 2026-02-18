/**     Класс, реализующий склад с двумя центрами затрат: на входе и на выходе, построен с
        использованием библиотеки "Free Operation Manager 2" (библиотека FROMA2).
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
#ifndef CLSCOSTCENTERSTORAGE_H
#define CLSCOSTCENTERSTORAGE_H

#include <warehouse_module.h>   // Подключаем класс склада clsStorage
#include <manufact_module.h>    // Подключаем класс производства clsManufactory
#include "Impex_module.h"       // Импорт исходных данных из файлов

#define cmeth thrd              // Выбираем типа вычислений
enum Clc_type{seq = sZero, fut, thrd};  // Сепаратор выбора типа вычислений: последовательные,
                                        // в асинхронных потоках или синхронных
enum count_choise{inStock=sZero, midStock, outStock};   // Сепаратор выбора подразделения

class clsCostCenterStorage : public clsStorage {
/** Класс "Склад" с центром затрат. Наследник класса clsStorage из библиотеки FROMA2. Состоит из:
ЦЗП - центр учета затрат по поступлению (PCC - Purchase Cost Centre), ЦУР - центр учета ресурсов
(RAC - Resource Accounting Centre) и ЦЗО - центр учета затрат по отгрузке (SCC - Shipment Cost Centre).
Роль ЦУР играет объект родительского класса clsStorage. **/
    private:
        size_t InCount, MidCount, OutCount; // Количество позиций на входе, в середине и выходе
        TLack Lack;                         // Размер дефицита и имя дефицитного ресурса
        clsManufactory* OutCost;            // ЦЗО - центр учета затрат по отгрузке
        clsManufactory* InCost;             // ЦЗП - центр учета затрат по поступлению
        vector<clsRecipeItem> RecipeOut;    // Набор тех.карт для ЦЗО
        vector<clsRecipeItem> RecipeIn;     // Набор тех.карт для ЦЗП
        strItem* Purchase;                  // Данные о ценах и объемах закупок; размер InCount*PrCount
        strItem* Shipment;                  // Данные о стоимости и объемах отгрузок; размер OutCount*PrCount

        strItem* cost_centre_purchase(clsManufactory* obj, vector<clsRecipeItem>&& _Recipy, strItem _ShipPlan[]);
        /** Метод вводит данные в Центр Затрат и возвращает потребность в ресурсах для него.
        Пареметры: obj - указатель на выбранный центр затрат (ЦЗП или ЦЗО); _Recipe - ссылка на
        перемещаемый контейнер с рецептурами для выбранного центра затрат; _ShipPlan - указатель
        на массив с отгрузками с выбранного ЦЗ. **/

        strItem* cost_centre_shipment(clsManufactory* obj, strItem _Purchase[]);
        /** Метод вводит цены из массива _Purchase в ЦЗ и возвращает себестоимость позиций на выходе ЦЗ.
        Параметры: obj - указатель на выбранный центр затрат (ЦЗП или ЦЗО); _Purchase - указатель на
        на массив с поступлениями в выбранный ЦЗ. **/

        void EraseVector(vector<clsRecipeItem>& Recipe);
        /** Метод для полной очистки вектора с рецептурами **/

        clsRecipeItem* find_recipe_by_name(const string& _name, vector<clsRecipeItem>& vec);
        /** Метод ищет элемент контейнера с именем _name и возвращает указатель на этот элемент.
        Если совпадение не найдено, метод возвращает nulptr. **/

    public:
        clsCostCenterStorage();                                             // Конструктор по умолчанию
        clsCostCenterStorage(size_t _PrCount, Currency _cur, AccountingMethod _ac, size_t ProdCount, \
            const size_t _MidCount, const size_t _RMCount, const strNameMeas _RMNames[]);   // Конструктор с параметрами
        virtual ~clsCostCenterStorage();                                            // Деструктор
        void swap(clsCostCenterStorage& other) noexcept;                    // Функция обмена значениями между объектами
        clsCostCenterStorage(const clsCostCenterStorage& other);            // Конструктор копирования
        clsCostCenterStorage(clsCostCenterStorage&& other);                 // Конструктор перемещения
        clsCostCenterStorage& operator=(const clsCostCenterStorage& other); // Оператор присваивания копированием
        clsCostCenterStorage& operator=(clsCostCenterStorage&& other);      // Оператор присваивания перемещением

        /** Методы сериализации и десериализации **/

        bool StF(ofstream &_outF);
            /** Метод имплементации записи в файловый поток текущего экземпляра класса (метод сериализации).
            Параметры: &_outF - экземпляр класса ofstream для записи данных **/

        bool SaveToFile(const string _filename);
            /** Метод записи текущего экземпляра класса clsCostCenterStorage в файл **/

        bool RfF(ifstream &_inF);
            /** Метод восстановления из файлового потока текущего экземпляра класса (метод десериализации).
            Параметры: &_outF - экземпляр класса ofstream для записи данных **/

        bool ReadFromFile(const string _filename);
            /** Метод чтения из файла и запись в экземпляр класса clsCostCenterStorage **/

        /** Set - методы **/

        bool SetStorage(strItem _ShipPlan[], strItem _Purchase[],\
            const clsRecipeItem _RecipeOut[], const clsRecipeItem _RecipeIn[]);
        /** Метод ввода исходных данных. Параметры: _ShipPlan - указатель на массив отгрузок со склада размером
        ProdCount * _PrCount, где ProdCount и _PrCount - количество позиций, отгружаемых со склада и количество
        периодов проекта, соответственно, введенные ранее в конструкторе с параметрами; _Purchase - указатель на
        массив с объемами поступлений (при отсутствии флага авторасчёта) и ценами поступаемых на склад ресурсов
        размером _RMCount * _PrCount, где _RMCount - число позиций ресурсов, поступающих на склад (включают в себя
        ресурсы, отгружаемые впоследствии со склада и ресурсы, потребляемые на складе), введенное также ранее в
        конструкторе с параметрами; _RecipeOut и _RecipeIn - константные указатели на копируемые массивы с
        технологическими картами для ЦЗО и ЦЗП соответственно. **/

        bool SetStorage(strItem _ShipPlan[], strItem _Purchase[],\
            vector<clsRecipeItem>&& _RecipeOut, vector<clsRecipeItem>&& _RecipeIn);
        /** Метод ввода исходных данных. Параметры: _ShipPlan - указатель на массив отгрузок со склада размером
        ProdCount * _PrCount, где ProdCount и _PrCount - количество позиций, отгружаемых со склада и количество
        периодов проекта, соответственно, введенные ранее в конструкторе с параметрами; _Purchase - указатель на
        массив с объемами поступлений (при отсутсвтии флага авторасчёта) и ценами поступаемых на склад ресурсов
        размером _RMCount * _PrCount, где _RMCount - число позиций ресурсов, поступающих на склад (включают в себя
        ресурсы, отгружаемые впоследствии со склада и ресурсы, потребляемые на складе), введенное также ранее в
        конструкторе с параметрами; _RecipeOut и _RecipeIn - ссылки на перемещаемые контейнеры с технологическими
        картами для ЦЗО и ЦЗП соответственно. **/

        void SetCurrency(const Currency& _cur);
        /** Метод устанавливает основную валюту проекта **/

        void Set_progress_shell(clsProgress_shell<type_progress>* val);
        /** Функция присваивает указателю pshell адрес объекта val **/

        /** Расчетные методы **/

        bool Calculate();
        /** Метод рассчитывает объем, удельную и полную себестоимость ресурсов, поступающих на склад и отгружаемых
        со склада. Если расчёты завершились удачно (метод вернул true), в массивах Purchase и Shipment будут
        содержаться расчётные данные: поступающих на склад и отгружаемых со склада соответственно. **/

        /** Get - методы **/

        TLack GetLack(); /** Метод возвращает размер дефицита и имя ресурса, где он обнаружен **/

        size_t Counts(count_choise _count); /** Метод возвращает число ресурсов выбранного подразделения.
        Параметры: _count - флаг выбранного подразделения: inStock - на входе в ЦЗП, midStock - в ЦУР,
        outStock - на выходе из ЦЗО. **/

        strNameMeas* GetNameMeas(count_choise _count); /** Метод возвращает указатель на вновь создаваемый массив
        типа strNameMeas с названием ресурсов и единицами измерения для выбранного подразделения. Параметры: _count
        - флаг выбранного подразделения: inStock - на входе в ЦЗП, midStock - в ЦУР, outStock - на выходе из ЦЗО.
        Метод удобен тогда, когда есть необходимость корректировки (например, сокращения длинных названий при
        выводе списка ресурсов в отчет или на экран. **/

        const strNameMeas* GetNameMeas(); /** Метод возвращает const- указатель на внутренний массив типа
        strNameMeas с названием ресурсов и единицами измерения. Общее число элементов массива равно
        clsCostCenterStorage::Counts(inStock), включая отгружаемые со склада и потребляемые складом ресурсы.
        Из них ресурсы под номерами от нуля до clsCostCenterStorage::Counts(outStock)-1 поступают и
        отгружаются со склада, ресурсы под номерами от clsCostCenterStorage::Counts(outStock) до
        clsCostCenterStorage::Counts(midStock)-1 потребляются на выходе со склада (в ЦЗО), а ресурсы под
        номерами от clsCostCenterStorage::Counts(midStock) до clsCostCenterStorage::Counts(inStock)-1
        потребляются на входе склада (в ЦЗП). **/

        const strItem* GetShip() const; /** Метод возвращает const- указатель на внутренний массив типа strItem
        с объемами, удельной и полной себестоимостью ресурсов, отгружаемых со склада. Общее число элементов
        массива равно clsCostCenterStorage::Counts(outStock)*clsCostCenterStorage::ProjectCount(). **/

        const strItem* GetPure() const; /** Метод возвращает const- указатель на внутренний массив типа strItem
        с объемами, удельной и полной себестоимостью ресурсов, поступающих на склад. Общее число элементов
        массива равно clsCostCenterStorage::Counts(inStock)*clsCostCenterStorage::ProjectCount(). **/

        strItem* GetInRAC() const; /** Метод возвращает указатель на вновь создаваемый массив типа strItem с
        объемами, удельной и полной себестоимостью ресурсов, поступаемых из ЦЗП (PCC) в ЦУР (RAC). Общее число
        элементов массива равно clsCostCenterStorage::Counts(MidStock)*clsCostCenterStorage::ProjectCount().
        ВНИМАНИЕ!!! После использования полученного массива, во избежание утечки памяти он требует явного
        удаления с помощью delete[]. **/

        strItem* GetOutRAC() const; /** Метод возвращает указатель на вновь создаваемый массив типа strItem с
        объемами, удельной и полной себестоимостью ресурсов, поступаемых из ЦУР (RAC) в ЦЗО (SCC). Общее число
        элементов массива равно clsCostCenterStorage::Counts(MidStock)*clsCostCenterStorage::ProjectCount().
        ВНИМАНИЕ!!! После использования полученного массива, во избежание утечки памяти он требует явного
        удаления с помощью delete[]. **/

        /** Export - методы **/

        bool Export_Data(string filename, count_choise _dep, ChoiseData _arr, \
            const ReportData& flg) const;
        /** Метод записывает массив поставок, остатков или отгрузок ЦЗП, ЦУР или ЦЗО в csv-файл с именем filename.
        Параметры: _dep - флаг выбора подразделения: "inStock" - ЦЗП, "midStock" - ЦУР, "outStock" - ЦЗО; _arr
        - выбор данных: "purchase" - поставки, "balance" - остатки/незавершенное производство, "shipment" - отгрузки;
        flg - тип выводимой в файл информации: volume - в натуральном, value - в стоимостном, price - в ценовом
        измерении. В качестве разделителя между полями используется символ _ch по умолчанию (';'). **/

        /** Удаляемые методы: не используются **/

        const string& Name(size_t) = delete;
        const string& Measure(size_t) = delete;
        decimal* GetShipPrice() = delete;
        const strItem* GetDataItem(const ChoiseData, size_t, size_t) = delete;
        void SetCount(const size_t) = delete;
        void SetName(size_t, const string&) = delete;
        void SetMeasure(size_t, const string&) = delete;
        bool SetDataItem(const ChoiseData, const strItem, size_t, size_t) = delete;
        bool SetSKU(const string&, const string&, PurchaseCalc, decimal, bool, strItem, strItem) = delete;
        bool SetData(const clsSKU&) = delete;
        bool SetData(clsSKU&&) = delete;
        void EraseSKU(size_t) = delete;
        void View() = delete;

    protected:                          // Скрываем методы от непреднамеренного использования,
        using clsStorage::Size;         // которые используются в других функциях класса-потомка
        using clsStorage::GetNameMeas;
        using clsStorage::GetShip;
        using clsStorage::GetPure;
        using clsStorage::SetCurrency;
        using clsStorage::SetPurchase;
        using clsStorage::SetPurPrice;
        using clsStorage::CheckPurchase;
        using clsStorage::SetStorage;
//        using clsStorage::Calculate;  // Видимость изменяется для всех перегруженных методов
        using clsStorage::Calculate_future;
        using clsStorage::Calculate_thread;
        using clsStorage::StF;
        using clsStorage::RfF;
};

#endif // CLSCOSTCENTERSTORAGE_H
