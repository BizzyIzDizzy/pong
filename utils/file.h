#pragma once

#include <string>
#include <optional>
#include <fstream>
#include <boost/algorithm/string/join.hpp>

#include "log.h"
#include "common.h"

namespace utils
{
#if defined WIN32 || defined _WIN32

#include <windows.h>

    /// It returns working directory of the current process.
    /// @returns Nothing if it fails or current working directory.
    inline std::optional<std::string> get_current_working_directory()
    {
        char path[FILENAME_MAX];
        if(GetCurrentDirectory(sizeof(path), path) == 0)
        {
            auto log = get_default_logger();
            log->error("Could not get current working directory!");
            return std::nullopt;
        }

        return path;
    }

#endif

    /// Reads an entire text file into a vector of strings if succeeds.
    /// @param [in] path Path to the file.
    /// @returns Nothing if it fails or a vector of strings.
    inline std::optional<std::vector<std::string>> read_text_file_lines(const std::string& path)
    {
        std::ifstream file(path);
        if(file.is_open()){
            std::vector<std::string> lines;
            std::string line;

            while(std::getline(file, line)){
                lines.push_back(line);
            }

            file.close();

            return lines;
        }

        auto log = utils::get_default_logger();
        log->error("Unable to open file '{}'!", path);
        return std::nullopt;
    }

    /// Reads an entire text file into a single string.
    /// @param [in] path Path to the file.
    /// @returns Nothing if it fails or a single string.
    inline std::optional<std::string> read_text_file(const std::string& path)
    {
        if(auto lines = read_text_file_lines(path))
        {
            return boost::algorithm::join(*lines, NEWLINE);
        }

        return std::nullopt;
    }
}