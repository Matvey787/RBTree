#include <iostream>
#include <fstream>
#include <string>
#include <random>

int main(int argc, char* argv[])
{
    std::string fileWriteTo = "tests/performanceTest.dat";
    size_t numberOfCommands = 100000;
    if (argc >= 2)
    {
        fileWriteTo = argv[1];
        if (argc >= 3) numberOfCommands = std::stoul(argv[2]);
    }
    std::ofstream file(fileWriteTo);
    if (!file)
    {
        std::cerr << "Error opening file: " << fileWriteTo << std::endl;
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr)));

    for (size_t i = 0; i < numberOfCommands; ++i)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> command(1, 2);
        std::uniform_int_distribution<std::mt19937::result_type> value1(1, numberOfCommands / 2 - 1);
        std::uniform_int_distribution<std::mt19937::result_type> value2(numberOfCommands / 2, numberOfCommands);

        if (command(rng) == 1) file << "k " << value1(rng) << " ";
        else file << "q " << value1(rng) << " " << value2(rng) << " ";
    }
    return 0;
}