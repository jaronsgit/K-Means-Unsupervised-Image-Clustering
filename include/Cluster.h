#ifndef CLUSTER_H
#define CLUSTER_H
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "../include/ClusterImage.h"

namespace CHNJAR003
{

    class Cluster
    {
    private:
        int clusterID;
        bool useRGB;

        std::vector<double> mean;
        std::vector<double> rCentroid; //histogram of intensity values - used for greyscale
        std::vector<double> gCentroid; //histogram of intensity values - used for greyscale
        std::vector<double> bCentroid; //histogram of intensity values - used for greyscale

        std::vector<std::shared_ptr<ClusterImage>> images; //All the images assigned to the cluster

    public:
        Cluster(void) = default;                          //default constructor
        Cluster(const Cluster &rhs) = default;            //copy constructor
        Cluster(Cluster &&rhs) = default;                 //move constructor
        Cluster &operator=(const Cluster &rhs) = default; //copy assignment operator
        Cluster &operator=(Cluster &&rhs) = default;      //move assignment operator
        ~Cluster();                                       //destructor

        Cluster(const int id, const std::shared_ptr<ClusterImage> &cImgPtr, bool colour); //parameterised constructor - takes in its cluster id number as well as an initial clusterImage

        std::vector<float> calculateNewMean(void) const;
        void recalculateCentroid(void);
        const std::vector<std::vector<double>> getMean(void) const;
        //void setMean(const std::vector<double> &mean);

        void clearCluster(void);                                            //clear the cluster of images
        void addClusterImage(const std::shared_ptr<ClusterImage> &cImgPtr); //Add an image to the cluster
        bool removeClusterImage(const int imgID);
        int getID(void) const;
        int getSize(void) const;

        friend std::ostream &operator<<(std::ostream &os, const Cluster &ct);
    };

} // namespace CHNJAR003

#endif