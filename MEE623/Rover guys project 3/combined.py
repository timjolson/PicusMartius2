import numpy as np
import cv2
import time

def combined(img):
	img2 = img
	img2size = img2.shape

	b = img2[:,:,0]
	g = img2[:,:,1]
	r = img2[:,:,2]

	filt = 0.541

	s1 = time.time()
	rchange = r*filt
	bchange = b*filt

	green = g - rchange - bchange
	# print 'Create green filt: ' + str(time.time() - s1)

	thresh = 20

	s1 = time.time()

	gi = np.zeros((img2size[0], img2size[1]), 'int')
	
	for i in range(0, img2size[0], 2):
		for j in range(0, img2size[1], 2):
			if green.item(i, j) > thresh:
				gi.itemset((i, j), 255)
			else:
				gi.itemset((i, j), 0)

	# ret, gi = cv2.threshold(green, thresh, 255, cv2.THRESH_BINARY)

	# cv2.imwrite('Bin_Image.jpg', gi)
	# print 'Create gi binary: ' + str(time.time() - s1)

	gisize = gi.shape

	distance = np.zeros(gisize[0], 'int')
	mid = np.zeros(gisize[0], 'int')

	s1 = time.time()

	for k in range(0, gisize[0], 2):
		rowend = 0
		rowstart = 0
		start = False
		for m in range(0, (gisize[1] - 1), 4):
			if gi.item(k, m) > 0:
				if start == False:
					rowstart = m
					start = True
				else:
					rowend = m

		diff = rowend - rowstart
		middle = rowstart + (diff/2)

		if diff < 0:
			distance.itemset(k, 0)
			mid.itemset(k, 0)
		else:
			distance.itemset(k, diff)
			mid.itemset(k, middle)

	# print 'Create distance array: ' + str(time.time() - s1)

	s1 = time.time()
	distancesize = distance.shape
	sum = 0
	summ = 0
	num = 0
	for d in range(0, distancesize[0], 1):
		sum = sum + distance.item(d)
		summ = summ + mid.item(d)
		if distance.item(d) > 1:
			num = num + 1

	if num == 0:
		width = 0
		jcenter = 0
	else:
		width = sum/num
		jcenter = summ/num
	# print 'Calculated width and j center: ' + str(time.time() - s1)

	return width, jcenter
