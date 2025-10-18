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

#include "manufact_module.h"

//#define CDtor_voice       // Макрос вывода отладочной информации. Раскомментировать для отладки
//#define DEBUG_THREADS     // Макрос вывода отладочной информации. Раскомментировать для отладки

std::mutex mu1;             // Защита от одновременного доступа потоков к одному ресурсу

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                             Вспомогательные функции                                                 **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

inline void Sum(decimal arr1[], const decimal arr2[], size_t N) {
/** Метод суммирует поэлементно два массива, результат сохраняет в первом массиве. Параметры: arr1 - первый массив,
arr2 - второй массив, N - размерность обоих массивов. Оба массива должны иметь одинаковый размер N. Метод inline для
встраивания его кода в точку вызова. Внимание!!! метод не проверяет совпадение размерности массивов **/
    if(arr1 && arr2)                                // Если оба массива существуют, то
        for(size_t i=sZero; i< N; i++) {            // Производим операцию суммирования
            *(arr1+i) += *(arr2+i);
        }
}   // Sum

inline void Dif(decimal arr1[], const decimal arr2[], size_t N) {
/** Метод вычитает поэлементно два массива, результат сохраняет в первом массиве. Параметры: arr1 - первый массив,
arr2 - второй массив, N - размерность обоих массивов. Оба массива должны иметь одинаковый размер N. Метод inline для
встраивания его кода в точку вызова. Внимание!!! метод не проверяет совпадение размерности массивов **/
    if(arr1 && arr2)                                // Если оба массива существуют, то
        for(size_t i=sZero; i< N; i++) {            // Производим операцию суммирования
            *(arr1+i) -= *(arr2+i);
        }
}   // Dif

inline decimal *Mult(const decimal arr1[], const decimal arr2[], const size_t N) {
/** Метод возвращает результат умножения элементов двух массивов с одинаковыми индексами друг на друга.
Результат возвращается в виде указателя на новый массив. Параметры: arr1 - первый массив, arr2 - второй массив,
N - размерность обоих массивов. Оба массива должны иметь одинаковый размер N. Внимание!!! метод не проверяет
совпадение размерности массивов и не проверяет массивы на nullptr **/
    if(arr1 && arr2) {                              // Если массивы существуют
        decimal *temp = new(nothrow) decimal[N];    // Выделяем память под вспомогательный массив temp
        if(!temp) { return nullptr; }               // Если память не выделена, то выход и возврат nullptr
        for(size_t i=sZero; i< N; i++) {            // Если память выделена, то умножаем элементы массива
            *(temp+i) = (*(arr1+i)) * (*(arr2+i));
        };
        return temp;
        } else return nullptr;
}   // Mult

inline void v_service(const strNameMeas* arr, size_t _rcount) {
/** Сервисная функция, использующаяся в методах ViewRawNames и ViewRefRawNames **/
    clsTextField name(15);              // Установка формата вывода имен с помощью класса clsTextField
    clsTextField meas(12);              // Установка формата вывода единиц измерения
    cout << name << strName << meas << strMeas << endl;
    for(size_t i=sZero; i<_rcount; i++) {
        cout << name << arr->name << meas << arr->measure << endl;
    };
    cout << endl;
}   // v_service

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                             class clsRecipeItem                                                     **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

