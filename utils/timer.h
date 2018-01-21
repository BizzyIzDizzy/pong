#pragma once

#include "common.h"

namespace utils{
    class timer{
    public:
        timer();
        ~timer();

        float get_time() const;
        float get_delta_time() const;

        void reset();
        void start();
        void stop();
        void tick();

    private:
        double seconds_per_count;
        double delta_time;

        int64_t base_time;
        int64_t paused_time;
        int64_t stop_time;
        int64_t prev_time;
        int64_t current_time;

        bool stopped;
    };
}