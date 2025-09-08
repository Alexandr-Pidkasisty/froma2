#ifndef LONGREALTEST_H
#define LONGREALTEST_H

#include <functional>               // ������� ������ ���������� (std::function<bool(double, double))
#include "serialization_module.hpp" // ���������� ������� ������������ � ��������������

/****************************************************************************************************/
/****************************************************************************************************/
/*** 																						      ***/
/***   Copyright � 2025 ����������� ��������� ��������										      ***/
/*** 																						      ***/
/***   ������ �������� ��������� �����, ���������� ����� ������� ������������ ����������� �	      ***/
/***   ������������� ������������ (����� � ����������� �����������), ������������ ������������    ***/
/***   ����������� ����������� ��� �����������, ������� �������������� ����� �� �������������,    ***/
/***   �����������, ���������, �������, ����������, ���������������, ����������������� �/���      ***/
/***   ������� ����� ������������ �����������, � ����� �����, ������� ��������������� ������      ***/
/***   ����������� �����������, ��� ���������� ��������� �������:								  ***/
/*** 																					      	  ***/
/***   ��������� ���� ����������� �� ��������� ����� � ������ ������� ������ ���� �������� ��	  ***/
/***   ��� ����� ��� �������� ����� ������� ������������ �����������.							  ***/
/*** 																						      ***/
/***   ������ ����������� ����������� ��������������� ���� ���ܻ, ��� �����-���� ��������, ����   ***/
/***   ���������� ��� ���������������, ������� �������� �������� �����������, ������������ ��	  ***/
/***   ��� ����������� ���������� � ���������� ���������, �� �� ������������� ���. �� � �����	  ***/
/***   ������ ������ ��� ��������������� �� ����� ��������������� �� �����-���� �����, �� �����   ***/
/***   ��� �� ���� �����������, � ��� �����, ��� �������� ���������, ������� ��� ���� ��������,   ***/
/***   ��������� ��-�� ������������� ������������ ����������� ��� ���� �������� � �����������     ***/
/***   ������������.																			  ***/
/*** 																						      ***/
/****************************************************************************************************/
/****************************************************************************************************/

/** �������� ���������������� ���� ������� **/
typedef bool (LongReal::*TLogical)(const LongReal&) const;      // TLogical - ��� ��������� �� ���������� �������-����
                                                                // ������ LongReal � ����������� const LongReal&
typedef LongReal (LongReal::*TArithm)(const LongReal&) const;   // TArithm - ��� ��������� �� �������������� �������-����
                                                                // ������ LongReal � ����������� const LongReal&
typedef std::function<bool(double, double)> Comparator;         // ��� ��������������� ������� ��� ���������� ��������
typedef std::function<double(double, double)> Arithmetor;       // ��� ��������������� ������� ��� �������������� ��������

/** �������������� ������� ���������� �������� **/
equal_to<double> equals;        // �������������� ������ ��� �������� ���������
not_equal_to<double> noeq;      // �������������� ������ ��� �������� �����������
greater<double> bigger;         // �������������� ������ ��� �������� �������� "������"
less<double> lesser;            // �������������� ������ ��� �������� �������� "������"
greater_equal<double> ebigger;  // �������������� ������ ��� �������� �������� "������ ��� �����"
less_equal<double> elesser;     // �������������� ������ ��� �������� �������� "������ ��� �����"

/** �������������� ������� �������������� �������� **/
plus<double> Plus;              // �������������� ������ ��������������� ��������
minus<double> Minus;            // �������������� ������ ��������������� ���������
multiplies<double> Mult;        // �������������� ������ ��������������� ���������
divides<double> Div;            // �������������� ������ ��������������� �������

namespace nmLRtest {
    const size_t sZero = 0;
}   // nmLRtest

using namespace nmLRtest;

class LongRealTest {
/** ����� ������������ ��� ������������ ������ LongReal. **/
    private:
        size_t lrlenth;     // ����� ������� � ������� ��� �����
        LongReal* data;     // ������������ ������ � ������� ���� LongReal
        double* dcml;       // ������������ ������ � ������� ������ ���� double
    public:
        LongRealTest() {
            lrlenth = sZero;
            data = nullptr;
            dcml = nullptr;
        }   // Default Ctor

