#pragma once

#include "utils.hpp"

class Day9
{
public:
    const int EMPTY_NUM = -1;
    vector<int> _memory;

    Day9() {}
    virtual ~Day9() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/day9/test.txt"
                               : "res/day9/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            const int EMPTY_STATE = 0;
            const int MEMORY_STATE = 1;
            int state = MEMORY_STATE;

            int id = 0;
            for (int i = 0; i < line.size(); i++)
            {
                int digit = line[i] - '0';
                if (state == MEMORY_STATE)
                {
                    for (int j = 0; j < digit; j++)
                    {
                        _memory.push_back(id);
                    }
                    id += 1;
                    state = EMPTY_STATE;
                }
                else
                {
                    for (int j = 0; j < digit; j++)
                    {
                        _memory.push_back(EMPTY_NUM);
                    }
                    state = MEMORY_STATE;
                }
            }
        }

        Utils::PrintVector(_memory);

        file.close();
    }

    unsigned long long find_checksum()
    {
        unsigned long long checksum = 0;
        for (int i = 0; i < (int)_memory.size(); i++)
        {
            if (_memory[i] == EMPTY_NUM)
            {
                continue;
            }
            checksum += (unsigned long long)_memory[i] * i;
        }
        return checksum;
    }

    int find_next_empty(int index)
    {
        for (int i = index; i < (int)_memory.size(); i++)
        {
            if (_memory[i] == EMPTY_NUM)
            {
                index = i;
                break;
            };
        }
        return index;
    }

    void part1()
    {
        int last_space = find_next_empty(0);
        for (int i = (int)_memory.size() - 1; i >= 0; i--)
        {
            if (last_space > i)
            {
                break;
            }
            if (_memory[i] != EMPTY_NUM)
            {
                _memory[last_space] = _memory[i];
                _memory[i] = EMPTY_NUM;
                last_space = find_next_empty(last_space);
            }
        }
        Utils::PrintVector(_memory);

        unsigned long long checksum = find_checksum();
        printf("Checksum = %llu\n", checksum);
    }

    struct Block
    {
        int start;
        int end;
        int id;
        int size;

        Block(int start, int id, int size)
        {
            this->start = start;
            this->end = start + size - 1;
            this->id = id;
            this->size = size;
        }

        void Print()
        {
            printf("%d %d %d %d\n", start, end, id, size);
        }
    };

    Block get_block(int begin, int id, int size_condition = -1)
    {
        for (int i = begin; i < (int)_memory.size(); i++)
        {
            if (_memory[i] == id)
            {
                int start = i;
                int end = i;
                while (end < (int)_memory.size() && _memory[end] == id)
                {
                    end += 1;
                }

                Block candidate = Block(start, id, end - start);
                if (size_condition > 0)
                {
                    if (candidate.size >= size_condition)
                    {
                        return candidate;
                    }
                    else
                    {
                        i = end - 1;
                    }
                }
                else
                {
                    return candidate;
                }
            }
        }

        return {0, 0, 0};
    }

    void part2()
    {
        for (int i = (int)_memory.size() - 1; i >= 0; i--)
        {
            int id = _memory[i];
            if (id == EMPTY_NUM)
            {
                continue;
            }

            // Find the size of this segment;
            int start = i;
            int end = i;
            while (start >= 0 && _memory[start] == id)
            {
                start -= 1;
            }
            start += 1;
            Block current(start, id, end - start + 1);

            // Find an empty block with size
            Block empty = get_block(0, EMPTY_NUM, current.size);
            if (empty.size == 0)
            {
                i = current.start;
                continue;
            }
            if (empty.start > current.start)
            {
                i = current.start;
                continue;
            }

            // copy the contents into the empty block
            // clear the contents of the current block
            int last_space = empty.start;
            for (int j = current.start; j <= current.end; j++)
            {
                _memory[last_space++] = id;
                _memory[j] = EMPTY_NUM;
            }
        }
        // Utils::PrintVector(_memory);

        unsigned long long checksum = find_checksum();
        printf("Checksum = %llu\n", checksum);
    }

    void
    Run()
    {

        bool readTest = false;
        ReadInput(readTest);
        // part1(); // test(1928), real(6366665108136)
        part2(); // test(2858), real(6398065450842)
    }
};