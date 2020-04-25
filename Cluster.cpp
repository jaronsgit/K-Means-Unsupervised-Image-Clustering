#include "Cluster.h"
#include <vector>
#include <algorithm>

namespace CHNJAR003
{

Cluster::Cluster(const int id, const std::shared_ptr<ClusterImage> &cImgPtr)
{
    this->clusterID = id;
    this->addClusterImage(cImgPtr);
    this->mean = calculateNewMean();
}

std::vector<float> Cluster::calculateNewMean(void) const
{
    std::vector<float> tempMean(256 / images[0]->getBinSize(), 0.0); //Based off the assumption that all the images have the same feature dimension

    //Can maybe replace this with a std::transform version later on
    for (auto const &img : images)
    {
        std::vector<unsigned int> tempFeature = img->getFeature();

        for (int i = 0; i < tempFeature.size(); i++)
        {
            tempMean[i] += tempFeature[i];
        }
    }
    int numImages = images.size();

    for (int i = 0; i < tempMean.size(); i++)
    {
        tempMean[i] /= numImages;
    }

    return tempMean;
}

std::vector<float> Cluster::getMean(void) const
{
    return mean;
}
void Cluster::setMean(std::vector<float> mean)
{
    this->mean = mean;
}
/*
float Cluster::calculateNewMean(void) const
{
    float newMean = 0.0;
    int numImages = images.size();
    for (int i = 0; i < numImages; i++)
    {
        newMean += images[i]->getFeatureMean();
    }
    return newMean / numImages;
}

float Cluster::getMean(void) const
{
    return mean;
}
void Cluster::setMean(const float mean)
{
    this->mean = mean;
}
*/
void Cluster::clearCluster(void)
{
    this->images.clear();
}
void Cluster::addClusterImage(const std::shared_ptr<ClusterImage> &cImgPtr)
{
    this->images.push_back(cImgPtr);
}
bool Cluster::removeClusterImage(int imgID)
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

Cluster::~Cluster()
{
    //std::cout << "Cluster:" << clusterID << " destroyed." << std::endl;
}

} // namespace CHNJAR003
