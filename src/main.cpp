#ifdef SET_M
#include <set>
#include <iterator>
#else
#include "RBT.hpp"
#endif

#include <iostream>
#include <vector>

void getNumber(int* num1 = nullptr);

int main()
{
    try
    {
    #ifdef SET_M
        std::set<int> tree;
    #else
        RBT::RBTree<int> tree;
    #endif

    std::vector<int> results;
    char command = 0;
    while (std::cin >> command)
    {
        if (command == 'k') {
            int key = 0;
            getNumber(&key);
            tree.insert(key);

        } else if (command == 'q') {
            int key1 = 0;
            int key2 = 0;
            getNumber(&key1);
            getNumber(&key2);

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
    }
    for (size_t i = 0; i < results.size(); ++i)
    {
        std::cout << results[i] << (i == results.size() - 1 ? "" : " ");
    }
    std::cout << "\n";
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

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

void getNumber(int* num1 = nullptr)
{
    if (num1) std::cin >> *num1;

    const size_t attempts = 3;
    while (!std::cin.good())
    {
        if (attempts == 0)
        {
            std::cerr << "Max attempts(3) reached. Exiting...\n";
            std::exit(1);
        }
        std::cerr << "Invalid input! " << "Try again: \n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (num1) std::cin >> *num1;
    }
}