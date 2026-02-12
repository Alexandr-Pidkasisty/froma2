#include "clsCostCenterStorage.h"

//#define DEBUG                                 // Вывод на экран результатов записи/чтения данных в/из файла
clsCostCenterStorage::clsCostCenterStorage(): clsStorage() {
/** Конструктор по умолчанию **/
    InCount = MidCount = OutCount = sZero;      // Инициализируем количество позиций
    Lack = {dZero, EmpStr};                     // Устанавливаем признак отсутствия дефицита ресурсов
    OutCost = nullptr;                          // Устанавливаем пустой указатель ЦЗО
    InCost  = nullptr;                          // Устанавливаем пустой указатель ЦЗП
    Purchase = nullptr;                         // Устанавливаем пустой указатель на закупки
    Shipment = nullptr;                         // Устанавливаем пустой указатель на отгрузки
}   // Сtor

clsCostCenterStorage::clsCostCenterStorage(size_t _PrCount, Currency _cur, AccountingMethod _ac, size_t ProdCount,\
    const size_t _MidCount, const size_t _RMCount, const strNameMeas _RMNames[]):\
    clsStorage(_PrCount, _cur, _ac, _MidCount) {
/** Конструктор с параметрами. Параметры: _PrCount - число периодов проекта, _cur - валюта проекта, _ac - принцип
учета запасов (FIFO, LIFO, AVG), ProdCount - число номенклатурных позиций, отгружаемых со склада, _MidCount - число
позиций на выходе ЦЗП/ входе ЦЗО, _RMCount - число номенклатурных позиций, поступающих на склад, _RMNames - указатель
на массив с именами и единицами измерения ресурсов, поступающих на склад, размером _RMCount. Конструктор валидирует
входные параметры и создает центры затрат - ЦЗП и ЦЗО. **/
    InCount = _RMCount;                         // Инициализируем количество позиций на входе ЦЗП
    MidCount = _MidCount;                       // Инициализируем количество позиций на выходе ЦЗП, ЦУР и входе ЦЗО
    OutCount = ProdCount;                       // Инициализируем количество позиций на выходе ЦЗО
    Lack = {dZero, EmpStr};                     // Устанавливаем признак отсутствия дефицита ресурсов
    if(MidCount != sZero) {                     // Валидация параметров и создание ЦЗО и ЦУР
        if(OutCount != sZero) {
            OutCost = new(nothrow) clsManufactory(_PrCount, MidCount, _RMNames, OutCount);
            if(OutCost) OutCost->SetCurrency(_cur);
        } else OutCost = nullptr;
        if(InCount != sZero) {
            InCost  = new(nothrow) clsManufactory(_PrCount, InCount, _RMNames, MidCount);
            if(InCost) InCost->SetCurrency(_cur);
        } else InCost = nullptr;
    } else {
        OutCost = nullptr;
        InCost = nullptr;
    }
    Purchase = nullptr;
    Shipment = nullptr;
}   // Ctor with parameters

clsCostCenterStorage::~clsCostCenterStorage() {
/** Деструктор по умолчанию **/
    if(OutCost) delete OutCost;
    if(InCost) delete InCost;
    if(Purchase) delete[] Purchase;
    if(Shipment) delete[] Shipment;
}   // Dtor

void clsCostCenterStorage::swap(clsCostCenterStorage& other) noexcept {
/** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
    clsStorage::swap(other);                // Вызываем функцию родительского класса
    std::swap(InCount, other.InCount);      // Обмениваем значения
    std::swap(MidCount, other.MidCount);    // Обмениваем значения
    std::swap(OutCount, other.OutCount);    // Обмениваем значения
    std::swap(Lack, other.Lack);            // Обмениваем значения
    std::swap(OutCost, other.OutCost);      // Обмениваем указатели
    std::swap(InCost, other.InCost);        // Обмениваем указатели
    RecipeOut.swap(other.RecipeOut);        // Обмениваем вектора
    RecipeIn.swap(other.RecipeIn);          // Обмениваем вектора
    std::swap(Purchase, other.Purchase);    // Обмениваем указатели
    std::swap(Shipment, other.Shipment);    // Обмениваем указатели
}   // clsCostCenterStorage::swap

clsCostCenterStorage::clsCostCenterStorage(const clsCostCenterStorage& other): clsStorage(other) {
/** Конструктор копирования **/
    InCount = other.InCount;                // Копируем число позиций на входе ЦЗП
    MidCount = other.MidCount;              // Копируем число позиций на ЦУР и входе ЦЗО
    OutCount = other.OutCount;              // Копируем число позиций на выходе ЦЗО
    Lack = other.Lack;                                  // Копируем данные о дефиците
    if(other.OutCost)                                   // Если other.OutCost существует,
        OutCost = new clsManufactory(*other.OutCost);   // то создаем объект копированием,
    else OutCost = nullptr;                             // иначе присваиваем nullptr
    if(other.InCost)                                    // Если other.InCost существует,
        InCost = new clsManufactory(*other.InCost);     // то создаем объект копированием,
    else InCost  = nullptr;                             // иначе присваиваем nullptr
    RecipeOut = other.RecipeOut;                        // Копируем тех.карты для ЦЗО
    RecipeIn  = other.RecipeIn;                         // Копируем тех.карты для ЦЗП
    if(other.Purchase) Purchase = new(nothrow) strItem[InCount*clsStorage::ProjectCount()];
    else Purchase = nullptr;                // Копируем массив с закупочными ценами и объемами закупок
    if(Purchase) var_cpy(Purchase, other.Purchase, InCount*clsStorage::ProjectCount());
    if(other.Shipment) Shipment = new(nothrow) strItem[OutCount*clsStorage::ProjectCount()];
    else Shipment = nullptr;                // Копируем массив с объемами отгрузок
    if(Shipment) var_cpy(Shipment, other.Shipment, OutCount*clsStorage::ProjectCount());
}   //Copy ctor

