#include "cpu.hpp"
#include <cctype>

CPU::CPU(const std::string& inputFile) 
    : m_instructions{}
    , m_registers{}
    , m_ram{} 
    , m_alu{}
{
    initializeRegisters();
    readFromFile(inputFile);
}

void CPU::initializeRegisters() {
    m_registers["AYB"] = 0;
    m_registers["BEN"] = 0;
    m_registers["GIM"] = 0;
    m_registers["DA"] = 0;
    m_registers["ECH"] = 0;
    m_registers["ZA"] = 0;
}

void CPU::readFromFile(const std::string& inputFileName) {
    std::ifstream file{inputFileName};
    if (!file) {
        std::cout << "Failed to open the file!" << std::endl;
        return;
    }
    std::string line;
    std::size_t addressNumber = 0;
    while (std::getline(file, line)) { // Read line by line
        //std::cout << line << std::endl; // Print each line to the console
        std::istringstream lineStream(line); // Create a string stream from the line
        std::string word;
        std::size_t count = 0;
        while (lineStream >> word) {
            ++count;
        }
        lineStream.clear();
        lineStream.seekg(0, std::ios::beg);

        std::string instruction;
        std::string operand1;
        std::string operand2;
        if (count == 3) { // Extract words from the line stream
            lineStream >> instruction;
            lineStream >> operand1;
            operand1.pop_back();
            lineStream >> operand2;
            m_instructions.push_back({instruction, {operand1, operand2}});
            std::cout << instruction << operand1 << operand2 << std::endl; // Print each word
        }
        else if (count == 2) { // Extract words from the line stream
            lineStream >> instruction;
            lineStream >> operand1;
            m_instructions.push_back({instruction, {operand1, operand2}});
            std::cout << instruction << operand1 << operand2 << std::endl; // Print each word
        }
        else if (count == 1) { // Extract words from the line stream
            lineStream >> instruction;
            if (instruction == "end") {
                file.close();
                return;
            }
            m_instructions.push_back({instruction, {"label", std::to_string(addressNumber)}});
            std::cout << instruction << operand1 << operand2 << std::endl; // Print each word
        }
        ++addressNumber;
    }

    file.close(); // Close the file
}

