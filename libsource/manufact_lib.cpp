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

#include "manufact_module.h"

//#define CDtor_voice       // ������ ������ ���������� ����������. ����������������� ��� �������
//#define DEBUG_THREADS     // ������ ������ ���������� ����������. ����������������� ��� �������

std::mutex mu1;             // ������ �� �������������� ������� ������� � ������ �������

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                             ��������������� �������                                                 **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

inline void Sum(decimal arr1[], const decimal arr2[], size_t N) {
/** ����� ��������� ����������� ��� �������, ��������� ��������� � ������ �������. ���������: arr1 - ������ ������,
arr2 - ������ ������, N - ����������� ����� ��������. ��� ������� ������ ����� ���������� ������ N. ����� inline ���
����������� ��� ���� � ����� ������. ��������!!! ����� �� ��������� ���������� ����������� �������� **/
    if(arr1 && arr2)                                // ���� ��� ������� ����������, ��
        for(size_t i=sZero; i< N; i++) {            // ���������� �������� ������������
            *(arr1+i) += *(arr2+i);
        }
}   // Sum

inline void Dif(decimal arr1[], const decimal arr2[], size_t N) {
/** ����� �������� ����������� ��� �������, ��������� ��������� � ������ �������. ���������: arr1 - ������ ������,
arr2 - ������ ������, N - ����������� ����� ��������. ��� ������� ������ ����� ���������� ������ N. ����� inline ���
����������� ��� ���� � ����� ������. ��������!!! ����� �� ��������� ���������� ����������� �������� **/
    if(arr1 && arr2)                                // ���� ��� ������� ����������, ��
        for(size_t i=sZero; i< N; i++) {            // ���������� �������� ������������
            *(arr1+i) -= *(arr2+i);
        }
}   // Dif

inline decimal *Mult(const decimal arr1[], const decimal arr2[], const size_t N) {
/** ����� ���������� ��������� ��������� ��������� ���� �������� � ����������� ��������� ���� �� �����.
��������� ������������ � ���� ��������� �� ����� ������. ���������: arr1 - ������ ������, arr2 - ������ ������,
N - ����������� ����� ��������. ��� ������� ������ ����� ���������� ������ N. ��������!!! ����� �� ���������
���������� ����������� �������� � �� ��������� ������� �� nullptr **/
    if(arr1 && arr2) {                              // ���� ������� ����������
        decimal *temp = new(nothrow) decimal[N];    // �������� ������ ��� ��������������� ������ temp
        if(!temp) { return nullptr; }               // ���� ������ �� ��������, �� ����� � ������� nullptr
        for(size_t i=sZero; i< N; i++) {            // ���� ������ ��������, �� �������� �������� �������
            *(temp+i) = (*(arr1+i)) * (*(arr2+i));
        };
        return temp;
        } else return nullptr;
}   // Mult

inline void v_service(const strNameMeas* arr, size_t _rcount) {
/** ��������� �������, �������������� � ������� ViewRawNames � ViewRefRawNames **/
    clsTextField name(15);              // ��������� ������� ������ ���� � ������� ������ clsTextField
    clsTextField meas(12);              // ��������� ������� ������ ������ ���������
    cout << name << strName << meas << strMeas << endl;
    for(size_t i=sZero; i<_rcount; i++) {
        cout << name << arr->name << meas << arr->measure << endl;
    };
    cout << endl;
}   // v_service

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                             class clsRecipeItem                                                     **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

