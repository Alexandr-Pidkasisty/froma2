#include <iostream>
#include <warehouse_module.h>           // Подключаем модуль склада

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");       // Установка русского языка для вывода
//    cout << endl;
//    cout << "***********************************************************" << endl;
//    cout << "***   Тестирование расчетных методов класса clsStorag   ***" << endl;
//    cout << "***********************************************************" << endl << endl;

    /** Общие данные проекта **/

    size_t PrCount = 480;           // Количество периодов проекта
    Currency Cur = RUR;             // Домашняя валюта проекта
    AccountingMethod Amethod = AVG; // Принцип учета запасов
    size_t RMCount = 100;           // Количество номенклатурных позиций
    bool PurchPerm = true;          // Разрешение закупок и отгрузок в одном периоде
    PurchaseCalc flag = calc;       // Разрешение на автоматический расчет закупок
    double Share = 0.5;             // Запас сырья в каждый период, выраженный
                                    // в доле от объема отгрузок за этот период
    TLack tlack;                    // Дефицит сырья и материалов

//	cout << "Количество периодов проекта 	   " << PrCount << endl;
//	cout << "Количество номенклатурных позиций " << RMCount << endl;
//	cout << "Принцип учета запасов 		   " << AccountTXT[Amethod] << endl;

    /** Формирование массива имен сырья и материалов с единицами измерения **/

    strNameMeas* RMNames = new strNameMeas[RMCount];
    for(size_t i{}; i<RMCount; i++) {
        (RMNames+i)->name = "Rawmat_" + to_string(i);
        (RMNames+i)->measure = "kg";
    };

    /** Формирование тестового массива отгрузок **/

    strItem* ship = new strItem[PrCount];
    for(size_t i=sZero; i<PrCount; i++) {
        (ship+i)->volume = 40.0;
        (ship+i)->value = (ship+i)->price  =  0.0;
    };

    /** Формирование тестового массива закупок (только закупочных цен) **/

    strItem* purc = new strItem[PrCount];
    for(size_t i=sZero; i<PrCount; i++) {
        (purc+i)->volume = (purc+i)->value = 0.0;
        (purc+i)->price  =  (5.0+i);
    };

    /** Создаем склад для нескольких продуктов **/

    // Создаем контейнер для склада
    clsStorage* Warehouse = new clsStorage(PrCount, Cur, Amethod, RMCount);
    for(size_t i=sZero; i<RMCount; i++) {   // Добавляем склад для i-го продукта
        if(!Warehouse->SetSKU((RMNames+i)->name, (RMNames+i)->measure, flag,\
            Share, PurchPerm, ship, purc)) {
            cout << "Ошибка добавления склада для " << (RMNames+i)->name << endl;
            delete[] RMNames;       // Освобождение динамической памяти
            delete[] ship;
            delete[] purc;
            delete Warehouse;
            return EXIT_FAILURE;    // выходим из программы с кодом неудачного завершения
        };
    };

    /** Рассчитываем проект **/

    clock_t start = clock(); // Начальное количество тиков процессора, которое сделала программа
    // Расчет проекта и расчет дефицита.
     tlack = Warehouse->Calculate();         	// Последовательные вычисления
    //tlack = Warehouse->Calculate_future();    // Параллельные вычисления
    //tlack = Warehouse->Calculate_thread();   	// Параллельные вычисления
    if(tlack.lack>dZero)
        cout << "Дефицит для " << tlack.Name << " равен " << tlack.lack << endl;
    clock_t end = clock();  // Конечное количество тиков процессора, которое сделала программа
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;    // Подсчет секунд выполнения алгоритма
    printf("Время вычислений: %f секунд\n", seconds);           // Вывод времени работы процессора на экран

//    Warehouse->ViewSettings();
//	Warehouse->ViewChoise(purchase, volume);
//	cout << "**************" << endl;

	/** Создаем ситуацию дефицита ресурсов **/

//    size_t Num_SKU = 49;                            				// Номер SKU, где создаем дефицит
//    size_t Num_Per = 239;                             				// Номер периода, где создается дефицит
//    Warehouse->SetAutoPurchase(Num_SKU, nocalc);    				// Запрещаем авторасчет закупок
//    strItem Item{0, 0, 0};                          				// Создаем нулевой приход ресурсов
//    if(Warehouse->SetDataItem(purchase, Item, Num_SKU, Num_Per)) {	// Редактируем уже рассчитанный массив закупок
//        cout << "SKU № " << Num_SKU << " в периоде № " << Num_Per << " изменен\n";
//    };

    /** Рассчитываем проект с дефицитом **/

//    start = clock(); // Начальное количество тиков процессора, которое сделала программа
//    // Расчет проекта и расчет дефицита.
//    // tlack = Warehouse->Calculate();          // Последовательные вычисления
//    // tlack = Warehouse->Calculate_future();   // Параллельные вычисления
//    tlack = Warehouse->Calculate_thread();  	// Параллельные вычисления
//    if(tlack.lack>dZero)
//        cout << "Дефицит для " << tlack.Name << " равен " << tlack.lack << endl;
//    end = clock();  // Конечное количество тиков процессора, которое сделала программа
//    seconds = (double)(end - start) / CLOCKS_PER_SEC;    // Подсчет секунд выполнения алгоритма
//    printf("Время вычислений: %f секунд\n", seconds);           // Вывод времени работы процессора на экран


//	Warehouse->ViewSettings();
//	Warehouse->ViewChoise(purchase, volume);

	/** Освобождение динамической памяти **/

    delete[] RMNames;
    delete[] ship;
    delete[] purc;
    delete Warehouse;

    return EXIT_SUCCESS;
}
