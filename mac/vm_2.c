#include <stdio.h>
#include <stdbool.h>

// 指令定义
typedef enum
{
  PSH,
  ADD,
  POP,
  SET,
  HLT,
  MOV,
  SUB,
  DIV,
  MUL,
  NOP,
  IF,
  LOG
} InstructionSet;

// 寄存器类型定义
typedef enum
{
  A,
  B,
  C,
  D,
  E,
  F,
  IP,
  SP,
  NUM_OF_REGISTERS
} Registers;

// 寄存器
int registers[NUM_OF_REGISTERS];

// 程序
const int program[] = {

    IF, B, 0, 6,
    PSH, 1,
    PSH, 2,
    PSH, 3,
    PSH, 4,
    PSH, 5,
    PSH, 6,
    PSH, 7,
    PSH, 8,
    ADD,
    MUL,
    DIV,
    SUB,
    POP,
    SET, C, 2,
    LOG, 4,
    MOV, B, A,
    HLT};

#define sp (registers[SP])
#define ip (registers[IP])

int stack[256];

bool is_jump = false;
bool running = true;

void printStack()
{
  printf("\n\n=========begin print stack:=========\n\n");

  for (int i = 0; i <= sp; i++)
  {
    printf("%d ", stack[i]);

    // 4 个一行
    if ((i + 1) % 4 == 0)
    {
      printf("\n");
    }
  }

  printf("\n\n=========print stack done=========\n\n");
}

void printRegisters()
{
  printf("\n\n=========begin print registers:=========\n\n");
  for (int i = 0; i < NUM_OF_REGISTERS; i++)
  {
    printf("%d ", registers[i]);
  }

  printf("\n\n=========print registers done=========\n\n");
}

void eval(int instr)
{
  is_jump = false;

  switch (instr)
  {
  case HLT:
  {
    running = false;
    break;
  }

  case PSH:
  {
    stack[++sp] = program[++ip];
    break;
  }

  case POP:
  {
    sp--;
    break;
  }

  case ADD:
  {
    // 从栈中取出两个数，相加，再 push 回栈
    int a = stack[sp--];
    int b = stack[sp--];

    int result = a + b;

    stack[++sp] = result;

    registers[A] = result;

    break;
  }

  case SUB:
  {
    // 从栈中取出两个数，相减，再 push 回栈
    int a = stack[sp--];
    int b = stack[sp--];

    int result = b - a;

    // 入栈
    stack[++sp] = result;
    registers[A] = result;

    break;
  }

  case MUL:
  {
    // 从栈中取出两个数，相乘，再 push 回栈
    int a = stack[sp--];
    int b = stack[sp--];

    int result = a * b;

    // 入栈
    stack[++sp] = result;
    registers[A] = result;

    break;
  }

  case DIV:
  {
    // 从栈中取出两个数，相除，再 push 回栈
    int a = stack[sp--];
    int b = stack[sp--];

    if (a != 0)
    {
      int result = b / a;

      // 入栈
      stack[++sp] = result;
      registers[A] = result;
    }
    else
    {
      printf("exception occur, divid 0 \n");
    }

    break;
  }

  case MOV:
  {
    // 将一个寄存器的值放到另一个寄存器中
    // 目的寄存器
    int dr = program[++ip];

    // 源寄存器
    int sr = program[++ip];

    // 源寄存器的值
    int sourceValue = registers[sr];
    registers[dr] = sourceValue;

    break;
  }

  case IF:
  {
    // 如果寄存器的值和后面的数值相等，则跳转
    int r = program[++ip];
    if (registers[r] == program[++ip])
    {
      ip = program[++ip];

      is_jump = true;
      printf("jump if:%d\n", ip);
    }
    else
    {
      ip += 3;
    }
    break;
  }

  case SET:
  {
    // set register value
    int r = program[++ip];
    int value = program[++ip];

    registers[r] = value;
    break;
  }

  case LOG:
  {
    int value = program[++ip];
    printf("log %d\n", value);
    break;
  }

  default:
  {
    printf("Unknown Instruction %d!\n", instr);
    break;
  }
  }
}

int main()
{
  //  初始化寄存器
  sp = -1;
  ip = 0;

  while (running)
  {
    int instr = program[ip];
    eval(instr);

    if (!is_jump)
    {
      ip++;
    }
  }

  printStack();

  printRegisters();

  return 0;
}