/** ����� - ��������� ���������� ��������. ��������� ������� �������� ������������ ����� �������: ������ ������� - ��� �����,
�������������� � ������������ ��������, ������� - ������� ����������������� �����; �� ����������� ����� � �������� �����������
�������� ������ ����� �� ������������ ������� ������� �������� � ����������� ��������� � ��������� ������ �����������������
�����. ������ � ����� ������� ������� ������������� ������� ������ �������� ��������. **/

        inline decimal* clsRecipeItem::CalcRawMatVolumeItem(const size_t PrCount, const size_t period, const decimal volume) const {
        /** ����� ������������ ����� ����������� �������� � ������ ������� ���������������� ����� � ����������� ��
        ���������� ������ �������� �������� �� ������ ����� �����. ����� ������������ ����������� � �������� ������ ���
        ������ ���������� ����������������� ����� (���). ��� - ��� ������� ������������ ������ ������������ �������� �
        �����-���� ��������� ������� �������. ���������:  PrCount - ����� �������� �������, period - ����� ������� �������,
        � ������� ��������� ������� ������������� �������, volume - ��������� ����� ����� ��������. ����� ����������
        ��������� �� ���������� ������, ������ ��������� �������, �������� rcount*PrCount � ������������� ��������
        ��������� � ������������ ��������: ����� ����� ������� ��������� � ������ ������� ��������, ����� �������� - �
        ������������� �������. ������� ������ ������� �� ������������� (������������� ���������� �������).
        ��������!!! ��� ������ ������� ������ ������� ������� � ������������� ������������� ������ � ��������
        ���������� ������ ������� ������������� ������� **/
            if(period < duration) { return nullptr; };      // ���� ������ ������, ��� ���������������� ����, ������� "�����"
            if(period >= PrCount) { return nullptr; };      // ���� ������ ������ ��� ����� ������������ �������, ������� "�����"
            if(rcount == sZero) { return nullptr; };        // ���� ����� �������������� ������� ����� �������, �� "�����"
            if(!rnames) { return nullptr; };                // ���� ����������� ������ � ������������� �����, �� "�����"
            if(!recipeitem) { return nullptr; };            // ���� ����������� ������� � �����������, �� "�����"
            const size_t tcount = rcount * PrCount;         // ������� ������������ �������
            decimal *temp = new(nothrow) decimal[tcount]{}; // �������� ������ ��� ������� � ���������� ���������� � nothrow:
            // ���� ������ �� ��������, �� ������� nullptr (https://cplusplus.com/reference/new/nothrow/)
            if(!temp) { return nullptr; };                  // ���� ������ �� ��������, �� ����� � ������� "�����"
//            for(size_t i=sZero; i<tcount; i++) {            // ��������� ������ ��������������� ������
//                *(temp+i) = dZero;
//            };
            const size_t diff = period - duration;          // ������ ������� ����� (������ ����������� ����� � ������������)
            for(size_t i=sZero; i<rcount; i++) {            // ���� �� ������������ �����
                for(size_t j=period, k=duration; (j>diff)&&(k>sZero); j--, k--) {   // ���� �� �������� ������ ����������������� �����
                    size_t n = k-sOne; // ��������� ���������� ��� �������������� ������������, ����� �� ����� ������� ������������� �����
                    *(temp + PrCount*i+j) = *(recipeitem + duration*i+n) * volume;  // ������ ������ �����
                }   // ���� �� �������� ������ ����������������� �����
            }       // ���� �� ������������ �����
            return temp;
        }   // CalcRawMatVolumeItem

        inline decimal* clsRecipeItem::CalcWorkingVolumeItem(const size_t PrCount, const size_t period, const decimal volume) const {
        /** ����� ������������ ������ �������������� ������������ ��� ��������, ������������ � ���������� ������ �������.
        ���������:  PrCount - ����� �������� �������, period - ����� ������� �������, � ������� ��������� ������� �������������
        �������, volume - ��������� ����� ����� ��������. ����� ���������� ��������� �� ���������� ������ ������������ �������
        �������������� ������������ � ����������� ��������� �������� PrCount. ��������!!! ��� ������ ������� ������ �������
        ������� � ������������� ������������� ������ � �������� ���������� ������ ������� ������������� �������. **/
            if(period < duration) { return nullptr; };      // ���� ������ ������, ��� ���������������� ����, ������� "�����"
            if(period >= PrCount) { return nullptr; };      // ���� ������ ������ ��� ����� ������������ �������, ������� "�����"
            decimal *temp = new(nothrow) decimal[PrCount]{};// �������� ������ �������
            if(!temp) { return nullptr; };                  // ���� ������ �� ��������, �� ����� � ������� "�����"
            const size_t diff = period - duration;          // ������ ������� ����� (������ ����������� ����� � ������������)
            const size_t frm = period-sOne;                 // ������� ������� ����� (������������ ���������� ��������)
            for(size_t j=frm; j>diff; j--) {
                *(temp+j) = volume;                         // ��������� ������ ���������� ������� �������������� ������������
            }
            return temp;
        }   // CalcWorkingVolumeItem

        decimal* clsRecipeItem::CalcWorkingVolume(const size_t PrCount, const strItem volume[]) const {
        /** ����� ������������ ������ �������������� ������������ ��� ����������� �������� � ����������� ���������,
        ������������ �� ���������� ����� �������. ���������:  PrCount - ����� �������� �������, volume - ���������
        �� ������ ����������� PrCount � �������� ������������ �������� �� �������� (���� ������� ��������). �����
        ���������� ��������� �� ���������� ������ ������������ ������� �������������� ������������ � �����������
        ��������� �������� PrCount. ��������!!! ��� ������ ������� ������ ������� ������� � ������������� �������������
        ������ � �������� ���������� ������ ������� ������������� �������.**/
            decimal *temp = new(nothrow) decimal[PrCount]{};// �������� ������ ��� ��������������� ����������
            if(!temp) { return nullptr; };                  // ���� ������ �� ��������, �� ���������� "�����"
//            for(size_t i=sZero; i<PrCount; i++) {           // ��������� ������ ������
//                *(temp+i) = dZero;
//            };
            decimal *Voltmp = nullptr;
            for(size_t i=duration; i<PrCount; i++) {
                Voltmp = CalcWorkingVolumeItem(PrCount, i, (volume+i)->volume);
                if(Voltmp) {
                    Sum(temp, Voltmp, PrCount);
                    delete[] Voltmp;                        // ������� ��������������� ������
                    Voltmp = nullptr;
                };
            };
            return temp;
        }   // CalcWorkingVolume

        decimal* clsRecipeItem::CalcWorkingValueItem(const size_t _PrCount, const size_t period, \
            const decimal rmprice[], const decimal volume) const {
        /** ����� ���������� ���������� ������ �������� ������������� �������� � ������� ���������� ���������������� �����.
        ������� ����������� � �������� ������ period. ����������� ������������� ������� _PrCount. ���������: _PrCount
        - ������������ �������, period - ������ ������� ��������, rmprice[] - 2D-������ � ������ ����� ������������
        rcount*PrCount, volume - ����� ������������ �������� � ������ period. ����� ���������� ���������� ������ ������������
        _PrCount. ��������!!! ��� ������ ������� ������ ������� ������� � ������������� ������������� ������ � ��������
        ���������� ������ ������� ������������� �������. **/
            if(period < duration) { return nullptr; };      // ���� ������ ������, ��� ���������������� ����, ������� "�����"
            if(period >=_PrCount) return nullptr;           // ���� ������ ������ ��� ����� ������������ �������, ������� "�����"
            if(!rmprice) { return nullptr; };               // �������� ������������� �������
            decimal *tmp = CalcRawMatVolumeItem(_PrCount, period, volume); // ������ �������� rcount*PrCount � �������� �����
            if(!tmp) { return nullptr; };                           // ���� ������ ����, � ����� � ������� nullptr
            decimal *temp = Mult(tmp, rmprice, rcount*_PrCount);    // �������� 2D-������ �� ���������� �����
            delete[] tmp;                                           // ������� ������
            tmp = new(nothrow) decimal[_PrCount]{};                 // �������� ������ ��� ������ �������
            if(!tmp) { delete[] temp; return nullptr; }             // ���� �� ����������, �� ������� ������ temp � �������
//            for(size_t i=sZero; i<_PrCount; i++) {                  // �������� ������
//                *(tmp+i) = dZero;
//            };
            for(size_t i=sZero; i<rcount; i++) {                    // ��������� ������ ������� temp � ���������� � ������ tmp
                for(size_t j=sZero; j<_PrCount; j++) {
                    *(tmp+j) += *(temp+_PrCount*i+j);
                };
            };
            if(temp) delete[] temp;                                 // ������� ������ temp
            return tmp;
        }   // CalcWorkingValueItem

        decimal* clsRecipeItem::CalcWorkingValue(const size_t _PrCount, const size_t period, const \
            decimal rmprice[], const decimal volume) const {
        /** ����� ���������� ���������� ������ ���������������� ������������� �������� � ������� ���������� ����������������
        �����. ������� ����������� � �������� ������ period. ����������� ������������� ������� _PrCount. ���������: _PrCount
        - ������������ �������, period - ������ ������� ��������, rmprice[] - 2D-������ � ������ ����� ������������
        rcount*PrCount, volume - ����� ������������ ��������� ������ period. ��������!!! ��� ������ ������� ������ �������
        ������� � ������������� ������������� ������ � �������� ���������� ������ ������� ������������� �������. **/
            if(period < duration) { return nullptr; };      // ���� ������ ������, ��� ���������������� ����, ������� "�����"
            if(period >=_PrCount) return nullptr;           // �������� ������������ �������
            if(!rmprice) { return nullptr; };               // �������� ������������� ������� � ������ �� �����
            decimal *tmp = CalcWorkingValueItem(_PrCount, period, rmprice, volume); // �������� ������ � ��������������
            if(!tmp) { return nullptr; };                   // ���� ������ ����, �� ����� � ������� nullptr
            decimal S = dZero;                              // ��������������� ���������� ��� ���������������
            for(size_t i=sZero; i<_PrCount; i++) {          // ������������ ������ tmp
                S += *(tmp+i);
                *(tmp+i) = S;
            };
            return tmp;
        }   // CalcWorkingValue

        decimal* clsRecipeItem::CalcWorkingBalanceItem(const size_t _PrCount, const size_t period, \
            const decimal rmprice[], const decimal volume) const {
        /** ����� ���������� ���������� ������ ���������������� ������������� �������������� ������������ � ������� ����������
        ���������������� �����. ������� ����������� � �������� ������ period. ����������� ������������� ������� _PrCount.
        ���������: _PrCount - ������������ �������, period - ������ ������� ��������, rmprice[] - 2D-������ � ������ �����
        ������������ rcount*PrCount, volume - ����� ������������ �������� � ������ period. ��������!!! ��� ������ ������� ������
        ������� ������� � ������������� ������������� ������ � �������� ���������� ������ ������� ������������� �������.**/
            if(period < duration) { return nullptr; };      // ���� ������ ������, ��� ���������������� ����, ������� "�����"
            if(period >=_PrCount) return nullptr;           // �������� ������������ �������
            if(!rmprice) { return nullptr; };               // �������� ������������� �������
            decimal *tmp = CalcWorkingValue(_PrCount, period, rmprice, volume); // �������� ������ � ���.�������������� ��������
            if(!tmp) { return nullptr; };                                       // ���� ������ ����, �� ����� � ������� nullptr
            decimal *wptemp=CalcWorkingVolumeItem(_PrCount, period, volume);    // �������� ������ ��������.������������
            if(!wptemp) {                                                       // ���� ������ �� �������, ��
                delete[] tmp;                                                   // ������� ������ tmp
                return nullptr;                                                 // ������� � ���������� Nullptr
            };
            for(size_t i=sZero; i<_PrCount; i++ ) {
                if(*(wptemp+i) < epsln) {*(tmp+i) = dZero; };   // ���� i-� ������� ������� wptemp �������, �� �
            };                                                  // i-� ������� ������� tmp ���� ������ �������
            delete[] wptemp;
            return tmp;
        }   // CalcWorkingBalanceItem

        decimal* clsRecipeItem::CalcProductBalanceItem(const size_t _PrCount, const size_t period, const \
            decimal rmprice[], const decimal volume) const {
        /** ����� ���������� ���������� ������ ������������� �������� ��������, ����������� � ������ period. �����������
        ������������� ������� _PrCount. ���������: _PrCount - ������������ �������, period - ������ ������� ��������, rmprice[]
        - 2D-������ � ������ ����� ������������ rcount*PrCount, volume - ����� ������������ �������� � ������ period. ��������!!!
        ��� ������ ������� ������ ������� ������� � ������������� ������������� ������ � �������� ���������� ������ �������
        ������������� �������.**/
            if(period < duration) { return nullptr; };      // ���� ������ ������, ��� ���������������� ����, ������� "�����"
            if(period >=_PrCount) return nullptr;           // �������� ������������ �������
            if(!rmprice) { return nullptr; };               // �������� ������������� �������
            decimal *tmp = CalcWorkingValue(_PrCount, period, rmprice, volume); // �������� ������ � ���.��������������
            if(!tmp) { return nullptr; };                                       // ���� ������ ����, �� ����� � ������� nullptr
            for(size_t i=sZero; i<_PrCount; i++ ) {
                if(i != period) *(tmp+i) = dZero;           // ��������, ������ ������� ������� �� period ����������
            };
            return tmp;
        }   // CalcProductBalanceItem

        inline void clsRecipeItem::clsEraser() {
        /** ����� "��������" ��� ���� ���������� ������. ������������ ������ � ������������ �����������**/
            name = measure = "";
            duration = rcount = sZero;
            rnames = nullptr;
            recipeitem = nullptr;

        }   // clsEraser

        void clsRecipeItem::View_f_Item(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[], const string&\
        _hmcur, decimal* (clsRecipeItem::*f)(const size_t, const size_t, const decimal*, const decimal) const) const {
        /** ��������� ������� ��� ���������� ����������� ��������. ��������� ����������� �������� �����������������
        �������: CalcWorkingValueItem, CalcWorkingValue, CalcWorkingBalanceItem � CalcProductBalanceItem. ������������
        ���������:  ViewWorkingValueItem, ViewWorkingValue, ViewWorkingBalanceItem � ViewProductBalanceItem. ���������:
        PrCount - ����� �������� �������, ProPlan[] - ������ c ������ ������ ������� ��������� ������������ PrCount,
        rmprice[]  - 2D-������ � ������ ����� ������������ rcount*PrCount, ��� rcount - ���������� ������� �����;
        decimal* (clsRecipeItem::*f)(const size_t, const size_t, const decimal*, const decimal) const - ��������� ��
        ���� �� ������� CalcWorkingValueItem, CalcWorkingValue, CalcWorkingBalanceItem ��� CalcProductBalanceItem. **/
            strNameMeas pname[PrCount];                             // ��������� ������ ��������� ��� ������� ArrPrint
            for(size_t i=sZero; i<PrCount; i++) {
                pname[i].name = to_string(i);                       // ��������� ����� ��������� � �������� �������� �������
                pname[i].measure = _hmcur;                           // �������� ������ ��������� (������)
            };
            decimal *tdata = new(nothrow) decimal[PrCount*PrCount]{};   // �������� ������ ��� ��������� ���������� � �������� ������
            if(!tdata) return;                                          // ���� ������ �� ��������, �� ����� � ������� nullptr
            decimal *temp;                                                      // ������ ��������� ����������
            for(size_t i=duration; i<PrCount; i++) {
                temp = ((*this).*f)(PrCount, i, rmprice, (ProPlan+i)->volume);  // �������� ������ �������
                if(temp)
                    var_cpy((tdata+PrCount*i), temp, PrCount);      // �������� �� ��������� ����������
                if(temp) delete[] temp;                             // ������� �������� �����������
            };
            ArrPrint(PrCount, pname, tdata, PrCount);               // ������� �� �����
            if(tdata) delete[] tdata;                               // ������� ��������� ����������
        }   // View_f_Item

        void clsRecipeItem::View_f_Balance(const size_t _PrCount, const strItem ProPlan[], const decimal rmprice[],
        const string& _hmcur, strItem* (clsRecipeItem::*f)(const size_t, const decimal*, const strItem*) const) const {
        /** ��������� ������� ��� ���������� ����������� ��������. ��������� ����������� �������� �����������������
        �������: CalcWorkingBalance � CalcProductBalance. ������������ ���������: ViewWorkingBalance � ViewProductBalance.
        ���������: PrCount - ����� �������� �������, ProPlan[] - ������ c ������ ������ ������� ��������� ������������
        PrCount, rmprice[]  - 2D-������ � ������ ����� ������������ rcount*PrCount, ��� rcount - ���������� ������� �����;
        strItem* (clsRecipeItem::*f)(const size_t, const decimal*, const strItem*) const - ��������� �� ���� �� �������
        CalcWorkingBalance ��� CalcProductBalance. **/
            strNameMeas pname[sOne] = {name, measure};                  // ��������� ������� ��� ������ � ���. ���������
//            strNameMeas pnamepr[sOne] = {name, _hmcur+'/'+measure};     // ��������� ������� ��� ������ �������� ���������
//            strNameMeas pnameva[sOne] = {name, _hmcur};                 // ��������� ������� ��� ������ ������ ���������
            strItem *temp = ((*this).*f)(_PrCount, rmprice, ProPlan);   // �������� ������ ������� � �������� ��������� �� ������
            if(!temp) return;                                           // ���� ������ �� ����������, �� �����
            ArrPrint(sOne, pname, temp, _PrCount, volume, _hmcur);      // ������� �� ����� � ����������� ���������
            ArrPrint(sOne, pname, temp, _PrCount, price, _hmcur);       // ������� �� ����� ����
            ArrPrint(sOne, pname, temp, _PrCount, value, _hmcur);       // ������� �� ����� ���������
            delete[] temp;                                              // ������� ��������� ����������
        }   // ViewWorkingBalance

        /** public **/

        clsRecipeItem::clsRecipeItem(): name(EmpStr), measure(EmpStr), duration(sZero), rcount(sZero) {
        /** ����������� ��� ����������. ������ ��� �������� ���������� ������ ��� ���������� �������������� ������ �� �����
        � ���� ���������, ��� ����������� ��� ����������� � ��������� �� ������� �������. **/
            rnames = nullptr;
            recipeitem = nullptr;
            #ifdef CDtor_voice      // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                cout << "Empty Ctor clsRecipeItem" << endl;
            #endif // CDtor_voice
        }   // Empty Ctor clsRecipeItem

        clsRecipeItem::clsRecipeItem(const string &_name, const string &_measure, const size_t _duration, const size_t _rcount,\
            const strNameMeas _rnames[], const decimal _recipeitem[])
        /** ����������� � ������ ���� ����������. ���������: &_name - ������ �� �������� ��������, &_measure - ������ ��
        ������������ ������� ��������� ������������ ������ ��������, _duration - ������������ ����������������� �����,
        _rcount - ���������� ������� ��������, _rnames[] - ��������� �� ������ � �������������� �������� � ��������� ���������
        �������� _rcount, _recipeitem[] - ��������� �� ������ ���������� ������ ������� � ����������� ������������
        _rcount*_duration. **/
            : name(_name), measure(_measure), duration(_duration), rcount(_rcount) {
            if( (rcount>sZero) && (_rnames) ) {         // ���� ���������� ������� ����� ������ ���� � ���������� ������ ��������
                rnames = new(nothrow) strNameMeas[rcount];  // ����� �� ������, �� ������ ������ �������� ����� �������� rcount,
                if(rnames)                              // ���� ������ ������� ��������, ��
                    for(size_t i=sZero; i<rcount; i++) {// �������� � ��������� ������ ������������ ����� �� �������� �������:
                        (rnames+i)->name = (_rnames+i)->name;       // ������������ �����
                        (rnames+i)->measure = (_rnames+i)->measure; // ������� ���������
                    };
            } else rnames = nullptr;                    // ���� ����������� �� rcount � _rnames �� �����������, �� "�����"
            if( (duration>sZero) && (_recipeitem) ) {   // ���� ������������ ����������������� ����� ������ ���� � ���������� ������ ��������
                recipeitem = new(nothrow) decimal[rcount*duration];  // �� ������, �� ������� ������ �������� �������� rcount*duration
                if(recipeitem)                          // ���� ������ ������� ��������, �� �������� �� �������� �������
//                    memcpy(recipeitem, _recipeitem, sizeof(decimal)*rcount*duration);//  ��������� �������� memcpy
                    var_cpy(recipeitem, _recipeitem, rcount*duration);  // ���������
            } else recipeitem = nullptr;                // ���� ����������� �� duration � _recipeitem �� �����������, �� "�����"
            #ifdef CDtor_voice                          // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                cout << "Ctor clsRecipeItem" << endl;
            #endif                                      // CDtor_voice
        }   // Ctor clsRecipeItem

        clsRecipeItem::clsRecipeItem(const clsRecipeItem &obj): name(obj.name), measure(obj.measure), \
            duration(obj.duration), rcount(obj.rcount) {
        /** ����������� ����������� **/
            if( (rcount>sZero) && (obj.rnames) ) {
                rnames = new(nothrow) strNameMeas[rcount];  // �������� ������ ������� rnames
                if(rnames)                                  // ���� ������ ��������, ��
                    for(size_t i=sZero; i<rcount; i++) {    // �������� � ��������� ������ ������������ ����� �� �������
                        (rnames+i)->name = (obj.rnames+i)->name;        // �������� �������,
                        (rnames+i)->measure = (obj.rnames+i)->measure;  // � ������� ��������
                    };
            } else rnames = nullptr;                    // ���� ����������� �� rcount � _rnames �� �����������, �� "�����"
            if( (duration>sZero) && (obj.recipeitem) ) {// ���� ������������ ����������������� ����� ������ ���� � ���������� ������ �������� �� ������, ��
                recipeitem = new(nothrow) decimal[rcount*duration];   // ������ ������ �������� �������� rcount*duration
                if(recipeitem)                          // ���� ������ ��������, ��
//                    memcpy(recipeitem, obj.recipeitem, sizeof(decimal)*rcount*duration); // �������� �� �������� ������� �������� �������� memcpy
                    var_cpy(recipeitem, obj.recipeitem, rcount*duration);
            } else recipeitem = nullptr;                // ���� ����������� �� duration � _recipeitem �� �����������, �� "�����"
            #ifdef CDtor_voice                          // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                cout << "Copy Ctor clsRecipeItem" << endl;
            #endif                                      // CDtor_voice
        }   // Copy Ctor clsRecipeItem

        void clsRecipeItem::swap(clsRecipeItem& obj) noexcept {
        /** ������� ������ ���������� ����� ���������. ������� ��������� noexcept - �� ���������� ���������� **/
            std::swap(name, obj.name);
            std::swap(measure, obj.measure);
            std::swap(duration, obj.duration);
            std::swap(rcount, obj.rcount);
            std::swap(rnames, obj.rnames);
            std::swap(recipeitem, obj.recipeitem);
            #ifdef CDtor_voice                          // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                cout << "clsRecipeItem::swap" << endl;
            #endif
        }   // swap

        clsRecipeItem::clsRecipeItem(clsRecipeItem &&obj) {
        /** ����������� ����������� **/
            clsEraser();        // �������� ���� ���������� ������
            swap(obj);          // ������������ ����������� � obj
            #ifdef CDtor_voice                          // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                cout << "Move Ctor clsRecipeItem" << endl;
            #endif                                      // CDtor_voice
        }   // Move Ctor clsRecipeItem

        clsRecipeItem& clsRecipeItem::operator= (const clsRecipeItem &obj) {
        /** ���������� ��������� ������������ ������������. ����������� � ������
        �����������-�-������ (copy-and-swap idiom)  **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsRecipeItem tmp(obj);             // �������� ����������� ����������� � �������� ����� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef CDtor_voice                  // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                cout << "clsRecipeItem Copy &operator=" << endl;
            #endif                              // CDtor_voice
            return *this;
        }   // clsRecipeItem Copy &operator=

        clsRecipeItem& clsRecipeItem::operator= (clsRecipeItem &&obj) {
        /** ���������� ��������� ������������ ������������. ����������� � ������
        �����������-�-������ (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsRecipeItem tmp(move(obj));       // �������� ����������� ����������� � �������� ���������� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef CDtor_voice                  // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                cout << "clsRecipeItem Move &operator=" << endl;
            #endif                              // CDtor_voice
            return *this;
        }   // clsRecipeItem Move &operator=

        clsRecipeItem::~clsRecipeItem() {
        /** ���������� **/
            if(rnames) delete[] rnames;
            if(recipeitem) delete[] recipeitem;
            #ifdef CDtor_voice                          // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                cout << "Dtor clsRecipeItem" << endl;
            #endif                                      // CDtor_voice
        }   // Dtor clsRecipeItem

        /** Get - ������ **/

        strItem* clsRecipeItem::CalcWorkingBalance(const size_t _PrCount, const decimal rmprice[], const strItem ProPlan[]) const {
        /** ����� ������������ �����, �������� � ������ ������������� �������������� ������������ ��� ����������� ��������,
        ������������ �� ���������� ����� �������. ���������:  _PrCount - ����� �������� �������, ProPlan - ��������� �� ������
        ����������� PrCount � �������� ������������ �������� �� �������� (���� ������� ��������), rmprice ��������� �� �������
        � ������ �� ����� � ���������, ������������ rcount*PrCount. ��������!!! ��� ������ ������� ������ ������� ������� �
        ������������� ������������� ������ � �������� ���������� ������ ������� ������������� �������. **/
            if(!rmprice) { return nullptr; };                           // �������� ������������� �������
            if(!ProPlan) { return nullptr; };                           // �������� ������������� �������
            /** ������������ ������������� ������������ � ����������� ��������� **/
            decimal *tvolume = CalcWorkingVolume(_PrCount, ProPlan);    // �������� ������ �������������� ��-�� � ���. ���������
            if(!tvolume) {return nullptr;};                             // ���� ������ �� ��������, �� ����� � nullptr
            /** ������������ ������������� ������������ � ����������� ��������� **/
            decimal *tvalue = new(nothrow) decimal[_PrCount]{};         // �������� ������ ��� ��������� ������ � ��������� ������
            if(!tvalue) {                                               // ���� ������ �� ��������, ��
                delete[] tvolume;                                       // ������� ����� ��������� ������ tvolume
                return nullptr;                                         // ������� � ���������� nullptr
            };
            decimal *temp;                                              // ������ ��������� ����������
            for(size_t i=duration; i<_PrCount; i++) {
                temp = CalcWorkingBalanceItem(_PrCount, i, rmprice, (ProPlan+i)->volume); // �������� ������������� ��� ���
                if(temp) {                                              // ���� ������ �� ������,��
                    Sum(tvalue, temp, _PrCount);                        // ��������� ��� ��� � ������ tvalue
                    delete[] temp;                                      // ������� �������� ������������
                };
            };
            /** ��������� �������� ������ **/
            strItem *twbalance = new(nothrow) strItem[_PrCount];        // �������� ������ ��� �������� ������
            if(!twbalance) {                                            // ���� ������ �� ��������, ��
                delete[] tvolume;                                       // ������� ����� ��������� ������ tvolume
                delete[] tvalue;                                        // ������� ����� ��������� ������ tvalue
                return nullptr;                                         // ������� � ���������� nullptr
            };
            for(size_t i=sZero; i<_PrCount; i++) {                      // �������� �������� �������� � �������� ������
                (twbalance+i)->value  = *(tvalue +i);
                (twbalance+i)->volume = (twbalance+i)->value > epsln ? *(tvolume+i) : dZero;
                (twbalance+i)->price = (twbalance+i)->volume > epsln ? (twbalance+i)->value / (twbalance+i)->volume : dZero;
            };
            delete[] tvolume;                                           // ������� ����� ��������� ������ tvolume
            delete[] tvalue;                                            // ��������� ��� ��� � ������ tvalue
            return twbalance;
        }   // CalcWorkingBalance

        strItem* clsRecipeItem::CalcProductBalance(const size_t _PrCount, const decimal rmprice[], const strItem ProPlan[]) const {
        /** ����� ������������ �����, �������� � ������ ������������� �������� ��������. ���������:  _PrCount - ����� ��������
        �������, ProPlan - ��������� �� ������ ����������� PrCount � �������� ������������ �������� �� �������� (���� �������
        ��������), rmprice ������� � ������ �� ����� � ���������, ������������ rcount*PrCount. ��������!!! ��� ������ �������
        ������ ������� ������� � ������������� ������������� ������ � �������� ���������� ������ ������� �������������
        �������. **/
            if(!rmprice) { return nullptr; };                           // �������� ������������� �������
            if(!ProPlan) { return nullptr; };                           // �������� ������������� �������
            /** ������������ ������� ��������� � ����������� ��������� **/
            decimal *tvalue = new(nothrow) decimal[_PrCount];           // �������� ������ ��� ��������� ������
            if(!tvalue) {                                               // ���� ������ �� ��������, ��
                return nullptr;                                         // ������� � ���������� nullptr
            };
            for(size_t i=sZero; i<_PrCount; i++) {                      // ��������� �������
                *(tvalue+i) = dZero;
            };
            decimal *temp;                                              // ������ ��������� ����������
            for(size_t i=duration; i<_PrCount; i++) {
                temp = CalcProductBalanceItem(_PrCount, i, rmprice, (ProPlan+i)->volume); // �������� ������������� ��� ���
                if(temp) {                                                      // ���� ������ �� ������,��
                    Sum(tvalue, temp, _PrCount);                                // ��������� ��� ��� � ������ tvalue
                    delete[] temp;                                              // ������� �������� ������������
                };
            };
            /** ��������� �������� ������ **/
            strItem *tpbalance = new(nothrow) strItem[_PrCount];        // �������� ������ ��� �������� ������
            if(!tpbalance) {                                            // ���� ������ �� ��������, ��
                delete[] tvalue;                                        // ������� ����� ��������� ������ tvalue
                return nullptr;                                         // ������� � ���������� nullptr
            };
            for(size_t i=sZero; i<_PrCount; i++) {                      // �������� �������� �������� � �������� ������
                (tpbalance+i)->value  = *(tvalue +i);                   // �������� �������������
                (tpbalance+i)->volume = (tpbalance+i)->value > epsln ? (ProPlan+i)->volume : dZero; // �������� ����� ���
                                                                        // �������, ��� ������������� �� ����� ����
                (tpbalance+i)->price = (tpbalance+i)->volume > epsln ? (tpbalance+i)->value / (tpbalance+i)->volume : dZero;
            };                                                          // ������������� ������� �� ����
            delete[] tvalue;                                            // ������� ��������������� ������
            return tpbalance;
        }   // CalcProductBalance

        decimal* clsRecipeItem::CalcRawMatVolume(const size_t PrCount, const strItem volume[]) const {
        /** ����� ������������ � ���������� ����� ����������� �������� � ����������� ��������� ��� ����� �����
        ������� ��������. ���������:  PrCount - ����� �������� �������, volume[] - ��������� �� ������ ����������� PrCount �
        �������� ������������ �������� �� �������� (���� ������� ��������). ����� ���������� 2D-������ ������ ��������
        ������������ ����� �������� ��������, � ������� - ������ �������. **/
            if(!rnames) { return nullptr; };                // ���� ����������� ������ � ������������� �����, �� "�����"
            if(!recipeitem) { return nullptr; };            // ���� ����������� ������� � �����������, �� "�����"
            if(!volume) { return nullptr; };                // ���� ����������� ������ � �������� ������������ ��������
            size_t tcount = rcount * PrCount;               // ������� ������������ �������
            decimal *temp = new(nothrow) decimal[tcount];   // �������� ������ ��� ��������������� ���������� (3D-������)
            if(!temp) { return nullptr; };                  // ���� ������ �� ��������, �� ���������� "�����"
            for(size_t i=sZero; i<tcount; i++) {            // ��������� ������ ������
                *(temp+i) = dZero;
            };
            decimal *GRFtmp = nullptr;
            for(size_t i=duration; i<PrCount; i++) {
                GRFtmp = CalcRawMatVolumeItem(PrCount, i, (volume+i)->volume);
                Sum(temp, GRFtmp, tcount);                  // ��������� 2D-������� ����� ��� ���� �������� �������,
                if(GRFtmp) delete[] GRFtmp;                 // ������������ ���������. ������� ��������������� ������
                GRFtmp = nullptr;
            };                                              // ��� �������� ������������ ���������
            return temp;
        }   // CalcRawMatVolume

        strNameMeas* clsRecipeItem::GetRawNamesItem() const {
        /** ����� ���������� ��������� �� ����� ����������� ������������ ������ � �������������� ��������
        � ��������� ��������� ������� �������� �� ������������ ������� ��������. ������ ��������� ��� ����� � ��������
        �������� ����� �������������. **/
            if(rcount == sZero) return nullptr;         // ���� ������ ������� � ������������� ����� �������, �� ����� � "�����"
            strNameMeas *temp = new(nothrow) strNameMeas[rcount];   // �������� �������� ������ ��� ���������������� �������
            if(!temp) return nullptr;                   // ���� ������ �� ��������, �� ����� � ������� "�����"
            for(size_t i=sZero; i<rcount; i++) {        // ���� ������ ��������, �� �������� �������
                (temp+i)->name = (rnames+i)->name;                  // ������������ �����
                (temp+i)->measure = (rnames+i)->measure;            // ����������� ������� �� �������
            }
            return temp;
        }   // GetRawNamesItem

        const strNameMeas* clsRecipeItem::GetRefRawNamesItem() const {
        /** ����� ���������� ����������� ��������� �� ���������� ������ � �������������� ����� � ���������� � ���������
        ������������ ��������� �����. **/
            return rnames;
        }   // GetRefRawNamesItem

        decimal* clsRecipeItem::GetRecipeitem() const {
        /** ����� ���������� ��������� �� ����� ����������� ������������ ������ � ����������� (������ ��������� �������
        �������� rcount*duration). ������ ��������� ��� ����� � �������� �������� ����� �������������. **/
            if(!recipeitem) return nullptr;                     // ���� ��� ������� ��������, �� �����
            if(!rnames) return nullptr;                         // ���� ��� ������� � ������������� ����� � ����������, �� �����
            if(rcount == sZero) return nullptr;                 // ���� ������ ������� � ������� ����� �������, �� �����
            if(duration == sZero) return nullptr;               // ���� ������������ ���������������� ����� �������, �� �����
            const size_t tcount = rcount*duration;
            decimal *temp = new(nothrow) decimal[tcount];       // �������� ������ ��� ������
            if(!temp) return nullptr;                           // ���� ������ �� ��������, �� ����� � ������� nullptr
            var_cpy(temp, recipeitem, tcount);
            return temp;
        }   // GetRecipeitem

        const decimal* clsRecipeItem::GetRefRecipeitem() const {
        /** ����� ���������� ����������� ��������� �� ���������� ������ � ����������� (������ ��������� ������� ��������
        rcount*duration). **/
            return recipeitem;
        }   // GetRefRecipeitem

        const size_t clsRecipeItem::GetDuration() const { return duration; };   // ������� ������������ ����������������� �����
        const size_t clsRecipeItem::GetRCount() const { return rcount; };       // ������� ������� ������� rnames (���������� ������� �����)
        const string& clsRecipeItem::GetName() const {return name; };           // ������� ������ �� ������������ ��������
        const string& clsRecipeItem::GetMeasure() const { return measure; };    // ������� ������ �� ������������ ������� ��������� ��������

