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

#include "impex_module.h"
#include <limits>

const unsigned short int mant = std::numeric_limits<double>::max_digits10;  /** Количество разрядов
вещественного числа double, используемого при выводе данных в поток в методе clsImpex::Import для
импорта данных из массивов типа strItem (по умолчанию равно 17),
см https://iq.opengenus.org/print-double-with-full-precision-in-cpp **/

/*************************************************************************************************************************/
/**                                             Вспомогательные методы                                                  **/
/*************************************************************************************************************************/

void MeasRestore(nmBPTypes::strNameMeas Big[], nmBPTypes::strNameMeas Lit[], const size_t brows, const size_t lrows) {
/** Метод заполняет поля с единицами измерения в массиве lit данными из массива Big при совпадении имен в этих массивах.
Параметры: brows - количество строк в массиве Big, lrows - количество строк в массиве Lit. **/
        for(size_t i=0; i<lrows; i++)                       // Цикл по малому массиву Lit
            for(size_t j=0; j<brows; j++) {                 // Цикл по большому массиву Big
                if((Big+j)->name == (Lit+i)->name)          // Если имена совпадают, то
                    (Lit+i)->measure = (Big+j)->measure;    // Заполняем поле с еди.измерения
            };
    }   // MeasRestore

/*************************************************************************************************************************/
/**                           Класс clsImpex для импорта информации из cvs-файлов                                       **/
/**                 и подготовки исходных данных для объектов типа clsStorage и clsManufactory                          **/
/*************************************************************************************************************************/

clsImpex::clsImpex() {
/** Конструктор по умолчанию **/
    m_rowcount = m_colcount = nmBPTypes::sZero;     // Обнуляем счетчики
    separator = ';';                                // Устанавливаем сепаратор По умолчанию
}   // Default Ctor

clsImpex::clsImpex(ifstream& ifs, const char& ch) {
/** Конструктор с параметром. Параметры: ifs - файловый поток на чтение, ch - символ разделителя между
данными в файле **/
    m_rowcount = m_colcount = nmBPTypes::sZero;     // Обнуляем счетчики
    separator = ';';                                // Устанавливаем сепаратор По умолчанию
    if(!ifs.is_open())  return;                     // Если файл не открыт, то выход
    if(!Import(ifs, ch))                            // Импорт данных из файла. Если неудачно, то
        reset();                                    // сбрасываем состояние объекта
}   // Ctor with parametr

clsImpex::clsImpex(const size_t ncount, const nmBPTypes::strNameMeas names[],\
const nmBPTypes::strItem data[], const size_t dcount, nmBPTypes::ReportData flg) {
/** Конструктор с параметрами для импорта из массивов. Параметры: const size_t ncount - число строк, равное числу элементов массива names[]
с наименованиями строк, names[] - массив с наименованиями строк и единицами измерения, data[] - одномерный массив,
аналог двумерной матрицы размером ncount*dcount с данными, dcount - число столбцов матрицы, flg - флаг, определяющий
тип используемых данных: volume, price или value. **/
    m_rowcount = m_colcount = nmBPTypes::sZero;     // Обнуляем счетчики
    separator = ';';                                // Устанавливаем сепаратор По умолчанию
    if((ncount==nmBPTypes::sZero) || (!names) || (!data) || (dcount==nmBPTypes::sZero))
        return;                                     // Если массивы не существуют, то выход
    if(!Import(ncount, names, data, dcount, flg))   // Импорт данных из массива. Если неудачно, то
        reset();                                    // сбрасываем состояние объекта
}   // Ctor with parametr

clsImpex::~clsImpex()
{
}   // Dtor

clsImpex::clsImpex(const clsImpex& other) {
/** Конструктор копирования **/
    m_rowcount = other.m_rowcount;
    m_colcount = other.m_colcount;
    separator  = other.separator;
    m_data     = other.m_data;
}   //Copy ctor

