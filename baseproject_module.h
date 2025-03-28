#ifndef BASEPROJECT_MODULE_H_INCLUDED
#define BASEPROJECT_MODULE_H_INCLUDED

#include <iostream>         // Заголовочный файл с классами, функциями и переменными для организации ввода-вывода
#include <fstream>          // Заголовочный файл для организации ввода/ вывода в файл на диске
#include <string>           // Заголовочный файл с классами, функциями и переменными для работы со строками
#include "common_values.hpp"
#include "serialization_module.hpp"

using namespace std;

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                               Интерфейс к базовому классу clsBaseProject                                            **/
/**                                                                                                                     **/
/*************************************************************************************************************************/
/** version from 2024.10.13 **/

enum Tdev{nulldev=nmBPTypes::sZero, terminal, file};    // Индикатор выбора устройства вывода: nulldev - пустое устройство,
                                                        // terminal - вывод на экран, file - вывод в файл
class clsBaseProject
{
    private:
        string Title;           // Название проекта
        struct Descript {       // Структура с описанием проекта:
            string *sComment;   // указатель на массив строк с описанием,
            size_t  sCount;     // переменная с количеством элементов в массиве (количество строк)
        } About;                // Переменная, содержащая структуру. Создается при инициализации класса

    public:
        clsBaseProject();                                               // Конструктор по умолчанию
        virtual ~clsBaseProject();                                      // Виртуальный деструктор
        clsBaseProject(const clsBaseProject& other);                    // Конструктор копирования
        virtual void swap(clsBaseProject& other) noexcept;              // Функция обмена значениями между объектами
        clsBaseProject(clsBaseProject&& other);                         // Конструктор перемещения
        clsBaseProject& operator=(const clsBaseProject& other);         // Перегрузка оператора присваивания копированием
        clsBaseProject& operator=(clsBaseProject&& other);              // Перегрузка оператора присваивания перемещением
        void SetTitle(const string& _title);                            // Метод ввода титула проекта
        void SetComment(const string* _comment, const size_t& _count);  // Метод ввода описания проекта
        void SetFName(const string _filename);          // Метод ввода имени файла для сериализации и десериализации
        void SetFName(const string _filename, const string _ext);   // ВВод имени файла и его расширения
        void SetRName(const string _filename);          // Метод ввода имени файла для отчета
        void SetDevice(const Tdev& val);                // Метод устанавливает выходное устройство для отчета
        virtual void Reset();                           // Метод сбрасывает всю информацию
//        virtual void View() const;                      // Метод визуального контроля
//        virtual void VPrint(const string _filename);    // Метод вывода контрольной информации в файл
        void Report() const;                            // Метод вывода отчета на экран
//        void Report(const string filename) const;       // Метод вывода отчета в файл
//        void Report(const Tdev dev) const;              // Метод вывода отчета на выбранное в переменной dev устройство
        bool SaveToFile(const string _filename);        // Метод записи текущего экземпляра класса в файл
        bool SaveToFile();                              // Метод записи текущего экземпляра класса в файл
        bool ReadFromFile(const string _filename);      // Метод чтения из файла и запись в экземпляр класса
        bool ReadFromFile();                            // Метод чтения из файла и запись в экземпляр класса

    protected:
        string FName;           // Переменная с именем файла для чтения/ записи состояния экземпляра класса на диск
        string RName;           // Переменная с именем файла для вывода информации о проекте вместо экрана в файл
        Tdev Rdevice;                                   // Переменная с признаком устройства для вывода отчета
        virtual bool StF(ofstream &_outF);              // Метод имплементации записи в файловую переменную
        virtual bool RfF(ifstream &_inF);               // Метод имплементации чтения из файловой переменной
        virtual void reportstream(ostream& os) const;   // Метод вывода отчета в поток
};  // clsBaseProject

#endif // BASEPROJECT_MODULE_H_INCLUDED
