#include <chrono>

struct Timer
{
    std::chrono::high_resolution_clock::time_point start, end;
    std::chrono::duration<double> duration;

    Timer();

    ~Timer();
};
