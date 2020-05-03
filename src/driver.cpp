#include <iostream>
#include <vector>
#include "../include/KMeansClusterer.h"
#define PRINT(x) std::cout << x;

int main(int argc, char *argv[])
{
    //Minimum number of arguments is: 2 (executable name and dataset directory)
    //Maximum valid number of arguments is 9

    if (argc <= 10)
    {
        std::string dataset;
        std::string output;
        bool colour = false;
        bool complex = false;
        int n = 10; //default 10
        int b = 1;  //default 1

        dataset = argv[1];

        for (int i = 1; i < argc; i++)
        {

            //std::cout << i << " : " << argv[i] << std::endl;
            if (std::string(argv[i]).compare("-o") == 0)
            {
                //std::cout << "-o : " << argv[i + 1] << std::endl;
                output = argv[i + 1];
            }
            else if (std::string(argv[i]).compare("-k") == 0)
            {
                //std::cout << "-k : " << argv[i + 1] << std::endl;

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
                //std::cout << "-bin : " << argv[i + 1] << std::endl;

                try
                {
                    b = std::stoi(argv[i + 1]);
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            else if (std::string(argv[i]).compare("-color") == 0)
            {
                std::cout << "-color : "
                          << "true" << std::endl;
                colour = true;
            }
            else if (std::string(argv[i]).compare("-complex") == 0)
            {
                std::cout << "-complex : "
                          << "true" << std::endl;
                complex = true;
            }
        }
        CHNJAR003::KMeansClusterer test = CHNJAR003::KMeansClusterer(dataset, output, n, b, colour, complex);
    }
    else
    {
        std::cout << "Invalid number of arguments given. (Maximum number allowed is 9)" << std::endl;
    }

    /*if (argc == 2 || argc == 4 || argc == 6 || argc == 8)
    {
        std::string dataset;
        std::string output;
        int n = 10; //default 10
        int b = 1;  //default 1

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

    //PRINT(dataset + "\n");
    //PRINT(output + "\n");
    //PRINT(std::to_string(n) + "\n");
    //PRINT(std::to_string(b) + "\n");

    /*std::vector<std::string> arguments;

        for (int i = 0; i < argc; i++)
        {
            arguments.push_back(argv[i]);
        }*/
    /*
}
else
{
    //Number of arguments is invalid
}*/
}