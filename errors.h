#define ADD_TO_LOG(str, num) fprintf(log, "%s [%i]\n", str, log);
#define CONDITION_CHECK(exit_code) if (silent_ok() != 0){\
                                       exit(exit_code); }\

enum stack_events{

    PUSH = 0,
    PUSH_RESIZE = 1,
    PUSH_WRONG_SIZE = 2,

    POP = 3,
    POP_EMPTY = 4,

    FATAL_ERROR = 5,
    LIGHT_ERROR = 6,

};

enum stack_errors{

    EXIT_IN_PUSH = 1,
    EXIT_IN_POP = 2,
    EXIT_IN_DESTR = 3,
};