/** --------------------------------- ������ ������������ � �������������� ------------------------------------------------ **/

        bool clsRecipeItem::StF(ofstream &_outF)  {
        /** ����� ������������� ������ � �������� ���������� �������� ���������� ������ clsRecipeItem (������ � ����, �����
        ������������). ���������: &_outF - ��������� ������ ofstream ��� ������ ������. ��������!!! ���� ������ ���� ������
        � ������ ios::binary (�� ��������� �����!). **/
            if( (duration==sZero) || (rcount==sZero) || (!rnames) || (!recipeitem) ) return false;  // ��������� ����������
            #ifdef CDtor_voice                            // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                long bpos = _outF.tellp();                // ���������� ������� � ������ �����
                cout << "clsRecipeItem::StF begin bpos= " << bpos << endl;
            #endif                                        // CDtor_voice
            if(!SEF(_outF, name)) return false;           // ��������� ������������ ��������
            if(!SEF(_outF, measure)) return false;        // ��������� ��.���������
            if(!SEF(_outF, duration)) return false;       // ��������� ������������ ����������������� �����
            if(!SEF(_outF, rcount)) return false;         // ��������� ���������� ������� � ������������ ����� � ���������� ��� ������������ ��������
            for(size_t i=sZero; i<rcount; i++) {          // ��������� ������ � ���������� ����� � ��������� ���������
                if(!SEF(_outF, (rnames+i)->name)) return false;
                if(!SEF(_outF, (rnames+i)->measure)) return false;
            };
            if(!SEF(_outF, recipeitem, rcount*duration)) return false;    // ��������� ������ ��������� ��������. ����������� rcount*duration
            #ifdef CDtor_voice
                long epos = _outF.tellp();                                // ���������� ������� � ����� �����
                cout << "clsRecipeItem::StF end epos= " << epos << endl;
            #endif // CDtor_voice
            return true;                                  // ���������� true
        }   // StF

        bool clsRecipeItem::SaveToFile(const string _filename) {
        /** ����� ������ �������� ���������� ������ � ���� **/
            if(_filename == EmpStr) return false;               // ���� ��� ����� ������, �� ����� � ������� false
                                                                // 1.   ������� ��������� ������ ofstream
            ofstream outF(_filename, ios::out | ios::binary);   // ��������� ��� ����� � �������� ������� ��� ������ �� ����
                                                                // 2.   ���������, ������� �� ������ ����,
            if (!outF.is_open()) {                              // ���� ���� �� ������, �� ��������� ������������ �
                cout << fileopenerror << endl;                  // ������� false � ����� �� �������
                return false;
            };
            if(!StF(outF)) {                                    // 3.   ���������� ������ � ����. ���� ���� �� �������, ��
                outF.close();                                   // �������� ���� �
                return false;                                   // ������� � false
            };
            outF.close();                                       // 4.   ��������� ����
            return true;                                        // 5.   ���������� true
        }   // SaveToFile

        bool clsRecipeItem::RfF(ifstream &_inF) {
        /** ����� ������������� ������ �� �������� ���������� �������� ���������� ������ clsRecipeItem(������ �� �����, �����
        ��������������). ���������: &_inF - ��������� ������ ifstream ��� ������ ������. ��������!!! ���� ������ ���� ������
        � ������ ios::binary (�� ��������� �����!).  **/
            #ifdef CDtor_voice
                long bpos = _inF.tellg();                     // ���������� ������� � ������ �����
                cout << "clsRecipeItem::RfF begin bpos= " << bpos << endl;
            #endif // CDtor_voice
            if(rnames) { delete[] rnames; };
            if(recipeitem) { delete[] recipeitem; };
            if(!DSF(_inF, name)) return false;            // ������ ������������ ��������
            if(!DSF(_inF, measure)) return false;         // ������ ��.���������
            if(!DSF(_inF, duration)) return false;        // ������ ������������ ����������������� �����
            if(!DSF(_inF, rcount)) return false;          // ������ ���������� ������� � ������������ ����� � ���������� ��� ������������ ��������
            rnames = new(nothrow) strNameMeas[rcount];          // �������� ������ ��� �������
            if(rnames) {                                        // ���� ������ ��������, ��
                for(size_t i=sZero; i<rcount; i++) {            // ������ ������ � ���������� ����� � ��������� ���������
                    if(!DSF(_inF, (rnames+i)->name)) return false;
                    if(!DSF(_inF, (rnames+i)->measure)) return false;
                }
            } else return false;
            const size_t tsize = rcount*duration;
            recipeitem = new(nothrow) decimal[tsize];           // �������� ������ ��� �������
            if(recipeitem) {                                    // ���� ������ ��������, ��
                if(!DSF(_inF, recipeitem, tsize)) return false; // ������ ������ � ����������� ��������
            } else return false;
            #ifdef CDtor_voice
                long epos = _inF.tellg();                       // ���������� ������� � ����� �����
                cout << "clsRecipeItem::RfF end epos= " << epos << endl;
            #endif // CDtor_voice
            return true;
        } // RfF

        bool clsRecipeItem::ReadFromFile(const string _filename) {
        /** ����� ������ �� ����� � ������ � ��������� ������ **/
            if(_filename == EmpStr) return false;               // ���� ��� ����� ������, �� ����� � ������� false
                                                                // 1.   ������� ��������� ������ ifstream
            ifstream inF(_filename, ios::in | ios::binary);     // ��������� ��� ����� � �������� ������� ��� ������ � �����
                                                                // 2.   ���������, ������� �� ������ ����
            if (!inF.is_open()) {                               // ���� ���� �� ������, �� ��������� ������������ �
                cout << fileopenerror << endl;                  // ������� false � ����� �� �������
                return false;
            };
            if(!RfF(inF)) {                                     // 3.   ��������� ������ �� �����. ���� ���� �� �������, ��
                inF.close();                                    // ��������� ����
                return false;                                   // ����� � ������� false
            }
            inF.close();                                        // 4.   ��������� ����
            return true;
        }   // ReadFromFile

/** -----------------------   ������ ����������� �������� ������� ������ clsRecipeItem   ---------------------------------  **/

        void clsRecipeItem::ViewMainParametrs() const {
        /** ������� �� ����� �������� ��������� ������ **/
            cout << "�������� ��������                                  " << GetName() << endl;
            cout << "������� ��������� ��������                         " << GetMeasure() << endl;
            cout << "������������ ���������������� �����                " << GetDuration() << endl;
            cout << "���������� ������� ����� � ���������� � ���������  " << GetRCount() << endl << endl;
        }   // ViewMainParametrs

        void clsRecipeItem::ViewRawNames() const {
        /** ����� ����������� �������� ����������������� ������� GetRawNamesItem **/
            cout << "�������� ������ ������ GetRawNamesItem" << endl << endl;
            strNameMeas* temp = GetRawNamesItem();
            if(temp) {
                v_service(temp, rcount);
                delete[] temp;
            }
        }   // ViewRawNames

        void clsRecipeItem::ViewRefRawNames() const {
        /** ����� ����������� �������� ����������������� ������� GetRefRawNamesItem **/
            cout << "�������� ������ ������ GetRefRawNamesItem" << endl << endl;
            const strNameMeas* temp = GetRefRawNamesItem();
            v_service(temp, rcount);
        }   // ViewRefRawNames

        void clsRecipeItem::ViewRecipeitem() const {
        /** ����� ����������� �������� ����������������� ������� GetRecipeitem. **/
            cout << "�������� ������ ������ GetRecipeitem" << endl;
            cout << "��������� �������� " << name << "; ������� ��������� �������� " << measure << endl;
            decimal *temp = GetRecipeitem();                        // �������� ������ ��������
            if(!temp) return;                                       // ���� ������ ����, �� �����
            strNameMeas *tnames = new(nothrow) strNameMeas[rcount]; // ��������������� ������ ����
            if(!tnames) {                                           // ���� ������ �� ��������, ��
                delete[] temp;                                      // ������� ������ temp
                return;                                             // � ������� �� ������
            }
            for(size_t i=sZero; i<rcount; i++) {
                (tnames+i)->name = (rnames+i)->name;                        // �������� ����� � ����������
                (tnames+i)->measure = (rnames+i)->measure + "/" + measure;  // ������� ��������� ��������� ������� �� �������
            };
            ArrPrint(rcount, tnames, temp, duration);   // ������� �� �����
            delete[] temp;                              // ������� ��������� ������
            delete[] tnames;                            // ������� ��������� ������
        }   // ViewRecipeitem

        void clsRecipeItem::ViewRefRecipeitem() const {
        /** ����� ����������� �������� ����������������� ������� GetRefRecipeitem. **/
            cout << "�������� ������ ������ GetRefRecipeitem" << endl;
            cout << "��������� �������� " << name << "; ������� ��������� �������� " << measure << endl;
            const decimal *temp = GetRefRecipeitem();                // �������� ��������� �� ������ ��������
            if(temp && rnames ) {                                   // ���� ������ �� ������, ��
                ArrPrint(rcount, rnames, temp, duration);           // ������� �� �����
            }
        }   // ViewGetRefRecipeitem

        void clsRecipeItem::ViewRawMatVolume(const size_t PrCount, const strItem ProPlan[]) const {
        /** ����� ����������� �������� ����������������� ������� CalcRawMatVolume **/
            cout << "�������� ������ ������ CalcRawMatVolume" << endl;
            cout << "����������� � ����� � ���������� ��� �������� " << name << endl;
            decimal *temp = CalcRawMatVolume(PrCount, ProPlan);
            ArrPrint(rcount, rnames, temp, PrCount);
            if(temp) delete[] temp;
        }   // ViewRawMatVolume

        void clsRecipeItem::ViewWorkingValueItem(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** ����� ����������� �������� ����������������� ������� CalcWorkingValueItem. ���������: PrCount - ����� ��������
        �������, ProPlan[] - ������ c ������ ������ ������� ��������� ������������ PrCount, rmprice[]  - 2D-������ � ������
        ����� ������������ rcount*PrCount, ��� rcount - ���������� ������� �����, hmcur - ������������ ������ �������
        ("RUR", "CNY", "USD", "EUR" � ����.). **/
            cout << "�������� ������ ������ CalcWorkingValueItem" << endl;
            cout << "������ ����� � �������� ��������� �� ������� " << name \
                 << " � ������� ���������� ���������������� �����" << endl;
            cout << "������ - ��������� ��������������� �����, ������� - ������ �� �������" << endl;
            View_f_Item(PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcWorkingValueItem);
        }   // ViewWorkingValueItem

        void clsRecipeItem::ViewWorkingValue(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** ����� ����������� �������� ����������������� ������� CalcWorkingValue **/
            cout << "�������� ������ ������ CalcWorkingValue" << endl;
            cout << "������������� �������� " << name << " � ������� ���������� ���������������� �����" << endl;
            cout << "������ - ��������� ��������������� �����, ������� - ���������. ������ � �������" << endl;
            View_f_Item(PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcWorkingValue);
        }   // ViewWorkingValue

        void clsRecipeItem::ViewWorkingBalanceItem(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** ����� ����������� �������� ����������������� ������� CalcWorkingBalanceItem **/
            cout << "�������� ������ ������ CalcWorkingBalanceItem" << endl;
            cout << "������������� ������������ � �������� ��������� �������� " << name \
                 << " � ������� ���������� ���������������� �����" << endl;
            cout << "������ - ��������� ��������������� �����, ������� - ������������� ������������ � �������" << endl;
            View_f_Item(PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcWorkingBalanceItem);
        }   // ViewWorkingBalanceItem

        void clsRecipeItem::ViewWorkingBalance(const size_t _PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** ����� ����������� �������� ����������������� ������� CalcWorkingBalance **/
            cout << "�������� ������ ������ CalcWorkingBalance" << endl;
            cout << "������������� ������������ � �����������, �������� � ������ ����������� ���������" << endl;
            View_f_Balance(_PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcWorkingBalance);
        }   // ViewWorkingBalance

        void clsRecipeItem::ViewProductBalanceItem(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** ����� ����������� �������� ����������������� ������� CalcProductBalanceItem **/
            cout << "�������� ������ ������ CalcProductBalanceItem" << endl;
            cout << "������������� �������� �������� " << name << " � ������� ���������� ���������������� �����" << endl;
            cout << "������ - ��������� ��������������� �����, ������� - ������������� �������� �������� � �������" << endl;
            View_f_Item(PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcProductBalanceItem);
        }   // ViewProductBalanceItem

        void clsRecipeItem::ViewProductBalance(const size_t _PrCount, const strItem ProPlan[], const decimal rmprice[],
        const string& hmcur) const {
        /** ����� ����������� �������� ������� CalcProductBalance **/
            cout << "�������� ������ ������ CalcProductBalance" << endl;
            cout << "������� ������� � �����������, �������� � ������ ����������� ���������" << endl;
            View_f_Balance(_PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcProductBalance);
        }   // ViewProductBalance

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                             class clsManufactItem                                                   **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

