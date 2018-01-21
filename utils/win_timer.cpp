#include "timer.h"

#include <Windows.h>

namespace utils {
    timer::timer()
            : seconds_per_count(0.0), delta_time(0), base_time(0), paused_time(0), stop_time(0), prev_time(0),
              current_time(0), stopped(false) {
        // query how many ticks per second does our high performance timer get
        __int64 ticks_per_second;
        QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&ticks_per_second));
        seconds_per_count = 1.0 / static_cast<double>(ticks_per_second);
    }

    timer::~timer() = default;

    float timer::get_time() const {
        if (stopped) {
            // calculate time since the start till the time we paused the timer without the paused moments
            return static_cast<float>((stop_time - paused_time - base_time) * seconds_per_count);
        }

        // calculate time since the start without the paused moments
        return static_cast<float>((current_time - paused_time - base_time) * seconds_per_count);
    }

    float timer::get_delta_time() const {
        return static_cast<float>(delta_time);
    }

    void timer::reset() {
        __int64 time;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&time));
        base_time = static_cast<int64_t>(time);
        prev_time = static_cast<int64_t>(time);
        stop_time = 0;
        paused_time = 0;
        stopped = false;
    }

    void timer::start() {
        // we can start only when we are stopped
        if (stopped) {
            __int64 start_time;
            QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&start_time));

            paused_time += start_time - stop_time;
            prev_time = static_cast<int64_t>(start_time);
            stopped = false;
        }
    }

    void timer::tick() {
        if (stopped) {
            // if we are stopped then delta time is 0
            delta_time = 0.0;
        } else {
            // calculate new delta time
            __int64 time;
            QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&time));
            current_time = static_cast<int64_t>(time);

            // calculate time difference between two ticks -> delta
            delta_time = (current_time - prev_time) * seconds_per_count;

            // store for the next tick
            prev_time = current_time;

            // force non-negative -> if the processor goes into a power save mode or we get shuffled to another processor, then we can get negative delta_time
            if (delta_time < 0.0) {
                delta_time = 0.0;
            }
        }
    }

    void timer::stop() {
        // we can stop only if we are started
        if (!stopped) {
            __int64 time;
            QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&time));
            stop_time = static_cast<int64_t>(time);
            stopped = true;
        }
    }
}