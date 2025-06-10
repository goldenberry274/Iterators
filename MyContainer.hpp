#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace containers {

// ------------------- MyContainer ---------------------
template <typename T = int>
class MyContainer {
protected:
    vector<T> values; //Used for storing container values
    //Caches are here to ensure proper memory handling when using the container
    mutable vector<T> sorted_cache;  // used for ascending order
    mutable vector<T> decsend_cache; // used for decsending order
    mutable vector<T> sidecross_cache; // used for sidecorss order
    mutable vector<T> middle_cache; // used for middleout order

    void update_sorted_cache() const {
        sorted_cache = values;
        sort(sorted_cache.begin(), sorted_cache.end());
    }

    void update_decsend_cache() const{
        decsend_cache = values;
        sort(decsend_cache.begin(), decsend_cache.end(), less<T>());
    }

    void update_sidecross_cache() const {
        vector<T> sorted = values;
        sort(sorted.begin(), sorted.end());

        sidecross_cache.clear();
        int left = 0, right = sorted.size() - 1;
        bool fromLeft = true;

        while (left <= right) {
            if (fromLeft) {
                sidecross_cache.push_back(sorted[left++]);
            } else {
                sidecross_cache.push_back(sorted[right--]);
            }
            fromLeft = !fromLeft;
        }
    }

    void update_middle_cache() const {
        vector<T> sorted = values;
        middle_cache.clear();
        size_t n = sorted.size();
        if (n == 0) return;

        size_t mid = (n - 1) / 2;
        middle_cache.push_back(sorted[mid]);
        for (size_t offset = 1; mid >= offset || mid + offset < n; ++offset) {
            if (mid >= offset)
                middle_cache.push_back(sorted[mid - offset]);
            if (mid + offset < n)
                middle_cache.push_back(sorted[mid + offset]);
        }
    }

public:

    // ------------------ Iterators ------------------
    //The natural order: From the first element to the last
    class Order {
        using iterator = typename vector<T>::iterator;
        iterator it_;
    public:
        Order(iterator it) : it_(it) {}
        T& operator*() const { return *it_; }
        Order& operator++() { 
            ++it_; 
            return *this;
        }
        bool operator==(const Order& other) const { return it_ == other.it_; }
        bool operator!=(const Order& other) const { return !(*this == other); }
    };
    //The opposite of the natural order: From the last element to the first
    class ReverseOrder {
        using iterator = typename vector<T>::reverse_iterator;
        iterator it_;
    public:
        ReverseOrder(iterator it) : it_(it) {}
        T& operator*() const { return *it_; }
        ReverseOrder& operator++() { 
            ++it_; 
            return *this; 
        }
        bool operator==(const ReverseOrder& other) const { return it_ == other.it_; }
        bool operator!=(const ReverseOrder& other) const { return !(*this == other); }
    };
    //The ascending order: From the smallest element to the biggest
    class AscendingOrder {
        const vector<T>* data;
        typename vector<T>::const_iterator current;
    public:
        AscendingOrder(const vector<T>* data, bool end = false)
            : data(data), current(end ? data->end() : data->begin()) {}
        const T& operator*() const { return *current; }
        AscendingOrder& operator++() { 
            ++current;
            return *this; 
        }
        bool operator==(const AscendingOrder& other) const { return current == other.current; }
        bool operator!=(const AscendingOrder& other) const { return !(*this == other); }
    };
    //The descending order: From the biggest element to the smallest
    class DescendingOrder {
        const vector<T>* data;
        typename vector<T>::const_reverse_iterator current;
    public:
        DescendingOrder(const vector<T>* data, bool end = false)
            : data(data), current(end ? data->crend() : data->crbegin()) {}
        const T& operator*() const { return *current; }
        DescendingOrder& operator++() {
            ++current; 
            return *this; 
        }
        bool operator==(const DescendingOrder& other) const { return current == other.current; }
        bool operator!=(const DescendingOrder& other) const { return !(*this == other); }
    };
    //The sidecross order: Smallest, then biggest. Rince and repeat until the end of the vector
    class SideCrossOrder {
        const vector<T>* data;
        typename vector<T>::const_iterator it_;
    public:
        SideCrossOrder(const vector<T>* data, bool is_end)
            : data(data), it_(is_end ? data->end() : data->begin()) {}
        const T& operator*() const { return *it_; }
        SideCrossOrder& operator++() {
            ++it_; 
            return *this; 
        }
        bool operator==(const SideCrossOrder& other) const { return it_ == other.it_; }
        bool operator!=(const SideCrossOrder& other) const { return !(*this == other); }
    };
    //The MiddleOut order: The element in the middle, then to the left, then to the right, rince and repeat
    class MiddleOutOrder {
        const vector<T>* data;
        typename vector<T>::const_iterator it_;
    public:
        MiddleOutOrder(const vector<T>* data, bool is_end)
            : data(data), it_(is_end ? data->end() : data->begin()) {}
        const T& operator*() const { return *it_; }
        MiddleOutOrder& operator++() { 
            ++it_; 
            return *this; 
        }
        bool operator==(const MiddleOutOrder& other) const { return it_ == other.it_; }
        bool operator!=(const MiddleOutOrder& other) const { return !(*this == other); }
    };

    // ------------------ Interface ------------------
    // Add element to the container
    void add(const T& value) { values.push_back(value); }
    // Remove all instances of an element from the container
    void remove(const T& value) {
        bool found = false;
        auto it = values.begin();
        while (it != values.end()) {
            if (*it == value) {
                it = values.erase(it); // erase returns the next iterator
                found = true;
            } else {
                ++it;
            }
        }
        if (!found) {
            throw runtime_error("Cannot remove non-existent element");
        }
    }
    // Returns the size of the container
    size_t size() const { return values.size(); }
    // prints out the elements of the container
    void print_container() const {
        for (const T& val : values) cout << val << ", ";
        cout << endl;
    }
    //An output operator, prints out all the elements in the container
    friend ostream& operator<<(ostream& output, const MyContainer& mycon) {
        for (const T& val : mycon.values) output << val << ", ";
        output << endl;
        return output;
    }
    
    Order begin() { return Order(values.begin()); }
    Order end()   { return Order(values.end()); }

    ReverseOrder begin_reverse_order() { return ReverseOrder(values.rbegin()); }
    ReverseOrder end_reverse_order()   { return ReverseOrder(values.rend()); }

    AscendingOrder begin_ascending_order() const {
        update_sorted_cache();
        return AscendingOrder(&sorted_cache, false);
    }
    AscendingOrder end_ascending_order() const {
        update_sorted_cache();
        return AscendingOrder(&sorted_cache, true);
    }

    DescendingOrder begin_descending_order() const {
        update_decsend_cache();
        return DescendingOrder(&decsend_cache, false);
    }
    DescendingOrder end_descending_order() const {
        update_decsend_cache();
        return DescendingOrder(&decsend_cache, true);
    }

    SideCrossOrder begin_sidecross_order() const {
        update_sidecross_cache();
        return SideCrossOrder(&sidecross_cache, false);
    }
    SideCrossOrder end_sidecross_order() const {
        update_sidecross_cache();
        return SideCrossOrder(&sidecross_cache, true);
    }

    MiddleOutOrder begin_middle_order() const {
        update_middle_cache();
        return MiddleOutOrder(&middle_cache, false);
    }
    MiddleOutOrder end_middle_order() const {
        update_middle_cache();
        return MiddleOutOrder(&middle_cache, true);
    }
};

}
