# -*- coding: utf-8 -*-
"""
Created on Sat Nov 10 12:08:14 2018

@author: traceur
"""

import numpy as np
import cv2
import dlib

def detect(jpgname,txtname):
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')
    
    # cv2读取图像
    img = cv2.imread(jpgname)
    #os.mknod("test.txt")       
    fp = open(txtname,'w')
    # 取灰度
    img_gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    
    # 人脸数rects
    rects = detector(img_gray, 0)
    for i in range(len(rects)):
        landmarks = np.matrix([[p.x, p.y] for p in predictor(img,rects[i]).parts()])
        for idx, point in enumerate(landmarks):
            # 68点的坐标
            pos = (point[0, 0], point[0, 1])
            print(idx,pos)
            new_context = str(float(point[0, 0])) + " " + str(float(point[0, 1])) + '\n'
            fp.write(new_context)     
    fp.close()

while 1:
	jpgname = input("输入人脸图片名称: ")
	txtname = input("输入关键点文档名称: ")
	detect(jpgname,txtname)
	print("识别成功")
	choice = input("exit? y/n:")
	if choice == 'y':
		break

	

	
