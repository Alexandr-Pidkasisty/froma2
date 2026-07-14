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
#include "agregate_module.h"

//#define DEBUG_StF_RfF   // Макрос вывода отладочной информации. Раскомментировать для отладки

/***************************************************************************************************************/
/**                                                                                                           **/
/**                                             clsAgregate                                                   **/
/**                                                                                                           **/
/***************************************************************************************************************/

clsAgregate::clsAgregate() {
/** Конструктор по умолчанию. Чтобы запретить создание экземпляров данного класса, конструктор перенесен
в секцию protected. В наследуемых классах конструкторы необходимо объявлять в секции public. **/
    PrCount = sh_size = sZero;  // Инициализируем количество позиций
    Purchase = nullptr;         // Устанавливаем пустой указатель на закупки
    Shipment = nullptr;         // Устанавливаем пустой указатель на отгрузки
}   //ctor

void clsAgregate::swap(clsAgregate& other) noexcept {
/** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
    clsBaseProject::swap(other);            // Вызываем функцию родительского класса
    std::swap(PrCount, other.PrCount);      // Обмениваем значения
    std::swap(sh_size, other.sh_size);      // Обмениваем значения
    CostChain.swap(other.CostChain);        // Обмениваем вектора
    std::swap(Purchase, other.Purchase);    // Обмениваем указатели
    std::swap(Shipment, other.Shipment);    // Обмениваем указатели
}   // clsCostCenterStorage_1::swap

clsAgregate::clsAgregate(const clsAgregate& other) : clsBaseProject(other) {
/** Конструктор копирования **/
    PrCount = other.PrCount;                // Копируем длительность проекта
    sh_size = other.sh_size;                // Копируем число строк массива Shipment
    CostChain = other.CostChain;            // Копируем контейнеры
    if(other.Purchase) {                    // Если массив other.Purchase существует,
        size_t tmpcount =                   // то вычисляем размер массива
            std::visit(Getter_exporting_count(purchase), *(CostChain.begin()))*PrCount;
        Purchase = new(nothrow) strItem[tmpcount];          // выделяем память,
        if(Purchase)                                        // если память выделена,
            var_cpy(Purchase, other.Purchase, tmpcount);    // то копируем массивы;
    } else Purchase = nullptr;              // если other.Purchase нет, то присваиваем nullptr
    if(other.Shipment) {                    // Если массив other.Shipment существует, то
        size_t tmpcount = sh_size*PrCount;  // вычисляем размер массива
        Shipment = new(nothrow) strItem[tmpcount];          // выделяем память,
        if(Shipment)                                        // если память выделена,
            var_cpy(Shipment, other.Shipment, tmpcount);    // то копируем массивы;
    } else Shipment = nullptr;              // если other.Shipment нет, то присваиваем nullptr
}   // Copy Ctor

clsAgregate::clsAgregate(clsAgregate&& other) : clsBaseProject() {
/** Конструктор перемещения **/
    PrCount = sh_size = sZero;              // Инициализируем количество позиций
    Purchase = nullptr;                     // Устанавливаем пустой указатель на закупки
    Shipment = nullptr;                     // Устанавливаем пустой указатель на отгрузки
    swap(other);
}   // Move Ctor

clsAgregate::~clsAgregate() {
/** Деструктор **/
    if(Purchase) delete[] Purchase;
    if(Shipment) delete[] Shipment;
}   //Dtor

clsAgregate& clsAgregate::operator=(const clsAgregate& rhs) {
/** Перегрузка оператора присваивания копированием. Реализовано в идиоме
КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/
    if (this == &rhs) return *this;         // Если объекты идентичны, то ничего не делаем
    clsAgregate tmp(rhs);                   // Получаем копию объекта rhs в переменной tmp
    this->swap(tmp);                        // Обмениваемся содержанием с tmp
    return *this;
}   // Copy operator=

clsAgregate& clsAgregate::operator=(clsAgregate&& rhs) {
/** Перегрузка оператора присваивания перемещением. Реализовано в идиоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
(move-and-swap idiom) **/
    if(this == &rhs) return *this;          // Если объекты идентичны, то ничего не делаем
    clsAgregate tmp(move(rhs));             // Перемещаем объект rhs в переменную tmp
    this->swap(tmp);                        // Обмениваемся содержанием с tmp
    return *this;
}   // Move operator=

/** Get - методы **/

size_t clsAgregate::ProjectCount() {
/** Метод возвращает количество периодов проекта **/
    return PrCount;
}

size_t clsAgregate::Counts(const ChoiseData _arr, const size_t _count) {
/** Метод возвращает число ресурсов выбранного подразделения. Параметры: _arr – сепаратор выбора данных:
"purchase" - поставки, "balance" - остатки/незавершенное производство, "shipment" – отгрузки; _count
- индекс выбранного подразделения: 0 - первое подразделение, принимающее ресурсы (элемент контейнера
CostChain с индексом ноль); подразделение, отгружающее готовую продукцию будет иметь самый большой номер
(элемент контейнера CostChain с максимальным индексом). **/
    if(CostChain.size() == sZero) return sZero;     // Если контейнер пуст, то возвращаем ноль
    else return std::visit(Getter_exporting_count(_arr), *(CostChain.begin()+_count));
}   // clsAgregate::Counts

