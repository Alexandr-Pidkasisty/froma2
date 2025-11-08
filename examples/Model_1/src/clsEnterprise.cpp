#include "clsEnterprise.h"

string DBLR_ind() {
/** Функция возвращает строку символов используемого типа вещественных чисел: "DB" - для вещественных
чисел используется тип double, "LR" - для вещественных чисел используется тип LongReal. Используется
для добавления суффикса к имени выводимых файлов. **/
    if(typeid(decimal) == typeid(double)) return "DB";
    if(typeid(decimal) == typeid(LongReal)) return "LR";
    return "unknown";
}   // DBLR_ind

void strItemReplace(size_t count, strItem* dstn, strItem* src, ReportData flg) {
/** Функция заменяет значения полей, выбранных флагом flg в массиве dstn значениями полей из массива
src у элементов с одинаковым индексом. count - размер массивов **/
    if((!dstn) || (!src)) return;       // Прверка массивов на существование
    for(size_t i{}; i<count; i++) {
        if((flg==volume) || (flg==price))
            if(flg==volume) (dstn+i)->volume = (src+i)->volume;
            else (dstn+i)->price = (src+i)->price;
        else (dstn+i)->value = (src+i)->value;
    }
}   // strItemMerge

bool inData(string &_data, const string _defdata) {
/** Метод вводит строковые данные пользователя. При отсутствии данных, подставляются значения по
умолчанию. Параметры: &_data - ссылка на вводимые пользователем данные, _defdata - значение по
умолчанию. Допускается в качестве параметров &_data и _defdata использовать имя одной и той же
переменной, например inData(x, x), поскольку второй параметр передается в функцию как копия. Метод
возвращает true в случае, если значение по умолчанию было отлично от NoFileName и новое значение
равно NoFileName; в противном случае метод возвращает false. **/
    const char ch = '\n';               // Символ переноса строки
    if (std::cin.peek() == ch) {        // Проверка, что следующий символ перевода строки, если "ДА", то
        _data = _defdata;               // присваиваем значение по умолчанию
    } else                              // Иначе пытаемся прочесть данные
        if(!(std::cin >> _data)) {      // Если ввод неудачный, то
            cout << "Ошибка ввода. Будет присвоено значение " << _defdata << ch;      // сообщаем об ошибке,
            cin.clear();                // сбрасываем флаг ошибки
            _data = _defdata;           // и присваиваем значение по умолчанию
        }
    cin.ignore(numeric_limits<streamsize>::max(), ch); // Очищаем буфер ввода
    if((_defdata != NoFileName) && (_data == NoFileName))
        return true;
    return false;
}   // bool inData

template<typename T, class=std::enable_if_t<is_ch_or_size_t_or_double_v<T>>>
void inData(T &_data, const T _defdata) {
/** Метод вводит данные пользователя. Предназначен для данных, определяемых условием на типы данных
is_ch_or_size_t_or_double_v. При отсутствии данных, подставляются значения по умолчанию. Параметры:
&_data - ссылка на вводимые пользователем данные, _defdata - значение по умолчанию. Допускается в
качестве параметров &_data и _defdata использовать имя одной и той же переменной, например inData(x, x),
поскольку второй параметр передается в функцию как копия. **/
    const char ch = '\n';               // Символ переноса строки
    if (std::cin.peek() == ch) {        // Проверка, что следующий символ перевода строки, если "ДА", то
        _data = _defdata;               // присваиваем значение по умолчанию
    } else                              // Иначе пытаемся прочесть данные
        if(!(std::cin >> _data)) {      // Если ввод неудачный, то
            cout << "Ошибка ввода. Будет присвоено значение " << _defdata << ch;      // сообщаем об ошибке,
            cin.clear();                // сбрасываем флаг ошибки
            _data = _defdata;           // и присваиваем значение по умолчанию
        }
    cin.ignore(numeric_limits<streamsize>::max(), ch); // Очищаем буфер ввода
}   // void inData

string FullFName(string _dir, string _fname) {
/** Метод возвращает полное имя файла. Параметры: _dir - путь к файлу, который может быть добавлен к его имени,
_fname - имя файла. Если имя файла уже содержит путь, т.е. является полным, то путь не добавляется. Иначе метод
возвращает полное имя файла, состоящее из пути и имени. **/
    size_t found = _fname.find('/');    // Ищем вхождение символа "/" в имени файла
    if (found != std::string::npos) {   // Если символ "/" найден, то имени файла уже содержится путь
        return _fname;                  // Возвращаем имя файла без изменения
    }
    return _dir.append(_fname);         // Соединям путь и имя файла и возвращаем полное имя файла
}   // clsEnterprise::FullFName

/****************************************************************************************************/
/**                                 Структура strImportConfig                                      **/
/****************************************************************************************************/

strImportConfig::strImportConfig() {
/** Конструктор по умолчанию **/
    filename_About = EmpStr;
    filename_Shipment = EmpStr;
    filename_Production = EmpStr;
    filename_Purchase = EmpStr;
    filename_Purchase_V = EmpStr;
    filenameprefix_Recipes = EmpStr;
    _ch = ';';
    HeadCols = sTwo;
    HeadRows = sOne;
    _cur = RUR;
    _amethod = FIFO;
    P_Share = dZero;
    S_Share = dZero;
    P_indr = true;
    S_indr = true;
    PsetCount = sZero;
    SsetCount = sZero;
    P_settings = nullptr;
    S_settings = nullptr;
}   // strImportConfig::Ctor

strImportConfig::strImportConfig(const strImportConfig &obj) {
/** Конструктор копирования **/
    filename_About = obj.filename_About;
    filename_Shipment = obj.filename_Shipment;
    filename_Production = obj.filename_Production;
    filename_Purchase = obj.filename_Purchase;
    filenameprefix_Recipes = obj.filenameprefix_Recipes;
    _ch = obj._ch;
    HeadCols = obj.HeadCols;
    HeadRows = obj.HeadRows;
    _cur = obj._cur;
    _amethod = obj._amethod;
    P_Share = obj.P_Share;
    S_Share = obj.S_Share;
    P_indr = obj.P_indr;
    S_indr = obj.S_indr;
    PsetCount = obj.PsetCount;
    SsetCount = obj.SsetCount;
    if(obj.P_settings) { P_settings = new(nothrow) strSettings[PsetCount]; } else P_settings = nullptr;
    if(P_settings) { var_cpy(P_settings, obj.P_settings, PsetCount); };
    if(obj.S_settings) { S_settings = new(nothrow) strSettings[SsetCount]; } else S_settings = nullptr;
    if(S_settings) { var_cpy(S_settings, obj.S_settings, SsetCount); };
}   // strImportConfig::CopyCtor

strImportConfig::strImportConfig(strImportConfig &&obj) {
/** Конструктор перемещения **/
    filename_About = EmpStr;
    filename_Shipment = EmpStr;
    filename_Production = EmpStr;
    filename_Purchase = EmpStr;
    filename_Purchase_V = EmpStr;
    filenameprefix_Recipes = EmpStr;
    _ch = ';';
    HeadCols = sTwo;
    HeadRows = sOne;
    _cur = RUR;
    _amethod = FIFO;
    P_Share = dZero;
    S_Share = dZero;
    P_indr = true;
    S_indr = true;
    PsetCount = sZero;
    SsetCount = sZero;
    P_settings = nullptr;
    S_settings = nullptr;
    swap(obj);
}   // strImportConfig::MoveCtor

strImportConfig& strImportConfig::operator=(const strImportConfig &obj) {
/** Перегрузка оператора присваивания копированием. Реализовано в идеоме
КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/
     if(this == &obj) { return *this; };    // Если объекты идентичны, то ничего не делаем
     strImportConfig tmp(obj);              // Вызываем конструктор копирования и получаем копию obj в переменной tmp
     swap(tmp);                             // Обмениваемся содержанием с tmp
     return *this;
}   // Copy operator=

strImportConfig& strImportConfig::operator=(strImportConfig &&obj) {
/** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
(move-and-swap idiom) **/
    if(this == &obj) { return *this; };     // Если объекты идентичны, то ничего не делаем
    strImportConfig tmp(move(obj));         // Вызываем конструктор перемещения и получаем содержание obj в переменной tmp
    swap(tmp);                              // Обмениваемся содержанием с tmp
    return *this;
}   // Move operator=

strImportConfig::~strImportConfig() {
/** Деструктор **/
    if(P_settings) delete[] P_settings;
    if(S_settings) delete[] S_settings;
}   // strImportConfig::Dtor

bool strImportConfig::SaveToFile(const string _filename) {
/** Метод записи текущей конфигурации в файл **/
    if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
    ofstream outF(confdir + _filename, ios::out| ios::binary);    // Связываем имя файла с файловым потоком для записи на диск
    if (!outF.is_open())                                // Если файл не создан, то
        return false;                                   // выход с false
    if(!SEF(outF, filename_About)) return false;        // Сохраняем имя файла с описанием
    if(!SEF(outF, filename_Shipment)) return false;     // Сохраняем имя файла с отгрузками с СГП
    if(filename_Production == EmpStr) filename_Production = NoFileName; // Проверка на пустое название
    if(!SEF(outF, filename_Production)) return false;   // Сохраняем имя файла с объемами производства
    if(!SEF(outF, filename_Purchase)) return false;         // Сохраняем имя файла с ценами закупок на ССМ
    if(filename_Purchase_V == EmpStr) filename_Purchase_V = NoFileName; // Проверка на пустое название
    if(!SEF(outF, filename_Purchase_V)) return false;       // Сохраняем имя файла с объемами закупок на ССМ
    if(!SEF(outF, filenameprefix_Recipes)) return false;    // Сохраняем префикс для имен файлов с рецептурами
    if(!SEF(outF, _ch)) return false;       // Сохраняем разделитель между данными
    if(!SEF(outF, HeadCols)) return false;  // Сохраняем количество столбцов с заголовками
    if(!SEF(outF, HeadRows)) return false;  // Сохраняем количество строк с заголовками
    if(!SEF(outF, _cur)) return false;      // Сохраняем домашнюю валюту
    if(!SEF(outF, _amethod)) return false;  // Сохраняем принцип учета запасов
    if(!SEF(outF, P_Share)) return false;   // Сохраняем норматив запасов на ССМ
    if(!SEF(outF, S_Share)) return false;   // Сохраняем норматив запасов на СГП
    if(!SEF(outF, P_indr)) return false;    // Сохраняем флаг флаг разрешения поступлений и отгрузок с ССМ в одном периоде
    if(!SEF(outF, S_indr)) return false;    // Сохраняем флаг флаг разрешения поступлений и отгрузок с СГП в одном периоде
    bool flag_P_settings, flag_S_settings;                                  // Флаги существования массивов
    if(P_settings) flag_P_settings = true; else flag_P_settings = false;    // Если массив сществует, то флаг True
    if(S_settings) flag_S_settings = true; else flag_S_settings = false;    // Если массив сществует, то флаг True
    if(!SEF(outF, flag_P_settings)) return false;                           // Сохраняем флаг в файл
    if(!SEF(outF, flag_S_settings)) return false;                           // Сохраняем флаг в файл
    if(!SEF(outF, PsetCount)) return false; // Сохраняем в файл размер массива P_settings
    if(!SEF(outF, SsetCount)) return false; // Сохраняем в файл размер массива S_settings
    if(flag_P_settings)                     // Сохраняем массив в файл
        for(size_t i{}; i<PsetCount; i++)   // поэлементно
            if(!(P_settings+i)->StF(outF)) return false;
    if(flag_S_settings)                     // Сохраняем массив в файл
        for(size_t i{}; i<SsetCount; i++)   // поэлементно
            if(!(S_settings+i)->StF(outF)) return false;
    outF.close();                           // Закрываем файловый поток
    return true;
}   // strImportConfig::SaveToFile

