#ifndef CLUSTERIMAGE_H
#define CLUSTERIMAGE_H
#include <vector>
#include <string>
namespace CHNJAR003
{
    typedef unsigned char u_char;
    class ClusterImage
    {
    private:
        std::string imageName;
        int imgID, clusterID;
        int binSize;
        std::vector<unsigned int> feature;  //histogram of intensity values - used for greyscale
        std::vector<unsigned int> rFeature; //histogram of intensity values - used for greyscale
        std::vector<unsigned int> gFeature; //histogram of intensity values - used for greyscale
        std::vector<unsigned int> bFeature; //histogram of intensity values - used for greyscale
        bool useRGB;

    public:
        ClusterImage(void);                                                                                                                //default constructor
        ClusterImage(const int id, const std::string &imageName, const int binSize, const std::vector<u_char> &pixels, const bool colour); //paramterised constructor
        ClusterImage(const ClusterImage &rhs);                                                                                             //copy constructor
        ClusterImage(ClusterImage &&rhs);                                                                                                  //move constructor
        ClusterImage &operator=(const ClusterImage &rhs);                                                                                  //copy assignment operator
        ClusterImage &operator=(ClusterImage &&rhs);                                                                                       //move assignment operator
        ~ClusterImage();                                                                                                                   //destructor

        const std::vector<unsigned int> extractFeatureFromGreyscale(const int binSize, const std::vector<u_char> &greyscalePixels) const;
        const std::vector<std::vector<unsigned int>> extractFeaturesFromRawRGB(const int binSize, const std::vector<u_char> &rawRGBdata) const;

        const std::string &getImageName() const;
        const int getImgID() const;
        const std::vector<std::vector<unsigned int>> getFeature() const; //will return vector containing greyscale histogram vector or \
                                                                    vector containing the R, G and B channel histogram vectors.
        const int getBinSize() const;

        void setClusterID(const int clusterID);
        const int getClusterID(void) const;

        /*bool setImageName(const std::string &imageName);
    bool setImgID(const int id);
    bool setFeature(const std::vector<u_char> &feature);*/

        //float calculateMean(const std::vector<unsigned char> &histogramArr);
        //float getMean(void) const;
    };

} // namespace CHNJAR003
#endif