strNameMeas* clsAgregate::GetNameMeas(const ChoiseData _arr, const size_t _count) const {
/** Метод возвращает указатель на новый массив типа strNameMeas с названием ресурсов и единицами измерения.
Число элементов массива равно Counts(_arr, _count). Параметры: _arr - сепаратор выбора данных: "purchase"
- поставки, "balance" - остатки/незавершенное производство, "shipment" - отгрузки; _count - индекс выбранного
подразделения: 0 - первое подразделение, принимающее ресурсы (элемент контейнера CostChain с индексом ноль);
подразделение, отгружающее готовую продукцию будет иметь самый большой номер (элемент контейнера CostChain с
максимальным индексом).**/
    if(CostChain.size() == sZero) return nullptr;   // Если контейнер пуст, то возвращаем nullptr
    else return std::visit(Getter_NameMeas{}, CostChain[_count]);
}   // clsAgregate::GetNameMeas

bool clsAgregate::Export_Data(const string filename, const size_t _dep, const ChoiseData _arr, \
    const ReportData& flg) const {
/** Метод записывает массив поставок, остатков или отгрузок выбранного подразделения в csv-файл с именем filename.
Параметры: _dep - индекс подразделения в цепочке подразделений CostChain; _arr - выбор данных: "purchase" - поставки,
"balance" - остатки/незавершенное производство, "shipment" - отгрузки; flg - тип выводимой в файл информации: volume
- в натуральном, value - в стоимостном, price - в ценовом измерении. В качестве разделителя между полями используется
символ _ch по умолчанию (';'). **/
    if(CostChain.size() == sZero) return false;             // Вылидация параметров
    const size_t NameCount = std::visit(Getter_exporting_count(_arr), CostChain[_dep]); // Размер экспортируемого массива
    strNameMeas* pNames = std::visit(Getter_exporting_names(_arr), CostChain[_dep]);    // Массив с именами
    if(!pNames) return false;                               // Если массив с именами пуст, то выход с false
    strItem* pData;                                         // Вспомогательный указатель
    if((std::holds_alternative<clsManufactory>(CostChain[_dep])) && (_arr == purchase) && (flg !=volume)) {
    // Если хотим взять стоимостные данные о закупках из объекта типа clsManufactory, то надо помнить, что метод
    // clsManufactory::GetRMPurchPlan возвращает данные с нулевыми полями price и value. Поэтому надо брать данные
    // из объекта, предшествующего текущему. Для самого первого объекта эти данные можно взять из массива Purchase,
    // для других объектов - с помощью методов структуры Setter_ship из предыдущего объекта.
        ptrdiff_t index = static_cast<ptrdiff_t>(_dep);     // Вспомогательный индекс
        index--;                                            // Уменьшаем индекс
        if(index < static_cast<ptrdiff_t>(sZero)) {         // Если индекс стал меньше нуля, то берем данные из Purchase
            size_t tmpCount = NameCount*PrCount;            // Временная переменная
            pData = new(nothrow) strItem[tmpCount];         // Выделяем память массиву
            if(!pData) {                                    // Если память не выделена, то
                delete[] pNames;                            // удаляем ранее созданный массив с именами
                return false;                               // и выходим с false
            }
            var_cpy(pData, Purchase, tmpCount);             // Если память выделена, то копируем данные из массива Purchase
        } else {                                                    // Если же индекс положительный, то
            pData = std::visit(Getter_ship{}, CostChain[index]);    // берем данные об отгрузках из предыдущего элемента цепочки
        }
    } else pData = std::visit(Getter_exporting_data{_arr}, CostChain[_dep]);
    /** Создаем объект экспорта/импорта и читаем в него данные **/
    clsImpex* Data = new clsImpex(NameCount, pNames, pData, PrCount, flg);
    delete[] pNames;                                            // Удаляем временный массив с именами
    delete[] pData;                                             // Удаляем временный массив с данными
    if(Data->is_Empty()) {                                      // Если контейнер с данными не создан, то
        return false;                                           // выходим с false
    };
    ofstream output(filename, std::ofstream::trunc);            // Открывем файл на запись
    if(output.is_open()) {                                      // Если файл открыт, то
        Data->csvExport(output);                                // Записываем данные в файл
        output.close();                                         // Закрываем файл
    } else {
        delete Data;
        return false;
    }
    delete Data;                                                // Удаляем объект экспорта/импорта
    return true;
}   // clsAgregate::Export_Data

void clsAgregate::reportstream(ostream& os) const {
/** Метод выводит отчет в поток os. Используется невиртуальным методом clsBaseProject::Report **/
    clsBaseProject::reportstream(os);   // Вывод базовой информации
    os << "Количество периодов проекта:\t\t\t\t\t\t"    << PrCount << "\n";
    os << "Количество подразделений:\t\t\t\t\t\t "      << CostChain.size() << "\n";
}   //  clsAgregate::reportstream