/** Класс - рецептура отдельного продукта. Рецептура каждого продукта представляет собой матрицу: строки матрицы - это сырье,
использующееся в производстве продукта, столбцы - периоды производственного цикла; на пересечении строк и столбцов указывается
удельный расход сырья на производство единицы данного продукта в натуральном выражении в указанный период производственного
цикла. Период с самым большим номером соответствует периоду выхода готового продукта. **/

        inline decimal* clsRecipeItem::CalcRawMatVolumeItem(const size_t PrCount, const size_t period, const decimal volume) const {
        /** Метод рассчитывает объем потребления ресурсов в каждом периоде технологического цикла в зависимости от
        требуемого объема готового продукта на выходе этого цикла. Метод рассчитывает потребность в ресурсах только для
        одного единичного производственного цикла (ЕПЦ). ЕПЦ - это процесс производства одного наименования продукта в
        каком-либо отдельном периоде проекта. Параметры:  PrCount - число периодов проекта, period - номер периода проекта,
        в котором требуется готовый произведенный продукт, volume - требуемый объем этого продукта. Метод возвращает
        указатель на одномерный массив, аналог двумерной матрицы, размером rcount*PrCount с рассчитанными объемами
        потребных в производстве ресурсов: число строк матрицы совпадает с числом позиций ресурсов, число столбцов - с
        длительностью проекта. Нулевой период проекта не задействуется (соответствует стартовому балансу).
        ВНИМАНИЕ!!! При вызове данного метода следует помнить о необходимости высвобождения памяти и удалении
        созданного данным методом динамического массива **/
            if(period < duration) { return nullptr; };      // Если период меньше, чем производственный цикл, возврат "пусто"
            if(period >= PrCount) { return nullptr; };      // Если период больше или равно длительности проекта, возврат "пусто"
            if(rcount == sZero) { return nullptr; };        // Если число номенклатурных позиций сырья нулевое, то "пусто"
            if(!rnames) { return nullptr; };                // Если отсутствует массив с номенклатурой сырья, то "пусто"
            if(!recipeitem) { return nullptr; };            // Если отсутствует матрица с рецептурами, то "пусто"
            const size_t tcount = rcount * PrCount;         // Емкость создаваемого массива
            decimal *temp = new(nothrow) decimal[tcount]{}; // Выделяем память для массива с обработкой исключения и nothrow:
            // если память не выделена, то возврат nullptr (https://cplusplus.com/reference/new/nothrow/)
            if(!temp) { return nullptr; };                  // Если память не выделена, то выход и возврат "пусто"
//            for(size_t i=sZero; i<tcount; i++) {            // Заполняем нулями вспомогательный массив
//                *(temp+i) = dZero;
//            };
            const size_t diff = period - duration;          // Нижняя граница цикла (начало поступления сырья в производство)
            for(size_t i=sZero; i<rcount; i++) {            // Цикл по номенклатуре сырья
                for(size_t j=period, k=duration; (j>diff)&&(k>sZero); j--, k--) {   // Цикл по периодам внутри производственного цикла
                    size_t n = k-sOne; // Временная переменная для предотвращения зацикливания, когда за нулем следуем положительное число
                    *(temp + PrCount*i+j) = *(recipeitem + duration*i+n) * volume;  // Расчет объема сырья
                }   // Цикл по периодам внутри производственного цикла
            }       // Цикл по номенклатуре сырья
            return temp;
        }   // CalcRawMatVolumeItem

        inline decimal* clsRecipeItem::CalcWorkingVolumeItem(const size_t PrCount, const size_t period, const decimal volume) const {
        /** Метод рассчитывает объемы незавершенного производства для продукта, выпускаемого в конкретный период проекта.
        Параметры:  PrCount - число периодов проекта, period - номер периода проекта, в котором требуется готовый произведенный
        продукт, volume - требуемый объем этого продукта. Метод возвращает указатель на одномерный массив рассчитанных объемов
        незавершенного производства в натуральном выражении размером PrCount. ВНИМАНИЕ!!! При вызове данного метода следует
        помнить о необходимости высвобождения памяти и удалении созданного данным методом динамического массива. **/
            if(period < duration) { return nullptr; };      // Если период меньше, чем производственный цикл, возврат "пусто"
            if(period >= PrCount) { return nullptr; };      // Если период больше или равно длительности проекта, возврат "пусто"
            decimal *temp = new(nothrow) decimal[PrCount]{};// Выделяем память массиву
            if(!temp) { return nullptr; };                  // Если память не выделена, то выход и возврат "пусто"
            const size_t diff = period - duration;          // Нижняя граница цикла (начало поступления сырья в производство)
            const size_t frm = period-sOne;                 // Верхняя граница цикла (предшествует готовности продукта)
            for(size_t j=frm; j>diff; j--) {
                *(temp+j) = volume;                         // Заполняем массив значениями объемов незавершенного производства
            }
            return temp;
        }   // CalcWorkingVolumeItem

        decimal* clsRecipeItem::CalcWorkingVolume(const size_t PrCount, const strItem volume[]) const {
        /** Метод рассчитывает объемы незавершенного производства для конкретного продукта в натуральном выражении,
        выпускаемого на протяжении всего проекта. Параметры:  PrCount - число периодов проекта, volume - указатель
        на массив размерности PrCount с объемами производства продукта по периодам (план выпуска продукта). Метод
        возвращает указатель на одномерный массив рассчитанных объемов незавершенного производства в натуральном
        выражении размером PrCount. ВНИМАНИЕ!!! При вызове данного метода следует помнить о необходимости высвобождения
        памяти и удалении созданного данным методом динамического массива.**/
            decimal *temp = new(nothrow) decimal[PrCount]{};// Выделяем память для вспомогательной переменной
            if(!temp) { return nullptr; };                  // Если память не выделена, то возвращаем "пусто"
//            for(size_t i=sZero; i<PrCount; i++) {           // Заполняем нулями массив
//                *(temp+i) = dZero;
//            };
            decimal *Voltmp = nullptr;
            for(size_t i=duration; i<PrCount; i++) {
                Voltmp = CalcWorkingVolumeItem(PrCount, i, (volume+i)->volume);
                if(Voltmp) {
                    Sum(temp, Voltmp, PrCount);
                    delete[] Voltmp;                        // Удаляем вспомогательный массив
                    Voltmp = nullptr;
                };
            };
            return temp;
        }   // CalcWorkingVolume

        decimal* clsRecipeItem::CalcWorkingValueItem(const size_t _PrCount, const size_t period, \
            const decimal rmprice[], const decimal volume) const {
        /** Метод возвращает одномерный массив сырьевой себестоимости продукта в течении единичного технологического цикла.
        Продукт выпускается в заданный период period. Размерность возвращаемого массива _PrCount. Параметры: _PrCount
        - длительность проекта, period - период выпуска продукта, rmprice[] - 2D-массив с ценами сырья размерностью
        rcount*PrCount, volume - объем выпускаемого продукта в период period. Метод возвращает одномерный массив размерностью
        _PrCount. ВНИМАНИЕ!!! При вызове данного метода следует помнить о необходимости высвобождения памяти и удалении
        созданного данным методом динамического массива. **/
            if(period < duration) { return nullptr; };      // Если период меньше, чем производственный цикл, возврат "пусто"
            if(period >=_PrCount) return nullptr;           // Если период больше или равно длительности проекта, возврат "пусто"
            if(!rmprice) { return nullptr; };               // Проверка существования массива
            decimal *tmp = CalcRawMatVolumeItem(_PrCount, period, volume); // Массив размером rcount*PrCount с объемами сырья
            if(!tmp) { return nullptr; };                           // Если массив пуст, о выход и возврат nullptr
            decimal *temp = Mult(tmp, rmprice, rcount*_PrCount);    // Получаем 2D-массив со стоимостью сырья
            delete[] tmp;                                           // Удаляем массив
            tmp = new(nothrow) decimal[_PrCount]{};                 // Выделяем память для нового массива
            if(!tmp) { delete[] temp; return nullptr; }             // если не получилось, то удаляем массив temp и выходим
//            for(size_t i=sZero; i<_PrCount; i++) {                  // Обнуляем массив
//                *(tmp+i) = dZero;
//            };
            for(size_t i=sZero; i<rcount; i++) {                    // Суммируем строки массива temp и записываеи в массив tmp
                for(size_t j=sZero; j<_PrCount; j++) {
                    *(tmp+j) += *(temp+_PrCount*i+j);
                };
            };
            if(temp) delete[] temp;                                 // Удаляем массив temp
            return tmp;
        }   // CalcWorkingValueItem

        decimal* clsRecipeItem::CalcWorkingValue(const size_t _PrCount, const size_t period, const \
            decimal rmprice[], const decimal volume) const {
        /** Метод возвращает одномерный массив аккумулированной себестоимости продукта в течении единичного технологического
        цикла. Продукт выпускается в заданный период period. Размерность возвращаемого массива _PrCount. Параметры: _PrCount
        - длительность проекта, period - период выпуска продукта, rmprice[] - 2D-массив с ценами сырья размерностью
        rcount*PrCount, volume - объем выпускаемого продуктав период period. ВНИМАНИЕ!!! При вызове данного метода следует
        помнить о необходимости высвобождения памяти и удалении созданного данным методом динамического массива. **/
            if(period < duration) { return nullptr; };      // Если период меньше, чем производственный цикл, возврат "пусто"
            if(period >=_PrCount) return nullptr;           // Проверка допустимости периода
            if(!rmprice) { return nullptr; };               // Проверка существования массива с ценами на сырье
            decimal *tmp = CalcWorkingValueItem(_PrCount, period, rmprice, volume); // Получаем массив с себестоимостью
            if(!tmp) { return nullptr; };                   // Если массив пуст, то выход и возврат nullptr
            decimal S = dZero;                              // Вспомогательная переменная для аккумулирования
            for(size_t i=sZero; i<_PrCount; i++) {          // Аккумулируем массив tmp
                S += *(tmp+i);
                *(tmp+i) = S;
            };
            return tmp;
        }   // CalcWorkingValue

        decimal* clsRecipeItem::CalcWorkingBalanceItem(const size_t _PrCount, const size_t period, \
            const decimal rmprice[], const decimal volume) const {
        /** Метод возвращает одномерный массив аккумулированной себестоимости незавершенного производства в течении единичного
        технологического цикла. Продукт выпускается в заданный период period. Размерность возвращаемого массива _PrCount.
        Параметры: _PrCount - длительность проекта, period - период выпуска продукта, rmprice[] - 2D-массив с ценами сырья
        размерностью rcount*PrCount, volume - объем выпускаемого продукта в период period. ВНИМАНИЕ!!! При вызове данного метода
        следует помнить о необходимости высвобождения памяти и удалении созданного данным методом динамического массива.**/
            if(period < duration) { return nullptr; };      // Если период меньше, чем производственный цикл, возврат "пусто"
            if(period >=_PrCount) return nullptr;           // Проверка допустимости периода
            if(!rmprice) { return nullptr; };               // Проверка существования массива
            decimal *tmp = CalcWorkingValue(_PrCount, period, rmprice, volume); // Получаем массив с акк.себестоимостью ресурсов
            if(!tmp) { return nullptr; };                                       // Если массив пуст, то выход и возврат nullptr
            decimal *wptemp=CalcWorkingVolumeItem(_PrCount, period, volume);    // Получаем массив незаверш.производства
            if(!wptemp) {                                                       // Если массив не получен, то
                delete[] tmp;                                                   // Удаляем массив tmp
                return nullptr;                                                 // Выходим и возвращаем Nullptr
            };
            for(size_t i=sZero; i<_PrCount; i++ ) {
                if(*(wptemp+i) < epsln) {*(tmp+i) = dZero; };   // Если i-й элемент массива wptemp нулевой, то и
            };                                                  // i-й элемент массива tmp тоже делаем нулевым
            delete[] wptemp;
            return tmp;
        }   // CalcWorkingBalanceItem

        decimal* clsRecipeItem::CalcProductBalanceItem(const size_t _PrCount, const size_t period, const \
            decimal rmprice[], const decimal volume) const {
        /** Метод возвращает одномерный массив себестоимости готового продукта, выпущенного в период period. Размерность
        возвращаемого массива _PrCount. Параметры: _PrCount - длительность проекта, period - период выпуска продукта, rmprice[]
        - 2D-массив с ценами сырья размерностью rcount*PrCount, volume - объем выпускаемого продукта в период period. ВНИМАНИЕ!!!
        При вызове данного метода следует помнить о необходимости высвобождения памяти и удалении созданного данным методом
        динамического массива.**/
            if(period < duration) { return nullptr; };      // Если период меньше, чем производственный цикл, возврат "пусто"
            if(period >=_PrCount) return nullptr;           // Проверка допустимости периода
            if(!rmprice) { return nullptr; };               // Проверка существования массива
            decimal *tmp = CalcWorkingValue(_PrCount, period, rmprice, volume); // Получаем массив с акк.себестоимостью
            if(!tmp) { return nullptr; };                                       // Если массив пуст, то выход и возврат nullptr
            for(size_t i=sZero; i<_PrCount; i++ ) {
                if(i != period) *(tmp+i) = dZero;           // Элементы, индекс которых отличен от period обнуляются
            };
            return tmp;
        }   // CalcProductBalanceItem

        inline void clsRecipeItem::clsEraser() {
        /** Метод "обнуляет" все поля экземпляра класса. Используется только в конструкторе перемещения**/
            name = measure = "";
            duration = rcount = sZero;
            rnames = nullptr;
            recipeitem = nullptr;

        }   // clsEraser

        void clsRecipeItem::View_f_Item(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[], const string&\
        _hmcur, decimal* (clsRecipeItem::*f)(const size_t, const size_t, const decimal*, const decimal) const) const {
        /** Служебная функция для реализации визуального контроля. Позволяет реализовать контроль работоспособности
        функциЙ: CalcWorkingValueItem, CalcWorkingValue, CalcWorkingBalanceItem и CalcProductBalanceItem. Используется
        функциями:  ViewWorkingValueItem, ViewWorkingValue, ViewWorkingBalanceItem и ViewProductBalanceItem. Параметры:
        PrCount - число периодов проекта, ProPlan[] - массив c планом выхода готовой продукции размерностью PrCount,
        rmprice[]  - 2D-массив с ценами сырья размерностью rcount*PrCount, где rcount - количество позиций сырья;
        decimal* (clsRecipeItem::*f)(const size_t, const size_t, const decimal*, const decimal) const - указатель на
        одну из функций CalcWorkingValueItem, CalcWorkingValue, CalcWorkingBalanceItem или CalcProductBalanceItem. **/
            strNameMeas pname[PrCount];                             // Формируем массив заголовка для функции ArrPrint
            for(size_t i=sZero; i<PrCount; i++) {
                pname[i].name = to_string(i);                       // Заголовки строк совпадают с номерами периодов проекта
                pname[i].measure = _hmcur;                           // Денежные диницы измерения (валюта)
            };
            decimal *tdata = new(nothrow) decimal[PrCount*PrCount]{};   // Выделяем память под временную переменную и обнуляем массив
            if(!tdata) return;                                          // Если память не выделена, то выход и возврат nullptr
            decimal *temp;                                                      // Другая временная переменная
            for(size_t i=duration; i<PrCount; i++) {
                temp = ((*this).*f)(PrCount, i, rmprice, (ProPlan+i)->volume);  // Вызываем нужную функцию
                if(temp)
                    var_cpy((tdata+PrCount*i), temp, PrCount);      // Копируем во временную переменную
                if(temp) delete[] temp;                             // Удаляем источник копирования
            };
            ArrPrint(PrCount, pname, tdata, PrCount);               // Выводим на экран
            if(tdata) delete[] tdata;                               // Удаляем временную переменную
        }   // View_f_Item

        void clsRecipeItem::View_f_Balance(const size_t _PrCount, const strItem ProPlan[], const decimal rmprice[],
        const string& _hmcur, strItem* (clsRecipeItem::*f)(const size_t, const decimal*, const strItem*) const) const {
        /** Служебная функция для реализации визуального контроля. Позволяет реализовать контроль работоспособности
        функций: CalcWorkingBalance и CalcProductBalance. Используется функциями: ViewWorkingBalance и ViewProductBalance.
        Параметры: PrCount - число периодов проекта, ProPlan[] - массив c планом выхода готовой продукции размерностью
        PrCount, rmprice[]  - 2D-массив с ценами сырья размерностью rcount*PrCount, где rcount - количество позиций сырья;
        strItem* (clsRecipeItem::*f)(const size_t, const decimal*, const strItem*) const - указатель на одну из функций
        CalcWorkingBalance или CalcProductBalance. **/
            strNameMeas pname[sOne] = {name, measure};                  // Заголовки таблицы для вывода в нат. выражении
//            strNameMeas pnamepr[sOne] = {name, _hmcur+'/'+measure};     // Заголовки таблицы для вывода удельной стоимости
//            strNameMeas pnameva[sOne] = {name, _hmcur};                 // Заголовки таблицы для вывода полной стоимости
            strItem *temp = ((*this).*f)(_PrCount, rmprice, ProPlan);   // Вызываем нужную функцию и получаем указатель на массив
            if(!temp) return;                                           // Если массив не существует, то выход
            ArrPrint(sOne, pname, temp, _PrCount, volume, _hmcur);      // Выводим на экран в натуральном выражении
            ArrPrint(sOne, pname, temp, _PrCount, price, _hmcur);       // Выводим на экран цены
            ArrPrint(sOne, pname, temp, _PrCount, value, _hmcur);       // Выводим на экран стоимость
            delete[] temp;                                              // Удаляем временную переменную
        }   // ViewWorkingBalance

        /** public **/

        clsRecipeItem::clsRecipeItem(): name(EmpStr), measure(EmpStr), duration(sZero), rcount(sZero) {
        /** Конструктор без параметров. Удобен при создании экземпляра класса для дальнейшей десериализации данных из файла
        в этот экземпляр, для копирования или перемещения в экземпляр из другого объекта. **/
            rnames = nullptr;
            recipeitem = nullptr;
            #ifdef CDtor_voice      // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                cout << "Empty Ctor clsRecipeItem" << endl;
            #endif // CDtor_voice
        }   // Empty Ctor clsRecipeItem

        clsRecipeItem::clsRecipeItem(const string &_name, const string &_measure, const size_t _duration, const size_t _rcount,\
            const strNameMeas _rnames[], const decimal _recipeitem[])
        /** Конструктор с вводом всех параметров. Параметры: &_name - ссылка на название продукта, &_measure - ссылка на
        наименование единицы измерения натурального объема продукта, _duration - длительность производственного цикла,
        _rcount - количество позиций ресурсов, _rnames[] - указатель на массив с наименованиями ресурсов и единицами измерения
        размером _rcount, _recipeitem[] - указатель на массив одномерный аналог матрицы с рецептурами размерностью
        _rcount*_duration. **/
            : name(_name), measure(_measure), duration(_duration), rcount(_rcount) {
            if( (rcount>sZero) && (_rnames) ) {         // Если количество позиций сырья больше нуля и переданный массив названий
                rnames = new(nothrow) strNameMeas[rcount];  // сырья не пустой, то содаем массив названий сырья размером rcount,
                if(rnames)                              // Если память массиву выделена, то
                    for(size_t i=sZero; i<rcount; i++) {// копируем в созданный массив наименования сырья из входного массива:
                        (rnames+i)->name = (_rnames+i)->name;       // наименование сырья
                        (rnames+i)->measure = (_rnames+i)->measure; // единицу измерения
                    };
            } else rnames = nullptr;                    // Если ограничения на rcount и _rnames не выполняюься, то "пусто"
            if( (duration>sZero) && (_recipeitem) ) {   // Если длительность производственного цикла больше нуля и переданный массив рецептур
                recipeitem = new(nothrow) decimal[rcount*duration];  // не пустой, то создаем массив рецептур размером rcount*duration
                if(recipeitem)                          // Если память массиву выделена, то копируем из входного массива
//                    memcpy(recipeitem, _recipeitem, sizeof(decimal)*rcount*duration);//  рецептуры функцией memcpy
                    var_cpy(recipeitem, _recipeitem, rcount*duration);  // рецептуры
            } else recipeitem = nullptr;                // Если ограничения на duration и _recipeitem не выполняюься, то "пусто"
            #ifdef CDtor_voice                          // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                cout << "Ctor clsRecipeItem" << endl;
            #endif                                      // CDtor_voice
        }   // Ctor clsRecipeItem

        clsRecipeItem::clsRecipeItem(const clsRecipeItem &obj): name(obj.name), measure(obj.measure), \
            duration(obj.duration), rcount(obj.rcount) {
        /** Конструктор копирования **/
            if( (rcount>sZero) && (obj.rnames) ) {
                rnames = new(nothrow) strNameMeas[rcount];  // Выделяем память массиву rnames
                if(rnames)                                  // Если память выделена, то
                    for(size_t i=sZero; i<rcount; i++) {    // копируем в созданный массив наименования сырья из массива
                        (rnames+i)->name = (obj.rnames+i)->name;        // входного объекта,
                        (rnames+i)->measure = (obj.rnames+i)->measure;  // и единицу измерени
                    };
            } else rnames = nullptr;                    // Если ограничения на rcount и _rnames не выполняюься, то "пусто"
            if( (duration>sZero) && (obj.recipeitem) ) {// Если длительность производственного цикла больше нуля и переданный массив рецептур не пустой, то
                recipeitem = new(nothrow) decimal[rcount*duration];   // содаем массив рецептур размером rcount*duration
                if(recipeitem)                          // Если память выделена, то
//                    memcpy(recipeitem, obj.recipeitem, sizeof(decimal)*rcount*duration); // копируем из входного массива рецептур функцией memcpy
                    var_cpy(recipeitem, obj.recipeitem, rcount*duration);
            } else recipeitem = nullptr;                // Если ограничения на duration и _recipeitem не выполняюься, то "пусто"
            #ifdef CDtor_voice                          // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                cout << "Copy Ctor clsRecipeItem" << endl;
            #endif                                      // CDtor_voice
        }   // Copy Ctor clsRecipeItem

        void clsRecipeItem::swap(clsRecipeItem& obj) noexcept {
        /** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
            std::swap(name, obj.name);
            std::swap(measure, obj.measure);
            std::swap(duration, obj.duration);
            std::swap(rcount, obj.rcount);
            std::swap(rnames, obj.rnames);
            std::swap(recipeitem, obj.recipeitem);
            #ifdef CDtor_voice                          // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                cout << "clsRecipeItem::swap" << endl;
            #endif
        }   // swap

        clsRecipeItem::clsRecipeItem(clsRecipeItem &&obj) {
        /** Конструктор перемещения **/
            clsEraser();        // Обнуляем поля экземпляра класса
            swap(obj);          // Обмениваемся состояниями с obj
            #ifdef CDtor_voice                          // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                cout << "Move Ctor clsRecipeItem" << endl;
            #endif                                      // CDtor_voice
        }   // Move Ctor clsRecipeItem

        clsRecipeItem& clsRecipeItem::operator= (const clsRecipeItem &obj) {
        /** Перегрузка оператора присваивания копированием. Реализовано в идеоме
        КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom)  **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsRecipeItem tmp(obj);             // Вызываем конструктор копирования и получаем копию obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef CDtor_voice                  // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                cout << "clsRecipeItem Copy &operator=" << endl;
            #endif                              // CDtor_voice
            return *this;
        }   // clsRecipeItem Copy &operator=

        clsRecipeItem& clsRecipeItem::operator= (clsRecipeItem &&obj) {
        /** Перегрузка оператора присваивания перемещением. Реализовано в идеоме
        ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsRecipeItem tmp(move(obj));       // Вызываем конструктор перемещения и получаем содержание obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef CDtor_voice                  // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                cout << "clsRecipeItem Move &operator=" << endl;
            #endif                              // CDtor_voice
            return *this;
        }   // clsRecipeItem Move &operator=

        clsRecipeItem::~clsRecipeItem() {
        /** Деструктор **/
            if(rnames) delete[] rnames;
            if(recipeitem) delete[] recipeitem;
            #ifdef CDtor_voice                          // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                cout << "Dtor clsRecipeItem" << endl;
            #endif                                      // CDtor_voice
        }   // Dtor clsRecipeItem

        /** Get - методы **/

        strItem* clsRecipeItem::CalcWorkingBalance(const size_t _PrCount, const decimal rmprice[], const strItem ProPlan[]) const {
        /** Метод рассчитывает объем, удельную и полную себестоимость незавершенного производства для конкретного продукта,
        выпускаемого на протяжении всего проекта. Параметры:  _PrCount - число периодов проекта, ProPlan - указатель на массив
        размерности PrCount с объемами производства продукта по периодам (план выпуска продукта), rmprice указатель на матрицу
        с ценами на сырье и материалы, размерностью rcount*PrCount. ВНИМАНИЕ!!! При вызове данного метода следует помнить о
        необходимости высвобождения памяти и удалении созданного данным методом динамического массива. **/
            if(!rmprice) { return nullptr; };                           // Проверка существования массива
            if(!ProPlan) { return nullptr; };                           // Проверка существования массива
            /** Рассчитываем незавершенное производство в натуральном выражении **/
            decimal *tvolume = CalcWorkingVolume(_PrCount, ProPlan);    // Получаем объемы незавершенного пр-ва в нат. выражении
            if(!tvolume) {return nullptr;};                             // Если память не выделена, то выход с nullptr
            /** Рассчитываем незавершенное производство в стоимостном выражении **/
            decimal *tvalue = new(nothrow) decimal[_PrCount]{};         // Выделяем память под временный массив и заполняем нулями
            if(!tvalue) {                                               // Если память не выделена, то
                delete[] tvolume;                                       // Удаляем ранее созданный массив tvolume
                return nullptr;                                         // выходим и возвращаем nullptr
            };
            decimal *temp;                                              // Другая временная переменная
            for(size_t i=duration; i<_PrCount; i++) {
                temp = CalcWorkingBalanceItem(_PrCount, i, rmprice, (ProPlan+i)->volume); // Получаем себестоимость для ЕТЦ
                if(temp) {                                              // Если массив не пустой,то
                    Sum(tvalue, temp, _PrCount);                        // Суммируем все ЕТЦ в массив tvalue
                    delete[] temp;                                      // Удаляем источник суммирования
                };
            };
            /** Формируем выходной массив **/
            strItem *twbalance = new(nothrow) strItem[_PrCount];        // Выделяем память под выходной массив
            if(!twbalance) {                                            // Если память не выделена, то
                delete[] tvolume;                                       // Удаляем ранее созданный массив tvolume
                delete[] tvalue;                                        // Удаляем ранее созданный массив tvalue
                return nullptr;                                         // выходим и возвращаем nullptr
            };
            for(size_t i=sZero; i<_PrCount; i++) {                      // Копируем элементы массивов в выходной массив
                (twbalance+i)->value  = *(tvalue +i);
                (twbalance+i)->volume = (twbalance+i)->value > epsln ? *(tvolume+i) : dZero;
                (twbalance+i)->price = (twbalance+i)->volume > epsln ? (twbalance+i)->value / (twbalance+i)->volume : dZero;
            };
            delete[] tvolume;                                           // Удаляем ранее созданный массив tvolume
            delete[] tvalue;                                            // Суммируем все ЕТЦ в массив tvalue
            return twbalance;
        }   // CalcWorkingBalance

        strItem* clsRecipeItem::CalcProductBalance(const size_t _PrCount, const decimal rmprice[], const strItem ProPlan[]) const {
        /** Метод рассчитывает объем, удельную и полную себестоимость готового продукта. Параметры:  _PrCount - число периодов
        проекта, ProPlan - указатель на массив размерности PrCount с объемами производства продукта по периодам (план выпуска
        продукта), rmprice матрица с ценами на сырье и материалы, размерностью rcount*PrCount. ВНИМАНИЕ!!! При вызове данного
        метода следует помнить о необходимости высвобождения памяти и удалении созданного данным методом динамического
        массива. **/
            if(!rmprice) { return nullptr; };                           // Проверка существования массива
            if(!ProPlan) { return nullptr; };                           // Проверка существования массива
            /** Рассчитываем готовую продукцию в стоимостном выражении **/
            decimal *tvalue = new(nothrow) decimal[_PrCount];           // Выделяем память под временный массив
            if(!tvalue) {                                               // Если память не выделена, то
                return nullptr;                                         // выходим и возвращаем nullptr
            };
            for(size_t i=sZero; i<_PrCount; i++) {                      // Обнуление массива
                *(tvalue+i) = dZero;
            };
            decimal *temp;                                              // Другая временная переменная
            for(size_t i=duration; i<_PrCount; i++) {
                temp = CalcProductBalanceItem(_PrCount, i, rmprice, (ProPlan+i)->volume); // Получаем себестоимость для ЕТЦ
                if(temp) {                                                      // Если массив не пустой,то
                    Sum(tvalue, temp, _PrCount);                                // Суммируем все ЕТЦ в массив tvalue
                    delete[] temp;                                              // Удаляем источник суммирования
                };
            };
            /** Формируем выходной массив **/
            strItem *tpbalance = new(nothrow) strItem[_PrCount];        // Выделяем память под выходной массив
            if(!tpbalance) {                                            // Если память не выделена, то
                delete[] tvalue;                                        // Удаляем ранее созданный массив tvalue
                return nullptr;                                         // выходим и возвращаем nullptr
            };
            for(size_t i=sZero; i<_PrCount; i++) {                      // Копируем элементы массивов в выходной массив
                (tpbalance+i)->value  = *(tvalue +i);                   // Копируем себестоимость
                (tpbalance+i)->volume = (tpbalance+i)->value > epsln ? (ProPlan+i)->volume : dZero; // Копируем объем при
                                                                        // условии, что себестоимость не равна нулю
                (tpbalance+i)->price = (tpbalance+i)->volume > epsln ? (tpbalance+i)->value / (tpbalance+i)->volume : dZero;
            };                                                          // Предотвращаем деление на ноль
            delete[] tvalue;                                            // Удаляем вспомогательный массив
            return tpbalance;
        }   // CalcProductBalance

        decimal* clsRecipeItem::CalcRawMatVolume(const size_t PrCount, const strItem volume[]) const {
        /** Метод рассчитывает и возвращает объем потребления ресурсов в натуральном выражении для всего плана
        выпуска продукта. Параметры:  PrCount - число периодов проекта, volume[] - указатель на массив размерности PrCount с
        объемами производства продукта по периодам (план выпуска продукта). Метод возвращает 2D-массив строки которого
        представляют собой название ресурсов, а столбцы - период проекта. **/
            if(!rnames) { return nullptr; };                // Если отсутствует массив с номенклатурой сырья, то "пусто"
            if(!recipeitem) { return nullptr; };            // Если отсутствует матрица с рецептурами, то "пусто"
            if(!volume) { return nullptr; };                // Если остутствует массив с объемами производства продукта
            size_t tcount = rcount * PrCount;               // Емкость создаваемого массива
            decimal *temp = new(nothrow) decimal[tcount];   // Выделяем память для вспомогательной переменной (3D-массив)
            if(!temp) { return nullptr; };                  // Если память не выделена, то возвращаем "пусто"
            for(size_t i=sZero; i<tcount; i++) {            // Заполняем нулями массив
                *(temp+i) = dZero;
            };
            decimal *GRFtmp = nullptr;
            for(size_t i=duration; i<PrCount; i++) {
                GRFtmp = CalcRawMatVolumeItem(PrCount, i, (volume+i)->volume);
                Sum(temp, GRFtmp, tcount);                  // Формируем 2D-матрицу сырья для всех периодов проекта,
                if(GRFtmp) delete[] GRFtmp;                 // производство продукции. Удаляем вспомогательный массив
                GRFtmp = nullptr;
            };                                              // где возможно производство продукции
            return temp;
        }   // CalcRawMatVolume

        strNameMeas* clsRecipeItem::GetRawNamesItem() const {
        /** Метод возвращает указатель на вновь создаваемый динамический массив с наименованиями ресурсов
        и единицами измерения расхода ресурсов на изготовление единицы продукта. Массив создается как копия и подлежит
        удалению после использования. **/
            if(rcount == sZero) return nullptr;         // Если размер массива с наименованием сырья рулевой, то выход и "пусто"
            strNameMeas *temp = new(nothrow) strNameMeas[rcount];   // Пытаемся выделить память для вспомогательного массива
            if(!temp) return nullptr;                   // Если память не выделена, то выход и возврат "пусто"
            for(size_t i=sZero; i<rcount; i++) {        // Если память выделена, то копируем массивы
                (temp+i)->name = (rnames+i)->name;                  // Наименование сырья
                (temp+i)->measure = (rnames+i)->measure;            // Размерность расхода на продукт
            }
            return temp;
        }   // GetRawNamesItem

        const strNameMeas* clsRecipeItem::GetRefRawNamesItem() const {
        /** Метод возвращает константный указатель на внутренний массив с наименованиями сырья и материалов и единицами
        натурального измерения сырья. **/
            return rnames;
        }   // GetRefRawNamesItem

        decimal* clsRecipeItem::GetRecipeitem() const {
        /** Метод возвращает указатель на вновь создаваемый динамический массив с рецептурами (аналог двумерной матрицы
        размером rcount*duration). Массив создается как копия и подлежит удалению после использования. **/
            if(!recipeitem) return nullptr;                     // Если нет массива рецептур, то выход
            if(!rnames) return nullptr;                         // Если нет массива с наименованием сырья и материалов, то выход
            if(rcount == sZero) return nullptr;                 // Если размер массива с именами сырья нулевой, то выход
            if(duration == sZero) return nullptr;               // Если длительность технологического цикла нулевая, то выход
            const size_t tcount = rcount*duration;
            decimal *temp = new(nothrow) decimal[tcount];       // Выделяем память под массив
            if(!temp) return nullptr;                           // Если память не выделена, то выход и возврат nullptr
            var_cpy(temp, recipeitem, tcount);
            return temp;
        }   // GetRecipeitem

        const decimal* clsRecipeItem::GetRefRecipeitem() const {
        /** Метод возвращает константный указатель на внутренний массив с рецептурами (аналог двумерной матрицы размером
        rcount*duration). **/
            return recipeitem;
        }   // GetRefRecipeitem

        const size_t clsRecipeItem::GetDuration() const { return duration; };   // Возврат длительности производственного цикла
        const size_t clsRecipeItem::GetRCount() const { return rcount; };       // Возврат размера массива rnames (количество позиций сырья)
        const string& clsRecipeItem::GetName() const {return name; };           // Возврат ссылки на наименование продукта
        const string& clsRecipeItem::GetMeasure() const { return measure; };    // Возврат ссылки на наименование единицы измерения продукта

