#pragma once

#include "utils.hpp"

class Day17
{
public:
    struct State
    {
        bool _exit;
        int _pc = 0;
        int64_t _regA = 0;
        int64_t _regB = 0;
        int64_t _regC = 0;
        vector<glm::ivec2> _instructions;
        vector<int> _program;
        vector<int> _output;

        bool isSameProgram(vector<int> &output) const
        {
            if (_program.size() != output.size())
            {
                return false;
            }
            for (int i = 0; i < _program.size(); i++)
            {
                if (_program[i] != output[i])
                {
                    return false;
                }
            }
            return true;
        }

        bool isSameProgramPrefix(vector<int> &output) const
        {
            int i = (int)_program.size() - 1;
            int j = (int)output.size() - 1;
            if (output.size() == 0)
            {
                return false;
            }
            while (i >= 0 && j >= 0)
            {
                if (_program[i] != output[j])
                {
                    return false;
                }
                i--;
                j--;
            }

            return true;
        }
    };
    State _orig;

    Day17() {}
    virtual ~Day17() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day17/test.txt"
                               : "res/Day17/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            if (line.empty())
            {
                continue;
            }

            std::regex registerARegex(R"(Register A: (\d+))");
            std::smatch match;
            if (std::regex_search(line, match, registerARegex))
            {
                int x = std::stoi(match[1]);
                _orig._regA = (int64_t)x;
                continue;
            }

            std::regex registerBRegex(R"(Register B: (\d+))");
            std::smatch match2;
            if (std::regex_search(line, match2, registerARegex))
            {
                int x = std::stoi(match2[1]);
                _orig._regB = (int64_t)x;
                ;
                continue;
            }

            std::regex registerCRegex(R"(Register C: (\d+))");
            std::smatch match3;
            if (std::regex_search(line, match3, registerARegex))
            {
                int x = std::stoi(match3[1]);
                _orig._regC = (int64_t)x;
                continue;
            }