bool clsAgregate::Detail_report(const size_t _dep, const string _depName[], const ChoiseData _arr, const ReportData& flg) const {
/** Функция выводит выбранный отчет. Параметры: _dep - индекс подразделения в цепочке подразделений CostChain;
_depName - указатель на массив с названиями подразделений (может указывать на nullptr; в этом случае вместо названий
подразделений в отчёт будут выведены их номера); _arr - выбор данных: "purchase" - поставки, "balance" - остатки/
незавершенное производство, "shipment" - отгрузки; flg - тип выводимой в файл информации: volume - в натуральном,
value - в стоимостном, price - в ценовом измерении. Отчет выводится в файл с именем, содержащимся в переменной
clsBaseProject::RName. **/
    if(CostChain.size() == sZero) return false;             // Вылидация параметров
    const size_t NameCount = std::visit(Getter_exporting_count(_arr), CostChain[_dep]); // Размер экспортируемого массива
    strNameMeas* pNames = std::visit(Getter_exporting_names(_arr), CostChain[_dep]);    // Массив с именами
    if(!pNames) return false;                               // Если массив с именами пуст, то выход с false
    strItem* pData;                                         // Вспомогательный указатель
    if((std::holds_alternative<clsManufactory>(CostChain[_dep])) && (_arr == purchase) && (flg !=volume)) {
    // Если хотим взять стоимостные данные о закупках из объекта типа clsManufactory, то надо помнить, что метод
    // clsManufactory::GetRMPurchPlan возвращает данные с нулевыми полями price и value. Поэтому надо брать данные
    // из объекта, предшествующего текущему. Для самого первого объекта эти данные можно взять из массива Purchase,
    // для других объектов - с помощью методов структуры Setter_ship из предыдущего объекта.
        ptrdiff_t index = static_cast<ptrdiff_t>(_dep);     // Вспомогательный индекс
        index--;                                            // Уменьшаем индекс
        if(index < static_cast<ptrdiff_t>(sZero)) {         // Если индекс стал меньше нуля, то берем данные из Purchase
            size_t tmpCount = NameCount*PrCount;            // Временная переменная
            pData = new(nothrow) strItem[tmpCount];         // Выделяем память массиву
            if(!pData) {                                    // Если память не выделена, то
                delete[] pNames;                            // удаляем ранее созданный массив с именами
                return false;                               // и выходим с false
            }
            var_cpy(pData, Purchase, tmpCount);             // Если память выделена, то копируем данные из массива Purchase
        } else {                                                    // Если же индекс положительный, то
            pData = std::visit(Getter_ship{}, CostChain[index]);    // берем данные об отгрузках из предыдущего элемента цепочки
        }
    } else pData = std::visit(Getter_exporting_data{_arr}, CostChain[_dep]);

    /** Формируем отчет **/
    string divsn = "";                  // Вспомогательная переменная: название подразделения
    string swork;                       // Вспомогательная переменная: название выводимого массива
    if(_depName)                        // Если массив с наименованиями подразделений существует,
        divsn = *(_depName+_dep);       // то присваиваем divsn имя подразделения,
    else divsn = std::to_string(_dep);  // иначе присваиваем номер подразделения
    divsn += ": ";                      // Добавляем двоеточие и пробел
    if((_arr == purchase) || (_arr == balance))
        if(_arr == purchase) swork = nmAgregate::aPurchase;
        else swork = nmAgregate::aBalance;
    else swork = nmAgregate::aShipment;
    std::ofstream ofs;                                      // Вспомогательная переменная для вывода отчета в файл
    ofs.open(RName, std::ofstream::app);                    // связываем поток с файлом и открываем его на запись
                                                            // в режиме добавления к существующему
    nmRePrint::PrintHeader2(ofs, nmRePrint::smblcunt+nmRePrint::uThree, divsn+swork);   // Выводим заголовок отчета
    nmRePrint::clsRePrint<strItem, strNameMeas>* MyRep = \
        new nmRePrint::clsRePrint<strItem, strNameMeas>(nmAgregate::w1, nmAgregate::w2, nmAgregate::w3);// Настройка
    MyRep->SetHeadings(nmAgregate::rTableName, nmAgregate::rTableMeas, \
        nmAgregate::rByVolume, nmAgregate::rByPrice, nmAgregate::rByValue); // Меняем заголовки таблицы
    auto Set_currency = [](auto& obj) { return obj.GetHomeCurrency(); };    // Лямбда для получения индекса валюты
    MyRep->SetCurrency(static_cast<nmBPTypes::Currency>(std::visit(Set_currency, CostChain[_dep])));// Установка валюты
    MyRep->SetPrecision(nmAgregate::precis);                // Установка количества знаков после запятой
    MyRep->SetStream(ofs);                                  // Устанавливаем вывод отчета в файловый поток
    bool result;                                            // Вспомогательный индикатор
    if(MyRep->SetReport(NameCount, pNames, pData, PrCount, flg)) {          // Вводим данные в отчет
        MyRep->Print(flg);                                  // Выводим отчет, если получилось,
        result = true;                                      // то индикатор true,
    } else result = false;                                  // иначае индикатор false
    ofs.close();                                            // закрываем файл
    if(pNames) delete[] pNames;                             // Удаляем всмомогательный массив
    if(pData) delete[] pData;                               // Удаляем всмомогательный массив
    if(MyRep) delete MyRep;                                 // Удаляем отчет
    return result;
}   // clsAgregate::GetReport