void CPU::executeProgramme() {
    for (auto& instruction: m_instructions) {
        if (instruction.second.first != "label" && instruction.first != "jg" && instruction.first != "jl" && instruction.first != "je" && instruction.first != "mov" && instruction.first != "print") {
            //find val2
            int val1, val2 = 0;
            bool binaryFlag = false;
            if (instruction.second.second != "") {
                binaryFlag = true;
                if (m_registers.find(instruction.second.second) != m_registers.end()) {
                    auto it = m_registers.find(instruction.second.second);
                    val2 = it->second;
                }
                else if (instruction.second.second.size() > 2 && instruction.second.second[0] == '[' && instruction.second.second[instruction.second.second.size() - 1] == ']') {
                    std::string strIndex = instruction.second.second.substr(1, instruction.second.second.size() - 2);
                    for (char itr: strIndex) {
                        if (!std::isdigit(itr)) {
                            std::cout << "Not valid address!" << std::endl;
                            return;
                        }
                    }
                    int index = std::stoi(strIndex);
                    val2 = m_ram.getMemoryValue(index);
                }
                else {
                    for (char itr: instruction.second.second) {
                        if (!std::isdigit(itr)) {
                            std::cout << "Not valid address!" << std::endl;
                            return;
                        }
                    }
                    val2 = stoi(instruction.second.second);
                }
            }
            if (m_registers.find(instruction.second.first) != m_registers.end()) {
                auto it = m_registers.find(instruction.second.first);
                val1 = it->second;
                if (binaryFlag) {
                    m_alu.executeOperation2Operands(instruction.first, val1, val2);
                }
                else {
                    m_alu.executeOperation1Operand(instruction.first, val1);
                }
                it->second = m_alu.getValue();
            }
            else if(instruction.second.first.size() > 2 && instruction.second.first[0] == '[' && instruction.second.first[instruction.second.first.size() - 1] == ']') {
                std::string strIndex = instruction.second.first.substr(1, instruction.second.first.size() - 2);
                for (char itr: strIndex) {
                    if (!std::isdigit(itr)) {
                        std::cout << "Not valid address!" << std::endl;
                        return;
                    }
                }
                int index = std::stoi(strIndex);
                val1 = m_ram.getMemoryValue(index);
                if (binaryFlag) {
                    m_alu.executeOperation2Operands(instruction.first, val1, val2);
                }
                else {
                    m_alu.executeOperation1Operand(instruction.first, val1);
                }
                m_ram.setMemoryValue(index, m_alu.getValue());
            }
            else {
                std::cout << "Invalid operand1" << std::endl;
                return;
            }
        }
        else if (instruction.first == "mov") {
            bool firstOperandIsFromMemory = false;
            bool firstOperandIsRegister = false;
            int index = 0;
            auto registerItr = m_registers.begin();
            if (m_registers.find(instruction.second.first) != m_registers.end()) {
                registerItr = m_registers.find(instruction.second.first);
                firstOperandIsRegister = true;
            }
            else if(instruction.second.first.size() > 2 && instruction.second.first[0] == '[' && instruction.second.first[instruction.second.first.size() - 1] == ']') {
                std::string strIndex = instruction.second.first.substr(1, instruction.second.first.size() - 2);
                for (char itr: strIndex) {
                    if (!std::isdigit(itr)) {
                        std::cout << "Not valid address!" << std::endl;
                        return;
                    }
                }
                index = std::stoi(strIndex);
                firstOperandIsFromMemory = true;
            }
            else {
                std::cout << "Invalid operand1" << std::endl;
                return;
            }
            int val2;
            if (m_registers.find(instruction.second.second) != m_registers.end()) {
                auto it = m_registers.find(instruction.second.second);
                val2 = it->second;
            }
            else if(instruction.second.second.size() > 2 && instruction.second.second[0] == '[' && instruction.second.second[instruction.second.second.size() - 1] == ']') {
                std::string strIndex = instruction.second.second.substr(1, instruction.second.second.size() - 2);
                for (char itr: strIndex) {
                    if (!std::isdigit(itr)) {
                        std::cout << "Not valid address!" << std::endl;
                        return;
                    }
                }
                int index = std::stoi(strIndex);
                val2 = m_ram.getMemoryValue(index);
            }
            else {
                for (char itr: instruction.second.second) {
                    if (!std::isdigit(itr)) {
                        std::cout << "Not valid address!" << std::endl;
                        return;
                    }
                }
                val2 = std::stoi(instruction.second.second);
            }
            if (firstOperandIsFromMemory) {
                m_ram.loadToMemory(index, val2);
            }
            else if (firstOperandIsRegister) {
                registerItr->second = val2;
            }
            else {
                std::cout << "Invalid operand1" << std::endl;
                return;
            }
        }
        else if (instruction.first == "print") {
            int val;
            if (m_registers.find(instruction.second.first) != m_registers.end()) {
                auto it = m_registers.find(instruction.second.first);
                val = it->second;
            }
            else if(instruction.second.first.size() > 2 && instruction.second.first[0] == '[' && instruction.second.first[instruction.second.first.size() - 1] == ']') {
                std::string strIndex = instruction.second.first.substr(1, instruction.second.first.size() - 2);
                for (char itr: strIndex) {
                    if (!std::isdigit(itr)) {
                        std::cout << "Not valid address!" << std::endl;
                        return;
                    }
                }
                int index = std::stoi(strIndex);
                val = m_ram.getMemoryValue(index);
            }
            else {
                for (char itr: instruction.second.first) {
                    if (!std::isdigit(itr)) {
                        std::cout << "Not valid address!" << std::endl;
                        return;
                    }
                }
                val = std::stoi(instruction.second.second);
            }
            std::cout << val;
        }
    }
    // for (const auto& item: m_registers) {
    //     std::cout << item.first << "-" << item.second << std::endl;
    // }
}


