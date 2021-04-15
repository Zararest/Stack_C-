

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

private:
    int** array_of_canary;
    int canary_count;
};