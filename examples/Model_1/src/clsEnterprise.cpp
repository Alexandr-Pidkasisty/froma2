#include "clsEnterprise.h"

string DBLR_ind() {
/** ������� ���������� ������ �������� ������������� ���� ������������ �����: "DB" - ��� ������������
����� ������������ ��� double, "LR" - ��� ������������ ����� ������������ ��� LongReal. ������������
��� ���������� �������� � ����� ��������� ������. **/
    if(typeid(decimal) == typeid(double)) return "DB";
    if(typeid(decimal) == typeid(LongReal)) return "LR";
    return "unknown";
}   // DBLR_ind

void strItemReplace(size_t count, strItem* dstn, strItem* src, ReportData flg) {
/** ������� �������� �������� �����, ��������� ������ flg � ������� dstn ���������� ����� �� �������
src � ��������� � ���������� ��������. count - ������ �������� **/
    if((!dstn) || (!src)) return;       // ������� �������� �� �������������
    for(size_t i{}; i<count; i++) {
        if((flg==volume) || (flg==price))
            if(flg==volume) (dstn+i)->volume = (src+i)->volume;
            else (dstn+i)->price = (src+i)->price;
        else (dstn+i)->value = (src+i)->value;
    }
}   // strItemMerge

template <typename T>
void inData(T &_data, const T _defdata) {
/** ����� ������ ������ ������������. ��� ���������� ������, ������������� �������� �� ���������.
���������: &_data - ������ �� �������� ������������� ������, _defdata - �������� ��
���������. ����������� � �������� ���������� &_data � _defdata ������������ ��� ����� � ��� ��
����������, �������� inData(x, x), ��������� ������ �������� ���������� � ������� ��� �����. **/
    const char ch = '\n';               // ������ �������� ������
    if (std::cin.peek() == ch) {        // ��������, ��� ��������� ������ �������� ������, ���� "��", ��
        _data = _defdata;               // ����������� �������� �� ���������
    } else if(!(std::cin >> _data)) {   // ���� ���� ���������, ��
        cout << "������ �����. ����� ��������� �������� " << _defdata << ch;      // �������� �� ������,
        cin.clear();                    // ���������� ���� ������
        _data = _defdata;               // � ����������� �������� �� ���������
    }
    cin.ignore(numeric_limits<streamsize>::max(), ch); // ������� ����� �����
}   // inData

string FullFName(string _dir, string _fname) {
/** ����� ���������� ������ ��� �����. ���������: _dir - ���� � �����, ������� ����� ���� �������� � ��� �����,
_fname - ��� �����. ���� ��� ����� ��� �������� ����, �.�. �������� ������, �� ���� �� �����������. ����� �����
���������� ������ ��� �����, ��������� �� ���� � �����. **/
    size_t found = _fname.find('/');    // ���� ��������� ������� "/" � ����� �����
    if (found != std::string::npos) {   // ���� ������ "/" ������, �� ����� ����� ��� ���������� ����
        return _fname;                  // ���������� ��� ����� ��� ���������
    }
    return _dir.append(_fname);         // �������� ���� � ��� ����� � ���������� ������ ��� �����
}   // clsEnterprise::FullFName

/****************************************************************************************************/
/**                                 ��������� strImportConfig                                      **/
/****************************************************************************************************/

strImportConfig::strImportConfig() {
/** ����������� �� ��������� **/
    filename_About = EmpStr;
    filename_Shipment = EmpStr;
    filename_Production = EmpStr;
    filename_Purchase = EmpStr;
    filename_Purchase_V = EmpStr;
    filenameprefix_Recipes = EmpStr;
    _ch = ';';
    HeadCols = sZero;
    HeadRows = sZero;
    _cur = RUR;
    _amethod = FIFO;
    P_Share = dZero;
    S_Share = dZero;
    P_indr = true;
    S_indr = true;
    PsetCount = sZero;
    SsetCount = sZero;
    P_settings = nullptr;
    S_settings = nullptr;
}   // strImportConfig::Ctor

strImportConfig::strImportConfig(const strImportConfig &obj) {
/** ����������� ����������� **/
    filename_About = obj.filename_About;
    filename_Shipment = obj.filename_Shipment;
    filename_Production = obj.filename_Production;
    filename_Purchase = obj.filename_Purchase;
    filenameprefix_Recipes = obj.filenameprefix_Recipes;
    _ch = obj._ch;
    HeadCols = obj.HeadCols;
    HeadRows = obj.HeadRows;
    _cur = obj._cur;
    _amethod = obj._amethod;
    P_Share = obj.P_Share;
    S_Share = obj.S_Share;
    P_indr = obj.P_indr;
    S_indr = obj.S_indr;
    PsetCount = obj.PsetCount;
    SsetCount = obj.SsetCount;
    if(obj.P_settings) { P_settings = new(nothrow) strSettings[PsetCount]; } else P_settings = nullptr;
    if(P_settings) { var_cpy(P_settings, obj.P_settings, PsetCount); };
    if(obj.S_settings) { S_settings = new(nothrow) strSettings[SsetCount]; } else S_settings = nullptr;
    if(S_settings) { var_cpy(S_settings, obj.S_settings, SsetCount); };
}   // strImportConfig::CopyCtor

strImportConfig::strImportConfig(strImportConfig &&obj) {
/** ����������� ����������� **/
    filename_About = EmpStr;
    filename_Shipment = EmpStr;
    filename_Production = EmpStr;
    filename_Purchase = EmpStr;
    filename_Purchase_V = EmpStr;
    filenameprefix_Recipes = EmpStr;
    _ch = ';';
    HeadCols = sZero;
    HeadRows = sZero;
    _cur = RUR;
    _amethod = FIFO;
    P_Share = dZero;
    S_Share = dZero;
    P_indr = true;
    S_indr = true;
    PsetCount = sZero;
    SsetCount = sZero;
    P_settings = nullptr;
    S_settings = nullptr;
    swap(obj);
}   // strImportConfig::MoveCtor

strImportConfig& strImportConfig::operator=(const strImportConfig &obj) {
/** ���������� ��������� ������������ ������������. ����������� � ������
�����������-�-������ (copy-and-swap idiom) **/
     if(this == &obj) { return *this; };    // ���� ������� ���������, �� ������ �� ������
     strImportConfig tmp(obj);              // �������� ����������� ����������� � �������� ����� obj � ���������� tmp
     swap(tmp);                             // ������������ ����������� � tmp
     return *this;
}   // Copy operator=

strImportConfig& strImportConfig::operator=(strImportConfig &&obj) {
/** ���������� ��������� ������������ ������������. ����������� � ������ �����������-�-������
(move-and-swap idiom) **/
    if(this == &obj) { return *this; };     // ���� ������� ���������, �� ������ �� ������
    strImportConfig tmp(move(obj));         // �������� ����������� ����������� � �������� ���������� obj � ���������� tmp
    swap(tmp);                              // ������������ ����������� � tmp
    return *this;
}   // Move operator=

strImportConfig::~strImportConfig() {
/** ���������� **/
    if(P_settings) delete[] P_settings;
    if(S_settings) delete[] S_settings;
}   // strImportConfig::Dtor

bool strImportConfig::SaveToFile(const string _filename) {
/** ����� ������ ������� ������������ � ���� **/
    if(_filename == EmpStr) return false;               // ���� ��� ����� ������, �� ����� � ������� false
    ofstream outF(confdir + _filename, ios::out| ios::binary);    // ��������� ��� ����� � �������� ������� ��� ������ �� ����
    if (!outF.is_open())                                // ���� ���� �� ������, ��
        return false;                                   // ����� � false
    if(!SEF(outF, filename_About)) return false;        // ��������� ��� ����� � ���������
    if(!SEF(outF, filename_Shipment)) return false;     // ��������� ��� ����� � ���������� � ���
    if(filename_Production == EmpStr) filename_Production = NoFileName; // �������� �� ������ ��������
    if(!SEF(outF, filename_Production)) return false;   // ��������� ��� ����� � �������� ������������
    if(!SEF(outF, filename_Purchase)) return false;         // ��������� ��� ����� � ������ ������� �� ���
    if(filename_Purchase_V == EmpStr) filename_Purchase_V = NoFileName; // �������� �� ������ ��������
    if(!SEF(outF, filename_Purchase_V)) return false;       // ��������� ��� ����� � �������� ������� �� ���
    if(!SEF(outF, filenameprefix_Recipes)) return false;    // ��������� ������� ��� ���� ������ � �����������
    if(!SEF(outF, _ch)) return false;       // ��������� ����������� ����� �������
    if(!SEF(outF, HeadCols)) return false;  // ��������� ���������� �������� � �����������
    if(!SEF(outF, HeadRows)) return false;  // ��������� ���������� ����� � �����������
    if(!SEF(outF, _cur)) return false;      // ��������� �������� ������
    if(!SEF(outF, _amethod)) return false;  // ��������� ������� ����� �������
    if(!SEF(outF, P_Share)) return false;   // ��������� �������� ������� �� ���
    if(!SEF(outF, S_Share)) return false;   // ��������� �������� ������� �� ���
    if(!SEF(outF, P_indr)) return false;    // ��������� ���� ���� ���������� ����������� � �������� � ��� � ����� �������
    if(!SEF(outF, S_indr)) return false;    // ��������� ���� ���� ���������� ����������� � �������� � ��� � ����� �������
    bool flag_P_settings, flag_S_settings;                                  // ����� ������������� ��������
    if(P_settings) flag_P_settings = true; else flag_P_settings = false;    // ���� ������ ���������, �� ���� True
    if(S_settings) flag_S_settings = true; else flag_S_settings = false;    // ���� ������ ���������, �� ���� True
    if(!SEF(outF, flag_P_settings)) return false;                           // ��������� ���� � ����
    if(!SEF(outF, flag_S_settings)) return false;                           // ��������� ���� � ����
    if(!SEF(outF, PsetCount)) return false; // ��������� � ���� ������ ������� P_settings
    if(!SEF(outF, SsetCount)) return false; // ��������� � ���� ������ ������� S_settings
    if(flag_P_settings)                     // ��������� ������ � ����
        for(size_t i{}; i<PsetCount; i++)   // �����������
            if(!(P_settings+i)->StF(outF)) return false;
    if(flag_S_settings)                     // ��������� ������ � ����
        for(size_t i{}; i<SsetCount; i++)   // �����������
            if(!(S_settings+i)->StF(outF)) return false;
    outF.close();                           // ��������� �������� �����
    return true;
}   // strImportConfig::SaveToFile

void strImportConfig::swap(strImportConfig& obj) noexcept {
/** ������� ������ ���������� ����� ���������. ������� ��������� noexcept - �� ���������� ���������� **/
    std::swap(filename_About, obj.filename_About);
    std::swap(filename_Shipment, obj.filename_Shipment);
    std::swap(filename_Production, obj.filename_Production);
    std::swap(filename_Purchase, obj.filename_Purchase);
    std::swap(filename_Purchase_V, obj.filename_Purchase_V);
    std::swap(filenameprefix_Recipes, obj.filenameprefix_Recipes);
    std::swap(_ch, obj._ch);
    std::swap(HeadCols, obj.HeadCols);
    std::swap(HeadRows, obj.HeadRows);
    std::swap(_cur, obj._cur);
    std::swap(_amethod, obj._amethod);
    std::swap(P_Share, obj.P_Share);
    std::swap(S_Share, obj.S_Share);
    std::swap(P_indr, obj.P_indr);
    std::swap(S_indr, obj.S_indr);
    std::swap(PsetCount, obj.PsetCount);
    std::swap(SsetCount, obj.SsetCount);
    std::swap(P_settings, obj.P_settings);
    std::swap(S_settings, obj.S_settings);
}   // strImportConfig::swap

