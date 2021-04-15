#include "headers/stack.h"

template <typename T>
Stack<T>::Stack(int size_stack, int level_of_secur){

    int size_elem = sizeof(T);
    log = fopen("Stack_log.txt", "w");
    //log = stdout;
    ADD_TO_LOG("Constructor was called", 0);
    int i = 0;

    if (size_stack <= 0){

        ADD_TO_LOG("Stack size is  minimal now", 0);
        size_stack = 1;
    }

    if (level_of_secur >= 0){

        arrays_of_data = (unsigned char**) calloc_class(level_of_secur + 1, sizeof(unsigned char*));
        arrays_count = level_of_secur + 1;

        for (i = 0; i <= level_of_secur; i++){

            arrays_of_data[i] = (unsigned char*) calloc_class(size_stack, size_elem);
        }
    } else{

        ADD_TO_LOG("Stack security is  minimal now", 0);
        arrays_of_data = (unsigned char**) calloc_class(1, sizeof(unsigned char*));
        arrays_count = 1;
        arrays_of_data[0] = (unsigned char*) calloc_class(size_stack, size_elem);
    }

    size_of_elem = size_elem;
    cur_point = 0;
    errors = 0;
    size_of_stack = size_stack;  
}

template <typename T>
Stack<T>::~Stack(){

    ADD_TO_LOG("Destructor was_called", 0);
    CONDITION_CHECK(EXIT_IN_DESTR)

    int i = 0;
    for (i = 0; i < arrays_count; i++)
    {
        free_class(arrays_of_data[i]);
    }
    free_class(arrays_of_data);

    errors = 0;
    arrays_count = 0;
    cur_point = 0;
    size_of_elem = 0;
    size_of_stack = 0;

    fclose(log);
}


template <typename T>
int Stack<T>::push(T val){//добавить в лог

    /*
    0 - все нормально
    1 - переполнение
    2 - неправильный тип
    */
    int i = 0, j = 0;

    CONDITION_CHECK(EXIT_IN_PUSH)

    if (sizeof(val) == size_of_elem){

        if ( (size_of_stack - 1) * size_of_elem >= cur_point ){

            for (i = 0; i < size_of_elem; i++){

                for(j = 0; j < arrays_count; j++){

                    arrays_of_data[j] [ cur_point + i ] =  ((unsigned char*) &val)[i];
                }

            }
            
            cur_point += i;

            ADD_TO_LOG("push", PUSH);
            return 0;

        } else{

            stack_resize();

            for (i = 0; i < size_of_elem; i++)
            {

                for(j = 0; j < arrays_count; j++ )
                {
                    arrays_of_data[j] [ cur_point + i ] =  ((unsigned char*) &val)[i];
                }

            }

            cur_point += i;

            ADD_TO_LOG("push", PUSH_RESIZE);
            return 1;
        }

    } else{
        ADD_TO_LOG("push", PUSH_WRONG_SIZE);
        return 2;
    }
}


template <typename T>
T Stack<T>::pop(){

    int i = 0, j = 0;

    CONDITION_CHECK(EXIT_IN_POP)
    
    unsigned char* tmp = (unsigned char*) calloc_class(size_of_elem + 1, sizeof(unsigned char));
    assert(tmp != NULL);

    if (cur_point >= size_of_elem){

        for (i = 0; i < size_of_elem; i++){

            tmp[i] = arrays_of_data[0][cur_point - size_of_elem + i];

            for (j = 0; j < arrays_count; j++){

                arrays_of_data[j][cur_point - size_of_elem + i] = '0';
            }
        }
        cur_point -= size_of_elem;

    } else{

        errors++;
        ADD_TO_LOG("pop:", POP_EMPTY)
        return 0;
    }

    ADD_TO_LOG("pop:", POP)
    T strange_tmp = *((T*)tmp);
    
    free_class(tmp);
    printf("dafuq\n");
    return strange_tmp;
}

template <typename T>
void Stack<T>::stack_resize(){

    int new_size = ((int) size_of_stack * 1.41) + 1;
    int j = 0, i = 0;

    for(j = 0; j < arrays_count; j++){

        arrays_of_data[j] = (unsigned char*) realloc_class(arrays_of_data[j], new_size * size_of_elem);

        for (i = cur_point; i < size_of_elem * new_size; i++){
            
            arrays_of_data[j][i] = '0';
        }
    }
}


