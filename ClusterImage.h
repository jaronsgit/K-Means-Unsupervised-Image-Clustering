#ifndef CLUSTERIMAGE_H
#define CLUSTERIMAGE_H
#include "Histogram.h"
#include <vector>
#include <string>
namespace CHNJAR003
{
class ClusterImage
{
private:
    std::string imageName;
    int imgID;
    int binSize;
    std::vector<unsigned char> feature;

public:
    typedef unsigned char u_char;

    ClusterImage(void);                                                                                        //default constructor
    ClusterImage(const std::string &imageName, const int binSize, const std::vector<u_char> &greyscalePixels); //paramterised constructor
    ClusterImage(const ClusterImage &rhs);                                                                     //copy constructor
    ClusterImage(ClusterImage &&rhs);                                                                          //move constructor
    ClusterImage &operator=(const ClusterImage &rhs);                                                          //copy assignment operator
    ClusterImage &operator=(ClusterImage &&rhs);                                                               //move assignment operator
    ~ClusterImage();                                                                                           //destructor

    std::vector<unsigned char> extractFeature(const int binSize, const std::vector<u_char> &greyscalePixels);
    std::string getImageName() const;
    int getImgID() const;
    std::vector<u_char> getFeature() const;
    //float calculateMean(const std::vector<unsigned char> &histogramArr);
    //float getMean(void) const;
};

} // namespace CHNJAR003
#endif