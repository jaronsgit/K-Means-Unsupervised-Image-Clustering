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

        if (!colour)
        {
            this->feature = extractFeatureFromGreyscale(binSize, pixels);
        }
        else
        {
            std::vector<std::vector<unsigned int>> features = extractFeaturesFromRawRGB(binSize, pixels);
            this->rFeature = features[0]; //set the R channel histogram
            this->gFeature = features[1]; //set the G channel histogram
            this->bFeature = features[2]; //set the B channel histogram

            /*std::cout << "rFeature size:\t" << rFeature.size() << std::endl;
            std::cout << "gFeature size:\t" << gFeature.size() << std::endl;
            std::cout << "bFeature size:\t" << bFeature.size() << std::endl;*/
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

    const std::vector<unsigned int> ClusterImage::extractFeatureFromGreyscale(const int binSize, const std::vector<u_char> &greyscalePixels) const
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

    const std::vector<std::vector<unsigned int>> ClusterImage::extractFeaturesFromRawRGB(const int binSize, const std::vector<u_char> &rawRGBdata) const
    {
        std::vector<u_char> rPixels; //Vector containing all the red pixel intensity values
        std::vector<u_char> gPixels; //Vector containing all the green pixel intensity values
        std::vector<u_char> bPixels; //Vector containing all the blue pixel intensity values

        for (int i = 0; i < rawRGBdata.size(); i += 3) //extract the intensities of each colour into separate vectors
        {
            rPixels.push_back(rawRGBdata[i]);
            gPixels.push_back(rawRGBdata[i + 1]);
            bPixels.push_back(rawRGBdata[i + 2]);
        }

        //Construct the histograms from the r, g and b pixel values
        std::vector<unsigned int> rFeature(std::ceil(256 / binSize), 0);
        std::vector<unsigned int> gFeature(std::ceil(256 / binSize), 0);
        std::vector<unsigned int> bFeature(std::ceil(256 / binSize), 0);

        std::vector<std::vector<unsigned int>> rgbFeatures;

        try
        {

            for (int i = 0; i < rPixels.size(); i++) //Should be the same number of r, g and b pixel values
            {
                rFeature[std::ceil(rPixels[i] / binSize)] += 1;
                gFeature[std::ceil(gPixels[i] / binSize)] += 1;
                bFeature[std::ceil(bPixels[i] / binSize)] += 1;
            }
            rgbFeatures.push_back(rFeature);
            rgbFeatures.push_back(gFeature);
            rgbFeatures.push_back(bFeature);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        return rgbFeatures;

        /*
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
        return tempFeature;*/
    }
    const std::string &ClusterImage::getImageName() const
    {
        return imageName;
    }
    const int ClusterImage::getImgID() const
    {
        return imgID;
    }
    const std::vector<std::vector<unsigned int>> ClusterImage::getFeature() const
    {
        std::vector<std::vector<unsigned int>> features;
        if (!useRGB)
        {
            features.push_back(feature);
            return features;
        }
        else
        {
            features.push_back(rFeature);
            features.push_back(gFeature);
            features.push_back(bFeature);
            return features;
        }
    }

    const int ClusterImage::getBinSize() const
    {
        return binSize;
    }

    void ClusterImage::setClusterID(const int clusterID)
    {
        this->clusterID = clusterID;
    }

    const int ClusterImage::getClusterID(void) const
    {
        return this->clusterID;
    }

    /*bool ClusterImage::setImageName(const std::string &imageName)
{
}
bool ClusterImage::setImgID(const int id) {}
bool ClusterImage::setFeature(const std::vector<u_char> &feature) {}*/
} // namespace CHNJAR003