/** ����� - ������������ ���������� ��������. ������ ������ � ����������� � ����� � ���������� ��� ������� �������� � ��������
������� �� ��������� �������, ���� �� ����� � ���������, ������, ������ � �������� ������������� �������������� ��������,
���������� ������ � �������� ��������� �������������� ������������. **/

        void clsManufactItem::clsEraser() {
        /** ����� "��������" ��� ���� ������. ������������ ������ � ������������ �����������. **/
            PrCount = sZero;
            name = nullptr;
            measure = nullptr;
            duration = sZero;
            rcount = sZero;
            Recipe = nullptr;
            ProductPlan = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice = nullptr;
            Balance = nullptr;
        }   // clsEraser

        clsManufactItem::clsManufactItem() {
        /** ����������� ��� ���������� **/
            PrCount = sZero;
            name = nullptr;
            measure = nullptr;
            duration = sZero;
            rcount = sZero;
            Recipe = nullptr;
            ProductPlan = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice = nullptr;
            Balance = nullptr;
            #ifdef CDtor_voice
                cout << "Empty Ctor clsManufactItem" << endl;
            #endif // CDtor_voice
        }   // clsManufactItem ����������� ��� ����������

        clsManufactItem::clsManufactItem(const size_t _PrCount, const clsRecipeItem &obj) {
        /** ����������� � �����������: _PrCount - ���������� �������� �������, obj - const ������ �� ������ ����
            clsRecipeItem. ������ obj ����������. **/
            PrCount = _PrCount;
            Recipe = new(nothrow) clsRecipeItem(obj);
            if(Recipe) {
                name = &Recipe->GetName();
                measure = &Recipe->GetMeasure();
            } else {
                name = nullptr;
                measure = nullptr;
            };
            duration = Recipe->GetDuration();
            rcount = Recipe->GetRCount();
            ProductPlan = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice = nullptr;
            Balance = nullptr;
            #ifdef CDtor_voice
                cout << "Ctor clsManufactItem with const clsRecipeItem &obj in param" << endl;
            #endif // CDtor_voice
        }   // clsManufactItem(const size_t _PrCount, clsRecipeItem &&obj)

        clsManufactItem::clsManufactItem(const size_t _PrCount, clsRecipeItem &&obj) {
        /** ����������� � �����������: _PrCount - ���������� �������� �������, obj - rvalue ������ �� ������ ����
        clsRecipeItem. ������ obj ������������. **/
            PrCount = _PrCount;
            Recipe = new(nothrow) clsRecipeItem(move(obj));
            if(Recipe) {
                name = &Recipe->GetName();
                measure = &Recipe->GetMeasure();
            } else {
                name = nullptr;
                measure = nullptr;
            };
            duration = Recipe->GetDuration();
            rcount = Recipe->GetRCount();
            ProductPlan = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice = nullptr;
            Balance = nullptr;
            #ifdef CDtor_voice
                cout << "Ctor clsManufactItem with clsRecipeItem &&obj in param" << endl;
            #endif // CDtor_voice
        }   // clsManufactItem(const size_t _PrCount, clsRecipeItem &&obj)

        clsManufactItem::clsManufactItem(const size_t _PrCount, const string &_name, const string &_measure, \
            const size_t _duration, const size_t _rcount, const strNameMeas _rnames[], const decimal _recipeitem[]) {
        /** ����������� � �����������, ������� ����� �������� **/
        PrCount = _PrCount;
        duration = _duration;
        rcount = _rcount;
        Recipe = new(nothrow) clsRecipeItem(_name, _measure, duration, rcount, _rnames, _recipeitem);
        if(Recipe) {
            name = &Recipe->GetName();
            measure = &Recipe->GetMeasure();
        } else {
            name = nullptr;
            measure = nullptr;
        };
        ProductPlan  = nullptr;
        RawMatPurchPlan = nullptr;
        RawMatPrice  = nullptr;
        Balance = nullptr;
        #ifdef CDtor_voice
            cout << "Ctor clsManufactItem with parameters" << endl;
        #endif // CDtor_voice
        }   // Ctor clsManufactItem

        clsManufactItem::clsManufactItem(const clsManufactItem &obj) {
        /** ����������� ����������� **/
            PrCount = obj.PrCount;
            duration = obj.duration;
            rcount = obj.rcount;
            if(obj.Recipe) {
                Recipe = new(nothrow) clsRecipeItem( *(obj.Recipe) );       // ��������������� ������� ������������
                if(Recipe) {
                    name = &Recipe->GetName();
                    measure = &Recipe->GetMeasure();
                } else {
                    name = nullptr;
                    measure = nullptr;
                }
            } else {
                Recipe = nullptr;
                name = nullptr;
                measure = nullptr;
            };
            if(obj.ProductPlan) {
                ProductPlan = new(nothrow) strItem[PrCount];
                if(ProductPlan)
                    var_cpy(ProductPlan, obj.ProductPlan, PrCount);     // ����������� ��������
            } else { ProductPlan = nullptr; };
            const size_t temp = rcount*PrCount;                         // ��������������� ����������
            if(obj.RawMatPurchPlan) {
                RawMatPurchPlan = new(nothrow) decimal[temp];
                if(RawMatPurchPlan)
                    var_cpy(RawMatPurchPlan, obj.RawMatPurchPlan, temp);
                } else {  RawMatPurchPlan = nullptr;}
            if(obj.RawMatPrice) {
                RawMatPrice = new(nothrow) decimal[temp];
                if(RawMatPrice)
                    var_cpy(RawMatPrice, obj.RawMatPrice, temp);        // ����������� ��������
            } else { RawMatPrice = nullptr; };
            if(obj.Balance) {
                Balance = new(nothrow) strItem[PrCount];
                if(Balance)
                    var_cpy(Balance, obj.Balance, PrCount);
            } else { Balance = nullptr; }
            #ifdef CDtor_voice
                cout << "Copy Ctor clsManufactItem" << endl;
            #endif // CDtor_voice
        }   // Copy Ctor clsManufactItem

        void clsManufactItem::swap(clsManufactItem& obj) noexcept {
        /** ������� ������ ���������� ����� ���������. ������� ��������� noexcept - �� ���������� ���������� **/
            std::swap(PrCount, obj.PrCount);
            std::swap(duration, obj.duration);
            std::swap(rcount, obj.rcount);
            std::swap(name, obj.name);
            std::swap(measure, obj.measure);
            std::swap(Recipe, obj.Recipe);
            std::swap(ProductPlan, obj.ProductPlan);
            std::swap(RawMatPurchPlan, obj.RawMatPurchPlan);
            std::swap(RawMatPrice, obj.RawMatPrice);
            std::swap(Balance, obj.Balance);
            #ifdef CDtor_voice    // ������ ������ ���������� ����������.
                cout << "clsManufactItem swap" << endl;
            #endif
        }   // swap

        clsManufactItem::clsManufactItem(clsManufactItem &&obj) {
        /** ����������� �����������. ����� ����������� ���������� obj �� ���������, ������ �� ���� ���������� ����
        ��������� �� nullptr. **/
            clsEraser();    // ���������� ��������� �������� ������� this
            swap(obj);      // ������������ ����������� � obj
            #ifdef CDtor_voice
                cout << "Move Ctor clsManufactItem" << endl;
            #endif // CDtor_voice
        }   // Move Ctor clsManufactItem

        clsManufactItem& clsManufactItem::operator= (const clsManufactItem &obj) {
        /** ���������� ��������� ������������ ������������. ����������� � ������
        �����������-�-������ (copy-and-swap idiom) **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsManufactItem tmp(obj);           // �������� ����������� ����������� � �������� ����� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef CDtor_voice
                cout << "clsManufactItem Copy &operator=" << endl;
            #endif // CDtor_voice
            return *this;
        }   // clsManufactItem Copy &operator=

        clsManufactItem& clsManufactItem::operator= (clsManufactItem &&obj) {
        /** ���������� ��������� ������������ ������������. ����������� � ������ �����������-�-������
        (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsManufactItem tmp(move(obj));     // �������� ����������� ����������� � �������� ���������� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef CDtor_voice
                cout << "clsManufactItem Move &operator=" << endl;
            #endif // CDtor_voice
            return *this;
        }   // clsManufactItem Move &operator=

        bool clsManufactItem::operator == (const string &Rightname) const {
        /** ��������������� ��������� ��������� ��� ������ ���������� ������� �� ������������ �������� **/
            return (*name == Rightname);
        }   // operator ==

        clsManufactItem::~clsManufactItem() {
            if(Recipe) {
                #ifdef CDtor_voice
                    cout << "delete Recipe" << endl;
                #endif // CDtor_voice
                delete Recipe; };
            if(ProductPlan) {
                #ifdef CDtor_voice
                    cout << "delete[] ProductPlan" << endl;
                #endif // CDtor_voice
                delete[] ProductPlan; };
            if(RawMatPurchPlan) {
                #ifdef CDtor_voice
                    cout << "delete[] RawMatPurchPlan" << endl;
                #endif // CDtor_voice
                delete[] RawMatPurchPlan; };
            if(RawMatPrice) {
                #ifdef CDtor_voice
                    cout << "delete[] RawMatPrice" << endl;
                #endif // CDtor_voice
                delete[] RawMatPrice; };
            if(Balance) {
                #ifdef CDtor_voice
                    cout << "delete[] Balance" << endl;
                #endif // CDtor_voice
                delete[] Balance; };
            #ifdef CDtor_voice
                cout << "Dtor clsManufactItem" << endl;
            #endif // CDtor_voice
        }   // Dtor clsManufactItem

        /** Get - ������ **/

        const size_t& clsManufactItem::GetPrCount() const { return PrCount; }   // ���������� const-������ �� ���������� ��������
        const size_t& clsManufactItem::GetRCount() const { return rcount; }     // ���������� const-������ �� ���������� ������� �����, ������������ � ������������
        const size_t& clsManufactItem::GetDuration() const { return duration; } // ���������� ������������ ����������������� �����

        const string* clsManufactItem::GetName() const {
        /** ����� ���������� ����������� ��������� �� ������������ ��������. **/
            if(!name) return nullptr;
            return name;
        }   // GetName

        const string* clsManufactItem::GetMeasure() const {
        /** ����� ���������� ����������� ��������� �� ������� ��������� ������������ ������ ��������. **/
            if(!measure) return nullptr;
            return measure;
        }   // GetMeasure

        const decimal* clsManufactItem::GetRefRecipe() const {
        /** ����� ���������� ����������� ��������� �� ���������� ������ � �����������. **/
            if(!Recipe) return nullptr;
            return Recipe->GetRefRecipeitem();
        }   // GetRefRecipe

        const strNameMeas* clsManufactItem::GetRefRawNames() const {
        /** ����� ���������� ����������� ��������� �� ���������� ������ � �������������� �������� �
        ��������� ������������ ��������� ��������. **/
            if(!Recipe) return nullptr;
            return Recipe->GetRefRawNamesItem();
        }   // GetRefRawNames

        const decimal* clsManufactItem::GetRawMatPurchPlan() const {
        /** ����� ���������� ����������� ��������� �� ������ RawMatPurchPlan � ������� ����������� �������� �
        ����������� ��������� ��� ����� ����� ������� �������� **/
            if(!RawMatPurchPlan) return nullptr;        // ���� ������ �� ����������, �� ����� � ������� nullptr
            return RawMatPurchPlan;
        } // GetRawMatPurchPlan

        const decimal* clsManufactItem::GetRawMatPrice() const {
        /** ����� ���������� ����������� ��������� �� ������ RawMatPrice � ������ �� �������. **/
            if(!RawMatPrice) return nullptr;    // ���� ������ �� ����������, �� ����� � ������� nullptr
            return RawMatPrice;
        }   // GetRawMatPrice

        const strItem* clsManufactItem::GetBalance() const {
        /** ����� ���������� ����������� ��������� �� ������ � �������, �������� � ������ �������������� ��������������
        ������������ ��� ����������� ��������, ������������ �� ���������� ����� �������. **/
            return Balance;
        }   // GetBalance

        const strItem* clsManufactItem::GetProductPlan() const {
        /** ����� ���������� ����������� ��������� �� ������ � �������, �������� � ������ �������������� ������� ��������� ���
        ����������� ��������, ������������ �� ���������� ����� �������. **/
            return ProductPlan;
        }   // GetProductPlan

        /** Set - ������ **/

        bool clsManufactItem::SetProductPlan(const strItem _ProductPlan[]) {
        /** ����� ����� ����� ������� �������� (����� ������� � ����������� ��������� � ������ �������).
        ���������: _ProductPlan - ������ ���� strItem ������������ PrCount. ������������ ������ ���� _Product.volume.  **/
            if((PrCount==sZero) || (!_ProductPlan)) return false;   // ��������� ����������
            strItem* temp = new(nothrow) strItem[PrCount];          // �������� ������ ��� ��������� ������
            if(!temp) return false;                                 // ���� ������ �� ��������, �� ����� � ������� false
            var_cpy(temp, _ProductPlan, PrCount);                   // �������� �������
            std::swap(temp, ProductPlan);                           // ���������� ��������� ��������
            if(temp) delete[] temp;                                 // ������� ��������������� ������, ���� �� �� ������
            return true;
        }   // SetProductPlan

        bool clsManufactItem::SetRawMatPrice(const decimal _Price[]) {
        /** ����� ����� ��� �� ����� � ���������. ��������������, ��� ����� ��������� ������� ���������� � �����������
        � ����� � ����������, ����� ���������� ���������� �� ������� ����� �� ����� � ���������. ��� ���������� � �������
        ������� ������ ���������� � ������ RawMatPrice �������� rcount*PrCount. ���������: _Price - ��������� �� ������
        (�������) ��� �� ����� � ��������� �������� rcount*PrCount. **/
            const size_t tcount = rcount*PrCount;               // ��������� ������ ������� (������ ��������� �������)
            if((tcount==sZero) || (!_Price)) return false;      // ��������� ����������
            decimal* temp = new(nothrow) decimal[tcount];       // �������� ������ ��� ������
            if(!temp) return false;                             // ���� ������ �� ��������, �� ����� � ������� false
            var_cpy(temp, _Price, tcount);                      // �������� �������
            std::swap(temp, RawMatPrice);                       // ���������� ��������� ��������
            if(temp) delete[] temp;                             // ������� ��������������� ������, ���� �� �� ������
            return true;
        }   // SetRawMatPrice

        bool clsManufactItem::MoveRawMatPrice(decimal _Price[]) {
        /** ����� ����� ��� �� ����� � ���������. ��������������, ��� ����� ��������� ������� ���������� � �����������
        � ����� � ����������, ����� ���������� ���������� �� ������� ����� �� ����� � ���������. ��� ���������� � �������
        ������� ������ ������������ � ������ RawMatPrice �������� rcount*PrCount. ���������: _Price - ��������� �� ������
        (�������) ��� �� ����� � ��������� �������� rcount*PrCount. �� ��������� ������ ��������� _Price ���������
        �������� nullptr. **/
            if(!_Price) return false;               // ��������� �������� �������
            if(RawMatPrice) delete[] RawMatPrice;   // ���� ��� ���� ������ � ������ � ���������� ������, �� ������� ���
            RawMatPrice = _Price;                   // ������������� ������
            _Price = nullptr;
            return true;
        }   // MoveRawMatPrice

        /** �������������� ������ **/

        bool clsManufactItem::Resize(size_t _n) {
        /** ������� �������� ������� ��������, ������������ ����� ����� �������� �������, ������ _n. ��� _n < PrCount,
        ������ ����������, ��� _n > PrCount, - ����������� ����� �������� �������� � �������� ����������. �������
        ���������� true ��� ������� ��������� �������� ��������, false - � ��������� ������ **/
            if(!ProductPlan) return false;          // ��� ������� ��� �������������
            if(_n < sOne) { return false; };        // ������������ ������
            if(_n == PrCount) { return true; };     // ��������� ������� �� ������������
            strItem *p;                             // ��������� ��������� ��� ������� ProductPlan
            p = new(nothrow) strItem[_n];           // �������� ������ �������
            if(!p) return false;                    // ���� ������ �� ��������, �� ����� � false
            if(_n < PrCount)                        // ���� ����� ����� �������� ������ �������, ��
                for(size_t i{}; i<_n; i++)
                    *(p+i) = *(ProductPlan+i);
            else {
                for(size_t i{}; i<PrCount; i++)
                    *(p+i) = *(ProductPlan+i);
                for(size_t i=PrCount; i<_n; i++) {
                    (p+i)->volume = dZero;
                    (p+i)->price  = dZero;
                    (p+i)->value  = dZero;
                }
            }
            delete[] ProductPlan;
            if (Balance) delete[] Balance;
            if (RawMatPurchPlan) delete[] RawMatPurchPlan;
            if (RawMatPrice) delete[] RawMatPrice;
            ProductPlan = p;
            Balance     = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice     = nullptr;
            PrCount = _n;
            return true;
        }   // Resize

        void clsManufactItem::CalcRawMatPurchPlan() {
        /** ����� ������������ ����� ����������� ����� � ���������� � ����������� ��������� ��� ����� �����
        ������� �������� � ��������� ������ RawMatPurchPlan. **/
            if(PrCount>sZero)
                RawMatPurchPlan = Recipe->CalcRawMatVolume(PrCount, ProductPlan);
        }   // CalcRawMatPurchPlan

        bool clsManufactItem::CalculateItem() {
        /** ����� ������������ �����, �������� � ������ ������������� �������������� ������������ � ������� ���������
        ��� ����������� ��������, ������������ �� ���������� ����� �������. **/
            if(!ProductPlan) return false;      // ���� ��� ������� � ������ ������ ������� ���������, �� ����� � false
            if(!RawMatPurchPlan) return false;  // ���� ��� ������� � ������ ������� ����� � ����������, �� ����� � false
            if(!RawMatPrice) return false;      // ���� ��� ������� � ������ ����� � ����������, �� ����� � false
            strItem* tmpBal = Recipe->CalcWorkingBalance(PrCount,RawMatPrice, ProductPlan); // �������� ������ �������. ���-��
            if(!tmpBal) return false;           // ���� ������ ����, �� ����� � false
            std::swap(tmpBal, Balance);         // ���������� ���������
            if(tmpBal) delete[] tmpBal;         // ������� ��������������� ������, ���� �� �� ������
            strItem* temp = Recipe->CalcProductBalance(PrCount, RawMatPrice, ProductPlan);  // �������� ������� ���������
            if(!temp) return false;             // ���� ������ ����, �� ����� � false
            std::swap(temp, ProductPlan);       // ���������� ���������
            if(temp) delete[] temp;             // ������� ��������������� ������, ���� �� �� ������
            return true;
        }   // CalculateItem

