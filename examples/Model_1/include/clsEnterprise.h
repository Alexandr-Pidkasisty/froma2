/**     ��������� ��� ������� ������ � �������� ���������� ������ ����������� � ����� ������� ������,
        ��������� � �������������� ���������� "Free Operation Manager 2" (���������� FROMA2).
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

#ifndef CLSENTERPRISE_H
#define CLSENTERPRISE_H

#include "baseproject_module.h"             // ���������� ������� ����� clsBaseProject
#include "warehouse_module.h"               // ���������� ����� ������ clsStorage
#include "manufact_module.h"                // ���������� ����� ������������ clsManufactory
#include "common_values.hpp"                // ��������� �������
#include "Impex_module.h"                   // ������ �������� ������ �� ������
#include "pathes.h"                         // ���� � ������

const size_t w1 = 25, w2 = 12, w3 = 15;     // ������� ����� � �������
const size_t precis = 4;                    // ���������� ������ ����� ������� � �������
const size_t num_strings = 20;              // ���������� �����, ��������� �� ����� ������� showSKUsettings

const unsigned int ProgressWide = 75u;      // ������ ���������� ���������

const decimal cShare = 0.1;                 // ����� ��������/ ��������� �� ������ �� ���������

struct strSettings {                        // ��������� �������������� �������� ��� ������
    decimal share;                          // ����� �������� �� ������, ���� �� ��������
    PurchaseCalc calc;                      // ���� ����/ ������� ������� ����������� �� �����
    bool perm;                              // ���� ���������� �� ����������� � �������� �� ������ � ��� �� �������
    bool StF(ofstream &_outF) {
    /** ����� ������������ ��������� ������� � ���� **/
        if(!SEF(_outF, share)) return false;
        if(!SEF(_outF,  calc)) return false;
        if(!SEF(_outF,  perm)) return false;
        return true;
	}   // StF
    bool RfF(ifstream &_inF) {
    /** ����� �������������� ��������� ������� �� ����� **/
        if(!DSF(_inF, share)) return false;
        if(!DSF(_inF,  calc)) return false;
        if(!DSF(_inF,  perm)) return false;
        return true;
	}   // RfF
};  // strSettings

namespace nmEnterprise {
    enum SelectDivision{warehouse = sZero, manufactory, rowmatstock};   // ���� ������: ���, ������������, ����� �����
    enum ManufData{manpurchase = 11, manbalance, manshipment, recipe};  // ���� ������: ����������� � �����, ������, ��������, ���������
} // namespace nmEnterprise

string DBLR_ind();
/** ������� ���������� ������ �������� ������������� ���� ������������ �����: "DB" - ��� ������������
����� ������������ ��� double, "LR" - ��� ������������ ����� ������������ ��� LongReal. ������������
��� ���������� �������� � ����� ��������� ������. **/

void strItemReplace(size_t count, strItem* dstn, strItem* src, ReportData flg);
/** ������� �������� �������� �����, ��������� ������ flg � ������� dstn ���������� ����� �� �������
src � ��������� � ���������� �������� **/

template <typename T>
inline void inData(T &_data, const T _defdata); /** ����� ������ ������ ������������. ��� ����������
������, ������������� �������� �� ���������. ���������: &_data - ������ �� �������� ������������� ������,
_defdata - �������� �� ���������. ����������� � �������� ���������� &_data � _defdata ������������ ���
����� � ��� �� ����������, �������� inData(x, x), ��������� ������ �������� ���������� � ������� ��� �����. **/

/****************************************************************************************************/
/**                                 ��������� strImportConfig                                      **/
/****************************************************************************************************/

const string confdir = V_DIR_CONFIG;
const string Configure_filename = "config.cfg"; // ��� ����������������� �����

//"../examples/Model_1/config/config.cfg";  // ��� ����������������� �����
const string NoFileName = "nofile";         // ��� ��� �������������� �����
const string indir = V_DIR_INPUTDATA;       // ���� � ������������� ������

