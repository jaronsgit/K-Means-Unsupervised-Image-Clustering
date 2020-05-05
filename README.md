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
