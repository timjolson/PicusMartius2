import numpy as np
import cv2
import os
from combined import combined
import time

video = cv2.VideoCapture(0)

while(True):
	start = time.time()
	ret, f = video.read()
	ret2, frame = video.read()
	if ret2 == True:
		print 'Read the Image: ' + str(time.time() - start)
		s1 = time.time()
		wid, jcent = combined(frame)
		#cv2.imwrite('Image_Original.jpg', frame)
		print 'Combined: ' + str(time.time() - s1)
		print 'Width: ' + str(wid)
		print 'JCenter: ' + str(jcent)
		print 'Total Time: ' + str(time.time() - start)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	else:
		print 'Error'	
#	if not cv2.waitKey(0):
#		break

video.release()
