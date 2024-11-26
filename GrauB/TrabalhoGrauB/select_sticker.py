import cv2 as cv

def sun_glasses():
    return cv.imread("GrauB/TrabalhoGrauB/imgs/Stickers/eyeglasses.png", cv.IMREAD_UNCHANGED)

def wow():
    sticker = cv.imread("GrauB/TrabalhoGrauB/imgs/Stickers/wow.png", cv.IMREAD_UNCHANGED)
    return cv.resize(sticker, (200,200))

def whatsapp():
    sticker = cv.imread("GrauB/TrabalhoGrauB/imgs/Stickers/whatsapp.png", cv.IMREAD_UNCHANGED)
    return cv.resize(sticker, (200,200))

def nerd():
    sticker = cv.imread("GrauB/TrabalhoGrauB/imgs/Stickers/nerd.png", cv.IMREAD_UNCHANGED)
    return cv.resize(sticker, (200,200))

def top():
    sticker = cv.imread("GrauB/TrabalhoGrauB/imgs/Stickers/top.png", cv.IMREAD_UNCHANGED)
    return cv.resize(sticker, (200,200))

def joia():
    sticker = cv.imread("GrauB/TrabalhoGrauB/imgs/Stickers/joia.png", cv.IMREAD_UNCHANGED)
    return cv.resize(sticker, (200,200))