bool strImportConfig::ReadFromFile(const string _filename) {
/** ����� ������ ������������ �� ����� � ������ �� � ������� ��������� ��������� **/
    if(_filename == EmpStr) return false;           // ���� ��� ����� ������, �� ����� � ������� false
    ifstream inF(confdir + _filename, ios::in | ios::binary); // ��������� ��� ����� � �������� ������� ��� ������ � �����
    if (!inF.is_open()) {                           // ���� ���� �� ������, �� ��������� ������������ �
        return false;                               // ����� � false
    };
    strImportConfig tmp;                            // ��������� ����������, � ������� ������������ ������
    if(!DSF(inF, tmp.filename_About)) return false; // ������ ����� ������,
    if(!DSF(inF, tmp.filename_Shipment)) return false;
    if(!DSF(inF, tmp.filename_Production)) return false;
    if(tmp.filename_Production == NoFileName) filename_Production = EmpStr;
    if(!DSF(inF, tmp.filename_Purchase)) return false;
    if(!DSF(inF, tmp.filename_Purchase_V)) return false;
    if(tmp.filename_Purchase_V == NoFileName) filename_Purchase_V = EmpStr;
    if(!DSF(inF, tmp.filenameprefix_Recipes)) return false;
    if(!DSF(inF, tmp._ch)) return false;            // ������ �����������
    if(!DSF(inF, tmp.HeadCols)) return false;       // ������ ���������� �������� � �����������
    if(!DSF(inF, tmp.HeadRows)) return false;       // ������ ���������� ����� � �����������
    if(!DSF(inF, tmp._cur)) return false;           // ������ �������� ������
    if(!DSF(inF, tmp._amethod)) return false;       // ������ ������� ����� �������
    if(!DSF(inF, tmp.P_Share)) return false;        // ������ �������� ������� �� ���
    if(!DSF(inF, tmp.S_Share)) return false;        // ������ �������� ������� �� ���
    if(!DSF(inF, tmp.P_indr)) return false; // ������ ���� ���� ���������� ����������� � �������� � ��� � ����� �������
    if(!DSF(inF, tmp.S_indr)) return false; // ������ ���� ���� ���������� ����������� � �������� � ��� � ����� �������
    bool flag_P_settings, flag_S_settings;                      // ����� ������������� ��������
    if(!DSF(inF, flag_P_settings)) return false;                // ������ ���� �� �����
    if(!DSF(inF, flag_S_settings)) return false;                // ������ ���� �� �����
    if(!DSF(inF, tmp.PsetCount)) return false;                  // ������ �� ����� ������ ������� P_settings
    if(!DSF(inF, tmp.SsetCount)) return false;                  // ������ �� ����� ������ ������� S_settings
    if(flag_P_settings) {                                       // ���� ���� true, �� ������ ���� ��������� �� �����
        strSettings* Ps_temp = new(nothrow) strSettings[tmp.PsetCount]; // �������� ������ ��� ���������� �������
        if(!Ps_temp) return false;                              // ���� ������ �� ��������, �� ����� � ������� false
        for(size_t i{}; i<tmp.PsetCount; i++)                   // ���� ������ ��������, �� ������ ������ �� �����
            if(!(Ps_temp+i)->RfF(inF)) return false;            // �����������
        std::swap(Ps_temp, tmp.P_settings);                     // ������������ �����������
        if(Ps_temp) delete[] Ps_temp;                           // ������� ��������������� ������
    } else tmp.P_settings = nullptr;                            // ���� ���� false, �� ��������� ������� nullptr
    if(flag_S_settings) {                                       // ���� ���� true, �� ������ ���� ��������� �� �����
        strSettings* Ss_temp = new(nothrow) strSettings[tmp.SsetCount]; // �������� ������ ��� ���������� �������
        if(!Ss_temp) return false;                              // ���� ������ �� ��������, �� ����� � ������� false
        for(size_t i{}; i<tmp.SsetCount; i++) {                 // ���� ������ ��������, �� ������ ������ �� �����
            if(!(Ss_temp+i)->RfF(inF)) return false;            // �����������
        }
        std::swap(Ss_temp, tmp.S_settings);                     // ������������ �����������
        if(Ss_temp) delete[] Ss_temp;                           // ������� ��������������� ������
    } else tmp.S_settings = nullptr;                            // ���� ���� false, �� ��������� ������� nullptr
    swap(tmp);                              // ������������ ���������� � ��������� ����������
    inF.close();                            // ��������� �������� �����
    return true;
}   // strImportConfig::ReadFromFile

void strImportConfig::Entry() {
/** ����� ����� ���������������� ������ � ��������� **/
    cout << "������� ������������ �������\n";
    cout << "� []-������� ������������ �������� �� ���������. ��� ��� ����� ������ ������� Enter\n";
    cout << " ��� ����� � ��������� ������� [" << filename_About << "]: ";
    inData(filename_About, filename_About);
    cout << " ��� ����� � ���������� �� ��� � ����������� ��������� [" << filename_Shipment << "]: ";
    inData(filename_Shipment, filename_Shipment);
    cout << " ��� ����� � �������� ������������, �������������. ��� �������� ������� nofile [" \
        << filename_Production << "]: ";
    inData(filename_Production, filename_Production);
    cout << " ��� ����� � ������ ������� �� ���  [" << filename_Purchase << "]: ";
    inData(filename_Purchase, filename_Purchase);
    cout << " ��� ����� � �������� ������� �� ���, �������������. ��� �������� ������� nofile [" \
        << filename_Purchase_V << "]: ";
    inData(filename_Purchase_V, filename_Purchase_V);
    cout << " ������� ��� ��� ������ � ����������� [" << filenameprefix_Recipes << "]: ";
    inData(filenameprefix_Recipes, filenameprefix_Recipes);
    cout << " ������ ����������� ����� ������ � CSV_������ [" << _ch << "]: ";
    inData(_ch, _ch);
    cout << " ����� �������� � ����������� � CSV_������ [" << HeadCols << "]: ";
    inData(HeadCols, HeadCols);
    cout << " ����� ����� � ����������� � CSV_������ [" << HeadRows << "]: ";
    inData(HeadRows, HeadRows);
    cout << " �������� ������ �������, ";
    for(size_t i=sOne; i<Currency::Currency_MAX-sOne; i++)
        cout << CurrencyTXT[i] << "=" << i << ", ";
    cout << CurrencyTXT[Currency::Currency_MAX-sOne] << "=" << Currency::Currency_MAX-sOne << " [" \
        << _cur << "]: ";
    size_t t_cur;
    inData(t_cur, static_cast<size_t>(_cur));
    if((t_cur == Currency::Currency_MIN) || (t_cur >= Currency::Currency_MAX))
        t_cur = sOne;                       // �������� ���������� ��������
    _cur = static_cast<Currency>(t_cur);
    cout << " ������� ����� �������, FIFO=0, LIFO=1, AVG=2 [" << _amethod << "]: ";
    inData(t_cur, static_cast<size_t>(_amethod));
    if(t_cur > sTwo) t_cur = sZero;
    _amethod = static_cast<AccountingMethod>(t_cur);
    double ps_tmp;          // ��������������� ����������
    if((filename_Production == NoFileName) || (filename_Production == EmpStr)) {
        cout << " ����� ��������� �� ������ ���, ���� �� �������� [" << S_Share << "]: ";
        inData(ps_tmp, S_Share.Get<double>());
        S_Share = ps_tmp;
    }
    if((filename_Purchase_V == NoFileName) || (filename_Purchase_V == EmpStr)) {
        cout << " ����� �������� �� ������ ���, ���� �� �������� [" << P_Share << "]: ";
        inData(ps_tmp, P_Share.Get<double>());
        P_Share = ps_tmp;
    }
    cout << " ���� ���������� ����������� � �������� � ��� � ����� �������, PROHIBITED=0, ALLOWED=1 ["\
        << S_indr << "]: ";
    inData(t_cur, static_cast<size_t>(S_indr));
    if(t_cur <=1)
        S_indr = static_cast<bool>(t_cur);
    else S_indr = true;
    cout << " ���� ���������� ����������� � �������� � ��� � ����� �������, PROHIBITED=0, ALLOWED=1 ["\
        << P_indr << "]: ";
//    cin >> t_cur;
    inData(t_cur, static_cast<size_t>(P_indr));
    if(t_cur <=1)
        P_indr = static_cast<bool>(t_cur);
    else P_indr = true;
    strImportConfig::Show();
}   // strImportConfig::Entry

void strImportConfig::Show() {
/** ����� ���������� ������� ������������ �� ������ **/
    cout << "������� ������������ �������\n";
    cout << " ��� ����� � ��������� �������: " << filename_About << endl;
    cout << " ��� ����� � ���������� �� ��� � ����������� ���������: " << filename_Shipment << endl;
    if(filename_Production == NoFileName)
        cout << " ��� ����� � �������� ������������: " << EmpStr << endl;
    else
        cout << " ��� ����� � �������� ������������: " << filename_Production << endl;
    cout << " ��� ����� � ������ ������� �� ���: " << filename_Purchase << endl;
    if(filename_Purchase_V == NoFileName)
        cout << " ��� ����� � �������� ������� �� ���: " << EmpStr << endl;
    else
        cout << " ��� ����� � �������� ������� �� ���: " << filename_Purchase_V << endl;
    cout << " ������� ��� ��� ������ � �����������: " << filenameprefix_Recipes << endl;
    cout << " ������ ����������� ����� ������ � CSV_������: " << _ch << endl;
    cout << " ����� �������� � ����������� � CSV_������: " << HeadCols << endl;
    cout << " ����� ����� � ����������� � CSV_������: " << HeadRows << endl;
    cout << " �������� ������ �������: " << nmBPTypes::CurrencyTXT[_cur] << endl;
    cout << " ������� ����� �������: " << AccountTXT[_amethod] << endl;
    cout << " ���� ���������� ����������� � �������� � ��� � ����� �������: " << ProhibitedTXT[S_indr] << endl;
    cout << " ���� ���������� ����������� � �������� � ��� � ����� �������: " << ProhibitedTXT[P_indr] << endl;
}   // strImportConfig::Show

void strImportConfig::Configure() {
/** ����� ������ ������������ ������� �, ��� ������������� ����������� � � �����������
����������� � ���������������� ����� � ������, ������������ � ���������� Configure_filename**/
    char Answer = 'N';
    if(ReadFromFile(Configure_filename)) {                      // ������ ������������ �� �����. ���� ������, ��
        strImportConfig::Show();                                // ������� ������������ �� �����;
        cout << "������������� ������������? [Y/N]";            // ���������� �� ���������������;
        cin >> Answer;                                          // �������� �����
        cin.ignore();                                           // ������� ����� cin
        if(Answer != 'Y') return;                               // ��� ����� ��������������� ������ �������
        strImportConfig::Entry();                               // ����� ����������� ������������
        strImportConfig::SaveToFile(Configure_filename);        // � ��������� ����� ������������ � ����
        return;
    };
    strImportConfig::Entry();                                   // ����������� ������������
    strImportConfig::SaveToFile(Configure_filename);            // ��������� ����� ������������ � ����
    return;
}   // strImportConfig::Configure

size_t strImportConfig::GetPsetCount() const {
/** ���������� ������ ������� P_settings **/
    return PsetCount;
}   // strImportConfig::GetPsetCount

size_t strImportConfig::GetSsetCount() const {
/** ���������� ������ ������� S_settings **/
    return SsetCount;
}   // strImportConfig::GetSsetCount

strSettings* strImportConfig::GetPurSettings() const {
/** ���������� ��������� �� ����� ����� ������� P_settings **/
    if(!P_settings) return nullptr;                             // ���������
    strSettings* temp = new(nothrow) strSettings[PsetCount];    // �������� ������ �������
    if(!temp) return nullptr;                                   // ���� ������ �� ��������, ����� � false
    var_cpy(temp, P_settings, PsetCount);                       // �������� �������
    return temp;
}   // strImportConfig::GetPurSettings

