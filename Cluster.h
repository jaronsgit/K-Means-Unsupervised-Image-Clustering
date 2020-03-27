#ifndef CLUSTER_H
#define CLUSTER_H
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "ClusterImage.h"

namespace CHNJAR003
{

class Cluster
{
private:
    int clusterID;
    std::vector<float> mean;
    std::vector<std::shared_ptr<ClusterImage>> images;

public:
    Cluster(void);                          //defeault constructor
    Cluster(const Cluster &rhs);            //copy constructor
    Cluster(Cluster &&rhs);                 //move constructor
    Cluster &operator=(const Cluster &rhs); //copy assignment operator
    Cluster &operator=(Cluster &&rhs);      //move assignment operator
    ~Cluster();                             //destructor

    std::vector<float> calculateNewMean(void) const;
    std::vector<float> getMean(void) const;
    void setMean(std::vector<float> mean);
    void clearCluster(void);                                            //clear the cluster of images
    void addClusterImage(const std::shared_ptr<ClusterImage> &cImgPtr); //Add an image to the cluster
    bool removeClusterImage(int imgID);
    int getID(void) const;
    int getSize(void) const;

    friend std::ostream &operator<<(std::ostream &os, const Cluster &ct);
};

} // namespace CHNJAR003

#endif