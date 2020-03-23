#ifndef CLUSTERIMAGE_H
#define CLUSTERIMAGE_H
#include "Histogram.h"
#include <vector>
#include <string>
namespace CHNJAR003
{
class ClusterImage
{
private:
    std::string imageName;
    Histogram feature;

public:
    ClusterImage(void);
    ~ClusterImage();
};

} // namespace CHNJAR003
#endif