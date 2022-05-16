#include "op.h"
#include "pd_api.h"

#include <stdbool.h>

#define DELIMITER " "

typedef struct BuiltIn
{
    const char *token;
    Op op;
} BuiltIn;

#define NUM_BUILTINS 23

static const BuiltIn builtIns[NUM_BUILTINS] = {
    {"t", T},
    {"+", Add},
    {"-", Sub},
    {"*", Mul},
    {"/", Div},
    {"%", Mod},
    {"++", Inc},
    {"--", Dec},
    {"=", Eq},
    {"!=", Neq},
    {">", Gt},
    {"<", Lt},
    {">=", Gte},
    {"<=", Lte},
    {"&&", And},
    {"||", Or},
    {"!", Not},
    {"&", Band},
    {"|", Bor},
    {"^", Bxor},
    {"~", Bnot},
    {"<<", Bsl},
    {">>", Bsr}};

static int count(const char *input)
{
    char *dup = strdup(input);
    char *token = strtok(dup, DELIMITER);
    int n = 0;

    while (token != NULL)
    {
        bool found = false;
        for (int j = 0; j < NUM_BUILTINS; j++)
        {
            BuiltIn builtIn = builtIns[j];
            if (strcmp(token, builtIn.token) == 0)
            {
                n++;
                found = true;
                break;
            }
        }

        if (!found)
        {
            char *end;
            strtol(token, &end, 10);
            if (end == token)
            {
                return 0;
            }

            n += 2;
        }

        token = strtok(NULL, DELIMITER);
    }

    return n;
}

Op *parse(const char *input, int *len)
{
    *len = count(input);
    if (*len == 0)
    {
        return NULL;
    }

    Op *ops = malloc(*len * sizeof(Op));

    char *dup = strdup(input);
    char *token = strtok(dup, DELIMITER);
    for (int i = 0; i < *len; i++)
    {
        bool found = false;
        for (int j = 0; j < NUM_BUILTINS; j++)
        {
            BuiltIn builtIn = builtIns[j];
            if (strcmp(token, builtIn.token) == 0)
            {
                ops[i] = builtIn.op;
                found = true;
                break;
            }
        }

        if (!found)
        {
            int val = strtol(token, NULL, 10);
            ops[i] = Push;
            i++;
            ops[i] = val;
        }

        token = strtok(NULL, DELIMITER);
    }

    return ops;
}