/** --------------------------------- Методы сериализации и десериализации ------------------------------------------------ **/

        bool clsRecipeItem::StF(ofstream &_outF)  {
        /** Метод имплементации записи в файловую переменную текущего экземпляра класса clsRecipeItem (запись в файл, метод
        сериализации). Параметры: &_outF - экземпляр класса ofstream для записи данных. Внимание!!! файл должен быть открыт
        с флагом ios::binary (не текстовый режим!). **/
            if( (duration==sZero) || (rcount==sZero) || (!rnames) || (!recipeitem) ) return false;  // Валидация параметров
            #ifdef CDtor_voice                            // Макрос вывода отладочной информации. Работает, если определен CDtor_voice
                long bpos = _outF.tellp();                // Определяем позицию в начале файла
                cout << "clsRecipeItem::StF begin bpos= " << bpos << endl;
            #endif                                        // CDtor_voice
            if(!SEF(_outF, name)) return false;           // Сохраняем наименование продукта
            if(!SEF(_outF, measure)) return false;        // Сохраняем ед.измерения
            if(!SEF(_outF, duration)) return false;       // Сохраняем длительность производственного цикла
            if(!SEF(_outF, rcount)) return false;         // Сохраняем количество позиций в номенклатуре сырья и материалов для производства продукта
            for(size_t i=sZero; i<rcount; i++) {          // Сохраняем массив с названиями сырья и единицами измерения
                if(!SEF(_outF, (rnames+i)->name)) return false;
                if(!SEF(_outF, (rnames+i)->measure)) return false;
            };
            if(!SEF(_outF, recipeitem, rcount*duration)) return false;    // Сохраняем массив рецкптуры продукта. Размерность rcount*duration
            #ifdef CDtor_voice
                long epos = _outF.tellp();                                // Определяем позицию в конце файла
                cout << "clsRecipeItem::StF end epos= " << epos << endl;
            #endif // CDtor_voice
            return true;                                  // Возвращаем true
        }   // StF

        bool clsRecipeItem::SaveToFile(const string _filename) {
        /** Метод записи текущего экземпляра класса в файл **/
            if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
                                                                // 1.   Создаем экземпляр класса ofstream
            ofstream outF(_filename, ios::out | ios::binary);   // Связываем имя файла с файловым потоком для записи на диск
                                                                // 2.   Проверяем, успешно ли создан файл,
            if (!outF.is_open()) {                              // Если файл не создан, то сообщение пользователю и
                cout << fileopenerror << endl;                  // возврат false и выход из функции
                return false;
            };
            if(!StF(outF)) {                                    // 3.   Записываем данные в файл. Если файл не записан, то
                outF.close();                                   // Закрыаем файл и
                return false;                                   // Выходим с false
            };
            outF.close();                                       // 4.   Закрываем файл
            return true;                                        // 5.   Возвращаем true
        }   // SaveToFile

        bool clsRecipeItem::RfF(ifstream &_inF) {
        /** Метод имплементации чтения из файловой переменной текущего экземпляра класса clsRecipeItem(чтение из файла, метод
        десериализации). Параметры: &_inF - экземпляр класса ifstream для чтения данных. Внимание!!! файл должен быть открыт
        с флагом ios::binary (не текстовый режим!).  **/
            #ifdef CDtor_voice
                long bpos = _inF.tellg();                     // Определяем позицию в начале файла
                cout << "clsRecipeItem::RfF begin bpos= " << bpos << endl;
            #endif // CDtor_voice
            if(rnames) { delete[] rnames; };
            if(recipeitem) { delete[] recipeitem; };
            if(!DSF(_inF, name)) return false;            // Читаем наименование продукта
            if(!DSF(_inF, measure)) return false;         // Читаем ед.измерения
            if(!DSF(_inF, duration)) return false;        // Читаем длительность производственного цикла
            if(!DSF(_inF, rcount)) return false;          // Читаем количество позиций в номенклатуре сырья и материалов для производства продукта
            rnames = new(nothrow) strNameMeas[rcount];          // Выделяем память для массива
            if(rnames) {                                        // Если память выделена, то
                for(size_t i=sZero; i<rcount; i++) {            // Читаем массив с названиями сырья и единицами измерения
                    if(!DSF(_inF, (rnames+i)->name)) return false;
                    if(!DSF(_inF, (rnames+i)->measure)) return false;
                }
            } else return false;
            const size_t tsize = rcount*duration;
            recipeitem = new(nothrow) decimal[tsize];           // Выделяем память для массива
            if(recipeitem) {                                    // Если память выделена, то
                if(!DSF(_inF, recipeitem, tsize)) return false; // Читаем массив с рецкптурами продукта
            } else return false;
            #ifdef CDtor_voice
                long epos = _inF.tellg();                       // Определяем позицию в конце файла
                cout << "clsRecipeItem::RfF end epos= " << epos << endl;
            #endif // CDtor_voice
            return true;
        } // RfF

        bool clsRecipeItem::ReadFromFile(const string _filename) {
        /** Метод чтения из файла и запись в экземпляр класса **/
            if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
                                                                // 1.   Создаем экземпляр класса ifstream
            ifstream inF(_filename, ios::in | ios::binary);     // Связываем имя файла с файловым потоком для чтения с диска
                                                                // 2.   Проверяем, успешно ли открыт файл
            if (!inF.is_open()) {                               // Если файл не открыт, то сообщение пользователю и
                cout << fileopenerror << endl;                  // возврат false и выход из функции
                return false;
            };
            if(!RfF(inF)) {                                     // 3.   Считываем данные из файла. Если файл не записан, то
                inF.close();                                    // Закрываем файл
                return false;                                   // Выход и возврат false
            }
            inF.close();                                        // 4.   Закрываем файл
            return true;
        }   // ReadFromFile

/** -----------------------   Методы визуального контроля функций класса clsRecipeItem   ---------------------------------  **/

        void clsRecipeItem::ViewMainParametrs() const {
        /** Выводит на экран основные параметры класса **/
            cout << "Название продукта                                  " << GetName() << endl;
            cout << "Единица измерения продукта                         " << GetMeasure() << endl;
            cout << "Длительность технологического цикла                " << GetDuration() << endl;
            cout << "Количество позиций сырья и материалов в рецептуре  " << GetRCount() << endl << endl;
        }   // ViewMainParametrs

        void clsRecipeItem::ViewRawNames() const {
        /** Метод визуального контроля работоспособности функции GetRawNamesItem **/
            cout << "Контроль работы метода GetRawNamesItem" << endl << endl;
            strNameMeas* temp = GetRawNamesItem();
            if(temp) {
                v_service(temp, rcount);
                delete[] temp;
            }
        }   // ViewRawNames

        void clsRecipeItem::ViewRefRawNames() const {
        /** Метод визуального контроля работоспособности функции GetRefRawNamesItem **/
            cout << "Контроль работы метода GetRefRawNamesItem" << endl << endl;
            const strNameMeas* temp = GetRefRawNamesItem();
            v_service(temp, rcount);
        }   // ViewRefRawNames

        void clsRecipeItem::ViewRecipeitem() const {
        /** Метод визуального контроля работоспособности функции GetRecipeitem. **/
            cout << "Контроль работы метода GetRecipeitem" << endl;
            cout << "Рецептура продукта " << name << "; Единица измерения продукта " << measure << endl;
            decimal *temp = GetRecipeitem();                        // Получаем массив рецептур
            if(!temp) return;                                       // Если массив пуст, то выход
            strNameMeas *tnames = new(nothrow) strNameMeas[rcount]; // Вспомогательный массив имен
            if(!tnames) {                                           // Если память не выделена, то
                delete[] temp;                                      // Удаляем массив temp
                return;                                             // И выходим из метода
            }
            for(size_t i=sZero; i<rcount; i++) {
                (tnames+i)->name = (rnames+i)->name;                        // Название сырья и материалов
                (tnames+i)->measure = (rnames+i)->measure + "/" + measure;  // Единица измерения удельного расхода на продукт
            };
            ArrPrint(rcount, tnames, temp, duration);   // Выводим на экран
            delete[] temp;                              // Удаляем временный массив
            delete[] tnames;                            // Удаляем временный массив
        }   // ViewRecipeitem

        void clsRecipeItem::ViewRefRecipeitem() const {
        /** Метод визуального контроля работоспособности функции GetRefRecipeitem. **/
            cout << "Контроль работы метода GetRefRecipeitem" << endl;
            cout << "Рецептура продукта " << name << "; Единица измерения продукта " << measure << endl;
            const decimal *temp = GetRefRecipeitem();                // Получаем указатель на массив рецептур
            if(temp && rnames ) {                                   // Если массив не пустой, то
                ArrPrint(rcount, rnames, temp, duration);           // Выводим на экран
            }
        }   // ViewGetRefRecipeitem

        void clsRecipeItem::ViewRawMatVolume(const size_t PrCount, const strItem ProPlan[]) const {
        /** Метод визуального контроля работоспособности функции CalcRawMatVolume **/
            cout << "Контроль работы метода CalcRawMatVolume" << endl;
            cout << "Потребность в сырье и материалах для продукта " << name << endl;
            decimal *temp = CalcRawMatVolume(PrCount, ProPlan);
            ArrPrint(rcount, rnames, temp, PrCount);
            if(temp) delete[] temp;
        }   // ViewRawMatVolume

        void clsRecipeItem::ViewWorkingValueItem(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** Метод визуального контроля работоспособности функции CalcWorkingValueItem. Параметры: PrCount - число периодов
        проекта, ProPlan[] - массив c планом выхода готовой продукции размерностью PrCount, rmprice[]  - 2D-массив с ценами
        сырья размерностью rcount*PrCount, где rcount - количество позиций сырья, hmcur - наименование валюты проекта
        ("RUR", "CNY", "USD", "EUR" и проч.). **/
            cout << "Контроль работы метода CalcWorkingValueItem" << endl;
            cout << "Расход сырья в денежном выражении на продукт " << name \
                 << " в течении единичного технологического цикла" << endl;
            cout << "Строки - единичные технологические циклы, столбцы - расход по периоду" << endl;
            View_f_Item(PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcWorkingValueItem);
        }   // ViewWorkingValueItem

        void clsRecipeItem::ViewWorkingValue(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** Метод визуального контроля работоспособности функции CalcWorkingValue **/
            cout << "Контроль работы метода CalcWorkingValue" << endl;
            cout << "Себестоимость продукта " << name << " в течении единичного технологического цикла" << endl;
            cout << "Строки - единичные технологические циклы, столбцы - аккумулир. расход в периоде" << endl;
            View_f_Item(PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcWorkingValue);
        }   // ViewWorkingValue

        void clsRecipeItem::ViewWorkingBalanceItem(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** Метод визуального контроля работоспособности функции CalcWorkingBalanceItem **/
            cout << "Контроль работы метода CalcWorkingBalanceItem" << endl;
            cout << "Незавершенное производство в денежном выражении продукта " << name \
                 << " в течении единичного технологического цикла" << endl;
            cout << "Строки - единичные технологические циклы, столбцы - незавершенное производство в периоде" << endl;
            View_f_Item(PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcWorkingBalanceItem);
        }   // ViewWorkingBalanceItem

        void clsRecipeItem::ViewWorkingBalance(const size_t _PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** Метод визуального контроля работоспособности функции CalcWorkingBalance **/
            cout << "Контроль работы метода CalcWorkingBalance" << endl;
            cout << "Незавершенное производство в натуральном, удельном и полном стоимостном выражении" << endl;
            View_f_Balance(_PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcWorkingBalance);
        }   // ViewWorkingBalance

        void clsRecipeItem::ViewProductBalanceItem(const size_t PrCount, const strItem ProPlan[], const decimal rmprice[],\
        const string& hmcur) const {
        /** Метод визуального контроля работоспособности функции CalcProductBalanceItem **/
            cout << "Контроль работы метода CalcProductBalanceItem" << endl;
            cout << "Себестоимость готового продукта " << name << " в течении единичного технологического цикла" << endl;
            cout << "Строки - единичные технологические циклы, столбцы - себестоимость готового продукта в периоде" << endl;
            View_f_Item(PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcProductBalanceItem);
        }   // ViewProductBalanceItem

        void clsRecipeItem::ViewProductBalance(const size_t _PrCount, const strItem ProPlan[], const decimal rmprice[],
        const string& hmcur) const {
        /** Метод визуального контроля функции CalcProductBalance **/
            cout << "Контроль работы метода CalcProductBalance" << endl;
            cout << "Готовый продукт в натуральном, удельном и полном стоимостном выражении" << endl;
            View_f_Balance(_PrCount, ProPlan, rmprice, hmcur, &clsRecipeItem::CalcProductBalance);
        }   // ViewProductBalance

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                             class clsManufactItem                                                   **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

