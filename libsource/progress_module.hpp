#ifndef FROMA2_PROGRESS_MODULE_HPP
#define FROMA2_PROGRESS_MODULE_HPP

/**     Этот файл является частью библиотеки программного обеспечения для экономического
        моделирования, финансового анализа и планирования операционной деятельности предприятия
        "Free Operation Manager 2" (библиотека FROMA2).
**/

/****************************************************************************************************/
/****************************************************************************************************/
/*** 																						      ***/
/***   Copyright © 2025 Пидкасистый Александр Павлович										      ***/
/*** 																						      ***/
/***   Данная лицензия разрешает лицам, получившим копию данного программного обеспечения и	      ***/
/***   сопутствующей документации (далее — Программное обеспечение), безвозмездно использовать    ***/
/***   Программное обеспечение без ограничений, включая неограниченное право на использование,    ***/
/***   копирование, изменение, слияние, публикацию, распространение, сублицензирование и/или      ***/
/***   продажу копий Программного обеспечения, а также лицам, которым предоставляется данное      ***/
/***   Программное обеспечение, при соблюдении следующих условий:								  ***/
/*** 																					      	  ***/
/***   Указанное выше уведомление об авторском праве и данные условия должны быть включены во	  ***/
/***   все копии или значимые части данного Программного обеспечения.							  ***/
/*** 																						      ***/
/***   ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ «КАК ЕСТЬ», БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, ЯВНО   ***/
/***   ВЫРАЖЕННЫХ ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ ГАРАНТИИ ТОВАРНОЙ ПРИГОДНОСТИ, СООТВЕТСТВИЯ ПО	  ***/
/***   ЕГО КОНКРЕТНОМУ НАЗНАЧЕНИЮ И ОТСУТСТВИЯ НАРУШЕНИЙ, НО НЕ ОГРАНИЧИВАЯСЬ ИМИ. НИ В КАКОМ	  ***/
/***   СЛУЧАЕ АВТОРЫ ИЛИ ПРАВООБЛАДАТЕЛИ НЕ НЕСУТ ОТВЕТСТВЕННОСТИ ПО КАКИМ-ЛИБО ИСКАМ, ЗА УЩЕРБ   ***/
/***   ИЛИ ПО ИНЫМ ТРЕБОВАНИЯМ, В ТОМ ЧИСЛЕ, ПРИ ДЕЙСТВИИ КОНТРАКТА, ДЕЛИКТЕ ИЛИ ИНОЙ СИТУАЦИИ,   ***/
/***   ВОЗНИКШИМ ИЗ-ЗА ИСПОЛЬЗОВАНИЯ ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ ИЛИ ИНЫХ ДЕЙСТВИЙ С ПРОГРАММНЫМ     ***/
/***   ОБЕСПЕЧЕНИЕМ.																			  ***/
/*** 																						      ***/
/****************************************************************************************************/
/****************************************************************************************************/

#include <ostream>
#include <string>
#include <iomanip>
#include <atomic>

using namespace std;

class clsprogress_bar {
/** Простой погресс бар для консоли. Сделан по мотивам ответа Toby Speight в
https://codereview.stackexchange.com/questions/186535/progress-bar-in-c **/

    private:
        static const int overhead = sizeof"[100%]"; // Размер поля с процентами
        ostream &os;                                // Выходной поток для вывода информации
        const int bar_width;                        // Ширина индикатора
        string message;                             // Сообщение во время вывода индикатора
        const string full_bar;                      // Строка, выбранная часть которой выводится как индикатор
        int maxcount;                               // Максимальное число операций, соответствующее 100% выполнению

