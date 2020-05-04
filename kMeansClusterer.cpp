#include "kMeansClusterer.h"

#include <iostream>
#include <map>
#include <cmath>
#include <stdio.h> 
#include <stdlib.h> 
#include<time.h> 


using namespace std; 
    vector <u_char *> images;
    vector<int *> histogramFeatures;
    map<int, int *> clusters; //a cluster is map of a centroid to matching image features (histrograms in this case)

    vector<int> * classification; // an array of vectors to hold the classification into clusters
    vector<u_char *> imageOutlines;
    vector<u_char *> differenceMaps;

    double * distances;
    bool feature = false;
    bool color;

    const string prefixes[10] = {"eight","five","four","nine", "one","seven","six","three", "two", "zero"};
    
    int rows = 0;
    int cols = 0;
    int intervals = 0;
   
    kMeansClusterer::kMeansClusterer(){}
    kMeansClusterer::~kMeansClusterer(){
            for(int i=0; i < images.size(); i++){
               delete [] images[i]; //delete pointers to images
            }
              for (auto &&pair : clusters)
            {
                delete [] pair.second; //delete the histogram to which the pointer points
            }
              for (auto &&outline : imageOutlines)
            {
                delete [] outline; //delete the histogram to which the pointer points
            }
            //    for (auto &&differenceMap : differenceMaps)
            // {
            //     delete [] differenceMap; //delete the histogram to which the pointer points
            // }
            for(int i = 0; i < histogramFeatures.size(); i++){ //delete pointers to histograms, i.e. the image features
                delete [] histogramFeatures[i];
            }
            delete [] distances;
            delete [] classification;
    
       cout << endl << "Cleaning up...\nMemory Freed!"<<endl; 
    }

    void kMeansClusterer::setColor(const bool c){
        color = c;
    }

    void kMeansClusterer::readPPMimages(const string folder){ //store image as a flattened 1D array in an array of pointers to each image
        cout << "\nLooking in the folder: ../"<<folder<< "/ for images."<<endl;
        
       
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
   
          }
     cout << "Images converted to grey scale"<<endl;
    }

     void kMeansClusterer::writeToFile(const string filename){
         int index = 0;
         
        ofstream ofs;
         for (auto &&image : images)
         {
          ofs.open("../output/"+filename+"_"+to_string(index)+".ppm", ios::binary);
          ofs << "P5\n"<<"32 32\n"<<"255\n";

          for(int i = 0; i < rows*cols; i++){
              ofs<<image[i];

          }
         
            int length = rows*cols;
            if(color){length = length*3;}
           
          //   ofs.write((char *)image, length);
             
            index ++;
            ofs.close();       
         }
    
         
    }

    void kMeansClusterer::printImageGrid(){ //print grey scale images
        for (auto && image : images)
        { int length = rows*cols;
          int mod = 32;
            if(color){length = rows*cols*3;
                       mod = 32*3;}
            for(int i = 0; i < length; i++){
                if (i%mod == 0)cout << "\n";
                cout << to_string(image[i]) <<" ";
            }
        }
    }
     void kMeansClusterer::generateHistograms(const int bin){
     //range of histogram is 0-255
     
     
     intervals = 256/bin;
     int length = rows*cols;
     if(color){
         intervals += 3; //1 columnn each for R, G, and B
         length*=3;
         cout<<"Three columns added do histogram for R, G and B counts."<<endl;
     }
    
     cout <<"Intervals in histogram feature: " <<intervals<<", with bin size: "<<bin<<endl;
     int index = 0;

             
     for (auto &&image : images) //for each image
     {  int * histogram = new int[intervals]{0}; //new array to hold the histogram feature of each image, 3 more if RGB
         
         if(color){
             for(int i = 0; i < length; i++){ //loop through each 1D array which represents each greyscale image
                 //increment RGB intensity totals accordingly
                 if((i%3)==0){ //R
                     histogram[intervals-3]+=image[i]; //add intensity at i to the 3rd last index of histogram which represents RED
                     //cout << histogram[intervals-3]<< " ";
                 }else if((i%3)==1){ //G
                     histogram[intervals-2]+=image[i]; //add intensity at i to the 2nd last index of histogram which represents GREEN
                 }
                 else if((i%3)==2){//B
                     histogram[intervals-1]+=image[i]; //add intensity at i to the last index of histogram which represents BLUE
                 }
             }
         } //end if(color)

         for(int i = 0; i < length; i++){ //loop through each 1D array which represents each greyscale image
                
            for(int j = -1; j < 255; j+=bin){ //loop over intervals of intensity
                if(j+1<= image[i]<=j+bin){ //if the image intensity lies within the range
                 index = image[i]/bin; //calculate index of histogram array to increment
                }//end if
            } //end for
                histogram[index]++; //increment the relevant index of the histogram
     } //end interation of image element

     histogramFeatures.push_back(histogram); //add corresponding histogram for each image
     
    }   //outer for auto
    
 } //end void

 void kMeansClusterer::assignToCluster(){
     //loop through all image histograms and find cluster with which it has smallest euclidean distance
     for(int i = 0; i < clusters.size(); i++){
         classification[i].clear(); //empty each cluster before reassigning
     }

     double min = MAXFLOAT;
     int index = 0;
     int outerIndex = 0; //tracks the image number
     double euclideanDistance = 0;
     int startPoint = 0;
     if(color)startPoint = intervals-3;

     for (auto &&image : histogramFeatures)  //loop through the image histograms
     {   
          min = MAXFLOAT;
         for (auto &&cluster : clusters) //compare each image histogram to each cluster
         {  
             euclideanDistance = 0.0;
             for(int i = startPoint; i < intervals; i++){ //for each interval of each histogram
             euclideanDistance += pow((cluster.second[i]-image[i]), 2); // (x-u)^2
             
             }    
             euclideanDistance = sqrt(euclideanDistance);  //the final euclidean distance which we wish to minimise for the cluster
             if(euclideanDistance < min){
                 min = euclideanDistance;
                 index = cluster.first; // id of cluster is where we must assign the image
             }     
         } //we should have our desired cluster id at this point. i is in the range 0 - k
         classification[index].push_back(outerIndex); //push image id on the the vector at the cluster id
       outerIndex++;
     }
 }

 void kMeansClusterer::recalculateCentroid(){
     //calculate mean histogram of images incluster then assign to cluster

     int imagesInCluster = 0;
     int startPoint = 0;
     int endPoint = intervals;
     if(color)startPoint = intervals-3;
     if(feature)endPoint = 1;
     for (auto &&pair : clusters)
     {    
         int * newCentroid = new int[intervals]{0}; //new array in memory to store cluster histogram
         imagesInCluster = classification[pair.first].size(); //returns number of images in the cluster
    
         for(int i = 0; i < imagesInCluster; i++){ //loop over each image in the cluster to get the mean
            int imageIndex = classification[pair.first][i]; 

            for(int j = startPoint; j < endPoint; j++){ //find the mean of each interval of images in the cluster
                newCentroid[j] += histogramFeatures[imageIndex][j];
            }
         }
            for(int j = 0; j < endPoint; j++){ //find the mean of each interval of images in the cluster
                if(newCentroid[j] !=0)  newCentroid[j] = newCentroid[j]/imagesInCluster; //take the mean
            }
         delete [] pair.second; //delete pointer to old centroid
         pair.second = newCentroid; //assign new centroid
     } //end cluster loop
     
 }

 void kMeansClusterer::createInitialClusters(const int k){ //creates random centroids initially for each cluster
 //randomly assign an image ID 0-99 to a classification vector
    classification = new vector<int>[k]; //aray of k vectors, each vector holds to the ID of the images in the cluster
    srand(time(0));

    for(int i = 0; i < k; i++){
        int * randomHistogram = new int[intervals];
        
        int r = (rand()%100)+0;

        for(int i = 0;i < intervals; i ++){ //TO PREVENT ALIASING OF MEMORY
            randomHistogram[i] = histogramFeatures[r][i]; //assign a random value in the data-set
        }
        clusters.insert(pair<int , int *>(i, randomHistogram));
        } //end for
       }

 void kMeansClusterer::applyKernel(const int kernel[3][3]){ //must be applied on a grey scale image
    int length = rows*cols;
    int c =cols;

    if(color){
        length *=3;
        c*=3;
    }
     for (auto &&image : images)
     {
         u_char image2D[rows][c]; //hold image in 2D array form, padding not needed since boundary values are always zero anyway
         u_char * imageOutline = new u_char[length]; //holds the output

         int index = 0;
         for(int i = 0; i < rows; i++){
            for(int j = 0; j < c; j++){
                image2D[i][j] = image[index];
                index++;
         }
         }
         index = 0;
         for(int i = 0; i < rows; i++){
             for(int j = 0; j < c; j++){
                 imageOutline[index] = 0; // give value of zero on boundary conditions
                 if(i!=0 && i!=(rows-1) && j !=0 && j!=(c-1)){ //i.e. dont perform the calculation on the borders of the image, they're all zero anyway
                    
                    u_char output = image2D[i][j];
                    if(image2D[i][j]>1){ //apply kernel to only values on the edge of the image
                         output = kernel[0][0]*image2D[i-1][j-1]+kernel[0][1]*image2D[i-1][j]+kernel[0][2]*image2D[i-1][j+1]+kernel[1][0]*image2D[i][j-1]+ 
                              (kernel[1][1]*image2D[i][j]) +kernel[1][2]*image2D[i][j+1]
                             +kernel[2][0]*image2D[i+1][j-1]+kernel[2][1]*image2D[i+1][j]+kernel[2][2]*image2D[i+1][j+1];

                        output +=100;
                    }
                    
                  imageOutline[index] = output;     
                  //cout << to_string(output)<<" ";    
                 }   
                
                 index++;
         } //end cols
        // cout<<endl;
         }
        // imageOutlines.push_back(imageOutline);
        delete [] image;
        image = imageOutline;
      
     } //end outer for
 }
 void kMeansClusterer::differenceMap(){
     int count = 0;
     for (auto &&image : images)
     {
          int length = rows*cols;
          int c =cols;
         
         u_char image2D[rows][c]; //hold image in 2D array form, padding not needed since boundary values are always zero anyway
         u_char * differenceMap = new u_char[length]; //holds the output
      

         int index = 0;
         for(int i = 0; i < rows; i++){ //put in 2D array format for easy windoe passing
            for(int j = 0; j < c; j++){
                image2D[i][j] = image[index];
                index++;
         }
         }
         index = 0;
         for(int i = 0; i < rows; i++){
             for(int j = 0; j < c; j++){
                 differenceMap[index] = 0; // give value of zero on boundary conditions
                 if(i!=0 && i!=(rows-1) && j !=0 && j!=(c-1)){ //i.e. dont perform the calculation on the borders of the image, they're all zero anyway
                    //subtract all neighbouring values in 3x3 grid to obtain the difference a value has with its neighbours
                    // int output = image2D[i][j]- min(image2D[i-1][j-1],image2D[i][j])  +image2D[i][j]-  min(image2D[i-1][j],image2D[i][j])  +image2D[i][j]-  min(image2D[i-1][j+1],image2D[i][j] ) + min(image2D[i][j-1],image2D[i][j] ) 
                    //            + image2D[i][j]- min(image2D[i][j+1],image2D[i][j] ) + image2D[i][j]- min(image2D[i+1][j-1],image2D[i][j] ) +
                    //            image2D[i][j]-  min(image2D[i+1][j],image2D[i][j] ) +image2D[i][j]-  min(image2D[i+1][j+1],image2D[i][j]); 
                    int val = image2D[i][j];
                    u_char output = 0;
                    if(val != 0){
                        for(int k = -1; k < 2; k++){
                            for(int m = -1; m<2; m++){
                                if(image2D[i+k][j+m]==0){
                                    output+=1; //count the number of zeros surrounding a value, if that value is no zero to begin with
                                }
                            }
                        }
                    }
                    //the higher the difference value, the more likely to be on a boundary, but != 0  
                  differenceMap[index] = output;   
                 
                  
                 }   
                 index++;
         } //end cols
         

         }
   //   differenceMaps.push_back(differenceMap);

      delete [] image;
      image = differenceMap;
         count ++;
       
     } //end outer for

 }
 void kMeansClusterer::findDistanceBetweenExtremeMaximums(){
     distances = new double[100]; //one distance to describe each image, 100 elements long
     int counter = 0;
     double average = 0.0;
     for (auto &&diffMap : images) //for each difference map
     {
         
         int minX, minY, maxX, maxY; //store these values to calculate final distance

         int diff2D[rows][cols];
         int index = 0;

          for(int i = 0; i < rows; i++){ //put in 2D array format for easy windoe passing
            for(int j = 0; j < cols; j++){
                diff2D[i][j] = diffMap[index];
                index ++;
            
         }//end for 
     } //end for

     //FIRST FIND THE MAX VALUE WE ARE LOOKING FOR
     int maxValue=0;
     for(int i = 0; i < rows; i++){ //put in 2D array format for easy windoe passing
            for(int j = 0; j < cols; j++){
                if(diff2D[i][j]>maxValue) maxValue = diff2D[i][j]; 
               // cout<< maxValue<< " ";
                
         }//end for 
     } //end for
    
    // cout <<endl;
     //FIND X,Y VALUE OF EXTREME VALUES 
     double minDistance = MAXFLOAT;
     double maxDistance = -1.0;
     int maxValueCount = 0;

     for(int i = 0; i < rows; i++){
         for(int j = 0; j< rows; j++){
             if(diff2D[i][j]==maxValue) maxValueCount+=1;
         }
     }

     if(maxValueCount==1)maxValue -=1;
    
     for(int i = 0; i < rows; i++){ //put in 2D array format for easy windoe passing
            for(int j = 0; j < cols; j++){
                int distance = 0;
                if(diff2D[i][j]==maxValue){ //find distance if true
                    distance = sqrt(pow(i,2)+pow(j,2)); //distance from the origin, the origin is the top left of the image

                    if(distance<minDistance){
                        minDistance = distance;
                        minX = i;
                        minY = j;
                    }
                    if(distance>maxDistance){
                        maxDistance = distance;
                        maxX = i;
                        maxY = j;
                    }
                } //end if 
         }//end for 
     } //end for

     
     double extremeDistance = sqrt(pow((maxX-minX), 2)+pow((maxY-minY),2));
     distances[counter] = extremeDistance;
     //cout<<counter<<": "<<extremeDistance<<" ";
     average += extremeDistance;

     if(counter%10 ==0 && counter>0){
         cout << average/10<<endl;
         average = 0.0;
     }
     counter ++;

 } //end outer for
 }

 int kMeansClusterer::min(const int x, const int y){
     if(x<y) return x;
     return y;
 }


ostream& operator<<(ostream& os, const kMeansClusterer& kt){
     os << endl;

     for(int i = 0; i < clusters.size(); i++){
         os <<"cluster "<<i<<": ";
         for(int j = 0; j < classification[i].size(); j++){
             for(int k = 0; k< 100; k++){ //print the necesarry prefix
                if(k==classification[i][j]){
                    int index = classification[i][j]/10;
                    os << prefixes[index]<< "_";
                }
             }
             os << classification[i][j]%10<< "  ";
         }
         os << endl;
     }
     return os;
 }

 
