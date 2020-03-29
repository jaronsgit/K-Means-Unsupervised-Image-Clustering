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

KMeansClusterer::KMeansClusterer(const std::string &dataset, const std::string &output, const int numClusters, const int binSize) : datasetDir(dataset), outputFileName(output), numClusters(numClusters), binSize(binSize)
{
    this->images = readInImages(datasetDir);
    runClustering();
    /*for (auto const &image : images)
    {
        std::cout << image.use_count() << std::endl;
        std::cout << image->getImageName() << std::endl;
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
        std::cout << "Extracting PPM file names from directoy..." << std::endl;
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

                std::vector<u_char> rgbP(buffer, buffer + numBytes);
                std::vector<u_char> greyscaleP = convertToGreyscale(rgbP);

                /*for (int i = 0; i < numBytes / 3; i++)
                {
                    u_char tempGreyPixel = 0.21 * buffer[i] + 0.72 * buffer[i + 1] + 0.07 * buffer[i + 2];
                    greyscaleP.push_back(tempGreyPixel);
                }*/

                //std::cout << id << std::endl;
                //std::cout << fileName << std::endl;
                //std::cout << binSize << std::endl;
                //std::cout << greyscaleP.size() << std::endl;

                std::shared_ptr<ClusterImage> tempCIPtr = std::make_shared<ClusterImage>(id, fileName, binSize, greyscaleP);
                id++;
                images.push_back(tempCIPtr);
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
    std::vector<u_char> tempFeature = image->getFeature();

    //calculate the euclidian distance of the point from the mean of the cluster
    for (int i = 0; i < 256 / binSize; i++)
    {
        tot += pow(tempMean[i] - tempFeature[i], 2.0);
    }

    minEuclidDist = sqrt(tot);
    nearestClusterID = clusters[0]->getID();

    for (int i = 1; i < numClusters; i++)
    {
        double newEuclidDist;
        tot = 0.0;

        std::vector<float> tempMean = clusters[i]->getMean();

        for (int j = 0; j < 256 / binSize; j++)
        {
            tot += pow(tempMean[j] - tempFeature[j], 2.0);
        }

        newEuclidDist = sqrt(tot);

        if (newEuclidDist < minEuclidDist)
        {
            minEuclidDist = newEuclidDist;
            nearestClusterID = clusters[i]->getID();
        }
    }

    return nearestClusterID;
}

std::vector<u_char> KMeansClusterer::convertToGreyscale(std::vector<u_char> rgbValues)
{
    std::vector<u_char> greyscaleP;

    for (int i = 0; i < rgbValues.size() / 3; i++)
    {
        u_char tempGreyPixel = 0.21 * rgbValues[i] + 0.72 * rgbValues[i + 1] + 0.07 * rgbValues[i + 2];
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
    for (int i = 0; i < numClusters; i++)
    {
        do
        {
            imageID = std::rand() % numImages;

        } while (find(usedImageIds.begin(), usedImageIds.end(), imageID) != usedImageIds.end());

        usedImageIds.push_back(imageID);
        std::unique_ptr<Cluster> tempCluster(new Cluster(i));
        clusters.push_back(std::move(tempCluster));
        clusters[i]->addClusterImage(images[imageID]);
    }
    std::cout << "clusters vector size: " << clusters.size() << std::endl;

    /*for (auto const &cluster : clusters)
    {
        std::cout << "cluster:" << cluster->getID() << " size:" << cluster->getSize() << std::endl;
    }*/

    std::cout << "Running the K-Means Clustering Algorithm..." << std::endl;

    int iterationCount = 1;
    while (true)
    {
        std::cout << "Iteration: " << iterationCount << std::endl;
        bool converged = true;

        //Assignment step - assign each observation to cluster with the nearest mean

        //Update step - recalculate the means of each cluster
    }
}

//std::vector<u_char> KMeansClusterer::convertToGreyscale(std::vector<u_char> rgbValues) {}
KMeansClusterer::~KMeansClusterer()
{
    std::cout << "KMeansClusterer destroyed." << std::endl;
}
} // namespace CHNJAR003
