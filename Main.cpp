#include <iostream>
#include <string>
#include <unistd.h>
#include <typeinfo>

#include "MyContainer.hpp"
#include "Animal.hpp"

using namespace std;
using namespace containers;

template <typename T>
void print_container(MyContainer<T>& container, const string& label) {
    cout << label << " (Normal): " << endl;
    for (auto it = container.begin(); it != container.end(); ++it)
        cout << *it << " ";
    cout << "\n";

    cout << label << " (Ascending): " << endl;
    for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it)
        cout << *it << " ";
    cout << "\n";

    cout << label << " (Descending): " << endl;
    for (auto it = container.begin_descending_order(); it != container.end_descending_order(); ++it)
        cout << *it << " ";
    cout << "\n";

    cout << label << " (SideCross): " << endl;
    for (auto it = container.begin_sidecross_order(); it != container.end_sidecross_order(); ++it)
        cout << *it << " ";
    cout << "\n";

    cout << label << " (MiddleOut): " << endl;
    for (auto it = container.begin_middle_order(); it != container.end_middle_order(); ++it)
        cout << *it << " ";
    cout << "\n\n";
}



int main() {
    MyContainer<int> int_con;
    int_con.add(9); int_con.add(5); int_con.add(7); int_con.add(3); int_con.add(1);
    int_con.add('h');
    cout << "Integer container: " << int_con << endl;
    print_container(int_con, "Int");

    // Char container
    MyContainer<char> char_con;
    char_con.add('d'); char_con.add('a'); char_con.add('c'); char_con.add('b');
    cout << "Char container: " << char_con << endl;

    print_container(char_con, "Char");

    // String container
    MyContainer<string> str_con;
    str_con.add("banana"); str_con.add("apple"); str_con.add("kiwi"); str_con.add("pear");
    cout << "String container: " << str_con << endl;
    print_container(str_con, "String");

    // size_t container
    MyContainer<size_t> size_con;
    size_con.add(50); size_con.add(10); size_con.add(30); size_con.add(20);
    cout << "Size_t container: " << size_con << endl;

    print_container(size_con, "Size_t");
    
    MyContainer<Animal> animal_con;
    animal_con.add(Animal(5, "Cat"));
    animal_con.add(Animal(3, "Dog"));
    animal_con.add(Animal(2, "Bird"));
    animal_con.add(Animal(7, "Horse"));
    print_container(animal_con, "Animal");
    return 0;
}