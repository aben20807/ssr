```bash
$ g++-8 -std=c++1z -Wall main.cpp -lpthread -lboost_system -lboost_thread -lboost_chrono
$ time ./a.out 0 & ./a.out 1
[2] 8080
snd[0]: 5
snd[size-1]: -5
rcv[0]: 5
rcv[size-1]: -5
snd[0]: 4
snd[size-1]: -4
rcv[0]: 4
rcv[size-1]: -4
snd[0]: 3
snd[size-1]: -3
rcv[0]: 3
rcv[size-1]: -3
snd[0]: 500
snd[size-1]: -500
rcv[0]: 500
rcv[size-1]: -500
snd[0]: 2
snd[size-1]: -2
rcv[0]: 2
rcv[size-1]: -2

real  0m0.220s
user  0m0.068s
sys   0m0.119s
```
