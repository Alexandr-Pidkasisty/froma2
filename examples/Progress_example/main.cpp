#include <iostream>
#include <thread>
#include <progress_module.hpp>

using namespace std;

void threadfunc(clsProgress_shell<clsprogress_bar> &obj) {
/** Тестовая функция для потока. Имеет в своем составе метод для изменения счетчика
прогресс-бара. **/
    for(int i{}; i<150; i++) {
        obj.Counter_inc();  // Увеличиваем счетчик для индикатора прогресс-бара
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}   // threadfunc

void testfunc(clsProgress_shell<clsprogress_bar> &obj, const int val) {
    const int num = val;
    for(int i{}; i<num; i++) {
        obj.Counter_inc();  // Увеличиваем счетчик для индикатора прогресс-бара
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}   // testfunc

int main() {
    setlocale(LC_ALL, "Russian");                       // Установка русского языка для вывода

    cout << "*** Пример работы индиакатора в однопоточном режиме без использования оболочки ***" << endl;
    const int maxc = 500;
    clsprogress_bar* progress = new clsprogress_bar(std::clog, 70u, "Working");
    progress->Set_maxcount(maxc);
    for(int i{}; i<maxc; i+=20) {
        progress->Update(i);            // Выводим прогресс-бар
        // Выполняется какая-то работа
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };
    progress->Update(maxc);
    delete progress;        // При удалении возникает цифра "100%" прогресс
    cout << endl;

    cout << "*** Команды на индикатор идут из разных потоков. Используется оболочка ***" << endl;
    const int max_cnt = 300;
    clsprogress_bar* progress1 = new clsprogress_bar (std::clog, 70u, "Working", '=', max_cnt);
    clsProgress_shell<clsprogress_bar> shell(progress1, max_cnt, 1);
    thread t1(threadfunc, std::ref(shell));     // Первый поток
    thread t2(threadfunc, std::ref(shell));     // Второй поток
    shell.Progress_indicate();   // Выводим прогресс-бар в основном потоке main
    t1.join();
    t2.join();
    cout << endl;

    shell.Counter_reset();
    thread t5(testfunc, std::ref(shell), 50);   // Первый поток
    thread t6(testfunc, std::ref(shell), 150);  // Второй поток
    thread t7(testfunc, std::ref(shell), 100);  // Третий поток
    shell.Progress_indicate();   // Выводим прогресс-бар в основном потоке main
    delete progress1;       // При удалении возникает цифра 100% прогресс
    t5.join();
    t6.join();
    t7.join();
    cout << endl;

    cout << "*** Используется оболочка без Прогресс-бара, но потоки работают ***" << endl;
    clsProgress_shell<clsprogress_bar> shell1(nullptr);
    thread t3(threadfunc, std::ref(shell1));     // Первый поток
    thread t4(threadfunc, std::ref(shell1));     // Второй поток
    shell1.Progress_indicate();   // Выводим прогресс-бар в основном потоке main
    t3.join();
    t4.join();
    cout << "*** Окончание работы потоков ***" << endl;
    return 0;
}