/** Класс - производство отдельного продукта. Хранит данные о потребности в сырье и материалах для выпуска продукта в заданных
объемах по заданному графику, цены на сырье и материалы, объемы, полную и удельную себестоимость изготовленного продукта,
Балансовую полную и удельную стоимость незаконченного производства. **/

        void clsManufactItem::clsEraser() {
        /** Метод "обнуляет" все поля класса. Используется только в конструкторе перемещения. **/
            PrCount = sZero;
            name = nullptr;
            measure = nullptr;
            duration = sZero;
            rcount = sZero;
            Recipe = nullptr;
            ProductPlan = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice = nullptr;
            Balance = nullptr;
        }   // clsEraser

        clsManufactItem::clsManufactItem() {
        /** Конструктор без параметром **/
            PrCount = sZero;
            name = nullptr;
            measure = nullptr;
            duration = sZero;
            rcount = sZero;
            Recipe = nullptr;
            ProductPlan = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice = nullptr;
            Balance = nullptr;
            #ifdef CDtor_voice
                cout << "Empty Ctor clsManufactItem" << endl;
            #endif // CDtor_voice
        }   // clsManufactItem Конструктор без параметром

        clsManufactItem::clsManufactItem(const size_t _PrCount, const clsRecipeItem &obj) {
        /** Конструктор с параметрами: _PrCount - количество периодов проекта, obj - const ссылка на объект типа
            clsRecipeItem. Объект obj копируется. **/
            PrCount = _PrCount;
            Recipe = new(nothrow) clsRecipeItem(obj);
            if(Recipe) {
                name = &Recipe->GetName();
                measure = &Recipe->GetMeasure();
            } else {
                name = nullptr;
                measure = nullptr;
            };
            duration = Recipe->GetDuration();
            rcount = Recipe->GetRCount();
            ProductPlan = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice = nullptr;
            Balance = nullptr;
            #ifdef CDtor_voice
                cout << "Ctor clsManufactItem with const clsRecipeItem &obj in param" << endl;
            #endif // CDtor_voice
        }   // clsManufactItem(const size_t _PrCount, clsRecipeItem &&obj)

        clsManufactItem::clsManufactItem(const size_t _PrCount, clsRecipeItem &&obj) {
        /** Конструктор с параметрами: _PrCount - количество периодов проекта, obj - rvalue ссылка на объект типа
        clsRecipeItem. Объект obj перемещается. **/
            PrCount = _PrCount;
            Recipe = new(nothrow) clsRecipeItem(move(obj));
            if(Recipe) {
                name = &Recipe->GetName();
                measure = &Recipe->GetMeasure();
            } else {
                name = nullptr;
                measure = nullptr;
            };
            duration = Recipe->GetDuration();
            rcount = Recipe->GetRCount();
            ProductPlan = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice = nullptr;
            Balance = nullptr;
            #ifdef CDtor_voice
                cout << "Ctor clsManufactItem with clsRecipeItem &&obj in param" << endl;
            #endif // CDtor_voice
        }   // clsManufactItem(const size_t _PrCount, clsRecipeItem &&obj)

        clsManufactItem::clsManufactItem(const size_t _PrCount, const string &_name, const string &_measure, \
            const size_t _duration, const size_t _rcount, const strNameMeas _rnames[], const decimal _recipeitem[]) {
        /** Конструктор с параметрами, создает класс рецептур **/
        PrCount = _PrCount;
        duration = _duration;
        rcount = _rcount;
        Recipe = new(nothrow) clsRecipeItem(_name, _measure, duration, rcount, _rnames, _recipeitem);
        if(Recipe) {
            name = &Recipe->GetName();
            measure = &Recipe->GetMeasure();
        } else {
            name = nullptr;
            measure = nullptr;
        };
        ProductPlan  = nullptr;
        RawMatPurchPlan = nullptr;
        RawMatPrice  = nullptr;
        Balance = nullptr;
        #ifdef CDtor_voice
            cout << "Ctor clsManufactItem with parameters" << endl;
        #endif // CDtor_voice
        }   // Ctor clsManufactItem

        clsManufactItem::clsManufactItem(const clsManufactItem &obj) {
        /** Конструктор копирования **/
            PrCount = obj.PrCount;
            duration = obj.duration;
            rcount = obj.rcount;
            if(obj.Recipe) {
                Recipe = new(nothrow) clsRecipeItem( *(obj.Recipe) );       // Конструирование объекта копированием
                if(Recipe) {
                    name = &Recipe->GetName();
                    measure = &Recipe->GetMeasure();
                } else {
                    name = nullptr;
                    measure = nullptr;
                }
            } else {
                Recipe = nullptr;
                name = nullptr;
                measure = nullptr;
            };
            if(obj.ProductPlan) {
                ProductPlan = new(nothrow) strItem[PrCount];
                if(ProductPlan)
                    var_cpy(ProductPlan, obj.ProductPlan, PrCount);     // Копирование массивов
            } else { ProductPlan = nullptr; };
            const size_t temp = rcount*PrCount;                         // Вспомогательная переменная
            if(obj.RawMatPurchPlan) {
                RawMatPurchPlan = new(nothrow) decimal[temp];
                if(RawMatPurchPlan)
                    var_cpy(RawMatPurchPlan, obj.RawMatPurchPlan, temp);
                } else {  RawMatPurchPlan = nullptr;}
            if(obj.RawMatPrice) {
                RawMatPrice = new(nothrow) decimal[temp];
                if(RawMatPrice)
                    var_cpy(RawMatPrice, obj.RawMatPrice, temp);        // Копирование массивов
            } else { RawMatPrice = nullptr; };
            if(obj.Balance) {
                Balance = new(nothrow) strItem[PrCount];
                if(Balance)
                    var_cpy(Balance, obj.Balance, PrCount);
            } else { Balance = nullptr; }
            #ifdef CDtor_voice
                cout << "Copy Ctor clsManufactItem" << endl;
            #endif // CDtor_voice
        }   // Copy Ctor clsManufactItem

        void clsManufactItem::swap(clsManufactItem& obj) noexcept {
        /** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/
            std::swap(PrCount, obj.PrCount);
            std::swap(duration, obj.duration);
            std::swap(rcount, obj.rcount);
            std::swap(name, obj.name);
            std::swap(measure, obj.measure);
            std::swap(Recipe, obj.Recipe);
            std::swap(ProductPlan, obj.ProductPlan);
            std::swap(RawMatPurchPlan, obj.RawMatPurchPlan);
            std::swap(RawMatPrice, obj.RawMatPrice);
            std::swap(Balance, obj.Balance);
            #ifdef CDtor_voice    // Макрос вывода отладочной информации.
                cout << "clsManufactItem swap" << endl;
            #endif
        }   // swap

        clsManufactItem::clsManufactItem(clsManufactItem &&obj) {
        /** Конструктор перемещения. После перемещения переменная obj не удаляется, только ее поля обнуляются либо
        указывают на nullptr. **/
            clsEraser();    // Сбрасываем состояние текущего объекта this
            swap(obj);      // Обмениваемся состояниями с obj
            #ifdef CDtor_voice
                cout << "Move Ctor clsManufactItem" << endl;
            #endif // CDtor_voice
        }   // Move Ctor clsManufactItem

        clsManufactItem& clsManufactItem::operator= (const clsManufactItem &obj) {
        /** Перегрузка оператора присваивания копированием. Реализовано в идеоме
        КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsManufactItem tmp(obj);           // Вызываем конструктор копирования и получаем копию obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef CDtor_voice
                cout << "clsManufactItem Copy &operator=" << endl;
            #endif // CDtor_voice
            return *this;
        }   // clsManufactItem Copy &operator=

        clsManufactItem& clsManufactItem::operator= (clsManufactItem &&obj) {
        /** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
        (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsManufactItem tmp(move(obj));     // Вызываем конструктор перемещения и получаем содержание obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef CDtor_voice
                cout << "clsManufactItem Move &operator=" << endl;
            #endif // CDtor_voice
            return *this;
        }   // clsManufactItem Move &operator=

        bool clsManufactItem::operator == (const string &Rightname) const {
        /** Переопределение оператора сравнения для поиска экземпляра объекта по наименованию продукта **/
            return (*name == Rightname);
        }   // operator ==

        clsManufactItem::~clsManufactItem() {
            if(Recipe) {
                #ifdef CDtor_voice
                    cout << "delete Recipe" << endl;
                #endif // CDtor_voice
                delete Recipe; };
            if(ProductPlan) {
                #ifdef CDtor_voice
                    cout << "delete[] ProductPlan" << endl;
                #endif // CDtor_voice
                delete[] ProductPlan; };
            if(RawMatPurchPlan) {
                #ifdef CDtor_voice
                    cout << "delete[] RawMatPurchPlan" << endl;
                #endif // CDtor_voice
                delete[] RawMatPurchPlan; };
            if(RawMatPrice) {
                #ifdef CDtor_voice
                    cout << "delete[] RawMatPrice" << endl;
                #endif // CDtor_voice
                delete[] RawMatPrice; };
            if(Balance) {
                #ifdef CDtor_voice
                    cout << "delete[] Balance" << endl;
                #endif // CDtor_voice
                delete[] Balance; };
            #ifdef CDtor_voice
                cout << "Dtor clsManufactItem" << endl;
            #endif // CDtor_voice
        }   // Dtor clsManufactItem

        /** Get - методы **/

        const size_t& clsManufactItem::GetPrCount() const { return PrCount; }   // Возвращает const-ссылку на количество периодов
        const size_t& clsManufactItem::GetRCount() const { return rcount; }     // Возвращает const-ссылку на количество позиций сырья, участвующего в производстве
        const size_t& clsManufactItem::GetDuration() const { return duration; } // Возвращает длительность производственного цикла

        const string* clsManufactItem::GetName() const {
        /** Метод возвращает константный указатель на наименование продукта. **/
            if(!name) return nullptr;
            return name;
        }   // GetName

        const string* clsManufactItem::GetMeasure() const {
        /** Метод возвращает константный указатель на единицу измерения натурального объема продукта. **/
            if(!measure) return nullptr;
            return measure;
        }   // GetMeasure

        const decimal* clsManufactItem::GetRefRecipe() const {
        /** Метод возвращает константный указатель на внутренний массив с рецептурами. **/
            if(!Recipe) return nullptr;
            return Recipe->GetRefRecipeitem();
        }   // GetRefRecipe

        const strNameMeas* clsManufactItem::GetRefRawNames() const {
        /** Метод возвращает константный указатель на внутренний массив с наименованиями ресурсов и
        единицами натурального измерения ресурсов. **/
            if(!Recipe) return nullptr;
            return Recipe->GetRefRawNamesItem();
        }   // GetRefRawNames

        const decimal* clsManufactItem::GetRawMatPurchPlan() const {
        /** Метод возвращает константный указатель на массив RawMatPurchPlan с объемом потребления ресурсов в
        натуральном выражении для всего плана выпуска продукта **/
            if(!RawMatPurchPlan) return nullptr;        // Если массив не существует, то выход и возврат nullptr
            return RawMatPurchPlan;
        } // GetRawMatPurchPlan

        const decimal* clsManufactItem::GetRawMatPrice() const {
        /** Метод возвращает константный указатель на массив RawMatPrice с ценами на ресурсы. **/
            if(!RawMatPrice) return nullptr;    // Если массив не существует, то выход и возврат nullptr
            return RawMatPrice;
        }   // GetRawMatPrice

        const strItem* clsManufactItem::GetBalance() const {
        /** Метод возвращает константный указатель на массив с объемом, удельной и полной себестоимостью незавершенного
        производства для конкретного продукта, выпускаемого на протяжении всего проекта. **/
            return Balance;
        }   // GetBalance

        const strItem* clsManufactItem::GetProductPlan() const {
        /** Метод возвращает константный указатель на массив с объемом, удельной и полной себестоимостью готовой продукции для
        конкретного продукта, выпускаемого на протяжении всего проекта. **/
            return ProductPlan;
        }   // GetProductPlan

        /** Set - методы **/

        bool clsManufactItem::SetProductPlan(const strItem _ProductPlan[]) {
        /** Метод ввода плана выпуска продукта (объем выпуска в натуральном выражении и график выпуска).
        Параметры: _ProductPlan - массив типа strItem размерностью PrCount. Используются только поля _Product.volume.  **/
            if((PrCount==sZero) || (!_ProductPlan)) return false;   // Валидация параметров
            strItem* temp = new(nothrow) strItem[PrCount];          // Выделяем память под временный массив
            if(!temp) return false;                                 // Если память не выделена, то выход и возврат false
            var_cpy(temp, _ProductPlan, PrCount);                   // Копируем массивы
            std::swap(temp, ProductPlan);                           // Обмениваем указатели массивов
            if(temp) delete[] temp;                                 // Удаляем вспомогательный массив, если он не пустой
            return true;
        }   // SetProductPlan

        bool clsManufactItem::SetRawMatPrice(const decimal _Price[]) {
        /** Метод ввода цен на сырье и материалы. Предполагается, что после получения складом информации о потребности
        в сырье и материалах, склад возвращает информацию об учетных ценах на сырье и материалы. Эта информация с помощью
        данного метода копируется в массив RawMatPrice размером rcount*PrCount. Параметры: _Price - указатель на массив
        (матрицу) цен на сырье и материалы размером rcount*PrCount. **/
            const size_t tcount = rcount*PrCount;               // Вычисляем размер массива (аналог двумерной матрицы)
            if((tcount==sZero) || (!_Price)) return false;      // Валидация параметров
            decimal* temp = new(nothrow) decimal[tcount];       // Выделяем память под массив
            if(!temp) return false;                             // Если память не выделена, то выход и возврат false
            var_cpy(temp, _Price, tcount);                      // Копируем массивы
            std::swap(temp, RawMatPrice);                       // Обмениваем указатели массивов
            if(temp) delete[] temp;                             // Удаляем вспомогательный массив, если он не пустой
            return true;
        }   // SetRawMatPrice

        bool clsManufactItem::MoveRawMatPrice(decimal _Price[]) {
        /** Метод ввода цен на сырье и материалы. Предполагается, что после получения складом информации о потребности
        в сырье и материалах, склад возвращает информацию об учетных ценах на сырье и материалы. Эта информация с помощью
        данного метода перемещается в массив RawMatPrice размером rcount*PrCount. Параметры: _Price - указатель на массив
        (матрицу) цен на сырье и материалы размером rcount*PrCount. По окончании работы указатель _Price принимает
        значение nullptr. **/
            if(!_Price) return false;               // Валидация входного массива
            if(RawMatPrice) delete[] RawMatPrice;   // Если уже есть массив с ценами в экземпляре класса, то удаляем его
            RawMatPrice = _Price;                   // Перебрасываем ссылки
            _Price = nullptr;
            return true;
        }   // MoveRawMatPrice

        /** Вычислительные методы **/

        bool clsManufactItem::Resize(size_t _n) {
        /** Функция изменяет размеры массивов, устанавливая новое число периодов проекта, равное _n. При _n < PrCount,
        данные обрезаются, при _n > PrCount, - добавляются новые элементы массивов с нулевыми значениями. Функция
        возвращает true при удачном изменении размеров массивов, false - в противном случае **/
            if(!ProductPlan) return false;          // Нет массива для корректировки
            if(_n < sOne) { return false; };        // Некорректный размер
            if(_n == PrCount) { return true; };     // Изменение размера не производится
            strItem *p;                             // Временный указатель для массива ProductPlan
            p = new(nothrow) strItem[_n];           // Выделяем память массиву
            if(!p) return false;                    // Если память не выделена, то выход с false
            if(_n < PrCount)                        // Если новое число периодов меньше старого, то
                for(size_t i{}; i<_n; i++)
                    *(p+i) = *(ProductPlan+i);
            else {
                for(size_t i{}; i<PrCount; i++)
                    *(p+i) = *(ProductPlan+i);
                for(size_t i=PrCount; i<_n; i++) {
                    (p+i)->volume = dZero;
                    (p+i)->price  = dZero;
                    (p+i)->value  = dZero;
                }
            }
            delete[] ProductPlan;
            if (Balance) delete[] Balance;
            if (RawMatPurchPlan) delete[] RawMatPurchPlan;
            if (RawMatPrice) delete[] RawMatPrice;
            ProductPlan = p;
            Balance     = nullptr;
            RawMatPurchPlan = nullptr;
            RawMatPrice     = nullptr;
            PrCount = _n;
            return true;
        }   // Resize

        void clsManufactItem::CalcRawMatPurchPlan() {
        /** Метод рассчитывает объем потребления сырья и материалов в натуральном выражении для всего плана
        выпуска продукта и заполняет массив RawMatPurchPlan. **/
            if(PrCount>sZero)
                RawMatPurchPlan = Recipe->CalcRawMatVolume(PrCount, ProductPlan);
        }   // CalcRawMatPurchPlan

        bool clsManufactItem::CalculateItem() {
        /** Метод рассчитывает объем, удельную и полную себестоимость незавершенного производства и готовой продукции
        для конкретного продукта, выпускаемого на протяжении всего проекта. **/
            if(!ProductPlan) return false;      // Если нет массива с планом выхода готовой продукции, то выход и false
            if(!RawMatPurchPlan) return false;  // Если нет массива с планом закупок сырья и материалов, то выход и false
            if(!RawMatPrice) return false;      // Если нет массива с ценами сырья и материалов, то выход и false
            strItem* tmpBal = Recipe->CalcWorkingBalance(PrCount,RawMatPrice, ProductPlan); // Получаем баланс незавер. про-ва
            if(!tmpBal) return false;           // Если массив пуст, то выход и false
            std::swap(tmpBal, Balance);         // Обмениваем указатели
            if(tmpBal) delete[] tmpBal;         // Удаляем вспомогательный массив, если он не пустой
            strItem* temp = Recipe->CalcProductBalance(PrCount, RawMatPrice, ProductPlan);  // Получаем готовую продукцию
            if(!temp) return false;             // Если массив пуст, то выход и false
            std::swap(temp, ProductPlan);       // Обмениваем указатели
            if(temp) delete[] temp;             // Удаляем вспомогательный массив, если он не пустой
            return true;
        }   // CalculateItem

