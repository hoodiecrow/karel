#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "world.h"
#include "vm.h"

VM vm; 

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

static Value worldNative(int argCount, Value* args) {
    if (argCount != 2) {
        runtimeError("Expected 2 arguments but got %d.", argCount);
        return ERR_VAL;
    }
    Value avenues = args[0];
    if (!IS_NUMBER(avenues)) {
        runtimeError("avenues is not a number.");
        return ERR_VAL;
    }
    Value streets = args[1];
    if (!IS_NUMBER(streets)) {
        runtimeError("streets is not a number.");
        return ERR_VAL;
    }
    return initWorld(AS_NUMBER(avenues), AS_NUMBER(streets)) ? ERR_VAL : NIL_VAL;
}

static Value robotNative(int argCount, Value* args) {
    if (argCount != 4) {
        runtimeError("Expected 4 arguments but got %d.", argCount);
        return ERR_VAL;
    }
    Value avenue = args[0];
    if (!IS_NUMBER(avenue)) {
        runtimeError("avenue is not a number.");
        return ERR_VAL;
    }
    Value street = args[1];
    if (!IS_NUMBER(street)) {
        runtimeError("street is not a number.");
        return ERR_VAL;
    }
    Value direction = args[2];
    if (!IS_NUMBER(direction)) {
        runtimeError("direction is not a number.");
        return ERR_VAL;
    }
    Value beepers = args[3];
    if (!IS_NUMBER(beepers)) {
        runtimeError("beepers is not a number.");
        return ERR_VAL;
    }
    return initRobot(AS_NUMBER(avenue), AS_NUMBER(street),
            AS_NUMBER(direction), AS_NUMBER(beepers)) ? ERR_VAL : NIL_VAL;
}

static Value homeNative(int argCount, Value* args) {
    if (argCount != 3) {
        runtimeError("Expected 3 arguments but got %d.", argCount);
        return ERR_VAL;
    }
    Value avenue = args[0];
    if (!IS_NUMBER(avenue)) {
        runtimeError("avenue is not a number.");
        return ERR_VAL;
    }
    Value street = args[1];
    if (!IS_NUMBER(street)) {
        runtimeError("street is not a number.");
        return ERR_VAL;
    }
    Value direction = args[2];
    if (!IS_NUMBER(direction)) {
        runtimeError("direction is not a number.");
        return ERR_VAL;
    }
    return placeHome(AS_NUMBER(avenue), AS_NUMBER(street),
            AS_NUMBER(direction)) ? ERR_VAL : NIL_VAL;
}

static Value beepersNative(int argCount, Value* args) {
    if (argCount != 3) {
        runtimeError("Expected 3 arguments but got %d.", argCount);
        return ERR_VAL;
    }
    Value avenue = args[0];
    if (!IS_NUMBER(avenue)) {
        runtimeError("avenue is not a number.");
        return ERR_VAL;
    }
    Value street = args[1];
    if (!IS_NUMBER(street)) {
        runtimeError("street is not a number.");
        return ERR_VAL;
    }
    Value number = args[2];
    if (!IS_NUMBER(number)) {
        runtimeError("number is not a number."); // oh yes
        return ERR_VAL;
    }
    return placeBeepers(AS_NUMBER(avenue), AS_NUMBER(street),
            AS_NUMBER(number)) ? ERR_VAL : NIL_VAL;
}

static Value wallNative(int argCount, Value* args) {
    if (argCount != 3) {
        runtimeError("Expected 3 arguments but got %d.", argCount);
        return ERR_VAL;
    }
    Value avenue = args[0];
    if (!IS_NUMBER(avenue)) {
        runtimeError("avenue is not a number.");
        return ERR_VAL;
    }
    Value street = args[1];
    if (!IS_NUMBER(street)) {
        runtimeError("street is not a number.");
        return ERR_VAL;
    }
    Value direction = args[2];
    if (!IS_NUMBER(direction)) {
        runtimeError("direction is not a number.");
        return ERR_VAL;
    }
    return placeWall(AS_NUMBER(avenue), AS_NUMBER(street), AS_NUMBER(direction)) ? ERR_VAL : NIL_VAL;
}

