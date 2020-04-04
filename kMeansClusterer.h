
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
typedef unsigned char u_char;

class kMeansClusterer{
    public:
    int rows, cols;
    vector <u_char *> images;
    bool color;
 

    kMeansClusterer();
    ~kMeansClusterer();
    void setColor(const bool c);
    void readPPMimages(const string folder);
    void convertToGreyScale();
    void writeToFile();
    void printImageGrid();
    void generateHistograms(const int bin);
    void assignToCluster();
    void createInitialClusters(const int k);
    void recalculateCentroid();
    void applyKernel(const int kernel[3][3]); //3x3 image kernel
    
   



    

    





}; //end class