        void write(double fraction) {
        /** Метод вывода прогресс-бара **/
            const double dZero = 0.0;
            const double dOne  = 1.0;
            if(fraction < dZero) fraction = dZero;                              // Валидация параметра на вхождение
            else if(fraction > dOne) fraction = dOne;                           // в интервал [0, 1]
            int width = bar_width - message.size();                             // Ширина индикатора без сообщения
            int offset = bar_width - static_cast<unsigned>(width * fraction);   // Расчет смещения в массиве символов,
                                                                                // соответствующее текущему fraction
            os << '\r' << message;                          // Вывод сообщения
            os.write(full_bar.data() + offset, width);      // Вывод части строки, соответствующей текущему fraction
            os << "[" << std::setw(3) << static_cast<int>(100.0 * fraction) << "%]" << std::flush;  // Вывод окончания
        }   // clsprogress_bar::write

    public:
        clsprogress_bar(ostream &_os, int const line_width, string _message, const char symbol='.', const int _mx=100):
            os(_os),
            bar_width(line_width-overhead),                                     // Расчет ширины индикатора
            message(std::move(_message)),
            full_bar(string(bar_width, symbol) + string(bar_width, ' ')),       // Создание строки для выбора части
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
class clsProgress_shell {
/** Оболочка для любого класса прогресс-бара типа T. Важно, чтобы в классе прогресс бара
присутсвовал метод вывода индикации прогресса void Update(int). **/
    private:
        T* pbar;                // Указатель на объект Прогресс-бар
        atomic<int> counter{};  // Счетчик операций; инициализируется нулем
        int maxcounter;         // Максимальное число итераций
        int stepcount;          // Шаг итерации

    public:
        clsProgress_shell(T* _pbar=nullptr, const int _maxcounter=100, const int _stepcount=1):
            maxcounter(_maxcounter),
            stepcount(_stepcount) {
            pbar = _pbar;
        };  // Ctor

        void Set_shell(T* _pbar, const int _maxcounter=100, const int _stepcount=1) {
        /** Метод устанавливает указатель на новый объект Прогресс-бар и инициализирует поля заново **/
            pbar = _pbar;
            maxcounter = _maxcounter;
            stepcount = _stepcount;
        }   // Set_shell

        void Counter_inc() {
        /** Метод увеличивает значение счетчика на заданную величину. Метод предназначен
        для имплементации в цикл вычислений в поток при многопоточном вычислении. **/
            counter.fetch_add(stepcount, memory_order_relaxed);
        }   // Counter_inc

        void Progress_indicate() {
        /** Метод вывода индикатора прогресс-бара в основной поток при многопоточном вычислении. **/
            if(!pbar) return;   // Проверка на существование объекта Прогресс-бар
            int oldcount{};     // Вспомогательная переменная для счётчика
            int newcount;       // Вспомогательная переменная для счётчика
            pbar->Update(0);    // Вывод начального значения счётчика
            while(true) {       // Цикл для ожидания информации от потоков
                newcount = counter.load(memory_order_relaxed);  // Загружаем значение счётчика
                if (newcount != oldcount) {                     // Если счетчик изменен, проверяем его на
                    if(newcount >= maxcounter) break;           // верхнюю границу; если достигнута, то выход из цикла
                    pbar->Update(newcount);                     // Выводим индикатор с текущим состоянием счетчика
                    oldcount = newcount;                        // Перегружаем ранее выведенное состояние счётчика
                };
            }
            pbar->Update(maxcounter);                           // Выводим максимальное значение индикатора прогресса
        }   // Progress_indicate

        void Counter_reset() {
        /** Метод сбрасывает счетчик counter в ноль. **/
            counter.store(0, memory_order_relaxed);
        }   // Reset

        void Counter_max() {
        /** Метод устанавливает счетчик counter в maxcounter для окончания работы **/
            counter.store(maxcounter, memory_order_relaxed);
        }   // Counter_max

        void Update(int val) {
        /** Метод вывода индикатора при однопоточных вычислениях **/
            if(!pbar) return;
            pbar->Update(val);
        }

};  // clsProgress_shell

#endif // FROMA2_PROGRESS_MODULE_HPP
