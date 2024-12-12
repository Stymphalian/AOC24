#pragma once

#include "utils.hpp"

// Hard one.
class Day12
{
public:
    // static const glm::ivec2 dirs[] = {
    //     glm::ivec2(0, 1),  // down
    //     glm::ivec2(0, -1), // up
    //     glm::ivec2(1, 0),  // right
    //     glm::ivec2(-1, 0)  // left
    // };
    vector<vector<char>> _grid;
    int _num_rows = 0;
    int _num_cols = 0;

    Day12() {}
    virtual ~Day12() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day12/test.txt"
                               : "res/Day12/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);

            vector<char> row;
            for (int i = 0; i < line.length(); i++)
            {
                row.push_back(line[i]);
            }
            _grid.push_back(row);
            _num_cols = (int)line.length();
        }
        _num_rows = (int)_grid.size();

        for (int i = 0; i < _num_rows; i++)
        {
            for (int j = 0; j < _num_cols; j++)
            {
                cout << _grid[i][j];
            }
            cout << endl;
        }
        printf("num_cols: %d\n", _num_cols);
        printf("num_rows: %d\n", _num_rows);

        file.close();
    }

    struct Walk
    {
        glm::ivec2 pos;
        int _forward;
        int _outside;
        int _turn;

        Walk() = default;

        Walk(glm::ivec2 pos, int outside, int turn)
        {
            this->pos = pos;
            this->_outside = outside;
            this->_turn = turn;
            if (turn == 0)
            {
                _forward = (_outside + 1) % 4;
            }
            else
            {
                _forward = (_outside == 0) ? 3 : _outside - 1;
            }
        }

        void turn_outside()
        {
            if (_turn == 0)
            {
                // ^    outside
                // |
                // ---> forward
                _forward = (_forward == 0) ? 3 : _forward - 1;
                _outside = (_forward == 0) ? 3 : _forward - 1;
            }
            else
            {
                // ---> forward
                // |
                // v   outside
                _forward = (_forward + 1) % 4;
                _outside = (_forward + 1) % 4;
            }
        }

        void turn_inside()
        {
            if (_turn == 0)
            {
                // ^  outside
                // |
                // ---> forward
                _forward = (_forward + 1) % 4;
                _outside = (_forward == 0) ? 3 : _forward - 1;
            }
            else
            {
                // ---> forward
                // |
                // v outside
                _forward = (_forward == 0) ? 3 : _forward - 1;
                _outside = (_forward + 1) % 4;
            }
        }

        int forward()
        {
            return _forward;
        }

        int outside()
        {
            return _outside;
        }
        int inside()
        {
            return (_outside + 2) % 4;
        }

        std::size_t operator()(const Walk &v) const
        {
            std::size_t h = 0;
            Utils::hash_combine(h, v.pos.x, v.pos.y, v._forward, v._outside, v._turn);
            return h;
        }

        bool operator()(const Walk &a, const Walk &b) const
        {
            return (
                a.pos == b.pos &&
                a._forward == b._forward &&
                a._outside == b._outside &&
                a._turn == b._turn);
        }
    };

    struct Region
    {

        std::vector<glm::ivec2> dirs = {
            glm::ivec2(1, 0),  // right
            glm::ivec2(0, 1),  // down
            glm::ivec2(-1, 0), // left
            glm::ivec2(0, -1)  // up
        };
        vector<glm::ivec2> pieces;
        // vector<glm::ivec2> ppieces;
        int perimeter = 0;
        int area = 0;

        void addPiece(glm::ivec2 piece, vector<vector<char>> &grid)
        {
            int p = calcPerim(piece, grid);
            if (p != 0)
            {
                pieces.push_back(piece);
            }
            perimeter += p;
            area += 1;
            // pieces.push_back(piece);
        }

        int calcPerim(glm::ivec2 piece, vector<vector<char>> &grid)
        {
            int num_rows = (int)grid.size();
            int num_cols = (int)grid[0].size();
            char c = grid[piece.y][piece.x];

            int x = piece.x;
            int y = piece.y;

            int fences = 0;
            for (int i = 0; i < 4; i++)
            {
                glm::ivec2 dir = dirs[i];
                glm::ivec2 next = piece + dir;
                if (!Utils::InRange(next, num_cols, num_rows))
                {
                    fences += 1;
                }
                else if (grid[next.y][next.x] != c)
                {
                    fences += 1;
                }
            }

            return fences;
        }

        // right, down, left, up
        glm::ivec4 getNeighbors(glm::ivec2 piece, vector<vector<char>> &grid)
        {
            int num_rows = (int)grid.size();
            int num_cols = (int)grid[0].size();
            glm::ivec4 neighbors;
            char c = grid[piece.y][piece.x];
            for (int i = 0; i < 4; i++)
            {
                glm::ivec2 dir = dirs[i];
                glm::ivec2 next = piece + dir;
                if (Utils::InRange(next, num_cols, num_rows))
                {
                    neighbors[i] = (grid[next.y][next.x] == c) ? 1 : 0;
                }
                else
                {
                    neighbors[i] = 0;
                }
            }

            return neighbors;
        }

        std::vector<Walk> getStartingWalks(glm::ivec2 pos, vector<vector<char>> &grid)
        {
            glm::ivec4 neighbors = getNeighbors(pos, grid);

            vector<Walk> output;
            for (int outside = 0; outside < 4; outside++)
            {
                if (neighbors[outside] == 0)
                {
                    output.push_back(Walk(pos, outside, 0));
                    output.push_back(Walk(pos, outside, 1));
                }
            }
            return output;
        }

        int getSides(vector<vector<char>> &grid)
        {
            unordered_set<Walk, Walk, Walk> seen;

            int sides = 0;

            for (int i = 0; i < pieces.size(); i++)
            {
                glm::ivec2 start = pieces[i];
                vector<Walk> walks = getStartingWalks(start, grid);

                for (Walk current : walks)
                {
                    if (seen.find(current) != seen.end())
                    {
                        continue;
                    }

                    while (true)
                    {
                        if (seen.find(current) != seen.end())
                        {
                            break;
                        }
                        seen.insert(current);

                        glm::ivec4 neighbors = getNeighbors(current.pos, grid);
                        if (neighbors[current.outside()] == 1)
                        {
                            current.pos += dirs[current.outside()];
                            current.turn_outside();
                            sides += 1;
                        }
                        else if (neighbors[current.forward()] == 1)
                        {
                            current.pos += dirs[current.forward()];
                        }
                        else if (neighbors[current.forward()] == 0)
                        {
                            current.turn_inside();
                            sides += 1;
                        }
                    }
                }
            }

            return sides;
        }
    };

    void part1()
    {
        static const glm::ivec2 dirs[] = {
            glm::ivec2(0, 1),  // down
            glm::ivec2(0, -1), // up
            glm::ivec2(1, 0),  // right
            glm::ivec2(-1, 0)  // left
        };
        vector<vector<int>> visited(_num_rows, vector<int>(_num_cols, 0));

        vector<Region> regions;

        for (int row = 0; row < _num_rows; row++)
        {
            for (int col = 0; col < _num_cols; col++)
            {
                char c = _grid[row][col];
                if (visited[row][col] == 1)
                {
                    continue;
                }

                // flood fill through the region
                queue<glm::ivec2> stack;
                stack.push(glm::ivec2(col, row));
                Region region;
                while (stack.size() > 0)
                {
                    glm::ivec2 current = stack.front();
                    stack.pop();
                    region.addPiece(current, _grid);

                    char plant = _grid[current.y][current.x];
                    visited[current.y][current.x] = 1;
                    for (int i = 0; i < 4; i++)
                    {
                        glm::ivec2 dir = dirs[i];
                        glm::ivec2 next = current + dir;
                        if (Utils::InRange(next, _num_cols, _num_rows) &&
                            visited[next.y][next.x] == 0 &&
                            _grid[next.y][next.x] == plant)
                        {
                            visited[next.y][next.x] = 1;
                            stack.push(next);
                        }
                    }
                }

                regions.push_back(region);
            }
        }

        int cost = 0;
        for (int i = 0; i < regions.size(); i++)
        {
            cost += regions[i].perimeter * regions[i].area;
        }
        printf("Cost = %d\n", cost);
    }

    void part2()
    {

        static const glm::ivec2 dirs[] = {
            glm::ivec2(0, 1),  // down
            glm::ivec2(0, -1), // up
            glm::ivec2(1, 0),  // right
            glm::ivec2(-1, 0)  // left
        };
        vector<vector<int>> visited(_num_rows, vector<int>(_num_cols, 0));

        vector<Region> regions;

        for (int row = 0; row < _num_rows; row++)
        {
            for (int col = 0; col < _num_cols; col++)
            {
                char c = _grid[row][col];
                if (visited[row][col] == 1)
                {
                    continue;
                }

                // flood fill through the region
                queue<glm::ivec2> stack;
                stack.push(glm::ivec2(col, row));
                Region region;
                while (stack.size() > 0)
                {
                    glm::ivec2 current = stack.front();
                    stack.pop();
                    region.addPiece(current, _grid);

                    char plant = _grid[current.y][current.x];
                    visited[current.y][current.x] = 1;
                    for (int i = 0; i < 4; i++)
                    {
                        glm::ivec2 dir = dirs[i];
                        glm::ivec2 next = current + dir;
                        if (Utils::InRange(next, _num_cols, _num_rows) &&
                            visited[next.y][next.x] == 0 &&
                            _grid[next.y][next.x] == plant)
                        {
                            visited[next.y][next.x] = 1;
                            stack.push(next);
                        }
                    }
                }

                regions.push_back(region);
            }
        }

        int cost = 0;
        for (int i = 0; i < regions.size(); i++)
        {
            int sides = regions[i].getSides(_grid);
            cost += regions[i].area * sides;
        }
        printf("Cost = %d\n", cost/2);
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // test(1930), real(1473408)
        part2(); // test(1206), real(886364)
    }
};