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

#include "impex_module.h"
#include <limits>

const unsigned short int mant = std::numeric_limits<double>::max_digits10;  /** ���������� ��������
������������� ����� double, ������������� ��� ������ ������ � ����� � ������ clsImpex::Import ���
������� ������ �� �������� ���� strItem (�� ��������� ����� 17),
�� https://iq.opengenus.org/print-double-with-full-precision-in-cpp **/

/*************************************************************************************************************************/
/**                                             ��������������� ������                                                  **/
/*************************************************************************************************************************/

void MeasRestore(nmBPTypes::strNameMeas Big[], nmBPTypes::strNameMeas Lit[], const size_t brows, const size_t lrows) {
/** ����� ��������� ���� � ��������� ��������� � ������� lit ������� �� ������� Big ��� ���������� ���� � ���� ��������.
���������: brows - ���������� ����� � ������� Big, lrows - ���������� ����� � ������� Lit. **/
        for(size_t i=0; i<lrows; i++)                       // ���� �� ������ ������� Lit
            for(size_t j=0; j<brows; j++) {                 // ���� �� �������� ������� Big
                if((Big+j)->name == (Lit+i)->name)          // ���� ����� ���������, ��
                    (Lit+i)->measure = (Big+j)->measure;    // ��������� ���� � ���.���������
            };
    }   // MeasRestore

/*************************************************************************************************************************/
/**                           ����� clsImpex ��� ������� ���������� �� cvs-������                                       **/
/**                 � ���������� �������� ������ ��� �������� ���� clsStorage � clsManufactory                          **/
/*************************************************************************************************************************/

clsImpex::clsImpex() {
/** ����������� �� ��������� **/
    m_rowcount = m_colcount = nmBPTypes::sZero;     // �������� ��������
    separator = ';';                                // ������������� ��������� �� ���������
}   // Default Ctor

clsImpex::clsImpex(ifstream& ifs, const char& ch) {
/** ����������� � ����������. ���������: ifs - �������� ����� �� ������, ch - ������ ����������� �����
������� � ����� **/
    m_rowcount = m_colcount = nmBPTypes::sZero;     // �������� ��������
    separator = ';';                                // ������������� ��������� �� ���������
    if(!ifs.is_open())  return;                     // ���� ���� �� ������, �� �����
    if(!Import(ifs, ch))                            // ������ ������ �� �����. ���� ��������, ��
        reset();                                    // ���������� ��������� �������
}   // Ctor with parametr

clsImpex::clsImpex(const size_t ncount, const nmBPTypes::strNameMeas names[],\
const nmBPTypes::strItem data[], const size_t dcount, nmBPTypes::ReportData flg) {
/** ����������� � ����������� ��� ������� �� ��������. ���������: const size_t ncount - ����� �����, ������ ����� ��������� ������� names[]
� �������������� �����, names[] - ������ � �������������� ����� � ��������� ���������, data[] - ���������� ������,
������ ��������� ������� �������� ncount*dcount � �������, dcount - ����� �������� �������, flg - ����, ������������
��� ������������ ������: volume, price ��� value. **/
    m_rowcount = m_colcount = nmBPTypes::sZero;     // �������� ��������
    separator = ';';                                // ������������� ��������� �� ���������
    if((ncount==nmBPTypes::sZero) || (!names) || (!data) || (dcount==nmBPTypes::sZero))
        return;                                     // ���� ������� �� ����������, �� �����
    if(!Import(ncount, names, data, dcount, flg))   // ������ ������ �� �������. ���� ��������, ��
        reset();                                    // ���������� ��������� �������
}   // Ctor with parametr

clsImpex::~clsImpex()
{
}   // Dtor

clsImpex::clsImpex(const clsImpex& other) {
/** ����������� ����������� **/
    m_rowcount = other.m_rowcount;
    m_colcount = other.m_colcount;
    separator  = other.separator;
    m_data     = other.m_data;
}   //Copy ctor

