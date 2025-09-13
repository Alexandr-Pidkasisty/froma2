#ifndef FROMA2_PROGRESS_MODULE_HPP
#define FROMA2_PROGRESS_MODULE_HPP

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

#include <ostream>
#include <string>
#include <iomanip>
#include <atomic>

using namespace std;

class clsprogress_bar {
/** ������� ������� ��� ��� �������. ������ �� ������� ������ Toby Speight �
https://codereview.stackexchange.com/questions/186535/progress-bar-in-c **/

    private:
        static const int overhead = sizeof"[100%]"; // ������ ���� � ����������
        ostream &os;                                // �������� ����� ��� ������ ����������
        const int bar_width;                        // ������ ����������
        string message;                             // ��������� �� ����� ������ ����������
        const string full_bar;                      // ������, ��������� ����� ������� ��������� ��� ���������
        int maxcount;                               // ������������ ����� ��������, ��������������� 100% ����������

        void write(double fraction) {
        /** ����� ������ ��������-���� **/
            const double dZero = 0.0;
            const double dOne  = 1.0;
            if(fraction < dZero) fraction = dZero;                              // ��������� ��������� �� ���������
            else if(fraction > dOne) fraction = dOne;                           // � �������� [0, 1]
            int width = bar_width - message.size();                             // ������ ���������� ��� ���������
            int offset = bar_width - static_cast<unsigned>(width * fraction);   // ������ �������� � ������� ��������,
                                                                                // ��������������� �������� fraction
            os << '\r' << message;                          // ����� ���������
            os.write(full_bar.data() + offset, width);      // ����� ����� ������, ��������������� �������� fraction
            os << "[" << std::setw(3) << static_cast<int>(100.0 * fraction) << "%]" << std::flush;  // ����� ���������
        }   // clsprogress_bar::write

    public:
        clsprogress_bar(ostream &_os, int const line_width, string _message, const char symbol='.', const int _mx=100):
            os(_os),
            bar_width(line_width-overhead),                                     // ������ ������ ����������
            message(std::move(_message)),
            full_bar(string(bar_width, symbol) + string(bar_width, ' ')),       // �������� ������ ��� ������ �����
            maxcount(_mx)  {
                if(static_cast<int>(message.size()+1) >= bar_width || message.find('\n')!=message.npos) {
                    os << message << '\n';
                    message.clear();
                }
                else {
                    message += ' ';
                }
        }   // Ctor

        clsprogress_bar(const clsprogress_bar&) = delete;               // ������ Copy Ctor
        clsprogress_bar& operator=(const clsprogress_bar&) = delete;    // ������ Copy =

        ~clsprogress_bar() {
            os << '\n';
        }   // Dtor

        void Update(int value) {
        /** ����� ������ ��������-���� **/
            write(value/static_cast<double>(maxcount));
        }   // clsprogress_bar::write

        void Set_maxcount(const int _mx) {
        /** ������������� ����� �������� ������������� ����� �������� **/
            maxcount = _mx;
        }   // Set_maxcount

};  // clsprogress_bar

template <typename T>
class clsProgress_shell {
/** �������� ��� ������ ������ ��������-���� ���� T. �����, ����� � ������ �������� ����
������������ ����� ������ ��������� ��������� void Update(int). **/
    private:
        T* pbar;                // ��������� �� ������ ��������-���
        atomic<int> counter{};  // ������� ��������; ���������������� �����
        int maxcounter;         // ������������ ����� ��������
        int stepcount;          // ��� ��������

    public:
        clsProgress_shell(T* _pbar=nullptr, const int _maxcounter=100, const int _stepcount=1):
            maxcounter(_maxcounter),
            stepcount(_stepcount) {
            pbar = _pbar;
        };  // Ctor

        void Set_shell(T* _pbar, const int _maxcounter=100, const int _stepcount=1) {
        /** ����� ������������� ��������� �� ����� ������ ��������-��� � �������������� ���� ������ **/
            pbar = _pbar;
            maxcounter = _maxcounter;
            stepcount = _stepcount;
        }   // Set_shell

        void Counter_inc() {
        /** ����� ����������� �������� �������� �� �������� ��������. ����� ������������
        ��� ������������� � ���� ���������� � ����� ��� ������������� ����������. **/
            counter.fetch_add(stepcount, memory_order_relaxed);
        }   // Counter_inc

        void Progress_indicate() {
        /** ����� ������ ���������� ��������-���� � �������� ����� ��� ������������� ����������. **/
            if(!pbar) return;   // �������� �� ������������� ������� ��������-���
            int oldcount{};     // ��������������� ���������� ��� ��������
            int newcount;       // ��������������� ���������� ��� ��������
            pbar->Update(0);    // ����� ���������� �������� ��������
            while(true) {       // ���� ��� �������� ���������� �� �������
                newcount = counter.load(memory_order_relaxed);  // ��������� �������� ��������
                if (newcount != oldcount) {                     // ���� ������� �������, ��������� ��� ��
                    if(newcount >= maxcounter) break;           // ������� �������; ���� ����������, �� ����� �� �����
                    pbar->Update(newcount);                     // ������� ��������� � ������� ���������� ��������
                    oldcount = newcount;                        // ����������� ����� ���������� ��������� ��������
                };
            }
            pbar->Update(maxcounter);                           // ������� ������������ �������� ���������� ���������
        }   // Progress_indicate

        void Counter_reset() {
        /** ����� ���������� ������� counter � ����. **/
            counter.store(0, memory_order_relaxed);
        }   // Reset

        void Counter_max() {
        /** ����� ������������� ������� counter � maxcounter ��� ��������� ������ **/
            counter.store(maxcounter, memory_order_relaxed);
        }   // Counter_max

        void Update(int val) {
        /** ����� ������ ���������� ��� ������������ ����������� **/
            if(!pbar) return;
            pbar->Update(val);
        }

};  // clsProgress_shell

#endif // FROMA2_PROGRESS_MODULE_HPP
