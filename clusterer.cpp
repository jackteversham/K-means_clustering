#include <iostream>
#include <string>
#include <fstream>

#include "kMeansClusterer.cpp"

using namespace std;


int main(int argc, char** argv){

    string folder = argv[1];
    kMeansClusterer kmClusterer;
    string bin = "1"; //defualt value
    string k = "10";
    for (int i = 0; i < argc; i++)
    {
        string str = "-bin";
        if(argv[i] == str){
            bin = argv[i+1];
        }
        str = "-k";
        if(argv[i] == str){
            k = argv[i+1];
        }
       
    }
    
    

   

    kmClusterer.readPPMimages(folder);
    kmClusterer.convertToGreyScale();
    //kmClusterer.writeToFile();
    //kmClusterer.printImageGrid();
    kmClusterer.generateHistograms(stoi(bin));
    kmClusterer.createInitialClusters(stoi(k));
    kmClusterer.assignToCluster();

    cout << kmClusterer;

    kmClusterer.recalculateCentroid();
    
    return 0;
}