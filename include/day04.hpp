#pragma once

#include "utils.hpp"

class Day4
{
public:
    std::vector<std::string> _grid;
    int _num_rows = 0;
    int _num_cols;

    Day4() {}
    virtual ~Day4() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/day4/test.txt"
                               : "res/day4/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            _grid.push_back(Utils::trim_copy(line));
        }

        _num_rows = (int)_grid.size();
        _num_cols = (int)_grid[0].length();

        file.close();
    }

    int count_xmas(int row, int col)
    {
        // left
        int count = 0;
        if (col - 3 >= 0 &&
            _grid[row][col] == 'X' &&
            _grid[row][col - 1] == 'M' &&
            _grid[row][col - 2] == 'A' &&
            _grid[row][col - 3] == 'S')
        {
            count += 1;
        }

        // right
        if (col + 3 < _num_cols &&
            _grid[row][col] == 'X' &&
            _grid[row][col + 1] == 'M' &&
            _grid[row][col + 2] == 'A' &&
            _grid[row][col + 3] == 'S')
        {
            count += 1;
        }

        // up
        if (row - 3 >= 0 &&
            _grid[row][col] == 'X' &&
            _grid[row - 1][col] == 'M' &&
            _grid[row - 2][col] == 'A' &&
            _grid[row - 3][col] == 'S')
        {
            count += 1;
        }

        // down
        if (row + 3 < _num_rows &&
            _grid[row][col] == 'X' &&
            _grid[row + 1][col] == 'M' &&
            _grid[row + 2][col] == 'A' &&
            _grid[row + 3][col] == 'S')
        {
            count += 1;
        }

        // diagonal tr
        if (row - 3 >= 0 &&
            col + 3 < _num_cols &&
            _grid[row][col] == 'X' &&
            _grid[row - 1][col + 1] == 'M' &&
            _grid[row - 2][col + 2] == 'A' &&
            _grid[row - 3][col + 3] == 'S')
        {
            count += 1;
        }

        // diagonal br
        if (row + 3 < _num_rows &&
            col + 3 < _num_cols &&
            _grid[row][col] == 'X' &&
            _grid[row + 1][col + 1] == 'M' &&
            _grid[row + 2][col + 2] == 'A' &&
            _grid[row + 3][col + 3] == 'S')
        {
            count += 1;
        }

        // diagonal tl
        if (row - 3 >= 0 &&
            col - 3 >= 0 &&
            _grid[row][col] == 'X' &&
            _grid[row - 1][col - 1] == 'M' &&
            _grid[row - 2][col - 2] == 'A' &&
            _grid[row - 3][col - 3] == 'S')
        {
            count += 1;
        }

        // diagonal bl
        if (row + 3 < _num_rows &&
            col - 3 >= 0 &&
            _grid[row][col] == 'X' &&
            _grid[row + 1][col - 1] == 'M' &&
            _grid[row + 2][col - 2] == 'A' &&
            _grid[row + 3][col - 3] == 'S')
        {
            count += 1;
        }

        return count;
    }

    void part1()
    {
        int xmas_count = 0;
        for (int row = 0; row < _num_rows; row++)
        {
            for (int col = 0; col < _num_cols; col++)
            {
                char c = _grid[row][col];
                if (c == 'X')
                {
                    xmas_count += count_xmas(row, col);
                }
            }
        }

        printf("XMAS Count: %d\n", xmas_count);
    }

    int count_x_mas(int row, int col)
    {
        int count = 0;

        if (row - 1 >= 0 && row + 1 < _num_rows &&
            col - 1 >= 0 && col + 1 < _num_cols)
        {
            char tl = _grid[row - 1][col - 1];
            char tr = _grid[row - 1][col + 1];
            char bl = _grid[row + 1][col - 1];
            char br = _grid[row + 1][col + 1];            
            bool flag1 = (tl == 'M' && br == 'S') || (tl == 'S' && br == 'M');
            bool flag2 = (tr == 'M' && bl == 'S') || (tr == 'S' && bl == 'M');
            count += (flag1 && flag2) ? 1 : 0;
        }
        return count;
    }

    void part2()
    {
        int xmas_count = 0;
        for (int row = 0; row < _num_rows; row++)
        {
            for (int col = 0; col < _num_cols; col++)
            {
                char c = _grid[row][col];
                if (c == 'A')
                {
                    xmas_count += count_x_mas(row, col);
                }
            }
        }
        printf("XMAS Count: %d\n", xmas_count);
    }

    void
    Run(bool readTestData = false)
    {
        ReadInput(readTestData);
        // part1(); // 2406
        part2(); // 1807
    }
};