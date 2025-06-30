#ifndef FROMA2_COMMON_VALUES_H_INCLUDED
#define FROMA2_COMMON_VALUES_H_INCLUDED

/**     ���� ���� �������� ������ ���������� ������������ ����������� ��� ��������������
        �������������, ����������� ������� � ������������ ������������ ������������ �����������
        "Free Operation Manager 2" (���������� FROMA2).
**/

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

#include <iostream>                 // ������� ����������� ��� ����������� �����-������
#include <fstream>                  // ������� ����������� ��� ����������� �����-������ � ���� �� �����
#include <iomanip>                  // ����������� ������� � ������������� ��� ������� cout � cin
#include <algorithm>                // ������� copy_n
#include "LongReal_module.h"        // ������� ���������� � ����� LongReal
#include "serialization_module.hpp" // ������ ������������� � ��������������
#include "progress_module.hpp"      // ������ �������� ��������� ����������

using namespace std;

/****************************************************************************************************/
/**                                                                                                **/
/**                       �������� ������ ��� �������������� ������� � �������.                    **/
/**                                     ������ �� 2025.02.13                                       **/
/**                                                                                                **/
/****************************************************************************************************/

/** ��� ������������ �����. ��������� "decimal" ����� ��������� �������� "double" (���������� � �++
    ��� ������������� �����) ��� "LongReal". ��� LongReal - ��� ����������� ��� ���������� FROMA2 ���
    ������������ ����� ���������� ��������. ��������� �������� ���������� decimal � ���� ������ ����
    ������ ��� ���� ������������ ����� �� ���� ������� ���������� �� ����������� ������ LONGREAL:

    �������� ����:

    typedef double decimal;     ������������ ����� ������������ ����� double
    typedef LongReal decimal;   ������������ ����� ������������ ����� LongReal  **/

typedef LongReal decimal;

/** ��� ���������� ���������. ��������� "type_progress" ����� ��������� ������ �������� � �����������
    �� ������������� ������ ��������-����������. � ������ progress_module.hpp ������ ��������-��������� ���
    ����������� ������ ����� clsprogress_bar. ��� ����������� ���������� � ����������� � �����������
    �����������, ���������, ��������, � ������� ���������� wxWidget, ����� ������������ ��������-���������
    ����� wxProgressDialog �� ������ wx/progdlg.h **/

typedef clsprogress_bar type_progress;

namespace nmBPTypes {   /** �������� ����������� ���������, ����, ������� **/

/** �������� ����������� ��������� **/
const size_t    sZero = 0,
                sOne  = 1,
                sTwo  = 2;
const decimal   dZero = 0.0;

const decimal   epsln = 1e-7;   // ���������� ����� ��������, ���������� ������������� ����
                                // ��� ��������� ������������ ����� ���� � ������ � �����

const string    EmpStr = "";    // ������ ������
const string    stSpc  = " ";   // ������

/** �������� ����������� ���� **/
enum ReportData{volume = sZero, price, value};              // ��� ������ ������ (� �����������, � �������
                                                            // � ����������� �������� ���������)
enum Currency{RUR = sZero, USD, EUR, CNY};                  // ��� ������ (���������� �����, ������ ���, ����,
                                                            // ��������� ����. ����� ����������� ������� ��������
const string CurrencyTXT[] = {"RUR", "USD", "EUR", "CNY"};  // ������������� ����� � ���� ������. ��� ���������� ������
                                                            // ���� Currency, ������ ����������� ������� ��������

struct strItem {        /** ��� ��������� ������ �������� **/
	decimal volume;     // ������ ������ �������� � ����������� ��������� (��������, � ��)
	decimal price;      // ���� �� ������� ������ (��������, ���./��)
	decimal value;      // ��������� ������ (��������, ���)

    bool StF(ofstream &_outF) {
    /** ����� ������������ ��������� ������� � ���� **/
        if(!SEF(_outF, volume)) return false;
        if(!SEF(_outF,  price)) return false;
        if(!SEF(_outF,  value)) return false;
        return true;
	}   // StF

	bool RfF(ifstream &_inF) {
    /** ����� �������������� ��������� ������� �� ����� **/
        if(!DSF(_inF, volume)) return false;
        if(!DSF(_inF,  price)) return false;
        if(!DSF(_inF,  value)) return false;
        return true;
	}   // RfF

	friend ostream& operator<<(ostream &stream, strItem st) {   //
	    /** ���������� ��������� ������ ���������� � ����� ostream **/
        stream << st.volume << ", ";
        stream << st.price << ", ";
        stream << st.value << "\n";
        return stream;
    };  // ostream &operator<<

};  // strItem

struct strNameMeas {    /** ��� �������� ��������� ������ �������� **/
    string name;        // ������������ �������������� �������
    string measure;     // ������������ ������� ��������� �������������� ������� (��������, �� ��� ����)
};  // strNameMeas

}   // namespace nmBPTypes

/****************************************************************************************************/

template<typename T>
void var_cpy(T* destin, const T* source, const size_t Num) {
/** ����� �������� ������ �� �������, �� ������� ��������� ��������� source � ������, �� ������� ���������
��������� destin. ������ ����� �������� ����� Num. ���� ������� ����� ���������� ���������� ���, �����������
�������������� ������� ������ � ������� ������� memcpy. � ��������� ������ ����������� ������������ � �������
������� copy_n. ���������� ���������� ��� (is_trivially_copyable) - ��� ���, ��������� �������� ��������
����������� �, �������������, ��� ����������� ������������� ����������� ����� ����� ������, ��� ���� �� ���
����������� � ������� memcpy. ��� ����������� ��� ��������� �����, ���������� ���������� ������� � ��������
����� �������� �����. https://cplusplus.com/reference/type_traits/is_trivially_copyable **/
    // ���� ��� ��������� ������� - ���������� ���������� ���,
    // �� �������� ������� �������� � ������� ������� memcpy
    // ����� �������� ������� ����������� � ������� ������� std::copy_n
    (std::is_trivially_copyable<T>::value) ? memcpy(destin, source, sizeof(T)*Num) : std::copy_n(source, Num, destin);
}   // var_cpy


/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                          ��������� � ������ ��� ������� ����������� �������� � �������                              **/
/**                                                                                                                     **/
/*************************************************************************************************************************/
/** version from 2024.03.31 **/

