#include "baseproject_module.h"

namespace nmbaseproject {
    const size_t  sZero = 0;
    const string EmpStr ="";
}   // nmbaseproject

namespace nmlocalBP {
    const auto buff = nullptr;          // Переменная для хранения буфера потока вывода в "пустой поток"

    string makefilename(const string _name, const string ext) {
    /** Метод формирует имя файла для сериализации/ отчета по умолчанию, Параметр _name - полное имя исполняемого файла
    (вместе с его раположением) находится в переменной argv[0] в параметрах метода main **/
        string fname=_name;                 // Вспомогательная переменная
        size_t pos;                         // Переменная для позиции, начиная с которой меняем полное имя файла
        const int cFour = 4;
        pos = fname.rfind(".exe");          // Поиск подстроки в исходной строке и определение позиции вхождения (с конца)
        if(pos == (fname.length()-cFour))   // Если подстрока есть, то меняем расширение ".exe" на
            fname.replace(pos, cFour, ext); // содержимое переменной ext в полном имени файла с указанной позиции,
        else fname+=ext;                    // иначе просто добавляем нужное расширение имени файла
        return fname;
    } // makeFname

} // nmlocalBP

/*************************************************************************************************************************/
/**                                         Базовый класс clsBaseProject                                                **/
/*************************************************************************************************************************/
/** version from 2024.10.13 **/

using namespace nmbaseproject;
using namespace nmlocalBP;

clsBaseProject::clsBaseProject() {
/** Конструктор без параметров **/
    Title = EmpStr;                 // Устанавливаем пустое название проекта
    About.sComment = nullptr;       // Устанавливаем указатель на массив строк с описанием проекта на "пусто"
    About.sCount = sZero;           // устанавливаем нулевое число строк в описании проекта
    FName = RName = EmpStr;         // Имя файлов не задано
    Rdevice = terminal;             // Устройство по умолчанию
}   // Default Ctor

clsBaseProject::~clsBaseProject() {
/** Виртуальный деструктор **/
    if((About.sCount > sZero) || (About.sComment))
        delete[] About.sComment;
}   // Dtor

clsBaseProject::clsBaseProject(const clsBaseProject& other) {
/** Конструктор копирования **/
    Title = other.Title;
    FName = other.FName;
    RName = other.RName;
    Rdevice = other.Rdevice;
    About.sCount = other.About.sCount;
    if(About.sComment) delete[] About.sComment;
    if(other.About.sComment)
        About.sComment = new(nothrow) string[About.sCount];
    else
        About.sComment = nullptr;   // Устанавливаем указатель на массив строк с описанием проекта на "пусто"
    if(About.sComment)
        for(size_t i=sZero; i<About.sCount; i++)
            *(About.sComment+i) = *(other.About.sComment+i);    // Копирование строк
}   // Copy Ctor

void clsBaseProject::swap(clsBaseProject& other) noexcept {
/** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
    std::swap(Title, other.Title);                      // Обмениваем значения
    std::swap(About.sComment, other.About.sComment);    // Обмениваем указатели
    std::swap(About.sCount, other.About.sCount);        // Обмениваем значения
    std::swap(FName, other.FName);                      // Обмениваем значения
    std::swap(RName, other.RName);                      // Обмениваем значения
    std::swap(Rdevice, other.Rdevice);                  // Обмениваем значения
}   // swap

clsBaseProject::clsBaseProject(clsBaseProject&& other) {
/** Конструктор перемещения **/
    Title = EmpStr;                 // Устанавливаем пустое название проекта
    About.sComment = nullptr;       // Устанавливаем указатель на массив строк с описанием проекта на "пусто"
    About.sCount = sZero;           // устанавливаем нулевое число строк в описании проекта
    FName = RName = EmpStr;         // Имя файлов не задано
    Rdevice = nulldev;              // Задано устройство "пусто"
    swap(other);                    // Обмениваемся знаяениями и указателями с other
}   // Move Ctor

clsBaseProject& clsBaseProject::operator=(const clsBaseProject& other) {
/** Перегрузка оператора присваивания. Реализовано в идеоме КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom)  **/
    clsBaseProject tmp(other);      // Вызываем конструктор копирования и получаем копию other в переменной tmp
    swap(tmp);                      // Обмениваемся содержанием с tmp
    return *this;
}   // Copy operator=

clsBaseProject& clsBaseProject::operator=(clsBaseProject&& other) {
/** Перегрузка оператора присваивания перемещением аналогичного объекта **/
    swap(other);                        // Обмениваемся содержанием с other
    return *this;
}   // Move operator=

