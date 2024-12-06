#pragma once

#include "utils.hpp"
#include <tuple>

class Day6
{
public:
    enum DIR
    {
        UP = 0,
        RIGHT = 1,
        DOWN = 2,
        LEFT = 3
    };

    vector<vector<int>> _grid;
    glm::ivec2 _start;
    DIR _start_dir = DIR::UP;
    int _num_rows = -1;
    int _num_cols = -1;

    Day6() {}
    virtual ~Day6() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/day6/test.txt"
                               : "res/day6/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);

            vector<int> row;
            row.reserve(line.length());
            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] == '#')
                {
                    row.push_back(1);
                }
                else
                {
                    row.push_back(0);
                }
                if (line[i] == '^')
                {
                    _start = {i, (int)_grid.size()};
                }
            }
            _grid.push_back(row);
        }

        _num_cols = (int)_grid[0].size();
        _num_rows = (int)_grid.size();
        // printf("start = (%d, %d)\n", _start.x, _start.y);
        // printf("num_rows = %d, num_cols = %d\n", _num_rows, _num_cols);
        // Utils::PrintIntVector2D(_grid);

        file.close();
    }

    glm::ivec2 get_dir_vec(DIR dir)
    {
        glm::ivec2 dir_vec;
        switch (dir)
        {
        case DIR::UP:
            dir_vec = {0, -1};
            break;
        case DIR::RIGHT:
            dir_vec = {1, 0};
            break;
        case DIR::DOWN:
            dir_vec = {0, 1};
            break;
        case DIR::LEFT:
            dir_vec = {-1, 0};
            break;
        }
        return dir_vec;
    }

    struct Path
    {
        glm::ivec2 pos;
        DIR dir;

        Path();
        Path(glm::ivec2 current, DIR current_dir) : pos(current), dir(current_dir) {}

        bool operator==(const Path &other) const
        {
            return pos == other.pos && dir == other.dir;
        }

        friend bool operator<(const Path &lhs, const Path &rhs) noexcept
        {
            return std::tie(lhs.pos.x, lhs.pos.y, lhs.dir) < std::tie(rhs.pos.x, rhs.pos.y, rhs.dir);
        }

        glm::ivec2 dirVec()
        {
            glm::ivec2 dir_vec;
            switch (dir)
            {
            case DIR::UP:
                dir_vec = {0, -1};
                break;
            case DIR::RIGHT:
                dir_vec = {1, 0};
                break;
            case DIR::DOWN:
                dir_vec = {0, 1};
                break;
            case DIR::LEFT:
                dir_vec = {-1, 0};
                break;
            }
            return dir_vec;
        }

        DIR prevDir()
        {
            return DIR((dir - 1) % 4);
        }
    };

    // direction = 0 up, 1 right, 2 down, 3 left
    Path walk(Path currentPath, int *steps, bool *finished)
    {
        glm::ivec2 current = currentPath.pos;
        DIR dir = currentPath.dir;
        glm::ivec2 dir_vec = get_dir_vec(dir);
        while (current.x >= 0 && current.x < _num_cols && current.y >= 0 && current.y < _num_rows)
        {
            if (_grid[current.y][current.x] == 1)
            {
                return Path(current - dir_vec, DIR((dir + 1) % 4));
            }
            else if (_grid[current.y][current.x] != 2)
            {
                *steps += 1;
                _grid[current.y][current.x] = 2;
            }

            current += dir_vec;
        }

        *finished = true;
        return Path(current, dir);
    }

    void part1()
    {
        glm::ivec2 current = _start;
        DIR current_dir = _start_dir;
        int steps = 0;
        bool finished = false;

        while (!finished)
        {
            auto result = walk(Path(current, current_dir), &steps, &finished);
            current = result.pos;
            current_dir = result.dir;
        }
        printf("current %s, dir %d, steps %d\n", glm::to_string(current).c_str(), current_dir, steps);
    }

    void get_path(Path start, vector<Path> &path, bool *found_cycle)
    {
        glm::ivec2 pos = start.pos;
        DIR dir = start.dir;
        glm::ivec2 dir_vec = get_dir_vec(dir);

        set<Path> visited;
        while (Utils::InRange(pos, _num_cols, _num_rows))
        {
            if (_grid[pos.y][pos.x] == 1)
            {
                pos -= dir_vec;
                dir = DIR((dir + 1) % 4);
                dir_vec = get_dir_vec(dir);
                continue;
            }
            if (visited.find(Path(pos, dir)) != visited.end())
            {
                *found_cycle = true;
                break;
            }
            path.push_back(Path(pos, dir));
            visited.insert(Path(pos, dir));
            pos += dir_vec;
        }
    }

    bool has_cycle(Path start) {
        static vector<Path> path;
        path.clear();
        bool found_cycle = false;
        get_path(start, path, &found_cycle);
        return found_cycle;
    }

    void part2()
    {

        int cycle_count = 0;
        glm::ivec2 pos = _start;
        DIR dir = _start_dir;
        glm::ivec2 dir_vec = get_dir_vec(dir);

        set<Path> cycles;
        while (Utils::InRange(pos, _num_cols, _num_rows))
        {
            if (_grid[pos.y][pos.x] == 1)
            {
                pos -= dir_vec;
                dir = DIR((dir + 1) % 4);
                dir_vec = get_dir_vec(dir);
                continue;
            }

            _grid[pos.y][pos.x] = 1;
            if (has_cycle(Path(_start, _start_dir))) {
                printf("Cycle at %s\n", glm::to_string(pos).c_str());
                cycles.insert(Path(pos, DIR::UP));
                // cycle_count += 1;
            }
            _grid[pos.y][pos.x] = 0;

            pos += dir_vec;
        }

        printf("Cycle Count %d\n", (int)cycles.size());
    }

    void
    Run(bool readTestData = true)
    {
        ReadInput(false);
        // part1(); // 4939, test(41)
        part2(); // 1434, test (6)
    }
};