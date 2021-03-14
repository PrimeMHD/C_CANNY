/**
 * author: HD Mi
 * email: primemhd@foxmail.com
 * This file
 * 
*/


#pragma once
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cstring>
using namespace std;



typedef int*  int_matrix;
typedef float* float_matrix;
typedef double* double_matrix;

template <class T>
T* mat_dot(const T* const m1, const T* const m2, const int row1, const int col1, const int row2, const int col2){
    /********************************
    Args:
        T* m1: first oprand of matrix multiplication
        T* m2: second oprand of  matrix multiplication 
        int row1, col1: row and col numbers of the 1st matrix
        int row2, col2: row and col numbers of the 2nd matrix
    Returns:
        T* outcom: the output of matrix multiplication, which is a 2-D matrix 
    Note:
        eg. m1 with shape of (row1, col1), m2 with shape of (row2, col2),
            the result of m1xm2 should be a maxtrix with shape of (row1, col2)
        OF COURSE, col1==row2 must be guaranteed.
    *********************************/
   //TODO: shape check
   if(col1!=row2){
       cout<<"Illegal Matrix Multiply!"<<endl;
       return nullptr;
   }

    T* outcom=new T[row1*col2]();
    for(int i=0; i<row1; i++){
        for(int j=0; j<col2; j++){
            for(int k=0; k<col1; k++){
                outcom[i*col2+j]+=m1[i*col1+k]*m2[k*col2+j];
            }
        }
    }
    return outcom;

}

template <class T>
T* mat_add(const T* const m1, const T* const m2, int row1, int col1, int row2, int col2){
    /***********************
    拥有广播功能的矩阵相加
    Note:目前仅支持单广播
    ***********************/
    int out_row,out_col;


    T* temp_extend_mat;
    T* temp_mat2;

    if(row1==row2&&col1==col2){
        out_row=row1;
        out_col=col1;
        temp_extend_mat=new(nothrow)T[row1*col1];
        temp_mat2=new(nothrow)T[row2*col2];
        memcpy(temp_extend_mat,m1,row1*col1*sizeof(T));
        memcpy(temp_mat2,m2,row2*col2*sizeof(T));

        // temp_extend_mat=m1;
        // temp_mat2=m2;
    }
    else if(row1==row2&&col1!=col2){
        if(col1==1){
            temp_extend_mat=new(nothrow)T[row1*col2];
            for(int j=0;j<col2;j++){
                for(int i=0;i<row1;i++){
                    temp_extend_mat[i*col2+j]=m1[i];
                }
            }
            out_row=row1;
            out_col=col2;
            // temp_mat2=m2;
    
            temp_mat2=new(nothrow)T[row2*col2];
            memcpy(temp_mat2,m2,row2*col2*sizeof(T));
        }
        else if(col2==1){
            temp_extend_mat=new(nothrow)T[row1*col1];
            for(int j=0;j<col1;j++){
                for(int i=0;i<row1;i++){
                    temp_extend_mat[i*col1+j]=m2[i];
                }
            }
            out_row=row1;
            out_col=col1;
            // temp_mat2=m1;
                    
            temp_mat2=new(nothrow)T[row1*col1];
            memcpy(temp_mat2,m1,row1*col1*sizeof(T));
        }
        else{
            printf("Error!");
        }
        
    }
    else if(col1==col2&&row1!=row2){
        if(row1==1){
            temp_extend_mat=new(nothrow)T[col1*row2];
            for(int i=0;i<row2;i++){
                for(int j=0;j<col1;j++){
                    temp_extend_mat[i*col1+j]=m1[i];
                }
            }
            out_row=row2;
            out_col=col1;
            // temp_mat2=m2;

                    
            temp_mat2=new(nothrow)T[row2*col2];
            memcpy(temp_mat2,m2,row2*col2*sizeof(T));

        }else if(row2==1){
            temp_extend_mat=new(nothrow)T[col1*row1];
            for(int i=0;i<row1;i++){
                for(int j=0;j<col1;j++){
                    temp_extend_mat[i*col1+j]=m2[i];
                }
            }
            out_row=row1;
            out_col=col1;
            // temp_mat2=m1;
            temp_mat2=new(nothrow)T[row1*col1];
            memcpy(temp_mat2,m1,row1*col1*sizeof(T));
        }else{
            printf("Error!");
        }
    }else{
        //TODO:
        printf("Error!");
    }
    
    
    T* out = new(nothrow)T[out_row*out_col];

    for(int i=0;i<out_row;i++){
        for(int j=0;j<out_col;j++){
            out[i*out_col+j]=temp_extend_mat[i*out_col+j]+temp_mat2[i*out_col+j];
        }
    }
    //release temp memory
    delete temp_mat2;
    delete temp_extend_mat;
    return out;
}



