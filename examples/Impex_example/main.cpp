#include <iostream>
#include <fstream>
#include <impex_module.h>
#include <common_values.hpp>

using namespace std;
using namespace nmBPTypes;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");                   // Установка русского языка

    string idir = "../../examples/Impex_example/input/"; // Расположение файлов входных данных
    string odir= "../examples/Impex_example/output/"; 	 // Расположение результатов расчетов

    /** Импортируем данные из CSV-файла во внутреннее хранилище **/
    ifstream input(idir + "ship.csv");              // Связываем файл с потоком
    const char ch =';';                             // Выбираем разделитель
    clsImpex* Data = new clsImpex(input, ch);       // Импортируем данные из файла
    input.close();                                  // Закрываем файл

    /** Визуальный контроль результата импорта **/
    ofstream View_ship(odir + "View_ship.txt"); // Связываем файл с потоком
    Data->View(View_ship);                      // Отображаем хранилище
    View_ship.close();                          // Закрываем файл

    /** Проверка работы Get - методов **/
    size_t PrCount   = Data->GetColCount()-2;   // Число периодов проекта
    size_t ProdCount = Data->GetRowCount()-1;   // Число продуктов
    cout << "Число периодов проекта " << PrCount << endl;
    cout << "Число продуктов " << ProdCount << endl;

    strItem* Ship = Data->GetstrItem(1, 6, 2, 61, volume);      // Получаем массив
    if(!Ship) cout << "Ship = nullptr" << endl;
    strNameMeas* ProdNames = Data->GetNames(1, 6, 0, 1);        // Получаем массив
    if(!ProdNames) cout << "ProdNames = nullptr" << endl;

    /** Печатаем в файл полученные массивы в виде таблицы **/
    ofstream Output(odir + "Array.txt");                        // Связываем файл с потоком
    Output << setw(15) << "Name" << setw(15) << "Measure";      // Заголовки
    for(size_t j{}; j<PrCount; j++)
        Output << setw(15) << j;
    Output << endl;
    for(size_t i{}; i<ProdCount; i++) {                         // Данные
        Output << setw(15) << ((ProdNames+i)->name).substr(0,15) << setw(15) << (ProdNames+i)->measure;
        for(size_t j{}; j<PrCount; j++)
            Output << fixed << setprecision(7) << setw(15) << (Ship+PrCount*i+j)->volume;
        Output << endl;
    };
    Output.close();                                             // Закрываем файл

    /** Транспонирование матрицы внутреннего хранилища **/
    Data->Transpon();

    /** Визуальный контроль результата транспонирования **/
    ofstream View_ship_T(odir + "View_ship_T.txt");             // Связываем файл с потоком
    Data->View(View_ship_T);                                    // Выводим результат в файл
    View_ship_T.close();                                        // Закрываем файл

    /** Экспорт транспонированной матрицы внутреннего хранилища в CSV-файл **/
    ofstream ship_T(odir + "ship_T.csv");   // Связываем файл с потоком
    Data->csvExport(ship_T);                // Экспорт транспонированной матрицы
    ship_T.close();                         // Закрываем выходной файл

    delete Data;                            // Удаляем класс импорта-экспорта
    delete[] Ship;                          // Удаляем массив
    delete[] ProdNames;                     // Удаляем массив
    return 0;
}
