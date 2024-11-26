import numpy as np
import cv2 as cv
import main

def originial_image(image):
    return image

def gray_scale(image):
    image = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    return cv.cvtColor(image, cv.COLOR_GRAY2BGR)

def binarized(image):
    image = cv.cvtColor(image, cv.COLOR_BGR2GRAY)

    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            if image[i,j] >= 150:
                image[i,j] = 255
            else:
                image[i,j] = 0

    return cv.cvtColor(image, cv.COLOR_GRAY2BGR)

def inverted(image):

    image_result = image.copy()

    for i in range(image.shape[0]): #percorre linhas
        for j in range(image.shape[1]): #percorre colunas

            image_result[i,j,0] = image[i,j,0] ^ 255 # B
            image_result[i,j,1] = image[i,j,1] ^ 255 # G
            image_result[i,j,2] = image[i,j,2] ^ 255 # R

    return image_result

def colorized(image, color):

    image_result = image.copy()

    for i in range(image.shape[0]): 
        for j in range(image.shape[1]):
            image_result[i,j,0] = image[i,j,0] | color[0] # B
            image_result[i,j,1] = image[i,j,1] | color[1] # G
            image_result[i,j,2] = image[i,j,2] | color[2] # R

    return image_result

def blue_colorized(image):
    return colorized(image, [255,0,0])

def green_colorized(image):
    return colorized(image, [0,255,0])

def red_colorized(image):
    return colorized(image, [0,0,255])

def edged(image):
    image = cv.Canny(image, 50, 100)
    return cv.cvtColor(image, cv.COLOR_GRAY2BGR)

def correlation(image):
    kernel = np.ones((3, 3), np.float32) / 2
    return cv.filter2D(image, -1, kernel)

def max_saturation(image):
    H, S, V = cv.split(image)
    S = np.clip(S * 2, 0, 255).astype(np.uint8)
    image = cv.merge([H, S, V])
    return cv.cvtColor(image, cv.COLOR_HSV2BGR)

def thermal(image):
    return cv.applyColorMap(image, cv.COLORMAP_JET)
