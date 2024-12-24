#pragma once

#include "utils.hpp"

class Day24
{
public:
    Day24() {}
    virtual ~Day24() {}

    const int AND = 0;
    const int OR = 1;
    const int XOR = 2;
    struct Equation
    {
        string a;
        string b;
        string c;
        int op;

        int evaluate(int a, int b)
        {
            switch (op)
            {
            case 0:
                return a & b;
            case 1:
                return a | b;
            case 2:
                return a ^ b;
            default:
                assert(false);
            };
        }

        bool hasInputs(OrdDict<string, int> &inputs)
        {
            return inputs.contains(a) && inputs.contains(b);
        }

        void swap(Equation& other) {
            string tempA = a;
            string tempB = b;
            int tempOp = op;
            this->a = other.a;
            this->b = other.b;
            this->op = other.op;
            other.a = tempA;
            other.b = tempB;
            other.op = tempOp;
        }
    };

    OrdDict<string, int> _inputs;
    vector<Equation> _equations;
    Dict<string, Equation> _wireToEquation;
    Dict<string, OrdSet<string>> _transCache;
    vector<string> _xinput;
    vector<string> _yinput;
    vector<string> _zinput;
    int _maxBits = 0;

    void ReadInput(bool readTest = false)
    {
        std::ifstream file(readTest
                               ? "res/Day24/test3.txt"
                               : "res/Day24/input.txt");

        if (file.is_open() == false)
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }

        std::string line;
        int state = 0;
        while (std::getline(file, line))
        {
            Utils::trim(line);

            if (line.empty())
            {
                state = 1;
                continue;
            }
            if (state == 0)
            {
                auto tokens = Utils::split(line, ":");
                assert(tokens.size() == 2);
                string a = Utils::trim_copy(tokens[0]);
                int b = std::stoi(Utils::trim_copy(tokens[1]));
                _inputs[a] = b;
            }
            else
            {
                auto tokens = Utils::split(line, "->");
                assert(tokens.size() == 2);

                string eq = Utils::trim_copy(tokens[0]);

                string a;
                string b;
                int op;
                if (eq.find("AND") != string::npos)
                {
                    op = AND;
                    auto tk = Utils::split(eq, "AND");
                    assert(tk.size() == 2);
                    a = Utils::trim_copy(tk[0]);
                    b = Utils::trim_copy(tk[1]);
                }
                else if (eq.find("XOR") != string::npos)
                {
                    op = XOR;
                    auto tk = Utils::split(eq, "XOR");
                    assert(tk.size() == 2);
                    a = Utils::trim_copy(tk[0]);
                    b = Utils::trim_copy(tk[1]);
                }
                else if (eq.find("OR") != string::npos)
                {
                    op = OR;
                    auto tk = Utils::split(eq, "OR");
                    assert(tk.size() == 2);
                    a = Utils::trim_copy(tk[0]);
                    b = Utils::trim_copy(tk[1]);
                }
                else
                {
                    assert(false);
                }

                string c = Utils::trim_copy(tokens[1]);
                Equation equation(a, b, c, op);
                _equations.push_back(equation);
                M_Assert(!_wireToEquation.contains(c), "Need multimap");
                _wireToEquation.insert({c, equation});
            }
        }


        file.close();
        _xinput = Find('x');
        _yinput = Find('y');
        _zinput = Find('z');
        _maxBits = _zinput.size();

