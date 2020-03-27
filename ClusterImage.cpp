#include "ClusterImage.h"

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

} // namespace CHNJAR003