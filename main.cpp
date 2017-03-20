#include <cstdio>
#include <chrono>
#include <fstream>
#include <array>
#include <iostream>
#include <list>

#define N 200000

using namespace std;

typedef array<int, N> intarr;

typedef intarr (*sorting_pointer)(intarr, int);

typedef intarr (*generation_pointer)(int);

intarr random_numbers(int n) {
    srand((unsigned int) time(0));
    intarr tab;
    for (int i = 0; i < n; i++) {
        tab[i] = rand() % 100 * (rand() * 100 < 50 ? -1 : 1);
    }
    return tab;
}

intarr increasing_numbers(int n) {
    srand((unsigned int) time(0));
    intarr tab;
    tab = random_numbers(1);
    for (int i = 1; i < n; i++) {
        tab[i] = tab[i - 1] + rand() % 100;
    }
    return tab;
}

intarr decreasing_numbers(int n) {
    srand((unsigned int) time(0));
    intarr tab;
    tab = random_numbers(1);
    for (int i = 1; i < n; i++) {
        tab[i] = tab[i - 1] - rand() % 100;
    }
    return tab;
}

intarr constant_numbers(int n) {
    intarr tab;
    tab.fill(random_numbers(1)[0]);
    return tab;
}

intarr A_shape_numbers(int n) {
    srand((unsigned int) time(0));
    intarr tab = random_numbers(1);
    for (int i = 1; i < n / 2 + 1; i++) {
        tab[i] = tab[i - 1] + (rand() % 100);
    }
    for (int i = (n / 2) + 1; i < n; i++) {
        tab[i] = tab[i - 1] - (rand() % 100);
    }
    return tab;
}

intarr insertion_sort(intarr tab, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i; j >= 0; j--) {
            if (tab[j + 1] < tab[j])
                swap(tab[j + 1], tab[j]);
        }
    }
    return tab;
}

intarr bubble_sort(intarr tab, int n) {
    bool swapped = true;
    for (int i = 0; i < n - 1 && swapped; i++) {
        swapped = false;
        for (int j = 0; j < n - (i + 1); j++) {
            if (tab[j] > tab[j + 1]) {
                swap(tab[j], tab[j + 1]);
                swapped = true;
            }
        }
    }
    return tab;
}

intarr selection_sort(intarr tab, int n) {
    for (int i = 0; i < n; i++) {
        int smallest = i;
        for (int j = i + 1; j < n; j++) {
            if (tab[j] < tab[smallest])
                smallest = j;
        }
        swap(tab[i], tab[smallest]);
    }
    return tab;
}

list<int> merge(list<int> a, list<int> b) {
    list<int> result;
    list<int>::iterator it1 = a.begin(), it2 = b.begin();
    while (it1 != a.end() && it2 != b.end()) {
        if ((*it1) < (*it2))
            result.push_back(*it1++);
        else
            result.push_back(*it2++);

    }
    if (it1 == a.end())
        while (it2 != b.end())
            result.push_back(*it2++);

    if (it2 == b.end())
        while (it1 != a.end())
            result.push_back(*it1++);


    return result;
}

list<int> merge_sort_backend(list<int> tab) {
    if (tab.size() == 1) return tab;
    list<int> a, b;
    int i = 0;
    list<int>::iterator it = tab.begin();
    while (i < tab.size() / 2) {
        a.push_back(*it++);
        i++;
    }
    while (i < tab.size()) {
        b.push_back(*it++);
        i++;
    }

    return merge(merge_sort_backend(a), merge_sort_backend(b));
}// start w main 0

intarr merge_sort(intarr tab, int n) {
    list<int> numbers;
    for (int i = 0; i < n; i++) numbers.push_back(tab[i]);
    numbers = merge_sort_backend(numbers);
    intarr ret;
    int i = 0;
    for (list<int>::iterator it = numbers.begin(); it != numbers.end(); it++) ret[i++] = *it;
    return ret;
}

list<int> quick_sort_backend(list<int> nrs) {
    if (nrs.size() < 2) return nrs;
    unsigned long split_index = rand() % nrs.size();
    list<int>::iterator temp = nrs.begin();
    for (int i = 0; i < split_index; i++) temp++;
    int split_value = *temp;
    unsigned int split_count = 0;
    list<int> l, r;
    for (list<int>::iterator it = nrs.begin(); it != nrs.end(); it++) {
        if (*it < split_value) l.push_back(*it);
        else if (*it == split_value) split_count++;
        else r.push_back(*it);
    }
    list<int> res = quick_sort_backend(l);
    while (split_count-- > 0) res.push_back(split_value);
    res.splice(res.end(), quick_sort_backend(r));
    return res;
}

intarr quick_sort(intarr tab, int n) {
    list<int> numbers;
    for (int i = 0; i < n; i++) numbers.push_back(tab[i]);
    numbers = quick_sort_backend(numbers);
    intarr ret;
    int i = 0;
    for (list<int>::iterator it = numbers.begin(); it != numbers.end(); it++) ret[i++] = *it;
    return ret;
}

int main() {
    typedef array<sorting_pointer, 5> sorting_array;
    typedef array<generation_pointer, 5> generators_array;
    sorting_array sorting_functions = {bubble_sort, insertion_sort, selection_sort, merge_sort, quick_sort};
    generators_array generators_functions = {random_numbers, increasing_numbers, decreasing_numbers, constant_numbers,
                                             A_shape_numbers};

    ofstream results;
    results.open("sorting_time.csv");
    for (int n = 2000; n < N; n += 2000) {
        results << n << endl;
        for (generators_array::iterator gen_func = generators_functions.begin();
             gen_func != generators_functions.end(); gen_func++) {
            intarr unsorted = (*gen_func)(N);
            for (sorting_array::iterator func = sorting_functions.begin(); func != sorting_functions.end(); func++) {
                chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
                int i = 1;
                while (i-- > 0) (*func)(unsorted, n);
                chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
                long duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
                results << duration << ",";
            }
            results << endl;
        }
    }
    results.close();
//    list<int> b = {3, 1, 8, -8};
//    list<int> a = quick_sort_backend(b);
//    for (list<int>::iterator it=a.begin(); it != a.end(); it++) cout << *it << endl;
    return 0;
}