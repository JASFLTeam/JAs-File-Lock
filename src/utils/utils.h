#ifndef UTILS_H
#define UTILS_H
#include <chrono>

namespace ut
{

/*
clase tiempo para resumir chrono
*/
class tiempo
{
    public:
        void inicio();
        void fin();
        double cuanto();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> _inicio;
        std::chrono::time_point<std::chrono::high_resolution_clock> _fin;
};
}

#endif