void strImportConfig::swap(strImportConfig& obj) noexcept {
/** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
    std::swap(filename_About, obj.filename_About);
    std::swap(filename_Shipment, obj.filename_Shipment);
    std::swap(filename_Production, obj.filename_Production);
    std::swap(filename_Purchase, obj.filename_Purchase);
    std::swap(filename_Purchase_V, obj.filename_Purchase_V);
    std::swap(filenameprefix_Recipes, obj.filenameprefix_Recipes);
    std::swap(_ch, obj._ch);
    std::swap(HeadCols, obj.HeadCols);
    std::swap(HeadRows, obj.HeadRows);
    std::swap(_cur, obj._cur);
    std::swap(_amethod, obj._amethod);
    std::swap(P_Share, obj.P_Share);
    std::swap(S_Share, obj.S_Share);
    std::swap(P_indr, obj.P_indr);
    std::swap(S_indr, obj.S_indr);
    std::swap(PsetCount, obj.PsetCount);
    std::swap(SsetCount, obj.SsetCount);
    std::swap(P_settings, obj.P_settings);
    std::swap(S_settings, obj.S_settings);
}   // strImportConfig::swap

bool strImportConfig::ReadFromFile(const string _filename) {
/** Метод чтения конфигурации из файла и записи ее в текущий экземпляр структуры **/
    if(_filename == EmpStr) return false;           // Если имя файла пустое, то выход и возврат false
    ifstream inF(confdir + _filename, ios::in | ios::binary); // Связываем имя файла с файловым потоком для чтения с диска
    if (!inF.is_open()) {                           // Если файл не открыт, то сообщение пользователю и
        return false;                               // выход с false
    };
    strImportConfig tmp;                            // Временная переменная, в которую производится чтение
    if(!DSF(inF, tmp.filename_About)) return false; // Читаем имена файлоа,
    if(!DSF(inF, tmp.filename_Shipment)) return false;
    if(!DSF(inF, tmp.filename_Production)) return false;
    if(tmp.filename_Production == NoFileName) filename_Production = EmpStr;
    if(!DSF(inF, tmp.filename_Purchase)) return false;
    if(!DSF(inF, tmp.filename_Purchase_V)) return false;
    if(tmp.filename_Purchase_V == NoFileName) filename_Purchase_V = EmpStr;
    if(!DSF(inF, tmp.filenameprefix_Recipes)) return false;
    if(!DSF(inF, tmp._ch)) return false;            // Читаем разделитель
    if(!DSF(inF, tmp.HeadCols)) return false;       // Читаем количество столбцов с заголовками
    if(!DSF(inF, tmp.HeadRows)) return false;       // Читаем количество строк с заголовками
    if(!DSF(inF, tmp._cur)) return false;           // Читаем домашнюю валюту
    if(!DSF(inF, tmp._amethod)) return false;       // Читаем принцип учета запасов
    if(!DSF(inF, tmp.P_Share)) return false;        // Читаем норматив запасов на ССМ
    if(!DSF(inF, tmp.S_Share)) return false;        // Читаем норматив запасов на СГП
    if(!DSF(inF, tmp.P_indr)) return false; // Читаем флаг флаг разрешения поступлений и отгрузок с ССМ в одном периоде
    if(!DSF(inF, tmp.S_indr)) return false; // Читаем флаг флаг разрешения поступлений и отгрузок с СГП в одном периоде
    bool flag_P_settings, flag_S_settings;                      // Флаги существования массивов
    if(!DSF(inF, flag_P_settings)) return false;                // Читаем флаг из файла
    if(!DSF(inF, flag_S_settings)) return false;                // Читаем флаг из файла
    if(!DSF(inF, tmp.PsetCount)) return false;                  // Читаем из файла размер массива P_settings
    if(!DSF(inF, tmp.SsetCount)) return false;                  // Читаем из файла размер массива S_settings
    if(flag_P_settings) {                                       // Если флаг true, то массив надо прочитать из файла
        strSettings* Ps_temp = new(nothrow) strSettings[tmp.PsetCount]; // Выделяем память для временного массива
        if(!Ps_temp) return false;                              // Если память не выделена, то выход и возврат false
        for(size_t i{}; i<tmp.PsetCount; i++)                   // Если память выделена, то читаем массив из файла
            if(!(Ps_temp+i)->RfF(inF)) return false;            // поэлементно
        std::swap(Ps_temp, tmp.P_settings);                     // Обмениваемся указателями
        if(Ps_temp) delete[] Ps_temp;                           // Удаляем вспомогательный массив
    } else tmp.P_settings = nullptr;                            // Если флаг false, то указатель массива nullptr
    if(flag_S_settings) {                                       // Если флаг true, то массив надо прочитать из файла
        strSettings* Ss_temp = new(nothrow) strSettings[tmp.SsetCount]; // Выделяем память для временного массива
        if(!Ss_temp) return false;                              // Если память не выделена, то выход и возврат false
        for(size_t i{}; i<tmp.SsetCount; i++) {                 // Если память выделена, то читаем массив из файла
            if(!(Ss_temp+i)->RfF(inF)) return false;            // поэлементно
        }
        std::swap(Ss_temp, tmp.S_settings);                     // Обмениваемся указателями
        if(Ss_temp) delete[] Ss_temp;                           // Удаляем вспомогательный массив
    } else tmp.S_settings = nullptr;                            // Если флаг false, то указатель массива nullptr
    swap(tmp);                              // Обмениваемся содержимым с временной переменной
    inF.close();                            // Закрываем файловый поток
    return true;
}   // strImportConfig::ReadFromFile

void strImportConfig::Entry() {
/** Метод ввода конфигурационных данных с терминала **/
    cout << "ВВЕДИТЕ КОНФИГУРАЦИЮ ИМПОРТА\n";
    cout << "В []-скобках представлено значение по умолчанию. Для его ввода просто нажмите Enter\n";
    cout << " имя файла с описанием проекта [" << filename_About << "]: ";
    inData(filename_About, filename_About);
    cout << " имя файла с отгрузками из СГП в натуральном выражении [" << filename_Shipment << "]: ";
    inData(filename_Shipment, filename_Shipment);
    cout << " имя файла с объемами производства, необязательно. Для пропуска введите nofile [" \
        << filename_Production << "]: ";
    if(inData(filename_Production, filename_Production))
        SetToAuto(warehouse);
    cout << " имя файла с ценами закупок на ССМ  [" << filename_Purchase << "]: ";
    inData(filename_Purchase, filename_Purchase);
    cout << " имя файла с объемами закупок на ССМ, необязательно. Для пропуска введите nofile [" \
        << filename_Purchase_V << "]: ";
    if(inData(filename_Purchase_V, filename_Purchase_V))
        SetToAuto(rowmatstock);
    cout << " префикс для имён файлов с рецептурами [" << filenameprefix_Recipes << "]: ";
    inData(filenameprefix_Recipes, filenameprefix_Recipes);
    cout << " символ разделителя между полями в CSV_файлах [" << _ch << "]: ";
    inData(_ch, _ch);
    cout << " число столбцов с заголовками в CSV_файлах [" << HeadCols << "]: ";
    inData(HeadCols, HeadCols);
    if(HeadCols < sTwo) {
        cout << " число столбцов с заголовками не может быть менее двух\n";
        exit(EXIT_FAILURE); // Завершение программы с кодом EXIT_FAILURE
    }
    cout << " число строк с заголовками в CSV_файлах [" << HeadRows << "]: ";
    inData(HeadRows, HeadRows);
    cout << " домашняя валюта проекта, ";
    for(size_t i=sOne; i<Currency::Currency_MAX-sOne; i++)
        cout << CurrencyTXT[i] << "=" << i << ", ";
    cout << CurrencyTXT[Currency::Currency_MAX-sOne] << "=" << Currency::Currency_MAX-sOne << " [" \
        << _cur << "]: ";
    size_t t_cur;
    inData(t_cur, static_cast<size_t>(_cur));
    if((t_cur == Currency::Currency_MIN) || (t_cur >= Currency::Currency_MAX))
        t_cur = sOne;                       // Валидаия введенного значения
    _cur = static_cast<Currency>(t_cur);
    cout << " принцип учета запасов, FIFO=0, LIFO=1, AVG=2 [" << _amethod << "]: ";
    inData(t_cur, static_cast<size_t>(_amethod));
    if(t_cur > sTwo) t_cur = sZero;
    _amethod = static_cast<AccountingMethod>(t_cur);
    double ps_tmp;          // Вспомогательная переменная
    if(((filename_Production == NoFileName) || (filename_Production == EmpStr)) && (!S_settings)) {
        cout << " запас продуктов на складе СГП, доля от отгрузок [" << S_Share << "]: ";
        inData(ps_tmp, S_Share.Get<double>());
        S_Share = ps_tmp;
    }
    if(((filename_Purchase_V == NoFileName) || (filename_Purchase_V == EmpStr)) && (!P_settings)) {
        cout << " запас ресурсов на складе ССМ, доля от отгрузок [" << P_Share << "]: ";
        inData(ps_tmp, P_Share.Get<double>());
        P_Share = ps_tmp;
    }
    if(!S_settings) {
        cout << " флаг разрешения поступлений и отгрузок с СГП в одном периоде, PROHIBITED=0, ALLOWED=1 ["\
            << S_indr << "]: ";
        inData(t_cur, static_cast<size_t>(S_indr));
        if(t_cur <=1)
            S_indr = static_cast<bool>(t_cur);
        else S_indr = true;
    }
    if(!P_settings) {
        cout << " флаг разрешения поступлений и отгрузок с ССМ в одном периоде, PROHIBITED=0, ALLOWED=1 ["\
            << P_indr << "]: ";
        inData(t_cur, static_cast<size_t>(P_indr));
        if(t_cur <=1)
            P_indr = static_cast<bool>(t_cur);
        else P_indr = true;
    }
    strImportConfig::Show();
}   // strImportConfig::Entry

void strImportConfig::Show() {
/** Метод Отображает текущую конфигурацию на экране **/
    cout << "ТЕКУЩАЯ КОНФИГУРАЦИЯ ИМПОРТА\n";
    cout << " имя файла с описанием проекта: " << filename_About << endl;
    cout << " имя файла с отгрузками из СГП в натуральном выражении: " << filename_Shipment << endl;
    if(filename_Production == NoFileName)
        cout << " имя файла с объемами производства: " << EmpStr << endl;
    else
        cout << " имя файла с объемами производства: " << filename_Production << endl;
    cout << " имя файла с ценами закупок на ССМ: " << filename_Purchase << endl;
    if(filename_Purchase_V == NoFileName)
        cout << " имя файла с объемами закупок на ССМ: " << EmpStr << endl;
    else
        cout << " имя файла с объемами закупок на ССМ: " << filename_Purchase_V << endl;
    cout << " префикс для имён файлов с рецептурами: " << filenameprefix_Recipes << endl;
    cout << " символ разделителя между полями в CSV_файлах: " << _ch << endl;
    cout << " число столбцов с заголовками в CSV_файлах: " << HeadCols << endl;
    cout << " число строк с заголовками в CSV_файлах: " << HeadRows << endl;
    cout << " домашняя валюта проекта: " << nmBPTypes::CurrencyTXT[_cur] << endl;
    cout << " принцип учета запасов: " << AccountTXT[_amethod] << endl;
}   // strImportConfig::Show

void strImportConfig::Configure() {
/** Метод читает конфигурацию импорта и, при необходимости редактирует её с последующим
сохранением в конфигурационном файле с именем, содержащимся в переменной Configure_filename**/
    char Answer = 'N';
    if(ReadFromFile(Configure_filename)) {                      // Читаем конфигурацию из файла. Если удачно, то
        strImportConfig::Show();                                // выводим конфигурацию на экран;
        cout << "Редактировать конфигурацию? [Y/N]";            // предлагаем ее скорректировать;
        cin >> Answer;                                          // получаем ответ
        cin.ignore();                                           // Очищаем буфер cin
        if(Answer != 'Y') return;                               // При любом неположительном ответе выходим
        strImportConfig::Entry();                               // Иначе редактируем конфигурацию
        strImportConfig::SaveToFile(Configure_filename);        // и сохраняем новую конфигурацию в файл
        return;
    };
    strImportConfig::Entry();                                   // Редактируем конфигурацию
    strImportConfig::SaveToFile(Configure_filename);            // Сохраняем новую конфигурацию в файл
    return;
}   // strImportConfig::Configure