strSettings* strImportConfig::GetShpSettings() const {
/** ���������� ��������� �� ����� ����� ������� S_settings **/
    if(!S_settings) return nullptr;                             // ���������
    strSettings* temp = new(nothrow) strSettings[SsetCount];    // �������� ������ �������
    if(!temp) return nullptr;                                   // ���� ������ �� ��������, ����� � false
    var_cpy(temp, S_settings, SsetCount);                       // �������� �������
    return temp;
}   // strImportConfig::GetShpSettings

bool strImportConfig::SetPurSettings(const strSettings* _P_settings, const size_t pcount) {
/** �������� ������ _P_settings � ������ P_settings **/
    if((!_P_settings) || (pcount == sZero)) return false;   // ��������� ����������
    strSettings* temp = new(nothrow) strSettings[pcount];   // �������� ������ ���������� �������
    if(!temp) return false;                                 // ���� ������ �� ��������, �� ����� � false
    var_cpy(temp, _P_settings, pcount);                     // �������� ������ �� ��������� ������
    std::swap(temp, P_settings);                            // ������������ �����������
    PsetCount = pcount;                                     // ����������� ����� ��������
    if(temp) delete[] temp;                                 // ������� ��������������� ������
    return true;
}   // strImportConfig::SetPurSettings

bool strImportConfig::SetShpSettings(const strSettings* _S_settings, const size_t scount) {
/** �������� ������ _S_settings � ������ S_settings **/
    if((!_S_settings) || (scount == sZero)) return false;   // ��������� ����������
    strSettings* temp = new(nothrow) strSettings[scount];   // �������� ������ ���������� �������
    if(!temp) return false;                                 // ���� ������ �� ��������, �� ����� � false
    var_cpy(temp, _S_settings, scount);                     // �������� ������ �� ��������� ������
    std::swap(temp, S_settings);                            // ������������ �����������
    SsetCount = scount;                                     // ����������� ����� ��������
    if(temp) delete[] temp;                                 // ������� ��������������� ������
    return true;
}   // strImportConfig::SetPurSettings

/****************************************************************************************************/
/**             ����� clsEnterprise. ��������� ������ ������� clsBaseProject                       **/
/****************************************************************************************************/

/********************************** ������ public ***************************************************/

clsEnterprise::clsEnterprise() {
/** ����������� �� ��������� **/
    PrCount = sZero;
    Cur = RUR;
    Amethod = FIFO;
    purcalc = calc;
    mancalc = calc;
    PurchShare = cShare;
    ShipShare = cShare;
    Purch_indr = true;
    Ship_indr = true;
    ProdCount = sZero;
    Shipment = nullptr;
    Production = nullptr;
    ProdNames = nullptr;
    RMCount = sZero;
    Consumpt = nullptr;
    Purchase = nullptr;
    PriceBus = nullptr;
    RMNames = nullptr;
    Warehouse = nullptr;
    Manufactory = nullptr;
    RawMatStock = nullptr;
    P_settings = nullptr;
    S_settings = nullptr;
}   //Ctor

void clsEnterprise::swap(clsEnterprise& other) noexcept {
/** ������� ������ ���������� ����� ���������. ������� ��������� noexcept - �� ���������� ���������� **/
    clsBaseProject::swap(other);                        // �������� ������� ������������� ������
    std::swap(PrCount, other.PrCount);                  // ���������� ��������
    std::swap(Cur, other.Cur);                          // ���������� ��������
    std::swap(Amethod, other.Amethod);                  // ���������� ��������
    std::swap(purcalc, other.purcalc);                  // ���������� ��������
    std::swap(mancalc, other.mancalc);                  // ���������� ��������
    std::swap(PurchShare, other.PurchShare);            // ���������� ��������
    std::swap(ShipShare, other.ShipShare);              // ���������� ��������
    std::swap(Purch_indr, other.Purch_indr);            // ���������� ��������
    std::swap(Ship_indr, other.Ship_indr);              // ���������� ��������
    std::swap(ProdCount, other.ProdCount);              // ���������� ��������
    std::swap(Shipment, other.Shipment);                // ���������� ���������
    std::swap(Production, other.Production);            // ���������� ���������
    std::swap(ProdNames, other.ProdNames);              // ���������� ���������
    std::swap(RMCount, other.RMCount);                  // ���������� ��������
    std::swap(Consumpt, other.Consumpt);                // ���������� ���������
    std::swap(Purchase, other.Purchase);                // ���������� ���������
    std::swap(PriceBus, other.PriceBus);                // ���������� ���������
    std::swap(RMNames, other.RMNames);                  // ���������� ���������
    std::swap(Warehouse, other.Warehouse);              // ���������� ���������
    std::swap(Manufactory, other.Manufactory);          // ���������� ���������
    std::swap(RawMatStock, other.RawMatStock);          // ���������� ���������
    std::swap(P_settings, other.P_settings);            // ���������� ���������
    std::swap(S_settings, other.S_settings);            // ���������� ���������
    Recipe.swap(other.Recipe);                          // ����� ��������
}   // // swap

void clsEnterprise::EraseVector(vector<clsRecipeItem>& Recipe) {
/** ����� ��� ������ ������� ������� **/
    vector<clsRecipeItem>().swap(Recipe);    // �������� ������ � ��������� �� ���������
}   // EraseVector

clsEnterprise::clsEnterprise(const clsEnterprise& other) {
/** ����������� ����������� **/
    PrCount = other.PrCount;
    Cur = other.Cur;
    Amethod = other.Amethod;
    purcalc = other.purcalc;
    mancalc = other.mancalc;
    Purchase = other.Purchase;
    ShipShare = other.ShipShare;
    Purch_indr = other.Purch_indr;
    Ship_indr = other.Ship_indr;
    ProdCount = other.ProdCount;
    RMCount = other.RMCount;
    size_t prdtemp = ProdCount*PrCount;
    if(other.Shipment) { Shipment = new(nothrow) strItem[prdtemp]; } else Shipment = nullptr;
    if(Shipment) { var_cpy(Shipment, other.Shipment, prdtemp); };
    if(other.Production) { Production = new(nothrow) strItem[prdtemp]; } else Production = nullptr;
    if(Production) { var_cpy(Production, other.Production, prdtemp); };
    if(other.ProdNames) { ProdNames = new(nothrow) strNameMeas[prdtemp]; } else ProdNames = nullptr;
    if(ProdNames) { var_cpy(ProdNames, other.ProdNames, prdtemp); };
    if(other.Consumpt) { Consumpt = new(nothrow) strItem[prdtemp]; } else Consumpt = nullptr;
    if(Consumpt) { var_cpy(Consumpt, other.Consumpt, prdtemp); };
    if(other.Purchase) { Purchase = new(nothrow) strItem[prdtemp]; } else Purchase = nullptr;
    if(Purchase) { var_cpy(Purchase, other.Purchase, prdtemp); };
    if(other.PriceBus) { PriceBus = new(nothrow) decimal[prdtemp]; } else Purchase = nullptr;
    if(Purchase) { var_cpy(Purchase, other.Purchase, prdtemp); };
    if(other.RMNames) { RMNames = new(nothrow) strNameMeas[prdtemp]; } else RMNames = nullptr;
    if(RMNames) { var_cpy(RMNames, other.RMNames, prdtemp); };
    if(other.Warehouse) { Warehouse = new clsStorage(*other.Warehouse); } else Warehouse = nullptr;
    if(other.Manufactory) { Manufactory = new clsManufactory(*other.Manufactory); } else Manufactory = nullptr;
    if(other.RawMatStock) { RawMatStock = new clsStorage(*other.RawMatStock); } else RawMatStock = nullptr;
    if(other.P_settings) { P_settings = new(nothrow) strSettings[RMCount]; } else P_settings = nullptr;
    if(P_settings) { var_cpy(P_settings, other.P_settings, RMCount); };
    if(other.S_settings) { S_settings = new(nothrow) strSettings[ProdCount]; } else S_settings = nullptr;
    if(S_settings) { var_cpy(S_settings, other.S_settings, ProdCount); };
    Recipe = other.Recipe;
}   //Copy ctor

clsEnterprise::clsEnterprise(clsEnterprise&& other) {
/** ����������� ����������� **/
    PrCount = sZero;
    Cur = RUR;
    Amethod = FIFO;
    purcalc = calc;
    mancalc = calc;
    PurchShare = cShare;
    ShipShare = cShare;
    Purch_indr = true;
    Ship_indr = true;
    ProdCount = sZero;
    Shipment = nullptr;
    Production = nullptr;
    ProdNames = nullptr;
    RMCount = sZero;
    Consumpt = nullptr;
    Purchase = nullptr;
    PriceBus = nullptr;
    RMNames = nullptr;
    Warehouse = nullptr;
    Manufactory = nullptr;
    RawMatStock = nullptr;
    P_settings = nullptr;
    S_settings = nullptr;
    swap(other);
}   // Move Ctor

clsEnterprise& clsEnterprise::operator=(const clsEnterprise& other) {
/** ���������� ��������� ������������ ������������. ����������� � ������ �����������-�-������
(copy-and-swap idiom) **/
    if (this == &other) return *this;       // ���� ������� ���������, �� ������ �� ������
    clsEnterprise tmp(other);               // �������� ����������� ����������� � �������� ����� other � ���������� tmp
    swap(tmp);                              // ������������ ����������� � tmp
    return *this;
}   // Copy operator=

clsEnterprise& clsEnterprise::operator=(clsEnterprise&& other) {
/** ���������� ��������� ������������ ������������. ����������� � ������ �����������-�-������
(move-and-swap idiom) **/
    if(this == &other) { return *this; };   // ���� ������� ���������, �� ������ �� ������
    clsEnterprise tmp(move(other));         // �������� ����������� ����������� � �������� ����� other � ���������� tmp
    swap(tmp);                              // ������������ ����������� � tmp
    return *this;
}   // Move operator=

clsEnterprise::~clsEnterprise() {
/** ���������� **/
    if(Shipment) delete[] Shipment;
    if(Production) delete[] Production;
    if(ProdNames) delete[] ProdNames;
    if(Consumpt) delete[] Consumpt;
    if(Purchase) delete[] Purchase;
    if(PriceBus) delete[] PriceBus;
    if(RMNames) delete[] RMNames;
    if(Warehouse) delete Warehouse;
    if(Manufactory) delete Manufactory;
    if(RawMatStock) delete RawMatStock;
    if(P_settings) delete[] P_settings;
    if(S_settings) delete[] S_settings;
}   //Dtor

/** Import - ������ **/