clsCostCenterStorage::clsCostCenterStorage(clsCostCenterStorage&& other): clsCostCenterStorage() {
/** Конструктор перемещения **/
    swap(other);
}   // Move Ctor

clsCostCenterStorage& clsCostCenterStorage::operator=(const clsCostCenterStorage& rhs) {
/** Перегрузка оператора присваивания копированием. Реализовано в идеоме
КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/
    if (this == &rhs) return *this; // Если объекты идентичны, то ничего не делаем
    clsCostCenterStorage tmp(rhs);  // Вызываем конструктор копирования и получаем копию rhs в переменной tmp
    swap(tmp);                      // Обмениваемся содержанием с tmp
    return *this;
}   // Copy operator=

clsCostCenterStorage& clsCostCenterStorage::operator=(clsCostCenterStorage&& rhs) {
/** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
(move-and-swap idiom) **/
    if(this == &rhs) return *this;          // Если объекты идентичны, то ничего не делаем
    clsCostCenterStorage tmp(move(rhs));    // Вызываем конструктор перемещения и получаем объект rhs в переменной tmp
    swap(tmp);                              // Обмениваемся содержанием с tmp
    return *this;
}   // Move operator=

bool clsCostCenterStorage::StF(ofstream &_outF) {
/** Метод имплементации записи в файловый поток текущего экземпляра класса (метод сериализации).
Параметры: &_outF - экземпляр класса ofstream для записи данных **/
    #ifdef DEBUG
        long bpos = _outF.tellp();                  // Определяем позицию в начале файла
        clog << "clsCostCenterStorage::StF begin bpos= " << bpos << endl;
    #endif // DEBUG
    /** Флаги состояния динамических объектов. Если флаг true, то соответствующий объект записывается в файл, если
    нет - то не записывается. Флаги сохраняются в файле и помогают при чтении данных из файла определять, есть там
    объект и его надо считывать, или его нет. **/
    bool flagOutCost, flagInCost, flagPurchase, flagShipment;
    if(OutCost) flagOutCost     = true; else flagOutCost    = false;
    if(InCost) flagInCost       = true; else flagInCost     = false;
    if(Purchase) flagPurchase   = true; else flagPurchase   = false;
    if(Shipment) flagShipment   = true; else flagShipment   = false;
    /** Сохраняем флаги в файл **/
    if(!SEF(_outF, flagOutCost))    return false;
    if(!SEF(_outF, flagInCost))     return false;
    if(!SEF(_outF, flagPurchase))   return false;
    if(!SEF(_outF, flagShipment))   return false;
    /** Сохраняем в файл основные данные **/
    if(!SEF(_outF, InCount)) return false;          // Сохраняем InCount
    if(!SEF(_outF, MidCount)) return false;         // Сохраняем MidCount
    if(!SEF(_outF, OutCount)) return false;         // Сохраняем OutCount
    if(!SEF(_outF, Lack.lack)) return false;        // Сохраняем поле lack переменной Lack
    if(!SEF(_outF, Lack.Name)) return false;        // Сохраняем поле Name переменной Lack
    if(!this->clsStorage::StF(_outF)) return false; // Сохраняем ЦУР
    if(flagOutCost)
        if(!OutCost->StF(_outF)) return false;      // Сохраняем ЦЗО
    if(flagInCost)
        if(!InCost->StF(_outF))  return false;      // Сохраняем ЦЗП
    size_t RecipeOutSize  = RecipeOut.size();       // Количество элементов вектора RecipeOut
    size_t RecipeOutCapac = RecipeOut.capacity();   // Размер выделенной памяти для вектора RecipeOut
    size_t RecipeInSize  = RecipeIn.size();         // Количество элементов вектора RecipeIn
    size_t RecipeInCapac = RecipeIn.capacity();     // Размер выделенной памяти для вектора RecipeIn
    if(!SEF(_outF, RecipeOutSize)) return false;    // Сохраняем в файл количество элементов вектора
    if(!SEF(_outF, RecipeOutCapac)) return false;   // Сохраняем в файл емкость вектора
    if(!SEF(_outF, RecipeInSize)) return false;     // Сохраняем в файл количество элементов вектора
    if(!SEF(_outF, RecipeInCapac)) return false;    // Сохраняем в файл емкость вектора
    if(RecipeOutSize != sZero) {                    // Если вектор RecipeOut непустой, то
        for(vector<clsRecipeItem>::iterator it = RecipeOut.begin(); it < RecipeOut.end(); it++)
            if(!it->StF(_outF)) return false;       // Сохраняем вектор RecipeOut
    }
    if(RecipeInSize != sZero) {                     // Если вектор RecipeIn непустой, то
        for(vector<clsRecipeItem>::iterator it = RecipeIn.begin(); it < RecipeIn.end(); it++)
            if(!it->StF(_outF)) return false;       // Сохраняем вектор RecipeIn
    }
    if(flagPurchase) {
        size_t tcount = InCount * clsStorage::ProjectCount();   // Размер массива Purchase
        for(size_t i{}; i<tcount; i++)                          // Сохраняем массив поэлементно
            if(!(Purchase+i)->StF(_outF)) return false;
    }
    if(flagShipment) {
        size_t tcount = OutCount * clsStorage::ProjectCount();  // Размер массива Shipment
        for(size_t i{}; i<tcount; i++)                          // Сохраняем массив поэлементно
            if(!(Shipment+i)->StF(_outF)) return false;
    }
    #ifdef DEBUG
        long epos = _outF.tellp();                  // Определяем позицию в конце файла
        clog << "clsCostCenterStorage::StF end epos= " << epos << endl;
    #endif // DEBUG
    return true;
}   // clsCostCenterStorage::StF