clsImpex::clsImpex(clsImpex&& other) {
/** Конструктор перемещения **/
    swap(other);
}   // Move Ctor

void clsImpex::swap(clsImpex& other) noexcept {
/** Функция обмена состояниями объектов **/
    std::swap(m_rowcount, other.m_rowcount);      // Обмениваем значения
    std::swap(m_colcount, other.m_colcount);
    std::swap(separator, other.separator);
    m_data.swap(other.m_data);
}   // swap

clsImpex& clsImpex::operator=(const clsImpex& rhs) {
/** Оператор присваивания копированием **/
    if (this == &rhs) return *this; // handle self assignment
    clsImpex temp(rhs);             // Вспомогательная переменная создается копированием из rhs
    swap(temp);                     // Обмен со вспомогательной переменной
    return *this;
}   // Copy assignment operator

clsImpex& clsImpex::operator=(clsImpex &&rhs) {
/** Перегрузка оператора присваивания перемещением аналогичного объекта **/
    if (this == &rhs) return *this; // handle self assignment
    swap(rhs);                      // Обмениваемся с объектом содержанием
    return *this;
}   // Move assignment operator

void clsImpex::reset() {
/** Сбрасывает состояние объекта до значения по умолчанию. Метод vector::clear не гарантирует
успешного перераспределения памяти, приведенный ниже метод используется как альтернатива ему
(http://cplusplus.com/reference/vector/vector/clear) **/
    m_rowcount = m_colcount = nmBPTypes::sZero;         // Обнуляем счетчики,
    separator = ';';                                    // Устанавливаем сепаратор по умолчанию
    vector<std::vector<std::string>>().swap(m_data);    // Приводим вектор в состояние по умолчанию
}   // reset

bool clsImpex::is_Empty() const {
/** Возвращает true, если m_data пустой **/
    if(m_data.size() == 0) return true;
    else return false;
}   // is_Empty

/** Методы импорта **/

bool clsImpex::Import(ifstream& ifs, const char& ch) {
/** Метод импортирует данные из файла, связанного с потоком ifs, в качестве разделителя используется символ ch **/
    if((m_rowcount>nmBPTypes::sZero) || (m_colcount>nmBPTypes::sZero) || (m_data.size()>nmBPTypes::sZero))\
        return false;                   // Проверка на наличие данных
    if(!ifs.is_open()) {                // Если файл не открыт, то
        return false;                   // выходим и возвращаем false
    };
    size_t tmpcount;                    // Вспомогательный счетчик требуется для проверки "прямоугольности" будущей матрицы
    for (string line; getline(ifs, line);) {        // Читаем в line каждую строку файла
        if(ifs.bad()) {                             // Если чтение из файла не удалось, то
            return false;                           // и выходим с false
        };
        m_rowcount++;                               // и увеличиваем счетчик строк
        istringstream ss(std::move(line));          // и перемещаем прочтенную строку в строковый поток ss
        vector<std::string> row;                    // Вспомогательный вектор для строки
        if(!m_data.empty())                         // Если основной вектор не пустой, то в дополнительном векторе
            row.reserve(m_data.front().size());     // резервируем память размером с первый элемент
        tmpcount = m_colcount;          // Устанавливаем вспомогательный счетчик равным предыдущему значению m_colcount
        m_colcount = nmBPTypes::sZero;  // Обнуляем счетчик столбцов
        for(string value; getline(ss, value, separator);) {     // Разбиваем строку на отдельные строки, разделенные символом ch
            m_colcount++;                                       // Увеличиваем счетчик столбцов
            row.push_back(std::move(value));                    // Переносим каждую малую строку во вспомогателный вектор
        };
        if(tmpcount != nmBPTypes::sZero)
            if(tmpcount != m_colcount) {            // Проверка, что количество полей одинаково во всех строках
                return false;                       // и выходим с false
            };
        m_data.push_back(std::move(row));           // Переносим вспомогателный вектор в элемент основного вектора
    };
    return true;
}   // Import from file

