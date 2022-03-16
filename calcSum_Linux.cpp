#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <ctime>
#include<sys/time.h>
#include<unistd.h>
using namespace std;

const int maxN = 1 << 23;
int a[maxN];

int getRandom(int x) {
    return rand() % x;
}

void init(int n) {
    if(n > maxN) {
        cout << "ERROR: n > maxN" << endl;
        return;
    }
    for(int i = 0; i < n; i++) {
        a[i] = getRandom(100);
    }
}

int calc_1_Thread(int n) {
    int sum = 0;
    for(int i = 0; i < n; i++)
        sum += a[i];
    return sum;
}

int calc_2_Thread(int n) {
    int rt = 0;
    int sum[2];
    sum[0] = 0; sum[1] = 0;
    for(int i = 0; i < n; i += 2) {
        sum[0] += a[i]; sum[1] += a[i + 1];
    }
    rt = sum[0] + sum[1];
    return rt;
}

int calc_4_Thread(int n) {
    int rt = 0;
    int sum[4];
    sum[0] = 0; sum[1] = 0; sum[2] = 0; sum[3] = 0;
    for(int i = 0; i < n; i += 4) {
        sum[0] += a[i]; sum[1] += a[i + 1];
        sum[2] += a[i + 2]; sum[3] += a[i + 3];
    }
    rt = sum[0] + sum[1] + sum[2] + sum[3];
    return rt;
}

int calc_8_Thread(int n) {
    int rt = 0;
    int sum[8];
    sum[0] = 0; sum[1] = 0; sum[2] = 0; sum[3] = 0;
    sum[4] = 0; sum[5] = 0; sum[6] = 0; sum[7] = 0;
    for(int i = 0; i < n; i += 8) {
        sum[0] += a[i]; sum[1] += a[i + 1];
        sum[2] += a[i + 2]; sum[3] += a[i + 3];
        sum[4] += a[i + 4]; sum[5] += a[i + 5];
        sum[6] += a[i + 6]; sum[7] += a[i + 7];
    }
    rt = sum[0] + sum[1] + sum[2] + sum[3];
    rt += sum[4] + sum[5] + sum[6] + sum[7];
    return rt;
}

int calc_Merge(int n) {
    // a[] will be modify!
    for(int m = n; m > 1; m = m >> 1) {
        for(int i = 0; i < (m >> 1); i++) {
            a[i] = a[i << 1] + a[(i << 1) + 1];
        }
    }
    return a[0];
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

void operate(int threadNum) {
    struct timeval start, finish;
    double timeMS = 0.0;

    int sum = 0;
    for(int n = 1 << 3; n <= maxN; n = n << 1) {
        if(threadNum > n) {
            cout << "ERROR: threadNum > n" << endl;
            return;
        }

        int counter = 0;
        gettimeofday(&start, NULL);
        do {
            counter++;
            init(n);
            
            switch (threadNum) {
            case 1:
                calc_1_Thread(n);
                break;
            case 2:
                calc_2_Thread(n);
                break;
            case 4:
                calc_4_Thread(n);
                break;
            case 8:
                calc_8_Thread(n);
                break;
            case 0:
                calc_Merge(n);
                break;
            default:
                cout << "ERROR: Wrong Argument" << endl;
                return;
            }

            gettimeofday(&finish, NULL);
            timeMS = getTime(start, finish);
        }
        while(timeMS < 1e2 || counter < 3);  

        double aver_Time = timeMS / counter;
        printf("%4d\t%8.4f\t%4d\n", n, aver_Time, counter);
    }
    
}

int main() {
    srand(time(0));

    int threadNum = 8;
    int repeatTimes = 10;
    cout << threadNum << " Threads:" << endl;
    for(int i = 0; i < repeatTimes; i++) {
        cout << "(" << (i + 1) << "/" << repeatTimes << ")" << endl;

        switch(threadNum) {
        case 1:
            operate(1); 
            break;
        case 2:
            operate(2);
            break;
        case 4:
            operate(4);
            break;
        case 8:
            operate(8);
            break;
        case 0:
        default:
            operate(0);

        }
    }

    return 0;
}