#include <iostream>
#include <climits>
#include <string>
#include <stdexcept>
#include <vector>
#include <regex>
#include <algorithm>

/*
Команды:
1) new A – добавить новое множество с названием A;
2) del A – удалить все элементы множества A и само множество;
3) add A x – добавить элемент x к множеству A;
4) rem A x – убрать элемент x из множества A;
5) pow A – вычислить булеан множества A;
6) see [A] – с аргументом «имя множества» вывести список элементов
   множества, без аргумента – список всех множеств;
7) A + B – вычислить объединение множеств A и B слиянием;
8) A & B – вычислить пересечение множеств A и B слиянием;
9) A - B – вычислить разность множеств A и B слиянием;
10) A < B – проверить, является ли A подмножеством B слиянием;
11) A = B – проверить, равны ли множества A и B.
*/

class Node {
public:
    char data;
    Node* next;

    Node(char value) : data(value), next(nullptr) {}
    ~Node() {}
};

class Set {
private:
    std::string name;
    Node* first;

    void checkName(const std::string& n) {
        if (n.empty()) {
            throw std::invalid_argument("The name of the set should not be empty.");
        }

        if (n.length() != 1 || n[0] < 'A' || n[0] > 'Z') {
            throw std::invalid_argument("The name of the set must be a single character in the A-Z range");
        }
    }

    void clear() {
        Node* current = first;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        first = nullptr;
    }

    void copyFrom(const Set& other) {
        Node* otherCurrent = other.first;
        Node* last = nullptr;

        while (otherCurrent != nullptr) {
            Node* newNode = new Node(otherCurrent->data);

            if (last == nullptr) {
                first = newNode;
            }
            else {
                last->next = newNode;
            }
            last = newNode;
            otherCurrent = otherCurrent->next;
        }
    }

public:
    Set(const std::string& setName) : first(nullptr) {
        checkName(setName);
        name = setName;
    }

    Set(const Set& other) : name(other.name), first(nullptr) {
        copyFrom(other);
    }

    Set& operator=(const Set& other) {
        if (this != &other) {
            clear();
            name = other.name;
            copyFrom(other);
        }
        return *this;
    }

    ~Set() {
        clear();
    }

    std::string getName() const {
        return name;
    }

    void setName(const std::string& newName) {
        checkName(newName);
        name = newName;
    }

    void addElement(char element) {
        if (element < 32 || element > 126) {
            throw std::invalid_argument("Element must be a printable character");
        }

        Node* newNode = new Node(element);

        if (first == nullptr || element < first->data) {
            newNode->next = first;
            first = newNode;
            return;
        }

        if (element == first->data) {
            delete newNode;
            return;
        }

        Node* current = first;
        while (current->next != nullptr && current->next->data < element) {
            current = current->next;
        }

        if (current->next != nullptr && current->next->data == element) {
            delete newNode;
            return;
        }

        newNode->next = current->next;
        current->next = newNode;
    }

    void removeElement(char element) {
        if (first == nullptr) return;

        if (first->data == element) {
            Node* temp = first;
            first = first->next;
            delete temp;
            return;
        }

        Node* current = first;
        while (current->next != nullptr && current->next->data != element) {
            current = current->next;
        }

        if (current->next != nullptr && current->next->data == element) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }

    bool contains(char element) const {
        Node* current = first;
        while (current != nullptr) {
            if (current->data == element) return true;
            current = current->next;
        }
        return false;
    }

