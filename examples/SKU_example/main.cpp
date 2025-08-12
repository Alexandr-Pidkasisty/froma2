#include <iostream>
#include <warehouse_module.h>           // ���������� ������ ������

using namespace std;

int main()
{
    /** �������� ������ **/

    size_t PrCount = 13;                // ���������� �������� �������
    AccountingMethod Amethod = AVG;     // ������� ����� �������
    string Name_01 = "Pork Meat";       // ������������ ������� �����
    string Meas_01 = "kg";              // ������� ���������
    bool PurchPerm = false;             // ���������� ������� � �������� � ����� �������
    PurchaseCalc flag = calc;           // ���������� �� �������������� ������ �������
    decimal Share = 0.5;                // ����� ����� � ������ ������, ���������� � ����
                                        // �� ������ �������� �� ���� ������
    strItem ship[PrCount] = {    0,  0,  0,
                                40,  5,  0,     // �������� �� ��������, ������� � 1-��
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

    strItem purc[PrCount] = {  100, 5, 500,     // ����� �� ������ �������
                                 0,  6,  0,     // ���������� ���� �� ��������
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

    // ������� ����� ��� �������
    clsSKU* SKU_A = new clsSKU(PrCount, Name_01, Meas_01, Amethod, PurchPerm, flag, Share, ship);
    SKU_A->SetPurchase(purc, PrCount);      // ��������� ������� �� ������ �������
                                            // � ���������� ���� �����
    TLack lack = SKU_A->Calculate();        // ������������ ����������� � ��������
                                            // � ��������� �������
    cout << "Checking deficit. Deficit is " << lack.lack << endl;
    cout << "*** Purchase viewing ***" << endl;
    SKU_A->ViewData(purchase);              // ����� ������ �������
    cout << "*** Shipment viewing ***" << endl;
    SKU_A->ViewData(shipment);              // ����� ������ ��������

    delete SKU_A;
    return 0;
}