        LongRealTest(const LongRealTest&) = delete;             // ������ Copy/Move Ctor
        LongRealTest& operator=(const LongRealTest&) = delete;  // ������ Copy/Move operator=

        ~LongRealTest() {
            if(data) delete[] data;
            if(dcml) delete[] dcml;
        }   // Dtor

        void Init() {
        /** ��������� ������ ������ ��� ������ **/
            lrlenth = 12;
            data = new(nothrow) LongReal[lrlenth];
            if(data) {
                *data     = 0;                      // ���������� ����������� ��������
                *(data+1) = numeric_limits<double>::infinity();
                *(data+2) = -numeric_limits<double>::infinity();
                *(data+3) = numeric_limits<double>::quiet_NaN();
                *(data+4) = 1;
                 (data+4)->Expchange(MaxEtype-3);
                *(data+5) = -(*(data+4));
                *(data+6) = 1;
                 (data+6)->Expchange(MinEtype);
                *(data+7) = -(*(data+6));
                for(size_t i=8; i<lrlenth; i++) {   // ��������� ��������� �����
                    *(data+i) = (((double)(rand()) - (double)(rand())) / (double)(rand()))*10;
                };
            };
        }   // Init

        void DcmlCopy() {
        /** ��������� ������ ������ ��� ��������� � ��������� � ������� double **/
            dcml = new(nothrow) double[lrlenth];   // �������� ������ �������
            *(dcml)   = 0.0;
            *(dcml+1) = numeric_limits<double>::infinity();
            *(dcml+2) = -numeric_limits<double>::infinity();
            *(dcml+3) = numeric_limits<double>::quiet_NaN();
            if(dcml) {
                for(size_t i=4; i<lrlenth; i++)
                    *(dcml+i) = (data+i)->Get<double>();
            };
        }   // DcmlCopy

        void View() const {
        /** ����� ����������� �������� ����������� ������������� �������� ������ ��� ����� **/
            if(!data) {
                cout << "������ ��� ������������ �����������\n";
                return;
            };
            for(size_t i=sZero; i<lrlenth; i++) {
                cout << "data[" << i << "] is:" << endl;
                (data+i)->View(cout);
            }
        }   // View()

        void dataView() const {
        /** ����� ������ �� ����� **/
            if(!data || !dcml) {
                cout << "������ ��� ������������ ��� ����� ��� �������� �����������\n";
                return;
            };
            for(size_t i=sZero; i<8; i++) {
                cout << setiosflags(ios::left) << "data[" << resetiosflags(ios::left) << setw(2) << i \
                << "] = " << setw(15) << (data+i)->EGet(5) << setiosflags(ios::left) << "; double data[" \
                << resetiosflags(ios::left) << setw(2) << i << "] = " << setw(15) << *(dcml+i) << endl;
            };
            for(size_t i=8; i<lrlenth; i++) {
                cout << setiosflags(ios::left) << "data[" << resetiosflags(ios::left) << setw(2) << i \
                << "] = " << setw(15) << (data+i)->Get(7) << setiosflags(ios::left) << "; double data[" \
                << resetiosflags(ios::left) << setw(2) << i << "] = " << setw(15) << *(dcml+i) << endl;
            };
        }   // dataView

        void Compare(string zn, TLogical f) {
        /** ������� ������������ ��������� ����� ���� LongReal ����� �����. ����� ���������� ������� ��� �����
        �� ������� � ������� data. ���������: string zn - ������ � ���������� ��������� ��������� (==, !=, >,
        <, >=, <=,), f - ��������� ���� TLogical �� �������-���� ������ LongReal, �������������� ���������
        ��������� (��� ������� ���� bool, ������� ���������� const LongReal& x). ��������!!! ����� �� ���������
        ������������ ������� � ������� ��������! **/
            cout << setiosflags(ios::right) << setw(14) << "LongReal " << setw(17) << "LongReal "\
            << setw(13) << "���������" << resetiosflags(ios::right) << endl;
            for(size_t i=sZero; i<lrlenth; i++) {           // ������� ���� ����� �� i
                for(size_t j=sZero; j<lrlenth; j++) {       // ������� ���� ����� �� j
                    cout << setw(13) << (data+i)->EGet(5) << zn << setw(13) << (data+j)->EGet(5) \
                    << " --> " << boolalpha << ((data+i)->*f)(*(data+j)) << endl;
                    // ������ �������� (*(data+i) == *(data+j)) � ����������� ���������� ���������
                    // ((data+i)->*f)(*(data+j)), ��� f - ��������� �� ������������ �������, *(data+j) - ��������
                }
            }
        }   // Compare