/** Set - методы **/

void clsAgregate::Set_progress_shell(clsProgress_shell<type_progress>* _val) {
/** Функция присваивает указателю pshell каждого объекта из контейнера CostChain адрес объекта
val - оболочки для индикатора прогресса. **/
    auto Set_shell = [&_val](auto& obj) { obj.Set_progress_shell(_val); };
    /** Лямбда для установки указателя на оболочку индикатора прогресса **/
    for(T_agregate &obj: CostChain) {
        std::visit(Set_shell, obj);
    }
}   // clsAgregate::Set_progress_shell

void clsAgregate::Set_share(const size_t index, const decimal _share[]) {
/** Функция устанавливает нормативы запасов на складе. Параметры: index - индекс элемента контейнера CostChain,
в котором находится объект типа clsStorage, _share - указатель на массив с нормативами запасов ресурсов для каждого
индивидуального склада. Запас выражается в долях от объемов отгрузки. **/
    auto Setter_share = [&_share](auto& obj) {          // Функция-посетитель
        using T = std::decay_t<decltype(obj)>;          // Преобразование к базовому типу и определения алиаса Т
        if constexpr (std::is_same_v<T, clsStorage>)    // Если базовый тип clsStorage,
            for(size_t i{}; i<obj.Size(); i++)                      // то для каждого индивидуального i-го склада
                obj.SetShare(i, *(_share+i));                       // устанавливаем соответствующий норматив запаса
        else if constexpr (std::is_same_v<T, clsManufactory>) {};   // Если базовый тип clsManufactory,
    };                                                              // то ничего не делаем
    if(CostChain.size() == sZero) return;                           // Если контейнер пуст, то выходим
    else std::visit(Setter_share, CostChain[index]);                // Вызываем посетителя для элемента
}   // clsAgregate::Set_share

void clsAgregate::Set_permission(const size_t index, const bool _perm[]) {
/** Функция устанавливает разрешение/запрет на отгрузку и закупку на складе в одном и том же периоде. Параметры:
index - индекс элемента контейнера CostChain, _perm - указатель на массив с флагами разрешения. **/
    auto Setter_permission = [&_perm](auto& obj) {      // Функция-посетитель
        using T = std::decay_t<decltype(obj)>;          // Преобразование к базовому типу и определения алиаса Т
        if constexpr (std::is_same_v<T, clsStorage>)    // Если базовый тип clsStorage,
            for(size_t i{}; i<obj.Size(); i++)                      // то для каждого индивидуального i-го склада
                obj.SetPermission(i, *(_perm+i));                   // устанавливаем соответствующий флаг разрешения
        else if constexpr (std::is_same_v<T, clsManufactory>) {};   // Если базовый тип clsManufactory,
    };                                                              // то ничего не делаем
    if(CostChain.size() == sZero) return;                           // Если контейнер пуст, то выходим
    else std::visit(Setter_permission, CostChain[index]);           // Вызываем посетителя для элемента
}   // clsAgregate::Set_permission

void clsAgregate::Set_autopurchase(const size_t index, const PurchaseCalc _pcalc[]) {
/** Функция устанавливает флаг авторасчета/ ручного расчета закупок на складе. Параметры: index - индекс элемента
контейнера CostChain, _pcalc - указатель на массив с флагами авторасчета. **/
    auto Setter_autopurchase = [&_pcalc](auto& obj) {   // Функция-посетитель
        using T = std::decay_t<decltype(obj)>;          // Преобразование к базовому типу и определения алиаса Т
        if constexpr (std::is_same_v<T, clsStorage>)    // Если базовый тип clsStorage,
            for(size_t i{}; i<obj.Size(); i++)                      // то для каждого индивидуального i-го склада
                obj.SetAutoPurchase(i, *(_pcalc+i));                // устанавливаем соответствующий флаг авторасчёта
        else if constexpr (std::is_same_v<T, clsManufactory>) {};   // Если базовый тип clsManufactory,
    };                                                              // то ничего не делаем
    if(CostChain.size() == sZero) return;                           // Если контейнер пуст, то выходим
    else std::visit(Setter_autopurchase, CostChain[index]);         // Вызываем посетителя для элемента
}   // clsAgregate::Set_autopurchase

bool clsAgregate::Set_ship_volume(const strItem _Shipment[]) {
/** Метод ввода объемов отгрузки в подразделение с самым большим индексом. Параметры: _Shipment
- указатель на вводимый массив с отгрузками. Метод возвращает true при удачном копировании входного
массива во внутренний массив Shipment.**/
    if((!_Shipment)  || (CostChain.size() == sZero)) return false;  // Валидация параметров
    sh_size = std::visit(Getter_exporting_count(shipment), *(CostChain.cend()-1));  // Получаем число строк массива
    size_t tcount = sh_size * PrCount;              // Вычисляем размер копируемых массивов
    strItem* temp = new(nothrow) strItem[tcount];   // Выделяем память временному массиву
    if(!temp) return false;                         // Если память не выделена, то выход
    var_cpy(temp, _Shipment, tcount);               // Копирование массивов
    std::swap(temp, Shipment);                      // Обмениваем указатели
    if(temp) delete[] temp;                         // Если temp не пуст, то удаляем его
    return true;
}   // clsAgregate::Set_ship_volume