bool clsImpex::Import(const size_t ncount, const nmBPTypes::strNameMeas names[], const nmBPTypes::strItem data[],\
const size_t dcount, nmBPTypes::ReportData flg) {
/** Метод импортирует данные из массивов. Параметры: const size_t ncount - число строк, равное числу элементов массива
names[] с наименованиями строк, names[] - массив с наименованиями строк и единицами измерения, data[] - одномерный массив,
аналог двумерной матрицы размером ncount*dcount с данными, dcount - число столбцов матрицы, flg - флаг, определяющий тип
используемых данных: volume, price или value. **/
    if((m_rowcount>nmBPTypes::sZero) || (m_colcount>nmBPTypes::sZero) || (m_data.size()>nmBPTypes::sZero))
        return false;                   // Проверка на наличие данных; если данные уже есть, то выход с false
    if((ncount==nmBPTypes::sZero) || (!names) || (!data) || (dcount==nmBPTypes::sZero))
        return false;                   // Если массивы не существуют, то выход
    m_rowcount = ncount + 1;            // Количество строк в матрице
    m_colcount = dcount + 2;            // Количество столбцов в матрице
    vector<std::string> frow;           // Вспомогательный вектор для строки
    m_data.reserve(m_rowcount);                 // Резервируем память основному массиву
    frow.reserve(m_colcount);                   // Резервируем память вспомогательному массиву
    frow.push_back(nmPrntSetup::c_TableName);   // Добавление заголовка "Наименование"
    frow.push_back(nmPrntSetup::c_TableMeas);   // Добавление заголовка "Ед.измерения"
    for(size_t j{}; j<dcount; j++)
        frow.push_back(std::to_string(j));      // Добавление номера периода
    m_data.push_back(std::move(frow));          // Переносим вспомогателный вектор в элемент основного вектора
    frow.~vector();                             // Уничтожаем вспомогательный вектор
    for(size_t i{}; i<ncount; i++) {            // Цикл по строкам
        vector<std::string> row;                // Вспомогательный вектор для строки
        row.reserve(m_colcount);                // Резервируем память
        row.push_back((names+i)->name);         // Добавляем наименование ресурса
        row.push_back((names+i)->measure);      // Добавляем единицу измерения
        for(size_t j{}; j<dcount; j++) {        // Цикл по столбцам
            stringstream ss;                    // Вспомогательный поток
            if((flg==nmBPTypes::ReportData::volume) || (flg==nmBPTypes::ReportData::price)) // Записываем в поток данные
                if(flg==nmBPTypes::ReportData::volume) ss << fixed << setprecision(mant) << (data+dcount*i+j)->volume;
                else ss << fixed << setprecision(mant) << (data+dcount*i+j)->price;
            else ss << fixed << setprecision(mant) << (data+dcount*i+j)->value;
            row.push_back(std::move(ss.str())); // Переносим в вектор элемент
        };
        m_data.push_back(std::move(row));       // Переносим вспомогателный вектор в элемент основного вектора
    };
    return true;
}   // Impotr from massive

/** Методы преобразования **/

void clsImpex::Transpon() {
/** Метод транспонирования матрицы m_data **/
    if(m_data.size() == nmBPTypes::sZero) return;           // Если вектор пуст, то не транспонируем его
    vector<std::vector<std::string>> tmp;                   // Вспомогательная матрица
    for(size_t j=nmBPTypes::sZero; j<m_colcount; j++) {     // Цикл по столбцам
        vector<std::string> col;                            // Вспомогательный вектор для столбца
        if(!tmp.empty())                                    // Если основной вектор не пустой, то в дополнительном векторе
            col.reserve(tmp.front().size());                // резервируем память размером с первый элемент
        for(vector<std::string> row: m_data) {                          // Цикл по строкам
            vector<std::string>::const_iterator cit = row.begin()+j;    // Устанавливаем итератор на j-й элемент строки
            col.push_back(*cit);                            // Формируем столбец col для новой матрицы tmp
        }
        tmp.push_back(std::move(col));                      // и добавляем его в новую матрицу
    };
    m_data.swap(tmp);                                       // Обмениваем содержание матрицы m_data и новой матрицы tmp
    std::swap(m_rowcount, m_colcount);                      // Обмениваем значения у счетчиков
}   // Transpon