size_t strImportConfig::GetPsetCount() const {
/** Возвращает размер массива P_settings **/
    return PsetCount;
}   // strImportConfig::GetPsetCount

size_t strImportConfig::GetSsetCount() const {
/** Возвращает размер массива S_settings **/
    return SsetCount;
}   // strImportConfig::GetSsetCount

strSettings* strImportConfig::GetPurSettings() const {
/** Возвращает указатель на новую копию массива P_settings **/
    if(!P_settings) return nullptr;                             // Валидация
    strSettings* temp = new(nothrow) strSettings[PsetCount];    // Выделяем память массиву
    if(!temp) return nullptr;                                   // Если память не выделена, выход с false
    var_cpy(temp, P_settings, PsetCount);                       // Копируем массивы
    return temp;
}   // strImportConfig::GetPurSettings

strSettings* strImportConfig::GetShpSettings() const {
/** Возвращает указатель на новую копию массива S_settings **/
    if(!S_settings) return nullptr;                             // Валидация
    strSettings* temp = new(nothrow) strSettings[SsetCount];    // Выделяем память массиву
    if(!temp) return nullptr;                                   // Если память не выделена, выход с false
    var_cpy(temp, S_settings, SsetCount);                       // Копируем массивы
    return temp;
}   // strImportConfig::GetShpSettings

bool strImportConfig::SetPurSettings(const strSettings* _P_settings, const size_t pcount) {
/** Копирует массив _P_settings в массив P_settings **/
    if((!_P_settings) || (pcount == sZero)) return false;   // Валидация параметров
    strSettings* temp = new(nothrow) strSettings[pcount];   // Выделяем память временному массиву
    if(!temp) return false;                                 // Если память не выделена, то выход с false
    var_cpy(temp, _P_settings, pcount);                     // Копируем данные во временный массив
    std::swap(temp, P_settings);                            // Обмениваемся указателями
    PsetCount = pcount;                                     // Присваиваем новое значение
    if(temp) delete[] temp;                                 // Удаляем вспомогательный массив
    return true;
}   // strImportConfig::SetPurSettings

bool strImportConfig::SetShpSettings(const strSettings* _S_settings, const size_t scount) {
/** Копирует массив _S_settings в массив S_settings **/
    if((!_S_settings) || (scount == sZero)) return false;   // Валидация параметров
    strSettings* temp = new(nothrow) strSettings[scount];   // Выделяем память временному массиву
    if(!temp) return false;                                 // Если память не выделена, то выход с false
    var_cpy(temp, _S_settings, scount);                     // Копируем данные во временный массив
    std::swap(temp, S_settings);                            // Обмениваемся указателями
    SsetCount = scount;                                     // Присваиваем новое значение
    if(temp) delete[] temp;                                 // Удаляем вспомогательный массив
    return true;
}   // strImportConfig::SetPurSettings

void strImportConfig::SetToAuto(const SelectDivision& _dep) {
/** Устанавливает поля calc массива P_settings или S_settings в состояние calc (авторасчет поступлений).
Выбор массива определяется флагом _dep: rowmatstock - для массива P_settings, warehouse - для массива
S_settings **/
    strSettings* tmp;                       // Вспомогательный указатель
    size_t tcount;                          // Вспомогательная переменная
    if(_dep == rowmatstock) {               // Если выбран ССМ, то
        tmp = P_settings;                   // Вспомогательный указатель устанавливаем на P_settings
        tcount = PsetCount;                 // и вспомогательной переменной присваиваем PsetCount
    } else {                                // Иначе отрабатываем S_settings и SsetCount
        tmp = S_settings;
        tcount = SsetCount;
    }
    if((!tmp) || (tcount == sZero)) return; // Если массив не существует Или его размер равен нулю, то выход
    for(size_t i{}; i<tcount; i++)          // Иначе сбрасываем поля calc
        (tmp+i)->calc = calc;               // в автоматический режим расчета
}   // strImportConfig::SetToAuto

/****************************************************************************************************/
/**             Класс clsEnterprise. Наследник класса проекта clsBaseProject                       **/
/****************************************************************************************************/

/********************************** Секция public ***************************************************/

clsEnterprise::clsEnterprise() {
/** Конструктор по умолчанию **/
    PrCount = sZero;
    Cur = RUR;
    Amethod = FIFO;
    purcalc = calc;
    mancalc = calc;
    PurchShare = cShare;
    ShipShare = cShare;
    Purch_indr = true;
    Ship_indr = true;
    ProdCount = sZero;
    Shipment = nullptr;
    Production = nullptr;
    ProdNames = nullptr;
    RMCount = sZero;
    Consumpt = nullptr;
    Purchase = nullptr;
    PriceBus = nullptr;
    RMNames = nullptr;
    Warehouse = nullptr;
    Manufactory = nullptr;
    RawMatStock = nullptr;
    P_settings = nullptr;
    S_settings = nullptr;
}   //Ctor

void clsEnterprise::swap(clsEnterprise& other) noexcept {
/** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
    clsBaseProject::swap(other);                        // Вызываем функцию родительского класса
    std::swap(PrCount, other.PrCount);                  // Обмениваем значения
    std::swap(Cur, other.Cur);                          // Обмениваем значения
    std::swap(Amethod, other.Amethod);                  // Обмениваем значения
    std::swap(purcalc, other.purcalc);                  // Обмениваем значения
    std::swap(mancalc, other.mancalc);                  // Обмениваем значения
    std::swap(PurchShare, other.PurchShare);            // Обмениваем значения
    std::swap(ShipShare, other.ShipShare);              // Обмениваем значения
    std::swap(Purch_indr, other.Purch_indr);            // Обмениваем значения
    std::swap(Ship_indr, other.Ship_indr);              // Обмениваем значения
    std::swap(ProdCount, other.ProdCount);              // Обмениваем значения
    std::swap(Shipment, other.Shipment);                // Обмениваем указатели
    std::swap(Production, other.Production);            // Обмениваем указатели
    std::swap(ProdNames, other.ProdNames);              // Обмениваем указатели
    std::swap(RMCount, other.RMCount);                  // Обмениваем значения
    std::swap(Consumpt, other.Consumpt);                // Обмениваем указатели
    std::swap(Purchase, other.Purchase);                // Обмениваем указатели
    std::swap(PriceBus, other.PriceBus);                // Обмениваем указатели
    std::swap(RMNames, other.RMNames);                  // Обмениваем указатели
    std::swap(Warehouse, other.Warehouse);              // Обмениваем указатели
    std::swap(Manufactory, other.Manufactory);          // Обмениваем указатели
    std::swap(RawMatStock, other.RawMatStock);          // Обмениваем указатели
    std::swap(P_settings, other.P_settings);            // Обмениваем указатели
    std::swap(S_settings, other.S_settings);            // Обмениваем указатели
    Recipe.swap(other.Recipe);                          // Обмен векторов
}   // // swap

void clsEnterprise::EraseVector(vector<clsRecipeItem>& Recipe) {
/** Метод для полной очистки вектора **/
    vector<clsRecipeItem>().swap(Recipe);    // Приводим вектор в состояние по умолчанию
}   // EraseVector

clsEnterprise::clsEnterprise(const clsEnterprise& other) {
/** Конструктор копирования **/
    PrCount = other.PrCount;
    Cur = other.Cur;
    Amethod = other.Amethod;
    purcalc = other.purcalc;
    mancalc = other.mancalc;
    Purchase = other.Purchase;
    ShipShare = other.ShipShare;
    Purch_indr = other.Purch_indr;
    Ship_indr = other.Ship_indr;
    ProdCount = other.ProdCount;
    RMCount = other.RMCount;
    size_t prdtemp = ProdCount*PrCount;
    if(other.Shipment) { Shipment = new(nothrow) strItem[prdtemp]; } else Shipment = nullptr;
    if(Shipment) { var_cpy(Shipment, other.Shipment, prdtemp); };
    if(other.Production) { Production = new(nothrow) strItem[prdtemp]; } else Production = nullptr;
    if(Production) { var_cpy(Production, other.Production, prdtemp); };
    if(other.ProdNames) { ProdNames = new(nothrow) strNameMeas[prdtemp]; } else ProdNames = nullptr;
    if(ProdNames) { var_cpy(ProdNames, other.ProdNames, prdtemp); };
    if(other.Consumpt) { Consumpt = new(nothrow) strItem[prdtemp]; } else Consumpt = nullptr;
    if(Consumpt) { var_cpy(Consumpt, other.Consumpt, prdtemp); };
    if(other.Purchase) { Purchase = new(nothrow) strItem[prdtemp]; } else Purchase = nullptr;
    if(Purchase) { var_cpy(Purchase, other.Purchase, prdtemp); };
    if(other.PriceBus) { PriceBus = new(nothrow) decimal[prdtemp]; } else Purchase = nullptr;
    if(Purchase) { var_cpy(Purchase, other.Purchase, prdtemp); };
    if(other.RMNames) { RMNames = new(nothrow) strNameMeas[prdtemp]; } else RMNames = nullptr;
    if(RMNames) { var_cpy(RMNames, other.RMNames, prdtemp); };
    if(other.Warehouse) { Warehouse = new clsStorage(*other.Warehouse); } else Warehouse = nullptr;
    if(other.Manufactory) { Manufactory = new clsManufactory(*other.Manufactory); } else Manufactory = nullptr;
    if(other.RawMatStock) { RawMatStock = new clsStorage(*other.RawMatStock); } else RawMatStock = nullptr;
    if(other.P_settings) { P_settings = new(nothrow) strSettings[RMCount]; } else P_settings = nullptr;
    if(P_settings) { var_cpy(P_settings, other.P_settings, RMCount); };
    if(other.S_settings) { S_settings = new(nothrow) strSettings[ProdCount]; } else S_settings = nullptr;
    if(S_settings) { var_cpy(S_settings, other.S_settings, ProdCount); };
    Recipe = other.Recipe;
}   //Copy ctor

clsEnterprise::clsEnterprise(clsEnterprise&& other) {
/** Конструктор перемещения **/
    PrCount = sZero;
    Cur = RUR;
    Amethod = FIFO;
    purcalc = calc;
    mancalc = calc;
    PurchShare = cShare;
    ShipShare = cShare;
    Purch_indr = true;
    Ship_indr = true;
    ProdCount = sZero;
    Shipment = nullptr;
    Production = nullptr;
    ProdNames = nullptr;
    RMCount = sZero;
    Consumpt = nullptr;
    Purchase = nullptr;
    PriceBus = nullptr;
    RMNames = nullptr;
    Warehouse = nullptr;
    Manufactory = nullptr;
    RawMatStock = nullptr;
    P_settings = nullptr;
    S_settings = nullptr;
    swap(other);
}   // Move Ctor

clsEnterprise& clsEnterprise::operator=(const clsEnterprise& other) {
/** Перегрузка оператора присваивания копированием. Реализовано в идеоме КОПИРОВАНИЯ-И-ЗАМЕНЫ
(copy-and-swap idiom) **/
    if (this == &other) return *this;       // Если объекты идентичны, то ничего не делаем
    clsEnterprise tmp(other);               // Вызываем конструктор копирования и получаем копию other в переменной tmp
    swap(tmp);                              // Обмениваемся содержанием с tmp
    return *this;
}   // Copy operator=

clsEnterprise& clsEnterprise::operator=(clsEnterprise&& other) {
/** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
(move-and-swap idiom) **/
    if(this == &other) { return *this; };   // Если объекты идентичны, то ничего не делаем
    clsEnterprise tmp(move(other));         // Вызываем конструктор перемещения и получаем копию other в переменной tmp
    swap(tmp);                              // Обмениваемся содержанием с tmp
    return *this;
}   // Move operator=

