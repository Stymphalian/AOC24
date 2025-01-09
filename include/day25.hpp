#pragma once

#include "utils.hpp"

class Day25
{
public:
    struct Key
    {
        vector<int> _heights;
        int _num_rows;
        int _num_cols;

        Key(vector<string> &input)
        {
            _num_rows = (int)input.size() - 1;
            _num_cols = (int)input[0].length();
            _heights.reserve(_num_cols);
            for (int i = 0; i < _num_cols; i++)
            {
                _heights.push_back(0);
            }

            // skip last row of #
            for (size_t row = 0; row < input.size() - 1; row++)
            {
                for (size_t col = 0; col < (size_t)_num_cols; col++)
                {
                    _heights[col] += input[row][col] == '.' ? 0 : 1;
                }
            }
        }
    };

    struct Lock
    {
        vector<int> _heights;
        int _num_rows;
        int _num_cols;

        Lock(vector<string> &input)
        {
            _num_rows = (int)input.size() - 1;
            _num_cols = (int)input[0].length();
            _heights.reserve(_num_cols);
            for (int i = 0; i < _num_cols; i++)
            {
                _heights.push_back(0);
            }

            // skip first row of #
            for (size_t row = 1; row < input.size(); row++)
            {
                for (size_t col = 0; col < (size_t)_num_cols; col++)
                {
                    _heights[col] += input[row][col] == '.' ? 0 : 1;
                }
            }
        }
    };

    vector<Key> _keys;
    vector<Lock> _locks;

    Day25() {}
    virtual ~Day25() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day25/test.txt"
                               : "res/Day25/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        int state = 0;

        vector<string> keyOrLockInfo;
        const int KEY_STATE = 1;
        const int LOCK_STATE = 2;

        vector<vector<string>> locks;
        vector<vector<string>> keys;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            if (line.empty())
            {
                if (state == KEY_STATE)
                {
                    keys.push_back(keyOrLockInfo);
                }
                else if (state == LOCK_STATE)
                {
                    // lock
                    locks.push_back(keyOrLockInfo);
                }
                state = 0;
                keyOrLockInfo.clear();
                continue;
            }

            if (state == 0)
            {
                bool allHash = true;
                for (char c : line)
                {
                    if (c != '#')
                    {
                        allHash = false;
                        break;
                    }
                }

                if (allHash)
                {
                    state = LOCK_STATE;
                }
                else
                {
                    state = KEY_STATE;
                }
            }
            keyOrLockInfo.push_back(line);
        }

        if (keyOrLockInfo.size() > 0)
        {
            if (state == KEY_STATE)
            {
                keys.push_back(keyOrLockInfo);
            }
            else if (state == LOCK_STATE)
            {
                // lock
                locks.push_back(keyOrLockInfo);
            }
        }

        for (auto lock : locks)
        {
            _locks.push_back(Lock(lock));
        }
        for (auto key : keys)
        {
            _keys.push_back(Key(key));
        }

        for (auto lock : _locks)
        {
            cout << "row, col " << lock._num_rows << "," << lock._num_cols << ": ";
            Utils::PrintVector(lock._heights);
        }
        for (auto key : _keys)
        {
            cout << "row, col " << key._num_rows << "," << key._num_cols << ": ";
            Utils::PrintVector(key._heights);
        }

        file.close();
    }

    int checkOverlap(Key &key, Lock &lock)
    {
        assert(key._num_cols == lock._num_cols);
        assert(key._num_rows == lock._num_rows);
        for (int col = 0; col < key._num_cols; col++)
        {
            int space = lock._num_rows - lock._heights[col];
            if (space <= key._heights[col])
            {
                return col;
            }
        }
        return -1;
    }

    void part1()
    {

        vector<glm::ivec2> matches;
        for (int lockIndex = 0; lockIndex < (int)_locks.size(); lockIndex++)
        {
            for (int keyIndex = 0; keyIndex < (int)_keys.size(); keyIndex++)
            {

                int col = checkOverlap(_keys[keyIndex], _locks[lockIndex]);
                if (col == -1)
                {
                    matches.push_back({keyIndex, lockIndex});
                }
            }
        }

        Utils::PrintVector(matches);
        cout << "Unique Combos " << matches.size() << endl;
    }

    void part2()
    {
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        part1();
        part2();
    }
};