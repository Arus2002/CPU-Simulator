#include "ram.hpp"

void RAM::loadToMemory(std::size_t index, int value) {
    if (index > m_memory.size()) {
        m_memory.resize(index + 1);
    }
    m_memory[index] = value;
}

int RAM::getMemoryValue(std::size_t index) const {
    if (index > m_memory.size()) {
        std::cout << "Overflow" << std::endl;
    }
    return m_memory[index];
}

void RAM::setMemoryValue(std::size_t index, int value) {
    if (index > m_memory.size()) {
        std::cout << "Overflow" << std::endl;
        return;
    }
    m_memory[index] = value;
}

std::size_t RAM::getMemorySize() const {
    return m_memory.size();
}