clsEnterprise::~clsEnterprise() {
/** Деструктор **/
    if(Shipment) delete[] Shipment;
    if(Production) delete[] Production;
    if(ProdNames) delete[] ProdNames;
    if(Consumpt) delete[] Consumpt;
    if(Purchase) delete[] Purchase;
    if(PriceBus) delete[] PriceBus;
    if(RMNames) delete[] RMNames;
    if(Warehouse) delete Warehouse;
    if(Manufactory) delete Manufactory;
    if(RawMatStock) delete RawMatStock;
    if(P_settings) delete[] P_settings;
    if(S_settings) delete[] S_settings;
}   //Dtor

/** Import - методы **/

bool clsEnterprise::Import_Data() {
/** Агрегированный метод импорта. Включает методы конфигурирования импорта, редактирования и сохранения
конфигурации в файл; а также методы импорта данных **/
    strImportConfig ImConfig;                               // Переменная для хранения конфигурации
    ImConfig.Configure();                                   // Читаем/ редактируем конфигурацию
    Cur = ImConfig._cur;                                    // Вводим валюту проекта
    Amethod = ImConfig._amethod;                            // Вводим принцип учета запасов
    if(!Import_About(ImConfig.filename_About)) {            // Вводим название и описание проекта
        return false;                                       // Если не введено, выходим из программы с false
    }
    Purch_indr = ImConfig.P_indr;           // Инициализируем поле с флагом для ССМ
    Ship_indr = ImConfig.S_indr;            // Инициализируем поле с флагом для СГП
    if(!ImportSingleArray(ImConfig.filename_Shipment, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows,\
    volume, Shipment, ProdNames, PrCount, ProdCount)) {
        return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
    }
    size_t tmp1, tmp2;                                      // Временные переменные - "заглушки"
    if(ImportSingleArray(ImConfig.filename_Production, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows,\
    volume, Production, ProdNames, tmp1, tmp2)) {
        mancalc = nocalc;                   // Если импорт удачный, меняем и отображаем флаг расчета закупок
        ShipShare = dZero;                  // и норматив запаса на складе СГП
        ImConfig.S_Share = ShipShare;
    } else {
        ShipShare = ImConfig.S_Share;
        ImConfig.filename_Production = NoFileName;
    }
    if(!ImportSingleArray(ImConfig.filename_Purchase, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows,\
    price, Purchase, RMNames, tmp1, RMCount)) {
        return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
    }
    strItem* tmpPurchase = nullptr;                         // Создаем временный указатель
    std::swap(tmpPurchase, Purchase);                       // Перебрасываем указатель на массив закупочных цен
    strNameMeas* tmpNames;                                  // Временные переменные - "заглушки"
    if(!ImportSingleArray(ImConfig.filename_Purchase_V, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows,\
    volume, Purchase, tmpNames, tmp1, tmp2)) {  // Если импорт неудачный, то возвращаем указатели в исходное состояние
        std::swap(tmpPurchase, Purchase);       // Если импорт неудачный, то возвращаем указатели в исходное состояние
        PurchShare = ImConfig.P_Share;
        ImConfig.filename_Purchase_V = NoFileName;
    } else {
        purcalc = nocalc;                       // Если импорт удачный, меняем и отображаем флаг расчета закупок
        PurchShare = sZero;                     // и норматив запаса на складе СММ
        ImConfig.P_Share = PurchShare;
        strItemReplace(RMCount*PrCount, Purchase, tmpPurchase, nmBPTypes::price);     // При удачном импорте
                        // в поля price нового массива Purchase вводим данные полей price массива tmpPurchase
        delete[] tmpPurchase;                       // Удаляем ненужный массив
        for(size_t i{}; i<(RMCount*PrCount); i++)   // Рассчитываем поля volume массива Purchase
            (Purchase+i)->value = (Purchase+i)->volume * (Purchase+i)->price;
    }
    if(!Import_Recipes(ImConfig.filenameprefix_Recipes, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows)) {
        return false;   // Вводим Инфорацию о рецептурах. Если не введено, выходим из программы с false
    };
    /** Формируем массивы с индивидуальными настройками **/
    if(P_settings) delete[] P_settings;     // Если массив существует, то удаляем его
    P_settings = ImConfig.GetPurSettings(); // и формируем новый массив
    if(S_settings) delete[] S_settings;     // Если массив существует, то удаляем его
    S_settings = ImConfig.GetShpSettings(); // и формируем новый массив
    ImConfig.SaveToFile(Configure_filename);// Сохраняем конфигурацию в файл
    return true;
}   // clsEnterprise::Import_Data

/** Создание и расчет Склада готовой продукции (СГП)  **/

bool clsEnterprise::SetWarehouse() {
/** Метод создания склада готовой продукции (СГП) и ввода параметров. **/
    if((ProdCount==sZero) || (!ProdNames) || (!Shipment)) return false;             // Валидация входных параметров
    clsStorage* temp = new(nothrow) clsStorage(PrCount, Cur, Amethod, ProdCount);   // Выделяем память объекту
    if(!temp) {
        cout << "Склад готовой продукции (СГП) создать не удалось" << endl;         // Если память не выделена, то
        return false;                                                               // выход с false
    }
    bool ch = temp->SetStorage(ProdCount, ProdNames, Shipment, Production); // Вводим параметры склада
    if(!ch) {                                                               // Если склад не создан, то
        delete temp;                                                        // удаляем вспомогательный массив
        return false;                                                       // и выход с false
    }
    if(Warehouse) {                                             // Если массив существовал, то
        Warehouse->swap(*temp);                                 // обмениваем указатели и
        delete temp;                                            // удаляем вспомогательный массив
    } else Warehouse = temp;                                    // Иначе устанавливаем указатель Warehouse на массив

    if(S_settings) {                                            // Если массив с настройками существует, то
        for(size_t i{}; i<ProdCount; i++) {                     // устанавливаем настройки для каждой позиции склада:
            Warehouse->SetShare(i, (S_settings+i)->share);      // норматив запаса готовой продукции
            Warehouse->SetAutoPurchase(i, (S_settings+i)->calc);// флаг автоматического расчета поступлений
            Warehouse->SetPermission(i, (S_settings+i)->perm);  // разрешение на отгрузку и поступление в одном периоде
        }
    } else {                                        // Если же массив не существует, то используем общие настройки:
        for(size_t i{}; i<ProdCount; i++) {         // для каждого продукта СГП устанавливаем
            Warehouse->SetShare(i, ShipShare);      // норматив запаса
            Warehouse->SetAutoPurchase(i, mancalc); // флаг автоматического расчета поступлений
            Warehouse->SetPermission(i, Ship_indr); // разрешение на отгрузку и поступление в одном периоде
        };
    }
    return true;
}   // clsEnterprise::SetWarehouse

void clsEnterprise::StockEditSettings(SelectDivision stk) {
/** Метод редактирует настройки учета склада (СГП или ССМ) для каждого SKU: номер, название и ед. измерения SKU,
флаг авторасчета закупок, флаг разрешения на отгрузку и закупку в одном периоде и норматив запаса
на складе. **/
    size_t num;                                 // Номер номенклатурной позиции, настройки которой надо редактировать
    clsStorage* tmpstock;                       // Вспомогательный указатель
    if(stk == warehouse) tmpstock = Warehouse;  // Если выбран СГП, то указатель направляем на Warehouse,
    else tmpstock = RawMatStock;                // иначе указатель направляем на RawMatStock
    showSKUsettings(std::cout, tmpstock);       // Показываем текущие настройки всех SKU
    while(true) {                               // Редактируем настройки для выбранных SKU
        cout << "Введите номер номенклатурной позиции для редактирования"\
                " [для выхода наберите \"q\" и нажмите \"Enter\"]: ";
        if(!(cin >> num)) break;                // Если номер не введен, то выход из редактора,
        SKUEdt(tmpstock, num);                  // иначе редактируем выбранный SKU
    }
    cin.clear();                                        // Очищаем флаг ошибки
    cin.ignore(numeric_limits<streamsize>::max(),'\n'); // Очищаем буфер ввода
    /** Сохранение индивидуальных настроек в конфигурационный файл и поля текущего экземпляра класса **/
    size_t pcount;                                                  // Вспомогательная переменная
    if(stk == warehouse) pcount = ProdCount;                        // в зависимости от stk равна ProdCount
    else pcount = RMCount;                                          // или RMCount
    strSettings* tmp_stttings = new(nothrow) strSettings[pcount];   // Выделяем память вспомогательному массиву
    if(tmp_stttings) {                                              // Если память выделена, то
        for(size_t i{}; i<pcount; i++) {                            // сохраняем в массив настройки склада;
            (tmp_stttings+i)->share = tmpstock->Share(i);
            (tmp_stttings+i)->calc  = static_cast<PurchaseCalc>(tmpstock->GetAutoPurchase(i));
            (tmp_stttings+i)->perm  = tmpstock->PermissionBool(i);
        }
        strImportConfig ImConfig;                                   // Временная переменная
        typedef bool (strImportConfig::*TypeSetSettings)(const strSettings*, const size_t); // Определение типа
        if(ImConfig.ReadFromFile(Configure_filename)) {             // Читаем конфигурационный файл, если удачно
            TypeSetSettings f;                                      // Вспомогательная переменная для функции
            if(stk == warehouse) f = &strImportConfig::SetShpSettings;  // Выбираем функцию
            else f = &strImportConfig::SetPurSettings;
            (ImConfig.*f)(tmp_stttings, pcount);                    // сохраняем в конфигурации массив настроек
            bool res = ImConfig.SaveToFile(Configure_filename);     // и сохраняем конфигурацию в файл
            if(!res) cout << "Не удалось сохранить конфигурацию в файл " << Configure_filename << endl;
        }
        if(stk == warehouse) std::swap(tmp_stttings, S_settings);   // Обмениваемся состоянием
        else std::swap(tmp_stttings, P_settings);
        if(tmp_stttings) delete[] tmp_stttings;
    }
}   // clsEnterprise::StockEditSettings

bool clsEnterprise::StockCalculate(const SelectDivision& _dep, size_t thr) {
/** Метод рассчитывает требуемый объем закупок/поступлений на склад для тех SKU, у которых выставлен разрешающий
такой расчет флаг. Выбор склада (СГП или ССМ) осуществляется значением параметра _dep: "warehouse" - СГП, любое
другое значение - ССМ. Значением переменной thr устанавливается метод вычисления: "1" -
TLack clsStorage::Calculate_future, "2" - TLack clsStorage::Calculate_thread, иначе TLack clsStorage::Calculate. **/
    TLack (clsStorage::*f)();                           // Определяем указатель на внутреннюю функцию класса clsStorage
    if((thr == 1) || (thr == 2)) {                      // Выбираем фуекцию для рассчетов
        if(thr == 1) f = &clsStorage::Calculate_future; // расчет в асинхронных потоках
        else f = &clsStorage::Calculate_thread;         // расчет в потоках thread
    } else f = &clsStorage::Calculate;                  // иначе последовательные вычисления
    TLack Deficit{};                            // Вспомогательная переменная
    clsStorage* val = nullptr;                  // Вспомогательный указатель
    string dep;                                 // Название склада
    if(_dep == warehouse) {                     // Выбираем склад: СГП
        val = Warehouse;
        dep = "СГП";
    } else {                                    // или ССМ
        val = RawMatStock;
        dep = "ССМ";
    };
    if(!val) return false;                      // Если склад не создан, то выход
    /** Создание и настройка индикатора прогресса **/
    size_t SKUcount = val->Size();              // Установка количества SKU
    type_progress* progress =\
        new type_progress(cout, ProgressWide, "Расчет " + dep, '.', SKUcount);  // Прогресс-бар
    clsProgress_shell<type_progress>* shell =\
        new clsProgress_shell<type_progress>(progress, static_cast<int>(SKUcount));         // Оболочка
    val->Set_progress_shell(shell);             // Устанавливаем индикацию прогресса
    /** Вычисления **/
    Deficit = (val->*f)();                      // Проводим вычисления
    delete progress;                            // Удаляем прогрксс-бар
    delete shell;                               // Удаляем оболочку прогресс-бара
    val->Set_progress_shell(nullptr);           // Устанавливаем внутренний указатель в nullptr
    if(fabs(Deficit.lack) > epsln) {            // Если дефицит условно не равен нулю, то выход с false
        cout << "Поступление на склад и отгрузка с него не сбалансированы. Дефицит равен " <<\
        scientific << Deficit.lack << endl;
        cout << "для продукта с наименованием " << Deficit.Name << endl << endl;
        return false;
    };
    /** Формируем массивы Production & Shipment или Purchase & Consumpt **/
    strItem* tmpPurch = val->GetPure();         // Заполняем вспомогательный массив новыми данными о поступлениях
    strItem* tmpShip = val->GetShip();          // Заполняем вспомогательный массив новыми данными об отгрузках
    if(_dep == warehouse) {
        std::swap(tmpPurch, Production);
        std::swap(tmpShip, Shipment);
    } else {
        std::swap(tmpPurch, Purchase);
        std::swap(tmpShip, Consumpt);
    }
    if(tmpPurch) delete[] tmpPurch;             // Удаляем массив, если он существовал
    if(tmpShip) delete[] tmpShip;               // Удаляем массив, если он существовал
    return true;
}   // clsEnterprise::StockCalculate

