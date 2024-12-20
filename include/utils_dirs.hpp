#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>
#include <string>
#include <regex>
#include <iterator>
#include <list>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <cmath>
#include <tuple>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <sstream>
#include <optional>
#include <cctype>
#include <locale>
#include <cassert>
#include <chrono>

namespace Utils
{
    glm::ivec2 VRIGHT = glm::ivec2(1, 0);
    glm::ivec2 VDOWN = glm::ivec2(0, 1);
    glm::ivec2 VLEFT = glm::ivec2(-1, 0);
    glm::ivec2 VUP = glm::ivec2(0, -1);
    inline glm::ivec2 turnCW(glm::ivec2 dir)
    {
        if (dir == VRIGHT)
        {
            return VDOWN;
        }
        else if (dir == VDOWN)
        {
            return VLEFT;
        }
        else if (dir == VLEFT)
        {
            return VUP;
        }
        else
        { // if (dir == VUP) {
            return VRIGHT;
        }
    }
    inline glm::ivec2 turnCCW(glm::ivec2 dir)
    {
        if (dir == VRIGHT)
        {
            return VUP;
        }
        else if (dir == VUP)
        {
            return VLEFT;
        }
        else if (dir == VLEFT)
        {
            return VDOWN;
        }
        else
        { // if (dir == VDOWN) {
            return VRIGHT;
        }
    }

    std::vector<glm::ivec2> DIRS = {
        glm::ivec2(1, 0),  // right
        glm::ivec2(0, 1),  // down
        glm::ivec2(-1, 0), // left
        glm::ivec2(0, -1)  // up
    };

    inline int turnDirBack(int dir)
    {
        return (dir + 2) % 4;
    }
    inline int turnDirRight(int dir)
    {
        return (dir + 1) % 4;
    }
    inline int turnDirLeft(int dir)
    {
        return ((dir + 4) - 1) % 4;
    }

    class CardinalDirectionIterator
    {
    public:
        glm::ivec2 _start;
        CardinalDirectionIterator(glm::ivec2 start) : _start(start) {}

        class iterator
        {
        private:
            int _index = 0;
            glm::ivec2 _start;
            glm::ivec2 _current;

        public:
            // using value_type = glm::ivec2;
            // using difference_type = std::ptrdiff_t;
            // using pointer = glm::ivec2 *;
            using reference = glm::ivec2 &;
            using iterator_category = std::input_iterator_tag;

            explicit iterator(int index, glm::ivec2 start)
                : _index(index),
                  _start(start)
            {
                _current = start;
                if (_index < 4)
                {
                    _current = _start + DIRS[_index];
                }
            }

            iterator &operator++()
            {
                _index += 1;
                if (_index < 4)
                {
                    _current = _start + DIRS[_index];
                }
                return *this;
            }

            iterator operator++(int)
            {
                iterator retval = *this;
                ++(*this);
                return retval;
            }
            bool operator==(iterator other) const
            {
                return _index == other._index;
            }
            bool operator!=(iterator other) const
            {
                return !(*this == other);
            }
            reference operator*() { return _current; }
        };

        iterator begin() { return iterator(0, _start); }
        iterator end() { return iterator(4, _start); }
    };

    std::vector<glm::ivec2> forEachDir(glm::ivec2 pos)
    {
        return {
            pos + VRIGHT,
            pos + VDOWN,
            pos + VLEFT,
            pos + VUP,
        };
    }

    CardinalDirectionIterator forEachDir3(glm::ivec2 pos)
    {
        return CardinalDirectionIterator(pos);
    }

    CardinalDirectionIterator forEachDir3()
    {
        return CardinalDirectionIterator(glm::ivec2(0, 0));
    }

};