struct strImportConfig {
/** ����������� ��� ��� ����������/ ������ ����������������� ����� ��� ������� **/
    string filename_About;          // ��� ����� � ��������� �������
    string filename_Shipment;       // ��� ����� � ���������� � ��� � ����������� ���������
    string filename_Production;     // ����� ������������ � ����������� ���������
    string filename_Purchase;       // ��� ����� � ��������� �� ��� � ������ �������
    string filename_Purchase_V;     // ��� ����� � ��������� �� ��� � �������� �������
    string filenameprefix_Recipes;  // ������� ��� ��� ������ � �����������
    char _ch;                       // ������ ����������� ����� ������ � CSV_������
    size_t HeadCols;                // ���������� �������� � ����������� � CSV_������
    size_t HeadRows;                // ���������� ����� � ����������� � CSV_������
    Currency _cur;                  // �������� ������ �������
    AccountingMethod _amethod;      // ������� ����� �������
    decimal P_Share;                // ����� �������� �� ������ ���, ���� �� ��������
    decimal S_Share;                // ����� ��������� �� ������ ���, ���� �� ��������
    bool P_indr;                    // ���� ���������� �� �������� � ��� � ��� �� �������, ��� � �����������
    bool S_indr;                    // ���� ���������� �� �������� � ��� � ��� �� �������, ��� � �����������

    size_t PsetCount;               // ������ ������� P_settings;
    size_t SsetCount;               // ������ ������� S_settings;
    strSettings* P_settings;        // ��������� �� ������ � ��������������� ����������� ��� �������� PsetCount
    strSettings* S_settings;        // ��������� �� ������ � ��������������� ����������� ��� �������� S_settings

    strImportConfig();                                  // ����������� �� ���������
    strImportConfig(const strImportConfig&);            // ����������� �����������
    strImportConfig(strImportConfig&&);                 // ����������� �����������
    strImportConfig& operator=(const strImportConfig&); // �������� ������������ ������������
    strImportConfig& operator=(strImportConfig&&);      // �������� ������������ ������������
    ~strImportConfig();                                 // ����������

    bool SaveToFile(const string _filename);    // ������ ������������ � ����
    void swap(strImportConfig& obj) noexcept;   // ������� ������ ���������� ����� ���������
    bool ReadFromFile(const string _filename);  // ������ ������������ � �����
    void Entry();                               // ���� ������ � ���������
    void Show();                                // ���������� ������� ������������ �� ������
    void Configure();                           // ���������� �������������

    size_t GetPsetCount() const;                        // ���������� ������ ������� P_settings
    size_t GetSsetCount() const;                        // ���������� ������ ������� S_settings
    strSettings* GetPurSettings() const;                // ���������� ��������� �� ����� ����� ������� P_settings
    strSettings* GetShpSettings() const;                // ���������� ��������� �� ����� ����� ������� S_settings
    bool SetPurSettings(const strSettings* _P_settings, const size_t pcount);   // �������� ������ _P_settings � ������ P_settings
    bool SetShpSettings(const strSettings* _S_settings, const size_t scount);   // �������� ������ _S_settings � ������ S_settings

};  // strImportConfig

/****************************************************************************************************/
/**             ����� clsEnterprise. ��������� ������ ������� clsBaseProject                       **/
/****************************************************************************************************/

using namespace nmEnterprise;
class clsEnterprise : public clsBaseProject {
/** ������������ ����� ��� ����� ����������� - �������� ��� ������� clsStorage, clsManufactory �
��� �������� ������ ����� ��������� ���� ������� **/
    private:
        size_t PrCount;                 // ���������� �������� �������, �� ��� 0 -���������, � 1 �� 59 - ����������
        Currency Cur;                   // �������� ������ �������
        AccountingMethod Amethod;       // ������� ����� �������
        PurchaseCalc purcalc;           // ���� �����������/ ����������� �������������� ������� ������� ������� ��������
        PurchaseCalc mancalc;           // ���� �����������/ ����������� �������������� ������� ������� ������������
        decimal PurchShare;             // ����� �������� �� ������ ���, ���� �� ��������
        decimal ShipShare;              // ����� ��������� �� ������ ���, ���� �� ��������
        bool Purch_indr;                // ���� ���������� �� �������� � ��� � ��� �� �������, ��� � �����������
        bool Ship_indr;                 // ���� ���������� �� �������� � ��� � ��� �� �������, ��� � �����������

