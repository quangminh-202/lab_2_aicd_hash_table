#include<../include/hash_table.h>
#include <iostream>
#include <string>

using namespace my_namespace;


// Пример использования
int main() {
    MyUnorderedMap<string, int> map(10, 3);
    map.insert("XXX", 30);
    map.insert("XXX", 30);
    map.insert("XXX", 30);
    map.print();

    map.insert_or_assign("XXX", 20);
    map.print();

    bool result = map.contains("XXX");
    result ? cout << "Yes" << endl : cout << "No" << endl;
    
    map.erase("XXX");
    map.print();

    string romanNumeral = "XXV"; // Roman numeral for 15
    int decimalNumber = my_namespace::romanToDecimal(romanNumeral);
    cout << "Roman numeral " << romanNumeral << " is equivalent to " << decimalNumber << " in decimal." << endl;
    return 0;
}
