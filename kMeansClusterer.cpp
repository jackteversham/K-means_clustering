#include "kMeansClusterer.h"

#include <iostream>


using namespace std; 
    vector <u_char *> images;
    vector<int *> histogramFeatures;
    int rows = 0;
    int cols = 0;


    kMeansClusterer::kMeansClusterer(){}
    kMeansClusterer::~kMeansClusterer(){
            for(int i=0; i < images.size(); i++){
               delete [] images[i]; //delete pointers to images
            }
       //delete [] images;
       cout << endl << "Cleaning up...\nMemory Freed!"<<endl; 
    }

    void kMeansClusterer::readPPMimages(const string folder){ //store image as a flattened 1D array in an array of pointers to each image
        cout << "\nLooking in the folder: ../"<<folder<< "/ for images."<<endl;
        
        const string prefixes[10] = {"eight","five","four","nine", "one","seven","six","three", "two", "zero"};
       
        for(int i=0; i < sizeof(prefixes)/sizeof(prefixes[0]); i++){ //for each prefix i.e. 10

            for(int k = 1; k<11; k++){ //for each sub_prefix
               //int counter = 0;
                ifstream input("../"+folder+"/"+prefixes[i]+"_"+to_string(k)+".ppm");
            
                if(!input){
                   cout <<"fileNotFound"<<endl;
                }
                string start="";
                string endline;
                int end = 0;
                
                //Read header
                input >> start >> ws;
                input >>rows >> ws >> cols;
                input >> end>>ws;
            
            
                u_char * imagePointer = new u_char[rows*cols*3]; //array of pointers to 1D array of each image
                
                input.read((char *)imagePointer, rows*cols*3); //read in all the data for one image and store in imagePointer
                images.push_back(imagePointer);

                // for(int m =0; m< rows*cols*3; m++){
                //     counter+=sizeof(imagePointer[m]);
                // }
                //cout <<counter<<endl;

                input.close(); //close input stream
            }
        }
        cout << images.size()<<" images loaded in from file."<<endl;
    }

    void kMeansClusterer::convertToGreyScale(){
       // cout<<images.size()<<endl;
    
       
       for(int j = 0; j < images.size(); j++ ){
            u_char * greyScaleImage = new u_char[rows*cols];
            int index = 0;

       for(int i = 0; i < rows*cols*3; i+=3){
           u_char * imagePointers = images[j];
           greyScaleImage[index] =(u_char) 0.21*imagePointers[i]+0.72*imagePointers[i+1]+0.07*imagePointers[i+2];
            //cout << to_string(greyScaleImage[index])<<" ";
           index++;
          
       }
       delete [] images[j]; //delete colour image 
       images[j] = greyScaleImage; //replace with greyed out image
      
       
    //    int counter = 0;
    //    for(int m =0; m< rows*cols; m++){
    //              counter+=sizeof(images[j][m]);
    //              cout <<to_string(images[j][m])<<" ";
    //             }
    //         cout <<counter<<endl;
    
    //    }
   
    }
     cout << "Images converted to grey scale"<<endl;
    }

     void kMeansClusterer::writeToFile(){
         ofstream ofs("output.ppm");
         ofs.write((char*)images[0], rows*cols);
         ofs.close();
    }

    void kMeansClusterer::printImageGrid(){ //print grey scale images
        for (auto && image : images)
        {
            for(int i = 0; i < rows*cols; i++){
                if (i%32 == 0)cout << "\n";
                cout << to_string(image[i]) <<" ";
            }
        }
    }

     void kMeansClusterer::generateHistograms(const int bin){
     //range of histogram is 0-255
     int intervals = 256/bin;
     cout <<"Intervals in histogram feature: " <<intervals<<", with bin size: "<<bin<<endl;
     int index = 0;

     

     for (auto &&image : images) //for each image
     {
         int * histogram = new int[intervals]; //new array to hold the histogram feature of each image

         for(int i = 0; i < rows*cols; i++){ //loop through each 1D array which represents each greyscale image

            for(int j = -1; j < 255; j+=bin){ //loop over intervals of intensity
                if(j+1<= image[i]<=j+bin){ //if the image intensity lies within the range
                 index = image[i]/bin; //calculate index of histogram array to increment
                 
                }//end if
               
            }
             if(index>0){
                      cout << index << "-------"<<endl;
                 }
                histogram[index]++; //increment the relevant index of the histogram
     } //end interation of image element
    
     histogramFeatures.push_back(histogram); //add corresponding histogram for each image
    }  //outer for auto   

    int * hist = histogramFeatures[0];
    for(int i =0; i < intervals; i++){
        cout << i << " "<< hist[i];
        for(int j = 0; j < hist[i]; j++){
            cout<< "|";
        }
        cout << endl;
    }

    cout << histogramFeatures.size();
 } //end void