bool clsCostCenterStorage::SaveToFile(const string _filename) {
/** Метод записи текущего экземпляра класса clsCostCenterStorage в файл **/
    if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
    ofstream outF(_filename, ios::out | ios::binary);   // Связываем имя файла с файловым потоком для записи на диск
    if (!outF.is_open()) {                              // Если файл не создан, то сообщение пользователю и
        clog << "Ошибка открытия файла " << _filename << endl;  // возврат false и выход из функции
        return false;
    }
    if(!StF(outF)) {                                    // Записываем данные в файл. Если файл недозаписан, то
        outF.close();                                   // Закрыаем файл и
        std::remove(_filename.c_str());                 // удаляем его
        return false;                                   // Выходим с false
    }
    outF.close();                                       // Закрываем файл
    return true;                                        // Возвращаем true
}   // clsCostCenterStorage::SaveToFile

bool clsCostCenterStorage::RfF(ifstream &_inF) {
/** Метод восстановления из файлового потока текущего экземпляра класса (метод десериализации).
Параметры: &_inF - экземпляр класса ofstream для записи данных **/
    #ifdef DEBUG
        long bpos = _inF.tellg();                   // Определяем позицию в начале файла
        clog << "clsCostCenterStorage::StF begin bpos= " << bpos << endl;
    #endif // DEBUG
    /** Флаги состояния динамических объектов. Если флаг true, то соответствующий объект читается из файла, если
    нет - то не читается. Флаги записаны в файле и помогают при чтении данных из файла определять, есть там
    объект и его надо считывать, или его нет. **/
    bool flagOutCost, flagInCost, flagPurchase, flagShipment;
    /** Читаем флаги из файла **/
    if(!DSF(_inF, flagOutCost))     return false;
    if(!DSF(_inF, flagInCost))      return false;
    if(!DSF(_inF, flagPurchase))    return false;
    if(!DSF(_inF, flagShipment))    return false;
    /** Читаем из файла основные данные **/
    clsCostCenterStorage temp;                      // Временный объект
    if(!DSF(_inF, temp.InCount))    return false;   // Читаем InCount
    if(!DSF(_inF, temp.MidCount))   return false;   // Читаем MidCount
    if(!DSF(_inF, temp.OutCount))   return false;   // Читаем OutCount
    if(!DSF(_inF, temp.Lack.lack))  return false;   // Читаем поле lack переменной Lack
    if(!DSF(_inF, temp.Lack.Name))  return false;   // Читаем поле Name переменной Lack
    if(!temp.clsStorage::RfF(_inF)) return false;   // Читаем ЦУР
    if(flagOutCost) {
        temp.OutCost = new clsManufactory();        // Создаем объект
        if(!temp.OutCost->RfF(_inF)) return false;  // Читаем ЦЗО
    } else temp.OutCost = nullptr;
    if(flagInCost) {
        temp.InCost = new clsManufactory();        // Создаем объект
        if(!temp.InCost->RfF(_inF))  return false;  // Читаем ЦЗП
    } else temp.InCost = nullptr;
    size_t RecipeOutSize, RecipeOutCapac, RecipeInSize, RecipeInCapac;
    if(!DSF(_inF, RecipeOutSize)) return false;     // Читаем из файла количество элементов вектора
    if(!DSF(_inF, RecipeOutCapac)) return false;    // Читаем из файла емкость вектора
    if(!DSF(_inF, RecipeInSize)) return false;      // Читаем из файла количество элементов вектора
    if(!DSF(_inF, RecipeInCapac)) return false;     // Читаем из файла емкость вектора
    if(RecipeOutSize != sZero) {                    // Если вектор непустой, то
        temp.RecipeOut.reserve(RecipeOutCapac);     // Резервируем память для вектора temp.RecipeOut
        for(size_t i{}; i<RecipeOutSize; i++) {     // Поэлементно
            temp.RecipeOut.emplace_back();                      // создаем элемент непосредственно в векторе,
            if(!temp.RecipeOut.back().RfF(_inF)) return false;  // вызываем метод десериализации этого элемента
        }
    };
    if(RecipeInSize != sZero) {                     // Если вектор непустой, то
        temp.RecipeIn.reserve(RecipeInCapac);       // Резервируем память для вектора temp.RecipeIn
        for(size_t i{}; i<RecipeInSize; i++) {
            temp.RecipeIn.emplace_back();                       // Создаем элемент непосредственно в векторе
            if(!temp.RecipeIn.back().RfF(_inF)) return false;   // Вызываем метод десериализации этого элемента
        }
    };
    if(flagPurchase) {
        size_t tcount = temp.InCount * temp.clsStorage::ProjectCount(); // Размер массива temp.Purchase
        temp.Purchase = new(nothrow) strItem[tcount];           // Выделяем память массиву
        if(!temp.Purchase) return false;                        // Если не выделена, то выход с false
        for(size_t i{}; i<tcount; i++)                          // Читаем массив поэлементно
            if(!(temp.Purchase+i)->RfF(_inF)) return false;
    } else temp.Purchase = nullptr;
    if(flagShipment) {
        size_t tcount = temp.OutCount * temp.clsStorage::ProjectCount();    // Размер массива temp.Shipment
        temp.Shipment = new(nothrow) strItem[tcount];           // Выделяем память массиву
        if(!temp.Shipment) return false;                        // Если не выделена, то выход с false
        for(size_t i{}; i<tcount; i++)                          // Читаем массив поэлементно
            if(!(temp.Shipment+i)->RfF(_inF)) return false;
    } else temp.Shipment = nullptr;
    this->swap(temp);                               // Обмениваемся состоянием с временным объектом
    #ifdef DEBUG
        long epos = _inF.tellg();                   // Определяем позицию в конце файла
        clog << "clsCostCenterStorage::StF end epos= " << epos << endl;
    #endif // DEBUG
    return true;
}   // clsCostCenterStorage::RfF

