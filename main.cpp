/*
 * Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
 * Project: V-CORE v147.1 
 */

#include "LinearSelector.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "[V-CORE]: Engine v147.1 Initialized. Waiting for Resonance..." << std::endl;
    
    // Тестовая инициализация для подтверждения работоспособности
    std::vector<float> dummy_q(1, 1.0f);
    try {
        MarkovAI::LinearSelector selector(1, 1, dummy_q);
        std::cout << "Resonance 144hz applied." << std::endl; 
    } catch (const std::exception& e) {
        std::cerr << "[ERROR]: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
