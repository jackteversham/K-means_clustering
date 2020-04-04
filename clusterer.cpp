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
    bool color = false;
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
        str = "-color";
         if(argv[i] == str){
            color = true;
        }
       
    }
    
    

   

    kmClusterer.readPPMimages(folder);

    if(!color){
        kmClusterer.convertToGreyScale();
    }else{
        kmClusterer.setColor(true);  //classifiy using RGB
    }
    
    //kmClusterer.writeToFile();
  //  kmClusterer.printImageGrid();

  
    kmClusterer.generateHistograms(stoi(bin));
    kmClusterer.createInitialClusters(stoi(k));
    kmClusterer.assignToCluster();

   // cout << kmClusterer;

   //3x3 outline image kernel to highlight large differences in pixel values 
   const int kernel[3][3] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}}; //outline image kernel
   kmClusterer.applyKernel(kernel);

    int iterations =100;

    for(int i = 0; i < iterations; i ++){
    kmClusterer.recalculateCentroid();
    kmClusterer.assignToCluster();

    }
    cout << kmClusterer;



   

    return 0;
}