clsImpex::clsImpex(clsImpex&& other) {
/** ����������� ����������� **/
    swap(other);
}   // Move Ctor

void clsImpex::swap(clsImpex& other) noexcept {
/** ������� ������ ����������� �������� **/
    std::swap(m_rowcount, other.m_rowcount);      // ���������� ��������
    std::swap(m_colcount, other.m_colcount);
    std::swap(separator, other.separator);
    m_data.swap(other.m_data);
}   // swap

clsImpex& clsImpex::operator=(const clsImpex& rhs) {
/** �������� ������������ ������������ **/
    if (this == &rhs) return *this; // handle self assignment
    clsImpex temp(rhs);             // ��������������� ���������� ��������� ������������ �� rhs
    swap(temp);                     // ����� �� ��������������� ����������
    return *this;
}   // Copy assignment operator

clsImpex& clsImpex::operator=(clsImpex &&rhs) {
/** ���������� ��������� ������������ ������������ ������������ ������� **/
    if (this == &rhs) return *this; // handle self assignment
    swap(rhs);                      // ������������ � �������� �����������
    return *this;
}   // Move assignment operator

void clsImpex::reset() {
/** ���������� ��������� ������� �� �������� �� ���������. ����� vector::clear �� �����������
��������� ����������������� ������, ����������� ���� ����� ������������ ��� ������������ ���
(http://cplusplus.com/reference/vector/vector/clear) **/
    m_rowcount = m_colcount = nmBPTypes::sZero;         // �������� ��������,
    separator = ';';                                    // ������������� ��������� �� ���������
    vector<std::vector<std::string>>().swap(m_data);    // �������� ������ � ��������� �� ���������
}   // reset

bool clsImpex::is_Empty() const {
/** ���������� true, ���� m_data ������ **/
    if(m_data.size() == 0) return true;
    else return false;
}   // is_Empty

/** ������ ������� **/

bool clsImpex::Import(ifstream& ifs, const char& ch) {
/** ����� ����������� ������ �� �����, ���������� � ������� ifs, � �������� ����������� ������������ ������ ch **/
    if((m_rowcount>nmBPTypes::sZero) || (m_colcount>nmBPTypes::sZero) || (m_data.size()>nmBPTypes::sZero))\
        return false;                   // �������� �� ������� ������
    if(!ifs.is_open()) {                // ���� ���� �� ������, ��
        return false;                   // ������� � ���������� false
    };
    size_t tmpcount;                    // ��������������� ������� ��������� ��� �������� "���������������" ������� �������
    for (string line; getline(ifs, line);) {        // ������ � line ������ ������ �����
        if(ifs.bad()) {                             // ���� ������ �� ����� �� �������, ��
            return false;                           // � ������� � false
        };
        m_rowcount++;                               // � ����������� ������� �����
        istringstream ss(std::move(line));          // � ���������� ���������� ������ � ��������� ����� ss
        vector<std::string> row;                    // ��������������� ������ ��� ������
        if(!m_data.empty())                         // ���� �������� ������ �� ������, �� � �������������� �������
            row.reserve(m_data.front().size());     // ����������� ������ �������� � ������ �������
        tmpcount = m_colcount;          // ������������� ��������������� ������� ������ ����������� �������� m_colcount
        m_colcount = nmBPTypes::sZero;  // �������� ������� ��������
        for(string value; getline(ss, value, separator);) {     // ��������� ������ �� ��������� ������, ����������� �������� ch
            m_colcount++;                                       // ����������� ������� ��������
            row.push_back(std::move(value));                    // ��������� ������ ����� ������ �� �������������� ������
        };
        if(tmpcount != nmBPTypes::sZero)
            if(tmpcount != m_colcount) {            // ��������, ��� ���������� ����� ��������� �� ���� �������
                return false;                       // � ������� � false
            };
        m_data.push_back(std::move(row));           // ��������� �������������� ������ � ������� ��������� �������
    };
    return true;
}   // Import from file

