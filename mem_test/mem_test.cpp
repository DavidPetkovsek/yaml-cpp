#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <yaml-cpp/yaml.h>
#include <thread>
#include <chrono>
#include <iostream>

static size_t getCurrentMemoryUsage() {
    FILE* file = fopen("/proc/self/status", "r");
    if (file) {
        char line[128];
        size_t vmRSS = 0;

        while (fgets(line, 128, file) != NULL) {
            if (strncmp(line, "VmRSS:", 6) == 0) {
                char* p = line + 6;
                while (*p == ' ' || *p == '\t') p++;
                vmRSS = atoi(p) * 1024;
                break;
            }
        }
        fclose(file);
        return vmRSS;
    }
    return 0;
}

int main(){
    std::jthread monitor = std::jthread([](std::stop_token st){
        while(!st.stop_requested()){
            std::cout << "Memory: " << (static_cast<double>(getCurrentMemoryUsage())/1024/1024) << "MB" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });

    YAML::Node collector = YAML::Load("{ x : 1 }");

    int total = 1000000000;
    int j = 0;
    for (int i = 0; i < total; ++i, ++j){
        YAML::Node node;
        node.push_back(collector["x"]);
        if(j % 1000000 == 0) std::cout << i << " out of " << total << " " << static_cast<float>(i)/static_cast<float>(total) << "%" << std::endl;
    }
    monitor.request_stop();
}