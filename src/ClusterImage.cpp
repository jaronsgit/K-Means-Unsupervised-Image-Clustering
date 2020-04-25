#include "../include/ClusterImage.h"
#include <iostream>
#include <math.h>
namespace CHNJAR003
{
ClusterImage::ClusterImage()
{
    this->imageName = "";
    this->imgID = -1;
    this->clusterID = -1;
    this->binSize = -1;
    this->useRGB = false;
}

ClusterImage::ClusterImage(const int id, const std::string &imageName, const int binSize, const std::vector<u_char> &pixels, const bool colour)
    : imgID(id), imageName(imageName), binSize(binSize), useRGB(colour)
{
    this->clusterID = -1; //not yet assigned to a cluster

    if (colour)
    {
        this->feature = extractFeatureFromGreyscale(binSize, pixels);
    }
    else
    {
        this->feature = extractFeatureFromRawRGB(binSize, pixels);
    }
}

ClusterImage::ClusterImage(const ClusterImage &rhs) : imgID(rhs.imgID), imageName(rhs.imageName), binSize(rhs.binSize), feature(rhs.feature), useRGB(rhs.useRGB)
{
    this->clusterID = rhs.clusterID;
}

ClusterImage::ClusterImage(ClusterImage &&rhs) : imgID(rhs.imgID), imageName(rhs.imageName), binSize(rhs.binSize), feature(std::move(rhs.feature)), useRGB(rhs.useRGB)
{
    rhs.imgID = -1;
    rhs.imageName = "";
    rhs.binSize = -1;
    rhs.clusterID = -1;
    rhs.useRGB = false;
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
        this->useRGB = rhs.useRGB;
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
        this->useRGB = rhs.useRGB;

        rhs.imgID = -1;
        rhs.imageName = "";
        rhs.binSize = -1;
        rhs.clusterID = -1;
        rhs.useRGB = false;
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

std::vector<unsigned int> ClusterImage::extractFeatureFromGreyscale(const int binSize, const std::vector<u_char> &greyscalePixels)
{

    std::vector<unsigned int> tempFeature(std::ceil(256 / binSize), 0);
    try
    {

        for (auto const &pixel : greyscalePixels)
        {
            tempFeature[std::ceil(pixel / binSize)] += 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return tempFeature;
}

std::vector<unsigned int> ClusterImage::extractFeatureFromRawRGB(const int binSize, const std::vector<u_char> &rawRGBdata)
{
    std::vector<unsigned int> rPixels; //Vector containing all the red pixel intensity values
    std::vector<unsigned int> gPixels; //Vector containing all the green pixel intensity values
    std::vector<unsigned int> bPixels; //Vector containing all the blue pixel intensity values

    for (int i = 0; i < rawRGBdata.size(); i += 3) //extract the intensities of each colour into separate vectors
    {
        rPixels.push_back(rawRGBdata[i]);
        gPixels.push_back(rawRGBdata[i + 1]);
        bPixels.push_back(rawRGBdata[i + 2]);
    }

    //Construct the histogram from the r, g and b pixel values
    //NB this has to be reworked - use separate histograms
    std::vector<unsigned int> tempFeature(256 / binSize, 0);
    try
    {

        for (int i = 0; i < rPixels.size(); i++) //Should be the same number of r, g and b pixel values
        {
            tempFeature[std::ceil(rPixels[i] / binSize)] += 1;
            tempFeature[std::ceil(gPixels[i] / binSize)] += 1;
            tempFeature[std::ceil(bPixels[i] / binSize)] += 1;
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
std::vector<unsigned int> ClusterImage::getFeature() const
{
    return feature;
}

/*float ClusterImage::getFeatureMean() const
{
    return featureMean;
}*/

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