#pragma once

#include <spdlog/spdlog.h>

namespace spd = spdlog;

#define DEFAULT_LOGGER "console"

namespace utils
{
#ifdef _MSC_VER

#include <spdlog/sinks/windebug_sink.h>

    typedef spd::sinks::msvc_sink_mt default_sink;

#else

#include <spdlog/sinks/stdout_sinks.h>

    typedef spd::sinks::stdout_sink_mt default_sink;

#endif

    inline std::shared_ptr<spd::logger> get_logger(const std::string& name){
        auto result = spd::get(name);
        if(result != nullptr){
            return result;
        }

        auto sink = std::make_shared<default_sink>();
        auto logger = std::make_shared<spd::logger>(name, sink);
        register_logger(logger);

        return logger;
    }

    inline std::shared_ptr<spd::logger> get_default_logger(){
        return get_logger(DEFAULT_LOGGER);
    }
}