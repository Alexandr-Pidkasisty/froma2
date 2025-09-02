#include <iostream>
#include <fstream>
#include <impex_module.h>
#include <common_values.hpp>

using namespace std;
using namespace nmBPTypes;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");                   // ��������� �������� �����

    string dir = "../../examples/Impex_example/";   // ������������ ������ ������

    /** ����������� ������ �� CSV-����� �� ���������� ��������� **/
    ifstream input(dir + "ship.csv");               // ��������� ���� � �������
    const char ch =';';                             // �������� �����������
    clsImpex* Data = new clsImpex(input, ch);       // ����������� ������ �� �����
    input.close();                                  // ��������� ����

    /** ���������� �������� ���������� ������� **/
    ofstream View_ship(dir + "View_ship.txt");  // ��������� ���� � �������
    Data->View(View_ship);                      // ���������� ���������
    View_ship.close();                          // ��������� ����

    /** �������� ������ Get - ������� **/
    size_t PrCount   = Data->GetColCount()-2;   // ����� �������� �������
    size_t ProdCount = Data->GetRowCount()-1;   // ����� ���������
    cout << "����� �������� ������� " << PrCount << endl;
    cout << "����� ��������� " << ProdCount << endl;

    strItem* Ship = Data->GetstrItem(1, 6, 2, 61, volume);      // �������� ������
    if(!Ship) cout << "Ship = nullptr" << endl;
    strNameMeas* ProdNames = Data->GetNames(1, 6, 0, 1);        // �������� ������
    if(!ProdNames) cout << "ProdNames = nullptr" << endl;

    /** �������� � ���� ���������� ������� � ���� ������� **/
    ofstream Output(dir + "Array.txt");                         // ��������� ���� � �������
    Output << setw(15) << "Name" << setw(15) << "Measure";      // ���������
    for(size_t j{}; j<PrCount; j++)
        Output << setw(15) << j;
    Output << endl;
    for(size_t i{}; i<ProdCount; i++) {                         // ������
        Output << setw(15) << ((ProdNames+i)->name).substr(0,15) << setw(15) << (ProdNames+i)->measure;
        for(size_t j{}; j<PrCount; j++)
            Output << fixed << setprecision(7) << setw(15) << (Ship+PrCount*i+j)->volume;
        Output << endl;
    };
    Output.close();                                             // ��������� ����

    /** ���������������� ������� ����������� ��������� **/
    Data->Transpon();

    /** ���������� �������� ���������� ���������������� **/
    ofstream View_ship_T(dir + "View_ship_T.txt");              // ��������� ���� � �������
    Data->View(View_ship_T);                                    // ������� ��������� � ����
    View_ship_T.close();                                        // ��������� ����

    /** ������� ����������������� ������� ����������� ��������� � CSV-���� **/
    ofstream ship_T(dir + "ship_T.csv");    // ��������� ���� � �������
    Data->csvExport(ship_T);                // ������� ����������������� �������
    ship_T.close();                         // ��������� �������� ����

    delete Data;                            // ������� ����� �������-��������
    delete[] Ship;                          // ������� ������
    delete[] ProdNames;                     // ������� ������
    return 0;
}
