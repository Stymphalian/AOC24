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

    Day16() {}
    virtual ~Day16() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day16/test2.txt"
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

    const int RIGHT = 0;
    const int DOWN = 1;
    const int LEFT = 2;
    const int UP = 3;

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
    // using CameFromMap2 = unordered_map<Position, std::vector<Position>, Position, Position>;
    using DistMap = unordered_map<Position, int, Position, Position>;
    using DistMapVec2 = unordered_map<glm::ivec2, int>;
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

    vector<glm::ivec2> reconstructPath2(
        CameFromMap2 &cameFrom,
        glm::ivec2 current)
    {
        vector<glm::ivec2> path;
        while (cameFrom.find(current) != cameFrom.end())
        {
            path.push_back(current);
            current = cameFrom[current];
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    int calculateScore(vector<Position> path)
    {
        int score = 0;
        for (int i = 1; i < path.size(); i++)
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

    Position popNextPosition(vector<Position> &open, DistMap &dist)
    {
        // Get next minimum distance node
        int currentIndex = -1;
        int bestDistance = INT_MAX;
        for (int i = 0; i < open.size(); i++)
        {
            Position candidate = open[i];
            if (dist[candidate] < bestDistance)
            {
                currentIndex = i;
                bestDistance = dist[candidate];
            }
        }
        // assert(currentIndex != -1);
        if (currentIndex == -1)
        {
            printf("No current index");
        }
        Position current = open[currentIndex];
        open.erase(open.begin() + currentIndex);
        return current;
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
                dist[{pos, RIGHT}] = INT_MAX;
                dist[{pos, DOWN}] = INT_MAX;
                dist[{pos, LEFT}] = INT_MAX;
                dist[{pos, UP}] = INT_MAX;
                // open.push({pos, RIGHT, INT_MAX});
                // open.push({pos, DOWN, INT_MAX});
                // open.push({pos, LEFT, INT_MAX});
                // open.push({pos, UP, INT_MAX});
            }
        }
        Position startPosition = {_start_pos, RIGHT};
        open.push({_start_pos, RIGHT, 0});
        dist[startPosition] = 0;

        unordered_map<glm::ivec2, bool> apartOfPath;

        Position current;
        int iterCount = 0;
        while (!open.empty())
        {
            iterCount += 1;
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

    void PrintGrid(unordered_map<glm::ivec2, bool> apart)
    {
        for (int y = 0; y < _grid.size(); ++y)
        {
            for (int x = 0; x < _grid[y].size(); ++x)
            {
                if (apart.find({x, y}) != apart.end())
                {
                    printf("O");
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

    void PrintGrid2(glm::ivec2 current)
    {
        for (int y = 0; y < _grid.size(); ++y)
        {
            for (int x = 0; x < _grid[y].size(); ++x)
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

    void bfs(
        glm::ivec2 current,
        int currentDir,
        int currentCost,
        std::unordered_map<glm::ivec2, int> &visited,
        std::unordered_map<glm::ivec2, int> &cost,
        std::unordered_map<glm::ivec2, glm::ivec2> &cameFrom,
        std::vector<tuple<int, vector<glm::ivec2>>> &paths)
    {
        if (current == _end_pos)
        {
            if (currentCost > cost[_end_pos]) {
                return;
            }
            if (currentCost < cost[_end_pos])
            {
                paths.clear();
            }

            vector<glm::ivec2> path;
            while (cameFrom.find(current) != cameFrom.end())
            {
                path.push_back(current);
                current = cameFrom[current];
            }
            path.push_back(_start_pos);
            std::reverse(path.begin(), path.end());

            paths.push_back({currentCost, std::move(path)});
            return;
        }
        if (visited[current] == 1)
        {
            return;
        }

        visited[current] = 1;
        for (int i = 0; i < 4; i++)
        {
            glm::ivec2 next = current + Utils::DIRS[i];
            if (visited[next] == 1)
            {
                continue;
            }
            if (_grid[next.y][next.x] == '#')
            {
                continue;
            }

            int extraCost = 1;
            if (i == Utils::turnDirLeft(currentDir))
            {
                extraCost += 1000;
            }
            else if (i == Utils::turnDirRight(currentDir))
            {
                extraCost += 1000;
            }
            else if (i == Utils::turnDirBack(currentDir))
            {
                extraCost += 2000;
            }

            int candidateCost = currentCost + extraCost;
            int existingCost = cost[next];

            if (existingCost < candidateCost)
            {
                continue;
            }

            cameFrom[next] = current;
            
            bfs(next, i, candidateCost, visited, cost, cameFrom, paths);
            cost[next] = candidateCost;
            // cost[next] = existingCost;
            cameFrom.erase(next);
        }
        visited[current] = 0;
    }

    // vector<vector<glm::ivec2>> constructPaths(
    //     glm::ivec2 current,
    //     std::unordered_map<glm::ivec2, vector<glm::ivec2>> &cameFrom)
    // {
    //     vector<glm::ivec2> path;
    //     // while (cameFrom.find(current) != cameFrom.end())
    //     // {
    //     //     path.push_back(current);
    //     //     current = cameFrom[current];
    //     // }
    //     // path.push_back(_start_pos);
    //     // std::reverse(path.begin(), path.end());

    //     return {};
    // }

    void
    part2()
    {
        std::unordered_map<glm::ivec2, int> visited;
        std::priority_queue<Node, vector<Node>, Node> open;
        std::unordered_map<glm::ivec2, glm::ivec2> cameFrom;
        std::unordered_map<glm::ivec2, int> cost;
        std::vector<tuple<int, vector<glm::ivec2>>> paths;

        for (int row = 0; row < _grid.size(); row++)
        {
            for (int col = 0; col < _grid[row].size(); col++)
            {
                if (_grid[row][col] == '.')
                {
                    cost[{col, row}] = INT_MAX;
                }
            }
        }
        cost[_start_pos] = 0;

        bfs(_start_pos, 0, 0, visited, cost, cameFrom, paths);

        int minCost = INT_MAX;
        for (auto [cost, path] : paths)
        {
            if (cost < minCost)
            {
                minCost = cost;
            }
        }
        for (auto [cost, path] : paths)
        {
            if (cost > minCost) {continue;}
            printf("Cost: %d\n", cost);
            for (auto pos : path)
            {
                _grid[pos.y][pos.x] = 'O';
            }
            PrintGrid2(_start_pos);
            for (auto pos : path)
            {
                _grid[pos.y][pos.x] = '.';
            }
        }
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // test1(7036), test2(11048)
        part2(); // test1(45), test2(64)
    }
};