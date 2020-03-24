#ifndef KMEANSCLUSTERER_H
#define KMEANSCLUSTERER_H
#include <string>
#include <vector>
#include "ClusterImage.h"
#include <dirent.h>

namespace CHNJAR003
{

class KMeansClusterer
{
private:
    std::string datasetDir;
    std::string outputFileName;
    int binSize;
    int numClusters;
    std::vector<std::vector<ClusterImage>> clusters; //2D vector with list of images associated with each cluster
    std::vector<ClusterImage> images;

public:
    typedef unsigned char u_char;
    KMeansClusterer(void);
    KMeansClusterer(const std::string &dataset, const std::string &output, const int numClusters, const int binSize);
    std::vector<ClusterImage> readInImages(const std::string &datasetDir);
    std::vector<u_char> convertToGreyscale(std::vector<u_char> rgbValues);
    ~KMeansClusterer();
};

} // namespace CHNJAR003

#endif