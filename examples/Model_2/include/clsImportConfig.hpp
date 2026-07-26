#ifndef FROMA2_MODEL_2_CLSIMPORTCONFIG_HPP
#define FROMA2_MODEL_2_CLSIMPORTCONFIG_HPP

#include <string>                   // Работа со строками
#include <filesystem>               // Работа с файлами и директориями
#include <regex>                    // Работа с регулярнями выражениями

#include "LongReal_module.h"    // Длинная арифметика
#include "baseproject_module.h" // Подключаем базовый класс clsBaseProject
#include "warehouse_module.h"   // Подключаем класс склада clsStorage
#include "manufact_module.h"    // Подключаем класс производства clsManufactory
#include "common_values.hpp"    // Некоторые функции
#include "Impex_module.h"       // Импорт исходных данных из файлов
#include "pathes.h"             // Пути к файлам отчета, входных и выходных данных

//#define Save_Read_File_debug  // Макрос вывода отладочной информации де- / сериализации

namespace fs = std::filesystem; // Создаем короткий алиас

const string confdir = V_DIR_CONFIG;// Макрос V_DIR_CONFIG определен в файле pathes.h
const string NoFileName = "nofile"; // Имя для отсутствующего файла

template<typename T>                // Ограничение типа для функции inData
constexpr bool is_ch_or_size_t_or_double_v =
    std::is_same<T, char>::value || std::is_same<T, size_t>::value || std::is_same<T, decimal>::value;

template<typename U>                // Ограничение типа для функции ImportSingleArray
constexpr bool is_decimal_bool_or_PurchaseCalc_v =
    std::is_same<U, decimal>::value || std::is_same<U, bool>::value || std::is_same<U, size_t>::value;

/****************************************************************************************************/
/**                                    Вспомогательные функции                                     **/
/****************************************************************************************************/

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
            cout << "Ошибка ввода. Будет присвоено значение " << _defdata << ch; // сообщаем об ошибке,
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

/****************************************************************************************************/
/**                                     class clsImportConfig                                      **/
/****************************************************************************************************/

class clsImportConfig {
/** Класс, формирующий тестовые данные **/
    private:
        /** Конфигурационные данные **/
        string filename_cfg;        // Имя файла конфигурации
        string filename_About;      // Имя файла с описанием проекта
        string filename_shipment;   // Имя файла с отгрузками
        string filename_purprice;   // Имя файла с ценами поставок
        string filename_purvolume;  // Имя файла с объемами поставок в ЦУР (nocalc-режим)
        string filename_recipe_in;  // Префикс для файлов с техкартами ЦЗП
        string filename_recipe_out; // Префикс для файлов с техкартами ЦЗО
        string msks{"_\\d{1,3}\\.csv"}; // Маска regex для поиска файлов техкарт с помощью filesystem

        string filename_shares;         // Имя файла c нормативами запасов на складе
        string filename_permission;     // Имя файла c Флагами разрешения на отгрузку и закупку в одном и том же периоде
        string filename_purchasecalc;   // Имя файла c Флагами авторасчета

//        string Title;           // Название проекта
        Descript About;         // Описание проекта. Тип объявлен в классе clsBaseProject.

        char _ch;               // Символ разделителя между полями в CSV_файлах
        size_t HeadCols;        // Количество столбцов с заголовками в CSV_файлах
        size_t HeadRows;        // Количество строк с заголовками в CSV_файлах
        Currency cur;           // Валюта проекта
        AccountingMethod ac;    // Принцип учёта запасов

        /** Данные для импорта **/
        size_t PrCount;         // Количество периодов проекта
        size_t ProdCount;       // Количество номенклатурных позиций, отгружаемых из ЦЗО
        size_t ResForOutCount;  // Количество номенклатурных позиций на входе-выходе ЦУР (выходе ЦЗП и входе в ЦЗО)
        size_t RMCount;         // Количество номенклатурных позиций на входе в ЦЗП

        strNameMeas* ResNames;  // Указатель на массив с перечнем номенклатурных позиций и их ед. измерения
        strItem* Purchase_p;    // Указатель на массив поступающих в ЦЗП ресурсов (цены)
        strItem* Purchase_v;    // Указатель на массив поступающих в ЦУР ресурсов (объемы)
        strItem* Shipment;      // Указатель на массив отгружаемых из ЦЗО ресурсов
        decimal* Shares;        // Указатель на массив с нормативами запасов на складе
        bool*   Permission;     // Указатель на массив с Флагами разрешения на отгрузку и закупку в одном периоде
        size_t* Purcalc;        // Указатель на массив с Флагами авторасчета

        vector<clsRecipeItem> Out_Recipe;   // Контейнер с тех.картами для ЦЗО
        vector<clsRecipeItem> In_Recipe;    // Контейнер с тех.картами для ЦЗП