namespace nmPrntSetup {

const size_t limit  = 9;                    // ���������� ����� �������� ��� ������ ������
const size_t sPrec  = 2;                    // ���������� ������ ����� ������� � �������� ������ (��������, �����������)
const char chFill   = ' ';                  // �����������
                                            // ��������� ��� �������� �����
const string    c_TableName = "Name",       // "������������"
                c_TableMeas = "Measure",    // "��.���������"
                c_ByVolume  = "By volume",  // "� ����������� ���������"
                c_ByPrice   = "By price",   // "� �������� ����������� ���������"
                c_ByValue   = "By value",   // "� ����������� ���������"
                c_HCurrency = "RUR";        // ������ �� ��������� ��� ������ clsRePrint ("RUR", "CNY" � ����.)

class clsTextField {
/** ����� ��� ��������� ���������� ������ ��������� �����
https://stackoverflow.com/questions/9206669/making-a-table-with-printf-in-c **/
    private:
        size_t myWidth; // ������ ����
    public:
        clsTextField( size_t width ) : myWidth( width ) {}
        friend std::ostream& operator<<( std::ostream& dest, clsTextField const& manip ) {
            dest.setf( std::ios_base::left, std::ios_base::adjustfield );
            dest.fill( chFill );
            dest.width( manip.myWidth );
            return dest;
        }   // friend std::ostream& operator<<
};  // clsTextField

class clsDataField {
/** ����� ��� ��������� ���������� ������ ����� � ��������� �������
https://stackoverflow.com/questions/9206669/making-a-table-with-printf-in-c **/
    private:
        size_t myWidth;     // ������ ����
        size_t precision;   // ���������� ������ ����� �������
    public:
        clsDataField( size_t width ) : myWidth( width ), precision(sPrec) {}                // Ctor � ��������� sPrec ����� ����� �������
        clsDataField( size_t width, size_t prec ) : myWidth( width ), precision(prec) {}    // Ctor � ���������� ��������
        friend std::ostream& operator<<( std::ostream& dest, clsDataField const& manip ) {
            dest.setf( std::ios_base::right, std::ios_base::adjustfield );
            dest.setf( std::ios_base::fixed, std::ios_base::floatfield );
            dest.fill( chFill );
            dest.precision( manip.precision );
            dest.width( manip.myWidth );
            return dest;
        }   // friend std::ostream& operator<<
};  // clsDataField


}   // nmPrntSetup

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                       ������ ����������� ��������                                                   **/
/**                                                                                                                     **/
/*************************************************************************************************************************/
/** version from 2024.03.31 **/

