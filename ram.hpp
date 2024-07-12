#ifndef RAM_HPP
#define RAM_HPP

#include <iostream>
#include <vector>

class RAM {
public:
    RAM() = default;
    void loadToMemory(std::size_t index, int value);
    int getMemoryValue(std::size_t index) const;
    void setMemoryValue(std::size_t index, int value);
private:
    std::vector<int> m_memory;
};
#endif