bool clsEnterprise::Import_Data() {
/** �������������� ����� �������. �������� ������ ���������������� �������, �������������� � ����������
������������ � ����; � ����� ������ ������� ������ **/
    strImportConfig ImConfig;                               // ���������� ��� �������� ������������
    ImConfig.Configure();                                   // ������/ ����������� ������������
    Cur = ImConfig._cur;                                    // ������ ������ �������
    Amethod = ImConfig._amethod;                            // ������ ������� ����� �������
    if(!Import_About(ImConfig.filename_About)) {            // ������ �������� � �������� �������
        return false;                                       // ���� �� �������, ������� �� ��������� � false
    }
    Purch_indr = ImConfig.P_indr;           // �������������� ���� � ������ ��� ���
    Ship_indr = ImConfig.S_indr;            // �������������� ���� � ������ ��� ���
    if(!ImportSingleArray(ImConfig.filename_Shipment, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows,\
    volume, Shipment, ProdNames, PrCount, ProdCount)) {
        return false;   // ������ ��������� �� ���������. ���� �� �������, ������� �� ��������� � false
    }
    size_t tmp1, tmp2;                                      // ��������� ���������� - "��������"
    if(ImportSingleArray(ImConfig.filename_Production, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows,\
    volume, Production, ProdNames, tmp1, tmp2)) {
        mancalc = nocalc;                   // ���� ������ �������, ������ � ���������� ���� ������� �������
        ShipShare = dZero;                  // � �������� ������ �� ������ ���
        ImConfig.S_Share = ShipShare;
    } else {
        ShipShare = ImConfig.S_Share;
        ImConfig.filename_Production = NoFileName;
    }
    cout << "���� �����������/ ����������� �������������� ������� ������� ������������: " << \
        PurchaseCalcTXT[mancalc] << endl;
    cout << "����� ��������� �� ��� � ����� �� ������� ��������: " << fixed << setprecision(sTwo)\
        << ShipShare << endl;
    if(!ImportSingleArray(ImConfig.filename_Purchase, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows,\
    price, Purchase, RMNames, tmp1, RMCount)) {
        return false;   // ������ ��������� �� ���������. ���� �� �������, ������� �� ��������� � false
    }
    strItem* tmpPurchase = nullptr;                         // ������� ��������� ���������
    std::swap(tmpPurchase, Purchase);                       // ������������� ��������� �� ������ ���������� ���
    strNameMeas* tmpNames;                                  // ��������� ���������� - "��������"
    if(!ImportSingleArray(ImConfig.filename_Purchase_V, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows,\
    volume, Purchase, tmpNames, tmp1, tmp2)) {  // ���� ������ ���������, �� ���������� ��������� � �������� ���������
        std::swap(tmpPurchase, Purchase);       // ���� ������ ���������, �� ���������� ��������� � �������� ���������
        PurchShare = ImConfig.P_Share;
        ImConfig.filename_Purchase_V = NoFileName;
    } else {
        purcalc = nocalc;                       // ���� ������ �������, ������ � ���������� ���� ������� �������
        PurchShare = sZero;                     // � �������� ������ �� ������ ���
        ImConfig.P_Share = PurchShare;
        strItemReplace(RMCount*PrCount, Purchase, tmpPurchase, nmBPTypes::price);     // ��� ������� �������
                        // � ���� price ������ ������� Purchase ������ ������ ����� price ������� tmpPurchase
        delete[] tmpPurchase;                       // ������� �������� ������
        for(size_t i{}; i<(RMCount*PrCount); i++)   // ������������ ���� volume ������� Purchase
            (Purchase+i)->value = (Purchase+i)->volume * (Purchase+i)->price;
    }
    // ������ ��������� �� ������� ������� � ���������� ���� ������� �������
    cout << "���� �����������/ ����������� �������������� ������� ������� �������: " << \
        PurchaseCalcTXT[purcalc] << endl;
    cout << "����� �������� �� ��� � ����� �� ������� ��������: " << fixed << setprecision(sTwo)\
        << PurchShare << endl;
    if(!Import_Recipes(ImConfig.filenameprefix_Recipes, ImConfig._ch, ImConfig.HeadCols, ImConfig.HeadRows)) {
        return false;   // ������ ��������� � ����������. ���� �� �������, ������� �� ��������� � false
    };
    /** ��������� ������� � ��������������� ����������� **/
    if(P_settings) delete[] P_settings;     // ���� ������ ����������, �� ������� ���
    P_settings = (ImConfig.filename_Purchase_V == NoFileName) ? nullptr : ImConfig.GetPurSettings();
    if(S_settings) delete[] S_settings;     // ���� ������ ����������, �� ������� ���
    S_settings = (ImConfig.filename_Production == NoFileName) ? nullptr : ImConfig.GetShpSettings();
    ImConfig.SaveToFile(Configure_filename);// ��������� ������������ � ����
    return true;
}   // clsEnterprise::Import_Data

/** �������� � ������ ������ ������� ��������� (���)  **/

bool clsEnterprise::SetWarehouse() {
/** ����� �������� ������ ������� ��������� (���) � ����� ����������. **/
    if((ProdCount==sZero) || (!ProdNames) || (!Shipment)) return false;             // ��������� ������� ����������
    clsStorage* temp = new(nothrow) clsStorage(PrCount, Cur, Amethod, ProdCount);   // �������� ������ �������
    if(!temp) {
        cout << "����� ������� ��������� (���) ������� �� �������" << endl;         // ���� ������ �� ��������, ��
        return false;                                                               // ����� � false
    }
    bool ch = temp->SetStorage(ProdCount, ProdNames, Shipment, Production); // ������ ��������� ������
    if(!ch) {                                                               // ���� ����� �� ������, ��
        delete temp;                                                        // ������� ��������������� ������
        return false;                                                       // � ����� � false
    }
    if(Warehouse) {                                             // ���� ������ �����������, ��
        Warehouse->swap(*temp);                                 // ���������� ��������� �
        delete temp;                                            // ������� ��������������� ������
    } else Warehouse = temp;                                    // ����� ������������� ��������� Warehouse �� ������

    if(S_settings) {                                            // ���� ������ � ����������� ����������, ��
        for(size_t i{}; i<ProdCount; i++) {                     // ������������� ��������� ��� ������ ������� ������:
            Warehouse->SetShare(i, (S_settings+i)->share);      // �������� ������ ������� ���������
            Warehouse->SetAutoPurchase(i, (S_settings+i)->calc);// ���� ��������������� ������� �����������
            Warehouse->SetPermission(i, (S_settings+i)->perm);  // ���������� �� �������� � ����������� � ����� �������
        }
    } else {                                        // ���� �� ������ �� ����������, �� ���������� ����� ���������:
        for(size_t i{}; i<ProdCount; i++) {         // ��� ������� �������� ��� �������������
            Warehouse->SetShare(i, ShipShare);      // �������� ������
            Warehouse->SetAutoPurchase(i, mancalc); // ���� ��������������� ������� �����������
            Warehouse->SetPermission(i, Ship_indr); // ���������� �� �������� � ����������� � ����� �������
        };
    }
    return true;
}   // clsEnterprise::SetWarehouse

void clsEnterprise::StockEditSettings(SelectDivision stk) {
/** ����� ����������� ��������� ����� ������ (��� ��� ���) ��� ������� SKU: �����, �������� � ��. ��������� SKU,
���� ����������� �������, ���� ���������� �� �������� � ������� � ����� ������� � �������� ������
�� ������. **/
    size_t num;                                 // ����� �������������� �������, ��������� ������� ���� �������������
    clsStorage* tmpstock;                       // ��������������� ���������
    if(stk == warehouse) tmpstock = Warehouse;  // ���� ������ ���, �� ��������� ���������� �� Warehouse,
    else tmpstock = RawMatStock;                // ����� ��������� ���������� �� RawMatStock
    showSKUsettings(std::cout, tmpstock);       // ���������� ������� ��������� ���� SKU
    while(true) {                               // ����������� ��������� ��� ��������� SKU
        cout << "������� ����� �������������� ������� ��� ��������������"\
                " [��� ������ �������� \"q\" � ������� \"Enter\"]: ";
        if(!(cin >> num)) break;                // ���� ����� �� ������, �� ����� �� ���������,
        SKUEdt(tmpstock, num);                  // ����� ����������� ��������� SKU
    }
    cin.clear();                                        // ������� ���� ������
    cin.ignore(numeric_limits<streamsize>::max(),'\n'); // ������� ����� �����
    /** ���������� �������������� �������� � ���������������� ���� � ���� �������� ���������� ������ **/
    size_t pcount;                                                  // ��������������� ����������
    if(stk == warehouse) pcount = ProdCount;                        // � ����������� �� stk ����� ProdCount
    else pcount = RMCount;                                          // ��� RMCount
    strSettings* tmp_stttings = new(nothrow) strSettings[pcount];   // �������� ������ ���������������� �������
    if(tmp_stttings) {                                              // ���� ������ ��������, ��
        for(size_t i{}; i<pcount; i++) {                            // ��������� � ������ ��������� ������;
            (tmp_stttings+i)->share = tmpstock->Share(i);
            (tmp_stttings+i)->calc  = static_cast<PurchaseCalc>(tmpstock->GetAutoPurchase(i));
            (tmp_stttings+i)->perm  = tmpstock->PermissionBool(i);
        }
        strImportConfig ImConfig;                                   // ��������� ����������
        typedef bool (strImportConfig::*TypeSetSettings)(const strSettings*, const size_t); // ����������� ����
        if(ImConfig.ReadFromFile(Configure_filename)) {             // ������ ���������������� ����, ���� ������
            TypeSetSettings f;                                      // ��������������� ���������� ��� �������
            if(stk == warehouse) f = &strImportConfig::SetShpSettings;  // �������� �������
            else f = &strImportConfig::SetPurSettings;
            (ImConfig.*f)(tmp_stttings, pcount);                    // ��������� � ������������ ������ ��������
            bool res = ImConfig.SaveToFile(Configure_filename);     // � ��������� ������������ � ����
            if(!res) cout << "�� ������� ��������� ������������ � ���� " << Configure_filename << endl;
        }
        if(stk == warehouse) std::swap(tmp_stttings, S_settings);   // ������������ ����������
        else std::swap(tmp_stttings, P_settings);
        if(tmp_stttings) delete[] tmp_stttings;
    }
}   // clsEnterprise::StockEditSettings

bool clsEnterprise::StockCalculate(const SelectDivision& _dep, size_t thr) {
/** ����� ������������ ��������� ����� �������/����������� �� ����� ��� ��� SKU, � ������� ��������� �����������
����� ������ ����. ����� ������ (��� ��� ���) �������������� ��������� ��������� _dep: "warehouse" - ���, �����
������ �������� - ���. ��������� ���������� thr ��������������� ����� ����������: "1" -
TLack clsStorage::Calculate_future, "2" - TLack clsStorage::Calculate_thread, ����� TLack clsStorage::Calculate. **/
    TLack (clsStorage::*f)();                           // ���������� ��������� �� ���������� ������� ������ clsStorage
    if((thr == 1) || (thr == 2)) {                      // �������� ������� ��� ���������
        if(thr == 1) f = &clsStorage::Calculate_future; // ������ � ����������� �������
        else f = &clsStorage::Calculate_thread;         // ������ � ������� thread
    } else f = &clsStorage::Calculate;                  // ����� ���������������� ����������
    TLack Deficit{};                            // ��������������� ����������
    clsStorage* val = nullptr;                  // ��������������� ���������
    string dep;                                 // �������� ������
    if(_dep == warehouse) {                     // �������� �����: ���
        val = Warehouse;
        dep = "���";
    } else {                                    // ��� ���
        val = RawMatStock;
        dep = "���";
    };
    if(!val) return false;                      // ���� ����� �� ������, �� �����
    /** �������� � ��������� ���������� ��������� **/
    size_t SKUcount = val->Size();              // ��������� ���������� SKU
    type_progress* progress =\
        new type_progress(cout, ProgressWide, "������ " + dep, '.', SKUcount);  // ��������-���
    clsProgress_shell<type_progress>* shell =\
        new clsProgress_shell<type_progress>(progress, static_cast<int>(SKUcount));         // ��������
    val->Set_progress_shell(shell);             // ������������� ��������� ���������
    /** ���������� **/
    Deficit = (val->*f)();                      // �������� ����������
    delete progress;                            // ������� ��������-���
    delete shell;                               // ������� �������� ��������-����
    val->Set_progress_shell(nullptr);           // ������������� ���������� ��������� � nullptr
    if(fabs(Deficit.lack) > epsln) {            // ���� ������� ������� �� ����� ����, �� ����� � false
        cout << "����������� �� ����� � �������� � ���� �� ��������������. ������� ����� " <<\
        scientific << Deficit.lack << endl;
        cout << "��� �������� � ������������� " << Deficit.Name << endl << endl;
        return false;
    };
    /** ��������� ������� Production & Shipment ��� Purchase & Consumpt **/
    strItem* tmpPurch = val->GetPure();         // ��������� ��������������� ������ ������ ������� � ������������
    strItem* tmpShip = val->GetShip();          // ��������� ��������������� ������ ������ ������� �� ���������
    if(_dep == warehouse) {
        std::swap(tmpPurch, Production);
        std::swap(tmpShip, Shipment);
    } else {
        std::swap(tmpPurch, Purchase);
        std::swap(tmpShip, Consumpt);
    }
    if(tmpPurch) delete[] tmpPurch;             // ������� ������, ���� �� �����������
    if(tmpShip) delete[] tmpShip;               // ������� ������, ���� �� �����������
    return true;
}   // clsEnterprise::StockCalculate