namespace nmPrntSrvs {      // ��������� � ������� ���������� � ������������ ���� nmPrntSrvs

using namespace nmBPTypes;  // ��������� ������������ ���� �� ������������ ���� nmBPTypes
using namespace nmPrntSetup;// ��������� ������������ ���������, ���� � ������ �� ������������ ���� nmPrntSetup

/** ��������� **/
const size_t sNames = 15;   // ��������� ������� ������ ���� ��� ������������� � ������ clsTextField
const size_t sNeas  = 12;   // ��������� ������� ������ ��.��������� ��� ������������� � ������ clsTextField
const size_t sNumb  = 12;   // ��������� ������� ������ �������� ������ ��� ������������� � ������ clsDataField
const string CurUnt     = c_HCurrency + "/";   // ������ �� ����������� �������

template<typename Tdata, typename TName>
void ArrPrint(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount) {
/** ������� ����������� ������, ��������� �� ������� ������� ������������ � ����������� �������� � �������. ���������:
const size_t ncount - ����� �����, ������ ����� ��������� ������� names[] � �������������� �����, names[] - ������ �
�������������� ����� � ��������� ���������, data[] - ���������� ������, ������ ��������� ������� �������� ncount*dcount
� �������, dcount - ����� �������� �������. ������� ������� ������������� ������������� ������ clsRePrint ��� ������
������ �� �����. ������ ����� ������������� � �� ����������. **/
    if(ncount==sZero) return;           // �������� ������������ ����������
    if(dcount==sZero) return;
    if(!names) return;
    if(!data) return;
    clsTextField name(sNames);          // ��������� ������� ������ ���� � ������� ������ clsTextField
    clsTextField meas(sNeas);           // ��������� ������� ������ ������ ���������
    clsDataField number(sNumb);         // ��������� ������� ������ �������� ������ � ������� ������ clsDataField
    size_t b = sZero;                   // ��������� ����� ������� ��� ������ ������
    size_t e = limit;                   // ���������� �������� ��� ������
    auto lambda = [&](){                // ������-������� ��� ������ �������
        cout << name << c_TableName <<stSpc;                        // ����� ���������� ��������
        cout << meas << c_TableMeas <<stSpc;                        // ����� ���������� ��������
        for(size_t j=b; j<e; j++) {                             // ����� ���������� �������� (������ ��������)
            cout << number << j <<stSpc;
        };
        cout << endl;
        for(size_t i=sZero; i<ncount; i++) {                    // ���� �� �������
            cout << name << (names+i)->name << stSpc;             // ����� �������� ������
            cout << meas << (names+i)->measure << stSpc;          // ����� ������� ���������
            for(size_t j=b; j<e; j++) {                         // ���� �� ��������
                cout << number << *(data+dcount*i+j) <<stSpc;     // ����� ������ �������
            };
            cout << endl;
        };  // ���� �� �������
    };  // lambda
    while(e < dcount) {     // ���� ���������� �������� ��� ������ ������ ������ ����� ��������, ������� ��
        cout << endl;       // e-�������� �� �������� � �����������
        lambda();           /** ����� ������� ������ ��������, ������� limit **/
        b += limit;         // �������� ��������� ����� �������
        e += limit;         // �������� ���������� �������� ��� ������
    };  // while
    cout << endl;
    e = dcount;
    lambda();               /** ����� ������� �������� ������� **/
    cout << endl;
}   // ArrPrint

template<typename Tdata, typename TName>
void ArrPrint(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount, ReportData flg) {
/** ������������� ������� ����������� ������ ��� ����������� ������, ���������� �� ������ ���� strItem. ��������������
�������� - flg - ����, ������������ ��� ������������ ������: volume, price ��� value. **/
    if(ncount==sZero) return;
    if(dcount==sZero) return;
    if(!names) return;
    if(!data) return;
    clsTextField name(sNames);          // ��������� ������� ������ ���� � ������� ������ clsTextField
    clsTextField meas(sNeas);           // ��������� ������� ������ ������ ���������
    clsDataField number(sNumb);         // ��������� ������� ������ �������� ������ � ������� ������ clsDataField
    size_t b = sZero;                   // ��������� ����� ������� ��� ������ ������
    size_t e = limit;                   // ���������� �������� ��� ������
    string aMeas;                       // ������� ���������
    auto lambda = [&]() {
        if((flg==volume) || (flg==price))
            if(flg==volume) cout << name << c_ByVolume << stSpc; // ����� "� ����������� ���������"
            else            cout << name << c_ByPrice  << stSpc; // ����� "� �������� ����������� ���������"
        else                cout << name << c_ByValue  << stSpc; // ����� "� ����������� ���������"
        cout << endl;
        cout << name << c_TableName <<stSpc;                   // ����� ���������� ��������
        cout << meas << c_TableMeas <<stSpc;                   // ����� ���������� ��������
        for(size_t j=b; j<e; j++) {                             // ����� ���������� �������� (������ ��������)
            cout << number << j <<stSpc;
        };
        cout << endl;
        // ����� ������ � �������
        for(size_t i=sZero; i<ncount; i++) {                    // ���� �� �������
            cout << name << (names+i)->name << stSpc;             // ����� �������� ������
            if((flg==volume) || (flg==price))
                if(flg==volume) aMeas = (names+i)->measure;
                else            aMeas = CurUnt + (names+i)->measure;
            else                aMeas = c_HCurrency;
            cout << meas << aMeas << stSpc;                       // ����� ������� ���������
                for(size_t j=b; j<e; j++) {                     // ���� �� ��������
                    if((flg==volume) || (flg==price))
                        if(flg==volume) cout << number << (data+dcount*i+j)->volume << stSpc; // ����� ������ �������
                        else            cout << number << (data+dcount*i+j)->price  << stSpc; // ����� ������ �������
                    else                cout << number << (data+dcount*i+j)->value  << stSpc; // ����� ������ �������
                };
            cout << endl;
        };  // ���� �� �������
    };  // lambda
    while(e < dcount) {                 // ���� ���������� �������� ��� ������ ������ ������ ����� ��������, ������� ��
        cout << endl;                   // e-�������� �� �������� � �����������
        lambda();
        b += limit;                                             // �������� ��������� ����� �������
        e += limit;                                             // �������� ���������� �������� ��� ������
    };  // while
    cout << endl;
    e = dcount;
    lambda();
    cout << endl;
}   // ArrPrint with ReportData flag

template<typename Tdata, typename TName>
void ArrPrint(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount, ReportData flg,
const string& _hmcur) {
/** ������������� ������� ����������� ������ ��� ����������� ������, ���������� �� ������ ���� strItem. ��������������
�������� - flg - ����, ������������ ��� ������������ ������: volume, price ��� value; _hmcur - ������ ������ **/
    if(ncount==sZero) return;
    if(dcount==sZero) return;
    if(!names) return;
    if(!data) return;
    clsTextField name(sNames);          // ��������� ������� ������ ���� � ������� ������ clsTextField
    clsTextField meas(sNeas);           // ��������� ������� ������ ������ ���������
    clsDataField number(sNumb);         // ��������� ������� ������ �������� ������ � ������� ������ clsDataField
    size_t b = sZero;                   // ��������� ����� ������� ��� ������ ������
    size_t e = limit;                   // ���������� �������� ��� ������
    string aMeas;                       // ������� ���������
    auto lambda = [&]() {
        if((flg==volume) || (flg==price))
            if(flg==volume) cout << name << c_ByVolume << stSpc; // ����� "� ����������� ���������"
            else            cout << name << c_ByPrice  << stSpc; // ����� "� �������� ����������� ���������"
        else                cout << name << c_ByValue  << stSpc; // ����� "� ����������� ���������"
        cout << endl;
        cout << name << c_TableName <<stSpc;                    // ����� ���������� ��������
        cout << meas << c_TableMeas <<stSpc;                    // ����� ���������� ��������
        for(size_t j=b; j<e; j++) {                             // ����� ���������� �������� (������ ��������)
            cout << number << j <<stSpc;
        };
        cout << endl;
        // ����� ������ � �������
        for(size_t i=sZero; i<ncount; i++) {                    // ���� �� �������
            cout << name << (names+i)->name << stSpc;           // ����� �������� ������
            if((flg==volume) || (flg==price))
                if(flg==volume) aMeas = (names+i)->measure;
                else            aMeas = _hmcur + '/' + (names+i)->measure;
            else                aMeas = _hmcur;
            cout << meas << aMeas << stSpc;        // ����� ������� ���������
                for(size_t j=b; j<e; j++) {                     // ���� �� ��������
                    if((flg==volume) || (flg==price))
                        if(flg==volume) cout << number << (data+dcount*i+j)->volume << stSpc; // ����� ������ �������
                        else            cout << number << (data+dcount*i+j)->price  << stSpc; // ����� ������ �������
                    else                cout << number << (data+dcount*i+j)->value  << stSpc; // ����� ������ �������
                };
            cout << endl;
        };  // ���� �� �������
    };  // lambda
    while(e < dcount) {                 // ���� ���������� �������� ��� ������ ������ ������ ����� ��������, ������� ��
        cout << endl;                   // e-�������� �� �������� � �����������
        lambda();
        b += limit;                                             // �������� ��������� ����� �������
        e += limit;                                             // �������� ���������� �������� ��� ������
    };  // while
    cout << endl;
    e = dcount;
    lambda();
    cout << endl;
}   // ArrPrint with ReportData flag

}   // namespace nmPrntSrvs

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                   ���������, ������ � ������� ��� ������ ������                                     **/
/**                                                                                                                     **/
/*************************************************************************************************************************/
/** version from 2024.08.31 **/

