#include "../include/KMeansClusterer.h"
#include <dirent.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <cmath>
#include <iterator>
#include <algorithm>

typedef unsigned char u_char;

namespace CHNJAR003
{

    KMeansClusterer::KMeansClusterer(const std::string &dataset, const std::string &output, const int numClusters, const int binSize, const bool colour, const bool complex) : datasetDir(dataset), outputFileName(output), numClusters(numClusters), binSize(binSize), useRGB(colour), useComplexFeature(complex)
    {
        this->images = readInImages(datasetDir);
        //runClustering();
    }

    std::vector<std::shared_ptr<ClusterImage>> KMeansClusterer::readInImages(const std::string &datasetDir)
    {

        std::vector<std::shared_ptr<ClusterImage>> images;

        std::vector<std::string> fileNames;

        char buffer[128];
        std::string commandOutput = "";

        FILE *pipe = popen(("ls " + datasetDir).c_str(), "r");
        if (!pipe)
        {
            std::cout << "popen command failed." << std::endl;
        }

        while (!feof(pipe))
        {
            //Read from the buffer
            if (fgets(buffer, 128, pipe) != NULL)
            {
                commandOutput = std::string(buffer);
                int len = commandOutput.size();
                fileNames.push_back(commandOutput.erase(len - 1, 1));
                //std::cout << std::string(buffer);
            }
        }

        pclose(pipe);
        //////////////
        int id = 0;

        std::ifstream ppmFile;
        std::string tempLine;
        std::cout << "-> Reading in the PPM files" << std::endl;
        for (auto const fileName : fileNames)
        {
            ppmFile.open(datasetDir + "/" + fileName.c_str(), std::ios::in | std::ios::binary);

            std::vector<std::string> tempLines;
            if (ppmFile.is_open())
            {

                ppmFile >> tempLine; //Read the "magic number" i.e. P6

                getline(ppmFile, tempLine); //Read the next line - could be a comment

                while (tempLine.compare("255") != 0) //Read all the lines of the header until reading the "255" max value
                {
                    tempLines.push_back(tempLine);
                    getline(ppmFile, tempLine);
                }

                try
                {
                    //Extract the file information from the header lines - width and height
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

                    //if color parameter was not specified and complex feature parameter was not specified - use greyscale of image
                    if (!useRGB && !useComplexFeature)
                    {
                        std::vector<u_char> greyscaleP = convertToGreyscale(rgbP); //vector now contains the greyscale image data
                        std::shared_ptr<ClusterImage> tempCIPtr = std::make_shared<ClusterImage>(id, fileName, binSize, greyscaleP, useRGB);
                        id++;
                        images.push_back(tempCIPtr);
                    }
                    //if the color parameter was specified and complex feature parameter was not specified - use rgb features
                    else if (useRGB && !useComplexFeature)
                    {
                        std::shared_ptr<ClusterImage> tempCIPtr = std::make_shared<ClusterImage>(id, fileName, binSize, rgbP, useRGB);
                        id++;
                        images.push_back(tempCIPtr);
                    }
                    else if (useComplexFeature) //if complex feature was specified - use complex feature
                    {

                        std::vector<u_char> convolvedImg = applyConvolution(rgbP, Nrows, Ncols); //apply Gaussian blur to image
                        this->useRGB = true;                                                     //requires RGB channels

                        std::shared_ptr<ClusterImage> tempCIPtr = std::make_shared<ClusterImage>(id, fileName, binSize, convolvedImg, useRGB);
                        id++;
                        images.push_back(tempCIPtr);
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }

                ppmFile.close();
            }
        }
        std::cout << "-> Finished reading and processing of PPM files" << std::endl;
        return images;
    }

    //function that returns the cluster ID of the cluster that is the lowest Euclidean distance away from a given image
    int KMeansClusterer::findNearestCluster(std::shared_ptr<ClusterImage> image)
    {
        //If colour parameter was not specified
        if (!useRGB)
        {
            double tot = 0.0, minEuclidDist;

            int nearestClusterID;

            //Temporarily store the centroid of the first cluster
            std::vector<double> tempMean = clusters[0]->getMean()[0];
            //Store the feature of the image - greyscale in this case
            std::vector<unsigned int> tempFeature = image->getFeature()[0];

            //Calculate the Euclidean distance from the first cluster
            for (int i = 0; i < 256 / binSize; i++)
            {
                tot += std::pow(tempMean[i] - tempFeature[i], 2.0);
            }

            //minEuclidDist = std::sqrt(tot);
            minEuclidDist = tot;
            nearestClusterID = clusters[0]->getID();

            //Calculate Euclidean distances to other clusters - find the lowest
            for (int i = 1; i < numClusters; i++)
            {
                double newEuclidDist;
                tot = 0.0;

                std::vector<double> tempMean = clusters[i]->getMean()[0];

                for (int j = 0; j < tempMean.size(); j++)
                {
                    tot += std::pow(tempMean[j] - tempFeature[j], 2.0);
                }

                //newEuclidDist = std::sqrt(tot);
                newEuclidDist = tot;

                //If new, lower distance has been found
                if (newEuclidDist < minEuclidDist)
                {
                    minEuclidDist = newEuclidDist;
                    nearestClusterID = clusters[i]->getID();
                }
            }

            return nearestClusterID;
        }
        //if colour parameter was specified
        else
        {

            double tot = 0.0, minEuclidDist;
            int nearestClusterID;
            //Temporarily store the R, G and B channel centroids of the first cluster
            std::vector<std::vector<double>> centroids = clusters[0]->getMean();
            std::vector<double> clusterRcentroid = centroids[0];
            std::vector<double> clusterGcentroid = centroids[1];
            std::vector<double> clusterBcentroid = centroids[2];
            //Store the R, G and B channel features of the image
            std::vector<std::vector<unsigned int>> features = image->getFeature();
            std::vector<unsigned int> imageRfeature = features[0];
            std::vector<unsigned int> imageGfeature = features[1];
            std::vector<unsigned int> imageBfeature = features[2];

            //Calculate the Euclidean distance of the image from the first cluster
            for (int i = 0; i < clusterRcentroid.size(); i++)
            {
                tot += std::pow(clusterRcentroid[i] - imageRfeature[i], 2.0);
                tot += std::pow(clusterGcentroid[i] - imageGfeature[i], 2.0);
                tot += std::pow(clusterBcentroid[i] - imageBfeature[i], 2.0);
            }

            //minEuclidDist = std::sqrt(tot);
            minEuclidDist = tot;
            nearestClusterID = clusters[0]->getID();

            //Calculate Euclidean distances to other clusters - find the lowest
            for (int i = 1; i < numClusters; i++)
            {
                double newEuclidDist;
                tot = 0.0;

                std::vector<std::vector<double>> centroids = clusters[i]->getMean();
                std::vector<double> clusterRcentroid = centroids[0];
                std::vector<double> clusterGcentroid = centroids[1];
                std::vector<double> clusterBcentroid = centroids[2];

                for (int j = 0; j < clusterRcentroid.size(); j++)
                {
                    tot += std::pow(clusterRcentroid[j] - imageRfeature[j], 2.0);
                    tot += std::pow(clusterGcentroid[j] - imageGfeature[j], 2.0);
                    tot += std::pow(clusterBcentroid[j] - imageBfeature[j], 2.0);
                }

                //newEuclidDist = std::sqrt(tot);
                newEuclidDist = tot;

                //If new, lower distance has been found
                if (newEuclidDist < minEuclidDist)
                {
                    minEuclidDist = newEuclidDist;
                    nearestClusterID = clusters[i]->getID();
                }
            }

            return nearestClusterID;
        }
    }

    //Convert the raw RGB data from the PPM file to greyscale pixel data
    const std::vector<u_char> KMeansClusterer::convertToGreyscale(const std::vector<u_char> &rawRGBdata)
    {
        std::vector<u_char> greyscaleP;

        for (int i = 0; i < rawRGBdata.size(); i += 3)
        {
            u_char tempGreyPixel = 0.21 * rawRGBdata[i] + 0.72 * rawRGBdata[i + 1] + 0.07 * rawRGBdata[i + 2];
            greyscaleP.push_back(tempGreyPixel);
        }

        return greyscaleP;
    }

    const double KMeansClusterer::calculateTotalSpread(void) const
    {
        double totalDist = 0.0;
        for (auto const &cluster : clusters)
        {
            totalDist = cluster->getClusterSpread();
        }
        return totalDist;
    }

    //Run the K-Means clustering algorithm on the images
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
            std::unique_ptr<Cluster> tempCluster(new Cluster(i, images[imageID], useRGB));
            images[imageID]->setClusterID(i);
            clusters.push_back(std::move(tempCluster));
        }
        ////////////New initialisation method
        /*int numImages = images.size();
        std::srand(unsigned(time(0)));
        std::random_shuffle(images.begin(), images.end());

        int stepSize = numImages / numClusters;

        for (int i = 0; i < numClusters; ++i)
        {
            std::unique_ptr<Cluster> tempCluster(new Cluster(i, useRGB));
            clusters.push_back(std::move(tempCluster));
        }

        //if even number of clusters
        if (numClusters % 2 == 0)
        {
            for (int i = 0; i < numImages; ++i)
            {
                images[i]->setClusterID(i / stepSize);
                clusters[i / stepSize]->addClusterImage(images[i]);
            }
        }
        else
        {
            for (int i = 0; i < numImages; ++i)
            {
                if (i < numImages - 1)
                {
                    images[i]->setClusterID(i / stepSize);
                    clusters[i / stepSize]->addClusterImage(images[i]);
                }
                else //place the last element in the last cluster
                {
                    images[i]->setClusterID(numClusters - 1);
                    clusters[numClusters - 1]->addClusterImage(images[i]);
                }
            }
        }

        for (int i = 0; i < numClusters; ++i)
        {
            clusters[i]->recalculateCentroid();
        }
*/
        ////////////

