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
bool getInput(T& var = nullptr, const std::string& warningMessage = "");

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
    std::cout << "\n";

    // tree.insert(10);
    // tree.insert(20);
    // tree.gdump();
    // tree.insert(30);
    // tree.gdump();
    // tree.insert(45);
    // tree.gdump();
    // tree.insert(8);
    // tree.insert(40);
    // tree.insert(100);
    // tree.insert(12);
    // tree.insert(15);
    // tree.insert(25);
    // tree.insert(1000);
    // tree.insert(10000);
    // tree.gdump();
    // 
    // tree.gdump();
    // tree.insert(45);
    // tree.gdump();
    // tree.insert(10);
    // tree.gdump();
    // tree.insert(5);
    // tree.gdump();
    // FIXME НЕ ВЫЗЫВАЙТЕ УДАЛЕНИЕ, ОНО РАБОТАЕТ БЕЗ БАЛАНСИРОВКИ, ЭТО УДАЛЕНИЕ В SBT, А НЕ В 
    // RBT !!!!!
    // // tree.erase(30); 
    // tree.gdump();

    return 0;
}
catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
}

template <typename T> bool getInput(T& var, const std::string& warningMessage)
{ 
    size_t attempts = 3;
    while (true)
    {
        std::cin >> std::ws >>  var;
        if (std::cin.eof())
        {
            return false; 
        }
        if (std::cin.good()) { break; }

        --attempts;

        if (attempts == 0)
        {
            throw std::runtime_error("Max attempts(3) reached. Exiting...");
        }

        std::cout << "Invalid input! Try again";
        if (!warningMessage.empty())
            std::cout << " (" << warningMessage << ")"; std::cout << ":\n";
    }
    return true;
}