/** --------------------------------- Методы сериализации и десериализации ------------------------------------------------ **/

        bool clsManufactItem::StF(ofstream &_outF) {
        /** Метод имплементации записи в файловую переменную текущего экземпляра класса clsManufactItem (запись в файл, метод
        сериализации). Параметры: &_outF - экземпляр класса ofstream для записи данных **/
            if( (PrCount==sZero) || (duration==sZero) || (rcount==sZero) || (!Recipe) ) return false;
            #ifdef CDtor_voice
                long bpos = _outF.tellp();                                      // Определяем позицию в начале файла (отладка)
                cout << "clsManufactItem::StF begin bpos= " << bpos << endl;    // Выводим позицию на экран (отладка)
            #endif // CDtor_voice
            /** Флаги состояния динамических массивов. Если флаг true, то соответствующий массив записывается в файл, если
            нет - то не записывается. Флаги сохраняются в файле и помогают при чтении данных из файла определять, есть там
            массив и его надо считывать, или его нет. **/
            bool flagProductPlan, flagRawMatPurchPlan, flagRawMatPrice, flagBalance;
            if(ProductPlan) flagProductPlan         = true; else flagProductPlan     = false;
            if(RawMatPurchPlan) flagRawMatPurchPlan = true; else flagRawMatPurchPlan = false;
            if(RawMatPrice) flagRawMatPrice         = true; else flagRawMatPrice     = false;
            if(Balance) flagBalance                 = true; else flagBalance         = false;
            #ifdef CDtor_voice
                cout << "flagProductPlan= " << flagProductPlan << endl;
                cout << "flagRawMatPurchPlan= " << flagRawMatPurchPlan << endl;
                cout << "flagRawMatPrice= " << flagRawMatPrice << endl;
                cout << "flagBalance= " << flagBalance << endl;
            #endif // CDtor_voice
            /** Сохраняем флаги в файл **/
            if(!SEF(_outF, flagProductPlan)) return false;
            if(!SEF(_outF, flagRawMatPurchPlan)) return false;
            if(!SEF(_outF, flagRawMatPrice)) return false;
            if(!SEF(_outF, flagBalance)) return false;
            /** Сохраняем в файл основные данные **/
            if(!SEF(_outF, PrCount)) return false;  // Сохраняем в файл Количество периодов проекта
            if(!Recipe->StF(_outF)) return false;   // Если не удалось сохранить объект, то выход и возврат false
            if(flagProductPlan)
                for(size_t i{}; i<PrCount; i++) {
                    if(!(ProductPlan+i)->StF(_outF)) return false;
                };
            size_t tcount = rcount*PrCount;
            if(flagRawMatPurchPlan)
                if(!SEF(_outF, RawMatPurchPlan, tcount)) return false;  // Сохраняем в файл план закупок сырья в нат. выражении
            if(flagRawMatPrice)
                if(!SEF(_outF, RawMatPrice, tcount)) return false;      // Сохраняем в файл цены на сырье и материалы
            if(flagBalance)
                for(size_t i{}; i<PrCount; i++) {
                    if(!(Balance+i)->StF(_outF)) return false;
                };
                                                                        // удельной и полной стоимости незав.производства
            #ifdef CDtor_voice
                long epos = _outF.tellp();                                  // Определяем позицию в конце файла (отладка)
                cout << "clsManufactItem::StF end epos= " << epos << endl;  // Выводим позицию на экран (отладка)
            #endif // CDtor_voice
            return true;
        }   // StF

        bool clsManufactItem::SaveToFile(const string _filename) {
        /** Метод записи текущего экземпляра класса clsManufactItem в файл **/
            if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
                                                                // 1.   Создаем экземпляр класса ofstream
            ofstream outF(_filename, ios::out | ios::binary);   // Связываем имя файла с файловым потоком для записи на диск
                                                                // 2.   Проверяем, успешно ли создан файл,
            if (!outF.is_open()) {                              // Если файл не создан, то сообщение пользователю и
                cout << "Ошибка открытия файла" <<endl;         // возврат false и выход из функции
                return false;
            };
            if(!StF(outF)) {
                outF.close();
                return false;
            }                                          // 3.   Записываем данные в файл
            #ifdef CDtor_voice
                long epos = outF.tellp();
                cout << "clsManufactItem::SaveToFile end epos= " << epos << endl;
            #endif // CDtor_voice
            outF.close();                                       // 4.   Закрываем файл
            return true;                                        // 5.   Возвращаем true
        }   // SaveToFile

        bool clsManufactItem::RfF(ifstream &_inF) {
        /** Метод имплементации чтения из файловой переменной текущего экземпляра класса clsManufactItem (чтение из файла,
        метод десериализации). Параметры: &_inF - экземпляр класса ifstream для чтения данных **/
            #ifdef CDtor_voice
                long bpos = _inF.tellg();                                       // Определяем позицию в начале файла (отладка)
                cout << "clsManufactItem::RfF begin bpos= " << bpos << endl;    // Выводим позицию на экран (отладка)
            #endif // CDtor_voice
            /** Флаги состояния динамических массивов. Если флаг true, то соответствующий массив читается из файла, если
            нет - то не читается. Флаги записаны в файле и помогают при чтении данных из файла определять, есть там
            массив и его надо считывать, или его нет. **/
            bool flagProductPlan, flagRawMatPurchPlan, flagRawMatPrice, flagBalance;
            /** Читаем флаги из файла **/
            if(!DSF(_inF, flagProductPlan)) return false;
            if(!DSF(_inF, flagRawMatPurchPlan)) return false;
            if(!DSF(_inF, flagRawMatPrice)) return false;
            if(!DSF(_inF, flagBalance)) return false;
            #ifdef CDtor_voice
                cout << "flagProductPlan= " << flagProductPlan << endl;
                cout << "flagRawMatPurchPlan= " << flagRawMatPurchPlan << endl;
                cout << "flagRawMatPrice= " << flagRawMatPrice << endl;
                cout << "flagBalance= " << flagBalance << endl;
            #endif // CDtor_voice
            if(!DSF(_inF, PrCount)) return false;       // Читаем из файла Количество периодов проекта
            if(!Recipe) Recipe = new(nothrow) clsRecipeItem(EmpStr, EmpStr, sZero, sZero, nullptr, nullptr);
            if(!Recipe) return false;
            if(!Recipe->RfF(_inF)) return false;    // Если не удалось прочитать объект, то выход и возврат false
            name = &Recipe->GetName();
            measure = &Recipe->GetMeasure();
            rcount = Recipe->GetRCount();
            duration = Recipe->GetDuration();
            if(flagProductPlan) {                                   // Если флаг true, то массив надо прочитать из файла
                strItem* PPtmp = new(nothrow) strItem[PrCount];     // Выделяем память для временного массива
                if(!PPtmp) return false;                            // Если память не выделена, то выход и возврат false
                for(size_t i{}; i<PrCount; i++)                     // Если память выделена, то читаем массив из файла
                    if(!(PPtmp+i)->RfF(_inF)) return false;         // поэлементно
                std::swap(PPtmp, ProductPlan);                      // Обмениваемся указателями
                if(PPtmp) delete[] PPtmp;                           // Удаляем вспомогательный массив
            } else ProductPlan = nullptr;
            size_t tcount = rcount*PrCount;                         // ПОлучаем размер массива
            if(flagRawMatPurchPlan) {                               // Если флаг true, то массив надо прочитать из файла
                decimal* RMPPtmp = new(nothrow) decimal[tcount];    // Выделяем память для временного массива
                if(!RMPPtmp) return false;                          // Если память не выделена, то выход и возврат false
                if(!DSF(_inF, RMPPtmp, tcount)) return false;       // Если память выделена, то читаем их файла массив
                std::swap(RMPPtmp, RawMatPurchPlan);                // Обмениваемся указателями
                if(RMPPtmp) delete[] RMPPtmp;                       // Удаляем вспомогательный массив
            } else RawMatPurchPlan = nullptr;
            if(flagRawMatPrice) {                                   // Если флаг true, то массив надо прочитать из файла
                decimal* RMprcTmp = new(nothrow) decimal[tcount];   // Выделяем память для временного массива
                if(!RMprcTmp) return false;                         // Если память не выделена, то выход и возврат false
                if(!DSF(_inF, RMprcTmp, tcount)) return false;      // Если память выделена, то читаем их файла массив
                std::swap(RMprcTmp, RawMatPrice);                   // Обмениваемся указателями
                if(RMprcTmp) delete[] RMprcTmp;                     // Удаляем вспомогательный массив
            } else RawMatPrice = nullptr;
            if(flagBalance) {                                       // Если флаг true, то массив надо прочитать из файла
                strItem* tmpBal = new(nothrow) strItem[PrCount];    // выделяем память для массива
                if(!tmpBal) return false;                           // Если память не выделена, то выход
                for(size_t i{}; i<PrCount; i++)                     // Если память выделена, то читаем массив из файла
                    if(!(tmpBal+i)->RfF(_inF)) return false;        // поэлементно
                std::swap(tmpBal, Balance);                         // Обмениваемся указателями
                if(tmpBal) delete[] tmpBal;                         // Удаляем вспомогательный массив
            } else Balance = nullptr;

            #ifdef CDtor_voice
                long epos = _inF.tellg();                     // Определяем позицию в начале файла
                cout << "clsManufactItem::RfF end epos= " << epos << endl;
            #endif // CDtor_voice
            return true;
        }   // RfF

        bool clsManufactItem::ReadFromFile(const string _filename) {
        /** Метод чтения из файла и запись в экземпляр класса clsManufactItem **/
            if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
                                                                // 1.   Создаем экземпляр класса ifstream
            ifstream inF(_filename, ios::in | ios::binary);     // Связываем имя файла с файловым потоком для чтения с диска
                                                                // 2.   Проверяем, успешно ли открыт файл
            if (!inF.is_open()) {                               // Если файл не открыт, то сообщение пользователю и
                cout << "Ошибка открытия файла" <<endl;         // возврат false и выход из функции
                return false;
            };
            if(!RfF(inF)) {                                     // 3.   Считываем данные из файла
                inF.close();
                return false;
            };
            #ifdef CDtor_voice
                long epos = inF.tellg();
                cout << "clsManufactItem::ReadFromFile end epos= " << epos << endl;
            #endif // CDtor_voice
            inF.close();                // 4.   Закрываем файл
            return true;
        }   // ReadFromFile

/** -----------------------   Методы визуального контроля функций класса clsManufactItem   -------------------------------  **/

        void clsManufactItem::ViewMainParametrs() const {
        /** Метод визуального контроля функций GetPrCount, GetName, GetMeasure, GetDuration, GetRCount **/
            cout << endl;
            cout << "Длительность проекта                               " << GetPrCount() << endl;
            const string* temp = GetName();
            if(temp)
                cout << "Название продукта                                  " << *temp << endl;
            temp = GetMeasure();
            if(temp)
                cout << "Единица измерения продукта                         " << *temp << endl;
            cout << "Длительность технологического цикла                " << GetDuration() << endl;
            cout << "Количество позиций сырья и материалов в рецептуре  " << GetRCount() << endl << endl;
        }   // ViewMainParametrs

        void clsManufactItem::ViewRefRecipe() const {
        /** Метод визуального контроля функций GetRefRecipe и GetRefRawNames **/
            if(name) {
                cout << "Контроль работы методов GetRefRecipe и GetRefRawNames" << endl;
                cout << "Рецептура продукта " << *name << " в расчете на " << *measure << endl; // Заголовок
            };
            const decimal *trecipe = GetRefRecipe();            // Получаем указатель на массив рецептур
            const strNameMeas *trnames = GetRefRawNames();      // Получаем указатель на массив с названиями сырья и ед.измерения
            if(trecipe && trnames ) {                           // Если массив не пустой, то
                ArrPrint(Recipe->GetRCount(), trnames, trecipe, Recipe->GetDuration()); // Выводим на экран
            };
        }   // ViewRefRecipe

        void clsManufactItem::ViewRawMatPurchPlan() const {
        /** Метод визуального контроля работоспособности функций GetRCount, GetRefRawNames, GetPrCount,
        CalcRawMatPurchPlan и GetRawMatPurchPlan. **/
            cout << "Контроль работы методов GetRCount, GetRefRawNames, GetPrCount, " << \
                "CalcRawMatPurchPlan и GetRawMatPurchPlan." << endl;
            cout << "Потребность в сырье и материалах для продукта " << *name << endl;
            ArrPrint(GetRCount(), GetRefRawNames(), GetRawMatPurchPlan(), GetPrCount());
        }   // ViewRawMatPurchPlan

        void clsManufactItem::ViewRawMatPrice(const string& hmcur) const {
        /** Метод визуального контроля работоспособности функций GetRCount, GetRefRawNames, GetPrCount,
        SetRawMatPrice и GetRawMatPrice. **/
            cout << "Контроль работы методов GetRCount, GetRefRawNames, GetPrCount, " << \
                "SetRawMatPrice и GetRawMatPrice." << endl;
            cout << "Цены на сырье и материалах для продукта " << *name << " в " << hmcur << " за ед." << endl;
            ArrPrint(GetRCount(), GetRefRawNames(), GetRawMatPrice(), GetPrCount());
        }   // ViewRawMatPrice

        void clsManufactItem::ViewCalculate(const string& hmcur) const {
        /** Метод визуального контроля работоспособности функций GetBalance и GetProductPlan. **/
            cout << "Контроль работы методов GetBalance и GetProductPlan" << endl << endl;
            cout << "Незавершенное производство в натуральном, удельном и полном стоимостном выражении" << endl;
            string tname = *(name);                             // Временная переменная для ограничения длины строки
            tname.resize(15);                                   // Обрезаем строку
            strNameMeas pname[sOne] = {tname, *measure};        // Формируем заголовки таблицы
            const strItem *Btemp = GetBalance();                // Получаем указатель на массив незавершенного производства
            if(Btemp) {                                         // Если массив не пуст, то печатаем отчет
                ArrPrint(sOne, pname, Btemp, PrCount, volume, hmcur);  // Печать натуральных показателей
                ArrPrint(sOne, pname, Btemp, PrCount, price, hmcur);   // Печать удельнх стоимостных показателей
                ArrPrint(sOne, pname, Btemp, PrCount, value, hmcur);   // Печать полных стоимостных показателей
            };
            cout << "Готовое производство в натуральном, удельном и полном стоимостном выражении" << endl;
            const strItem *Ptemp = GetProductPlan();            // Получаем указатель на массив готового производства
            if(Ptemp) {                                         // Если массив не пуст, то печатаем отчет
                ArrPrint(sOne, pname, Ptemp, PrCount, volume, hmcur);  // Печать натуральных показателей
                ArrPrint(sOne, pname, Ptemp, PrCount, price, hmcur);   // Печать удельнх стоимостных показателей
                ArrPrint(sOne, pname, Ptemp, PrCount, value, hmcur);   // Печать полных стоимостных показателей
            }
        }   // ViewCalculate

