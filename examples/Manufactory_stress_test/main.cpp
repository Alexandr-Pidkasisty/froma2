#include <iostream>
#include <manufact_module.h>    // Подключаем модуль "производство"

using namespace std;

int main() {

    setlocale(LC_ALL, "Russian");   // Установка русского языка для вывода

    /** Основные параметры проекта **/
    size_t PrCount = 480;           // Длительность проекта
    size_t RMCount = 100;           // Полное число наименований ресурсов
    size_t MSize = 100;             // Полное число позиций в ассортименте готовой продукции (предварительно)
    size_t rcount  = 5;             // Число наименований сырья и материалов в рецептуре для единичного продукта
    size_t duratio = 7;             // Длительность производственного цикла для единичного продукта

    /** Формирование полного массива имен ресурсов с единицами измерения **/
    strNameMeas* RMNames = new strNameMeas[RMCount];
    for(size_t i =sZero; i<RMCount; i++) {
        (RMNames+i)->name = "Rawmat_" + to_string(i);
        (RMNames+i)->measure = "kg";
    };

    /** Формирование массива цен (удельной себестоимости) для ресурсов **/
    decimal* RMPrice = new decimal[RMCount*PrCount];
    for(size_t i=sZero; i<RMCount; i+=5 ) {
        for(size_t j=sZero; j<PrCount; j++) {
            *(RMPrice+PrCount*i+j) = 10.0;
            *(RMPrice+PrCount*(i+1)+j) =  5.0;
            *(RMPrice+PrCount*(i+2)+j) = 20.0;
            *(RMPrice+PrCount*(i+3)+j) = 2.5;
            *(RMPrice+PrCount*(i+4)+j) = 50.0;
        };
    };

    /** Рецептура для единичного продукта **/
    strNameMeas* rnames = new strNameMeas[rcount] { {"Rawmat_0", "kg"}, {"Rawmat_1", "kg"},
    {"Rawmat_2", "kg"}, {"Rawmat_3", "kg"}, {"Rawmat_4", "kg"} };   // Номенклатура ресурсов
    decimal* recipe = new decimal[rcount*duratio]{  10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0,  // Рецептура
                                                  50.0,  0.0,  0.0, 50.0,  0.0,  0.0,  0.0,
                                                  30.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
                                                   0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 20.0,
                                                   0.0,  0.0,  0.0,  10.0, 0.0,  0.0,  0.0 };
    /** Формирование плана выпуска продуктов **/
    strItem* ProPlan = new strItem[PrCount];
    for(size_t i=sZero; i<PrCount; i++) {
        (ProPlan+i)->volume = 10.0;
        (ProPlan+i)->value = (ProPlan+i)->price  =  0.0;
    };

    /** Формирование списка продуктов **/
    strNameMeas* ProdNames = new strNameMeas[MSize];
    for(size_t i =sZero; i<MSize; i++) {
        (ProdNames+i)->name = "Product_" + to_string(i);
        (ProdNames+i)->measure = "kg";
    };

    /** Создание произвоства **/
    clsManufactory* MMM = new clsManufactory(PrCount, RMCount, RMNames, MSize);

    /** Ввод продуктов, рецептур и плана выпуска продуктов **/
    for(size_t i=sZero; i<MSize; i++)
        if(!MMM->SetManufItem((ProdNames+i)->name, (ProdNames+i)->measure, duratio, rcount, rnames, recipe, ProPlan))
            cout << "Ошибка добавления производства для " << (ProdNames+i)->name << " продукта" << endl;

    /** Расчет потребности в сырье и материалах **/
    clock_t tstart1 = clock();   // Начальное количество тиков процессора
//    MMM->CalcRawMatPurchPlan();
//    MMM->CalcRawMatPurchPlan_future();
    MMM->CalcRawMatPurchPlan_thread();
    clock_t tend1 = clock();     // Конечное количество тиков процессора

    /** Ввод цен сырья и материалов **/
    MMM->SetRawMatPrice(RMPrice);

    /** Расчет себестоимости продуктов и незавершенного производства **/
    clock_t tstart2 = clock();   // Начальное количество тиков процессора, которое сделала программа
//    MMM->Calculate();
//    MMM->Calculate_future();
    MMM->Calculate_thread();
    clock_t tend2 = clock();     // Конечное количество тиков процессора, которое сделала программа
    double seconds = (double)(tend1 - tstart1 + tend2 - tstart2) / CLOCKS_PER_SEC;    // Подсчет секунд выполнения алгоритма
    printf("The time: %f seconds\n", seconds);                      // Вывод времени работы процессора на экран


    delete[] RMNames;
    delete[] RMPrice;
    delete[] rnames;
    delete[] recipe;
    delete[] ProPlan;
    delete[] ProdNames;
    delete MMM;

    return 0;
}
