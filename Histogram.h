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
    Histogram(void);
    ~Histogram();
};

} // namespace CHNJAR003
#endif