/*************************************************************************************************************************/
/**                                                                                                                     **/
/**                                             class clsManufactory                                                    **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

/** Класс - производство всех необходимых для проекта продуктов. Позволяет осуществить расчет потребности в сырье и материалах
для выпуска продуктов в заданных объемах по заданному графику, рассчитать себестоимость изготовленных продуктов, рассчитать
объемы, балансовую стоимость и удельную стоимость незаконченного производства каждого продукта. При наличии производственной
зависимости продуктов друг от друга (когда вместо вырья в рецептуры вводятся те же самые продукты) позволяет оценить
реализуемость такой зависимости. **/

        inline bool clsManufactory::Checkrnames(const size_t _rcount, const strNameMeas _rnames[]) {
        /** Метод проводит поиск имен из массива _rnames в массиве RMNames. В случае наличия в массиве _rnames хотя бы
        одного имени, отсутствующего в RMNames, выдает false. Если несовпадающих имен нет, то возвращает true. **/
            if(!_rnames) { return false; };                 // Если массив _rnames пуст, то выход и возврат false
            if(!RMNames) { return false; };                 // Если массив RMNames пуст, то выход и возврат false
            if(RMCount == sZero) { return false; };         // Если счетчик имен в полном массиве сырья нулевой, то выход и false
            if(_rcount == sZero) { return false; };         // Если счетчик в массие _rnames нулевой, то выход и false
            size_t coincidence = sZero;                     // Устанавливаем счетчик в нуль
            for(size_t i=sZero; i<_rcount; i++) {           // Цикл по всем именам в _rnames
                for(size_t j=sZero; j<RMCount; j++) {       // Цикл по всем именами в RMNames
                    if( (_rnames+i)->name == (RMNames+j)->name ) {    // Ищем совпадение имен, если находим то
                        coincidence++;                      // увеличиваем счетчик на единицу
                        break;                              // и прерываем цикл по j
                    }
                }
            };
            if(coincidence == _rcount) { return true; }     // При совпадении всех имен из массива _rnames возвращаем true
            else { return false; }                          // Если хотя бы одно имя из _rnames не находится в RMNames, false
        }   // Checkrnames

        strItem* clsManufactory::gettotal(const strItem* (clsManufactItem::*f)() const) const {
        /** Метод взвращает указатель на одномерный массив, являющийся аналогом двумерной матрицы с балансами незавершенного
        производства (при подстановки вместо f метода GetBalance) или планами выхода всех продуктов (при подстановки вместо
        f метода GetProductPlan) в натуральном, удельном и полном стоимостном выражении. Размер матрицы Manuf.size()*PrCount.
        Каждый элемент матрицы имеет тип strItem, т.е. имеет в своем составе значения volume, price и value для продукта.
        Массив создается в динамической памяти, после использования требует явного удаления с помощью оператора delete[]. **/
            if(Manuf.size() == sZero) return nullptr;   // Если вектор не содержит ни одного единичного производства, то выход с nullptr
            if(PrCount == sZero) return nullptr;        // Если число периодов нулевое, то выход с nullptr
            strItem *temp = new(nothrow) strItem[Manuf.size()*PrCount]; // Пытаемся выделить память
            if(!temp) return nullptr;                   // Если память не выделена, то выход с nullptr
            size_t i = sZero;                           // Вспомогательный счётчик
            const strItem *tmp;                         // Вспомогательная переменная
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++) {  // Цикл по продуктам
                tmp = ((*it).*f)();
                if(tmp) {
                    var_cpy((temp+PrCount*i), tmp, PrCount);   // Копируем одномерный i-й баланс
                    i++;                                        // в строку временного массива
                }
            };
            return temp;
        }   // gettotal

        clsManufactory::clsManufactory() {
        /** Пустой конструктор. Используетя в методе emplace_back при десериализации данных из файла. **/
            PrCount = sZero;
            RMCount = sZero;
            hmcur = RUR;
            RMNames = nullptr;
            pshell = nullptr;
            #ifdef CDtor_voice
                cout << "Empty Ctor clsManufactory" << endl;
            #endif // CDtor_voice
        }   // Emplty Ctor clsManufactory

        clsManufactory::clsManufactory(const size_t _PrCount, const size_t _RMCount, const strNameMeas _RMNames[], \
            const size_t msize):PrCount(_PrCount), RMCount(_RMCount), hmcur(RUR) {
        /** Конструктор с параметрами: _PrCount - количество периодов проекта, _RMCount - полное количество позиций сырья и
        материалов, _RMNames - полный массив с названиями сырья и материалов, msize - полное количество наименований
        продуктов. **/
            if(_RMNames && (_RMCount>sZero)) {              // Если массив _RMNames существует и _RMCount>0, то
                RMNames = new(nothrow) strNameMeas[RMCount];// Выделяем память под массив со списком наименований сырья и материалов
                if(RMNames)                                 // Если память выделена, то
                    for(size_t i=sZero; i<RMCount; i++) {   // Заполняем массив значениями из массива _RMNames
                        *(RMNames+i) = *(_RMNames+i);       // Нельзя использовать memcpy, т.к. strNameMeas - это тип со строками
                    }
                else { RMCount = sZero; };
            } else { RMNames = nullptr; RMCount = sZero; };
            Manuf.reserve(msize); /** Чтобы при добавлении нового единичного производства в вектор не происходило перемещение
            уже записанных в вектор элементов и для ускорения работы программы, желательно сразу зарезервировать память на
            всю номенклатуру готовой продукции. https://pro-prof.com/forums/topic/эффективное-использование-vector-в-c **/
            #ifdef CDtor_voice
                cout << "Ctor clsManufactory with parameters" << endl;
            #endif // CDtor_voice
        }   // clsManufactory

        clsManufactory::clsManufactory(const clsManufactory& obj) {
        /** Конструктор копирования **/
            PrCount = obj.PrCount;
            RMCount = obj.RMCount;
            hmcur = obj.hmcur;
            if(obj.RMNames) { RMNames = new(nothrow) strNameMeas[RMCount]; } else RMNames = nullptr;
            if(RMNames) { var_cpy(RMNames, obj.RMNames, RMCount); };
            Manuf = obj.Manuf;
            pshell = obj.pshell;
            #ifdef CDtor_voice
                cout << "Copy clsManufactory Ctor " << endl;
            #endif // CDtor_voice
        }   // Copy Ctor

        void clsManufactory::swap(clsManufactory& obj) noexcept {
        /** Функция обмена значениями между объектами **/
            std::swap(PrCount, obj.PrCount);    // Обмен значениями
            std::swap(RMCount, obj.RMCount);    // Обмен значениями
            std::swap(hmcur, obj.hmcur);        // Обмен значениями
            std::swap(RMNames, obj.RMNames);    // Обмен указателями
            std::swap(pshell, obj.pshell);      // Обмен указателями
            Manuf.swap(obj.Manuf);              // Обмен векторами
            #ifdef CDtor_voice
                cout << "clsManufactory::swap" << endl;
            #endif // CDtor_voice
        }   // swap

        clsManufactory::clsManufactory(clsManufactory&& obj) {
        /** Конструктор перемещения **/
            PrCount = sZero;
            RMCount = sZero;
            hmcur = RUR;
            RMNames = nullptr;
            pshell = nullptr;
            swap(obj);
            #ifdef CDtor_voice
                cout << "Move clsManufactory Ctor" << endl;
            #endif // CDtor_voice
        }   // Move Ctor

        clsManufactory& clsManufactory::operator=(const clsManufactory& obj) {
        /** Перегрузка оператора присваивания копированием. Реализовано в идеоме
        КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsManufactory tmp(obj);            // Вызываем конструктор копирования и получаем копию obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef CDtor_voice
                cout << "clsManufactory Copy operator=" << endl;
            #endif // CDtor_voice
            return *this;
        }   // Copy operator=

        clsManufactory& clsManufactory::operator=(clsManufactory&& obj) {
        /** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
        (move-and-swap idiom) **/
            if(this == &obj) { return *this; }; // Если объекты идентичны, то ничего не делаем
            clsManufactory tmp(move(obj));      // Вызываем конструктор перемещения и получаем содержание obj в переменной tmp
            swap(tmp);                          // Обмениваемся содержанием с tmp
            #ifdef CDtor_voice
                cout << "clsManufactory Move operator=" << endl;
            #endif // CDtor_voice
            return *this;
        }   // Move operator=

        clsManufactory::~clsManufactory() {
        /** Деструктор **/
            if(RMNames) delete[] RMNames;
            #ifdef CDtor_voice
                cout << "Dtor clsManufactory" << endl;
            #endif // CDtor_voice
        }   // ~clsManufactory

        /** Set - методы **/

        void clsManufactory::Set_progress_shell(clsProgress_shell<type_progress>* val) {
        /** Функция присваивает указателю pshell адрес объекта val **/
            pshell = val;
        }   // Set_progress_shell

        void clsManufactory::SetCurrency(const Currency& _cur) {
        /** Устанавливаем основную валюту проекта **/
             hmcur = _cur;
        }   // Устанавливаем основную валюту проекта

        bool clsManufactory::SetManufItem(const string &_name, const string &_measure, const size_t _duration, \
            const size_t _rcount, const strNameMeas _rnames[], const decimal _recipe[], const strItem _pplan[]) {
        /** Метод создания производства для конкретного продукта. Создает новый экземпляр класса clsManufactItem, добавляет его
        в вектор, вводит список сырья и материалов и рецептуры в созданное производство. Параметры: _name - наименование продукта,
        _measure - наименование единицы измерения, _duration - длительность производственного цикла, _rcount - число позиций
        сырья и материалов, участвующих в производстве данного продукта, _rnames - массив с наименованиями сырья, размерностью
        rcount, являющихся подмножеством наименований из полного массива с названиями сырья и материалов, _recipe - указатель на
        2D-массив с рецептурами, размером rcount*duration, _pplan - указатель на массив размером PrCount с планом выпуска
        продукта. **/
            if( (_duration==sZero) || (_rcount==sZero) || (!_rnames) || (!_recipe) || (!_pplan) )   // Валидация параметров
                return false;
            if(!Checkrnames(_rcount, _rnames)) { return false; }; // Проверяем совпадение имен сырья и материалов большому списку
            vector <clsManufactItem>::iterator it;                // Итератор для массива
            it = find(Manuf.begin(), Manuf.end(), _name);   // Поиск объекта по имени (ищем совпадение названий продукта)
            if(it != Manuf.end()) { return false; };        // Если объект с таким именем уже есть, то выход и возврат false
            Manuf.emplace_back(PrCount, _name, _measure, _duration, _rcount, _rnames, _recipe); /** Если имя уникально, то
            создаем производство для данного продукта: создаем экземпляр класса clsManufactItem в элементе вектора **/
            Manuf.back().SetProductPlan(_pplan);    // Вводим план выпуска продукта в только что добавленный элемент вектора
            return true;
        }   // SetManufItem

        bool clsManufactory::SetManufItem(const clsRecipeItem &obj, const strItem _pplan[]) {
        /** Метод создания производства для конкретного продукта. Создает новый экземпляр класса clsManufactItem,
        добавляет его в вектор. Параметры: obj - копируемый экземпляр класса clsRecipeItem, _pplan - указатель
        на массив размером PrCount с планом выпуска продукта **/
            if( (!_pplan) ) return false;                                               // Валидация параметров
            if(!Checkrnames(obj.GetRCount(), obj.GetRefRawNamesItem()) )
                return false;  // Валидация параметров
            const string tname = obj.GetName();             // Временная константа
            vector <clsManufactItem>::iterator it;          // Итератор для массива
            it = find(Manuf.begin(), Manuf.end(), tname);   // Поиск объекта по имени (ищем совпадение названий продукта)
            if(it != Manuf.end()) { return false; };        // Если объект с таким именем уже есть, то выход и возврат false
            Manuf.emplace_back(PrCount, obj);               /** Если имя уникально, то создаем производство для данного
            продукта: создаем экземпляр класса clsManufactItem в элементе вектора **/
            Manuf.back().SetProductPlan(_pplan);    // Вводим план выпуска продукта в только что добавленный элемент вектора
            return true;
        }   // SetManufItem

        bool clsManufactory::SetManufItem(clsRecipeItem &&obj, const strItem _pplan[]) {
        /** Метод создания производства для конкретного продукта. Создает новый экземпляр класса clsManufactItem, добавляет его
        в вектор. Параметры: obj - перемещаемый экземпляр класса clsRecipeItem, _pplan - указатель на массив размером PrCount с
        планом выпуска продукта **/
            if( (!_pplan) ) return false;                                               // Валидация параметров
            if(!Checkrnames(obj.GetRCount(), obj.GetRefRawNamesItem()) ) return false;  // Валидация параметров
            const string tname = obj.GetName();             // Временная константа
            vector <clsManufactItem>::iterator it;          // Итератор для массива
            it = find(Manuf.begin(), Manuf.end(), tname);   // Поиск объекта по имени (ищем совпадение названий продукта)
            if(it != Manuf.end()) { return false; };        // Если объект с таким именем уже есть, то выход и возврат false
            Manuf.emplace_back(PrCount, move(obj));         /** Если имя уникально, то создаем производство для данного
            продукта: создаем экземпляр класса clsManufactItem в элементе вектора **/
            Manuf.back().SetProductPlan(_pplan);    // Вводим план выпуска продукта в только что добавленный элемент вектора
            return true;
        }   // SetManufItem

        bool clsManufactory::SetManufItem(const clsManufactItem &obj) {
        /** Метод создания производства для конкретного продукта. Создает новый экземпляр класса clsManufactItem,
        добавляет его в вектор. Параметры: obj - копируемый экземпляр класса clsManufactItem. Создан 2024.08.10 **/
            const string tname = *(obj.GetName());          // Временная константа
            vector <clsManufactItem>::iterator it;          // Итератор для массива
            it = find(Manuf.begin(), Manuf.end(), tname);   // Поиск объекта по имени (ищем совпадение названий продукта)
            if(it != Manuf.end()) { return false; };        // Если объект с таким именем уже есть, то выход и возврат false
            Manuf.emplace_back(obj);                        /** Если имя уникально, то создаем производство для данного
            продукта: создаем экземпляр класса clsManufactItem в элементе вектора копированием obj **/
            return true;
        }   // SetManufItem

        bool clsManufactory::SetManufItem(clsManufactItem &&obj) {
        /** Метод создания производства для конкретного продукта. Создает новый экземпляр класса clsManufactItem,
        добавляет его в вектор. Параметры: obj - перемещаемый экземпляр класса clsManufactItem. Создан 2024.08.10 **/
            const string tname = *(obj.GetName());          // Временная константа
            vector <clsManufactItem>::iterator it;          // Итератор для массива
            it = find(Manuf.begin(), Manuf.end(), tname);   // Поиск объекта по имени (ищем совпадение названий продукта)
            if(it != Manuf.end()) { return false; };        // Если объект с таким именем уже есть, то выход и возврат false
            Manuf.emplace_back(move(obj));                  /** Если имя уникально, то создаем производство для данного
            продукта: создаем экземпляр класса clsManufactItem в элементе вектора копированием obj **/
            return true;
        }   // SetManufItem

        bool clsManufactory::SetProdPlan(const strItem _ProdPlan[]) {
            /** Метод вводит план выпуска продуктов в производство. Параметры: _ProdPlan - указатель на полный массив
            с планом выпуска всех продуктов, размером Manuf.size()*PrCount **/
            if( (PrCount == sZero) || (RMCount == sZero) || (!RMNames) || (Manuf.size() == sZero) ) return false;
            vector<clsManufactItem>::iterator it;           // Итератор для массива
            size_t i = sZero;                               // Вспомогательный счетчик
            for(it=Manuf.begin(); it!=Manuf.end(); it++) {  // Для каждого элемента вектора
                it->SetProductPlan(_ProdPlan+PrCount*i);    // добавляем соответствующую строку плана продаж
                i++;
            };
            return true;
        }   // SetProdPlan

        /** Сервисные методы **/

        bool clsManufactory::IncreaseCapacity(size_t msize) {
        /** Метод увеличивает память вектора для эффектвного добавления новых элементов. Однако сам метод вызывает копирование
        и удаление всех элементов вектора, что весьма затратно. Лучше не менять размер вектора с момента его использования. **/
            size_t current_capacity = Manuf.capacity();     // Получаем текущий объем памяти
            if(msize>current_capacity) {                    // Если новый объем больше текущего, то
                Manuf.reserve(msize);                       // Увеличиваем память
                return true;                                // и возвращаем true
            } else return false;                            // Если нет, - ничего не делаем и выходим с false
        }   // IncreaseCapacity

        /** Вычислительные методы. Приводятся в очередности вызова **/

        void clsManufactory::CalcRawMatPurchPlan(size_t bg, size_t en) {
        /** Метод рассчитывает объем потребления ресурсов в натуральном выражении в соответствии с планом
            выпуска для каждого продукта в диапазоне: от продукта с индексом bg до продукта с индексом en-1. **/
            size_t lim = Manuf.size();                  // Верхняя граница опустимого значения индекса
            if(bg>=lim) return;                         // Валидация нижней границы индекса
            if(en>=lim) en = lim;                       // Валидация верхней границы индекса
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // Сбрасываем счетчик
            for(size_t i = bg; i<en; i++) {
                clsManufactItem* p = (Manuf.data()+i);  // Вспомогательный указатель уснанавливаем на элемент вектора
                if(p->GetPrCount() != PrCount)          // Если длительности проекта не совпадают, то
                    p->Resize(PrCount);                 // изменяем массивы и длительность
                p->CalcRawMatPurchPlan();               // Выполняем расчет для каждого элемента вектора
                (pshell) ? pshell->Counter_inc() : (void)([](){return;});           // Вызываем счетчик
            }
        }   // CalcRawMatPurchPlan(size_t bg, size_t en)

        void clsManufactory::CalcRawMatPurchPlan() {
        /** Метод рассчитывает объем потребления сырья и материалов в натуральном выражении для всего плана
        выпуска всех продуктов. **/
            size_t lim = Manuf.size();                  // Верхняя граница опустимого значения индекса
            for(size_t i{}; i<lim; i++) {
                clsManufactItem* p = (Manuf.data()+i);  // Вспомогательный указатель уснанавливаем на элемент вектора
                if(p->GetPrCount() != PrCount)          // Если длительности проекта не совпадают, то
                    p->Resize(PrCount);                 // изменяем массивы и длительность
                p->CalcRawMatPurchPlan();               // Выполняем расчет для каждого элемента вектора
                (pshell) ? pshell->Update((int)i) : (void)([](){return;});          // Вызываем индикатор прогресса
            }
            (pshell) ? pshell->Update((int)lim) : (void)([](){return;});   // Индикатор на 100%
        }   // CalcRawMatPurchPlan

        void clsManufactory::CalcRawMatPurchPlan_future() {
        /** Метод рассчитывает объем потребления сырья и материалов в натуральном выражении для всего плана
        выпуска всех продуктов. Все расчеты запускаются в асинхронных потоках, число которых на единицу
        меньше числа ядер компьютера. **/
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // Определение макс. числа потоков через число ядер
            if(maxthreads < sTwo) {                                         // Если макс. число потоков менее двух, то
                CalcRawMatPurchPlan();                                      // проводим вычисления методом CalcRawMatPurchPlan()
                return;                                                     // и выходим
            };
            vector<std::future<void>> pool;             // Пул потоков в виде вектора
            pool.reserve(maxthreads);                   // Резервируем память вектору потоков
            size_t stocksize = Manuf.size();            // Получаем размер массива с единичными прозводствами
            size_t ncap = (stocksize/maxthreads)+sOne;  // Количество ед.производств для каждого потока
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // Сбрасываем счетчик
            for(size_t i{}; i<maxthreads; i++) {        // Цикл по всему пулу потоков
                size_t bg = i*ncap;                     // Определяем нижнюю границу индекса
                if(bg>=stocksize) break;                // Если нижняя граница больше или равна числу ед.пр-в, выходим из цикла
                size_t en = (i+sOne)*ncap;              // Определяем верхнюю границу индекса
                pool.push_back(std::async([](clsManufactory* pobj, size_t _bg, size_t _en) {    // Создаем поток и запускаем вычисления
                    #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                        mu1.lock();                          // Блокируем ресурс cout для монопольного использования
                        cout << "Async thread #" << std::this_thread::get_id() << " has been started" << endl;
                        mu1.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                    #endif
                    pobj->CalcRawMatPurchPlan(_bg, _en);    // Вызываем метод с вычислениями и выходим
                    #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                        mu1.lock();                          // Блокируем ресурс cout для монопольного использования
                        cout << "Async thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                        mu1.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                    #endif
                }, this, bg, en));                          // После лямбды стоят аргументы для нее: this, bg, en
            };
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;});   // Вызываем отрисовку индикатора
        }   // CalcRawMatPurchPlan_future

        void clsManufactory::CalcRawMatPurchPlan_thread() {
        /** Метод рассчитывает объем потребления сырья и материалов в натуральном выражении для всего плана
        выпуска всех продуктов. Все расчеты запускаются в потоках, число которых на единицу
        меньше числа ядер компьютера. **/
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // Определение макс. числа потоков через число ядер
            if(maxthreads < sTwo) {                                         // Если макс. число потоков менее двух, то
                CalcRawMatPurchPlan();                                      // проводим вычисления методом CalcRawMatPurchPlan()
                return;                                                     // и выходим
            };
            vector<std::thread> pool;                   // Пул потоков в виде вектора
            pool.reserve(maxthreads);                   // Резервируем память вектору потоков
            size_t stocksize = Manuf.size();            // Получаем размер массива с единичными прозводствами
            size_t ncap = (stocksize/maxthreads)+sOne;  // Количество ед.производств для каждого потока
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // Сбрасываем счетчик
            for(size_t i{}; i<maxthreads; i++) {        // Цикл по всему пулу потоков
                size_t bg = i*ncap;                     // Определяем нижнюю границу индекса
                if(bg>=stocksize) break;                // Если нижняя граница больше или равна числу ед.пр-в, выходим из цикла
                size_t en = (i+sOne)*ncap;              // Определяем верхнюю границу индекса
                pool.emplace_back([](clsManufactory* pobj, size_t _bg, size_t _en){ // Создаем поток и запускаем вычисления
                    #ifdef DEBUG_THREADS                // Макрос вывода отладочной информации. Работает, если определен DEBUG
                        mu1.lock();                      // Блокируем ресурс cout для монопольного использования
                        cout << "Thread #" << std::this_thread::get_id() << " has been started" << endl;
                        mu1.unlock();                    // Разблокируем ресурс cout для использования другими объектами
                    #endif
                    pobj->CalcRawMatPurchPlan(_bg, _en);// Вызываем метод с вычислениями и выходим
                    #ifdef DEBUG_THREADS                // Макрос вывода отладочной информации. Работает, если определен DEBUG
                        mu1.lock();                      // Блокируем ресурс cout для монопольного использования
                        cout << "Thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                        mu1.unlock();                    // Разблокируем ресурс cout для использования другими объектами
                    #endif
                }, this, bg, en);                       // После лямбды стоят аргументы для нее: this, bg, en
            };
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;}); // Вызываем отрисовку индикатора
            for(auto &t : pool)                         // Цикл по всему пулу потоков
                t.join();                               // Ожидаем завершения каждого запущенного потока
        }   // CalcRawMatPurchPlan_thread

        decimal* clsManufactory::GetRawMatPurchPlan() const {
        /** Вычислительный метод возвращает указатель на вновь создаваемый массив размером RMCount*PrCount, в котором содержится
        потребность для каждого наименования сырья в каждом периоде проекта. Потребность в сырье суммарная, по всем единичным
        производствам всех продуктов. Метод const. **/
            if(Manuf.size() == sZero) { return nullptr; };              // Если в векторе нет элементов
            decimal *Rtemp = new(nothrow) decimal[RMCount*PrCount]{};   // Выделям память под вспомогательный массив и обнуляем его
            if(!Rtemp) { return nullptr; };                             // Если память не выделена, то выход и возврат nullptr
            for(vector<clsManufactItem>::const_iterator cit = Manuf.cbegin(); cit < Manuf.cend(); cit++) {
                const decimal *rtemp = cit->GetRawMatPurchPlan();       // Получаем указатель на массив с планом закупок
                if(rtemp) {
                    const size_t _rcount = cit->GetRCount();            // Узнаем число сырья и материалов
                    const strNameMeas *rnames = cit->GetRefRawNames();  // Получаем массив с наименованием сырья и материалов
                    if(rnames) {
                        for(size_t j=sZero; j<_rcount; j++) {
                            for(size_t k=sZero; k<RMCount; k++) {
                                if( (rnames+j)->name == (RMNames+k)->name ) {  // Сравниваем имена, если они совпадают, то
                                    // прибавляем к k-строке массива Rtemp j-ю строку массива rtemp
                                    Sum((Rtemp+PrCount*k), (rtemp+PrCount*j), PrCount);
                                    break;
                                }
                            }   // for k
                        }   // for j
                    }   // if(rnames)
                }   // if(rtemp)
            }   // Цикл по всем элементам вектора
            return Rtemp;
        }   //  GetRawMatPurchPlan

        strItem* clsManufactory::GetRMPurchPlan() const {
        /** Вычислительный метод возвращает указатель на вновь создаваемый массив размером RMCount*PrCount, в котором
        содержится потребность для каждого наименования сырья в каждом периоде проекта. Потребность в сырье суммарная,
        по всем единичным производствам всех продуктов. Тип массива - strItem, заполнены данными только поля volume;
        поля price и value нулевые. **/
            decimal* temp = GetRawMatPurchPlan();       // Получаем указатель на создаваемый массив с планом закупок
            if(!temp) return nullptr;                   // Если массив не сущетвует, то возвращаем nullptr
            strItem *Rtemp = new(nothrow) strItem[RMCount*PrCount];     // Выделям память под вспомогательный массив
            if(!Rtemp) {                                                // Если память не выделена, то
                delete[] temp;                                          // удаляем вспомогательный массив с планом закупок
                return nullptr;                                         // выходим и возвращаем nullptr
            };
            for(size_t i{}; i<(RMCount*PrCount); i++) {                 // Копируем все элемены массива temp
                (Rtemp+i)->volume = *(temp+i);                          // в поля массива Rtemp
                (Rtemp+i)->price = dZero;                               // остальные поля
                (Rtemp+i)->value = dZero;                               // обнуляем
            };
            delete[] temp;                              // удаляем вспомогательный массив с планом закупок
            return Rtemp;                               // и возвращаем указатель на strItem-массив с планом закупок
        }   // GetRMPurchPlan

        bool clsManufactory::SetRawMatPrice(const decimal _Price[]) {
        /** Метод ввода цен на сырье и материалы. Предполагается, что после получения складом информации о
        потребности в сырье и материалах, склад возвращает информацию об учетных ценах на сырье и материалы.
        Данный метод вызывает одноименные методы каждого элемента вектора, которые записывают информацию о ценах
        в массивы RawMatPrice каждого единичного производства. Параметры: _Price - матрица цен на сырье и материалы
        размером RMCount*PrCount. **/
            if(!_Price) return false;                                   // Валидация входного массива
            if(Manuf.size() == sZero) return false;                     // Проверка вектора на остутствие элементов
            bool check = true;                                          // Индикатор успешности операции
            for(vector<clsManufactItem>::iterator it = Manuf.begin(); it!=Manuf.end(); it++) { // Цикл по элементам вектора
                const size_t _rcount = it->GetRCount();                 // Узнаем число сырья и материалов в элементе вектора
                decimal *rtemp =  new(nothrow) decimal[_rcount*PrCount];// Создаем массив для ввода цен в элемент вектора
                if(rtemp) {                                             // Если массив создан, то
                    const strNameMeas *rnames = it->GetRefRawNames();   // Получаем константный указатель на внутренний массив
                                                                        // с наименованием сырья и материалов элемента
                    if(rnames) {                                        // Если указатель не пуст, то
                        for(size_t j=sZero; j<_rcount; j++) {           // Заполняем массив для ввода цен ценами тех позиций
                            for(size_t k=sZero; k<RMCount; k++) {       // сырья, которые присутствуют в рецептуре элемента
                                if( (rnames+j)->name == (RMNames+k)->name ) { // Сравниваем имена, если они совпадают, то
                                    // копируем k-строку с ценами из общего массива цен в j-строку временного массив цен
                                    var_cpy((rtemp+PrCount*j), (_Price+PrCount*k), PrCount);
                                    break;                              // и досрочно завершаем цмкл по k
                                }
                            }   // for k
                        }   // for j
                        check = check && it->MoveRawMatPrice(rtemp); // Перемещаем временный массив цен в элемент вектора
                        // и изменяем Индикатор успешности операции
                    };  // Если используется функция clsManufactItem::SetRawMatPrice, то в следующих строках необходимо
                        // удалить временный массив rtemp и присвоить ему значение Nullptr
                        // delete[] rtemp;
                        // rtemp = nullptr;
                };  // if(rtemp)
            };
            return check;   // Если хоть один элемент не заполнен ценами, то check==false
        }   // SetRawMatPrice

        bool clsManufactory::Calculate(size_t bg, size_t en) {
        /** Метод рассчитывает объем, удельную и полную себестоимость незавершенного производства и готовой продукции
        для продуктов в диапазоне от bg до en-1, выпускаемых на протяжении всего проекта. Метод поочередно вызывает
        методы CalculateItem для каждого единичного производства; формирует массивы продуктов и баланса незавершенного
        производства для каждого продукта из заданного диапазона **/
            size_t lim = Manuf.size();                      // Верхняя граница опустимого значения индекса
            if(lim == sZero) return false;                  // Проверка вектора на остутствие элементов
            if(bg>=lim) return false;                       // Валидация нижней границы индекса
            if(en>=lim) en = lim;                           // Валидация верхней границы индекса
            bool check = true;                              // Индикатор успешности операции
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // Сбрасываем счетчик
            for(size_t i = bg; i<en; i++)  {                 // Цикл по всем единичным производствам
                check = check && (Manuf.data()+i)->CalculateItem(); // Расчет незаверш. пр-ва и готовой продукции в i-элементе
                (pshell) ? pshell->Counter_inc() : (void)([](){return;});           // Вызываем счетчик
            }
            return check;
        }   // Calculate(size_t, size_t)

        bool clsManufactory::Calculate() {
        /** Метод рассчитывает объем, удельную и полную себестоимость незавершенного производства и готовой продукции
        для всех продуктов, выпускаемых на протяжении всего проекта. Метод поочередно вызывает методы CalculateItem для
        каждого единичного производства; формирует массивы продуктов и баланса незавершенного производства для каждого
        продукта **/
            size_t lim = Manuf.size();                      // Верхняя граница опустимого значения индекса
            bool check = true;                              // Индикатор успешности операции
            for(size_t i{}; i<lim; i++)  {                  // Цикл по всем единичным производствам
                check = check && (Manuf.data()+i)->CalculateItem(); // Расчет незаверш. пр-ва и готовой продукции в i-элементе
                (pshell) ? pshell->Update((int)i) : (void)([](){return;});  // Вызываем индикатор прогресса
            }
            (pshell) ? pshell->Update((int)lim) : (void)([](){return;});    // Индикатор на 100%
            return check;
        }   // Calculate

        bool clsManufactory::Calculate_future() {
        /** Метод рассчитывает объем, удельную и полную себестоимость незавершенного производства и готовой продукции
        для всех продуктов, выпускаемых на протяжении всего проекта. Метод поочередно вызывает методы CalculateItem для
        каждого единичного производства; формирует массивы продуктов и баланса незавершенного производства для каждого
        продукта. Все расчеты запускаются в асинхронных потоках, число которых на единицу меньше числа ядер компьютера. **/
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // Определение макс. числа потоков через число ядер
            if(maxthreads < sTwo)                                           // Если макс. число потоков менее двух, то
                return Calculate();                                         // проводим вычисления методом Calculate()
            vector<std::future<bool>> pool;             // Пул потоков в виде вектора
            pool.reserve(maxthreads);                   // Резервируем память вектору потоков
            size_t stocksize = Manuf.size();            // Получаем размер массива с единичными прозводствами
            size_t ncap = (stocksize/maxthreads)+sOne;  // Количество ед.производств для каждого потока
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // Сбрасываем счетчик
            for(size_t i{}; i<maxthreads; i++) {        // Цикл по всему пулу потоков
                size_t bg = i*ncap;                     // Определяем нижнюю границу индекса
                if(bg>=stocksize) break;                // Если нижняя граница больше или равна числу ед.пр-в, выходим из цикла
                size_t en = (i+sOne)*ncap;              // Определяем верхнюю границу индекса
                pool.push_back(std::async([](clsManufactory* pobj, size_t _bg, size_t _en) {
                    #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                        mu1.lock();                          // Блокируем ресурс cout для монопольного использования
                        cout << "Async thread #" << std::this_thread::get_id() << " has been started" << endl;
                        mu1.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                    #endif
                    bool res = pobj->Calculate(_bg, _en);                   // Вызываем метод с вычислениями,
                    #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                        mu1.lock();                          // Блокируем ресурс cout для монопольного использования
                        cout << "Async thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                        mu1.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                    #endif
                    return res;                                             // выходим и возвращаем результат
                }, this, bg, en));
            };                                          // Цикл по всему пулу потоков
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;});   // Вызываем отрисовку индикатора
            bool check = true;                          // Индикатор успешности операции
            for(auto &t : pool) {                       // Цикл по всему пулу потоков
                check = check && t.get();               // Формируем индикатор успешности
                if(!check) break;                       // Если индикатор становится false, то прерываем цикл
            };
            return check;
        }   // Calculate_future

        bool clsManufactory::Calculate_thread() {
        /** Метод рассчитывает объем, удельную и полную себестоимость незавершенного производства и готовой продукции
        для всех продуктов, выпускаемых на протяжении всего проекта. Метод поочередно вызывает методы CalculateItem для
        каждого единичного производства; формирует массивы продуктов и баланса незавершенного производства для каждого
        продукта. Все расчеты запускаются в потоках, число которых на единицу меньше числа ядер компьютера. **/
            auto do_Calculate = [](void *param, size_t bg, size_t en, bool &res) {
            /** "Обертка" в виде Лямбда-функции для метода bool Calculate(size_t, size_t), предназначенная для запуска
            указанного метода в потоке tread. **/
                clsManufactory* pobj = (clsManufactory*)param;  // Вспомогательный указатель на экземпляр класса clsManufactory
                #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                    mu1.lock();                          // Блокируем ресурс cout для монопольного использования
                    cout << "Thread #" << std::this_thread::get_id() << " has been started" << endl;
                    mu1.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                #endif
                res = pobj->Calculate(bg, en);          // Производим вычисления
                #ifdef DEBUG_THREADS                    // Макрос вывода отладочной информации. Работает, если определен DEBUG
                    mu1.lock();                          // Блокируем ресурс cout для монопольного использования
                    cout << "Thread #" << std::this_thread::get_id() << " has been calculated" << endl;
                    mu1.unlock();                        // Разблокируем ресурс cout для использования другими объектами
                #endif
            };  // do_Calculate
            size_t maxthreads = std::thread::hardware_concurrency()-sOne;   // Определение макс. числа потоков через число ядер
            if(maxthreads < sTwo)                                           // Если макс. число потоков менее двух, то
                return Calculate();                                         // проводим вычисления методом Calculate и выходим
            vector<std::thread> pool;                   // Пул потоков в виде вектора
            pool.reserve(maxthreads);                   // Резервируем память вектору потоков
            size_t stocksize = Manuf.size();            // Получаем размер массива с SKU
            size_t ncap = (stocksize/maxthreads)+sOne;  // Количество SKU для каждого потока
            bool Res[maxthreads];                       // Вспомогательный массив с результатами вычислений
            for(size_t k{}; k<maxthreads; k++)          // Инициализация вспомогательного массива значениями true (при выходе из
                *(Res+k) = true;                        // цикла по break элементы с индексами не прошедшими цикл д.б. true
            (pshell != nullptr) ? pshell->Counter_reset() : (void)([](){return;});  // Сбрасываем счетчик
            for(size_t i{}; i<maxthreads; i++) {        // Цикл по всему пулу потоков
                size_t bg = i*ncap;                     // Определяем нижнюю границу индекса
                if(bg>=stocksize) break;                // Если нижняя граница больше или равна числу SKU, выходим из цикла
                size_t en = (i+sOne)*ncap;              // Определяем верхнюю границу индекса
                pool.emplace_back(do_Calculate, this, bg, en, std::ref(*(Res+i)));  // Создаем поток и запускаем вычисления
            };  // Для возврата значения из потока используем эмулятор ссылки std::ref(...), см.:
                // https://pro--prof-com.turbopages.org/pro-prof.com/s/forums/topic/cplusplus_reference_wrapper
            (pshell) ? pshell->Progress_indicate() : (void)([](){return;}); // Вызываем отрисовку индикатора
            for(auto &t : pool)                         // Цикл по всему пулу потоков
                t.join();                               // Ожидаем завершения каждого запущенного потока
            for(size_t i{}; i<maxthreads; i++ )         // Цикл по всем элементам вспомогательного массива с результатами
                if(!(*(Res+i)))                         // если хоть один из расчетов вернул false, то
                    return *(Res+i);                    // возвращаем false
            return true;
        }   // Calculate_thread

        /** Get - методы **/

        const size_t clsManufactory::GetRMCount() const { return RMCount; }         // Метол возвращает общее число позиций сырья и материалов
        const size_t clsManufactory::GetPrCount() const { return PrCount; }         // Метол возвращает число периодов проекта
        const size_t clsManufactory::GetProdCount() const { return Manuf.size(); }  // Возвращает число продуктов

        const string clsManufactory::GetCurrency() const {
        /** Возвращает основную валюту проекта в виде текстовой строки **/
            return *(nmBPTypes::CurrencyTXT+hmcur);
        }; // GetCurrency

        strNameMeas* clsManufactory::GetProductDescription() const {
        /** Метод возвращает указатель на вновь создаваемый массив с именами и ед.измерения всех продуктов. **/
            strNameMeas *temp = new(nothrow) strNameMeas[Manuf.size()]; // Выделяем память массиву
            if(!temp) { return nullptr; };
            size_t i = sZero;
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++) {
                /** Если указатели не равны nullptr, то присваиваем имена и ед. размерности, если равны, то присваиваем "" **/
                (temp+i)->name    = (it->GetName())    != nullptr ? *(it->GetName())    : "";
                (temp+i)->measure = (it->GetMeasure()) != nullptr ? *(it->GetMeasure()) : "";
                i++;
            };
            return temp;
        }   // GetProductDescription

        const strNameMeas* clsManufactory::GetRMNames() const {
        /** Метод возвращает const-указатель на внутренний массив с именами и ед. измерения сырья и материалов. **/
            return RMNames;
        }   // GetRMNames

        strNameMeas* clsManufactory::GetRawMatDescription() const {
        /** Метод возвращает указатель на вновь создаваемый массив с именами и ед. измерения сырья и материалов. **/
            strNameMeas *temp = new(nothrow) strNameMeas[RMCount];  // // Выделяем память массиву
            if(!temp) { return nullptr; };
            var_cpy(temp, RMNames, RMCount);
            return temp;
        }   // GetRawMatDescription

        strItem* clsManufactory::GetTotalBalance() const {
        /** Метод возвращает указатель на одномерный массив, являющийся аналогом двумерной матрицы с балансами незавершенного
        производства для всех продуктов. Размер матрицы Manuf.size()*PrCount. Каждый элемент матрицы имеет тип strItem, т.е.
        имеет в своем составе значения volume, price и value для незавершенного производства. Массив создается в динамической
        памяти, после использования требует явного удаления с помощью оператора delete[]. **/
            return gettotal(&clsManufactItem::GetBalance);
        }   // GetTotalBalance

        strItem* clsManufactory::GetTotalProduct() const {
        /** Метод взвращает указатель на одномерный массив, являющийся аналогом двумерной матрицы с планами выхода всех
        продуктов в натуральном, удельном и полном стоимостном выражении. Размер матрицы Manuf.size()*PrCount. Каждый элемент
        матрицы имеет тип strItem, т.е. имеет в своем составе значения volume, price и value для продукта. Массив создается
        в динамической памяти, после использования требует явного удаления с помощью оператора delete[].**/
            return gettotal(&clsManufactItem::GetProductPlan);
        }   // GetTotalProduct

        const decimal* clsManufactory::GetRecipeItem(const string& Name) const {
        /** Метод возвращает константный указатель на внутренний массив с рецептурами для производства продукта с именем Name **/
            vector <clsManufactItem>::const_iterator it;            // Итератор для массива
            it = find(Manuf.cbegin(), Manuf.cend(), Name);          // Поиск объекта по имени
            if(it !=Manuf.cend()) { return it->GetRefRecipe(); }    // Если объект с таким именем есть, то выход и возврат ссылки
            return nullptr;                                         // Если объекта с таким именем нет, то выход и возврат nullptr
        }   // GetRecipeItem

        const decimal* clsManufactory::GetRecipeItem(const size_t _ind) const {
        /** Метод возвращает константный указатель на внутренний массив с рецептурами для производства продукта с
        индексом _ind **/
            if(_ind < Manuf.size()) {                       // Если индекс в допустимом диапазоне, то
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // устанавливаем итератор на элемент
                return it->GetRefRecipe();                  // и получаем указатель на массив с рецептурой для элемента
            }                                               // с индексом _ind
            else return nullptr;                            // иначе выход и возврат nullptr
        }   // GetRecipeItem

        const strNameMeas* clsManufactory::GetRawNamesItem(const string& Name) const {
        /** Метод возвращает константный указатель на внутренний массив с наименованиями сырья и материалов в рецептуре продукта с именем Name **/
            vector <clsManufactItem>::const_iterator it;            // Итератор для массива
            it = find(Manuf.cbegin(), Manuf.cend(), Name);          // Поиск объекта по имени
            if(it !=Manuf.cend()) { return it->GetRefRawNames(); }  // Если объект с таким именем есть, то выход и возврат ссылки
            return nullptr;                                         // Если объекта с таким именем нет, то выход и возврат nullptr
        }   // GetRawNamesItem

        const strNameMeas* clsManufactory::GetRawNamesItem(const size_t _ind) const {
        /** Метод возвращает константный указатель на внутренний массив с наименованиями сырья и материалов в рецептуре продукта с
        индексом _ind **/
            if(_ind < Manuf.size()) {                       // Если индекс в допустимом диапазоне, то
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // устанавливаем итератор на элемент
                return it->GetRefRawNames();                // и получаем указатель на массив наименованиями сырья и материалов
            }                                               // для продукта с индексом _ind
            else return nullptr;                            // иначе выход и возврат nullptr
        }   // GetRawNamesItem

        const size_t clsManufactory::GetDuration(const string& Name) const {
        /** Метод возвращает длительность производственного цикла в рецептуре продукта с именем Name  **/
            vector <clsManufactItem>::const_iterator it;            // Итератор для массива
            it = find(Manuf.cbegin(), Manuf.cend(), Name);          // Поиск объекта по имени
            if(it !=Manuf.cend()) { return it->GetDuration(); }     // Если объект с таким именем есть, то выход и возврат длительности
            return PrCount+sOne;                                    // иначе выход и возврат заведомо неправильного числа
        }   // GetDuration

        const size_t clsManufactory::GetDuration(const size_t _ind) const {
        /** Метод возвращает длительность производственного цикла в рецептуре продукта с индексом _ind **/
            if(_ind < Manuf.size()) {                       // Если индекс в допустимом диапазоне, то
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // устанавливаем итератор на элемент
                return it->GetDuration();                   // и получаем длительность производственного цикла для продукта
            }                                               // с индексом _ind
            else return PrCount+sOne;                       // иначе выход и возврат числа большего, чем количество периодов проекта
        }   // GetDuration

        const size_t clsManufactory::GetRCount(const string& Name) const {
        /** Метод возвращает число позиций сырья и материалов, участвующих в рецептуре продукта с именем Name
        (размер массива с наименованиями сырья) **/
            vector <clsManufactItem>::const_iterator it;            // Итератор для массива
            it = find(Manuf.cbegin(), Manuf.cend(), Name);          // Поиск объекта по имени
            if(it !=Manuf.cend()) { return it->GetRCount(); }       // Если объект с таким именем есть, то выход и возврат числа
            return RMCount+sOne;                                    // иначе выход и возврат числа большего, чем полное количество сырья
        }   // GetRCount

        const size_t clsManufactory::GetRCount(const size_t _ind) const {
        /** Метод возвращает число позиций сырья и материалов, участвующих в рецептуре продукта с индексом _ind **/
            if(_ind < Manuf.size()) {                       // Если индекс в допустимом диапазоне, то
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // устанавливаем итератор на элемент
                return it->GetRCount();                     // и получаем число позиций сырья и материалов для продукта
            }                                               // с индексом _ind
            else return RMCount+sOne;                       // иначе выход и возврат числа большего, чем полное количество сырья
        }   // GetRCount

        const strNameMeas clsManufactory::GetNameItem(const size_t _ind) const {
        /** Метод возвращает наименование продукта и единицу его измерения для продукта с с индексом _ind **/
            strNameMeas NameMeas;                           // Вспомогательная переменная
            NameMeas.name = EmpStr;                         // Инициализируем ее пустыми строками
            NameMeas.measure = EmpStr;
            if(_ind < Manuf.size()) {                       // Если индекс в допустимом диапазоне, то
                vector <clsManufactItem>::const_iterator it = Manuf.cbegin()+_ind;  // устанавливаем итератор на элемент,
                NameMeas.name = *(it->GetName());           // получаем наименование для продукта с индексом _ind
                NameMeas.measure = *(it->GetMeasure());     // получаем единицу измерения для продукта с индексом _ind
            };
            return NameMeas;                                // выходим и возвращаем информацию
        }   // GetNameItem

