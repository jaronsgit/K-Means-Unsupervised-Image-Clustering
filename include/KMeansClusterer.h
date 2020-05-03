#ifndef KMEANSCLUSTERER_H
#define KMEANSCLUSTERER_H
#include <string>
#include <vector>
#include <memory>
#include "../include/ClusterImage.h"
#include "../include/Cluster.h"

namespace CHNJAR003
{
    typedef unsigned char u_char;
    class KMeansClusterer
    {
    private:
        std::string datasetDir;
        std::string outputFileName;
        int binSize;
        int numClusters;
        bool useRGB;
        bool useComplexFeature;
        std::vector<std::shared_ptr<ClusterImage>> images;
        std::vector<std::unique_ptr<Cluster>> clusters;

        int findNearestCluster(std::shared_ptr<ClusterImage> image); //return the id number of the nearest cluster

    public:
        KMeansClusterer(void) = default;
        KMeansClusterer(const KMeansClusterer &rhs) = delete;
        KMeansClusterer(KMeansClusterer &&rhs) = default;
        KMeansClusterer &operator=(const KMeansClusterer &rhs) = delete;
        KMeansClusterer &operator=(KMeansClusterer &&rhs) = default;
        KMeansClusterer(const std::string &dataset, const std::string &output, const int numClusters, const int binSize, const bool colour, const bool complex);

        std::vector<std::shared_ptr<ClusterImage>> readInImages(const std::string &datasetDir);
        const std::vector<u_char> convertToGreyscale(const std::vector<u_char> &rawRGBdata);

        const std::vector<u_char> thresholdGreyscaleImage(const std::vector<u_char> &greyscalePixels);
        const std::vector<u_char> detectImageEdges(const std::vector<u_char> &greyscalePixels, int numRows, int numCols);
        const std::vector<u_char> applyConvolution(const std::vector<u_char> &rgbPixels, int numRows, int numCols);
        void runClustering();

        ~KMeansClusterer();

        friend std::ostream &operator<<(std::ostream &os, const KMeansClusterer &kt);
    };

} // namespace CHNJAR003

#endif