
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
    void writeToFile(const string filename);
    void printImageGrid(const bool printOne);
    void generateHistograms(const int bin);
    void assignToCluster();
    void createInitialClusters(const int k);
    void recalculateCentroid();
    void applyKernel(const int kernel[3][3]); //3x3 image kernel
    void differenceMap();
    int min(const int x, const int y) const;
    bool hasConverged() const;
    string exec(const string command);
    void findDistanceBetweenExtremeMaximums();
    
   



    

    





}; //end class