/**
 * Note: mat_conv_narrow is deprecated, use mat_conv with specific parameters to archive mat_conv
 * 
/
// template <class T>
// T* mat_conv_narrow(const T* const m1,const T* const m2,const int row1,const int col1,const int row2,const int col2){
//     /********************************
//     Args:
//         T* m1: 卷积核
//         T* m2: 被卷积的矩阵
//         int row1, col1: 第一个矩阵的行列维度
//         int row2, col2: 第二个矩阵的行列维度
//     Returns:
//         T* outcom: 窄卷积的计算结果，二维矩阵
//     Note:
//         Narrow Convolution的计算函数，所谓Narrow的定义是padding=0, strides=1
//         本函数执行的操作用数学语言来表达为：m1(*)m2   这里用(*)来表示“不翻转窄卷积”
//         本函数使用了函数模板，适用于不同数据类型的二维矩阵的相乘。
//         T*类型是一个指针，例如：矩阵元素是整数类型的，那么T*应当是int *类型。
//     m1的形状为(row1, col1), m2的形状为(row2, col2)
//     *********************************/

//    //TODO:形状检查，非法处理
//     const int U = row1;
//     const int V = col1;
//     const int M = row2;
//     const int N = col2;
//     const T* const W=m1;
//     const T* const X=m2;
//     //变量换名，仅仅为了方便对应数学公式



//     int out_row=M-U+1;
//     int out_col=N-V+1;
//     T* out=new T[out_row*out_col]();  //注意这里要加括号，否则不会初始化为0


//     for(int i=0;i<out_row;i++){
//         for(int j=0;j<out_col;j++){
            
//             for(int u=0;u<U;u++){
//                 for(int v=0;v<V;v++){

//                     // int index_i, index_j, index_u, index_v = i-1, j-1, u-1, v-1;
//                     // out[index_i*out_col+index_j]+=W[index_u*col1+index_v] * X[(index_i+index_u-1)*col2+(index_j+index_v-1)];
//                     out[i*out_col+j]+=W[u*V+v] * X[(i+u-1)*col2+(j+v-1)];

//                 }

//             }

//         }
//     }
    
//    return out;
// }



template <class T1, class T2>
T2* mat_conv(const T1* const m1, const T2* const m2, const int row1, const int col1, const int row2, const int col2, const int padding, const int strides){
    /********************************
    Args:
        T* m1: conv filters
        T* m2: 2-D matrix to be convoluted
        int row1, col1: row and col numbers of m1
        int row2, col2: row and col numbers of m2
        int paddings: padding
        int strides: strides
    Returns:
        T* outcom: the outcom of general convolution, 2-D matrix
    Note:
        Function of General Convolution
        The operation performed by this function is expressed in mathematical language as: m1(*)m2 
        Here (*) is used to mean "no flip wide convolution"
    *********************************/

   //TODO: shape check, legality check
    const int U = row1;
    const int V = col1;
    const int M = row2;
    const int N = col2;
    const T1* const W=m1;
    const T2* const X=m2;
    //change the name 


    const int out_row=(M+2*padding-U)/strides+1;
    const int out_col=(N+2*padding-V)/strides+1;  //Round down
    T2* out=new T2[out_row*out_col]();  //Note that brackets is necessary, otherwise it will not be initialized to 0


    //i，j is the subscript of the out element

    for(int i=0; i<out_row; i++){
        for(int j=0; j<out_col;j++){
            
            int window_r=-padding+i*strides;
            int window_c=-padding+j*strides;

            for(int u=0;u<U;u++){
                for(int v=0;v<V;v++){

 

                    T2 temp2;
                    if(window_r+u<0||window_r+u>=M||window_c+v<0||window_c+v>=N){//if falls in to padding area 
                        temp2=0;
                    }else{
                        temp2=X[(window_r+u)*N+(window_c+v)];
                    }

                    out[i*out_col+j]+=W[u*V+v] * temp2;


                }

            }

        }
    }

    return out;
}

template <class T>
T* conv_gradient_m1(const T* const g_fy, const T* const m2, const int row1, const int col1, const int row2, const int col2, const int padding, const int strides){
    /********************************
    Args:
        T* g_fy: the gradient of f(Y)
        T* m2: matrix convoluted in forward propagation
        int row1, col1: row and col numbers of g_fy
        int row2, col2: row and col numbers of m2
        int paddings: padding
        int strides: strides
    Returns:
        T* out: dW
    Note:
    *********************************/
    //TODO:Shape and legality check
    const int U = row1;
    const int V = col1;
    const int M = row2;
    const int N = col2;
    // const T* const W=m1;
    const T* const X=m2;


    const int y_row=(M+2*padding-U)/strides+1;
    const int y_col=(N+2*padding-V)/strides+1;  //Round down


    T *dW=new T[U*V]();
    
    for(int u=0; u<U; u++){
        for(int v=0; v<V; v++){

            for(int i=0; i<y_row; i++){
                for(int j=0; j<y_col; j++){
                    T tempx;
                    if((-padding+i*strides+u)<0||(-padding+i*strides+u)>=M||(-padding+j*strides+v)<0||(-padding+j*strides+v)>=N){
                        tempx=0;
                    }else{
                        tempx=X[(-padding+i*strides+u)*N+(-padding+j*strides+v)];
                    }
                    dW[u*V+v] += tempx * g_fy[i*y_col +j];
                }
            }

        }
    }

    return dW;
}


