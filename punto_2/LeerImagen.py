
import matplotlib.pyplot as plt
import numpy as np
import sys
rojo=open('rojo.txt','w')
verde=open('verde.txt','w')
azul=open('azul.txt','w')
nombrearchivo=sys.argv[1]
img=plt.imread(nombrearchivo)

rows = img.shape[1]
cols = img.shape[0]
rojo.write(str(rows)+','+str(cols)+','+'\n')

for i in range(rows):
	for j in range(cols):
		rojo.write(str(img[i][j][0])+",")
		verde.write(str(img[i][j][1])+",")
		azul.write(str(img[i][j][2])+",")
print(img[131][125][1]*255)
rojo.close()
verde.close()
azul.close()
