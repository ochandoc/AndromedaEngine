#include <chrono>

class Timer {
public:
    Timer() {
        lastTime = std::chrono::high_resolution_clock::now();
    }

    double GetDeltaTime() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = currentTime - lastTime;
        lastTime = currentTime;
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
};