/** --------------------------------- ������ ������������ � �������������� ------------------------------------------------ **/

        bool clsManufactItem::StF(ofstream &_outF) {
        /** ����� ������������� ������ � �������� ���������� �������� ���������� ������ clsManufactItem (������ � ����, �����
        ������������). ���������: &_outF - ��������� ������ ofstream ��� ������ ������ **/
            if( (PrCount==sZero) || (duration==sZero) || (rcount==sZero) || (!Recipe) ) return false;
            #ifdef CDtor_voice
                long bpos = _outF.tellp();                                      // ���������� ������� � ������ ����� (�������)
                cout << "clsManufactItem::StF begin bpos= " << bpos << endl;    // ������� ������� �� ����� (�������)
            #endif // CDtor_voice
            /** ����� ��������� ������������ ��������. ���� ���� true, �� ��������������� ������ ������������ � ����, ����
            ��� - �� �� ������������. ����� ����������� � ����� � �������� ��� ������ ������ �� ����� ����������, ���� ���
            ������ � ��� ���� ���������, ��� ��� ���. **/
            bool flagProductPlan, flagRawMatPurchPlan, flagRawMatPrice, flagBalance;
            if(ProductPlan) flagProductPlan         = true; else flagProductPlan     = false;
            if(RawMatPurchPlan) flagRawMatPurchPlan = true; else flagRawMatPurchPlan = false;
            if(RawMatPrice) flagRawMatPrice         = true; else flagRawMatPrice     = false;
            if(Balance) flagBalance                 = true; else flagBalance         = false;
            #ifdef CDtor_voice
                cout << "flagProductPlan= " << flagProductPlan << endl;
                cout << "flagRawMatPurchPlan= " << flagRawMatPurchPlan << endl;
                cout << "flagRawMatPrice= " << flagRawMatPrice << endl;
                cout << "flagBalance= " << flagBalance << endl;
            #endif // CDtor_voice
            /** ��������� ����� � ���� **/
            if(!SEF(_outF, flagProductPlan)) return false;
            if(!SEF(_outF, flagRawMatPurchPlan)) return false;
            if(!SEF(_outF, flagRawMatPrice)) return false;
            if(!SEF(_outF, flagBalance)) return false;
            /** ��������� � ���� �������� ������ **/
            if(!SEF(_outF, PrCount)) return false;  // ��������� � ���� ���������� �������� �������
            if(!Recipe->StF(_outF)) return false;   // ���� �� ������� ��������� ������, �� ����� � ������� false
            if(flagProductPlan)
                for(size_t i{}; i<PrCount; i++) {
                    if(!(ProductPlan+i)->StF(_outF)) return false;
                };
            size_t tcount = rcount*PrCount;
            if(flagRawMatPurchPlan)
                if(!SEF(_outF, RawMatPurchPlan, tcount)) return false;  // ��������� � ���� ���� ������� ����� � ���. ���������
            if(flagRawMatPrice)
                if(!SEF(_outF, RawMatPrice, tcount)) return false;      // ��������� � ���� ���� �� ����� � ���������
            if(flagBalance)
                for(size_t i{}; i<PrCount; i++) {
                    if(!(Balance+i)->StF(_outF)) return false;
                };
                                                                        // �������� � ������ ��������� �����.������������
            #ifdef CDtor_voice
                long epos = _outF.tellp();                                  // ���������� ������� � ����� ����� (�������)
                cout << "clsManufactItem::StF end epos= " << epos << endl;  // ������� ������� �� ����� (�������)
            #endif // CDtor_voice
            return true;
        }   // StF

        bool clsManufactItem::SaveToFile(const string _filename) {
        /** ����� ������ �������� ���������� ������ clsManufactItem � ���� **/
            if(_filename == EmpStr) return false;               // ���� ��� ����� ������, �� ����� � ������� false
                                                                // 1.   ������� ��������� ������ ofstream
            ofstream outF(_filename, ios::out | ios::binary);   // ��������� ��� ����� � �������� ������� ��� ������ �� ����
                                                                // 2.   ���������, ������� �� ������ ����,
            if (!outF.is_open()) {                              // ���� ���� �� ������, �� ��������� ������������ �
                cout << "������ �������� �����" <<endl;         // ������� false � ����� �� �������
                return false;
            };
            if(!StF(outF)) {
                outF.close();
                return false;
            }                                          // 3.   ���������� ������ � ����
            #ifdef CDtor_voice
                long epos = outF.tellp();
                cout << "clsManufactItem::SaveToFile end epos= " << epos << endl;
            #endif // CDtor_voice
            outF.close();                                       // 4.   ��������� ����
            return true;                                        // 5.   ���������� true
        }   // SaveToFile

        bool clsManufactItem::RfF(ifstream &_inF) {
        /** ����� ������������� ������ �� �������� ���������� �������� ���������� ������ clsManufactItem (������ �� �����,
        ����� ��������������). ���������: &_inF - ��������� ������ ifstream ��� ������ ������ **/
            #ifdef CDtor_voice
                long bpos = _inF.tellg();                                       // ���������� ������� � ������ ����� (�������)
                cout << "clsManufactItem::RfF begin bpos= " << bpos << endl;    // ������� ������� �� ����� (�������)
            #endif // CDtor_voice
            /** ����� ��������� ������������ ��������. ���� ���� true, �� ��������������� ������ �������� �� �����, ����
            ��� - �� �� ��������. ����� �������� � ����� � �������� ��� ������ ������ �� ����� ����������, ���� ���
            ������ � ��� ���� ���������, ��� ��� ���. **/
            bool flagProductPlan, flagRawMatPurchPlan, flagRawMatPrice, flagBalance;
            /** ������ ����� �� ����� **/
            if(!DSF(_inF, flagProductPlan)) return false;
            if(!DSF(_inF, flagRawMatPurchPlan)) return false;
            if(!DSF(_inF, flagRawMatPrice)) return false;
            if(!DSF(_inF, flagBalance)) return false;
            #ifdef CDtor_voice
                cout << "flagProductPlan= " << flagProductPlan << endl;
                cout << "flagRawMatPurchPlan= " << flagRawMatPurchPlan << endl;
                cout << "flagRawMatPrice= " << flagRawMatPrice << endl;
                cout << "flagBalance= " << flagBalance << endl;
            #endif // CDtor_voice
            if(!DSF(_inF, PrCount)) return false;       // ������ �� ����� ���������� �������� �������
            if(!Recipe) Recipe = new(nothrow) clsRecipeItem(EmpStr, EmpStr, sZero, sZero, nullptr, nullptr);
            if(!Recipe) return false;
            if(!Recipe->RfF(_inF)) return false;    // ���� �� ������� ��������� ������, �� ����� � ������� false
            name = &Recipe->GetName();
            measure = &Recipe->GetMeasure();
            rcount = Recipe->GetRCount();
            duration = Recipe->GetDuration();
            if(flagProductPlan) {                                   // ���� ���� true, �� ������ ���� ��������� �� �����
                strItem* PPtmp = new(nothrow) strItem[PrCount];     // �������� ������ ��� ���������� �������
                if(!PPtmp) return false;                            // ���� ������ �� ��������, �� ����� � ������� false
                for(size_t i{}; i<PrCount; i++)                     // ���� ������ ��������, �� ������ ������ �� �����
                    if(!(PPtmp+i)->RfF(_inF)) return false;         // �����������
                std::swap(PPtmp, ProductPlan);                      // ������������ �����������
                if(PPtmp) delete[] PPtmp;                           // ������� ��������������� ������
            } else ProductPlan = nullptr;
            size_t tcount = rcount*PrCount;                         // �������� ������ �������
            if(flagRawMatPurchPlan) {                               // ���� ���� true, �� ������ ���� ��������� �� �����
                decimal* RMPPtmp = new(nothrow) decimal[tcount];    // �������� ������ ��� ���������� �������
                if(!RMPPtmp) return false;                          // ���� ������ �� ��������, �� ����� � ������� false
                if(!DSF(_inF, RMPPtmp, tcount)) return false;       // ���� ������ ��������, �� ������ �� ����� ������
                std::swap(RMPPtmp, RawMatPurchPlan);                // ������������ �����������
                if(RMPPtmp) delete[] RMPPtmp;                       // ������� ��������������� ������
            } else RawMatPurchPlan = nullptr;
            if(flagRawMatPrice) {                                   // ���� ���� true, �� ������ ���� ��������� �� �����
                decimal* RMprcTmp = new(nothrow) decimal[tcount];   // �������� ������ ��� ���������� �������
                if(!RMprcTmp) return false;                         // ���� ������ �� ��������, �� ����� � ������� false
                if(!DSF(_inF, RMprcTmp, tcount)) return false;      // ���� ������ ��������, �� ������ �� ����� ������
                std::swap(RMprcTmp, RawMatPrice);                   // ������������ �����������
                if(RMprcTmp) delete[] RMprcTmp;                     // ������� ��������������� ������
            } else RawMatPrice = nullptr;
            if(flagBalance) {                                       // ���� ���� true, �� ������ ���� ��������� �� �����
                strItem* tmpBal = new(nothrow) strItem[PrCount];    // �������� ������ ��� �������
                if(!tmpBal) return false;                           // ���� ������ �� ��������, �� �����
                for(size_t i{}; i<PrCount; i++)                     // ���� ������ ��������, �� ������ ������ �� �����
                    if(!(tmpBal+i)->RfF(_inF)) return false;        // �����������
                std::swap(tmpBal, Balance);                         // ������������ �����������
                if(tmpBal) delete[] tmpBal;                         // ������� ��������������� ������
            } else Balance = nullptr;

            #ifdef CDtor_voice
                long epos = _inF.tellg();                     // ���������� ������� � ������ �����
                cout << "clsManufactItem::RfF end epos= " << epos << endl;
            #endif // CDtor_voice
            return true;
        }   // RfF

        bool clsManufactItem::ReadFromFile(const string _filename) {
        /** ����� ������ �� ����� � ������ � ��������� ������ clsManufactItem **/
            if(_filename == EmpStr) return false;               // ���� ��� ����� ������, �� ����� � ������� false
                                                                // 1.   ������� ��������� ������ ifstream
            ifstream inF(_filename, ios::in | ios::binary);     // ��������� ��� ����� � �������� ������� ��� ������ � �����
                                                                // 2.   ���������, ������� �� ������ ����
            if (!inF.is_open()) {                               // ���� ���� �� ������, �� ��������� ������������ �
                cout << "������ �������� �����" <<endl;         // ������� false � ����� �� �������
                return false;
            };
            if(!RfF(inF)) {                                     // 3.   ��������� ������ �� �����
                inF.close();
                return false;
            };
            #ifdef CDtor_voice
                long epos = inF.tellg();
                cout << "clsManufactItem::ReadFromFile end epos= " << epos << endl;
            #endif // CDtor_voice
            inF.close();                // 4.   ��������� ����
            return true;
        }   // ReadFromFile

/** -----------------------   ������ ����������� �������� ������� ������ clsManufactItem   -------------------------------  **/

        void clsManufactItem::ViewMainParametrs() const {
        /** ����� ����������� �������� ������� GetPrCount, GetName, GetMeasure, GetDuration, GetRCount **/
            cout << endl;
            cout << "������������ �������                               " << GetPrCount() << endl;
            const string* temp = GetName();
            if(temp)
                cout << "�������� ��������                                  " << *temp << endl;
            temp = GetMeasure();
            if(temp)
                cout << "������� ��������� ��������                         " << *temp << endl;
            cout << "������������ ���������������� �����                " << GetDuration() << endl;
            cout << "���������� ������� ����� � ���������� � ���������  " << GetRCount() << endl << endl;
        }   // ViewMainParametrs

        void clsManufactItem::ViewRefRecipe() const {
        /** ����� ����������� �������� ������� GetRefRecipe � GetRefRawNames **/
            if(name) {
                cout << "�������� ������ ������� GetRefRecipe � GetRefRawNames" << endl;
                cout << "��������� �������� " << *name << " � ������� �� " << *measure << endl; // ���������
            };
            const decimal *trecipe = GetRefRecipe();            // �������� ��������� �� ������ ��������
            const strNameMeas *trnames = GetRefRawNames();      // �������� ��������� �� ������ � ���������� ����� � ��.���������
            if(trecipe && trnames ) {                           // ���� ������ �� ������, ��
                ArrPrint(Recipe->GetRCount(), trnames, trecipe, Recipe->GetDuration()); // ������� �� �����
            };
        }   // ViewRefRecipe

        void clsManufactItem::ViewRawMatPurchPlan() const {
        /** ����� ����������� �������� ����������������� ������� GetRCount, GetRefRawNames, GetPrCount,
        CalcRawMatPurchPlan � GetRawMatPurchPlan. **/
            cout << "�������� ������ ������� GetRCount, GetRefRawNames, GetPrCount, " << \
                "CalcRawMatPurchPlan � GetRawMatPurchPlan." << endl;
            cout << "����������� � ����� � ���������� ��� �������� " << *name << endl;
            ArrPrint(GetRCount(), GetRefRawNames(), GetRawMatPurchPlan(), GetPrCount());
        }   // ViewRawMatPurchPlan

        void clsManufactItem::ViewRawMatPrice(const string& hmcur) const {
        /** ����� ����������� �������� ����������������� ������� GetRCount, GetRefRawNames, GetPrCount,
        SetRawMatPrice � GetRawMatPrice. **/
            cout << "�������� ������ ������� GetRCount, GetRefRawNames, GetPrCount, " << \
                "SetRawMatPrice � GetRawMatPrice." << endl;
            cout << "���� �� ����� � ���������� ��� �������� " << *name << " � " << hmcur << " �� ��." << endl;
            ArrPrint(GetRCount(), GetRefRawNames(), GetRawMatPrice(), GetPrCount());
        }   // ViewRawMatPrice

        void clsManufactItem::ViewCalculate(const string& hmcur) const {
        /** ����� ����������� �������� ����������������� ������� GetBalance � GetProductPlan. **/
            cout << "�������� ������ ������� GetBalance � GetProductPlan" << endl << endl;
            cout << "������������� ������������ � �����������, �������� � ������ ����������� ���������" << endl;
            string tname = *(name);                             // ��������� ���������� ��� ����������� ����� ������
            tname.resize(15);                                   // �������� ������
            strNameMeas pname[sOne] = {tname, *measure};        // ��������� ��������� �������
            const strItem *Btemp = GetBalance();                // �������� ��������� �� ������ �������������� ������������
            if(Btemp) {                                         // ���� ������ �� ����, �� �������� �����
                ArrPrint(sOne, pname, Btemp, PrCount, volume, hmcur);  // ������ ����������� �����������
                ArrPrint(sOne, pname, Btemp, PrCount, price, hmcur);   // ������ ������� ����������� �����������
                ArrPrint(sOne, pname, Btemp, PrCount, value, hmcur);   // ������ ������ ����������� �����������
            };
            cout << "������� ������������ � �����������, �������� � ������ ����������� ���������" << endl;
            const strItem *Ptemp = GetProductPlan();            // �������� ��������� �� ������ �������� ������������
            if(Ptemp) {                                         // ���� ������ �� ����, �� �������� �����
                ArrPrint(sOne, pname, Ptemp, PrCount, volume, hmcur);  // ������ ����������� �����������
                ArrPrint(sOne, pname, Ptemp, PrCount, price, hmcur);   // ������ ������� ����������� �����������
                ArrPrint(sOne, pname, Ptemp, PrCount, value, hmcur);   // ������ ������ ����������� �����������
            }
        }   // ViewCalculate

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                             class clsManufactory                                                    **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

