#include "complier.hpp"
#include "runner.hpp"

using namespace ns_complier;
using namespace ns_runner;

int main() {
    std::string code = "code";
    std::string code2 = "code2";
    Complier::Complie(code);
    Runner::Run(code);
    Complier::Complie(code2);
    Runner::Run(code2);
    return 0;
}