/** --------------------------------- Методы сериализации и десериализации clsManufactory ---------------------------------- **/


        bool clsManufactory::StF(ofstream &_outF)  {
        /** Метод имплементации записи в файловую переменную текущего экземпляра класса clsManufactory (метод сериализации).
        Параметры: &_outF - экземпляр класса ofstream для записи данных **/
            size_t manufsize = Manuf.size();            // Количество элементов вектора
            size_t capac= Manuf.capacity();             // Размер выделенной памяти для вектора; используется для десериализации
            bool flagRMNames;                           // Флаг существования массива
            #ifdef CDtor_voice
                long bpos = _outF.tellp();                                      // Определяем позицию в начале файла
                cout << "clsManufactory::StF begin bpos= " << bpos << endl;     // Выводим позицию на экран
            #endif // CDtor_voice
            if(!SEF(_outF, manufsize)) return false;    // Сохраняем в файл количество элементов вектора
            if(!SEF(_outF, capac)) return false;        // Сохраняем в файл емкость вектора
            if(!SEF(_outF, PrCount)) return false;      // Сохраняем в файл количество периодов проекта
            if(!SEF(_outF, RMCount)) return false;      // Сохраняем в файл количнство позиций сырья и материалов
            if(!SEF(_outF, hmcur)) return false;        // Сохраняем в файл домашнюю валюту
            if(RMNames) flagRMNames = true; else flagRMNames = false;           // Устанавливаем флаг
            #ifdef CDtor_voice
                cout << "flagRMNames= " << flagRMNames << endl;
            #endif // CDtor_voice
            if(!SEF(_outF, flagRMNames)) return false;                          // Сохраняем флаг в файл
            if(flagRMNames)                                                     // Если массив существует, то
                for(size_t i=sZero; i<RMCount; i++) {                           // сохраняем полный массив
                    if(!SEF(_outF, (RMNames+i)->name)) return false;            // с названиями сырья и
                    if(!SEF(_outF, (RMNames+i)->measure)) return false;         // единицами измерения
                };
            if(manufsize == sZero) return true;         // Если вектор пустой, то сериализация заканчивается
            for(vector<clsManufactItem>::iterator it = Manuf.begin(); it!=Manuf.end(); it++) {
                if(!it->StF(_outF)) return false;       // Сохраняем данные из экземпляров типа clsManufatcItem
            }
            #ifdef CDtor_voice
                long epos = _outF.tellp();                  // Определяем позицию в начале файла
                cout << "clsManufactory::StF end epos= " << epos << endl;
            #endif // CDtor_voice
            return true;
        }   // StF

        bool clsManufactory::SaveToFile(const string _filename) {
        /** Метод записи текущего экземпляра класса clsManufactory в файл **/
            if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
                                                                // 1.   Создаем экземпляр класса ofstream
            ofstream outF(_filename, ios::out | ios::binary);   // Связываем имя файла с файловым потоком для записи на диск
                                                                // 2.   Проверяем, успешно ли создан файл,
            if (!outF.is_open()) {                              // Если файл не создан, то сообщение пользователю и
                cout << "Ошибка открытия файла" <<endl;         // возврат false и выход из функции
                return false;
            };
            if(!StF(outF)) {                                    // 3.   Записываем данные в файл. Если файл не записан, то
                outF.close();                                   // Закрыаем файл и
                return false;                                   // Выходим с false
            };
            #ifdef CDtor_voice
                long epos = outF.tellp();
                cout << "clsManufactory::SaveToFile end epos= " << epos << endl;
            #endif // CDtor_voice
            outF.close();                                       // 4.   Закрываем файл
            return true;                                        // 5.   Возвращаем true
        }   // SaveToFile

        bool clsManufactory::RfF(ifstream &_inF) {
        /** Метод имплементации чтения из файловой переменной текущего экземпляра класса clsManufactory (метод десериализации).
        Параметры: &_inF - экземпляр класса ifstream для чтения данных **/
            size_t manufsize;                           // Размер вектора (число элементов)
            size_t capac;                               // Размер выделенной памяти для вектора; используется для десериализации
            bool flagRMNames;                           // Флаг существования массива
            #ifdef CDtor_voice
                long bpos = _inF.tellg();                                   // Определяем позицию в начале файла
                cout << "clsManufactory::RfF begin bpos= " << bpos << endl; // Выводим позицию на экран
            #endif // CDtor_voice
            if(!DSF(_inF, manufsize)) return false;     // Читаем из файла количество элементов вектора
            if(!DSF(_inF, capac)) return false;         // Читаем из файла емкость вектора
            if(!DSF(_inF, PrCount)) return false;       // Читаем из файла количество периодов проекта
            if(!DSF(_inF, RMCount)) return false;       // Читаем из файла количнство позиций сырья и материалов
            if(!DSF(_inF, hmcur)) return false;         // Читаем из файла домашнюю валюту
            if(!DSF(_inF, flagRMNames)) return false;   // Читаем из файла флаг
            #ifdef CDtor_voice
                cout << "flagRMNames= " << flagRMNames << endl;
            #endif // CDtor_voice
            if(flagRMNames) {                                           // Если флаг true, то массив надо прочитать из файла
                strNameMeas* tmp = new(nothrow) strNameMeas[RMCount];   // Выделяем память под массив
                if(!tmp) return false;                                  // Если память не выделена, то выход и возврат false
                for(size_t i=sZero; i<RMCount; i++) {                   // Читаем из файла массив
                    if(!DSF(_inF, (tmp+i)->name)) return false;         // с названиями сырья и
                    if(!DSF(_inF, (tmp+i)->measure)) return false;      // единицами измерения
                };
                std::swap(tmp, RMNames);                                // Обмениваемся указателями
                if(tmp) delete[] tmp;                                   // Удаляем вспомогательный массив
            } else RMNames = nullptr;

//            if(RMNames) delete[] RMNames;               // Если массив существует, то удаляем его
//            RMNames = new(nothrow) strNameMeas[RMCount];// Выделяем память под массив
//            if(RMNames) {                               // Если память выделена, то
//                for(size_t i=sZero; i<RMCount; i++) {   // Читаем из файла массив с названиями сырья и единицами измерения
//                    if(!DSF(_inF, (RMNames+i)->name)) return false;
//                    if(!DSF(_inF, (RMNames+i)->measure)) return false;
//                };
//            } else return false;

            if(manufsize == sZero) return true;             // Если вектор пустой, то десериализация заканчивается
            Manuf.reserve(capac);                           // Резервируем память для вектора
            for(size_t i=sZero; i<manufsize; i++) {
                Manuf.emplace_back();                       // Создаем элемент непосредственно в векторе
                if(!Manuf.back().RfF(_inF)) return false;   // Вызываем метод десериализации этого элемента
            };
            #ifdef CDtor_voice
                long epos = _inF.tellg();                                   // Определяем позицию в начале файла
                cout << "clsManufactory::RfF end epos= " << epos << endl;   // Выводим позицию на экран
            #endif // CDtor_voice
            return true;
        }   // RfF

        bool clsManufactory::ReadFromFile(const string _filename) {
        /** Метод чтения из файла и запись в экземпляр класса **/
            if(_filename == EmpStr) return false;               // Если имя файла пустое, то выход и возврат false
                                                                // 1.   Создаем экземпляр класса ifstream
            ifstream inF(_filename, ios::in | ios::binary);     // Связываем имя файла с файловым потоком для чтения с диска
                                                                // 2.   Проверяем, успешно ли открыт файл
            if (!inF.is_open()) {                               // Если файл не открыт, то сообщение пользователю и
                cout << "Ошибка открытия файла" <<endl;         // возврат false и выход из функции
                return false;
            };
            if(!RfF(inF)) {                                     // 3.   Считываем данные из файла. Если файл не записан, то
                inF.close();                                    // Закрываем файл
                return false;                                   // Выход и возврат false
            };
            #ifdef CDtor_voice
                long epos = inF.tellg();
                cout << "clsManufactory::ReadFromFile end epos= " << epos << endl;
            #endif // CDtor_voice
            inF.close();                                        // 4.   Закрываем файл
            return true;
        }   // ReadFromFile