        // for (auto [k, v] : _inputs)
        // {
        //     cout << k << ": " << v << endl;
        // }
        // for (auto &eq : _equations)
        // {
        //     cout << eq.a << " " << eq.op << " " << eq.b << " -> " << eq.c << endl;
        // }
    }

    void PrintInputs()
    {
        for (auto [k, v] : _inputs)
        {
            cout << k << ": " << v << endl;
        }
    }

    vector<string> Find(char t)
    {
        vector<string> zValues;
        for (auto [k, v] : _inputs)
        {
            if (k[0] == t)
            {
                zValues.push_back(k);
            }
        }
        for (auto eq : _equations)
        {
            if (eq.c[0] == t)
            {
                zValues.push_back(eq.c);
            }
        }
        std::sort(zValues.begin(), zValues.end());
        return zValues;
    }

    bitset<64> ToBitSet(vector<string> &zValues, OrdDict<string, int> &cache)
    {
        bitset<64> num;
        for (size_t i = 0; i < zValues.size(); i++)
        {
            num.set(i, cache[zValues[i]]);
        }
        return num;
    }

    bitset<64> AddBitSets(const bitset<64> &a, const bitset<64> &b)
    {
        std::bitset<64> result;
        bool carry = false;

        // Iterate over all 64 bits (assuming 64-bit bitset for simplicity)
        for (size_t i = 0; i < (size_t)_maxBits; ++i)
        {
            // Calculate the sum of the corresponding bits and the carry
            bool bit_a = a[i];
            bool bit_b = b[i];

            bool sum = bit_a ^ bit_b ^ carry;                       // XOR for sum without carry
            carry = (bit_a && bit_b) || (carry && (bit_a ^ bit_b)); // Calculate new carry

            result[i] = sum;
        }

        // Return the resulting bitset (which already contains the carry bits handled)
        return result;
    }

    int computeTrans(
        string t,
        Dict<string, Equation> &wireToEquation,
        Set<string> &visited,
        OrdDict<string, int> &cache)
    {
        if (cache.contains(t))
        {
            return cache[t];
        }
        Equation &eq = wireToEquation.at(t);
        // std::cout << eq.a << " " << eq.op << " " << eq.b << " -> " << eq.c << std::endl;

        if (visited.contains(eq.a))
        {
            // M_Assert(false, "cycle found with a");
            return false;
        }
        if (visited.contains(eq.b))
        {
            // M_Assert(false, "cycle found with b");
            return false;
        }
        visited.insert(t);
        int a1 = computeTrans(eq.a, wireToEquation, visited, cache);
        int b1 = computeTrans(eq.b, wireToEquation, visited, cache);
        int c1 = eq.evaluate(a1, b1);
        cache[t] = c1;
        return c1;
    }

    std::bitset<64> computeZ()
    {
        OrdDict<string, int> cache = _inputs;
        for (auto z : _zinput)
        {
            Set<string> visited;
            computeTrans(z, _wireToEquation, visited, cache);
        }
        bitset<64> gotZ = ToBitSet(_zinput, cache);
        return gotZ;
    }

    OrdSet<string> getTransitiveSet(
        string target,
        Dict<string, OrdSet<string>> &cache)
    {
        if (cache.contains(target))
        {
            return cache[target];
        }

        Equation &eq = _wireToEquation[target];
        cache[target] = {};
        auto setA = getTransitiveSet(eq.a, cache);
        auto setB = getTransitiveSet(eq.b, cache);
        OrdSet<string> setC;
        std::set_union(
            setA.begin(), setA.end(),
            setB.begin(), setB.end(),
            std::inserter(setC, setC.begin()));
        if (eq.a[0] != 'y' && eq.a[0] != 'x')
        {
            setC.insert(eq.a);
        }
        if (eq.b[0] != 'y' && eq.b[0] != 'x')
        {
            setC.insert(eq.b);
        }

        cache[target] = setC;
        return setC;
    }

    void part1()
    {
        // compute();
        // PrintInputs();
        // Print('z');
    }

    vector<int> getBitDifferences(const bitset<64> &target, const bitset<64> &got)
    {
        vector<int> diffs;
        for (int i = 0; i < 64; i++)
        {
            if (target[i] != got[i])
            {
                diffs.push_back(i);
            }
        }
        return diffs;
    }

    std::string zToString(int z)
    {
        if (z < 10)
        {
            return "z0" + std::to_string(z);
        }
        else
        {
            return "z" + std::to_string(z);
        }
    }

    template <class T>
    void PrintSet(T &set)
    {
        for (auto s : set)
        {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }

    int strToInt(string z)
    {
        std::string sub = z.substr(1);
        int zInt = std::atoi(sub.c_str());
        return zInt;
    }

    tuple<bool, string, string> fixEquations(
        string z, 
        bitset<64> targetZ, 
        OrdSet<string> &search)
    {
        int zInt = strToInt(z);
        for (auto a : search)
        {
            for (auto b : search)
            {
                if (a == b)
                {
                    continue;
                }

                // try swapping and solving
                _wireToEquation[a].swap(_wireToEquation[b]);
                bitset<64> got = computeZ();
                _wireToEquation[a].swap(_wireToEquation[b]);

                bool allGood = true;
                for (int i = 0; i <= zInt; i++)
                {
                    if (got[i] != targetZ[i])
                    {
                        allGood = false;
                        break;
                    }
                }
                if (allGood)
                {
                    _wireToEquation[a].swap(_wireToEquation[b]);
                    return {true, a, b};
                }
            }
        }
        return {false, "", ""};
    }

    void part2()
    {
        bitset<64> xBits = ToBitSet(_xinput, _inputs);
        bitset<64> yBits = ToBitSet(_yinput, _inputs);
        bitset<64> targetZ = AddBitSets(xBits, yBits);
        bitset<64> gotZ = computeZ();
        // u64 targetZNum = targetZ.to_ullong();
        cout << "Got Z: " << gotZ.to_ullong() << endl;
        cout << "Target Z: " << targetZ.to_ullong() << endl;

        vector<int> bitDiffs = getBitDifferences(targetZ, gotZ);
        cout << "Bit Diffs ";
        Utils::PrintVector(bitDiffs);

        for (int i = 0; i < 64; i++)
        {
            std::cout << i << " " << targetZ[i] << ": " << gotZ[i] << std::endl;
        }

        _wireToEquation["ggn"].swap(_wireToEquation["z10"]);
        _wireToEquation["jcb"].swap(_wireToEquation["ndw"]);
        _wireToEquation["z32"].swap(_wireToEquation["grm"]);
        _wireToEquation["z39"].swap(_wireToEquation["twr"]);
            // "ggn", "z10",
            // "ndw", "jcb",
            // "z32", "grm",
            // "z39", "twr",

        OrdSet<string> visited;
        OrdSet<string> search;
        Dict<string, OrdSet<string>> cache;
        vector<string> candidates;
        for (auto z : _zinput)
        {
            cache.clear();
            int zInt = strToInt(z);
            
            search.insert(z);
            for (auto dep: getTransitiveSet(z, cache)) {
                if (!visited.contains(dep)) {
                    search.insert(dep);
                }
            }
            std::cout << z << ":" << search.size() << "  ";
            PrintSet(search);
            
            if (targetZ[zInt] != gotZ[zInt]) {
                auto [fixed, a, b] = fixEquations(z, targetZ, search);
                if (fixed) {
                    candidates.push_back(a);
                    candidates.push_back(b);
                    visited.insert(search.begin(), search.end());
                }
            } else {
                visited.insert(search.begin(), search.end());
                search.clear();
            }
        }

        // ggn z10
        // ndw, jcb
        // z32, grm
        // z39, twr

        // std::vector<string> sorted = {
        //     "ggn", "z10",
        //     "ndw", "jcb",
        //     "z32", "grm",
        //     "z39", "twr",
        // };
        // std::sort(sorted.begin(), sorted.end());
        // Utils::PrintVector(sorted);
        // std::cout << Utils::join(sorted, ",")<< std::endl;

        std::bitset<64> gotZFinal = computeZ();
        std::cout << "Got Z: " << gotZFinal.to_ullong() << endl;
        std::cout << "Target Z: " << targetZ.to_ullong() << endl;

        // Utils::PrintVector(candidates);
    }

    void Run()
    {
        bool readTest = false;
        ReadInput(readTest);
        // part1(); // real(54715147844840)
        part2();
    }
};