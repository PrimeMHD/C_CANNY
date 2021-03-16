int gaussian_filter(const int* img, int *result, int filter_n, int img_c, int img_h, int img_w, float sigma=1);
int pixelGradient(const int* img, int *gx, int *gy, int *result, int img_c, int img_h, int img_w);
int pixelGradientSuppression(int* gradient, int* gradient_x, int* gradient_y, int* suppressed_gradient, int channel, int height, int width);
int weakEdgeControl(int* suppressed_gradient, int* controlled_edge, int channel, int height, int width, int threshold, int brighter=0);