bool clsImpex::Import(const size_t ncount, const nmBPTypes::strNameMeas names[], const nmBPTypes::strItem data[],\
const size_t dcount, nmBPTypes::ReportData flg) {
/** ����� ����������� ������ �� ��������. ���������: const size_t ncount - ����� �����, ������ ����� ��������� �������
names[] � �������������� �����, names[] - ������ � �������������� ����� � ��������� ���������, data[] - ���������� ������,
������ ��������� ������� �������� ncount*dcount � �������, dcount - ����� �������� �������, flg - ����, ������������ ���
������������ ������: volume, price ��� value. **/
    if((m_rowcount>nmBPTypes::sZero) || (m_colcount>nmBPTypes::sZero) || (m_data.size()>nmBPTypes::sZero))
        return false;                   // �������� �� ������� ������; ���� ������ ��� ����, �� ����� � false
    if((ncount==nmBPTypes::sZero) || (!names) || (!data) || (dcount==nmBPTypes::sZero))
        return false;                   // ���� ������� �� ����������, �� �����
    m_rowcount = ncount + 1;            // ���������� ����� � �������
    m_colcount = dcount + 2;            // ���������� �������� � �������
    vector<std::string> frow;           // ��������������� ������ ��� ������
    m_data.reserve(m_rowcount);                 // ����������� ������ ��������� �������
    frow.reserve(m_colcount);                   // ����������� ������ ���������������� �������
    frow.push_back(nmPrntSetup::c_TableName);   // ���������� ��������� "������������"
    frow.push_back(nmPrntSetup::c_TableMeas);   // ���������� ��������� "��.���������"
    for(size_t j{}; j<dcount; j++)
        frow.push_back(std::to_string(j));      // ���������� ������ �������
    m_data.push_back(std::move(frow));          // ��������� �������������� ������ � ������� ��������� �������
    frow.~vector();                             // ���������� ��������������� ������
    for(size_t i{}; i<ncount; i++) {            // ���� �� �������
        vector<std::string> row;                // ��������������� ������ ��� ������
        row.reserve(m_colcount);                // ����������� ������
        row.push_back((names+i)->name);         // ��������� ������������ �������
        row.push_back((names+i)->measure);      // ��������� ������� ���������
        for(size_t j{}; j<dcount; j++) {        // ���� �� ��������
            stringstream ss;                    // ��������������� �����
            if((flg==nmBPTypes::ReportData::volume) || (flg==nmBPTypes::ReportData::price)) // ���������� � ����� ������
                if(flg==nmBPTypes::ReportData::volume) ss << fixed << setprecision(mant) << (data+dcount*i+j)->volume;
                else ss << fixed << setprecision(mant) << (data+dcount*i+j)->price;
            else ss << fixed << setprecision(mant) << (data+dcount*i+j)->value;
            row.push_back(std::move(ss.str())); // ��������� � ������ �������
        };
        m_data.push_back(std::move(row));       // ��������� �������������� ������ � ������� ��������� �������
    };
    return true;
}   // Impotr from massive

/** ������ �������������� **/

void clsImpex::Transpon() {
/** ����� ���������������� ������� m_data **/
    if(m_data.size() == nmBPTypes::sZero) return;           // ���� ������ ����, �� �� ������������� ���
    vector<std::vector<std::string>> tmp;                   // ��������������� �������
    for(size_t j=nmBPTypes::sZero; j<m_colcount; j++) {     // ���� �� ��������
        vector<std::string> col;                            // ��������������� ������ ��� �������
        if(!tmp.empty())                                    // ���� �������� ������ �� ������, �� � �������������� �������
            col.reserve(tmp.front().size());                // ����������� ������ �������� � ������ �������
        for(vector<std::string> row: m_data) {                          // ���� �� �������
            vector<std::string>::const_iterator cit = row.begin()+j;    // ������������� �������� �� j-� ������� ������
            col.push_back(*cit);                            // ��������� ������� col ��� ����� ������� tmp
        }
        tmp.push_back(std::move(col));                      // � ��������� ��� � ����� �������
    };
    m_data.swap(tmp);                                       // ���������� ���������� ������� m_data � ����� ������� tmp
    std::swap(m_rowcount, m_colcount);                      // ���������� �������� � ���������
}   // Transpon

