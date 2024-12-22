#pragma once

#include "utils.hpp"


class Day10
{
public:
    Utils::GridInt _grid;
    int _num_rows; 
    int _num_cols;
    std::vector<glm::ivec2> _starts;
    std::vector<glm::ivec2> _ends;

    Day10() {}
    virtual ~Day10() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day10/test.txt"
                               : "res/Day10/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        int rows =0;
        while (std::getline(file, line))
        {
            Utils::trim(line);

            vector<int> row;
            for(int i = 0; i < (int) line.length(); i++) {
                if (line[i] == '.') {
                    row.push_back(-1);
                    continue;
                }
                int num = line[i] - '0';
                if (num == 0) {
                    _starts.push_back(glm::ivec2(i, rows));
                } else if (num == 9) {
                    _ends.push_back(glm::ivec2(i, rows));
                }
                row.push_back(num);
            }
            _grid.push_back(row);
            rows += 1;
        }
        _num_rows = rows;
        _num_cols = (int) _grid[0].size();

        Utils::PrintIntVector2D(_grid);
        Utils::PrintVector(_starts);
        Utils::PrintVector(_ends);
        printf("num_rows = %d, num_cols = %d\n", _num_rows, _num_cols);

        file.close();
    }

    int traverse(glm::ivec2 start) {
        static std::vector<glm::ivec2> dirs = {
            glm::ivec2(1, 0), // right
            glm::ivec2(-1, 0), // left
            glm::ivec2(0, 1), // down
            glm::ivec2(0, -1), // up
        };

        
        int count = 0;
        vector<glm::ivec2> stack;
        stack.push_back(start);

        Utils::SetVec2 visited;
        while (stack.size() > 0) {
            glm::ivec2 current = stack.back();
            stack.pop_back();

            int current_elevation = _grid[current.y][current.x];
            if (current_elevation == 9) {
                if (visited.find(current) == visited.end()) {
                    visited.insert(current);
                    count += 1;
                }
                continue;
            }

            for (int i = 0; i < (int) dirs.size(); i++) {
                glm::ivec2 dir = dirs[i];
                glm::ivec2 next = current + dir;
                if (Utils::InRange(next, _num_cols, _num_rows)) {
                    int next_elevation = _grid[next.y][next.x];
                    if (next_elevation - current_elevation == 1) {
                        stack.push_back(next);
                    }
                }
            }
        }

        return count;
    }

    void part1()
    {
        int score = 0;
        for(int i = 0; i < (int) _starts.size(); i++) {
            glm::ivec2 start = _starts[i];
            int count = traverse(start);
            score += count;
        }
        printf("Score: %d\n", score);
    }

    int traverse2(glm::ivec2 start) {
        static std::vector<glm::ivec2> dirs = {
            glm::ivec2(1, 0), // right
            glm::ivec2(-1, 0), // left
            glm::ivec2(0, 1), // down
            glm::ivec2(0, -1), // up
        };

        
        int count = 0;
        vector<glm::ivec2> stack;
        stack.push_back(start);

        vector<glm::ivec2> visited;
        while (stack.size() > 0) {
            glm::ivec2 current = stack.back();
            stack.pop_back();

            int current_elevation = _grid[current.y][current.x];
            if (current_elevation == 9) {
                count += 1;
                continue;
            }

            for (int i = 0; i < (int) dirs.size(); i++) {
                glm::ivec2 dir = dirs[i];
                glm::ivec2 next = current + dir;
                if (Utils::InRange(next, _num_cols, _num_rows)) {
                    int next_elevation = _grid[next.y][next.x];
                    if (next_elevation - current_elevation == 1) {
                        stack.push_back(next);
                    }
                }
            }
        }

        return count;
    }

    void part2()
    {
        int score = 0;
        for(int i = 0; i < (int) _starts.size(); i++) {
            glm::ivec2 start = _starts[i];
            int count = traverse2(start);
            score += count;
        }
        printf("Score: %d\n", score);
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        part1();  // test(36), real(468)
        // part2(); // test(81), real(966)
    }
};