static Value condFacingDirNative(int argCount, Value* args) {
    if (argCount != 1) {
        runtimeError("Expected 1 argument but got %d.", argCount);
        return ERR_VAL;
    }
    Value direction = args[0];
    if (!IS_NUMBER(direction)) {
        runtimeError("direction is not a number.");
        return ERR_VAL;
    }
    return BOOL_VAL(facingDirection(AS_NUMBER(direction)));
}

static Value condBlockedFacingNative(int argCount, Value* args) {
    if (argCount != 1) {
        runtimeError("Expected 1 argument but got %d.", argCount);
        return ERR_VAL;
    }
    Value facing = args[0];
    if (!IS_NUMBER(facing)) {
        runtimeError("facing is not a number.");
        return ERR_VAL;
    }
    int f = AS_NUMBER(facing);
    if (!(f == 0 || f == 1 || f == -1)) {
        runtimeError("invalid facing.");
        return ERR_VAL;
    }
    return BOOL_VAL(facingIsBlocked(f));
}

static Value condBeepersAtCornerNative(int argCount, Value* args) {
    if (argCount != 0) {
        runtimeError("Expected 0 arguments but got %d.", argCount);
        return ERR_VAL;
    }
    return BOOL_VAL(!noBeepersAtCorner());
}

static Value condBeeperBagEmptyNative(int argCount, Value* args) {
    if (argCount != 0) {
        runtimeError("Expected 0 arguments but got %d.", argCount);
        return ERR_VAL;
    }
    return BOOL_VAL(beeperBagEmpty());
}

#pragma clang diagnostic pop

static void resetStack() {
  vm.stackTop = vm.stack;
  vm.frameCount = 0;
}

void runtimeError(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  for (int i = vm.frameCount - 1; i >= 0; i--) {
    CallFrame* frame = &vm.frames[i];
    ObjFunction* function = frame->function;
    size_t instruction = frame->ip - function->chunk.code - 1;
    fprintf(stderr, "[line %d] in ", 
            function->chunk.lines[instruction]);
    if (function->name == NULL) {
      fprintf(stderr, "script\n");
    } else {
      fprintf(stderr, "%s()\n", function->name->chars);
    }
  }

  resetStack();
}

static void defineNative(const char* name, NativeFn function) {
  push(OBJ_VAL(copyString(name, (int)strlen(name))));
  push(OBJ_VAL(newNative(function)));
  tableSet(&vm.globals, AS_STRING(vm.stack[0]), vm.stack[1]);
  pop();
  pop();
}

void initVM() {
  resetStack();
  vm.objects = NULL;

  initTable(&vm.globals);
  initTable(&vm.strings);

  defineNative("world", worldNative);
  defineNative("robot", robotNative);
  defineNative("home", homeNative);
  defineNative("beepers", beepersNative);
  defineNative("wall", wallNative);
  defineNative("facing", condFacingDirNative);
  defineNative("blocked", condBlockedFacingNative);
  defineNative("beepersAtCorner", condBeepersAtCornerNative);
  defineNative("beeperBagEmpty", condBeeperBagEmptyNative);
}

void freeVM() {
  freeTable(&vm.globals);
  freeTable(&vm.strings);
  freeObjects();
}

void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static Value peek(int distance) {
  return vm.stackTop[-1 - distance];
}

static bool call(ObjFunction* function, int argCount) {
  if (argCount != function->arity) {
    runtimeError("Expected %d arguments but got %d.",
        function->arity, argCount);
    return false;
  }

  if (vm.frameCount == FRAMES_MAX) {
    runtimeError("Stack overflow.");
    return false;
  }

  CallFrame* frame = &vm.frames[vm.frameCount++];
  frame->function = function;
  frame->ip = function->chunk.code;
  frame->slots = vm.stackTop - argCount - 1;
  return true;
}

static bool callValue(Value callee, int argCount) {
  if (IS_OBJ(callee)) {
    switch (OBJ_TYPE(callee)) {
      case OBJ_FUNCTION: 
        return call(AS_FUNCTION(callee), argCount);
      case OBJ_NATIVE: {
        NativeFn native = AS_NATIVE(callee);
        Value result = native(argCount, vm.stackTop - argCount);
        vm.stackTop -= argCount + 1;
        if (valuesEqual(result, ERR_VAL))
            return false;
        push(result);
        return true;
      }
      default:
        break; // Non-callable object type.
    }
  }
  runtimeError("Can only call functions and classes.");
  return false;
}

