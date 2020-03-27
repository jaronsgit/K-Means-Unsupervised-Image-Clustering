#ifndef KMEANSCLUSTERER_H
#define KMEANSCLUSTERER_H
#include <string>
#include <vector>
#include <memory>
#include "ClusterImage.h"
#include "Cluster.h"

namespace CHNJAR003
{

class KMeansClusterer
{
private:
    std::string datasetDir;
    std::string outputFileName;
    int binSize;
    int numClusters;
    std::vector<std::shared_ptr<ClusterImage>> images;
    std::vector<std::unique_ptr<Cluster>> clusters;

    int findNearestCluster(std::shared_ptr<ClusterImage> image); //return the id number of the nearest cluster

public:
    typedef unsigned char u_char;
    KMeansClusterer(void);
    KMeansClusterer(const std::string &dataset, const std::string &output, const int numClusters, const int binSize);
    std::vector<std::shared_ptr<ClusterImage>> readInImages(const std::string &datasetDir);
    std::vector<u_char> convertToGreyscale(std::vector<u_char> rgbValues);
    ~KMeansClusterer();

    friend std::ostream &operator<<(std::ostream &os, const KMeansClusterer &kt);
};

} // namespace CHNJAR003

#endif