#include <iostream>
#include <chrono>

#include "timer.h"

Timer::Timer()
{
    start = std::chrono::high_resolution_clock::now();
}
Timer::~Timer()
{
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout<<"Timer took " << duration.count() << "s" <<std::endl;
}
