#include "stack_class.h"

//-------------этого не должно быть здесь 
int push(my_stack* _stack, double val)
{
    /*
    0 - ��� ���������
    1 - ���� ������������
    2 - ������������ ���
    */

    if ( silent_ok(_stack) != 0)
    {
        exit(4);
    }

    int i = 0, j = 0;
    char* str = (char*) calloc(MAXLEN, sizeof(char));

    if (sizeof(val) == _stack->size_of_elem)
    {

        if ( ( _stack->size_of_stack - 1) * _stack->size_of_elem >= _stack->cur_point )
        {

            for (i = 0; i < _stack->size_of_elem; i++)
            {

                for(j = 0; j < _stack->array_count; j++ )
                {
                    _stack->array_[j] [ _stack->cur_point + i ] =  ((unsigned char*) &val)[i];
                }

            }

            _stack->cur_point += i;
            _stack->push_count++;
            _stack->_hash = my_hash(_stack);

             return 0;

        } else
        {
            stack_resize(_stack);

            for (i = 0; i < _stack->size_of_elem; i++)
            {

                for(j = 0; j < _stack->array_count; j++ )
                {
                    _stack->array_[j] [ _stack->cur_point + i ] =  ((unsigned char*) &val)[i];
                }

            }

            _stack->cur_point += i;
            _stack->push_count++;
            _stack->_hash = my_hash(_stack);

            return 1;
        }

    } else
    {
        return 2;
    }

}
//-------------

my_stack* stack_create(int size_stack, int size_elem, int level_of_secur)
{
    int i = 0;
    my_stack* tmp = (my_stack*) calloc(1, sizeof(my_stack));

    if (size_stack <= 0)
    {
        printf("Stack size is  minimal now\n");
        size_stack = 1;
    }

    if (size_elem <= 0)
    {
        printf("Element size is  minimal now(1 byte)\n");
        size_elem = 1;
    }

    if (level_of_secur >= 0)
    {
        tmp->array_ = (unsigned char**) calloc(level_of_secur + 1, sizeof(unsigned char*));
        tmp->array_count = level_of_secur + 1;

        for (i = 0; i <= level_of_secur; i++)
        {

            tmp->array_[i] = (unsigned char*) calloc(size_stack, size_elem);
        }
    } else
    {
        printf("Stack security is  minimal now\n");
        tmp->array_ = (unsigned char**) calloc(1, sizeof(unsigned char*));
        tmp->array_count = 1;
        tmp->array_[0] = (unsigned char*) calloc(size_stack, size_elem);
    }

    tmp->size_of_elem = size_elem;
    tmp->cur_point = 0; //� ������
    tmp->push_count = 0;
    tmp->pop_count = 0;
    tmp->errors = 0;
    tmp->size_of_stack = size_stack; //���������� ���������
    tmp->_hash = my_hash(tmp);

    return tmp;
}


void stack_clean(my_stack* _stack)
{
    if (silent_ok(_stack) != 0)
    {
        printf("Stack have been damaged\n");
        exit(6);
    }

    int i = 0, j = 0;

    for (i = 0; i < _stack->array_count; i++)
    {
        for (j = 0; j < (_stack->size_of_elem) * (_stack->size_of_stack); j++ )
        {
            _stack->array_[i][j] = '0';
        }
    }

    _stack->cur_point = 0;
    _stack->push_count = 0;
    _stack->pop_count = 0;
    _stack->size_of_elem = 0;
    _stack->size_of_stack = 0;
    _stack->errors = 0;
    _stack->_hash = my_hash(_stack);
}


void stack_destruct(my_stack* _stack)
{
    if (silent_ok(_stack) != 0)
    {
        printf("Stack have been damaged (in destruct) silent_ok = %i\n", silent_ok);
        exit(5);
    }

    int i = 0;
    for (i = 0; i < _stack->array_count; i++)
    {
        free(_stack->array_[i]);
    }
    free(_stack->array_);
    free(_stack);
    _stack = NULL;
}

void stack_resize(my_stack* _stack)
{
    //�� ������ � ����������(����� ��������� ��������� � �������������� �������� �������) ����� ������ ������ ��������� � ������ �� 2 �� � ���� � 2 ���� ��� ���������
    int new_size = ((int) _stack->size_of_stack * 1.41) + 1;
    int j = 0, i = 0;

    for(j = 0; j < _stack->array_count; j++ )
    {
        _stack->array_[j] = (unsigned char*) realloc(_stack->array_[j], new_size * _stack->size_of_elem);

        for (i = _stack->cur_point; i < _stack->size_of_elem * new_size; i++)
        {
            _stack->array_[j] [i] = '0';
        }
    }
}