/** Методы экспорта **/

void clsImpex::csvExport(ofstream& ofs) const {
/** Метод экспорта вектора в csv-файл с разделителем, заданным в переменной separator **/
    for (const vector<string>& row : m_data) {
        for(const string& value: row) {
            ofs << value << separator;
        }
        ofs << endl;
    };
}   // csvExport

/** Методы Get **/

decimal* clsImpex::GetDecimal(const size_t brow, const size_t erow,\
const size_t bcol, const size_t ecol) const {
/** Метод возвращает выбранные в матрице данные, конвертированные в тип decimal. Параметры:
brow - начальная строка данных, erow - конечная строка, bcol - начальный столбец, ecol -
конечный столбец. Метод возвращает одномерный динамический массив, являющийся аналогом
двумерной матрицы размером (erow-brow+1)*(ecol-bcol+1). Тип и формат данных совместим с данными,
принимаемыми в качестве параметров методами классов clsStorage и clsManufactory. ВНИМАНИЕ!!!
Для совместимости с параметрами методов классов clsStorage и clsManufactory матрица (в векторе m_data)
с импортированными данными должна иметь горизонтальную ориентацию (разные периоды - в разных столбцах).
В противном случае перед применением настоящего метода матрицу необходимо транспонировать. **/
    if((m_rowcount==nmBPTypes::sZero) || (m_colcount==nmBPTypes::sZero) ||\
        (m_data.size()==nmBPTypes::sZero)) return nullptr;                          // Проверка на наличие данных
    if((brow>=m_rowcount) || (erow>=m_rowcount) || (brow>erow) ||\
        (bcol>=m_colcount) || (ecol>=m_colcount) || (bcol>ecol)) return nullptr;    // Проверка корректности параметров
    size_t trows = erow-brow+nmBPTypes::sOne;           // Количество строк в выходном массиве
    size_t tcols = ecol-bcol+nmBPTypes::sOne;           // Количество столбцов в выходном массиве
    decimal* dData = new(nothrow) decimal[trows*tcols];       // Выделяем память массиву
    if(!dData) return nullptr;                          // Если память не выделена, то выход и возврат nullptr
    vector<vector<string>>::const_iterator citrow;      // Итератор для строк
    vector<string>::const_iterator citcell;             // итератор для столбцов
    size_t i=nmBPTypes::sZero;                          // Индекс строки
    for(citrow=m_data.cbegin()+brow; citrow<=m_data.cbegin()+erow; citrow++) {              // Перебор по строкам
        size_t j=nmBPTypes::sZero;                                                          // Индекс столбца
        for(citcell=(*citrow).cbegin()+bcol; citcell<=(*citrow).cbegin()+ecol; citcell++) { // перебор по столбцам
            stringstream ss;                            // Вспомогательный поток строк
            ss << *citcell;                             // Читаем из матрицы в поток строк
            ss >> *(dData+tcols*i+j);                   // Экспортируем из потока в число
            j++;                                        // Изменяем индекс столбца
        };
        i++;                                            // Изменяем индекс строки
    };
    return dData;
}   // GetDecimal

