#include <iostream>
#include <math.h>
#include "matrix.h"
#include "canny_filter.h"
using namespace std;
float gaussian2D(float x, float y, float sigma){
    float theta1=1.0/(2*M_PI*sigma*sigma);
    float theta2=1.0/(2*sigma*sigma);
    return theta1*exp(-theta2*(x*x+y*y));
}
int gaussian_filter(const DataType* img, float *result, int filter_n, int img_c, int img_h, int img_w, float sigma){
    int filter_h=2*filter_n+1, filter_w=2*filter_n+1;
   

   
    //generate gaussian filter
    DataType **kernel = new DataType*[filter_h]();
    for(int i=0; i<filter_h; i++){
        kernel[i]=new DataType[filter_w]();
    }
    //create array to store gaussian filter
    float normalizer=0;
    for(int i=0; i<2*filter_n+1; i++){
        for(int j=0; j<2*filter_n+1; j++){
            int x=filter_n-j;
            int y=filter_n-i;
            kernel[i][j]=gaussian2D(x,y,sigma);
        }
    }

    // for(int i=0; i<filter_n; i++)
    // {
    //     for(int j=0; j<filter_n; j++)
    //     {
    //         kernel[filter_n+i][filter_n+j] = gaussian2D(i,j,sigma);
            
    //         if(i!=0&&j!=0){
    //             kernel[filter_n+i][filter_n-j] = gaussian2D(i,j,sigma);
    //             kernel[filter_n-i][filter_n+j] = gaussian2D(i,j,sigma);
    //             kernel[filter_n-i][filter_n-j] = gaussian2D(i,j,sigma);
    //         }

    //     }
    // }
    for(int i=0; i<2*filter_n+1; i++){
        for(int j=0; j<2*filter_n+1; j++){
            normalizer+=kernel[i][j];
        }
    }
    for(int i=0; i<2*filter_n+1; i++){
        for(int j=0; j<2*filter_n+1; j++){
            kernel[i][j]/=normalizer;
        }
    }




    float* out_img;
    for(int ch=0; ch<img_c; ch++){
        out_img=mat_conv((float*)*kernel, img+ch*img_h*img_w, filter_h,filter_w,img_h, img_w,filter_n,1);
        memcpy(result+ch*img_h*img_w, out_img, sizeof(float)*img_h*img_w);
        delete out_img;
    }


    return 0;
}

int pixelGradient(const float* img, float *result, int img_c, int img_h, int img_w){
    float kernel_gx[9]{-1,0,1,-2,0,2,-1,0,1};
    float kernel_gy[9]{-1,-2,-1,0,0,0,1,2,1};
    float* gx_conved=new float[img_c*img_h*img_w]();
    float* gy_conved=new float[img_c*img_h*img_w]();
    // float* gxy=new float[img_c*img_h*img_w]();
    float* temp;
    for(int ch=0; ch<img_c; ch++){
        temp=mat_conv(kernel_gx, img+ch*img_h*img_w, 3,3, img_h, img_w,1,1);
        memcpy(gx_conved+ch*img_h*img_w, temp, sizeof(float)*img_h*img_w);
        delete temp;

        temp=mat_conv(kernel_gy, img+ch*img_h*img_w, 3,3, img_h, img_w,1,1);
        memcpy(gy_conved+ch*img_h*img_w, temp, sizeof(float)*img_h*img_w);
        delete temp;
    }
    for(int ch=0; ch<img_c; ch++){
        for(int h=0; h<img_h; h++){
            for(int w=0; w<img_w; w++){
                float temp1=gx_conved[ch*img_h*img_w+h*img_w+w];
                float temp2=gy_conved[ch*img_h*img_w+h*img_w+w];
                result[ch*img_h*img_w+h*img_w+w]=sqrt(temp1*temp1+temp2*temp2);
            }
        }
    }
    return 0;
}

int pixelGradientSuppression(float* gradient, )