/** ������ �������� **/

void clsImpex::csvExport(ofstream& ofs) const {
/** ����� �������� ������� � csv-���� � ������������, �������� � ���������� separator **/
    for (const vector<string>& row : m_data) {
        for(const string& value: row) {
            ofs << value << separator;
        }
        ofs << endl;
    };
}   // csvExport

/** ������ Get **/

decimal* clsImpex::GetDecimal(const size_t brow, const size_t erow,\
const size_t bcol, const size_t ecol) const {
/** ����� ���������� ��������� � ������� ������, ���������������� � ��� decimal. ���������:
brow - ��������� ������ ������, erow - �������� ������, bcol - ��������� �������, ecol -
�������� �������. ����� ���������� ���������� ������������ ������, ���������� ��������
��������� ������� �������� (erow-brow+1)*(ecol-bcol+1). ��� � ������ ������ ��������� � �������,
������������ � �������� ���������� �������� ������� clsStorage � clsManufactory. ��������!!!
��� ������������� � ����������� ������� ������� clsStorage � clsManufactory ������� (� ������� m_data)
� ���������������� ������� ������ ����� �������������� ���������� (������ ������� - � ������ ��������).
� ��������� ������ ����� ����������� ���������� ������ ������� ���������� ���������������. **/
    if((m_rowcount==nmBPTypes::sZero) || (m_colcount==nmBPTypes::sZero) ||\
        (m_data.size()==nmBPTypes::sZero)) return nullptr;                          // �������� �� ������� ������
    if((brow>=m_rowcount) || (erow>=m_rowcount) || (brow>erow) ||\
        (bcol>=m_colcount) || (ecol>=m_colcount) || (bcol>ecol)) return nullptr;    // �������� ������������ ����������
    size_t trows = erow-brow+nmBPTypes::sOne;           // ���������� ����� � �������� �������
    size_t tcols = ecol-bcol+nmBPTypes::sOne;           // ���������� �������� � �������� �������
    decimal* dData = new(nothrow) decimal[trows*tcols];       // �������� ������ �������
    if(!dData) return nullptr;                          // ���� ������ �� ��������, �� ����� � ������� nullptr
    vector<vector<string>>::const_iterator citrow;      // �������� ��� �����
    vector<string>::const_iterator citcell;             // �������� ��� ��������
    size_t i=nmBPTypes::sZero;                          // ������ ������
    for(citrow=m_data.cbegin()+brow; citrow<=m_data.cbegin()+erow; citrow++) {              // ������� �� �������
        size_t j=nmBPTypes::sZero;                                                          // ������ �������
        for(citcell=(*citrow).cbegin()+bcol; citcell<=(*citrow).cbegin()+ecol; citcell++) { // ������� �� ��������
            stringstream ss;                            // ��������������� ����� �����
            ss << *citcell;                             // ������ �� ������� � ����� �����
            ss >> *(dData+tcols*i+j);                   // ������������ �� ������ � �����
            j++;                                        // �������� ������ �������
        };
        i++;                                            // �������� ������ ������
    };
    return dData;
}   // GetDecimal