/** ����� - ������������ ���� ����������� ��� ������� ���������. ��������� ����������� ������ ����������� � ����� � ����������
��� ������� ��������� � �������� ������� �� ��������� �������, ���������� ������������� ������������� ���������, ����������
������, ���������� ��������� � �������� ��������� �������������� ������������ ������� ��������. ��� ������� ����������������
����������� ��������� ���� �� ����� (����� ������ ����� � ��������� �������� �� �� ����� ��������) ��������� �������
������������� ����� �����������. **/

        inline bool clsManufactory::Checkrnames(const size_t _rcount, const strNameMeas _rnames[]) {
        /** ����� �������� ����� ���� �� ������� _rnames � ������� RMNames. � ������ ������� � ������� _rnames ���� ��
        ������ �����, �������������� � RMNames, ������ false. ���� ������������� ���� ���, �� ���������� true. **/
            if(!_rnames) { return false; };                 // ���� ������ _rnames ����, �� ����� � ������� false
            if(!RMNames) { return false; };                 // ���� ������ RMNames ����, �� ����� � ������� false
            if(RMCount == sZero) { return false; };         // ���� ������� ���� � ������ ������� ����� �������, �� ����� � false
            if(_rcount == sZero) { return false; };         // ���� ������� � ������ _rnames �������, �� ����� � false
            size_t coincidence = sZero;                     // ������������� ������� � ����
            for(size_t i=sZero; i<_rcount; i++) {           // ���� �� ���� ������ � _rnames
                for(size_t j=sZero; j<RMCount; j++) {       // ���� �� ���� ������� � RMNames
                    if( (_rnames+i)->name == (RMNames+j)->name ) {    // ���� ���������� ����, ���� ������� ��
                        coincidence++;                      // ����������� ������� �� �������
                        break;                              // � ��������� ���� �� j
                    }
                }
            };
            if(coincidence == _rcount) { return true; }     // ��� ���������� ���� ���� �� ������� _rnames ���������� true
            else { return false; }                          // ���� ���� �� ���� ��� �� _rnames �� ��������� � RMNames, false
        }   // Checkrnames

        strItem* clsManufactory::gettotal(const strItem* (clsManufactItem::*f)() const) const {
        /** ����� ��������� ��������� �� ���������� ������, ���������� �������� ��������� ������� � ��������� ��������������
        ������������ (��� ����������� ������ f ������ GetBalance) ��� ������� ������ ���� ��������� (��� ����������� ������
        f ������ GetProductPlan) � �����������, �������� � ������ ����������� ���������. ������ ������� Manuf.size()*PrCount.
        ������ ������� ������� ����� ��� strItem, �.�. ����� � ����� ������� �������� volume, price � value ��� ��������.
        ������ ��������� � ������������ ������, ����� ������������� ������� ������ �������� � ������� ��������� delete[]. **/
            if(Manuf.size() == sZero) return nullptr;   // ���� ������ �� �������� �� ������ ���������� ������������, �� ����� � nullptr
            if(PrCount == sZero) return nullptr;        // ���� ����� �������� �������, �� ����� � nullptr
            strItem *temp = new(nothrow) strItem[Manuf.size()*PrCount]; // �������� �������� ������
            if(!temp) return nullptr;                   // ���� ������ �� ��������, �� ����� � nullptr
            size_t i = sZero;                           // ��������������� �������
            const strItem *tmp;                         // ��������������� ����������
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++) {  // ���� �� ���������
                tmp = ((*it).*f)();
                if(tmp) {
                    var_cpy((temp+PrCount*i), tmp, PrCount);   // �������� ���������� i-� ������
                    i++;                                        // � ������ ���������� �������
                }
            };
            return temp;
        }   // gettotal

        clsManufactory::clsManufactory() {
        /** ������ �����������. ����������� � ������ emplace_back ��� �������������� ������ �� �����. **/
            PrCount = sZero;
            RMCount = sZero;
            hmcur = RUR;
            RMNames = nullptr;
            pshell = nullptr;
            #ifdef CDtor_voice
                cout << "Empty Ctor clsManufactory" << endl;
            #endif // CDtor_voice
        }   // Emplty Ctor clsManufactory

        clsManufactory::clsManufactory(const size_t _PrCount, const size_t _RMCount, const strNameMeas _RMNames[], \
            const size_t msize):PrCount(_PrCount), RMCount(_RMCount), hmcur(RUR) {
        /** ����������� � �����������: _PrCount - ���������� �������� �������, _RMCount - ������ ���������� ������� ����� �
        ����������, _RMNames - ������ ������ � ���������� ����� � ����������, msize - ������ ���������� ������������
        ���������. **/
            if(_RMNames && (_RMCount>sZero)) {              // ���� ������ _RMNames ���������� � _RMCount>0, ��
                RMNames = new(nothrow) strNameMeas[RMCount];// �������� ������ ��� ������ �� ������� ������������ ����� � ����������
                if(RMNames)                                 // ���� ������ ��������, ��
                    for(size_t i=sZero; i<RMCount; i++) {   // ��������� ������ ���������� �� ������� _RMNames
                        *(RMNames+i) = *(_RMNames+i);       // ������ ������������ memcpy, �.�. strNameMeas - ��� ��� �� ��������
                    }
                else { RMCount = sZero; };
            } else { RMNames = nullptr; RMCount = sZero; };
            Manuf.reserve(msize); /** ����� ��� ���������� ������ ���������� ������������ � ������ �� ����������� �����������
            ��� ���������� � ������ ��������� � ��� ��������� ������ ���������, ���������� ����� ��������������� ������ ��
            ��� ������������ ������� ���������. https://pro-prof.com/forums/topic/�����������-�������������-vector-�-c **/
            #ifdef CDtor_voice
                cout << "Ctor clsManufactory with parameters" << endl;
            #endif // CDtor_voice
        }   // clsManufactory

        clsManufactory::clsManufactory(const clsManufactory& obj) {
        /** ����������� ����������� **/
            PrCount = obj.PrCount;
            RMCount = obj.RMCount;
            hmcur = obj.hmcur;
            if(obj.RMNames) { RMNames = new(nothrow) strNameMeas[RMCount]; } else RMNames = nullptr;
            if(RMNames) { var_cpy(RMNames, obj.RMNames, RMCount); };
            Manuf = obj.Manuf;
            pshell = obj.pshell;
            #ifdef CDtor_voice
                cout << "Copy clsManufactory Ctor " << endl;
            #endif // CDtor_voice
        }   // Copy Ctor

        void clsManufactory::swap(clsManufactory& obj) noexcept {
        /** ������� ������ ���������� ����� ��������� **/
            std::swap(PrCount, obj.PrCount);    // ����� ����������
            std::swap(RMCount, obj.RMCount);    // ����� ����������
            std::swap(hmcur, obj.hmcur);        // ����� ����������
            std::swap(RMNames, obj.RMNames);    // ����� �����������
            std::swap(pshell, obj.pshell);      // ����� �����������
            Manuf.swap(obj.Manuf);              // ����� ���������
            #ifdef CDtor_voice
                cout << "clsManufactory::swap" << endl;
            #endif // CDtor_voice
        }   // swap

        clsManufactory::clsManufactory(clsManufactory&& obj) {
        /** ����������� ����������� **/
            PrCount = sZero;
            RMCount = sZero;
            hmcur = RUR;
            RMNames = nullptr;
            pshell = nullptr;
            swap(obj);
            #ifdef CDtor_voice
                cout << "Move clsManufactory Ctor" << endl;
            #endif // CDtor_voice
        }   // Move Ctor

        clsManufactory& clsManufactory::operator=(const clsManufactory& obj) {
        /** ���������� ��������� ������������ ������������. ����������� � ������
        �����������-�-������ (copy-and-swap idiom) **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsManufactory tmp(obj);            // �������� ����������� ����������� � �������� ����� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef CDtor_voice
                cout << "clsManufactory Copy operator=" << endl;
            #endif // CDtor_voice
            return *this;
        }   // Copy operator=

        clsManufactory& clsManufactory::operator=(clsManufactory&& obj) {
        /** ���������� ��������� ������������ ������������. ����������� � ������ �����������-�-������
        (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsManufactory tmp(move(obj));      // �������� ����������� ����������� � �������� ���������� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef CDtor_voice
                cout << "clsManufactory Move operator=" << endl;
            #endif // CDtor_voice
            return *this;
        }   // Move operator=

        clsManufactory::~clsManufactory() {
        /** ���������� **/
            if(RMNames) delete[] RMNames;
            #ifdef CDtor_voice
                cout << "Dtor clsManufactory" << endl;
            #endif // CDtor_voice
        }   // ~clsManufactory

        /** Set - ������ **/

        void clsManufactory::Set_progress_shell(clsProgress_shell<type_progress>* val) {
        /** ������� ����������� ��������� pshell ����� ������� val **/
            pshell = val;
        }   // Set_progress_shell

        void clsManufactory::SetCurrency(const Currency& _cur) {
        /** ������������� �������� ������ ������� **/
             hmcur = _cur;
        }   // ������������� �������� ������ �������

        bool clsManufactory::SetManufItem(const string &_name, const string &_measure, const size_t _duration, \
            const size_t _rcount, const strNameMeas _rnames[], const decimal _recipe[], const strItem _pplan[]) {
        /** ����� �������� ������������ ��� ����������� ��������. ������� ����� ��������� ������ clsManufactItem, ��������� ���
        � ������, ������ ������ ����� � ���������� � ��������� � ��������� ������������. ���������: _name - ������������ ��������,
        _measure - ������������ ������� ���������, _duration - ������������ ����������������� �����, _rcount - ����� �������
        ����� � ����������, ����������� � ������������ ������� ��������, _rnames - ������ � �������������� �����, ������������
        rcount, ���������� ������������� ������������ �� ������� ������� � ���������� ����� � ����������, _recipe - ��������� ��
        2D-������ � �����������, �������� rcount*duration, _pplan - ��������� �� ������ �������� PrCount � ������ �������
        ��������. **/
            if( (_duration==sZero) || (_rcount==sZero) || (!_rnames) || (!_recipe) || (!_pplan) )   // ��������� ����������
                return false;
            if(!Checkrnames(_rcount, _rnames)) { return false; }; // ��������� ���������� ���� ����� � ���������� �������� ������
            vector <clsManufactItem>::iterator it;                // �������� ��� �������
            it = find(Manuf.begin(), Manuf.end(), _name);   // ����� ������� �� ����� (���� ���������� �������� ��������)
            if(it != Manuf.end()) { return false; };        // ���� ������ � ����� ������ ��� ����, �� ����� � ������� false
            Manuf.emplace_back(PrCount, _name, _measure, _duration, _rcount, _rnames, _recipe); /** ���� ��� ���������, ��
            ������� ������������ ��� ������� ��������: ������� ��������� ������ clsManufactItem � �������� ������� **/
            Manuf.back().SetProductPlan(_pplan);    // ������ ���� ������� �������� � ������ ��� ����������� ������� �������
            return true;
        }   // SetManufItem

        bool clsManufactory::SetManufItem(const clsRecipeItem &obj, const strItem _pplan[]) {
        /** ����� �������� ������������ ��� ����������� ��������. ������� ����� ��������� ������ clsManufactItem,
        ��������� ��� � ������. ���������: obj - ���������� ��������� ������ clsRecipeItem, _pplan - ���������
        �� ������ �������� PrCount � ������ ������� �������� **/
            if( (!_pplan) ) return false;                                               // ��������� ����������
            if(!Checkrnames(obj.GetRCount(), obj.GetRefRawNamesItem()) )
                return false;  // ��������� ����������
            const string tname = obj.GetName();             // ��������� ���������
            vector <clsManufactItem>::iterator it;          // �������� ��� �������
            it = find(Manuf.begin(), Manuf.end(), tname);   // ����� ������� �� ����� (���� ���������� �������� ��������)
            if(it != Manuf.end()) { return false; };        // ���� ������ � ����� ������ ��� ����, �� ����� � ������� false
            Manuf.emplace_back(PrCount, obj);               /** ���� ��� ���������, �� ������� ������������ ��� �������
            ��������: ������� ��������� ������ clsManufactItem � �������� ������� **/
            Manuf.back().SetProductPlan(_pplan);    // ������ ���� ������� �������� � ������ ��� ����������� ������� �������
            return true;
        }   // SetManufItem

        bool clsManufactory::SetManufItem(clsRecipeItem &&obj, const strItem _pplan[]) {
        /** ����� �������� ������������ ��� ����������� ��������. ������� ����� ��������� ������ clsManufactItem, ��������� ���
        � ������. ���������: obj - ������������ ��������� ������ clsRecipeItem, _pplan - ��������� �� ������ �������� PrCount �
        ������ ������� �������� **/
            if( (!_pplan) ) return false;                                               // ��������� ����������
            if(!Checkrnames(obj.GetRCount(), obj.GetRefRawNamesItem()) ) return false;  // ��������� ����������
            const string tname = obj.GetName();             // ��������� ���������
            vector <clsManufactItem>::iterator it;          // �������� ��� �������
            it = find(Manuf.begin(), Manuf.end(), tname);   // ����� ������� �� ����� (���� ���������� �������� ��������)
            if(it != Manuf.end()) { return false; };        // ���� ������ � ����� ������ ��� ����, �� ����� � ������� false
            Manuf.emplace_back(PrCount, move(obj));         /** ���� ��� ���������, �� ������� ������������ ��� �������
            ��������: ������� ��������� ������ clsManufactItem � �������� ������� **/
            Manuf.back().SetProductPlan(_pplan);    // ������ ���� ������� �������� � ������ ��� ����������� ������� �������
            return true;
        }   // SetManufItem

        bool clsManufactory::SetManufItem(const clsManufactItem &obj) {
        /** ����� �������� ������������ ��� ����������� ��������. ������� ����� ��������� ������ clsManufactItem,
        ��������� ��� � ������. ���������: obj - ���������� ��������� ������ clsManufactItem. ������ 2024.08.10 **/
            const string tname = *(obj.GetName());          // ��������� ���������
            vector <clsManufactItem>::iterator it;          // �������� ��� �������
            it = find(Manuf.begin(), Manuf.end(), tname);   // ����� ������� �� ����� (���� ���������� �������� ��������)
            if(it != Manuf.end()) { return false; };        // ���� ������ � ����� ������ ��� ����, �� ����� � ������� false
            Manuf.emplace_back(obj);                        /** ���� ��� ���������, �� ������� ������������ ��� �������
            ��������: ������� ��������� ������ clsManufactItem � �������� ������� ������������ obj **/
            return true;
        }   // SetManufItem

        bool clsManufactory::SetManufItem(clsManufactItem &&obj) {
        /** ����� �������� ������������ ��� ����������� ��������. ������� ����� ��������� ������ clsManufactItem,
        ��������� ��� � ������. ���������: obj - ������������ ��������� ������ clsManufactItem. ������ 2024.08.10 **/
            const string tname = *(obj.GetName());          // ��������� ���������
            vector <clsManufactItem>::iterator it;          // �������� ��� �������
            it = find(Manuf.begin(), Manuf.end(), tname);   // ����� ������� �� ����� (���� ���������� �������� ��������)
            if(it != Manuf.end()) { return false; };        // ���� ������ � ����� ������ ��� ����, �� ����� � ������� false
            Manuf.emplace_back(move(obj));                  /** ���� ��� ���������, �� ������� ������������ ��� �������
            ��������: ������� ��������� ������ clsManufactItem � �������� ������� ������������ obj **/
            return true;
        }   // SetManufItem

        bool clsManufactory::SetProdPlan(const strItem _ProdPlan[]) {
            /** ����� ������ ���� ������� ��������� � ������������. ���������: _ProdPlan - ��������� �� ������ ������
            � ������ ������� ���� ���������, �������� Manuf.size()*PrCount **/
            if( (PrCount == sZero) || (RMCount == sZero) || (!RMNames) || (Manuf.size() == sZero) ) return false;
            vector<clsManufactItem>::iterator it;           // �������� ��� �������
            size_t i = sZero;                               // ��������������� �������
            for(it=Manuf.begin(); it!=Manuf.end(); it++) {  // ��� ������� �������� �������
                it->SetProductPlan(_ProdPlan+PrCount*i);    // ��������� ��������������� ������ ����� ������
                i++;
            };
            return true;
        }   // SetProdPlan

        /** ��������� ������ **/

        bool clsManufactory::IncreaseCapacity(size_t msize) {
        /** ����� ����������� ������ ������� ��� ����������� ���������� ����� ���������. ������ ��� ����� �������� �����������
        � �������� ���� ��������� �������, ��� ������ ��������. ����� �� ������ ������ ������� � ������� ��� �������������. **/
            size_t current_capacity = Manuf.capacity();     // �������� ������� ����� ������
            if(msize>current_capacity) {                    // ���� ����� ����� ������ ��������, ��
                Manuf.reserve(msize);                       // ����������� ������
                return true;                                // � ���������� true
            } else return false;                            // ���� ���, - ������ �� ������ � ������� � false
        }   // IncreaseCapacity

        /** �������������� ������. ���������� � ����������� ������ **/

        void clsManufactory::CalcRawMatPurchPlan(size_t bg, size_t en) {
        /** ����� ������������ ����� ����������� �������� � ����������� ��������� � ������������ � ������
            ������� ��� ������� �������� � ���������: �� �������� � �������� bg �� �������� � �������� en-1. **/
            size_t lim = Manuf.size();                  // ������� ������� ���������� �������� �������
            if(bg>=lim) return;                         // ��������� ������ ������� �������
            if(en>=lim) en = lim;                       // ��������� ������� ������� �������
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // ���������� �������
            for(size_t i = bg; i<en; i++) {
                clsManufactItem* p = (Manuf.data()+i);  // ��������������� ��������� ������������� �� ������� �������
                if(p->GetPrCount() != PrCount)          // ���� ������������ ������� �� ���������, ��
                    p->Resize(PrCount);                 // �������� ������� � ������������
                p->CalcRawMatPurchPlan();               // ��������� ������ ��� ������� �������� �������
                (pshell) ? pshell->Counter_inc() : (void)([](){return;});           // �������� �������
            }
        }   // CalcRawMatPurchPlan(size_t bg, size_t en)

        void clsManufactory::CalcRawMatPurchPlan() {
        /** ����� ������������ ����� ����������� ����� � ���������� � ����������� ��������� ��� ����� �����
        ������� ���� ���������. **/
            size_t lim = Manuf.size();                  // ������� ������� ���������� �������� �������
            for(size_t i{}; i<lim; i++) {
                clsManufactItem* p = (Manuf.data()+i);  // ��������������� ��������� ������������� �� ������� �������
                if(p->GetPrCount() != PrCount)          // ���� ������������ ������� �� ���������, ��
                    p->Resize(PrCount);                 // �������� ������� � ������������
                p->CalcRawMatPurchPlan();               // ��������� ������ ��� ������� �������� �������
                (pshell) ? pshell->Update((int)i) : (void)([](){return;});          // �������� ��������� ���������
            }
            (pshell) ? pshell->Update((int)lim) : (void)([](){return;});   // ��������� �� 100%
        }   // CalcRawMatPurchPlan

        void clsManufactory::CalcRawMatPurchPlan_future() {
        /** ����� ������������ ����� ����������� ����� � ���������� � ����������� ��������� ��� ����� �����
        ������� ���� ���������. ��� ������� ����������� � ����������� �������, ����� ������� �� �������
        ������ ����� ���� ����������. **/
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // ����������� ����. ����� ������� ����� ����� ����
            if(maxthreads < sTwo) {                                         // ���� ����. ����� ������� ����� ����, ��
                CalcRawMatPurchPlan();                                      // �������� ���������� ������� CalcRawMatPurchPlan()
                return;                                                     // � �������
            };
            vector<std::future<void>> pool;             // ��� ������� � ���� �������
            pool.reserve(maxthreads);                   // ����������� ������ ������� �������
            size_t stocksize = Manuf.size();            // �������� ������ ������� � ���������� �������������
            size_t ncap = (stocksize/maxthreads)+sOne;  // ���������� ��.����������� ��� ������� ������
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // ���������� �������
            for(size_t i{}; i<maxthreads; i++) {        // ���� �� ����� ���� �������
                size_t bg = i*ncap;                     // ���������� ������ ������� �������
                if(bg>=stocksize) break;                // ���� ������ ������� ������ ��� ����� ����� ��.��-�, ������� �� �����
                size_t en = (i+sOne)*ncap;              // ���������� ������� ������� �������
                pool.push_back(std::async([](clsManufactory* pobj, size_t _bg, size_t _en) {    // ������� ����� � ��������� ����������
                    #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                        mu1.lock();                          // ��������� ������ cout ��� ������������ �������������
                        cout << "Async thread #" << std::this_thread::get_id() << " has been started" << endl;
                        mu1.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                    #endif
                    pobj->CalcRawMatPurchPlan(_bg, _en);    // �������� ����� � ������������ � �������
                    #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                        mu1.lock();                          // ��������� ������ cout ��� ������������ �������������
                        cout << "Async thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                        mu1.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                    #endif
                }, this, bg, en));                          // ����� ������ ����� ��������� ��� ���: this, bg, en
            };
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;});   // �������� ��������� ����������
        }   // CalcRawMatPurchPlan_future

        void clsManufactory::CalcRawMatPurchPlan_thread() {
        /** ����� ������������ ����� ����������� ����� � ���������� � ����������� ��������� ��� ����� �����
        ������� ���� ���������. ��� ������� ����������� � �������, ����� ������� �� �������
        ������ ����� ���� ����������. **/
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // ����������� ����. ����� ������� ����� ����� ����
            if(maxthreads < sTwo) {                                         // ���� ����. ����� ������� ����� ����, ��
                CalcRawMatPurchPlan();                                      // �������� ���������� ������� CalcRawMatPurchPlan()
                return;                                                     // � �������
            };
            vector<std::thread> pool;                   // ��� ������� � ���� �������
            pool.reserve(maxthreads);                   // ����������� ������ ������� �������
            size_t stocksize = Manuf.size();            // �������� ������ ������� � ���������� �������������
            size_t ncap = (stocksize/maxthreads)+sOne;  // ���������� ��.����������� ��� ������� ������
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // ���������� �������
            for(size_t i{}; i<maxthreads; i++) {        // ���� �� ����� ���� �������
                size_t bg = i*ncap;                     // ���������� ������ ������� �������
                if(bg>=stocksize) break;                // ���� ������ ������� ������ ��� ����� ����� ��.��-�, ������� �� �����
                size_t en = (i+sOne)*ncap;              // ���������� ������� ������� �������
                pool.emplace_back([](clsManufactory* pobj, size_t _bg, size_t _en){ // ������� ����� � ��������� ����������
                    #ifdef DEBUG_THREADS                // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                        mu1.lock();                      // ��������� ������ cout ��� ������������ �������������
                        cout << "Thread #" << std::this_thread::get_id() << " has been started" << endl;
                        mu1.unlock();                    // ������������ ������ cout ��� ������������� ������� ���������
                    #endif
                    pobj->CalcRawMatPurchPlan(_bg, _en);// �������� ����� � ������������ � �������
                    #ifdef DEBUG_THREADS                // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                        mu1.lock();                      // ��������� ������ cout ��� ������������ �������������
                        cout << "Thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                        mu1.unlock();                    // ������������ ������ cout ��� ������������� ������� ���������
                    #endif
                }, this, bg, en);                       // ����� ������ ����� ��������� ��� ���: this, bg, en
            };
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;}); // �������� ��������� ����������
            for(auto &t : pool)                         // ���� �� ����� ���� �������
                t.join();                               // ������� ���������� ������� ����������� ������
        }   // CalcRawMatPurchPlan_thread

        decimal* clsManufactory::GetRawMatPurchPlan() const {
        /** �������������� ����� ���������� ��������� �� ����� ����������� ������ �������� RMCount*PrCount, � ������� ����������
        ����������� ��� ������� ������������ ����� � ������ ������� �������. ����������� � ����� ���������, �� ���� ���������
        ������������� ���� ���������. ����� const. **/
            if(Manuf.size() == sZero) { return nullptr; };              // ���� � ������� ��� ���������
            decimal *Rtemp = new(nothrow) decimal[RMCount*PrCount]{};   // ������� ������ ��� ��������������� ������ � �������� ���
            if(!Rtemp) { return nullptr; };                             // ���� ������ �� ��������, �� ����� � ������� nullptr
            for(vector<clsManufactItem>::const_iterator cit = Manuf.cbegin(); cit < Manuf.cend(); cit++) {
                const decimal *rtemp = cit->GetRawMatPurchPlan();       // �������� ��������� �� ������ � ������ �������
                if(rtemp) {
                    const size_t _rcount = cit->GetRCount();            // ������ ����� ����� � ����������
                    const strNameMeas *rnames = cit->GetRefRawNames();  // �������� ������ � ������������� ����� � ����������
                    if(rnames) {
                        for(size_t j=sZero; j<_rcount; j++) {
                            for(size_t k=sZero; k<RMCount; k++) {
                                if( (rnames+j)->name == (RMNames+k)->name ) {  // ���������� �����, ���� ��� ���������, ��
                                    // ���������� � k-������ ������� Rtemp j-� ������ ������� rtemp
                                    Sum((Rtemp+PrCount*k), (rtemp+PrCount*j), PrCount);
                                    break;
                                }
                            }   // for k
                        }   // for j
                    }   // if(rnames)
                }   // if(rtemp)
            }   // ���� �� ���� ��������� �������
            return Rtemp;
        }   //  GetRawMatPurchPlan

        strItem* clsManufactory::GetRMPurchPlan() const {
        /** �������������� ����� ���������� ��������� �� ����� ����������� ������ �������� RMCount*PrCount, � �������
        ���������� ����������� ��� ������� ������������ ����� � ������ ������� �������. ����������� � ����� ���������,
        �� ���� ��������� ������������� ���� ���������. ��� ������� - strItem, ��������� ������� ������ ���� volume;
        ���� price � value �������. **/
            decimal* temp = GetRawMatPurchPlan();       // �������� ��������� �� ����������� ������ � ������ �������
            if(!temp) return nullptr;                   // ���� ������ �� ���������, �� ���������� nullptr
            strItem *Rtemp = new(nothrow) strItem[RMCount*PrCount];     // ������� ������ ��� ��������������� ������
            if(!Rtemp) {                                                // ���� ������ �� ��������, ��
                delete[] temp;                                          // ������� ��������������� ������ � ������ �������
                return nullptr;                                         // ������� � ���������� nullptr
            };
            for(size_t i{}; i<(RMCount*PrCount); i++) {                 // �������� ��� ������� ������� temp
                (Rtemp+i)->volume = *(temp+i);                          // � ���� ������� Rtemp
                (Rtemp+i)->price = dZero;                               // ��������� ����
                (Rtemp+i)->value = dZero;                               // ��������
            };
            delete[] temp;                              // ������� ��������������� ������ � ������ �������
            return Rtemp;                               // � ���������� ��������� �� strItem-������ � ������ �������
        }   // GetRMPurchPlan

        bool clsManufactory::SetRawMatPrice(const decimal _Price[]) {
        /** ����� ����� ��� �� ����� � ���������. ��������������, ��� ����� ��������� ������� ���������� �
        ����������� � ����� � ����������, ����� ���������� ���������� �� ������� ����� �� ����� � ���������.
        ������ ����� �������� ����������� ������ ������� �������� �������, ������� ���������� ���������� � �����
        � ������� RawMatPrice ������� ���������� ������������. ���������: _Price - ������� ��� �� ����� � ���������
        �������� RMCount*PrCount. **/
            if(!_Price) return false;                                   // ��������� �������� �������
            if(Manuf.size() == sZero) return false;                     // �������� ������� �� ���������� ���������
            bool check = true;                                          // ��������� ���������� ��������
            for(vector<clsManufactItem>::iterator it = Manuf.begin(); it!=Manuf.end(); it++) { // ���� �� ��������� �������
                const size_t _rcount = it->GetRCount();                 // ������ ����� ����� � ���������� � �������� �������
                decimal *rtemp =  new(nothrow) decimal[_rcount*PrCount];// ������� ������ ��� ����� ��� � ������� �������
                if(rtemp) {                                             // ���� ������ ������, ��
                    const strNameMeas *rnames = it->GetRefRawNames();   // �������� ����������� ��������� �� ���������� ������
                                                                        // � ������������� ����� � ���������� ��������
                    if(rnames) {                                        // ���� ��������� �� ����, ��
                        for(size_t j=sZero; j<_rcount; j++) {           // ��������� ������ ��� ����� ��� ������ ��� �������
                            for(size_t k=sZero; k<RMCount; k++) {       // �����, ������� ������������ � ��������� ��������
                                if( (rnames+j)->name == (RMNames+k)->name ) { // ���������� �����, ���� ��� ���������, ��
                                    // �������� k-������ � ������ �� ������ ������� ��� � j-������ ���������� ������ ���
                                    var_cpy((rtemp+PrCount*j), (_Price+PrCount*k), PrCount);
                                    break;                              // � �������� ��������� ���� �� k
                                }
                            }   // for k
                        }   // for j
                        check = check && it->MoveRawMatPrice(rtemp); // ���������� ��������� ������ ��� � ������� �������
                        // � �������� ��������� ���������� ��������
                    };  // ���� ������������ ������� clsManufactItem::SetRawMatPrice, �� � ��������� ������� ����������
                        // ������� ��������� ������ rtemp � ��������� ��� �������� Nullptr
                        // delete[] rtemp;
                        // rtemp = nullptr;
                };  // if(rtemp)
            };
            return check;   // ���� ���� ���� ������� �� �������� ������, �� check==false
        }   // SetRawMatPrice

        bool clsManufactory::Calculate(size_t bg, size_t en) {
        /** ����� ������������ �����, �������� � ������ ������������� �������������� ������������ � ������� ���������
        ��� ��������� � ��������� �� bg �� en-1, ����������� �� ���������� ����� �������. ����� ���������� ��������
        ������ CalculateItem ��� ������� ���������� ������������; ��������� ������� ��������� � ������� ��������������
        ������������ ��� ������� �������� �� ��������� ��������� **/
            size_t lim = Manuf.size();                      // ������� ������� ���������� �������� �������
            if(lim == sZero) return false;                  // �������� ������� �� ���������� ���������
            if(bg>=lim) return false;                       // ��������� ������ ������� �������
            if(en>=lim) en = lim;                           // ��������� ������� ������� �������
            bool check = true;                              // ��������� ���������� ��������
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // ���������� �������
            for(size_t i = bg; i<en; i++)  {                 // ���� �� ���� ��������� �������������
                check = check && (Manuf.data()+i)->CalculateItem(); // ������ ��������. ��-�� � ������� ��������� � i-��������
                (pshell) ? pshell->Counter_inc() : (void)([](){return;});           // �������� �������
            }
            return check;
        }   // Calculate(size_t, size_t)

        bool clsManufactory::Calculate() {
        /** ����� ������������ �����, �������� � ������ ������������� �������������� ������������ � ������� ���������
        ��� ���� ���������, ����������� �� ���������� ����� �������. ����� ���������� �������� ������ CalculateItem ���
        ������� ���������� ������������; ��������� ������� ��������� � ������� �������������� ������������ ��� �������
        �������� **/
            size_t lim = Manuf.size();                      // ������� ������� ���������� �������� �������
            bool check = true;                              // ��������� ���������� ��������
            for(size_t i{}; i<lim; i++)  {                  // ���� �� ���� ��������� �������������
                check = check && (Manuf.data()+i)->CalculateItem(); // ������ ��������. ��-�� � ������� ��������� � i-��������
                (pshell) ? pshell->Update((int)i) : (void)([](){return;});  // �������� ��������� ���������
            }
            (pshell) ? pshell->Update((int)lim) : (void)([](){return;});    // ��������� �� 100%
            return check;
        }   // Calculate

        bool clsManufactory::Calculate_future() {
        /** ����� ������������ �����, �������� � ������ ������������� �������������� ������������ � ������� ���������
        ��� ���� ���������, ����������� �� ���������� ����� �������. ����� ���������� �������� ������ CalculateItem ���
        ������� ���������� ������������; ��������� ������� ��������� � ������� �������������� ������������ ��� �������
        ��������. ��� ������� ����������� � ����������� �������, ����� ������� �� ������� ������ ����� ���� ����������. **/
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // ����������� ����. ����� ������� ����� ����� ����
            if(maxthreads < sTwo)                                           // ���� ����. ����� ������� ����� ����, ��
                return Calculate();                                         // �������� ���������� ������� Calculate()
            vector<std::future<bool>> pool;             // ��� ������� � ���� �������
            pool.reserve(maxthreads);                   // ����������� ������ ������� �������
            size_t stocksize = Manuf.size();            // �������� ������ ������� � ���������� �������������
            size_t ncap = (stocksize/maxthreads)+sOne;  // ���������� ��.����������� ��� ������� ������
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // ���������� �������
            for(size_t i{}; i<maxthreads; i++) {        // ���� �� ����� ���� �������
                size_t bg = i*ncap;                     // ���������� ������ ������� �������
                if(bg>=stocksize) break;                // ���� ������ ������� ������ ��� ����� ����� ��.��-�, ������� �� �����
                size_t en = (i+sOne)*ncap;              // ���������� ������� ������� �������
                pool.push_back(std::async([](clsManufactory* pobj, size_t _bg, size_t _en) {
                    #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                        mu1.lock();                          // ��������� ������ cout ��� ������������ �������������
                        cout << "Async thread #" << std::this_thread::get_id() << " has been started" << endl;
                        mu1.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                    #endif
                    bool res = pobj->Calculate(_bg, _en);                   // �������� ����� � ������������,
                    #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                        mu1.lock();                          // ��������� ������ cout ��� ������������ �������������
                        cout << "Async thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                        mu1.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                    #endif
                    return res;                                             // ������� � ���������� ���������
                }, this, bg, en));
            };                                          // ���� �� ����� ���� �������
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;});   // �������� ��������� ����������
            bool check = true;                          // ��������� ���������� ��������
            for(auto &t : pool) {                       // ���� �� ����� ���� �������
                check = check && t.get();               // ��������� ��������� ����������
                if(!check) break;                       // ���� ��������� ���������� false, �� ��������� ����
            };
            return check;
        }   // Calculate_future

        bool clsManufactory::Calculate_thread() {
        /** ����� ������������ �����, �������� � ������ ������������� �������������� ������������ � ������� ���������
        ��� ���� ���������, ����������� �� ���������� ����� �������. ����� ���������� �������� ������ CalculateItem ���
        ������� ���������� ������������; ��������� ������� ��������� � ������� �������������� ������������ ��� �������
        ��������. ��� ������� ����������� � �������, ����� ������� �� ������� ������ ����� ���� ����������. **/
            auto do_Calculate = [](void *param, size_t bg, size_t en, bool &res) {
            /** "�������" � ���� ������-������� ��� ������ bool Calculate(size_t, size_t), ��������������� ��� �������
            ���������� ������ � ������ tread. **/
                clsManufactory* pobj = (clsManufactory*)param;  // ��������������� ��������� �� ��������� ������ clsManufactory
                #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                    mu1.lock();                          // ��������� ������ cout ��� ������������ �������������
                    cout << "Thread #" << std::this_thread::get_id() << " has been started" << endl;
                    mu1.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                #endif
                res = pobj->Calculate(bg, en);          // ���������� ����������
                #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                    mu1.lock();                          // ��������� ������ cout ��� ������������ �������������
                    cout << "Thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                    mu1.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                #endif
            };  // do_Calculate
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // ����������� ����. ����� ������� ����� ����� ����
            if(maxthreads < sTwo)                                           // ���� ����. ����� ������� ����� ����, ��
                return Calculate();                                         // �������� ���������� ������� Calculate � �������
            vector<std::thread> pool;                   // ��� ������� � ���� �������
            pool.reserve(maxthreads);                   // ����������� ������ ������� �������
            size_t stocksize = Manuf.size();            // �������� ������ ������� � SKU
            size_t ncap = (stocksize/maxthreads)+sOne;  // ���������� SKU ��� ������� ������
            bool Res[maxthreads];                       // ��������������� ������ � ������������ ����������
            for(size_t k{}; k<maxthreads; k++)          // ������������� ���������������� ������� ���������� true (��� ������ ��
                *(Res+k) = true;                        // ����� �� break �������� � ��������� �� ���������� ���� �.�. true
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // ���������� �������
            for(size_t i{}; i<maxthreads; i++) {        // ���� �� ����� ���� �������
                size_t bg = i*ncap;                     // ���������� ������ ������� �������
                if(bg>=stocksize) break;                // ���� ������ ������� ������ ��� ����� ����� SKU, ������� �� �����
                size_t en = (i+sOne)*ncap;              // ���������� ������� ������� �������
                pool.emplace_back(do_Calculate, this, bg, en, std::ref(*(Res+i)));  // ������� ����� � ��������� ����������
            };  // ��� �������� �������� �� ������ ���������� �������� ������ std::ref(...), ��.:
                // https://pro--prof-com.turbopages.org/pro-prof.com/s/forums/topic/cplusplus_reference_wrapper
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;}); // �������� ��������� ����������
            for(auto &t : pool)                         // ���� �� ����� ���� �������
                t.join();                               // ������� ���������� ������� ����������� ������
            for(size_t i{}; i<maxthreads; i++ )         // ���� �� ���� ��������� ���������������� ������� � ������������
                if(!(*(Res+i)))                         // ���� ���� ���� �� �������� ������ false, ��
                    return *(Res+i);                    // ���������� false
            return true;
        }   // Calculate_thread

        /** Get - ������ **/

        const size_t clsManufactory::GetRMCount() const { return RMCount; }         // ����� ���������� ����� ����� ������� ����� � ����������
        const size_t clsManufactory::GetPrCount() const { return PrCount; }         // ����� ���������� ����� �������� �������
        const size_t clsManufactory::GetProdCount() const { return Manuf.size(); }  // ���������� ����� ���������

        const string clsManufactory::GetCurrency() const {
        /** ���������� �������� ������ ������� � ���� ��������� ������ **/
            return *(nmBPTypes::CurrencyTXT+hmcur);
        }; // GetCurrency

        strNameMeas* clsManufactory::GetProductDescription() const {
        /** ����� ���������� ��������� �� ����� ����������� ������ � ������� � ��.��������� ���� ���������. **/
            strNameMeas *temp = new(nothrow) strNameMeas[Manuf.size()]; // �������� ������ �������
            if(!temp) { return nullptr; };
            size_t i = sZero;
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++) {
                /** ���� ��������� �� ����� nullptr, �� ����������� ����� � ��. �����������, ���� �����, �� ����������� "" **/
                (temp+i)->name    = (it->GetName())    != nullptr ? *(it->GetName())    : "";
                (temp+i)->measure = (it->GetMeasure()) != nullptr ? *(it->GetMeasure()) : "";
                i++;
            };
            return temp;
        }   // GetProductDescription

        const strNameMeas* clsManufactory::GetRMNames() const {
        /** ����� ���������� const-��������� �� ���������� ������ � ������� � ��. ��������� ����� � ����������. **/
            return RMNames;
        }   // GetRMNames

        strNameMeas* clsManufactory::GetRawMatDescription() const {
        /** ����� ���������� ��������� �� ����� ����������� ������ � ������� � ��. ��������� ����� � ����������. **/
            strNameMeas *temp = new(nothrow) strNameMeas[RMCount];  // // �������� ������ �������
            if(!temp) { return nullptr; };
            var_cpy(temp, RMNames, RMCount);
            return temp;
        }   // GetRawMatDescription

        strItem* clsManufactory::GetTotalBalance() const {
        /** ����� ���������� ��������� �� ���������� ������, ���������� �������� ��������� ������� � ��������� ��������������
        ������������ ��� ���� ���������. ������ ������� Manuf.size()*PrCount. ������ ������� ������� ����� ��� strItem, �.�.
        ����� � ����� ������� �������� volume, price � value ��� �������������� ������������. ������ ��������� � ������������
        ������, ����� ������������� ������� ������ �������� � ������� ��������� delete[]. **/
            return gettotal(&clsManufactItem::GetBalance);
        }   // GetTotalBalance

        strItem* clsManufactory::GetTotalProduct() const {
        /** ����� ��������� ��������� �� ���������� ������, ���������� �������� ��������� ������� � ������� ������ ����
        ��������� � �����������, �������� � ������ ����������� ���������. ������ ������� Manuf.size()*PrCount. ������ �������
        ������� ����� ��� strItem, �.�. ����� � ����� ������� �������� volume, price � value ��� ��������. ������ ���������
        � ������������ ������, ����� ������������� ������� ������ �������� � ������� ��������� delete[].**/
            return gettotal(&clsManufactItem::GetProductPlan);
        }   // GetTotalProduct

        const decimal* clsManufactory::GetRecipeItem(const string& Name) const {
        /** ����� ���������� ����������� ��������� �� ���������� ������ � ����������� ��� ������������ �������� � ������ Name **/
            vector <clsManufactItem>::const_iterator it;            // �������� ��� �������
            it = find(Manuf.cbegin(), Manuf.cend(), Name);          // ����� ������� �� �����
            if(it !=Manuf.cend()) { return it->GetRefRecipe(); }    // ���� ������ � ����� ������ ����, �� ����� � ������� ������
            return nullptr;                                         // ���� ������� � ����� ������ ���, �� ����� � ������� nullptr
        }   // GetRecipeItem

        const decimal* clsManufactory::GetRecipeItem(const size_t _ind) const {
        /** ����� ���������� ����������� ��������� �� ���������� ������ � ����������� ��� ������������ �������� �
        �������� _ind **/
            if(_ind < Manuf.size()) {                       // ���� ������ � ���������� ���������, ��
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // ������������� �������� �� �������
                return it->GetRefRecipe();                  // � �������� ��������� �� ������ � ���������� ��� ��������
            }                                               // � �������� _ind
            else return nullptr;                            // ����� ����� � ������� nullptr
        }   // GetRecipeItem

        const strNameMeas* clsManufactory::GetRawNamesItem(const string& Name) const {
        /** ����� ���������� ����������� ��������� �� ���������� ������ � �������������� ����� � ���������� � ��������� �������� � ������ Name **/
            vector <clsManufactItem>::const_iterator it;            // �������� ��� �������
            it = find(Manuf.cbegin(), Manuf.cend(), Name);          // ����� ������� �� �����
            if(it !=Manuf.cend()) { return it->GetRefRawNames(); }  // ���� ������ � ����� ������ ����, �� ����� � ������� ������
            return nullptr;                                         // ���� ������� � ����� ������ ���, �� ����� � ������� nullptr
        }   // GetRawNamesItem

        const strNameMeas* clsManufactory::GetRawNamesItem(const size_t _ind) const {
        /** ����� ���������� ����������� ��������� �� ���������� ������ � �������������� ����� � ���������� � ��������� �������� �
        �������� _ind **/
            if(_ind < Manuf.size()) {                       // ���� ������ � ���������� ���������, ��
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // ������������� �������� �� �������
                return it->GetRefRawNames();                // � �������� ��������� �� ������ �������������� ����� � ����������
            }                                               // ��� �������� � �������� _ind
            else return nullptr;                            // ����� ����� � ������� nullptr
        }   // GetRawNamesItem

        const size_t clsManufactory::GetDuration(const string& Name) const {
        /** ����� ���������� ������������ ����������������� ����� � ��������� �������� � ������ Name  **/
            vector <clsManufactItem>::const_iterator it;            // �������� ��� �������
            it = find(Manuf.cbegin(), Manuf.cend(), Name);          // ����� ������� �� �����
            if(it !=Manuf.cend()) { return it->GetDuration(); }     // ���� ������ � ����� ������ ����, �� ����� � ������� ������������
            return PrCount+sOne;                                    // ����� ����� � ������� �������� ������������� �����
        }   // GetDuration

        const size_t clsManufactory::GetDuration(const size_t _ind) const {
        /** ����� ���������� ������������ ����������������� ����� � ��������� �������� � �������� _ind **/
            if(_ind < Manuf.size()) {                       // ���� ������ � ���������� ���������, ��
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // ������������� �������� �� �������
                return it->GetDuration();                   // � �������� ������������ ����������������� ����� ��� ��������
            }                                               // � �������� _ind
            else return PrCount+sOne;                       // ����� ����� � ������� ����� ��������, ��� ���������� �������� �������
        }   // GetDuration

        const size_t clsManufactory::GetRCount(const string& Name) const {
        /** ����� ���������� ����� ������� ����� � ����������, ����������� � ��������� �������� � ������ Name
        (������ ������� � �������������� �����) **/
            vector <clsManufactItem>::const_iterator it;            // �������� ��� �������
            it = find(Manuf.cbegin(), Manuf.cend(), Name);          // ����� ������� �� �����
            if(it !=Manuf.cend()) { return it->GetRCount(); }       // ���� ������ � ����� ������ ����, �� ����� � ������� �����
            return RMCount+sOne;                                    // ����� ����� � ������� ����� ��������, ��� ������ ���������� �����
        }   // GetRCount

        const size_t clsManufactory::GetRCount(const size_t _ind) const {
        /** ����� ���������� ����� ������� ����� � ����������, ����������� � ��������� �������� � �������� _ind **/
            if(_ind < Manuf.size()) {                       // ���� ������ � ���������� ���������, ��
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // ������������� �������� �� �������
                return it->GetRCount();                     // � �������� ����� ������� ����� � ���������� ��� ��������
            }                                               // � �������� _ind
            else return RMCount+sOne;                       // ����� ����� � ������� ����� ��������, ��� ������ ���������� �����
        }   // GetRCount

        const strNameMeas clsManufactory::GetNameItem(const size_t _ind) const {
        /** ����� ���������� ������������ �������� � ������� ��� ��������� ��� �������� � � �������� _ind **/
            strNameMeas NameMeas;                           // ��������������� ����������
            NameMeas.name = EmpStr;                         // �������������� �� ������� ��������
            NameMeas.measure = EmpStr;
            if(_ind < Manuf.size()) {                       // ���� ������ � ���������� ���������, ��
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // ������������� �������� �� �������,
                NameMeas.name = *(it->GetName());           // �������� ������������ ��� �������� � �������� _ind
                NameMeas.measure = *(it->GetMeasure());     // �������� ������� ��������� ��� �������� � �������� _ind
            };
            return NameMeas;                                // ������� � ���������� ����������
        }   // GetNameItem