namespace nmRePrint {

using namespace nmBPTypes;      // ��������� ������������ ���� �� ������������ ���� nmBPTypes
using namespace nmPrntSetup;    // ��������� ������������ ���������, ���� � ������ �� ������������ ���� nmPrntSetup
                                // ��������� ��� ������������ �������� �����
const size_t    c_n_min = 11,   // ����������� ������ ���� ��� �������� ��������/ �������
                c_m_min = 12,   // ����������� ������ ���� ��� �������� ������� ���������
                c_d_min =  3,   // ����������� ������ ���� ��� �������� ������
                c_n_max = 15,   // ������������ ������ ���� ��� �������� ��������/ �������
                c_m_max = 15,   // ������������ ������ ���� ��� �������� ������� ���������
                c_d_max = 16;   // ������������ ������ ���� ��� �������� ������ (�������� decimal 15 ������, ���� ���� �����������)

const size_t smblcunt  = 120;   // ���������� ����� �������� � ������

const char ch1 = ' ';
const char ch2 = '*';
const size_t uOne = 1;
const size_t uTwo = 2;
const size_t uThree = 3;

template<typename T>
size_t CalcSingleDataLenth(const T& val) {
/** ����� ������� ���������� ������ ��� ��������������� ������ ����� ���� T. **/
    const T tOne = 1,
            tTen = 10;
    T temp = val;
    size_t i = sZero;
    while(temp > tOne) {    // ����� ����� ��������
        temp = temp / tTen;
        i++;
    };// while
    i += 4;                 // ��������� ���� �������, ����� ������� ����� ����� ������� � ������ ����
    return i;
}   // CalcSingleDataLenth for decimal

template<typename T=string>
size_t CalcSingleDataLenth(const string& str) {
/** ����� ������� ���������� ����� ��� ������ ������ **/
    int len = str.length(); // ���������� ����� ���������� ���� string
    return len += 3;        // ��������� ������ ����
}   // CalcSingleDataLenth for string type

template<typename T>
size_t CalcArrayDataLenth(const size_t _asize, const T _arr[], const size_t _minlimit, const size_t _maxlimit) {
/** ����� ������� ������������� ���������� ������ � ��������� ������� ����� ��� �����. **/
    if((!_arr) || (_asize==sZero)) return sZero;        // ��������� ����������
    size_t temp = _minlimit;
    for(size_t i=sZero; i<_asize; i++) {
        temp = CalcSingleDataLenth(*(_arr+i))>temp ? CalcSingleDataLenth(*(_arr+i)): temp;
    };
    if(temp>_maxlimit) temp = _maxlimit;
    return temp;
} // CalcArrayDataLenth

template<typename Tdata, typename TName>
class clsRePrint {
/** �������� ����� ��� ���������������� ������ ������ �� ����� ��� � ����. **/
    private:
        bool calcflag;          // ���� ��������������/������ ��������� ���������� ��������� ������

        size_t nwidth;          // ������ ������� ��� ���� � ���������
        size_t mwidth;          // ������ ������� ��� ���� � ��.���������
        size_t dwidth;          // ������ ������� ��� ��������� ����
        size_t precis;          // ���������� ������ ����� �������

        size_t n_min;           // ����������� ������ ���� ��� ��������
        size_t m_min;           // ����������� ������ ���� ��� ��.���������
        size_t d_min;           // ����������� ������ ��� ��������� ����
        size_t n_max;           // ����������� ������ ���� ��� ��������
        size_t m_max;           // ����������� ������ ���� ��� ��.���������
        size_t d_max;           // ����������� ������ ��� ��������� ����

        clsTextField* tname;    // ������ ������ ����
        clsTextField* tmeas;    // ������ ������ ������ ���������
        clsDataField* tnumb;    // ������ ������ �������� ������

        string TableName;       // ��������� ������� ������� ������
        string TableMeas;       // ��������� ������� ������� ������
        string ByVolume;        // ��������� ������� ��� ������ ����������� ����������
        string ByPrice;         // ��������� ������� ��� ������ �������� ����������� ����������
        string ByValue;         // ��������� ������� ��� ������ ������ ����������� ����������
        string HCurrency;       // ������ �������. ������������ � �������� ��������� price � value ���� strItem

        size_t rowcount;        // ���������� ����� ������
        size_t colcount;        // ���������� �������� ������
        const TName* rownames;  // ��������� �� ������ � ���������� �����
        const Tdata* Tcoldata;  // ��������� �� ������ � ������� ���� Tdata

        streambuf* backup;      // ��������� �� ���������� ���� ����� ��� ���������� ��������� ������ ������ cout
        ofstream filestr;       // ���������� ��� ������ � �������� �����
        ostream* out;           // ����� ��� ������

    public:
        clsRePrint() {
        /** ����������� � �������������� ���������� ������ ������� ������ **/
            calcflag = true;    // �������������� ��������� ���������� ��������� ������
            nwidth = sZero;     // ��������� �������� �� ���������
            mwidth = sZero;
            dwidth = sZero;
            precis = sPrec;
            n_min = c_n_min;    // ��������� �������� �� ���������
            m_min = c_m_min;
            d_min = c_d_min;
            n_max = c_n_max;
            m_max = c_m_max;
            d_max = c_d_max;
            tname = nullptr;
            tmeas = nullptr;
            tnumb = nullptr;
            TableName = c_TableName;
            TableMeas = c_TableMeas;
            ByVolume =  c_ByVolume;
            ByPrice  =  c_ByPrice;
            ByValue  =  c_ByValue;
            HCurrency = c_HCurrency;
            rowcount = sZero;
            colcount = sZero;
            rownames = nullptr;
            Tcoldata = nullptr;
            backup = nullptr;
            out = &std::cout;
        }   // Ctor clsRePrint

        clsRePrint(const size_t namewidth, const size_t measwidth, const size_t datawidth) {
        /** ����������� � ������ ���������� ������ ������� ������. ���������: namewidth - ������ ���� "������������",
         measwidth - ������ ���� "������� ���������", datawidth - ������ ������� � �������. **/
            calcflag = false;   // ������ ��������� ���������� ��������� ������
            nwidth = namewidth; // ��������� ��������� ��������
            mwidth = measwidth;
            dwidth = datawidth;
            precis = sPrec;
            n_min = c_n_min;    // ��������� �������� �� ���������
            m_min = c_m_min;
            d_min = c_d_min;
            n_max = c_n_max;
            m_max = c_m_max;
            d_max = c_d_max;
            tname = nullptr;
            tmeas = nullptr;
            tnumb = nullptr;
            TableName = c_TableName;
            TableMeas = c_TableMeas;
            ByVolume =  c_ByVolume;
            ByPrice  =  c_ByPrice;
            ByValue  =  c_ByValue;
            HCurrency = c_HCurrency;
            rowcount = sZero;
            colcount = sZero;
            rownames = nullptr;
            Tcoldata = nullptr;
            backup = nullptr;
            out = &std::cout;
        }   // Ctor clsRePrint