/** �������� � ������ ������������ **/

bool clsEnterprise::SetManufactory() {
/** ����� �������� ������������ � ����� ���������� **/
    if((PrCount==sZero) || (RMCount==sZero) || (!RMNames)) return false;                        // ��������� ����������
    clsManufactory* temp = new(nothrow) clsManufactory(PrCount, RMCount, RMNames, ProdCount);   // �������� ������
    if(!temp) {                                                 // ���� ������ �� ��������
        return false;                                           // ����� � false
    };
    temp->SetCurrency(Cur);                                     // ������ ������������ ������
    size_t i{};                                                 // ������ �������� (��������� ����������)
    for(vector<clsRecipeItem>::const_iterator cit=Recipe.cbegin(); cit!=Recipe.cend(); cit++) {
        bool ch = temp->SetManufItem(*cit, (Production + PrCount*i)); // ������� ��������� ������������
                                                                        // ��� �������� � �������� i
        if(!ch) {                                               // ���� ������������ �� �������, ��
            delete temp;                                        // ������� ��������������� ������
            return false;                                       // � ������� � false
        };
        i++;                                                    // ����������� ������� ���������
    };
    if(Manufactory) {                                           // ���� ������ �����������, ��
        Manufactory->swap(*temp);                               // ���������� ��������� �
        delete temp;                                            // ������� ��������������� ������
    } else Manufactory = temp;                                  // ����� ������������� ��������� Warehouse �� ������
    return true;
}   // clsEnterprise::SetManufactory

void clsEnterprise::ManufCalculateIn(size_t thr) {
/** ����� ������������ ����� ����������� ����� � ���������� � ������������ � ����������� ���������
��� ����� ����� ������� ���� ���������. ��������� ���������� thr ��������������� ����� ����������:
"1" - void clsManufactory::CalcRawMatPurchPlan_future, "2" - void clsManufactory::CalcRawMatPurchPlan_thread,
����� void clsManufactory::CalcRawMatPurchPlan. **/
    void (clsManufactory::*f)();        // ���������� ��������� �� ���������� ������� ������ clsManufactory
    if((thr == 1) || (thr == 2)) {      // �������� ������� ��� ���������:
        if(thr == 1) f = &clsManufactory::CalcRawMatPurchPlan_future;   // ������ � ����������� �������
        else f = &clsManufactory::CalcRawMatPurchPlan_thread;           // ������ � ������� thread
    } else f = &clsManufactory::CalcRawMatPurchPlan;                    // ����� ���������������� ����������
    /** �������� � ��������� ���������� ��������� **/
    type_progress* progress =\
        new type_progress(cout, ProgressWide, "������ �������� � ������������", '.', ProdCount);    // ��������-���
    clsProgress_shell<type_progress>* shell =\
        new clsProgress_shell<type_progress>(progress, static_cast<int>(ProdCount));                // ��������
    Manufactory->Set_progress_shell(shell);         // ������������� ��������� ���������
    /** ���������� **/
    (Manufactory->*f)();                            // �������� ����������
    delete progress;                                // ������� ��������-���
    delete shell;                                   // ������� �������� ��������-����
    Manufactory->Set_progress_shell(nullptr);       // ������������� ���������� ��������� � nullptr
    strItem* tempConsumpt = Manufactory->GetRMPurchPlan();  // �������� ����������� ������ �����������
    if(tempConsumpt)                                        // ���� ������ ������, ��
        std::swap(tempConsumpt, Consumpt);                  // ���������� ���������
    if(tempConsumpt) delete[] tempConsumpt;                 // ������� ��������������� ������
}   // clsEnterprise::ManufCalculateIn

bool clsEnterprise::ManufCalculateOut(size_t thr) {
/** ����� ������������ �����, �������� � ������ ������������� �������������� ������������ �
������� ��������� ��� ���� ���������, ����������� �� ���������� ����� �������. ���������
���������� thr ��������������� ����� ����������: "1" - bool clsManufactory::Calculate_future(),
"2" - bool clsManufactory::Calculate_thread(), ����� bool clsManufactory::Calculate(). **/
    bool (clsManufactory::*f)();        // ���������� ��������� �� ���������� ������� ������ clsManufactory
    if((thr == 1) || (thr == 2)) {      // �������� ������� ��� ���������
        if(thr == 1) f = &clsManufactory::Calculate_future; // ������ � ����������� �������
        else f = &clsManufactory::Calculate_thread;         // ������ � ������� thread
    } else f = &clsManufactory::Calculate;                  // ����� ���������������� ����������
    /** �������� � ��������� ���������� ��������� **/
    type_progress* progress =\
        new type_progress(cout, ProgressWide, "������ ���������������� �������������", '.', ProdCount);    // ��������-���
    clsProgress_shell<type_progress>* shell =\
        new clsProgress_shell<type_progress>(progress, static_cast<int>(ProdCount));                // ��������
    Manufactory->Set_progress_shell(shell);     // ������������� ��������� ���������
    /** ���������� **/
    bool result = (Manufactory->*f)();          // �������� ����������
    delete progress;                            // ������� ��������-���
    delete shell;                               // ������� �������� ��������-����
    Manufactory->Set_progress_shell(nullptr);   // ������������� ���������� ��������� � nullptr
    /** ��������� ���� price � value � ������� Production **/
    if(result) {
        strItem* tempProduction = Manufactory->GetTotalProduct();   // �������� ����������� ������ ��������
        if(!tempProduction) return false;                           // ���� ��� �������, �� ����� � false
        std::swap(tempProduction, Production);                      // ���������� ���������
        if(tempProduction) delete[] tempProduction;                 // ������� ��������������� ������
    }
    return result;                          // ���������� ���������
}   // clsEnterprise::ManufCalculateOut

bool clsEnterprise::MWCostTransmition() {
/** ����� �������� ������� ������������� ������������� ��������� �� ������������ �� ����� ������� ��������� **/
    if(!Production) return false;           // ���� ������ �� ���������� (nullptr), �� ����� � false
    if(!Warehouse->SetPurPrice(Production)) // ������ ������� ������������� ��������� � ���, ���� ��� ��
        return false;                       // � ������� � false
    return true;
}   // clsEnterprise::MWCostTransmition

/** ��������  � ������ ������ ����� � ���������� (���) **/

bool clsEnterprise::SetRawMatStock() {
/** ����� �������� ������ ����� � ���������� (���) � ����� ����������. **/
    if((RMCount==sZero) || (!RMNames) || (!Consumpt)) return false;             // ��������� ������� ����������
    clsStorage* temp = new(nothrow) clsStorage(PrCount, Cur, Amethod, RMCount); // �������� ������ �������
    if(!temp) {                                                                 // ���� ������ �� ��������, ��
        cout << "����� ����� � ���������� ������� �� �������" << endl;          // �������� ������������ �
        return false;                                                           // ������� � false
    }
    bool ch = temp->SetStorage(RMCount, RMNames, Consumpt, Purchase);          // ������ ��������� ������
    if(!ch) {                                       // ���� ����� �� ������, ��
        delete temp;                                // ������� ��������������� ������
        return false;                               // � ����� � false
    }
    if(RawMatStock) {                               // ���� ������ �����������, ��
        RawMatStock->swap(*temp);                   // ���������� ��������� �
        delete temp;                                // ������� ��������������� ������
    } else RawMatStock = temp;                      // ����� ������������� ��������� Warehouse �� ������
    if(P_settings) {                                // ���� ������ � ����������� ����������, ��
        for(size_t i{}; i<RMCount; i++) {           // ������������� ��������� ��� ������ ������� ������:
            RawMatStock->SetShare(i, (P_settings+i)->share);        // �������� ������ ������� ���������
            RawMatStock->SetAutoPurchase(i, (P_settings+i)->calc);  // ���� ��������������� ������� �����������
            RawMatStock->SetPermission(i, (P_settings+i)->perm);    // ���������� �� �������� � ����������� � ����� �������
        }
    } else {                                        // ���� �� ������ �� ����������, �� ���������� ����� ���������:
        for(size_t i{}; i<RMCount; i++) {         // ��� ������� �������� ��� �������������
            RawMatStock->SetShare(i, PurchShare);       // �������� ������
            RawMatStock->SetAutoPurchase(i, purcalc);   // ���� ��������������� ������� �����������
            RawMatStock->SetPermission(i, Purch_indr);  // ���������� �� �������� � ����������� � ����� �������
        };
    }
    return true;
}   // clsEnterprise::SetRawMatStock

bool clsEnterprise::RMCostTransmition() {
/** ����� �������� ������� ������������� ����� � ���������� �� ������ ����� � ���������� � ������������ **/
    PriceBus = RawMatStock->GetShipPrice(); // ��������� �� ����� ������ � ������� �������������� �����
    if(!PriceBus) return false;             // ���� ������ �� ������ (nullptr), �� ����� � false
    if(!Manufactory->SetRawMatPrice(PriceBus)) {    // ������ ������� ������������� ����� � ������������, ���� ��� ��
        return false;                               // � ������� � false
    }
    return true;
}   // clsEnterprise::RMCostTransmition

/** ������ ��� ����������� �������� � ������� **/

void clsEnterprise::ReportView(const SelectDivision& _rep, const int _arr, const ReportData flg) const {
/** ������� ������� ��������� �����. ���������: _rep - ��������� ����� ������ (warehouse - ���, manufactory
- ������������, rowmatstock- ���), _arr -  ��� ������ (purchase - ������ �����������. balance - ������ ��������,
shipment - ������ ��������). �������� flg - ��� ��������� ����������: volume - � �����������, value - � �����������,
price - � ������� ��������� **/
    bool (clsEnterprise::*f) (clsStorage* obj, const int, const ReportData) const;  // ��������� �� �������
//    Typerepfun f;                                           // ��������������� ��������� �� ������� ���� Typerepfun
    if((_rep == warehouse) || (_rep == rowmatstock)) {      // ���� ������ ��� ��� ���, ��
        f = &clsEnterprise::Report_Storage_to_dev;          // ��������� ���������� �� ������� Report_Storage_to_dev
        if(_rep == warehouse)
            {if((this->*f)(Warehouse, _arr, flg)) return;}  // � ������� ����� �� ��� � �������
        else
            {if((this->*f)(RawMatStock, _arr, flg)) return;}// ����� ������� ����� �� ��� � �������
    }
    else {
        if(Report_Manufactory(_arr, flg)) return;           // ������� ����� � �������
    }
}   // clsEnterprise::ReportView

void clsEnterprise::StockSettingsView(const SelectDivision& _rep) {
/** ������� ������� ������������� ��������� ������. ���������: _rep - ��������� ����� (warehouse - ���,
rowmatstock ��� ����� ������ - ���) **/
    clsStorage* stock;                  // ��������������� ���������
    std::ofstream ofs;                  // ��������������� ���������� ��� ������ ������ � ����
    std::ostream* os;                   // ��������������� ���������� ��� ������ ������
    if(Rdevice == nulldev) {            // ���� ������� "������" ����������, ��
        std::ostream strNull(nullptr);  // ��������� ������ �����
        os = &strNull;
    };
    if(Rdevice == terminal) {           // ���� ������ ����� �� �����, ��
        os = &std::cout;                // ��������� ���������� os ����������� ������ �� cout
    };
    if(Rdevice == file) {                       // ���� ������ ����� � ����, ��
        ofs.open(RName, std::ofstream::app);    // ��������� ����� � ������ � ��������� ��� �� ������
                                                // � ������ ���������� � �������������
        os = &ofs;                              // ��������� ���������� os ����������� ������ �� �������� �����
    };
    if(_rep == warehouse) {
        stock = Warehouse;              // ��������� �� ���
    } else {
        stock = RawMatStock;            // ��������� �� ���
    }
    showSKUsettings(*os, stock);        // ������� �����
    *os << '\n';
    if(Rdevice == file) ofs.close();    // ��������� ����, ���� ��� ��� �������� �����
    return;
}   // clsEnterprise::StockSettingsView