bool clsCostCenterStorage::ReadFromFile(const string _filename) {
/** Метод чтения из файла и запись в экземпляр класса clsCostCenterStorage **/
    if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
    ifstream inF(_filename, ios::in | ios::binary);     // Связываем имя файла с файловым потоком для чтения с диска
    if (!inF.is_open()) {                               // Если файл не открыт, то сообщение пользователю и
        clog << "Ошибка открытия файла " << _filename << endl;  // возврат false и выход из функции
         return false;
    }
    if(!RfF(inF)) {                                     // Считываем данные из файла. Если файл не записан, то
        inF.close();                                    // Закрываем файл
        return false;                                   // Выход и возврат false
    };
    inF.close();                                        // Закрываем файл
    return true;
}   // clsCostCenterStorage::ReadFromFile

strItem* clsCostCenterStorage::cost_centre_purchase(clsManufactory* obj, \
    vector<clsRecipeItem>&& _Recipe, strItem _ShipPlan[]) {
/** Метод вводит данные в заданный Центр Затрат и возвращает потребность в ресурсах для него.
Пареметры: obj - указатель на выбранный центр затрат (ЦЗП или ЦЗО); _Recipe - ссылка на
перемещаемый контейнер с рецептурами для выбранного центра затрат; _ShipPlan - указатель на
массив с отгрузками с выбранного ЦЗ. **/
    if(!_ShipPlan) return nullptr;                      // Валидация параметров
    if(_Recipe.size() == sZero) return nullptr;         // Если нет рецептур, то возвращаем nullptr
    if(!obj) return nullptr;                            // Если ЦЗ не существует, то возвращаем nullptr
    size_t tRMCount, tProdCount;                        // Переменные для хранения числа ресурсов на входе и выходе ЦЗ
    if(obj == InCost) {                                 // Если ЦЗП, то
        tRMCount = InCount;                             // число входящих ресурсов равно InCount
        tProdCount = MidCount;                          // число исходящих ресурсов равно MidCount
    } else {                                            // Иначе это ЦЗО и
        tRMCount = MidCount;                            // число входящих ресурсов равно MidCount
        tProdCount = OutCount;                          // число исходящих ресурсов равно OutCount
    }
    if(tRMCount < tProdCount) return nullptr;           // Валидация параметров

    /** Ввод данных в ЦЗ **/
    const strNameMeas* pnames = obj->GetRMNames();      // Получаем указатель на массив с наименованиями ресурсов
    if(!pnames) return nullptr;                         // Если массив пуст, то выход с nullptr
    clsRecipeItem* pRec = _Recipe.data();               // Получаем указатель на данные контейнера
    if(!pRec) return nullptr;                           // Если массив пуст, то выход с nullptr
    for(size_t i{}; i<tProdCount; i++) {                // Цикл по списку исходящих ресурсов
//        clog << "(pnames+i)->name= " << (pnames+i)->name << endl;
        if((pnames+i)->name == (pRec+i)->GetName()) {   // Если имена совпдают, добавляем ед.производство
            obj->SetManufItem(move(*(pRec+i)), (_ShipPlan + obj->GetPrCount()*i));  // Добавляем ед. производство
        } else {                                        // Если имена НЕ совпадают, то
            clsRecipeItem* pp = find_recipe_by_name((pnames+i)->name, _Recipe); // ищем рецептуру с совпадающим именем
            if(!pp) {                                   // Если рецептура с подходящим именем не найдена,
                return nullptr;                         // то выход с nullptr
            }
            obj->SetManufItem(move(*pp), (_ShipPlan + obj->GetPrCount()*i));        // Добавляем ед. производство
        }
    }
    /** Расчёт потребности ЦЗ в ресурсах **/
    void (clsManufactory::*g)();                // Определяем указатель на внутреннюю функцию класса clsManufactory
    if((cmeth == sOne) || (cmeth == sTwo)) {    // Выбираем метод расчета производства
        if(cmeth == sOne) g = &clsManufactory::CalcRawMatPurchPlan_future;
        else g = &clsManufactory::CalcRawMatPurchPlan_thread;
    } else g = &clsManufactory::CalcRawMatPurchPlan;
    string msg;                             // Переменная для сообщения индикатора
    if(obj == InCost) msg = "Расчёт ЦЗП";
    else msg = "Расчёт ЦЗО";
    obj->Set_progress_message(move(msg));   // Устанавливаем сообщение индикатора
    obj->Set_progress_maxcount(tProdCount); // Устанавливаем число итераций для индикатора
    (obj->*g)();                            // Рассчитываем потребность в ресурсах для ЦЗ
    return obj->GetRMPurchPlan();           // Создаем и возвращаем массив с потребностью в ресурсах для производства
}   // clsCostCenterStorage::cost_centre_purchase

