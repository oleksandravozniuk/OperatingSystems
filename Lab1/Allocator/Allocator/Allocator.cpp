#include <iostream>
#include "Memory.h"

using namespace std;

int main() {
    Memory m = Memory(64);
    cout << "Create 64 bytes of memory \n";
    m.mem_dump();

    void* ref0 = m.mem_alloc(16);
    cout << "create reference 1 on allocating 16 bytes \n";
    m.mem_dump();

    void* ref1 = m.mem_alloc(2);
    cout<<"Create reference 2 on allocating 2 bytes \n";
    m.mem_dump();

    m.mem_alloc(3);
    cout << "Allocate 3 bytes \n";
    m.mem_dump();
    
    m.mem_realloc(ref0, 8);
    cout << "Reallocate reference 1 from 16 bytes to 8 \n";
    m.mem_dump();

    m.mem_realloc(ref0, 24);
    cout << "Reallocate reference 1 from 8 bytes to 24 \n";
    m.mem_dump();

    m.mem_free(ref1);
    cout << "Free reference 2 \n";
    m.mem_dump();



    return 0;
}