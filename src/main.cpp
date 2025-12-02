#ifdef SET_M
#include <set>
#include <iterator>
#else
#include "RBT.hpp"
#endif

#include <iostream>
#include <vector>
#include <limits>
#include <sstream>

template <typename T>
bool getInput(T& var, std::string_view warningMessage = "");

int main() try {
    #ifdef SET_M
        std::set<int> tree;
    #else
        RBT::RBTree<int> tree;
    #endif

    std::vector<int> results;
    char command = 0;
    while (getInput(command))
    {
        if (command == 'k') {
            int key = 0;
            getInput(key, "need number");
            tree.insert(key);

        } else if (command == 'q') {
            int key1 = 0;
            int key2 = 0;
            getInput(key1, "need number");
            getInput(key2, "need number");

            #ifdef SET_M
                if (key1 > key2)
                {
                    results.push_back(0);
                    continue;
                }
                auto it1 = tree.lower_bound(key1);
                auto it2 = tree.upper_bound(key2);
                size_t numOfNodes = std::distance(it1, it2);
                results.push_back(numOfNodes);
            #else
                size_t numOfNodes = RBT::range_query(tree, key1, key2);
                results.push_back(numOfNodes);
            #endif
        }
        else
        {
            throw std::runtime_error("Input (k) - insert key; (q) - want to query");
        }
    }
    for (size_t i = 0; i < results.size(); ++i)
    {
        std::cout << results[i] << (i == results.size() - 1 ? "" : " ");
    }
    std::cout << '\n';

    return 0;
}
catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return EXIT_FAILURE;
}

template <typename T> bool getInput(T& var, std::string_view warningMessage)
{
    size_t attempt = 0;
    for (; attempt < 3u; ++attempt)
    {
        std::cin >> std::ws >>  var;
        if (std::cin.eof())
        {
            return false; 
        }
        if (std::cin.good()) { break; }

        std::cout << "Invalid input! Try again";
        if (!warningMessage.empty())
            std::cout << " (" << warningMessage << ")"; std::cout << ":\n";
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (attempt == 3) throw std::runtime_error("Max attempts(3) reached. Exiting...");
    
    return true;
}
