#include <iostream>
#include <vector>
#define PRINT(x) std::cout << x;

int main(int argc, char *argv[])
{
    //Minimum number of arguments is: 2 (executable name and dataset directory)
    //Valid number of arguments is 2,4,6 or 8

    if (argc == 2 || argc == 4 || argc == 6 || argc == 8)
    {
        std::string dataset;
        std::string output;
        int n;
        int b;

        dataset = argv[1];

        if (argc > 2)
        {
            for (int i = 2; i < argc; i++)
            {

                if (std::string(argv[i]).compare("-o") == 0)
                {
                    output = argv[i + 1];
                }
                else if (std::string(argv[i]).compare("-k") == 0)
                {
                    try
                    {
                        n = std::stoi(argv[i + 1]);
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
                else if (std::string(argv[i]).compare("-bin") == 0)
                {
                    try
                    {
                        b = std::stoi(argv[i + 1]);
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
            }
        }

        PRINT(dataset + "\n");
        PRINT(output + "\n");
        PRINT(std::to_string(n) + "\n");
        PRINT(std::to_string(b) + "\n");

        /*std::vector<std::string> arguments;

        for (int i = 0; i < argc; i++)
        {
            arguments.push_back(argv[i]);
        }*/
    }
    else
    {
        //Number of arguments is invalid
    }
}