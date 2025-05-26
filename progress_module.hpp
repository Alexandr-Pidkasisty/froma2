#ifndef PROGRESS_MODULE_HPP
#define PROGRESS_MODULE_HPP

#include <ostream>
#include <string>
#include <iomanip>
#include <atomic>
#include <thread>

using namespace std;
memory_order sync = memory_order_relaxed;   // Вид синхронизации (https://cplusplus.com/reference/atomic/atomic/store)

class clsprogress_bar {
/** Простой погресс бар для консоли. Сделан по мотивам
https://codereview.stackexchange.com/questions/186535/progress-bar-in-c **/

    private:
        static const int overhead = sizeof"[100%}"; // Размер поля с процентами
        ostream &os;                                // Выходной поток для вывода информации
        const int bar_width;                        // Ширина индикатора
        string message;                             // Сообщение
        const string full_bar;                      // Индикатор
        int maxcount;                               // Максимальное число итераций

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

        clsprogress_bar(const clsprogress_bar&) = delete;               // Запрет Copy Ctor
        clsprogress_bar& operator=(const clsprogress_bar&) = delete;    // Запрет Copy =

        ~clsprogress_bar() {
            os << '\n';
        }   // Dtor

        void write(double fraction) {
        /** Метод вывода прогресс-бара **/
            if(fraction<0) fraction = 0;        // Валидация параметра на вхождение
            else if(fraction>1) fraction = 1;   // в интервал [0, 1]
            int width = bar_width - message.size();
            int offset = bar_width - static_cast<unsigned>(width * fraction);
            os << '\r' << message;
            os.write(full_bar.data() + offset, width);
            os << "[" << std::setw(3) << static_cast<int>(100 * fraction) << "%]" << std::flush;
        }   // clsprogress_bar::write

        void Update(int value) {
        /** Метод вывода прогресс-бара **/
            write(value/static_cast<double>(maxcount));
        }   // clsprogress_bar::write

        void Set_maxcount(const int _mx) {
        /** Устанавливает новое значение максимального числа итераций **/
            maxcount = _mx;
        }   // Set_maxcount

};  // clsprogress_bar

template <typename T>
class clsprogress_shell {
/** Оболочка для любого класса прогресс-бара типа T. Важно, чтобы в классе прогресс бара
присутсвовал метод вывода индикации прогресса void Update(int). **/
    private:
        T* pbar;                // Указатель на объект Прогресс-бар
        atomic<int> counter{0}; // Счетчик операций; инициализируется нулем
        const int maxcounter;   // Максимальное число итераций
        const int stepcount;    // Шаг итерации

    public:
        clsprogress_shell(T* _pbar=nullptr, const int _maxcounter=100, const int _stepcount=1):
            maxcounter(_maxcounter),
            stepcount(_stepcount) {
            pbar = _pbar;
        };  // Ctor

        void Counter_inc() {
        /** Метод увеличивает значение счетчика на заданную величину. Метод предназначен
        для имплементации в цикл вычислений. **/
            counter.fetch_add(stepcount, sync);
        }   // Counter_inc

        void Progress_indicate() {
        /** Метод вывода индикатора прогресс-бара в основной поток. **/
            if(!pbar) return;
            int oldcount{0};
            int newcount;
            pbar->Update(0);
            while(true) {
                newcount = counter.load(sync);
                if (newcount != oldcount) { // Если счетчик изменен
                    if (newcount >= maxcounter) break;
                    pbar->Update(newcount); // Выводим индикатор
                    oldcount = newcount;
                };
            }
            pbar->Update(maxcounter);
        }   // Progress_indicate

        void Counter_reset() {
        /** Сбрасывает счетчик counter в ноль. **/
            counter.store(0, sync);
        }   // Reset

};  // clsprogress_shell

#endif // PROGRESS_MODULE_HPP
