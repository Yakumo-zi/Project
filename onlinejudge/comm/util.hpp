#pragma once

#include<iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
namespace ns_util {
    const std::string path = "./temp/";
    class PathUtil {
    public:
        static std::string AddSubfix(const std::string& file_name, const std::string& subfix) {
            std::string path_name = path;
            path_name += file_name;
            path_name += subfix;
            return path_name;
        }
        static std::string Src(const std::string& file_name) {
            return AddSubfix(file_name, ".cpp");
        }
        static std::string Exe(const std::string& file_name) {
            return AddSubfix(file_name, ".exe");
        }
        static std::string Stderr(const std::string& file_name) {
            return AddSubfix(file_name, ".stderr");
        }
        static std::string Stdin(const std::string& file_name) {
            return AddSubfix(file_name, ".stdin");
        }
        static std::string Stdout(const std::string& file_name) {
            return AddSubfix(file_name, ".stdout");
        }
        static std::string ComplieErr(const std::string& file_name) {
            return AddSubfix(file_name, ".complierr");
        }
    };
    class FileUtil {
    public:
        static bool IsFileExists(const std::string& path_name) {
            struct stat st;
            if (stat(path_name.c_str(), &st) == 0) {
                return true;
            }
            return false;
        }
    };
    class TimeUtil {
    public:
        static std::string GetTimeStap() {
            struct timeval _time;
            gettimeofday(&_time, nullptr);
            return std::to_string(_time.tv_sec);
        }
    };
}