strItem* clsCostCenterStorage::cost_centre_shipment(clsManufactory* obj, strItem _Purchase[]) {
/** Метод вводит цены из массива _Purchase в заданный ЦЗ и возвращает себестоимость позиций на выходе ЦЗ.
Параметры: obj - указатель на выбранный центр затрат (ЦЗП или ЦЗО); _Purchase - указатель на на массив с
поступлениями в выбранный ЦЗ. **/
    if(!_Purchase) return nullptr;  // Валидация параметров
    if(!obj) return nullptr;        // Если ЦЗ не существует, то возвращаем nullptr
    size_t tRMCount, tProdCount;    // Переменные для хранения числа ресурсов на входе и выходе ЦЗ
    if(obj == InCost) {             // Если ЦЗП, то
        tRMCount = InCount;         // число входящих ресурсов равно InCount
        tProdCount = MidCount;      // число исходящих ресурсов равно MidCount
    } else {                        // Иначе это ЦЗО и
        tRMCount = MidCount;        // число входящих ресурсов равно MidCount
        tProdCount = OutCount;      // число исходящих ресурсов равно OutCount
    }
    if(tRMCount < tProdCount) return nullptr; // Валидация параметров

    /** Ввод данных в ЦЗ  **/
    size_t Count = tRMCount * obj->GetPrCount();        // Рассчитываем размер копируемого массива
    decimal* PurPrice = new(nothrow) decimal[Count];    // Выделяем память временному массиву
    if(!PurPrice) return nullptr;
    for(size_t i{}; i<Count; i++ )
        *(PurPrice+i) = (_Purchase+i)->price;   // Формируем массив для ввода в ЦЗ

    /** Расчет себестоимости на выходе ЦЗ **/
    obj->SetRawMatPrice(PurPrice);              // Вводим цены в ЦЗ
    delete[] PurPrice;                          // Удаляем вспомогательный массив
    bool (clsManufactory::*h)();                // Определяем указатель на внутреннюю функцию класса clsManufactory
    if((cmeth == sOne) || (cmeth == sTwo)) {    // Выбираем метод расчета производства
        if(cmeth == sOne) h = &clsManufactory::Calculate_future;
        else h = &clsManufactory::Calculate_thread;
    } else h = &clsManufactory::Calculate;
    string msg;                                 // Переменная для сообщения индикатора
    if(obj == InCost) msg = "Расчёт ЦЗП";
    else msg = "Расчёт ЦЗО";
    obj->Set_progress_message(move(msg));       // Устанавливаем сообщение индикатора
    obj->Set_progress_maxcount(tProdCount);     // Устанавливаем число итераций для индикатора
    (obj->*h)();                                // Рассчитываем удельную и полную себестоимость на выходе ЦЗ
    return obj->GetTotalProduct();              // Создаем массив с объемами и себестоимостью на выходе ЦЗ
}   // clsCostCenterStorage::cost_centre_shipment

void clsCostCenterStorage::EraseVector(vector<clsRecipeItem>& Recipe) {
/** Метод для полной очистки вектора с рецептурами **/
    vector<clsRecipeItem>().swap(Recipe);       // Приводим вектор в состояние по умолчанию
}   // clsCostCenterStorage::EraseVector

clsRecipeItem* clsCostCenterStorage::find_recipe_by_name(const string& _name, vector<clsRecipeItem>& vec) {
/** Метод ищет элемент контейнера с именем _name в контейнере vec и возвращает указатель на этот элемент.
Если совпадение не найдено, метод возвращает nulptr. **/
    for(clsRecipeItem* p = vec.data(); p < vec.data()+vec.size(); p++)  // Цикл по всем элементами контейнера
        if(p->GetName() == _name) return p;         // Если имена совпадают, возврат указателя на элемент
    return nullptr;                                 // Если совпадения не найдено, возврат nullptr
}   // clsCostCenterStorage::find_recipe_by_name

bool clsCostCenterStorage::SetStorage(strItem _ShipPlan[], strItem _Purchase[],\
    const clsRecipeItem _RecipeOut[], const clsRecipeItem _RecipeIn[]) {
/** Метод ввода исходных данных. Параметры: _ShipPlan - указатель на массив отгрузок со склада размером
ProdCount * _PrCount, где ProdCount и _PrCount - количество позиций, отгружаемых со склада и количество
периодов проекта, соответственно, введенные ранее в конструкторе с параметрами; _Purchase - указатель на
массив с объемами поступлений (при отсутсвтии флага авторасчёта) и ценами поступаемых на склад ресурсов
размером _RMCount * _PrCount, где _RMCount - число позиций ресурсов, поступающих на склад (включают в себя
ресурсы, отгружаемые впоследствии со склада и ресурсы, потребляемые на складе), введенное также ранее в
конструкторе с параметрами; _RecipeOut и _RecipeIn - константные указатели на копируемые массивы с
технологическими картами для ЦЗО и ЦЗП соответственно. **/
    if(!_Purchase) return false;                    // Если в массив с ценами отсутствует, выход с false
    if(!OutCost) return false;                      // Если ЦЗО отсутствует, выход с false
    if(!InCost) return false;                       // Если ЦЗП отсутствует, выход с false
    size_t tmpCount = InCount*clsStorage::ProjectCount();   // Размер массива поступлений на ЦЗП
    if(Purchase) delete[] Purchase;                         // Если массив существует, то удаляем его
    Purchase = new(nothrow) strItem[tmpCount];              // Выделяем память массиву
    if(!Purchase) return false;                             // Если память не выделена, выход с false
    var_cpy(Purchase, _Purchase, tmpCount);                 // Заполняем массив Purchase данными
    RecipeOut.resize(OutCount);                             // Изменяем размер RecipeOut
    var_cpy(RecipeOut.data(), _RecipeOut, OutCount);        // Копируем элементы в контейнер
    /** Получаем данные для ввода из ЦЗО в ЦУР (Создаем массив с потребностью в ресурсах для ЦЗО) **/
    strItem* OutPurtmp = cost_centre_purchase(OutCost, move(RecipeOut), _ShipPlan);
    EraseVector(RecipeOut);                                 // Опустошаем вектор
    if(!OutPurtmp)  return false;                           // Если массив не создан, то выход с false
    const strNameMeas* OutNamtmp = OutCost->GetRMNames();   // Получаем указатель на внутренний массив
                                                            // с именами и ед.измерения
    /** Вводим данные в в ЦУР (из массива _Purchase используются только MidCount * PrCount данных) **/
    if(!clsStorage::SetStorage(MidCount, OutNamtmp, OutPurtmp, _Purchase)) {        // Создаем склад,
        delete[] OutPurtmp;                                 // если склад не создан, удаляем массив
        return false;                                       // и выходим с false
    }
    if(OutPurtmp) delete[] OutPurtmp;                       // Удаляем массив за ненадобностью

    /** Сохраняем техкарты для ЦЗП **/
    RecipeIn.resize(MidCount);
    var_cpy(RecipeIn.data(), _RecipeIn, MidCount);
    return true;
}   // clsCostCenterStorage::SetStorage with pointers