        size_t ProdCount;               // ������ ����� ����������� ���������
        strItem* Shipment;              // ��������� �� ������ �������� �� ������ ���; ������ ������� ProdCount*PrCount
        strItem* Production;            // ��������� �� ������ �������� �� ������������ �� ���; ������ ������� ProdCount*PrCount
        strNameMeas* ProdNames;         // ��������� �� ������ �������� � ��. ��������� ���������; ������ ������� ProdCount

        size_t RMCount;                 // ������ ����� ������������ ����� � ����������
        strItem* Consumpt;              // ��������� �� ������ ����������� �������� � ������������; ������ ������� RMCount*PrCount
        strItem* Purchase;              // ��������� �� ������ ����������� �������� �� ����� ���; ������ ������� RMCount*PrCount
        decimal* PriceBus;              // ��������� �� ������ ��������� �������� ��� ������������; ������ ������� RMCount*PrCount
        strNameMeas* RMNames;           // ��������� �� ������ �������� � ��. ��������� ��������; ������ ������� RMCount

        vector<clsRecipeItem> Recipe;   // ������ � ����������� ���������; ������ ������� ProdCount
        clsStorage* Warehouse;          // ��������� �� ����� ������� ��������� (���)
        clsManufactory* Manufactory;    // ��������� �� ������������
        clsStorage* RawMatStock;        // ��������� �� ����� ����� � ���������� / �������� (���)

        strSettings* P_settings;        // ��������� �� ������ � ��������������� ����������� ��� �������� RMCount
        strSettings* S_settings;        // ��������� �� ������ � ��������������� ����������� ��� �������� ProdCount

    public:
        clsEnterprise();                                        // ����������� �� ���������
        virtual void swap(clsEnterprise& other) noexcept;       // ������� ������ ���������� ����� ���������
        void EraseVector(vector<clsRecipeItem>& Recipe);        // ����� ��� ������ ������� ������� � �����������
        clsEnterprise(const clsEnterprise& other);              // ����������� �����������
        clsEnterprise(clsEnterprise&& other);                   // ����������� �����������
        clsEnterprise& operator=(const clsEnterprise& other);   // �������� ������������ ������������
        clsEnterprise& operator=(clsEnterprise&& other);        // �������� ������������ ������������
        virtual ~clsEnterprise();                               // ����������

        /** Import - ������ **/
        bool Import_Data();     /** �������������� ����� ������� **/

        /** �������� � ������ ������ ������� ��������� (���)  **/
        bool SetWarehouse();    /** ����� �������� ������ ������� ��������� (���) � ����� ����������. **/

        void StockEditSettings(SelectDivision stk); /** ����� ����������� ��������� ����� ������ (��� ��� ���)
        ��� ������� SKU: �����, �������� � ��. ��������� SKU, ���� ����������� �������, ���� ���������� ��
        �������� � ������� � ����� ������� � �������� ������ �� ������. **/

        bool StockCalculate(const SelectDivision& _dep, size_t thr); /** ����� ������������ ��������� �����
        �������/����������� �� ����� ��� ��� SKU, � ������� ��������� ����������� ����� ������ ����. �����
        ������ (��� ��� ���) �������������� ��������� ��������� _dep: "warehouse" - ���, ����� ������ ��������
        - ���. ��������� ���������� thr ��������������� ����� ����������: "1" - TLack clsStorage::Calculate_future,
        "2" - TLack clsStorage::Calculate_thread, ����� TLack clsStorage::Calculate. **/

        /** �������� � ������ ������������ **/
        bool SetManufactory(); /** ����� �������� ������������ � ����� ���������� **/

        void ManufCalculateIn(size_t thr); /** ����� ������������ ����� ����������� ����� � ���������� � ������������
        � ����������� ��������� ��� ����� ����� ������� ���� ���������. ��������� ���������� thr ��������������� �����
        ����������: "1" - void clsManufactory::CalcRawMatPurchPlan_future,
        "2" - void clsManufactory::CalcRawMatPurchPlan_thread, ����� void clsManufactory::CalcRawMatPurchPlan. **/

