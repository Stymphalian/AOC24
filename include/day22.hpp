#pragma once

#include "utils.hpp"

class Day22
{
public:
    vector<u64> _secrets;

    Day22() {}
    virtual ~Day22() {}

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day22/test.txt"
                               : "res/Day22/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            Utils::trim(line);
            _secrets.push_back(std::stoull(line));
        }
        file.close();

        Utils::PrintVector(_secrets);
        std::cout << std::endl;
    }

    inline u64 mix(u64 a, u64 b)
    {
        return a ^ b;
    }
    inline u64 prune(u64 num)
    {
        return num % 16777216;
    }
    u64 rng(u64 secret)
    {
        secret = prune(mix(secret, secret * 64));
        secret = prune(mix(secret, u64(secret / 32)));
        secret = prune(mix(secret, secret * 2048));
        return secret;
    }
    int price(u64 secret)
    {
        return (int)secret % 10;
    }

    void part1()
    {
        int num_iters = 2000;
        vector<u64> newSecrets;
        for (u64 secret : _secrets)
        {
            std::cout << secret << std::endl;
            for (int i = 0; i < num_iters; i++)
            {
                secret = rng(secret);
            }
            newSecrets.push_back(secret);
        }

        u64 sum = 0;
        for (u64 secret : newSecrets)
        {
            sum += secret;
        }
        std::cout << sum << std::endl;
    }

    struct Record
    {
        u64 secret;
        int price;
        int change;
        glm::ivec4 sequence;

        bool operator()(const Record &l, const Record &r)
        {
            return l.price < r.price;
        }
    };

    void PrintSecretsGrid(vector<vector<Record>> &grid)
    {

        ofstream file("output.txt");
        for (int i = 0; i < grid.size(); i++)
        {
            file << "Secret: " << grid[i][0].secret << std::endl;
            for (auto record : grid[i])
            {
                file << "  ";
                file << record.secret;
                file << " ";
                file << record.price;
                file << " (";
                file << record.change;
                file << ") [";
                file << record.sequence[0];
                file << " ";
                file << record.sequence[1];
                file << " ";
                file << record.sequence[2];
                file << " ";
                file << record.sequence[3];
                file << "]";
                file << std::endl;

                // printf("  %10ld: %3d (%3d) [%3d %3d %3d %3d]\n",
                //        record.secret, record.price, record.change, record.sequence[0], record.sequence[1], record.sequence[2], record.sequence[3]);
            }
            file << std::endl;
        }
    }

    int priceForSequence(vector<vector<Record>> &grid, int monkey, glm::ivec4 target)
    {

        int bestPrice = 0;
        for (int i = 4; i < grid[monkey].size(); i++)
        {
            if (grid[monkey][i].sequence == target)
            {
                if (grid[monkey][i].price > bestPrice)
                {
                    bestPrice = grid[monkey][i].price;
                }
            }
        }
        return bestPrice;
    }

    u64 solveInner(vector<vector<Record>> &grid, int monkey)
    {
        return 0;
    }
    u64 solve(vector<vector<Record>> &grid, int monkey)
    {
        return 0;
        // if (monkey >= grid.size()) {
        //     return 0;
        // }

        // u64 secret = grid[monkey][0].secret;
        // priority_queue<Record, vector<Record>, Record> pq;

        // for(int i = 4; i < grid[monkey].size(); i++) {
        //     pq.push(grid[monkey][i]);
        // }

        // int best = 0;
        // while(!pq.empty()) {
        //     Record record = pq.top();
        //     pq.pop();

        //     int bananas = record.price;
        //     int children = solveInner(grid, monkey + 1);

        //     if (bananas + children > best) {
        //         best = bananas + children;
        //     }
        // }

        // return best;
    }

    void part2()
    {
        int num_iters = 2001;
        vector<vector<Record>> secretsGrid;
        for (int i = 0; i < _secrets.size(); i++)
        {
            vector<Record> temp;
            temp.reserve(num_iters);
            secretsGrid.push_back(std::move(temp));
        }

        Dict<glm::ivec4, bool> uniqueSequences;

        u64 prevSecret = 0;
        for (int i = 0; i < _secrets.size(); i++)
        {
            u64 secret = _secrets[i];
            Record prev = {secret, price(secret), 0, {0, 0, 0, 0}};
            secretsGrid[i].push_back(prev);

            Record current;
            for (int j = 1; j < num_iters; j++)
            {
                secret = rng(secret);

                int currentPrice = price(secret);
                int change = currentPrice - prev.price;
                current = {
                    secret,
                    currentPrice,
                    change,
                    {prev.sequence[1], prev.sequence[2], prev.sequence[3], change}};

                if (j >= 4)
                {
                    uniqueSequences.insert({current.sequence, true});
                }
                secretsGrid[i].push_back(current);
                prev = current;
            };
        }
        // PrintSecretsGrid(secretsGrid);

        Dict<int, Dict<glm::ivec4, int>> cache;
        for (int monkey = 0; monkey < (int)_secrets.size(); monkey++)
        {
            cache[monkey] = Dict<glm::ivec4, int>();

            for (int i = 4; i < secretsGrid[monkey].size(); i++)
            {
                Record record = secretsGrid[monkey][i];
                if (!cache[monkey].contains(record.sequence))
                {
                    cache[monkey][record.sequence] = record.price;
                }
            }
        }

        int best = 0;
        for (auto seq : uniqueSequences)
        {
            int bananaCount = 0;
            for (int monkey = 0; monkey < (int)_secrets.size(); monkey++)
            {
                if (cache[monkey].contains(seq.first))
                {
                    bananaCount += cache[monkey][seq.first];
                }
            }

            if (bananaCount > best)
            {
                best = bananaCount;
            }
        }

        std::cout << "Answer: " << best << std::endl;
    }

    void Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1();
        part2(); //fail(1722)
    }
};