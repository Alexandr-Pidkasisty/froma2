#include <iostream>
#include <clsCCStorageTest.h>
#include <clsCostCenterStorage.h>

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

    setlocale(LC_ALL, "Russian");                   // Установка русского языка для вывода

/** Вывод заголовка на экран **/
    string clogHead = "Демонстрация работы класса clsCostCenterStorage";
    nmRePrint::PrintHeader0(clog, nmRePrint::smblcunt+nmRePrint::uThree, clogHead);

    Currency Cur = RUR;                             // Валюта проекта
    const AccountingMethod ac = AVG;                // Устанавливаем принцип учёта запасов

/****************************************************************************************************/
/**                     Импортируем исходные данные и создаем тестовое задание                     **/
/****************************************************************************************************/

    clsCCStorageTest* Test = new clsCCStorageTest();// Создаем объект с тестовыми данными
    if(!Test->Import_Data(Cur, AVG)) {
        cerr << "Импорт данных неудачен\n";         // Если импорт неудачен, то
        delete Test;                                // удаляем объет,
        return EXIT_FAILURE;                        // выходим из программы с кодом неудачного завершения
    }
/****************************************************************************************************/
/**                                 Создаем Склад с ЦЗП и ЦЗО                                      **/
/****************************************************************************************************/

    clsCostCenterStorage* Stock_A = new clsCostCenterStorage(Test->GetProjectCount(), Cur, ac, Test->GetOutCount(),\
    Test->GetMidCount(), Test->GetInCount(), Test->GetNames());

/********************************* Вывод информации на экран ****************************************/

    cout << "Количество периодов проекта:\t\t\t\t\t\t" << Stock_A->ProjectCount() << endl;
    cout << "Число ресурсов на входе склада (включая отгружаемые и потребляемые)\t " << Stock_A->Counts(inStock) << endl;
    cout << "Число ресурсов на хранении\t\t\t\t\t\t " << Stock_A->Counts(midStock) << endl;
    cout << "Число отгружаемых со склада ресурсов\t\t\t\t\t " << Stock_A->Counts(outStock) << endl << endl;

    clsTextField name(40);  // Установка формата вывода имен с помощью класса clsTextField
    clsTextField meas(10);  // Установка формата вывода единиц измерения
    const strNameMeas *Resources = Stock_A->GetNameMeas();  // Получаем указатель на массив с ресурсами
    nmRePrint::PrintHeader1(cout, nmRePrint::smblcunt+nmRePrint::uThree, \
    "Ресурсы на складе (включая отгружаемые и потребляемые)");
    nmRePrint::PrintUnderHeader(nmRePrint::smblcunt+nmRePrint::uThree, \
    "Ресурсы поступающие на склад и отгружаемые со склада");
    for(size_t i{}; i<Stock_A->Counts(inStock); i++) {
        if(i==Stock_A->Counts(outStock))
            nmRePrint::PrintUnderHeader(nmRePrint::smblcunt+nmRePrint::uThree, \
            "Ресурсы, потребляемые в ЦЗО");
        if(i==Stock_A->Counts(midStock))
            nmRePrint::PrintUnderHeader(nmRePrint::smblcunt+nmRePrint::uThree, \
            "Ресурсы, потребляемые в ЦЗП");
        cout << name << (Resources+i)->name;
        cout << meas << (Resources+i)->measure << endl;
    }


/****************************************************************************************************/
/**                                Создаем индикатор прогресса                                     **/
/****************************************************************************************************/

    string Working = "Working";
    type_progress* progress = new type_progress(std::clog, 75u, move(Working), '.', Test->GetOutCount());
    clsProgress_shell<type_progress>* shell = new clsProgress_shell<type_progress>(progress, Test->GetOutCount(), 1);
    Stock_A->Set_progress_shell(shell);

/** Этот фрагмент кода проверяет правильность методов сериализации и десериализации **/
    clog << endl;
    string temp_dir;                                                // Временная папка
    (temp_dir.assign(V_DIR_TEMP)).append("CostCenterStorage.dat");  // Полный путь до файла состояния
    Stock_A->SaveToFile(temp_dir);
    delete Stock_A;
    Stock_A = new clsCostCenterStorage();
    if(!Stock_A->ReadFromFile(temp_dir)) return EXIT_FAILURE;
    Stock_A->Set_progress_shell(shell);

