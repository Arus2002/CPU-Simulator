#include "cpu.hpp"
#include <cctype>
#include <algorithm>

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
        // Extract words from the line stream
        if (count == 3) { 
            lineStream >> instruction;
            lineStream >> operand1;
            operand1.pop_back();
            lineStream >> operand2;
            m_instructions.push_back({instruction, {operand1, operand2}});
        }
        else if (count == 2) {
            lineStream >> instruction;
            lineStream >> operand1;
            m_instructions.push_back({instruction, {operand1, operand2}});
        }
        else if (count == 1) {
            lineStream >> instruction;
            if (instruction == "end") {
                file.close();
                return;
            }
            m_instructions.push_back({instruction, {"label", ""}});
        }
        ++addressNumber;
    }

    file.close();
}

bool CPU::isRegister(const std::string& instruction) const {
    if (m_registers.find(instruction) != m_registers.end()) {
        return true;
    }
    return false;
}

bool CPU::isFromRam(const std::string& instruction) const {
    if (instruction.size() > 2 && instruction[0] == '[' && instruction[instruction.size() - 1] == ']') {
        std::string strIndex = instruction.substr(1, instruction.size() - 2);
        for (char itr: strIndex) {
            if (!std::isdigit(itr)) {
                std::cout << "Not valid address!" << std::endl;
                return false;
            }
        }
        return true;
    }
    return false;
}

bool CPU::isLiteral(const std::string& instruction) const {
    for (char itr: instruction) {
        if (!std::isdigit(itr)) {
            std::cout << "Not valid address!" << std::endl;
            return false;
        }
    }
    return true;
}

void CPU::executeProgramme() {
    for (auto instruction = m_instructions.begin(); instruction != m_instructions.end(); ++instruction) {
        if (instruction->second.first != "label" && instruction->first != "jg" && instruction->first != "jl" && instruction->first != "je" && instruction->first != "mov" && instruction->first != "print") {
            //find val2
            int val1, val2 = 0;
            bool binaryFlag = false;
            if (instruction->second.second != "") {
                binaryFlag = true;
                if (isRegister(instruction->second.second)) {
                    auto it = m_registers.find(instruction->second.second);
                    val2 = it->second;
                }
                else if (isFromRam(instruction->second.second)) {
                    std::string strIndex = instruction->second.second.substr(1, instruction->second.second.size() - 2);
                    int index = std::stoi(strIndex);
                    val2 = m_ram.getMemoryValue(index);
                }
                else if (isLiteral(instruction->second.second)){
                    val2 = stoi(instruction->second.second);
                }
            }
            if (isRegister(instruction->second.first)) {
                auto it = m_registers.find(instruction->second.first);
                val1 = it->second;
                if (binaryFlag) {
                    m_alu.executeOperation2Operands(instruction->first, val1, val2);
                }
                else {
                    m_alu.executeOperation1Operand(instruction->first, val1);
                }
                if (instruction->first != "cmp") {
                    it->second = m_alu.getValue();
                }
            }
            else if(isFromRam(instruction->second.first)) {
                std::string strIndex = instruction->second.first.substr(1, instruction->second.first.size() - 2);
                int index = std::stoi(strIndex);
                val1 = m_ram.getMemoryValue(index);
                if (binaryFlag) {
                    m_alu.executeOperation2Operands(instruction->first, val1, val2);
                }
                else {
                    m_alu.executeOperation1Operand(instruction->first, val1);
                }
                if (instruction->first != "cmp") {
                    m_ram.setMemoryValue(index, m_alu.getValue());
                }
            }
            else {
                std::cout << "Invalid operand1" << std::endl;
                return;
            }
        }
        else if (instruction->first == "mov") {
            bool firstOperandIsFromMemory = false;
            bool firstOperandIsRegister = false;
            int index = 0;
            auto registerItr = m_registers.begin();
            if (isRegister(instruction->second.first)) {
                registerItr = m_registers.find(instruction->second.first);
                firstOperandIsRegister = true;
            }
            else if(isFromRam(instruction->second.first)) {
                std::string strIndex = instruction->second.first.substr(1, instruction->second.first.size() - 2);
                index = std::stoi(strIndex);
                firstOperandIsFromMemory = true;
            }
            else {
                std::cout << "Invalid operand1" << std::endl;
                return;
            }
            int val2;
            if (isRegister(instruction->second.second)) {
                auto it = m_registers.find(instruction->second.second);
                val2 = it->second;
            }
            else if (isFromRam(instruction->second.second)) {
                std::string strIndex = instruction->second.second.substr(1, instruction->second.second.size() - 2);
                int index = std::stoi(strIndex);
                val2 = m_ram.getMemoryValue(index);
            }
            else if (isLiteral(instruction->second.second)){
                val2 = std::stoi(instruction->second.second);
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
        else if (instruction->first == "print") {
            int val;
            if (isRegister(instruction->second.first)) {
                auto it = m_registers.find(instruction->second.first);
                val = it->second;
            }
            else if(isFromRam(instruction->second.first)) {
                std::string strIndex = instruction->second.first.substr(1, instruction->second.first.size() - 2);
                int index = std::stoi(strIndex);
                val = m_ram.getMemoryValue(index);
            }
            else if (isLiteral(instruction->second.first)){
                val = std::stoi(instruction->second.first);
            }
            std::cout << instruction->second.first << " value is: " << val << std::endl;;
        }
        else if (instruction->first == "je") {
            if (m_alu.getValue() == 0) {
                std::pair<std::string, std::pair<std::string, std::string>> target = {instruction->second.first, {"label", ""}};
                auto it = std::find(m_instructions.begin(), m_instructions.end(), target);
                if (it == m_instructions.end()) {
                    std::cout << "Invalid label" << std::endl;
                    return;
                }
                instruction = it;
            }
        }
        else if (instruction->first == "jl") {
            if (m_alu.getValue() < 0) {
                std::pair<std::string, std::pair<std::string, std::string>> target = {instruction->second.first, {"label", ""}};
                auto it = std::find(m_instructions.begin(), m_instructions.end(), target);
                if (it == m_instructions.end()) {
                    std::cout << "Invalid label" << std::endl;
                    return;
                }
                instruction = it;
            }
        }
        else if (instruction->first == "jg") {
            if (m_alu.getValue() > 0) {
                std::pair<std::string, std::pair<std::string, std::string>> target = {instruction->second.first, {"label", ""}};
                auto it = std::find(m_instructions.begin(), m_instructions.end(), target);
                if (it == m_instructions.end()) {
                    std::cout << "Invalid label" << std::endl;
                    return;
                }
                instruction = it;
            }
        }
    }
}

void CPU::seeRam32BitAddress() const {
    std::size_t size = 0;
    if (m_ram.getMemorySize() < 32) {
        size = m_ram.getMemorySize();
    }
    else {
        size = 32;
    }
    std::cout << "Your values of RAM till 32 address!" << std::endl;
    int index = 0;
    for ( ; index < size; ++index) {
        std::cout << index << "-" << m_ram.getMemoryValue(index) << std::endl;
    }
    if (size < 32) {
        for (; index < 32; ++ index) {
            std::cout << index << "-" << 0 << std::endl;
        }
    }
}