nmBPTypes::strNameMeas* clsImpex::GetNames(const size_t brow, const size_t erow,\
const size_t idName, const size_t idMeas) const {
/** ����� ���������� ��������� � ������� ������, ���������������� � ��� strNameMeas. ���������:
brow - ��������� ������ ������, erow - �������� ������, idName - ����� ������� � ����������,
idMeas - ����� ������� � ��������� ���������. ����� ���������� ���������� ������������ ������ �������� (erow-brow+1).
��� � ������ ������ ��������� � �������, ������������ � �������� ���������� �������� ������� clsStorage �
clsManufactory. ��������!!! ��� ������������� � ����������� ������� ������� clsStorage � clsManufactory
������� (� ������� m_data) � ���������������� ������� ������ ����� �������������� ���������� (������
������� - � ������ ��������). � ��������� ������ ����� ����������� ���������� ������ ������� ����������
���������������. **/
    if((m_rowcount==nmBPTypes::sZero) || (m_colcount==nmBPTypes::sZero) ||\
        (m_data.size()==nmBPTypes::sZero)) return nullptr;                          // �������� �� ������� ������
    if((idName>=m_colcount) || (idMeas>=m_colcount) ||\
        (brow>=m_rowcount) || (erow>=m_rowcount) || (brow>erow)) return nullptr;    // �������� ������������ ����������
    size_t trows = erow-brow+nmBPTypes::sOne;                   // ���������� ����� � �������� �������
    nmBPTypes::strNameMeas* dData = new(nothrow) nmBPTypes::strNameMeas[trows];     // �������� ������ �������
    if(!dData) return nullptr;                                  // ���� ������ �� ��������, �� ����� � ������� nullptr
    vector<vector<string>>::const_iterator citrow;              // �������� ��� �����
    size_t i=nmBPTypes::sZero;                                  // ������ ������
    for(citrow=m_data.cbegin()+brow; citrow<=m_data.cbegin()+erow; citrow++) {
        (dData+i)->name    = *((*citrow).cbegin() + idName);    // �������� ���
        (dData+i)->measure = *((*citrow).cbegin() + idMeas);    // �������� ������� ���������
        i++;
    };
    return dData;
}   // GetNames

string* clsImpex::GetNames(const size_t brow, const size_t erow, const size_t idName) const {
/** ����� ���������� ��������� � ������� ������, ���������������� � ��� string. ���������:
brow - ��������� ������ ������, erow - �������� ������, idName - ����� ������� � ����������.
����� ���������� ���������� ������������ ������ �������� (erow-brow+1). ��������!!!
��� ������������� � ����������� ������� ������� clsStorage � clsManufactory ������� (� ������� m_data)
� ���������������� ������� ������ ����� �������������� ���������� (������ ������� - � ������ ��������).
� ��������� ������ ����� ����������� ���������� ������ ������� ���������� ���������������. **/
    if((m_rowcount==nmBPTypes::sZero) || (m_colcount==nmBPTypes::sZero) ||\
        (m_data.size()==nmBPTypes::sZero)) return nullptr;                          // �������� �� ������� ������
    if((idName>=m_colcount) || \
        (brow>=m_rowcount) || (erow>=m_rowcount) || (brow>erow)) return nullptr;    // �������� ������������ ����������
    size_t trows = erow-brow+nmBPTypes::sOne;                   // ���������� ����� � �������� �������
    string* dData = new(nothrow) string[trows];                 // �������� ������ �������
    if(!dData) return nullptr;                                  // ���� ������ �� ��������, �� ����� � ������� nullptr
    vector<vector<string>>::const_iterator citrow;              // �������� ��� �����
    size_t i=nmBPTypes::sZero;                                  // ������ ������
    for(citrow=m_data.cbegin()+brow; citrow<=m_data.cbegin()+erow; citrow++) {
        *(dData+i) = *((*citrow).cbegin() + idName);            // �������� ���
        i++;
    };
    return dData;
}   // GetNames

