#include "clsCompany.h"

//#define DEBUG_StF_RfF   // Макрос вывода отладочной информации. Раскомментировать для отладки

clsCompany::clsCompany() : clsAgregate() {
/** Конструктор по умолчанию **/
    InCount = MidCount = OutCount = sZero;                      // Инициализируем количество позиций
    CostChain.reserve(3);                                       // Резервируем память на 3 подразделения
    CostChain.emplace_back(clsManufactory());                   // Добавляем производство (ЦЗП)
    CostChain.emplace_back(clsStorage());                       // Добавляем склад (ЦУР)
    CostChain.emplace_back(clsManufactory());                   // Добавляем производство (ЦЗО)
}   // Default Ctor

clsCompany::clsCompany(const size_t _PrCount, const Currency _cur, const AccountingMethod _ac, const size_t ProdCount,\
    const size_t _MidCount, const size_t _RMCount, const strNameMeas _RMNames[], const clsRecipeItem _RecipeOut[],\
    const clsRecipeItem _RecipeIn[]) {
/** Конструктор с параметрами. Параметры: _PrCount - количество периодов проекта, _cur - валюта проекта, _ac - принцип
учета запасов, ProdCount - количество номенклатурных позиций отгружаемых из ЦЗО продуктов, _MidCount - количество
номенклатурных позиций, хранимых в ЦУР, _RMCount - количество номенклатурных позиций, поставляемых в ЦЗП, _RMNames -
указатель на массив с наименованиями ресурсов и ед. их измерения, _RecipeOut - указатель на массив с рецептурами для
ЦЗО, _RecipeIn - указатель на массив с рецептурами для ЦЗП. **/
    PrCount =_PrCount;                          // Инициализируем длительность проекта
    InCount = _RMCount;                         // Инициализируем количество позиций на входе ЦЗП
    MidCount = _MidCount;                       // Инициализируем количество позиций на выходе ЦЗП, ЦУР и входе ЦЗО
    sh_size = OutCount = ProdCount;             // Инициализируем количество позиций на выходе ЦЗО
    CostChain.reserve(3);                       // Резервируем память на 3 подразделения
    CostChain.emplace_back(clsManufactory(_PrCount, InCount, _RMNames, MidCount, _cur, _RecipeIn));    // Добавляем производство (ЦЗП)
    CostChain.emplace_back(clsStorage(_PrCount, _cur, _ac, MidCount, _RMNames));                       // Добавляем склад (ЦУР)
    CostChain.emplace_back(clsManufactory(_PrCount, MidCount, _RMNames, OutCount, _cur, _RecipeOut));  // Добавляем производство (ЦЗО)
    Purchase = nullptr;                         // Устанавливаем пустой указатель на закупки
    Shipment = nullptr;                         // Устанавливаем пустой указатель на отгрузки
}   // Ctor with parametrs

void clsCompany::swap(clsCompany& other) noexcept {
/** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
    clsAgregate::swap(other);               // Вызываем функцию родительского класса
    std::swap(InCount, other.InCount);      // Обмениваем значения
    std::swap(MidCount, other.MidCount);    // Обмениваем значения
    std::swap(OutCount, other.OutCount);    // Обмениваем значения
}   // clsCompany::swap

clsCompany::clsCompany(const clsCompany& other) : clsAgregate(other) {
/** Конструктор копирования **/
    InCount = other.InCount;                // Копируем число позиций на входе ЦЗП
    MidCount = other.MidCount;              // Копируем число позиций на ЦУР и входе ЦЗО
    OutCount = other.OutCount;              // Копируем число позиций на выходе ЦЗО
}   //Copy Ctor

clsCompany::clsCompany(clsCompany&& other) : clsCompany() {
/** Конструктор перемещения **/
    swap(other);
}   // Move Ctor

clsCompany& clsCompany::operator=(const clsCompany& rhs) {
/** Перегрузка оператора присваивания копированием. Реализовано в идеоме
КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/
    if (this == &rhs) return *this;         // Если объекты идентичны, то ничего не делаем
    clsCompany tmp(rhs);    // Вызываем конструктор копирования и получаем копию rhs в переменной tmp
    swap(tmp);                              // Обмениваемся содержанием с tmp
    return *this;
}   // Copy operator=

clsCompany& clsCompany::operator=(clsCompany&& other) {
/** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
(move-and-swap idiom) **/
    if(this == &other) return *this;        // Если объекты идентичны, то ничего не делаем
    clsCompany tmp(move(other));            // Вызываем конструктор и перемещаем объект rhs в переменную tmp
    this->swap(tmp);                        // Обмениваемся содержанием с tmp
    return *this;
}   // Move operator=

//clsCompany::~clsCompany() {
///** Перегрузка виртуального деструктора **/
//    vector<T_agregate>().swap(CostChain);
//} // Dtor

