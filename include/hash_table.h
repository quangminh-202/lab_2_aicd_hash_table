#pragma once
#include <iostream>
#include <string>
#include <random>
#include <stdexcept>
#include <functional> //for hash()
#include <vector>
#include <cmath> // floor

using namespace std;

namespace my_namespace {

    // Мультипликативная схема hash function 
    // https://drive.google.com/file/d/1ijBV8tm7xdLpWnvRvPiX2cNSK0Hsor6A/view       
    template<typename Key>
    size_t hashFunction(const Key& key, size_t tableSize) {
        const size_t w = sizeof(size_t) * 8; // Размер машинного слова
        const size_t a = 65; // Целая константа, взаимно простая с w
        size_t k = hash<Key>{}(key);// Преобразуем ключ в целое число
        // Вычисляем произведение ak и берем его дробную часть
        double akw = (a / static_cast<double>(w)) * static_cast<double>(k);
        double akw01 = akw - floor(akw);
        // Умножаем на размер таблицы и берем целую часть
        size_t index = static_cast<size_t>(floor(tableSize * akw01));
        return index;
    }

    int romanToDecimal(const std::string& roman);

    template<typename Key, typename Value>
    class MyUnorderedMap {
        struct Pair {
            Key key;
            Value value;
            bool empty;

            Pair() : key(), value(), empty(true) {}

            Pair(Key k, Value v) : key(k), value(v), empty(false) {}
        };

        std::vector<Pair> _data;
        size_t _size;

        Pair* find(const Key& key) {
            size_t index = hashFunction(key, _data.size());
            size_t count = 0; 
            while (count < _data.size()) { 
                if (_data[index].empty || (_data[index].key == key && !_data[index].empty))
                    return &_data[index];
                index = (index + 1) % _data.size();
                count++; 
            }
            return nullptr;
        }


    public:
        MyUnorderedMap(size_t size) : _size(0) {
            if (size == 0)
                throw std::invalid_argument("size = 0.");
            _data.resize(size);
        }


        MyUnorderedMap(size_t size, size_t num_random_pairs) : _size(0) {
            if (size == 0)
                throw std::invalid_argument("size = 0.");
            _data.resize(size);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::vector<std::string> roman_numerals = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X" }; 
            std::uniform_int_distribution<size_t> dis(0, roman_numerals.size() - 1);
            for (size_t i = 0; i < num_random_pairs; ++i) {
                size_t random_index = dis(gen);
                std::string random_roman = roman_numerals[random_index];
                int decimal_value = romanToDecimal(random_roman);
                if (decimal_value != -1) {
                    insert(random_roman, decimal_value);
                }
                else {
                    cerr << "Invalid Roman numeral: " << random_roman << endl;
                }
            }
        }


        MyUnorderedMap(const MyUnorderedMap& other){
            if (other._size == 0)
                throw std::invalid_argument("size = 0.");
            _size = other._size;
            _data.resize(other._size);
            for (size_t i = 0; i < _size; ++i) {
                if (!other._data[i].empty)
                    _data[i] = other._data[i];
            }
        }


        ~MyUnorderedMap() {}


        MyUnorderedMap& operator=(const MyUnorderedMap& other) {
            if (this != &other) {
                _data = other._data;
                _size = other._size;
            }
            return *this;
        }


        void print() {
            for (const auto& node : _data) {
                if (!node.empty)
                    cout << "Key: " << node.key << ", Value: " << node.value << endl;
            }
            cout << endl;
        }


        void grow() {
            std::vector<Pair> new_data;
            new_data.resize(static_cast<size_t>(_data.size() * 1.3f));
            std::swap(_data, new_data);
            _size = 0;
            for (size_t i = 0; i < new_data.size(); ++i) {
                if (!new_data[i].empty)
                    insert(new_data[i].key, new_data[i].value);
            }
        }


        bool insert(const Key& key, const Value& value) {
            double load_factor = _size / (static_cast<double>(_data.size()));
            if (load_factor > 0.8)
                grow();
            Pair* found = find(key);
            if (found != nullptr && !found->empty)
                return false; // Key already exists
            size_t index = hashFunction(key, _data.size());
            while (!_data[index].empty) {
                index = (index + 1) % _data.size();
            }
            _data[index].key = key;
            _data[index].value = value;
            _data[index].empty = false;
            ++_size;
            return true;
        }


        bool insert_or_assign(const Key& key, const Value& value) {
            Pair* found = find(key);
            if (found != nullptr) {
                found->value = value;
                return true; // Key exists, value updated
            }
            size_t index = hashFunction(key, _data.size());
            while (_data[index].empty) {
                index = (index + 1) % _data.size();
            }
            _data[index] = Pair(key, value);
            _size++;
            return true;
        }


        bool contains(const Value& value) {
            for (const auto& pair : _data) {
                if (!pair.empty && pair.value == value)
                    return true;
            }
            return false;
        }


        Value* search(const Key& key) {
            Pair* found = find(key);
            if (found != nullptr && !found->empty) 
                return &(found->value);
            return nullptr;
        }


        bool erase(const Key& key) {
            Pair* found = find(key);
            if (found == nullptr)
                return false; 
            found->empty = true;
            _size--;
            return true;
        }

       
        int count(const Key& key) {
            Pair* found = find(key);
            if (found == nullptr)
                return -1; 
            return 1; 
        }

        size_t size() {
            return _size;
        }

        bool check(const Key& key) {
            return find(key) != nullptr;
        }
    };


    int romanToDecimal(const string& roman) {
        my_namespace::MyUnorderedMap<char, int> romanMap(7);
        romanMap.insert('I', 1);
        romanMap.insert('V', 5);
        romanMap.insert('X', 10);
        romanMap.insert('L', 50);
        romanMap.insert('C', 100);
        romanMap.insert('D', 500);
        romanMap.insert('M', 1000);

        int decimal = 0;
        int prevValue = 0;

        for (int i = roman.length() - 1; i >= 0; --i) {
            char currentChar = roman[i];
            if (romanMap.check(currentChar)) {
                int value = *(romanMap.search(currentChar));
                if (value < prevValue) {
                    decimal -= value;
                }
                else {
                    decimal += value;
                }
                prevValue = value;
            }
            else {
                cerr << "No value found for characters: " << currentChar << endl;
                return -1;
            }
        }
        return decimal;
    }
}