/** Export - ������ **/

bool clsEnterprise::Export_Data(string filename, const SelectDivision& _dep, const ChoiseData& _arr, \
    const ReportData& flg) const {
/** ����� ���������� ������ ��������, �������� ��� �������� �� ������ ������� ��������� (���), ������ �����
� ���������� (���) ��� � ������������ � csv-���� � ������ filename. ���������: _dep - ���� ������ ������:
"warehouse" - ���, "rowmatstock" - ���, "manufactory" - ������������; _arr - ����� ������: "purchase" -
��������, "balance" - �������/������������� ������������, "shipment" - ��������; flg - ��� ��������� � ����
����������: volume - � �����������, value - � �����������, price - � ������� ���������. � �������� �����������
����� ������ ������������ ������ _ch �� ��������� (';'). **/
    strItem* (clsStorage::*fSdata)() const;     // ���������� ��������� �� ���������� ������� ������ clsStorage
    clsStorage* val = nullptr;                  // ��������������� ���������
    strItem* (clsManufactory::*fMdata)() const; // ���������� ��������� �� ���������� ������� ������ clsManufactory
    const size_t (clsManufactory::*fMcount) () const;    // ��� ��������� �� ������� �������� ����� ��������/���������
    strNameMeas* (clsManufactory::*fMnames)() const;     // ��� ��������� �� ������� �������� �������� ��������/���������
    if(_dep == manufactory) {
        if(!Manufactory) return false;                   // �������� ������������� ������������
        if((_arr==balance) || (_arr==shipment)) {
            fMcount = &clsManufactory::GetProdCount;            // ��������� �� ������� �������� ����� ���������
            fMnames = &clsManufactory::GetProductDescription;   // ��������� �� ������� �������� �������� ���������;
            if(_arr==balance)                                   // � ������ �������������� ������������ ��������� �� �������
                fMdata = &clsManufactory::GetTotalBalance;      // �������� ������� � ������������� �������������
            else fMdata = &clsManufactory::GetTotalProduct;     // ����� �������� ������� � ������� ����������
        } else {                                                // ����� ��������� �� �������
            fMcount = &clsManufactory::GetRMCount;              // �������� ����� ������� ����� � ����������
            fMnames = &clsManufactory::GetRawMatDescription;    // �������� ��������� �� ����� ������ �������� ����� � ����������
            fMdata = &clsManufactory::GetRMPurchPlan;           // �������� ��������� �� ����� ������ ����������� �����
        }
    } else {
        if(_dep == warehouse) val = Warehouse;  // ����� ������: ���
        else val = RawMatStock;                 // ��� ���
        if(!val) return false;                  // �������� ������������� ������
        if((_arr==purchase) || (_arr==balance))                 // ���������� ��������� �������:
            if(_arr==purchase) fSdata = &clsStorage::GetPure;   // �������� ��������� �� ������ ����������� �� �����
            else fSdata = &clsStorage::GetBal;                  // �������� ��������� �� ������ �������� �� ������
        else fSdata = &clsStorage::GetShip;                     // �������� ��������� �� ������ �������� �� ������
    }

    size_t NameCount = (_dep == manufactory) ? ((Manufactory->*fMcount)()) : (val->Size());
    strNameMeas* pNames = (_dep == manufactory) ? ((Manufactory->*fMnames)()) : (val->GetNameMeas());
    if(!pNames) return false;                                   // ���� ������ � ������� ����, �� ����� � false
    strItem* pData = (_dep == manufactory) ? ((Manufactory->*fMdata)()) : ((val->*fSdata)());
    if(!pData) {                                                // ���� ������ � ������� ����, ��
        delete[] pNames;                                        // ������� ��������������� ������ � �������
        return false;                                           // � ������� � false
    };

    clsImpex* Data = new clsImpex(NameCount, pNames, pData, PrCount, flg);  // ������� ������ � ������ � ���� ������
    delete[] pNames;                            // ������� ��������� ������ � �������
    delete[] pData;                             // ������� ��������� ������ � �������
    if(Data->is_Empty()) {                      // ���� ������ �� ������, ��
        return false;                           // � ������� � false
    };
    filename.push_back('_');                    // ��������� � ����� ����� ������ �����������
    filename.append(DBLR_ind());                // ��������� ������� ���� ������������� �����
    filename +=".csv";                          // ���������� ���������� �����
    ofstream output(filename, std::ofstream::trunc);    // �������� ���� �� ������
    if(output.is_open()) {                              // ���� ���� ������, ��
        Data->csvExport(output);                        // ���������� ������ � ����
        output.close();                                 // ��������� ����
    } else {
        delete Data;
        return false;
    }
    delete Data;                                // ������� ������
    return true;
}   // clsEnterprise::Export_Data

/******************************** ������ protected **************************************************/

/** Import - ������ **/

bool clsEnterprise::Import_About(const string _filename) {
/** ����� ������ ���������� �� ����� � ��������� ������� � ��������� ���� Title � Descript ������
clsBaseProject. **/
    string filename = FullFName(indir, _filename);  // ��������� ������ ��� �����
    ifstream input(filename);                       // ��������� ���� � ������ filename �� ������
    if(input.is_open()) {                           // ���� ���� ������:
        stringstream ss;                            // ��������������� ��������� �����
        const size_t Charlimit = nmRePrint::smblcunt+nmRePrint::uThree; // ������ ������������ ����� ������
        size_t strCount = sZero;                    // �������� ������� �����
        for(string line; getline(input, line);) {   // ������ � ���������� line ������ ������ �����
            if(input.bad()) {                       // ���� ������ �� ����� �� �������, ��
                return false;                       // ������� � false
            };
            if(line.length()<=Charlimit) {      // ���� ����� ������ �� ������ ������������, ��
                ss << move(line) << endl;       // ���������� �� � �����, �������� � ���� ������ ����� ������
                strCount++;                     // � ����������� ������� �����
            }
            else {                                          // ���� ����� ������ ������ ������������, ��
                size_t nums = sZero;                        // �������� ������� ��������,
                for(size_t i{}; i<line.length(); i++) {     // ������ ������ �����������
                    ss << move(line[i]);                    // ��������� ������ ������ � ��������� �����
                    if(nums==Charlimit) {                   // ��� ������ ����� �������� ������� �� �������������:
                        if(((i+1)<line.length()) && (line[i+1] != ' '))   // ���� ��������� ������ �� ������, ��
                            ss << '-';          // ��������� ���� �������� �����
                        nums = sZero;           // �������� ������� ��������,
                        ss << endl;             // ��������� ������ �������� ������
                        strCount++;             // ����������� ������� �����
                    };
                    nums++;                     // ����������� ������� ��������
                };
                ss << endl;                     // ��������� ������ �������� ������ � �����
                strCount++;                     // ����������� ������� �����
            };
        };
        input.close();                                                      // ��������� ����
        string* ArrString = new(nothrow) string[strCount];                  // �������� ������ �������
        if(!ArrString) return false;            // ���� ������ �� ��������, �� ������� � false
        for(size_t i{}; i<strCount; i++)        // ���� �� ���������� ����� � ������
            getline(ss, *(ArrString+i), '\n');  // ������ � ����� ������ �� ������ �� ������� �������� ������
        SetTitle(*ArrString);                   // ������ �������� �������
        SetComment(ArrString+1, strCount-sOne); // ������ �������� �������
        delete[] ArrString;                     // ������� ��������������� ������
        return true;
    }
    else return false;
}   // Import_About

bool clsEnterprise::ImportSingleArray(const string _filename, const char _ch, size_t hcols, size_t hrows,\
    ReportData flg, strItem* &_data, strNameMeas* &_names, size_t& ColCount, size_t& RowCount) {
/** ����� ������ ���������� �� ����� � ������ filename � ������������� ����� ������ ch � ��������� ����:
RowCount - ����� �������������� ������� (�������� ��� ���������), ColCount - ����� �������� �������,
names - ������ �� ��������� �� ������ � �������������� �������������� ������� � ������ �� ���������,
_data - ������ �� ��������� �� ����������� ������, flg - ����, ������������ ��� ������������� ������:
"volume" - ������ � ����������� ���������, "price" - ����, "value" - ���������. **/
    string filename = FullFName(indir, _filename);  // ��������� ������ ��� �����
    ifstream input(filename);                       // ��������� ���� � ������� �� ������
    const char ch = _ch;                        // �������� �����������
    clsImpex* Data = new clsImpex(input, ch);   // ������� ����� ��� ������� � ����������� ������ �� �����
    input.close();                              // ��������� ���� � ��������� �������
    if(Data->is_Empty()) {                      // ���� ������ �� ������, ��
        delete Data;                            // ������� ������
        return false;                           // � ������� � false
    };
    ColCount = Data->GetColCount()-hcols;       // �������� ����� �������� �������
    RowCount = Data->GetRowCount()-hrows;       // �������� ����� �������������� ������� (�������� ��� ���������)
    strItem* tmpdata;                           // ��������� ����������-���������
    if((flg==volume) || (flg==price))           // �������� ��������� �� ������� ������ � �������
        if(flg==volume) tmpdata = Data->GetstrItem(hrows, RowCount, hcols, ColCount+hrows, volume);
        else tmpdata = Data->GetstrItem(hrows, RowCount, hcols, ColCount+hrows, price);
    else tmpdata = Data->GetstrItem(hrows, RowCount, hcols, ColCount+hrows, value);
    _names = Data->GetNames(hrows, RowCount, sZero, hrows);   // �������� ��������� �� ��������
    delete Data;                                // ������� ������ ��� �������
    std::swap(_data, tmpdata);                  // ������������ ������ �� ������� ���������
    return true;
}   // clsEnterprise::ImportSingleArray

bool clsEnterprise::Import_Recipes(const string _filename, const char _ch, size_t hcols, size_t hrows) {
/** ����� ������ ���������� �� ������ � �������, ����������� ������� filename � ���������������� �� _i, ���
i- ����� ���������. � �������� ����������� ������������ ������ _ch. ����� ��������� ������ �������� Recipe. **/
    string filename = FullFName(indir, _filename);      // ��������� ������ ��� �����
    Recipe.reserve(ProdCount);                          // ����������� ������ ��� ��������� �������
    string file;                                        // ��������� ���������� ��� ����� �����
    ifstream rec;                                       // ����� ��� ������ �� �����
    clsImpex* Data = new(nothrow) clsImpex();           // ������� ��������� ������ ��� �������
    for(size_t i{}; i<ProdCount; i++) {
        file = filename + '_' + to_string(i) + ".csv";  // ��������� ��� ����� � ����������
        rec.open(file);                                 // ��������� ����� � ������
        if(!Data->Import(rec, _ch)) {                   // ����������� ������ �� �����. ���� ������ �� ������,
            rec.close();                                // �� ��������� ����;
            delete Data;                                // ������� ��������� ������ ��� �������
            EraseVector(Recipe);                        // �������� ������ � ��������� �� ���������
            return false;                               // � ������� � false
        };
        rec.close();                                    // ��������� ����
        string _name = (ProdNames+i)->name;             // �������� �������� ��������
        string _meas = (ProdNames+i)->measure;          // �������� �������� ������� ��������� ��������
        size_t _duration = Data->GetColCount()-hcols;   // �������� ������������ ����������������� �����
        size_t _rcount = Data->GetRowCount()-hrows;     // �������� ���������� ������� ����� � ���������
        strNameMeas* _rnames = Data->GetNames(hrows, _rcount, sZero, hrows);// ��. �� ������ � ������� � ��. ��������� ����� � ���������
        MeasRestore(RMNames, _rnames, RMCount, _rcount);                    // ��������������� ��. ��������� �����
        decimal* _recipeitem = Data->GetDecimal(hrows, _rcount, hcols, hcols); // �������� ��������� ��������
        Recipe.emplace_back(_name, _meas, _duration, _rcount, _rnames, _recipeitem); // ������� ������ "���������" � �������
        delete[] _rnames;                               // ������� ��������������� ������
        delete[] _recipeitem;                           // ������� ��������������� ������
        Data->reset();                                  // ���������� ��������� ������� �� ����������
    };
    delete Data;
    return true;
}   // Import_Recipes

