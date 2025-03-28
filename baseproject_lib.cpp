#include "baseproject_module.h"

namespace nmbaseproject {
    const size_t  sZero = 0;
    const string EmpStr ="";
}   // nmbaseproject

namespace nmlocalBP {
    const auto buff = nullptr;          // ���������� ��� �������� ������ ������ ������ � "������ �����"

    string makefilename(const string _name, const string ext) {
    /** ����� ��������� ��� ����� ��� ������������/ ������ �� ���������, �������� _name - ������ ��� ������������ �����
    (������ � ��� ������������) ��������� � ���������� argv[0] � ���������� ������ main **/
        string fname=_name;                 // ��������������� ����������
        size_t pos;                         // ���������� ��� �������, ������� � ������� ������ ������ ��� �����
        const int cFour = 4;
        pos = fname.rfind(".exe");          // ����� ��������� � �������� ������ � ����������� ������� ��������� (� �����)
        if(pos == (fname.length()-cFour))   // ���� ��������� ����, �� ������ ���������� ".exe" ��
            fname.replace(pos, cFour, ext); // ���������� ���������� ext � ������ ����� ����� � ��������� �������,
        else fname+=ext;                    // ����� ������ ��������� ������ ���������� ����� �����
        return fname;
    } // makeFname

} // nmlocalBP

/*************************************************************************************************************************/
/**                                         ������� ����� clsBaseProject                                                **/
/*************************************************************************************************************************/
/** version from 2024.10.13 **/

using namespace nmbaseproject;
using namespace nmlocalBP;

clsBaseProject::clsBaseProject() {
/** ����������� ��� ���������� **/
    Title = EmpStr;                 // ������������� ������ �������� �������
    About.sComment = nullptr;       // ������������� ��������� �� ������ ����� � ��������� ������� �� "�����"
    About.sCount = sZero;           // ������������� ������� ����� ����� � �������� �������
    FName = RName = EmpStr;         // ��� ������ �� ������
    Rdevice = terminal;             // ���������� �� ���������
}   // Default Ctor

clsBaseProject::~clsBaseProject() {
/** ����������� ���������� **/
    if((About.sCount > sZero) || (About.sComment))
        delete[] About.sComment;
}   // Dtor

clsBaseProject::clsBaseProject(const clsBaseProject& other) {
/** ����������� ����������� **/
    Title = other.Title;
    FName = other.FName;
    RName = other.RName;
    Rdevice = other.Rdevice;
    About.sCount = other.About.sCount;
    if(About.sComment) delete[] About.sComment;
    if(other.About.sComment)
        About.sComment = new(nothrow) string[About.sCount];
    else
        About.sComment = nullptr;   // ������������� ��������� �� ������ ����� � ��������� ������� �� "�����"
    if(About.sComment)
        for(size_t i=sZero; i<About.sCount; i++)
            *(About.sComment+i) = *(other.About.sComment+i);    // ����������� �����
}   // Copy Ctor

void clsBaseProject::swap(clsBaseProject& other) noexcept {
/** ������� ������ ���������� ����� ���������. ������� ��������� noexcept - �� ���������� ���������� **/
    std::swap(Title, other.Title);                      // ���������� ��������
    std::swap(About.sComment, other.About.sComment);    // ���������� ���������
    std::swap(About.sCount, other.About.sCount);        // ���������� ��������
    std::swap(FName, other.FName);                      // ���������� ��������
    std::swap(RName, other.RName);                      // ���������� ��������
    std::swap(Rdevice, other.Rdevice);                  // ���������� ��������
}   // swap

clsBaseProject::clsBaseProject(clsBaseProject&& other) {
/** ����������� ����������� **/
    Title = EmpStr;                 // ������������� ������ �������� �������
    About.sComment = nullptr;       // ������������� ��������� �� ������ ����� � ��������� ������� �� "�����"
    About.sCount = sZero;           // ������������� ������� ����� ����� � �������� �������
    FName = RName = EmpStr;         // ��� ������ �� ������
    Rdevice = nulldev;              // ������ ���������� "�����"
    swap(other);                    // ������������ ���������� � ����������� � other
}   // Move Ctor

clsBaseProject& clsBaseProject::operator=(const clsBaseProject& other) {
/** ���������� ��������� ������������. ����������� � ������ �����������-�-������ (copy-and-swap idiom)  **/
    clsBaseProject tmp(other);      // �������� ����������� ����������� � �������� ����� other � ���������� tmp
    swap(tmp);                      // ������������ ����������� � tmp
    return *this;
}   // Copy operator=

clsBaseProject& clsBaseProject::operator=(clsBaseProject&& other) {
/** ���������� ��������� ������������ ������������ ������������ ������� **/
    swap(other);                        // ������������ ����������� � other
    return *this;
}   // Move operator=

