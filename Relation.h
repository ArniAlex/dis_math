#ifndef ДИСКРЕТКА_RELATION_H
#define ДИСКРЕТКА_RELATION_H

#include <set>
#include <vector>
#include <string>

class Relation {
    std::set<char> inp_set; //автомат сортировка и проверка на уникальность элементов
    std::set<std::pair<char, char>> pairs; //сортированные пары
    std::vector<int> properties; //свойства

public:
    void input_data_proc(int argc, const char* const argv[]);

    //вспом ф
    void filling_file(std::string file_name);

    //вывод
    void print_data();
    void print_properties();
    void print_command();

    //ф для вывода эквивалентности
    void print_equivalence_classes();

    //ф для вывода порядка
    void print_min_max_elements();

    //ф для определения свойств
    void defining_properties();

    //отдельные ф для каждого свойства 
    int is_reflexive();
    int is_irreflexive();
    int is_symmetric();
    int is_antisymmetric();
    int is_asymmetric();
    int is_transitive();
    int is_antitransitive();
    int is_connected();
};

#endif //ДИСКРЕТКА_RELATION_H
