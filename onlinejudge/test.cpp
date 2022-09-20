#include <iostream>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

void handle(int signo) {
    std::cout << signo << std::endl;
}
int main() {
    // for (int i = 0;i < 32;i++) {
    //     signal(i, handle);
    // }
    struct rlimit r;
    r.rlim_cur = 1;
    r.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CPU, &r);
    while (1);
    // struct rlimit r;
    // r.rlim_cur = 1024 * 1024 * 20;
    // r.rlim_max = RLIM_INFINITY;
    // setrlimit(RLIMIT_AS, &r);
    // int count = 0;
    // while (1) {
    //     char* a = new char[1024 * 1024];
    //     count++;
    //     sleep(1);
    //     std::cout << "size:" << count << "M" << std::endl;
    // }
    return 0;
}