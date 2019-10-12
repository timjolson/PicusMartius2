import cv2

def picture():
    camera=cv2.VideoCapture(0)

    retval, im = camera.read()#previuos image
    retval, image = camera.read()#current image

    cv2.imwrite('Image'+'.jpg', image)# writes the image to SD card

    del(camera)

    return image

picture()