unsigned char* pop(my_stack* _stack)
{
    if (silent_ok(_stack) != 0)
    {
        exit(3);
    }

    unsigned char* tmp = (unsigned char*) calloc(_stack->size_of_elem + 1, sizeof(unsigned char));
    int i = 0, j = 0;

    if (tmp == NULL)
    {
        return NULL;
    }

    if (silent_ok(_stack) == 0)
    {
        if (_stack->cur_point >= _stack->size_of_elem)
        {

            for (i = 0; i < _stack->size_of_elem; i++)
            {
                tmp[i] = _stack->array_[0][_stack->cur_point - _stack->size_of_elem + i];

                for (j = 0; j < _stack->array_count; j++)
                {
                    _stack->array_[j][_stack->cur_point - _stack->size_of_elem + i] = '0';
                }
            }
            _stack->cur_point -= _stack->size_of_elem;
            _stack->pop_count++;

        } else
        {
            _stack->errors++;
            printf("This bitch is empty (in POP)\n");
            tmp = NULL;
        }
        _stack->_hash = my_hash(_stack);

        return tmp;

    } else
    {
        _stack->errors++;
        _stack->_hash = my_hash(_stack);
        free(tmp);

        return NULL;
    }
}


int silent_ok(my_stack* _stack)
{
    /*
    -n - ������ � �����(��������� �������. ��� ��� ������ �� ������ ��� ������ ���� �� ������)
    0 - ��� ���������
    1 - ����� ��� null
    2 - ���� ��������������� �����
    3 - ����� ������
    */
    if (_stack == NULL)
    {
        return 1;
    }

    int* num_of_each_elem = (int*) calloc(256, sizeof(int));
    int max_num_of_elem = 0, i = 0, j = 0, err_count = 0, position_of_max_elem = 0, count_of_max = 0, fatal_err_count = 0;

    for (i = 0; i < (_stack->size_of_stack * _stack->size_of_elem); i++ )
    {
        for (j = 0; j < _stack->array_count; j++)
        {
            num_of_each_elem[ _stack->array_[j][i] ]++;
        }

        for (j = 0; j < 256; j++)
        {
            if (num_of_each_elem[j] == max_num_of_elem)
            {
                count_of_max++;
            }

            if (num_of_each_elem[j] > max_num_of_elem)
            {
                max_num_of_elem = num_of_each_elem[j];
                position_of_max_elem = j;
                count_of_max = 0;
            }
        }

        if (max_num_of_elem != _stack->array_count)
        {
            err_count++;
        }
        if (count_of_max > 1)
        {
            fatal_err_count++;
        }


        for (j = 0; j < 256; j++)
        {
            num_of_each_elem[ j ] = 0;
        }
        count_of_max = 0;
        max_num_of_elem = 0;
    }


    if ((err_count == 0) && (my_hash(_stack) == _stack->_hash ))
    {
        return 0;
    } else
    {
        if (my_hash(_stack) != _stack->_hash)
        {
            return (-1) * abs(my_hash(_stack) - _stack->_hash);
        }

        if (fatal_err_count != 0)
        {
            return 3;
        } else
        {
            return 2;
        }


    }

}


