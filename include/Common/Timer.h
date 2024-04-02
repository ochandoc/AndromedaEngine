#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point lastTime;

public:
    Timer() : lastTime(std::chrono::high_resolution_clock::now()) {}

    // Devuelve el tiempo delta en segundos
    float GetDeltaTime() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = currentTime - lastTime;
        lastTime = currentTime;
        return delta.count();
    }
};