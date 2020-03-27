#include "KMeansClusterer.h"
#include <dirent.h>

typedef unsigned char u_char;

namespace CHNJAR003
{

//KMeansClusterer::KMeansClusterer() {}

KMeansClusterer::KMeansClusterer(const std::string &dataset, const std::string &output, const int numClusters, const int binSize) : datasetDir(dataset), outputFileName(output), numClusters(numClusters), binSize(binSize)
{
    readInImages(datasetDir);
}

std::vector<std::shared_ptr<ClusterImage>> KMeansClusterer::readInImages(const std::string &datasetDir)
{

    std::vector<std::shared_ptr<ClusterImage>> images;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(datasetDir.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        std::cout << "About to print all the files in the directory:" << std::endl;
        while ((ent = readdir(dir)) != NULL)
        {
            //printf("%s\n", ent->d_name);
            std::cout << ent->d_name << std::endl;
        }
        closedir(dir);
    }
    else
    {
        /* could not open directory */
        perror("");
    }

    return images;
}
//std::vector<u_char> KMeansClusterer::convertToGreyscale(std::vector<u_char> rgbValues) {}
KMeansClusterer::~KMeansClusterer() {}
} // namespace CHNJAR003