nmBPTypes::strItem* clsImpex::GetstrItem(const size_t brow, const size_t erow, const size_t bcol,\
const size_t ecol, nmBPTypes::ReportData flg) const {
/** ����� ���������� ��������� � ������� ������, ���������������� � ����������� ��� strItem. ���������:
brow - ��������� ������ ������, erow - �������� ������, bcol - ��������� �������, ecol - ��������
�������, flg - ����, ������������ ������������ � ��������� ����: volume, price ��� value. ������������
���� ����������� ������� �� �������, �������������� - ������. ����� ���������� ���������� ������������
������, ���������� �������� ��������� ������� �������� (erow-brow+1)*(ecol-bcol+1). ��� � ������ ������
��������� � �������, ������������ � �������� ���������� �������� ������� clsStorage � clsManufactory.
��������!!! ��� ������������� � ����������� ������� ������� clsStorage � clsManufactory ������� (� �������
m_data) � ���������������� ������� ������ ����� �������������� ���������� (������ ������� - � ������
��������). � ��������� ������ ����� ����������� ���������� ������ ������� ���������� ���������������. **/
    if((m_rowcount==nmBPTypes::sZero) || (m_colcount==nmBPTypes::sZero) ||\
        (m_data.size()==nmBPTypes::sZero)) return nullptr;                          // �������� �� ������� ������
    if((brow>=m_rowcount) || (erow>=m_rowcount) || (brow>erow) ||\
        (bcol>=m_colcount) || (ecol>=m_colcount) || (bcol>ecol)) return nullptr;    // �������� ������������ ����������
    size_t trows = erow-brow+nmBPTypes::sOne;           // ���������� ����� � �������� �������
    size_t tcols = ecol-bcol+nmBPTypes::sOne;           // ���������� �������� � �������� �������
    nmBPTypes::strItem* dData = new(nothrow) nmBPTypes::strItem[trows*tcols];       // �������� ������ �������
    if(!dData) return nullptr;                          // ���� ������ �� ��������, �� ����� � ������� nullptr
    vector<vector<string>>::const_iterator citrow;      // �������� ��� �����
    vector<string>::const_iterator citcell;             // �������� ��� ��������
    size_t i=nmBPTypes::sZero;                          // ������ ������
    for(citrow=m_data.cbegin()+brow; citrow<=m_data.cbegin()+erow; citrow++) {              // ������� �� �������
        size_t j=nmBPTypes::sZero;                                                          // ������ �������
        for(citcell=(*citrow).cbegin()+bcol; citcell<=(*citrow).cbegin()+ecol; citcell++) { // ������� �� ��������
            stringstream ss;                                            // ��������������� ����� �����
            ss << *citcell;                                             // ������ �� ������� � ����� �����
            if(flg==nmBPTypes::volume) {                                // ���� ������ ���� volume
                ss >> (dData+tcols*i+j)->volume;                        // ������������ �� ������ � ����� � ���������� � ���� volume
                (dData+tcols*i+j)->price = nmBPTypes::dZero;            // ��������� ���� ��������� ������
                (dData+tcols*i+j)->value = nmBPTypes::dZero;
            } else                                                      // �����
                if(flg==nmBPTypes::price) {                             // ���� ������ ���� price
                    (dData+tcols*i+j)->volume = nmBPTypes::dZero;
                    ss >> (dData+tcols*i+j)->price;                     // ������������ �� ������ � ����� � ���������� � ���� price
                    (dData+tcols*i+j)->value = nmBPTypes::dZero;
                } else {
                        (dData+tcols*i+j)->volume = nmBPTypes::dZero;
                        (dData+tcols*i+j)->price  = nmBPTypes::dZero;
                        ss >> (dData+tcols*i+j)->value;
                    };
            j++;                                        // �������� ������ �������
        };
        i++;                                            // �������� ������ ������
    };
    return dData;
}   // GetstrItem

size_t clsImpex::GetRowCount() const { return m_rowcount; }
size_t clsImpex::GetColCount() const { return m_colcount; }

/** ������ ����������� �������� **/

void clsImpex::View(ostream& os) const {
/** ����� ����������� �������� ��������� �� ����� ���������� **/
    os << "Rows " << m_rowcount << "; columns " << m_colcount << endl;
    for (const vector<string>& row : m_data) {
        for(const string& value: row) {
            os << setw(15) << value.substr(0,15) << " ";
        }
        os << endl;
    };
}   // View
