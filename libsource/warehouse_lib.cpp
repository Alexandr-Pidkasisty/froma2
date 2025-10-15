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

#include "warehouse_module.h"

//#define DEBUG             // Макрос вывода отладочной информации. Раскомментировать для отладки
//#define DEBUG_THREADS     // Макрос вывода отладочной информации. Раскомментировать для отладки

std::mutex mu;  // Защита от одновременного доступа потоков к одному ресурсу

void EraseVector(vector<thread>& _pool) {
    /** Метод для очискти вектора в процессе поиска утечек памяти **/
        vector<std::thread>().swap(_pool);    // Приводим вектор в состояние по умолчанию
    }   // DebugEraseVector

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                                class clsSKU                                                         **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

    /** Склад сырья и материалов для одной номенклатурной позиции (Stock Keeping Unit - единица складского учета)**/

        decimal clsSKU::AVGcalc(const strItem P[], strItem R[], strItem S[], size_t N, bool ip) {
            /** Функция рассчитывает цены и стоимость сырья и материалов, отгружаемых со склада в каждом периоде проекта
            по принципу AVG, формирует массив остатков с учетом плановых отгрузок и возвращает дефицит сырья для отгрузок.
            Параметры: P[] - указатель на массив закупок (массив изменять запрещено, const), R[] - указатель на вспомогательный
            массив, - после всех проведенных расчетов показывает остатки от каждой партии на конец проекта, S[] - - указатель
            на массив отгрузок, у которого заполнены поля S.volume (изменяется в процессе работы функции: рассчитываются поля
            price и value); N - размерность массивов закупок, остатков и отгрузок; ip - индикатор, признак того, можно ли
            использовать закупаемые в каком-либо периоде сырье и материалы для отгрузки в этом же периоде: "true" означает,
            что закупка осуществляется в начале каждого периода и закупаемые сырье и материалы могут быть использованы в
            отгрузках того же периода, "false" означает, что закупки производятся в конце каждого периода, поэтому отгрузка
            этих сырья и материалов может быть только в следующих периодах. ВНИМАНИЕ!!! Функция не оптимизирует план отгрузок,
            если возникает дефицит сырья и материалов; в периоды где возникает дефицит, расчет цен и стоимости отгружаемых
            партий НЕ КОРРЕКТЕН.**/
            strItem Q, M, Z;                // Вспомогательные локальные переменные
            decimal deficit = dZero;        // Дефицит сырья и материалов для отгрузки со склада. Локальная переменная
            size_t L;                       // Вспомогательная локальная переменная
            for(size_t i=sZero; i<N; i++) { // В случае, если мы ранее вызывали функцию, массив Rem отличается от Pur,
                *(R+i) = *(P+i);            // и повторный вызов функции приведет к ошибке, поэтому сбрасываем Rem к состоянию Pur
                (S+i)->value = (S+i)->price = dZero;    // Также обнуляем стоимость и цену в отгрузках
            };
            for(size_t n=sOne; n<N; n++) {  // Основной цикл по всем периодам отгрузки, кроме нулевого (отгрузок нет)
                if(ip) { L = n; }           // Можно использовать сырье и материалы из того же периода, что и отгрузки
                else { L = n-sOne; };       // Сырье и материалы используются только из предыдущих текущему периоду отгрузки
                Z.value = Z.volume = dZero;
                for(size_t i=sZero; i<=L; i++) {                    // Рассчитываем среднюю цену
                    Z.volume += (R+i)->volume;
                    Z.value += (R+i)->value;
                    Z.price = (fabs(Z.volume) > epsln)? Z.value/Z.volume:dZero;
                };
                Q.volume = (S+n)->volume;                           // Приравниваем дефицит величине отгрузки
                for(size_t j=sZero; j<=L; j++) {                    // Проходим по всему массиву остатков
                    (R+j)->price = Z.price;                         // Записываем среднюю цену в остатки
                    if(Q.volume > (R+j)->volume) { M.volume = (R+j)->volume; }
                    else { M.volume = Q.volume; };
                    (R+j)->volume -= M.volume;                      // Уменьшаем остатки на величину отгрузки,
                    (R+j)->value = (R+j)->volume * (R+j)->price;    // корректируем стоимость остатков
                    (S+n)->price = (R+j)->price;                    // Записываем в отгрузки цену
                    (S+n)->value = (S+n)->volume * (S+n)->price;    // и стоимость партии
                    Q.volume -= M.volume;                           // Уменьшаем дефицит
                }; // Цикл по j
                // Если модуль Q.volume меньше epsln, то считать его равным нулю, т.к. числа decimal не бывают
                // точно равны друг другу
                Q.volume = (fabs(Q.volume) < epsln) ? dZero : Q.volume;
                deficit += Q.volume;                                // Формируем общий дефицит остатков
            };  // Цикл по n
            return deficit;
        }   // AVGcalc

        decimal clsSKU::FLFcalc(const strItem P[], strItem R[], strItem S[], size_t N, bool ip, AccountingMethod ind) {
            /** Функция рассчитывает цены и стоимость сырья и материалов, отгружаемых со склада в каждом периоде проекта.
            по принципу FIFO или LIFO, формирует массив остатков с учетом плановых отгрузок и возвращает дефицит сырья
            для отгрузок. Параметры: P[] - указатель на массив закупок (массив изменять запрещено, const), R[] - указатель на
            вспомогательный массив, - после всех проведенных расчетов показывает остатки от каждой партии на конец проекта.
            S[] - указатель на массив отгрузок, у которого заполнены поля S.volume (изменяется в процессе работы функции:
            рассчитываются поля price и value); N - размерность массивов закупок, остатков и отгрузок; ip - индикатор, признак
            того, можно ли использовать закупаемые в каком-либо периоде сырье и материалы для отгрузки в этом же периоде:
            "true" означает, что закупка осуществляется в начале каждого периода и закупаемые сырье и материалы могут быть
            использованы в отгрузках того же периода, "false" означает, что закупки производятся в конце каждого периода,
            поэтому отгрузка этих сырья и материалов может быть только в следующих периодах. Ind – флаг, устанавливающий
            принцип учета запасов: FIFO или LIFO.ВНИМАНИЕ!!! Функция не оптимизирует план отгрузок, если возникает дефицит
            сырья и материалов; в периоды где возникает дефицит, расчет цен и стоимости отгружаемых партий НЕ КОРРЕКТЕН.**/
            const int iZero = 0;
            int i, j;                    // Счетчики для перебора массива остатков
            int *k;                      // Ссылка на действующий счетчик: или i, или j
            strItem Q, M;                // Вспомогательные локальные переменные
            decimal deficit = dZero;     // Дефицит сырья и материалов для отгрузки со склада
            int L;                       // Вспомогательная переменная
            for(size_t i=sZero; i<N; i++) { // В случае, если мы ранее вызывали функцию, массив Rem отличается от Pur,
                *(R+i) = *(P+i);            // и повторный вызов функции приведет к ошибке, поэтому сбрасываем Rem к состоянию Pur
                (S+i)->value = (S+i)->price = dZero;            // Также обнуляем стоимость и цену в отгрузках
            };
            if(ind == FIFO) { k = &i; }     // В зависимости от метода учета FIFO или LIFO перебор массива закупок идет
                else { k = &j;};            // от ранних к поздним или от поздних к ранним периодам: указателю k присваивается
                                            // ссылка на соответствующий счетчик
            size_t n;
            /** Используем лямбда-выражение (аналог определения функции внутри другой функции), см
            http://прохоренок.рф/pdf/cppmingw/ch12-cpp-lyambda-vyrazheniya.html **/
            auto lambda = [&M, &R, &k, &S, &n, &Q]() {  // Захват переменных по ссылке
                M.price = (R+*k)->price;        // периоде на величину M, пересчитываем стоимость оставшегося сырья,
                M.value = M.volume * M.price;   // расчитываем стоимость отгруженного сырья, рассчитываем дефицит
                (R+*k)->volume -= M.volume;     // запасов для черпания из следующих периодов и переходим к следующей
                (R+*k)->value = (R+*k)->volume * (R+*k)->price; // итерации цикла
                (S+n)->value += M.value;
                Q.volume -= M.volume;
            };
            for(n=sOne; n<N; n++) {  // Основной цикл по всем периодам отгрузки, кроме нулевого (отгрузок нет)
                if(ip) { L = n; }           // Можно использовать сырье и материалы из того же периода, что и отгрузки
                else { L = n-sOne; };       // Сырье и материалы используются только из предыдущих текущему периоду отгрузки
                Q.volume = (S+n)->volume;   // Приравниваем дефицит величине отгрузки
                for(i=iZero, j=L; (i<=L)&&(j>=iZero); i++, j--) // Проходим по всему массиву закупок: по i от нуля до L-1 или
                /**  ВНИМАНИЕ ПОТЕНЦИАЛЬНАЯ ОШИБКА!!! в типе size_t числом, следующим за нулем при декременте j будет большое
                ПОЛОЖИТЕЛЬНОЕ ЧИСЛО!!! Цикл может стать бесконечным!!! **/
                {                                           // по j от L-1 до нуля включительно: в зависимости от указателя k
//                    if(is_bigger(Q.volume, (R+*k)->volume)) // Если требуемая отгрузка превышает остатки в k-ом периоде, то
                    if(Q.volume > (R+*k)->volume)           // Если требуемая отгрузка превышает остатки в k-ом периоде, то
                    {                                       // вспомогательная переменная M.volume принимает значение всего
                        M.volume = (R+*k)->volume;          // объема остатков k-го периода; уменьшаем запас сырья в текущем
                        // периоде на величину M, пересчитываем стоимость оставшегося сырья, расчитываем стоимость отгруженного
                        // сырья, рассчитываем дефицит запасов для черпания из следующих периодов и переходим к следующей
                        // итерации цикла
                        lambda();                           // Исподьзую лямбда выражение вместо повторяющихся строчек кода
                    }
                    else                                    // Если же требуемая отгрузка не превышает остатки в k-ом периоде, то
                    {                                       // уменьшаем запас сырья в текущем периоде на величину M, пересчитываем
                        M.volume = Q.volume;                // стоимость оставшегося сырья, стоимость отгруженного сырья,
                        // рассчитываем дефицит запасов для черпания из следующих периодов и выходим из цикла дострочно
                        lambda();
                        break;      // Исподьзую лямбда выражение вместо повторяющихся строчек кода
                    };
                };  // Цикл по i и j
                // Если модуль Q.volume меньше epsln, то считать его равным нулю, т.к. числа decimal не бывают
                // точно равны друг другу
                Q.volume = fabs(Q.volume) < epsln ? dZero : Q.volume;
                deficit += Q.volume;
                (S+n)->price = fabs((S+n)->volume)>epsln?(S+n)->value/(S+n)->volume:dZero;  // Рассчитываем взвешенную
            } // Основной цикл по n                                                         // цену отгруженного сырья в n-ом периоде
            return deficit;
        }   // FLFcalc

        decimal clsSKU::PURcalc(strItem P[], const strItem S[], size_t N, bool ip, decimal Shr) {
        /** Функция рассчитывает объемы закупок, необходимые для выполнения плана отгрузок и обеспечения заданного
        остатка сырья и материалов (остаток задается как доля от объема отгрузок за период) и возвращает дефицит сырья
        в предплановом (нулевом) периоде. Рассчитываются закупки в плановых периодах - с первого до последнего (N-1).
        Закупки в нулевой период задаются вручную и характеризуют остатки сырья и материалов к началу проекта (элемент
        стартового баланса). Параметры: P[] - указатель на массив закупок (изменяется в процессе работы функции), S[] -
        указатель на массив отгрузок, у которого заполнены поля S.volume (массив изменять запрещено, const); N - размерность
        массивов закупок и отгрузок; ip - признак того, можно ли использовать закупаемые в каком-либо i-м периоде сырье и
        материалы для отгрузки в этом же периоде: True - можно, False - нельзя, для отгрузки можно использовать только
        партии из предыдущих периодов; Shr - плановый остаток сырья, заданный, как доля отгрузок за период (число от 0) **/
            if((!P) || (!S) || N==sZero) return dZero;    // Если нет массивов или их размерность нулевая, то выход с нулем
            if(!ip) // Проверяем дефицит сырья в нулевом периоде; если дефицит есть, то выход из функции и возврат величины
                if((S+sOne)->volume > (P+sZero)->volume) return ((S+sOne)->volume - (P+sZero)->volume);   // этого дефицита
            decimal Z[N], Q[N], V[N];
            size_t i, k;
            *Z = dZero;                                         // Массив отгрузок нарастающим итогом.
            *V = *Q = P->volume;
            for(i=sOne; i<N; i++) {
                *(Z+i) = *(Z+i-sOne) + (S+i)->volume;           // Рассчет плановых закупок нарастающим итогом
                *(Q+i) = (S+i)->volume * Shr;                   // Рассчитываем требуемый запас
                if((*(Z+i)+*(Q+i)) < *(V+i-sOne)) { *(V+i)= *(V+i-sOne); } // Если объем плановых закупок нарастающим итогом
                else { *(V+i)= *(Z+i)+*(Q+i); };                // меньше, чем в предыдущем периоде, приравниваем его к предыдущему объему
                if(ip) { k = sZero; }                           // Закупки из того же периода, что и отгрузки
                else { k = sOne; (P+N-sOne)->volume = dZero; }; // Закупки в периоде, предшествующего периоду отгрузок
                if((i-k) != sZero) {                            // Записываем новые значения объемов закупок
                    (P+i-k)->volume = *(V+i) - *(V+i-sOne);
                };
                (P+i-k)->value = (P+i-k)->volume * (P+i-k)->price;  // Рассчитываем стоимость
            }; // Цикл по i
            return dZero;
        }   // PURcalc

        void clsSKU::BALcalc(const strItem P[], strItem B[], const strItem S[], size_t N) {
        /** Функция формирует массив балансовых остатков сырья на складе. Функция может быть вызвана только после
        корректного расчета объемов закупок (функцией PURcalc) и цен отгрузок (функции FLFcalc или AVGcalc).
        Параметры: P[] - указатель на массив корректно рассчитанных закупок (неизменен), B[] - указатель на массив
        балансовых остатков (изменяется в процессе работы функции), S[] - указатель на массив корректно рассчитанных
        отгрузок (неизменен), N - размерность массивов закупок и отгрузок **/
            strItem Z[N], Q[N];      // Вспомогательные массивы
            *B = *P;                 // Баласн сырья в нулевом периоде совпадает с закупками
            *Q = *P;                 // Присваиваем начальные значения для накопительного итога
            Z->volume = Z->value = dZero;
            for(size_t i=sOne; i<N; i++) {
                (Q+i)->volume = (Q+i-sOne)->volume + (P+i)->volume;      // Закупки накопительным итогом
                (Q+i)->value = (Q+i-sOne)->value + (P+i)->value;         // Закупки накопительным итогом
                (Z+i)->volume = (Z+i-sOne)->volume + (S+i)->volume;      // Отгрузки накопительным итогом
                (Z+i)->value = (Z+i-sOne)->value + (S+i)->value;         // Отгрузки накопительным итогом

                (B+i)->volume = (Q+i)->volume - (Z+i)->volume;           // Балансовый статок на складе для каждого периода
                (B+i)->value = (Q+i)->value - (Z+i)->value;
                (B+i)->price = fabs((B+i)->volume)>epsln?(B+i)->value / (B+i)->volume:dZero;
            };
        } // BALcalc

        clsSKU::clsSKU() {
        /** Пустой конструктор. **/
            PrCount = sZero;
            name = measure = EmpStr;
            Pur = Rem = Bal = Ship = nullptr;
            lack = dZero;
            indr = false;
            acct = FIFO;
            pcalc = calc;
            share = dZero;
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsSKU Empty Ctor" << endl;
            #endif
        } // clsSKU Empty Ctor

        clsSKU::clsSKU(const size_t _PrCount, const string &_name, const string &_measure, AccountingMethod _ac, bool _ind,\
            PurchaseCalc _flag, const decimal& _share, const strItem _Ship[]) {
        /** Конструктор с вводом объемов отгрузок. Параметры: _PrCount - количество периодов проекта, _name - наименование
        сырья, _measure - единица измерения сырья, _ac - метод учета запасов, _ind - разрешение на отгрузки и закупки в одном
        периоде, _flag - флаг ручного/ автоматического расчета закупок сырья, _share - Запас сырья и материалов на складе в
        каждый период, выраженный в доле от объема отгрузок за этот период, _Ship - массив с объемом отгрузок (заполнены
        только поля volume) **/
            PrCount = _PrCount;
            name = _name;
            measure = _measure;
            acct = _ac;
            indr = _ind;
            pcalc = _flag;
            if(_share < dZero) share = dZero;
            else share = _share;
            lack = dZero;
            if(PrCount>sZero) {
                Ship = new(nothrow) strItem[PrCount]{};
                if(_Ship) { var_cpy(Ship, _Ship, PrCount); };
                Pur = new(nothrow) strItem[PrCount]{};
                Rem = new(nothrow) strItem[PrCount]{};
                Bal = new(nothrow) strItem[PrCount]{};
            } else
                Ship = Pur = Rem = Bal = nullptr;
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsSKU Ctor with parametrs, include strItem _Ship" << endl;
            #endif
        }   // Ctor clsSKU

        clsSKU::clsSKU(const size_t _PrCount, const string &_name, const string &_measure, AccountingMethod _ac, bool _ind,\
            PurchaseCalc _flag, const decimal& _share) {
        /** Конструктор с параметрами. Аналогичен предыдущему, но без ввода данных об отгрузках. Используется в методе
            clsStorage::RfF для десериализации. **/
            PrCount = _PrCount;
            name = _name;
            measure = _measure;
            acct = _ac;
            indr = _ind;
            pcalc = _flag;
            if(_share < dZero) share = dZero;
            else share = _share;
            lack = dZero;
            if(PrCount>sZero) {
                Ship = new(nothrow) strItem[PrCount]{};
                Pur = new(nothrow) strItem[PrCount]{};
                Rem = new(nothrow) strItem[PrCount]{};
                Bal = new(nothrow) strItem[PrCount]{};
            } else
                Ship = Pur = Rem = Bal = nullptr;
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsSKU Ctor with parametrs" << endl;
            #endif
        }   // CTOR clsSKU

        clsSKU::clsSKU(const clsSKU &obj) {
        /** Поскольку в классе присутствует динамическое выделение памяти для переменных, необходимо явно создать
         конструктор копирования. Конструктор копирования необходим для того, чтобы мы могли создавать “реальные”
         (а не побитовые) копии для объектов класса. Такая копия объекта может понадобиться в следующих случаях:
         при передаче объекта класса в функцию, как параметра по значению (а не по ссылке); при возвращении из
         функции объекта класса, как результата её работы; при инициализации  одного объекта класса другим объектом
         этого класса.**/
            PrCount = obj.PrCount;
            name = obj.name;
            measure = obj.measure;
            acct = obj.acct;
            indr = obj.indr;
            pcalc = obj.pcalc;
            share = obj.share;
            lack = obj.lack;
            if(obj.Pur) { Pur = new(nothrow) strItem[PrCount]; } else Pur = nullptr;
            if(Pur) { var_cpy(Pur, obj.Pur, PrCount); };
            if(obj.Rem) { Rem = new(nothrow) strItem[PrCount]; } else Rem = nullptr;
            if(Rem) { var_cpy(Rem, obj.Rem, PrCount); };
            if(obj.Bal) { Bal = new(nothrow) strItem[PrCount]; } else Bal = nullptr;
            if(Bal) { var_cpy(Bal, obj.Bal, PrCount); };
            if(obj.Ship) { Ship = new(nothrow) strItem[PrCount]; } else Ship = nullptr;
            if(Ship) { var_cpy(Ship, obj.Ship, PrCount); };
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsSKU Copy Ctor" << endl;
            #endif
        }   // CopyCTOR clsSKU

        clsSKU::clsSKU(clsSKU&& obj) {
        /** Конструктор перемещения **/
            PrCount = sZero;
            name = measure = EmpStr;
            Pur = Rem = Bal = Ship = nullptr;
            lack = dZero;
            indr = false;
            acct = FIFO;
            pcalc = calc;
            share = dZero;
            swap(obj);
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsSKU Move Ctor" << endl;
            #endif
        }   // Move Ctor clsSKU

        clsSKU& clsSKU::operator=(const clsSKU &obj) {
        /** Перегрузка оператора присваивания копированием. Реализовано в идеоме
        КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsSKU tmp(obj);                    // Вызываем конструктор копирования и получаем копию obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsSKU Copy operator=" << endl;
            #endif
            return *this;
        }   // Copy operator=

        clsSKU& clsSKU::operator=(clsSKU &&obj) {
        /** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
        (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsSKU tmp(move(obj));              // Вызываем конструктор перемещения и получаем содержание obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsSKU Move operator=" << endl;
            #endif
            return *this;
        }   // clsSKU Move operator=

        clsSKU::~clsSKU() {
            if(PrCount > sZero) {
                if(Pur) {  delete[] Pur;  Pur = nullptr; }; // Удаляем из памяти непустой массив закупок
                if(Rem) {  delete[] Rem;  Rem = nullptr; }; // Удаляем из памяти непустой массив остатков на конец проекта
                if(Bal) {  delete[] Bal;  Bal = nullptr; }; // Удаляем из памяти непустой массив балансов остатков по периодам
                if(Ship) { delete[] Ship; Ship = nullptr;}; // Удаляем из памяти непустой массив отгрузок
            };
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsSKU Dtor" << endl;
            #endif
        }   // ~clsSKU

        bool clsSKU::operator == (const  string& Rightname) const {
        /** Переопределение оператора сравнения для поиска экземпляра объекта по имени**/
            return (name == Rightname);
        }   // operator ==

        void clsSKU::swap(clsSKU& obj) noexcept {
        /** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
            std::swap(PrCount, obj.PrCount);
            std::swap(name, obj.name);
            std::swap(measure, obj.measure);
            std::swap(Pur, obj.Pur);
            std::swap(Rem, obj.Rem);
            std::swap(Bal, obj.Bal);
            std::swap(Ship, obj.Ship);
            std::swap(lack, obj.lack);
            std::swap(indr, obj.indr);
            std::swap(acct, obj.acct);
            std::swap(pcalc, obj.pcalc);
            std::swap(share, obj.share);
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsSKU swap" << endl;
            #endif
        }   // swap

        /** Get-методы **/

        const size_t& clsSKU::Count() const { return PrCount; }     // Возвращает const-ссылку на количество периодов
        const string& clsSKU::Name() const { return name; }         // Возвращает const-ссылку на название номенклатурной единицы складского учета
        const string& clsSKU::Measure() const { return measure; }   // Возвращает const-ссылку на единицу натурального измерения
        string clsSKU::AccMethod() const {                          // Возвращает принцип учета запасов в
            if(acct==FIFO) return "FIFO";                           // виде текстовой строки
            else if (acct==LIFO) return "LIFO";
                 else return "AVERAGE";
        } // AccMethod
        string clsSKU::Permission() const {         // Возвращает флаг разрешения/запрещения отгрузок в одном периоде
            return ProhibitedTXT[static_cast<size_t>(indr)];
        }   // Permission
        string clsSKU::AutoPurchase() const {       // Возвращает флаг авторасчета/ ручного расчета закупок
            return PurchaseCalcTXT[pcalc];
        }   // AutoPurchase
        const PurchaseCalc& clsSKU::GetAutoPurchase() const { return pcalc; }
        /** Возвращает const-ссылку на флаг авторасчета/ ручного расчета закупок **/

        const decimal& clsSKU::Share() const { return share; }
        /** Возвращает const-ссылку на норматив запаса сырья в долях от объема отгрузки **/

        const strItem* clsSKU::GetDataItem(const ChoiseData _cd, size_t _N) const {
        /** Функция возвращает данные элемента массива с индексом _N в виде const-указателя на структуру типа strItem;
        выбор массива производится значением переменной _cd (purchase - закупки, balance - остатки, shipment - отгрузки). **/
            if( _N < PrCount ) {   // Если индекс в разрешенных границах будем присваивать значения
                strItem* p;
                if( (_cd == purchase) || (_cd == balance) ) {
                    if(_cd == purchase) { p = Pur; }    // Выбираем массив для ввода данных: "purchase" - массив закупок
                    else { p = Bal; };             // "shipment" - массив отгрузок
                }
                else { p = Ship; }
                return (p+_N);
            }
            else
                return nullptr;
        }   // GetDataItem

        const strItem* clsSKU::GetShipment() const {
        /** Метод возвращает константный указатель на массив Ship **/
            return Ship;
        }   // GetShipment

        const strItem* clsSKU::GetPurchase() const {
        /** Метод возвращает константный указатель на массив Pur **/
            return Pur;
        }   // GetPurchase

        const strItem* clsSKU::GetBalance() const {
        /** Метод возвращает константный указатель на массив Bal **/
            return Bal;
        }   // GetBalance

        const decimal& clsSKU::GetLack() const {
        /** Метод возвращает дефицит сырья и материалов для отгрузки со склада **/
            return lack;
        }   // GetLack

        /** Set-методы **/

        void clsSKU::SetName(const string& _name) { name = _name; }             // Меняет имя SKU
        void clsSKU::SetMeasure(const string& _measure) { measure = _measure; } // Меняем единицу измерения
        void clsSKU::SetPermission(bool _indr) { indr = _indr;  }               // Устанавливает флаг отгрузок и закупок в одном периоде
        void clsSKU::SetAutoPurchase(PurchaseCalc _pcalc) { pcalc = _pcalc; }   // Устанавливает флаг авторасчета закупок
        void clsSKU::SetShare(decimal _share) { share = _share; }               // Устанавливаем норматив запаса сырья на складе
        void clsSKU::SetAccount(AccountingMethod _acct) { acct = _acct; }       // Устанавливает принцип учета запасов

        bool clsSKU::SetDataItem(const ChoiseData _cd, const strItem _U, size_t _N) {
        /** Функция записывает в элемент массива с индексом _N значение volume, price и value из переменной _U;
        выбор массива производится значением переменной _cd. Возвращает True, если запись удалась **/
            if( _N < PrCount) {                     // Если индекс в разрешенных границах будем присваивать значения
                strItem* p;                         // Вспомогательный указатель
                if(_cd == purchase) { p = Pur; }    // Выбираем массив для ввода данных: "purchase" - массив закупок
                    else { p = Ship; };             // "shipment" - массив отгрузок
                *(p+_N) = _U;                       // Ввод данных в массив
                return true;
            } else return false;
        }   // SetDataItem

        bool clsSKU::SetPurchase(const strItem _unit[], size_t _count) {
        /** Функция загружает данные о закупке сырья и материалов на склад (объемы и цены). Параметры:  _unit[]
            - указатель на массив загружаемых данных, _count - размерность массива (может не совпадать с размерностью
            массива закупок PrCount) **/
            if((!_unit) || (_count==sZero)) return false;       // валидация параметров
            size_t M;
            if(_count > PrCount) M = PrCount; else M = _count;  // Если размер переданного в функцию массива больше массива
            var_cpy(Pur, _unit, M);         // закупок, то данные обрезаются, иначе используется размер полученного массива
            var_cpy(Rem, _unit, M);         // Загружаем данные в массивы Pur (закупки) и Rem (остатки)
            return true;
        }   // SetPurchase

        bool clsSKU::SetPurPrice(const strItem _unit[], size_t _count) {
        /** Функция загружает только цены на закупаемое на склад сырье. Параметры:  _unit[] - указатель на массив
        загружаемых данных, _count - размерность массива (может не совпадать с размерностью массива закупок PrCount)**/
            if((!_unit) || (_count==sZero)) return false;            // валидация параметров
            size_t M;
            if(_count > PrCount) M = PrCount; else M = _count;       // Если размер переданного в функцию массива больше массива
            for(size_t i = sZero; i<M; i++) {    // закупок, то данные обрезаются, иначе используется размер полученного массива
                 (Rem+i)->price = (Pur+i)->price = (_unit+i)->price; // Загружаем данные в массивы Pur (закупки) и Rem (остатки)
            };
            return true;
        }   // SetPurPrice

        void clsSKU::SetShipment(const decimal _unit[], size_t _count) {
        /** Функция загружает данные о плановых отгрузках сырья и материалов со склада (только объемы). Параметры:
            _unit[] - указатель на массив, содержащий объемы отгрузок, _count - размерность массива (может не совпадать
            с размерностью массива закупок PrCount). **/
            if((!_unit) || (_count==sZero)) return;             // валидация параметров
            size_t M;
            if(_count > PrCount) M = PrCount; else M = _count;
            for(size_t i = sZero; i<M; i++) {
                (Ship+i)->volume = *(_unit+i);
            };
        }   // SetShipment

        /** Методы визуального контроля **/

        void clsSKU::View() const {                   // Временная (?) функция. Выводит на экран состояние закупок, остатков, отгрузок
            cout << "Project periods quantity = " << PrCount << "; name of Raw Material = " << name << endl;
            cout << "Measure = " << Measure() << " Accounting Method is " << AccMethod() << \
                " AutoPurchase= " << AutoPurchase() <<endl;
            cout << "Share is " << Share() << " Permission= " << Permission() << endl << endl;

            for(size_t i=sZero; i<PrCount; i++) {
                cout << "Purchase[" << i << "]= " << Pur[i];
            };
            for(size_t i=sZero; i<PrCount; i++) {
                cout << "Remain[" << i << "]= " << Rem[i];
            };
            for(size_t i=sZero; i<PrCount; i++) {
                cout << "Balance[" << i << "]= " << Bal[i];
            };
            for(size_t i=sZero; i<PrCount; i++) {
                cout << "Shipment[" << i << "]= " << Ship[i];
            };
            cout << "Lack for shipment = " << lack << endl << endl;
        }   // Show

        void clsSKU::ViewData(const ChoiseData _cd) const {
        /** Метод визуального контроля состояние закупок, остатков, отгрузок; выбор массива производится значением
        переменной _cd (purchase - закупки, balance - остатки, shipment - отгрузки). **/
            strItem* p;                                         // Временная переменная -  указатель
                if( (_cd == purchase) || (_cd == balance) ) {   // Выбираем массив для ввода данных:
                    if(_cd == purchase) { p = Pur; }            // "purchase" - массив закупок
                    else { p = Bal; };                          // "balance" - массив остатков
                } else { p = Ship; }                            // "shipment" - массив отгрузок
            nmBPTypes::strNameMeas names[sOne];                 // Временный массив имен. Заполняем его
            names->name = Name();                               // Названием SKU
            names->name.resize(15);                             // Урезаем название до заданного числа символов
            names->measure = Measure();                         // Размерностью SKU
            names->measure.resize(8);                           // Урезаем размерност до заданного числа символов
            nmPrntSrvs::ArrPrint(sOne, names, p, PrCount, volume);  // Выводим информацию в поток вывода
            nmPrntSrvs::ArrPrint(sOne, names, p, PrCount, price);
            nmPrntSrvs::ArrPrint(sOne, names, p, PrCount, value);
        }   // ViewData

        /** Расчетные методы **/

        const decimal& clsSKU::CalcPrice() {
        /** Функция рассчитывает цены и стоимость отгружаемых сырья и материалов в каждом периоде на основе установленного
        пользователем принципа учета запасов: FIFO, LIFO или AVERAGE, записывает результаты расчетов в массив Ship, возвращает
        величину дефицита сырья и материалов для обеспечения плановых отгрузок. Если дефцит не равен нулю, рассчитанные
        цены и стоимость могут нести некорректные значения. **/
            if(acct == AVG) { lack = AVGcalc(Pur, Rem, Ship, PrCount, indr); }
            else { lack = FLFcalc(Pur, Rem, Ship, PrCount, indr, acct); };
//            if(is_smaller(lack, epsln)) {           // Если дефицит меньше минимального значения, то считаем его нулевым
            if(lack < epsln) {                      // Если дефицит меньше минимального значения, то считаем его нулевым
                lack=dZero;                         // Обнуляем дефицит и
                BALcalc(Pur, Bal, Ship, PrCount);   // Рассчитываем баланс остатков на складе
            };
            return lack;
        }   // CalcPrice

        const decimal& clsSKU::CalcPurchase() {
        /** Функция рассчитывает объемы закупок, необходимые для выполнения плана отгрузок и обеспечения заданного остатка
        сырья и материалов, записывает результаты расчетов в массив Pur, возвращает величину дефицита сырья и материалов для
        обеспечения плановых отгрузок в нулевом периоде.**/
            if(pcalc == nocalc) { return dZero; };  // Если расчет закупок запрещен, то выход из функции
//            decimal def;
            lack = PURcalc(Pur, Ship, PrCount, indr, share);
            if(lack < epsln) { lack = dZero;};        // Если дефицит меньше минимального значения, то считаем его нулевым
            return lack;
        }   // CalcPurchase

        bool clsSKU::Resize(size_t _n) {
        /** Функция изменяет размеры массивов типа strItem, устанавливая новое число периодов проекта, равное _n.
        При _n < PrCount, данные обрезаются, при _n > PrCount, - добавляются новые элементы массивов с нулевыми значениями.
        Функция возвращает true при удачном изменении размеров массивов, false - в противном случае **/
            if(_n < sOne) { return false; };        // Некорректный размер
            if(_n == PrCount) { return true; };     // Изменение размера не производится
            strItem *p, *r, *b, *s;
            p = new strItem[_n];
            r = new strItem[_n];
            b = new strItem[_n];
            s = new strItem[_n];
            if(_n < PrCount) {
                for(size_t i= sZero; i<_n; i++) {
                    *(p+i) = *(Pur+i);              // Побитовое копирование экземпляров допустимо, т.к. каждый элемент массива -
                    *(r+i) = *(Rem+i);              // это структура strItem, которая содержит простые базовые типы
                    *(b+i) = *(Bal+i);              // и в самой структуре strItem отсутствует динамическое
                    *(s+i) = *(Ship+i);             // выделение памяти
                }
            }
            else {
                for(size_t i= sZero; i<PrCount; i++) {
                    *(p+i) = *(Pur+i);
                    *(r+i) = *(Rem+i);
                    *(b+i) = *(Bal+i);
                    *(s+i) = *(Ship+i);
                };
                for(size_t i= PrCount; i<_n; i++) {
                    (b+i)->volume = (p+i)->volume = (r+i)->volume = (s+i)->volume = dZero;  // Обнуляем исходный массив закупок
                    (b+i)->price = (p+i)->price  = (r+i)->price  = (s+i)->price = dZero;    // p, остатков r и b и
                    (b+i)->value = (p+i)->value  = (r+i)->value  = (s+i)->value = dZero;    // отгрузок s
                }
            }
            if(PrCount > sZero) {
                delete[] Pur;
                delete[] Rem;
                delete[] Bal;
                delete[] Ship;
            }
            Pur = p;
            Rem = r;
            Bal = b;
            Ship =s;
            PrCount = _n;
            return true;
        }   // Resize

        TLack clsSKU::Calculate() {
        /** Функция рассчитывает требуемый объем закупок, если выставлен флаг pcalc разрешающий расчет. Функция также
        рассчитывает цены и стоимость отгружаемой партии в соответствии с выбранным принципом FIFO, LIFO или AVG. Функция
        рассчитывает дефицит сырья и материалов для отгрузки со склада lack. Если lack > epsln, сделанные ранее расчеты
        объемных и стоимостных покзателей могут быть некорректными. Информация возвращается в виде структуры типа TLack **/
            TLack tmp = {dZero, EmpStr};                                                // Вспомогательная переменная
            if((PrCount == sZero) || (!Pur) || (!Rem) || (!Bal) || (!Ship)) return tmp; // Валидация параметров
            lack == CalcPurchase();         // Рассчитываем автозакупки:
            if(fabs(lack) > epsln) {        // Если образовался дефицит, то
                tmp.lack = lack;            // Заполняем поля
                tmp.Name = name;            // выходной переменной
                return tmp;                 // выходим и возвращаем ее
            };
            lack == CalcPrice();            // Рассчитываем цены отгрузок
            if(fabs(lack) > epsln) {        // Если образовался дефицит, то
                tmp.lack = lack;            // Заполняем поля
                tmp.Name = name;            // выходной переменной
                return tmp;                 // выходим и возвращаем ее
            };
            lack = dZero;                   // если дефицита нет, то обнуляем lack
            return tmp;                     // и выходим с неизмененной переменнной tmp
        }   // Calculate

        /** Методы сериализациии и десериализации **/

        bool clsSKU::StF(ofstream &_outF)  {
        /** Метод имплементации записи в файловую переменную текущего экземпляра класса (запись в файл, метод сериализации).
        Параметры: &_outF - экземпляр класса ofstream для записи данных **/
            if(PrCount == sZero) return false;      // Если количество периодов нулевое, выход с false
            #ifdef DEBUG                            // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                long bpos = _outF.tellp();          // Определяем позицию в начале файла
                cout << "clsSKU::StF begin bpos= " << bpos << endl;
            #endif
            if(!SEF(_outF, PrCount)) return false;  // Сохраняем количество периодов
            if(!SEF(_outF, name)) return false;     // Сохраняем наименование SKU
            if(!SEF(_outF, measure)) return false;  // Сохраняем ед.измерения
            if(!SEF(_outF, lack)) return false;     // Сохраняем величину дефицита
            if(!SEF(_outF, indr)) return false;     // Сохраняем флаг разрешения/запрета отгрузок и закупок в одном периоде
            if(!SEF(_outF, acct)) return false;     // Сохраняем принцип учета
            if(!SEF(_outF, pcalc)) return false;    // Сохраняем флаг авторасчета/ ручного расчета закупок
            if(!SEF(_outF, share)) return false;    // Сохраняем норматив запаса сырья на складе

            for(size_t i{}; i<PrCount; i++) {       // Сохраняем все массивы в едином цикле
                if(!(Pur+i)->StF(_outF)) return false;      // Сохраняем массив закупок
                if(!(Rem+i)->StF(_outF)) return false;      // Сохраняем массив остатков из партий сырья и материалов
                if(!(Bal+i)->StF(_outF)) return false;      // Сохраняем массив балансовых остатков
                if(!(Ship+i)->StF(_outF)) return false;     // Сохраняем массив отгрузок
            };
            #ifdef DEBUG
                long epos = _outF.tellp();                    // Определяем позицию в конце файла
                cout << "clsSKU::StF end epos= " << epos << endl;
            #endif // DEBUG
            return true;                 // Возвращаем true
        }   // StF

        bool clsSKU::RfF(ifstream &_inF) {
        /** Метод имплементации чтения из файловой переменной текущего экземпляра класса (чтение из файла, метод десериализации).
        Параметры: &_inF - экземпляр класса ifstream для чтения данных **/
            if(PrCount > sZero) {        // Если количество периодов больше нуля, то массивы не пустые,
                delete[] Pur;            // их нужно удалить
                delete[] Rem;
                delete[] Bal;
                delete[] Ship;
            };
            #ifdef DEBUG                            // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                long bpos = _inF.tellg();                // Определяем позицию в начале файла
                cout << "clsSKU::RfF begin bpos= " << bpos << endl;
            #endif
            if(!DSF(_inF, PrCount)) return false;          // Читаем количество периодов
            if(!DSF(_inF, name)) return false;             // Читаем наименование SKU
            if(!DSF(_inF, measure)) return false;          // Читаем ед.измерения
            if(!DSF(_inF, lack)) return false;             // Читаем величину дефицита
            if(!DSF(_inF, indr)) return false;             // Читаем флаг разрешения/запрета отгрузок и закупок в одном периоде
            if(!DSF(_inF, acct)) return false;             // Читаем принцип учета
            if(!DSF(_inF, pcalc)) return false;            // Читаем флаг авторасчета/ ручного расчета закупок
            if(!DSF(_inF, share)) return false;            // Читаем норматив запаса сырья на складе
            Pur = new strItem[PrCount];     // Создаем новые массивы: закупок
            Rem = new strItem[PrCount];     // Остатков
            Bal = new strItem[PrCount];     // Балансовых остатков
            Ship= new strItem[PrCount];     // Отгрузок

            for(size_t i{}; i<PrCount; i++) {               // Читаем все массивы в едином цикле
                if(!(Pur+i)->RfF(_inF)) return false;       // Читаем массив закупок
                if(!(Rem+i)->RfF(_inF)) return false;       // Читаем массив остатков из партий сырья и материалов
                if(!(Bal+i)->RfF(_inF)) return false;       // Читаем массив балансовых остатков
                if(!(Ship+i)->RfF(_inF)) return false;      // Читаем массив отгрузок
            };
            #ifdef DEBUG
                long epos = _inF.tellg();                    // Определяем позицию в конце файла
                cout << "clsSKU::RfF end epos= " << epos << endl;
            #endif // DEBUG
            return true;                // Возвращаем true
        }   // RfF

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                                class clsStorage                                                     **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

        strItem* clsStorage::getresult(const strItem* (clsSKU::*f)() const) const {
        /** Метод создает и возвращает указатель на массив с объемами, учетной удельной и полной себестоимостями
        для всей номенклатуры отгрузок или закупок или остатков в зависимости от выбранной в качестве параметра
        функции f типа const strItem* (clsSKU::*)() const (GetPurchase - указатель на функцию, возвращающую массив
        с закупками, GetBalance - с остатками, GetShipment - с отгрузками). **/
            size_t RMCount = stock.size();                  // Получаем количество позиций сырья и материалов
            size_t tcount = RMCount * PrCount;              // Размер создаваемого массива
            if(tcount == sZero) return nullptr;             // Валидация параметров
            strItem* temp = new(nothrow) strItem[tcount];   // Выделяем память массиву
            if(!temp) {return nullptr; };                   // Если память не выделена, возвращаем nullptr
            size_t i = sZero;                                   // Индекс SKU
            for(clsSKU val: stock) {                            // Цикл по всем SKU
                var_cpy((temp+i*PrCount), (val.*f)(), PrCount); // Копируем данные в новый массив
                i++;
            };
//            for(clsSKU val: stock) {                            // Цикл по всем SKU
//                for(size_t j=sZero; j<PrCount; j++) {           // Цикл по всем периодам
//                    *(temp+PrCount*i+j) = *((val.*f)() + j);    // Копируем данные в новый массив
//                };
//                i++;
//            };
            return temp;
        }   // getresult

        void clsStorage::_setdataitem(size_t i, const auto val, void (clsSKU::*f)(const auto)) {
        /** 2024.11.01 Для SKU с индексом i метод устанавливает новые: наименование, единицу измерения,
        разрешение на отгрузку и закупку в одном и том же периоде, флаг авторасчета/ ручного расчета
        закупок, норматив запаса сырья. const auto val - новый устанавливаемый параметр. Для наименования
        и ед.измерения тип const string&; для разрешение на отгрузку и закупку const bool; для флага
        авторасчета const PurchaseCalc; для норматива const decimal (вещественное число). **/
            if(i >= stock.size()) return;                   // Валидация параметра
            vector<clsSKU>::iterator it = stock.begin()+i;  // Устанавливаем итератор на i-й элемент
            ((*it).*f)(val);                                // Вызываем нужную функцию С аргументом нужного типа
        }   // _setdataitem

        clsStorage::clsStorage() {
        /** Пустой конструктор. **/
            PrCount = sZero;
            hmcur = RUR;
            acct = FIFO;
            Calculation_Exit.store(false);
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsStorage Empty Ctor" << endl;
            #endif
        }   // clsStorage Empty Ctor

        clsStorage::clsStorage(size_t _pcnt, Currency _cur, AccountingMethod _ac) {
        /** Конструктор с параметрами **/
            PrCount = _pcnt;
            hmcur = _cur;
            acct = _ac;
            Calculation_Exit.store(false);
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsStorage Ctor with parametrs" << endl;
            #endif
        }   // clsStorage Ctor with parametrs

        clsStorage::clsStorage(size_t _pcnt, Currency _cur, AccountingMethod _ac, size_t stocksize) {
        /** Конструктор с созданием вектора заданного размера **/
            PrCount = _pcnt;            // Количество периодов проекта
            hmcur = _cur;               // Валюта проекта
            acct = _ac;                 // ПРинцип учета запасов
            Calculation_Exit.store(false);
            stock.reserve(stocksize);   // Резервируем память для stocksize элементов вектора
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsStorage Ctor with parametrs and size of vector" << endl;
            #endif
        }   // clsStorage Ctor with parametrs and size of vector

        clsStorage::clsStorage(const clsStorage& obj) {
        /** Конструктор копирования **/
            PrCount = obj.PrCount;
            hmcur   = obj.hmcur;
            acct    = obj.acct;
            Calculation_Exit.store(false);
            stock   = obj.stock;
            pshell = obj.pshell;
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsStorage Copy Ctor" << endl;
            #endif
        }   // Copy Ctor

        clsStorage::clsStorage(clsStorage&& obj) {
        /** Конструктор перемещения **/
            PrCount = sZero;
            hmcur   = RUR;
            acct    = FIFO;
            Calculation_Exit.store(false);
            pshell = nullptr;
            swap(obj);
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsStorage Move Ctor" << endl;
            #endif
        }   // Move Ctor

        void clsStorage::swap(clsStorage& obj) noexcept {
        /** Функция обмена значениями между объектами **/
            std::swap(PrCount, obj.PrCount);
            std::swap(hmcur, obj.hmcur);
            std::swap(acct, obj.acct);
            std::swap(pshell, obj.pshell);
            stock.swap(obj.stock);
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsStorage::swap" << endl;
            #endif
        }   // swap

        clsStorage& clsStorage::operator=(const clsStorage& obj){
        /** Перегрузка оператора присваивания копированием. Реализовано в идеоме
        КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsStorage tmp(obj);                // Вызываем конструктор копирования и получаем копию obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsStorage Copy operator=" << endl;
            #endif
            return *this;
        }   // Copy operator=

        clsStorage& clsStorage::operator=(clsStorage&& obj) {
        /** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
        (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsStorage tmp(move(obj));          // Вызываем конструктор перемещения и получаем содержание obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "clsStorage Move operator=" << endl;
            #endif
            return *this;
        }   // Move operator=

        clsStorage::~clsStorage() {
            #ifdef DEBUG    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                cout << "Dtor clsStorage" << endl;
            #endif
        }        // ~clsStorage

        /** Get-методы **/
        const size_t& clsStorage::ProjectCount() const { return PrCount; }  // Возвращает количество периодов проекта

        size_t clsStorage::Size() const { return stock.size(); }    // Возвращает размер вектора, равный количеству SKU

        const string& clsStorage::Name(size_t i) const {            // Возвращает Наименование SKU для элемента с индексом i
            if(i >= stock.size()) { return EmpStr; };
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->Name();
//            return stock.at(i).Name();
        }
        const string& clsStorage::Measure(size_t i) const {                 // Возвращает Единицу измерения SKU для элемента с индексом i
            if(i >= stock.size()) { return EmpStr; };
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->Measure();
//            return stock.at(i).Measure();
        }

        strNameMeas* clsStorage::GetNameMeas() {    // Функция создана 25.03.2024
        /** Метод возвращает указатель на вновь создаваемый массив типа strNameMeas с названием сыоья и
        единицами измерения для всех позиций **/
            size_t RMCount = stock.size();                          // Получаем количество позиций сырья и материалов
            if(RMCount == sZero) return nullptr;                    // Если ноль, то выход с nullptr
            strNameMeas* temp = new(nothrow) strNameMeas[RMCount];  // Выделяем память массиву
            if(!temp) {return nullptr; };                           // Если память не выделена, возвращаем nullptr
            for(size_t i=sZero; i<RMCount; i++) {                   // Цикл по элементам вектора
                (temp+i)->name = Name(i);
                (temp+i)->measure = Measure(i);
            }
            return temp;
        }   // GetNameMeas

        string clsStorage::Permission(size_t i) const {
        /** Возвращает разрешение на отгрузку и закупку для i-го SKU в виде текстовой строки **/
            if(i >= stock.size()) { return EmpStr; };
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->Permission();
        }
        string clsStorage::AutoPurchase(size_t i) const {
        /** Возвращает признак авторасчета закупок для i-го SKU в виде текстовой строки **/
            if(i >= stock.size()) { return EmpStr; };
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->AutoPurchase();
        }
        const size_t clsStorage::GetAutoPurchase(size_t i) const {
        /** Возвращает признак авторасчета закупок для i-го SKU **/
            if(i >= stock.size()) { return 2; } // Если элемента с таким индексом нет, возвращается заведомо неправильное число
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->GetAutoPurchase();
        }
        string clsStorage::HomeCurrency() const {       // Возвращает основную валюту проекта в виде текстовой строки
            return *(nmBPTypes::CurrencyTXT+hmcur);
        }   // HomeCurrency

        string clsStorage::Accounting() const {         // Возвращает принцип учета в виде текстовой строки
            return *(AccountTXT+acct);
        }   // Accounting

        const decimal& clsStorage::Share(size_t i) const {
        /** Возвращает норматив запаса сырья в долях от объема отгрузки **/
            if(i >= stock.size()) { return dZero; }
            vector<clsSKU>::const_iterator cit = stock.cbegin()+i;
            return cit->Share();
        }

        decimal* clsStorage::GetShipPrice() {
        /** Метод создает массив с учетной себестоимостью сырья и материалов для всей номенкладтуры и возвращает
        указатель на него. Массив является одномерным аналогом матрицы размером stock.size()*PrCount. **/
            size_t RMCount = stock.size();                  // Получаем количество позиций сырья и материалов
            size_t tcount = RMCount * PrCount;              // Размер создаваемого массива
            if(tcount == sZero) return nullptr;             // Валидация параметров
            decimal* temp = new(nothrow) decimal[tcount];   // Выделяем память массиву
            if(!temp) {return nullptr; };                   // Если память не выделена, возвращаем nullptr
            size_t i = sZero;
            for(vector<clsSKU>::const_iterator cit = stock.cbegin(); cit < stock.cend(); cit++) {
                for(size_t j=sZero; j<PrCount; j++) {
                    *(temp+PrCount*i+j) = (cit->GetShipment()+j)->price;
                };
                i++;
            };
            return temp;
        }   // GetShipPrice

        strItem* clsStorage::GetShip() const {
        /** Метод создает массив с объемами, учетной удельной и полной себестоимостями для всей номенклатуры отгрузок и
        возвращает указатель на него. Массив является одномерным аналогом матрицы размером stock.size()*PrCount.**/
            return getresult(&clsSKU::GetShipment);
        }   // GetShip

        strItem* clsStorage::GetPure() const {
        /** Метод создает массив с объемами, учетной удельной и полной себестоимостями для всей номенклатуры закупок и
        возвращает указатель на него. Массив является одномерным аналогом матрицы размером stock.size()*PrCount **/
            return getresult(&clsSKU::GetPurchase);
        }   // GetPure

        strItem* clsStorage::GetBal() const {
        /** Метод создает массив с объемами, учетной удельной и полной себестоимостями для всей номенклатуры остатков и
        возвращает указатель на него. Массив является одномерным аналогом матрицы размером stock.size()*PrCount **/
            return getresult(&clsSKU::GetBalance);
        }   // GetBal

        const strItem* clsStorage::GetDataItem(const ChoiseData _cd, size_t index, size_t _N) const {
        /** Метод возвращает данные для SKU с индексом index и периодом _N. Данные возвращаются в виде const-указателя
        на структуру типа strItem, содержащую информацию об объемах закупки или отгрузки, ценах и стоимости; выбор
        массива производится значением переменной _cd. **/
            if(index >= stock.size()) return nullptr;                   // Валидация параметра
            vector<clsSKU>::const_iterator cit = stock.cbegin()+index;  // Устанавливаем итератор на элемент с индексом index
            return cit->GetDataItem(_cd, _N);                           // Возвращаем указатель на данные
        }   // GetDataItem


        /** Set-методы **/

        void clsStorage::Set_progress_shell(clsProgress_shell<type_progress>* val) {
        /** Функция присваивает указателю pshell адрес объекта val **/
            pshell = val;
        }   // Set_progress_shell

        void clsStorage::SetCount(const size_t _n) {PrCount = _n; }     // Устанавливаем количество периодов проекта

        void clsStorage::SetName(size_t i, const string& _name) {
        /** Меняем наименование номенклатурной единицы (SKU) для элемента с индексом i **/
            _setdataitem(i, _name, &clsSKU::SetName);
        }   // SetName

        void clsStorage::SetMeasure(size_t i, const string& _measure) {
        /** Меняем ед.измерения номенклатурной единицы (SKU) для элемента с индексом i **/
            _setdataitem(i, _measure, &clsSKU::SetMeasure);
        }   // SetMeasure

        void clsStorage::SetCurrency(const Currency _cur) {
        /** Устанавливаем основную валюту проекта **/
             hmcur = _cur;
        }   // SetCurrency

        void clsStorage::SetPermission(size_t i, const bool _indr) {
        /** Меняем разрешение на отгрузку и закупку в одном и том же периоде для элемента с индексом i**/
            _setdataitem(i, _indr, &clsSKU::SetPermission);
        }   // SetPermission

        void clsStorage::SetAutoPurchase(size_t i, const PurchaseCalc _pcalc) {
        /** Меняем флаг авторасчета/ ручного расчета закупок для элемента с индексом i **/
            _setdataitem(i, _pcalc, &clsSKU::SetAutoPurchase);
        }   // SetAutoPurchase

        void clsStorage::SetShare(size_t i, const decimal _share) {
        /** Меняем норматив запаса сырья в долях от объема отгрузок в периоде для элемента с индексом i **/
            _setdataitem(i, _share, &clsSKU::SetShare);
        }   // SetShare

        void clsStorage::SetAccounting(const AccountingMethod _acct) { acct = _acct; }  /** Устанавливает принцип учета запасов **/

        bool clsStorage::SetDataItem(const ChoiseData _cd, const strItem _U, size_t index, size_t _N) {
        /** Функция записывает в элемент вектора с индексом index и период с индексом _N значение volume, price и value
        из переменной _U (побитовая запись); выбор массива производится значением переменной _cd. Метод работает только
        с непустым вектором. В случае успешной записи, метод возвращает true. **/
            if(index >= stock.size()) return false;             // Валидация параметра
            vector<clsSKU>::iterator it = stock.begin()+index;  // Устанавливаем итератор на i-й элемент
            return it->SetDataItem(_cd, _U, _N);                // Записываем данные в указанный элемент вектора
        }   // SetDataItem

        bool clsStorage::SetSKU(const string& Name, const string& Measure, PurchaseCalc _flag, decimal _share, bool _perm,\
            strItem _ship[], strItem _pur[]) {
        /** Метод создания склада для конкретного SKU. Создает новый экземпляр класса cksSKU непоредственно в векторе.
        Параметры: Name - наименование SKU, Measure - единица измерения SKU, _flag - флаг разрешающий авторасчет закупок,
        _share - запас ресурсов на складе в каждый период, выраженный в доле от объема отгрузок за этот период,
        _perm - флаг разрешающий/запрещающий закупки и отгрузки в одном периоде, _ship - массив отгрузок, _pur - массив
        закупок. Метод работает как с пустым, так и не пустым вектором. **/
            if(_flag==nocalc) {                             // Если установлен ручной расчет закупок, то проверяем
                if(!CheckPurchase(_pur)) { return false; }  // корректность. Если массив некорректен, то выход и возврат false
            };
            vector <clsSKU>::iterator it;                   // Итератор для вектора
            it = find(stock.begin(), stock.end(), Name);    // Поиск объекта по имени
            if(it != stock.end()) { return false; };        // Если объект с таким именем уже есть, то выход и возврат false
            stock.emplace_back(PrCount, Name, Measure, acct, _perm, _flag, _share, _ship);  // Если имя уникально, то создаем
                                                                                            // склад для данного SKU
            if(_pur) {                                          // Если массив закупок не равен nullptr, то
                return stock.back().SetPurchase(_pur, PrCount); // и вводим закупки
            };
            return true;
        }   // SetSKU

        bool clsStorage::SetPurchase(size_t isku, const strItem _unit[], size_t _count) {
        /** Метод загружает массив закупок _unit размером _count в индивидуальный склад с номером isku. **/
            if((!_unit) || (isku >= stock.size()) || (_count==sZero)) return false;;    // Валидация параметров
            if(!CheckPurchase(_unit)) { return false; }                                 // Проверка корректности массива
            vector <clsSKU>::iterator it = stock.begin() + isku;    // Установка итератора на SKU с индексом isku
            return it->SetPurchase(_unit, _count);                  // Загружаем массив закупок
        }   // SetPurchase

        bool clsStorage::SetPurPrice(const strItem _unit[]) {
        /** Метод загружает цены из одномерного массива, являющегося аналогом матрицы с планами закупок всех позиций сырья
        в натуральном, удельном и полном стоимостном выражении. Размер матрицы RMCount*PrCount. Каждый элемент матрицы имеет
        тип strItem, т.е. имеет в своем составе значения volume, price и value для продукта. Используются только поля price. **/
            if(!_unit) return false;    // Валидация параметров
            size_t i=sZero;             // Счетчик
            for(vector<clsSKU>::iterator it = stock.begin(); it < stock.end(); it++) {  // По каждому складу загружаем цены,
               if(!it->SetPurPrice((_unit+PrCount*i), PrCount)) return false;   // ели загрузка не удалась, то выход с false
               i++;
            };
            return true;
        }   // SetPurPrice

        bool clsStorage::CheckPurchase(const strItem _pur[]) const {
        /** При флаге, запрещающем автоматический расчет закупок, массив закупок должен содержать корректные цифры, т.е
        произведение volume * price == value. Если это условие не соблюдается, расчетная себестоимость сырья может
        считаться некорректно. Данный метод проверяет корректность массива закупок. **/
            if(!_pur) return false;                 // Если массив не существует, возврат false
            for(size_t i=sZero; i<PrCount; i++) {
                if( fabs(((_pur+i)->volume * (_pur+i)->price) - (_pur+i)->value) > epsln ) {
                    return false;
                }
            };
            return true;
        }   // CheckPurchase

        bool clsStorage::SetStorage(size_t RMCount, const strNameMeas RMNames[], decimal ShipPlan[], decimal PricePur[]) {
        /** Метод ввода исходных данных в формате, совпадающим с выходным форматом strNameMeas* clsManufactory::GetRefRawNames
        и clsManufactory::decimal* GetRawMatPurchPlan(). После создания "пустого" экземпляра класса clsStorage, данный метод
        создает склады для отдельных продуктов в количестве, равном количеству позиций сырья и материалов, требуемых для
        производственной программы объекта типа clsManufactory (равно clsManufactory::RMCount. Параметры: RMCount - число
        номенклатурных позиций сырья и материалов, RMNames - массив с названиями сырья и материалов, размерностью RMCount;
        помимо названий в массиве присутствуют единицы измерения. ShipPlan - массив отгрузок сырья и материалов в производство
        для каждого наименования сырья в каждом периоде проекта; массив представляет собой одномерный аналог двумерной матрицы
        размером RMCount*PrCount. Для доступа к элементу с индексом i,j используется арифметика указателей: *(X+cols*i+j),
        где X - указатель на первый элемент массива, cols - число столбцов в матрице, i и j - соответственно номер строки и
        столбца в матрице. PricePur - массив цен закупаемых сырья и материалов размером RMCount*PrCount.
        Этот метод устанавливает для всех частных складов флаг разрешающий автоматический расчет закупок (calc),
        нулевой запас сырья и материалов на складе в каждый период и флаг разрешающий закупки и отгрузки в одном периоде (true);
        в дальнейшем эти параметры можно скорректировать для каждого склада отдельно другими методами класса. Данный метод
        целесообразно применять в паре с конструктором с параметрами, устанавливающим число периодов проекта, домашнюю валюту
        и принцип учета запасов (clsStorage(size_t _pcnt, Currency _cur, AccountingMethod _ac)).
        ВНИМАНИЕ!!!  массив ShipPlan - динамический, после их использования требуют удаления в ручном
        режиме оператором delete[].**/
            if((RMCount==sZero) || (!RMNames) || (!ShipPlan) || (!PricePur)) return false;  // Валидация входных параметров
            if(!stock.empty()) stock.clear();                                   // Если вектор не пустой, то удаляем все элементы
            stock.reserve(RMCount);                                             // Резервируем память для вектора
            strItem *tship, *tppur;                                             // Вспомогательный массив
            for(size_t i=sZero; i<RMCount; i++) {                               // Цикл по номенклатуре сырья
                tship = new(nothrow) strItem[PrCount];                          // Резервируем память массиву
                if(!tship) return false;                                        // Если память не выделена, то выход с false
                tppur = new(nothrow) strItem[PrCount];                          // Резервируем память массиву
                if(!tppur) {                                                    // Если память не выделена, то
                    delete[] tship;                                             // удаляем созданный ранее массив
                    return false;                                               // и выходим с false
                };
                for(size_t j=sZero; j<PrCount; j++) {                           // Копируем во вспомогательную переменную
                    (tship+j)->volume = *(ShipPlan+PrCount*i+j);                // план отгрузок для j-го сырья
                    (tship+j)->price = (tship+j)->value = dZero;                // поля price и value заполняем нулями
                    (tppur+j)->price = *(PricePur+PrCount*i+j);                 // Закупочные цены сырья и материалов
                    (tppur+j)->volume = (tppur)->value = dZero;                 // поля volume и value заполняем нулями

                };
                /** Создаем склад для i-го сырья **/
                stock.emplace_back(PrCount, (RMNames+i)->name, (RMNames+i)->measure, acct, true, calc, dZero, tship);
                bool rez = stock.back().SetPurchase(tppur, PrCount);    // Вводим закупки
                delete[] tship;                                         // Удаляем вспомогательный массив
                delete[] tppur;                                         // Удаляем вспомогательный массив
                if(!rez) return false;                                  // Если запись закупок не удачна, выход с false
            };
            return true;
        }   // SetStorage

        bool clsStorage::SetStorage(size_t RMCount, const strNameMeas RMNames[], strItem ShipPlan[], strItem Purchase[]) {
        /** Аналогичный предыдущему, но массивы отгрузок и закупок в формате strItem, загружаются и цены и объемы.
        Метод допускает nullptr вместо ссылки на массив закупок. **/
            if((RMCount==sZero) || (!RMNames) || (!ShipPlan)) return false;     // Валидация входных параметров
            if(!stock.empty()) stock.clear();                                   // Если вектор не пустой, то удаляем все элементы
            stock.reserve(RMCount);                                             // Резервируем память для вектора
            for(size_t i=sZero; i<RMCount; i++) {                               // Цикл по номенклатуре сырья
                /** Создаем склад для i-го сырья **/
                stock.emplace_back(PrCount, (RMNames+i)->name, (RMNames+i)->measure, acct, true, calc, dZero, (ShipPlan+i*PrCount));
                if(Purchase)
                    stock.back().SetPurchase((Purchase+i*PrCount), PrCount); // Вводим закупки
            };
            return true;
        }   // SetStorage 2024.12.10

        bool clsStorage::SetData(const clsSKU &obj) {
        /** Функция заносит в вектор данных информацию о конкретном SKU путем копирования экземпляра класса clsSKU в вектор **/
            vector <clsSKU>::iterator it;                       // Итератор для массива
            it = find(stock.begin(), stock.end(), obj.Name());  // Поиск объекта по имени
            if(it != stock.end()) { return false; };            // Если объект с таким именем уже есть, то выход и возврат false
            stock.emplace_back(obj);                            // Если имя уникально, то создаем склад для данного SKU
            return true;
        };

        bool clsStorage::SetData(clsSKU &&obj) {
        /** Функция заносит в вектор данных информацию о конкретном SKU путем перемещения экземпляра класса clsSKU в вектор **/
            vector <clsSKU>::iterator it;                       // Итератор для массива
            it = find(stock.begin(), stock.end(), obj.Name());  // Поиск объекта по имени
            if(it != stock.end()) { return false; };            // Если объект с таким именем уже есть, то выход и возврат false
            stock.emplace_back(move(obj));                      // Если имя уникально, то создаем склад для данного SKU
            return true;
        };

        void clsStorage::EraseSKU(size_t _i) {
        /** Метод удаляет номенклатурную позицию под номером _i и все связанные с ней данные **/
            stock.erase(stock.begin() + _i);
        }   // EraseSKU

        /** Методы визуального контроля **/

        void clsStorage::ViewSettings() const {
        /** Функция выводит на экран информацию о настройках для расчета. К настройкам относятся параметры:
        длительность проекта, домашняя (основная) валюта, метод учета запасов, разрешение/запрет на закупки
        и отгрузки в одном периоде, признак авто/ ручного расчета закупок, норматив запаса ресурсов. **/
            nmPrntSrvs::clsTextField fld(15);
            cout << fld << "Name" << fld << "Measure" << fld << "Accounting" << fld << "Permission" << \
            fld << "AutoPurchase" << "Share" << endl;
            for(vector <clsSKU>::const_iterator cit = stock.cbegin(); cit < stock.cend(); cit++) {
                string _Name = cit->Name();
                _Name.resize(15);
                string _Meas = cit->Measure();
                _Meas.resize(8);
                string _APurch = cit->AutoPurchase();
                _APurch.resize(8);
                cout << fld << _Name << fld << _Meas << fld << cit->AccMethod() << \
                fld << cit->Permission()<< fld << _APurch << cit->Share() << endl;
            }
        }   // ViewSettings

        void clsStorage::View() const {
        /** Функция выводит на экран информацию по всему складу **/
            vector <clsSKU>::const_iterator it;   // Итератор для массива
            it = stock.cbegin();
            while(it != stock.cend()) {
                it->View();
                it++;
            }
        }

        void clsStorage::ViewChoise(ChoiseData _arr, ReportData flg) const {
        /** Метод визуального контроля выбранного массива для всех складов. Доступные массивы:
        _arr = purchase, массив поступлений на склад; _arr = balance, массив остатков на складе;
        _arr = shipment, массив отгрузок сос клада. Параметр flg - тип выводимой информации:
        volume - в натуральном, value - в стоимостном, price - в ценовом измерении **/
            size_t ProdNameCount = Size();              // Количество продуктов в складе
            size_t tmpcount = ProdNameCount*PrCount;    // Вспомогательная переменная
            if(tmpcount == sZero) return;               // Валидация параметров
            const strItem* (clsSKU::*f)() const;        // Определяем указатель на внутреннюю функцию класса clsSKU
            if(_arr==purchase) f = &clsSKU::GetPurchase;    // Назначение указателю функции
            else                                            // возврата указателя на массив поступлений на склад
                if(_arr==balance) f = &clsSKU::GetBalance;  // возврата указателя на массив остатков на складе
                else f = &clsSKU::GetShipment;              // возврата указателя на массив отгрузок со склада
            nmBPTypes::strNameMeas* pNames = new(nothrow) nmBPTypes::strNameMeas[ProdNameCount];    // Временный массив имен
            if(!pNames) return;                 // Если память не выделена, то выход из метода
            nmBPTypes::strItem* pData = new(nothrow) nmBPTypes::strItem[tmpcount];                  // Временный массив данных
            if(!pData) {                        // Если память не выделена, то
                delete[] pNames;                // удаляем массив имен
                return;                         // и выходим из метода
            };
            for(size_t i=sZero; i<ProdNameCount; i++) {     // Цикл по всем индивидуальным складам
                (pNames+i)->name = stock[i].Name();         // Заполняем массив именами
                (pNames+i)->name.resize(15);                // Урезаем длину строки
                (pNames+i)->measure = stock[i].Measure();   // Заполняем массив наименованиямиединиц измерения
                (pNames+i)->measure.resize(8);              // Урезаем длину строки
                var_cpy((pData+i*PrCount), (stock[i].*f)(), PrCount); // Копируем данные
            };
            nmPrntSrvs::ArrPrint(ProdNameCount, pNames, pData, PrCount, flg);                   // Выводим отчет на экран
            if(pNames) delete[] pNames;                 // Удаляем всмомогательный массив
            if(pData) delete[] pData;                   // Удаляем всмомогательный массив
        }   // ViewChoise

        /** Расчетные методы **/

        TLack clsStorage::Calculate(size_t bg, size_t en) {
        /** Функция проверяет количество периодов у каждого SKU: если оно отличается от количества, установленного в
        переменной PRCount, то для этого SKU вызывается функция Resize, которая приводит количество периодов к заданному.
        Функция также рассчитывает требуемый объем закупок для тех SKU, у которых выставлен разрешающий такой расчет флаг.
        Для каждого SKU функция рассчитывает цены и стоимость отгружаемых партий в соответствии с выбранным принципом FIFO,
        LIFO или AVG. Функция завершает работу при нахождении первого дефицита и сообщает о размере этого дефицита и
        наименовании SKU, где найден дефицит. Дальнейшие расчеты не производятся. Информация возвращается в виде структуры
        типа TLack. Все расчеты производятся в тех SKU, чьи индексы в векторе stock лежат между bg и en. **/
            TLack Lack;                         // Вспомогательная переменная
            size_t lim = stock.size();          // Верхняя граница опустимого значения индекса
            if(bg>=lim) return Lack;            // Валидация нижней границы индекса
            if(en>=lim) en = lim;               // Валидация верхней границы индекса
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // Сбрасываем счетчик
            for(size_t i = bg; i<en; i++) {
                if(Calculation_Exit.load(memory_order_relaxed)) {                   // Если установлен флаг останова,
                    (pshell) ? pshell->Counter_max() : (void)([](){return;});       // то максимизируем счетчик
                    break;                      // и выходим из цикла
                }
                clsSKU* p = (stock.data()+i);   // Вспомогательный указатель на экземпляр массива SKU
                p->SetAccount(acct);            // Устанавливаем принцип учета запасов
                if(p->Count() != PrCount)       // Если длительности проектов по SKU отличаются от PrCount
                    p->Resize(PrCount);         // выравниваем длительности
                Lack = p->Calculate();          // Основные вычисления
                (pshell) ? pshell->Counter_inc() : (void)([](){return;});       // Вызываем счетчик
                if(fabs(Lack.lack) > epsln) {                                   // и если обнаруживаем дефицит, то
                    Calculation_Exit.store(true, memory_order_relaxed);         // устанавливаем флаг выхода для других
                    (pshell) ? pshell->Counter_max() : (void)([](){return;});   // потоков и максимизируем счетчик
                    return Lack;    // и заканчиваем цикл, поскольку все расчеты не действительны и выходим из функции
                };
            };
            return {dZero, EmpStr};
        }   // void Calculate(size_t, size_t)

        TLack clsStorage::Calculate() {
        /** Функция проверяет количество периодов у каждого SKU: если оно отличается от количества, установленного в
        переменной PRCount, то для этого SKU вызывается функция Resize, которая приводит количество периодов к заданному.
        Функция также рассчитывает требуемый объем закупок для тех SKU, у которых выставлен разрешающий такой расчет флаг.
        Для каждого SKU функция рассчитывает цены и стоимость отгружаемых партий в соответствии с выбранным принципом FIFO,
        LIFO или AVG. Функция завершает работу при нахождении первого дефицита и сообщает о размере этого дефицита и
        наименовании SKU, где найден дефицит. Дальнейшие расчеты не производятся. Информация возвращается в виде структуры
        типа TLack. Расчеты производятся во всех SKU. **/
            TLack Lack;                         // Вспомогательная переменная
            for(size_t i{}; i<stock.size(); i++) {
                clsSKU* p = (stock.data()+i);   // Вспомогательный указатель на экземпляр массива SKU
                p->SetAccount(acct);            // Устанавливаем принцип учета запасов
                if(p->Count() != PrCount)       // Если длительности проектов по SKU отличаются от PrCount
                    p->Resize(PrCount);         // выравниваем длительности;
                Lack = p->Calculate();          // Основные вычисления
                (pshell) ? pshell->Update((int)i) : (void)([](){return;});    // Вызываем индикатор прогресса
                if(fabs(Lack.lack) > epsln) {   // и если обнаруживаем дефицит, то
                    (pshell) ? pshell->Update((int)stock.size()) : (void)([](){return;});    // индикатор на 100%,
                    return Lack;    // заканчиваем цикл, поскольку все расчеты не действительны и выходим из функции
                };
            };
            (pshell) ? pshell->Update((int)stock.size()) : (void)([](){return;});    // Индикатор на 100%
            return {dZero, EmpStr};
        }   // Calculate1

        TLack clsStorage::Calculate_future() {
        /** Функция проверяет количество периодов у каждого SKU: если оно отличается от количества, установленного в
        переменной PRCount, то для этого SKU вызывается функция Resize, которая приводит количество периодов к заданному.
        Функция также рассчитывает требуемый объем закупок для тех SKU, у которых выставлен разрешающий такой расчет флаг.
        Для каждого SKU функция рассчитывает цены и стоимость отгружаемых партий в соответствии с выбранным принципом FIFO,
        LIFO или AVG. Функция завершает работу при нахождении первого дефицита и сообщает о размере этого дефицита и
        наименовании SKU, где найден дефицит. Дальнейшие расчеты не производятся. Информация возвращается в виде структуры
        типа TLack. Все расчеты запускаются в асинхронных потоках, число которых на единицу меньше числа ядер компьютера. **/
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // Определение макс. числа потоков через число ядер
            if(maxthreads < sTwo)                                           // Если макс. число потоков менее двух, то
                return Calculate();                                         // проводим вычисления методом Calculate и выходим
            vector<std::future<TLack>> pool;            // Пул потоков в виде вектора
            pool.reserve(maxthreads);                   // Резервируем память вектору потоков
            size_t stocksize = stock.size();            // Получаем размер массива с SKU
            size_t ncap = (stocksize/maxthreads)+sOne;  // Количество SKU для каждого потока
            TLack Lack[maxthreads]{};                   // Вспомогательный массив с результатами вычислений
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // Сбрасываем счетчик
            for(size_t i{}; i<maxthreads; i++) {        // Цикл по всему пулу потоков
                size_t bg = i*ncap;                     // Определяем нижнюю границу индекса
                if(bg>=stocksize) break;                // Если нижняя граница больше или равна числу SKU, выходим из цикла
                size_t en = (i+sOne)*ncap;              // Определяем верхнюю границу индекса
                pool.push_back(std::async([](clsStorage* pobj, size_t _bg, size_t _en) {    // Создаем поток и запускаем вычисления
                    // В качестве первого аргумента функции async используем лямбду "[](){}"
                    #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                        mu.lock();                          // Блокируем ресурс cout для монопольного использования
                        cout << "Async thread #" << std::this_thread::get_id() << " has been started" << endl;
                        mu.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                    #endif
                    TLack res = pobj->Calculate(_bg, _en);  // Вызываем метод с вычислениями
                    #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                        mu.lock();                          // Блокируем ресурс cout для монопольного использования
                        cout << "Async thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                        mu.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                    #endif
                    return res;
                }, this, bg, en));                      // После лямбды стоят аргументы для нее: this, bg, en
            };                                          // Цикл по всему пулу потоков
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;});   // Вызываем отрисовку индикатора
            size_t k{};
            for(auto &t : pool) {
                *(Lack+k) = t.get();
                k++;
            };
            Calculation_Exit.store(false, memory_order_relaxed);    // Сбрасываем флаг, чтобы можно было снова вызвать Calculate(size_t
            for(size_t i{}; i<maxthreads; i++ )         // Цикл по всем элементам вспомогательного массива с результатами
                if((Lack+i)->lack>epsln)                // Если обнаружен дефицит, то
                    return *(Lack+i);                   // выходим и возвращаем величину дефицита и наименование SKU
            return {dZero, EmpStr};                     // Благополучное завершение и выход без дефицита
       }    // Calculate2

        TLack clsStorage::Calculate_thread() {
        /** Функция проверяет количество периодов у каждого SKU: если оно отличается от количества, установленного в
        переменной PRCount, то для этого SKU вызывается функция Resize, которая приводит количество периодов к заданному.
        Функция также рассчитывает требуемый объем закупок для тех SKU, у которых выставлен разрешающий такой расчет флаг.
        Для каждого SKU функция рассчитывает цены и стоимость отгружаемых партий в соответствии с выбранным принципом FIFO,
        LIFO или AVG. Функция завершает работу при нахождении первого дефицита и сообщает о размере этого дефицита и
        наименовании SKU, где найден дефицит. Дальнейшие расчеты не производятся. Информация возвращается в виде структуры
        типа TLack. Все расчеты запускаются в потоках, число которых на единицу меньше числа ядер компьютера. **/
            auto do_Calculate = [](void *param, size_t bg, size_t en, TLack &res) {
            /** "Обертка" в виде Лямбда-функции для метода TLack Calculate(size_t&, size_t&), предназначенная для запуска
            указанного метода в потоке tread. **/
                clsStorage* pobj = (clsStorage*)param;  // Вспомогательный указатель на экземпляр класса clsStorage
                #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                    mu.lock();                          // Блокируем ресурс cout для монопольного использования
                    cout << "Thread #" << std::this_thread::get_id() << " has been started" << endl;
                    mu.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                #endif
                res = pobj->Calculate(bg, en);          // Основные вычисления
                #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                    mu.lock();                          // Блокируем ресурс cout для монопольного использования
                    cout << "Thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                    mu.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                #endif
            };  // do_Calculate
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // Определение макс. числа потоков через число ядер
            if(maxthreads < sTwo)                                           // Если макс. число потоков менее двух, то
                return Calculate();                                         // проводим вычисления методом Calculate и выходим
            vector<std::thread> pool;                   // Пул потоков в виде вектора
            pool.reserve(maxthreads);                   // Резервируем память вектору потоков
            size_t stocksize = stock.size();            // Получаем размер массива с SKU
            size_t ncap = (stocksize/maxthreads)+sOne;  // Количество SKU для каждого потока
            TLack Lack[maxthreads]{};                   // Вспомогательный массив с результатами вычислений
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // Сбрасываем счетчик
            for(size_t i{}; i<maxthreads; i++) {        // Цикл по всему пулу потоков
                size_t bg = i*ncap;                     // Определяем нижнюю границу индекса
                if(bg>=stocksize) break;                // Если нижняя граница больше или равна числу SKU, выходим из цикла
                size_t en = (i+sOne)*ncap;              // Определяем верхнюю границу индекса
                pool.emplace_back(do_Calculate, this, bg, en, std::ref(*(Lack+i)));   // Создаем поток и запускаем вычисления
            };  // Для возврата значения из потока используем эмулятор ссылки std::ref(...), см.:
                // https://pro--prof-com.turbopages.org/pro-prof.com/s/forums/topic/cplusplus_reference_wrapper
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;}); // Вызываем отрисовку индикатора
            for(auto &t : pool)                         // Цикл по всему пулу потоков
                t.join();                               // Ожидаем завершения каждого запущенного потока
            Calculation_Exit.store(false, memory_order_relaxed);// Сбрасываем флаг, чтобы можно было снова вызвать Calculate(size_t
            for(size_t i{}; i<maxthreads; i++ )         // Цикл по всем элементам вспомогательного массива с результатами
                if((Lack+i)->lack>epsln)                // Если обнаружен дефицит, то
                    return *(Lack+i);                   // выходим и возвращаем величину дефицита и наименование SKU
            return {dZero, EmpStr};                     // Благополучное завершение и выход без дефицита
       }    // Calculate3

        bool clsStorage::StF(ofstream &_outF)  {
        /** Метод имплементации записи в файловую переменную текущего экземпляра класса (метод сериализации).
        Параметры: &_outF - экземпляр класса ofstream для записи данных **/
            size_t stocksize = stock.size();        // Записываем в переменную количество элементов вектора
            size_t capac = stock.capacity();        // Записываем в переменную текущий размер (емкость) вектора
            if(stocksize == sZero) return false;    // Если вектор пустой, то выход и возврат false
            if(!SEF(_outF, PrCount)) return false;        // Сохраняем количество периодов
            if(!SEF(_outF, hmcur)) return false;          // Сохраняем основную валюту проекта
            if(!SEF(_outF, acct)) return false;           // Сохраняем принцип учета
            if(!SEF(_outF, capac)) return false;          // Сохраняем емкость вектора в файл
            if(!SEF(_outF, stocksize)) return false;      // Сохраняем количество элементов вектора в файл
//            for(size_t i=sZero; i<stocksize; i++) {     // Сохраняем массив экземпляров типа clsSKU
//                stock.at(i).StF(_outF);
//            };
            for(vector<clsSKU>::iterator it = stock.begin(); it < stock.end(); it++)
                if(!it->StF(_outF)) return false;         // Сохраняем массив экземпляров типа clsSKU
            return true;                                  // Возвращаем true
        }   // StF

        bool clsStorage::RfF(ifstream &_inF) {
        /** Метод имплементации чтения из файловой переменной текущего экземпляра класса (метод десериализации).
        Параметры: &_inF - экземпляр класса ifstream для чтения данных **/
            size_t stocksize;           // Временная переменная с числом элементов вектора
            size_t capac;               // Временная переменная с объемом памяти вектора
            if(!DSF(_inF, PrCount)) return false;         // Читаем количество периодов
            if(!DSF(_inF, hmcur)) return false;           // Читаем основную валюту проекта
            if(!DSF(_inF, acct)) return false;            // Читаем принцип учета
            if(!DSF(_inF, capac)) return false;           // Читаем емкость вектора
            if(!DSF(_inF, stocksize)) return false;       // Читаем количество элементов вектора
            if(capac == sZero) return false;
            stock.reserve(capac);       // Резервируем память для вектора
            for(size_t i=sZero; i<stocksize; i++) {
                stock.emplace_back(PrCount, "", "", acct, true, calc, dZero);   // Создаем элемент непосредственно в векторе
                if(!stock.back().RfF(_inF)) return false;                       // Вызываем метод десериализации этого элемента
            };
            return true;                    // Возвращаем true
        }   // RfF

# undef DEBUG           // отменить макрос информации об инструменте DEBUG
# undef DEBUG_THREADS   // отменить макрос информации об инструменте DEBUG
