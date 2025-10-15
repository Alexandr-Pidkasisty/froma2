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

#include "warehouse_module.h"

//#define DEBUG             // ������ ������ ���������� ����������. ����������������� ��� �������
//#define DEBUG_THREADS     // ������ ������ ���������� ����������. ����������������� ��� �������

std::mutex mu;  // ������ �� �������������� ������� ������� � ������ �������

void EraseVector(vector<thread>& _pool) {
    /** ����� ��� ������� ������� � �������� ������ ������ ������ **/
        vector<std::thread>().swap(_pool);    // �������� ������ � ��������� �� ���������
    }   // DebugEraseVector

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                                class clsSKU                                                         **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

    /** ����� ����� � ���������� ��� ����� �������������� ������� (Stock Keeping Unit - ������� ���������� �����)**/

        decimal clsSKU::AVGcalc(const strItem P[], strItem R[], strItem S[], size_t N, bool ip) {
            /** ������� ������������ ���� � ��������� ����� � ����������, ����������� �� ������ � ������ ������� �������
            �� �������� AVG, ��������� ������ �������� � ������ �������� �������� � ���������� ������� ����� ��� ��������.
            ���������: P[] - ��������� �� ������ ������� (������ �������� ���������, const), R[] - ��������� �� ���������������
            ������, - ����� ���� ����������� �������� ���������� ������� �� ������ ������ �� ����� �������, S[] - - ���������
            �� ������ ��������, � �������� ��������� ���� S.volume (���������� � �������� ������ �������: �������������� ����
            price � value); N - ����������� �������� �������, �������� � ��������; ip - ���������, ������� ����, ����� ��
            ������������ ���������� � �����-���� ������� ����� � ��������� ��� �������� � ���� �� �������: "true" ��������,
            ��� ������� �������������� � ������ ������� ������� � ���������� ����� � ��������� ����� ���� ������������ �
            ��������� ���� �� �������, "false" ��������, ��� ������� ������������ � ����� ������� �������, ������� ��������
            ���� ����� � ���������� ����� ���� ������ � ��������� ��������. ��������!!! ������� �� ������������ ���� ��������,
            ���� ��������� ������� ����� � ����������; � ������� ��� ��������� �������, ������ ��� � ��������� �����������
            ������ �� ���������.**/
            strItem Q, M, Z;                // ��������������� ��������� ����������
            decimal deficit = dZero;        // ������� ����� � ���������� ��� �������� �� ������. ��������� ����������
            size_t L;                       // ��������������� ��������� ����������
            for(size_t i=sZero; i<N; i++) { // � ������, ���� �� ����� �������� �������, ������ Rem ���������� �� Pur,
                *(R+i) = *(P+i);            // � ��������� ����� ������� �������� � ������, ������� ���������� Rem � ��������� Pur
                (S+i)->value = (S+i)->price = dZero;    // ����� �������� ��������� � ���� � ���������
            };
            for(size_t n=sOne; n<N; n++) {  // �������� ���� �� ���� �������� ��������, ����� �������� (�������� ���)
                if(ip) { L = n; }           // ����� ������������ ����� � ��������� �� ���� �� �������, ��� � ��������
                else { L = n-sOne; };       // ����� � ��������� ������������ ������ �� ���������� �������� ������� ��������
                Z.value = Z.volume = dZero;
                for(size_t i=sZero; i<=L; i++) {                    // ������������ ������� ����
                    Z.volume += (R+i)->volume;
                    Z.value += (R+i)->value;
                    Z.price = (fabs(Z.volume) > epsln)? Z.value/Z.volume:dZero;
                };
                Q.volume = (S+n)->volume;                           // ������������ ������� �������� ��������
                for(size_t j=sZero; j<=L; j++) {                    // �������� �� ����� ������� ��������
                    (R+j)->price = Z.price;                         // ���������� ������� ���� � �������
                    if(Q.volume > (R+j)->volume) { M.volume = (R+j)->volume; }
                    else { M.volume = Q.volume; };
                    (R+j)->volume -= M.volume;                      // ��������� ������� �� �������� ��������,
                    (R+j)->value = (R+j)->volume * (R+j)->price;    // ������������ ��������� ��������
                    (S+n)->price = (R+j)->price;                    // ���������� � �������� ����
                    (S+n)->value = (S+n)->volume * (S+n)->price;    // � ��������� ������
                    Q.volume -= M.volume;                           // ��������� �������
                }; // ���� �� j
                // ���� ������ Q.volume ������ epsln, �� ������� ��� ������ ����, �.�. ����� decimal �� ������
                // ����� ����� ���� �����
                Q.volume = (fabs(Q.volume) < epsln) ? dZero : Q.volume;
                deficit += Q.volume;                                // ��������� ����� ������� ��������
            };  // ���� �� n
            return deficit;
        }   // AVGcalc

        decimal clsSKU::FLFcalc(const strItem P[], strItem R[], strItem S[], size_t N, bool ip, AccountingMethod ind) {
            /** ������� ������������ ���� � ��������� ����� � ����������, ����������� �� ������ � ������ ������� �������.
            �� �������� FIFO ��� LIFO, ��������� ������ �������� � ������ �������� �������� � ���������� ������� �����
            ��� ��������. ���������: P[] - ��������� �� ������ ������� (������ �������� ���������, const), R[] - ��������� ��
            ��������������� ������, - ����� ���� ����������� �������� ���������� ������� �� ������ ������ �� ����� �������.
            S[] - ��������� �� ������ ��������, � �������� ��������� ���� S.volume (���������� � �������� ������ �������:
            �������������� ���� price � value); N - ����������� �������� �������, �������� � ��������; ip - ���������, �������
            ����, ����� �� ������������ ���������� � �����-���� ������� ����� � ��������� ��� �������� � ���� �� �������:
            "true" ��������, ��� ������� �������������� � ������ ������� ������� � ���������� ����� � ��������� ����� ����
            ������������ � ��������� ���� �� �������, "false" ��������, ��� ������� ������������ � ����� ������� �������,
            ������� �������� ���� ����� � ���������� ����� ���� ������ � ��������� ��������. Ind � ����, ���������������
            ������� ����� �������: FIFO ��� LIFO.��������!!! ������� �� ������������ ���� ��������, ���� ��������� �������
            ����� � ����������; � ������� ��� ��������� �������, ������ ��� � ��������� ����������� ������ �� ���������.**/
            const int iZero = 0;
            int i, j;                    // �������� ��� �������� ������� ��������
            int *k;                      // ������ �� ����������� �������: ��� i, ��� j
            strItem Q, M;                // ��������������� ��������� ����������
            decimal deficit = dZero;     // ������� ����� � ���������� ��� �������� �� ������
            int L;                       // ��������������� ����������
            for(size_t i=sZero; i<N; i++) { // � ������, ���� �� ����� �������� �������, ������ Rem ���������� �� Pur,
                *(R+i) = *(P+i);            // � ��������� ����� ������� �������� � ������, ������� ���������� Rem � ��������� Pur
                (S+i)->value = (S+i)->price = dZero;            // ����� �������� ��������� � ���� � ���������
            };
            if(ind == FIFO) { k = &i; }     // � ����������� �� ������ ����� FIFO ��� LIFO ������� ������� ������� ����
                else { k = &j;};            // �� ������ � ������� ��� �� ������� � ������ ��������: ��������� k �������������
                                            // ������ �� ��������������� �������
            size_t n;
            /** ���������� ������-��������� (������ ����������� ������� ������ ������ �������), ��
            http://����������.��/pdf/cppmingw/ch12-cpp-lyambda-vyrazheniya.html **/
            auto lambda = [&M, &R, &k, &S, &n, &Q]() {  // ������ ���������� �� ������
                M.price = (R+*k)->price;        // ������� �� �������� M, ������������� ��������� ����������� �����,
                M.value = M.volume * M.price;   // ����������� ��������� ������������ �����, ������������ �������
                (R+*k)->volume -= M.volume;     // ������� ��� �������� �� ��������� �������� � ��������� � ���������
                (R+*k)->value = (R+*k)->volume * (R+*k)->price; // �������� �����
                (S+n)->value += M.value;
                Q.volume -= M.volume;
            };
            for(n=sOne; n<N; n++) {  // �������� ���� �� ���� �������� ��������, ����� �������� (�������� ���)
                if(ip) { L = n; }           // ����� ������������ ����� � ��������� �� ���� �� �������, ��� � ��������
                else { L = n-sOne; };       // ����� � ��������� ������������ ������ �� ���������� �������� ������� ��������
                Q.volume = (S+n)->volume;   // ������������ ������� �������� ��������
                for(i=iZero, j=L; (i<=L)&&(j>=iZero); i++, j--) // �������� �� ����� ������� �������: �� i �� ���� �� L-1 ���
                /**  �������� ������������� ������!!! � ���� size_t ������, ��������� �� ����� ��� ���������� j ����� �������
                ������������� �����!!! ���� ����� ����� �����������!!! **/
                {                                           // �� j �� L-1 �� ���� ������������: � ����������� �� ��������� k
//                    if(is_bigger(Q.volume, (R+*k)->volume)) // ���� ��������� �������� ��������� ������� � k-�� �������, ��
                    if(Q.volume > (R+*k)->volume)           // ���� ��������� �������� ��������� ������� � k-�� �������, ��
                    {                                       // ��������������� ���������� M.volume ��������� �������� �����
                        M.volume = (R+*k)->volume;          // ������ �������� k-�� �������; ��������� ����� ����� � �������
                        // ������� �� �������� M, ������������� ��������� ����������� �����, ����������� ��������� ������������
                        // �����, ������������ ������� ������� ��� �������� �� ��������� �������� � ��������� � ���������
                        // �������� �����
                        lambda();                           // ��������� ������ ��������� ������ ������������� ������� ����
                    }
                    else                                    // ���� �� ��������� �������� �� ��������� ������� � k-�� �������, ��
                    {                                       // ��������� ����� ����� � ������� ������� �� �������� M, �������������
                        M.volume = Q.volume;                // ��������� ����������� �����, ��������� ������������ �����,
                        // ������������ ������� ������� ��� �������� �� ��������� �������� � ������� �� ����� ���������
                        lambda();
                        break;      // ��������� ������ ��������� ������ ������������� ������� ����
                    };
                };  // ���� �� i � j
                // ���� ������ Q.volume ������ epsln, �� ������� ��� ������ ����, �.�. ����� decimal �� ������
                // ����� ����� ���� �����
                Q.volume = fabs(Q.volume) < epsln ? dZero : Q.volume;
                deficit += Q.volume;
                (S+n)->price = fabs((S+n)->volume)>epsln?(S+n)->value/(S+n)->volume:dZero;  // ������������ ����������
            } // �������� ���� �� n                                                         // ���� ������������ ����� � n-�� �������
            return deficit;
        }   // FLFcalc

        decimal clsSKU::PURcalc(strItem P[], const strItem S[], size_t N, bool ip, decimal Shr) {
        /** ������� ������������ ������ �������, ����������� ��� ���������� ����� �������� � ����������� ���������
        ������� ����� � ���������� (������� �������� ��� ���� �� ������ �������� �� ������) � ���������� ������� �����
        � ������������ (�������) �������. �������������� ������� � �������� �������� - � ������� �� ���������� (N-1).
        ������� � ������� ������ �������� ������� � ������������� ������� ����� � ���������� � ������ ������� (�������
        ���������� �������). ���������: P[] - ��������� �� ������ ������� (���������� � �������� ������ �������), S[] -
        ��������� �� ������ ��������, � �������� ��������� ���� S.volume (������ �������� ���������, const); N - �����������
        �������� ������� � ��������; ip - ������� ����, ����� �� ������������ ���������� � �����-���� i-� ������� ����� �
        ��������� ��� �������� � ���� �� �������: True - �����, False - ������, ��� �������� ����� ������������ ������
        ������ �� ���������� ��������; Shr - �������� ������� �����, ��������, ��� ���� �������� �� ������ (����� �� 0) **/
            if((!P) || (!S) || N==sZero) return dZero;    // ���� ��� �������� ��� �� ����������� �������, �� ����� � �����
            if(!ip) // ��������� ������� ����� � ������� �������; ���� ������� ����, �� ����� �� ������� � ������� ��������
                if((S+sOne)->volume > (P+sZero)->volume) return ((S+sOne)->volume - (P+sZero)->volume);   // ����� ��������
            decimal Z[N], Q[N], V[N];
            size_t i, k;
            *Z = dZero;                                         // ������ �������� ����������� ������.
            *V = *Q = P->volume;
            for(i=sOne; i<N; i++) {
                *(Z+i) = *(Z+i-sOne) + (S+i)->volume;           // ������� �������� ������� ����������� ������
                *(Q+i) = (S+i)->volume * Shr;                   // ������������ ��������� �����
                if((*(Z+i)+*(Q+i)) < *(V+i-sOne)) { *(V+i)= *(V+i-sOne); } // ���� ����� �������� ������� ����������� ������
                else { *(V+i)= *(Z+i)+*(Q+i); };                // ������, ��� � ���������� �������, ������������ ��� � ����������� ������
                if(ip) { k = sZero; }                           // ������� �� ���� �� �������, ��� � ��������
                else { k = sOne; (P+N-sOne)->volume = dZero; }; // ������� � �������, ��������������� ������� ��������
                if((i-k) != sZero) {                            // ���������� ����� �������� ������� �������
                    (P+i-k)->volume = *(V+i) - *(V+i-sOne);
                };
                (P+i-k)->value = (P+i-k)->volume * (P+i-k)->price;  // ������������ ���������
            }; // ���� �� i
            return dZero;
        }   // PURcalc

        void clsSKU::BALcalc(const strItem P[], strItem B[], const strItem S[], size_t N) {
        /** ������� ��������� ������ ���������� �������� ����� �� ������. ������� ����� ���� ������� ������ �����
        ����������� ������� ������� ������� (�������� PURcalc) � ��� �������� (������� FLFcalc ��� AVGcalc).
        ���������: P[] - ��������� �� ������ ��������� ������������ ������� (���������), B[] - ��������� �� ������
        ���������� �������� (���������� � �������� ������ �������), S[] - ��������� �� ������ ��������� ������������
        �������� (���������), N - ����������� �������� ������� � �������� **/
            strItem Z[N], Q[N];      // ��������������� �������
            *B = *P;                 // ������ ����� � ������� ������� ��������� � ���������
            *Q = *P;                 // ����������� ��������� �������� ��� �������������� �����
            Z->volume = Z->value = dZero;
            for(size_t i=sOne; i<N; i++) {
                (Q+i)->volume = (Q+i-sOne)->volume + (P+i)->volume;      // ������� ������������� ������
                (Q+i)->value = (Q+i-sOne)->value + (P+i)->value;         // ������� ������������� ������
                (Z+i)->volume = (Z+i-sOne)->volume + (S+i)->volume;      // �������� ������������� ������
                (Z+i)->value = (Z+i-sOne)->value + (S+i)->value;         // �������� ������������� ������

                (B+i)->volume = (Q+i)->volume - (Z+i)->volume;           // ���������� ������ �� ������ ��� ������� �������
                (B+i)->value = (Q+i)->value - (Z+i)->value;
                (B+i)->price = fabs((B+i)->volume)>epsln?(B+i)->value / (B+i)->volume:dZero;
            };
        } // BALcalc

        clsSKU::clsSKU() {
        /** ������ �����������. **/
            PrCount = sZero;
            name = measure = EmpStr;
            Pur = Rem = Bal = Ship = nullptr;
            lack = dZero;
            indr = false;
            acct = FIFO;
            pcalc = calc;
            share = dZero;
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsSKU Empty Ctor" << endl;
            #endif
        } // clsSKU Empty Ctor

        clsSKU::clsSKU(const size_t _PrCount, const string &_name, const string &_measure, AccountingMethod _ac, bool _ind,\
            PurchaseCalc _flag, const decimal& _share, const strItem _Ship[]) {
        /** ����������� � ������ ������� ��������. ���������: _PrCount - ���������� �������� �������, _name - ������������
        �����, _measure - ������� ��������� �����, _ac - ����� ����� �������, _ind - ���������� �� �������� � ������� � �����
        �������, _flag - ���� �������/ ��������������� ������� ������� �����, _share - ����� ����� � ���������� �� ������ �
        ������ ������, ���������� � ���� �� ������ �������� �� ���� ������, _Ship - ������ � ������� �������� (���������
        ������ ���� volume) **/
            PrCount = _PrCount;
            name = _name;
            measure = _measure;
            acct = _ac;
            indr = _ind;
            pcalc = _flag;
            if(_share < dZero) share = dZero;
            else share = _share;
            lack = dZero;
            if(PrCount>sZero) {
                Ship = new(nothrow) strItem[PrCount]{};
                if(_Ship) { var_cpy(Ship, _Ship, PrCount); };
                Pur = new(nothrow) strItem[PrCount]{};
                Rem = new(nothrow) strItem[PrCount]{};
                Bal = new(nothrow) strItem[PrCount]{};
            } else
                Ship = Pur = Rem = Bal = nullptr;
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsSKU Ctor with parametrs, include strItem _Ship" << endl;
            #endif
        }   // Ctor clsSKU

        clsSKU::clsSKU(const size_t _PrCount, const string &_name, const string &_measure, AccountingMethod _ac, bool _ind,\
            PurchaseCalc _flag, const decimal& _share) {
        /** ����������� � �����������. ���������� �����������, �� ��� ����� ������ �� ���������. ������������ � ������
            clsStorage::RfF ��� ��������������. **/
            PrCount = _PrCount;
            name = _name;
            measure = _measure;
            acct = _ac;
            indr = _ind;
            pcalc = _flag;
            if(_share < dZero) share = dZero;
            else share = _share;
            lack = dZero;
            if(PrCount>sZero) {
                Ship = new(nothrow) strItem[PrCount]{};
                Pur = new(nothrow) strItem[PrCount]{};
                Rem = new(nothrow) strItem[PrCount]{};
                Bal = new(nothrow) strItem[PrCount]{};
            } else
                Ship = Pur = Rem = Bal = nullptr;
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsSKU Ctor with parametrs" << endl;
            #endif
        }   // CTOR clsSKU

        clsSKU::clsSKU(const clsSKU &obj) {
        /** ��������� � ������ ������������ ������������ ��������� ������ ��� ����������, ���������� ���� �������
         ����������� �����������. ����������� ����������� ��������� ��� ����, ����� �� ����� ��������� ���������
         (� �� ���������) ����� ��� �������� ������. ����� ����� ������� ����� ������������ � ��������� �������:
         ��� �������� ������� ������ � �������, ��� ��������� �� �������� (� �� �� ������); ��� ����������� ��
         ������� ������� ������, ��� ���������� � ������; ��� �������������  ������ ������� ������ ������ ��������
         ����� ������.**/
            PrCount = obj.PrCount;
            name = obj.name;
            measure = obj.measure;
            acct = obj.acct;
            indr = obj.indr;
            pcalc = obj.pcalc;
            share = obj.share;
            lack = obj.lack;
            if(obj.Pur) { Pur = new(nothrow) strItem[PrCount]; } else Pur = nullptr;
            if(Pur) { var_cpy(Pur, obj.Pur, PrCount); };
            if(obj.Rem) { Rem = new(nothrow) strItem[PrCount]; } else Rem = nullptr;
            if(Rem) { var_cpy(Rem, obj.Rem, PrCount); };
            if(obj.Bal) { Bal = new(nothrow) strItem[PrCount]; } else Bal = nullptr;
            if(Bal) { var_cpy(Bal, obj.Bal, PrCount); };
            if(obj.Ship) { Ship = new(nothrow) strItem[PrCount]; } else Ship = nullptr;
            if(Ship) { var_cpy(Ship, obj.Ship, PrCount); };
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsSKU Copy Ctor" << endl;
            #endif
        }   // CopyCTOR clsSKU

        clsSKU::clsSKU(clsSKU&& obj) {
        /** ����������� ����������� **/
            PrCount = sZero;
            name = measure = EmpStr;
            Pur = Rem = Bal = Ship = nullptr;
            lack = dZero;
            indr = false;
            acct = FIFO;
            pcalc = calc;
            share = dZero;
            swap(obj);
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsSKU Move Ctor" << endl;
            #endif
        }   // Move Ctor clsSKU

        clsSKU& clsSKU::operator=(const clsSKU &obj) {
        /** ���������� ��������� ������������ ������������. ����������� � ������
        �����������-�-������ (copy-and-swap idiom) **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsSKU tmp(obj);                    // �������� ����������� ����������� � �������� ����� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsSKU Copy operator=" << endl;
            #endif
            return *this;
        }   // Copy operator=

        clsSKU& clsSKU::operator=(clsSKU &&obj) {
        /** ���������� ��������� ������������ ������������. ����������� � ������ �����������-�-������
        (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsSKU tmp(move(obj));              // �������� ����������� ����������� � �������� ���������� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsSKU Move operator=" << endl;
            #endif
            return *this;
        }   // clsSKU Move operator=

        clsSKU::~clsSKU() {
            if(PrCount > sZero) {
                if(Pur) {  delete[] Pur;  Pur = nullptr; }; // ������� �� ������ �������� ������ �������
                if(Rem) {  delete[] Rem;  Rem = nullptr; }; // ������� �� ������ �������� ������ �������� �� ����� �������
                if(Bal) {  delete[] Bal;  Bal = nullptr; }; // ������� �� ������ �������� ������ �������� �������� �� ��������
                if(Ship) { delete[] Ship; Ship = nullptr;}; // ������� �� ������ �������� ������ ��������
            };
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsSKU Dtor" << endl;
            #endif
        }   // ~clsSKU

        bool clsSKU::operator == (const  string& Rightname) const {
        /** ��������������� ��������� ��������� ��� ������ ���������� ������� �� �����**/
            return (name == Rightname);
        }   // operator ==

        void clsSKU::swap(clsSKU& obj) noexcept {
        /** ������� ������ ���������� ����� ���������. ������� ��������� noexcept - �� ���������� ���������� **/
            std::swap(PrCount, obj.PrCount);
            std::swap(name, obj.name);
            std::swap(measure, obj.measure);
            std::swap(Pur, obj.Pur);
            std::swap(Rem, obj.Rem);
            std::swap(Bal, obj.Bal);
            std::swap(Ship, obj.Ship);
            std::swap(lack, obj.lack);
            std::swap(indr, obj.indr);
            std::swap(acct, obj.acct);
            std::swap(pcalc, obj.pcalc);
            std::swap(share, obj.share);
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsSKU swap" << endl;
            #endif
        }   // swap

        /** Get-������ **/

        const size_t& clsSKU::Count() const { return PrCount; }     // ���������� const-������ �� ���������� ��������
        const string& clsSKU::Name() const { return name; }         // ���������� const-������ �� �������� �������������� ������� ���������� �����
        const string& clsSKU::Measure() const { return measure; }   // ���������� const-������ �� ������� ������������ ���������
        string clsSKU::AccMethod() const {                          // ���������� ������� ����� ������� �
            if(acct==FIFO) return "FIFO";                           // ���� ��������� ������
            else if (acct==LIFO) return "LIFO";
                 else return "AVERAGE";
        } // AccMethod
        string clsSKU::Permission() const {         // ���������� ���� ����������/���������� �������� � ����� �������
            return ProhibitedTXT[static_cast<size_t>(indr)];
        }   // Permission
        string clsSKU::AutoPurchase() const {       // ���������� ���� �����������/ ������� ������� �������
            return PurchaseCalcTXT[pcalc];
        }   // AutoPurchase
        const PurchaseCalc& clsSKU::GetAutoPurchase() const { return pcalc; }
        /** ���������� const-������ �� ���� �����������/ ������� ������� ������� **/

        const decimal& clsSKU::Share() const { return share; }
        /** ���������� const-������ �� �������� ������ ����� � ����� �� ������ �������� **/

        const strItem* clsSKU::GetDataItem(const ChoiseData _cd, size_t _N) const {
        /** ������� ���������� ������ �������� ������� � �������� _N � ���� const-��������� �� ��������� ���� strItem;
        ����� ������� ������������ ��������� ���������� _cd (purchase - �������, balance - �������, shipment - ��������). **/
            if( _N < PrCount ) {   // ���� ������ � ����������� �������� ����� ����������� ��������
                strItem* p;
                if( (_cd == purchase) || (_cd == balance) ) {
                    if(_cd == purchase) { p = Pur; }    // �������� ������ ��� ����� ������: "purchase" - ������ �������
                    else { p = Bal; };             // "shipment" - ������ ��������
                }
                else { p = Ship; }
                return (p+_N);
            }
            else
                return nullptr;
        }   // GetDataItem

        const strItem* clsSKU::GetShipment() const {
        /** ����� ���������� ����������� ��������� �� ������ Ship **/
            return Ship;
        }   // GetShipment

        const strItem* clsSKU::GetPurchase() const {
        /** ����� ���������� ����������� ��������� �� ������ Pur **/
            return Pur;
        }   // GetPurchase

        const strItem* clsSKU::GetBalance() const {
        /** ����� ���������� ����������� ��������� �� ������ Bal **/
            return Bal;
        }   // GetBalance

        const decimal& clsSKU::GetLack() const {
        /** ����� ���������� ������� ����� � ���������� ��� �������� �� ������ **/
            return lack;
        }   // GetLack

        /** Set-������ **/

        void clsSKU::SetName(const string& _name) { name = _name; }             // ������ ��� SKU
        void clsSKU::SetMeasure(const string& _measure) { measure = _measure; } // ������ ������� ���������
        void clsSKU::SetPermission(bool _indr) { indr = _indr;  }               // ������������� ���� �������� � ������� � ����� �������
        void clsSKU::SetAutoPurchase(PurchaseCalc _pcalc) { pcalc = _pcalc; }   // ������������� ���� ����������� �������
        void clsSKU::SetShare(decimal _share) { share = _share; }               // ������������� �������� ������ ����� �� ������
        void clsSKU::SetAccount(AccountingMethod _acct) { acct = _acct; }       // ������������� ������� ����� �������

        bool clsSKU::SetDataItem(const ChoiseData _cd, const strItem _U, size_t _N) {
        /** ������� ���������� � ������� ������� � �������� _N �������� volume, price � value �� ���������� _U;
        ����� ������� ������������ ��������� ���������� _cd. ���������� True, ���� ������ ������� **/
            if( _N < PrCount) {                     // ���� ������ � ����������� �������� ����� ����������� ��������
                strItem* p;                         // ��������������� ���������
                if(_cd == purchase) { p = Pur; }    // �������� ������ ��� ����� ������: "purchase" - ������ �������
                    else { p = Ship; };             // "shipment" - ������ ��������
                *(p+_N) = _U;                       // ���� ������ � ������
                return true;
            } else return false;
        }   // SetDataItem

        bool clsSKU::SetPurchase(const strItem _unit[], size_t _count) {
        /** ������� ��������� ������ � ������� ����� � ���������� �� ����� (������ � ����). ���������:  _unit[]
            - ��������� �� ������ ����������� ������, _count - ����������� ������� (����� �� ��������� � ������������
            ������� ������� PrCount) **/
            if((!_unit) || (_count==sZero)) return false;       // ��������� ����������
            size_t M;
            if(_count > PrCount) M = PrCount; else M = _count;  // ���� ������ ����������� � ������� ������� ������ �������
            var_cpy(Pur, _unit, M);         // �������, �� ������ ����������, ����� ������������ ������ ����������� �������
            var_cpy(Rem, _unit, M);         // ��������� ������ � ������� Pur (�������) � Rem (�������)
            return true;
        }   // SetPurchase

        bool clsSKU::SetPurPrice(const strItem _unit[], size_t _count) {
        /** ������� ��������� ������ ���� �� ���������� �� ����� �����. ���������:  _unit[] - ��������� �� ������
        ����������� ������, _count - ����������� ������� (����� �� ��������� � ������������ ������� ������� PrCount)**/
            if((!_unit) || (_count==sZero)) return false;            // ��������� ����������
            size_t M;
            if(_count > PrCount) M = PrCount; else M = _count;       // ���� ������ ����������� � ������� ������� ������ �������
            for(size_t i = sZero; i<M; i++) {    // �������, �� ������ ����������, ����� ������������ ������ ����������� �������
                 (Rem+i)->price = (Pur+i)->price = (_unit+i)->price; // ��������� ������ � ������� Pur (�������) � Rem (�������)
            };
            return true;
        }   // SetPurPrice

        void clsSKU::SetShipment(const decimal _unit[], size_t _count) {
        /** ������� ��������� ������ � �������� ��������� ����� � ���������� �� ������ (������ ������). ���������:
            _unit[] - ��������� �� ������, ���������� ������ ��������, _count - ����������� ������� (����� �� ���������
            � ������������ ������� ������� PrCount). **/
            if((!_unit) || (_count==sZero)) return;             // ��������� ����������
            size_t M;
            if(_count > PrCount) M = PrCount; else M = _count;
            for(size_t i = sZero; i<M; i++) {
                (Ship+i)->volume = *(_unit+i);
            };
        }   // SetShipment

        /** ������ ����������� �������� **/

        void clsSKU::View() const {                   // ��������� (?) �������. ������� �� ����� ��������� �������, ��������, ��������
            cout << "Project periods quantity = " << PrCount << "; name of Raw Material = " << name << endl;
            cout << "Measure = " << Measure() << " Accounting Method is " << AccMethod() << \
                " AutoPurchase= " << AutoPurchase() <<endl;
            cout << "Share is " << Share() << " Permission= " << Permission() << endl << endl;

            for(size_t i=sZero; i<PrCount; i++) {
                cout << "Purchase[" << i << "]= " << Pur[i];
            };
            for(size_t i=sZero; i<PrCount; i++) {
                cout << "Remain[" << i << "]= " << Rem[i];
            };
            for(size_t i=sZero; i<PrCount; i++) {
                cout << "Balance[" << i << "]= " << Bal[i];
            };
            for(size_t i=sZero; i<PrCount; i++) {
                cout << "Shipment[" << i << "]= " << Ship[i];
            };
            cout << "Lack for shipment = " << lack << endl << endl;
        }   // Show

        void clsSKU::ViewData(const ChoiseData _cd) const {
        /** ����� ����������� �������� ��������� �������, ��������, ��������; ����� ������� ������������ ���������
        ���������� _cd (purchase - �������, balance - �������, shipment - ��������). **/
            strItem* p;                                         // ��������� ���������� -  ���������
                if( (_cd == purchase) || (_cd == balance) ) {   // �������� ������ ��� ����� ������:
                    if(_cd == purchase) { p = Pur; }            // "purchase" - ������ �������
                    else { p = Bal; };                          // "balance" - ������ ��������
                } else { p = Ship; }                            // "shipment" - ������ ��������
            nmBPTypes::strNameMeas names[sOne];                 // ��������� ������ ����. ��������� ���
            names->name = Name();                               // ��������� SKU
            names->name.resize(15);                             // ������� �������� �� ��������� ����� ��������
            names->measure = Measure();                         // ������������ SKU
            names->measure.resize(8);                           // ������� ���������� �� ��������� ����� ��������
            nmPrntSrvs::ArrPrint(sOne, names, p, PrCount, volume);  // ������� ���������� � ����� ������
            nmPrntSrvs::ArrPrint(sOne, names, p, PrCount, price);
            nmPrntSrvs::ArrPrint(sOne, names, p, PrCount, value);
        }   // ViewData

        /** ��������� ������ **/

        const decimal& clsSKU::CalcPrice() {
        /** ������� ������������ ���� � ��������� ����������� ����� � ���������� � ������ ������� �� ������ ��������������
        ������������� �������� ����� �������: FIFO, LIFO ��� AVERAGE, ���������� ���������� �������� � ������ Ship, ����������
        �������� �������� ����� � ���������� ��� ����������� �������� ��������. ���� ������ �� ����� ����, ������������
        ���� � ��������� ����� ����� ������������ ��������. **/
            if(acct == AVG) { lack = AVGcalc(Pur, Rem, Ship, PrCount, indr); }
            else { lack = FLFcalc(Pur, Rem, Ship, PrCount, indr, acct); };
//            if(is_smaller(lack, epsln)) {           // ���� ������� ������ ������������ ��������, �� ������� ��� �������
            if(lack < epsln) {                      // ���� ������� ������ ������������ ��������, �� ������� ��� �������
                lack=dZero;                         // �������� ������� �
                BALcalc(Pur, Bal, Ship, PrCount);   // ������������ ������ �������� �� ������
            };
            return lack;
        }   // CalcPrice

        const decimal& clsSKU::CalcPurchase() {
        /** ������� ������������ ������ �������, ����������� ��� ���������� ����� �������� � ����������� ��������� �������
        ����� � ����������, ���������� ���������� �������� � ������ Pur, ���������� �������� �������� ����� � ���������� ���
        ����������� �������� �������� � ������� �������.**/
            if(pcalc == nocalc) { return dZero; };  // ���� ������ ������� ��������, �� ����� �� �������
//            decimal def;
            lack = PURcalc(Pur, Ship, PrCount, indr, share);
            if(lack < epsln) { lack = dZero;};        // ���� ������� ������ ������������ ��������, �� ������� ��� �������
            return lack;
        }   // CalcPurchase

        bool clsSKU::Resize(size_t _n) {
        /** ������� �������� ������� �������� ���� strItem, ������������ ����� ����� �������� �������, ������ _n.
        ��� _n < PrCount, ������ ����������, ��� _n > PrCount, - ����������� ����� �������� �������� � �������� ����������.
        ������� ���������� true ��� ������� ��������� �������� ��������, false - � ��������� ������ **/
            if(_n < sOne) { return false; };        // ������������ ������
            if(_n == PrCount) { return true; };     // ��������� ������� �� ������������
            strItem *p, *r, *b, *s;
            p = new strItem[_n];
            r = new strItem[_n];
            b = new strItem[_n];
            s = new strItem[_n];
            if(_n < PrCount) {
                for(size_t i= sZero; i<_n; i++) {
                    *(p+i) = *(Pur+i);              // ��������� ����������� ����������� ���������, �.�. ������ ������� ������� -
                    *(r+i) = *(Rem+i);              // ��� ��������� strItem, ������� �������� ������� ������� ����
                    *(b+i) = *(Bal+i);              // � � ����� ��������� strItem ����������� ������������
                    *(s+i) = *(Ship+i);             // ��������� ������
                }
            }
            else {
                for(size_t i= sZero; i<PrCount; i++) {
                    *(p+i) = *(Pur+i);
                    *(r+i) = *(Rem+i);
                    *(b+i) = *(Bal+i);
                    *(s+i) = *(Ship+i);
                };
                for(size_t i= PrCount; i<_n; i++) {
                    (b+i)->volume = (p+i)->volume = (r+i)->volume = (s+i)->volume = dZero;  // �������� �������� ������ �������
                    (b+i)->price = (p+i)->price  = (r+i)->price  = (s+i)->price = dZero;    // p, �������� r � b �
                    (b+i)->value = (p+i)->value  = (r+i)->value  = (s+i)->value = dZero;    // �������� s
                }
            }
            if(PrCount > sZero) {
                delete[] Pur;
                delete[] Rem;
                delete[] Bal;
                delete[] Ship;
            }
            Pur = p;
            Rem = r;
            Bal = b;
            Ship =s;
            PrCount = _n;
            return true;
        }   // Resize

        TLack clsSKU::Calculate() {
        /** ������� ������������ ��������� ����� �������, ���� ��������� ���� pcalc ����������� ������. ������� �����
        ������������ ���� � ��������� ����������� ������ � ������������ � ��������� ��������� FIFO, LIFO ��� AVG. �������
        ������������ ������� ����� � ���������� ��� �������� �� ������ lack. ���� lack > epsln, ��������� ����� �������
        �������� � ����������� ���������� ����� ���� �������������. ���������� ������������ � ���� ��������� ���� TLack **/
            TLack tmp = {dZero, EmpStr};                                                // ��������������� ����������
            if((PrCount == sZero) || (!Pur) || (!Rem) || (!Bal) || (!Ship)) return tmp; // ��������� ����������
            lack == CalcPurchase();         // ������������ �����������:
            if(fabs(lack) > epsln) {        // ���� ����������� �������, ��
                tmp.lack = lack;            // ��������� ����
                tmp.Name = name;            // �������� ����������
                return tmp;                 // ������� � ���������� ��
            };
            lack == CalcPrice();            // ������������ ���� ��������
            if(fabs(lack) > epsln) {        // ���� ����������� �������, ��
                tmp.lack = lack;            // ��������� ����
                tmp.Name = name;            // �������� ����������
                return tmp;                 // ������� � ���������� ��
            };
            lack = dZero;                   // ���� �������� ���, �� �������� lack
            return tmp;                     // � ������� � ������������ ����������� tmp
        }   // Calculate

        /** ������ ������������� � �������������� **/

        bool clsSKU::StF(ofstream &_outF)  {
        /** ����� ������������� ������ � �������� ���������� �������� ���������� ������ (������ � ����, ����� ������������).
        ���������: &_outF - ��������� ������ ofstream ��� ������ ������ **/
            if(PrCount == sZero) return false;      // ���� ���������� �������� �������, ����� � false
            #ifdef DEBUG                            // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                long bpos = _outF.tellp();          // ���������� ������� � ������ �����
                cout << "clsSKU::StF begin bpos= " << bpos << endl;
            #endif
            if(!SEF(_outF, PrCount)) return false;  // ��������� ���������� ��������
            if(!SEF(_outF, name)) return false;     // ��������� ������������ SKU
            if(!SEF(_outF, measure)) return false;  // ��������� ��.���������
            if(!SEF(_outF, lack)) return false;     // ��������� �������� ��������
            if(!SEF(_outF, indr)) return false;     // ��������� ���� ����������/������� �������� � ������� � ����� �������
            if(!SEF(_outF, acct)) return false;     // ��������� ������� �����
            if(!SEF(_outF, pcalc)) return false;    // ��������� ���� �����������/ ������� ������� �������
            if(!SEF(_outF, share)) return false;    // ��������� �������� ������ ����� �� ������

            for(size_t i{}; i<PrCount; i++) {       // ��������� ��� ������� � ������ �����
                if(!(Pur+i)->StF(_outF)) return false;      // ��������� ������ �������
                if(!(Rem+i)->StF(_outF)) return false;      // ��������� ������ �������� �� ������ ����� � ����������
                if(!(Bal+i)->StF(_outF)) return false;      // ��������� ������ ���������� ��������
                if(!(Ship+i)->StF(_outF)) return false;     // ��������� ������ ��������
            };
            #ifdef DEBUG
                long epos = _outF.tellp();                    // ���������� ������� � ����� �����
                cout << "clsSKU::StF end epos= " << epos << endl;
            #endif // DEBUG
            return true;                 // ���������� true
        }   // StF

        bool clsSKU::RfF(ifstream &_inF) {
        /** ����� ������������� ������ �� �������� ���������� �������� ���������� ������ (������ �� �����, ����� ��������������).
        ���������: &_inF - ��������� ������ ifstream ��� ������ ������ **/
            if(PrCount > sZero) {        // ���� ���������� �������� ������ ����, �� ������� �� ������,
                delete[] Pur;            // �� ����� �������
                delete[] Rem;
                delete[] Bal;
                delete[] Ship;
            };
            #ifdef DEBUG                            // ������ ������ ���������� ����������. ��������, ���� ��������� CDtor_voice
                long bpos = _inF.tellg();                // ���������� ������� � ������ �����
                cout << "clsSKU::RfF begin bpos= " << bpos << endl;
            #endif
            if(!DSF(_inF, PrCount)) return false;          // ������ ���������� ��������
            if(!DSF(_inF, name)) return false;             // ������ ������������ SKU
            if(!DSF(_inF, measure)) return false;          // ������ ��.���������
            if(!DSF(_inF, lack)) return false;             // ������ �������� ��������
            if(!DSF(_inF, indr)) return false;             // ������ ���� ����������/������� �������� � ������� � ����� �������
            if(!DSF(_inF, acct)) return false;             // ������ ������� �����
            if(!DSF(_inF, pcalc)) return false;            // ������ ���� �����������/ ������� ������� �������
            if(!DSF(_inF, share)) return false;            // ������ �������� ������ ����� �� ������
            Pur = new strItem[PrCount];     // ������� ����� �������: �������
            Rem = new strItem[PrCount];     // ��������
            Bal = new strItem[PrCount];     // ���������� ��������
            Ship= new strItem[PrCount];     // ��������

            for(size_t i{}; i<PrCount; i++) {               // ������ ��� ������� � ������ �����
                if(!(Pur+i)->RfF(_inF)) return false;       // ������ ������ �������
                if(!(Rem+i)->RfF(_inF)) return false;       // ������ ������ �������� �� ������ ����� � ����������
                if(!(Bal+i)->RfF(_inF)) return false;       // ������ ������ ���������� ��������
                if(!(Ship+i)->RfF(_inF)) return false;      // ������ ������ ��������
            };
            #ifdef DEBUG
                long epos = _inF.tellg();                    // ���������� ������� � ����� �����
                cout << "clsSKU::RfF end epos= " << epos << endl;
            #endif // DEBUG
            return true;                // ���������� true
        }   // RfF

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                                class clsStorage                                                     **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

        strItem* clsStorage::getresult(const strItem* (clsSKU::*f)() const) const {
        /** ����� ������� � ���������� ��������� �� ������ � ��������, ������� �������� � ������ ���������������
        ��� ���� ������������ �������� ��� ������� ��� �������� � ����������� �� ��������� � �������� ���������
        ������� f ���� const strItem* (clsSKU::*)() const (GetPurchase - ��������� �� �������, ������������ ������
        � ���������, GetBalance - � ���������, GetShipment - � ����������). **/
            size_t RMCount = stock.size();                  // �������� ���������� ������� ����� � ����������
            size_t tcount = RMCount * PrCount;              // ������ ������������ �������
            if(tcount == sZero) return nullptr;             // ��������� ����������
            strItem* temp = new(nothrow) strItem[tcount];   // �������� ������ �������
            if(!temp) {return nullptr; };                   // ���� ������ �� ��������, ���������� nullptr
            size_t i = sZero;                                   // ������ SKU
            for(clsSKU val: stock) {                            // ���� �� ���� SKU
                var_cpy((temp+i*PrCount), (val.*f)(), PrCount); // �������� ������ � ����� ������
                i++;
            };
//            for(clsSKU val: stock) {                            // ���� �� ���� SKU
//                for(size_t j=sZero; j<PrCount; j++) {           // ���� �� ���� ��������
//                    *(temp+PrCount*i+j) = *((val.*f)() + j);    // �������� ������ � ����� ������
//                };
//                i++;
//            };
            return temp;
        }   // getresult

        void clsStorage::_setdataitem(size_t i, const auto val, void (clsSKU::*f)(const auto)) {
        /** 2024.11.01 ��� SKU � �������� i ����� ������������� �����: ������������, ������� ���������,
        ���������� �� �������� � ������� � ����� � ��� �� �������, ���� �����������/ ������� �������
        �������, �������� ������ �����. const auto val - ����� ��������������� ��������. ��� ������������
        � ��.��������� ��� const string&; ��� ���������� �� �������� � ������� const bool; ��� �����
        ����������� const PurchaseCalc; ��� ��������� const decimal (������������ �����). **/
            if(i >= stock.size()) return;                   // ��������� ���������
            vector<clsSKU>::iterator it = stock.begin()+i;  // ������������� �������� �� i-� �������
            ((*it).*f)(val);                                // �������� ������ ������� � ���������� ������� ����
        }   // _setdataitem

        clsStorage::clsStorage() {
        /** ������ �����������. **/
            PrCount = sZero;
            hmcur = RUR;
            acct = FIFO;
            Calculation_Exit.store(false);
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsStorage Empty Ctor" << endl;
            #endif
        }   // clsStorage Empty Ctor

        clsStorage::clsStorage(size_t _pcnt, Currency _cur, AccountingMethod _ac) {
        /** ����������� � ����������� **/
            PrCount = _pcnt;
            hmcur = _cur;
            acct = _ac;
            Calculation_Exit.store(false);
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsStorage Ctor with parametrs" << endl;
            #endif
        }   // clsStorage Ctor with parametrs

        clsStorage::clsStorage(size_t _pcnt, Currency _cur, AccountingMethod _ac, size_t stocksize) {
        /** ����������� � ��������� ������� ��������� ������� **/
            PrCount = _pcnt;            // ���������� �������� �������
            hmcur = _cur;               // ������ �������
            acct = _ac;                 // ������� ����� �������
            Calculation_Exit.store(false);
            stock.reserve(stocksize);   // ����������� ������ ��� stocksize ��������� �������
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsStorage Ctor with parametrs and size of vector" << endl;
            #endif
        }   // clsStorage Ctor with parametrs and size of vector

        clsStorage::clsStorage(const clsStorage& obj) {
        /** ����������� ����������� **/
            PrCount = obj.PrCount;
            hmcur   = obj.hmcur;
            acct    = obj.acct;
            Calculation_Exit.store(false);
            stock   = obj.stock;
            pshell = obj.pshell;
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsStorage Copy Ctor" << endl;
            #endif
        }   // Copy Ctor

        clsStorage::clsStorage(clsStorage&& obj) {
        /** ����������� ����������� **/
            PrCount = sZero;
            hmcur   = RUR;
            acct    = FIFO;
            Calculation_Exit.store(false);
            pshell = nullptr;
            swap(obj);
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsStorage Move Ctor" << endl;
            #endif
        }   // Move Ctor

        void clsStorage::swap(clsStorage& obj) noexcept {
        /** ������� ������ ���������� ����� ��������� **/
            std::swap(PrCount, obj.PrCount);
            std::swap(hmcur, obj.hmcur);
            std::swap(acct, obj.acct);
            std::swap(pshell, obj.pshell);
            stock.swap(obj.stock);
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsStorage::swap" << endl;
            #endif
        }   // swap

        clsStorage& clsStorage::operator=(const clsStorage& obj){
        /** ���������� ��������� ������������ ������������. ����������� � ������
        �����������-�-������ (copy-and-swap idiom) **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsStorage tmp(obj);                // �������� ����������� ����������� � �������� ����� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsStorage Copy operator=" << endl;
            #endif
            return *this;
        }   // Copy operator=

        clsStorage& clsStorage::operator=(clsStorage&& obj) {
        /** ���������� ��������� ������������ ������������. ����������� � ������ �����������-�-������
        (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // ���� ������� ���������, �� ������ �� ������
            clsStorage tmp(move(obj));          // �������� ����������� ����������� � �������� ���������� obj � ���������� tmp
            swap(tmp);                          // ������������ ����������� � tmp
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "clsStorage Move operator=" << endl;
            #endif
            return *this;
        }   // Move operator=

        clsStorage::~clsStorage() {
            #ifdef DEBUG    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                cout << "Dtor clsStorage" << endl;
            #endif
        }        // ~clsStorage

        /** Get-������ **/
        const size_t& clsStorage::ProjectCount() const { return PrCount; }  // ���������� ���������� �������� �������

        size_t clsStorage::Size() const { return stock.size(); }    // ���������� ������ �������, ������ ���������� SKU

        const string& clsStorage::Name(size_t i) const {            // ���������� ������������ SKU ��� �������� � �������� i
            if(i >= stock.size()) { return EmpStr; };
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->Name();
//            return stock.at(i).Name();
        }
        const string& clsStorage::Measure(size_t i) const {                 // ���������� ������� ��������� SKU ��� �������� � �������� i
            if(i >= stock.size()) { return EmpStr; };
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->Measure();
//            return stock.at(i).Measure();
        }

        strNameMeas* clsStorage::GetNameMeas() {    // ������� ������� 25.03.2024
        /** ����� ���������� ��������� �� ����� ����������� ������ ���� strNameMeas � ��������� ����� �
        ��������� ��������� ��� ���� ������� **/
            size_t RMCount = stock.size();                          // �������� ���������� ������� ����� � ����������
            if(RMCount == sZero) return nullptr;                    // ���� ����, �� ����� � nullptr
            strNameMeas* temp = new(nothrow) strNameMeas[RMCount];  // �������� ������ �������
            if(!temp) {return nullptr; };                           // ���� ������ �� ��������, ���������� nullptr
            for(size_t i=sZero; i<RMCount; i++) {                   // ���� �� ��������� �������
                (temp+i)->name = Name(i);
                (temp+i)->measure = Measure(i);
            }
            return temp;
        }   // GetNameMeas

        string clsStorage::Permission(size_t i) const {
        /** ���������� ���������� �� �������� � ������� ��� i-�� SKU � ���� ��������� ������ **/
            if(i >= stock.size()) { return EmpStr; };
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->Permission();
        }
        string clsStorage::AutoPurchase(size_t i) const {
        /** ���������� ������� ����������� ������� ��� i-�� SKU � ���� ��������� ������ **/
            if(i >= stock.size()) { return EmpStr; };
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->AutoPurchase();
        }
        const size_t clsStorage::GetAutoPurchase(size_t i) const {
        /** ���������� ������� ����������� ������� ��� i-�� SKU **/
            if(i >= stock.size()) { return 2; } // ���� �������� � ����� �������� ���, ������������ �������� ������������ �����
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->GetAutoPurchase();
        }
        string clsStorage::HomeCurrency() const {       // ���������� �������� ������ ������� � ���� ��������� ������
            return *(nmBPTypes::CurrencyTXT+hmcur);
        }   // HomeCurrency

        string clsStorage::Accounting() const {         // ���������� ������� ����� � ���� ��������� ������
            return *(AccountTXT+acct);
        }   // Accounting

        const decimal& clsStorage::Share(size_t i) const {
        /** ���������� �������� ������ ����� � ����� �� ������ �������� **/
            if(i >= stock.size()) { return dZero; }
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->Share();
        }

        decimal* clsStorage::GetShipPrice() {
        /** ����� ������� ������ � ������� �������������� ����� � ���������� ��� ���� ������������� � ����������
        ��������� �� ����. ������ �������� ���������� �������� ������� �������� stock.size()*PrCount. **/
            size_t RMCount = stock.size();                  // �������� ���������� ������� ����� � ����������
            size_t tcount = RMCount * PrCount;              // ������ ������������ �������
            if(tcount == sZero) return nullptr;             // ��������� ����������
            decimal* temp = new(nothrow) decimal[tcount];   // �������� ������ �������
            if(!temp) {return nullptr; };                   // ���� ������ �� ��������, ���������� nullptr
            size_t i = sZero;
            for(vector<clsSKU>::const_iterator cit = stock.cbegin(); cit < stock.cend(); cit++) {
                for(size_t j=sZero; j<PrCount; j++) {
                    *(temp+PrCount*i+j) = (cit->GetShipment()+j)->price;
                };
                i++;
            };
            return temp;
        }   // GetShipPrice

        strItem* clsStorage::GetShip() const {
        /** ����� ������� ������ � ��������, ������� �������� � ������ ��������������� ��� ���� ������������ �������� �
        ���������� ��������� �� ����. ������ �������� ���������� �������� ������� �������� stock.size()*PrCount.**/
            return getresult(&clsSKU::GetShipment);
        }   // GetShip

        strItem* clsStorage::GetPure() const {
        /** ����� ������� ������ � ��������, ������� �������� � ������ ��������������� ��� ���� ������������ ������� �
        ���������� ��������� �� ����. ������ �������� ���������� �������� ������� �������� stock.size()*PrCount **/
            return getresult(&clsSKU::GetPurchase);
        }   // GetPure

        strItem* clsStorage::GetBal() const {
        /** ����� ������� ������ � ��������, ������� �������� � ������ ��������������� ��� ���� ������������ �������� �
        ���������� ��������� �� ����. ������ �������� ���������� �������� ������� �������� stock.size()*PrCount **/
            return getresult(&clsSKU::GetBalance);
        }   // GetBal

        const strItem* clsStorage::GetDataItem(const ChoiseData _cd, size_t index, size_t _N) const {
        /** ����� ���������� ������ ��� SKU � �������� index � �������� _N. ������ ������������ � ���� const-���������
        �� ��������� ���� strItem, ���������� ���������� �� ������� ������� ��� ��������, ����� � ���������; �����
        ������� ������������ ��������� ���������� _cd. **/
            if(index >= stock.size()) return nullptr;                   // ��������� ���������
            vector<clsSKU>::const_iterator cit = stock.cbegin()+index;  // ������������� �������� �� ������� � �������� index
            return cit->GetDataItem(_cd, _N);                           // ���������� ��������� �� ������
        }   // GetDataItem


        /** Set-������ **/

        void clsStorage::Set_progress_shell(clsProgress_shell<type_progress>* val) {
        /** ������� ����������� ��������� pshell ����� ������� val **/
            pshell = val;
        }   // Set_progress_shell

        void clsStorage::SetCount(const size_t _n) {PrCount = _n; }     // ������������� ���������� �������� �������

        void clsStorage::SetName(size_t i, const string& _name) {
        /** ������ ������������ �������������� ������� (SKU) ��� �������� � �������� i **/
            _setdataitem(i, _name, &clsSKU::SetName);
        }   // SetName

        void clsStorage::SetMeasure(size_t i, const string& _measure) {
        /** ������ ��.��������� �������������� ������� (SKU) ��� �������� � �������� i **/
            _setdataitem(i, _measure, &clsSKU::SetMeasure);
        }   // SetMeasure

        void clsStorage::SetCurrency(const Currency _cur) {
        /** ������������� �������� ������ ������� **/
             hmcur = _cur;
        }   // SetCurrency

        void clsStorage::SetPermission(size_t i, const bool _indr) {
        /** ������ ���������� �� �������� � ������� � ����� � ��� �� ������� ��� �������� � �������� i**/
            _setdataitem(i, _indr, &clsSKU::SetPermission);
        }   // SetPermission

        void clsStorage::SetAutoPurchase(size_t i, const PurchaseCalc _pcalc) {
        /** ������ ���� �����������/ ������� ������� ������� ��� �������� � �������� i **/
            _setdataitem(i, _pcalc, &clsSKU::SetAutoPurchase);
        }   // SetAutoPurchase

        void clsStorage::SetShare(size_t i, const decimal _share) {
        /** ������ �������� ������ ����� � ����� �� ������ �������� � ������� ��� �������� � �������� i **/
            _setdataitem(i, _share, &clsSKU::SetShare);
        }   // SetShare

        void clsStorage::SetAccounting(const AccountingMethod _acct) { acct = _acct; }  /** ������������� ������� ����� ������� **/

        bool clsStorage::SetDataItem(const ChoiseData _cd, const strItem _U, size_t index, size_t _N) {
        /** ������� ���������� � ������� ������� � �������� index � ������ � �������� _N �������� volume, price � value
        �� ���������� _U (��������� ������); ����� ������� ������������ ��������� ���������� _cd. ����� �������� ������
        � �������� ��������. � ������ �������� ������, ����� ���������� true. **/
            if(index >= stock.size()) return false;             // ��������� ���������
            vector<clsSKU>::iterator it = stock.begin()+index;  // ������������� �������� �� i-� �������
            return it->SetDataItem(_cd, _U, _N);                // ���������� ������ � ��������� ������� �������
        }   // SetDataItem

        bool clsStorage::SetSKU(const string& Name, const string& Measure, PurchaseCalc _flag, decimal _share, bool _perm,\
            strItem _ship[], strItem _pur[]) {
        /** ����� �������� ������ ��� ����������� SKU. ������� ����� ��������� ������ cksSKU �������������� � �������.
        ���������: Name - ������������ SKU, Measure - ������� ��������� SKU, _flag - ���� ����������� ���������� �������,
        _share - ����� �������� �� ������ � ������ ������, ���������� � ���� �� ������ �������� �� ���� ������,
        _perm - ���� �����������/����������� ������� � �������� � ����� �������, _ship - ������ ��������, _pur - ������
        �������. ����� �������� ��� � ������, ��� � �� ������ ��������. **/
            if(_flag==nocalc) {                             // ���� ���������� ������ ������ �������, �� ���������
                if(!CheckPurchase(_pur)) { return false; }  // ������������. ���� ������ �����������, �� ����� � ������� false
            };
            vector <clsSKU>::iterator it;                   // �������� ��� �������
            it = find(stock.begin(), stock.end(), Name);    // ����� ������� �� �����
            if(it != stock.end()) { return false; };        // ���� ������ � ����� ������ ��� ����, �� ����� � ������� false
            stock.emplace_back(PrCount, Name, Measure, acct, _perm, _flag, _share, _ship);  // ���� ��� ���������, �� �������
                                                                                            // ����� ��� ������� SKU
            if(_pur) {                                          // ���� ������ ������� �� ����� nullptr, ��
                return stock.back().SetPurchase(_pur, PrCount); // � ������ �������
            };
            return true;
        }   // SetSKU

        bool clsStorage::SetPurchase(size_t isku, const strItem _unit[], size_t _count) {
        /** ����� ��������� ������ ������� _unit �������� _count � �������������� ����� � ������� isku. **/
            if((!_unit) || (isku >= stock.size()) || (_count==sZero)) return false;;    // ��������� ����������
            if(!CheckPurchase(_unit)) { return false; }                                 // �������� ������������ �������
            vector <clsSKU>::iterator it = stock.begin() + isku;    // ��������� ��������� �� SKU � �������� isku
            return it->SetPurchase(_unit, _count);                  // ��������� ������ �������
        }   // SetPurchase

        bool clsStorage::SetPurPrice(const strItem _unit[]) {
        /** ����� ��������� ���� �� ����������� �������, ����������� �������� ������� � ������� ������� ���� ������� �����
        � �����������, �������� � ������ ����������� ���������. ������ ������� RMCount*PrCount. ������ ������� ������� �����
        ��� strItem, �.�. ����� � ����� ������� �������� volume, price � value ��� ��������. ������������ ������ ���� price. **/
            if(!_unit) return false;    // ��������� ����������
            size_t i=sZero;             // �������
            for(vector<clsSKU>::iterator it = stock.begin(); it < stock.end(); it++) {  // �� ������� ������ ��������� ����,
               if(!it->SetPurPrice((_unit+PrCount*i), PrCount)) return false;   // ��� �������� �� �������, �� ����� � false
               i++;
            };
            return true;
        }   // SetPurPrice

        bool clsStorage::CheckPurchase(const strItem _pur[]) const {
        /** ��� �����, ����������� �������������� ������ �������, ������ ������� ������ ��������� ���������� �����, �.�
        ������������ volume * price == value. ���� ��� ������� �� �����������, ��������� ������������� ����� �����
        ��������� �����������. ������ ����� ��������� ������������ ������� �������. **/
            if(!_pur) return false;                 // ���� ������ �� ����������, ������� false
            for(size_t i=sZero; i<PrCount; i++) {
                if( fabs(((_pur+i)->volume * (_pur+i)->price) - (_pur+i)->value) > epsln ) {
                    return false;
                }
            };
            return true;
        }   // CheckPurchase

        bool clsStorage::SetStorage(size_t RMCount, const strNameMeas RMNames[], decimal ShipPlan[], decimal PricePur[]) {
        /** ����� ����� �������� ������ � �������, ����������� � �������� �������� strNameMeas* clsManufactory::GetRefRawNames
        � clsManufactory::decimal* GetRawMatPurchPlan(). ����� �������� "�������" ���������� ������ clsStorage, ������ �����
        ������� ������ ��� ��������� ��������� � ����������, ������ ���������� ������� ����� � ����������, ��������� ���
        ���������������� ��������� ������� ���� clsManufactory (����� clsManufactory::RMCount. ���������: RMCount - �����
        �������������� ������� ����� � ����������, RMNames - ������ � ���������� ����� � ����������, ������������ RMCount;
        ������ �������� � ������� ������������ ������� ���������. ShipPlan - ������ �������� ����� � ���������� � ������������
        ��� ������� ������������ ����� � ������ ������� �������; ������ ������������ ����� ���������� ������ ��������� �������
        �������� RMCount*PrCount. ��� ������� � �������� � �������� i,j ������������ ���������� ����������: *(X+cols*i+j),
        ��� X - ��������� �� ������ ������� �������, cols - ����� �������� � �������, i � j - �������������� ����� ������ �
        ������� � �������. PricePur - ������ ��� ���������� ����� � ���������� �������� RMCount*PrCount.
        ���� ����� ������������� ��� ���� ������� ������� ���� ����������� �������������� ������ ������� (calc),
        ������� ����� ����� � ���������� �� ������ � ������ ������ � ���� ����������� ������� � �������� � ����� ������� (true);
        � ���������� ��� ��������� ����� ��������������� ��� ������� ������ �������� ������� �������� ������. ������ �����
        ������������� ��������� � ���� � ������������� � �����������, ��������������� ����� �������� �������, �������� ������
        � ������� ����� ������� (clsStorage(size_t _pcnt, Currency _cur, AccountingMethod _ac)).
        ��������!!!  ������ ShipPlan - ������������, ����� �� ������������� ������� �������� � ������
        ������ ���������� delete[].**/
            if((RMCount==sZero) || (!RMNames) || (!ShipPlan) || (!PricePur)) return false;  // ��������� ������� ����������
            if(!stock.empty()) stock.clear();                                   // ���� ������ �� ������, �� ������� ��� ��������
            stock.reserve(RMCount);                                             // ����������� ������ ��� �������
            strItem *tship, *tppur;                                             // ��������������� ������
            for(size_t i=sZero; i<RMCount; i++) {                               // ���� �� ������������ �����
                tship = new(nothrow) strItem[PrCount];                          // ����������� ������ �������
                if(!tship) return false;                                        // ���� ������ �� ��������, �� ����� � false
                tppur = new(nothrow) strItem[PrCount];                          // ����������� ������ �������
                if(!tppur) {                                                    // ���� ������ �� ��������, ��
                    delete[] tship;                                             // ������� ��������� ����� ������
                    return false;                                               // � ������� � false
                };
                for(size_t j=sZero; j<PrCount; j++) {                           // �������� �� ��������������� ����������
                    (tship+j)->volume = *(ShipPlan+PrCount*i+j);                // ���� �������� ��� j-�� �����
                    (tship+j)->price = (tship+j)->value = dZero;                // ���� price � value ��������� ������
                    (tppur+j)->price = *(PricePur+PrCount*i+j);                 // ���������� ���� ����� � ����������
                    (tppur+j)->volume = (tppur)->value = dZero;                 // ���� volume � value ��������� ������

                };
                /** ������� ����� ��� i-�� ����� **/
                stock.emplace_back(PrCount, (RMNames+i)->name, (RMNames+i)->measure, acct, true, calc, dZero, tship);
                bool rez = stock.back().SetPurchase(tppur, PrCount);    // ������ �������
                delete[] tship;                                         // ������� ��������������� ������
                delete[] tppur;                                         // ������� ��������������� ������
                if(!rez) return false;                                  // ���� ������ ������� �� ������, ����� � false
            };
            return true;
        }   // SetStorage

        bool clsStorage::SetStorage(size_t RMCount, const strNameMeas RMNames[], strItem ShipPlan[], strItem Purchase[]) {
        /** ����������� �����������, �� ������� �������� � ������� � ������� strItem, ����������� � ���� � ������.
        ����� ��������� nullptr ������ ������ �� ������ �������. **/
            if((RMCount==sZero) || (!RMNames) || (!ShipPlan)) return false;     // ��������� ������� ����������
            if(!stock.empty()) stock.clear();                                   // ���� ������ �� ������, �� ������� ��� ��������
            stock.reserve(RMCount);                                             // ����������� ������ ��� �������
            for(size_t i=sZero; i<RMCount; i++) {                               // ���� �� ������������ �����
                /** ������� ����� ��� i-�� ����� **/
                stock.emplace_back(PrCount, (RMNames+i)->name, (RMNames+i)->measure, acct, true, calc, dZero, (ShipPlan+i*PrCount));
                if(Purchase)
                    stock.back().SetPurchase((Purchase+i*PrCount), PrCount); // ������ �������
            };
            return true;
        }   // SetStorage 2024.12.10

        bool clsStorage::SetData(const clsSKU &obj) {
        /** ������� ������� � ������ ������ ���������� � ���������� SKU ����� ����������� ���������� ������ clsSKU � ������ **/
            vector <clsSKU>::iterator it;                       // �������� ��� �������
            it = find(stock.begin(), stock.end(), obj.Name());  // ����� ������� �� �����
            if(it != stock.end()) { return false; };            // ���� ������ � ����� ������ ��� ����, �� ����� � ������� false
            stock.emplace_back(obj);                            // ���� ��� ���������, �� ������� ����� ��� ������� SKU
            return true;
        };

        bool clsStorage::SetData(clsSKU &&obj) {
        /** ������� ������� � ������ ������ ���������� � ���������� SKU ����� ����������� ���������� ������ clsSKU � ������ **/
            vector <clsSKU>::iterator it;                       // �������� ��� �������
            it = find(stock.begin(), stock.end(), obj.Name());  // ����� ������� �� �����
            if(it != stock.end()) { return false; };            // ���� ������ � ����� ������ ��� ����, �� ����� � ������� false
            stock.emplace_back(move(obj));                      // ���� ��� ���������, �� ������� ����� ��� ������� SKU
            return true;
        };

        void clsStorage::EraseSKU(size_t _i) {
        /** ����� ������� �������������� ������� ��� ������� _i � ��� ��������� � ��� ������ **/
            stock.erase(stock.begin() + _i);
        }   // EraseSKU

        /** ������ ����������� �������� **/

        void clsStorage::ViewSettings() const {
        /** ������� ������� �� ����� ���������� � ���������� ��� �������. � ���������� ��������� ���������:
        ������������ �������, �������� (��������) ������, ����� ����� �������, ����������/������ �� �������
        � �������� � ����� �������, ������� ����/ ������� ������� �������, �������� ������ ��������. **/
            nmPrntSrvs::clsTextField fld(15);
            cout << fld << "Name" << fld << "Measure" << fld << "Accounting" << fld << "Permission" << \
            fld << "AutoPurchase" << "Share" << endl;
            for(vector <clsSKU>::const_iterator cit = stock.cbegin(); cit < stock.cend(); cit++) {
                string _Name = cit->Name();
                _Name.resize(15);
                string _Meas = cit->Measure();
                _Meas.resize(8);
                string _APurch = cit->AutoPurchase();
                _APurch.resize(8);
                cout << fld << _Name << fld << _Meas << fld << cit->AccMethod() << \
                fld << cit->Permission()<< fld << _APurch << cit->Share() << endl;
            }
        }   // ViewSettings

        void clsStorage::View() const {
        /** ������� ������� �� ����� ���������� �� ����� ������ **/
            vector <clsSKU>::const_iterator it;   // �������� ��� �������
            it = stock.cbegin();
            while(it != stock.cend()) {
                it->View();
                it++;
            }
        }

        void clsStorage::ViewChoise(ChoiseData _arr, ReportData flg) const {
        /** ����� ����������� �������� ���������� ������� ��� ���� �������. ��������� �������:
        _arr = purchase, ������ ����������� �� �����; _arr = balance, ������ �������� �� ������;
        _arr = shipment, ������ �������� ��� �����. �������� flg - ��� ��������� ����������:
        volume - � �����������, value - � �����������, price - � ������� ��������� **/
            size_t ProdNameCount = Size();              // ���������� ��������� � ������
            size_t tmpcount = ProdNameCount*PrCount;    // ��������������� ����������
            if(tmpcount == sZero) return;               // ��������� ����������
            const strItem* (clsSKU::*f)() const;        // ���������� ��������� �� ���������� ������� ������ clsSKU
            if(_arr==purchase) f = &clsSKU::GetPurchase;    // ���������� ��������� �������
            else                                            // �������� ��������� �� ������ ����������� �� �����
                if(_arr==balance) f = &clsSKU::GetBalance;  // �������� ��������� �� ������ �������� �� ������
                else f = &clsSKU::GetShipment;              // �������� ��������� �� ������ �������� �� ������
            nmBPTypes::strNameMeas* pNames = new(nothrow) nmBPTypes::strNameMeas[ProdNameCount];    // ��������� ������ ����
            if(!pNames) return;                 // ���� ������ �� ��������, �� ����� �� ������
            nmBPTypes::strItem* pData = new(nothrow) nmBPTypes::strItem[tmpcount];                  // ��������� ������ ������
            if(!pData) {                        // ���� ������ �� ��������, ��
                delete[] pNames;                // ������� ������ ����
                return;                         // � ������� �� ������
            };
            for(size_t i=sZero; i<ProdNameCount; i++) {     // ���� �� ���� �������������� �������
                (pNames+i)->name = stock[i].Name();         // ��������� ������ �������
                (pNames+i)->name.resize(15);                // ������� ����� ������
                (pNames+i)->measure = stock[i].Measure();   // ��������� ������ �������������������� ���������
                (pNames+i)->measure.resize(8);              // ������� ����� ������
                var_cpy((pData+i*PrCount), (stock[i].*f)(), PrCount); // �������� ������
            };
            nmPrntSrvs::ArrPrint(ProdNameCount, pNames, pData, PrCount, flg);                   // ������� ����� �� �����
            if(pNames) delete[] pNames;                 // ������� ��������������� ������
            if(pData) delete[] pData;                   // ������� ��������������� ������
        }   // ViewChoise

        /** ��������� ������ **/

        TLack clsStorage::Calculate(size_t bg, size_t en) {
        /** ������� ��������� ���������� �������� � ������� SKU: ���� ��� ���������� �� ����������, �������������� �
        ���������� PRCount, �� ��� ����� SKU ���������� ������� Resize, ������� �������� ���������� �������� � ���������.
        ������� ����� ������������ ��������� ����� ������� ��� ��� SKU, � ������� ��������� ����������� ����� ������ ����.
        ��� ������� SKU ������� ������������ ���� � ��������� ����������� ������ � ������������ � ��������� ��������� FIFO,
        LIFO ��� AVG. ������� ��������� ������ ��� ���������� ������� �������� � �������� � ������� ����� �������� �
        ������������ SKU, ��� ������ �������. ���������� ������� �� ������������. ���������� ������������ � ���� ���������
        ���� TLack. ��� ������� ������������ � ��� SKU, ��� ������� � ������� stock ����� ����� bg � en. **/
            TLack Lack;                         // ��������������� ����������
            size_t lim = stock.size();          // ������� ������� ���������� �������� �������
            if(bg>=lim) return Lack;            // ��������� ������ ������� �������
            if(en>=lim) en = lim;               // ��������� ������� ������� �������
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // ���������� �������
            for(size_t i = bg; i<en; i++) {
                if(Calculation_Exit.load(memory_order_relaxed)) {                   // ���� ���������� ���� ��������,
                    (pshell) ? pshell->Counter_max() : (void)([](){return;});       // �� ������������� �������
                    break;                      // � ������� �� �����
                }
                clsSKU* p = (stock.data()+i);   // ��������������� ��������� �� ��������� ������� SKU
                p->SetAccount(acct);            // ������������� ������� ����� �������
                if(p->Count() != PrCount)       // ���� ������������ �������� �� SKU ���������� �� PrCount
                    p->Resize(PrCount);         // ����������� ������������
                Lack = p->Calculate();          // �������� ����������
                (pshell) ? pshell->Counter_inc() : (void)([](){return;});       // �������� �������
                if(fabs(Lack.lack) > epsln) {                                   // � ���� ������������ �������, ��
                    Calculation_Exit.store(true, memory_order_relaxed);         // ������������� ���� ������ ��� ������
                    (pshell) ? pshell->Counter_max() : (void)([](){return;});   // ������� � ������������� �������
                    return Lack;    // � ����������� ����, ��������� ��� ������� �� ������������� � ������� �� �������
                };
            };
            return {dZero, EmpStr};
        }   // void Calculate(size_t, size_t)

        TLack clsStorage::Calculate() {
        /** ������� ��������� ���������� �������� � ������� SKU: ���� ��� ���������� �� ����������, �������������� �
        ���������� PRCount, �� ��� ����� SKU ���������� ������� Resize, ������� �������� ���������� �������� � ���������.
        ������� ����� ������������ ��������� ����� ������� ��� ��� SKU, � ������� ��������� ����������� ����� ������ ����.
        ��� ������� SKU ������� ������������ ���� � ��������� ����������� ������ � ������������ � ��������� ��������� FIFO,
        LIFO ��� AVG. ������� ��������� ������ ��� ���������� ������� �������� � �������� � ������� ����� �������� �
        ������������ SKU, ��� ������ �������. ���������� ������� �� ������������. ���������� ������������ � ���� ���������
        ���� TLack. ������� ������������ �� ���� SKU. **/
            TLack Lack;                         // ��������������� ����������
            for(size_t i{}; i<stock.size(); i++) {
                clsSKU* p = (stock.data()+i);   // ��������������� ��������� �� ��������� ������� SKU
                p->SetAccount(acct);            // ������������� ������� ����� �������
                if(p->Count() != PrCount)       // ���� ������������ �������� �� SKU ���������� �� PrCount
                    p->Resize(PrCount);         // ����������� ������������;
                Lack = p->Calculate();          // �������� ����������
                (pshell) ? pshell->Update((int)i) : (void)([](){return;});    // �������� ��������� ���������
                if(fabs(Lack.lack) > epsln) {   // � ���� ������������ �������, ��
                    (pshell) ? pshell->Update((int)stock.size()) : (void)([](){return;});    // ��������� �� 100%,
                    return Lack;    // ����������� ����, ��������� ��� ������� �� ������������� � ������� �� �������
                };
            };
            (pshell) ? pshell->Update((int)stock.size()) : (void)([](){return;});    // ��������� �� 100%
            return {dZero, EmpStr};
        }   // Calculate1

        TLack clsStorage::Calculate_future() {
        /** ������� ��������� ���������� �������� � ������� SKU: ���� ��� ���������� �� ����������, �������������� �
        ���������� PRCount, �� ��� ����� SKU ���������� ������� Resize, ������� �������� ���������� �������� � ���������.
        ������� ����� ������������ ��������� ����� ������� ��� ��� SKU, � ������� ��������� ����������� ����� ������ ����.
        ��� ������� SKU ������� ������������ ���� � ��������� ����������� ������ � ������������ � ��������� ��������� FIFO,
        LIFO ��� AVG. ������� ��������� ������ ��� ���������� ������� �������� � �������� � ������� ����� �������� �
        ������������ SKU, ��� ������ �������. ���������� ������� �� ������������. ���������� ������������ � ���� ���������
        ���� TLack. ��� ������� ����������� � ����������� �������, ����� ������� �� ������� ������ ����� ���� ����������. **/
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // ����������� ����. ����� ������� ����� ����� ����
            if(maxthreads < sTwo)                                           // ���� ����. ����� ������� ����� ����, ��
                return Calculate();                                         // �������� ���������� ������� Calculate � �������
            vector<std::future<TLack>> pool;            // ��� ������� � ���� �������
            pool.reserve(maxthreads);                   // ����������� ������ ������� �������
            size_t stocksize = stock.size();            // �������� ������ ������� � SKU
            size_t ncap = (stocksize/maxthreads)+sOne;  // ���������� SKU ��� ������� ������
            TLack Lack[maxthreads]{};                   // ��������������� ������ � ������������ ����������
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // ���������� �������
            for(size_t i{}; i<maxthreads; i++) {        // ���� �� ����� ���� �������
                size_t bg = i*ncap;                     // ���������� ������ ������� �������
                if(bg>=stocksize) break;                // ���� ������ ������� ������ ��� ����� ����� SKU, ������� �� �����
                size_t en = (i+sOne)*ncap;              // ���������� ������� ������� �������
                pool.push_back(std::async([](clsStorage* pobj, size_t _bg, size_t _en) {    // ������� ����� � ��������� ����������
                    // � �������� ������� ��������� ������� async ���������� ������ "[](){}"
                    #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                        mu.lock();                          // ��������� ������ cout ��� ������������ �������������
                        cout << "Async thread #" << std::this_thread::get_id() << " has been started" << endl;
                        mu.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                    #endif
                    TLack res = pobj->Calculate(_bg, _en);  // �������� ����� � ������������
                    #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                        mu.lock();                          // ��������� ������ cout ��� ������������ �������������
                        cout << "Async thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                        mu.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                    #endif
                    return res;
                }, this, bg, en));                      // ����� ������ ����� ��������� ��� ���: this, bg, en
            };                                          // ���� �� ����� ���� �������
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;});   // �������� ��������� ����������
            size_t k{};
            for(auto &t : pool) {
                *(Lack+k) = t.get();
                k++;
            };
            Calculation_Exit.store(false, memory_order_relaxed);    // ���������� ����, ����� ����� ���� ����� ������� Calculate(size_t
            for(size_t i{}; i<maxthreads; i++ )         // ���� �� ���� ��������� ���������������� ������� � ������������
                if((Lack+i)->lack>epsln)                // ���� ��������� �������, ��
                    return *(Lack+i);                   // ������� � ���������� �������� �������� � ������������ SKU
            return {dZero, EmpStr};                     // ������������� ���������� � ����� ��� ��������
       }    // Calculate2

        TLack clsStorage::Calculate_thread() {
        /** ������� ��������� ���������� �������� � ������� SKU: ���� ��� ���������� �� ����������, �������������� �
        ���������� PRCount, �� ��� ����� SKU ���������� ������� Resize, ������� �������� ���������� �������� � ���������.
        ������� ����� ������������ ��������� ����� ������� ��� ��� SKU, � ������� ��������� ����������� ����� ������ ����.
        ��� ������� SKU ������� ������������ ���� � ��������� ����������� ������ � ������������ � ��������� ��������� FIFO,
        LIFO ��� AVG. ������� ��������� ������ ��� ���������� ������� �������� � �������� � ������� ����� �������� �
        ������������ SKU, ��� ������ �������. ���������� ������� �� ������������. ���������� ������������ � ���� ���������
        ���� TLack. ��� ������� ����������� � �������, ����� ������� �� ������� ������ ����� ���� ����������. **/
            auto do_Calculate = [](void *param, size_t bg, size_t en, TLack &res) {
            /** "�������" � ���� ������-������� ��� ������ TLack Calculate(size_t&, size_t&), ��������������� ��� �������
            ���������� ������ � ������ tread. **/
                clsStorage* pobj = (clsStorage*)param;  // ��������������� ��������� �� ��������� ������ clsStorage
                #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                    mu.lock();                          // ��������� ������ cout ��� ������������ �������������
                    cout << "Thread #" << std::this_thread::get_id() << " has been started" << endl;
                    mu.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                #endif
                res = pobj->Calculate(bg, en);          // �������� ����������
                #ifdef DEBUG_THREADS                    // ������ ������ ���������� ����������. ��������, ���� ��������� DEBUG
                    mu.lock();                          // ��������� ������ cout ��� ������������ �������������
                    cout << "Thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                    mu.unlock();                        // ������������ ������ cout ��� ������������� ������� ���������
                #endif
            };  // do_Calculate
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // ����������� ����. ����� ������� ����� ����� ����
            if(maxthreads < sTwo)                                           // ���� ����. ����� ������� ����� ����, ��
                return Calculate();                                         // �������� ���������� ������� Calculate � �������
            vector<std::thread> pool;                   // ��� ������� � ���� �������
            pool.reserve(maxthreads);                   // ����������� ������ ������� �������
            size_t stocksize = stock.size();            // �������� ������ ������� � SKU
            size_t ncap = (stocksize/maxthreads)+sOne;  // ���������� SKU ��� ������� ������
            TLack Lack[maxthreads]{};                   // ��������������� ������ � ������������ ����������
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // ���������� �������
            for(size_t i{}; i<maxthreads; i++) {        // ���� �� ����� ���� �������
                size_t bg = i*ncap;                     // ���������� ������ ������� �������
                if(bg>=stocksize) break;                // ���� ������ ������� ������ ��� ����� ����� SKU, ������� �� �����
                size_t en = (i+sOne)*ncap;              // ���������� ������� ������� �������
                pool.emplace_back(do_Calculate, this, bg, en, std::ref(*(Lack+i)));   // ������� ����� � ��������� ����������
            };  // ��� �������� �������� �� ������ ���������� �������� ������ std::ref(...), ��.:
                // https://pro--prof-com.turbopages.org/pro-prof.com/s/forums/topic/cplusplus_reference_wrapper
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;}); // �������� ��������� ����������
            for(auto &t : pool)                         // ���� �� ����� ���� �������
                t.join();                               // ������� ���������� ������� ����������� ������
            Calculation_Exit.store(false, memory_order_relaxed);// ���������� ����, ����� ����� ���� ����� ������� Calculate(size_t
            for(size_t i{}; i<maxthreads; i++ )         // ���� �� ���� ��������� ���������������� ������� � ������������
                if((Lack+i)->lack>epsln)                // ���� ��������� �������, ��
                    return *(Lack+i);                   // ������� � ���������� �������� �������� � ������������ SKU
            return {dZero, EmpStr};                     // ������������� ���������� � ����� ��� ��������
       }    // Calculate3

        bool clsStorage::StF(ofstream &_outF)  {
        /** ����� ������������� ������ � �������� ���������� �������� ���������� ������ (����� ������������).
        ���������: &_outF - ��������� ������ ofstream ��� ������ ������ **/
            size_t stocksize = stock.size();        // ���������� � ���������� ���������� ��������� �������
            size_t capac = stock.capacity();        // ���������� � ���������� ������� ������ (�������) �������
            if(stocksize == sZero) return false;    // ���� ������ ������, �� ����� � ������� false
            if(!SEF(_outF, PrCount)) return false;        // ��������� ���������� ��������
            if(!SEF(_outF, hmcur)) return false;          // ��������� �������� ������ �������
            if(!SEF(_outF, acct)) return false;           // ��������� ������� �����
            if(!SEF(_outF, capac)) return false;          // ��������� ������� ������� � ����
            if(!SEF(_outF, stocksize)) return false;      // ��������� ���������� ��������� ������� � ����
//            for(size_t i=sZero; i<stocksize; i++) {     // ��������� ������ ����������� ���� clsSKU
//                stock.at(i).StF(_outF);
//            };
            for(vector<clsSKU>::iterator it = stock.begin(); it < stock.end(); it++)
                if(!it->StF(_outF)) return false;         // ��������� ������ ����������� ���� clsSKU
            return true;                                  // ���������� true
        }   // StF

        bool clsStorage::RfF(ifstream &_inF) {
        /** ����� ������������� ������ �� �������� ���������� �������� ���������� ������ (����� ��������������).
        ���������: &_inF - ��������� ������ ifstream ��� ������ ������ **/
            size_t stocksize;           // ��������� ���������� � ������ ��������� �������
            size_t capac;               // ��������� ���������� � ������� ������ �������
            if(!DSF(_inF, PrCount)) return false;         // ������ ���������� ��������
            if(!DSF(_inF, hmcur)) return false;           // ������ �������� ������ �������
            if(!DSF(_inF, acct)) return false;            // ������ ������� �����
            if(!DSF(_inF, capac)) return false;           // ������ ������� �������
            if(!DSF(_inF, stocksize)) return false;       // ������ ���������� ��������� �������
            if(capac == sZero) return false;
            stock.reserve(capac);       // ����������� ������ ��� �������
            for(size_t i=sZero; i<stocksize; i++) {
                stock.emplace_back(PrCount, "", "", acct, true, calc, dZero);   // ������� ������� ��������������� � �������
                if(!stock.back().RfF(_inF)) return false;                       // �������� ����� �������������� ����� ��������
            };
            return true;                    // ���������� true
        }   // RfF

# undef DEBUG           // �������� ������ ���������� �� ����������� DEBUG
# undef DEBUG_THREADS   // �������� ������ ���������� �� ����������� DEBUG
