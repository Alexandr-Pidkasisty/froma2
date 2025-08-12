#include <iostream>
#include <warehouse_module.h>           // Подключаем модуль склада

using namespace std;

int main()
{
    /** Исходные данные **/

    size_t PrCount = 13;                // Количество периодов проекта
    AccountingMethod Amethod = AVG;     // Принцип учета запасов
    string Name_01 = "Pork Meat";       // Наименование позиции сырья
    string Meas_01 = "kg";              // Единица измерения
    bool PurchPerm = false;             // Разрешение закупок и отгрузок в одном периоде
    PurchaseCalc flag = calc;           // Разрешение на автоматический расчет закупок
    decimal Share = 0.5;                // Запас сырья в каждый период, выраженный в доле
                                        // от объема отгрузок за этот период
    strItem ship[PrCount] = {    0,  0,  0,
                                40,  5,  0,     // Отгрузки по периодам, начиная с 1-го
                                40,  0,  0,
                                45,  0,  0,
                                50,  0,  0,
                                55,  0,  0,
                                55,  0,  0,
                                55,  0,  0,
                                60,  0,  0,
                                60,  0,  0,
                                60,  0,  0,
                                60,  0,  0,
                                60,  0,  0 };

    strItem purc[PrCount] = {  100, 5, 500,     // Запас на начало проекта
                                 0,  6,  0,     // Закупочные цены по периодам
                                 0,  7,  0,
                                 0,  8,  0,
                                 0,  9,  0,
                                 0, 10,  0,
                                 0, 11,  0,
                                 0, 12,  0,
                                 0, 13,  0,
                                 0, 14,  0,
                                 0, 15,  0,
                                 0, 16,  0,
                                 0, 17,  0 };

    // Создаем склад для ресурса
    clsSKU* SKU_A = new clsSKU(PrCount, Name_01, Meas_01, Amethod, PurchPerm, flag, Share, ship);
    SKU_A->SetPurchase(purc, PrCount);      // Загружаем остаток на начало проекта
                                            // и закупочные цены сырья
    TLack lack = SKU_A->Calculate();        // Рассчитываем потребность в ресурсах
                                            // и проверяем дефицит
    cout << "Checking deficit. Deficit is " << lack.lack << endl;
    cout << "*** Purchase viewing ***" << endl;
    SKU_A->ViewData(purchase);              // Вывод данных закупок
    cout << "*** Shipment viewing ***" << endl;
    SKU_A->ViewData(shipment);              // Вывод данных отгрузок

    delete SKU_A;
    return 0;
}
