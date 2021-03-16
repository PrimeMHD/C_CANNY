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
int gaussian_filter(const int* img, int *result, int filter_n, int img_c, int img_h, int img_w, float sigma){
    int filter_h=2*filter_n+1, filter_w=2*filter_n+1;
    float normalizer=0;

   
    //generate gaussian filter
    float **kernel = new float*[filter_h]();
    for(int i=0; i<filter_h; i++){
        kernel[i]=new float[filter_w]();
    }
    //create array to store gaussian filter
    for(int i=0; i<2*filter_n+1; i++){
        for(int j=0; j<2*filter_n+1; j++){
            int x=filter_n-j;
            int y=filter_n-i;
            kernel[i][j]=gaussian2D(x,y,sigma);
        }
    }


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




    int* out_img;
    for(int ch=0; ch<img_c; ch++){
        out_img=mat_conv((float*)*kernel, img+ch*img_h*img_w, filter_h,filter_w,img_h, img_w,filter_n,1);
        memcpy(result+ch*img_h*img_w, out_img, sizeof(int)*img_h*img_w);
        delete out_img;
    }


    return 0;
}

int pixelGradient(const int* img, int *gx, int *gy, int *result, int img_c, int img_h, int img_w){
    int kernel_gx[9]{-1,0,1,-2,0,2,-1,0,1};
    int kernel_gy[9]{-1,-2,-1,0,0,0,1,2,1};
    int* gx_conved=new int[img_c*img_h*img_w]();
    int* gy_conved=new int[img_c*img_h*img_w]();
    // float* gxy=new float[img_c*img_h*img_w]();
    int* temp;
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
                int temp1=gx_conved[ch*img_h*img_w+h*img_w+w];
                int temp2=gy_conved[ch*img_h*img_w+h*img_w+w];
                result[ch*img_h*img_w+h*img_w+w]=int(sqrt(temp1*temp1+temp2*temp2));
            }
        }
    }
    return 0;
}

