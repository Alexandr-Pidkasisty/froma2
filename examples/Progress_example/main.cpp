#include <iostream>
#include <thread>
#include <progress_module.hpp>

using namespace std;

void threadfunc(clsProgress_shell<clsprogress_bar> &obj) {
/** �������� ������� ��� ������. ����� � ����� ������� ����� ��� ��������� ��������
��������-����. **/
    for(int i{}; i<150; i++) {
        obj.Counter_inc();  // ����������� ������� ��� ���������� ��������-����
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}   // threadfunc

void testfunc(clsProgress_shell<clsprogress_bar> &obj, const int val) {
    const int num = val;
    for(int i{}; i<num; i++) {
        obj.Counter_inc();  // ����������� ������� ��� ���������� ��������-����
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}   // testfunc

int main() {
    setlocale(LC_ALL, "Russian");                       // ��������� �������� ����� ��� ������

    cout << "*** ������ ������ ����������� � ������������ ������ ��� ������������� �������� ***" << endl;
    const int maxc = 500;
    clsprogress_bar* progress = new clsprogress_bar(std::clog, 70u, "Working");
    progress->Set_maxcount(maxc);
    for(int i{}; i<maxc; i+=20) {
        progress->Update(i);            // ������� ��������-���
        // ����������� �����-�� ������
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };
    progress->Update(maxc);
    delete progress;        // ��� �������� ��������� ����� "100%" ��������
    cout << endl;

    cout << "*** ������� �� ��������� ���� �� ������ �������. ������������ �������� ***" << endl;
    const int max_cnt = 300;
    clsprogress_bar* progress1 = new clsprogress_bar (std::clog, 70u, "Working", '=', max_cnt);
    clsProgress_shell<clsprogress_bar> shell(progress1, max_cnt, 1);
    thread t1(threadfunc, std::ref(shell));     // ������ �����
    thread t2(threadfunc, std::ref(shell));     // ������ �����
    shell.Progress_indicate();   // ������� ��������-��� � �������� ������ main
    t1.join();
    t2.join();
    cout << endl;

    shell.Counter_reset();
    thread t5(testfunc, std::ref(shell), 50);   // ������ �����
    thread t6(testfunc, std::ref(shell), 150);  // ������ �����
    thread t7(testfunc, std::ref(shell), 100);  // ������ �����
    shell.Progress_indicate();   // ������� ��������-��� � �������� ������ main
    delete progress1;       // ��� �������� ��������� ����� 100% ��������
    t5.join();
    t6.join();
    t7.join();
    cout << endl;

    cout << "*** ������������ �������� ��� ��������-����, �� ������ �������� ***" << endl;
    clsProgress_shell<clsprogress_bar> shell1(nullptr);
    thread t3(threadfunc, std::ref(shell1));     // ������ �����
    thread t4(threadfunc, std::ref(shell1));     // ������ �����
    shell1.Progress_indicate();   // ������� ��������-��� � �������� ������ main
    t3.join();
    t4.join();
    cout << "*** ��������� ������ ������� ***" << endl;
    return 0;
}