        clsRePrint(const string& filename, ios_base::openmode mode) {
        /** ����������� � �������������� ���������� ������ ������� ������ ��� ��������������� ������ � ����.
        ���������: filename - ��� �����, ios_base::openmode mode - ���� ������ ������ � ������: app -
        ������ ����������� � ����� ������������� �����, trunc - ���������� ����� ��������� ����� �������. **/
            calcflag = true;    // �������������� ��������� ���������� ��������� ������
            nwidth = sZero;     // ��������� �������� �� ���������
            mwidth = sZero;
            dwidth = sZero;
            precis = sPrec;
            n_min = c_n_min;    // ��������� �������� �� ���������
            m_min = c_m_min;
            d_min = c_d_min;
            n_max = c_n_max;
            m_max = c_m_max;
            d_max = c_d_max;
            tname = nullptr;
            tmeas = nullptr;
            tnumb = nullptr;
            TableName = c_TableName;
            TableMeas = c_TableMeas;
            ByVolume =  c_ByVolume;
            ByPrice  =  c_ByPrice;
            ByValue  =  c_ByValue;
            HCurrency = c_HCurrency;
            rowcount = sZero;
            colcount = sZero;
            rownames = nullptr;
            Tcoldata = nullptr;
            streambuf* psbuf;               // ��������������� ����� ��� �����
            filestr.open(filename, mode);   // ��������� �������� ���������� � ������ ��� ������ � ��������� ���� ��� ������
            backup = cout.rdbuf();          // ���������� ��������� �������� ������ cout � ���������� backup
            psbuf = filestr.rdbuf();        // �������� ������ � ������ �����
            cout.rdbuf(psbuf);              // �������������� ����� �� cout � ����: ������ ��� ����� � ����
            out = &std::cout;
        }   // Ctor clsRePrint

        clsRePrint(const size_t namewidth, const size_t measwidth, const size_t datawidth, \
            const string& filename, ios_base::openmode mode) {
        /** ����������� � ������ ���������� ������ ������� ������ ��� ��������������� ������ � ����. ���������:
        namewidth - ������ ���� "������������", measwidth - ������ ���� "������� ���������", datawidth - ������
        ������� � �������; filename - ��� �����, ios_base::openmode mode - ���� ������ ������ � ������: app -
        ������ ����������� � ����� ������������� �����, trunc - ���������� ����� ��������� ����� �������. **/
            calcflag = false;   // ������ ��������� ���������� ��������� ������
            nwidth = namewidth; // ��������� ��������� ��������
            mwidth = measwidth;
            dwidth = datawidth;
            precis = sPrec;
            n_min = c_n_min;    // ��������� �������� �� ���������
            m_min = c_m_min;
            d_min = c_d_min;
            n_max = c_n_max;
            m_max = c_m_max;
            d_max = c_d_max;
            tname = nullptr;
            tmeas = nullptr;
            tnumb = nullptr;
            TableName = c_TableName;
            TableMeas = c_TableMeas;
            ByVolume =  c_ByVolume;
            ByPrice  =  c_ByPrice;
            ByValue  =  c_ByValue;
            HCurrency = c_HCurrency;
            rowcount = sZero;
            colcount = sZero;
            rownames = nullptr;
            Tcoldata = nullptr;
            streambuf* psbuf;               // ��������������� ����� ��� �����
            filestr.open(filename, mode);   // ��������� �������� ���������� � ������ ��� ������ � ��������� ���� ��� ������
            backup = cout.rdbuf();          // ���������� ��������� �������� ������ � ���������� backup
            psbuf = filestr.rdbuf();        // �������� ������ � ������ �����
            cout.rdbuf(psbuf);              // �������������� ����� �� cout � ����: ������ ��� ����� � ����
            out = &std::cout;
        }   // Ctor clsRePrint

        ~clsRePrint() {
            if(tname) delete tname;         // ���� ���������� ����������, �� ������� ��
            if(tmeas) delete tmeas;
            if(tnumb) delete tnumb;
            rownames = nullptr;
            Tcoldata = nullptr;
            if(backup) {
                cout.rdbuf(backup);         // ��������������� ��������� ������ cout
                backup = nullptr;           // �������� ����������
                filestr.close();            // ��������� ����
            };
        }   // Dtor clsRePrint

        void ResetReport() {
        /** ����� ���������� ������� ������ ����, ������� ��������� �� ������� �������, ���������� ���������������
        ������ � ����� cout � ��������� ���� **/
            if(tname) { delete tname; tname = nullptr; };
            if(tmeas) { delete tmeas; tmeas = nullptr; };
            if(tnumb) { delete tnumb; tnumb = nullptr; };
            rownames = nullptr;
            Tcoldata = nullptr;
            precis = sPrec;
            if(backup) {                    // ���� ���� ����������� ����� ������ � ����� cout, ��:
                cout.rdbuf(backup);         // ��������������� ��������� ������ cout
                backup = nullptr;           // ���������� ���������
                filestr.close();            // ��������� ����
            };
            out = &std::cout;
        }   // ResetReport

        void SetStream(ostream& val) {
        /** ��������� ������ ��� ������ **/
            out = &val;
        }   // SetStream

        void SetLimits(size_t _nmin, size_t _mnin, size_t _dmin, size_t _nmax, size_t _mmax, size_t _dmax) {
        /** ����� ������������� ����������� � ������������ ������� ��� �����. **/
            n_min = _nmin;
            m_min = _mnin;
            d_min = _dmin;
            n_max = _nmax;
            m_max = _mmax;
            d_max = _dmax;
        }   // SetLimits

        void SetHeadings(const string& _tname, const string& _tmeasure, const string& _byvol, const string& _byprice, const string& _byvalue) {
        /** ����� ������������� ��������� ������� **/
            TableName = _tname;
            TableMeas = _tmeasure;
            ByVolume = _byvol;
            ByPrice  = _byprice;
            ByValue  = _byvalue;
        }   // SetHeadings

        void SetCurrency(Currency val) {
        /** ����� ������������� ������������ ������, �������������� � ������ ������ �� price- � value- �����
        ������� ������ ���� strItem. **/
            HCurrency = *(nmBPTypes::CurrencyTXT+val);
        }   // SetCurrency

        void ResetToAuto() {
        /** ����� ��������� ���� ��������� ���������� ��������� ������ � �������������� �����. **/
            calcflag = true;    // ������������� ���� � �������������� ����� ������� ������ ����� ��������� ������
            nwidth = sZero;     // �������� ����
            mwidth = sZero;
            dwidth = sZero;
            precis = sPrec;
        }   // ResetToAuto

