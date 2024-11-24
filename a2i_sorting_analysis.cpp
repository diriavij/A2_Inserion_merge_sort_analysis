#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

class ArrayGenerator {
    size_t size_;
    int min_value_;
    int max_value_;
public:
    explicit ArrayGenerator(size_t size, int min_value = 0, int max_value = 6000) : size_(size),
    min_value_(min_value), max_value_(max_value) {
    }

    [[nodiscard]] std::vector<int> getBasicArray() const {
        std::vector<int> return_array(size_);
        for (size_t i = 0; i < size_; i++) {
            return_array[i] = std::rand() % (max_value_ - min_value_ + 1) + min_value_;
        }
        return return_array;
    }

    [[nodiscard]] std::vector<int> getReversedArray() const {
        std::vector<int> return_array = getBasicArray();
        std::ranges::sort(return_array, std::greater<int>());
        return return_array;
    }

    std::vector<int> getAlmostSortedArray() {
        std::vector<int> return_array = getBasicArray();
        std::ranges::sort(return_array);
        size_t swaps = size_ / 100;
        for (size_t i = 0; i < swaps; i++) {
            std::swap(return_array[std::rand() % size_], return_array[std::rand() % size_]);
        }
        return return_array;
    }

};

void merge(std::vector<int>& container, int left, int mid, int right) {
    int size_left = mid - left + 1;
    int size_right = right - mid;
    std::vector<int> left_half(size_left);
    std::vector<int> right_half(size_right);
    for (int i = 0; i < size_left; ++i)
        left_half[i] = container[left + i];
    for (int j = 0; j < size_right; ++j)
        right_half[j] = container[mid + 1 + j];
    int i = 0;
    int j = 0;
    int k = left;
    while (i < size_left && j < size_right) {
        if (left_half[i] <= right_half[j]) {
            container[k] = left_half[i];
            ++i;
        } else {
            container[k] = right_half[j];
            ++j;
        }
        ++k;
    }
    while (i < size_left) {
        container[k] = left_half[i];
        ++i;
        ++k;
    }
    while (j < size_right) {
        container[k] = right_half[j];
        ++j;
        ++k;
    }
}

void insertionSort(std::vector<int>& container, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = container[i];
        int j = i - 1;
        while (j >= left && container[j] > key) {
            container[j + 1] = container[j];
            --j;
        }
        container[j + 1] = key;
    }
}

void mergeSort(std::vector<int>& container, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(container, left, mid);
        mergeSort(container, mid + 1, right);
        merge(container, left, mid, right);
    }
}

void upgradedSort(std::vector<int>& container, int left, int right) {
    if (left < right) {
        if (right - left > 15) {
            int mid = left + (right - left) / 2;
            upgradedSort(container, left, mid);
            upgradedSort(container, mid + 1, right);
            merge(container, left, mid, right);
        } else {
            insertionSort(container, left, right);
        }
    }
}

class SortTester {
public:
    static long long runMergeSort(std::vector<int> tester_array) {
        size_t left = 0;
        size_t right = tester_array.size() - 1;
        auto start = std::chrono::high_resolution_clock::now();
        mergeSort(tester_array, left, right);
        auto elapsed = std::chrono::high_resolution_clock::now()- start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    static long long runUpgradedSort(std::vector<int> tester_array) {
        size_t left = 0;
        size_t right = tester_array.size() - 1;
        auto start = std::chrono::high_resolution_clock::now();
        upgradedSort(tester_array, left, right);
        auto elapsed = std::chrono::high_resolution_clock::now()- start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }
};



int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    ArrayGenerator generator(10000);
    std::vector<int> basic_array = generator.getBasicArray();
    std::vector<int> reversed_array = generator.getReversedArray();
    std::vector<int> almost_sorted_array = generator.getAlmostSortedArray();
    std::cout << "BASIC ARRAY\n" << std::endl;
    for (int array_size = 500; array_size <= 10000; array_size += 100) {
        std::vector<int> tester_array(basic_array.begin(), basic_array.begin() + array_size);
        long long microsec_first = SortTester::runMergeSort(tester_array);
        long long microsec_second = SortTester::runUpgradedSort(tester_array);
        std::cout << array_size << " " << microsec_first << " " << microsec_second << std::endl;
    }
    std::cout << "\nREVERSED ARRAY\n" << std::endl;
    for (int array_size = 500; array_size <= 10000; array_size += 100) {
        std::vector<int> tester_array(reversed_array.begin(), reversed_array.begin() + array_size);
        long long microsec_first = SortTester::runMergeSort(tester_array);
        long long microsec_second = SortTester::runUpgradedSort(tester_array);
        std::cout << array_size << " " << microsec_first << " " << microsec_second << std::endl;
    }
    std::cout << "\nALMOST SORTED ARRAY\n" << std::endl;
    for (int array_size = 500; array_size <= 10000; array_size += 100) {
        std::vector<int> tester_array(almost_sorted_array.begin(), almost_sorted_array.begin() + array_size);
        long long microsec_first = SortTester::runMergeSort(tester_array);
        long long microsec_second = SortTester::runUpgradedSort(tester_array);
        std::cout << array_size << " " << microsec_first << " " << microsec_second << std::endl;
    }
    return 0;
}
