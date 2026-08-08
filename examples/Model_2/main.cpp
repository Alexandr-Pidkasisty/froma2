#include <iostream>
#include "clsImportConfig.hpp"
#include "clsCompany.h"

#define ALT_PLACE_FOR_PRICE_SETTING         // Изменяем место в коде для ввода цен на ресурсы

/** Имена файлов для экспорта данных со склада **/
#define f_ws_volume "f_ws_volume.csv"       // Объемы отгрузок со склада в натуральном выражении
#define f_ws_price  "f_ws_price.csv"        // Цены отгрузок
#define f_ws_value  "f_ws_value.csv"        // Стоимость отгрузок

#define f_wp_volume "f_wp_volume.csv"       // Объемы закупок на склад в натуральном выражении
#define f_wp_price  "f_wp_price.csv"        // Цены закупок
#define f_wp_value  "f_wp_value.csv"        // Стоимость закупок

#define f_PCCs_volume "f_PCCs_volume.csv"   // Объем отгрузок из ЦЗП в ЦУР в натуральном выражении
#define f_PCCs_price  "f_PCCs_price.csv"    // Цены отгрузок из ЦЗП в ЦУР
#define f_PCCs_value  "f_PCCs_value.csv"    // Стоимость отгрузок из ЦЗП в ЦУР

#define f_RACp_volume "f_RACp_volume.csv"   // Объем поставок в ЦУР из ЦЗП в натуральном выражении
#define f_RACp_price  "f_RACp_price.csv"    // Цены поставок в ЦУР из ЦЗП
#define f_RACp_value  "f_RACp_value.csv"    // Стоимость поставок в ЦУР из ЦЗП

#define f_RACs_volume "f_RACs_volume.csv"   // Объем отгрузок из ЦУР в ЦЗО в натуральном выражении
#define f_RACs_price  "f_RACs_price.csv"    // Цены отгрузок из ЦУР в ЦЗО
#define f_RACs_value  "f_RACs_value.csv"    // Стоимость отгрузок из ЦУР в ЦЗО

#define f_SCCp_volume "f_SCCp_volume.csv"   // Объем поставок в ЦЗО из ЦУР в натуральном выражении
#define f_SCCp_price  "f_SCCp_price.csv"    // Цены поставок в ЦЗО из ЦУР
#define f_SCCp_value  "f_SCCp_value.csv"    // Стоимость поставок в ЦЗО из ЦУР

using namespace std;

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "Russian");       // Установка русского языка для вывода

/****************************************************************************************************/
/**      Вывод заголовка программы на экран, задание валюты проекта и принципа учёта затрат        **/
/****************************************************************************************************/

    string ProgramHeader = "Программа для расчета полных и удельных переменных затрат склада Model_2";
    nmRePrint::PrintHeader0(cout, nmRePrint::smblcunt+nmRePrint::uThree, ProgramHeader);

    string cfg_file = "model_2.cfg";    // Определение имени конфигурационного файла по умолчанию
    Currency Cur = RUR;                 // Устанавливаем валюту проекта
    const AccountingMethod ac = AVG;    // Устанавливаем принцип учёта запасов

/****************************************************************************************************/
/**                                    Импорт конфигурации                                        **/
/****************************************************************************************************/

    if(argc > 1)                        // Проверяем количество элементов массива argv; если введено
        cfg_file = string(argv[1]);     // имя файла конфигурации, то заменяем его в переменной cfg_file,
                                        // иначе используем имя файла по умолчанию
    clsImportConfig* Test = new clsImportConfig(cfg_file);  // Создаем объект импорта данных
    try {
        Test->Configure();                  // Читаем/редактируем конфигурационные данных
    }
    catch(const char* error_message) {      // Если получено исключение, то
        cout << error_message << endl;      // выводим сообщение об ошибке,
        delete Test;                        // удаляем объет и
        return EXIT_FAILURE;                // выходим из программы с кодом неудачного завершения
    }
    if(!Test->Import_Data(Cur, ac)) {       // Испортируем данные из файлов
        cerr << "Импорт данных неудачен\n"; // Если импорт неудачен, то
        delete Test;                        // удаляем объет и
        return EXIT_FAILURE;                // выходим из программы с кодом неудачного завершения
    }

