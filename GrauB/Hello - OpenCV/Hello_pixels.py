import numpy as np
import cv2 as cv

img = cv.imread("GrauB/Hello - OpenCV/imgs/baboon.png")
imgResult = img.copy()
imgResult2 = img.copy()

print("Atributo da imagem", img.shape, "\n")

for i in range(img.shape[0]):
    for j in range(img.shape[1]):
        media = (img.item(i,j,0) + img.item(i,j,1) + img.item(i,j,2)) / 3.0
        imgResult[i,j,0] = media # canal B
        imgResult[i,j,1] = media # canal G
        imgResult[i,j,2] = media # canal R

        media = (img.item(i,j,0) * 0.07 + img.item(i,j,1) *0.71 + img.item(i,j,2) *0.2125)
        imgResult2[i,j,0] = media # canal B
        imgResult2[i,j,1] = media # canal G
        imgResult2[i,j,2] = media # canal R

cv.imshow("Imagem original - Ramiro", img)
cv.imshow("Imagem grey scale - Ramiro", imgResult)
cv.imshow("Imagem grey scale ponderada - Ramiro", imgResult2)

k = cv.waitKey(0)