template <typename T>
int Stack<T>::dump(void (*type_print)(unsigned char*, FILE*)){

    int i = 0;
    FILE* dump_file = fopen("stack_dump.txt", "w");

    fprintf(dump_file, "Stack condition:\n" );

    switch ( silent_ok() ){

    case 0:
        printf("Probably, stack is ok\n");
        printf("is stack ok?");
        system("yes");//надо удалить(скорее всего наеб)
        //int a = *(0);
        break;

    case 1:
        fprintf(dump_file, "The stack pointer is NULL\n");
        return 1;

    case 2:
        fprintf(dump_file, "Some of bytes have been recovered\n");
        break;

    case 3:
        fprintf(dump_file, "Some of bytes have been fatally damaged\n");
        break;

    }

    fprintf(dump_file, "Number of errors, which appeared, while the program was running = %i\n\n", errors);

    fprintf(dump_file, " -Current top point in stack: |%i|(point in bytes) |%i|(point in elements of stack)\n", cur_point, cur_point / size_of_elem );
    fprintf(dump_file, " -Size of stack: |%i|(size in bytes) |%i|(size in elements of stack)\n");
    fprintf(dump_file, " -Number of arrays in stack: |%i|\n", arrays_count);

    fprintf(dump_file, "\n");
    stack_recovery();
    fprintf(dump_file, "\n");

    fprintf(dump_file, "Points to arrays:\n");
    for (i = 0; i < arrays_count; i++){

        fprintf(dump_file, "   -array_of_array[%i] = |%x|\n", i, arrays_of_data[i]);
    }

    fprintf(dump_file, "Data:\n");
    for (i = 0; i < size_of_stack; i++)
    {
        if (i < cur_point / size_of_elem )
        {
            fprintf(dump_file, "   ");
            type_print(&arrays_of_data[0][i * size_of_elem], dump_file);
            fprintf(dump_file, "\n");
        } else
        {
            fprintf(dump_file, "   ");
            type_print(&arrays_of_data[0][i * size_of_elem], dump_file);
            fprintf(dump_file, " ( should be poison '0' )\n");
        }
    }    
}

template <typename T>
int Stack<T>::silent_ok(){
    
    int* num_of_each_elem = (int*) calloc(256, sizeof(int));
    assert(num_of_each_elem != NULL);
    int max_num_of_elem = 0, i = 0, j = 0, err_count = 0, position_of_max_elem = 0, count_of_max = 0, fatal_err_count = 0;

    for (i = 0; i < (size_of_stack * size_of_elem); i++ ){

        for (j = 0; j < arrays_count; j++){

            num_of_each_elem[ arrays_of_data[j][i] ]++;
        }

        for (j = 0; j < 256; j++){

            if (num_of_each_elem[j] == max_num_of_elem){

                count_of_max++;
            }

            if (num_of_each_elem[j] > max_num_of_elem){

                max_num_of_elem = num_of_each_elem[j];
                position_of_max_elem = j;
                count_of_max = 0;
            }
        }

        if (max_num_of_elem != arrays_count){

            err_count++;
        }
        if (count_of_max > 1){

            fatal_err_count++;
        }


        for (j = 0; j < 256; j++){

            num_of_each_elem[j] = 0;
        }
        count_of_max = 0;
        max_num_of_elem = 0;
    }

    if (err_count == 0){

        free(num_of_each_elem);
        return 0;
    } else
    {
        if (fatal_err_count != 0){
            
            free(num_of_each_elem);
            return 3;
        } else{
            
            free(num_of_each_elem);
            return 2;
        }
    }
}

template <typename T>
int Stack<T>::stack_recovery(){

    int* num_of_each_elem = (int*) calloc(256, sizeof(int));
    unsigned char* real_array = (unsigned char*) calloc(size_of_stack, size_of_elem);
    int* array_of_err = (int*) calloc(size_of_stack, size_of_elem);
    int max_num_of_elem = 0, i = 0, j = 0, err_count = 0, position_of_max_elem = 0, count_of_max = 0, fatal_err_count = 0;

    for (i = 0; i < (size_of_stack * size_of_elem); i++){

        for (j = 0; j < arrays_count; j++){
  
            num_of_each_elem [ arrays_of_data[j][i] ]++;
        }

        for (j = 0; j < 256; j++){

            if (num_of_each_elem[j] == max_num_of_elem){

                count_of_max++;
            }

            if (num_of_each_elem[j] > max_num_of_elem){

                max_num_of_elem = num_of_each_elem[j];
                position_of_max_elem = j;
                count_of_max = 0;
            }
        }

        if (max_num_of_elem != arrays_count){

            err_count++;
            array_of_err[i] = 1;
            real_array[i] = (char) j;
        }

        if (count_of_max > 1){

            array_of_err[i] = -1;
            fatal_err_count++;
        }


        for (j = 0; j < arrays_count; j++){

            num_of_each_elem[ arrays_of_data[j][i] ] = 0;
        }
        count_of_max = 0;
        position_of_max_elem = 0;
        max_num_of_elem = 0;
    }

    if (err_count != 0){

        fprintf(log, "---ATTENTION/ACHTUNG/ATTENTION---\n");
        fprintf(log, "Total number of errors:\n"
               "Number of corrupted stack bytes == %i\n", err_count);
        fprintf(log, "Number of bytes, that could not been recovered == %i\n", fatal_err_count);
        fprintf(log, "List of suspicious bytes:\n");

        for (i = 0; i < size_of_stack * size_of_elem; i++ ){

            if (array_of_err[i] == 1){

                fprintf(log,  "-byte of stack number |%i| was damaged and then recovered.(element of stack number |%i| includes this byte)\n", i, (i + 1) / size_of_elem);
                fprintf(log, "[The real value of this byte is |%c|]\n", real_array[i]);

                for (j = 0; j < arrays_count; j){

                    arrays_of_data[j][i] = real_array[i];
                }


            } else
            {
                if (array_of_err[i] == -1){

                    fprintf(log, "-BYTE OF STACK NUMBER |%i| WAS LOST FOREVER.(element of stack number |%i| includes this byte)\n", i, (i + 1) / size_of_elem);
                } 
            }
        }
    }

    return err_count;
}


