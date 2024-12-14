#pragma once

#include "utils.hpp"

class Day8
{
public:
    std::unordered_map<char, std::vector<glm::ivec2>> _stations;
    int _num_rows;
    int _num_cols;

    Day8() {}
    virtual ~Day8() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/day8/test.txt"
                               : "res/day8/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        int row = 0;
        while (std::getline(file, line))
        {
            Utils::trim(line);

            for (int col = 0; col < line.size(); col++)
            {
                if (line[col] == '.')
                {
                    continue;
                }
                char freq = line[col];
                _stations[freq].push_back({col, row});
            }
            _num_cols = (int)line.size();
            row += 1;
        }
        _num_rows = row;

        for (auto &s : _stations)
        {
            printf("%c: ", s.first);
            for (auto &pos : s.second)
            {
                printf("%s, ", glm::to_string(pos).c_str());
            }
            printf("\n");
        }
        printf("num rows: %d\n", _num_rows);
        printf("num cols: %d\n", _num_cols);

        file.close();
    }

    int getAntinodes(glm::ivec2 a, glm::ivec2 b, vector<vector<int>> &visited)
    {
        glm::ivec2 ba = a - b;
        glm::ivec2 ab = b - a;

        int count = 0;
        assert(a + ab == b);
        assert(b + ba == a);

        glm::ivec2 p1 = a + 2 * ab;
        glm::ivec2 p2 = b + 2 * ba;
        if (Utils::InRange(p1, _num_cols, _num_rows))
        {
            if (visited[p1.y][p1.x] == 0)
            {
                visited[p1.y][p1.x] = 1;
                count += 1;
            }
        }
        if (Utils::InRange(p2, _num_cols, _num_rows))
        {
            if (visited[p2.y][p2.x] == 0)
            {
                visited[p2.y][p2.x] = 1;
                count += 1;
            }
        }
        return count;
    }

    int getAllAntinodes(vector<glm::ivec2> &stations, std::vector<std::vector<int>> &visited)
    {
        int count = 0;
        for (int i = 0; i < (int)stations.size(); i++)
        {
            for (int j = i + 1; j < (int)stations.size(); j++)
            {
                // These debug values messed me up and cost me an hour of time.
                // visited[stations[i].y][stations[i].x] = 2;
                // visited[stations[j].y][stations[j].x] = 2;
                count += getAntinodes(stations[i], stations[j], visited);
            }
        }

        return count;
    }

    void print(std::vector<std::vector<int>> &visited, char target) {
        ofstream out("visited.txt");
        for (int i = 0; i < _num_rows; i++)
        {
            for (int j = 0; j < _num_cols; j++)
            {
                if (visited[i][j] == 0)
                {
                    out << ".";
                } else if (visited[i][j] == 1) {
                    out << "#";
                } else {
                    out << target;
                }
            }
            out << endl;
        }
        out.close();

    }

    void part1()
    {
        int count = 0;
        vector<vector<int>> visited(_num_rows, vector<int>(_num_cols, 0));
        for (auto &s : _stations)
        {
            count += getAllAntinodes(s.second, visited);
        }
        printf("Count: %d\n", count);
    }

    int getAntinodes2(glm::ivec2 a, glm::ivec2 b, vector<vector<int>> &visited)
    {
        glm::ivec2 ba = a - b;
        glm::ivec2 ab = b - a;

        int count = 0;
        assert(a + ab == b);
        assert(b + ba == a);

        glm::ivec2 p1 = a + ab;
        glm::ivec2 p2 = b + ba;
        while(Utils::InRange(p1, _num_cols, _num_rows)) {
            if (visited[p1.y][p1.x] == 0)
            {
                visited[p1.y][p1.x] = 1;
                count += 1;
            }
            p1 += ab;
        }
        while(Utils::InRange(p2, _num_cols, _num_rows)) {
            if (visited[p2.y][p2.x] == 0)
            {
                visited[p2.y][p2.x] = 1;
                count += 1;
            }
            p2 += ba;
        }
        return count;
    }


    int getAllAntinodes2(vector<glm::ivec2> &stations, std::vector<std::vector<int>> &visited)
    {
        int count = 0;
        for (int i = 0; i < (int)stations.size(); i++)
        {
            for (int j = i + 1; j < (int)stations.size(); j++)
            {
                count += getAntinodes2(stations[i], stations[j], visited);
            }
        }

        return count;
    }

    void part2()
    {
        int count = 0;
        vector<vector<int>> visited(_num_rows, vector<int>(_num_cols, 0));
        for (auto &s : _stations)
        {
            count += getAllAntinodes2(s.second, visited);
        }
        printf("Count: %d\n", count);
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // test(14), real(261)
        part2(); // test(34), real(898)
    }
};