/** Создание и расчет производства **/

bool clsEnterprise::SetManufactory() {
/** Метод создания производства и ввода параметров **/
    if((PrCount==sZero) || (RMCount==sZero) || (!RMNames)) return false;                        // Валидация параметров
    clsManufactory* temp = new(nothrow) clsManufactory(PrCount, RMCount, RMNames, ProdCount);   // Выделяем память
    if(!temp) {                                                 // Если память не выделена
        return false;                                           // Выход с false
    };
    temp->SetCurrency(Cur);                                     // Вводим наименование валюты
    size_t i{};                                                 // Индекс продукта (временная переменная)
    for(vector<clsRecipeItem>::const_iterator cit=Recipe.cbegin(); cit!=Recipe.cend(); cit++) {
        bool ch = temp->SetManufItem(*cit, (Production + PrCount*i)); // Создаем единичное производство
                                                                        // для продукта с индексом i
        if(!ch) {                                               // Если производство не создано, то
            delete temp;                                        // удаляем вспомогательный объект
            return false;                                       // и выходим с false
        };
        i++;                                                    // Увеличиваем счётчик продуктов
    };
    if(Manufactory) {                                           // Если массив существовал, то
        Manufactory->swap(*temp);                               // обмениваем указатели и
        delete temp;                                            // удаляем вспомогательный объект
    } else Manufactory = temp;                                  // Иначе устанавливаем указатель Warehouse на массив
    return true;
}   // clsEnterprise::SetManufactory

void clsEnterprise::ManufCalculateIn(size_t thr) {
/** Метод рассчитывает объем потребления сырья и материалов в производстве в натуральном выражении
для всего плана выпуска всех продуктов. Значением переменной thr устанавливается метод вычисления:
"1" - void clsManufactory::CalcRawMatPurchPlan_future, "2" - void clsManufactory::CalcRawMatPurchPlan_thread,
иначе void clsManufactory::CalcRawMatPurchPlan. **/
    void (clsManufactory::*f)();        // Определяем указатель на внутреннюю функцию класса clsManufactory
    if((thr == 1) || (thr == 2)) {      // Выбираем фуекцию для рассчетов:
        if(thr == 1) f = &clsManufactory::CalcRawMatPurchPlan_future;   // расчет в асинхронных потоках
        else f = &clsManufactory::CalcRawMatPurchPlan_thread;           // расчет в потоках thread
    } else f = &clsManufactory::CalcRawMatPurchPlan;                    // иначе последовательные вычисления
    /** Создание и настройка индикатора прогресса **/
    type_progress* progress =\
        new type_progress(cout, ProgressWide, "Расчет поставок в производство", '.', ProdCount);    // Прогресс-бар
    clsProgress_shell<type_progress>* shell =\
        new clsProgress_shell<type_progress>(progress, static_cast<int>(ProdCount));                // Оболочка
    Manufactory->Set_progress_shell(shell);         // Устанавливаем индикацию прогресса
    /** Вычисления **/
    (Manufactory->*f)();                            // Проводим вычисления
    delete progress;                                // Удаляем прогрксс-бар
    delete shell;                                   // Удаляем оболочку прогресс-бара
    Manufactory->Set_progress_shell(nullptr);       // Устанавливаем внутренний указатель в nullptr
    strItem* tempConsumpt = Manufactory->GetRMPurchPlan();  // Получаем обновленный массив поступлений
    if(tempConsumpt)                                        // Если массив создан, то
        std::swap(tempConsumpt, Consumpt);                  // обмениваем указатели
    if(tempConsumpt) delete[] tempConsumpt;                 // Удаляем вспомогательный массив
}   // clsEnterprise::ManufCalculateIn

bool clsEnterprise::ManufCalculateOut(size_t thr) {
/** Метод рассчитывает объем, удельную и полную себестоимость незавершенного производства и
готовой продукции для всех продуктов, выпускаемых на протяжении всего проекта. Значением
переменной thr устанавливается метод вычисления: "1" - bool clsManufactory::Calculate_future(),
"2" - bool clsManufactory::Calculate_thread(), иначе bool clsManufactory::Calculate(). **/
    bool (clsManufactory::*f)();        // Определяем указатель на внутреннюю функцию класса clsManufactory
    if((thr == 1) || (thr == 2)) {      // Выбираем фуекцию для рассчетов
        if(thr == 1) f = &clsManufactory::Calculate_future; // расчет в асинхронных потоках
        else f = &clsManufactory::Calculate_thread;         // расчет в потоках thread
    } else f = &clsManufactory::Calculate;                  // иначе последовательные вычисления
    /** Создание и настройка индикатора прогресса **/
    type_progress* progress =\
        new type_progress(cout, ProgressWide, "Расчет производственной себестоимости", '.', ProdCount);    // Прогресс-бар
    clsProgress_shell<type_progress>* shell =\
        new clsProgress_shell<type_progress>(progress, static_cast<int>(ProdCount));                // Оболочка
    Manufactory->Set_progress_shell(shell);     // Устанавливаем индикацию прогресса
    /** Вычисления **/
    bool result = (Manufactory->*f)();          // Проводим вычисления
    delete progress;                            // Удаляем прогрксс-бар
    delete shell;                               // Удаляем оболочку прогресс-бара
    Manufactory->Set_progress_shell(nullptr);   // Устанавливаем внутренний указатель в nullptr
    /** Заполняем поля price и value в массиве Production **/
    if(result) {
        strItem* tempProduction = Manufactory->GetTotalProduct();   // Получаем обновленный массив отгрузок
        if(!tempProduction) return false;                           // Если нет массива, то выход с false
        std::swap(tempProduction, Production);                      // Обмениваем указатели
        if(tempProduction) delete[] tempProduction;                 // Удаляем вспомогательный массив
    }
    return result;                          // Возвращаем результат
}   // clsEnterprise::ManufCalculateOut

bool clsEnterprise::MWCostTransmition() {
/** Метод передает учетную себестоимость произведенной продукции из производства на склад готовой продукции **/
    if(!Production) return false;           // Если массив не существует (nullptr), то выход с false
    if(!Warehouse->SetPurPrice(Production)) // Вводим учетную себестоимость продукции в СПГ, если нет то
        return false;                       // и выходим с false
    return true;
}   // clsEnterprise::MWCostTransmition

/** Создание  и расчет склада сырья и материалов (ССМ) **/

bool clsEnterprise::SetRawMatStock() {
/** Метод создания склада сырья и материалов (ССМ) и ввода параметров. **/
    if((RMCount==sZero) || (!RMNames) || (!Consumpt)) return false;             // Валидация входных параметров
    clsStorage* temp = new(nothrow) clsStorage(PrCount, Cur, Amethod, RMCount); // Выделяем память объекту
    if(!temp) {                                                                 // Если память не выделена, то
        cout << "Склад Сырья и материалов создать не удалось" << endl;          // сообщаем пользователю и
        return false;                                                           // выходим с false
    }
    bool ch = temp->SetStorage(RMCount, RMNames, Consumpt, Purchase);          // Вводим параметры склада
    if(!ch) {                                       // Если склад не создан, то
        delete temp;                                // удаляем вспомогательный массив
        return false;                               // и выход с false
    }
    if(RawMatStock) {                               // Если массив существовал, то
        RawMatStock->swap(*temp);                   // обмениваем указатели и
        delete temp;                                // удаляем вспомогательный массив
    } else RawMatStock = temp;                      // Иначе устанавливаем указатель Warehouse на массив
    if(P_settings) {                                // Если массив с настройками существует, то
        for(size_t i{}; i<RMCount; i++) {           // устанавливаем настройки для каждой позиции склада:
            RawMatStock->SetShare(i, (P_settings+i)->share);        // норматив запаса готовой продукции
            RawMatStock->SetAutoPurchase(i, (P_settings+i)->calc);  // флаг автоматического расчета поступлений
            RawMatStock->SetPermission(i, (P_settings+i)->perm);    // разрешение на отгрузку и поступление в одном периоде
        }
    } else {                                        // Если же массив не существует, то используем общие настройки:
        for(size_t i{}; i<RMCount; i++) {         // для каждого продукта ССМ устанавливаем
            RawMatStock->SetShare(i, PurchShare);       // норматив запаса
            RawMatStock->SetAutoPurchase(i, purcalc);   // флаг автоматического расчета поступлений
            RawMatStock->SetPermission(i, Purch_indr);  // разрешение на отгрузку и поступление в одном периоде
        };
    }
    return true;
}   // clsEnterprise::SetRawMatStock

bool clsEnterprise::RMCostTransmition() {
/** Метод передает учетную себестоимость сырья и материалов со склада сырья и материалов в производство **/
    PriceBus = RawMatStock->GetShipPrice(); // Указатель на новый массив с учетной себестоимостью сырья
    if(!PriceBus) return false;             // Если массив не создан (nullptr), то выход с false
    if(!Manufactory->SetRawMatPrice(PriceBus)) {    // Вводим учетную себестоимость сырья в производство, если нет то
        return false;                               // и выходим с false
    }
    return true;
}   // clsEnterprise::RMCostTransmition

/** Методы для визуального контроля и отчетов **/

void clsEnterprise::ReportView(const SelectDivision& _rep, const int _arr, const ReportData flg) const {
/** Функция выводит выбранный отчет. Параметры: _rep - выбранный центр затрат (warehouse - СГП, manufactory
- производство, rowmatstock- ССМ), _arr -  тип данных (purchase - массив поступлений. balance - массив остатков,
shipment - массив отгрузок). Параметр flg - тип выводимой информации: volume - в натуральном, value - в стоимостном,
price - в ценовом измерении **/
    bool (clsEnterprise::*f) (clsStorage* obj, const int, const ReportData) const;  // Указатель на функцию
//    Typerepfun f;                                           // Вспомогательный указатель на функцию типа Typerepfun
    if((_rep == warehouse) || (_rep == rowmatstock)) {      // Если выбран СГП или ССМ, то
        f = &clsEnterprise::Report_Storage_to_dev;          // указатель направляем на функцию Report_Storage_to_dev
        if(_rep == warehouse)
            {if((this->*f)(Warehouse, _arr, flg)) return;}  // и выводим отчет по СГП и выходим
        else
            {if((this->*f)(RawMatStock, _arr, flg)) return;}// иначе выводим отчет по ССМ и выходим
    }
    else {
        if(Report_Manufactory(_arr, flg)) return;           // выводим отчет и выходим
    }
}   // clsEnterprise::ReportView