void clsBaseProject::SetTitle(const string& _title) {
/** Метод ввода титула проекта **/
    Title = _title;
}   // SetTitle

void clsBaseProject::SetComment(const string* _comment, const size_t& _count) {
/** Метод ввода описания проекта. Параметры:  _comment - указатель на массив строк, _count - размер массива**/
    if((!_comment) || (_count == sZero)) return;        // Проверка корректности вводимых данных
    About.sCount = _count;
    if(About.sComment)                                  // Если массив существует,
        delete[] About.sComment;                        // то удаляем его
    About.sComment = new(nothrow) string[About.sCount]; // Выделяем память новому массиву
    if(About.sComment)                                  // Если память выделена, то
        for(size_t i=sZero; i<About.sCount; i++)        // копируем в новый массив строки
            *(About.sComment+i) = *(_comment+i);
}   // SetComment

void clsBaseProject::SetFName(const string _filename) {
/** Метод ввода имени файла для сериализации и десериализации **/
    FName = makefilename(_filename, ".dat");
}   // SetFName

void clsBaseProject::SetFName(const string _filename, const string _ext) {
/** Метод ввода имени файла для сериализации и десериализации **/
    FName = _filename + _ext;
}   // SetFName

void clsBaseProject::SetRName(const string _filename) {
/** Метод ввода имени файла для отчета **/
    RName = makefilename(_filename, ".txt");
}   // SetRName

void clsBaseProject::SetDevice(const Tdev& val) {
/** Метод устанавливает выходное устройство для отчета. Параметр val - индекс устройства:
nulldev - пустое устройство, terminal - вывод на экран, file - вывож в файл **/
    Rdevice = val;
} // SetDevice

void clsBaseProject::Reset() {
/** Метод сбрасывает всю информацию в экземпляре класса до "пусто" **/
    clsBaseProject tmp;         // Создаем "пустую" переменную tmp
    swap(tmp);                  // Обмениваемся содержанием с tmp
}   // Reset

//void clsBaseProject::View() const {
///** Метод визуального контроля. Выводит информацию на экран **/
//    cout << Title << endl << endl;
//    for(size_t i=sZero; i<About.sCount; i++) {
//        cout << *(About.sComment+i) << endl;
//    }
//}   // View

//void clsBaseProject::VPrint(const string _filename) {
///** Метод визуального контроля. Выводит информацию в текстовый файл **/
//    if(_filename == EmpStr) return;                     // Если имя файла пустое, то выход
//    ofstream out(_filename);                            // Откроем файл для вывода
//    streambuf* coutbuf = cout.rdbuf();                  // запомним старый буфер в переменной coutbuf
//    cout.rdbuf(out.rdbuf());                            // перенаправляем поток в файл: теперь все будет в файл
//    View();                                             // Выводим информацию о проекте в поток
//    cout.rdbuf(coutbuf);                                // восстанавливаем вывод на экран
//    out.close();                                        // закрываем файд
//}   // VPrint

//void clsBaseProject::Report() const {
///** Метод визуального контроля. Выводит информацию на экран **/
//    reportstream(std::cout);
//}   // Report()

void clsBaseProject::Report() const {
/** Метод вывода отчета на выбранное устройство **/
    if(Rdevice == nulldev) {
        std::ostream strNull(buff);
        reportstream(strNull);
        return;
    };
    if(Rdevice == terminal) {
        reportstream(std::cout);
        return;
    };
    if(Rdevice == file) {
        if(RName == EmpStr) return;                     // Если имя файла пустое, то выход
        ofstream out(RName);                            // Откроем файл для вывода
        if(out.is_open())                               // Если файл открыт, то
            reportstream(out);                          // Выводим в поток информацию
        out.close();                                    // и закрываем файл
        return;
    }
}   // Report()

//void clsBaseProject::Report(const string _filename) const {
///** Метод визуального контроля. Выводит информацию в текстовый файл **/
//    string filename;                                    // Вспомогательная переменная
//    if(_filename != EmpStr) filename = _filename;       // Если имя файла не пустое, то используем его
//    else                                                // Иначе
//        if(RName != EmpStr) filename = RName;           // Если имя по умолчанию не пустое, то используем его,
//        else return;                                    // Иначе выход
////    if(filename == EmpStr) return;                      // Если имя файла пустое, то выход
//    ofstream out(filename);                             // Откроем файл для вывода
//    if(out.is_open())                                   // Если файл открыт, то
//        reportstream(out);                              // Выводим в поток информацию
//    out.close();                                        // и закрываем файл
//}   // Report(...)

