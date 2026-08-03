#include "clsCCStorageTest.h"

/****************************************************************************************************/
/**                                 Класс clsCCStorageTest                                         **/
/****************************************************************************************************/

clsCCStorageTest::clsCCStorageTest() {
/** Конструктор без параметром **/
    PrCount = sZero;
    cur = RUR;
    ac = AVG;
    ProdCount = sZero;
    ResForOutCount = sZero;
    RMCount = sZero;
    ResNames = nullptr;
    Purchase = nullptr;
    Shipment = nullptr;
}   //Ctor

clsCCStorageTest::~clsCCStorageTest() {
    if(ResNames) delete[] ResNames;
    if(Purchase) delete[] Purchase;
    if(Shipment) delete[] Shipment;
}   // Dtor

bool clsCCStorageTest::Import_Data(Currency _cur, AccountingMethod _ac){
/** Метод читает информацию из файлов с исходными данными (имена файлов в данном примере определяются макросами
filename_shipment, filename_purprice, filename_recipe_in, filename_recipe_out и маской msks. Метод также вводит
основные параметры: _cur - валюту проекта и _ac - принцип учета запасов. **/
    string InputShipment, InputPurprice, InputRecipeIn, InputRecipeOut;     // Переменные для имен файлов
    (InputShipment.assign(V_DIR_INPUTDATA)).append(filename_shipment);      // Путь и имя файла отгрузок
    (InputPurprice.assign(V_DIR_INPUTDATA)).append(filename_purprice);      // Путь и имя файла поставок
    cur = _cur;
    ac = _ac;
    /** Читаем данные с объемами отгрузок со склада **/
    if(!ImportSingleArray(InputShipment, ';', sTwo, sOne, volume, Shipment, ResNames, PrCount, ProdCount)) {
        return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
    }
    /** Читаем данные с ценами поставок на склад **/
    if(!ImportSingleArray(InputPurprice, ';', sTwo, sOne, price, Purchase, ResNames, PrCount, RMCount)) {
        return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
    }
    /** Читаем тех.карты для ЦЗП **/
//    if(!Import_Recipes(filename_recipe_in, ';', sTwo, sOne, In_Recipe, ResForOutCount)) {
//        return false;   // Вводим Инфорацию об тех.картах ЦЗП. Если не введено, выходим из программы с false
//    }
    if(!Import_Recipes(filename_recipe_in, ';', sTwo, sOne, In_Recipe, ResForOutCount, msks, V_DIR_INPUTDATA))
        return false;   // Вводим Инфорацию о рецептурах. Если не введено, выходим из программы с false
    ResForOutCount = In_Recipe.size();  // Получаем число продуктов на выходе ЦЗП
    /** Читаем тех.карты для ЦЗО **/
//    if(!Import_Recipes(filename_recipe_out, ';', sTwo, sOne, Out_Recipe, ProdCount)) {
//        return false;   // Вводим Инфорацию об тех.картах ЦЗП. Если не введено, выходим из программы с false
//    }
    if(!Import_Recipes(filename_recipe_out, ';', sTwo, sOne, Out_Recipe, ProdCount, msks, V_DIR_INPUTDATA))
        return false;   // Вводим Инфорацию о рецептурах. Если не введено, выходим из программы с false
    return true;
}   // clsCCStorageTest::Import_Data

/** Get - методы **/

size_t clsCCStorageTest::GetProjectCount() const {
/** Возвращает число периодов проекта **/
    return PrCount;
}   // clsCCStorageTest::GetProjectCount

Currency clsCCStorageTest::GetCurrency() const {
/** Возвращает валюту проекта **/
    return cur;
}   // clsCCStorageTest::GetCurrency

AccountingMethod clsCCStorageTest::GetAccounting() const {
/** Возвращает принцип учёта запасов **/
    return ac;
}   // clsCCStorageTest::GetAccounting

size_t clsCCStorageTest::GetOutCount() const {
/** Возвращает число отгружаемых со склада номенклатурных позиций **/
    return ProdCount;
}   // clsCCStorageTest::GetOutCount

size_t clsCCStorageTest::GetMidCount() const {
/** Возвращает число номенклатурных позиций на входе в ЦЗО (SCC) **/
    return ResForOutCount;
}   // clsCCStorageTest::GetMidCount

size_t clsCCStorageTest::GetInCount() const {
/** Возвращает число номенклатурных позиций поступающих на склад **/
    return RMCount;
}   // clsCCStorageTest::GetInCount

strNameMeas* clsCCStorageTest::GetNames() {
/** Возвращает указатель на внутренний массив с перечнем номенклатурных позиций и их ед. измерения **/
    return ResNames;
}   // clsCCStorageTest::GetNames

strItem* clsCCStorageTest::GetPurch() {
/** Возвращает указатель на внутренний массив поставленных на склад ресурсов **/
    return Purchase;
}   // clsCCStorageTest::GetPurch

strItem* clsCCStorageTest::GetShip() {
/** Возвращает указатель на внутренний массив отгружаемых со склада ресурсов **/
    return Shipment;
}   // clsCCStorageTest::GetShip

vector<clsRecipeItem> clsCCStorageTest::GetInRecipe() {
/** Возвращает контейнер с тех.картами для ЦЗП путем перемещения **/
    return move(In_Recipe);
}   // clsCCStorageTest::GetInRecipe

vector<clsRecipeItem> clsCCStorageTest::GetOutRecipe() {
/** Возвращает контейнер с тех.картами для ЦЗО путем перемещения **/
    return move(Out_Recipe);
}   // clsCCStorageTest::GetOutRecipe

