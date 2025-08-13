#include <iostream>
#include <warehouse_module.h>           // ���������� ������ ������

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");       // ��������� �������� ����� ��� ������
    cout << endl;
    cout << "***********************************************************" << endl;
    cout << "***   ������������ ��������� ������� ������ clsStorage   ***" << endl;
    cout << "***********************************************************" << endl << endl;

    /** ����� ������ ������� **/

    size_t PrCount = 17;            // ���������� �������� �������
    Currency Cur = RUR;             // �������� ������ �������
    AccountingMethod Amethod = AVG; // ������� ����� �������
    size_t RMCount = 10;            // ���������� �������������� �������
    bool PurchPerm = true;          // ���������� ������� � �������� � ����� �������
    PurchaseCalc flag = calc;       // ���������� �� �������������� ������ �������
    double Share = 0.5;             // ����� ����� � ������ ������, ����������
                                    // � ���� �� ������ �������� �� ���� ������
    TLack tlack;                    // ������� ����� � ����������

    /** ������������ ������� ���� ����� � ���������� � ��������� ��������� **/

    strNameMeas* RMNames = new strNameMeas[RMCount];
    for(size_t i{}; i<RMCount; i++) {
        (RMNames+i)->name = "Rawmat_" + to_string(i);
        (RMNames+i)->measure = "kg";
    };

    /** ������������ ��������� ������� �������� **/

    strItem* ship = new strItem[PrCount];
    for(size_t i=sZero; i<PrCount; i++) {
        (ship+i)->volume = 40.0;
        (ship+i)->value = (ship+i)->price  =  0.0;
    };

    /** ������������ ��������� ������� ������� (������ ���������� ���) **/

    strItem* purc = new strItem[PrCount];
    for(size_t i=sZero; i<PrCount; i++) {
        (purc+i)->volume = (purc+i)->value = 0.0;
        (purc+i)->price  =  (5.0+i);
    };

    /** ������� ����� ��� ���������� ��������� **/

    // ������� ��������� ��� ������
    clsStorage* Warehouse = new clsStorage(PrCount, Cur, Amethod, RMCount);
    for(size_t i=sZero; i<RMCount; i++) {   // ��������� ����� ��� i-�� ��������
        if(!Warehouse->SetSKU((RMNames+i)->name, (RMNames+i)->measure, flag,\
            Share, PurchPerm, ship, purc)) {
            cout << "������ ���������� ������ ��� " << (RMNames+i)->name << endl;
            delete[] RMNames;       // ������������ ������������ ������
            delete[] ship;
            delete[] purc;
            delete Warehouse;
            return EXIT_FAILURE;    // ������� �� ��������� � ����� ���������� ����������
        };
    };

    /** ������������ ������ **/

    int stop;
    cout << "������������ ����� ������� ���������. ��� ����������� ������� ENTER";
    stop = getchar();        // ������������ �� ����� ������ �������
    clock_t start = clock(); // ��������� ���������� ����� ����������, ������� ������� ���������
    // ������ ������� � ������ ��������.
    // tlack = Warehouse->Calculate();           // ���������������� ����������
    // tlack = Warehouse->Calculate_future();    // ������������ ����������
    tlack = Warehouse->Calculate_thread();       // ������������ ����������
    if(tlack.lack>dZero)
        cout << "������� ��� " << tlack.Name << " ����� " << tlack.lack << endl;
    clock_t end = clock();  // �������� ���������� ����� ����������, ������� ������� ���������
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;    // ������� ������ ���������� ���������
    printf("����� ����������: %f ������\n", seconds);           // ����� ������� ������ ���������� �� �����
    cout << "������������ ����� ������ ���������. ��� ����������� ������� ENTER";
    stop = getchar();

    /** ������ ����������� �������� **/
    // ����������� �������� ������������� ����������� ��������
    Warehouse->ViewChoise(shipment, price);

    /** ������������ ������������ ������ **/

    delete[] RMNames;
    delete[] ship;
    delete[] purc;
    delete Warehouse;

    return EXIT_SUCCESS;
}