        void SetToManual(const size_t namewidth, const size_t measwidth, const size_t datawidth, size_t prec) {
        /** ����� ��������� ���� ��������� ���������� ��������� ������ � ������ ����� � ������ �������� �����.
        ���������: namewidth - ������ ���� "������������", measwidth - ������ ���� "������� ���������", datawidth
        - ������ ������� � �������, prec - ���������� ������ ����� �������. **/
            calcflag = false;   // ������ ��������� ���������� ��������� ������
            nwidth = namewidth; // ����������� ��������� �������� �����
            mwidth = measwidth;
            dwidth = datawidth;
            precis = prec;
        }   // SetToManual

        void SetPrecision(size_t _prec) {
        /** ����� ������������� ���������� ������ ����� ������� ��� ������ � ����� ������������ ����� **/
            precis = _prec;
        }   // SetPrecision

        bool SetReport(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount) {
        /** ����� ������ ������ ��� ������ � ������������ ������ �������. ���������: ncount - ����� ��������� �������
        names � ����� ����� ������; names - ������ � ������� � ��������� ��������� ����� (�������� ���������� ��� - ���
        ��� strNameMeas); data - ������ � �������; dcount - ����������� ������� data � ����� �������� ������. **/
            if( (ncount==sZero) || (!names) || (dcount==sZero) || (!data) ) return false;   // ��������� ����������
            rowcount = ncount;                              // ������ ������� ������������ �����
            colcount = dcount;                              // ������ ������� ������ ��� ��������
            rownames = names;                               // ����������� ������ ������������ �����
            Tcoldata = data;                                // ��������� �� ������ ������ ��� ��������
            if(calcflag) {                                  // ���� ������ ��������������, ��
                string* ntemp = new(nothrow) string[rowcount];  // �������� ������ ���������������� ������� ������������
                if(!ntemp) return false;                        // ���� ������ �� �������, �� ����� � false
                string* mtemp = new(nothrow) string[rowcount];  // �������� ������ ���������������� ������� � ��������� ���������
                if(!mtemp) { delete[] ntemp; return false; };   // ���� ������ �� �������, �� ����� � false
                for(size_t i=sZero; i<rowcount; i++) {          // ��������� ��������� ������� ��� �������
                    *(ntemp+i) = (rownames+i)->name;            // ��������� ��������������� ������ �������������� �����
                    *(mtemp+i) = (rownames+i)->measure;         // � ��������� ���������
                };
                nwidth = CalcArrayDataLenth(rowcount, ntemp, n_min, n_max); // ���������� ������ ���� ��� ������� � �������������
                mwidth = CalcArrayDataLenth(rowcount, mtemp, m_min, m_max); // ���������� ������ ���� ��� ������� � ��.���������
                dwidth = CalcArrayDataLenth(colcount, Tcoldata, d_min, d_max); // ���������� ������ ���� ��� ������
                delete[] ntemp;
                delete[] mtemp;
            };
            if(tname) delete tname;                             // ���� ������� ����������, �� ������� ��
            if(tmeas) delete tmeas;
            if(tnumb) delete tnumb;
            tname = new(nothrow) clsTextField(nwidth);          // ������ ���� ��� ������ ������ ��������� �������
            if(!tname) { return false; };
            tmeas = new(nothrow) clsTextField(mwidth);          // ������ ���� ��� ������ ������ ��������� �������
            if(!tmeas) { delete tname; return false; };
            tnumb = new(nothrow) clsDataField(dwidth, precis);  // ������ ���� ��� ������ ������� � �������
            if(!tnumb) { delete tname; delete tmeas; return false; };
            return true;
        }   // SetReport

        bool SetReport(const size_t ncount, const TName names[], const Tdata data[], const size_t dcount, ReportData flg) {
        /** ����� ������ ������ ��� ������ � ������������ ������ ������� ��� ������, ��������� ������ flg. ���������:
        ncount - ����� ��������� ������� names � ����� ����� ������; names - ������ � ������� � ��������� ��������� �����;
        data - ������ � ������� � ����� ������; dcount - ����������� ������� data � ����� �������� ������ **/
            if( (ncount==sZero) || (!names) || (dcount==sZero) || (!data) ) return false;   // ��������� ����������
            rowcount = ncount;
            colcount = dcount;
            rownames = names;
            Tcoldata = data;
            if(calcflag) {
                string* ntemp = new(nothrow) string[rowcount];
                if(!ntemp) return false;
                string* mtemp = new(nothrow) string[rowcount];
                if(!mtemp) { delete[] ntemp; return false; };
                decimal* dtemp = new(nothrow) decimal[colcount];
                if(!dtemp) { delete[] ntemp; delete[] mtemp; return false; };
                for(size_t i=sZero; i<rowcount; i++) {            // ��������� ��������� ������� ��� �������
                    *(ntemp+i) = (rownames+i)->name;
                    *(mtemp+i) = (rownames+i)->measure;
                };
                nwidth = CalcArrayDataLenth(rowcount, ntemp, n_min, n_max); // ���������� ������ ����
                mwidth = CalcArrayDataLenth(rowcount, mtemp, m_min, m_max); // ���������� ������ ����
                switch(flg) {
                case volume:                                // ��������� ��������� ������� ��� ������� � ����������� �� ����� flg
                    for(size_t i=sZero; i<colcount; i++) {
                        *(dtemp+i) = (Tcoldata+i)->volume;
                    } break;
                case price:
                    for(size_t i=sZero; i<colcount; i++) {
                        *(dtemp+i) = (Tcoldata+i)->price;
                    } break;
                case value:
                    for(size_t i=sZero; i<colcount; i++) {
                        *(dtemp+i) = (Tcoldata+i)->value;
                    } break;
                };
                dwidth = CalcArrayDataLenth(colcount, dtemp, d_min, d_max); // ���������� ������ ����
                delete[] ntemp;
                delete[] mtemp;
                delete[] dtemp;
            };
            if(tname) delete tname;                             // ���� ������� ����������, �� ������� ��
            if(tmeas) delete tmeas;
            if(tnumb) delete tnumb;
            tname = new(nothrow) clsTextField(nwidth);          // ������ ���� ��� ������ ������ ��������� �������
            if(!tname)  return false;
            tmeas = new(nothrow) clsTextField(mwidth);          // ������ ���� ��� ������ ������ ��������� �������
            if(!tmeas) { delete[] tname; return false; };
            tnumb = new(nothrow) clsDataField(dwidth, precis);  // ������ ���� ��� ������ ������� � �������
            if(!tnumb) { delete[] tname; delete[] tmeas; return false; };
            return true;
        }   // SetReport