/** -----------------------   Методы визуального контроля функций класса clsManufactory   ---------------------------------  **/

        void clsManufactory::ViewProjectParametrs() const {
        /** Метод визуального контроля функций GetPrCount, GetProdCount, GetRMCount **/
            cout << "*** Основные параметры проекта ***" << endl;
            cout << "Количество периодов проекта            " << GetPrCount() << endl;
            cout << "Общее число позиций продуктов          " << GetProdCount() << endl;
            cout << "Общее число позиций сырья и материалов " << GetRMCount() << endl;
            cout << "Валюта проекта                         " << GetCurrency() << endl;
            cout << endl;
        }   // ViewMainParametrs

        void clsManufactory::ViewRawMatPurchPlan() const {
        /** Метод визуального контроля функции GetRawMatPurchPlan **/
            cout << "*** Контроль работы метода GetRawMatPurchPlan ***" << endl;
            cout << "Потребность в ресурсах для всех продуктов" << endl;
            decimal* temp = GetRawMatPurchPlan();
            strNameMeas* RMNtemp = new(nothrow) nmBPTypes::strNameMeas[RMCount];
            if(!RMNtemp) return;
            for(size_t i=sZero; i<RMCount; i++) {
                (RMNtemp+i)->name = (RMNames+i)->name;
                (RMNtemp+i)->name.resize(15);            // Урезаем длину строки
                (RMNtemp+i)->measure = (RMNames+i)->measure;
                (RMNtemp+i)->measure.resize(8);          // Урезаем длину строки
            };
            ArrPrint(RMCount, RMNtemp, temp, PrCount);
            delete[] temp;
            delete[] RMNtemp;
        }   // ViewRawMatPurchPlan

        void clsManufactory::ViewRawMatPrice() const {
        /** Метод визуального контроля работоспособности функции SetRawMatPrice. **/
            cout << "*** Контроль работы метода SetRawMatPrice ***" << endl;
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++) {
                it->ViewRawMatPrice(*(nmBPTypes::CurrencyTXT+hmcur));
            }
        }   // ViewRawMatPrice

        void clsManufactory::ViewCalculate() const {
        /** Метод визуального контроля работоспособности функции Calculate. **/
            cout << "*** Контроль работы метода Calculate ***" << endl;
            if(Manuf.size() == sZero) return;           // Проверка вектора на остутствие элементов
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++) {
                it->ViewCalculate(*(nmBPTypes::CurrencyTXT+hmcur));
            }
        }   // ViewCalculate

        void clsManufactory::ViewBalance() const {
        /** Метод визуального контроля функций GetProductDescription, GetTotalBalance **/
            strNameMeas *ntemp = GetProductDescription();    // Формируем массив имен продуктов и ед.измерения
            if(!ntemp) return;
            strItem *btemp = GetTotalBalance();
            if(!btemp) return;
            cout << "*** Незавершенное производство ***" << endl;
            const string money = *(nmBPTypes::CurrencyTXT+hmcur);
            ArrPrint(GetProdCount(), ntemp, btemp, GetPrCount(), volume, money);
            ArrPrint(GetProdCount(), ntemp, btemp, GetPrCount(), price, money);
            ArrPrint(GetProdCount(), ntemp, btemp, GetPrCount(), value, money);
            delete[] ntemp;
            delete[] btemp;
        }   // ViewBalance

        void clsManufactory::ViewProductPlan() const {
        /** Метод визуального контроля функций GetProductDescription, GetTotalProduct **/
            strNameMeas *ntemp = GetProductDescription();    // Формируем массив имен продуктов и ед.измерения
            if(!ntemp) return;
            for(size_t i=sZero; i<Manuf.size(); i++) {       // Урезаем длину строк
                (ntemp+i)->name.resize(15);
                (ntemp+i)->measure.resize(8);
            };
            strItem *ptemp = GetTotalProduct();
            if(!ptemp) return;
            cout << "*** Готовая продукция ***" << endl;
            const string money = *(nmBPTypes::CurrencyTXT+hmcur);
            ArrPrint(GetProdCount(), ntemp, ptemp, GetPrCount(), volume, money);
            ArrPrint(GetProdCount(), ntemp, ptemp, GetPrCount(), price, money);
            ArrPrint(GetProdCount(), ntemp, ptemp, GetPrCount(), value, money);
            delete[] ntemp;
            delete[] ptemp;
        }   // ViewProductPlan

        void clsManufactory::ViewRecipes() const {
        /** Метод поочередно выводит на экран рецептуры всех продуктов. **/
            for(vector<clsManufactItem>::const_iterator it = Manuf.cbegin(); it!=Manuf.cend(); it++)
                    it->ViewRefRecipe();
        }   // ViewRecipe

#undef CDtor_voice          // Отменить вывод отладочной информации
# undef DEBUG_THREADS   // отменить макрос информации об инструменте DEBUG
