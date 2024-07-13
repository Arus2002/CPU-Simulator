#include "cpu.hpp"

int main() {
    CPU cpu("test_1");
    cpu.executeProgramme();
    cpu.seeRam32BitAddress();
}