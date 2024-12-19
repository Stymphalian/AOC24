#pragma once

#include "utils.hpp"

class Day15
{
public:
    const int SPACE = -1;
    const int WALL = -2;
    const int BOX = -3;
    const int ROBOT = -4;
    const int BOX1 = -5;
    const int BOX2 = -6;

    const int RIGHT = 0;
    const int DOWN = 1;
    const int LEFT = 2;
    const int UP = 3;

    Utils::GridInt _grid;
    vector<glm::ivec2> _boxes;
    glm::ivec2 _robot_start;
    glm::ivec2 _robot;
    int _num_rows = 0;
    int _num_cols = 0;
    vector<int> _moves;

    Day15() {}
    virtual ~Day15() {}

    void ReadInput(bool readTest = false, bool isPhase2 = false)
    {
        std::ifstream file(readTest
                               ? "res/Day15/test2.txt"
                               : "res/Day15/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        int state = 0;
        int rows = 0;
        int box_id = 0;
        while (std::getline(file, line))
        {
            Utils::trim(line);

            if (line.empty())
            {
                state += 1;
                continue;
            }
            if (state == 0)
            {
                std::vector<int> row;
                int col_count = 0;
                for (int col = 0; col < line.size(); col++)
                {
                    if (line[col] == '#')
                    {
                        row.push_back(WALL);
                        if (isPhase2)
                        {
                            row.push_back(WALL);
                        }
                    }
                    else if (line[col] == '.')
                    {
                        row.push_back(SPACE);
                        if (isPhase2)
                        {
                            row.push_back(SPACE);
                        }
                    }
                    else if (line[col] == '@')
                    {
                        if (isPhase2)
                        {
                            _robot_start = glm::ivec2(col_count, rows);
                            row.push_back(SPACE);
                            row.push_back(SPACE);
                        }
                        else
                        {
                            _robot_start = glm::ivec2(col, rows);
                            row.push_back(SPACE);
                        }
                    }
                    else if (line[col] == 'O')
                    {
                        if (isPhase2)
                        {
                            row.push_back(SPACE);
                            row.push_back(SPACE);
                            _boxes.push_back(glm::ivec2(col_count, rows));
                            box_id += 1;
                        }
                        else
                        {
                            row.push_back(BOX);
                        }
                    }
                    col_count += 2;
                }
                rows += 1;
                _grid.push_back(row);
            }
            else if (state == 1)
            {
                for (int col = 0; col < line.size(); col++)
                {
                    if (line[col] == '>')
                    {
                        _moves.push_back(RIGHT);
                    }
                    else if (line[col] == 'v')
                    {
                        _moves.push_back(DOWN);
                    }
                    else if (line[col] == '<')
                    {
                        _moves.push_back(LEFT);
                    }
                    else if (line[col] == '^')
                    {
                        _moves.push_back(UP);
                    }
                }
            }
        }
        _num_rows = (int)_grid.size();
        _num_cols = (int)_grid[0].size();
        _robot = _robot_start;


        file.close();
    }

    void updateGrid(int move)
    {
        glm::ivec2 dir = Utils::DIRS[move];
        glm::ivec2 next = _robot + dir;

        int box_count = 0;
        glm::ivec2 anchor;
        while (Utils::InRange(next, _grid))
        {
            int c = _grid[next.y][next.x];
            if (c == SPACE)
            {
                anchor = next;
                break;
            }
            else if (c == WALL)
            {
                anchor = next - dir;
                break;
            }
            else if (c == BOX)
            {
                _grid[next.y][next.x] = SPACE;
                next = next + dir;
                box_count += 1;
            }
        }

        // Move the robot
        glm::ivec2 reverse = dir * -1;
        for (int i = 0; i < box_count; i++)
        {
            glm::ivec2 next = anchor + i * reverse;
            _grid[next.y][next.x] = BOX;
        }
        _robot = anchor + (box_count)*reverse;
    }

    void PrintGrid()
    {
        _grid[_robot.y][_robot.x] = ROBOT;
        Utils::PrintIntVector2D(_grid);
        _grid[_robot.y][_robot.x] = SPACE;
    }

    int calcGPS()
    {
        int sum = 0;
        for (int row = 0; row < _num_rows; row++)
        {
            for (int col = 0; col < _num_cols; col++)
            {
                if (_grid[row][col] == BOX)
                {
                    sum += 100 * row + col;
                }
            }
        }
        return sum;
    }

    void part1()
    {
        _robot = _robot_start;
        for (int move : _moves)
        {
            updateGrid(move);
            // PrintGrid();
        }
        int gps = calcGPS();
        printf("GPS = %d\n", gps);
    }

    void PrintGrid2(int move = 0)
    {
        std::ofstream out(fmt::format("grid-{}.txt", move));
        if (!out.is_open())
        {
            std::cerr << "Can't open file.\n";
            return;
        }

        _grid[_robot.y][_robot.x] = ROBOT;
        for (int box_id = 0; box_id < (int)_boxes.size(); box_id++)
        {
            glm::ivec2 box = _boxes[box_id];
            assert(Utils::InRange(box, _grid));
            _grid[box.y][box.x] = box_id;
            _grid[box.y][box.x + 1] = box_id;
        }

        for (int row = 0; row < _num_rows; row++)
        {
            for (int col = 0; col < _num_cols; col++)
            {
                int c = _grid[row][col];
                if (c == SPACE)
                {
                    out << ".";
                }
                else if (c == WALL)
                {
                    out << "#";
                }
                else if (c == ROBOT)
                {
                    out << "@";
                }
                else if (c == BOX)
                {
                    out << "O";
                }
                else if (c == BOX1)
                {
                    out << "[";
                }
                else if (c == BOX2)
                {
                    out << "]";
                }
                else if (c >= 0)
                {
                    if (glm::ivec2(col, row) == _boxes[c])
                    {
                        out << "[";
                    }
                    else
                    {
                        out << "]";
                    }
                }
                else
                {
                    out << "?";
                }
            }
            out << std::endl;
        }
        out << std::endl;

        _grid[_robot.y][_robot.x] = SPACE;
        for (int i = 0; i < _boxes.size(); i++)
        {
            glm::ivec2 box = _boxes[i];
            _grid[box.y][box.x] = SPACE;
            _grid[box.y][box.x + 1] = SPACE;
        }
    }

    int calcGPS2()
    {
        int sum = 0;
        for (int i = 0; i < _boxes.size(); i++)
        {
            glm::ivec2 box = _boxes[i];
            sum += 100 * box.y + box.x;
        }
        return sum;
    }

    int hasBox(glm::ivec2 pos)
    {
        for (int i = 0; i < (int)_boxes.size(); i++)
        {
            int box_id = i;
            glm::ivec2 box_pos = _boxes[box_id];
            if (pos == box_pos || pos == (box_pos + glm::ivec2(1, 0)))
            {
                return box_id;
            }
        }
        return -1;
    }

    void updateGrid2(int move)
    {
        glm::ivec2 dir = Utils::DIRS[move];
        glm::ivec2 next = _robot + dir;
        assert(Utils::InRange(next, _grid));

        int boxId = hasBox(next);
        bool isWall = (_grid[next.y][next.x] == WALL);
        if (isWall)
        {
            return;
        }
        else if (boxId == -1)
        {
            _robot = next;
            return;
        }

        // isBox
        vector<int> stack;
        vector<int> chain;

        assert(boxId != -1);
        stack.push_back(boxId);
        unordered_set<int> seen;

        bool hitWall = false;
        while (stack.size() > 0)
        {
            int box_id = stack.back();
            stack.pop_back();
            if (seen.find(box_id) != seen.end())
            {
                continue;
            }

            // BUG WAS HERE. I was using BoxId instead of box_id
            // How to avoid this in the future? better naming of variables
            // Create functions for logic so that it is easier to avoid sharing state
            seen.insert(box_id);
            chain.push_back(box_id);

            assert(dir == Utils::DIRS[0] || dir == Utils::DIRS[1] || dir == Utils::DIRS[2] || dir == Utils::DIRS[3]);
            glm::ivec2 box_pos = _boxes[box_id];
            glm::ivec2 next1 = box_pos + dir;
            glm::ivec2 next2 = box_pos + glm::ivec2(1, 0) + dir;

            if (move == LEFT)
            {
                int isBoxLeft = hasBox(next1);
                if (isBoxLeft != -1 && seen.find(isBoxLeft) == seen.end())
                {
                    stack.push_back(isBoxLeft);
                }
            }
            else if (move == RIGHT)
            {
                int isBoxRight = hasBox(next2);
                if (isBoxRight != -1 && seen.find(isBoxRight) == seen.end())
                {
                    stack.push_back(isBoxRight);
                }
            }
            else
            {
                assert(move == UP || move == DOWN);
                int isBoxLeft = hasBox(next1);
                int isBoxRight = hasBox(next2);
                if (isBoxLeft != -1 && seen.find(isBoxLeft) == seen.end())
                {
                    stack.push_back(isBoxLeft);
                }
                if (isBoxRight != -1 && seen.find(isBoxRight) == seen.end())
                {
                    stack.push_back(isBoxRight);
                }
            }

            bool n1IsWall = (_grid[next1.y][next1.x] == WALL);
            bool n2IsWall = (_grid[next2.y][next2.x] == WALL);
            if (n1IsWall || n2IsWall)
            {
                hitWall = true;
                break;
            }
        }

        // Move all the things
        if (hitWall)
        {
            // can't move;
            return;
        }

        // did not hit a wall, move all the boxes and the robot up
        assert(dir == Utils::DIRS[0] || dir == Utils::DIRS[1] || dir == Utils::DIRS[2] || dir == Utils::DIRS[3]);
        for (int box_id : chain)
        {
            _boxes[box_id] = _boxes[box_id] + dir;
        }

        _robot = _robot + dir;
    }

    bool checkOverlappingBoxes()
    {
        bool bad = false;
        unordered_map<glm::ivec2, int> seen;
        for (int i = 0; i < _boxes.size(); i++)
        {
            glm::ivec2 pos = _boxes[i];
            glm::ivec2 pos1 = pos + glm::ivec2(1, 0);
            if (seen.find(pos) != seen.end())
            {
                int otherBoxID = seen[pos];
                bad = true;
                printf("Overlap betweeen box %d and %d\n", i, otherBoxID);
            }
            if (seen.find(pos1) != seen.end())
            {
                int otherBoxID = seen[pos1];
                bad = true;
                printf("Overlap betweeen box %d and %d\n", i, otherBoxID);
            }
            seen[pos] = i;
            seen[pos1] = i;
        }
        return bad;
    }

    void part2()
    {
        for (int i = 0; i < _moves.size(); i++)
        {
            int move = _moves[i];
            updateGrid2(move);
        }

        int gps = calcGPS2();
        printf("GPS = %d\n", gps);
    }

    void
    Run()
    {
        bool readTest = false;
        bool isPhase2 = true;
        ReadInput(readTest, isPhase2);
        // part1(); // test2(10092), test(2028), real(1526673)
        part2(); // test2(9021), real(1535509)
    }
};

// Write up:
// I got stuck on a bug for 4 hours. 
// There was something causing my updateGrid function to not move the blocks correctly
// in phase2. In the end it turned out that I was the variable boxId instead box_id.
// Using a vector to look up the positions of the boxes is very inefficient, but
// it took only ~4 seconds to calculate the right answer so whatever.
// Probably better to use a map from the id to the position