#include <cstdio>
#include <ctime>
#include <fstream>
#include <array>
#include <iostream>

#define N 11

using namespace std;

typedef array<int, N> intarr;

typedef intarr (*sorting_pointer)(intarr, int);

typedef intarr (*generation_pointer)(int);

const string file_name = "sorting_time.csv";

struct data {
    string name;
    double time[9];
};

array<struct data, 29> time_array;

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
    tab[0] = random_numbers(1)[0];
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
    intarr tab = {random_numbers(1)[0]};
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

void mrg(intarr tab, int start, int n) {
    int helper[n];
    for (int i = start; i <= n; i++) {
        helper[i] = tab[i];
    }
    int firsthalf = start;
    int middle = n / 2;
    int secondhalf = middle + 1;
    int iterator = start;

    for (iterator; firsthalf <= secondhalf && middle <= n; iterator++) {
        if (tab[firsthalf] < tab[secondhalf]) {
            helper[iterator] = tab[firsthalf];
            firsthalf++;
        } else {
            helper[iterator] = tab[secondhalf];
            secondhalf++;
        }
    }
    for (int i = start; i <= n; i++) {
        tab[i] = helper[i];
    }
} //n = end of array we are sorting
void merge_sort(intarr tab, int start, int n) {
    int middle = n / 2;
    if (start < n) {
        merge_sort(tab, start, middle);
        merge_sort(tab, middle + 1, n);
        mrg(tab, start, n);
    }
}// start w main 0

int qsort(intarr tab, int start, int n) {
    int x = (start + n) / 2;
    int j = n;
    int i = start;

    while (true) {
        while (tab[j] > tab[x]) {
            j--;
        }
        while (tab[i] < tab[x]) {
            i++;
        }
        if (i < j) {
            int helper = tab[j];
            tab[j] = tab[i];
            tab[i] = helper;
            j--;
            i++;
        } else {
            return j;
        }
    }
}

void quick_sort(intarr tab, int start, int n) {
    if (start < n) {
        int middle = qsort(tab, start, n);
        quick_sort(tab, start, n);
        quick_sort(tab, middle + 1, n);
    }
}

void quick_sort_iterative(intarr tab, int start, int n) {

}

double insertion_time(intarr tab, int n) {
    clock_t my_clock;
    my_clock = clock();
    insertion_sort(tab, n);
    my_clock = clock() - my_clock;
    double time = (double(my_clock)) / CLOCKS_PER_SEC;
    return time;
}

double bubble_time(intarr tab, int n) {
    clock_t my_clock = clock();
    bubble_sort(tab, n);
    my_clock = clock() - my_clock;
    double time = (double(my_clock)) / CLOCKS_PER_SEC;
    return time;
}

double selection_time(intarr tab, int n) {
    clock_t my_clock = clock();
    selection_sort(tab, n);
    my_clock = clock() - my_clock;
    double time = (double(my_clock)) / CLOCKS_PER_SEC;
    return time;
}

double merge_time(intarr tab, int n) {
    clock_t my_clock = clock();
    merge_sort(tab, 0, n);
    my_clock = clock() - my_clock;
    double time = (double(my_clock)) / CLOCKS_PER_SEC;
    return time;
}

double quick_time(intarr tab, int n) {
    clock_t my_clock = clock();
    quick_sort(tab, 0, n);
    my_clock = clock() - my_clock;
    double time = (double(my_clock)) / CLOCKS_PER_SEC;
    return time;
}

void insertion(intarr helper, int index, int n) {
    for (int i = 0; i < 10; i++) {
        time_array[index].time[i] = insertion_time(helper, n);
        n += 20000;
    }
}

void bubble(intarr helper, int index, int n) {
    for (int i = 0; i < 10; i++) {
        time_array[index].time[i] = bubble_time(helper, n);
        n += 20000; //hello is it me you lookin for
    }
}

void selection(intarr helper, int index, int n) {
    for (int i = 0; i < 10; i++) {
        time_array[index].time[i] = selection_time(helper, n);
        n += 20000;//helo is it me you lookin for
    }
}

void merge(intarr helper, int index, int n) {
    for (int i = 0; i < 10; i++) {
        time_array[index].time[i] = merge_time(helper, n);
        n += 20000;//helo is it me you lookin for
    }
}

void save_to_file() {
    ofstream base;
    base.open(file_name);
    for (int i = 0; i < 20; i++) {
        base << time_array[i].name << ",\n";
        for (int j = 0; j < 10; j++) {
            base << time_array[i].time[j] << ",";
        }
    }
    base.close();
}

bool issorted(intarr tab) {
    for (intarr::iterator it = tab.begin() + 1; it != tab.end(); it++) {
        if (*(it - 1) > *it) return false;
    }
    return true;
}

int main() {
    typedef array<sorting_pointer, 3> sorting_array;
    typedef array<generation_pointer, 5> generators_array;
    sorting_array sorting_functions = {bubble_sort, insertion_sort, selection_sort};
    generators_array generators_functions = {random_numbers, increasing_numbers, decreasing_numbers, constant_numbers,
                                             A_shape_numbers};

    for (generators_array::iterator gen_func = generators_functions.begin();
         gen_func != generators_functions.end(); gen_func++) {
        intarr unsorted = (*gen_func)(N);
        for (sorting_array::iterator func = sorting_functions.begin(); func != sorting_functions.end(); func++) {
            time_t temp = time(NULL);
            int i = 1000000;
            while (i --> 0)(*func)(unsorted, N);
            cout << time(NULL) - temp << " ";
        }
        cout << endl;
    }


    return 0;
}