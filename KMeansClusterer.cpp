#include "KMeansClusterer.h"
#include <dirent.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

typedef unsigned char u_char;

namespace CHNJAR003
{

//KMeansClusterer::KMeansClusterer() {}

KMeansClusterer::KMeansClusterer(const std::string &dataset, const std::string &output, const int numClusters, const int binSize) : datasetDir(dataset), outputFileName(output), numClusters(numClusters), binSize(binSize)
{
    this->images = readInImages(datasetDir);

    /*for (auto const &image : images)
    {
        std::cout << image.use_count() << std::endl;
        std::cout << image->getImageName() << std::endl;
    }*/
}

std::vector<std::shared_ptr<ClusterImage>> KMeansClusterer::readInImages(const std::string &datasetDir)
{

    std::vector<std::shared_ptr<ClusterImage>> images;

    std::vector<std::string> fileNames;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(datasetDir.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        std::cout << "Extracting PPM file names from directoy..." << std::endl;
        std::string tempName;
        while ((ent = readdir(dir)) != NULL)
        {
            //printf("%s\n", ent->d_name);
            //std::cout << ent->d_type << std::endl;
            tempName = ent->d_name;
            if (tempName.find(".ppm") != std::string::npos)
            {
                //std::cout << ent->d_name << std::endl;
                fileNames.push_back(tempName);
            }
        }
        closedir(dir);
    }
    else
    {
        /* could not open directory */
        perror("");
    }
    int id = 0;

    std::ifstream ppmFile;
    std::string tempLine;
    std::cout << "Reading in the PPM files..." << std::endl;
    for (auto const fileName : fileNames)
    {
        //std::cout << fileName << std::endl;
        ppmFile.open(datasetDir + "/" + fileName.c_str(), std::ios::in | std::ios::binary);

        std::vector<std::string> tempLines;
        if (ppmFile.is_open())
        {

            ppmFile >> tempLine; //should read the "magic number" i.e. P6
            //std::cout << tempLine << std::endl;

            getline(ppmFile, tempLine);
            //std::cout << tempLine << std::endl;
            while (tempLine.compare("255") != 0)
            {
                tempLines.push_back(tempLine);
                getline(ppmFile, tempLine);
                //std::cout << tempLine << std::endl;
            }

            /*for (auto const &line : tempLines)
            {
                std::cout << line << std::endl;
            }*/

            try
            {
                std::istringstream ss(tempLines.back());
                std::string token;
                getline(ss, token, ' ');
                int Nrows = std::stoi(token);
                getline(ss, token, ' ');
                int Ncols = std::stoi(token);

                //std::cout << "rows: " << Nrows << " cols: " << Ncols << std::endl;

                int numBytes = Nrows * Ncols * 3;
                u_char buffer[numBytes];
                ppmFile.read((char *)buffer, numBytes); //buffer now stores RGB image data

                std::vector<u_char> greyscaleP;

                for (int i = 0; i < numBytes / 3; i++)
                {
                    u_char tempGreyPixel = 0.21 * buffer[i] + 0.72 * buffer[i + 1] + 0.07 * buffer[i + 2];
                    greyscaleP.push_back(tempGreyPixel);
                }

                //std::cout << id << std::endl;
                //std::cout << fileName << std::endl;
                //std::cout << binSize << std::endl;
                //std::cout << greyscaleP.size() << std::endl;

                std::shared_ptr<ClusterImage> tempCIPtr = std::make_shared<ClusterImage>(id, fileName, binSize, greyscaleP);
                id++;
                images.push_back(tempCIPtr);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }

            ppmFile.close();
        }
    }
    //std::shared_ptr<ClusterImage> tempCIPtr = std::make_shared<ClusterImage>();
    std::cout << "Finished reading and processing of PPM files." << std::endl;
    return images;
}
//std::vector<u_char> KMeansClusterer::convertToGreyscale(std::vector<u_char> rgbValues) {}
KMeansClusterer::~KMeansClusterer() {}
} // namespace CHNJAR003