/****************************************************************************************************/
/**                               Создаем Склад с ЦЗП, ЦУР и ЦЗО                                   **/
/****************************************************************************************************/

    clsCompany* Stock = new clsCompany(Test->GetProjectCount(), Cur, ac, Test->GetOutCount(), \
        Test->GetMidCount(), Test->GetInCount(), Test->GetNames(), Test->GetOutRecipe_ptr(), \
        Test->GetInRecipe_ptr());                           // Создаем склад и вводим в него данные
                                                            // Устанавливаем:
    Stock->SetTitle(Test->GetTitle());                      // заголовок программы
    Stock->SetComment(Test->GetAboutComment(), Test->GetAboutCount());  // описание проекта
    Stock->Set_share(midStock, Test->GetShares());          // норматив запасов на складе
    Stock->Set_permission(midStock, Test->GetPermission()); // флаг разрешения закупки/отгрузки в одном периоде
    Stock->Set_autopurchase(midStock, Test->GetPurcalc());  // флаг авторасчета

    if(!Stock->Set_ship_volume(Test->GetShip())) {              // Вводим массив с отгрузками из ЦЗО
        cerr << "Ввод данных об отгрузках из ЦЗО неудачен\n";   // Если ввод неудачен, то
        delete Test;                                            // удаляем объект ипорта,
        delete Stock;                                           // удаляем объект склада
        return EXIT_FAILURE;                    // и выходим из программы с кодом неудачного завершения
    };
    if(!Stock->Set_pur_volume(Test->GetPurch_v()))              // Вводим массив с объемами поступлений
    // в ЦЗП. Это необходимо, если установлен флаг ручного расчёта: в массиве Pcalc есть элементы nocalc.
    // Ввод должен осуществляться ДО ВЫЗОВА метода RCalculate.
            cerr << "Файл с объемами поставок в ЦУР не импортировался\n";

    #ifdef ALT_PLACE_FOR_PRICE_SETTING          // Вводить цены закупок можно как до расчета объемов
    // потребляемых ресурсов (вызов метода RCalculate), так и после такого расчёта
    if(!Stock->Set_pur_price(Test->GetPurch_p())) {             // Вводим массив с ценами в ЦЗП
            cerr << "Ввод данных с ценами поставок на ЦЗП неудачен\n";  // Если ввод неудачен, то
            delete Test;                                        // удаляем объект ипорта,
            delete Stock;                                       // удаляем объект склада
            return EXIT_FAILURE;                // и выходим из программы с кодом неудачного завершения
    };
    #endif // ALT_PLACE_FOR_PRICE_SETTING

/********************************* Вывод информации на экран ****************************************/

    cout << "Количество периодов проекта:\t\t\t\t\t\t" << Stock->ProjectCount() << endl;
    cout << "Число ресурсов на входе склада (включая отгружаемые и потребляемые)\t " << Stock->Counts(purchase, inStock) << endl;
    cout << "Число ресурсов на хранении\t\t\t\t\t\t " << Stock->Counts(purchase, midStock) << endl;
    cout << "Число отгружаемых со склада ресурсов\t\t\t\t\t " << Stock->Counts(shipment, outStock) << endl << endl;

    clsTextField name(40);  // Установка формата вывода имен с помощью класса clsTextField
    clsTextField meas(10);  // Установка формата вывода единиц измерения
    const strNameMeas *Resources = Stock->GetNameMeas(purchase, inStock);  // Получаем указатель на массив с ресурсами
    nmRePrint::PrintHeader1(cout, nmRePrint::smblcunt+nmRePrint::uThree, \
    "Ресурсы на складе (включая отгружаемые и потребляемые)");
    nmRePrint::PrintUnderHeader(nmRePrint::smblcunt+nmRePrint::uThree, \
    "Ресурсы поступающие на склад и отгружаемые со склада");
    for(size_t i{}; i<Stock->Counts(purchase, inStock); i++) {
        if(i==Stock->Counts(shipment, outStock))
            nmRePrint::PrintUnderHeader(nmRePrint::smblcunt+nmRePrint::uThree, \
            "Ресурсы, потребляемые в ЦЗО");
        if(i==Stock->Counts(purchase, midStock))
            nmRePrint::PrintUnderHeader(nmRePrint::smblcunt+nmRePrint::uThree, \
            "Ресурсы, потребляемые в ЦЗП");
        cout << name << (Resources+i)->name;
        cout << meas << (Resources+i)->measure << endl;
    }
    delete[] Resources;
    cout << endl << endl;
    nmRePrint::PrintHeader1(cout, nmRePrint::smblcunt+nmRePrint::uThree, "Настройки расчёта");
    Stock->ViewSettings();   // Выводим основные настройки

