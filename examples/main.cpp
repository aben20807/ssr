#include "../ssr.h"
#define p(x) std::cout << #x << ": " << x << std::endl

int main(int argc, char *argv[])
{
    ssr::e_role role = atoi(argv[1]) == 0 ? ssr::SENDER : ssr::RECEIVER;
    ssr::Communicator<float> *float_comm = ssr::init<float>(role, "127.0.0.1", 8877);
    const uint32_t size = 10000000;
    if (role == ssr::SENDER) {
        float* snd = new float[size]();
        snd[0] = 5.0;
        snd[size-1] = -5.0;
        float_comm->send(snd, size);
        p(snd[0]);
        p(snd[size-1]);
        delete[] snd;
    } else {
        float* rcv = new float[size]();
        float_comm->receive(rcv, size);
        p(rcv[0]);
        p(rcv[size-1]);
        delete[] rcv;
    }

    if (role == ssr::RECEIVER) {
        float* rcv = new float[size]();
        float_comm->receive(rcv, size);
        p(rcv[0]);
        p(rcv[size-1]);
        delete[] rcv;
    } else {
        float* snd = new float[size]();
        snd[0] = 4.0;
        snd[size-1] = -4.0;
        float_comm->send(snd, size);
        p(snd[0]);
        p(snd[size-1]);
        delete[] snd;
    }

    if (role == ssr::SENDER) {
        float* snd = new float[size]();
        snd[0] = 3.0;
        snd[size-1] = -3.0;
        float_comm->send(snd, size);
        p(snd[0]);
        p(snd[size-1]);
        delete[] snd;
    } else {
        float* rcv = new float[size]();
        float_comm->receive(rcv, size);
        p(rcv[0]);
        p(rcv[size-1]);
        delete[] rcv;
    }


    ssr::Communicator<long> *long_comm = ssr::init<long>(role, "127.0.0.1", 8787);
    if (role == ssr::SENDER) {
        long* snd = new long[size]();
        snd[0] = 500;
        snd[size-1] = -500;
        long_comm->send(snd, size);
        p(snd[0]);
        p(snd[size-1]);
        delete[] snd;
    } else {
        long* rcv = new long[size]();
        long_comm->receive(rcv, size);
        p(rcv[0]);
        p(rcv[size-1]);
        delete[] rcv;
    }


    if (role == ssr::SENDER) {
        float* snd = new float[size]();
        snd[0] = 2.0;
        snd[size-1] = -2.0;
        float_comm->send(snd, size);
        p(snd[0]);
        p(snd[size-1]);
        delete[] snd;
    } else {
        float* rcv = new float[size]();
        float_comm->receive(rcv, size);
        p(rcv[0]);
        p(rcv[size-1]);
        delete[] rcv;
    }
    delete long_comm;
    delete float_comm;
    return 0;
}