/** ������ �������������� **/

bool clsEnterprise::SKUEdt(clsStorage* stock, const size_t num) {
/**  ����� �������������� ��������� ����� ����������: �������������� �������������� �������
(SKU). ���������: stock - ��������� �� ���������� ����� (Warehouse ��� RawMatStock, num -
����� ������������� �������������� �������. **/
    size_t choise;                              // ��������������� ���������� ��� ����� ������
    double ps_tmp;                              // ��������������� ���������� ��� ����� ������
    PurchaseCalc tcalc = (stock == Warehouse) ? mancalc : purcalc;          // ����������� ��������
    if(num > (stock->Size()-sOne)) {                                        // ��������� ������ SKU
        cout << "����� �������������� ������� ������ �������. �� �� ������ ���� ������ " <<\
        (stock->Size()-sOne) << endl;
        return false;
    }
    auto lambda = [&stock, &num](){             // ������� ������ ������� ���������� �� �����
        cout << " ������������ �������:                                     " << stock->Name(num) << endl;
        cout << " ������� ��������� ������������ ������:                    " << stock->Measure(num) << endl;
        cout << " ���� ���������� ����������� � �������� � ����� �������:   " << stock->Permission(num) << endl;
        cout << " ���� ��������������� ������� ����������� �� �����:        " << stock->AutoPurchase(num) << endl;
        cout << " �������� ������ �������������� ������� �� ������:         " << stock->Share(num) << endl;
    };  // lambda
    cout << "������� ��������� ����� ��� ������� � " << num << endl;
    lambda();
    cout << "������� ����� ��������� ����� ��� ������� � " << num << endl;
    cout << " ���� ���������� ����������� � �������� � ����� ������� [PROHIBITED=0, ALLOWED=1]: ";
    cin >> choise;
    if(choise <=1) stock->SetPermission(num, static_cast<bool>(choise));    // ��������� � ���� ���������
    cout << " ���� ��������������� ������� ����������� �� ����� [AUTO=0, MANUAL=1]: ";
    PurchaseCalc pcl;                           // ��������������� ���������� ��� ����� �����
    cin >> choise;
    if(choise <=1) {                            // ��������� � ���� ���������
        pcl = static_cast<PurchaseCalc>(choise);
        if((tcalc == calc) && (pcl == nocalc)) {
            pcl = calc;
            cout << "����� ������� ������� ����������� ���������� ������: ������ � ������������ �� �������!" << endl;
        }
        stock->SetAutoPurchase(num, pcl);
    }
    cout << " �������� ������ �������������� ������� �� ������ (���� �� ��������)   ";
    cin >> ps_tmp;
    stock->SetShare(num, ps_tmp);
    cout << "����� ��������� ����� ��� ������� � " << num << endl;
    lambda();
    return true;
}   // clsEnterprise::SKUEdt

/** ������ ��� ����������� �������� � ������� **/

void clsEnterprise::reportstream(ostream& os) const {
/** ����� ������� ���������� � ������� � ����� os. ����� �������������� ����������� ����� ������ **/
    clsBaseProject::reportstream(os);                   // ����� ������� ����������
    os << "���������� �������� �������                          " << PrCount << "\n";
    os << "�������� ������ �������                              " << *(nmBPTypes::CurrencyTXT+Cur) << "\n";
    os << "������� ����� �������                                " << *(AccountTXT+Amethod) << "\n";
    os << "���������� ������� � ������������ ������� ���������  " << ProdCount << "\n";
    os << "���������� ������� � ������������ ����� � ���������� " << RMCount << "\n";
    /** ����������� ��� �������� ����� ������������� ������� ��������� **/
    os << "\n";
}   // reportstream

void clsEnterprise::showSKUsettings(ostream& os, clsStorage* stock) const {
/** ����� ������ � �������� ����� �������� ����� �� ������ ��� ������� SKU: �����, �������� �
��. ��������� SKU, ���� ����������� �������, ���� ���������� �� �������� � ������� � ����� �������
� �������� ������ �� ������, ���������: os - ����� ��� ������, stock - ��������� �� ���������� �����
(Warehouse ��� RawMatStock. **/
//    cin.ignore(numeric_limits < streamsize > ::max(),'\n');
    size_t stock_count = stock->Size();             // �������� ���������� SKU
    strNameMeas* tmpNames = stock->GetNameMeas();   // �������� ��������� �� ����� ��������� ������ ����
    for(size_t i{}; i< stock_count; i++) {          // ��� ������� ������������
        (tmpNames+i)->name.resize(w1);              // �������� ����� �������� SKU
        (tmpNames+i)->measure.resize(w2);           // �������� ����� �������� ������ ���������
    }
    clsTextField name(w1);  // ��������� ������� ������ ���� � ������� ������ clsTextField
    clsTextField meas(w2);  // ��������� ������� ������ ��. ��������� � ������� ������ clsTextField
    clsTextField perm(w2);  // ��������� ������� ������ ����� ���������� ����������� � �������� � ����� �������
    clsTextField apur(w2);  // ��������� ������� ������ ����� ����������� �����������
    clsDataField nSKU(4);   // ��������� ������� ������ ������ SKU � ��������� ���������� ������
    /** ����� ������� � ��������������� ����������� **/
    string msgHead;                                 // ����� ��������� �������
    if(stock == Warehouse) msgHead = "�������������� ��������� ����� ��������� �� ���";
    else msgHead = "�������������� ��������� ����� �������� �� ���";
    nmRePrint::PrintHeader1(os, nmRePrint::smblcunt+nmRePrint::uThree, msgHead);
    os << nSKU   << "��"<< stSpc;                   // ����� ��������� 0-�� ������� �������
    os << name   << "��������" << stSpc;            // ����� ��������� 1-�� ������� �������
    os << meas   << "��.���������" << stSpc;        // ����� ��������� 2-�� ������� �������
    os << perm   << "���� �������" << stSpc;        // ����� ��������� 3-�� ������� �������
    os << apur   << "����������" << stSpc;          // ����� ��������� 4-�� ������� �������
    os << nSKU   << "�����" << stSpc;               // ����� ��������� 5-�� ������� �������
    os << endl;
    for(size_t i{}; i<stock_count; i++) {           // ����� ������ � �������
        os << nSKU   << i                      << stSpc;
        os << name   << (tmpNames+i)->name     << stSpc;
        os << meas   << (tmpNames+i)->measure  << stSpc;
        os << perm   << stock->Permission(i)   << stSpc;
        os << apur   << stock->AutoPurchase(i) << stSpc;
        os << nSKU   << stock->Share(i)        << stSpc;
        os << endl;
        if(os.rdbuf() == std::cout.rdbuf())         // ���� ������������ ����� ����������� cout, �� ��������� �����
                                                    // �� ��������� �� 10 ����� ��� ������������� � ������
            if((i!=sZero) && (i!=stock_count-1) &&(i%num_strings) == sZero) {
                os << "��� ����������� ������ ������� ������� \"Enter\"";
                cin.ignore();
//                cin.get();
                cin.clear();
            }
    }   // for(size_t i{}; i<stock_count; i++)
    if(tmpNames) delete[] tmpNames;                 // ���� ������ ���� ����������, �� ������� ���
}   // clsEnterprise::showSKUsettings

bool clsEnterprise::Report_Storage(clsStorage* obj, const int _arr, const ReportData flg) const {
/** ����� ��� ������ �� ����� ������� �� ������� ���� clsStorage (Warehouse ��� RawMatStock). �������� obj -
��������� ������ clsStorage. ��������� �������: _arr = purchase, ������ ����������� �� �����; _arr = balance,
������ �������� �� ������; _arr = shipment, ������ �������� �� ������. �������� flg - ��� ��������� ����������:
volume - � �����������, value - � �����������, price - � ������� ���������. ���� �������� _arr �� ��������� �
�������������� ���� �� ������ warehouse_module.h enum ChoiseData{purchase = sZero, balance, shipment}, �� �����
�� ���������: ������ �������� _arr ������������� ��� ������ ������� **/
    size_t NameCount = obj->Size();             // ���������� SKU � ������
    if(NameCount == sZero) return false;        // ���� ����� ������, �� ����� � false
    strNameMeas* pNames = obj->GetNameMeas();   // �������� ��������� �� ����� ������ � ���������� �������� � ��. ���������
    if(!pNames) return false;                   // ���� ������ � ��������, �� ����� � false
    strItem* (clsStorage::*f)() const;          // ���������� ��������� �� ���������� ������� ������ clsStorage
    if(_arr==purchase) f = &clsStorage::GetPure;// ���������� ��������� �������:
    else                                        // �������� ��������� �� ������ ����������� �� �����
        if(_arr==balance) f = &clsStorage::GetBal;          // �������� ��������� �� ������ �������� �� ������
        else
            if(_arr==shipment) f = &clsStorage::GetShip;    // �������� ��������� �� ������ �������� �� ������
            else {                                          // ���� �� ������� purchase, balance ��� shipment,
                if(pNames) delete[] pNames;                 // ������� ��������������� ������ � �������
                return false;                               // � ������� � false
            }
    strItem* pData = (obj->*f)();               // �������� ��������� �� ������ � �������
    if(!pData) {                                // ���� ������ ������, ��
        if(pNames) delete[] pNames;             // ������� ��������������� ������ � �������
        return false;                           // � ������� � false
    };
    /** ��������� ����� **/
    std::ofstream ofs;                  // ��������������� ���������� ��� ������ ������ � ����
    nmRePrint::clsRePrint<strItem, strNameMeas>* MyRep = new nmRePrint::clsRePrint<strItem, strNameMeas>(w1, w2, w3);
    MyRep->SetHeadings(rTableName, rTableMeas, rByVolume, rByPrice, rByValue);  // ������ ��������� �������
    MyRep->SetCurrency(Cur);                                                    // ��������� ������
    MyRep->SetPrecision(precis);                // ��������� ���������� ������ ����� �������
    if(Rdevice == nulldev) {                    // ���� ������� "������" ����������, ��
        std::ostream strNull(nullptr);          // ��������� ������ �����
        MyRep->SetStream(strNull);              // � ������������� ����� ������ � ������ �����
    };
    if(Rdevice == terminal)                     // ���� ������ ����� �� �����, ��
        MyRep->SetStream(std::cout);            // ������������� ����� ������ �� �����
    if(Rdevice == file) {                       // ���� ������ ����� � ����, ��
        ofs.open(RName, std::ofstream::app);    // ��������� ����� � ������ � ��������� ��� �� ������
                                                // � ������ ���������� � �������������
        MyRep->SetStream(ofs);                  // ������������� ����� ������ � �������� �����
    };
    if(MyRep->SetReport(NameCount, pNames, pData, PrCount, flg)) {
        MyRep->Print(flg);                      // ���������� �����, ���� ����������,
        if(Rdevice == file) ofs.close();        // ��������� ����, ���� ��� ��� �������� �����
    }
    else {                                      // � ��������� ������:
        if(Rdevice == file) ofs.close();        // ��������� ����, ���� ��� ��� �������� �����
        if(pNames) delete[] pNames;             // ������� ��������������� ������
        if(pData) delete[] pData;               // ������� ��������������� ������
        if(MyRep) delete MyRep;                 // ������� �����
        return false;                           // � ������� � false
    };
    if(pNames) delete[] pNames;                 // ������� ��������������� ������
    if(pData) delete[] pData;                   // ������� ��������������� ������
    if(MyRep) delete MyRep;                     // ������� �����
    return true;
}   // clsEnterprise::Report_Storage

