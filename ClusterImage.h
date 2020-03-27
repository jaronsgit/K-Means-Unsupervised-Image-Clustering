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
    int imgID;
    int binSize;
    std::vector<u_char> feature; //histogram of intensity values

public:
    ClusterImage(void);                                                                                                      //default constructor
    ClusterImage(const int id, const std::string &imageName, const int binSize, const std::vector<u_char> &greyscalePixels); //paramterised constructor
    ClusterImage(const ClusterImage &rhs);                                                                                   //copy constructor
    ClusterImage(ClusterImage &&rhs);                                                                                        //move constructor
    ClusterImage &operator=(const ClusterImage &rhs);                                                                        //copy assignment operator
    ClusterImage &operator=(ClusterImage &&rhs);                                                                             //move assignment operator
    ~ClusterImage();                                                                                                         //destructor

    std::vector<u_char> extractFeature(const int binSize, const std::vector<u_char> &greyscalePixels);

    //u_char * extractFeature(const int binSize, const std::vector<u_char> &greyscalePixels);

    std::string getImageName() const;
    int getImgID() const;
    std::vector<u_char> getFeature() const;
    int getBinSize() const;

    /*bool setImageName(const std::string &imageName);
    bool setImgID(const int id);
    bool setFeature(const std::vector<u_char> &feature);*/

    //float calculateMean(const std::vector<unsigned char> &histogramArr);
    //float getMean(void) const;
};

} // namespace CHNJAR003
#endif