/****************************************************************************************************/
/**                                Создаем индикатор прогресса                                     **/
/****************************************************************************************************/

    string Working = "Working";
    type_progress* progress = new type_progress(std::clog, 75u, move(Working), '.', Test->GetOutCount());
    clsProgress_shell<type_progress>* shell = new clsProgress_shell<type_progress>\
        (progress, Test->GetOutCount(), 1);
    Stock->Set_progress_shell(shell);

/****************************************************************************************************/
/**                                    Выполняем вычисления                                        **/
/****************************************************************************************************/

    if(!Stock->RCalculate(Message)) {   // Вычисляем потребность в ресурсах в натуральном выражении
        cerr << "Расчёт неудачен\n";    // Если расчёт неудачен, то
        delete Test;                    // удаляем объект импорта,
        delete progress;                // удаляем индикатор прогресса,
        delete shell;                   // удаляем оболочку индикатора прогресса,
        delete Stock;                   // удаляем объект склада
        return EXIT_FAILURE;            // и выходим с кодом аварийного завершения
    };

    #ifndef ALT_PLACE_FOR_PRICE_SETTING
        if(!Stock->Set_pur_price(Test->GetPurch_p())) { // Вводим массив с ценами в ЦЗП
            cerr << "Ввод данных с ценами поставок на ЦЗП неудачен\n";  // Если ввод неудачен, то
            delete Test;                                                // удаляем объект ипорта,
            delete progress;                    // удаляем индикатор прогресса,
            delete shell;                       // удаляем оболочку индикатора прогресса,
            delete Stock;                       // удаляем объект склада
            return EXIT_FAILURE;                // и выходим из программы с кодом неудачного завершения
        };
    #endif // ALT_PLACE_FOR_PRICE_SETTING

    delete Test;                                // Удаляем объект за ненадобностью

    cout << '\n';
    if(!Stock->FCalculate(Message)) {   // Вычисляем стоимостные показатели отгружаемых из подразделений ресурсов
        cerr << "Расчёт неудачен\n";    // Если расчёт неудачен, то
        delete progress;                // удаляем индикатор прогресса,
        delete shell;                   // удаляем оболочку индикатора прогресса,
        delete Stock;                   // удаляем объект склада
        return EXIT_FAILURE;            // и выходим с кодом аварийного завершения
    }

    delete progress;                    // Удаляем индикатор прогресса за ненадобностью
    delete shell;                       // Удаляем оболочку индикатора прогресса за ненадобностью

/****************************************************************************************************/
/**                           Формирование и вывод отчетов                                         **/
/****************************************************************************************************/

    string ReportName = V_DIR_REPORTS;  // Устанавливаем полный путь до файла отчета
    ReportName.append("Model_2");       // Добавляем имя отчета
    cout << "Введите имя файла отчета без расширения [" << ReportName << "]: ";
    inData(ReportName, ReportName);                         // Читаем имя файла в переменную ReportName
    Stock->SetDevice(file);             // Устанавливаем устройство для вывода отчета
    Stock->SetRName(ReportName);        // Устанавливаем имя файла отчета
    Stock->Report();

    bool rep_check = true;              // Индикатор корректного вывода отчёта
    for(short dep{2}; dep>=0; dep--)        // Цикл по подразделениям
        for(short arr{2}; arr>=0; arr--)    // Цикл по набору данных
            for(size_t flg{}; flg<3; flg++) // Цикл по типу данных
                if(!Stock->Detail_report(static_cast<size_t>(dep), Message, static_cast<ChoiseData>(arr),\
                static_cast<ReportData>(flg))) {    // Если вывод какого-либо отчёта неудачен
                    rep_check = false;              // Присваиваем индикатору false
                    break;                          // и завершаем циклы
                };
    if(rep_check)
        cout << "Готово. Отчет выведен в файл " << ReportName << endl;
    else
        cout << "Ошибка вывода отчета" << endl;

