#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::time_point;

/// @brief Class for measuring the execution time of a piece of code.
class Timer
{
  private:
    /// @brief The start time point of the timer
    time_point<high_resolution_clock> m_start_time;

    /// @brief The end time point of the timer.
    time_point<high_resolution_clock> m_end_time;

  public:
    /// @brief Start the timer.
    void start();

    /// @brief Stop the timer.
    void stop();

    /// @brief Get the elapsed time in milliseconds.
    /// @return The elapsed time in milliseconds.
    double elapsedMilliseconds() const;
};

#endif // TIMER_H