void clsEnterprise::StockSettingsView(const SelectDivision& _rep) {
/** Функция выводит индивидульные настройки склада. Параметры: _rep - выбранный склад (warehouse - СГП,
rowmatstock или любой другой - ССМ) **/
    clsStorage* stock;                  // Вспомогательный указатель
    std::ofstream ofs;                  // Вспомогательная переменная для вывода отчета в файл
    std::ostream* os;                   // Вспомогательная переменная для вывода отчета
    if(Rdevice == nulldev) {            // Если выбрано "пустое" устройство, то
        std::ostream strNull(nullptr);  // формируем пустой поток
        os = &strNull;
    };
    if(Rdevice == terminal) {           // Если выбран вывод на экран, то
        os = &std::cout;                // потоковой переменной os присваиваем ссылку на cout
    };
    if(Rdevice == file) {                       // Если выбран вывод в файл, то
        ofs.open(RName, std::ofstream::app);    // связываем поток с файлом и открываем его на запись
                                                // в режиме добавления к существующему
        os = &ofs;                              // потоковой переменной os присваиваем ссылку на файловый поток
    };
    if(_rep == warehouse) {
        stock = Warehouse;              // Указатель на СГП
    } else {
        stock = RawMatStock;            // Указатель на ССМ
    }
    showSKUsettings(*os, stock);        // Выводим отчет
    *os << '\n';
    if(Rdevice == file) ofs.close();    // Закрываем файл, если это был файловый поток
    return;
}   // clsEnterprise::StockSettingsView

/** Export - методы **/

bool clsEnterprise::Export_Data(string filename, const SelectDivision& _dep, const ChoiseData& _arr, \
    const ReportData& flg) const {
/** Метод записывает массив поставок, остатков или отгрузок со склада готовой продукции (СГП), склада сырья
и материалов (ССМ) или с Производства в csv-файл с именем filename. Параметры: _dep - флаг выбора склада:
"warehouse" - СГП, "rowmatstock" - ССМ, "manufactory" - Производство; _arr - выбор данных: "purchase" -
поставки, "balance" - остатки/незавершенное производство, "shipment" - отгрузки; flg - тип выводимой в файл
информации: volume - в натуральном, value - в стоимостном, price - в ценовом измерении. В качестве разделителя
между полями используется символ _ch по умолчанию (';'). **/
    strItem* (clsStorage::*fSdata)() const;     // Определяем указатель на внутреннюю функцию класса clsStorage
    clsStorage* val = nullptr;                  // Вспомогательный указатель
    strItem* (clsManufactory::*fMdata)() const; // Определяем указатель на внутреннюю функцию класса clsManufactory
    const size_t (clsManufactory::*fMcount) () const;    // Тип указателя на функцию возврата числа ресурсов/продуктов
    strNameMeas* (clsManufactory::*fMnames)() const;     // Тип указателя на функцию возврата названий ресурсов/продуктов
    if(_dep == manufactory) {
        if(!Manufactory) return false;                   // Проверка существования производства
        if((_arr==balance) || (_arr==shipment)) {
            fMcount = &clsManufactory::GetProdCount;            // указываем на функцию возврата числа продуктов
            fMnames = &clsManufactory::GetProductDescription;   // указываем на функцию возврата описания продуктов;
            if(_arr==balance)                                   // в случае незавершенного производства указываем на функцию
                fMdata = &clsManufactory::GetTotalBalance;      // возврата массива с незавершенным производством
            else fMdata = &clsManufactory::GetTotalProduct;     // иначе возврата массива с готовой продукцией
        } else {                                                // Иначе указываем на функции
            fMcount = &clsManufactory::GetRMCount;              // возврата числа позиций сырья и материалов
            fMnames = &clsManufactory::GetRawMatDescription;    // возврата указателя на новый массив описания сырья и материалов
            fMdata = &clsManufactory::GetRMPurchPlan;           // возврата указателя на новый массив поступлений сырья
        }
    } else {
        if(_dep == warehouse) val = Warehouse;  // Выбор склада: СГП
        else val = RawMatStock;                 // или ССМ
        if(!val) return false;                  // Проверка существования склада
        if((_arr==purchase) || (_arr==balance))                 // Назначение указателю функции:
            if(_arr==purchase) fSdata = &clsStorage::GetPure;   // возврата указателя на массив поступлений на склад
            else fSdata = &clsStorage::GetBal;                  // возврата указателя на массив остатков на складе
        else fSdata = &clsStorage::GetShip;                     // возврата указателя на массив отгрузок со склада
    }
    size_t NameCount = (_dep == manufactory) ? ((Manufactory->*fMcount)()) : (val->Size());
    strNameMeas* pNames = (_dep == manufactory) ? ((Manufactory->*fMnames)()) : (val->GetNameMeas());
    if(!pNames) return false;                                   // Если массив с именами пуст, то выход с false
    strItem* pData = (_dep == manufactory) ? ((Manufactory->*fMdata)()) : ((val->*fSdata)());
    if(!pData) {                                                // Если массив с данными пуст, то
        delete[] pNames;                                        // удаляем всмомогательный массив с именами
        return false;                                           // и выходим с false
    };
    clsImpex* Data = new clsImpex(NameCount, pNames, pData, PrCount, flg);  // Создаем объект и читаем в него данные
    delete[] pNames;                            // Удаляем временный массив с именами
    delete[] pData;                             // Удаляем временный массив с данными
    if(Data->is_Empty()) {                      // Если вектор не создан, то
        return false;                           // и выходим с false
    };
    filename.push_back('_');                    // Добавляем к имени файла символ разделителя
    filename.append(DBLR_ind());                // Добавляем признак типа вещественного числа
    filename +=".csv";                          // Добавление расширения файла
    ofstream output(filename, std::ofstream::trunc);    // Открывем файл на запись
    if(output.is_open()) {                              // Если файл открыт, то
        Data->csvExport(output);                        // Записываем данные в файл
        output.close();                                 // Закрываем файл
    } else {
        delete Data;
        return false;
    }
    delete Data;                                // Удаляем объект
    return true;
}   // clsEnterprise::Export_Data

/******************************** Секция protected **************************************************/

/** Import - методы **/

bool clsEnterprise::Import_About(const string _filename) {
/** Метод читает информацию из файла с описанием проекта и формирует поля Title и Descript класса
clsBaseProject. **/
    string filename = FullFName(indir, _filename);  // Формируем полное имя файла
    ifstream input(filename);                       // Открываем файл с именем filename на чтение
    if(input.is_open()) {                           // Если файл открыт:
        stringstream ss;                            // Вспомогательный строковый поток
        const size_t Charlimit = nmRePrint::smblcunt+nmRePrint::uThree; // Задаем максимальную длину строки
        size_t strCount = sZero;                    // Обнуляем счетчик строк
        for(string line; getline(input, line);) {   // Читаем в переменную line каждую строку файла
            if(input.bad()) {                       // Если чтение из файла не удалось, то
                return false;                       // выходим с false
            };
            if(line.length()<=Charlimit) {      // Если длина строки не больше максимальной, то
                ss << move(line) << endl;       // записываем ее в поток, добавляя в него символ конца строки
                strCount++;                     // и увеличиваем счетчик строк
            }
            else {                                          // Если длина строки больше максимальной, то
                size_t nums = sZero;                        // Обнуляем счетчик символов,
                for(size_t i{}; i<line.length(); i++) {     // Читаем строку посимвольно
                    ss << move(line[i]);                    // Переносим каждый символ в строковый поток
                    if(nums==Charlimit) {                   // Как только число символов доходит до максимального:
                        if(((i+1)<line.length()) && (line[i+1] != ' '))   // если следующий символ НЕ пробел, то
                            ss << '-';          // вставляем знак переноса слова
                        nums = sZero;           // Обнуляем счетчик символов,
                        ss << endl;             // Добавляем символ переноса строки
                        strCount++;             // Увеличиваем счетчик строк
                    };
                    nums++;                     // Увеличиваем счетчик символов
                };
                ss << endl;                     // Добавляем символ переноса строки в поток
                strCount++;                     // Увеличиваем счетчик строк
            };
        };
        input.close();                                                      // Закрываем файл
        string* ArrString = new(nothrow) string[strCount];                  // Выделяем память массиву
        if(!ArrString) return false;            // Если память не выделена, то выходим с false
        for(size_t i{}; i<strCount; i++)        // Цикл по количеству строк в потоке
            getline(ss, *(ArrString+i), '\n');  // Читаем в новый массив из потока до символа переноса строки
        SetTitle(*ArrString);                   // Вводим название проекта
        SetComment(ArrString+1, strCount-sOne); // Вводим описание проекта
        delete[] ArrString;                     // Удаляем вспомогательный массив
        return true;
    }
    else return false;
}   // Import_About

bool clsEnterprise::ImportSingleArray(const string _filename, const char _ch, size_t hcols, size_t hrows,\
    ReportData flg, strItem* &_data, strNameMeas* &_names, size_t& ColCount, size_t& RowCount) {
/** Метод читает информацию из файла с именем filename и разделителями между полями ch и заполняет поля:
RowCount - число номенклатурных позиций (ресурсов или продуктов), ColCount - число периодов проекта,
_names - ссылка на указатель на массив с наименованиями номенклатурных позиций и единиц их измерения,
_data - ссылка на указатель на формируемый массив, flg - флаг, определяющий тип импортируемых данных:
"volume" - объемы в натуральном выражении, "price" - цены, "value" - стоимость. **/
    string filename = FullFName(indir, _filename);  // Формируем полное имя файла
    ifstream input(filename);                       // Связываем файл с потоком на чтение
    const char ch = _ch;                        // Выбираем разделитель
    clsImpex* Data = new clsImpex(input, ch);   // Создаем класс для импорта и импортируем данные из файла
    input.close();                              // Закрываем файл с исходными данными
    if(Data->is_Empty()) {                      // Если вектор не создан, то
        delete Data;                            // удаляем объект
        return false;                           // и выходим с false
    };
    ColCount = Data->GetColCount()-hcols;       // Получаем число периодов проекта
    RowCount = Data->GetRowCount()-hrows;       // Получаем число номенклатурных позиций (ресурсов или продуктов)
    strItem* tmpdata;                           // Временная переменная-указатель
    size_t maxRow = RowCount-sOne+hrows;        // Последняя строка
    size_t maxCol = ColCount-sOne+hcols;        // Последний столбец
    if((flg==volume) || (flg==price))           // Получаем указатель на искомый массив с данными
        if(flg==volume) tmpdata = Data->GetstrItem(hrows, maxRow, hcols, maxCol, volume);
        else tmpdata = Data->GetstrItem(hrows, maxRow, hcols, maxCol, price);
    else tmpdata = Data->GetstrItem(hrows, maxRow, hcols, maxCol, value);
    _names = Data->GetNames(hrows, maxRow, hcols-sTwo, hcols-sOne); // Получаем указатель на названия
    delete Data;                                // Удаляем объект для импорта
    std::swap(_data, tmpdata);                  // Перекидываем ссылку на целевой указатель
    return true;
}   // clsEnterprise::ImportSingleArray

bool clsEnterprise::Import_Recipes(const string _filename, const char _ch, size_t hcols, size_t hrows) {
/** Метод читает информацию из файлов с именами, содержащими вначале filename и заканчивающимися на _i, где
i- номер рецептуры. В качестве разделителя используется символ _ch. Метод заполняет вектор рецептур Recipe. **/
    string filename = FullFName(indir, _filename);      // Формируем полное имя файла
    Recipe.reserve(ProdCount);                          // Резервируем память для элементов вектора
    string file;                                        // Временная переменная для имени файла
    ifstream rec;                                       // Поток для чтения из файла
    clsImpex* Data = new(nothrow) clsImpex();           // Создаем экземпляр класса для импорта
    for(size_t i{}; i<ProdCount; i++) {
        file = filename + '_' + to_string(i) + ".csv";  // Формируем имя файла с рецептурой
        rec.open(file);                                 // Связываем поток с файлом
        if(!Data->Import(rec, _ch)) {                   // Импортируем данные из файла. Если импорт не удался,
            rec.close();                                // то закрываем файл;
            delete Data;                                // удаляем экземпляр класса для импорта
            EraseVector(Recipe);                        // приводим вектор к состоянию по умолчанию
            return false;                               // и выходим с false
        };
        rec.close();                                    // Закрываем файл
        string _name = (ProdNames+i)->name;             // Получаем название продукта
        string _meas = (ProdNames+i)->measure;          // Получаем название единицы измерения продукта
        size_t _duration = Data->GetColCount()-hcols;   // Получаем длительность производственного цикла
        size_t _rcount = Data->GetRowCount()-hrows;     // Получаем количество позиций сырья в рецептуре
        size_t maxRow = _rcount-sOne+hrows;             // Последняя строка
        strNameMeas* _rnames = Data->GetNames(hrows, maxRow, hcols-sTwo, hcols-sOne);// Ук. на массив с именами и ед. измерения сырья в рецептуре
        MeasRestore(RMNames, _rnames, RMCount, _rcount);                        // Восстанавливаем ед. измерения сырья
        decimal* _recipeitem = Data->GetDecimal(hrows, maxRow, hcols, hcols);   // Получаем рецептуру продукта
        Recipe.emplace_back(_name, _meas, _duration, _rcount, _rnames, _recipeitem); // Создаем объект "рецептура" в векторе
        delete[] _rnames;                               // Удаляем вспомогательный массив
        delete[] _recipeitem;                           // Удаляем вспомогательный массив
        Data->reset();                                  // Сбрасываем состояние объекта до дефолтного
    };
    delete Data;
    return true;
}   // clsEnterprise::Import_Recipes