/** --------------------------------- ������ ������������ � �������������� clsManufactory ---------------------------------- **/


        bool clsManufactory::StF(ofstream &_outF)  {
        /** ����� ������������� ������ � �������� ���������� �������� ���������� ������ clsManufactory (����� ������������).
        ���������: &_outF - ��������� ������ ofstream ��� ������ ������ **/
            size_t manufsize = Manuf.size();            // ���������� ��������� �������
            size_t capac= Manuf.capacity();             // ������ ���������� ������ ��� �������; ������������ ��� ��������������
            bool flagRMNames;                           // ���� ������������� �������
            #ifdef CDtor_voice
                long bpos = _outF.tellp();                                      // ���������� ������� � ������ �����
                cout << "clsManufactory::StF begin bpos= " << bpos << endl;     // ������� ������� �� �����
            #endif // CDtor_voice
            if(!SEF(_outF, manufsize)) return false;    // ��������� � ���� ���������� ��������� �������
            if(!SEF(_outF, capac)) return false;        // ��������� � ���� ������� �������
            if(!SEF(_outF, PrCount)) return false;      // ��������� � ���� ���������� �������� �������
            if(!SEF(_outF, RMCount)) return false;      // ��������� � ���� ���������� ������� ����� � ����������
            if(!SEF(_outF, hmcur)) return false;        // ��������� � ���� �������� ������
            if(RMNames) flagRMNames = true; else flagRMNames = false;           // ������������� ����
            #ifdef CDtor_voice
                cout << "flagRMNames= " << flagRMNames << endl;
            #endif // CDtor_voice
            if(!SEF(_outF, flagRMNames)) return false;                          // ��������� ���� � ����
            if(flagRMNames)                                                     // ���� ������ ����������, ��
                for(size_t i=sZero; i<RMCount; i++) {                           // ��������� ������ ������
                    if(!SEF(_outF, (RMNames+i)->name)) return false;            // � ���������� ����� �
                    if(!SEF(_outF, (RMNames+i)->measure)) return false;         // ��������� ���������
                };
            if(manufsize == sZero) return true;         // ���� ������ ������, �� ������������ �������������
            for(vector<clsManufactItem>::iterator it = Manuf.begin(); it!=Manuf.end(); it++) {
                if(!it->StF(_outF)) return false;       // ��������� ������ �� ����������� ���� clsManufatcItem
            }
            #ifdef CDtor_voice
                long epos = _outF.tellp();                  // ���������� ������� � ������ �����
                cout << "clsManufactory::StF end epos= " << epos << endl;
            #endif // CDtor_voice
            return true;
        }   // StF

        bool clsManufactory::SaveToFile(const string _filename) {
        /** ����� ������ �������� ���������� ������ clsManufactory � ���� **/
            if(_filename == EmpStr) return false;               // ���� ��� ����� ������, �� ����� � ������� false
                                                                // 1.   ������� ��������� ������ ofstream
            ofstream outF(_filename, ios::out | ios::binary);   // ��������� ��� ����� � �������� ������� ��� ������ �� ����
                                                                // 2.   ���������, ������� �� ������ ����,
            if (!outF.is_open()) {                              // ���� ���� �� ������, �� ��������� ������������ �
                cout << "������ �������� �����" <<endl;         // ������� false � ����� �� �������
                return false;
            };
            if(!StF(outF)) {                                    // 3.   ���������� ������ � ����. ���� ���� �� �������, ��
                outF.close();                                   // �������� ���� �
                return false;                                   // ������� � false
            };
            #ifdef CDtor_voice
                long epos = outF.tellp();
                cout << "clsManufactory::SaveToFile end epos= " << epos << endl;
            #endif // CDtor_voice
            outF.close();                                       // 4.   ��������� ����
            return true;                                        // 5.   ���������� true
        }   // SaveToFile

        bool clsManufactory::RfF(ifstream &_inF) {
        /** ����� ������������� ������ �� �������� ���������� �������� ���������� ������ clsManufactory (����� ��������������).
        ���������: &_inF - ��������� ������ ifstream ��� ������ ������ **/
            size_t manufsize;                           // ������ ������� (����� ���������)
            size_t capac;                               // ������ ���������� ������ ��� �������; ������������ ��� ��������������
            bool flagRMNames;                           // ���� ������������� �������
            #ifdef CDtor_voice
                long bpos = _inF.tellg();                                   // ���������� ������� � ������ �����
                cout << "clsManufactory::RfF begin bpos= " << bpos << endl; // ������� ������� �� �����
            #endif // CDtor_voice
            if(!DSF(_inF, manufsize)) return false;     // ������ �� ����� ���������� ��������� �������
            if(!DSF(_inF, capac)) return false;         // ������ �� ����� ������� �������
            if(!DSF(_inF, PrCount)) return false;       // ������ �� ����� ���������� �������� �������
            if(!DSF(_inF, RMCount)) return false;       // ������ �� ����� ���������� ������� ����� � ����������
            if(!DSF(_inF, hmcur)) return false;         // ������ �� ����� �������� ������
            if(!DSF(_inF, flagRMNames)) return false;   // ������ �� ����� ����
            #ifdef CDtor_voice
                cout << "flagRMNames= " << flagRMNames << endl;
            #endif // CDtor_voice
            if(flagRMNames) {                                           // ���� ���� true, �� ������ ���� ��������� �� �����
                strNameMeas* tmp = new(nothrow) strNameMeas[RMCount];   // �������� ������ ��� ������
                if(!tmp) return false;                                  // ���� ������ �� ��������, �� ����� � ������� false
                for(size_t i=sZero; i<RMCount; i++) {                   // ������ �� ����� ������
                    if(!DSF(_inF, (tmp+i)->name)) return false;         // � ���������� ����� �
                    if(!DSF(_inF, (tmp+i)->measure)) return false;      // ��������� ���������
                };
                std::swap(tmp, RMNames);                                // ������������ �����������
                if(tmp) delete[] tmp;                                   // ������� ��������������� ������
            } else RMNames = nullptr;

//            if(RMNames) delete[] RMNames;               // ���� ������ ����������, �� ������� ���
//            RMNames = new(nothrow) strNameMeas[RMCount];// �������� ������ ��� ������
//            if(RMNames) {                               // ���� ������ ��������, ��
//                for(size_t i=sZero; i<RMCount; i++) {   // ������ �� ����� ������ � ���������� ����� � ��������� ���������
//                    if(!DSF(_inF, (RMNames+i)->name)) return false;
//                    if(!DSF(_inF, (RMNames+i)->measure)) return false;
//                };
//            } else return false;

            if(manufsize == sZero) return true;             // ���� ������ ������, �� �������������� �������������
            Manuf.reserve(capac);                           // ����������� ������ ��� �������
            for(size_t i=sZero; i<manufsize; i++) {
                Manuf.emplace_back();                       // ������� ������� ��������������� � �������
                if(!Manuf.back().RfF(_inF)) return false;   // �������� ����� �������������� ����� ��������
            };
            #ifdef CDtor_voice
                long epos = _inF.tellg();                                   // ���������� ������� � ������ �����
                cout << "clsManufactory::RfF end epos= " << epos << endl;   // ������� ������� �� �����
            #endif // CDtor_voice
            return true;
        }   // RfF

        bool clsManufactory::ReadFromFile(const string _filename) {
        /** ����� ������ �� ����� � ������ � ��������� ������ **/
            if(_filename == EmpStr) return false;               // ���� ��� ����� ������, �� ����� � ������� false
                                                                // 1.   ������� ��������� ������ ifstream
            ifstream inF(_filename, ios::in | ios::binary);     // ��������� ��� ����� � �������� ������� ��� ������ � �����
                                                                // 2.   ���������, ������� �� ������ ����
            if (!inF.is_open()) {                               // ���� ���� �� ������, �� ��������� ������������ �
                cout << "������ �������� �����" <<endl;         // ������� false � ����� �� �������
                return false;
            };
            if(!RfF(inF)) {                                     // 3.   ��������� ������ �� �����. ���� ���� �� �������, ��
                inF.close();                                    // ��������� ����
                return false;                                   // ����� � ������� false
            };
            #ifdef CDtor_voice
                long epos = inF.tellg();
                cout << "clsManufactory::ReadFromFile end epos= " << epos << endl;
            #endif // CDtor_voice
            inF.close();                                        // 4.   ��������� ����
            return true;
        }   // ReadFromFile