nmBPTypes::strNameMeas* clsImpex::GetNames(const size_t brow, const size_t erow,\
const size_t idName, const size_t idMeas) const {
/** Метод возвращает выбранные в матрице данные, конвертированные в тип strNameMeas. Параметры:
brow - начальная строка данных, erow - конечная строка, idName - номер столбца с названиями,
idMeas - номер столбца с единицами измерения. Метод возвращает одномерный динамический массив размером (erow-brow+1).
Тип и формат данных совместим с данными, принимаемыми в качестве параметров методами классов clsStorage и
clsManufactory. ВНИМАНИЕ!!! Для совместимости с параметрами методов классов clsStorage и clsManufactory
матрица (в векторе m_data) с импортированными данными должна иметь горизонтальную ориентацию (разные
периоды - в разных столбцах). В противном случае перед применением настоящего метода матрицу необходимо
транспонировать. **/
    if((m_rowcount==nmBPTypes::sZero) || (m_colcount==nmBPTypes::sZero) ||\
        (m_data.size()==nmBPTypes::sZero)) return nullptr;                          // Проверка на наличие данных
    if((idName>=m_colcount) || (idMeas>=m_colcount) ||\
        (brow>=m_rowcount) || (erow>=m_rowcount) || (brow>erow)) return nullptr;    // Проверка корректности параметров
    size_t trows = erow-brow+nmBPTypes::sOne;                   // Количество строк в выходном массиве
    nmBPTypes::strNameMeas* dData = new(nothrow) nmBPTypes::strNameMeas[trows];     // Выделяем память массиву
    if(!dData) return nullptr;                                  // Если память не выделена, то выход и возврат nullptr
    vector<vector<string>>::const_iterator citrow;              // Итератор для строк
    size_t i=nmBPTypes::sZero;                                  // Индекс строки
    for(citrow=m_data.cbegin()+brow; citrow<=m_data.cbegin()+erow; citrow++) {
        (dData+i)->name    = *((*citrow).cbegin() + idName);    // Получаем имя
        (dData+i)->measure = *((*citrow).cbegin() + idMeas);    // Получаем единицу измерения
        i++;
    };
    return dData;
}   // GetNames

string* clsImpex::GetNames(const size_t brow, const size_t erow, const size_t idName) const {
/** Метод возвращает выбранные в матрице данные, конвертированные в тип string. Параметры:
brow - начальная строка данных, erow - конечная строка, idName - номер столбца с названиями.
Метод возвращает одномерный динамический массив размером (erow-brow+1). ВНИМАНИЕ!!!
Для совместимости с параметрами методов классов clsStorage и clsManufactory матрица (в векторе m_data)
с импортированными данными должна иметь горизонтальную ориентацию (разные периоды - в разных столбцах).
В противном случае перед применением настоящего метода матрицу необходимо транспонировать. **/
    if((m_rowcount==nmBPTypes::sZero) || (m_colcount==nmBPTypes::sZero) ||\
        (m_data.size()==nmBPTypes::sZero)) return nullptr;                          // Проверка на наличие данных
    if((idName>=m_colcount) || \
        (brow>=m_rowcount) || (erow>=m_rowcount) || (brow>erow)) return nullptr;    // Проверка корректности параметров
    size_t trows = erow-brow+nmBPTypes::sOne;                   // Количество строк в выходном массиве
    string* dData = new(nothrow) string[trows];                 // Выделяем память массиву
    if(!dData) return nullptr;                                  // Если память не выделена, то выход и возврат nullptr
    vector<vector<string>>::const_iterator citrow;              // Итератор для строк
    size_t i=nmBPTypes::sZero;                                  // Индекс строки
    for(citrow=m_data.cbegin()+brow; citrow<=m_data.cbegin()+erow; citrow++) {
        *(dData+i) = *((*citrow).cbegin() + idName);            // Получаем имя
        i++;
    };
    return dData;
}   // GetNames