bool clsAgregate::Set_pur_price(const strItem _Purchase[]) {
/** Метод ввода цен поставок в подразделение с нулевым индексом. Параметры: _Purchase - указатель на вводимый
массив с поставками. Метод возвращает true при удачном копировании входного массива во внутренний массив Purchase. **/
    if((!_Purchase)  || (CostChain.size() == sZero)) return false;  // Валидация параметров
    sh_size = std::visit(Getter_exporting_count(purchase), *(CostChain.cbegin()));  // Получаем число строк массива
    size_t tcount = sh_size * PrCount;              // Вычисляем размер копируемых массивов
    if(Purchase) {                                  // Если массив Purchase непустой
        for(size_t i{}; i<tcount; i++) {
            (Purchase +i)->price = (_Purchase +i)->price;                           // Копируем цены
            (Purchase +i)->value = (Purchase +i)->volume * (Purchase +i)->price;    // Рассчитываем стоимость
        }
        var_cpy(Shipment, Purchase, tcount);        // Копируем массивы
    } else {                                        // Если массив Purchase пустой
        Purchase = new(nothrow) strItem[tcount];    // Выделяем память массиву
        if(!Purchase) return false;                 // Если память не выделена, то выход c false
        for(size_t i{}; i<tcount; i++) {
            (Purchase +i)->price = (_Purchase +i)->price;                           // Копируем цены
            (Purchase +i)->value = (Purchase +i)->volume = dZero;                   // Заполняем нулями
        }
    }
    return true;
}   // clsAgregate::Set_pur_price

bool clsAgregate::Set_pur_volume(const strItem _Pur_vol[]) {
/** Метод ввода объемов поставок в подразделение типа clsStorage с самым низким индексом. Используется в случае, когда
хотя бы для одного SKU установлен флаг ручного ввода объемов поставок (флаг "nocalc" - без автоматического расчета).
ВНИМАНИЕ!!! Метод вызывается ДО вызова функции RCalculate, примерно рядом с вызовом метода Set_ship_volume.
Параметры: _Pur_vol - указатель на вводимый массив с поставками. **/
    if(!_Pur_vol) return false;                             // Валидация параметров
    for(vector<T_agregate>::iterator it = CostChain.begin(); it != CostChain.end(); it++)
        if(std::holds_alternative<clsStorage>(*it)) {       // Если it указывает на clsStorage, то
            sh_size = std::visit(Getter_exporting_count(purchase), *it); // Получаем число строк массива
            size_t tcount = sh_size * PrCount;              // Вычисляем размер копируемых массивов
            strItem* tmp = new(nothrow) strItem[tcount];    // Выделяем память временному массиву
            if(!tmp) return false;                          // Если память не выделена, то выход c false
            var_cpy(tmp, _Pur_vol, tcount);                 // Копируем массивы
            if(!std::visit(Setter_purch(tmp), *it)) {       // Перемещаем временный массив
                if(tmp) delete[] tmp;                       // Если неудачно, то удаляем временный массив
                return false;                               // и выходим с false
            }
            return true;                                    // В случае успеха выходим с true
        }
    return false;                           // Если не нашли ни одного clsStorage, то выходим с false
}   // clsAgregate::Set_pur_volume

void clsAgregate::Reset() {
/** Метод сбрасывает всю информацию и приводит объект к состоянию по умолчанию **/
    clsAgregate temp;   // Создаем "пустой объект"
    swap(temp);         // Обмениваемся состоянием с пустым объектом
}   // clsAgregate::Reset

/** View- методы **/

void clsAgregate::ViewSettings() const {
/** Функция выводит на экран информацию о настройках для расчета. **/
    auto caller_view = [](auto& obj) {                  // Функция-посетитель
        using T = std::decay_t<decltype(obj)>;          // Преобразование к базовому типу и определения алиаса Т
        if constexpr (std::is_same_v<T, clsStorage>)    // Если базовый тип clsStorage,
            obj.ViewSettings();                         // вызываем соответствующую функцию
        else if constexpr (std::is_same_v<T, clsManufactory>)       // Если базовый тип clsManufactory,
            obj.ViewProjectParametrs();                             // вызываем соответствующую функцию
    };
    for(vector<T_agregate>::const_reverse_iterator crit=CostChain.crbegin(); crit!=CostChain.crend(); crit++ ) {
        // Для всех элементов контейнера в обратном порядке
        std::visit(caller_view, *crit);                 // вызываем функцию-посетителя
    }
}   // clsAgregate::ViewSettings

/** Расчётные методы **/

