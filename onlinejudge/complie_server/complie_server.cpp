#include "complier.hpp"
#include "runner.hpp"

using namespace ns_complier;
using namespace ns_runner;

int main() {
    std::string code = "code";
    std::string code2 = "code2";
    Complier::Complie(code);
    Complier::Complie(code2);
    return 0;
}