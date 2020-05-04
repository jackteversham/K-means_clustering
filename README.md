## README for CSC3022F Assignment 4

## Unserpvised Image Classification using K-Means CLustering

# How to run
## LOCATION OF DATASET NB
Please note that when running, simply enter the name of the folder where the dataset is located as the first argument.
My program will look for this folder in the directory above src.

So make sure the dataset is in the directory above src.
E.g. if folder is called "dataset" then run using

./clusterer dataset otherArgs

NOT:

./clusterer ../dataset otherA rgs

## To Compile

cd to the src directory and type the command make

## To Run

In the src folder, invoke the program as follows:
- The dataset must be specified
- output is the name of the file where the clusters will be written to, located in the src directory
- n specifies the number of clusters
- b specifies the bin interval size
- invoke with -f flag to apply the additional feature (note this only takes affect without -color flag i.e. when grayscale)
- invoke with -color to apply clustering according to color feature

./clusterer <dataset> [-o output] [-k n] [-bin b] [-f] [-color]


## clusterer.cpp

## Additional Feature



 