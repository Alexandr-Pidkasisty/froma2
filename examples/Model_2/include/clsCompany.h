#ifndef FROMA2_CLSCOMPANY_H
#define FROMA2_CLSCOMPANY_H

#include <agregate_module.h>

enum count_choise{inStock=sZero, midStock, outStock};   // Сепаратор выбора подразделения
const std::string Message[3] = {"ЦЗП", "ЦУР", "ЦЗО"};   // Заголовки индикаторов прогресса

/***************************************************************************************************************/
/**                                                                                                           **/
/**                                              clsCompany                                                   **/
/**                                                                                                           **/
/***************************************************************************************************************/

class clsCompany : public clsAgregate {
/** Класс конкретной реализации предприятия из трех подразделений: ЦЗП (центр учета затрат по поступлению, PCC -
Purchase Cost Centre), ЦУР (центр учета ресурсов, RAC - Resource Accounting Centre) и ЦЗО (центр учета затрат по
отгрузке, SCC - Shipment Cost Centre). Является наследником класса clsAgregate. **/
    private:
        size_t InCount, MidCount, OutCount; // Количество позиций на входе ЦЗП, в ЦУР и выходе ЦЗО
    public:
        clsCompany();
        /** Конструктор по умолчанию **/

        clsCompany(const size_t _PrCount, const Currency _cur, const AccountingMethod _ac, const size_t ProdCount,\
            const size_t _MidCount, const size_t _RMCount, const strNameMeas _RMNames[], const clsRecipeItem _RecipeOut[],\
            const clsRecipeItem _RecipeIn[]);
        /** Конструктор с параметрами. Параметры: _PrCount - количество периодов проекта, _cur - валюта проекта, _ac - принцип
        учета запасов, ProdCount - количество номенклатурных позиций отгружаемых из ЦЗО продуктов, _MidCount - количество
        номенклатурных позиций, хранимых в ЦУР, _RMCount - количество номенклатурных позиций, поставляемых в ЦЗП, _RMNames -
        указатель на массив с наименованиями ресурсов и ед. их измерения, _RecipeOut - указатель на массив с рецептурами для
        ЦЗО, _RecipeIn - указатель на массив с рецептурами для ЦЗП. **/

        void swap(clsCompany& other) noexcept;
        /** Функция обмена значениями между объектами. Функция объявлена noexcept - не вызывающей исключения **/

        clsCompany(const clsCompany& other);
        /** Конструктор копирования **/

        clsCompany(clsCompany&& other);
        /** Конструктор перемещения **/

        clsCompany& operator=(const clsCompany& other);
        /** Перегрузка оператора присваивания копированием. Реализовано в идеоме
        КОПИРОВАНИЯ-И-ЗАМЕНЫ (copy-and-swap idiom) **/

        clsCompany& operator=(clsCompany&& other);
        /** Перегрузка оператора присваивания перемещением. Реализовано в идеоме ПЕРЕМЕЩЕНИЯ-И-ЗАМЕНЫ
        (move-and-swap idiom) **/

        ~clsCompany() override = default;

    protected:
        virtual void reportstream(ostream& os) const override;
        /** Метод выводит отчет в поток os. Используется невиртуальным методом clsBaseProject::Report **/

        virtual bool StF(ofstream &_outF) override;
        /** Метод имплементации записи в файловую переменную текущего экземпляра класса (запись в файл, метод
        сериализации). Параметры: &_outF - экземпляр класса ofstream для записи данных **/

        virtual bool RfF(ifstream &_inF) override;
        /** Метод имплементации чтения из файловой переменной экземпляра класса (чтение из файла, метод
        десериализации). Параметры: &_inF - ссылка на экземпляр класса ifstream для чтения данных. **/
};

#endif // FROMA2_CLSCOMPANY_H