    int getSize() const {
        int count = 0;
        Node* current = first;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    void print() const {
        std::cout << name << " = {";
        Node* current = first;
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) std::cout << ", ";
            current = current->next;
        }
        std::cout << "}" << std::endl;
    }

    std::vector<char> getElements() const {
        std::vector<char> elements;
        Node* current = first;
        while (current != nullptr) {
            elements.push_back(current->data);
            current = current->next;
        }
        return elements;
    }

    std::vector<Set> powerSet() const {
        std::vector<Set> result;
        std::vector<char> elements = getElements();
        int n = elements.size();

        // 2^n подмножеств
        for (int i = 0; i < (1 << n); i++) {
            Set subset("S");
            for (int j = 0; j < n; j++) {
                if (i & (1 << j)) {
                    subset.addElement(elements[j]);
                }
            }
            result.push_back(subset);
        }
        return result;
    }

    static Set unionSets(const Set& setA, const Set& setB) {
        Set result("T");

        Node* currentA = setA.first;
        Node* currentB = setB.first;

        while (currentA != nullptr && currentB != nullptr) {
            if (currentA->data < currentB->data) {
                result.addElement(currentA->data);
                currentA = currentA->next;
            }
            else if (currentB->data < currentA->data) {
                result.addElement(currentB->data);
                currentB = currentB->next;
            }
            else {
                result.addElement(currentA->data);
                currentA = currentA->next;
                currentB = currentB->next;
            }
        }

        while (currentA != nullptr) {
            result.addElement(currentA->data);
            currentA = currentA->next;
        }

        while (currentB != nullptr) {
            result.addElement(currentB->data);
            currentB = currentB->next;
        }

        return result;
    }

    static Set intersection(const Set& setA, const Set& setB) {
        Set result("T");

        Node* currentA = setA.first;
        Node* currentB = setB.first;

        while (currentA != nullptr && currentB != nullptr) {
            if (currentA->data < currentB->data) {
                currentA = currentA->next;
            }
            else if (currentB->data < currentA->data) {
                currentB = currentB->next;
            }
            else {
                result.addElement(currentA->data);
                currentA = currentA->next;
                currentB = currentB->next;
            }
        }

        return result;
    }

    static Set difference(const Set& setA, const Set& setB) {
        Set result("T");

        Node* currentA = setA.first;
        Node* currentB = setB.first;

        while (currentA != nullptr && currentB != nullptr) {
            if (currentA->data < currentB->data) {
                result.addElement(currentA->data);
                currentA = currentA->next;
            }
            else if (currentB->data < currentA->data) {
                currentB = currentB->next;
            }
            else {
                currentA = currentA->next;
                currentB = currentB->next;
            }
        }

        while (currentA != nullptr) {
            result.addElement(currentA->data);
            currentA = currentA->next;
        }

        return result;
    }

    static bool isSubset(const Set& setA, const Set& setB) {
        Node* currentA = setA.first;
        Node* currentB = setB.first;

        while (currentA != nullptr && currentB != nullptr) {
            if (currentA->data < currentB->data) {
                return false;
            }
            else if (currentB->data < currentA->data) {
                currentB = currentB->next;
            }
            else {
                currentA = currentA->next;
                currentB = currentB->next;
            }
        }

        return currentA == nullptr;
    }

    static bool areEqual(const Set& setA, const Set& setB) {
        Node* currentA = setA.first;
        Node* currentB = setB.first;

        while (currentA != nullptr && currentB != nullptr) {
            if (currentA->data != currentB->data) {
                return false;
            }
            currentA = currentA->next;
            currentB = currentB->next;
        }

        return currentA == nullptr && currentB == nullptr;
    }
};

class SetManager {
private:
    std::vector<Set> sets;

    int findSetIndex(const std::string& name) {
        for (int i = 0; i < sets.size(); i++) {
            if (sets[i].getName() == name) {
                return i;
            }
        }
        return -1;
    }

public:
    void createSet(const std::string& name) {
        if (findSetIndex(name) != -1) {
            std::cout << "Set " << name << " already exists!" << std::endl;
            return;
        }
        sets.push_back(Set(name));
        std::cout << "Set " << name << " created successfully." << std::endl;
    }

    void deleteSet(const std::string& name) {
        int index = findSetIndex(name);
        if (index == -1) {
            std::cout << "Set " << name << " not found!" << std::endl;
            return;
        }
        sets.erase(sets.begin() + index);
        std::cout << "Set " << name << " deleted successfully." << std::endl;
    }

    void addElement(const std::string& setName, char element) {
        int index = findSetIndex(setName);
        if (index == -1) {
            std::cout << "Set " << setName << " not found!" << std::endl;
            return;
        }
        sets[index].addElement(element);
        std::cout << "Element '" << element << "' added to set " << setName << std::endl;
    }

