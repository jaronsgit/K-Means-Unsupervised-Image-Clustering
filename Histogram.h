#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <string>

namespace CHNJAR003
{
class Histogram
{
private:
    int binSize;
    std::vector<unsigned char> histogramArr;

public:
    Histogram(void);                                                                 //default constructor
    Histogram(const int binSize, const std::vector<unsigned char> &greyscalePixels); //parameterized constructor
    Histogram(const Histogram &rhs);                                                 //copy constructor
    Histogram(Histogram &&rhs);                                                      //move constructor
    Histogram &operator=(const Histogram &rhs);                                      //copy assignment operator
    Histogram &operator=(Histogram &&rhs);                                           //move assignment operator

    std::vector<unsigned char> createHistogram(const int binSize, const std::vector<unsigned char> &greyscalePixels);
    float calculateMean(const std::vector<unsigned char> &histogramArr);
    float getMean(void) const;
    ~Histogram();
};

} // namespace CHNJAR003
#endif