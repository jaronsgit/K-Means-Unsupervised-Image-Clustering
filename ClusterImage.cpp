#include "ClusterImage.h"
#include <iostream>
namespace CHNJAR003
{
ClusterImage::ClusterImage()
{
    this->imageName = "";
    this->imgID = -1;
    this->binSize = -1;
}

ClusterImage::ClusterImage(const int id, const std::string &imageName, const int binSize, const std::vector<u_char> &greyscalePixels)
    : imgID(id), imageName(imageName), binSize(binSize), feature(extractFeature(binSize, greyscalePixels))
{
    /*this->imgID = id;
    this->imageName = imageName;
    this->binSize = binSize;
    this->feature = extractFeature(binSize,greyscalePixels);*/
}

ClusterImage::ClusterImage(const ClusterImage &rhs) : imgID(rhs.imgID), imageName(rhs.imageName), binSize(rhs.binSize), feature(rhs.feature)
{
}

ClusterImage::ClusterImage(ClusterImage &&rhs) : imgID(rhs.imgID), imageName(rhs.imageName), binSize(rhs.binSize), feature(std::move(rhs.feature))
{
    rhs.imgID = -1;
    rhs.imageName = "";
    rhs.binSize = -1;
}

ClusterImage &ClusterImage::operator=(const ClusterImage &rhs)
{
    if (this != &rhs)
    {
        this->imageName = rhs.imageName;
        this->imgID = rhs.imgID;
        this->binSize = rhs.binSize;
        this->feature = rhs.feature;
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

        rhs.imgID = -1;
        rhs.imageName = "";
        rhs.binSize = -1;
    }
    return *this;
}

ClusterImage::~ClusterImage()
{
    this->imageName = "";
    this->imgID = -1;
    this->binSize = -1;

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

int ClusterImage::getBinSize() const
{
    return binSize;
}

/*bool ClusterImage::setImageName(const std::string &imageName)
{
}
bool ClusterImage::setImgID(const int id) {}
bool ClusterImage::setFeature(const std::vector<u_char> &feature) {}*/
} // namespace CHNJAR003