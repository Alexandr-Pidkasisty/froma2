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

bool clsCCStorageTest::ImportSingleArray(const string _filename, const char _ch, size_t hcols, size_t hrows,\
    ReportData flg, strItem* &_data, strNameMeas* &_names, size_t& ColCount, size_t& RowCount) {
/** Метод читает информацию из файла с именем filename и разделителями между полями ch и заполняет поля: RowCount
- число номенклатурных позиций (ресурсов или продуктов), ColCount - число периодов проекта, _names - ссылка на
указатель на массив с наименованиями номенклатурных позиций и единиц их измерения, _data - ссылка на указатель на
формируемый массив, flg - флаг, определяющий тип импортируемых данных: "volume" - объемы в натуральном выражении,
"price" - цены, "value" - стоимость; hcols и hrows - количество столбцов и строк с заголовками, содержащие названия
ресурсов и номера периодов проекта. **/
    ifstream input(_filename);                  // Связываем файл с потоком на чтение
    const char ch = _ch;                        // Выбираем разделитель
    clsImpex* Data = new clsImpex(input, ch);   // Создаем класс для импорта и импортируем данные из файла
    input.close();                              // Закрываем файл с исходными данными
    if(Data->is_Empty()) {                      // Если вектор не создан, то
        delete Data;                            // удаляем объект
        return false;                           // и выходим с false
    };
    ColCount = Data->GetColCount()-hcols;       // Получаем число периодов проекта
    RowCount = Data->GetRowCount()-hrows;       // Получаем число номенклатурных позиций (ресурсов или продуктов)
    strItem* tmpdata;                           // Временная переменная-указатель на массив с данными
    strNameMeas* tmpnames;                      // Временная переменная-указатель на массив с названиями
    size_t maxRow = RowCount-sOne+hrows;        // Последняя строка
    size_t maxCol = ColCount-sOne+hcols;        // Последний столбец
    tmpdata = Data->GetstrItem(hrows, maxRow, hcols, maxCol, flg);      // Получаем указатель на массив с данными
    tmpnames = Data->GetNames(hrows, maxRow, hcols-sTwo, hcols-sOne);   // Получаем указатель на массив с названиями
    delete Data;                                // Удаляем объект для импорта
    std::swap(_data, tmpdata);                  // Перекидываем ссылку на целевой указатель
    std::swap(_names, tmpnames);                // Перекидываем ссылку на целевой указатель
    if(tmpdata) delete[] tmpdata;               // Удаляем вспомогательный массив, если он есть
    if(tmpnames) delete[] tmpnames;             // Удаляем вспомогательный массив, если он есть
    return true;
}   // clsCCStorageTest::ImportSingleArray

bool clsCCStorageTest::Import_Recipes(const string _prefixname, const char _ch, size_t hcols, size_t hrows,\
    vector<clsRecipeItem>& _Recipe, size_t _Count) {
/** Метод читает информацию из файлов с именами, содержащими префикс имени рецептуры/ технологической карты
_prefixname (в данном конкретном примере строка с префиксом определяется макросом filename_recipe_in для
техкарт ЦЗП и префиксом filename_recipe_out для тех.карт ЦЗО). Обрабатываются все файлы, удовлетворяющие
маске (в примере строка с регулярным выражением определяется макросом msks) и лежащие в одной папке. Метод
заполняет контейнер рецептур _Recipe размером _Count. Параметры: _prefixname - префикс имен файлов тех.карт,
_ch - разделитель, используемый в файлах типа CSV, hcols - количество столбцов с заголовками, hrows -
количество строк с заголовками в файлах, _Recipe - выходной контейнер, _Count - размер этого контейнера. **/
    vector<clsRecipeItem> tmpRecipe;                    // Вспомогательный вектор
    tmpRecipe.reserve(_Count);                          // Резервируем память для элементов вектора
    ifstream rec;                                       // Поток для чтения из файла
    clsImpex* Data = new(nothrow) clsImpex();           // Создаем экземпляр класса для импорта
    regex fmask(_prefixname + msks);                    // Маска поиска файлов

    string pth = V_DIR_INPUTDATA;
    pth.resize(pth.length()-1);

    const fs::path indata{pth};             // Папка с файлами рецептур
    if(!fs::exists(indata)) return false;               // Проверяем существование папки
    for(auto &p : fs::directory_iterator(indata)) {
        if(!fs::is_regular_file(p.status())) continue;  // Проверяем, что анйденный файл регулярный (не папка, не ссылка)
        string name((p.path().filename()).string());    // Получаем имя файла
        if(regex_match(name, fmask)) {                  // Проверяем имя файла на совпадение с маской
            rec.open(p.path());                         // Связываем поток с файлом
            if(!Data->Import(rec, _ch)) {               // Импортируем данные из файла. Если импорт не удался,
                rec.close();                            // то закрываем файл;
                delete Data;                            // удаляем экземпляр класса для импорта
                return false;                           // и выходим с false
            };
            rec.close();                                // Закрываем файл
            strNameMeas* _names = Data->GetNames(sZero, sZero, sZero, sOne);    // Читаем название и ед.измерения продукта
            size_t _duration = Data->GetColCount()-hcols;   // Получаем длительность производственного цикла
            size_t _rcount = Data->GetRowCount()-hrows;     // Получаем количество позиций сырья в рецептуре
            size_t maxRow = _rcount-sOne+hrows;             // Последняя строка
            size_t maxCol = _duration-sOne+hcols;           // Последний столбец
            strNameMeas* _rnames = Data->GetNames(hrows, maxRow, hcols-sTwo, hcols-sOne);   // Моссив с наименованиями ресурсов
            decimal* _recipeitem = Data->GetDecimal(hrows, maxRow, hcols, maxCol);          // Получаем тех.карту ресурса
            tmpRecipe.emplace_back(_names->name, _names->measure, _duration, _rcount, _rnames, _recipeitem); // Создаем объект "рецептура" в векторе
            delete[] _names;                                // Удаляем вспомогательный массив
            delete[] _rnames;                               // Удаляем вспомогательный массив
            delete[] _recipeitem;                           // Удаляем вспомогательный массив
            Data->reset();                                  // Сбрасываем состояние объекта до дефолтного
        }
    }   // for(auto &p...)
    delete Data;
    _Recipe = move(tmpRecipe);                          // Перемещаем вспомогательный массив в основной
    return true;
}   // clsCCStorageTest::Import_Recipes

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
    if(!Import_Recipes(filename_recipe_in, ';', sTwo, sOne, In_Recipe, ResForOutCount)) {
        return false;   // Вводим Инфорацию об тех.картах ЦЗП. Если не введено, выходим из программы с false
    }
    ResForOutCount = In_Recipe.size();  // Получаем число продуктов на выходе ЦЗП
    /** Читаем тех.карты для ЦЗО **/
    if(!Import_Recipes(filename_recipe_out, ';', sTwo, sOne, Out_Recipe, ProdCount)) {
        return false;   // Вводим Инфорацию об тех.картах ЦЗП. Если не введено, выходим из программы с false
    }
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