        std::cout << "-> Running the K-Means Clustering Algorithm" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;

        int iterationCount = 0;

        while (true)
        {
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
                clusters[cl]->recalculateCentroid();
            }

            if (converged || iterationCount >= 50) //max iteration count of 50
            {

                std::cout << std::endl
                          << "=> Clustering completed in: " << iterationCount << " iterations."
                          << std::endl;

                int lineLen = std::string("=> Clustering completed in: " + std::to_string(iterationCount) + " iterations.").length();
                std::string line;
                for (int a = 0; a < lineLen; ++a)
                {
                    line += "^";
                }

                std::cout << line << std::endl;
                for (auto const &cluster : clusters)
                {
                    std::cout << *cluster;
                }

                if (outputFileName.compare("") != 0)
                {
                    std::cout << "----------------------------------------------------------" << std::endl;
                    std::cout << "-> Writing clustering results to: " << outputFileName << std::endl;
                    std::ofstream outFile;
                    outFile.open(outputFileName, std::ofstream::out);
                    for (auto const &cluster : clusters)
                    {
                        outFile << *cluster;
                    }
                    outFile.close();
                    std::cout << "----------------------------------------------------------" << std::endl;
                }
                break;
            }
            iterationCount++;
        }
    }

    const std::vector<u_char> KMeansClusterer::thresholdGreyscaleImage(const std::vector<u_char> &greyscalePixels)
    {
        std::vector<unsigned int> histogram(256, 0);
        try
        {
            for (auto const &pixel : greyscalePixels)
            {
                histogram[pixel] += 1;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        std::vector<u_char> thresholdPixels;
        long numPixels = greyscalePixels.size();
        //std::cout << "numPixels=" << numPixels << std::endl;

        int classProb1 = 0, classProb2 = 0;
        float sumForeground = 0;
        float sumBackground = 0;
        int weightBackground = 0;
        int weightForeground = 0;
        float maxBetweenVariance = 0;
        int thresholdBin = 0;

        for (int i = 0; i < histogram.size(); i++)
        {
            sumForeground += i * int(histogram[i]);
        }

        for (int i = 0; i < histogram.size(); i++)
        {
            weightBackground += int(histogram[i]);
            if (weightBackground == 0)
            {
                continue;
            }
            weightForeground = numPixels - weightBackground;

            if (weightForeground == 0)
            {
                break;
            }

            sumBackground += float(i * int(histogram[i]));

            float meanBackground = sumBackground / weightBackground;
            float meanForeground = (sumForeground - sumBackground) / weightForeground;

            float betweenClassVariance = weightBackground * weightForeground * std::pow((meanBackground - meanForeground), 2.0);

            if (betweenClassVariance > maxBetweenVariance)
            {
                maxBetweenVariance = betweenClassVariance;
                thresholdBin = i;
            }
        }

        for (int i = 0; i < greyscalePixels.size(); i++)
        {
            if (greyscalePixels[i] > thresholdBin)
            {
                thresholdPixels.push_back(255);
            }
            else
            {
                thresholdPixels.push_back(0);
            }
        }

        return thresholdPixels;
    }

    const std::vector<u_char> KMeansClusterer::detectImageEdges(const std::vector<u_char> &greyscalePixels, int numRows, int numCols)
    {
        std::vector<u_char> edgeImage;

        unsigned int Gx[3][3];
        unsigned int Gy[3][3];

        //SOBEL Horizontal Mask
        Gx[0][0] = 1;
        Gx[0][1] = 0;
        Gx[0][2] = -1;
        Gx[1][0] = 2;
        Gx[1][1] = 0;
        Gx[1][2] = -2;
        Gx[2][0] = 1;
        Gx[2][1] = 0;
        Gx[2][2] = -1;

        //SOBEL Vertical Mask
        Gy[0][0] = 1;
        Gy[0][1] = 2;
        Gy[0][2] = 1;
        Gy[1][0] = 0;
        Gy[1][1] = 0;
        Gy[1][2] = 0;
        Gy[2][0] = -1;
        Gy[2][1] = -2;
        Gy[2][2] = -1;

        int gx = 0, gy = 0;

        for (int i = 0; i < greyscalePixels.size(); i++)
        {
            int x = i % numCols;
            int y = i / numRows;

            //if on one of the edges of the image
            if ((x == 0) || (x == numRows - 1) || (y == 0) || (y == numRows - 1))
            {
                gx = 0, gy = 0;
            }
            else
            {
                gx = 0, gy = 0;
                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {

                        gx = gx + Gx[1 + dx][1 + dy] * greyscalePixels[i + dx + (dy * numRows)];
                        gy = gy + Gy[1 + dx][1 + dy] * greyscalePixels[i + dx + (dy * numRows)];
                    }
                }
            }

            edgeImage.push_back(std::sqrt(gx * gx + gy * gy));
        }
        return edgeImage;
    }

    const std::vector<u_char> KMeansClusterer::applyConvolution(const std::vector<u_char> &rgbPixels, int numRows, int numCols)
    {
        std::vector<u_char> rPixels; //Vector containing all the red pixel intensity values
        std::vector<u_char> gPixels; //Vector containing all the green pixel intensity values
        std::vector<u_char> bPixels; //Vector containing all the blue pixel intensity values

        std::vector<unsigned char> convolvedRGB;

        for (int i = 0; i < rgbPixels.size(); i += 3) //extract the intensities of each colour into separate vectors
        {
            rPixels.push_back(rgbPixels[i]);
            gPixels.push_back(rgbPixels[i + 1]);
            bPixels.push_back(rgbPixels[i + 2]);
        }
        unsigned int W[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
        int r = 0, g = 0, b = 0;
        for (int i = 0; i < rPixels.size(); i++)
        {
            int x = i % numCols;
            int y = i / numRows;

            //if on one of the edges of the image
            if ((x == 0) || (x == numRows - 1) || (y == 0) || (y == numRows - 1))
            {
                r = 0;
                g = 0;
                b = 0;
            }
            else
            {
                r = 0;
                g = 0;
                b = 0;
                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {

                        r = r + W[1 + dx][1 + dy] * rPixels[i + dx + (dy * numRows)];
                        g = g + W[1 + dx][1 + dy] * gPixels[i + dx + (dy * numRows)];
                        b = b + W[1 + dx][1 + dy] * bPixels[i + dx + (dy * numRows)];
                    }
                }
            }

            convolvedRGB.push_back(r / 9);
            convolvedRGB.push_back(g / 9);
            convolvedRGB.push_back(b / 9);
        }

        return convolvedRGB;
    }

    //Overloaded output operator - write's the current clustering of the images to a specified output stream
    std::ostream &operator<<(std::ostream &os, const KMeansClusterer &kt)
    {
        for (auto const &cluster : kt.clusters)
        {
            std::cout << *cluster;
        }

        return os;
    }

    KMeansClusterer::~KMeansClusterer()
    {
    }
} // namespace CHNJAR003