    void removeElement(const std::string& setName, char element) {
        int index = findSetIndex(setName);
        if (index == -1) {
            std::cout << "Set " << setName << " not found!" << std::endl;
            return;
        }
        sets[index].removeElement(element);
        std::cout << "Element '" << element << "' removed from set " << setName << std::endl;
    }

    void showPowerSet(const std::string& setName) {
        int index = findSetIndex(setName);
        if (index == -1) {
            std::cout << "Set " << setName << " not found!" << std::endl;
            return;
        }

        std::vector<Set> power = sets[index].powerSet();
        std::cout << "Power set of " << setName << " (size: " << power.size() << "):" << std::endl;
        for (size_t i = 0; i < power.size(); i++) {
            std::cout << "  " << i + 1 << ". ";
            power[i].print();
        }
    }

    void showSets(const std::string& setName = "") {
        if (setName.empty()) {
            if (sets.empty()) {
                std::cout << "No sets available." << std::endl;
                return;
            }
            std::cout << "All sets (" << sets.size() << "):" << std::endl;
            for (const auto& set : sets) {
                set.print();
            }
        }
        else {
            int index = findSetIndex(setName);
            if (index == -1) {
                std::cout << "Set " << setName << " not found!" << std::endl;
                return;
            }
            sets[index].print();
        }
    }

    void performOperation(const std::string& operation, const std::string& setNameA, const std::string& setNameB) {
        int indexA = findSetIndex(setNameA);
        int indexB = findSetIndex(setNameB);

        if (indexA == -1 || indexB == -1) {
            std::cout << "One or both sets not found!" << std::endl;
            return;
        }

        if (operation == "+") {
            Set result = Set::unionSets(sets[indexA], sets[indexB]);
            std::cout << setNameA << " + " << setNameB << " = ";
            result.print();
        }
        else if (operation == "&") {
            Set result = Set::intersection(sets[indexA], sets[indexB]);
            std::cout << setNameA << " & " << setNameB << " = ";
            result.print();
        }
        else if (operation == "-") {
            Set result = Set::difference(sets[indexA], sets[indexB]);
            std::cout << setNameA << " - " << setNameB << " = ";
            result.print();
        }
        else if (operation == "<") {
            bool isSubset = Set::isSubset(sets[indexA], sets[indexB]);
            std::cout << setNameA << " < " << setNameB << " = "
                << (isSubset ? "true" : "false") << std::endl;
        }
        else if (operation == "=") {
            bool areEqual = Set::areEqual(sets[indexA], sets[indexB]);
            std::cout << setNameA << " = " << setNameB << " = "
                << (areEqual ? "true" : "false") << std::endl;
        }
    }

    bool setExists(const std::string& name) {
        return findSetIndex(name) != -1;
    }

    const std::vector<Set>& getSets() const {
        return sets;
    }
};

class CommandProcessor {
private:
    SetManager manager;

    void printHelp() {
        std::cout << "\n=== Available Commands ===\n";
        std::cout << "new A           - Create new set A (A-Z)\n";
        std::cout << "del A           - Delete set A\n";
        std::cout << "add A x         - Add element x to set A\n";
        std::cout << "rem A x         - Remove element x from set A\n";
        std::cout << "pow A           - Show power set of A\n";
        std::cout << "see             - Show all sets\n";
        std::cout << "see A           - Show set A\n";
        std::cout << "A + B           - Union of sets A and B\n";
        std::cout << "A & B           - Intersection of sets A and B\n";
        std::cout << "A - B           - Difference of sets A and B\n";
        std::cout << "A < B           - Check if A is subset of B\n";
        std::cout << "A = B           - Check if A equals B\n";
        std::cout << "demo            - Auto demonstration\n";
        std::cout << "help            - Show this help\n";
        std::cout << "exit            - Exit program\n";
        std::cout << "==========================\n\n";
    }

