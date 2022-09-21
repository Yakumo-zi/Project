#pragma once

#include<iostream>
#include<fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <atomic>
#include <sys/time.h>
namespace ns_util {
    class TimeUtil {
    public:
        static std::string GetTimeStap() {
            struct timeval _time;
            gettimeofday(&_time, nullptr);
            return std::to_string(_time.tv_sec);
        }
        //获得毫秒级时间戳
        static std::string GetTimeMs() {
            struct timeval _time;
            gettimeofday(&_time, nullptr);
            return std::to_string(_time.tv_usec / 1000 + _time.tv_sec * 1000);
        }
    };
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
        //毫秒级时间戳+原子性递增唯一值，来保证唯一性
        static std::string UniqFileName() {
            static std::atomic_uint id(0);
            id++;
            std::string ms = TimeUtil::GetTimeMs();
            std::string uid = std::to_string(id);

            return ms + "_" + uid;
        }
        static bool WriteFile(const std::string& path_name, const std::string& content) {
            std::ofstream out(path_name);
            if (!out.is_open()) {
                return false;
            }
            out.write(content.c_str(), content.size());
            out.close();
            return true;
        }
        static bool ReadFile(const std::string& path_name, std::string* content, bool keep = false) {
            (*content).clear();
            std::ifstream in(path_name);
            std::string res;
            if (!in.is_open()) {
                return false;
            };
            std::string line;
            while (std::getline(in, line)) {
                if (keep) {
                    line += "\n";
                }
                (*content) += line;
            }
            in.close();
            return true;
        }
    };

}