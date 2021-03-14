#encoding=utf-8
import cv2
import numpy as np
import matplotlib.pyplot as plt

def show_blured_img(path):
    blured=np.loadtxt(path).reshape((300,300))
    # blured=np.transpose(blured, (1,2,0))
    blured=blured.astype(np.uint8)
    print(blured)
    cv2.imshow("blured c", blured)
    cv2.waitKey()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    
    # img=cv2.imread("../Imgs/duck.png", cv2.IMREAD_GRAYSCALE)
    # # img=cv2.imread("Imgs/duck.png")

    # print(img.shape)
    # np.savetxt('../Imgs/duck.txt', img.flatten(), fmt='%.6f')

    # img=cv2.resize(img,(300,300))
    # cv2.imwrite('duck_small.png',img)

    # r=cv2.GaussianBlur(img,(3,3),1)
    # print(r.shape)
    # cv2.imshow("original", img)
    # cv2.imshow("result", r)
    show_blured_img("Imgs/duck_blured_gradient.txt")


