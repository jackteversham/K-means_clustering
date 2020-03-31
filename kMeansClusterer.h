
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
typedef unsigned char u_char;

class kMeansClusterer{
    public:
    int rows, cols;
    vector <u_char *> images;
 

    kMeansClusterer();
    ~kMeansClusterer();
    void readPPMimages(const string folder);
    void convertToGreyScale();
    void writeToFile();
    void printImageGrid();
    void generateHistograms(const int bin);


    

    





}; //end class