//void clsBaseProject::Report(const Tdev dev) const {
///** Метод вывода отчета на выбранное устройство. Параметры: dev - индекс устройства **/
//    if(dev==nulldev) {
//        std::ostream strNull(buff);
//        reportstream(strNull);
//        return;
//    };
//    if(dev==terminal) {
//        reportstream(std::cout);
//        return;
//    };
//    if(dev==file) {
//        if(RName == EmpStr) return;                     // Если имя файла пустое, то выход
//        ofstream out(RName);                            // Откроем файл для вывода
//        if(out.is_open())                               // Если файл открыт, то
//            reportstream(out);                          // Выводим в поток информацию
//        out.close();                                    // и закрываем файл
//        return;
//    }
//}   // Report(const Tstream dev)

bool clsBaseProject::SaveToFile(const string _filename) {
/** Метод записи текущего экземпляра класса в файл **/
    if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
    ofstream outF(_filename, ios::out | ios::binary);   // Связываем имя файла с файловым потоком для записи
    if (!outF.is_open()) {                              // Если файл не создан, то сообщение пользователю и
        cout << "Ошибка открытия файла" <<endl;         // возврат false и выход из функции
        return false;
    };
    if(!StF(outF)) {                                    // Записываем данные в файл. Если файл не записан, то
        outF.close();                                   // Закрыаем файл и
        return false;                                   // Выходим с false
    };
    outF.close();                                       // Закрываем файл
    return true;                                        // Возвращаем true
}   // SaveToFile

bool clsBaseProject::SaveToFile() {
/** Метод записи текущего экземпляра класса в файл с именем по умолчанию FName **/
    if(!SaveToFile(FName)) return false;
    return true;
}   // SaveToFile

bool clsBaseProject::ReadFromFile(const string _filename) {
/** Метод чтения из файла и запись в экземпляр класса **/
    if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
    ifstream inF(_filename, ios::in | ios::binary);     // Связываем имя файла с файловым потоком для чтения
    if (!inF.is_open()) {                               // Если файл не открыт, то сообщение пользователю и
        cout << "Ошибка открытия файла" <<endl;         // возврат false и выход из функции
        return false;
    };
    if(!RfF(inF)) {                                     // Считываем данные из файла. Если файл не записан, то
        inF.close();                                    // Закрываем файл
        return false;                                   // Выход и возврат false
    }
    inF.close();                                        // 4.   Закрываем файл
    return true;
}   // ReadFromFile

bool clsBaseProject::ReadFromFile() {
/** Метод чтения из файла с именем по умолчанию и запись в экземпляр класса **/
    if(!ReadFromFile(FName)) return false;
    return true;
}   // ReadFromFile

bool clsBaseProject::StF(ofstream &_outF) {
/** Метод имплементации записи в файловую переменную текущего экземпляра класса (запись в файл, метод
сериализации). Параметры: &_outF - экземпляр класса ofstream для записи данных **/
    if(!SEF(_outF, Title)) return false;
    if(!SEF(_outF, About.sCount)) return false;
    for(size_t i=sZero; i<About.sCount; i++)
        if(!SEF(_outF, *(About.sComment+i))) return false;
    if(!SEF(_outF, FName)) return false;
    if(!SEF(_outF, RName)) return false;
    return true;
}   // StF

bool clsBaseProject::RfF(ifstream &_inF) {
/** Метод имплементации чтения из файловой переменной текущего экземпляра класса (чтение из файла, метод
десериализации). Параметры: &_inF - экземпляр класса ifstream для чтения данных **/
    if(About.sComment) delete[] About.sComment;
    if(!DSF(_inF, Title)) return false;
    if(!DSF(_inF, About.sCount)) return false;
    About.sComment = new(nothrow) string[About.sCount];
    if(!About.sComment) return false;
    for(size_t i=sZero; i<About.sCount; i++)
        if(!DSF(_inF, *(About.sComment+i))) return false;
    if(!DSF(_inF, FName)) return false;
    if(!DSF(_inF, RName)) return false;
    return true;
}   // RfF

void clsBaseProject::reportstream(ostream& os) const {
/** Метод выводит отчет в поток os **/
    nmRePrint::PrintHeader0(os, nmRePrint::smblcunt+nmRePrint::uThree, Title);
//    os << Title << "\n\n";                      // Вывод названия проекта
    for(size_t i=sZero; i<About.sCount; i++) {  // Вывод описания проекта
        os << *(About.sComment+i) << "\n";
    };
    os << "\n";
}   // reportstream