bool clsCostCenterStorage::SetStorage(strItem _ShipPlan[], strItem _Purchase[],\
    vector<clsRecipeItem>&& _RecipeOut, vector<clsRecipeItem>&& _RecipeIn) {
/** Метод ввода исходных данных. Параметры: _ShipPlan - указатель на массив отгрузок со склада размером
ProdCount * _PrCount, где ProdCount и _PrCount - количество позиций, отгружаемых со склада и количество
периодов проекта, соответственно, введенные ранее в конструкторе с параметрами; _Purchase - указатель на
массив с объемами поступлений (при отсутсвтии флага авторасчёта) и ценами поступаемых на склад ресурсов
размером _RMCount * _PrCount, где _RMCount - число позиций ресурсов, поступающих на склад (включают в себя
ресурсы, отгружаемые впоследствии со склада и ресурсы, потребляемые на складе), введенное также ранее в
конструкторе с параметрами; _RecipeOut и _RecipeIn - ссылки на перемещаемые контейнеры с технологическими
картами для ЦЗО и ЦЗП соответственно. **/
    if(!_Purchase) return false;                    // Если в массив с ценами отсутствует, выход с false
    if(!OutCost) return false;                      // Если ЦЗО отсутствует, выход с false
    if(!InCost) return false;                       // Если ЦЗП отсутствует, выход с false
    size_t tmpCount = InCount*clsStorage::ProjectCount();   // Размер массива поступлений на ЦЗП
    if(Purchase) delete[] Purchase;                         // Если массив существует, то удаляем его
    Purchase = new(nothrow) strItem[tmpCount];              // Выделяем память массиву
    if(!Purchase) return false;                             // Если память не выделена, выход с false
    var_cpy(Purchase, _Purchase, tmpCount);                 // Заполняем массив Purchase данными
    RecipeOut = move(_RecipeOut);                           // Перемещаем контейнер с тех.картами
    /** Получаем данные для ввода из ЦЗО в ЦУР (Создаем массив с потребностью в ресурсах для ЦЗО) **/
    strItem* OutPurtmp = cost_centre_purchase(OutCost, move(RecipeOut), _ShipPlan);
    EraseVector(RecipeOut);                                 // Опустошаем вектор
    if(!OutPurtmp)  return false;                           // Если массив не создан, то выход с false
    const strNameMeas* OutNamtmp = OutCost->GetRMNames();   // Получаем указатель на внутренний массив
                                                            // с именами и ед.измерения
    /** Вводим данные в в ЦУР (из массива _Purchase используются только MidCount * PrCount данных) **/
    if(!clsStorage::SetStorage(MidCount, OutNamtmp, OutPurtmp, _Purchase)) {        // Создаем склад,
        delete[] OutPurtmp;                                 // если склад не создан, удаляем массив
        return false;                                       // и выходим с false
    }
    if(OutPurtmp) delete[] OutPurtmp;                       // Удаляем массив за ненадобностью

    /** Сохраняем техкарты для ЦЗП **/
    RecipeIn  = move(_RecipeIn);                            // Перемещаем вектор
    return true;
}   // clsCostCenterStorage::SetStorage with vectors

bool clsCostCenterStorage::Calculate() {
/** Метод рассчитывает объем, удельную и полную себестоимость ресурсов, поступающих на склад и отгружаемых
со склада. Если расчёты завершились удачно (метод вернул true), в массивах Purchase и Shipment будут
содержаться расчётные данные: поступающих на склад и отгружаемых со склада соответственно. **/
    TLack (clsStorage::*f)();                   // Определяем указатель на внутреннюю функцию класса clsStorage
    if((cmeth == sOne) || (cmeth == sTwo)) {    // Выбираем метод расчета ЦУР
        if(cmeth == sOne) f = &clsStorage::Calculate_future;
        else f = &clsStorage::Calculate_thread;
    } else f = &clsStorage::Calculate;

    /** Рассчёт объемов потребления ресурсов в ЦУР **/
    this->clsStorage::Set_progress_message("Расчёт ЦУР");   // Устанавливаем сообщение индикатора
    this->clsStorage::Set_progress_maxcount(MidCount);      // Устанавливаем число итераций для индикатора
    Lack = (this->*f)();                                    // Проводим вычисления объемов потребления ресурсов
    if(Lack.lack > dZero) return false;                     // Если обнаружен дефицит, то выходим и возвращаем false
    strItem* _Purchase = this->clsStorage::GetPure();       // Создаем массив поступлений из ЦЗП в ЦУР
                                                            // размером MidCount * PrCount
    strItem* temp = cost_centre_purchase(InCost, move(RecipeIn), _Purchase);// Получили массив с объемами
                                    // закупок на ЦЗП размером InCount * PrCount (стоимостей в этом массиве нет)
    EraseVector(RecipeIn);          // Опустошаем вектор
    delete[] _Purchase;             // Удаляем вспомогательный массив
    if(!temp) return false;         // Если массив пустой, то выход с false

    /** Формирование ценовых данных для ввода в ЦЗП и расчёта себестоимости на выходе ЦЗП **/
    for(size_t i{}; i< InCount*InCost->GetPrCount(); i++) {
        (Purchase+i)->volume = (temp+i)->volume;                            // Переносим объемы из temp в Purchase
        (Purchase+i)->value = (Purchase+i)->volume * (Purchase+i)->price;   // Рассчитываем поля value
    }                               // Теперь массив Purchase стал окончательным
    delete[] temp;

    /** Расчет себестоимости на выходе ЦЗП **/
    strItem* tmp = cost_centre_shipment(InCost, Purchase);
    if(!tmp) return false;          // Если массив пустой, то выход с false

    /** Вводим цены в ЦУР и рассчитываем себестоимость на выходе ЦУР **/
    clsStorage::SetPurPrice(tmp);   // Вводим цены в ЦУР
    delete[] tmp;                   // Удаляем вспомогательный массив
    this->clsStorage::Set_progress_message("Расчёт ЦУР");   // Устанавливаем сообщение индикатора
    this->clsStorage::Set_progress_maxcount(MidCount);      // Устанавливаем число итераций для индикатора
    (this->*f)();                   // Проводим вычисления себестоимости на выходе ЦУР

    /** Вводим цены в ЦЗО и рассчитываем себестоимость на выходе ЦЗО **/
    tmp = clsStorage::GetShip();    // Получаем себестоимости для ввода в ЦЗО
    if(!tmp) return false;          // Если массив пустой, то выход с false
    if(Shipment) delete[] Shipment;                 // Если массив существует, удаляем его
    Shipment = cost_centre_shipment(OutCost, tmp);  // Получаем массив с объемами и себестоимости на выходе ЦЗО
    delete[] tmp;                                   // Удаляем вспомогательный массив
    if(!Shipment) return false;                     // Если массив пустой, то выход с false
    return true;
}   // clsCostCenterStorage::Calculate

