#include <iostream>
#include "RBT.hpp"

int main()
{
    try
    {
        RBT::SearchTree<int> tree;
        tree.insert(50);
        tree.insert(30);
        tree.gdump();
        tree.insert(70);
        tree.insert(20);
        tree.insert(40);

        tree.gdump();
        tree.insert(45);
        tree.gdump();
        tree.insert(10);
        tree.gdump();
        tree.insert(5);
        tree.gdump();
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}