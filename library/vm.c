#include "vm.h"
#include "op.h"

#define STACK_SIZE 256

typedef struct VM
{
    int *stack;
    int sp;
    Op *program;
    int len;
} VM;

VM *VMAlloc(void)
{
    VM *vm = malloc(sizeof(VM));

    size_t bytes = STACK_SIZE * sizeof(int);
    vm->stack = malloc(bytes);
    memset(vm->stack, 0, bytes);
    vm->sp = 0;

    vm->program = NULL;
    vm->len = 0;

    return vm;
}

void VMFree(VM *vm)
{
    free(vm->stack);
    vm->stack = NULL;

    if (vm->program != NULL && vm->len > 0)
    {
        free(vm->program);
        vm->program = NULL;
        vm->len = 0;
    }

    free(vm);
}

void VMReset(VM *vm)
{
    size_t bytes = STACK_SIZE * sizeof(int);
    memset(vm->stack, 0, bytes);
    vm->sp = 0;
}

void VMSetProgram(VM *vm, Op *program, int len)
{
    if (vm->program != NULL && vm->len > 0)
    {
        free(vm->program);
        vm->program = NULL;
        vm->len = 0;
    }

    if (program != NULL && len > 0)
    {
        vm->program = program;
        vm->len = len;
    }
}

static void vmPush(VM *vm, int v)
{
    vm->stack[vm->sp] = v;
    vm->sp = (vm->sp + 1) % STACK_SIZE;
}

static int vmPop(VM *vm)
{
    vm->sp = (vm->sp - 1);
    if (vm->sp < 0)
    {
        vm->sp = STACK_SIZE - 1;
    }
    return vm->stack[vm->sp];
}

uint8_t VMEval(VM *vm, int t)
{
    for (int ip = 0; ip < vm->len; ip++)
    {
        int op = vm->program[ip];
        switch (op)
        {
        case Push:
        {
            ip++;
            int val = vm->program[ip];
            vmPush(vm, val);
            break;
        }
        case T:
            vmPush(vm, t);
            break;
        case Add:
            vmPush(vm, vmPop(vm) + vmPop(vm));
            break;
        case Sub:
        {

            int b = vmPop(vm);
            int a = vmPop(vm);
            vmPush(vm, a - b);
            break;
        }
        case Mul:
            vmPush(vm, vmPop(vm) * vmPop(vm));
            break;
        case Div:
        {

            int b = vmPop(vm);
            int a = vmPop(vm);
            if (b == 0)
            {
                vmPush(vm, 0);
            }
            else
            {
                vmPush(vm, a / b);
            }
            break;
        }
        case Mod:
        {
            int b = vmPop(vm);
            int a = vmPop(vm);
            if (b == 0)
            {
                vmPush(vm, 0);
            }
            else
            {
                vmPush(vm, a % b);
            }
            break;
        }
        case Inc:
            vmPush(vm, vmPop(vm) + 1);
            break;
        case Dec:
            vmPush(vm, vmPop(vm) - 1);
            break;
        case Eq:
            vmPush(vm, vmPop(vm) == vmPop(vm));
            break;
        case Neq:
            vmPush(vm, vmPop(vm) != vmPop(vm));
            break;
        case Gt:
        {
            int b = vmPop(vm);
            int a = vmPop(vm);
            vmPush(vm, a > b);
            break;
        }
        case Lt:
        {
            int b = vmPop(vm);
            int a = vmPop(vm);
            vmPush(vm, a < b);
            break;
        }
        case Gte:
        {
            int b = vmPop(vm);
            int a = vmPop(vm);
            vmPush(vm, a >= b);
            break;
        }
        case Lte:
        {
            int b = vmPop(vm);
            int a = vmPop(vm);
            vmPush(vm, a <= b);
            break;
        }
        case And:
            vmPush(vm, vmPop(vm) && vmPop(vm));
            break;
        case Or:
            vmPush(vm, vmPop(vm) || vmPop(vm));
            break;
        case Not:
            vmPush(vm, !vmPop(vm));
            break;
        case Band:
            vmPush(vm, vmPop(vm) & vmPop(vm));
            break;
        case Bor:
            vmPush(vm, vmPop(vm) | vmPop(vm));
            break;
        case Bxor:
            vmPush(vm, vmPop(vm) ^ vmPop(vm));
            break;
        case Bnot:
            vmPush(vm, ~vmPop(vm));
            break;
        case Bsl:
        {
            int b = vmPop(vm);
            int a = vmPop(vm);
            vmPush(vm, a << b);
            break;
        }
        case Bsr:
        {
            int b = vmPop(vm);
            int a = vmPop(vm);
            vmPush(vm, a >> b);
            break;
        }
        }
    }

    return vmPop(vm);
}
