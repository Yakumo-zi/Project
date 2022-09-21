#pragma once
#include <jsoncpp/json/json.h>
#include "complier.hpp"
#include "runner.hpp"
#include "../comm/log.hpp"
#include "../comm/util.hpp"
namespace ns_complie_and_run {
    using namespace ns_complier;
    using namespace ns_runner;
    using namespace ns_log;
    using namespace ns_util;
    class ComplieAndRun {
    public:
        static void RemoveTempFile(const std::string& file_name) {
            std::string src = PathUtil::Src(file_name);
            if (FileUtil::IsFileExists(src)) {
                unlink(src.c_str());
            }
            std::string complierr = PathUtil::ComplieErr(file_name);
            if (FileUtil::IsFileExists(complierr)) {
                unlink(complierr.c_str());
            }
            std::string exe = PathUtil::Exe(file_name);
            if (FileUtil::IsFileExists(exe)) {
                unlink(exe.c_str());
            }
            std::string _stdin = PathUtil::Stdin(file_name);
            if (FileUtil::IsFileExists(_stdin)) {
                unlink(_stdin.c_str());
            }
            std::string _stdout = PathUtil::Stdout(file_name);
            if (FileUtil::IsFileExists(_stdout)) {
                unlink(_stdout.c_str());
            }
            std::string _stderr = PathUtil::Stderr(file_name);
            if (FileUtil::IsFileExists(_stderr)) {
                unlink(_stderr.c_str());
            }

        }
        //code>0:进程收到了信号，导致异常终止
        //code<0:整个过程非运行报错,代码为空或者编译报错
        //code=0:整个过程全部成功
        static std::string CodeoToDesc(int code, const std::string& file_name) {
            std::string desc;
            switch (code) {
            case 0:
                desc = "编译运行成功";
                break;
            case -1:
                desc = "用户提交的代码是空";
                break;
            case -2:
                desc = "未知错误";
                break;
            case -3:
                //应该将编译失败的原因发给用户，将报错文件的内容读取出来
                FileUtil::ReadFile(PathUtil::ComplieErr(file_name), &desc, true);
                break;
            case -4:
                desc = "用户提交的代码是空";
                break;
            case SIGABRT://6
                desc = "内存超过范围";
                break;
            case SIGXCPU://24
                desc = "CPU使用超时";
                break;
            case SIGFPE://8
                desc = "浮点数溢出错误";
                break;
            default:
                desc = "debug:unknown " + std::to_string(code);
                break;
            }
            return desc;
        }
        /***************************************
         * 输入:
         * code： 用户提交的代码
         * input: 用户给自己提交的代码对应的输入，不做处理
         * cpu_limit: 时间要求
         * mem_limit: 空间要求
         * 输出:
         * 必填
         * status: 状态码
         * reason: 请求结果
         * 选填：
         * stdout: 我的程序运行完的结果
         * stderr: 我的程序运行完的错误结果
         * 参数：
         * in_json: {"code": "#include...", "input": "","cpu_limit":1, "mem_limit":10240}
         * out_json: {"status":"0", "reason":"","stdout":"","stderr":"",}
         * ************************************/
        static void Start(const std::string& in_json, std::string* out_json) {
            Json::Value in_value;
            Json::Reader reader;
            reader.parse(in_json, in_value);
            std::string code = in_value["code"].asString();
            std::string input = in_value["input"].asString();
            int cpu_limit = in_value["cpu_limit"].asInt();
            int mem_limit = in_value["mem_limit"].asInt();

            std::string filename = FileUtil::UniqFileName();
            Json::Value out_value;
            int status_code = 0;
            int run_code = 0;
            if (code.size() == 0) {
                status_code = -1;
                goto END;

            }
            //形成文件名具有唯一性，没有目录后缀
            //毫秒级时间戳+原子性递增唯一值，来保证唯一性

            if (!FileUtil::WriteFile(PathUtil::Src(filename), code)) {
                status_code = -2;
                goto END;

            }
            if (!Complier::Complie(filename)) {
                status_code = -3;
                goto END;
            }
            run_code = Runner::Run(filename, cpu_limit, mem_limit);
            if (run_code < 0) {
                status_code = -2;
            }
            else if (run_code > 0) {
                status_code = run_code;
            }
            else {
                status_code = 0;
            }
        END:
            out_value["status"] = status_code;
            out_value["reason"] = CodeoToDesc(status_code, filename);
            if (status_code == 0) {
                //整个过程全部成功
                std::string _stdout, _stderr;
                FileUtil::ReadFile(PathUtil::Stdout(filename), &_stdout);
                FileUtil::ReadFile(PathUtil::Stderr(filename), &_stderr);
                out_value["stdout"] = _stdout;
                out_value["stderr"] = _stderr;
            }
            Json::StyledWriter writer;
            *out_json = writer.write(out_value);
            RemoveTempFile(filename);
        }
    };
}