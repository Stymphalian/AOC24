#pragma once

#include "utils.hpp"

class Day23
{
public:
    Day23() {}
    virtual ~Day23() {}

    // struct Connection {
    //     string a;
    //     string b;
    // };
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

        for (auto &kv : _connections)
        {
            string a = kv.first;
            std::cout << a << ": ";
            for (auto &b : kv.second)
            {
                std::cout << b << " ";
            }
            std::cout << std::endl;
        }
    }

    struct Lan
    {
        vector<string> path;

        Lan() {}
        Lan(vector<string> path) : path(path) {}

        bool operator==(const Lan &other) const
        {
            if (path.size() != other.path.size())
            {
                return false;
            }
            for (size_t i = 0; i < path.size(); i++)
            {
                if (path[i] != other.path[i])
                {
                    return false;
                }
            }
            return true;
        }

        Lan addPath(string a)
        {
            Lan lan = *this;
            lan.path.push_back(a);
            std::sort(lan.path.begin(), lan.path.end());
            return lan;
        }
    };

    vector<Lan> findLans2(string a)
    {
        vector<Lan> lans;

        Set<string> visited;
        vector<tuple<string, int, Lan>> stack;
        stack.push_back({a, 3, Lan({a})});
        while (!stack.empty())
        {
            tuple<string, int, Lan> cs = stack.back();
            stack.pop_back();
            string current = get<0>(cs);
            int depth = get<1>(cs);
            Lan lan = get<2>(cs);

            if (visited.find(current) != visited.end())
            {
                continue;
            }
            if (depth == 1)
            {
                std::sort(lan.path.begin(), lan.path.end());
                lans.push_back(lan);
                continue;
            }
            visited.insert(current);

            for (auto n : _connections[current])
            {
                if (visited.find(n) != visited.end())
                {
                    continue;
                }
                if (depth - 1 <= 0)
                {
                    continue;
                }
                stack.push_back({n, depth - 1, lan.addPath(n)});
            }
        }
        return lans;
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

    using LanGroup = std::set<string>;

    struct LanGroupCompare
    {
        std::size_t operator()(const LanGroup &v) const
        {
            std::size_t h = 0;
            for (auto a : v)
            {
                Utils::hash_combine(h, a);
            }
            return h;
        }

        bool operator()(const LanGroup &a, const LanGroup &b) const
        {
            return a == b;
        }
    };

    bool isFullyConnected(LanGroup as, LanGroup bs)
    {
        for (auto &a : as)
        {
            for (auto &b : bs)
            {
                if (a == b)
                {
                    continue;
                }
                if (!_connections[a].contains(b))
                {
                    return false;
                }
            }
        }
        return true;
    }

    tuple<LanGroup, int> joinGroups(LanGroup a, LanGroup b)
    {
        LanGroup next;
        next.insert(a.begin(), a.end());
        next.insert(b.begin(), b.end());
        return {next, next.size()};
    }

    void part2_bad()
    {
        int rank = 1;
        int maxRank = 0;
        vector<LanGroup> groups;
        for (auto &c : _computers)
        {
            groups.push_back({c});
            maxRank = std::max(maxRank, (int)_connections[c].size());
        }
        std::cout << "Max rank: " << maxRank << std::endl;

        while (rank < maxRank)
        {
            Set<LanGroup, LanGroupCompare, LanGroupCompare> nextGroup;
            printf("Computing rank %d with groups size %d\n", rank, (int)groups.size());

            for (size_t i = 0; i < groups.size(); i++)
            {
                for (size_t j = 0; j < groups.size(); j++)
                {
                    if (i == j)
                    {
                        continue;
                    }

                    bool connected = isFullyConnected(groups[i], groups[j]);
                    if (connected)
                    {
                        auto [next, nextRank] = joinGroups(groups[i], groups[j]);
                        if (nextRank >= rank)
                        {
                            nextGroup.insert(next);
                        }
                    }
                }
            }
            if (nextGroup.size() <= 0)
            {
                break;
            }

            groups.clear();
            groups.insert(groups.end(), nextGroup.begin(), nextGroup.end());
            rank += 1;
        }

        vector<string> printable;
        for (auto group : groups)
        {
            vector<string> as;
            for (auto a : group)
            {
                as.push_back(a);
            }
            std::sort(as.begin(), as.end());
            printable.push_back(Utils::join(as, ","));
        }
        std::sort(printable.begin(), printable.end());

        std::cout << "Count: " << printable.size() << std::endl;
        for (auto lan : printable)
        {
            std::cout << lan << std::endl;
        }
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

    vector<string> findPath(string a, int maxRank)
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

            if ((int) lan.size() == maxRank)
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

                // isFullyConnected
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
        std::sort(printable.begin(), printable.end());
        return printable;
    }

    void part2()
    {
        int maxRank = 0;
        for (auto &c : _computers)
        {
            maxRank = std::max(maxRank, (int)_connections[c].size());
        }

        std::set<string> uniqueLans;
        for (auto c : _computers)
        {
            vector<string> paths = findPath(c, maxRank);
            uniqueLans.insert(paths.begin(), paths.end());
        }

        for (auto path : uniqueLans)
        {
            std::cout << path << std::endl;
        }
    }

    void
    Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // real(1194)
        part2();
    }
};