#include "alu.hpp"

ALU::ALU()
    : m_2OperandsOperations{}
    , m_1OperandOperations{}
    , m_value{0}
    , m_booleanValue{}
    , m_flag{} 
{
    initialize2OperandsOperations();
    initialize1OperandOperations();
}

void ALU::initialize2OperandsOperations() {
    m_2OperandsOperations["add"] = &ALU::add;
    m_2OperandsOperations["sub"] = &ALU::sub;
    m_2OperandsOperations["mul"] = &ALU::mul;
    m_2OperandsOperations["div"] = &ALU::div;
    m_2OperandsOperations["and"] = &ALU::myAnd;
    m_2OperandsOperations["or"] = &ALU::myOr;
    m_2OperandsOperations["cmp"] = &ALU::cmp;
}

void ALU::initialize1OperandOperations() {
    m_1OperandOperations["inc"] = &ALU::inc;
    m_1OperandOperations["dec"] = &ALU::dec;
    m_1OperandOperations["not"] = &ALU::myNot;
}

void ALU::executeOperation2Operands(const std::string& op, int val1, int val2) {
    auto it = m_2OperandsOperations.find(op);
    if (it != m_2OperandsOperations.end()) {
        (this->*(it->second))(val1, val2);
    } else {
        std::cout << "Operation not found!" << std::endl;
    }
}

void ALU::executeOperation1Operand(const std::string& op, int val1) {
    auto it = m_1OperandOperations.find(op);
    if (it != m_1OperandOperations.end()) {
        (this->*(it->second))(val1);
    } else {
        std::cout << "Operation not found!" << std::endl;
    }
}

void ALU::add(int val1, int val2) {
    m_value = val1 + val2;
}

void ALU::sub(int val1, int val2) {
    m_value = val1 - val2;
}

void ALU::mul(int val1, int val2) {
    m_value = val1 * val2;
}

void ALU::div(int val1, int val2) {
    if (val2 != 0) {
        m_value = val1 / val2;
    }
}

void ALU::dec(int val) {
    m_value = --val;
}

void ALU::inc(int val) {
    m_value = ++val;
}

void ALU::myNot(int val) {
    m_booleanValue = !val; 
}

void ALU::myAnd(int val1, int val2) {
    m_booleanValue = val1 & val2;
}

void ALU::myOr(int val1, int val2) {
    m_booleanValue = val1 | val2;
}

void ALU::cmp(int val1, int val2) {
    sub(val1, val2);
    m_flag = m_value;
}

int ALU::getValue() const {
    return m_value;
}

bool ALU::getBooleanValue() const {
    return m_booleanValue;
}

int ALU::getFlag() const {
    return m_flag;
}

std::map<std::string, void(ALU::*)(int, int)> ALU::get2OperandsOperations() const {
    return m_2OperandsOperations;
}

std::map<std::string, void(ALU::*)(int)> ALU::get1OperandOperations() const {
    return m_1OperandOperations;
}