void clsBaseProject::SetTitle(const string& _title) {
/** ����� ����� ������ ������� **/
    Title = _title;
}   // SetTitle

void clsBaseProject::SetComment(const string* _comment, const size_t& _count) {
/** ����� ����� �������� �������. ���������:  _comment - ��������� �� ������ �����, _count - ������ �������**/
    if((!_comment) || (_count == sZero)) return;        // �������� ������������ �������� ������
    About.sCount = _count;
    if(About.sComment)                                  // ���� ������ ����������,
        delete[] About.sComment;                        // �� ������� ���
    About.sComment = new(nothrow) string[About.sCount]; // �������� ������ ������ �������
    if(About.sComment)                                  // ���� ������ ��������, ��
        for(size_t i=sZero; i<About.sCount; i++)        // �������� � ����� ������ ������
            *(About.sComment+i) = *(_comment+i);
}   // SetComment

void clsBaseProject::SetFName(const string _filename) {
/** ����� ����� ����� ����� ��� ������������ � �������������� **/
    FName = makefilename(_filename, ".dat");
}   // SetFName

void clsBaseProject::SetFName(const string _filename, const string _ext) {
/** ����� ����� ����� ����� ��� ������������ � �������������� **/
    FName = _filename + _ext;
}   // SetFName

void clsBaseProject::SetRName(const string _filename) {
/** ����� ����� ����� ����� ��� ������ **/
    RName = makefilename(_filename, ".txt");
}   // SetRName

void clsBaseProject::SetDevice(const Tdev& val) {
/** ����� ������������� �������� ���������� ��� ������. �������� val - ������ ����������:
nulldev - ������ ����������, terminal - ����� �� �����, file - ����� � ���� **/
    Rdevice = val;
} // SetDevice

void clsBaseProject::Reset() {
/** ����� ���������� ��� ���������� � ���������� ������ �� "�����" **/
    clsBaseProject tmp;         // ������� "������" ���������� tmp
    swap(tmp);                  // ������������ ����������� � tmp
}   // Reset

//void clsBaseProject::View() const {
///** ����� ����������� ��������. ������� ���������� �� ����� **/
//    cout << Title << endl << endl;
//    for(size_t i=sZero; i<About.sCount; i++) {
//        cout << *(About.sComment+i) << endl;
//    }
//}   // View

//void clsBaseProject::VPrint(const string _filename) {
///** ����� ����������� ��������. ������� ���������� � ��������� ���� **/
//    if(_filename == EmpStr) return;                     // ���� ��� ����� ������, �� �����
//    ofstream out(_filename);                            // ������� ���� ��� ������
//    streambuf* coutbuf = cout.rdbuf();                  // �������� ������ ����� � ���������� coutbuf
//    cout.rdbuf(out.rdbuf());                            // �������������� ����� � ����: ������ ��� ����� � ����
//    View();                                             // ������� ���������� � ������� � �����
//    cout.rdbuf(coutbuf);                                // ��������������� ����� �� �����
//    out.close();                                        // ��������� ����
//}   // VPrint

//void clsBaseProject::Report() const {
///** ����� ����������� ��������. ������� ���������� �� ����� **/
//    reportstream(std::cout);
//}   // Report()

void clsBaseProject::Report() const {
/** ����� ������ ������ �� ��������� ���������� **/
    if(Rdevice == nulldev) {
        std::ostream strNull(buff);
        reportstream(strNull);
        return;
    };
    if(Rdevice == terminal) {
        reportstream(std::cout);
        return;
    };
    if(Rdevice == file) {
        if(RName == EmpStr) return;                     // ���� ��� ����� ������, �� �����
        ofstream out(RName);                            // ������� ���� ��� ������
        if(out.is_open())                               // ���� ���� ������, ��
            reportstream(out);                          // ������� � ����� ����������
        out.close();                                    // � ��������� ����
        return;
    }
}   // Report()

//void clsBaseProject::Report(const string _filename) const {
///** ����� ����������� ��������. ������� ���������� � ��������� ���� **/
//    string filename;                                    // ��������������� ����������
//    if(_filename != EmpStr) filename = _filename;       // ���� ��� ����� �� ������, �� ���������� ���
//    else                                                // �����
//        if(RName != EmpStr) filename = RName;           // ���� ��� �� ��������� �� ������, �� ���������� ���,
//        else return;                                    // ����� �����
////    if(filename == EmpStr) return;                      // ���� ��� ����� ������, �� �����
//    ofstream out(filename);                             // ������� ���� ��� ������
//    if(out.is_open())                                   // ���� ���� ������, ��
//        reportstream(out);                              // ������� � ����� ����������
//    out.close();                                        // � ��������� ����
//}   // Report(...)

