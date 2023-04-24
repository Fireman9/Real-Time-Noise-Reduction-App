#include "Timer.h"

void Timer::start()
{
    m_start_time = high_resolution_clock::now();
}

void Timer::stop()
{
    m_end_time = high_resolution_clock::now();
}

double Timer::elapsedMilliseconds() const
{
    return duration_cast<milliseconds>(m_end_time - m_start_time).count();
}