        void Print() {
        /** ����� ������� �� ����� ������, ��������� � ������� ������� SetReport **/
            if(rowcount==sZero) return;           // �������� ������������ ����������
            if(colcount==sZero) return;
            if(!rownames) return;
            if(!Tcoldata) return;
            TName* tmpnames = new(nothrow) TName[rowcount]; // ������� ��������������� ������, ������ � ������� ����� �������
            if(!tmpnames) return;                           // ���� ������ �� ��������, �� ������� �� �������
            for(size_t i=sZero; i<rowcount; i++) {
                *(tmpnames+i) = *(rownames+i);              // �������� �� ��������������� ������ ������ �� ������� � ����������
                (tmpnames+i)->name.resize(nwidth);          // ������� ������ � ����������
                (tmpnames+i)->measure.resize(mwidth);       // ������� ������ � ��������� ���������
            };
            TableName.resize(nwidth);                       // ������� ������ ����������: ������������ ��������
            TableMeas.resize(mwidth);                       // ������� ������ ����������: ������� ���������
            const size_t limit = dwidth!=sZero?(smblcunt-nwidth-mwidth)/dwidth:sZero;    // ���������� ����� �������� ��� ������ ������
            size_t b = sZero;                   // ��������� ����� ������� ��� ������ ������
            size_t e = limit;                   // ���������� �������� ��� ������
            /** ������ ������� **/
            auto lambda = [this, &b, &e, &tmpnames](){      // �����-�������
                *out << *tname << TableName <<stSpc;        // ����� ���������� ��������
                *out << *tmeas << TableMeas <<stSpc;        // ����� ���������� ��������
                for(size_t j=b; j<e; j++) {                 // ����� ���������� �������� (������ ��������)
                    *out << *tnumb << j <<stSpc;
                };
                *out << endl;
                // ����� ������ � �������
                for(size_t i=sZero; i<rowcount; i++) {                      // ���� �� �������
                    *out << *tname << (tmpnames+i)->name << stSpc;          // ����� �������� ������
                    *out << *tmeas << (tmpnames+i)->measure << stSpc;       // ����� ������� ���������
                    for(size_t j=b; j<e; j++) {                             // ���� �� ��������
                        *out << *tnumb << *(Tcoldata+colcount*i+j) <<stSpc; // ����� ������ �������
                    };
                    *out << endl;
                };  // ���� �� �������

            };  // lambda
            while(e < colcount) {               // ���� ���������� �������� ��� ������ ������ ������ ����� ��������, ������� ��
                *out << endl;                   // e-�������� �� �������� � �����������
                lambda();                       // ����� ������� ������ ��������, ������� limit
                b += limit;                     // �������� ��������� ����� �������
                e += limit;                     // �������� ���������� �������� ��� ������
            };
            *out << endl;
            e = colcount;
            lambda();                           // ����� ������� �������� �������
            *out << endl;
            delete[] tmpnames;                  // ������� ��������������� ������
        }   // Print

        void Print(ReportData flg) {
        /** ����� ������� �� ����� ������, ��������� � ������� ������� SetReport. ���������: flg - ����� ������ ��� �������
        ���� strItem **/
            if(rowcount==sZero) return;           // �������� ������������ ����������
            if(colcount==sZero) return;
            if(!rownames) return;
            if(!Tcoldata) return;
            TName* tmpnames = new(nothrow) TName[rowcount];
            if(!tmpnames) return;
            for(size_t i=sZero; i<rowcount; i++) {
                *(tmpnames+i) = *(rownames+i);
                (tmpnames+i)->name.resize(nwidth);
                (tmpnames+i)->measure.resize(mwidth);
            };
            TableName.resize(nwidth);
            TableMeas.resize(mwidth);
            const size_t limit = dwidth!=sZero?(smblcunt-nwidth-mwidth)/dwidth:sZero;    // ���������� ����� �������� ��� ������ ������
            size_t b = sZero;                   // ��������� ����� ������� ��� ������ ������
            size_t e = limit;                   // ���������� �������� ��� ������
            string aMeas;                       // ������� ���������
            /** ������ ������� **/
            auto lambda = [this, &b, &e, &flg, &aMeas, &tmpnames]() {  // �����-�������
                if((flg==volume) || (flg==price))
                    if(flg==volume) *out << *tname << ByVolume << stSpc; // ����� "� ����������� ���������"
                    else            *out << *tname << ByPrice  << stSpc; // ����� "� �������� ����������� ���������"
                else                *out << *tname << ByValue  << stSpc; // ����� "� ����������� ���������"
                *out << endl;
                *out << *tname << TableName <<stSpc;                 // ����� ���������� ��������
                *out << *tmeas << TableMeas <<stSpc;                 // ����� ���������� ��������
                for(size_t j=b; j<e; j++) {                          // ����� ���������� �������� (������ ��������)
                    *out << *tnumb << j <<stSpc;
                };
                *out << endl;
                // ����� ������ � �������
                for(size_t i=sZero; i<rowcount; i++) {               // ���� �� �������
                    *out << *tname << (tmpnames+i)->name << stSpc;   // ����� �������� ������
                    if((flg==volume) || (flg==price))
                        if(flg==volume) aMeas = (tmpnames+i)->measure;
                        else            aMeas = HCurrency + "/" + (tmpnames+i)->measure;
                    else                aMeas = HCurrency;
                    aMeas.resize(mwidth);                            // ����������� ����� ������, ����� ������� �� �������
                    *out << *tmeas << aMeas << stSpc;                // ����� ������� ���������
                    for(size_t j=b; j<e; j++) {                      // ���� �� ��������
                        if((flg==volume) || (flg==price))
                            if(flg==volume) *out << *tnumb << (Tcoldata+colcount*i+j)->volume << stSpc; // ����� ������ �������
                            else            *out << *tnumb << (Tcoldata+colcount*i+j)->price  << stSpc; // ����� ������ �������
                        else                *out << *tnumb << (Tcoldata+colcount*i+j)->value  << stSpc; // ����� ������ �������
                    };
                    *out << endl;
                };  // ���� �� �������
            };  // lambda
            while(e < colcount) {               // ���� ���������� �������� ��� ������ ������ ������ ����� ��������, ������� ��
                *out << endl;                   // e-�������� �� �������� � �����������
                lambda();                       // ����� ������� ������ ��������, ������� limit
                b += limit;                     // �������� ��������� ����� �������
                e += limit;                     // �������� ���������� �������� ��� ������
            };
            *out << endl;
            e = colcount;
            lambda();                           // ����� ������� �������� �������
            *out << endl;
            delete[] tmpnames;
        }   // Print