bool clsAgregate::RCalculate(const string msg[]) {
/** Метод рассчитывает объем ресурсов в натуральном выражении, поступающих в подразделение с нулевым индексом.
Если расчёты завершились удачно (метод вернул true), в массиве Purchase будут содержаться расчётные данные.
Параметры: msg - указатель на массив с наименованиями индикаторов прогресса. **/
    if(CostChain.size() == sZero) return false;                 // Вылидация параметров
    TLack Lack;                         // Вспомогательная переменная
    size_t i = CostChain.size()-sOne;   // Определяем индекс последнего элемента контейнера
    size_t MCount;                      // Количество позиций на входе, в середине и выходе
    auto Setter_maxcount = [&MCount](auto& obj) {
    //Функция устанавливает новое значение максимального числа итераций для индикатора прогресса
        obj.Set_progress_maxcount(MCount);
    };
    for(vector<T_agregate>::reverse_iterator rit=CostChain.rbegin(); rit!=CostChain.rend(); rit++) {
    // Устанавливаем наименование индикатора прогресса, устанавливаем максимальное значение счётчика
    // индикатора прогресса, вводим данные об отгрузке из массива Shipment в элемент контейнера,
    // рассчитываем этот элемент, выводим расчётные данные в массив Shipment. И так для всех элементов
    // контейнера в реверсивном направлении
        std::visit(Setter_progress_message{*(msg+i)}, *rit);    // Устанавливаем заголовок индикатора прогресса
        MCount = std::visit(Getter_exporting_count(shipment), *rit);    // Определяем число отгружаемых SKU в подразделении
        std::visit(Setter_maxcount, *rit);                      // Устанавливаем счётчик индикатора прогресса
        i--;                                                    // Уменьшаем счётчик массива msg
        if(!std::visit(Setter_ship(Shipment), *rit)) return false;      // Ввод объемов отгрузок из Shipment
        Lack = std::visit(Calculater_back{}, *rit);                     // Расчёт потребности в ресурсах
        cout << '\n';
        if(fabs(Lack.lack) > epsln) {                   // Контроль дефицита ресурсов
            cout << "Дефицит поставок у ресурса " << Lack.Name << " в объеме ";
            cout << Lack.lack << '\n';
            return false;
        };
        Shipment = std::visit(Getter_purch(), *rit);    // Получение массива с потребностью в массив Shipment
    };
    sh_size = std::visit(Getter_exporting_count(purchase), *(CostChain.cbegin()));  // Получаем число строк массива Shipment
    size_t tcount = sh_size * PrCount;                  // Размер копируемых массивов
    if(Purchase) {                                      // Если массив Purchase существует, то
        for(size_t i{}; i<tcount; i++) {
            (Purchase + i)->volume = (Shipment +i)->volume;                         // Копируем объемы поставок
            (Purchase + i)->value = (Purchase + i)->volume * (Purchase + i)->price; // Рассчитываем полную стоимость
        }
        var_cpy(Shipment, Purchase, tcount);            // Копируем массивы
    } else {
        Purchase = new(nothrow) strItem[tcount];        // Выделяем память массиву
        if(!Purchase) return false;                     // Если память не выделена, то выход с false
        var_cpy(Purchase, Shipment, tcount);            // Копируем массивы
    }
    return true;
}   // clsAgregate::RCalculate

bool clsAgregate::FCalculate(const string msg[]) {
/** Метод рассчитывает удельную и полную стоимости ресурсов, отгружаемых из подразделения с максимальным индексом.
Если расчёты завершились удачно (метод вернул true), в массиве Shipment будут содержаться расчётные данные.
Параметры: msg - указатель на массив с наименованиями индикаторов прогресса. **/
    if(CostChain.size() == sZero) return false;                 // Вылидация параметров
    TLack Lack;                         // Вспомогательная переменная
    size_t i{};                         // Вспомогательный счётчик
    size_t MCount;                      // Количество позиций на входе, в середине и выходе
    auto Setter_maxcount = [&MCount](auto& obj) {
    //Функция устанавливает новое значение максимального числа итераций для индикатора прогресса
        obj.Set_progress_maxcount(MCount);
    };
    for(vector<T_agregate>::iterator it = CostChain.begin(); it != CostChain.end(); it++) {
    // Устанавливаем наименование индикатора прогресса, устанавливаем максимальное значение счётчика
    // индикатора прогресса, вводим данные об поставке из массива Shipment в элемент контейнера,
    // рассчитываем этот элемент, выводим расчётные данные в массив Shipment. И так для всех элементов
    // контейнера в реверсивном направлении
        std::visit(Setter_progress_message{*(msg+i)}, *it);    // Устанавливаем заголовок индикатора прогресса
        MCount = std::visit(Getter_exporting_count(shipment), *it); // Определяем число отгружаемых SKU в подразделении
        std::visit(Setter_maxcount, *it);                      // Устанавливаем счётчик индикатора прогресса
        i++;                                                // Увеличиваем счётчик
        if(!std::visit(Setter_purch(Shipment), *it)) return false;  // Ввод цен из Shipment
        Lack = std::visit(Calculater_fwrd{}, *it);          // Расчёт потребности в ресурсах
        cout << '\n';
        if(fabs(Lack.lack) > epsln) {                       // Контроль дефицита ресурсов
            cout << "Дефицит поставок у ресурса " << Lack.Name << " в объеме ";
            cout << Lack.lack << '\n';
            return false;
        };
        Shipment = std::visit(Getter_ship(), *it);          // Получение массива со стоимостями в массив Shipment
    };
    sh_size = std::visit(Getter_exporting_count(shipment), *(CostChain.cend()-1));   // Получаем число строк массива Shipment
    return true;
}   // clsAgregate::FCalculate

