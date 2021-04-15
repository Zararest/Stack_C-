#include "Memory.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>


template <class T>
class Stack: public Memory{

    public:
    /*
   +1) скорее всего стек должен быть шаблонным 

    2) разобраться со ссылками тк нужны для перегрузки операторов 

   +3) канарейки будут через my_calloc 

   -4) будет только один массив 

   +5) надо сделать лог файл частью стека 

   +6) можно попробовать переделать класс канареек, сделав статические переменные обычными 

   +7) сделать наследование от класса канареек 

   +8) отдельный файл с расшифровкой ошибок

    9) наладить гитхаб для этого проекта 

    */
    Stack(int, int);

    ~Stack();

    Stack(const Stack&) = delete;

    T& operator = (const Stack&) = delete;

    T& operator [] (int);

    int push(T);

    T pop();

    void stack_resize();

    int dump(void (*type_print)(unsigned char*, FILE*));

    int silent_ok();

    int stack_recovery();

    int errors;

    private:

    unsigned char** arrays_of_data;

    int arrays_count;

    int cur_point;

    int size_of_elem;

    int size_of_stack;

    FILE* log;

};