        /** Методы **/
        bool ImportSingleArray(const string _filename, const char _ch, size_t hcols, size_t hrows, ReportData flg,\
            strItem* &_data, strNameMeas* &_names, size_t& ColCount, size_t& RowCount) {
        /** Метод читает информацию из файла с именем _filename и разделителями между полями _ch и заполняет поля: RowCount
        - число номенклатурных позиций (ресурсов или продуктов), ColCount - число периодов проекта, _names - ссылка на
        указатель на массив с наименованиями номенклатурных позиций и единиц их измерения, _data - ссылка на указатель на
        формируемый массив, flg - флаг, определяющий тип импортируемых данных: "volume" - объемы в натуральном выражении,
        "price" - цены, "value" - стоимость; hcols и hrows - количество столбцов и строк с заголовками, содержащие названия
        ресурсов и номера периодов проекта. **/
            ifstream input(_filename);                  // Связываем файл с потоком на чтение
            if(!input.is_open()) return false;          // Проверка открытия файла
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
        }   // ImportSingleArray

        template<typename T, class=std::enable_if_t<is_decimal_bool_or_PurchaseCalc_v<T>>>
        bool ImportSingleArray(const string _filename, const char _ch, size_t hcols, size_t hrows, \
            T* &_data, strNameMeas* &_names, size_t& ColCount, size_t& RowCount) {
        /** Метод читает информацию из файла с именем _filename и разделителями между полями _ch и заполняет поля: RowCount
        - число номенклатурных позиций (ресурсов или продуктов), ColCount - число периодов проекта, _names - ссылка на
        указатель на массив с наименованиями номенклатурных позиций и единиц их измерения, _data - ссылка на указатель на
        формируемый массив; hcols и hrows - количество столбцов и строк с заголовками, содержащие названия ресурсов и номера
        периодов проекта. **/
            ifstream input(_filename);                  // Связываем файл с потоком на чтение
            if(!input.is_open()) return false;          // Проверка открытия файла
            const char ch = _ch;                        // Выбираем разделитель
            clsImpex* Data = new clsImpex(input, ch);   // Создаем класс для импорта и импортируем данные из файла
            input.close();                              // Закрываем файл с исходными данными
            if(Data->is_Empty()) {                      // Если вектор не создан, то
                delete Data;                            // удаляем объект
                return false;                           // и выходим с false
            };
            ColCount = Data->GetColCount()-hcols;       // Получаем число периодов проекта
            RowCount = Data->GetRowCount()-hrows;       // Получаем число номенклатурных позиций (ресурсов или продуктов)
            T* tmpdata;                                 // Временная переменная-указатель на массив с данными
            strNameMeas* tmpnames;                      // Временная переменная-указатель на массив с названиями
            size_t maxRow = RowCount-sOne+hrows;        // Последняя строка
            size_t maxCol = ColCount-sOne+hcols;        // Последний столбец
            tmpdata = Data->GetData<T>(hrows, maxRow, hcols, maxCol);           // Получаем указатель на массив с данными
            tmpnames = Data->GetNames(hrows, maxRow, hcols-sTwo, hcols-sOne);   // Получаем указатель на массив с названиями
            delete Data;                                // Удаляем объект для импорта
            std::swap(_data, tmpdata);                  // Перекидываем ссылку на целевой указатель
            std::swap(_names, tmpnames);                // Перекидываем ссылку на целевой указатель
            if(tmpdata) delete[] tmpdata;               // Удаляем вспомогательный массив, если он есть
            if(tmpnames) delete[] tmpnames;             // Удаляем вспомогательный массив, если он есть
            return true;
        }   // template-ImportSingleArray

