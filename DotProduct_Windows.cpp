#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <windows.h>
using namespace std;

const int maxN = 10500;
int sum[maxN];
int a[maxN], b[maxN][maxN];
bool cacheTest = true;

int getRandom(int x) {
    return rand() % x;
}

void init(int n) {
    memset(sum, 0, sizeof(sum));

    for(int i = 0; i < n; i++) {
        a[i] = getRandom(100);
        for(int j = 0; j < n; j++) {
            b[i][j] = getRandom(100);
        }
    }
}

void calc_ColOrder(int n) {
    for(int j = 0; j < n; j++) {
        for(int i = 0; i < n; i++) {
            sum[j] += b[i][j] * a[i];
        }
    }
}

void calc_RowOrder(int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            sum[j] += b[i][j] * a[i];
        }
    }
}

double getTime(long long start, long long finish, long long freq) {
    // ms
    double duration = (finish - start) * 1000.0 / freq;
    return duration;
}

void showResult(int n) {
    for(int i = 0; i < n; i++) {
        printf("%3d ", sum[i]);
    }
    cout << endl;
}

void operate(bool colOrder) {
    int step = 10;
    long long start, finish, freq;
    double timeMS = 0;

    int max_n;
    if(cacheTest) max_n = 1500;
    else max_n = maxN;

    for (int n = 0; n <= max_n; n += step)
    {
        QueryPerformanceFrequency((LARGE_INTEGER*) &freq);

        int counter = 0;
        QueryPerformanceCounter((LARGE_INTEGER*) &start);
        do {
            counter++;
            init(n);
            if(colOrder) calc_ColOrder(n);
            else calc_RowOrder(n);

            QueryPerformanceCounter((LARGE_INTEGER*) &finish);
            timeMS = getTime(start, finish, freq);
        }
        while(timeMS < 100 || counter < 3);
        if(cacheTest) printf("%d\t%10.4f\n", n, timeMS / counter);    
        else printf("%d\t%10.4f\t%4d\n", n, timeMS / counter, counter);

        if(cacheTest) continue;
        if(n == 100) step = 100;
        if(n == 1000) step = 1000;
    }
}

int main() {
    srand(time(0));
    cacheTest = false;

    cout << "Column Order:" << endl;
    operate(true);

    cout << endl << "Row Order:" << endl;
    operate(false);

    return 0;
}