bool clsAgregate::StF(ofstream &_outF) {
/** Метод имплементации записи в файловую переменную текущего экземпляра класса (запись в файл, метод
сериализации). Параметры: &_outF - экземпляр класса ofstream для записи данных **/
    #ifdef DEBUG_StF_RfF                            // Макрос вывода отладочной информации. Работает, если определен DEBUG_StF_RfF
        long bpos = _outF.tellp();          // Определяем позицию в начале файла
        cout << "clsAgregate::StF begin bpos= " << bpos << endl;
    #endif
    bool flagPurchase;                              // Флаг существования массива Purchase
    bool flagShipment;                              // Флаг существования массива Shipment
    size_t c_chain_size = CostChain.size();         // Размер контейнера
    bool c_chain_flag[c_chain_size];                // Массив с флагами для определения типа элемента контейнера
    if(!clsBaseProject::StF(_outF)) return false;   // Сохраняем в файл данные родительского класса
    if(!SEF(_outF, PrCount)) return false;          // Сохраняем в файл количество периодов проекта
    if(!SEF(_outF, c_chain_size)) return false;     // Сохраняем в файл размер контейнера
    if(!SEF(_outF, sh_size)) return false;          // Сохраняем в файл динамическОе число строк массива Shipment;
    auto flagCostChainItem = [] (auto& obj) -> bool {
    /** Лямбда для определения типа элемента контейнера **/
        using T = std::decay_t<decltype(obj)>;      // Преобразование к базовому типу и определения алиаса Т
        if constexpr(std::is_same_v<T, clsStorage>) return true;            // При clsStorage возвращает true,
        else if constexpr (std::is_same_v<T, clsManufactory>) return false; // при clsManufactory возвращает false
    };  // auto flagCostChainItem

    auto StF_writer = [&_outF](auto& obj) -> bool {
    /** Лямбда для вызова метода сериализации StF **/
        return obj.StF(_outF);
    };  // StF_writer

    /** Сохраняем в файл контейнер CostChain **/
    if(c_chain_size != sZero) {                     // Если размер контейнера ненулевой, то
        size_t j{};                                 // используем вспомогательный счётчик и
        for(T_agregate& val : CostChain) {          // Формируем массив флагов c_chain_flag
             *(c_chain_flag+j) = std::visit(flagCostChainItem, val);
             j++;                                   // Увеличиваем счётчик
        };
        if(!SEF(_outF, c_chain_flag, c_chain_size)) return false;   // Сохраняем массив флагов
        for(vector<T_agregate>::iterator it = CostChain.begin(); it != CostChain.end(); it++)
            if(!std::visit(StF_writer, *it)) return false;  // Сохраняем данные из контейнера
    };  // Сохраняем в файл контейнер CostChain

    size_t tm_size;                                 // Число строк массива Purchase или Shipment
    /** Сохраняем массив Purchase **/
    if(Purchase) {                                  // Если массив существует, то
        flagPurchase = true;                        // устанавливаем флаг true и определяем число
        tm_size = std::visit(Getter_exporting_count(purchase), *(CostChain.cbegin()));  // строк массива
    } else {
        flagPurchase = false;                       // устанавливаем флаг false
        tm_size = sZero;                            // присваиваем нулевое число строк
    }
    if(!SEF(_outF, flagPurchase)) return false;         // Сохраняем флаг в файл
    if(!SEF(_outF, tm_size)) return false;              // Сохраняем число строк массива Purchase в файл
    if(flagPurchase && tm_size>sZero) {                 // Если флаг true и число строк более нуля, то
        size_t tmpCount = tm_size * PrCount;            // вычисляем размер массива
        for(size_t i{}; i<tmpCount; i++)                // и каждый его элемент
            if(!(Purchase+i)->StF(_outF)) return false; // сохраняем в файл
    }
    /** Сохраняем массив Shipment **/
    if(Shipment) {                                  // Если массив существует, то
        flagShipment = true;                        // устанавливаем флаг true и определяем число
        tm_size = sh_size;                          // строк массива Shipment
    } else {
        flagShipment = false;                       // устанавливаем флаг false
        tm_size = sZero;                            // присваиваем нулевое число строк
    }
    if(!SEF(_outF, flagShipment)) return false;         // Сохраняем флаг в файл
    if(!SEF(_outF, tm_size)) return false;              // Сохраняем число строк массива Shipment в файл
    if(flagShipment && tm_size>sZero) {                 // Если флаг true и число строк более нуля, то
        size_t tmpCount = tm_size * PrCount;            // вычисляем размер массива
        for(size_t i{}; i<tmpCount; i++)                // и каждый его элемент
            if(!(Shipment+i)->StF(_outF)) return false; // сохраняем в файл
    }
    #ifdef DEBUG_StF_RfF
        long epos = _outF.tellp();                    // Определяем позицию в конце файла
        cout << "clsAgregate::StF end epos= " << epos << endl;
    #endif // DEBUG_StF_RfF
    return true;
}   // clsAgregate::StF