        void TwiceCompare(string zn, TLogical f, Comparator comp) {
        /** ������� ������������ ��������� ����� ���� LongReal ����� �����. ����� ���������� ������� ��� �����
        �� ������� � ������� data. ���������: string zn - ������ � ���������� ��������� ��������� (==, !=, >,
        <, >=, <=,), f - ��������� ���� TLogical �� �������-���� ������ LongReal, �������������� ���������
        ��������� (��� ������� ���� bool, ������� ���������� const LongReal& x). ��������!!! ����� �� ���������
        ������������ ������� � ������� ��������! � ������� �� ����������� ������, ���������� ����� ��������� �����
        ���� double �� ������� ����� dcml. **/
            cout << setiosflags(ios::right) << setw(14) << "LongReal " << setw(17) << "LongReal "\
            << setw(13) << "���������" <<  setw(14) << "double" << resetiosflags(ios::right) << endl;
            for(size_t i=sZero; i<lrlenth; i++) {           // ������� ���� ����� �� i
                for(size_t j=sZero; j<lrlenth; j++) {       // ������� ���� ����� �� j
                    cout << setw(15) << (data+i)->EGet(5) << zn << setw(15) << (data+j)->EGet(5) \
                    << " --> " << boolalpha << ((data+i)->*f)(*(data+j)) << setw(13) << comp(*(dcml+i), *(dcml+j)) << endl;
                    // ������ �������� (*(data+i) == *(data+j)) � ����������� ���������� ���������
                    // ((data+i)->*f)(*(data+j)), ��� f - ��������� �� ������������ �������, *(data+j) - ��������
                }
            }
        }   // TwiceCompare

        void Arithmetic(string zn, TArithm f) {
        /** ����� ������������ �������������� ���������� ������ LongReal. ����� ���������� ��������������
        �������� ��� ������ ����� �� ������� � ������� data. ���������: string zn - ������ � ���������� ���������
        (+, -, *, /), f - ��������� ���� TArithm �� �������-���� ������ LongReal, �������������� ���������
        �������������� ��������. ��������!!! ����� �� ��������� ������������ ������� � ������� ��������! **/
            for(size_t i=sZero; i<lrlenth; i++) {           // ������� ���� ����� �� i
                for(size_t j=sZero; j<lrlenth; j++) {       // ������� ���� ����� �� j
                    cout << setw(13) << (data+i)->EGet(5) << zn << setw(13) << (data+j)->EGet(5) \
                    << " = " << (((data+i)->*f)(*(data+j))).EGet(5) << endl;
                }
            }
        }   // Arithmetic

        void TwiceArithmetic(string zn, TArithm f, Arithmetor math) {
        /** ����� ������������ �������������� ���������� ������ LongReal. ����� ���������� ��������������
        �������� ��� ������ ����� �� ������� � ������� data. ���������: string zn - ������ � ���������� ���������
        (+, -, *, /), f - ��������� ���� TArithm �� �������-���� ������ LongReal, �������������� ���������
        �������������� ��������. ��������!!! ����� �� ��������� ������������ ������� � ������� ��������!
        � ������� �� ����������� ������, ���������� ����� �������� ��� ������� ���� double �� ������� ����� dcml.**/
            cout << setiosflags(ios::right) << setw(14) << "LongReal " << setw(16) << "LongReal "\
            << setw(16) << "���������" <<  setw(18) << "double" << resetiosflags(ios::right) << endl;
            for(size_t i=sZero; i<lrlenth; i++) {           // ������� ���� ����� �� i
                for(size_t j=sZero; j<lrlenth; j++) {       // ������� ���� ����� �� j
                    cout << setw(15) << (data+i)->EGet(5) << zn << setw(15) << (data+j)->EGet(5) \
                    << " = " << setw(14) << ((data+i)->*f)(*(data+j)).EGet(5) << setw(14) << math(*(dcml+i), *(dcml+j)) << endl;
                }
            }
        }   // TwiceArithmetic
};
#endif // LONGREALTEST_H
