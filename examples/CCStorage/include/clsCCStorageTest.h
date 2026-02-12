#ifndef CLSCCSTORAGETEST_H
#define CLSCCSTORAGETEST_H

#include <vector>
#include <string>               // Работа со строками
#include <filesystem>           // Работа с файлами и диреткориями
#include <regex>                // Работа с регулярнями выражениями
#include <io.h>                 // Поиск файлов по маске в Windoes

#include "LongReal_module.h"    // Длинная арифметика
#include "baseproject_module.h" // Подключаем базовый класс clsBaseProject
#include "warehouse_module.h"   // Подключаем класс склада clsStorage
#include "manufact_module.h"    // Подключаем класс производства clsManufactory
#include "common_values.hpp"    // Некоторые функции
#include "Impex_module.h"       // Импорт исходных данных из файлов
#include "pathes.h"             // Пути к файлам отчета, входных и выходных данных

#define filename_shipment "ship.csv"            // Имя файла с отгрузками
#define filename_purprice "pur_price.csv"       // Имя файла с ценами поставок
#define filename_recipe_in "recipe_in"          // Префикс для файлов с техкартами ЦЗП
#define filename_recipe_out "recipe_out"        // Префикс для файлов с техкартами ЦЗО
#define msks "_\\d{1,3}\\.csv"                  // Маска regex для поиска файлов техкарт с помощью filesystem
#define filename_export_purprice "export_pur_price.csv" // Имя файла экспорта с ценами поставок
#define filename_export_shipment "export_ship.csv"      // Имя файла экспорта с отгрузками

namespace fs = std::filesystem; // Создаем короткий алиас

/****************************************************************************************************/
/**                                 Класс clsCCStorageTest                                         **/
/****************************************************************************************************/

class clsCCStorageTest {
/** Класс, формирующий тестовые данные для класса clsCostCentreStorage **/
    private:
        size_t PrCount;         // Количество периодов проекта
        Currency cur;           // Валюта проекта
        AccountingMethod ac;    // Принцип учёта запасов
        size_t ProdCount;       // Количество номенклатурных позиций, отгружаемых со склада
        size_t ResForOutCount;  // Количество номенклатурных позиций на входе в ЦЗО (SCC)
        size_t RMCount;         // Количество номенклатурных позиций на входе в ЦЗП (PCC)

        strNameMeas* ResNames;  // Указатель на массив с перечнем номенклатурных позиций и их ед. измерения
        strItem* Purchase;      // Указатель на массив поступающих на склад ресурсов
        strItem* Shipment;      // Указатель на массив отгружаемых со склада ресурсов

        vector<clsRecipeItem> Out_Recipe;   // Контейнер с тех.картами для ЦЗО
        vector<clsRecipeItem> In_Recipe;    // Контейнер с тех.картами для ЦЗП

        bool ImportSingleArray(const string _filename, const char _ch, size_t hcols, size_t hrows,\
            ReportData flg, strItem* &_data, strNameMeas* &_names, size_t& ColCount, size_t& RowCount);
        /** Метод читает информацию из файла с именем filename и разделителями между полями _ch и заполняет
        поля: RowCount - число номенклатурных позиций (ресурсов или продуктов), ColCount - число периодов
        проекта, _names - ссылка на указатель на массив с наименованиями номенклатурных позиций и единиц их
        измерения, _data - ссылка на указатель на формируемый массив, flg - флаг, определяющий тип импортируемых
        данных: "volume" - объемы в натуральном выражении, "price" - цены, "value" - стоимость; hcols и hrows
        - количество столбцов и строк с заголовками, содержащие названия ресурсов и номера периодов проекта. **/

        bool Import_Recipes(const string _prefixname, const char _ch, size_t hcols, size_t hrows,\
            vector<clsRecipeItem>& _Recipe, size_t _Count);
        /** Метод читает информацию из файлов с именами, содержащими префикс имени рецептуры/ технологической карты
        _prefixname (в данном конкретном примере строка с префиксом определяется макросом filename_recipe_in для
        техкарт ЦЗП и префиксом filename_recipe_out для тех.карт ЦЗО). Обрабатываются все файлы, удовлетворяющие
        маске (в примере строка с регулярным выражением определяется макросом msks) и лежащие в одной папке. Метод
        заполняет контейнер рецептур _Recipe размером _Count. Параметры: _prefixname - префикс имен файлов тех.карт,
        _ch - разделитель, используемый в файлах типа CSV, hcols - количество столбцов с заголовками, hrows -
        количество строк с заголовками в файлах, _Recipe - выходной контейнер, _Count - размер этого контейнера. **/

    public:
        clsCCStorageTest();
        clsCCStorageTest(const clsCCStorageTest&) = delete;
        clsCCStorageTest(clsCCStorageTest&&) = delete;
        ~clsCCStorageTest();

        bool Import_Data(Currency _cur, AccountingMethod _ac);
        /** Метод читает информацию из файлов с исходными данными (имена файлов в данном примере определяются макросами
        filename_shipment, filename_purprice, filename_recipe_in, filename_recipe_out и маской msks. Метод также вводит
        основные параметры: _cur - валюту проекта и _ac - принцип учета запасов. **/

        /** Get - методы **/

        size_t GetProjectCount() const; /** Возвращает число периодов проекта **/
        Currency GetCurrency() const;   /** Возвращает валюту проекта **/
        AccountingMethod GetAccounting() const; /** Возвращает принцип учёта запасов **/
        size_t GetOutCount() const;     /** Возвращает число отгружаемых со склада номенклатурных позиций **/
        size_t GetMidCount() const;     /** Возвращает число номенклатурных позиций на входе в ЦЗО (SCC) **/
        size_t GetInCount() const;      /** Возвращает число номенклатурных позиций поступающих на склад **/

        strNameMeas* GetNames();
        /** Возвращает указатель на внутренний массив с перечнем номенклатурных позиций и их
        ед. измерения **/

        strItem* GetPurch();
        /** Возвращает указатель на внутренний массив поставленных на склад ресурсов **/

        strItem* GetShip();
        /** Возвращает указатель на внутренний массив отгружаемых со склада ресурсов **/

        vector<clsRecipeItem> GetInRecipe();
        /** Возвращает контейнер с тех.картами для ЦЗП путем перемещения **/

        vector<clsRecipeItem> GetOutRecipe();
        /** Возвращает контейнер с тех.картами для ЦЗО путем перемещения **/

        /** Заголовки таблиц в отчетах **/
        const string rTableName  = "Наименование";
        const string rTableMeas  = "Ед.измерения";
        const string rByVolume   = "В натуральном измерении";
        const string rByPrice    = "В удельном стоимостном измерении";
        const string rByValue    = "В стоимостном измерении";

    protected:

};  // clsCostCenterStorage

#endif // CLSCCSTORAGETEST_H
