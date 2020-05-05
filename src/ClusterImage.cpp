#include "../include/ClusterImage.h"
#include <iostream>
#include <math.h>
namespace CHNJAR003
{
    ClusterImage::ClusterImage()
    {
        this->imageName = "";
        this->imgID = -1;     //not yet assigned
        this->clusterID = -1; //not yet assigned to a cluster
        this->binSize = -1;   //not yet assigned one
        this->useRGB = false; //assume false by default
    }

    ClusterImage::ClusterImage(const int id, const std::string &imageName, const int binSize, const std::vector<u_char> &pixels, const bool colour)
        : imgID(id), imageName(imageName), binSize(binSize), useRGB(colour)
    {
        this->clusterID = -1; //not yet assigned to a cluster

        if (!colour) //if colour parameter not specified
        {
            this->feature = extractFeatureFromGreyscale(binSize, pixels);
        }
        else //colour parameter specified
        {
            std::vector<std::vector<unsigned int>> features = extractFeaturesFromRawRGB(binSize, pixels);
            this->rFeature = features[0]; //set the R channel histogram
            this->gFeature = features[1]; //set the G channel histogram
            this->bFeature = features[2]; //set the B channel histogram
        }
    }

    ClusterImage::ClusterImage(const ClusterImage &rhs) : imgID(rhs.imgID), imageName(rhs.imageName), binSize(rhs.binSize), useRGB(rhs.useRGB)
    {
        this->clusterID = rhs.clusterID;
        if (!useRGB) //colour parameter not specified
        {
            this->feature = rhs.feature;
        }
        else //colour parameter specified
        {
            this->rFeature = rhs.rFeature;
            this->gFeature = rhs.gFeature;
            this->bFeature = rhs.bFeature;
        }
    }

    ClusterImage::ClusterImage(ClusterImage &&rhs) : imgID(rhs.imgID), imageName(rhs.imageName), binSize(rhs.binSize), useRGB(rhs.useRGB)
    {
        rhs.imgID = -1;
        rhs.imageName = "";
        rhs.binSize = -1;
        rhs.clusterID = -1;
        rhs.useRGB = false;
        if (!this->useRGB) //colour parameter not specified
        {
            this->feature = std::move(rhs.feature);
        }
        else //colour parameter specified
        {
            this->rFeature = std::move(rhs.rFeature);
            this->gFeature = std::move(rhs.gFeature);
            this->bFeature = std::move(rhs.bFeature);
        }
    }

    ClusterImage &ClusterImage::operator=(const ClusterImage &rhs)
    {
        if (this != &rhs)
        {
            this->imageName = rhs.imageName;
            this->imgID = rhs.imgID;
            this->binSize = rhs.binSize;
            this->useRGB = rhs.useRGB;
            if (!this->useRGB) //colour parameter not specified
            {
                this->feature = rhs.feature;
            }
            else //colour parameter specified
            {
                this->rFeature = rhs.rFeature;
                this->gFeature = rhs.gFeature;
                this->bFeature = rhs.bFeature;
            }
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
            this->useRGB = rhs.useRGB;
            if (!this->useRGB) //colour parameter not specified
            {
                this->feature = std::move(rhs.feature);
            }
            else //colour parameter specified
            {
                this->rFeature = std::move(rhs.rFeature);
                this->gFeature = std::move(rhs.gFeature);
                this->bFeature = std::move(rhs.bFeature);
            }
            this->clusterID = rhs.clusterID;

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
    }
    const std::string &ClusterImage::getImageName() const
    {
        return imageName;
    }

    void ClusterImage::setImageName(const std::string &imgName)
    {
        this->imageName = imgName;
    }

    const int ClusterImage::getImgID() const
    {
        return imgID;
    }

    void ClusterImage::setImgID(const int id)
    {
        this->imgID = id;
    }

    const std::vector<std::vector<unsigned int>> ClusterImage::getFeature() const
    {
        std::vector<std::vector<unsigned int>> features;
        if (!useRGB) //colour paramemeter not specified - return greyscale channel
        {
            features.push_back(feature);
            return features;
        }
        else //colour parameter specified - return R, G and B channels
        {
            features.push_back(rFeature);
            features.push_back(gFeature);
            features.push_back(bFeature);
            return features;
        }
    }

    void ClusterImage::setFeature(const std::vector<std::vector<unsigned int>> &features)
    {
        if (!this->useRGB) //colour parameter not specified
        {
            this->feature = features[0];
        }
        else //colour parameter specified
        {
            this->rFeature = features[0];
            this->gFeature = features[1];
            this->bFeature = features[2];
        }
    }

    const int ClusterImage::getBinSize() const
    {
        return binSize;
    }
    void ClusterImage::setBinSize(const int binSize)
    {
        this->binSize = binSize;
    }

    void ClusterImage::setClusterID(const int clusterID)
    {
        this->clusterID = clusterID;
    }

    const int ClusterImage::getClusterID(void) const
    {
        return this->clusterID;
    }

} // namespace CHNJAR003