            if (line.starts_with("Program: "))
            {
                vector<string> tokens = Utils::split(line.substr(9), ", ");
                M_Assert(tokens.size() % 2 == 0, "Expected even number of tokens");
                for (int i = 0; i < tokens.size(); i += 2)
                {
                    _orig._program.push_back(std::stoi(tokens[i]));
                    _orig._program.push_back(std::stoi(tokens[i + 1]));
                    _orig._instructions.push_back({std::stoi(tokens[i]), std::stoi(tokens[i + 1])});
                }
            }
        }

        printf("Register A: %lld\n", _orig._regA);
        printf("Register B: %lld\n", _orig._regB);
        printf("Register C: %lld\n", _orig._regC);
        for (int i = 0; i < _orig._instructions.size(); i++)
        {
            printf("%d %d\n", _orig._instructions[i].x, _orig._instructions[i].y);
        }
        printf("\n");
    }

    int64_t comboOperand(State &state, int64_t operand)
    {
        switch (operand)
        {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return state._regA;
        case 5:
            return state._regB;
        case 6:
            return state._regC;
        case 7:
            M_Assert(false, "Invalid operand");
            return 0;
        default:
            M_Assert(false, "Invalid operand");
            return 0;
        }
    }

    void processADV(int opCode, int64_t operand, State &state)
    {
        // printf("[%d] ADV %d\n", state.programCounter, operand);

        int64_t numerator = state._regA;
        int64_t combo = comboOperand(state, operand);
        M_Assert(combo <= 8, "ADV combo operand out of range");

        int64_t denom = (int64_t)pow(2, combo);
        state._regA = (int64_t)numerator / denom;
        state._pc += 1;
    }
    void processBXL(int opCode, int64_t operand, State &state)
    {
        // printf("[%d] BXL %d\n", state.programCounter, operand);
        state._regB ^= operand;
        state._pc += 1;
    }
    void processBST(int opCode, int64_t operand, State &state)
    {
        // printf("[%d] BST %d\n", state.programCounter, operand);
        state._regB = comboOperand(state, operand) % 8;
        state._pc += 1;
    }
    void processJNZ(int opCode, int64_t operand, State &state)
    {
        // printf("[%d] JNZ %d\n", state.programCounter, operand);
        if (state._regA == 0)
        {
            state._pc += 1;
        }
        else
        {
            state._pc = (int)operand;
            // not increment of program counter;
        }
    }
    void processBXC(int opCode, int64_t operand, State &state)
    {
        // printf("[%d] BXC %d\n", state.programCounter, operand);
        state._regB = state._regB ^ state._regC;
        state._pc += 1;
    }
    void processOUT(int opCode, int64_t operand, State &state)
    {
        // printf("[%d] OUT %d\n", state.programCounter, operand);
        int output = (int)(comboOperand(state, operand) % 8);
        state._output.push_back(output);
        state._pc += 1;

        if (state._output.size() > 20)
        {
            state._exit = true;
            return;
        }
    }
    void processBDV(int opCode, int64_t operand, State &state)
    {
        // printf("[%d] BDV %d\n", state.programCounter, operand);
        int64_t numerator = state._regA;
        int64_t combo = comboOperand(state, operand);
        M_Assert(combo <= 8, "BDV combo operand out of range");
        int64_t denom = (int64_t)pow(2, combo);
        state._regB = (int64_t)numerator / denom;
        state._pc += 1;
    }
    void processCDV(int opCode, int64_t operand, State &state)
    {
        // printf("[%d] CDV %d\n", state.programCounter, operand);
        int64_t numerator = state._regA;
        int64_t combo = comboOperand(state, operand);
        M_Assert(combo <= 8, "BDV combo operand out of range");
        int64_t denom = (int64_t)pow(2, combo);
        state._regC = (int64_t)numerator / denom;
        state._pc += 1;
    }

    void processInstruction(int opCode, int64_t operand, State &state)
    {
        switch (opCode)
        {
        case 0:
            processADV(opCode, operand, state);
            break;
        case 1:
            processBXL(opCode, operand, state);
            break;
        case 2:
            processBST(opCode, operand, state);
            break;
        case 3:
            processJNZ(opCode, operand, state);
            break;
        case 4:
            processBXC(opCode, operand, state);
            break;
        case 5:
            processOUT(opCode, operand, state);
            break;
        case 6:
            processBDV(opCode, operand, state);
            break;
        case 7:
            processCDV(opCode, operand, state);
            break;
        }
    }

    void part1()
    {
        State state = _orig;
        while (state._pc < state._instructions.size())
        {
            glm::ivec2 instruction = state._instructions[state._pc];
            processInstruction(instruction.x, (int64_t)instruction.y, state);
        }

        printf("Register A: %lld\n", state._regA);
        printf("Register B: %lld\n", state._regB);
        printf("Register C: %lld\n", state._regC);
        printf("\n");
        for (auto p : state._output)
        {
            printf("%d,", p);
        }
        printf("\n");
    }

    void emulatePart2Program(int64_t a, vector<int> &output)
    {
        output.clear();
        while (a > 0)
        {
            int64_t b = 0;
            int64_t c = 0;
            b = a % 8;
            b = b ^ 1;
            c = (int64_t)(a / (int64_t)pow(2, b));
            a = (int64_t)a / 8;
            b = b ^ 4;
            b = b ^ c;
            b %= 8;
            output.push_back((int)b);
        }
    }

    // Cheated.
    // https://pastebin.com/FmhQE1iH
    // https://github.com/efox4335/advent_of_code/blob/main/advent_of_code_2024/day_17_part_2_emu.c
    void part2()
    {
        vector<int> output;
        output.reserve(16);

        vector<int64_t> stack;
        stack.push_back(0);

        int64_t bestA = std::numeric_limits<int64_t>::max();
        while (!stack.empty())
        {
            int64_t currentA = stack.back();
            stack.pop_back();

            emulatePart2Program(currentA, output);
            bool isMatch = _orig.isSameProgram(output);
            if (isMatch)
            {
                bestA = std::min(currentA, bestA);
            }

            // Examining the Program instructions we find that in each
            // iteration the value of A gets divided by 3 (A >> 3) and that
            // only the last 3 bits of A are used to produce the next output
            // value (i.e. register B). Knowing this fact means that we only
            // need to check in the range <prefix>...000 to <prefix>...111
            // and try to find a sequence which matches our program.
            // Note that there could be multiple values between 0 and 7 which
            // could produce the same program sequence. We need to check each
            // of them as some may produce larger/smaller A's in the end.
            // so we add them to the stack for further processing.
            for (int i = 0; i <= 7; i++)
            {
                int64_t candidateA = (currentA << 3) + i;
                emulatePart2Program(candidateA, output);

                if (output.size() > _orig._program.size())
                {
                    // The size of A will produce a program larger
                    // then the original program. This is our stop condition
                    // for the search.
                    break;
                }

                if (_orig.isSameProgramPrefix(output))
                {
                    stack.push_back(candidateA);
                }
            }
        }

        emulatePart2Program(bestA, output);
        bool isMatch = _orig.isSameProgram(output);

        printf("Starting A = %lld\n", bestA);
        printf("IsMatch = %d\n", isMatch);
        Utils::PrintIntVector(output);
    }

    void Run()
    {
        bool readTest = false;

        ReadInput(readTest);
        // part1(); // real(7,1,2,3,2,6,7,2,5)
        part2(); // real(202356708354602)
    }
};