/** Вводим в склад исходные данные и рассчитываем потребление ресурсов **/

    if(!Stock_A->SetStorage(Test->GetShip(), Test->GetPurch(),Test->GetOutRecipe(), Test->GetInRecipe())) {
        cout << "Ошибка создания склада\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        delete Test;
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    delete Test;                        // Удаляем объект с тестовыми данными за ненадобностью
    for(size_t i{}; i<Stock_A->Counts(outStock); i++) { // Для каждого отгружаемого ресурса устанавливаем
        Stock_A->SetShare(i, 0.5);      // норматив запаса готовой продукции в долях от объема отгрузок
    };
/********************************* Вывод информации на экран ****************************************/

    cout << endl << endl;
    nmRePrint::PrintHeader1(cout, nmRePrint::smblcunt+nmRePrint::uThree, "Настройки расчёта");
    Stock_A->ViewSettings();

    /** Создаем дефицит для проверки **/
//    Stock_A->SetAutoPurchase(2, nocalc);

/** Этот фрагмент кода проверяет правильность конструкторов копирования и перемещения  и операторов присвоения **/
    clsCostCenterStorage* A = new clsCostCenterStorage(*Stock_A);   // Создаем объект копированием
    clsCostCenterStorage* B = new clsCostCenterStorage(move(*A));   // Создаем объект перемещением
    clsCostCenterStorage* C = new clsCostCenterStorage();           // СОздаем пустой объект
    clsCostCenterStorage* D = new clsCostCenterStorage();           // СОздаем пустой объект
    *C = *B;
    *D = move(*C);
    delete Stock_A;
    Stock_A = new clsCostCenterStorage(move(*D));
    delete A;
    delete B;
    delete C;
    delete D;

/** Этот фрагмент кода проверяет правильность методов сериализации и десериализации **/
    clog << endl;
    Stock_A->SaveToFile(temp_dir);
    delete Stock_A;
    Stock_A = new clsCostCenterStorage();
    if(!Stock_A->ReadFromFile(temp_dir)) return EXIT_FAILURE;
    Stock_A->Set_progress_shell(shell);

/** Рассчитываем склад **/
    if(!Stock_A->Calculate()) {
        if(Stock_A->GetLack().lack > dZero)
            cerr << endl << "Обнаружен дефицит " << Stock_A->GetLack().lack << " для ресурса: " \
                << Stock_A->GetLack().Name << endl;
        cerr << "Расчёт прерван" << endl;
        delete Stock_A;
        return EXIT_FAILURE;    // выходим из программы с кодом неудачного завершения
    };

/****************************************************************************************************/
/**                                    Экспорт csv-файлов                                          **/
/****************************************************************************************************/

    string outdir = V_DIR_OUTPUTDATA;
    cout << endl << "Экспорт CSV-файлов в папку " << outdir << endl;

    /** Объем отгрузок в натуральном выражении **/
    if(!Stock_A->Export_Data(outdir+f_ws_volume, outStock, shipment, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем отгрузок в удельном стоимостном выражении **/
    if(!Stock_A->Export_Data(outdir+f_ws_price, outStock, shipment, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Объем отгрузок в полном стоимостном выражении **/
    if(!Stock_A->Export_Data(outdir+f_ws_value, outStock, shipment, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Объем закупок в натуральном выражении **/
    if(!Stock_A->Export_Data(outdir+f_wp_volume, inStock, purchase, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем закупок в удельном стоимостном выражении **/
    if(!Stock_A->Export_Data(outdir+f_wp_price, inStock, purchase, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем закупок в полном стоимостном выражении **/
    if(!Stock_A->Export_Data(outdir+f_wp_value, inStock, purchase, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем отгрузок из ЦЗП в ЦУР в натуральном выражении **/
    if(!Stock_A->Export_Data(outdir+f_PCCs_volume, inStock, shipment, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Цены отгрузок из ЦЗП в ЦУР **/
    if(!Stock_A->Export_Data(outdir+f_PCCs_price, inStock, shipment, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Стоимость отгрузок из ЦЗП в ЦУР **/
    if(!Stock_A->Export_Data(outdir+f_PCCs_value, inStock, shipment, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем поставок в ЦУР из ЦЗП в натуральном выражении **/
    if(!Stock_A->Export_Data(outdir+f_RACp_volume, midStock, purchase, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Цены поставок в ЦУР из ЦЗП **/
    if(!Stock_A->Export_Data(outdir+f_RACp_price, midStock, purchase, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Стоимость поставок в ЦУР из ЦЗП **/
    if(!Stock_A->Export_Data(outdir+f_RACp_value, midStock, purchase, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    };
    /** Объем отгрузок из ЦУР в ЦЗО в натуральном выражении **/
    if(!Stock_A->Export_Data(outdir+f_RACs_volume, midStock, shipment, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Цены отгрузок из ЦУР в ЦЗО **/
    if(!Stock_A->Export_Data(outdir+f_RACs_price, midStock, shipment, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Стоимость отгрузок из ЦУР в ЦЗО **/
    if(!Stock_A->Export_Data(outdir+f_RACs_value, midStock, shipment, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Объем поставок в ЦЗО из ЦУР в натуральном выражении **/
    if(!Stock_A->Export_Data(outdir+f_SCCp_volume, outStock, purchase, volume)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Цены поставок в ЦЗО из ЦУР. Данные будут нулевые, поэтому экспорт не состоится **/
    if(!Stock_A->Export_Data(outdir+f_SCCp_price, outStock, purchase, price)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }
    /** Стоимость поставок в ЦЗО из ЦУР. Данные будут нулевые, поэтому экспорт не состоится **/
    if(!Stock_A->Export_Data(outdir+f_SCCp_value, outStock, purchase, value)) {
        cout << "Экспорт исходных данных не удачен\n";  // Если экспорт неудачен, то
        delete Stock_A;                                 // удаляем объет,
        return EXIT_FAILURE;                            // выходим из программы с кодом неудачного завершения
    }

    cout << "Готово. Данные экспортированы" << endl;
    cout << "Copyright (c) 2025 Пидкасистый Александр Павлович";

    delete Stock_A;
    delete progress;
    delete shell;
    return EXIT_SUCCESS;
}
