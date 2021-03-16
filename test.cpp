#include <iostream>
#include <fstream>
#include "canny_filter.h"
#define OUTPUT_BLURED
#define OUTPUT_PIXEL_GRADIENT
#define OUTPUT_SUP_GRADIENT
#define OUTPUT_RMWEAK_GRADIENT
using namespace std;
int main(int argc, char*argv[]){
    //load an image 
    const int IMG_CHANNEL=1, IMG_HEIGHT=300, IMG_WEIGHT=300;
    int *img=new int[IMG_CHANNEL*IMG_HEIGHT*IMG_WEIGHT]();
    int *processed_img=new int[IMG_CHANNEL*IMG_HEIGHT*IMG_WEIGHT]();
    int *pixel_gradients=new int[IMG_CHANNEL*IMG_HEIGHT*IMG_WEIGHT]();
    int *pixel_gradients_x=new int[IMG_CHANNEL*IMG_HEIGHT*IMG_WEIGHT]();
    int *pixel_gradients_y=new int[IMG_CHANNEL*IMG_HEIGHT*IMG_WEIGHT]();
    int *suppressed_pixel_gradients=new int[IMG_CHANNEL*IMG_HEIGHT*IMG_WEIGHT]();
    int *suppressed_rmweak_pixel_gradients=new int[IMG_CHANNEL*IMG_HEIGHT*IMG_WEIGHT]();


    ifstream ifs;
    ofstream ofs;
    ifs.open("./Imgs/duck.txt",ios::in);
    for(int c=0; c<IMG_CHANNEL; c++){
        for(int h=0; h<IMG_HEIGHT; h++){
            for(int w=0; w<IMG_WEIGHT; w++){
                ifs>>img[c*IMG_HEIGHT*IMG_WEIGHT+h*IMG_WEIGHT+w];
            }
        }
    }
    ifs.close();
    
    gaussian_filter(img, processed_img, 1, IMG_CHANNEL, IMG_HEIGHT, IMG_WEIGHT);
    #ifdef OUTPUT_BLURED
    ofs.open("./Imgs/duck_blured.txt",ios::out);
    for(int c=0; c<IMG_CHANNEL; c++){
        for(int h=0; h<IMG_HEIGHT; h++){
            for(int w=0; w<IMG_WEIGHT; w++){
                ofs<<processed_img[c*IMG_HEIGHT*IMG_WEIGHT+h*IMG_WEIGHT+w]<<'\n';
            }
        }
    }
    ofs.close();
    #endif
    pixelGradient(processed_img, pixel_gradients_x,pixel_gradients_y,pixel_gradients,IMG_CHANNEL, IMG_HEIGHT, IMG_WEIGHT);
    #ifdef OUTPUT_PIXEL_GRADIENT
    ofs.open("./Imgs/duck_blured_gradient.txt",ios::out);
    for(int c=0; c<IMG_CHANNEL; c++){
        for(int h=0; h<IMG_HEIGHT; h++){
            for(int w=0; w<IMG_WEIGHT; w++){
                ofs<<pixel_gradients[c*IMG_HEIGHT*IMG_WEIGHT+h*IMG_WEIGHT+w]<<'\n';
            }
        }
    }
    ofs.close();
    #endif

    
    pixelGradientSuppression(pixel_gradients,pixel_gradients_x,pixel_gradients_y,suppressed_pixel_gradients, IMG_CHANNEL, IMG_HEIGHT, IMG_WEIGHT);
    #ifdef OUTPUT_SUP_GRADIENT
    ofs.open("./Imgs/duck_blured_gradient_sup.txt",ios::out);
    for(int c=0; c<IMG_CHANNEL; c++){
        for(int h=0; h<IMG_HEIGHT; h++){
            for(int w=0; w<IMG_WEIGHT; w++){
                ofs<<suppressed_pixel_gradients[c*IMG_HEIGHT*IMG_WEIGHT+h*IMG_WEIGHT+w]<<'\n';
            }
        }
    }
    ofs.close();
    #endif

    
    weakEdgeControl(suppressed_pixel_gradients, suppressed_rmweak_pixel_gradients, IMG_CHANNEL, IMG_HEIGHT, IMG_WEIGHT, 30);
    #ifdef OUTPUT_RMWEAK_GRADIENT
    ofs.open("./Imgs/duck_blured_gradient_sup_rmweak.txt",ios::out);
    for(int c=0; c<IMG_CHANNEL; c++){
        for(int h=0; h<IMG_HEIGHT; h++){
            for(int w=0; w<IMG_WEIGHT; w++){
                ofs<<suppressed_rmweak_pixel_gradients[c*IMG_HEIGHT*IMG_WEIGHT+h*IMG_WEIGHT+w]<<'\n';
            }
        }
    }
    ofs.close();
    #endif

    delete[] processed_img;
    delete[] pixel_gradients;
    delete[] pixel_gradients_x;
    delete[] pixel_gradients_y;
    delete[] suppressed_pixel_gradients;
    delete[] suppressed_rmweak_pixel_gradients;
    return 0;
}