//void clsBaseProject::Report(const Tdev dev) const {
///** ����� ������ ������ �� ��������� ����������. ���������: dev - ������ ���������� **/
//    if(dev==nulldev) {
//        std::ostream strNull(buff);
//        reportstream(strNull);
//        return;
//    };
//    if(dev==terminal) {
//        reportstream(std::cout);
//        return;
//    };
//    if(dev==file) {
//        if(RName == EmpStr) return;                     // ���� ��� ����� ������, �� �����
//        ofstream out(RName);                            // ������� ���� ��� ������
//        if(out.is_open())                               // ���� ���� ������, ��
//            reportstream(out);                          // ������� � ����� ����������
//        out.close();                                    // � ��������� ����
//        return;
//    }
//}   // Report(const Tstream dev)

bool clsBaseProject::SaveToFile(const string _filename) {
/** ����� ������ �������� ���������� ������ � ���� **/
    if(_filename == EmpStr) return false;               // ���� ��� ����� ������, �� ����� � ������� false
    ofstream outF(_filename, ios::out | ios::binary);   // ��������� ��� ����� � �������� ������� ��� ������
    if (!outF.is_open()) {                              // ���� ���� �� ������, �� ��������� ������������ �
        cout << "������ �������� �����" <<endl;         // ������� false � ����� �� �������
        return false;
    };
    if(!StF(outF)) {                                    // ���������� ������ � ����. ���� ���� �� �������, ��
        outF.close();                                   // �������� ���� �
        return false;                                   // ������� � false
    };
    outF.close();                                       // ��������� ����
    return true;                                        // ���������� true
}   // SaveToFile

bool clsBaseProject::SaveToFile() {
/** ����� ������ �������� ���������� ������ � ���� � ������ �� ��������� FName **/
    if(!SaveToFile(FName)) return false;
    return true;
}   // SaveToFile

bool clsBaseProject::ReadFromFile(const string _filename) {
/** ����� ������ �� ����� � ������ � ��������� ������ **/
    if(_filename == EmpStr) return false;               // ���� ��� ����� ������, �� ����� � ������� false
    ifstream inF(_filename, ios::in | ios::binary);     // ��������� ��� ����� � �������� ������� ��� ������
    if (!inF.is_open()) {                               // ���� ���� �� ������, �� ��������� ������������ �
        cout << "������ �������� �����" <<endl;         // ������� false � ����� �� �������
        return false;
    };
    if(!RfF(inF)) {                                     // ��������� ������ �� �����. ���� ���� �� �������, ��
        inF.close();                                    // ��������� ����
        return false;                                   // ����� � ������� false
    }
    inF.close();                                        // 4.   ��������� ����
    return true;
}   // ReadFromFile

bool clsBaseProject::ReadFromFile() {
/** ����� ������ �� ����� � ������ �� ��������� � ������ � ��������� ������ **/
    if(!ReadFromFile(FName)) return false;
    return true;
}   // ReadFromFile

bool clsBaseProject::StF(ofstream &_outF) {
/** ����� ������������� ������ � �������� ���������� �������� ���������� ������ (������ � ����, �����
������������). ���������: &_outF - ��������� ������ ofstream ��� ������ ������ **/
    if(!SEF(_outF, Title)) return false;
    if(!SEF(_outF, About.sCount)) return false;
    for(size_t i=sZero; i<About.sCount; i++)
        if(!SEF(_outF, *(About.sComment+i))) return false;
    if(!SEF(_outF, FName)) return false;
    if(!SEF(_outF, RName)) return false;
    return true;
}   // StF

bool clsBaseProject::RfF(ifstream &_inF) {
/** ����� ������������� ������ �� �������� ���������� �������� ���������� ������ (������ �� �����, �����
��������������). ���������: &_inF - ��������� ������ ifstream ��� ������ ������ **/
    if(About.sComment) delete[] About.sComment;
    if(!DSF(_inF, Title)) return false;
    if(!DSF(_inF, About.sCount)) return false;
    About.sComment = new(nothrow) string[About.sCount];
    if(!About.sComment) return false;
    for(size_t i=sZero; i<About.sCount; i++)
        if(!DSF(_inF, *(About.sComment+i))) return false;
    if(!DSF(_inF, FName)) return false;
    if(!DSF(_inF, RName)) return false;
    return true;
}   // RfF

void clsBaseProject::reportstream(ostream& os) const {
/** ����� ������� ����� � ����� os **/
    nmRePrint::PrintHeader0(os, nmRePrint::smblcunt+nmRePrint::uThree, Title);
//    os << Title << "\n\n";                      // ����� �������� �������
    for(size_t i=sZero; i<About.sCount; i++) {  // ����� �������� �������
        os << *(About.sComment+i) << "\n";
    };
    os << "\n";
}   // reportstream
