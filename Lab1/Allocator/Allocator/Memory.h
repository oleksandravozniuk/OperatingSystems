#ifndef LAB1_MEMORY_H
#define LAB1_MEMORY_H

#include <vector>
#include <iostream>
using namespace std;

class Memory {
    struct Memory_unit_info {
        int addr;
        size_t size;

        Memory_unit_info(int addr, size_t size) : size(size), addr(addr) { }
    };

    class Memory_unit {
        int memory;

    public:
        void operator=(const Memory_unit& unit) {
            this->memory = unit.memory;
        }
    };

    // Information about memory
    vector<Memory_unit_info> info_free;
    vector<Memory_unit_info> info_in_use;

    // Memory
    vector<Memory_unit> memory_block;

    // Returns index of memory start from addr
    long what_number_am_i(void* addr);

    // Returns size aligned to 4B
    size_t size_with_align_4B(size_t size);

public:
    Memory(size_t size);

    // Allocates memory. If cannot returns NULL
    void* mem_alloc(size_t size);

    // Reallocates memory. Firstly tries to extend current block and if unsuccessful to find new
    void* mem_realloc(void* addr, size_t size);

    // Make memory free
    void mem_free(void* addr);

    // Prints memory info. '_' - free byte, '*' - byte in use
    void mem_dump();
};


#endif //LAB1_MEMORY_H