/** -----------------------   ������ ����������� �������� ������� ������ clsManufactory   ---------------------------------  **/

        void clsManufactory::ViewProjectParametrs() const {
        /** ����� ����������� �������� ������� GetPrCount, GetProdCount, GetRMCount **/
            cout << "*** �������� ��������� ������� ***" << endl;
            cout << "���������� �������� �������            " << GetPrCount() << endl;
            cout << "����� ����� ������� ���������          " << GetProdCount() << endl;
            cout << "����� ����� ������� ����� � ���������� " << GetRMCount() << endl;
            cout << "������ �������                         " << GetCurrency() << endl;
            cout << endl;
        }   // ViewMainParametrs

        void clsManufactory::ViewRawMatPurchPlan() const {
        /** ����� ����������� �������� ������� GetRawMatPurchPlan **/
            cout << "*** �������� ������ ������ GetRawMatPurchPlan ***" << endl;
            cout << "����������� � �������� ��� ���� ���������" << endl;
            decimal* temp = GetRawMatPurchPlan();
            strNameMeas* RMNtemp = new(nothrow) nmBPTypes::strNameMeas[RMCount];
            if(!RMNtemp) return;
            for(size_t i=sZero; i<RMCount; i++) {
                (RMNtemp+i)->name = (RMNames+i)->name;
                (RMNtemp+i)->name.resize(15);            // ������� ����� ������
                (RMNtemp+i)->measure = (RMNames+i)->measure;
                (RMNtemp+i)->measure.resize(8);          // ������� ����� ������
            };
            ArrPrint(RMCount, RMNtemp, temp, PrCount);
            delete[] temp;
            delete[] RMNtemp;
        }   // ViewRawMatPurchPlan

        void clsManufactory::ViewRawMatPrice() const {
        /** ����� ����������� �������� ����������������� ������� SetRawMatPrice. **/
            cout << "*** �������� ������ ������ SetRawMatPrice ***" << endl;
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++) {
                it->ViewRawMatPrice(*(nmBPTypes::CurrencyTXT+hmcur));
            }
        }   // ViewRawMatPrice

        void clsManufactory::ViewCalculate() const {
        /** ����� ����������� �������� ����������������� ������� Calculate. **/
            cout << "*** �������� ������ ������ Calculate ***" << endl;
            if(Manuf.size() == sZero) return;           // �������� ������� �� ���������� ���������
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++) {
                it->ViewCalculate(*(nmBPTypes::CurrencyTXT+hmcur));
            }
        }   // ViewCalculate

        void clsManufactory::ViewBalance() const {
        /** ����� ����������� �������� ������� GetProductDescription, GetTotalBalance **/
            strNameMeas *ntemp = GetProductDescription();    // ��������� ������ ���� ��������� � ��.���������
            if(!ntemp) return;
            strItem *btemp = GetTotalBalance();
            if(!btemp) return;
            cout << "*** ������������� ������������ ***" << endl;
            const string money = *(nmBPTypes::CurrencyTXT+hmcur);
            ArrPrint(GetProdCount(), ntemp, btemp, GetPrCount(), volume, money);
            ArrPrint(GetProdCount(), ntemp, btemp, GetPrCount(), price, money);
            ArrPrint(GetProdCount(), ntemp, btemp, GetPrCount(), value, money);
            delete[] ntemp;
            delete[] btemp;
        }   // ViewBalance

        void clsManufactory::ViewProductPlan() const {
        /** ����� ����������� �������� ������� GetProductDescription, GetTotalProduct **/
            strNameMeas *ntemp = GetProductDescription();    // ��������� ������ ���� ��������� � ��.���������
            if(!ntemp) return;
            for(size_t i=sZero; i<Manuf.size(); i++) {       // ������� ����� �����
                (ntemp+i)->name.resize(15);
                (ntemp+i)->measure.resize(8);
            };
            strItem *ptemp = GetTotalProduct();
            if(!ptemp) return;
            cout << "*** ������� ��������� ***" << endl;
            const string money = *(nmBPTypes::CurrencyTXT+hmcur);
            ArrPrint(GetProdCount(), ntemp, ptemp, GetPrCount(), volume, money);
            ArrPrint(GetProdCount(), ntemp, ptemp, GetPrCount(), price, money);
            ArrPrint(GetProdCount(), ntemp, ptemp, GetPrCount(), value, money);
            delete[] ntemp;
            delete[] ptemp;
        }   // ViewProductPlan

        void clsManufactory::ViewRecipes() const {
        /** ����� ���������� ������� �� ����� ��������� ���� ���������. **/
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++)
                    it->ViewRefRecipe();
        }   // ViewRecipe

#undef CDtor_voice          // �������� ����� ���������� ����������
# undef DEBUG_THREADS   // �������� ������ ���������� �� ����������� DEBUG
