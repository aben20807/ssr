```bash
$ g++-8 -std=c++1z main.cpp -lboost_system -lboost_thread -lboost_chrono
$ time ./a.out 0 & ./a.out 1
[8] 22899
snd[0]: 5
rcv[0]: 5
snd[0]: 4
rcv[0]: 4
snd[0]: 3
rcv[0]: 3
snd[0]: 500
rcv[0]: 500
snd[0]: 2
rcv[0]: 2

real  0m0.395s
user  0m0.036s
sys   0m0.141s
```
