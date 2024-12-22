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

        // Utils::PrintVector(_secrets);
        // std::cout << std::endl;
    }

    inline u64 mix(u64 a, u64 b)
    {
        return a ^ b;
    }
    inline u64 prune(u64 num)
    {
        return num % 16777216;
    }
    inline u64 rng(u64 secret)
    {
        secret = prune(mix(secret, secret * 64));
        secret = prune(mix(secret, u64(secret / 32)));
        secret = prune(mix(secret, secret * 2048));
        return secret;
    }
    inline int price(u64 secret)
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
        int sequence_base19 = 0;

        bool operator()(const Record &l, const Record &r)
        {
            return l.price < r.price;
        }
    };

    void PrintSecretsGrid(vector<vector<Record>> &grid)
    {
        ofstream file("output.txt");
        for (int i = 0; i < (int) grid.size(); i++)
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
                // file << record.sequence[0];
                // file << " ";
                // file << record.sequence[1];
                // file << " ";
                // file << record.sequence[2];
                // file << " ";
                // file << record.sequence[3];
                file << "]";
                file << std::endl;

                // printf("  %10ld: %3d (%3d) [%3d %3d %3d %3d]\n",
                //        record.secret, record.price, record.change, record.sequence[0], record.sequence[1], record.sequence[2], record.sequence[3]);
            }
            file << std::endl;
        }
    }


    // Learning:
    // https://github.com/UnicycleBloke/aoc2024/blob/main/day22/solution.cpp
    void part2Optimal()
    {

        int num_iters = 2000;

        const int BASE_19_CAP = 19 * 19 * 19 * 19;
        const int BASE_19 = 19;
        std::array<int, BASE_19_CAP> seen;
        std::array<int, BASE_19_CAP> prices;
        for (int i = 0; i < (int) _secrets.size(); i++)
        {
            const int monkey = _secrets[i];

            u64 secret = _secrets[i];
            Record prev = {secret, price(secret), 0, {0, 0, 0, 0}, 0};
            Record current;
            for (int j = 1; j <= num_iters; j++)
            {
                secret = rng(secret);

                int currentPrice = price(secret);
                int change = currentPrice - prev.price;
                current = {
                    secret,
                    currentPrice,
                    change,
                    {0, 0, 0, 0},
                    ((prev.sequence_base19*BASE_19) + (change + 9)) % (BASE_19_CAP)
                };
                if (j >= 4 && seen[current.sequence_base19] != monkey) {
                    seen[current.sequence_base19] = monkey;
                    prices[current.sequence_base19] += current.price;
                }
                prev = current;
            };
        }

        int maxBananas = *std::max_element(prices.begin(), prices.end());
        std::cout << "Max Bananas: " << maxBananas << std::endl;
    }

    void part2()
    {

        int num_iters = 2000;

        Set<glm::ivec4> seen;
        Dict<glm::ivec4, int> prices;
        for (int i = 0; i < (int) _secrets.size(); i++)
        {
            seen.clear();

            u64 secret = _secrets[i];
            Record prev = {secret, price(secret), 0, {0, 0, 0, 0}};
            Record current;
            for (int j = 1; j <= num_iters; j++)
            {
                secret = rng(secret);

                int currentPrice = price(secret);
                int change = currentPrice - prev.price;
                current = {
                    secret,
                    currentPrice,
                    change,
                    {prev.sequence[1], prev.sequence[2], prev.sequence[3], change}};

                if (j >=4 && !seen.contains(current.sequence)) {
                    seen.insert(current.sequence);
                    prices[current.sequence] += current.price;
                }
                prev = current;
            };
        }

        int maxBananas = 0;
        for (auto [seq, bananaCount] : prices)
        {
            if (bananaCount > maxBananas)
            {
                maxBananas = bananaCount;
            }
        }

        std::cout << "Max Bananas: " << maxBananas << std::endl;
    }

    void Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1();
        part2(); //fail(1722), real(1717)
    }
};