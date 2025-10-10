#include <iostream>
#include <climits>
#include <string>
#include <stdexcept>
#include <vector>

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

    void showSets(const std::string& setName = "") {
        if (setName.empty()) {
            std::cout << "All sets:" << std::endl;
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
};

void demonstration() {
    SetManager manager;

    std::cout << "Demonstration of Set Operations\n\n" << std::endl;

    manager.createSet("A");
    manager.createSet("B");

    manager.addElement("A", 'a');
    manager.addElement("A", 'c');
    manager.addElement("A", 'e');

    manager.addElement("B", 'b');
    manager.addElement("B", 'c');
    manager.addElement("B", 'd');

    manager.showSets();

    manager.performOperation("+", "A", "B");
    manager.performOperation("&", "A", "B");
    manager.performOperation("-", "A", "B");
    manager.performOperation("<", "A", "B");
    manager.performOperation("=", "A", "B");

    std::cout << "\n\nDemonstration Complete" << std::endl;
}

int main() {
    demonstration();
    return 0;
}