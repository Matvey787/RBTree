#include <iostream>
#include "RBT.hpp"

int main()
{
    try
    {
        RBT::RBTree<int> tree;
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

    std::vector<int> results;
    char command;
    while (std::cin >> command) {
        if (command == 'k') {
            int key;
            if (std::cin >> key) {
                tree.insert(key);
            }
        } else if (command == 'q') {
            int key1 = 0;
            int key2 = 0;
            if (std::cin >> key1 >> key2) {
                results.push_back(RBT::range_query(tree, key1, key2));

            
            }
        }
    }
    for (size_t i = 0; i < results.size(); ++i)
    {
        std::cout << results[i] << (i == results.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}