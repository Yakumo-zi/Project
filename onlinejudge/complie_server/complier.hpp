#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <wait.h>
#include "../comm/util.hpp"
#include "../comm/log.hpp"
namespace ns_complier {
    using namespace ns_util;
    using namespace ns_log;
    class Complier {
    public:
        Complier() {}
        ~Complier() {}
        static bool Complie(const std::string& file_name) {
            pid_t res = fork();
            if (res < 0) {
                LOG(ERROR) << "内部错误，创建自进程失败" << std::endl;
                return false;
            }
            else if (res == 0) {
                //打开一个complierrerr文件来保存错误，如果不能打开，就直接退出该子进程
                int _err = open(PathUtil::ComplieErr(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
                if (_err < 0) {
                    LOG(WARNING) << "没有成功形成Complierr文件" << std::endl;
                    exit(1);
                }
                //将该文件重定向到stderr
                dup2(_err, 2);
                //用程序替换进行编译，如果编译出错，错误信息会打到complierr
                execlp("g++", "g++", "-o", PathUtil::Exe(file_name).c_str()\
                       , PathUtil::Src(file_name).c_str(), "-std=c++11"\
                       , nullptr);
                LOG(ERROR) << "启动编译器g++失败，可能是参数错误" << std::endl;
                exit(2);
            }
            else {
                waitpid(res, nullptr, 0);
                //如果生成了可执行文件那么就编译成功了
                if (FileUtil::IsFileExists(PathUtil::Exe(file_name))) {
                    LOG(INFO) << "源文件: " << PathUtil::Src(file_name) << " 编译成功:" << PathUtil::Exe(file_name) << std::endl;
                    return true;
                }
            }
            return false;
        }
    };
}