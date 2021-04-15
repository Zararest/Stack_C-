#include <stdio.h>
#include <stdlib.h>
#include <cassert>

class Memory
{
public:

    Memory();

    ~Memory();

    Memory(const Memory&) = delete;

    Memory& operator = (const Memory&) = delete;

    void* calloc_class(int, int);

    void* realloc_class(void*, int);

    void free_class(void* point);

    int light_check();

    int memory_check (void* point);

    void memory_dump();

    void add_to_log(const char*);

    void add_to_log(const char*, long int);

private:
    FILE* Memory_log;
    long int** array_of_canary;
    int canary_count;
};