bool clsEnterprise::Report_Storage_to_dev(clsStorage* obj, const int _arr, const ReportData flg) const {
/** ����� ���������� �����, ��������� ������� Report_Storage �� ��������� ����������: "������" ����������, �����
��� ���� (������������ ����������� ���������� Rdevice). ���������� ����� Report_Storage. ��������� ��������� �
����������� ������ Report_Storage. **/
    string divsn;                       // ��������������� ����������: �������� ������
    string swork;                       // ��������������� ����������: �������� ���������� �������
    std::ofstream ofs;                  // ��������������� ���������� ��� ������ ������ � ����
    std::ostream* os;                   // ��������������� ���������� ��� ������ ������
    if(Rdevice == nulldev) {            // ���� ������� "������" ����������, ��
        std::ostream strNull(nullptr);  // ��������� ������ �����
        os = &strNull;
    };
    if(Rdevice == terminal) {           // ���� ������ ����� �� �����, ��
        os = &std::cout;                // ��������� ���������� os ����������� ������ �� cout
    };
    if(Rdevice == file) {                       // ���� ������ ����� � ����, ��
        ofs.open(RName, std::ofstream::app);    // ��������� ����� � ������ � ��������� ��� �� ������
                                                // � ������ ���������� � �������������
        os = &ofs;                              // ��������� ���������� os ����������� ������ �� �������� �����
    };
    if(obj == Warehouse) divsn = dWarehouse;    // ������ ��� ������������� ���
    else divsn = dRawMatStock;                  // ������ ��� ������������� ���
    if(_arr == purchase) swork = aPurchase;     // ����������� ��������������� ����������
    if(_arr == balance)  swork = aBalance;      // �������� ���������������� �������
    if(_arr == shipment) swork = aShipment;     // ��� ������ ��������� ������
    nmRePrint::PrintHeader2(*os, nmRePrint::smblcunt+nmRePrint::uThree, divsn+swork);   // ������� ��������� ������
    if(Rdevice == file) ofs.close();            // ��������� ����, ���� ��� ��� �������� �����
    return Report_Storage(obj, _arr, flg);      // ������� �����
}   // clsEnterprise::Report_Storage_to_dev

void clsEnterprise::Report_Recipe() const {
/** ����� ��� ������ � ����� �������� ���� ��������� **/
    string swork;                       // ��������������� ����������
    std::ofstream ofs;                  // ��������������� ���������� ��� ������ ������ � ����
    std::ostream* os;                   // ��������������� ���������� ��� ������ ������ �� �����
    size_t PrdCnt = Manufactory->GetProdCount();    // �������� ����� ����� ��������� � ������������
    for(size_t i=sZero; i<PrdCnt; i++) {                                // ���� �� ���� ��������� �������������
        strNameMeas tprod_id = Manufactory->GetNameItem(i);             // �������� ������������ �������� � ������� ���������
        size_t trcount = Manufactory->GetRCount(i);                     // �������� ����� ������� ����� � ���������
        const strNameMeas* trnames = Manufactory->GetRawNamesItem(i);   // �������� ��������� �� ����. ������ � �������������� ����� � ���������
        const decimal* trecipe = Manufactory->GetRecipeItem(i);         // �������� ��������� �� ����. ������ � ���������� ��������
        size_t tduration = Manufactory->GetDuration(i);                 // �������� ������������ ����������������� �����

                nmRePrint::clsRePrint<decimal, strNameMeas>* MyRep0 =\
                    new(nothrow) nmRePrint::clsRePrint<decimal, strNameMeas>(w1, w2, w3);   // ������� �����
                if(!MyRep0) return;                                                         // ���� ����� �� ������, �� �������
                MyRep0->SetHeadings(rTableName, rTableMeas, rByVolume, rByPrice, rByValue); // ������ ��������� �������
                MyRep0->SetCurrency(Cur);                                                   // ��������� ������
                MyRep0->SetPrecision(precis);                                               // ���������� ������ ����� �������
                if(Rdevice == nulldev) {            // ���� ������� "������" ����������, ��
                    std::ostream strNull(nullptr);  // ��������� ������ �����
                    MyRep0->SetStream(strNull);     // � ������������� ����� ������ � ������ �����
                    os = &strNull;
                };
                if(Rdevice == terminal) {           // ���� ������ ����� �� �����, ��
                    MyRep0->SetStream(std::cout);   // ������������� ����� ������ �� �����
                    os = &std::cout;
                };
                if(Rdevice == file) {               // ���� ������ ����� � ����, ��
                    ofs.open(RName, std::ofstream::app);    // ��������� ����� � ������ � ��������� ��� �� ������
                                                            // � ������ ���������� � �������������
                    MyRep0->SetStream(ofs);                 // ������������� ����� ������ � �������� �����
                    os = &ofs;
                };
                if(MyRep0->SetReport(trcount, trnames, trecipe, tduration)) {
                    *os << aRecipe1 << tprod_id.name << endl;
                    *os << aRecipe2 << tprod_id.measure << endl;
                    for(size_t j=sZero; j<(w1+w2+w3+2); j++) {*os << '-'; };
                    MyRep0->Print();                        // ���� ����� �����������, �� �������� ���,
                };
                if(Rdevice == file) ofs.close();        // ��������� ����, ���� ��� ��� �������� �����
                if(MyRep0) delete MyRep0;
    }
}   // clsEnterprise::Report_Recipe

bool clsEnterprise::Report_Manufactory(const int _arr, const ReportData flg) const {
/** ����� ��� ������ �� ����� ������� �� �����������. ��������� �������: _arr = manpurchase, ������  �����������
� ����� � ����������; _arr = manbalance, ������ c ��������� �������������� ������������ ��� ���� ���������;
_arr = manshipment, ������ �������� �� ���; _arr = recipe, ��������� ���������. flg - ��� ��������� ����������:
volume - � �����������, value - � �����������, price - � ������� ���������. ���� �������� _arr �� ��������� �
�������������� ���� �� ������� ������ enum ManufData{manpurchase = 11, manbalance, manshipment, recipe}, �� �����
�� ��������� **/
    string swork;                                               // ��������������� ����������
    if((Manufactory->GetRMCount()) == sZero) return false;      // ���� ��� ����� � ����������, �� ����� � false
    if((Manufactory->GetProdCount()) == sZero) return false;    // ���� ��� ���������, �� ����� � false
    if((Manufactory->GetPrCount()) == sZero) return false;      // ���� ����� �������� ������� ����� ����, �� ����� � false
    if(_arr == recipe) {                                        // ���� ������ ����� "���������", ��
        cout << endl;
        Report_Recipe();                        // ������� �� ����� ��������� ���� ���������
        return true;
    };
    strItem* (clsManufactory::*fs)() const;     // ���������� ��������� �� ���������� ������� ������ clsManufactory
    /** ��������� ����� **/
    std::ofstream ofs;                          // ��������������� ���������� ��� ������ ������ � ����
    if(_arr == manpurchase) {
        decimal* pData = Manufactory->GetRawMatPurchPlan(); // �������� ������ �������� RMCount*PrCount � ������������ � �����
        if(!pData) return false;
        nmRePrint::clsRePrint<decimal, strNameMeas>* MyRep1 = new nmRePrint::clsRePrint<decimal, strNameMeas>(w1, w2, w3);
        MyRep1->SetHeadings(rTableName, rTableMeas, rByVolume, rByPrice, rByValue); // ������ ��������� �������
        MyRep1->SetCurrency(Cur);                                                   // ��������� ������
        MyRep1->SetPrecision(precis);                                               // ���������� ������ ����� �������
        bool checkrez = true;                                                       // ��������� ��������
        if(Rdevice == nulldev) {            // ���� ������� "������" ����������, ��
            std::ostream strNull(nullptr);  // ��������� ������ �����
            MyRep1->SetStream(strNull);     // � ������������� ����� ������ � ������ �����
        };
        if(Rdevice == terminal)             // ���� ������ ����� �� �����, ��
            MyRep1->SetStream(std::cout);   // ������������� ����� ������ �� �����
        if(Rdevice == file) {               // ���� ������ ����� � ����, ��
            ofs.open(RName, std::ofstream::app);    // ��������� ����� � ������ � ��������� ��� �� ������
                                                    // � ������ ���������� � �������������
            MyRep1->SetStream(ofs);                 // ������������� ����� ������ � �������� �����
        };
        if(MyRep1->SetReport(RMCount, RMNames, pData, PrCount)) {   // ���� ����� �����������, ��
            MyRep1->Header2(dManufactory+aPurchase);                // ����� ���������
            MyRep1->Print();                                        // �������� ���,
        }
        else checkrez = false;                      // ���� ���, �� ������ ��������� �������� �� false
        if(Rdevice == file) ofs.close();            // ��������� ����, ���� ��� ��� �������� �����
        if(pData) delete[] pData;
        if(MyRep1) delete MyRep1;
        return checkrez;
    } else                                                                  // ���������� ��������� �������:
        if(_arr == manbalance) {
            fs = &clsManufactory::GetTotalBalance;      // �������� ��������� �� ������ �������� � ������������
            swork = aBalance;
        } else
            if(_arr == manshipment) {
                fs = &clsManufactory::GetTotalProduct;  // �������� ��������� �� ������ �������� �� ������������
                swork = aShipment;
            } else return false;
    strItem* pData = (Manufactory->*fs)();                                  // �������� ��������� �� ������ � �������
    if(!pData) return false;
    const size_t ProdCount = Manufactory->GetProdCount();                   // �������� ����� ���������
    strNameMeas* ProdNames = Manufactory->GetProductDescription();  // �������� ������ � ������� � ��.��������� ���� ���������
    nmRePrint::clsRePrint<strItem, strNameMeas>* MyRep2 = new nmRePrint::clsRePrint<strItem, strNameMeas>(w1, w2, w3);
    MyRep2->SetHeadings(rTableName, rTableMeas, rByVolume, rByPrice, rByValue);         // ������ ��������� �������
    MyRep2->SetCurrency(Cur);
    MyRep2->SetPrecision(precis);
    bool checkrez = true;
    if(Rdevice == nulldev) {            // ���� ������� "������" ����������, ��
        std::ostream strNull(nullptr);  // ��������� ������ �����
        MyRep2->SetStream(strNull);     // � ������������� ����� ������ � ������ �����
    };
    if(Rdevice == terminal)             // ���� ������ ����� �� �����, ��
        MyRep2->SetStream(std::cout);   // ������������� ����� ������ �� �����
    if(Rdevice == file) {               // ���� ������ ����� � ����, ��
        ofs.open(RName, std::ofstream::app);    // ��������� ����� � ������ � ��������� ��� �� ������
                                                // � ������ ���������� � �������������
        MyRep2->SetStream(ofs);                 // ������������� ����� ������ � �������� �����
    };
    if(MyRep2->SetReport(ProdCount, ProdNames, pData, PrCount, flg)) {  // ���� ����� �����������, ��
        MyRep2->Header2(dManufactory+swork);                            // ����� ���������
        MyRep2->Print(flg);                                             // �������� ���,
    }
    else checkrez = false;                      // ���� ���, �� ������ ��������� �������� �� false
    if(Rdevice == file) ofs.close();            // ��������� ����, ���� ��� ��� �������� �����
    if(pData) delete[] pData;
    if(MyRep2) delete MyRep2;
    if(ProdNames) delete[] ProdNames;
    return checkrez;
}   // clsEnterprise::Report_Manufactory
