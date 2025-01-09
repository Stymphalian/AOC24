#pragma once

#include "utils.hpp"

class Day16
{
public:
    vector<vector<char>> _grid;
    int _num_rows;
    int _num_cols;
    glm::ivec2 _start_pos;
    glm::ivec2 _end_pos;
    const int RIGHT = 0;
    const int DOWN = 1;
    const int LEFT = 2;
    const int UP = 3;

    Day16() {}
    virtual ~Day16() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day16/test.txt"
                               : "res/Day16/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        int rows = 0;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            std::vector<char> row;
            for (int col = 0; col < (int)line.size(); col++)
            {
                if (line[col] == 'E')
                {
                    _end_pos = glm::ivec2(col, rows);
                    row.push_back('.');
                }
                else if (line[col] == 'S')
                {
                    _start_pos = glm::ivec2(col, rows);
                    row.push_back('.');
                }
                else
                {
                    row.push_back(line[col]);
                }
            }
            _grid.push_back(row);
            rows += 1;
        }
        _num_cols = (int)line.size();
        _num_rows = (int)_grid.size();

        for (int i = 0; i < _num_rows; i++)
        {
            for (int j = 0; j < _num_cols; j++)
            {
                std::cout << _grid[i][j];
            }
            std::cout << std::endl;
        }
        printf("start_pos = (%d, %d), end_pos = (%d, %d)\n", _start_pos.x, _start_pos.y, _end_pos.x, _end_pos.y);
        printf("rows = %d, cols = %d\n", _num_rows, _num_cols);

        file.close();
    }

    struct Position
    {
        glm::ivec2 pos;
        int dir;
        int dist;

        Position Key()
        {
            return {pos, dir, 0};
        }

        std::size_t operator()(const Position &v) const
        {
            std::size_t h = 0;
            Utils::hash_combine(h, v.pos.x, v.pos.y, v.dir, v.dist);
            return h;
        }

        bool operator()(const Position &a, const Position &b) const
        {
            return a.pos.x == b.pos.x && a.pos.y == b.pos.y && a.dir == b.dir && a.dist == b.dist;
        }
    };

    struct Node
    {
        glm::ivec2 pos;
        int dir;
        int dist;
        std::vector<glm::ivec2> path;

        std::size_t operator()(const Node &v) const
        {
            std::size_t h = 0;
            Utils::hash_combine(h, v.pos.x, v.pos.y, v.dir, v.dist);
            return h;
        }

        bool operator()(const Node &l, const Node &r)
        {
            return l.dist > r.dist;
        }
    };

    struct PositionCompare
    {
        bool operator()(const Position &l, const Position &r)
        {
            return l.dist > r.dist;
        }
    };

    using CameFromMap = unordered_map<Position, Position, Position, Position>;
    using DistMap = unordered_map<Position, int, Position, Position>;
    using CameFromMap2 = unordered_map<glm::ivec2, glm::ivec2>;

    vector<Position> reconstructPath(
        CameFromMap &cameFrom,
        Position current)
    {
        vector<Position> path;
        current = {current.pos, current.dir};
        while (cameFrom.find(current) != cameFrom.end())
        {
            path.push_back(current);
            current = cameFrom[current];
            current = {current.pos, current.dir};
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    int calculateScore(vector<Position> path)
    {
        int score = 0;
        for (int i = 1; i < (int) path.size(); i++)
        {
            Position prev = path[i - 1];
            Position current = path[i];
            if (prev.pos == current.pos)
            {
                // Must have been a turn;
                score += 1000;
            }
            else
            {
                score += 1;
            }
        }
        return score;
    }

    void part1()
    {
        // vector<Position> open;
        std::priority_queue<Position, std::vector<Position>, PositionCompare> open;
        CameFromMap cameFrom;
        DistMap dist;

        for (int i = 0; i < _num_rows; i++)
        {
            for (int j = 0; j < _num_cols; j++)
            {
                glm::ivec2 pos = glm::ivec2(j, i);
                dist[Position{pos, RIGHT}] = INT_MAX;
                dist[Position{pos, DOWN}] = INT_MAX;
                dist[Position{pos, LEFT}] = INT_MAX;
                dist[Position{pos, UP}] = INT_MAX;
            }
        }
        Position startPosition = {_start_pos, RIGHT};
        open.push({_start_pos, RIGHT, 0});
        dist[startPosition] = 0;

        Position current;
        // int iterCount = 0;
        while (!open.empty())
        {
            // iterCount += 1;
            current = open.top();
            if (current.pos == _end_pos)
            {
                break;
            }

            glm::ivec2 next = current.pos + Utils::DIRS[current.dir];
            if (Utils::InRange(next, _grid))
            {
                if (_grid[next.y][next.x] != '#')
                {
                    Position nextPos = {next, current.dir};
                    int altDist = current.dist + 1;
                    if (altDist < dist[nextPos])
                    {
                        dist[nextPos] = altDist;
                        cameFrom[nextPos] = current;
                        open.push({next, current.dir, altDist});
                    }
                }
            }
            // Turn Left
            Position leftPos = {current.pos, Utils::turnDirLeft(current.dir)};
            Position rightPos = {current.pos, Utils::turnDirRight(current.dir)};
            if (dist[leftPos] + 1000 < dist[leftPos])
            {
                dist[leftPos] = current.dist + 1000;
                cameFrom[leftPos] = current;
                open.push({current.pos, leftPos.dir, current.dist + 1000});
            }
            if (dist[rightPos] + 1000 < dist[rightPos])
            {
                dist[rightPos] = current.dist + 1000;
                cameFrom[rightPos] = current;
                open.push({current.pos, rightPos.dir, current.dist + 1000});
            }
        }

        vector<Position> path = reconstructPath(cameFrom, current);
        path.insert(path.begin(), startPosition);
        assert(path[0].pos == startPosition.pos && path[0].dir == startPosition.dir);
        int score = calculateScore(path);
        printf("Score: %d\n", score);
    }

    void PrintGrid(glm::ivec2 current)
    {
        for (int y = 0; y < (int) _grid.size(); ++y)
        {
            for (int x = 0; x < (int) _grid[y].size(); ++x)
            {
                if (glm::ivec2(x, y) == current)
                {
                    printf("S");
                }
                else
                {
                    printf("%c", _grid[y][x]);
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    struct SeenNode
    {
        glm::ivec2 pos;
        int dir;

        std::size_t operator()(const SeenNode &v) const
        {
            std::size_t h = 0;
            Utils::hash_combine(h, v.pos.x, v.pos.y, v.dir);
            return h;
        }

        bool operator()(const SeenNode &a, const SeenNode &b) const
        {
            return a.pos.x == b.pos.x && a.pos.y == b.pos.y && a.dir == b.dir;
        }
    };

    void part3()
    {
        std::priority_queue<Node, vector<Node>, Node> open;
        std::unordered_map<SeenNode, int, SeenNode, SeenNode> cost;
        std::vector<tuple<int, vector<glm::ivec2>>> all_paths;

        for (int row = 0; row < (int) _grid.size(); ++row)
        {
            for (int col = 0; col < (int) _grid[row].size(); ++col)
            {
                if (_grid[row][col] == '.')
                {
                    cost[{glm::ivec2(col, row), RIGHT}] = INT_MAX;
                    cost[{glm::ivec2(col, row), DOWN}] = INT_MAX;
                    cost[{glm::ivec2(col, row), LEFT}] = INT_MAX;
                    cost[{glm::ivec2(col, row), UP}] = INT_MAX;
                }
            }
        }

        open.push({_start_pos, RIGHT, 0, {_start_pos}});
        // LEARN: cost is keyed by postion and direction, not just postition
        // The direction allows us to iterate through the multiple number
        // of possible solutions
        cost[{_start_pos, RIGHT}] = 0;

        while (!open.empty())
        {
            // Learn: Storing the path directoy in the iteration
            // saves us from having to traverse through a prev,cameFrom list
            auto [current, currentDir, currentCost, path] = open.top();
            open.pop();

            if (current == _end_pos)
            {
                // Learn: Need to record the currentBest cost here.
                // paths.push_back(constructPaths(current, cameFrom));
                // continue;
                all_paths.push_back({currentCost, path});
                printf("Found\n");
            }

            vector<tuple<int, int>> neighbors{
                {1, currentDir},
                {1001, Utils::turnDirLeft(currentDir)},
                {1001, Utils::turnDirRight(currentDir)},
            };
            for (auto [comingCost, nextDir] : neighbors)
            {
                glm::ivec2 next = current + Utils::DIRS[nextDir];
                if (_grid[next.y][next.x] == '#')
                {
                    continue;
                }
                int candidateCost = currentCost + comingCost;
                int existingCost = cost[{next, nextDir}];
                if (candidateCost <= existingCost)  
                {
                    // missing the condition for nextCost < currentBest
                    cost[{next, nextDir}] = candidateCost;
                    std::vector<glm::ivec2> newPath = path;
                    newPath.push_back(next);
                    open.push({next, nextDir, candidateCost, newPath});
                }
            }
        }
        // assert(paths.size() == 1);

        for (auto [cost, path]: all_paths) {
            printf("Cost: %d\n", cost);
            for (auto pos : path) {
                _grid[pos.y][pos.x] = 'O';
            }
            PrintGrid(_start_pos);
            for (auto pos : path) {
                _grid[pos.y][pos.x] = '.';
            }
        }

        std::unordered_map<glm::ivec2, bool> seats;
        for (auto [cost, path] : all_paths)
        {
            for (auto pos : path)
            {
                seats.insert({pos, true});
            }
        }
        printf("Seats: %d\n", (int)seats.size());
    }

    // I cheated.
    // https://github.com/timfennis/advent-of-code-2024/blob/master/16.ndc
    void
    part2()
    {
        std::priority_queue<Node, vector<Node>, Node> open;
        std::unordered_map<SeenNode, int, SeenNode, SeenNode> cost;
        std::vector<tuple<int, vector<glm::ivec2>>> all_paths;

        for (int row = 0; row < (int) _grid.size(); ++row)
        {
            for (int col = 0; col < (int) _grid[row].size(); ++col)
            {
                if (_grid[row][col] == '.')
                {
                    cost[{glm::ivec2(col, row), RIGHT}] = INT_MAX;
                    cost[{glm::ivec2(col, row), DOWN}] = INT_MAX;
                    cost[{glm::ivec2(col, row), LEFT}] = INT_MAX;
                    cost[{glm::ivec2(col, row), UP}] = INT_MAX;
                }
            }
        }

        open.push({_start_pos, RIGHT, 0, {_start_pos}});
        cost[{_start_pos, RIGHT}] = 0;

        int currentBest = INT_MAX;
        while (!open.empty())
        {
            auto [current, currentDir, currentCost, path] = open.top();
            open.pop();

            if (current == _end_pos)
            {
                currentBest = std::min(currentCost, currentBest);
                all_paths.push_back({currentCost, path});
            }

            vector<tuple<int, int>> neighbors = {
                {1, currentDir},
                {1001, Utils::turnDirLeft(currentDir)},
                {1001, Utils::turnDirRight(currentDir)}};
            for (auto [comingCost, nextDir] : neighbors)
            {
                glm::ivec2 next = current + Utils::DIRS[nextDir];
                int nextCost = currentCost + comingCost;
                if (_grid[next.y][next.x] == '.')
                {
                    SeenNode key = {next, nextDir};
                    if (cost[key] >= nextCost && nextCost < currentBest)
                    {
                        cost[key] = nextCost;

                        std::vector newPath = path;
                        newPath.push_back(next);
                        open.push({next, nextDir, nextCost, std::move(newPath)});
                    }
                }
            }
        }

        for (auto [cost, path]: all_paths) {
            printf("Cost: %d\n", cost);
            for (auto pos : path) {
                _grid[pos.y][pos.x] = 'O';
            }
            PrintGrid(_start_pos);
            for (auto pos : path) {
                _grid[pos.y][pos.x] = '.';
            }
        }

        std::unordered_map<glm::ivec2, bool> seats;
        for (auto [cost, path] : all_paths)
        {
            for (auto pos : path)
            {
                seats.insert({pos, true});
            }
        }
        printf("Seats: %d\n", (int)seats.size());
    }

    void
    Run()
    {
        bool readTest = true;
        ReadInput(readTest);
        // part1(); // test1(7036), test2(11048), real(101492)
        part2(); // test1(45), test2(64), real(543)
        // part3(); // test1(45), test2(64), real(543)
    }
};

// Learning:
// 1. I don't know shortest path algorithms well enough
// 2. I don't understand how to record multiple paths in an interative fashion