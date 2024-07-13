#ifndef ALU_HPP
#define ALU_HPP

#include <map>
#include <iostream>

class ALU {
public:
    ALU();
    void add(int val1, int val2);
    void sub(int val1, int val2);
    void mul(int val1, int val2);
    void div(int val1, int val2);
    void myAnd(int val1, int val2);
    void myOr(int val1, int val2);
    void cmp(int val1, int val2);
    void inc(int val);
    void dec(int val);
    void myNot(int val);
    int getValue() const;
    std::map<std::string, void(ALU::*)(int, int)> get2OperandsOperations() const;
    std::map<std::string, void(ALU::*)(int)> get1OperandOperations() const;
    void executeOperation2Operands(const std::string& op, int val1, int val2);
    void executeOperation1Operand(const std::string& op, int val1);

private:
    void initialize2OperandsOperations();
    void initialize1OperandOperations();
    
private:
    std::map<std::string, void(ALU::*)(int, int)> m_2OperandsOperations;
    std::map<std::string, void(ALU::*)(int)> m_1OperandOperations;
    int m_value;
};

#endif