void clsCompany::reportstream(ostream& os) const {
/** Метод выводит отчет в поток os. Используется невиртуальным методом clsBaseProject::Report **/
    clsAgregate::reportstream(os);                   // Вывод базовой информации
    os << "Число ресурсов на входе склада (включая отгружаемые и потребляемые)\t " << InCount << "\n";
    os << "Число ресурсов на хранении\t\t\t\t\t\t "         << MidCount << "\n";
    os << "Число отгружаемых со склада ресурсов\t\t\t\t\t " << OutCount << "\n" << "\n";
    clsTextField name(40);  // Установка формата вывода имен с помощью класса clsTextField
    clsTextField meas(10);  // Установка формата вывода единиц измерения
    strNameMeas *Resources = GetNameMeas(purchase, inStock);  // Получаем указатель на массив с ресурсами
    nmRePrint::PrintHeader1(os, nmRePrint::smblcunt+nmRePrint::uThree, \
    "Ресурсы на складе (включая отгружаемые и потребляемые)");
    nmRePrint::PrintHeader2(os, nmRePrint::smblcunt+nmRePrint::uThree, \
    "Ресурсы поступающие на склад и отгружаемые со склада");
    for(size_t i{}; i<InCount; i++) {
        if(i == OutCount)
            nmRePrint::PrintHeader2(os, nmRePrint::smblcunt+nmRePrint::uThree, \
            "Ресурсы, потребляемые в ЦЗО");
        if(i == MidCount)
            nmRePrint::PrintHeader2(os, nmRePrint::smblcunt+nmRePrint::uThree, \
            "Ресурсы, потребляемые в ЦЗП");
        os << name << (Resources+i)->name;
        os << meas << (Resources+i)->measure << "\n";
    }
    delete[] Resources;
    os << "\n";
    streambuf* coutbuf = cout.rdbuf();      // Запомним старый буфер в переменной coutbuf
    cout.rdbuf(os.rdbuf());                 // Перенаправляем поток в os: теперь все будет в os

    auto caller_view = [](auto& obj) {                  // Функция-посетитель
        using T = std::decay_t<decltype(obj)>;          // Преобразование к базовому типу и определения алиаса Т
        if constexpr (std::is_same_v<T, clsStorage>)    // Если базовый тип clsStorage,
            obj.ViewSettings();                         // вызываем соответствующую функцию
        else if constexpr (std::is_same_v<T, clsManufactory>)       // Если базовый тип clsManufactory,
            obj.ViewProjectParametrs();                             // вызываем соответствующую функцию
    };
    std::visit(caller_view, CostChain[1]);  // вызываем функцию-посетителя для ЦУР
    cout.rdbuf(coutbuf);                    // Восстанавливаем вывод на экран
    os << "\n";
}   // clsCompany::reportstream

bool clsCompany::StF(ofstream &_outF){
/** Метод имплементации записи в файловую переменную текущего экземпляра класса (запись в файл, метод
сериализации). Параметры: &_outF - экземпляр класса ofstream для записи данных **/
    #ifdef DEBUG_StF_RfF                            // Макрос вывода отладочной информации. Работает, если определен DEBUG_StF_RfF
        long bpos = _outF.tellp();                  // Определяем позицию в начале файла
        cout << "clsCompany::StF begin bpos= " << bpos << endl;
    #endif
    if(!clsAgregate::StF(_outF)) return false;      // Сохраняем в файл данные родительского класса
    if(!SEF(_outF, InCount)) return false;          // Сохраняем в файл количество позиций на входе ЦЗП
    if(!SEF(_outF, MidCount)) return false;         // Сохраняем в файл количество позиций в ЦУР
    if(!SEF(_outF, OutCount)) return false;         // Сохраняем в файл количество позиций на выходе ЦЗО
    #ifdef DEBUG_StF_RfF
        long epos = _outF.tellp();                    // Определяем позицию в конце файла
        cout << "clsCompany::StF end epos= " << epos << endl;
    #endif // DEBUG_StF_RfF
    return true;
}   // clsCompany::StF

bool clsCompany::RfF(ifstream &_inF) {
/** Метод имплементации чтения из файловой переменной экземпляра класса (чтение из файла, метод
десериализации). Параметры: &_inF - ссылка на экземпляр класса ifstream для чтения данных. **/
    #ifdef DEBUG_StF_RfF                            // Макрос вывода отладочной информации. Работает, если определен DEBUG_StF_RfF
        long bpos = _inF.tellg();                   // Определяем позицию в начале файла
        cout << "clsCompany::RfF begin bpos= " << bpos << endl;
    #endif
    if(!clsAgregate::RfF(_inF)) return false;       // Читаем из файла данные родительского класса
    if(!DSF(_inF, InCount)) return false;           // Читаем из файла количество позиций на входе ЦЗП
    if(!DSF(_inF, MidCount)) return false;          // Читаем из файла количество позиций в ЦУР
    if(!DSF(_inF, OutCount)) return false;          // Читаем из файла количество позиций на выходе ЦЗО
    #ifdef DEBUG_StF_RfF
        long epos = _inF.tellg();                   // Определяем позицию в конце файла
        cout << "clsCompany::RfF end epos= " << epos << endl;
    #endif // DEBUG_StF_RfF
    return true;
}   // clsCompany::RfF

# undef DEBUG_StF_RfF   // отменить макрос информации об инструменте DEBUG_StF_RfF
