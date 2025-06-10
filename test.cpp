#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Animal.hpp"
#include "MyContainer.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>

using namespace containers;
using namespace std;


// Helper to collect iterator output
template<typename Iterator>
auto collect(Iterator begin, Iterator end) {
    using RawT = typename std::remove_cv<typename std::remove_reference<decltype(*begin)>::type>::type;
    std::vector<RawT> result;
    for (; begin != end; ++begin)
        result.push_back(*begin);
    return result;
}
template<typename T>
void fill_container(MyContainer<T>& c) {
    vector<T> values;
    if constexpr (is_same<T, int>::value) {
        values = {3, 1, 4, 2};
    } else if constexpr (is_same<T, char>::value) {
        values = {'d', 'a', 'c', 'b'};
    } else if constexpr (is_same<T, double>::value) {
        values = {2.2, 0.5, 1.1, 3.3};
    } else if constexpr (is_same<T, string>::value) {
        values = {"dog", "apple", "cat", "banana"};
    }
    for (const auto& val : values) c.add(val);
}

template<typename T>
void test_all_iterators() {
    MyContainer<T> c;
    fill_container(c);
    vector<T> expected;
    if constexpr (is_same<T, int>::value) expected = {3,1,4,2};
    else if constexpr (is_same<T, char>::value) expected = {'d','a','c','b'};
    else if constexpr (is_same<T, double>::value) expected = {2.2, 0.5, 1.1, 3.3};
    else if constexpr (is_same<T, string>::value) expected = {"dog","apple","cat","banana"};

    SUBCASE("Natural Order") {
        auto result = collect(c.begin(), c.end());
        CHECK(result == expected);
    }
    SUBCASE("Reverse Order") {
        auto result = collect(c.begin_reverse_order(), c.end_reverse_order());
        reverse(expected.begin(), expected.end());
        CHECK(result == expected);
    }
    SUBCASE("Ascending Order") {
        auto result = collect(c.begin_ascending_order(), c.end_ascending_order());
        sort(expected.begin(), expected.end());
        CHECK(result == expected);
    }
    SUBCASE("Descending Order") {
        auto result = collect(c.begin_descending_order(), c.end_descending_order());
        sort(expected.begin(), expected.end(), greater<T>());
        CHECK(result == expected);
    }
    SUBCASE("SideCross Order") {
        auto result = collect(c.begin_sidecross_order(), c.end_sidecross_order());
        sort(expected.begin(), expected.end());
        vector<T> reordered;
        size_t left = 0, right = expected.size() - 1;
        bool fromLeft = true;
        while (left <= right) {
            if (fromLeft) reordered.push_back(expected[left++]);
            else reordered.push_back(expected[right--]);
            fromLeft = !fromLeft;
        }
        CHECK(result == reordered);
    }
    SUBCASE("MiddleOut Order") {
        auto result = collect(c.begin_middle_order(), c.end_middle_order());

        vector<T> reordered;
        size_t n = expected.size();
        size_t mid = (n - 1) / 2;

        reordered.push_back(expected[mid]);
        for (size_t offset = 1; mid >= offset || mid + offset < n; ++offset) {
            if (mid >= offset)
                reordered.push_back(expected[mid - offset]);
            if (mid + offset < n)
                reordered.push_back(expected[mid + offset]);
        }

        CHECK(result == reordered);
    }
}

TEST_CASE("Basic operations") {
    MyContainer<int> c;
    CHECK(c.size() == 0);
    c.add(10); c.add(20); c.add(30);
    CHECK(c.size() == 3);
    c.remove(20);
    CHECK(c.size() == 2);
    CHECK_THROWS(c.remove(99));
}

TEST_CASE("Iterators - multiple types") {
    test_all_iterators<int>();
    test_all_iterators<char>();
    test_all_iterators<double>();
    test_all_iterators<string>();
}

TEST_CASE("Single element behavior") {
    MyContainer<string> c;
    c.add("one");

    CHECK(collect(c.begin(), c.end()) == vector<string>{"one"});
    CHECK(collect(c.begin_reverse_order(), c.end_reverse_order()) == vector<string>{"one"});
    CHECK(collect(c.begin_ascending_order(), c.end_ascending_order()) == vector<string>{"one"});
    CHECK(collect(c.begin_descending_order(), c.end_descending_order()) == vector<string>{"one"});
    CHECK(collect(c.begin_sidecross_order(), c.end_sidecross_order()) == vector<string>{"one"});
    CHECK(collect(c.begin_middle_order(), c.end_middle_order()) == vector<string>{"one"});
}

TEST_CASE("MiddleOut pattern check") {
    MyContainer<int> c;
    c.add(9); c.add(5); c.add(7); c.add(3); c.add(1); // Unordered input

    // Expected order: sorted input is {1,3,5,7,9}
    // MiddleOut should be: 5 (index 2), then 3 (1), 7 (3), 1 (0), 9 (4)
    vector<int> expected = {7, 5, 3, 9, 1};

    auto result = collect(c.begin_middle_order(), c.end_middle_order());
    CHECK(result == expected);
}

TEST_CASE("Empty container behavior") {
    MyContainer<int> c;
    CHECK(c.size() == 0);

    CHECK(collect(c.begin(), c.end()).empty());
    CHECK(collect(c.begin_reverse_order(), c.end_reverse_order()).empty());
    CHECK(collect(c.begin_ascending_order(), c.end_ascending_order()).empty());
    CHECK(collect(c.begin_descending_order(), c.end_descending_order()).empty());
    CHECK(collect(c.begin_sidecross_order(), c.end_sidecross_order()).empty());
    CHECK(collect(c.begin_middle_order(), c.end_middle_order()).empty());
}

TEST_CASE("Add and Remove methods") {
    MyContainer<int> c;
    c.add(1);
    c.add(2);
    c.add(2);
    c.add(3);
    c.add(2);

    CHECK(c.size() == 5);

    c.remove(2); // Should remove all 2s
    CHECK(c.size() == 2);

    auto result = collect(c.begin(), c.end());
    vector<int> expected = {1, 3};
    CHECK(result == expected);

    // Check that removing a non-existent element throws
    CHECK_THROWS(c.remove(999));
}

TEST_CASE("Size method correctness") {
    MyContainer<string> c;
    CHECK(c.size() == 0);

    c.add("a");
    CHECK(c.size() == 1);

    c.add("b");
    c.add("c");
    CHECK(c.size() == 3);

    c.remove("b");
    CHECK(c.size() == 2);

    c.remove("a");
    c.remove("c");
    CHECK(c.size() == 0);
}