template <class T>
T* conv_gradient_m2(const T* const g_fy, const T* const m1, const int row1, const int col1, const int row2, const int col2, const int padding, const int strides){
    /********************************
    Args:
        T* g_fy: gradient of f(Y)
        T* m1: filters weights 
        int row1, col1: row and col numbers of g_fy
        int row2, col2: row and col numbers of m1
        int paddings: padding
        int strides: strides
    Returns:
        T* out: dX
    Note:
    *********************************/
    const int U = row1;
    const int V = col1;
    const int M = row2;
    const int N = col2;
    const T* const W=m1;
    // const T* const X=m2;


    int y_row=(M+2*padding-U)/strides+1;
    int y_col=(N+2*padding-V)/strides+1;  //Round donw


    T *dX=new T[M*N]();

    for(int s=0; s<M; s++){
        for(int t=0; t<N; t++){

            for(int i=0; i<y_row; i++){
                for(int j=0; j<y_col; j++){

                    if(s-i*strides+padding<0||s-i*strides+padding>=U||t-j*strides+padding<0||t-j*strides+padding>=V){
                        //do nothing
                    }else{
                        dX[s*N+t] +=W[(s-i*strides+padding)*V+(t-j*strides+padding)]  * g_fy[i*y_col +j];
                    }
                    
                }
            }

        }
    }

    return dX;
}


template <class T>
T* mat_maxpooling(const T* const m1, const int row1, const int col1,const int kernel_h, const int kernel_w, const int strides){
    /*
    subsampling
    */
   int out_h=(row1-kernel_h)/strides+1;
   int out_w=(col1-kernel_w)/strides+1;

    T *out=new T[out_h*out_w]();
    for(int m=0; m<out_h; m++){
        for(int n=0; n<out_w; n++){

            int filter_start_m=m*strides;
            int filter_start_n=n*strides;

            T max_one_with_filter=m1[filter_start_m*col1 + filter_start_n];

            //这个二重循环是一个O(1)复杂度
            for(int inner_m=filter_start_m; inner_m<filter_start_m+kernel_h; inner_m++){
                for(int inner_n=filter_start_n; inner_n<filter_start_n+kernel_w; inner_n++){
                    if(m1[inner_m*col1 + inner_n]>max_one_with_filter){
                        max_one_with_filter=m1[inner_m*col1 + inner_n];
                    }

                }
            }
        

            out[m*out_w +n]=max_one_with_filter;
        }
    }
    return out;
}



template <class T>
T* mat_avgpooling(const T* const m1, const int row1, const int col1,const int kernel_h, const int kernel_w, const int strides){
    /*
    subsampling
    */
   int out_h=(row1-kernel_h)/strides+1;
   int out_w=(col1-kernel_w)/strides+1;

    T *out=new T[out_h*out_w]();
    for(int m=0; m<out_h; m++){
        for(int n=0; n<out_w; n++){

            int filter_start_m=m*strides;
            int filter_start_n=n*strides;

            T avg_one_with_filter=0;
            //这个二重循环是一个O(1)复杂度
            for(int inner_m=filter_start_m; inner_m<filter_start_m+kernel_h; inner_m++){
                for(int inner_n=filter_start_n; inner_n<filter_start_n+kernel_w; inner_n++){
                    
                    // if(m1[inner_m*col1 + inner_n]>max_one_with_filter){
                    //     max_one_with_filter=m1[inner_m*col1 + inner_n];
                    // }
                    avg_one_with_filter+=m1[inner_m*col1 + inner_n];

                }
            }
        

            out[m*out_w +n]=avg_one_with_filter/(kernel_h*kernel_w);
        }
    }
    return out;
}


template <class T>
T* mat_avgpooling_upsampling(const T* const pooled, const int row1, const int col1,const int kernel_h, const int kernel_w, const int strides){
    /*
    subsampling
    */
   int out_h=(row1-kernel_h)/strides+1;
   int out_w=(col1-kernel_w)/strides+1;

    T *uped=new T[row1*col1]();
    for(int m=0; m<out_h; m++){
        for(int n=0; n<out_w; n++){

            int filter_start_m=m*strides;
            int filter_start_n=n*strides;

            T avg_one_with_filter=0;
            //这个二重循环是一个O(1)复杂度
            for(int inner_m=filter_start_m; inner_m<filter_start_m+kernel_h; inner_m++){
                for(int inner_n=filter_start_n; inner_n<filter_start_n+kernel_w; inner_n++){
                    
                    // if(m1[inner_m*col1 + inner_n]>max_one_with_filter){
                    //     max_one_with_filter=m1[inner_m*col1 + inner_n];
                    // }
                    uped[inner_m*col1 + inner_n]=pooled[m*out_w +n]/(kernel_h*kernel_w);

                }
            }
    
        }
    }
    return uped;
}


template<class T>
T* mat_T(const T* const X, int batch_size, int row, int col){
    T* out=new(nothrow)T[batch_size*row*col]();

    for(int bi=0; bi<batch_size; bi++){
        for(int ci=0; ci<col; ci++){
            for(int ri=0; ri<row; ri++){
                out[bi*row*col + ci*row + ri]=X[bi*row*col + ri*col +ci];
            }
        }
    }
    return out;
}
