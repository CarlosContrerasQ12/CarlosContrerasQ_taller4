import numpy as np
import matplotlib.pyplot as plt
import sys
import os
ared=open("rojoc.txt",'r')
agreen=open("verdec.txt",'r')
ablue=open("bluec.txt",'r')
nombrearchivo=sys.argv[1]
img=plt.imread(nombrearchivo)
rows = img.shape[1]
cols = img.shape[0]
img=img[:,:,0:3]


red=[]
green=[]
blue=[]

for linea in ared.readlines():
	rowr=[]
	a=linea.split(',')
	for j in range(len(a)-1):
		rowr.append(float(a[j]))
	red.append(rowr)

for linea in agreen.readlines():
	rowr=[]
	a=linea.split(',')
	for j in range(len(a)-1):
		rowr.append(float(a[j]))
	green.append(rowr)
	
for linea in ablue.readlines():
	rowr=[]
	a=linea.split(',')
	for j in range(len(a)-1):
		rowr.append(float(a[j]))
	blue.append(rowr)
			
		
red=np.array(red)
green=np.array(green)	
blue=np.array(blue)	
for i in range(rows-1):
	for j in range(cols-1):
		img[i][j][0]=red[i][j]
		img[i][j][1]=green[i][j]
		img[i][j][2]=blue[i][j]

img=img/np.max(img)

fig = plt.figure(1, figsize=(9.5,9))
plt.imshow(img)
if(sys.argv[2][0]=='a'):
	plt.savefig("altas.png")
if(sys.argv[2][0]=='b'):
	plt.savefig("bajas.png")
if(sys.argv[2][0]=='s'):
	plt.savefig("suave.png")
os.system('rm *.txt')
