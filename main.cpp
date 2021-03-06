#include "darknet_wrapper/wrapper_cpp.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(){

    char* cfg_path = "test/tiny-yolo-voc.cfg";
    char* weights_path = "test/tiny-yolo-voc.weights";

    // apply the detector to an image file
    char* filename="test/dog.jpg";

    float thresh = 0.4;
    int hits;
    box** outboxes = new box*;
    float** outprobs = new float*;
    int** outclasses = new int*;

    // the first time you request an instance, you have to supply the config and weights
    // you could use Darknet::getInstance(cfg, weights) at the start of your program, to initialize the network
    // and then use Darknet::getInstance() without parameters everywhere else
    // the initialization can take some time, this is only done once

    // apply the detector to a file
    Darknet::getInstance(cfg_path, weights_path).darknet_detect_file(filename, thresh, &hits, outboxes, outprobs, outclasses);





    // apply the detector to an opencv mat
    cv::Mat3b img= cv::imread("test/dog.jpg", cv::IMREAD_UNCHANGED);
    if(!img.data){
        cout<<"couldn't load the test image"<<endl;
    }

    // the detector expects an IplImage
    IplImage convertedImage = img;

    // this is the second time you use the singleton
    // the network object is already created, no need to provide config files
    // if you would provide such files, they would be ignored
    Darknet::getInstance().darknet_detect_img(&convertedImage, thresh, &hits, outboxes, outprobs, outclasses);

    cout<<"hits: "<<hits;
    for (int i = 0; i < hits; i++) {
        box hitbox = (*outboxes)[i];
        int label = (*outclasses)[i];
        float prob = (*outprobs)[i];

        // calculate coordinates of bounding box
        // yolo returns the center of the bbox
        float x, y, w, h;
        x = hitbox.x - hitbox.w / 2;
        y = hitbox.y - hitbox.h / 2;
        w = hitbox.w;
        h = hitbox.h;

        cv::rectangle(img, {x,y}, {x+w, y+h}, {0,0,255}, 10);
    }

    cv::imshow("rectangles in output", img);
    cv::waitKey(0);

    free((*outboxes));
    free((*outprobs));
    free((*outclasses));

    return 0;
}