bool clsAgregate::RfF(ifstream &_inF) {
/** Метод имплементации чтения из файловой переменной экземпляра класса (чтение из файла, метод
десериализации). Параметры: &_inF - ссылка на экземпляр класса ifstream для чтения данных. **/
    size_t c_chain_size{};                          // Размер контейнера
    #ifdef DEBUG_StF_RfF                            // Макрос вывода отладочной информации. Работает, если определен DEBUG_StF_RfF
        long bpos = _inF.tellg();                   // Определяем позицию в начале файла
        cout << "clsAgregate::RfF begin bpos= " << bpos << endl;
    #endif
    if(!clsBaseProject::RfF(_inF)) return false;    // Читаем из файла данные родительского класса
    if(!DSF(_inF, PrCount)) return false;           // Читаем из файла количество периодов проекта
    if(!DSF(_inF, c_chain_size)) return false;      // Читаем из файла размер контейнера
    if(!DSF(_inF, sh_size)) return false;           // Читаем из файла динамическОе число строк массива Shipment;

    auto RfF_reader = [&_inF](auto& obj) -> bool {
    /** Лямбда для вызова метода десериализации RfF **/
        return obj.RfF(_inF);
    };  // RfF_reader

    /** Читаем из файла контейнер CostChain **/
    bool c_chain_flag[c_chain_size];                // Массив с флагами для определения типа элемента контейнера
    if(c_chain_size != sZero) {                     // Если восстановленный размер контейнера не равен нулю, то
        vector<T_agregate>().swap(CostChain);       // Приводим вектор в состояние по умолчанию
        if(!DSF(_inF, c_chain_flag, c_chain_size)) return false;    // Читаем из файла массив флагов
        CostChain.reserve(c_chain_size);                            // Резервируем память контейнера
        for(size_t i{}; i<c_chain_size; i++) {                      // Восстанавливаем контейнер:
            if(*(c_chain_flag+i)) CostChain.emplace_back(clsStorage()); // если флаг true, то создаем clsStorage;
            else CostChain.emplace_back(clsManufactory());              // иначе создаем clsManufactory; читаем из
            if(!std::visit(RfF_reader, *(CostChain.end()-1))) return false; // файла данные в последний элемент контейнера
        }
    }   // Читаем из файла контейнер CostChain

    size_t tm_size;                                 // Число строк массива Purchase или Shipment
    /** Читаем из файла массив Purchase **/
    bool flagPurchase;                              // Флаг существования массива Purchase
    if(!DSF(_inF, flagPurchase)) return false;      // Читаем из файла флаг существования массива
    if(!DSF(_inF, tm_size)) return false;           // Читаем число строк массива. Если флаг true и число
    if(flagPurchase && tm_size>sZero) {             // строк более нуля, можно восстанавливать массив:
        size_t tmpCount = tm_size * PrCount;            // вычисляем размер массива;
        strItem* tmp = new(nothrow) strItem[tmpCount];  // выделяем память вспомогательному массиву;
        if(!tmp) return false;                          // если память не выделена, то выход с false;
        for(size_t i{}; i<tmpCount; i++)                // если память выделена, то
            if(!(tmp+i)->RfF(_inF)) return false;       // каждый его элемент читаем из файла;
        std::swap(tmp, Purchase);                       // обмениваемся указателями
        if(tmp) delete[] tmp;                           // удаляем вспомогательный массив.
    } else {                                        // Иначе
        if(Purchase) delete[] Purchase;                 // если массив существует, удаляем массив
        Purchase = nullptr;                             // и устанавливаем указатель массива в nullptr
    }

    /** Читаем из файла массив Shipment **/
    bool flagShipment;                              // Флаг существования массива Shipment
    if(!DSF(_inF, flagShipment)) return false;      // Читаем из файла флаг существования массива
    if(!DSF(_inF, tm_size)) return false;           // Читаем число строк массива. Если флаг true и число
    if(flagShipment && tm_size>sZero) {             // строк более нуля, можно восстанавливать массив:
        size_t tmpCount = tm_size * PrCount;            // вычисляем размер массива;
        strItem* tmp = new(nothrow) strItem[tmpCount];  // выделяем память вспомогательному массиву;
        if(!tmp) return false;                          // если память не выделена, то выход с false;
        for(size_t i{}; i<tmpCount; i++)                // если память выделена, то
            if(!(tmp+i)->RfF(_inF)) return false;       // каждый его элемент читаем из файла;
        std::swap(tmp, Shipment);                       // обмениваемся указателями
        if(tmp) delete[] tmp;                           // удаляем вспомогательный массив.
    } else {                                        // Иначе
        if(Shipment) delete[] Shipment;                 // если массив существует, удаляем массив
        Shipment = nullptr;                             // и устанавливаем указатель массива в nullptr
    }
    #ifdef DEBUG_StF_RfF
        long epos = _inF.tellg();                   // Определяем позицию в конце файла
        cout << "clsAgregate::RfF end epos= " << epos << endl;
    #endif // DEBUG_StF_RfF
    return true;
}   // clsAgregate::RfF

# undef DEBUG_StF_RfF   // отменить макрос информации об инструменте DEBUG_StF_RfF