/** Методы редактирования **/

bool clsEnterprise::SKUEdt(clsStorage* stock, const size_t num) {
/**  Метод редактирования введенной ранее информации: редактирование номенклатурной позиции
(SKU). Параметры: stock - указатель на конкернтый склад (Warehouse или RawMatStock, num -
номер редактируемой номенклатурной позиции. **/
    size_t choise;                              // Вспомогательная переменная для ввода флагов
    double ps_tmp;                              // Вспомогательная переменная для ввода запаса
    PurchaseCalc tcalc = (stock == Warehouse) ? mancalc : purcalc;          // Контрольное значение
    if(num > (stock->Size()-sOne)) {                                        // Валидация номера SKU
        cout << "Номер номенклатурной позиции указан неверно. Он не должен быть больше " <<\
        (stock->Size()-sOne) << endl;
        return false;
    }
    auto lambda = [&stock, &num](){             // Функция вывода текущей информации на экран
        cout << " наименование позиции:                                     " << stock->Name(num) << endl;
        cout << " единица измерения натурального объема:                    " << stock->Measure(num) << endl;
        cout << " флаг разрешения поступлений и отгрузок в одном периоде:   " << stock->Permission(num) << endl;
        cout << " флаг автоматического расчета поступлений на склад:        " << stock->AutoPurchase(num) << endl;
        cout << " норматив запаса номенклатурной позиции на складе:         " << stock->Share(num) << endl;
    };  // lambda
    cout << "ТЕКУЩИЕ ПАРАМЕТРЫ УЧЕТА ДЛЯ ПОЗИЦИИ № " << num << endl;
    lambda();
    cout << "ВВЕДИТЕ НОВЫЕ ПАРАМЕТРЫ УЧЕТА ДЛЯ ПОЗИЦИИ № " << num << endl;
    cout << " флаг разрешения поступлений и отгрузок в одном периоде [PROHIBITED=0, ALLOWED=1]: ";
    cin >> choise;
    if(choise <=1) stock->SetPermission(num, static_cast<bool>(choise));    // Валидация и ввод параметра
    cout << " флаг автоматического расчета поступлений на склад [AUTO=0, MANUAL=1]: ";
    PurchaseCalc pcl;                           // Вспомогательная переменная для ввода флага
    cin >> choise;
    if(choise <=1) {                            // Валидация и ввод параметра
        pcl = static_cast<PurchaseCalc>(choise);
        if((tcalc == calc) && (pcl == nocalc)) {
            pcl = calc;
            cout << "Режим ручного расчета поступлений установить нельзя: данные о поступлениях не введены!" << endl;
        }
        stock->SetAutoPurchase(num, pcl);
    }
    cout << " норматив запаса номенклатурной позиции на складе (доля от отгрузки)   ";
    cin >> ps_tmp;
    stock->SetShare(num, ps_tmp);
    cout << "НОВЫЕ ПАРАМЕТРЫ УЧЕТА ДЛЯ ПОЗИЦИИ № " << num << endl;
    lambda();
    return true;
}   // clsEnterprise::SKUEdt

/** Методы для визуального контроля и отчетов **/

void clsEnterprise::reportstream(ostream& os) const {
/** Метод выводит информацию о проекте в поток os. Метод переопределяет одноименный метод предка **/
    clsBaseProject::reportstream(os);                   // Вывод базовой информации
    os << "Количество периодов проекта                          " << PrCount << "\n";
    os << "Домашняя валюта проекта                              " << *(nmBPTypes::CurrencyTXT+Cur) << "\n";
    os << "Принцип учета запасов                                " << *(AccountTXT+Amethod) << "\n";
    os << "Количество позиций в ассортименте готовой продукции  " << ProdCount << "\n";
    os << "Количество позиций в ассортименте сырья и материалов " << RMCount << "\n";
    /** Нижестоящий код включить после окончательной отладки программы **/
    os << "\n";
}   // reportstream

void clsEnterprise::showSKUsettings(ostream& os, clsStorage* stock) const {
/** Метод вывода в выходной поток настроек учета на складе для каждого SKU: номер, название и
ед. измерения SKU, флаг авторасчета закупок, флаг разрешения на отгрузку и закупку в одном периоде
и норматив запаса на складе, Параметры: os - поток для вывода, stock - указатель на конкернтый склад
(Warehouse или RawMatStock. **/
//    cin.ignore(numeric_limits < streamsize > ::max(),'\n');
    size_t stock_count = stock->Size();             // Получаем количество SKU
    strNameMeas* tmpNames = stock->GetNameMeas();   // Получаем указатель на вновь созданный массив имен
    for(size_t i{}; i< stock_count; i++) {          // Для каждого наименования
        (tmpNames+i)->name.resize(w1);              // обрезаем длину названий SKU
        (tmpNames+i)->measure.resize(w2);           // обрезаем длину названий единиц измерений
    }
    clsTextField name(w1);  // Установка формата вывода имен с помощью класса clsTextField
    clsTextField meas(w2);  // Установка формата вывода ед. измерения с помощью класса clsTextField
    clsTextField perm(w2);  // Установка формата вывода флага разрешения поступлений и отгрузок в одном периоде
    clsTextField apur(w2);  // Установка формата вывода флага авторасчета поступлений
    clsDataField nSKU(4);   // Установка формата вывода номера SKU и норматива складского запаса
    /** Вывод таблицы с индивидуальными настройками **/
    string msgHead;                                 // Текст заголовка таблицы
    if(stock == Warehouse) msgHead = "Индивидуальные настройки учета продуктов на СГП";
    else msgHead = "Индивидуальные настройки учета ресурсов на ССМ";
    nmRePrint::PrintHeader1(os, nmRePrint::smblcunt+nmRePrint::uThree, msgHead);
    os << nSKU   << "№№"<< stSpc;                   // Вывод заголовка 0-го столбца таблицы
    os << name   << "Название" << stSpc;            // Вывод заголовка 1-го столбца таблицы
    os << meas   << "Ед.измерения" << stSpc;        // Вывод заголовка 2-го столбца таблицы
    os << perm   << "Флаг периода" << stSpc;        // Вывод заголовка 3-го столбца таблицы
    os << apur   << "Авторасчет" << stSpc;          // Вывод заголовка 4-го столбца таблицы
    os << nSKU   << "Запас" << stSpc;               // Вывод заголовка 5-го столбца таблицы
    os << endl;
    for(size_t i{}; i<stock_count; i++) {           // Вывод данных в таблицу
        os << nSKU   << i                      << stSpc;
        os << name   << (tmpNames+i)->name     << stSpc;
        os << meas   << (tmpNames+i)->measure  << stSpc;
        os << perm   << stock->Permission(i)   << stSpc;
        os << apur   << stock->AutoPurchase(i) << stSpc;
        os << nSKU   << stock->Share(i)        << stSpc;
        os << endl;
        if(os.rdbuf() == std::cout.rdbuf())         // Если используемый буфер принадлежит cout, то разбиваем вывод
                                                    // на фрагменты по 10 строк для читабельности с экрана
            if((i!=sZero) && (i!=stock_count-1) &&(i%num_strings) == sZero) {
                os << "Для продолжения вывода таблицы нажмите \"Enter\"";
                cin.ignore();
//                cin.get();
                cin.clear();
            }
    }   // for(size_t i{}; i<stock_count; i++)
    if(tmpNames) delete[] tmpNames;                 // Если массив имен существует, то удаляем его
}   // clsEnterprise::showSKUsettings

bool clsEnterprise::Report_Storage(clsStorage* obj, const int _arr, const ReportData flg) const {
/** Метод для вывода на экран отчетов по Объекту типа clsStorage (Warehouse или RawMatStock). Параметр obj -
экземпляр класса clsStorage. Доступные массивы: _arr = purchase, массив поступлений на склад; _arr = balance,
массив остатков на складе; _arr = shipment, массив отгрузок со склада. Параметр flg - тип выводимой информации:
volume - в натуральном, value - в стоимостном, price - в ценовом измерении. Если значение _arr не относится к
перечисляемому типу из модуля warehouse_module.h enum ChoiseData{purchase = sZero, balance, shipment}, то отчет
не выводится: другие значения _arr предназначены для других методов **/
    size_t NameCount = obj->Size();             // Количество SKU в складе
    if(NameCount == sZero) return false;        // Если склад пустой, то выход с false
    strNameMeas* pNames = obj->GetNameMeas();   // Получаем указатель на новый массив с названиями ресурсов и ед. измерения
    if(!pNames) return false;                   // Если память н выделена, то выход с false
    strItem* (clsStorage::*f)() const;          // Определяем указатель на внутреннюю функцию класса clsStorage
    if(_arr==purchase) f = &clsStorage::GetPure;// Назначение указателю функции:
    else                                        // возврата указателя на массив поступлений на склад
        if(_arr==balance) f = &clsStorage::GetBal;          // возврата указателя на массив остатков на складе
        else
            if(_arr==shipment) f = &clsStorage::GetShip;    // возврата указателя на массив отгрузок со склада
            else {                                          // Если не выбраны purchase, balance или shipment,
                if(pNames) delete[] pNames;                 // удаляем всмомогательный массив с именами
                return false;                               // и выходим с false
            }
    strItem* pData = (obj->*f)();               // Получаем указатель на массив с данными
    if(!pData) {                                // Если массив пустой, то
        if(pNames) delete[] pNames;             // удаляем всмомогательный массив с именами
        return false;                           // и выходим с false
    };
    /** Формируем отчет **/
    std::ofstream ofs;                  // Вспомогательная переменная для вывода отчета в файл
    nmRePrint::clsRePrint<strItem, strNameMeas>* MyRep = new nmRePrint::clsRePrint<strItem, strNameMeas>(w1, w2, w3);
    MyRep->SetHeadings(rTableName, rTableMeas, rByVolume, rByPrice, rByValue);  // Меняем заголовки таблицы
    MyRep->SetCurrency(Cur);                                                    // Установка валюты
    MyRep->SetPrecision(precis);                // Установка количества знаков после запятой
    if(Rdevice == nulldev) {                    // Если выбрано "пустое" устройство, то
        std::ostream strNull(nullptr);          // формируем пустой поток
        MyRep->SetStream(strNull);              // и устанавливаем вывод отчета в пустой поток
    };
    if(Rdevice == terminal)                     // Если выбран вывод на экран, то
        MyRep->SetStream(std::cout);            // устанавливаем вывод отчета на экран
    if(Rdevice == file) {                       // Если выбран вывод в файл, то
        ofs.open(RName, std::ofstream::app);    // связываем поток с файлом и открываем его на запись
                                                // в режиме добавления к существующему
        MyRep->SetStream(ofs);                  // устанавливаем вывод отчета в файловый поток
    };
    if(MyRep->SetReport(NameCount, pNames, pData, PrCount, flg)) {
        MyRep->Print(flg);                      // Генерируем отчет, если получилось,
        if(Rdevice == file) ofs.close();        // Закрываем файл, если это был файловый поток
    }
    else {                                      // в противном случае:
        if(Rdevice == file) ofs.close();        // Закрываем файл, если это был файловый поток
        if(pNames) delete[] pNames;             // удаляем всмомогательный массив
        if(pData) delete[] pData;               // удаляем всмомогательный массив
        if(MyRep) delete MyRep;                 // удаляем отчет
        return false;                           // и выходим с false
    };
    if(pNames) delete[] pNames;                 // Удаляем всмомогательный массив
    if(pData) delete[] pData;                   // Удаляем всмомогательный массив
    if(MyRep) delete MyRep;                     // Удаляем отчет
    return true;
}   // clsEnterprise::Report_Storage