nmBPTypes::strItem* clsImpex::GetstrItem(const size_t brow, const size_t erow, const size_t bcol,\
const size_t ecol, nmBPTypes::ReportData flg) const {
/** Метод возвращает выбранные в матрице данные, конвертированные в структурный тип strItem. Параметры:
brow - начальная строка данных, erow - конечная строка, bcol - начальный столбец, ecol - конечный
столбец, flg - флаг, определяющий используемые в структуре поля: volume, price или value. Используемые
поля заполняются данными из матрицы, неиспользуемые - нулями. Метод возвращает одномерный динамический
массив, являющийся аналогом двумерной матрицы размером (erow-brow+1)*(ecol-bcol+1). Тип и формат данных
совместим с данными, принимаемыми в качестве параметров методами классов clsStorage и clsManufactory.
ВНИМАНИЕ!!! Для совместимости с параметрами методов классов clsStorage и clsManufactory матрица (в векторе
m_data) с импортированными данными должна иметь горизонтальную ориентацию (разные периоды - в разных
столбцах). В противном случае перед применением настоящего метода матрицу необходимо транспонировать. **/
    if((m_rowcount==nmBPTypes::sZero) || (m_colcount==nmBPTypes::sZero) ||\
        (m_data.size()==nmBPTypes::sZero)) return nullptr;                          // Проверка на наличие данных
    if((brow>=m_rowcount) || (erow>=m_rowcount) || (brow>erow) ||\
        (bcol>=m_colcount) || (ecol>=m_colcount) || (bcol>ecol)) return nullptr;    // Проверка корректности параметров
    size_t trows = erow-brow+nmBPTypes::sOne;           // Количество строк в выходном массиве
    size_t tcols = ecol-bcol+nmBPTypes::sOne;           // Количество столбцов в выходном массиве
    nmBPTypes::strItem* dData = new(nothrow) nmBPTypes::strItem[trows*tcols];       // Выделяем память массиву
    if(!dData) return nullptr;                          // Если память не выделена, то выход и возврат nullptr
    vector<vector<string>>::const_iterator citrow;      // Итератор для строк
    vector<string>::const_iterator citcell;             // итератор для столбцов
    size_t i=nmBPTypes::sZero;                          // Индекс строки
    for(citrow=m_data.cbegin()+brow; citrow<=m_data.cbegin()+erow; citrow++) {              // Перебор по строкам
        size_t j=nmBPTypes::sZero;                                                          // Индекс столбца
        for(citcell=(*citrow).cbegin()+bcol; citcell<=(*citrow).cbegin()+ecol; citcell++) { // перебор по столбцам
            stringstream ss;                                            // Вспомогательный поток строк
            ss << *citcell;                                             // Читаем из матрицы в поток строк
            if(flg==nmBPTypes::volume) {                                // Если выбран флаг volume
                ss >> (dData+tcols*i+j)->volume;                        // Экспортируем из потока в число и записываем в поле volume
                (dData+tcols*i+j)->price = nmBPTypes::dZero;            // Остальные поля заполняем нулями
                (dData+tcols*i+j)->value = nmBPTypes::dZero;
            } else                                                      // Иначе
                if(flg==nmBPTypes::price) {                             // Если выбран флаг price
                    (dData+tcols*i+j)->volume = nmBPTypes::dZero;
                    ss >> (dData+tcols*i+j)->price;                     // Экспортируем из потока в число и записываем в поле price
                    (dData+tcols*i+j)->value = nmBPTypes::dZero;
                } else {
                        (dData+tcols*i+j)->volume = nmBPTypes::dZero;
                        (dData+tcols*i+j)->price  = nmBPTypes::dZero;
                        ss >> (dData+tcols*i+j)->value;
                    };
            j++;                                        // Изменяем индекс столбца
        };
        i++;                                            // Изменяем индекс строки
    };
    return dData;
}   // GetstrItem

size_t clsImpex::GetRowCount() const { return m_rowcount; }
size_t clsImpex::GetColCount() const { return m_colcount; }

/** Методы визуального контроля **/

void clsImpex::View(ostream& os) const {
/** Метод визуального контроля считанной из файла информации **/
    os << "Rows " << m_rowcount << "; columns " << m_colcount << endl;
    for (const vector<string>& row : m_data) {
        for(const string& value: row) {
            os << setw(15) << value.substr(0,15) << " ";
        }
        os << endl;
    };
}   // View
