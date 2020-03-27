#include "Cluster.h"
#include <vector>
#include <algorithm>

namespace CHNJAR003
{

std::vector<float> Cluster::calculateNewMean(void) const
{
    std::vector<float> tempMean(256 / images[0]->getBinSize()); //Based off the assumption that all the images have the same feature dimension

    //Can maybe replace this with a std::transform version later on
    for (auto const &img : images)
    {
        std::vector<u_char> tempFeature = img->getFeature();

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
}

std::vector<float> Cluster::getMean(void) const
{
    return mean;
}
void Cluster::setMean(std::vector<float> mean)
{
    this->mean = mean;
}

} // namespace CHNJAR003
