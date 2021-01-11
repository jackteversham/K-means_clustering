## README

## Unsupervised Image Classification using K-Means CLustering

## LOCATION OF DATASET NB
Please note that when running, simply enter the name of the folder where the dataset is located as the first argument.
My program will look for this folder in the directory above src.

So make sure the dataset is in the directory above src.
E.g. if folder is called "dataset" then run using

./clusterer dataset otherArgs

NOT:

./clusterer ../dataset otherArgs

Here the PPM images are located in the current directory and should be be removed into the above directory when running.

## To Compile

cd to the src directory and type the command make

## To Run

In the src folder, invoke the program as follows:
- The dataset must be specified
- output is the name of the file where the clusters will be written to, located in the src directory
- n specifies the number of clusters
- b specifies the bin interval size
- invoke with -f flag to apply the additional feature (note this only takes affect without -color flag i.e. when grayscale)
   --> When running with -f, an example image will be printed to demonstrate what the algorithm is doing
- invoke with -color to apply clustering according to color feature

./clusterer <dataset> [-o output] [-k n] [-bin b] [-f] [-color]

## Additional Feature

For my additionaly feature I tried a combination of methods:

- Firstly, I computed a difference map over all pixels of each image. Effectively, I iterated over each non-zero pixel and counted the number of neighbouring zeros in a 3x3 grid surrounding the pixel. The greater the number of neighbouring pixels, the more likely to be on a boundary and thus I could construct an outline.

- Secondly, I used an image kernel which I passed over each grayscale image like a window moving average function. The image kernel was obtain from https://setosa.io/ev/image-kernels/.
The specific kernel in question is one to emphasie the outline of the image.

- After this I computed the distance between the maximum extremes (i.e. the terminal points of the images) for each image - theoretically this distance shouldn't change as the image terminates. This is the final single value per image that I used as my feature to cluster against.

Implementation shown in the following methods in kMeansClusterer.cpp:

void differenceMap();
void applyKernel(const int kernal[3][3]);
void findDistanceBetweenExtremeMaximums();

All source code was original and implemented by me.
Note that the feature is only implemented when -f flag is present and -color or NOT present.

## Files

## clusterer.cpp
Contains the main entry point to the program and calls methods of the KMeansClusterer class implemented in kMeansCLusterer.cpp. Handles command line args


## KMeansClusterer.cpp
Contains all functionality of the KMeansClusterer class.

## KMeansClusterer.h
Contains the kMeansCLusterer class declaration and definition.


 
