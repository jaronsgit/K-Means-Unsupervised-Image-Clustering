# C++ Assignment 4: Unsupervised Image Clustering

The project includes ClusterImage.h, ClusterImage.cpp, Cluster.h, Cluster.cpp, KMeansClusterer.h, KMeansClusterer.cpp, driver.cpp as well as a Makefile.

The ClusterImage class is responsible for extracting and storing the features from the PPM images that are read into the program.

The Cluster class is responsible for representing the image clusters - it maintains the cluster centroid and all necessary operations associated with the clusters.

The KMeansClusterer class provides functionality for reading in the PPM images from a given directory, constructing the ClusterImage objects and implementing the K-Means algorithm that clusters them into the Cluster objects accordingly - there is additional functionality for converting the raw RGB PPM image data to its greyscale equivalent as well as the implementation of a more complex feature.

driver.cpp is the program that parses command line input and calls the necessary commands.

## How to use the program

The program directory structure is as follows:

Assignment4_CHNJAR003
bin - contains the output executables
build - contains the object files (emptied on "make clean")
include - contains all the project header files
src - contains the application source files
test - contains the unit tests (Not used in this assignment.)

### The structure of the command line execution is as follows:

NB: the executable is located in the bin directory

```bash
clusterer <dataset> [-o output] [-k n] [-bin b] [-color] [-complex]
```

Note that due to the inherent unpredictability in the initial assignments of the cluster centroids, the program may given variable results between run with some giving better results than others.

NB - when the complex flag is specified, the color flag's specification is simply ignored for the purposes of the complex feature.

The more complex feature that should cluster the images with greater accuracy in this case, is an application of a Gaussian Blur convolution (http://web.pdx.edu/~jduh/courses/Archive/geog481w07/Students/Ludwig_ImageConvolution.pdf)(https://en.wikipedia.org/wiki/Kernel_(image_processing)#Convolution) to the RGB image data. The blur should even out the inconsistencies within the digit image colour gradients, allowing the images to be clustered more accurately through the reduction in noise. Other methods were attempted - namely implementations of Otsu's method of automatic image thresholding (http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/MORSE/threshold.pdf), as well as the "Sobel–Feldman operator" for detecting image edges (http://homepages.inf.ed.ac.uk/rbf/HIPR2/sobel.htm). The code developed for these implementations has been left in the assignment and the results of applying these methods can be found within the folders: thresholdedImages (Otsu's Method results) and edgeImages (Sobel-Feldman Operator results). These methods did not improve the clustering results on average and I suppose this is due to the lack of use of RGB channels resulting in dimensionality reduction that loses too much information.

#### For example: (if from within the main assignment directory)

```bash
./bin/clusterer Gradient_Numbers_PPMS -o testoutput -k 10 -bin 1 -color
```

Navigate to the folder in Terminal and utilise the provided Makefile to compile the program source code using:

```bash
make
```

The following command can be used to run the executable with the testing dataset (Gradient_Numbers_PPMS) with "-k 10 -bin 1":

```bash
make run
```

Use the following command to remove the binary files and executable from the build and bin folders respectively:

```bash
make clean
```

## Authors

**CHNJAR003**
