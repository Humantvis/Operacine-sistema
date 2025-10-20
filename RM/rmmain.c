#include <stdint.h>

typedef int64_t reg_t;
typedef uint8_t byte_t;
typedef struct{
    reg_t r[16];        // general purpose registers
    reg_t ic;           // instruction counter
    byte_t fr;          // flags register
    reg_t mode;         // cpu mode
    reg_t ptr;          // page table register
    reg_t pi;           // program interupt register
    reg_t si;           // supervisor interupt register
    reg_t ti;           // timer register
    byte_t mem[1024];   // memory
} cpu_t;

int main() {

}