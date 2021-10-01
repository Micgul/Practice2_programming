import cv2
import numpy as np
from matplotlib import pyplot as plt

# 모델 영상 히스토그램 계산
img_m = cv2.imread('model.jpg')
hsv_m = cv2.cvtColor(img_m, cv2.COLOR_BGR2HSV)
hist_m = cv2.calcHist([hsv_m], [0, 1], None, [180, 256], [0, 180, 0, 256])

# 손(입력) 영상 히스토그램 계산
img_i = cv2.imread('hand.jpg')
hsv_i = cv2.cvtColor(img_i, cv2.COLOR_BGR2HSV)
hist_i = cv2.calcHist([hsv_i], [0, 1], None, [180, 256], [0, 180, 0, 256])

# 2d histogram 시각화
#cv2.imshow('Hist',hist_m)
#plt.imshow(hist_m)
#plt.show()

# 히스토그램 정규화
img_norm_m = cv2.normalize(hist_m, None, 0, 255, cv2.NORM_MINMAX)
img_norm_i = cv2.normalize(hist_i, None, 0, 255, cv2.NORM_MINMAX)
print("maximum of hist_m: % f" % img_norm_m.max())
print("maximum of hist_i: % f" % img_norm_i.max())

# 비율 히스토그램 계산 - (0~1)
hist_r = hist_m/(hist_i+1)
print("range of hist_r: [%.1f, %.1f]" % (hist_r.min(), hist_r.max()))

# 히스토그램 역투영 수행
h,s,v = cv2.split(hsv_i)
B = hist_r[h.ravel(), s.ravel()]
B = np.minimum(B, 1)
B = B.reshape(hsv_i.shape[:2])

disc = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(5,5))
cv2.filter2D(B, -1, disc, B)
B = np.uint8(B)
cv2.normalize(B, B, 0, 255, cv2.NORM_MINMAX)

# 이진화 수행
ret, thresholded = cv2.threshold(B, 0.02, 255, cv2.THRESH_BINARY)
cv2.imwrite('result.jpg', thresholded)

# 모폴로지 (Dilation) 연산 적용 (주변 추가)
img = cv2.imread('result.jpg',0)
kernel = np.ones((5,5), np.uint8)
improved = cv2.dilate(img, kernel, iterations = 1)
cv2.imwrite('morphology.jpg', improved)

# 처음 검출할 때 깨끗하지는 않아서 그런지 많이 개선되지는 않습니다..