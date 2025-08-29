#include <iostream>
#include <manufact_module.h>    // ���������� ������ "������������"

using namespace std;

int main() {

    setlocale(LC_ALL, "Russian");   // ��������� �������� ����� ��� ������

    /** �������� ��������� ������� **/
    size_t PrCount = 480;           // ������������ �������
    size_t RMCount = 100;           // ������ ����� ������������ ��������
    size_t MSize = 100;             // ������ ����� ������� � ������������ ������� ��������� (��������������)
    size_t rcount  = 5;             // ����� ������������ ����� � ���������� � ��������� ��� ���������� ��������
    size_t duratio = 7;             // ������������ ����������������� ����� ��� ���������� ��������

    /** ������������ ������� ������� ���� �������� � ��������� ��������� **/
    strNameMeas* RMNames = new strNameMeas[RMCount];
    for(size_t i =sZero; i<RMCount; i++) {
        (RMNames+i)->name = "Rawmat_" + to_string(i);
        (RMNames+i)->measure = "kg";
    };

    /** ������������ ������� ��� (�������� �������������) ��� �������� **/
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

    /** ��������� ��� ���������� �������� **/
    strNameMeas* rnames = new strNameMeas[rcount] { {"Rawmat_0", "kg"}, {"Rawmat_1", "kg"},
    {"Rawmat_2", "kg"}, {"Rawmat_3", "kg"}, {"Rawmat_4", "kg"} };   // ������������ ��������
    decimal* recipe = new decimal[rcount*duratio]{  10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0,  // ���������
                                                  50.0,  0.0,  0.0, 50.0,  0.0,  0.0,  0.0,
                                                  30.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
                                                   0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 20.0,
                                                   0.0,  0.0,  0.0,  10.0, 0.0,  0.0,  0.0 };
    /** ������������ ����� ������� ��������� **/
    strItem* ProPlan = new strItem[PrCount];
    for(size_t i=sZero; i<PrCount; i++) {
        (ProPlan+i)->volume = 10.0;
        (ProPlan+i)->value = (ProPlan+i)->price  =  0.0;
    };

    /** ������������ ������ ��������� **/
    strNameMeas* ProdNames = new strNameMeas[MSize];
    for(size_t i =sZero; i<MSize; i++) {
        (ProdNames+i)->name = "Product_" + to_string(i);
        (ProdNames+i)->measure = "kg";
    };

    /** �������� ����������� **/
    clsManufactory* MMM = new clsManufactory(PrCount, RMCount, RMNames, MSize);

    /** ���� ���������, �������� � ����� ������� ��������� **/
    for(size_t i=sZero; i<MSize; i++)
        if(!MMM->SetManufItem((ProdNames+i)->name, (ProdNames+i)->measure, duratio, rcount, rnames, recipe, ProPlan))
            cout << "������ ���������� ������������ ��� " << (ProdNames+i)->name << " ��������" << endl;

    /** ������ ����������� � ����� � ���������� **/
    clock_t tstart1 = clock();   // ��������� ���������� ����� ����������
//    MMM->CalcRawMatPurchPlan();
//    MMM->CalcRawMatPurchPlan_future();
    MMM->CalcRawMatPurchPlan_thread();
    clock_t tend1 = clock();     // �������� ���������� ����� ����������

    /** ���� ��� ����� � ���������� **/
    MMM->SetRawMatPrice(RMPrice);

    /** ������ ������������� ��������� � �������������� ������������ **/
    clock_t tstart2 = clock();   // ��������� ���������� ����� ����������, ������� ������� ���������
//    MMM->Calculate();
//    MMM->Calculate_future();
    MMM->Calculate_thread();
    clock_t tend2 = clock();     // �������� ���������� ����� ����������, ������� ������� ���������
    double seconds = (double)(tend1 - tstart1 + tend2 - tstart2) / CLOCKS_PER_SEC;    // ������� ������ ���������� ���������
    printf("The time: %f seconds\n", seconds);                      // ����� ������� ������ ���������� �� �����


    delete[] RMNames;
    delete[] RMPrice;
    delete[] rnames;
    delete[] recipe;
    delete[] ProPlan;
    delete[] ProdNames;
    delete MMM;

    return 0;
}
