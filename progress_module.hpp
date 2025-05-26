#ifndef PROGRESS_MODULE_HPP
#define PROGRESS_MODULE_HPP

#include <ostream>
#include <string>
#include <iomanip>
#include <atomic>
#include <thread>

using namespace std;
memory_order sync = memory_order_relaxed;   // ��� ������������� (https://cplusplus.com/reference/atomic/atomic/store)

class clsprogress_bar {
/** ������� ������� ��� ��� �������. ������ �� �������
https://codereview.stackexchange.com/questions/186535/progress-bar-in-c **/

    private:
        static const int overhead = sizeof"[100%}"; // ������ ���� � ����������
        ostream &os;                                // �������� ����� ��� ������ ����������
        const int bar_width;                        // ������ ����������
        string message;                             // ���������
        const string full_bar;                      // ���������
        int maxcount;                               // ������������ ����� ��������

    public:
        clsprogress_bar(ostream &_os, int const line_width, string _message, const char symbol='.', const int _mx=100):
            os(_os),
            bar_width(line_width-overhead),
            message(std::move(_message)),
            full_bar(string(bar_width, symbol) + string(bar_width, ' ')),
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

        void write(double fraction) {
        /** ����� ������ ��������-���� **/
            if(fraction<0) fraction = 0;        // ��������� ��������� �� ���������
            else if(fraction>1) fraction = 1;   // � �������� [0, 1]
            int width = bar_width - message.size();
            int offset = bar_width - static_cast<unsigned>(width * fraction);
            os << '\r' << message;
            os.write(full_bar.data() + offset, width);
            os << "[" << std::setw(3) << static_cast<int>(100 * fraction) << "%]" << std::flush;
        }   // clsprogress_bar::write

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
class clsprogress_shell {
/** �������� ��� ������ ������ ��������-���� ���� T. �����, ����� � ������ �������� ����
������������ ����� ������ ��������� ��������� void Update(int). **/
    private:
        T* pbar;                // ��������� �� ������ ��������-���
        atomic<int> counter{0}; // ������� ��������; ���������������� �����
        const int maxcounter;   // ������������ ����� ��������
        const int stepcount;    // ��� ��������

    public:
        clsprogress_shell(T* _pbar=nullptr, const int _maxcounter=100, const int _stepcount=1):
            maxcounter(_maxcounter),
            stepcount(_stepcount) {
            pbar = _pbar;
        };  // Ctor

        void Counter_inc() {
        /** ����� ����������� �������� �������� �� �������� ��������. ����� ������������
        ��� ������������� � ���� ����������. **/
            counter.fetch_add(stepcount, sync);
        }   // Counter_inc

        void Progress_indicate() {
        /** ����� ������ ���������� ��������-���� � �������� �����. **/
            if(!pbar) return;
            int oldcount{0};
            int newcount;
            pbar->Update(0);
            while(true) {
                newcount = counter.load(sync);
                if (newcount != oldcount) { // ���� ������� �������
                    if (newcount >= maxcounter) break;
                    pbar->Update(newcount); // ������� ���������
                    oldcount = newcount;
                };
            }
            pbar->Update(maxcounter);
        }   // Progress_indicate

        void Counter_reset() {
        /** ���������� ������� counter � ����. **/
            counter.store(0, sync);
        }   // Reset

};  // clsprogress_shell

#endif // PROGRESS_MODULE_HPP
