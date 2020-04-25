#include "KMeansClusterer.h"
#include <dirent.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <cmath>
#include <algorithm>

typedef unsigned char u_char;

namespace CHNJAR003
{

//KMeansClusterer::KMeansClusterer() {}

KMeansClusterer::KMeansClusterer(const std::string &dataset, const std::string &output, const int numClusters, const int binSize, const bool colour) : datasetDir(dataset), outputFileName(output), numClusters(numClusters), binSize(binSize), useRGB(colour)
{
    this->images = readInImages(datasetDir);
    runClustering();
    /*for (auto const &image : images)
    {
        std::vector<u_char> tempFeature = image->getFeature();
        std::cout << "[";
        for (auto const &bin : tempFeature)
        {
            std::cout << int(bin) << ", ";
        }
        std::cout << "]" << std::endl;
    }*/
}

std::vector<std::shared_ptr<ClusterImage>> KMeansClusterer::readInImages(const std::string &datasetDir)
{

    std::vector<std::shared_ptr<ClusterImage>> images;

    std::vector<std::string> fileNames;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(datasetDir.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        std::cout << "Extracting PPM file names from directory..." << std::endl;
        std::string tempName;
        while ((ent = readdir(dir)) != NULL)
        {
            //printf("%s\n", ent->d_name);
            //std::cout << ent->d_type << std::endl;
            tempName = ent->d_name;
            if (tempName.find(".ppm") != std::string::npos)
            {
                //std::cout << ent->d_name << std::endl;
                fileNames.push_back(tempName);
            }
        }
        closedir(dir);
    }
    else
    {
        /* could not open directory */
        perror("");
    }
    int id = 0;

    std::ifstream ppmFile;
    std::string tempLine;
    std::cout << "Reading in the PPM files..." << std::endl;
    for (auto const fileName : fileNames)
    {
        //std::cout << fileName << std::endl;
        ppmFile.open(datasetDir + "/" + fileName.c_str(), std::ios::in | std::ios::binary);

        std::vector<std::string> tempLines;
        if (ppmFile.is_open())
        {

            ppmFile >> tempLine; //should read the "magic number" i.e. P6
            //std::cout << tempLine << std::endl;

            getline(ppmFile, tempLine);
            //std::cout << tempLine << std::endl;
            while (tempLine.compare("255") != 0)
            {
                tempLines.push_back(tempLine);
                getline(ppmFile, tempLine);
                //std::cout << tempLine << std::endl;
            }

            /*for (auto const &line : tempLines)
            {
                std::cout << line << std::endl;
            }*/

            try
            {
                std::istringstream ss(tempLines.back());
                std::string token;
                getline(ss, token, ' ');
                int Nrows = std::stoi(token);
                getline(ss, token, ' ');
                int Ncols = std::stoi(token);

                //std::cout << "rows: " << Nrows << " cols: " << Ncols << std::endl;

                int numBytes = Nrows * Ncols * 3;
                u_char buffer[numBytes];
                ppmFile.read((char *)buffer, numBytes); //buffer now stores RGB image data

                std::vector<u_char> rgbP(buffer, buffer + numBytes); //vector now contains the RGB image data

                if (!useRGB) //if color parameter was not used
                {
                    std::vector<u_char> greyscaleP = convertToGreyscale(rgbP); //vector now contains the greyscale image data
                    std::shared_ptr<ClusterImage> tempCIPtr = std::make_shared<ClusterImage>(id, fileName, binSize, greyscaleP, useRGB);
                    id++;
                    images.push_back(tempCIPtr);

                    //TESTING
                    if (fileName.compare("zero_9.ppm") == 0)
                    {

                        std::cout << tempCIPtr->getImageName() << " histogram:" << std::endl;
                        std::cout << "[ ";
                        for (int i = 0; i < tempCIPtr->getFeature().size(); i++)
                        {
                            std::cout << int(tempCIPtr->getFeature()[i]) << " ";
                        }
                        std::cout << " ]\n";
                    }
                }
                else
                { //if the color parameter was specified
                    std::shared_ptr<ClusterImage> tempCIPtr = std::make_shared<ClusterImage>(id, fileName, binSize, rgbP, useRGB);
                    id++;
                    images.push_back(tempCIPtr);
                }

                /*for (int i = 0; i < numBytes / 3; i++)
                {
                    u_char tempGreyPixel = 0.21 * buffer[i] + 0.72 * buffer[i + 1] + 0.07 * buffer[i + 2];
                    greyscaleP.push_back(tempGreyPixel);
                }*/

                //std::cout << id << std::endl;
                //std::cout << fileName << std::endl;
                //std::cout << binSize << std::endl;
                //std::cout << greyscaleP.size() << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }

            ppmFile.close();
        }
    }
    //std::shared_ptr<ClusterImage> tempCIPtr = std::make_shared<ClusterImage>();
    std::cout << "Finished reading and processing of PPM files." << std::endl;
    return images;
}

int KMeansClusterer::findNearestCluster(std::shared_ptr<ClusterImage> image)
{
    double tot = 0.0, minEuclidDist;

    int nearestClusterID;

    std::vector<float> tempMean = clusters[0]->getMean();
    std::vector<unsigned int> tempFeature = image->getFeature();

    //calculate the euclidian distance of the point from the mean of the cluster
    for (int i = 0; i < 256 / binSize; i++)
    {
        tot += std::pow(tempMean[i] - tempFeature[i], 2.0);
    }

    //minEuclidDist = std::sqrt(tot);
    minEuclidDist = tot;
    nearestClusterID = clusters[0]->getID();

    for (int i = 1; i < numClusters; i++)
    {
        double newEuclidDist;
        tot = 0.0;

        std::vector<float> tempMean = clusters[i]->getMean();

        for (int j = 0; j < 256 / binSize; j++)
        {
            tot += std::pow(tempMean[j] - tempFeature[j], 2.0);
        }

        //newEuclidDist = std::sqrt(tot);
        newEuclidDist = tot;

        if (newEuclidDist < minEuclidDist)
        {
            minEuclidDist = newEuclidDist;
            nearestClusterID = clusters[i]->getID();
        }
    }

    return nearestClusterID;

    /*
    float imageMean = image->getFeatureMean();
    float clusterMean = clusters[0]->getMean();
    int nearestClusterID = clusters[0]->getID();

    double minDist = std::abs(clusterMean - imageMean);

    double newDist;

    for (int i = 1; i < numClusters; i++)
    {
        clusterMean = clusters[i]->getMean();
        newDist = std::abs(imageMean - clusterMean);

        if (newDist < minDist)
        {
            minDist = newDist;
            nearestClusterID = clusters[i]->getID();
        }
    }

    return nearestClusterID;*/
}

std::vector<u_char> KMeansClusterer::convertToGreyscale(std::vector<u_char> rawRGBdata)
{
    std::vector<u_char> greyscaleP;

    for (int i = 0; i < rawRGBdata.size(); i += 3)
    {
        u_char tempGreyPixel = 0.21 * rawRGBdata[i] + 0.72 * rawRGBdata[i + 1] + 0.07 * rawRGBdata[i + 2];
        greyscaleP.push_back(tempGreyPixel);
    }

    return greyscaleP;
}

void KMeansClusterer::runClustering()
{

    //Initialise the clusters
    //Must initialise k=numClusters unique clusters
    std::vector<int> usedImageIds;
    int numImages = images.size();
    int imageID;
    srand(time(NULL)); //intialize random algorithm with "random" seed
    for (int i = 0; i < numClusters; i++)
    {
        do
        {
            imageID = std::rand() % numImages;

        } while (find(usedImageIds.begin(), usedImageIds.end(), imageID) != usedImageIds.end());

        usedImageIds.push_back(imageID);
        std::unique_ptr<Cluster> tempCluster(new Cluster(i, images[imageID]));
        images[imageID]->setClusterID(i);
        clusters.push_back(std::move(tempCluster));
    }
    std::cout << "clusters vector size: " << clusters.size() << std::endl;

    /*for (auto const &cluster : clusters)
    {
        std::cout << "cluster:" << *cluster << " mean:" << cluster->getMean() << std::endl;
    }

    for (auto const &image : images)
    {
        std::cout << image->getImageName() << " \t" << image->getFeatureMean() << std::endl;
    }*/

    std::cout << "Running the K-Means Clustering Algorithm..." << std::endl;

    int iterationCount = 1;
    /*
    std::cout << "Iteration: " << iterationCount << std::endl;
    for (auto const &cluster : clusters)
    {
        std::cout << *cluster << cluster->getMean() << std::endl;
    }

    //Assignment step - assign each observation to cluster with the nearest mean
    for (int i = 0; i < numImages; i++)
    {
        int currentClusterID = images[i]->getClusterID();
        int nearestClusterID = findNearestCluster(images[i]);

        std::cout << images[i]->getImageName() << "\tfeature mean: " << images[i]->getFeatureMean() << " \tnearest cluster: " << nearestClusterID << std::endl;
    }*/

    while (true)
    {
        std::cout << "Iteration: " << iterationCount << std::endl
                  << std::endl;
        for (auto const &cluster : clusters)
        {
            std::cout << *cluster; //<< cluster->getMean()[0] << std::endl;
        }
        bool converged = true;

        //Assignment step - assign each observation to cluster with the nearest mean
        for (int i = 0; i < numImages; i++)
        {
            int currentClusterID = images[i]->getClusterID();
            int nearestClusterID = findNearestCluster(images[i]);

            if (currentClusterID != nearestClusterID)
            {
                converged = false;

                //if the image has already been assigned to a cluster
                if (currentClusterID != -1)
                {
                    //remove it from the old cluster
                    for (int cl = 0; cl < numClusters; cl++)
                    {
                        if (clusters[cl]->getID() == currentClusterID)
                        {
                            clusters[cl]->removeClusterImage(images[i]->getImgID());
                        }
                    }
                }
                //add the image to the new cluster
                for (int cl = 0; cl < numClusters; cl++)
                {
                    if (clusters[cl]->getID() == nearestClusterID)
                    {
                        clusters[cl]->addClusterImage(images[i]);
                    }
                }
                //update the image's cluster ID
                images[i]->setClusterID(nearestClusterID);
            }
        }

        //Update step - recalculate the means of each cluster
        for (int cl = 0; cl < numClusters; cl++)
        {
            clusters[cl]->setMean(clusters[cl]->calculateNewMean());
        }

        if (converged || iterationCount >= 50)
        {
            std::cout << std::endl
                      << "Clustering completed in: " << iterationCount << " iterations." << std::endl;
            for (auto const &cluster : clusters)
            {
                std::cout << *cluster;
            }
            break;
        }
        iterationCount++;
    }
}

//std::vector<u_char> KMeansClusterer::convertToGreyscale(std::vector<u_char> rgbValues) {}
KMeansClusterer::~KMeansClusterer()
{
    //std::cout << "KMeansClusterer destroyed." << std::endl;
}
} // namespace CHNJAR003
