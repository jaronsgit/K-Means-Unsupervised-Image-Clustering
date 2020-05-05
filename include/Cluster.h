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
        //The "Rule of Five" has been obeyed - none of the copy, move, copy assignment, move assingment or destructor needed to be manually defined and hence were not implemented
        Cluster(void) = default;                          //default constructor
        Cluster(const Cluster &rhs) = default;            //copy constructor
        Cluster(Cluster &&rhs) = default;                 //move constructor
        Cluster &operator=(const Cluster &rhs) = default; //copy assignment operator
        Cluster &operator=(Cluster &&rhs) = default;      //move assignment operator
        ~Cluster() = default;                             //destructor

        Cluster(const int id, const std::shared_ptr<ClusterImage> &cImgPtr, bool colour); //parameterised constructor - takes in its cluster id number as well as an initial clusterImage
        Cluster(const int id, bool colour);                                               //parameterised constructor
        void recalculateCentroid(void);                                                   //recomputes the centroid of the cluster based on the images assigned to the cluster
        const std::vector<std::vector<double>> getMean(void) const;                       //returns a vector containing the centroid histogram(s) of the cluster \
                                                                    either just the greyscale channel, or the R, G and B channels depending one what was specified initially

        void clearCluster(void);                                            //Remove all the images assigned to the cluster
        void addClusterImage(const std::shared_ptr<ClusterImage> &cImgPtr); //Assign an image to the cluster
        bool removeClusterImage(const int imgID);                           //Remove an image from the cluster by its imgID
        int getID(void) const;                                              //Get the id of the cluster
        int getSize(void) const;                                            //Get the number of images assigned to the cluster

        const double getClusterSpread(void) const;
        friend std::ostream &operator<<(std::ostream &os, const Cluster &ct); //Overloaded output operator - write's the contents of the cluster to a specified output stream
    };

} // namespace CHNJAR003

#endif