#pragma once

#include "utils.hpp"

class Day23
{
public:
    Day23() {}
    virtual ~Day23() {}

    Dict<string, Set<string>> _connections;
    Set<string> _computers;

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day23/test.txt"
                               : "res/Day23/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            vector<string> tokens = Utils::splitOld(line, "-");
            assert(tokens.size() == 2);
            string a = tokens[0];
            string b = tokens[1];
            if (!_connections.contains(a))
            {
                _connections[a] = {};
            }
            if (!_connections.contains(b))
            {
                _connections[b] = {};
            }
            _connections[a].insert(b);
            _connections[b].insert(a);

            _computers.insert(a);
            _computers.insert(b);
        }
        file.close();

        // for (auto &kv : _connections)
        // {
        //     string a = kv.first;
        //     std::cout << a << ": ";
        //     for (auto &b : kv.second)
        //     {
        //         std::cout << b << " ";
        //     }
        //     std::cout << std::endl;
        // }
    }

    vector<string> findLans(string a)
    {
        vector<string> lans;

        for (auto &b : _connections[a])
        {
            vector<string> ns;
            ns.insert(ns.end(), _connections[a].begin(), _connections[a].end());
            ns.insert(ns.end(), _connections[b].begin(), _connections[b].end());

            for (auto &c : ns)
            {
                if (c == a || c == b)
                {
                    continue;
                }

                if (_connections[c].contains(a) && _connections[c].contains(b))
                {
                    assert(_connections[a].contains(c));
                    assert(_connections[b].contains(c));
                    vector<string> path = {a, b, c};
                    std::sort(path.begin(), path.end());
                    lans.push_back(Utils::join(path, ","));
                }
            }
        }

        return lans;
    }

    bool containsTComputers(string lan, vector<string> &tComputers)
    {
        for (auto &c : tComputers)
        {
            if (lan.find(c) != string::npos)
            {
                return true;
            }
        }
        return false;
    }

    void part1()
    {
        std::set<string> allLans;
        for (auto &c : _computers)
        {
            auto lans = findLans(c);

            for (auto &lan : lans)
            {
                allLans.insert(lan);
            }
        }

        vector<string> tComputers;
        for (auto &c : _computers)
        {
            if (c.starts_with("t"))
            {
                tComputers.push_back(c);
            }
        }

        // std::cout << "Count T computer "<< std::endl;
        int numComputers = 0;
        for (auto lan : allLans)
        {
            if (containsTComputers(lan, tComputers))
            {
                // std::cout << lan << std::endl;
                numComputers++;
            };
        }

        std::cout << allLans.size() << std::endl;
        for (auto &lan : allLans)
        {
            std::cout << lan.length() << ": " << lan << std::endl;
        }
        std::cout << numComputers << std::endl;
    }

    bool isConnected(string b, Set<string> &lan)
    {
        for (string a : lan)
        {
            if (!_connections[a].contains(b))
            {
                return false;
            }
        }
        return true;
    }

    vector<string> findPaths(string a, int maxRank)
    {
        Set<string> visited;
        vector<tuple<string, Set<string>>> stack;
        stack.push_back({a, {}});

        vector<Set<string>> allLans = {};
        while (!stack.empty())
        {
            auto [b, lan] = stack.back();
            stack.pop_back();
            visited.insert(b);

            if ((int)lan.size() == maxRank)
            {
                allLans.push_back(lan);
                continue;
            }

            for (auto n : _connections[b])
            {
                if (visited.contains(n))
                {
                    continue;
                }

                if (isConnected(n, lan))
                {
                    Set<string> newLan = lan;
                    newLan.insert(n);
                    stack.push_back({n, newLan});
                }
            }
        }

        vector<string> printable;
        for (auto lan : allLans)
        {
            vector<string> as;
            for (auto a : lan)
            {
                as.push_back(a);
            }
            std::sort(as.begin(), as.end());
            printable.push_back(Utils::join(as, ","));
        }
        return printable;
    }

    template <class T>
    string SetToString(T lan)
    {
        vector<string> as;
        for (auto a : lan)
        {
            as.push_back(a);
        }
        std::sort(as.begin(), as.end());
        return Utils::join(as, ",");
    }

    void part2()
    {
        int maxRank = 0;
        for (auto &c : _computers)
        {
            maxRank = std::max(maxRank, (int)_connections[c].size());
        }

        OrdSet<string> uniqueLans;
        for (auto c : _computers)
        {
            vector<string> paths = findPaths(c, maxRank);
            uniqueLans.insert(paths.begin(), paths.end());
        }

        for (auto path : uniqueLans)
        {
            std::cout << path << std::endl;
        }
        std::cout << uniqueLans.size() << std::endl;
    }

    template <class T>
    void PrintSet(T &lan)
    {
        for (auto a : lan)
        {
            std::cout << a << " ";
        }
        std::cout << std::endl;
    }

    OrdSet<string> solve(string a)
    {
        OrdSet<string> best;

        OrdSet<string> A;
        A.insert(_connections[a].begin(), _connections[a].end());
        A.insert(a);

        for (auto b : _connections[a])
        {
            OrdSet<string> B;
            B.insert(_connections[b].begin(), _connections[b].end());
            B.insert(b);

            OrdSet<string> common;
            std::set_intersection(
                A.begin(), A.end(),
                B.begin(), B.end(),
                std::inserter(common, common.end()));

            if (common.size() > best.size())
            {
                best = common;
            }
        }

        return best;
    }

    // https://www.reddit.com/r/adventofcode/comments/1hkgj5b/comment/m3eotca/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
    void part3()
    {

        Dict<OrdSet<string>, int, Utils::OrdSetCompare> lanCounts;
        for (auto c : _computers)
        {
            auto lan = solve(c);
            lanCounts[lan] += 1;
        }

        OrdSet<string> best;
        for (auto [lan, count] : lanCounts)
        {
            if (count != (int) lan.size())
            {
                // Not a clique
                continue;
            }
            if (lan.size() > best.size())
            {
                best = lan;
            }
        }
        std::cout << SetToString(best) << std::endl;
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // real(1194)
        // part2(); // real(bd,bu,dv,gl,qc,rn,so,tm,wf,yl,ys,ze,zr)
        part3();
    }
};