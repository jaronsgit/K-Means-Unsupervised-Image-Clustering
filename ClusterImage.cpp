#include "ClusterImage.h"
#include <iostream>
namespace CHNJAR003
{
ClusterImage::ClusterImage()
{
    this->imageName = "";
    this->imgID = -1;
    this->clusterID = -1;
    this->binSize = -1;
}

ClusterImage::ClusterImage(const int id, const std::string &imageName, const int binSize, const std::vector<u_char> &greyscalePixels)
    : imgID(id), imageName(imageName), binSize(binSize), feature(extractFeature(binSize, greyscalePixels))
{
    this->clusterID = -1; //not yet assigned to a cluster

    double mean = 0.0;

    for (int i = 0; i < feature.size(); i++)
    {
        mean += feature[i] * i;
    }

    this->featureMean = mean / feature.size();
}

ClusterImage::ClusterImage(const ClusterImage &rhs) : imgID(rhs.imgID), imageName(rhs.imageName), binSize(rhs.binSize), feature(rhs.feature)
{
    this->clusterID = rhs.clusterID;
}

ClusterImage::ClusterImage(ClusterImage &&rhs) : imgID(rhs.imgID), imageName(rhs.imageName), binSize(rhs.binSize), feature(std::move(rhs.feature))
{
    rhs.imgID = -1;
    rhs.imageName = "";
    rhs.binSize = -1;
    rhs.clusterID = -1;
}

ClusterImage &ClusterImage::operator=(const ClusterImage &rhs)
{
    if (this != &rhs)
    {
        this->imageName = rhs.imageName;
        this->imgID = rhs.imgID;
        this->binSize = rhs.binSize;
        this->feature = rhs.feature;
        this->clusterID = rhs.clusterID;
    }

    return *this;
}

ClusterImage &ClusterImage::operator=(ClusterImage &&rhs)
{
    if (this != &rhs)
    {
        this->imageName = rhs.imageName;
        this->imgID = rhs.imgID;
        this->binSize = rhs.binSize;
        this->feature = std::move(rhs.feature);
        this->clusterID = rhs.clusterID;

        rhs.imgID = -1;
        rhs.imageName = "";
        rhs.binSize = -1;
        rhs.clusterID = -1;
    }
    return *this;
}

ClusterImage::~ClusterImage()
{
    //std::cout << "ClusterImage: " << imgID << " destroyed." << std::endl;
    this->imageName = "";
    this->imgID = -1;
    this->binSize = -1;
    this->clusterID = -1;

    feature.clear();
}

std::vector<u_char> ClusterImage::extractFeature(const int binSize, const std::vector<u_char> &greyscalePixels)
{
    std::vector<u_char> tempFeature(256 / binSize, 0);
    try
    {

        for (auto const &pixel : greyscalePixels)
        {
            tempFeature[(pixel - 1) / binSize] += 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return tempFeature;
}
std::string ClusterImage::getImageName() const
{
    return imageName;
}
int ClusterImage::getImgID() const
{
    return imgID;
}
std::vector<u_char> ClusterImage::getFeature() const
{
    return feature;
}

float ClusterImage::getFeatureMean() const
{
    return featureMean;
}

int ClusterImage::getBinSize() const
{
    return binSize;
}

void ClusterImage::setClusterID(const int clusterID)
{
    this->clusterID = clusterID;
}

int ClusterImage::getClusterID(void) const
{
    return this->clusterID;
}

/*bool ClusterImage::setImageName(const std::string &imageName)
{
}
bool ClusterImage::setImgID(const int id) {}
bool ClusterImage::setFeature(const std::vector<u_char> &feature) {}*/
} // namespace CHNJAR003