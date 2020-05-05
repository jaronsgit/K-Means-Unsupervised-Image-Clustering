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
        //The "Rule of Five" has been obeyed - none of the copy, move, copy assignment, move assingment or destructor needed to be manually defined and hence were not implemented
        KMeansClusterer(void) = default;
        KMeansClusterer(const KMeansClusterer &rhs) = delete;
        KMeansClusterer(KMeansClusterer &&rhs) = default;
        KMeansClusterer &operator=(const KMeansClusterer &rhs) = delete;
        KMeansClusterer &operator=(KMeansClusterer &&rhs) = default;
        KMeansClusterer(const std::string &dataset, const std::string &output, const int numClusters, const int binSize, const bool colour, const bool complex);

        std::vector<std::shared_ptr<ClusterImage>> readInImages(const std::string &datasetDir); //reads the image files from the given directory name and processes them into ClusterImages
        const std::vector<u_char> convertToGreyscale(const std::vector<u_char> &rawRGBdata);    //converts the raw RGB data from a PPM file to its greyscale equivalent

        const std::vector<u_char> thresholdGreyscaleImage(const std::vector<u_char> &greyscalePixels);
        const std::vector<u_char> detectImageEdges(const std::vector<u_char> &greyscalePixels, int numRows, int numCols);
        const std::vector<u_char> applyConvolution(const std::vector<u_char> &rgbPixels, int numRows, int numCols);

        const double calculateTotalSpread(void) const;
        void runClustering(); //Runs the K-Means clustering algorithm on the images that have been read in

        ~KMeansClusterer();

        friend std::ostream &operator<<(std::ostream &os, const KMeansClusterer &kt); //Overloaded output operator - write's the current clustering of the images to a specified output stream
    };

} // namespace CHNJAR003

#endif