        void Header0(const string& val) {
        /** ����� ������� ��������� 0-�� ������ ��� ��������� ���������� ������. Num - ������ ���������
        � ��������, val - ������ ��������� **/
            size_t Num = smblcunt+uThree;
            size_t len = val.length();
            size_t field1, field2;
            field1 = (Num - len) / uTwo;
            field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
            auto SLIne = [&Num, this]() {
                *out << "**";
                for(size_t i=sZero; i<Num; i++)
                    *out << ch2;
                *out << "**\n";
            };
            auto SBord = [&Num, this]() {
                *out << "**";
                for(size_t i=sZero; i<Num; i++)
                    *out << ch1;
                *out << "**\n";
            };
            SLIne();
            SLIne();
            SBord();
            *out << "**";
            for(size_t i=sZero; i<field1; i++)
                *out << ch1;
            *out << val;
            for(size_t i=sZero; i<field2; i++)
                *out << ch1;
            *out << "**\n";
            SBord();
            SLIne();
            SLIne();
        }   // Header0

        void Header1(const string& val) {
        /** ����� ������� ��������� 1-�� ������ ��� ��������� ���������� ������. Num - ������ ���������
        � ��������, val - ������ ��������� **/
            size_t Num = smblcunt+uThree;
            size_t len = val.length();
            size_t field1, field2;
            field1 = (Num - len) / uTwo;
            field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
            auto SLIne = [&Num, this]() {
                *out << "**";
                for(size_t i=sZero; i<Num; i++)
                    *out << ch2;
                *out << "**\n";
            };
            SLIne();
            *out << "**";
            for(size_t i=sZero; i<field1; i++)
                *out << ch1;
            *out << val;
            for(size_t i=sZero; i<field2; i++)
                *out << ch1;
            *out << "**\n";
            SLIne();
        }   // Header1

        void Header2(const string& val) {
        /** ����� ������� ��������� 2-�� ������ ��� ��������� ���������� ������. Num - ������ ���������
        � ��������, val - ������ ��������� **/
            size_t Num = smblcunt+uThree;
            size_t len = val.length();
            size_t field1, field2;
            field1 = (Num - len) / uTwo;
            field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
            *out << "**";
            for(size_t i=sZero; i<field1-sOne; i++)
                *out << ch2;
            *out << ' ' << val << ch1;
            for(size_t i=sZero; i<field2-sOne; i++)
                *out << ch2;
            *out << "**\n";
        }   // Header2

};  // clsRePrint

template<typename T=void>
void PrintHeader0(std::ostream& out, const size_t& Num, const string& val) {
/** ����� ������� ��������� ������ 0 � ����� os ��� ��������� ���������� ������. out - �������� �����,
Num - ������ ��������� � ��������, val - ������ ��������� **/
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / uTwo;
    field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
    auto SLIne = [&Num, &out]() {
        out << "**";
        for(size_t i=sZero; i<Num; i++)
            out << ch2;
        out << "**\n";
        };
    auto SBord = [&Num, &out]() {
        out << "**";
        for(size_t i=sZero; i<Num; i++)
            out << ch1;
        out << "**\n";
        };
    SLIne();
    SLIne();
    SBord();
    out << "**";
    for(size_t i=sZero; i<field1; i++)
        out << ch1;
    out << val;
    for(size_t i=sZero; i<field2; i++)
        out << ch1;
    out << "**\n";
    SBord();
    SLIne();
    SLIne();
}   // PrintHeader0

template<typename T=void>
void PrintHeader1(std::ostream& out, const size_t& Num, const string& val) {
/** ����� ������� ��������� ������ 1 � ����� os ��� ��������� ���������� ������. out - �������� �����,
Num - ������ ��������� � ��������, val - ������ ��������� **/
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / uTwo;
    field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
    auto SLIne = [&Num, &out]() {
        out << "**";
        for(size_t i=sZero; i<Num; i++)
            out << ch2;
        out << "**\n";
        };
    SLIne();
    out << "**";
    for(size_t i=sZero; i<field1; i++)
        out << ch1;
    out << val;
    for(size_t i=sZero; i<field2; i++)
        out << ch1;
    out << "**\n";
    SLIne();
}   // PrintHeader1

template<typename T=void>
void PrintHeader2(std::ostream& out, const size_t& Num, const string& val) {
/** ����� ������� ��������� ������ 2 � ����� os ��� ��������� ���������� ������. out - �������� �����,
Num - ������ ��������� � ��������, val - ������ ��������� **/
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / uTwo;
    field2 = (Num - len) % uTwo == sZero? field1 : field1+uOne;
    out << "**";
    for(size_t i=sZero; i<field1-sOne; i++)
        out << ch2;
    out << ' ' << val << ch1;
    for(size_t i=sZero; i<field2-sOne; i++)
        out << ch2;
    out << "**\n";
}   // PrintHeader2

template<typename T=void>
void PrintHeader(const size_t& Num, const string& val) {
/** ����� ������� ��������� ��� ��������� ���������� ������. Num - ������ ��������� � ��������,
val - ������ ��������� **/
    const char ch1 = ' ';
    const char ch2 = '*';
    const size_t sTwo = 2;
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / sTwo;
    field2 = (Num - len) % sTwo == sZero? field1 : field1+sOne;
    auto SLIne = [&Num]() {
        cout << "**";
        for(size_t i=sZero; i<Num; i++)
            cout << ch2;
        cout << "**\n";
    };
    SLIne();
    cout << "**";
    for(size_t i=sZero; i<field1; i++)
        cout << ch1;
    cout << val;
    for(size_t i=sZero; i<field2; i++)
        cout << ch1;
    cout << "**\n";
    SLIne();
};  // PrintHeader

template<typename T=void>
void PrintUnderHeader(const size_t& Num, const string& val) {
/** ����� ������� ������������ ��� ��������� ���������� ������. Num - ������ ��������� � ��������,
val - ������ ��������� **/
    const char ch1 = ' ';
    const char ch2 = '*';
    const size_t sTwo = 2;
    size_t len = val.length();
    size_t field1, field2;
    field1 = (Num - len) / sTwo;
    field2 = (Num - len) % sTwo == sZero? field1 : field1+sOne;
    cout << "**";
    for(size_t i=sZero; i<field1-sOne; i++)
        cout << ch2;
    cout << ' ' << val << ch1;
    for(size_t i=sZero; i<field2-sOne; i++)
        cout << ch2;
    cout << "**\n";
}   //

}   // nmRePrint


#endif // FROMA2_COMMON_VALUES_H_INCLUDED
