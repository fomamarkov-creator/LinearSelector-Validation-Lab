#include "LinearSelector.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "[V-CORE]: Engine v147.1 Initialized. Waiting for Resonance..." << std::endl;
    // Минимальный код, чтобы линковщик не удалил класс
    std::vector<float> dummy_q(1, 1.0f);
    try {
        MarkovAI::LinearSelector selector(1, 1, dummy_q);
        std::cout << "Resonance 144hz applied." << std::endl; 
    } catch (...) {}
    return 0;
}