        bool ManufCalculateOut(size_t thr); /** ����� ������������ �����, �������� � ������ �������������
        �������������� ������������ � ������� ��������� ��� ���� ���������, ����������� �� ���������� ����� �������.
        ��������� ���������� thr ��������������� ����� ����������: "1" - bool clsManufactory::Calculate_future(),
        "2" - bool clsManufactory::Calculate_thread(), ����� bool clsManufactory::Calculate(). **/

        bool MWCostTransmition(); /** ����� �������� ������� ������������� ������������� ��������� �� ������������
        �� ����� ������� ��������� **/

        /** ��������  � ������ ������ ����� � ���������� (���) **/
        bool SetRawMatStock();  /** ����� �������� ������ ����� � ���������� (���) � ����� ����������. **/

        bool RMCostTransmition();   /** ����� �������� ������� ������������� ����� � ���������� �� ������ �����
        � ���������� � ������������ **/

        /** ������ ��� ����������� �������� � ������� **/
        void ReportView(const SelectDivision& _rep, const int _arr, const ReportData flg) const; /** ������� �������
        ��������� �����. ���������: _rep - ��������� ����� ������ (warehouse - ���, manufactory - ������������,
        rowmatstock- ���), _arr -  ��� ������ (purchase - ������ �����������. balance - ������ ��������, shipment -
        ������ ��������). �������� flg - ��� ��������� ����������: volume - � �����������, value - � �����������,
        price - � ������� ��������� **/

        void StockSettingsView(const SelectDivision& _rep); /** ������� ������� ������������� ��������� ������.
        ���������: _rep - ��������� ����� ������ (warehouse - ���, rowmatstock ��� ����� ������ - ���) **/

        /** Export - ������ **/

        bool Export_Storage(string filename, const SelectDivision& _dep, const ChoiseData& _arr, \
        const ReportData& flg) const; /** ����� ���������� ������ ��������, �������� ��� �������� �� ������
        ������� ��������� (���) ��� ������ ����� � ���������� (���) � csv-���� � ������ filename. ���������:
        _dep - ���� ������ ������: "warehouse" - ���, "rowmatstock" - ���; _arr - ����� ������: "purchase" -
        ��������, "balance" - �������, "shipment" - ��������; flg - ��� ��������� � ���� ����������: volume
        - � �����������, value - � �����������, price - � ������� ���������. � �������� ����������� �����
        ������ ������������ ������ _ch �� ��������� (';'). **/

        bool Export_Manufactory(string filename, const ChoiseData& _arr, const ReportData& flg) const;
        /** ����� ���������� ������ �������� �����, �������������� �����-�� ��� �������� ��������� � csv-����
        � ������ filename. ���������: _arr - ����� ������: "purchase" - ��������, "balance" - �������, "shipment"
        - ��������; flg - ��� ��������� � ���� ����������: volume - � �����������, value - � �����������, price
        - � ������� ���������. � �������� ����������� ����� ������ ������������ ������ _ch �� ��������� (';').
        ��������!!! ��� ������ ������ � ����������� _arr = purchase, ��������� ����� � price ��� value ��������
        � ������ � ���� ������� ������, �.�. ������� GetRMPurchPlan �������� ���� price � value � �����������
        �������. **/

    protected:
        /** ��������� ������ � ������� **/
        const string rTableName  = "������������";
        const string rTableMeas  = "��.���������";
        const string rByVolume   = "� ����������� ���������";
        const string rByPrice    = "� �������� ����������� ���������";
        const string rByValue    = "� ����������� ���������";
        const string dWarehouse  = "����� ������� ���������: ";
        const string dManufactory= "������������: ";
        const string dRawMatStock= "����� ����� � ����������: ";
        const string aPurchase   = "�����������";
        const string aBalance    = "�������";
        const string aShipment   = "��������";
        const string aRecipe1    = "��������� ��������: ";
        const string aRecipe2    = "������ �� ";

        /** Import - ������ **/
        virtual bool Import_About(const string filename);   /** ����� ������ ���������� �� ����� � ���������
        ������� � ��������� ���� Title � Descript ������ clsBaseProject. **/