static bool isFalsey(Value value) {
  return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate() {
  ObjString* b = AS_STRING(pop());
  ObjString* a = AS_STRING(pop());

  int length = a->length + b->length;
  char* chars = ALLOCATE(char, length + 1);
  memcpy(chars, a->chars, a->length);
  memcpy(chars + a->length, b->chars, b->length);
  chars[length] = '\0';

  ObjString* result = takeString(chars, length);
  push(OBJ_VAL(result));
}

void markFirstMove(void) {
  mvaddstr(0, 0, "Press any key to start");
  clrtoeol();
  refresh();
  getch();
  mvaddstr(0, 0, "");
  clrtoeol();
}

void showError(const char *msg) {
    mvprintw(0, 0, "Error--%s", msg);
    clrtoeol();
    refresh();
    getch();
}

static InterpretResult run() {
  CallFrame* frame = &vm.frames[vm.frameCount - 1];

#define READ_BYTE() (*frame->ip++)

#define READ_SHORT() \
    (frame->ip += 2, \
    (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]))

#define READ_CONSTANT() \
    (frame->function->chunk.constants.values[READ_BYTE()])

#define READ_STRING() AS_STRING(READ_CONSTANT())
#define BINARY_OP(valueType, op) \
    do { \
      if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
        runtimeError("Operands must be numbers."); \
        return INTERPRET_RUNTIME_ERROR; \
      } \
      double b = AS_NUMBER(pop()); \
      double a = AS_NUMBER(pop()); \
      push(valueType(a op b)); \
    } while (false)

  bool firstMove = true;
  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(&frame->function->chunk,
        (int)(frame->ip - frame->function->chunk.code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }
      case OP_NIL: push(NIL_VAL); break;
      case OP_TRUE: push(BOOL_VAL(true)); break;
      case OP_FALSE: push(BOOL_VAL(false)); break;
      case OP_POP: pop(); break;
      case OP_GET_LOCAL: {
        uint8_t slot = READ_BYTE();
        push(frame->slots[slot]);
        break;
      }
      case OP_SET_LOCAL: {
        uint8_t slot = READ_BYTE();
        frame->slots[slot] = peek(0);
        break;
      }
      case OP_GET_GLOBAL: {
        ObjString* name = READ_STRING();
        Value value;
        if (!tableGet(&vm.globals, name, &value)) {
          runtimeError("Undefined variable '%s'.", name->chars);
          return INTERPRET_RUNTIME_ERROR;
        }
        push(value);
        break;
      }
      case OP_DEFINE_GLOBAL: {
        ObjString* name = READ_STRING();
        tableSet(&vm.globals, name, peek(0));
        pop();
        break;
      }
      case OP_SET_GLOBAL: {
        ObjString* name = READ_STRING();
        if (tableSet(&vm.globals, name, peek(0))) {
          tableDelete(&vm.globals, name); 
          runtimeError("Undefined variable '%s'.", name->chars);
          return INTERPRET_RUNTIME_ERROR;
        }
        break;
      }
      case OP_EQUAL: {
        Value b = pop();
        Value a = pop();
        push(BOOL_VAL(valuesEqual(a, b)));
        break;
      }
      case OP_GREATER:  BINARY_OP(BOOL_VAL, >); break;
      case OP_LESS:     BINARY_OP(BOOL_VAL, <); break;
      case OP_ADD: {
        if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
          concatenate();
        } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
          double b = AS_NUMBER(pop());
          double a = AS_NUMBER(pop());
          push(NUMBER_VAL(a + b));
        } else {
          runtimeError(
              "Operands must be two numbers or two strings.");
          return INTERPRET_RUNTIME_ERROR;
        }
        break;
      }
      case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
      case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
      case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
      case OP_NOT:
        push(BOOL_VAL(isFalsey(pop())));
        break;
      case OP_NEGATE:
        if (!IS_NUMBER(peek(0))) {
          runtimeError("Operand must be a number.");
          return INTERPRET_RUNTIME_ERROR;
        }
        push(NUMBER_VAL(-AS_NUMBER(pop())));
        break;
      case OP_PRINT: {
        printValue(pop());
        printf("\n");
        break;
      }
      case OP_JUMP: {
        uint16_t offset = READ_SHORT();
        frame->ip += offset;
        break;
      }
      case OP_JUMP_IF_FALSE: {
        uint16_t offset = READ_SHORT();
        if (isFalsey(peek(0))) frame->ip += offset;
        break;
      }
      case OP_LOOP: {
        uint16_t offset = READ_SHORT();
        frame->ip -= offset;
        break;
      }
      case OP_CALL: {
        int argCount = READ_BYTE();
        if (!callValue(peek(argCount), argCount)) {
          return INTERPRET_RUNTIME_ERROR;
        }
        frame = &vm.frames[vm.frameCount - 1];
        break;
      }
      case OP_MOVE:
        if (firstMove) {
            markFirstMove();
            firstMove = false;
        }
        if (facingIsBlocked(0)) {
            showError("movement is blocked\n");
            runtimeError("Forbidden movement.");
            return INTERPRET_RUNTIME_ERROR;
        }
        showCorner(karel.avenue, karel.street);
        moveToNext();
        showRobot();
        // refresh() ?
        napms(900);
        break;
      case OP_LEFT:
        if (firstMove) {
            markFirstMove();
            firstMove = false;
        }
        turnLeft();
        showRobot();
        napms(900);
        break;
      case OP_COLOR: {
          Value v = pop();
          if (!IS_NUMBER(v)) {
              showError("color must be a number");
            runtimeError("Data error.");
            return INTERPRET_RUNTIME_ERROR;
          }
          setColor(AS_NUMBER(v));
        }
        break;
      case OP_DONE: {
        // compare world and robot to expected outcome
        bool homeIsOk = false;
        if (homeDefined()) {
            // compare robot's x, y, d to home
            homeIsOk = (home.avenue == karel.avenue &&
                home.street == karel.street &&
                home.direction == karel.direction);
        } else {
            homeIsOk = true;
        }
        bool beepsIsOk = false;
        // compare colors and beepers in world to expected
        mvprintw(0, 0, "%s--press any key to exit", (homeIsOk && beepsIsOk) ?
                "Success" : "Failure");
        clrtoeol();
        refresh();
        getch();
        }
        break;
      case OP_GET:
        if (firstMove) {
            markFirstMove();
            firstMove = false;
        }
          if (noBeepersAtCorner()) {
            showError("no beepers here");
            runtimeError("No beepers at current corner.");
            return INTERPRET_RUNTIME_ERROR;
          }
          decrementBeepersAtCorner();
          incrementBeeperBag();
        break;
      case OP_PUT:
        if (firstMove) {
            markFirstMove();
            firstMove = false;
        }
          if (beeperBagEmpty()) {
            showError("no beepers in bag\n");
            runtimeError("No beepers in beeper bag.");
            return INTERPRET_RUNTIME_ERROR;
          }
          if (world[karel.avenue][karel.street].beepers == 99) {
            showError("too many beepers in corner\n");
            runtimeError("Too many beepers in corner.");
            return INTERPRET_RUNTIME_ERROR;
          }
          decrementBeeperBag();
          incrementBeepersAtCorner();
        break;
      case OP_RETURN: {
        Value result = pop();
        vm.frameCount--;
        if (vm.frameCount == 0) {
          pop();
          return INTERPRET_OK;
        }

        vm.stackTop = frame->slots;
        push(result);
        frame = &vm.frames[vm.frameCount - 1];
        break;
      }
    }
  }
#undef READ_BYTE
#undef READ_SHORT
#undef READ_CONSTANT
#undef READ_STRING
#undef BINARY_OP
}

InterpretResult interpret(const char* source) {
  ObjFunction* function = compile(source);
  if (function == NULL) return INTERPRET_COMPILE_ERROR;

  push(OBJ_VAL(function));
  call(function, 0);

  initscr();
  noecho();
  InterpretResult result = run();
  endwin();
  return result;
}
