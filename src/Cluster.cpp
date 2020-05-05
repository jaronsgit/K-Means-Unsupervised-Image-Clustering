#include "../include/Cluster.h"
#include <vector>
#include <algorithm>
#include <math.h>

namespace CHNJAR003
{

    Cluster::Cluster(const int id, const std::shared_ptr<ClusterImage> &cImgPtr, bool colour) : clusterID(id), useRGB(colour)
    {
        this->addClusterImage(cImgPtr); //Initial image randomly assigned to initialise the centroid for the clustering algorithm
        recalculateCentroid();          //Compute the initial centroid
    }

    void Cluster::recalculateCentroid(void)
    {
        if (this->useRGB)
        {
            std::vector<double> newRcentroid(std::ceil(256 / images[0]->getBinSize()), 0.0); //Based off the assumption that all the images have the same feature dimension
            std::vector<double> newGcentroid(std::ceil(256 / images[0]->getBinSize()), 0.0); //Based off the assumption that all the images have the same feature dimension
            std::vector<double> newBcentroid(std::ceil(256 / images[0]->getBinSize()), 0.0); //Based off the assumption that all the images have the same feature dimension

            //Can maybe replace this with a std::transform version later on
            for (auto const &img : images)
            {
                std::vector<std::vector<unsigned int>> features = img->getFeature();
                std::vector<unsigned int> tempRfeature = features[0];
                std::vector<unsigned int> tempGfeature = features[1];
                std::vector<unsigned int> tempBfeature = features[2];

                for (int i = 0; i < tempRfeature.size(); i++)
                {
                    newRcentroid[i] += tempRfeature[i];
                    newGcentroid[i] += tempGfeature[i];
                    newBcentroid[i] += tempBfeature[i];
                }
            }
            int numImages = images.size();

            for (int i = 0; i < newRcentroid.size(); i++)
            {
                newRcentroid[i] /= numImages;
                newGcentroid[i] /= numImages;
                newBcentroid[i] /= numImages;
            }
            this->rCentroid = newRcentroid;
            this->gCentroid = newGcentroid;
            this->bCentroid = newBcentroid;
        }
        else
        {
            std::vector<double> newMean(std::ceil(256 / images[0]->getBinSize()), 0.0); //Based off the assumption that all the images have the same feature dimension

            //Can maybe replace this with a std::transform version later on
            for (auto const &img : images)
            {
                std::vector<unsigned int> tempFeature = img->getFeature()[0];

                for (int i = 0; i < tempFeature.size(); i++)
                {
                    newMean[i] += tempFeature[i];
                }
            }
            int numImages = images.size();

            for (int i = 0; i < newMean.size(); i++)
            {
                newMean[i] /= numImages;
            }
            this->mean = newMean;
        }
    }

    const std::vector<std::vector<double>> Cluster::getMean(void) const
    {
        if (!useRGB) //colour parameter not specified - return just the greyscale channel centroid
        {
            std::vector<std::vector<double>> centroid;
            centroid.push_back(mean);
            return centroid;
        }
        else //colour parameter was specified - return R, G and B channels for the centroid
        {
            std::vector<std::vector<double>> centroid;
            centroid.push_back(rCentroid);
            centroid.push_back(gCentroid);
            centroid.push_back(bCentroid);
            return centroid;
        }
    }

    void Cluster::clearCluster(void)
    {
        this->images.clear();
    }
    void Cluster::addClusterImage(const std::shared_ptr<ClusterImage> &cImgPtr)
    {
        this->images.push_back(cImgPtr);
    }
    bool Cluster::removeClusterImage(const int imgID)
    {
        for (int i = 0; i < images.size(); i++)
        {
            if (images[i]->getImgID() == imgID)
            {
                images.erase(images.begin() + i);
                return true;
            }
        }
        return false;
    }
    int Cluster::getID(void) const
    {
        return this->clusterID;
    }
    int Cluster::getSize(void) const
    {
        return this->images.size();
    }

    const double Cluster::getClusterSpread(void) const
    {
        double spread = 0.0;

        for (auto const &image : images)
        {
            if (!useRGB)
            {
                for (int j = 0; j < mean.size(); j++)
                {
                    spread += std::pow(mean[j] - image->getFeature()[0][j], 2.0);
                }
            }
            else
            {
                std::vector<std::vector<unsigned int>> features = image->getFeature();
                std::vector<unsigned int> imageRfeature = features[0];
                std::vector<unsigned int> imageGfeature = features[1];
                std::vector<unsigned int> imageBfeature = features[2];

                for (int i = 0; i < rCentroid.size(); i++)
                {
                    spread += std::pow(rCentroid[i] - imageRfeature[i], 2.0);
                    spread += std::pow(gCentroid[i] - imageGfeature[i], 2.0);
                    spread += std::pow(bCentroid[i] - imageBfeature[i], 2.0);
                }
            }
        }
        return spread;
    }

    std::ostream &operator<<(std::ostream &os, const Cluster &ct)
    {
        os << "Cluster " << ct.getID() << " :";
        for (auto const &img : ct.images)
        {
            os << " " << img->getImageName();
        }
        os << std::endl;

        return os;
    }

} // namespace CHNJAR003