int pixelGradientSuppression(int* gradient, int* gradient_x, int* gradient_y, int* suppressed_gradient, int channel, int height, int width){
    for(int ch_i=0; ch_i<channel; ch_i++){
        for(int h_i=0; h_i<height; h_i++){
            for(int w_i=0; w_i<width; w_i++){
                //skip image edge
                if(h_i==0||h_i==height-1||w_i==0||w_i==width-1){
                    suppressed_gradient[ch_i*height*width+h_i*width+w_i]=gradient[ch_i*height*width+h_i*width+w_i];
                    continue;
                }


                int gradient_val=gradient[ch_i*height*width+h_i*width+w_i];
                int gradient_val_x=gradient_x[ch_i*height*width+h_i*width+w_i];
                int gradient_val_y=gradient_y[ch_i*height*width+h_i*width+w_i];

                if(gradient_val==0){
                    suppressed_gradient[ch_i*height*width+h_i*width+w_i]=0;
                    continue;
                }
                int gup_interpolation=0, gdown_interpolation=0;
                float tan_g, cos_g, abs_tan_g;
                
                
                if(gradient_val_x==0){
                    gup_interpolation=gradient[ch_i*height*width+(h_i-1)*width+w_i];
                    gdown_interpolation=gradient[ch_i*height*width+(h_i+1)*width+w_i];

                }else if(gradient_val_y==0){
                    gup_interpolation=gradient[ch_i*height*width+h_i*width+w_i-1];
                    gdown_interpolation=gradient[ch_i*height*width+h_i*width+w_i+1];
                }else{//neither gradient_val_x nor gradient_val_y is zero
                    tan_g=gradient_val_y/gradient_val_x;
                    abs_tan_g=abs(tan_g);

                    if(tan_g>0&&tan_g<1){//region 1
                        gup_interpolation=(1-abs_tan_g)*gradient[ch_i*height*width+h_i*width+w_i+1]+abs_tan_g*gradient[ch_i*height*width+(h_i-1)*width+w_i+1];
                        gdown_interpolation=(1-abs_tan_g)*gradient[ch_i*height*width+h_i*width+w_i-1]+abs_tan_g*gradient[ch_i*height*width+(h_i+1)*width+w_i-1];
                    }else if(tan_g>=1){ //region 2
                        gup_interpolation=(1-1/abs_tan_g)*gradient[ch_i*height*width+(h_i-1)*width+w_i+1]+(1/abs_tan_g)*gradient[ch_i*height*width+(h_i-1)*width+w_i];
                        gdown_interpolation=(1-1/abs_tan_g)*gradient[ch_i*height*width+(h_i+1)*width+w_i-1]+(1/abs_tan_g)*gradient[ch_i*height*width+(h_i+1)*width+w_i];
                    }else if(tan_g<=-1){ //region 3
                        gup_interpolation=(1-1/abs_tan_g)*gradient[ch_i*height*width+(h_i-1)*width+w_i-1]+(1/abs_tan_g)*gradient[ch_i*height*width+(h_i-1)*width+w_i];
                        gdown_interpolation=(1-1/abs_tan_g)*gradient[ch_i*height*width+(h_i+1)*width+w_i+1]+(1/abs_tan_g)*gradient[ch_i*height*width+(h_i+1)*width+w_i];
                    }else if(tan_g>-1&&tan_g<0){ //region 4
                        gup_interpolation=(1-abs_tan_g)*gradient[ch_i*height*width+(h_i-1)*width+w_i-1]+abs_tan_g*gradient[ch_i*height*width+h_i*width+w_i-1];
                        gdown_interpolation=(1-abs_tan_g)*gradient[ch_i*height*width+(h_i+1)*width+w_i+1]+abs_tan_g*gradient[ch_i*height*width+h_i*width+w_i+1];
                    }
                }

                if(gradient_val>=gup_interpolation&&gradient_val>=gdown_interpolation){
                    suppressed_gradient[ch_i*height*width+h_i*width+w_i]=gradient_val;
                }else{
                    suppressed_gradient[ch_i*height*width+h_i*width+w_i]=0;
                }



         
            }
        }
    }
    return 0;
}

int weakEdgeControl(int* suppressed_gradient, int* controlled_edge, int channel, int height, int width, int threshold, int brighter){
    for(int ch_i=0; ch_i<channel; ch_i++){
        for(int h_i=0; h_i<height; h_i++){
            for(int w_i=0; w_i<width; w_i++){
                //skip image edge
                if(h_i==0||h_i==height-1||w_i==0||w_i==width-1){
                    controlled_edge[ch_i*height*width+h_i*width+w_i]=suppressed_gradient[ch_i*height*width+h_i*width+w_i];
                    continue;
                }

                if(suppressed_gradient[ch_i*height*width+h_i*width+w_i]>=threshold){
                    controlled_edge[ch_i*height*width+h_i*width+w_i]=suppressed_gradient[ch_i*height*width+h_i*width+w_i];
                    continue; //strong edge
                }

                //weak edge, shoulde judge more
                int neighbor[9];
                for(int i=-1; i<=1; i++){
                    for(int j=-1; j<=1; j++){
                        neighbor[(i+1)*3+ j+1 ]=suppressed_gradient[ch_i*height*width + (h_i+i)*width+ w_i+j];
                    }
                }
                bool should_enhance=false;
                for(int i=0; i<9; i++){
                    if(i==4){
                        continue;
                    }
                    if(neighbor[i]>=threshold){
                        //if one of his neighbor is strong edge, keep this weak edge
                        should_enhance=true;
                    }
                }
                if(should_enhance){
                    controlled_edge[ch_i*height*width+h_i*width+w_i]=suppressed_gradient[ch_i*height*width+h_i*width+w_i];
                }else{
                    controlled_edge[ch_i*height*width+h_i*width+w_i]=0;
                }



            }
        }
    }    
}