void clsCostCenterStorage::Set_progress_shell(clsProgress_shell<type_progress>* val) {
/** Функция присваивает указателю pshell адрес объекта val **/
    clsStorage::Set_progress_shell(val);
    if(OutCost) OutCost->Set_progress_shell(val);
    if(InCost) InCost->Set_progress_shell(val);
}   // clsCostCenterStorage::Set_progress_shell

TLack clsCostCenterStorage::GetLack() {
/** Метод возвращает размер дефицита и имя ресурса, где он обнаружен **/
    return Lack;
}   // clsCostCenterStorage::GetLuck

size_t clsCostCenterStorage::Counts(count_choise _count) {
/** Метод возвращает число ресурсов выбранного подразделения. Параметры: _count - флаг
выбранного подразделения: inStock - на входе ЦЗП, midStock - в ЦУР, outStock - на выходе ЦЗО. **/
    if((_count == inStock) || (_count == outStock))
        if(_count == outStock) return OutCount;
        else return InCount;
    else return MidCount;
}   // clsCostCenterStorage::Counts

strNameMeas* clsCostCenterStorage::GetNameMeas(count_choise _count) {
/** Метод возвращает указатель на вновь создаваемый массив типа strNameMeas с названием ресурсов и единицами
измерения для выбранного подразделения. Параметры: _count - флаг выбранного подразделения: inStock - на входе
в ЦЗП, midStock - в ЦУР, outStock - на выходе из ЦЗО. Метод удобен тогда, когда есть необходимость корректировки
(например, сокращения длинных названий при выводе списка ресурсов в отчет или на экран. **/
    if((_count == inStock) || (_count == outStock))
        if(_count == outStock) return OutCost == nullptr ? nullptr : OutCost->GetProductDescription();
        else return InCost == nullptr ? nullptr : InCost->GetRawMatDescription();
    else return clsStorage::GetNameMeas();
}   // clsCostCenterStorage::GetNameMeas

const strNameMeas* clsCostCenterStorage::GetNameMeas() {
/** Метод возвращает const- указатель на внутренний массив типа strNameMeas с названием ресурсов
и единицами измерения. Общее число элементов массива равно clsCostCenterStorage::Counts(inStock),
включая отгружаемые со склада и потребляемые складом ресурсы. Из них ресурсы под номерами от нуля
до clsCostCenterStorage::Counts(outStock)-1 поступают и отгружаются со склада, ресурсы под номерами
от clsCostCenterStorage::Counts(outStock) до clsCostCenterStorage::Counts(midStock)-1 потребляются
на выходе со склада (в ЦЗО), а ресурсы под номерами от clsCostCenterStorage::Counts(midStock) до
clsCostCenterStorage::Counts(inStock)-1 потребляются на входе склада (в ЦЗП). **/
    return InCost == nullptr ? nullptr : InCost->GetRMNames();
}   // const clsCostCenterStorage::GetNameMeas

const strItem* clsCostCenterStorage::GetShip() const {
/** Метод возвращает const- указатель на внутренний массив типа strItem с объемами, удельной
и полной себестоимостью ресурсов, отгружаемых со склада. Общее число элементов массива равно
OutCount*clsCostCenterStorage::ProjectCount(). **/
    return Shipment;
}   // clsCostCenterStorage::GetShip

const strItem* clsCostCenterStorage::GetPure() const {
/** Метод возвращает const- указатель на внутренний массив типа strItem с объемами, удельной
и полной себестоимостью ресурсов, поступающих на склад. Общее число элементов массива равно
InCount*clsCostCenterStorage::ProjectCount(). **/
    return Purchase;
}   // clsCostCenterStorage::GetPure

strItem* clsCostCenterStorage::GetInRAC() const {
/** Метод возвращает указатель на вновь создаваемый массив типа strItem с объемами, удельной
и полной себестоимостью ресурсов, поступаемых из ЦЗП (PCC) в ЦУР (RAC). Общее число элементов
массива равно MidCount*clsCostCenterStorage::ProjectCount(). ВНИМАНИЕ!!! После использования
полученного массива, во избежании утечки памяти он требует явного удаления с помощью delete[]. **/
    strItem* temp = clsStorage::GetPure();
    return temp;
}   // clsCostCenterStorage::GetInRAC

