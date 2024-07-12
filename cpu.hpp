#ifndef CPU_HPP
#define CPU_HPP

#include "ram.hpp"
#include "alu.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

using instructions_vector = std::vector<std::pair<std::string, std::pair<std::string, std::string>>>;

class CPU {
public:
    CPU(const std::string& inputFile);
    void initializeRegisters();
    void readFromFile(const std::string& inputFileName);
    void executeProgramme();
    void loadProgrammeToMemory() const;
    
private:
    instructions_vector m_instructions;
    std::unordered_map<std::string, int> m_registers;
    RAM m_ram;
    ALU m_alu;
};

#endif