        virtual bool ImportSingleArray(const string filename, const char _ch, size_t hcols, size_t hrows,\
        ReportData flg, strItem* &_data, strNameMeas* &_names, size_t& ColCount, size_t& RowCount);
        /** ����� ������ ���������� �� ����� � ������ filename � ������������� ����� ������ ch � ��������� ����:
        RowCount - ����� �������������� ������� (�������� ��� ���������), ColCount - ����� �������� �������,
        names - ������ �� ��������� �� ������ � �������������� �������������� ������� � ������ �� ���������,
        _data - ������ �� ��������� �� ����������� ������, flg - ����, ������������ ��� ������������� ������:
        "volume" - ������ � ����������� ���������, "price" - ����, "value" - ���������. **/

        virtual bool Import_Recipes(const string filename, const char _ch, size_t hcols, size_t hrows); /** �����
        ������ ���������� �� ������ � �������, ����������� ������� filename � ���������������� �� _i, ��� i- �����
        ���������. � �������� ����������� ������������ ������ _ch. ����� ��������� ������ �������� Recipe. **/

        /** ������ �������������� **/

        virtual bool SKUEdt(clsStorage* stock, const size_t num);
        /**  ����� �������������� ��������� ����� ����������: �������������� �������������� ������� (SKU).
        ���������: stock - ��������� �� ���������� ����� (Warehouse ��� RawMatStock. **/

        /** ������ ��� ����������� �������� � ������� **/
        virtual void reportstream(ostream& os) const;   /** ����� ������� ���������� � ������� � ����� os.
        ����� �������������� ����������� ����� ������ clsBaseProject **/

        virtual void showSKUsettings(ostream& os, clsStorage* stock) const; /** ����� ������ � �������� �����
        �������� ����� �� ������ ��� ������� SKU: �����, �������� � ��. ��������� SKU, ���� ����������� �������,
        ���� ���������� �� �������� � ������� � ����� ������� � �������� ������ �� ������, ���������: os - �����
        ��� ������, stock - ��������� �� ���������� �����(Warehouse ��� RawMatStock. **/

        virtual bool Report_Storage(clsStorage* obj, const int _arr, const ReportData flg) const;   /** ����� ���
        ������ �� ����� ������� �� ������� ���� clsStorage (Warehouse ��� RawMatStock). �������� obj - ���������
        ������ clsStorage. ��������� �������: _arr = purchase, ������ ����������� �� �����; _arr = balance, ������
        �������� �� ������; _arr = shipment, ������ �������� �� ������. �������� flg - ��� ��������� ����������:
        volume - � �����������, value - � �����������, price - � ������� ���������. ���� �������� _arr �� ���������
        � �������������� ���� �� ������ warehouse_module.h enum ChoiseData{purchase = sZero, balance, shipment},
        �� ����� �� ���������: ������ �������� _arr ������������� ��� ������ �������. ������������ � ������
        Dev_Report_Storage **/

        virtual bool Report_Storage_to_dev(clsStorage* obj, const int _arr, const ReportData flg) const; /** �����
        ���������� �����, ��������� ������� Report_Storage �� ��������� ����������: "������" ����������, ����� ���
        ���� (������������ ����������� ���������� Rdevice). ���������� ����� Report_Storage. ��������� ��������� �
        ����������� ������ Report_Storage. **/

        virtual void Report_Recipe() const; /** ����� ��� ������ � ����� �������� ���� ��������� **/

        virtual bool Report_Manufactory(const int _arr, const ReportData flg) const; /** ����� ��� ������ �� �����
        ������� �� �����������. ��������� �������: _arr = manpurchase, ������  ����������� � ����� � ����������;
        _arr = manbalance, ������ c ��������� �������������� ������������ ��� ���� ���������; _arr = manshipment,
        ������ �������� �� ���; _arr = recipe, ��������� ���������. flg - ��� ��������� ����������: volume - �
        �����������, value - � �����������, price - � ������� ���������. ���� �������� _arr �� ��������� �
        �������������� ���� �� ������� ������ enum ManufData{manpurchase = 11, manbalance, manshipment, recipe},
        �� ����� �� ��������� **/

};  // clsEnterprise

#endif // CLSENTERPRISE_H
