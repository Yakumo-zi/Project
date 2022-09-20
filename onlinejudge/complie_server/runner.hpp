#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "../comm/util.hpp"
#include "../comm/log.hpp"



namespace ns_runner {
    using namespace ns_util;
    using namespace ns_log;
    class Runner {
    public:
        Runner();
        ~Runner();
    public:
        //提供设置进程占用资源大小
        //cpu资源单位是秒
        //内存单位为KB
        static void SetProcLimit(int cpu_limit, int mem_limit) {

            struct rlimit cpu;
            cpu.rlim_cur = cpu_limit;
            cpu.rlim_max = RLIM_INFINITY;
            setrlimit(RLIMIT_CPU, &cpu);

            struct rlimit mem;
            mem.rlim_cur = mem_limit * 1024;
            mem.rlim_max = RLIM_INFINITY;
            setrlimit(RLIMIT_AS, &mem);

        }
        //返回值如果是大于0，说明程序异常了，退出时收到了信号，返回值就是对应的信号编号
        //返回值等于0，正常运行完毕，结果保存到了对应的临时文件中
        //返回值小于0，内部错误
        static int Run(const std::string& file_name, int cpu_limit, int mem_limit) {
            /*****************************
            * Run跑完代码不需要考虑结果正确与否，只需要考虑运行是否正常
            * 我们必须知道可执行程序是谁
            *
            *
            * 一个程序在默认启动时，输出结果是什么
            * 标准输入：不处理
            * 标准输出：程序运行完成，输出结果是什么
            * 标准错误：运行时错误信息
            ******************************/
            std::string _execute = PathUtil::Exe(file_name);
            std::string _stdout = PathUtil::Stdout(file_name);
            std::string _stdin = PathUtil::Stdin(file_name);
            std::string _stderr = PathUtil::Stderr(file_name);

            umask(0);
            int _stdin_fd = open(_stdin.c_str(), O_CREAT | O_RDONLY, 0644);
            int _stdout_fd = open(_stdout.c_str(), O_CREAT | O_WRONLY, 0644);
            int _stderr_fd = open(_stderr.c_str(), O_CREAT | O_WRONLY, 0644);
            if (_stdin_fd < 0 || _stdout_fd < 0 || _stderr_fd < 0) {
                LOG(ERROR) << "运行时打开标准文件失败" << std::endl;
                //打开文件失败
                return -1;
            }
            pid_t pid = fork();
            if (pid < 0) {
                LOG(ERROR) << "运行时创建子进程失败" << std::endl;
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);
                //创建子进程失败
                return -2;
            }
            else if (pid == 0) {
                dup2(_stdin_fd, 0);
                dup2(_stdout_fd, 1);
                dup2(_stderr_fd, 2);
                SetProcLimit(cpu_limit, mem_limit);
                execl(_execute.c_str(), _execute.c_str(), nullptr);
                exit(1);
            }
            else {
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);
                int status = 0;
                waitpid(pid, &status, 0);
                LOG(INFO) << "运行完毕:INFO" << (status & 0x7f) << std::endl;
                //程序运行异常，在linux中一定是受到了信号
                return status & 0x7f;
            }
        }
    };
}