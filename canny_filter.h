#define DataType float
int gaussian_filter(const DataType* img, float *result, int filter_n, int img_c, int img_h, int img_w, float sigma=1);
int pixelGradient(const float* img, float *result, int img_c, int img_h, int img_w);