strItem* clsCostCenterStorage::GetOutRAC() const {
/** Метод возвращает указатель на вновь создаваемый массив типа strItem с объемами, удельной
и полной себестоимостью ресурсов, поступаемых из ЦУР (RAC) в ЦЗО (SCC). Общее число элементов
массива равно MidCount*clsCostCenterStorage::ProjectCount(). ВНИМАНИЕ!!! После использования
полученного массива, во избежании утечки памяти он требует явного удаления с помощью delete[]. **/
    strItem* temp = clsStorage::GetShip();
    return temp;
}   // clsCostCenterStorage::GetOutRAC

void clsCostCenterStorage::SetCurrency(const Currency& _cur) {
/** Метод устанавливает основную валюту проекта **/
    clsStorage::SetCurrency(_cur);
    if(InCost) InCost->SetCurrency(_cur);
    if(OutCost) OutCost->SetCurrency(_cur);
}   // clsCostCenterStorage::SetCurrency

bool clsCostCenterStorage::Export_Data(string filename, count_choise _dep, ChoiseData _arr, \
    const ReportData& flg) const {
/** Метод записывает массив поставок, остатков или отгрузок ЦЗП, ЦУР или ЦЗО в csv-файл с именем filename.
Параметры: _dep - флаг выбора подразделения: "inStock" - ЦЗП, "midStock" - ЦУР, "outStock" - ЦЗО; _arr
- выбор данных: "purchase" - поставки, "balance" - остатки/незавершенное производство, "shipment" - отгрузки;
flg - тип выводимой в файл информации: volume - в натуральном, value - в стоимостном, price - в ценовом
измерении. В качестве разделителя между полями используется символ _ch по умолчанию (';'). **/
    strItem* (clsStorage::*fSdata)() const;     // Определяем указатель на внутреннюю функцию класса clsStorage
    clsManufactory* val = nullptr;              // Вспомогательный указатель на ЦЗП или ЦЗО
    strItem* (clsManufactory::*fMdata)() const; // Определяем указатель на внутреннюю функцию класса clsManufactory
    const size_t (clsManufactory::*fMcount) () const;   // Тип указателя на функцию возврата числа ресурсов/продуктов
    strNameMeas* (clsManufactory::*fMnames)() const;    // Тип указателя на функцию возврата названий ресурсов/продуктов
    if((_dep == outStock) && (_arr == purchase) && (flg !=volume)) {    // Поступление в денежном выражении в ЦЗО
        _dep = midStock;                                // невозможно получить легким способом, но можно получить
        _arr = shipment;                                // отгрузку из ЦУР: эти данные тождественны
    }
    if(_dep == midStock) {                              // Если выбран ЦУР
        if((_arr==purchase) || (_arr==balance))                 // Назначение указателю функции:
            if(_arr==purchase) fSdata = &clsStorage::GetPure;   // возврата указателя на массив поступлений на склад
            else fSdata = &clsStorage::GetBal;                  // возврата указателя на массив остатков на складе
        else fSdata = &clsStorage::GetShip;                     // возврата указателя на массив отгрузок со склада
    } else {                                            // Выбор ЦЗП или ЦЗО
        if(_dep == inStock) val = InCost;               // Выбор ЦЗП
        else val = OutCost;                             // или ЦЗО
        if(!val) return false;                          // Проверка существования ЦЗ
        if((_arr==balance) || (_arr==shipment)) {
            fMcount = &clsManufactory::GetProdCount;            // указываем на функцию возврата числа продуктов
            fMnames = &clsManufactory::GetProductDescription;   // указываем на функцию возврата описания продуктов
            if(_arr==balance)                                   // в случае незавершенного производства указываем на функцию
                fMdata = &clsManufactory::GetTotalBalance;      // возврата массива с незавершенным производством
            else fMdata = &clsManufactory::GetTotalProduct;     // иначе возврата массива с готовой продукцией
        } else {
            fMcount = &clsManufactory::GetRMCount;              // возврата числа позиций сырья и материалов
            fMnames = &clsManufactory::GetRawMatDescription;    // возврата указателя на новый массив описания сырья и материалов
            fMdata = &clsManufactory::GetRMPurchPlan;           // возврата указателя на новый массив поступлений сырья
        }
    }
    /** Получение данных для экспорта: размер массива с наименованиями и сам массив с наименованиями **/
    size_t NameCount = (_dep == midStock) ? (this->clsStorage::Size()) : ((val->*fMcount)());
    strNameMeas* pNames = (_dep == midStock) ? (this->clsStorage::GetNameMeas()) : ((val->*fMnames)());
    if(!pNames) return false;                                   // Если массив с именами пуст, то выход с false
    strItem* pData;                                             // Вспомогательный указатель
    if((_dep == inStock) && (_arr == purchase) && (flg !=volume)) { // Эти данные берем не из ЦЗП, а из из массива
        size_t tmpCount = NameCount*clsStorage::ProjectCount();     // Purchase, иначе при вызове
        pData = new strItem[tmpCount];                          // clsManufactory::GetRMPurchPlan мы получим
        var_cpy(pData, Purchase, tmpCount);                     // файлы с рулевыми данными
    } else
        pData = (_dep == midStock) ? ((this->*fSdata)()) : ((val->*fMdata)()); // Получаем данные для экспорта
    if(!pData) {                                                // Если массив с данными пуст, то
        delete[] pNames;                                        // удаляем всмомогательный массив с именами
        return false;                                           // и выходим с false
    };
    /** Создаем объект экспорта/импорта и читаем в него данные **/
    clsImpex* Data = new clsImpex(NameCount, pNames, pData, clsStorage::ProjectCount(), flg);
    delete[] pNames;                                            // Удаляем временный массив с именами
    delete[] pData;                                             // Удаляем временный массив с данными
    if(Data->is_Empty()) {                                      // Если контейнер с данными не создан, то
        return false;                                           // и выходим с false
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
}   //

//# undef DEBUG
