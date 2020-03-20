#include "Memory.h"

Memory::Memory(size_t size) {
    size = size_with_align_4B(size);

    info_free = vector<Memory_unit_info>();
    info_free.push_back(Memory_unit_info(0, size)); // All memory is free

    info_in_use = vector<Memory_unit_info>();//memory in use

    memory_block = vector<Memory_unit>((unsigned long)size);//memory blocks
}

void* Memory::mem_alloc(size_t size) {
    size = size_with_align_4B(size);//count how many memory to take with 4bytes alignment

    for (int i = 0; i < info_free.size(); i++)//all free memory
        if (size <= info_free[i].size) {//if size is lesser or equal then enable free memory
            info_in_use.push_back(Memory_unit_info(info_free[i].addr, size));//take free memory with this size
            if (info_free[i].size == size)  // If allocating the whole block
                info_free.erase(info_free.begin() + i);
            else {  // PArt of block
                info_free[i].addr += size;
                info_free[i].size -= size;
            }

            return &memory_block[info_in_use[info_in_use.size() - 1].addr];//return an adress of memory block
        }

    return NULL;//if a block of memory wasn't taken successfully
}

void* Memory::mem_realloc(void* addr, size_t size) {//what block to realloc and how (increase or decrease a block size memory)
    size = size_with_align_4B(size);//count size with 4 bytes allignment

    if (addr == NULL)
        return mem_alloc(size);

    // Trying enhance current block
    long idx = what_number_am_i(addr);  // Index in memory
    size_t my_size = 0;                 // Size of current block
    int used_idx = 0;                   // Current block index in info vector
    for (int i = 0; i < info_in_use.size(); i++)    // Finding this index
        if (info_in_use[i].addr == idx) {
            my_size = (int)info_in_use[i].size;
            used_idx = i;
            break;
        }
    int need_size = (int)(size - my_size);  // How many more we need
    if (need_size > 0) {    // If we need to extend
        for (int i = 0; i < info_free.size(); i++)
            if (info_free[i].addr == idx + my_size &&
                need_size <= info_free[i].size) {     // If have on right free block
                info_in_use[used_idx].size += need_size;
                if (info_free[i].size == need_size)
                    info_free.erase(info_free.begin() + i);
                else {
                    info_free[i].addr += need_size;
                    info_free[i].size -= need_size;
                }
                return addr;
            }
    }
    else {  // If we need to reduce
        info_in_use[used_idx].size += need_size;
        for (int i = 0; i < info_free.size(); i++)
            if (info_free[i].addr == idx + info_in_use[used_idx].size - need_size) {
                info_free[i].addr += need_size;
                info_free[i].size -= need_size;
                return addr;
            }
        info_free.push_back(Memory_unit_info((int)(info_in_use[used_idx].addr + info_in_use[used_idx].size),
            (size_t)-need_size));
    }

    // If cannot enhance current block
    for (int i = 0; i < info_free.size(); i++)
        if (size <= info_free[i].size) {
            info_in_use.push_back(Memory_unit_info(info_free[i].addr, size));
            if (info_free[i].size == size)
                info_free.erase(info_free.begin() + i);
            else {
                info_free[i].addr += size;
                info_free[i].size -= size;
            }

            // Moving data to new place
            for (int j = 0; j < info_in_use[used_idx].size; j++)
                memory_block[info_in_use[used_idx].addr + j] = memory_block[info_free[i].addr + j];
            mem_free(addr);

            return &memory_block[info_in_use[info_in_use.size() - 1].addr];
        }

    return NULL;
}

void Memory::mem_free(void* addr) {
    long idx = what_number_am_i(addr);
    for (int i = 0; i < info_in_use.size(); i++)
        if (idx == info_in_use[i].addr) {
            info_free.push_back(Memory_unit_info(info_in_use[i].addr, info_in_use[i].size));
            info_in_use.erase(info_in_use.begin() + i);
            break;
        }

}

void Memory::mem_dump() {
    int i = 0;
    while (i < memory_block.size()) {
        for (int j = 0; j < info_free.size(); j++)
            if (info_free[j].addr == i) {
                cout << string(info_free[j].size, '_');
                i += info_free[j].size;
            }

        for (int j = 0; j < info_in_use.size(); j++)
            if (info_in_use[j].addr == i) {
                cout << string(info_in_use[j].size, '*');
                i += info_in_use[j].size;
            }
    }
    cout << endl;
}

long Memory::what_number_am_i(void* addr) {
    if (addr == NULL)
        return -1;
    return ((long)addr - (long)&memory_block[0]) / sizeof(Memory_unit);
}

size_t Memory::size_with_align_4B(size_t size) {
    if (size % 4 == 0)
        return size;
    return size - size % 4 + 4;
}