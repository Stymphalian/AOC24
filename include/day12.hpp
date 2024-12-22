#pragma once

#include "utils.hpp"

// Hard one.
// Improvements
// 1. Garden class
// 2. Finding number of sides is the same as counting number of corners.
class Day12
{
public:
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
            for (int i = 0; i < (int) line.length(); i++)
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

        Walk getInverse()
        {
            Walk other;
            other.pos = pos;
            other._forward = (_forward + 2) % 4;
            other._outside = _outside;
            other._turn = (_turn + 1) % 2;
            return other;
        }

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
        vector<glm::ivec2> perimiterLocations;
        vector<glm::ivec2> tiles;
        int perimeter = 0;
        int area = 0;

        void addPiece(glm::ivec2 pos, vector<vector<char>> &grid)
        {
            int p = calcPerim(pos, grid);
            if (p != 0)
            {
                perimiterLocations.push_back(pos);
            }
            perimeter += p;
            area += 1;
            tiles.push_back(pos);
        }

        int calcPerim(glm::ivec2 pos, vector<vector<char>> &grid)
        {
            char c = grid[pos.y][pos.x];
            int fences = 0;
            for (int i = 0; i < 4; i++)
            {
                glm::ivec2 dir = Utils::DIRS[i];
                glm::ivec2 next = pos + dir;
                if (!Utils::InRange(next, grid))
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
            glm::ivec4 neighbors;
            char c = grid[piece.y][piece.x];
            for (int i = 0; i < 4; i++)
            {
                glm::ivec2 next = piece + Utils::DIRS[i];
                if (Utils::InRange(next, grid))
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

        bool contains(glm::ivec2 pos, glm::ivec2 dir, vector<vector<char>> &grid)
        {
            glm::ivec2 next = pos + dir;
            return Utils::InRange(next, grid) && (grid[next.y][next.x] == grid[pos.y][pos.x]);
        }

        // https://github.com/kupuguy/aoc2024/blob/main/src/day12.py
        int getSides(vector<vector<char>> &grid)
        {
            int sides = 0;

            // count corners
            for (int i = 0; i < (int) tiles.size(); i++)
            {
                glm::ivec4 nb = getNeighbors(tiles[i], grid);
                bool right = (nb[0] == 1);
                bool down = (nb[1] == 1);
                bool left = (nb[2] == 1);
                bool up = (nb[3] == 1);

                int tl = (!up && !left) || (up && left && !contains(tiles[i], glm::ivec2(-1, -1), grid));
                int tr = (!up && !right) || (up && right && !contains(tiles[i], glm::ivec2(1, -1), grid));
                int bl = (!down && !left) || (down && left && !contains(tiles[i], glm::ivec2(-1, 1), grid));
                int br = (!down && !right) || (down && right && !contains(tiles[i], glm::ivec2(1, 1), grid));
                sides += tl + tr + br + bl;
            }

            return sides;
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

        int getSidesOld(vector<vector<char>> &grid)
        {
            unordered_set<Walk, Walk, Walk> seen;

            int sides = 0;

            // Walk through every tile which belongs to the perimiter
            // Makes turns based on if we have reached a corner or not
            // We increment the 'sides' counter whenever we make a turn.
            // The direction we turn is based on the direction which is
            // considered on the "outside".
            // Once we reach a tile we have already seen then we know we have
            // completed a circuit around the perimeter and have counted all
            // the sides.
            // By iterating over every tile considering both sides as "outside"
            // and changing our "turn" condition we can count internal sides
            // aswell.
            for (int i = 0; i < (int) perimiterLocations.size(); i++)
            {
                glm::ivec2 start = perimiterLocations[i];
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
                        seen.insert(current.getInverse());

                        glm::ivec4 neighbors = getNeighbors(current.pos, grid);
                        if (neighbors[current.outside()] == 1)
                        {
                            // The outside has a neighbor tile, we make a turn
                            // into that tile and continue
                            current.pos += Utils::DIRS[current.outside()];
                            current.turn_outside();
                            sides += 1;
                        }
                        else if (neighbors[current.forward()] == 1)
                        {
                            // We can keep moving forward without needing to
                            // make a turn.
                            current.pos += Utils::DIRS[current.forward()];
                        }
                        else if (neighbors[current.forward()] == 0)
                        {
                            // We have reached a dead end, we can't move
                            // forward anymore and there isn't a neighbor on the
                            // outside. So turn inside and continue.
                            current.turn_inside();
                            sides += 1;
                        }
                    }
                }
            }

            return sides;
        }
    };

    std::vector<Region> getRegions()
    {
        vector<vector<int>> visited(_num_rows, vector<int>(_num_cols, 0));
        vector<Region> regions;

        for (int row = 0; row < _num_rows; row++)
        {
            for (int col = 0; col < _num_cols; col++)
            {
                // char c = _grid[row][col];
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
                        glm::ivec2 dir = Utils::DIRS[i];
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
        return regions;
    }

    void part1()
    {
        vector<Region> regions = getRegions();

        int cost = 0;
        for (int i = 0; i < (int) regions.size(); i++)
        {
            cost += regions[i].perimeter * regions[i].area;
        }
        printf("Cost = %d\n", cost);
    }

    void part2()
    {
        vector<Region> regions = getRegions();

        int cost = 0;
        for (int i = 0; i < (int) regions.size(); i++)
        {
            int sides = regions[i].getSides(_grid);
            cost += regions[i].area * sides;
        }
        printf("Cost = %d\n", cost);
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // test(1930), real(1473408)
        part2(); // test(1206), test2(368), real(886364)
    }
};