/****************************************************************************************************/
/**                                    Экспорт csv-файлов                                          **/
/****************************************************************************************************/

    string outdir = V_DIR_OUTPUTDATA;
    cout << "Введите папку для экспорта CSV-файлов [" << outdir << "]: ";
    inData(outdir, outdir);                 // Читаем имя папки в переменную outdir
    struct stat sb;                         // Переменная для записи методанных о файле
    if (stat(outdir.c_str(), &sb) != 0) {   // Если папка для вывода csv-файлов не существует, то
        cout << "Ошибка вывода csv-файлов: указанная папка не существует\n";
        delete Stock;
        cout << "Copyright (c) 2026 Пидкасистый Александр Павлович" << endl;
        return EXIT_FAILURE;                // выходим из программы с кодом неудачного завершения
    }
    cout << "Экспорт CSV-файлов в папку " << outdir << endl;

       /** Объем отгрузок в натуральном выражении **/
    if(!Stock->Export_Data(outdir+f_ws_volume, outStock, shipment, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем отгрузок в удельном стоимостном выражении **/
    if(!Stock->Export_Data(outdir+f_ws_price, outStock, shipment, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем отгрузок в полном стоимостном выражении **/
    if(!Stock->Export_Data(outdir+f_ws_value, outStock, shipment, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем закупок в натуральном выражении **/
    if(!Stock->Export_Data(outdir+f_wp_volume, inStock, purchase, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем закупок в удельном стоимостном выражении **/
    if(!Stock->Export_Data(outdir+f_wp_price, inStock, purchase, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем закупок в полном стоимостном выражении **/
    if(!Stock->Export_Data(outdir+f_wp_value, inStock, purchase, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;
    };
    /** Объем отгрузок из ЦЗП в ЦУР в натуральном выражении **/
    if(!Stock->Export_Data(outdir+f_PCCs_volume, inStock, shipment, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Цены отгрузок из ЦЗП в ЦУР **/
    if(!Stock->Export_Data(outdir+f_PCCs_price, inStock, shipment, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Стоимость отгрузок из ЦЗП в ЦУР **/
    if(!Stock->Export_Data(outdir+f_PCCs_value, inStock, shipment, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем поставок в ЦУР из ЦЗП в натуральном выражении **/
    if(!Stock->Export_Data(outdir+f_RACp_volume, midStock, purchase, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Цены поставок в ЦУР из ЦЗП **/
    if(!Stock->Export_Data(outdir+f_RACp_price, midStock, purchase, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Стоимость поставок в ЦУР из ЦЗП **/
    if(!Stock->Export_Data(outdir+f_RACp_value, midStock, purchase, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем отгрузок из ЦУР в ЦЗО в натуральном выражении **/
    if(!Stock->Export_Data(outdir+f_RACs_volume, midStock, shipment, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Цены отгрузок из ЦУР в ЦЗО **/
    if(!Stock->Export_Data(outdir+f_RACs_price, midStock, shipment, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Стоимость отгрузок из ЦУР в ЦЗО **/
    if(!Stock->Export_Data(outdir+f_RACs_value, midStock, shipment, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Объем поставок в ЦЗО из ЦУР в натуральном выражении **/
    if(!Stock->Export_Data(outdir+f_SCCp_volume, outStock, purchase, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Цены поставок в ЦЗО из ЦУР. **/
    if(!Stock->Export_Data(outdir+f_SCCp_price, outStock, purchase, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Стоимость поставок в ЦЗО из ЦУР. **/
    if(!Stock->Export_Data(outdir+f_SCCp_value, outStock, purchase, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock;                                // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }

    cout << "Готово. Данные экспортированы" << endl;
    cout << "Copyright (c) 2026 Пидкасистый Александр Павлович";

    delete Stock;

# undef ALT_PLACE_FOR_PRICE_SETTING
    return EXIT_SUCCESS;
}
