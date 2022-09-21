#include "complier.hpp"
#include "runner.hpp"
#include "complie_run.hpp"
#include <httplib.h>
using namespace ns_complier;
using namespace ns_runner;
using namespace ns_complie_and_run;
using namespace httplib;

int main(int argc, char* argv[]) {
    // std::string in_json;
    // Json::Value in_value;
    // in_value["code"] = R"(
    // #include<iostream>
    // int main(){
    //     std::cout<<"you can look me!"<<std::endl;
    //     while(1);
    //     return 0;
    // }
    // )";
    // in_value["input"] = "";
    // in_value["cpu_limit"] = 1;
    // in_value["mem_limit"] = 10240 * 30;
    // Json::FastWriter writer;
    // in_json = writer.write(in_value);
    // std::cout << in_json << std::endl;
    // std::string out_json;
    // ComplieAndRun::Start(in_json, &out_json);
    // std::cout << out_json << std::endl;
    // Server svr;
    // svr.Get("/hi", [](const Request& req, Response& res) {
    //     res.set_content("Hello World!", "text/plain");
    //         });
    // svr.listen("0.0.0.0", 8080);
    Server svr;
    svr.Post("/complie_and_run", [](const Request& req, Response& res) {
        std::string in_json = req.body;
        std::string out_json;
        if (!in_json.empty()) {
            ComplieAndRun::Start(in_json, &out_json);
            res.set_content(out_json, "application/json;charset=utf-8;");
        }
             });
    svr.listen("0.0.0.0", atoi(argv[1]));
    return 0;
}