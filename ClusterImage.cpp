#include "ClusterImage.h"
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

ClusterImage::ClusterImage(const int id, const std::string &imageName, const int binSize, const std::vector<u_char> &greyscalePixels, const bool colour)
    : imgID(id), imageName(imageName), binSize(binSize), useRGB(colour), feature(extractFeature(binSize, greyscalePixels, useRGB))
{
    this->clusterID = -1; //not yet assigned to a cluster

    double mean = 0.0;

    for (int i = 0; i < feature.size(); i++)
    {
        mean += feature[i] * i;
    }

    this->featureMean = mean / feature.size();
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

std::vector<u_char> ClusterImage::extractFeature(const int binSize, const std::vector<u_char> &pixels, const bool colour)
{
    if (!colour) //image to be processed in greyscale - expecting pixels to be vector of unsigned char of all the pixels in the image
    {
        std::vector<u_char> tempFeature(256 / binSize, 0);
        try
        {

            for (auto const &pixel : pixels)
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
    else //Expecting raw PPM RGB image byte data in a vector of unsigned chars
    {

        std::vector<u_char> rPixels; //Vector containing all the red pixel intensity values
        std::vector<u_char> gPixels; //Vector containing all the green pixel intensity values
        std::vector<u_char> bPixels; //Vector containing all the blue pixel intensity values

        for (int i = 0; i < pixels.size(); i += 3) //extract the intensities of each colour
        {
            rPixels.push_back(pixels[i]);
            gPixels.push_back(pixels[i + 1]);
            bPixels.push_back(pixels[i + 2]);
        }

        std::vector<u_char> tempFeature(256 / binSize, 0);
        try
        {

            for (int i = 0; i < pixels.size(); i++)
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