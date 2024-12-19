#pragma once

#include "utils.hpp"

class Day14
{
public:
    struct Robot
    {
        glm::ivec2 _pos;
        glm::ivec2 _velocity;
    };

    vector<Robot> _robots;
    int _width;
    int _height;

    Day14() {}
    virtual ~Day14() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day14/test.txt"
                               : "res/Day14/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);

            std::regex particleRegex(R"(p=(\d+),(\d+) v=(-?\d+),(-?\d+))");
            std::smatch match;
            if (std::regex_search(line, match, particleRegex))
            {
                int x = std::stoi(match[1]);
                int y = std::stoi(match[2]);
                int vx = std::stoi(match[3]);
                int vy = std::stoi(match[4]);

                Robot robot;
                robot._pos = glm::ivec2(x, y);
                robot._velocity = glm::ivec2(vx, vy);
                _robots.push_back(robot);
            }
        }

        if (readTest)
        {
            _width = 11;
            _height = 7;
        }
        else
        {
            _width = 101;
            _height = 103;
        }

        std::cout << "Robots: " << std::endl;
        for (const auto &r : _robots)
        {
            std::cout << r._pos.x << ", " << r._pos.y << ", " << r._velocity.x << ", " << r._velocity.y << std::endl;
        }
        printf("width = %d, height = %d\n", _width, _height);

        file.close();
    }
    void part1()
    {

        for (int second = 0; second < 100; second++)
        {
            for (auto &robot : _robots)
            {
                robot._pos += robot._velocity;
                if (robot._pos.x < 0)
                {
                    robot._pos.x = _width - std::abs(robot._pos.x);
                }
                else if (robot._pos.x >= _width)
                {
                    robot._pos.x = robot._pos.x - _width;
                }
                if (robot._pos.y < 0)
                {
                    robot._pos.y = _height - std::abs(robot._pos.y);
                }
                else if (robot._pos.y >= _height)
                {
                    robot._pos.y = robot._pos.y - _height;
                }
            }
        }

        int mid_x = _width / 2;
        int mid_y = _height / 2;
        int tl = 0;
        int tr = 0;
        int bl = 0;
        int br = 0;
        for (const auto &r : _robots)
        {
            // tl
            if (r._pos.x < mid_x && r._pos.y < mid_y)
            {
                tl += 1;
            }
            else if (r._pos.x > mid_x && r._pos.y < mid_y)
            {
                tr += 1;
            }
            else if (r._pos.x < mid_x && r._pos.y > mid_y)
            {
                bl += 1;
            }
            else if (r._pos.x > mid_x && r._pos.y > mid_y)
            {
                br += 1;
            }
        }
        int safety = tl * tr * bl * br;
        printf("Safety = %d\n", safety);
    }

    int get_safety()
    {
        int mid_x = _width / 2;
        int mid_y = _height / 2;
        int tl = 0;
        int tr = 0;
        int bl = 0;
        int br = 0;
        for (const auto &r : _robots)
        {
            // tl
            if (r._pos.x < mid_x && r._pos.y < mid_y)
            {
                tl += 1;
            }
            else if (r._pos.x > mid_x && r._pos.y < mid_y)
            {
                tr += 1;
            }
            else if (r._pos.x < mid_x && r._pos.y > mid_y)
            {
                bl += 1;
            }
            else if (r._pos.x > mid_x && r._pos.y > mid_y)
            {
                br += 1;
            }
        }
        int safety = tl * tr * bl * br;
        return safety;
    }

    bool is_tree()
    {

        static vector<glm::ivec2> dirs = {
            glm::ivec2(1, 0),  // right
            glm::ivec2(0, 1),  // down
            glm::ivec2(-1, 0), // left
            glm::ivec2(0, -1), // up
            glm::ivec2(1, 1),  // right down
            glm::ivec2(-1, 1), // left down
            glm::ivec2(1, -1), // right up
            glm::ivec2(-1, -1) // left up
        };
        std::vector<std::vector<int>> grid(_height, std::vector<int>(_width, 0));
        for (const auto &r : _robots)
        {
            grid[r._pos.y][r._pos.x] += 1;
        }

        for (const auto &r : _robots)
        {
            // check quadrant of nine neighbors
            int count = 0;
            for (const auto &dir : dirs)
            {
                glm::ivec2 next = r._pos + dir;
                if (Utils::InRange(next, grid) && grid[next.y][next.x] > 0)
                {
                    count += 1;
                }
            }
            if (count == 8) {
                return true;
            }
        }

        for (const auto &r : _robots)
        {
            // check quadrent of nine neighbors
            int count = 0;
            for (int i = 0; i < 10; i++)
            {
                glm::ivec2 next = r._pos + glm::ivec2(i, 0);
                if (Utils::InRange(next, grid) && grid[next.y][next.x] > 0)
                {
                    count += 1;
                }
            }
            if (count > 10) {
                return true;
            }
        }

        return false;
    }

    void print_tree(int second)
    {
        // static int candidate = 0;
        std::vector<std::vector<int>> grid(_height, std::vector<int>(_width, 0));
        for (const auto &r : _robots)
        {
            grid[r._pos.y][r._pos.x] += 1;
        }
        std::ofstream out(fmt::format("out-{}.txt", second));
        for (int y = 0; y < _height; y++)
        {
            for (int x = 0; x < _width; x++)
            {
                if (grid[y][x] > 0)
                {
                    out << grid[y][x];
                }
                else
                {
                    out << " ";
                }
            }
            out << std::endl;
        }
    }

    void part2()
    {
        unsigned long long safety = 0;
        unsigned long long averageSafety = 220696655;
        for (int second = 0; second <= 10403; second++)
        {
            for (auto &robot : _robots)
            {
                robot._pos += robot._velocity;
                if (robot._pos.x < 0)
                {
                    robot._pos.x = _width - std::abs(robot._pos.x);
                }
                else if (robot._pos.x >= _width)
                {
                    robot._pos.x = robot._pos.x - _width;
                }
                if (robot._pos.y < 0)
                {
                    robot._pos.y = _height - std::abs(robot._pos.y);
                }
                else if (robot._pos.y >= _height)
                {
                    robot._pos.y = robot._pos.y - _height;
                }
            }

            if (is_tree())
            {
                print_tree(second);
            }

            // int newSafety = get_safety();
            // safety += newSafety;
            // if (newSafety < averageSafety / 2)
            // {
            //     printf("newSafety = %d\n", newSafety);
            //     print_tree(second);
            // }

            // 7370
        }
        uint64_t average_safety = safety / 10403;
        printf("Average safety = %ld\n", average_safety);
    }

    void
    Run()
    {
        bool readTest = true;
        ReadInput(readTest);
        part1(); //real(225552000)
        // part2(); // real(7371)
    }
};