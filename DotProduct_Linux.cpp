#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include<sys/time.h>
using namespace std;

const int maxN = 10500;
int sum[maxN];
int a[maxN], b[maxN][maxN];
bool cacheTest = false;

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

double getTime(struct timeval start, struct timeval finish) {
    // ms
    double duration = 0.0;
    long long startUsec, finishUsec;
    startUsec = start.tv_sec * 1e6 + start.tv_usec;
    finishUsec = finish.tv_sec * 1e6 + finish.tv_usec;
    duration = (finishUsec - startUsec) / 1e3;
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
    struct timeval start, finish;
    double timeMS = 0.0;

    int max_n;
    if(cacheTest) max_n = 1500;
    else max_n = maxN;

    for (int n = 0; n <= max_n; n += step)
    {
        int counter = 0;
        gettimeofday(&start, NULL);
        do {
            counter++;
            init(n);
            if(colOrder) calc_ColOrder(n);
            else calc_RowOrder(n);

            gettimeofday(&finish, NULL);
            timeMS = getTime(start, finish);
        }
        while(timeMS < 1e2); 

        double aver_Time = timeMS / counter;
        if(cacheTest) printf("%d\t%10.4f\n", n, timeMS / counter);    
        else printf("%4d\t%10.4f\t%4d\n", n, timeMS / counter, counter);

        if(cacheTest) continue;
        if(n == 100) step = 100;
        if(n == 1000) step = 1000;
    }
}

int main() {
    srand(time(0));

    cout << "Column Order:" << endl;
    operate(true);

    cout << endl << "Row Order:" << endl;
    operate(false);

    return 0;
}