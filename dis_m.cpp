#include <iostream>
#include <vector>
#include <stdexcept>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "Relation.h"

/* Операции
 *  1.рефлексивность
 *  2.антирефлексивность
 *  3.симметричность
 *  4.антисимметричность
 *  5.асимметричность
 *  6.транзитивность
 *  7.антитранзитивность
 *  8.полнота
 * */

using namespace std;

void Relation::input_data_proc(int argc, const char* const argv[]) {
    cout << "====================================================================================" << endl;
    cout << "Hello! " << endl;
    cout << "This is a program for performing properties of sets. Select an action." << endl;

    bool file_loaded = false;
    string input_l;
    string command;

    if (argc == 2) {
        input_l = argv[1];
        try {
            filling_file(input_l);
            file_loaded = true;
            defining_properties();
            print_command();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    if (argc > 2) {
        throw invalid_argument("Too many arguments...");
    }

    while (true) {
        if (!file_loaded) {
            cout << "Specify the name of the input file." << endl;
            cout << "> ";
            getline(cin, input_l);

            try {
                filling_file(input_l);
                file_loaded = true;
                defining_properties();
                print_command();
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
                continue;
            }
        }

        cout << "> ";
        getline(cin, command);

        if (command == "exit") {
            cout << "Goodbye!\n";
            break;
        }
        else if (command == "other file") {
            file_loaded = false;
            continue;
        }
        else if (command == "data") {
            print_data();
            continue;
        }
        else if (command == "properties") {
            print_properties();
            continue;
        }
        else if (command == "help") {
            print_command();
            continue;
        }
        else {
            cout << "The command is not defined" << endl;
            print_command();
            continue;
        }
    }
}

void Relation::filling_file(string file_name) {
    ifstream i_file(file_name);
    if (!i_file.is_open()) {
        throw runtime_error("Cannot open file: " + file_name);
    }

    inp_set.clear();
    pairs.clear();
    properties.clear();
    properties.resize(10, 0); // 0-7: свойства, 8: эквивалентность, 9: порядок

    string line;

    if (getline(i_file, line)) {
        istringstream iss(line);
        char ch;
        while (iss >> ch) {
            inp_set.insert(ch);
        }
    }
    else {
        throw runtime_error("File is empty or cannot read first line");
    }

    while (getline(i_file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        char a, b;
        if (iss >> a >> b) {
            if (inp_set.find(a) == inp_set.end() || inp_set.find(b) == inp_set.end()) {
                throw runtime_error("Element in pair not found in base set");
            }
            pairs.insert({ a, b });
        }
        else {
            throw runtime_error("Invalid pair format in line: " + line);
        }
    }
    i_file.close();
}

void Relation::defining_properties() {
    properties[0] = is_reflexive();
    properties[1] = is_irreflexive();
    properties[2] = is_symmetric();
    properties[3] = is_antisymmetric();
    properties[4] = is_asymmetric();
    properties[5] = is_transitive();
    properties[6] = is_antitransitive();
    properties[7] = is_connected();

    // проверка на отношение эквивалентности
    if (properties[0] && properties[2] && properties[5]) {
        properties[8] = 1;
    }

    // проверка на отношение порядка
    if (properties[0] && properties[3] && properties[5]) {
        properties[9] = 1;
    }
}

int Relation::is_reflexive() {
    for (char elem : inp_set) {
        if (pairs.find({ elem, elem }) == pairs.end()) {
            return 0;
        }
    }
    return 1;
}

int Relation::is_irreflexive() {
    for (char elem : inp_set) {
        if (pairs.find({ elem, elem }) != pairs.end()) {
            return 0;
        }
    }
    return 1;
}

int Relation::is_symmetric() {
    for (const auto& pair : pairs) {
        if (pair.first != pair.second) { 
            if (pairs.find({ pair.second, pair.first }) == pairs.end()) {
                return 0;
            }
        }
    }
    return 1;
}

int Relation::is_antisymmetric() {
    for (const auto& pair : pairs) {
        if (pair.first != pair.second) {
            if (pairs.find({ pair.second, pair.first }) != pairs.end()) {
                return 0;
            }
        }
    }
    return 1;
}

int Relation::is_asymmetric() {
    // асимметричность = антисимметричность + антирефлексивность
    for (const auto& pair : pairs) {
        if (pairs.find({ pair.second, pair.first }) != pairs.end()) {
            return 0;
        }
    }
    return 1;
}

int Relation::is_transitive() {
    for (const auto& pair1 : pairs) {
        for (const auto& pair2 : pairs) {
            if (pair1.second == pair2.first) {
                if (pairs.find({ pair1.first, pair2.second }) == pairs.end()) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int Relation::is_antitransitive() {
    for (const auto& pair1 : pairs) {
        for (const auto& pair2 : pairs) {
            if (pair1.second == pair2.first && pair1.first != pair2.second) {
                if (pairs.find({ pair1.first, pair2.second }) != pairs.end()) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int Relation::is_connected() {
    for (char a : inp_set) {
        for (char b : inp_set) {
            if (a != b) {
                bool has_ab = (pairs.find({ a, b }) != pairs.end());
                bool has_ba = (pairs.find({ b, a }) != pairs.end());
                if (!has_ab && !has_ba) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void Relation::print_data() {
    cout << "\n=== The input data ===" << endl;
    cout << "Set: {";
    bool first = true;
    for (char elem : inp_set) {
        if (!first) cout << ", ";
        cout << elem;
        first = false;
    }
    cout << "}" << endl;

    cout << "Pairs (" << pairs.size() << "):" << endl;
    for (const auto& pair : pairs) {
        cout << "  (" << pair.first << ", " << pair.second << ")" << endl;
    }
}
void Relation::print_properties() {
    cout << "\n=== The relationship properties ===" << endl;
    cout << "1. Reflexive:        " << (properties[0] ? '+' : '-') << endl;
    cout << "2. Irreflexive:      " << (properties[1] ? '+' : '-') << endl;
    cout << "3. Symmetric:        " << (properties[2] ? '+' : '-') << endl;
    cout << "4. Antisymmetric:    " << (properties[3] ? '+' : '-') << endl;
    cout << "5. Asymmetric:       " << (properties[4] ? '+' : '-') << endl;
    cout << "6. Transitive:       " << (properties[5] ? '+' : '-') << endl;
    cout << "7. Antitransitive:   " << (properties[6] ? '+' : '-') << endl;
    cout << "8. Connected:        " << (properties[7] ? '+' : '-') << endl;

    if (properties[8]) {
        cout << "\n=== This is an equivalence relation ===" << endl;
        print_equivalence_classes();
    }

    if (properties[9]) {
        cout << "\n=== This is an order relation ===" << endl;
        print_min_max_elements();
    }
}

void Relation::print_command() {
    cout << "\n=== Available Commands ===" << endl;
    cout << "data            - Output the input data" << endl;
    cout << "properties      - Derive the relationship properties" << endl;
    cout << "other file      - Change the input file" << endl;
    cout << "help            - Show this help" << endl;
    cout << "exit            - Exit program" << endl;
    cout << "==========================" << endl << endl;
}

void Relation::print_equivalence_classes() {
    map<char, set<char>> classes;

    for (char elem : inp_set) {
        set<char> eq_class;
        for (char other : inp_set) {
            // элементы эквивалентны, если есть (a,b) и (b,a)
            if (pairs.find({ elem, other }) != pairs.end() &&
                pairs.find({ other, elem }) != pairs.end()) {
                eq_class.insert(other);
            }
        }
        classes[elem] = eq_class;
    }

    set<set<char>> unique_classes;
    for (const auto& pair : classes) {
        unique_classes.insert(pair.second);
    }

    cout << "Equivalence classes:" << endl;
    int index = 1;
    for (const auto& eq_class : unique_classes) {
        cout << "Class " << index++ << ": {";
        bool first = true;
        for (char elem : eq_class) {
            if (!first) cout << ", ";
            cout << elem;
            first = false;
        }
        cout << "}" << endl;
    }
    cout << "Partition index: " << unique_classes.size() << endl;
}

void Relation::print_min_max_elements() {
    set<char> minimal;
    set<char> maximal;

    for (char a : inp_set) {
        bool is_minimal = true;
        for (char b : inp_set) {
            if (a != b && pairs.find({ b, a }) != pairs.end()) {
                is_minimal = false;
                break;
            }
        }
        if (is_minimal) {
            minimal.insert(a);
        }
    }

    for (char a : inp_set) {
        bool is_maximal = true;
        for (char b : inp_set) {
            if (a != b && pairs.find({ a, b }) != pairs.end()) {
                is_maximal = false;
                break;
            }
        }
        if (is_maximal) {
            maximal.insert(a);
        }
    }

    cout << "Minimal elements: {";
    bool first = true;
    for (char elem : minimal) {
        if (!first) cout << ", ";
        cout << elem;
        first = false;
    }
    cout << "}" << endl;

    cout << "Maximal elements: {";
    first = true;
    for (char elem : maximal) {
        if (!first) cout << ", ";
        cout << elem;
        first = false;
    }
    cout << "}" << endl;
}

int main(int argc, char* argv[]) {
    Relation relation;

    try {
        relation.input_data_proc(argc, argv);
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}