    void autoDemo() {
        std::cout << "=== Automatic Demonstration ===" << std::endl;

        SetManager manager;

        //создание
        manager.createSet("A");
        manager.createSet("B");
        manager.createSet("C");

        //добавление эл
        manager.addElement("A", 'a');
        manager.addElement("A", 'c');
        manager.addElement("A", 'e');
        manager.addElement("A", 'g');

        manager.addElement("B", 'b');
        manager.addElement("B", 'c');
        manager.addElement("B", 'd');
        manager.addElement("B", 'e');

        manager.addElement("C", 'a');
        manager.addElement("C", 'e');

        //все множества
        manager.showSets();

        //операции
        std::cout << "\n--- Set Operations ---" << std::endl;
        manager.performOperation("+", "A", "B"); //объединение
        manager.performOperation("&", "A", "B"); //пересечение
        manager.performOperation("-", "A", "B"); //разность
        manager.performOperation("<", "C", "A"); //подмножество
        manager.performOperation("=", "A", "B"); //равенство

        //булеан
        std::cout << "\n--- Power Set Demo ---" << std::endl;
        SetManager tempManager;
        tempManager.createSet("X");
        tempManager.addElement("X", 'x');
        tempManager.addElement("X", 'y');
        tempManager.showPowerSet("X");

        std::cout << "\n=== Demonstration Complete ===" << std::endl;
    }

    void processCommand(const std::string& input) {
        std::string trimmed = input;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t") + 1);

        if (trimmed.empty()) return;

        //регулярные выражения для команд
        std::regex new_pattern(R"(^\s*new\s+([A-Z])\s*$)");
        std::regex del_pattern(R"(^\s*del\s+([A-Z])\s*$)");
        std::regex add_pattern(R"(^\s*add\s+([A-Z])\s+(\S)\s*$)");
        std::regex rem_pattern(R"(^\s*rem\s+([A-Z])\s+(\S)\s*$)");
        std::regex pow_pattern(R"(^\s*pow\s+([A-Z])\s*$)");
        std::regex see_all_pattern(R"(^\s*see\s*$)");
        std::regex see_one_pattern(R"(^\s*see\s+([A-Z])\s*$)");
        std::regex operation_pattern(R"(^\s*([A-Z])\s*([+&=\-<])\s*([A-Z])\s*$)");
        std::regex help_pattern(R"(^\s*help\s*$)");

        std::smatch matches;

        try {
            if (std::regex_match(trimmed, matches, new_pattern)) {
                manager.createSet(matches[1]);
            }
            else if (std::regex_match(trimmed, matches, del_pattern)) {
                manager.deleteSet(matches[1]);
            }
            else if (std::regex_match(trimmed, matches, add_pattern)) {
                manager.addElement(matches[1], matches[2].str()[0]);
            }
            else if (std::regex_match(trimmed, matches, rem_pattern)) {
                manager.removeElement(matches[1], matches[2].str()[0]);
            }
            else if (std::regex_match(trimmed, matches, pow_pattern)) {
                manager.showPowerSet(matches[1]);
            }
            else if (std::regex_match(trimmed, matches, see_all_pattern)) {
                manager.showSets();
            }
            else if (std::regex_match(trimmed, matches, see_one_pattern)) {
                manager.showSets(matches[1]);
            }
            else if (std::regex_match(trimmed, matches, operation_pattern)) {
                manager.performOperation(matches[2], matches[1], matches[3]);
            }
            else if (input == "demo") {
                autoDemo();
            }
            else if (input == "help") {
                printHelp();
            }
            else {
                std::cout << "Error: Unknown command '" << trimmed << "'\n";
                std::cout << "Type 'help' for available commands.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

public:
    void demonstration() {
        std::cout << "====================================================================================" << std::endl;
        std::cout << "Hello! " << std::endl;
        std::cout << "This is a program for performing operations on sets. Select an action." << std::endl;
        std::cout << "====================================================================================" << std::endl;

        printHelp();

        std::string command;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, command);

            if (command == "exit") {
                std::cout << "Goodbye!\n";
                break;
            }

            processCommand(command);
        }
    }
};

int main() {
    CommandProcessor processor;
    processor.demonstration();

    return 0;
}
