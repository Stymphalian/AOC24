#pragma once

#include "utils.hpp"

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
                if (found_cycle != nullptr)
                {
                    *found_cycle = true;
                }
                break;
            }
            path.push_back(Path(pos, dir));
            visited.insert(Path(pos, dir));
            pos += dir_vec;
        }
    }

    bool has_cycle(Path start)
    {
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
            if (has_cycle(Path(_start, _start_dir)))
            {
                printf("Cycle at %s\n", glm::to_string(pos).c_str());
                cycles.insert(Path(pos, DIR::UP));
            }
            _grid[pos.y][pos.x] = 0;

            pos += dir_vec;
        }

        printf("Cycle Count %d\n", (int)cycles.size());
    }

    void part2_2()
    {
        int cycle_count = 0;
        std::vector<Path> normal_path;
        get_path(Path(_start, _start_dir), normal_path, nullptr);

        set<Path> visited;
        for (Path &p : normal_path)
        {

            _grid[p.pos.y][p.pos.x] = 1;

            int current_row = _start.y;
            int current_col = _start.x;
            DIR current_dir = DIR::UP;
            visited.clear();

            while (true)
            {
                glm::ivec2 step = get_dir_vec(current_dir);
                const DIR offset = current_dir;
                const int new_row = current_row + step.y;
                const int new_col = current_col + step.x;
                if (!Utils::InRange(new_col, new_row, _num_cols, _num_rows))
                {
                    break;
                }
                visited.insert(Path({current_col, current_row}, current_dir));
                if (_grid[new_row][new_col] == 1)
                {
                    current_dir = DIR((current_dir + 1) % 4);
                    visited.insert(Path({current_col, current_row}, current_dir));
                    continue;
                }

                Path new_path = Path({new_col, new_row}, offset);
                if (visited.find(new_path) != visited.end())
                {
                    cycle_count += 1;
                    break;
                }
                current_row = new_row;
                current_col = new_col;
            }

            _grid[p.pos.y][p.pos.x] = 0;
        }

        printf("Cycle Count: %d\n", cycle_count);
    }

    // https://topaz.github.io/paste/#XQAAAQDQBwAAAAAAAAA4GEiZzRd1JAgz+whYRQxSFI7XvmlfhtGDinguAj8sFyd/fs4fcP3B6Imi9C/+8eB6lqGjnGJl9G0yIGbnMG8Z8gcQ/dxngqIIYy9KQvtaR2fxd80pndeBlGamWtmvmGD7fAvzWtFTCIw3fvkYEifpA7Oun9M0k7+mny+lS5qdZ7A+lNXQpdWK568461Iqt2/gbNiKMaY7s15Nj1831VI0zvDm1yyGdv6Sxazs2o5KMPXnDLwrqfYIQ3llYQbzWEXaZ8qfP5QHVS2yH8SCK1BlDbLITENDVa2jek7fSIuBwD7VXPV9HJDNpvmaJCgKVsxxn5X+N6ZwXLAK12vqxPQOZni9Sshwl3oyb9TowfbQUE/npWrk6gVzvMfWI2boJwDPu2GobwvZis/3gJckEdH1E+PpTN5NPi1WIg9pwKiFGtiRW1Gw2v82WF7wN+gTzhBXHueAKBTQvCmmr9bIC2BW+ricYDUL4u1OmJMrTEHUqSaHWsKjgwQAjCo/Qg4Qpl00GrP3Mghh/k04fwVBEOXcVBfHVzaAKrQkls24KpxhRTNuSNvEP4/zkcHSaqFZA72K7eSnqvhVgffiZ1StiIaiqmB5NKCnAI0SSqMvD+bG9Jy1rHe6f7IOybCGUn32Lp0s2jULwhZaZvaOSrNVVQJfdxGvi/aP+LKTlc6UCcYfH25V8dwdMxaaHKiwag/s0lMvDiMTrV/aaYANuqKQAHRxIZ+rLUZ+5ZdiTe+1Jvqxd3j5Ovuk4bD9ThDR6cnsTZCXZLZaGRJvyM9EA+bEQBpo1XSnafRO8qsJU7kALRiGmpJdjc7/0Rey7jmG1jMEd5YgybnDOliuZIcp2DsnuqovHrW6EUnKeonvP9TqrFKeDpKNfvmtoQqvZ7YZiWqcYNrWYiY99iH+D1kEH9OrK0Ogo+SJ4NLC02Mq3MJV8p9Mlcv3MTIDpZnVkPJCDp7zFga6hW/Yj/9MPkoYi9GGhpF1qq/W9VoaJw5bxIQKVTvsUQnGi8f0HcCEcdPKHx8z2NQNK6Perr6//9kDlKWJ+ZDJg3VIqPB7nrFtyLgWKNfRDzlqWicI7bRRSocy/xu+igA=
    // const int UP = 2;
    // const int RIGHT = 3;
    // const int DOWN = 4;
    // const int LEFT = 5;
    // const int OUT_OF_BOUNDS = 6;
    // const int NEW_CELL = 7;
    // const int VISITED = 8;
    // const int CYCLE = 9;
    // const int BLOCKED = 10;

    // int make_step(vector<vector<int>> &grid, glm::ivec2 &pos, int col, int row, int dir)
    // {

    //     if (!Utils::InRange(col, row, _num_cols, _num_rows))
    //     {
    //         return OUT_OF_BOUNDS;
    //     }
    //     int val = grid[row][col];
    //     if (val == 1)
    //     {
    //         return BLOCKED;
    //     }

    //     pos = glm::ivec2(col, row);
    //     if (val == 0)
    //     {
    //         grid[row][col] = dir;
    //         return NEW_CELL;
    //     }
    //     else if (val == dir)
    //     {
    //         return CYCLE;
    //     }
    //     return VISITED;
    // }

    // void part2()
    // {
    //     _grid[_start.y][_start.x] = UP;

    //     int cycle_count = 0;
    //     for (int row = 0; row < _num_rows; row++)
    //     {
    //         for (int col = 0; col < _num_cols; col++)
    //         {
    //             if (_grid[row][col] == 1)
    //             {
    //                 continue;
    //             }

    //             std::vector<vector<int>> grid = _grid;
    //             grid[row][col] = 1;
    //             int dir = UP;
    //             glm::ivec2 current = _start;

    //             while (true)
    //             {
    //                 int step;
    //                 if (dir == UP)
    //                 {
    //                     step = make_step(grid, current, current.x, current.y - 1, UP);
    //                     if (step == BLOCKED)
    //                     {
    //                         dir = RIGHT;
    //                     }
    //                 }
    //                 else if (dir == RIGHT)
    //                 {
    //                     step = make_step(grid, current, current.x + 1, current.y, RIGHT);
    //                     if (step == BLOCKED)
    //                     {
    //                         dir = DOWN;
    //                     }
    //                 }
    //                 else if (dir == DOWN)
    //                 {
    //                     step = make_step(grid, current, current.x, current.y + 1, DOWN);
    //                     if (step == BLOCKED)
    //                     {
    //                         dir = LEFT;
    //                     }
    //                 }
    //                 else if (dir == LEFT)
    //                 {
    //                     step = make_step(grid, current, current.x - 1, current.y, LEFT);
    //                     if (step == BLOCKED)
    //                     {
    //                         dir = UP;
    //                     }
    //                 }

    //                 if (step == OUT_OF_BOUNDS)
    //                 {
    //                     break;
    //                 }
    //                 if (step == CYCLE)
    //                 {
    //                     cycle_count++;
    //                     break;
    //                 }
    //             }
    //         }
    //     }

    //     printf("Cycle Count %d\n", cycle_count);
    // }

    // https://topaz.github.io/paste/#XQAAAQByEQAAAAAAAAARmknGRw8TogB3OxjFr5+Tb3pjORDCkxUtRlsQU8+XHY/G9l3iG66wKTltIn4cTAzB/k1EvSvaJXPw3dMAbW6y/GPrPYznEEk9n5JWgpyXwRz2NZup4xsmy6EL+qfaDK0dSanlCRHzntfi6X646kFatPhGCD0Aod93rwvDFy7+InEdpWmMQwbQrwrv+a2WxXLyfFUT9rP6hxK681Cd4yXRUe0KoyDj39nMws6VmUJMyUHnV7srwN+uH+CYotRlGo+s67VMPYm3nc/AEXIPAjVV4YYPHY4axBdKobsEDsA6EkrGeImghq8GWycP7TKtYR3uVU8c4S/t/M3/upi/wBRAeOiCXZNOCedIKJ9/T7pf4ZzzKP4+3OXbMTxSM511DkXKIZClGiZYW8sIwPyP2oAHnWvlfiwADcEbocrje1UgbxO6otjlOudrlBV1h3Wwidhvq9fCVdgjI85jYLQ83NvSLA41DX6gYnfBsgCldWnzmUaB7kkQ2w4TcTNtSPTPM3nBcFM/vXpfgwTAXSRxHwzPZx8KvsUfl2c8R9GChGZI2ymceHdcgKmdc7GRnDq/dbSG9Boe47sM58Th5skgb5vStdAh/1uzG23uzQKIWQ/+7NXjogQTU3kALmAIXAlYf0M7pm+bzRhJs0H7r2XKC5ypj0zFc1YvUervPI5OUHFJntEUuqdiEWOCfdI+g/Ik453fX+Eb0X+Amub12BRC8i3WGfQORcIy2dmfUQ9hJidb0QqT77xOXgkkaE0yBBbPGVqQreg9SgCon3b54k5o1YUCDgJaw7ucZwd5Vz9R96cfVQSxw1uWz5LhmwXGzWpfZFjV5LJqRkwR+V+7H295OpndmxQw9YoXAOiKrXWnZk3CwEW1AJ7e00KbPNJTiutCJbQ3zjnufv6giODZUFRw/O0DTkNd5dDEJr+XVXNknYxewUVPHnD408dXJY/AT+lrpqylV56jJ/P6PQV4h6XxM1gHnwnS2DfajLjipFuD1rQ3j58tLf97AowWrxpUdqgQcPhHXBLthpfCGe21kDMrXa2I9okhAijt6wbS8uJXD6OcFN5fPuQTh2zLRTfs6sHdFwVielH+H60bFpuhr1wjSyaSk60wFPXHDCtklveQ9Zs09091SpPlT61oXTeynbo8r7C1cuyQQlZn6YJ1N72HSD3GnvKwrOaSWk8hbgIJCEWrf7IHvKPuSUZcyIHOx22GcnqrN48xdC1nZ/RRewbkIUkqGVBFzLoL9cqvAISi7HWlJ/so2JMU1HXzcXy51IGjBLnTYl6TJzvUC8RM9Qz3vANLxrlFDv6gWjkMW1G2DO3vcjAiJELp61JaR7JUSHUPhpJSzofoatR9Vmrm0eTjWvBDeqPRKURtMxBfaYWELX3tIsA4Han26JKb5Y2k/Wi5Mv7EDtr7d59X/15G+UhoO3Qos/dT7Ytx5mTTR+2c+6lhN8dQ12ExAvprrHNP7udrUqz11ndtD8UEF4bKWtGv0C4DIlK+SFLw36G7tA5rEGFgzqFn4c/EphTIyEM5xEkxsOdcHCUnwZZWn9ldkWjzyocOrkddSAnjsOk3rzkycReYOq0npF+x7pPiA6M7DWpIAJ1nIMgS1v5OXrGW95kXR+BOjFJOzDJktVZcXtrr3hEAIDfKij7CQVUynoiUnPCAs/+ualsO
    void
    Run(bool readTestData = true)
    {
        ReadInput(false);
        // part1(); // 4939, test(41)
        part2(); // 1434, test (6)
    }
};