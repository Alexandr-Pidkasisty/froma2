#include <iostream>
#include <warehouse_module.h>           // ���������� ������ ������

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");       // ��������� �������� ����� ��� ������
//    cout << endl;
//    cout << "***********************************************************" << endl;
//    cout << "***   ������������ ��������� ������� ������ clsStorag   ***" << endl;
//    cout << "***********************************************************" << endl << endl;

    /** ����� ������ ������� **/

    size_t PrCount = 480;           // ���������� �������� �������
    Currency Cur = RUR;             // �������� ������ �������
    AccountingMethod Amethod = AVG; // ������� ����� �������
    size_t RMCount = 100;           // ���������� �������������� �������
    bool PurchPerm = true;          // ���������� ������� � �������� � ����� �������
    PurchaseCalc flag = calc;       // ���������� �� �������������� ������ �������
    double Share = 0.5;             // ����� ����� � ������ ������, ����������
                                    // � ���� �� ������ �������� �� ���� ������
    TLack tlack;                    // ������� ����� � ����������

//	cout << "���������� �������� ������� 	   " << PrCount << endl;
//	cout << "���������� �������������� ������� " << RMCount << endl;
//	cout << "������� ����� ������� 		   " << AccountTXT[Amethod] << endl;

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

    clock_t start = clock(); // ��������� ���������� ����� ����������, ������� ������� ���������
    // ������ ������� � ������ ��������.
     tlack = Warehouse->Calculate();         	// ���������������� ����������
    //tlack = Warehouse->Calculate_future();    // ������������ ����������
    //tlack = Warehouse->Calculate_thread();   	// ������������ ����������
    if(tlack.lack>dZero)
        cout << "������� ��� " << tlack.Name << " ����� " << tlack.lack << endl;
    clock_t end = clock();  // �������� ���������� ����� ����������, ������� ������� ���������
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;    // ������� ������ ���������� ���������
    printf("����� ����������: %f ������\n", seconds);           // ����� ������� ������ ���������� �� �����

//    Warehouse->ViewSettings();
//	Warehouse->ViewChoise(purchase, volume);
//	cout << "**************" << endl;

	/** ������� �������� �������� �������� **/

//    size_t Num_SKU = 49;                            				// ����� SKU, ��� ������� �������
//    size_t Num_Per = 239;                             				// ����� �������, ��� ��������� �������
//    Warehouse->SetAutoPurchase(Num_SKU, nocalc);    				// ��������� ���������� �������
//    strItem Item{0, 0, 0};                          				// ������� ������� ������ ��������
//    if(Warehouse->SetDataItem(purchase, Item, Num_SKU, Num_Per)) {	// ����������� ��� ������������ ������ �������
//        cout << "SKU � " << Num_SKU << " � ������� � " << Num_Per << " �������\n";
//    };

    /** ������������ ������ � ��������� **/

//    start = clock(); // ��������� ���������� ����� ����������, ������� ������� ���������
//    // ������ ������� � ������ ��������.
//    // tlack = Warehouse->Calculate();          // ���������������� ����������
//    // tlack = Warehouse->Calculate_future();   // ������������ ����������
//    tlack = Warehouse->Calculate_thread();  	// ������������ ����������
//    if(tlack.lack>dZero)
//        cout << "������� ��� " << tlack.Name << " ����� " << tlack.lack << endl;
//    end = clock();  // �������� ���������� ����� ����������, ������� ������� ���������
//    seconds = (double)(end - start) / CLOCKS_PER_SEC;    // ������� ������ ���������� ���������
//    printf("����� ����������: %f ������\n", seconds);           // ����� ������� ������ ���������� �� �����


//	Warehouse->ViewSettings();
//	Warehouse->ViewChoise(purchase, volume);

	/** ������������ ������������ ������ **/

    delete[] RMNames;
    delete[] ship;
    delete[] purc;
    delete Warehouse;

    return EXIT_SUCCESS;
}
