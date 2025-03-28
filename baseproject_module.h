#ifndef BASEPROJECT_MODULE_H_INCLUDED
#define BASEPROJECT_MODULE_H_INCLUDED

#include <iostream>         // ������������ ���� � ��������, ��������� � ����������� ��� ����������� �����-������
#include <fstream>          // ������������ ���� ��� ����������� �����/ ������ � ���� �� �����
#include <string>           // ������������ ���� � ��������, ��������� � ����������� ��� ������ �� ��������
#include "common_values.hpp"
#include "serialization_module.hpp"

using namespace std;

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                               ��������� � �������� ������ clsBaseProject                                            **/
/**                                                                                                                     **/
/*************************************************************************************************************************/
/** version from 2024.10.13 **/

enum Tdev{nulldev=nmBPTypes::sZero, terminal, file};    // ��������� ������ ���������� ������: nulldev - ������ ����������,
                                                        // terminal - ����� �� �����, file - ����� � ����
class clsBaseProject
{
    private:
        string Title;           // �������� �������
        struct Descript {       // ��������� � ��������� �������:
            string *sComment;   // ��������� �� ������ ����� � ���������,
            size_t  sCount;     // ���������� � ����������� ��������� � ������� (���������� �����)
        } About;                // ����������, ���������� ���������. ��������� ��� ������������� ������

    public:
        clsBaseProject();                                               // ����������� �� ���������
        virtual ~clsBaseProject();                                      // ����������� ����������
        clsBaseProject(const clsBaseProject& other);                    // ����������� �����������
        virtual void swap(clsBaseProject& other) noexcept;              // ������� ������ ���������� ����� ���������
        clsBaseProject(clsBaseProject&& other);                         // ����������� �����������
        clsBaseProject& operator=(const clsBaseProject& other);         // ���������� ��������� ������������ ������������
        clsBaseProject& operator=(clsBaseProject&& other);              // ���������� ��������� ������������ ������������
        void SetTitle(const string& _title);                            // ����� ����� ������ �������
        void SetComment(const string* _comment, const size_t& _count);  // ����� ����� �������� �������
        void SetFName(const string _filename);          // ����� ����� ����� ����� ��� ������������ � ��������������
        void SetFName(const string _filename, const string _ext);   // ���� ����� ����� � ��� ����������
        void SetRName(const string _filename);          // ����� ����� ����� ����� ��� ������
        void SetDevice(const Tdev& val);                // ����� ������������� �������� ���������� ��� ������
        virtual void Reset();                           // ����� ���������� ��� ����������
//        virtual void View() const;                      // ����� ����������� ��������
//        virtual void VPrint(const string _filename);    // ����� ������ ����������� ���������� � ����
        void Report() const;                            // ����� ������ ������ �� �����
//        void Report(const string filename) const;       // ����� ������ ������ � ����
//        void Report(const Tdev dev) const;              // ����� ������ ������ �� ��������� � ���������� dev ����������
        bool SaveToFile(const string _filename);        // ����� ������ �������� ���������� ������ � ����
        bool SaveToFile();                              // ����� ������ �������� ���������� ������ � ����
        bool ReadFromFile(const string _filename);      // ����� ������ �� ����� � ������ � ��������� ������
        bool ReadFromFile();                            // ����� ������ �� ����� � ������ � ��������� ������

    protected:
        string FName;           // ���������� � ������ ����� ��� ������/ ������ ��������� ���������� ������ �� ����
        string RName;           // ���������� � ������ ����� ��� ������ ���������� � ������� ������ ������ � ����
        Tdev Rdevice;                                   // ���������� � ��������� ���������� ��� ������ ������
        virtual bool StF(ofstream &_outF);              // ����� ������������� ������ � �������� ����������
        virtual bool RfF(ifstream &_inF);               // ����� ������������� ������ �� �������� ����������
        virtual void reportstream(ostream& os) const;   // ����� ������ ������ � �����
};  // clsBaseProject

#endif // BASEPROJECT_MODULE_H_INCLUDED
