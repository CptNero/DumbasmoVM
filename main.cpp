#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

enum Opcode {
    READ = 0,
    MOV = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
};

union Value {
    uint32_t address;
    double real;
};

struct Operand {
    bool is_memory;
    Value value;

    Operand() = default;

    Operand(const std::string& arg)
    {
      if (is_number(arg, value.real))
      {
        is_memory = false;
        return;
      }

      is_memory = true;
      value.address = arg[2] - '0';
    }

    bool is_number(const std::string& string, double &result) {
      char* end;
      result = strtod(string.c_str(), &end);
      if (end == string.c_str() || *end != '\0') return false;
      return true;
    }
};

static std::map<std::string, Opcode> opcode_map {
        {"READ", Opcode::READ },
        {"MOV", Opcode::MOV },
        {"ADD", Opcode::ADD },
        {"SUB", Opcode::SUB },
        {"MUL", Opcode::MUL },
        {"DIV", Opcode::DIV },
};

struct Command {
    Opcode opcode;
    Operand op1;
    Operand op2;
    bool isBinary;

    Command(const std::string& cmd)
    {
      std::vector<std::string> split_cmd;
      std::stringstream ss(cmd);
      std::string temp;
      while (ss >> temp)
      {
        split_cmd.emplace_back(temp);
      }

      isBinary = (split_cmd.size() > 2);
      opcode = opcode_map[split_cmd[0]];
      op1 = Operand(split_cmd[1]);
      if (isBinary)
      {
        op2 = Operand(split_cmd[2]);
      }
    }
};



int main(int argc, char *argv[]) {
  std::vector<double> memory(99, 0.0);

  if (argc != 2)
  {
    printf("Incorrect number of arguments");
    exit(-1);
  }

  FILE* filePtr = fopen(argv[1], "r");
  char* line = nullptr;
  size_t length = 0;

  while(getline(&line, &length, filePtr) != -1)
  {
    std::string cmd(line, length);
    Command command(cmd);

    switch (command.opcode) {
      case READ:
      {
        printf("Please enter a value for the register m[%u]\n", command.op1.value.address);
        std::cin >> memory[static_cast<uint32_t>(command.op1.value.address)];
        break;
      }
      case MOV:
      {
        if (command.op1.is_memory && command.op2.is_memory)
        {
          memory[command.op1.value.address] = memory[command.op2.value.address];
          break;
        }

        memory[command.op1.value.address] = command.op2.value.real;
        break;
      }
      case ADD:
      {
        if (command.op1.is_memory && command.op2.is_memory)
        {
          memory[command.op1.value.address] += memory[command.op2.value.address];
          break;
        }

        memory[command.op1.value.address] += command.op2.value.real;
        break;
      }
      case SUB:
      {
        if (command.op1.is_memory && command.op2.is_memory)
        {
          memory[command.op1.value.address] -= memory[command.op2.value.address];
          break;
        }

        memory[command.op1.value.address] -= command.op2.value.real;
        break;
      }
      case MUL:
      {
        if (command.op1.is_memory && command.op2.is_memory)
        {
          memory[command.op1.value.address] *= memory[command.op2.value.address];
          break;
        }

        memory[command.op1.value.address] *= command.op2.value.real;
        break;
      }
      case DIV:
      {
        if (command.op1.is_memory && command.op2.is_memory)
        {
          memory[command.op1.value.address] /= memory[command.op2.value.address];
          break;
        }

        memory[command.op1.value.address] /= command.op2.value.real;
        break;
      }
    }
  }

  for (uint32_t idx = 0; idx < memory.size(); idx++)
  {
    if (memory[idx] != 0.0)
    {
      printf("m[%u] = %f\n", idx, memory[idx]);
    }
  }

  return 0;
}
