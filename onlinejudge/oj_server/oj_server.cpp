#include <iostream>
#include <httplib.h>
using namespace httplib;
int main() {
    Server svr;
    svr.Get("/all_questions", [](const Request& req, Response& res) {
        res.set_content("获取所有题目", "text/plain;charset=utf8;");
            });
    svr.Get(R"(/question/(\d+))", [](const Request& req, Response& res) {
        std::string number = req.matches[1];
        res.set_content("获取指定题目" + number, "text/plain;charset=utf8;");
            });
    svr.listen("0.0.0.0", 8080);
    return 0;
}