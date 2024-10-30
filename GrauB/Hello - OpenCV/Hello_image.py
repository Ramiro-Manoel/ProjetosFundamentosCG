import numpy
import cv2 as cv

img = cv.imread("GrauB/Hello - OpenCV/imgs/baboon.png")

cv.imshow("Hello OpenCV - Ramiro", img)

k = cv.waitKey(0)