bool clsEnterprise::Report_Storage_to_dev(clsStorage* obj, const int _arr, const ReportData flg) const {
/** Метод направляет отчет, созданный методом Report_Storage на выбранное устройство: "пустое" устройство, экран
или файл (определяется содержанием переменной Rdevice). Использует метод Report_Storage. Параметры совпадают с
параметрами метода Report_Storage. **/
    string divsn;                       // Вспомогательная переменная: название склада
    string swork;                       // Вспомогательная переменная: название выводимого массива
    std::ofstream ofs;                  // Вспомогательная переменная для вывода отчета в файл
    std::ostream* os;                   // Вспомогательная переменная для вывода отчета
    if(Rdevice == nulldev) {            // Если выбрано "пустое" устройство, то
        std::ostream strNull(nullptr);  // формируем пустой поток
        os = &strNull;
    };
    if(Rdevice == terminal) {           // Если выбран вывод на экран, то
        os = &std::cout;                // потоковой переменной os присваиваем ссылку на cout
    };
    if(Rdevice == file) {                       // Если выбран вывод в файл, то
        ofs.open(RName, std::ofstream::app);    // связываем поток с файлом и открываем его на запись
                                                // в режиме добавления к существующему
        os = &ofs;                              // потоковой переменной os присваиваем ссылку на файловый поток
    };
    if(obj == Warehouse) divsn = dWarehouse;    // Строка для идентификации СГП
    else divsn = dRawMatStock;                  // Строка для идентификации ССМ
    if(_arr == purchase) swork = aPurchase;     // Присваиваем вспомогательной переменной
    if(_arr == balance)  swork = aBalance;      // название соответствующего массива
    if(_arr == shipment) swork = aShipment;     // для вывода заголовка отчета
    nmRePrint::PrintHeader2(*os, nmRePrint::smblcunt+nmRePrint::uThree, divsn+swork);   // Выводим заголовок отчета
    if(Rdevice == file) ofs.close();            // Закрываем файл, если это был файловый поток
    return Report_Storage(obj, _arr, flg);      // Выводим отчет
}   // clsEnterprise::Report_Storage_to_dev

void clsEnterprise::Report_Recipe() const {
/** Метод для вывода в отчет рецептур всех продуктов **/
    string swork;                       // Вспомогательная переменная
    std::ofstream ofs;                  // Вспомогательная переменная для вывода отчета в файл
    std::ostream* os;                   // Вспомогательная переменная для вывода отчета на экран
    size_t PrdCnt = Manufactory->GetProdCount();    // Получаем общее число продуктов в производстве
    for(size_t i=sZero; i<PrdCnt; i++) {                                // Цикл по всем единичным производствам
        strNameMeas tprod_id = Manufactory->GetNameItem(i);             // Получаем наименование продукта и единицу измерения
        size_t trcount = Manufactory->GetRCount(i);                     // Получаем число позиций сырья в рецептуре
        const strNameMeas* trnames = Manufactory->GetRawNamesItem(i);   // Получаем указатель на внут. массив с наименованиями сырья в рецептуре
        const decimal* trecipe = Manufactory->GetRecipeItem(i);         // Получаем указатель на внут. массив с рецептурой продукта
        size_t tduration = Manufactory->GetDuration(i);                 // ПОлучаем длительность производственного цикла

                nmRePrint::clsRePrint<decimal, strNameMeas>* MyRep0 =\
                    new(nothrow) nmRePrint::clsRePrint<decimal, strNameMeas>(w1, w2, w3);   // Создаем отчет
                if(!MyRep0) return;                                                         // Если отчет не создан, то выходим
                MyRep0->SetHeadings(rTableName, rTableMeas, rByVolume, rByPrice, rByValue); // Меняем заголовки таблицы
                MyRep0->SetCurrency(Cur);                                                   // Установка валюты
                MyRep0->SetPrecision(precis);                                               // Количество знаков после запятой
                if(Rdevice == nulldev) {            // Если выбрано "пустое" устройство, то
                    std::ostream strNull(nullptr);  // формируем пустой поток
                    MyRep0->SetStream(strNull);     // и устанавливаем вывод отчета в пустой поток
                    os = &strNull;
                };
                if(Rdevice == terminal) {           // Если выбран вывод на экран, то
                    MyRep0->SetStream(std::cout);   // устанавливаем вывод отчета на экран
                    os = &std::cout;
                };
                if(Rdevice == file) {               // Если выбран вывод в файл, то
                    ofs.open(RName, std::ofstream::app);    // связываем поток с файлом и открываем его на запись
                                                            // в режиме добавления к существующему
                    MyRep0->SetStream(ofs);                 // устанавливаем вывод отчета в файловый поток
                    os = &ofs;
                };
                if(MyRep0->SetReport(trcount, trnames, trecipe, tduration)) {
                    *os << aRecipe1 << tprod_id.name << endl;
                    *os << aRecipe2 << tprod_id.measure << endl;
                    for(size_t j=sZero; j<(w1+w2+w3+2); j++) {*os << '-'; };
                    MyRep0->Print();                        // Если отчет сформирован, то печатаем его,
                };
                if(Rdevice == file) ofs.close();        // Закрываем файл, если это был файловый поток
                if(MyRep0) delete MyRep0;
    }
}   // clsEnterprise::Report_Recipe

bool clsEnterprise::Report_Manufactory(const int _arr, const ReportData flg) const {
/** Метод для вывода на экран отчетов по производсту. Доступные массивы: _arr = manpurchase, массив  потребности
в сырье и материалах; _arr = manbalance, массив c балансами незавершенного производства для всех продуктов;
_arr = manshipment, массив отгрузок на СГП; _arr = recipe, рецептуры продуктов. flg - тип выводимой информации:
volume - в натуральном, value - в стоимостном, price - в ценовом измерении. Если значение _arr не относится к
перечисляемому типу из данного модуля enum ManufData{manpurchase = 11, manbalance, manshipment, recipe}, то отчет
не выводится **/
    string swork;                                               // Вспомогательная переменная
    if((Manufactory->GetRMCount()) == sZero) return false;      // Если нет сырья и материалов, то выход с false
    if((Manufactory->GetProdCount()) == sZero) return false;    // Если нет продуктов, то выход с false
    if((Manufactory->GetPrCount()) == sZero) return false;      // Если число периодов проекта равно нулю, то выход с false
    if(_arr == recipe) {                                        // Если выбран отчет "Рецептуры", то
        cout << endl;
        Report_Recipe();                        // Выводим на экран рецептуры всех продуктов
        return true;
    };
    strItem* (clsManufactory::*fs)() const;     // Определяем указатель на внутреннюю функцию класса clsManufactory
    /** Формируем отчет **/
    std::ofstream ofs;                          // Вспомогательная переменная для вывода отчета в файл
    if(_arr == manpurchase) {
        decimal* pData = Manufactory->GetRawMatPurchPlan(); // Получаем массив размером RMCount*PrCount с потребностью в сырье
        if(!pData) return false;
        nmRePrint::clsRePrint<decimal, strNameMeas>* MyRep1 = new nmRePrint::clsRePrint<decimal, strNameMeas>(w1, w2, w3);
        MyRep1->SetHeadings(rTableName, rTableMeas, rByVolume, rByPrice, rByValue); // Меняем заголовки таблицы
        MyRep1->SetCurrency(Cur);                                                   // Установка валюты
        MyRep1->SetPrecision(precis);                                               // Количество знаков после запятой
        bool checkrez = true;                                                       // Индикатор операции
        if(Rdevice == nulldev) {            // Если выбрано "пустое" устройство, то
            std::ostream strNull(nullptr);  // формируем пустой поток
            MyRep1->SetStream(strNull);     // и устанавливаем вывод отчета в пустой поток
        };
        if(Rdevice == terminal)             // Если выбран вывод на экран, то
            MyRep1->SetStream(std::cout);   // устанавливаем вывод отчета на экран
        if(Rdevice == file) {               // Если выбран вывод в файл, то
            ofs.open(RName, std::ofstream::app);    // связываем поток с файлом и открываем его на запись
                                                    // в режиме добавления к существующему
            MyRep1->SetStream(ofs);                 // устанавливаем вывод отчета в файловый поток
        };
        if(MyRep1->SetReport(RMCount, RMNames, pData, PrCount)) {   // Если отчет сформирован, то
            MyRep1->Header2(dManufactory+aPurchase);                // Вывод заголовка
            MyRep1->Print();                                        // печатаем его,
        }
        else checkrez = false;                      // если нет, то меняем индикатор операции на false
        if(Rdevice == file) ofs.close();            // Закрываем файл, если это был файловый поток
        if(pData) delete[] pData;
        if(MyRep1) delete MyRep1;
        return checkrez;
    } else                                                                  // Назначение указателю функции:
        if(_arr == manbalance) {
            fs = &clsManufactory::GetTotalBalance;      // возврата указателя на массив остатков в производстве
            swork = aBalance;
        } else
            if(_arr == manshipment) {
                fs = &clsManufactory::GetTotalProduct;  // возврата указателя на массив отгрузок из производства
                swork = aShipment;
            } else return false;
    strItem* pData = (Manufactory->*fs)();                                  // Получаем указатель на массив с данными
    if(!pData) return false;
    const size_t ProdCount = Manufactory->GetProdCount();                   // Получаем число продуктов
    strNameMeas* ProdNames = Manufactory->GetProductDescription();  // Получаем массив с именами и ед.измерения всех продуктов
    nmRePrint::clsRePrint<strItem, strNameMeas>* MyRep2 = new nmRePrint::clsRePrint<strItem, strNameMeas>(w1, w2, w3);
    MyRep2->SetHeadings(rTableName, rTableMeas, rByVolume, rByPrice, rByValue);         // Меняем заголовки таблицы
    MyRep2->SetCurrency(Cur);
    MyRep2->SetPrecision(precis);
    bool checkrez = true;
    if(Rdevice == nulldev) {            // Если выбрано "пустое" устройство, то
        std::ostream strNull(nullptr);  // формируем пустой поток
        MyRep2->SetStream(strNull);     // и устанавливаем вывод отчета в пустой поток
    };
    if(Rdevice == terminal)             // Если выбран вывод на экран, то
        MyRep2->SetStream(std::cout);   // устанавливаем вывод отчета на экран
    if(Rdevice == file) {               // Если выбран вывод в файл, то
        ofs.open(RName, std::ofstream::app);    // связываем поток с файлом и открываем его на запись
                                                // в режиме добавления к существующему
        MyRep2->SetStream(ofs);                 // устанавливаем вывод отчета в файловый поток
    };
    if(MyRep2->SetReport(ProdCount, ProdNames, pData, PrCount, flg)) {  // Если отчет сформирован, то
        MyRep2->Header2(dManufactory+swork);                            // Вывод заголовка
        MyRep2->Print(flg);                                             // печатаем его,
    }
    else checkrez = false;                      // если нет, то меняем индикатор операции на false
    if(Rdevice == file) ofs.close();            // Закрываем файл, если это был файловый поток
    if(pData) delete[] pData;
    if(MyRep2) delete MyRep2;
    if(ProdNames) delete[] ProdNames;
    return checkrez;
}   // clsEnterprise::Report_Manufactory