int stack_recovery(my_stack* _stack)
{
    int* num_of_each_elem = (int*) calloc(256, sizeof(int));
    unsigned char* real_array = (unsigned char*) calloc( _stack->size_of_stack, _stack->size_of_elem);
    int* array_of_err = (int*) calloc( _stack->size_of_stack, _stack->size_of_elem);
    int max_num_of_elem = 0, i = 0, j = 0, err_count = 0, position_of_max_elem = 0, count_of_max = 0, fatal_err_count = 0;

    for (i = 0; i < (_stack->size_of_stack * _stack->size_of_elem); i++ )
    {
        for (j = 0; j < _stack->array_count; j++)
        {
            num_of_each_elem[ _stack->array_[j][i] ]++;
        }

        for (j = 0; j < 256; j++)
        {
            if (num_of_each_elem[j] == max_num_of_elem)
            {
                count_of_max++;
            }

            if (num_of_each_elem[j] > max_num_of_elem)
            {
                max_num_of_elem = num_of_each_elem[j];
                position_of_max_elem = j;
                count_of_max = 0;
            }
        }

        if (max_num_of_elem != _stack->array_count)
        {
            err_count++;
            array_of_err[i] = 1;
            real_array[i] = (char) j;
        }

        if (count_of_max > 1)
        {
            array_of_err[i] = -1;
            fatal_err_count++;
        }


        for (j = 0; j < _stack->array_count; j++)
        {
            num_of_each_elem[ _stack->array_[j][i] ] = 0;
        }
        count_of_max = 0;
        position_of_max_elem = 0;
        max_num_of_elem = 0;
    }

    if (err_count != 0)
    {
        printf("---ATTENTION/ACHTUNG/ATTENTION---\n");
        printf("Total number of errors:\n"
               "Number of corrupted stack bytes == %i\n", err_count);
        printf("Number of bytes, that could not been recovered == %i\n", fatal_err_count);
        printf("List of suspicious bytes:\n");

        for (i = 0; i < (_stack->size_of_stack * _stack->size_of_elem); i++ )
        {
            if (array_of_err[i] == 1)
            {
                printf("-byte of stack number |%i| was damaged and then recovered.(element of stack number |%i| includes this byte)\n", i, (i + 1)/_stack->size_of_elem);
                printf("[The real value of this byte is |%c|]\n", real_array[i]);

                for (j = 0; j < _stack->array_count; j++)
                {
                    _stack->array_[j] [i] = real_array[i];
                }


            } else
            {
                if (array_of_err[i] == -1)
                {
                    printf("-BYTE OF STACK NUMBER |%i| WAS LOST FOREVER.(element of stack number |%i| includes this byte)\n", i, (i + 1)/_stack->size_of_elem);
                }
            }
        }
    }
    _stack->_hash = my_hash(_stack);

    return err_count;
}

long int my_hash(my_stack* _stack)
{
    long int sum = 0, final_hash = 0;
    int i = 0, j = 0;


    if (_stack == NULL)
    {
        return -1;
    }

    for (i = 0; i < (_stack->size_of_stack * _stack->size_of_elem); i++ )
    {
        for (j = 0; j < _stack->array_count; j++)
        {
            sum+= _stack->array_[j][i];
        }
    }

    final_hash = sum * _stack->size_of_elem * _stack->size_of_elem;
    final_hash = final_hash * _stack->cur_point * _stack->size_of_stack;
    return (final_hash + _stack->pop_count + _stack->push_count +_stack->errors + _stack->array_count) * ((long int)_stack->array_);
}


int dump(my_stack* _stack, void (*type_print)(unsigned char*) )
{
    /*
    0 - ��� ���������
    1 - ����� ��� null
    */
    int i = 0;

    printf("Stack[%x] condition:\n", _stack);

    switch ( silent_ok(_stack) )
    {
    case 0:
        printf("Probably, stack is ok\n");
        break;

    case 1:
        printf("The stack pointer is NULL\n");
        return 1;

    case 2:
        printf("Some of bytes have been recovered\n");
        break;

    case 3:
        printf("Some of bytes have been fatally damaged\n");
        break;

    default:
        if (silent_ok(_stack) < 0)
        {
            printf("Hash was corrupted\n");
        }
    break;
    }

    printf("Number of errors, which appeared, while the program was running = %i\n\n", _stack->errors);

    printf(" -Current top point in stack: |%i|(point in bytes) |%i|(point in elements of stack)\n", _stack->cur_point, _stack->cur_point/_stack->size_of_elem );
    printf(" -Size of stack: |%i|(size in bytes) |%i|(size in elements of stack)\n");
    printf(" -Number of arrays in stack: |%i|\n", _stack->array_count);
    printf(" -Number of function 'pop' calls: |%i|. Number of function 'push' calls: |%i|\n", _stack->pop_count, _stack->push_count);

    printf("\n");
    stack_recovery(_stack);
    printf("\n");

    printf("Points to arrays:\n");
    for (i = 0; i < _stack->array_count; i++)
    {
        printf("   -array_of_array[%i] = |%x|\n", i, _stack->array_[i]);
    }

    printf("Data:\n");
    for (i = 0; i < (_stack->size_of_stack); i++)
    {
        if (i < (_stack->cur_point / _stack->size_of_elem) )
        {
            printf("   ");
            type_print(&_stack->array_[0][i * _stack->size_of_elem]);
            printf("\n");
        } else
        {
            printf("   ");
            type_print(&_stack->array_[0][i * _stack->size_of_elem]);
            printf(" ( should be poison '0' )\n");
        }
    }
}