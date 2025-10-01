#include <iostream>
#include "RBT.hpp"

int main()
{
    try
    {
        RBT::SearchTree<int> tree;
        // tree.insert(50);
        // tree.insert(30);
        // tree.gdump();
        // tree.insert(70);
        // tree.insert(20);
        // tree.insert(40);

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
        do {
            std::cin >> command;
            if (command == 'k')
            {
                int key = 0;
                std::cin >> key;
                tree.insert(key);
            }
            else if (command == 'q')
            {
                int key1 = 0; 
                int key2 = 0;
                std::cin >> key1 >> key2;
                results.push_back(RBT::range_query(tree, key1, key2));
            }
            // tree.erase(30);
        } while (command != 'o');

        for (int result : results) {
            std::cout << result << " " << std::endl;
        }
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}