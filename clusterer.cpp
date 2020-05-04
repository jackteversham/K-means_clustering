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
    string output = "";
    bool color = false;
    bool customFeature = false;
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
        str = "-f";
        if(argv[i] == str){
            customFeature = true;
        }
        str = "-o";
        if(argv[i] == str){
            output = argv[i+1];
        }
        
       
    }

    //3x3 outline image kernel to highlight large differences in pixel values
    const int kernel[3][3] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}}; //outline image kernel
    kmClusterer.readPPMimages(folder);

    if(!color){
        kmClusterer.convertToGreyScale(); 
        
        if(customFeature){
      
         kmClusterer.differenceMap(); 
         kmClusterer.applyKernel(kernel);

         kmClusterer.findDistanceBetweenExtremeMaximums();
         const bool printOne = true;
         kmClusterer.printImageGrid(printOne);

        }


    }else{
        kmClusterer.setColor(true);  //classifiy using RGB
       
    }

    

  
    kmClusterer.generateHistograms(stoi(bin));
    kmClusterer.createInitialClusters(stoi(k));
    kmClusterer.assignToCluster();
    

   // cout << kmClusterer;


    
    while(!kmClusterer.hasConverged()){ 
        kmClusterer.recalculateCentroid();
    kmClusterer.assignToCluster();
    

    }
    cout << kmClusterer;

    kmClusterer.writeToFile("out");
    
    ofstream ofs;
    ofs.open(output+".txt");
    ofs << kmClusterer;
    ofs.close();

    cout << "\n\nClusters written to "<<output<<".txt, located in the src folder"<<endl;

    

    return 0;
}