        bool Import_Recipes(const string _prefixname, const char _ch, size_t hcols, size_t hrows,vector<clsRecipeItem>\
            &_Recipe, size_t _Count) {
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

            const fs::path indata{pth};                         // Папка с файлами рецептур
            if(!fs::exists(indata)) return false;               // Проверяем существование папки
            for(auto &p : fs::directory_iterator(indata)) {     // Поиск в папке
                if(!fs::is_regular_file(p.status())) continue;  // Проверяем, что анйденный файл регулярный (не папка, не ссылка)
                string name((p.path().filename()).string());    // Получаем имя файла
                if(regex_match(name, fmask)) {                  // Проверяем имя файла на совпадение с маской
                    rec.open(p.path());                         // Связываем поток с файлом
                    if(!rec.is_open()) {                        // Если файл не открыт, то
                        delete Data;                            // удаляем экземпляр класса для импорта
                        return false;                           // и выходим с false
                    }
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
        }   // Import_Recipes

        void Show() {
        /** Метод отображает текущую конфигурацию на экране **/
            cout << "ТЕКУЩАЯ КОНФИГУРАЦИЯ ИМПОРТА (" << filename_cfg << ")\n";
            cout << " имя файла с описанием проекта: " << filename_About << endl;
            cout << " имя файла с отгрузками из СГП в натуральном выражении: " << filename_shipment << endl;
            cout << " имя файла с ценами поставок: " << filename_purprice << endl;
            if(filename_purvolume == NoFileName)
                cout << " имя файла с объемами поставок в ЦУР (nocalc-режим): " << EmpStr << endl;
            else
                cout << " имя файла с объемами поставок в ЦУР (nocalc-режим): " << filename_purvolume << endl;
            cout << " префикс для файлов с техкартами ЦЗП: " << filename_recipe_in << endl;
            cout << " префикс для файлов с техкартами ЦЗО: " << filename_recipe_out << endl;
            cout << " маска regex для поиска файлов техкарт: " << msks << endl;
            cout << " имя файла с нормативами запасов на складе: " << filename_shares << endl;
            cout << " имя файла с флагами разрешения на отгрузку и закупку в одном и том же периоде: " << filename_permission << endl;
            cout << " имя файла с флагами авторасчёта: " << filename_purchasecalc << endl;
            cout << " символ разделителя между полями в CSV_файлах: " << _ch << endl;
            cout << " число столбцов с заголовками в CSV_файлах: " << HeadCols << endl;
            cout << " число строк с заголовками в CSV_файлах: " << HeadRows << endl;
            cout << " домашняя валюта проекта: " << nmBPTypes::CurrencyTXT[cur] << endl;
            cout << " принцип учета запасов: " << AccountTXT[ac] << endl;
        }   // Show

        void Entry() {
        /** Метод ввода конфигурационных данных с терминала **/
            cout << "ВВЕДИТЕ КОНФИГУРАЦИЮ ИМПОРТА\n";
            cout << "В []-скобках представлено значение по умолчанию. Для его ввода просто нажмите Enter\n";
            cout << " имя файла с описанием проекта [" << filename_About << "]: ";
            inData(filename_About, filename_About);
            cout << " имя файла с отгрузками из ЦЗО в натуральном выражении [" << filename_shipment << "]: ";
            inData(filename_shipment, filename_shipment);
            cout << " имя файла с закупочными ценами на ЦЗП [" << filename_purprice << "]: ";
            inData(filename_purprice, filename_purprice);
            cout << " имя файла с фиксированными объемами закупок (необязательно). Для пропуска введите nofile [" \
                 << filename_purvolume << "]: ";
            inData(filename_purvolume, filename_purvolume);
            cout << " префикс для имён файлов с тех.картами на ЦЗП [" << filename_recipe_in << "]: ";
            inData(filename_recipe_in, filename_recipe_in);
            cout << " префикс для имён файлов с тех.картами на ЦЗО [" << filename_recipe_out << "]: ";
            inData(filename_recipe_out, filename_recipe_out);
            cout << " маска regex для поиска файлов техкарт [" << msks << "]: ";
            inData(msks, msks);
            cout << " имя файла с нормативами запасов на складе [" << filename_shares << "]: ";
            inData(filename_shares, filename_shares); // !!! Здесь надо проверить неотрицательность значений
            cout << " имя файла с флагами разрешения на отгрузку и закупку в одном и том же периоде [" \
                << filename_permission << "]: ";
            inData(filename_permission, filename_permission);
            cout << " имя файла с флагами авторасчёта [" << filename_purchasecalc << "]: ";
            inData(filename_purchasecalc, filename_purchasecalc);
            cout << " символ разделителя между полями в CSV_файлах [" << _ch << "]: ";
            inData(_ch, _ch);
            cout << " число столбцов с заголовками в CSV_файлах [" << HeadCols << "]: ";
            inData(HeadCols, HeadCols);
            if(HeadCols < sTwo) {
                throw "Ошибка ввода: число столбцов с заголовками не может быть менее двух";
            }
            cout << " число строк с заголовками в CSV_файлах [" << HeadRows << "]: ";
            inData(HeadRows, HeadRows);
            cout << " домашняя валюта проекта, ";
            for(size_t i=sOne; i<Currency::Currency_MAX-sOne; i++)
                cout << CurrencyTXT[i] << "=" << i << ", ";
            cout << CurrencyTXT[Currency::Currency_MAX-sOne] << "=" << Currency::Currency_MAX-sOne << " [" \
                << cur << "]: ";
            size_t t_cur;
            inData(t_cur, static_cast<size_t>(cur));
            if((t_cur == Currency::Currency_MIN) || (t_cur >= Currency::Currency_MAX))
                t_cur = sOne;                       // Валидаия введенного значения
            cur = static_cast<Currency>(t_cur);
            cout << " принцип учета запасов, FIFO=0, LIFO=1, AVG=2 [" << ac << "]: ";
            inData(t_cur, static_cast<size_t>(ac));
            if(t_cur > sTwo) t_cur = sZero;
            ac = static_cast<AccountingMethod>(t_cur);
            Show();
        }   // Entry

        bool SCdtF(ofstream &outF) {
        /** Метод сериализации конфигурационных данных в файловый поток. Параметры: outF - ссылка на файловый поток на запись **/
            #ifdef Save_Read_File_debug
                long bpos = outF.tellp();                                      // Определяем позицию в начале файла
                cout << "clsImportConfig::SCtF begin bpos= " << bpos << endl;   // Выводим позицию на экран
            #endif // Save_Read_File_debug
            if(!SEF(outF, filename_About)) return false;        // Сохраняем имя файла с описанием
            if(!SEF(outF, filename_shipment)) return false;
            if(!SEF(outF, filename_purprice)) return false;
            if(filename_purvolume == EmpStr) filename_purvolume = NoFileName;   // Проверка на пустое название
            if(!SEF(outF, filename_purvolume)) return false;
            if(!SEF(outF, filename_recipe_in)) return false;
            if(!SEF(outF, filename_recipe_out)) return false;
            if(!SEF(outF, msks)) return false;
            if(!SEF(outF, filename_shares)) return false;
            if(!SEF(outF, filename_permission)) return false;
            if(!SEF(outF, filename_purchasecalc)) return false;
            if(!SEF(outF, _ch)) return false;       // Сохраняем разделитель между данными
            if(!SEF(outF, HeadCols)) return false;  // Сохраняем количество столбцов с заголовками
            if(!SEF(outF, HeadRows)) return false;  // Сохраняем количество строк с заголовками
            if(!SEF(outF, PrCount)) return false;   // Сохраняем количество периодов проекта
            if(!SEF(outF, cur)) return false;
            if(!SEF(outF, ac)) return false;        // Сохраняем принцип учета запасов
            #ifdef Save_Read_File_debug
                long epos = outF.tellp();                   // Определяем позицию в начале файла
                cout << "clsImportConfig::SCtF end epos= " << epos << endl;
            #endif // Save_Read_File_debug
            return true;
        }   // SCdtF

        bool RCdfF(ifstream &inF) {
        /** Метод десериализации конфигурационных данных из файлового потока. Параметры: inF - ссылка на файловый поток на чтение**/
            clsImportConfig tmp(filename_cfg);  // Буфферный объект, копия текущего
            #ifdef Save_Read_File_debug
                long bpos = inF.tellg();                                    // Определяем позицию в начале файла
                cout << "clsImportConfig::RCdfF begin bpos= " << bpos << endl;  // Выводим позицию на экран
            #endif // Save_Read_File_debug
            if(!DSF(inF, tmp.filename_About)) return false; // Читаем имя файла с описанием
            if(!DSF(inF, tmp.filename_shipment)) return false;
            if(!DSF(inF, tmp.filename_purprice)) return false;
            if(!DSF(inF, tmp.filename_purvolume)) return false;
            if(!DSF(inF, tmp.filename_recipe_in)) return false;
            if(!DSF(inF, tmp.filename_recipe_out)) return false;
            if(!DSF(inF, tmp.msks)) return false;
            if(!DSF(inF, tmp.filename_shares)) return false;
            if(!DSF(inF, tmp.filename_permission)) return false;
            if(!DSF(inF, tmp.filename_purchasecalc)) return false;
            if(!DSF(inF, tmp._ch)) return false;            // Читаем разделитель
            if(!DSF(inF, tmp.HeadCols)) return false;       // Читаем количество столбцов с заголовками
            if(!DSF(inF, tmp.HeadRows)) return false;       // Читаем количество строк с заголовками
            if(!DSF(inF, tmp.PrCount)) return false;
            if(!DSF(inF, tmp.cur)) return false;
            if(!DSF(inF, tmp.ac)) return false;
            swap(tmp);                          // Обмениваемся состояниями с временным объектом
            #ifdef Save_Read_File_debug
                long epos = inF.tellg();                                // Определяем позицию в начале файла
                cout << "clsImportConfig::RCdfF end epos= " << epos << endl;
            #endif // Save_Read_File_debug
            return true;
        }   // RCdfF

        bool import_about(const string _filename) {
        /** Метод читает информацию из файла с описанием проекта и формирует поля Title и Descript класса
        clsBaseProject. **/
            ifstream input(_filename);                      // Открываем файл с именем filename на чтение
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
                input.close();                          // Закрываем файл
                getline(ss, Title, '\n');               // Читаем название проекта из потока до символа переноса строки
                strCount--;                             // Уменьшаем счётчик на единицу
                string* ArrString = new(nothrow) string[strCount];  // Выделяем память массиву
                if(!ArrString) return false;            // Если память не выделена, то выходим с false
                for(size_t i{}; i<strCount; i++)        // Цикл по количеству строк в потоке
                    getline(ss, *(ArrString+i), '\n');  // Читаем в новый массив из потока до символа переноса строки
                About.sCount = strCount;                // Вводим размер массива с комментариями
                std::swap(About.sComment, ArrString);   // Обмениваемся указателями
                return true;
            }
            else return false;
        }   // import_about

    public:

        string Title;           // Название проекта

        clsImportConfig() {
        /** Конструктор по умолчанию **/
            filename_cfg = "config.cfg";
            filename_About = EmpStr;
            filename_shipment = EmpStr;
            filename_purprice = EmpStr;
            filename_purvolume = EmpStr;
            filename_recipe_in = EmpStr;
            filename_recipe_out = EmpStr;
            msks  = "_\\d{1,3}\\.csv";
            filename_shares = EmpStr;
            filename_permission = EmpStr;
            filename_purchasecalc = EmpStr;
            Title = EmpStr;
            About.sCount = sZero;
            About.sComment = nullptr;
            _ch = ';';
            HeadCols = sTwo;
            HeadRows = sOne;
            PrCount = sZero;
            cur = RUR;
            ac = AVG;
            ProdCount = sZero;
            ResForOutCount = sZero;
            RMCount = sZero;
            ResNames = nullptr;
            Purchase_p = nullptr;
            Purchase_v = nullptr;
            Shipment = nullptr;
            Shares = nullptr;
            Permission = nullptr;
            Purcalc = nullptr;
        }   // Default Ctor

//        clsImportConfig(const string& _key, const string& _filename_cfg) {
        clsImportConfig(const string& _filename_cfg) {
        /** Конструктор с параметрами. Параметры: _filename_cfg - имя файла конфигурации. **/
            filename_cfg = _filename_cfg;
            filename_About = EmpStr;
            filename_shipment = EmpStr;
            filename_purprice = EmpStr;
            filename_purvolume = EmpStr;
            filename_recipe_in = EmpStr;
            filename_recipe_out = EmpStr;
            msks  = "_\\d{1,3}\\.csv";
            filename_shares = EmpStr;
            filename_permission = EmpStr;
            filename_purchasecalc = EmpStr;
            Title = EmpStr;
            About.sCount = sZero;
            About.sComment = nullptr;
            _ch = ';';
            HeadCols = sTwo;
            HeadRows = sOne;
            PrCount = sZero;
            cur = RUR;
            ac = AVG;
            ProdCount = sZero;
            ResForOutCount = sZero;
            RMCount = sZero;
            ResNames = nullptr;
            Purchase_p = nullptr;
            Purchase_v = nullptr;
            Shipment = nullptr;
            Shares = nullptr;
            Permission = nullptr;
            Purcalc = nullptr;
        }   // Ctor with parameters

        void swap(clsImportConfig& obj) noexcept {
        /** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
            filename_cfg.swap(obj.filename_cfg);
            filename_About.swap(obj.filename_About);
            filename_shipment.swap(obj.filename_shipment);
            filename_purprice.swap(obj.filename_purprice);
            filename_purvolume.swap(obj.filename_purvolume);
            filename_recipe_in.swap(obj.filename_recipe_in);
            filename_recipe_out.swap(obj.filename_recipe_out);
            msks.swap(obj.msks);
            filename_shares.swap(obj.filename_shares);
            filename_permission.swap(obj.filename_permission);
            filename_purchasecalc.swap(obj.filename_purchasecalc);
            Title.swap(obj.Title);
            std::swap(About.sCount, obj.About.sCount);
            std::swap(About.sComment, obj.About.sComment);
            std::swap(_ch, obj._ch);
            std::swap(HeadCols, obj.HeadCols);
            std::swap(HeadRows, obj.HeadRows);
            std::swap(PrCount, obj.PrCount);
            std::swap(cur, obj.cur);
            std::swap(ac, obj.ac);
            std::swap(ProdCount, obj.ProdCount);
            std::swap(ResForOutCount, obj.ResForOutCount);
            std::swap(RMCount, obj.RMCount);
            std::swap(ResNames, obj.ResNames);
            std::swap(Purchase_p, obj.Purchase_p);
            std::swap(Purchase_v, obj.Purchase_v);
            std::swap(Shipment, obj.Shipment);
            std::swap(Shares, obj.Shares);
            std::swap(Permission, obj.Permission);
            std::swap(Purcalc, obj.Purcalc);
            Out_Recipe.swap(obj.Out_Recipe);
            In_Recipe.swap(obj.In_Recipe);
        }   // swap

        clsImportConfig(const clsImportConfig &obj) {
        /** Конструктор копирования **/
            filename_cfg = obj.filename_cfg;
            filename_About = obj.filename_About;
            filename_shipment = obj.filename_shipment;
            filename_purprice = obj.filename_purprice;
            filename_purvolume = obj.filename_purvolume;
            filename_recipe_in = obj.filename_recipe_in;
            filename_recipe_out = obj.filename_recipe_out;
            msks = obj.msks;
            filename_shares = obj.filename_shares;
            filename_permission = obj.filename_permission;
            filename_purchasecalc = obj.filename_purchasecalc;
            Title = obj.Title;
            About.sCount = obj.About.sCount;
            if(obj.About.sComment) {
                About.sComment = new(nothrow) string[About.sCount];
                if(About.sComment) var_cpy(About.sComment, obj.About.sComment, About.sCount);
            } else About.sComment = nullptr;
            _ch = obj._ch;
            HeadCols = obj.HeadCols;
            HeadRows = obj.HeadRows;
            PrCount = obj.PrCount;
            cur = obj.cur;
            ac = obj.ac;
            ProdCount = obj.ProdCount;
            ResForOutCount = obj.ResForOutCount;
            RMCount = obj.RMCount;
            if(obj.ResNames) {
                ResNames = new(nothrow) strNameMeas[RMCount];
                if(ResNames) var_cpy(ResNames, obj.ResNames, RMCount);
            } else ResNames = nullptr;
            if(obj.Purchase_p) {
                Purchase_p = new(nothrow) strItem[RMCount];
                if(Purchase_p) var_cpy(Purchase_p, obj.Purchase_p, RMCount);
            } else Purchase_p = nullptr;
            if(obj.Purchase_v) {
                Purchase_v = new(nothrow) strItem[ResForOutCount];
                if(Purchase_v) var_cpy(Purchase_v, obj.Purchase_v, ResForOutCount);
            } else Purchase_v = nullptr;
            if(obj.Shipment) {
                Shipment = new(nothrow) strItem[ProdCount];
                if(Shipment) var_cpy(Shipment, obj.Shipment, ProdCount);
            } else Shipment = nullptr;
            if(obj.Shares) {
                Shares = new(nothrow) decimal[ResForOutCount];
                if(Shares) var_cpy(Shares, obj.Shares, ResForOutCount);
            } else Shares = nullptr;
            if(obj.Permission) {
                Permission = new(nothrow) bool[ResForOutCount];
                if(Permission) var_cpy(Permission, obj.Permission, ResForOutCount);
            } else Permission = nullptr;
            if(obj.Purcalc) {
                Purcalc = new(nothrow) size_t[ResForOutCount];
                if(Purcalc) var_cpy(Purcalc, obj.Purcalc, ResForOutCount);
            } else Purcalc = nullptr;
            Out_Recipe = obj.Out_Recipe;
            In_Recipe = obj.In_Recipe;
        }   // Copy-Ctor

        clsImportConfig(clsImportConfig &&obj) {
        /** Конструктор перемещения **/
            filename_cfg = "config.cfg";
            filename_About = EmpStr;
            filename_shipment = EmpStr;
            filename_purprice = EmpStr;
            filename_purvolume = EmpStr;
            filename_recipe_in = EmpStr;
            filename_recipe_out = EmpStr;
            msks  = "_\\d{1,3}\\.csv";
            filename_shares = EmpStr;
            filename_permission = EmpStr;
            filename_purchasecalc = EmpStr;
            Title = EmpStr;
            About.sCount = sZero;
            About.sComment = nullptr;
            _ch = ';';
            HeadCols = sTwo;
            HeadRows = sOne;
            PrCount = sZero;
            cur = RUR;
            ac = AVG;
            ProdCount = sZero;
            ResForOutCount = sZero;
            RMCount = sZero;
            ResNames = nullptr;
            Purchase_p = nullptr;
            Purchase_v = nullptr;
            Shipment = nullptr;
            Shares = nullptr;
            Permission = nullptr;
            Purcalc = nullptr;
            swap(obj);
        }   // Move-Ctor

        clsImportConfig& operator=(const clsImportConfig &obj) {
        /** Перегрузка оператора присваивания копированием. Реализовано в идеоме
        КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/
            if(this == &obj) { return *this; };    // Если объекты идентичны, то ничего не делаем
            clsImportConfig tmp(obj);              // Вызываем конструктор копирования и получаем копию obj в переменной tmp
            swap(tmp);                             // Обмениваемся содержанием с tmp
            return *this;
        }   // Copy operator=

        clsImportConfig& operator=(clsImportConfig &&obj) {
        /** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
        (move-and-swap idiom) **/
            if(this == &obj) { return *this; };     // Если объекты идентичны, то ничего не делаем
            clsImportConfig tmp(move(obj));         // Вызываем конструктор перемещения и получаем содержание obj в переменной tmp
            swap(tmp);                              // Обмениваемся содержанием с tmp
            return *this;
        }   // Move operator=

        ~clsImportConfig() {
        /** Деструктор **/
            if(About.sComment) delete[] About.sComment;
            if(ResNames) delete[] ResNames;
            if(Purchase_p) delete[] Purchase_p;
            if(Purchase_v) delete[] Purchase_v;
            if(Shipment) delete[] Shipment;
            if(Shares) delete[] Shares;
            if(Permission) delete[] Permission;
            if(Purcalc) delete[] Purcalc;
        }   // Dtor

        bool SaveConfigToFile(const string _filename) {
        /** Метод записи конфигурационных данных в файл **/
            if(_filename == EmpStr) return false;                       // Если имя файла пустое, то выход и возврат false
            ofstream outF(confdir + _filename, ios::out| ios::binary);  // Связываем имя файла с файловым потоком для записи на диск
            if (!outF.is_open()) {                                      // Если файл не открыт, то
                cout << "Ошибка открытия файла " << (confdir + _filename) << endl;  // сообщение пользователю и
                return false;                                                       // возврат false и выход из функции
            }
            if(!SCdtF(outF)) {                                          // Если запись в файл неудачна, то
                outF.close();                                           // закрываем файл
                return false;                                           // и выходим с false
            }
            outF.close();
            return true;
        }   // SaveConfigToFile

        bool ReadConfigFromFile(const string _filename) {
        /** Метод чтения конфигурационных данных из файла **/
            if(_filename == EmpStr) return false;                       // Если имя файла пустое, то выход и возврат false
            ifstream inF(confdir + _filename, ios::in | ios::binary);   // Связываем имя файла с файловым потоком для чтения с диска
            if (!inF.is_open())                                         // Если файл не открыт, то
                return false;                                           // выход из функции с false
            if(!RCdfF(inF)) {                                           // Если запись в файл неудачна, то
                inF.close();                                            // закрываем файл
                return false;                                           // и выходим с false
            }
            inF.close();
            return true;
        }   // ReadConfigFromFile

        void Configure() {
        /** Метод читает конфигурацию импорта и, при необходимости редактирует её с последующим
        сохранением в конфигурационном файле с именем, содержащимся в переменной filename_cfg **/
            char Answer = 'N';
            if(ReadConfigFromFile(filename_cfg)) {  // Читаем конфигурацию из файла. Если удачно, то
                Show();                     // выводим конфигурацию на экран;
                cout << "Редактировать конфигурацию? [Y/N]";    // предлагаем ее скорректировать;
                cin >> Answer;              // получаем ответ
                cin.ignore();               // Очищаем буфер cin
                if(Answer != 'Y') return;   // При любом неположительном ответе выходим
                Entry();                    // Иначе редактируем конфигурацию
                SaveConfigToFile(filename_cfg); // и сохраняем новую конфигурацию в файл
                return;
            };
            Entry();                            // Редактируем конфигурацию
            SaveConfigToFile(filename_cfg);     // Сохраняем новую конфигурацию в файл
            return;
        }   // Configure

        bool Import_Data(Currency _cur, AccountingMethod _ac) {
        /** Метод читает информацию из файлов с исходными данными. Метод также вводит основные параметры:
        _cur - валюту проекта и _ac - принцип учета запасов. **/
            bool bcheck;                                // Вспомогательная переменная
            clsImportConfig tmp(*this);                 // Создаем буфферный объект, как копию текущего
            string InputAbout, InputShipment, InputPurprice, InputPurvolm, InputShares,
                InputPermission, InputPurchasecalc;     // Переменные для имен файлов
            (InputAbout.assign(V_DIR_INPUTDATA)).append(tmp.filename_About);        // Путь и имя файла с описанием проекта
            (InputShipment.assign(V_DIR_INPUTDATA)).append(tmp.filename_shipment);  // Путь и имя файла отгрузок
            (InputPurprice.assign(V_DIR_INPUTDATA)).append(tmp.filename_purprice);  // Путь и имя файла поставок
            if((tmp.filename_purvolume != EmpStr) && (tmp.filename_purvolume != NoFileName)) {
                bcheck = true;
                (InputPurvolm.assign(V_DIR_INPUTDATA)).append(tmp.filename_purvolume);  // Путь и имя файла поставок (nocalc-режим)
            } else bcheck = false;
            (InputShares.assign(V_DIR_INPUTDATA)).append(tmp.filename_shares);              // Путь и имя файла нормативов запасов
            (InputPermission.assign(V_DIR_INPUTDATA)).append(tmp.filename_permission);      // Путь и имя файла разрешений одного периода
            (InputPurchasecalc.assign(V_DIR_INPUTDATA)).append(tmp.filename_purchasecalc);  // Путь и имя файла флагов авторасчета
            cur = _cur;
            ac = _ac;
            /** Читаем название и описание проекта **/
            if(!tmp.import_about(InputAbout))
                cout << "Импорт из файла " << InputAbout << " неудачен. В отчёте будет отсутствовать описание проекта" << '\n';
            /** Читаем данные с объемами отгрузок со склада **/
            if(!ImportSingleArray(InputShipment, ';', sTwo, sOne, volume, tmp.Shipment, tmp.ResNames, tmp.PrCount, tmp.ProdCount)) {
                /** Читаем данные. При повторных вызовах метода массив tmp.ResNames перезаписывается данными с другим числом строк:
                tmp.RMCount > tmp.ResForOutCount > tmp.ProdCount. Правило корректных вызовов метода ImportSingleArray: сначала
                вызывается метод с минимальным значением строк (у нас это tmp.ProdCount), далее - со средним (tmp.ResForOutCount)
                и последний вызов - с самым большим числом строк (tmp.RMCount). **/
                return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
            }
            /** Читаем данные с нормативами запасов в ЦУР **/
            size_t share_count;
            if(!ImportSingleArray(InputShares, ';', sTwo, sOne, tmp.Shares, tmp.ResNames, share_count, tmp.ResForOutCount)) {
                return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
            }
            /** Читаем данные с Флагами разрешения на отгрузку и закупку в одном периоде в ЦУР **/
            if(!ImportSingleArray(InputPermission, ';', sTwo, sOne, tmp.Permission, tmp.ResNames, share_count, tmp.ResForOutCount)) {
                return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
            }
            /** Читаем данные с Флагами авторасчета в ЦУР **/
            if(!ImportSingleArray(InputPurchasecalc, ';', sTwo, sOne, tmp.Purcalc, tmp.ResNames, share_count, tmp.ResForOutCount)) {
                return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
            }
            /** Читаем данные с объемами поставок на склад (nocalc-режим) в случае, если bcheck == true **/
            if(bcheck)
                if(!ImportSingleArray(InputPurvolm, ';', sTwo, sOne, volume, tmp.Purchase_v, tmp.ResNames, tmp.PrCount, tmp.ResForOutCount)) {
                    return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
                }
            /** Читаем данные с ценами поставок на склад. **/
            if(!ImportSingleArray(InputPurprice, ';', sTwo, sOne, price, tmp.Purchase_p, tmp.ResNames, tmp.PrCount, tmp.RMCount)) {
                /** Читаем данные. При повторных вызовах метода массив tmp.ResNames перезаписывается данными с другим числом строк:
                tmp.RMCount > tmp.ResForOutCount > tmp.ProdCount. Правило корректных вызовов метода ImportSingleArray: сначала
                вызывается метод с минимальным значением строк (у нас это tmp.ProdCount), далее - со средним (tmp.ResForOutCount)
                и последний вызов - с самым большим числом строк (tmp.RMCount). **/
                return false;   // Вводим Инфорацию об отгрузках. Если не введено, выходим из программы с false
            }
            /** Читаем тех.карты для ЦЗП **/
            if(!Import_Recipes(filename_recipe_in, ';', sTwo, sOne, tmp.In_Recipe, tmp.ResForOutCount)) {
                return false;   // Вводим Инфорацию об тех.картах ЦЗП. Если не введено, выходим из программы с false
            }
            /** Читаем тех.карты для ЦЗО **/
            if(!Import_Recipes(filename_recipe_out, ';', sTwo, sOne, tmp.Out_Recipe, tmp.ProdCount)) {
                return false;   // Вводим Инфорацию об тех.картах ЦЗП. Если не введено, выходим из программы с false
            }
            swap(tmp);  // Обмениваемся состоянием с буфферным объектом
            return true;
        }   // Import_Data

        /** Get - методы **/

        const string& GetTitle() const {
        /** Возвращает строку с названием проекта **/
            return Title;
        }   // GetTitle

        size_t GetAboutCount() const {
        /** Возвращает число элементов массива About.sComment, равное About.sCount **/
            return About.sCount;
        }   // GetAboutCount

        const string* GetAboutComment() const {
        /** Возвращает const-указатель на внутренний массив About.sComment **/
            return About.sComment;
        }   // GetAboutComment

        size_t GetProjectCount() const {
        /** Возвращает число периодов проекта **/
            return PrCount;
        }   // GetProjectCount

        Currency GetCurrency() const {
        /** Возвращает валюту проекта **/
            return cur;
        }   // GetCurrency

        AccountingMethod GetAccounting() const {
        /** Возвращает принцип учёта запасов **/
            return ac;
        }   // GetAccounting

        size_t GetOutCount() const {
        /** Возвращает число отгружаемых со склада номенклатурных позиций (на выходе ЦЗО) **/
            return ProdCount;
        }   // GetOutCount

        size_t GetMidCount() const {
        /** Возвращает число номенклатурных позиций в ЦУР **/
            return ResForOutCount;
        }   // GetMidCount

        size_t GetInCount() const {
        /** Возвращает число номенклатурных позиций поступающих на склад (в ЦЗП) **/
            return RMCount;
        }   // GetInCount

        const strNameMeas* GetNames() const {
        /** Возвращает указатель на внутренний массив с перечнем номенклатурных позиций и их ед. измерения **/
            return ResNames;
        }   // GetNames

        const strItem* GetPurch_p() const {
        /** Возвращает указатель на внутренний массив с ценами поставленных на ЦЗП ресурсов **/
            return Purchase_p;
        }   // GetPurch_p

        const strItem* GetPurch_v() const {
        /** Возвращает указатель на внутренний массив с объемами в натуральном выражении поставленных
        в ЦУР ресурсов **/
            return Purchase_v;
        }   // GetPurch_v

        const strItem* GetShip() const {
        /** Возвращает указатель на внутренний массив отгружаемых со склада (ЦЗО) ресурсов **/
            return Shipment;
        }   // GetShip

        const decimal* GetShares() const {
        /** Возвращает указатель на внутренний массив с нормативами запасов на складе (ЦУР) **/
            return Shares;
        }   // GetShares

        const bool* GetPermission() const {
        /** Возвращает указатель на внутренний массив с флагами разрешения на отгрузку и закупку
        в одном периоде на складе (ЦУР) **/
            return Permission;
        }   // GetPermission

        const size_t* GetPurcalc() const {
        /** Возвращает указатель на внутренний массив с флагами авторасчета (ЦУР) **/
            return Purcalc;
        }   // GetPurcalc

        vector<clsRecipeItem> GetInRecipe() {
        /** Возвращает контейнер с тех.картами для ЦЗП путем перемещения **/
            return move(In_Recipe);
        }   // GetInRecipe

        const clsRecipeItem* GetInRecipe_ptr() const {
        /** Возвращает const-указатель на внутренний массив с тех.картами для ЦЗП **/
            return In_Recipe.data();
        }   // GetInRecipe_ptr

        vector<clsRecipeItem> GetOutRecipe() {
        /** Возвращает контейнер с тех.картами для ЦЗО путем перемещения **/
            return move(Out_Recipe);
        }   // GetOutRecipe

        const clsRecipeItem* GetOutRecipe_ptr() const {
        /** Возвращает const-указатель на внутренний массив с тех.картами для ЦЗО **/
            return Out_Recipe.data();
        }   // GetOutRecipe_ptr

    protected:

};

#undef Save_Read_File_debug     // Отменить вывод отладочной информации де- / сериализации

#endif // FROMA2_MODEL_2_CLSIMPORTCONFIG_HPP
