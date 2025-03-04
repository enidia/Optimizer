from posixpath import split
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
from mpl_toolkits.mplot3d import Axes3D
import os
import numpy as np
 
Path = "./Result"
Route = "wf_route1.data"
 
Routes = os.path.join(Path,Route)
 
with open(Routes,"r+",encoding="utf-8") as f:
    RealRoute = [i[:-1] for i in f.readlines()]
RealRoute = np.array(RealRoute)
RealRoute = RealRoute.astype("int")
 
pos = np.where(RealRoute>0)[0]
split = np.where(np.diff(pos) != 1)[0] + 1
arrays = np.split(RealRoute[pos], split)
arrays = np.array(arrays)
for i in range(0,arrays.shape[0]):
    arrays[i] = np.insert(arrays[i],0,0,axis=0)
# Linecolors = [i for i in (np.linspace(1,22,21)/100)]
# Linecolors = [Linecolors[i][1:4] for i in range(0,len(Linecolors))]
# Linecolors = np.array(Linecolors)
Linecolors = np.linspace(0.5,0.9,(3*arrays.shape[0]))
Linecolors = Linecolors.reshape(arrays.shape[0],3)
Linecolors = Linecolors * np.random.rand(arrays.shape[0],3)
Linecolors = 1- Linecolors
Linecolors = Linecolors*Linecolors
col = int(arrays.shape[0]/5)
if float(col) - arrays.shape[0] < 0:
    col = col + 1
k = 1
j = 1
longer = len(arrays[0])
plt.figure(1)
for i in range(0,arrays.shape[0]):
    if(j==6):
        j = 1
        k = k + 1
        longer = len(arrays[i])
    Ylabel = arrays[i]
    Xlabel = range(0,len(arrays[i]))
    Xlabel = np.array(Xlabel)
 
    # x_major_locator = plt.MultipleLocator(1)
    # plt.gca().xaxis.set_major_locator(x_major_locator)#步进1画图，确认每一个点
    plt.subplot(col,1,k)
    plt.subplots_adjust(wspace=0,hspace=1)
    plt.xlabel("the step from the start to the end")
    plt.ylabel("the point number")
    plt.title("The "+str(k)+" th figure")
    print(k)
    print(Xlabel,Ylabel)
    print("\n")
    if len(arrays[i]) > longer:
        longer = len(arrays[i]) + 3
    plt.xlim(-0.5,longer)
    plt.scatter(Xlabel,Ylabel,marker='o',c=Linecolors[i],label = "第"+str(i)+"个解决方案",s=10)
    plt.plot(Xlabel,Ylabel,c=Linecolors[i], linewidth = 1)
    j = j + 1
 
 
for i in range(0,col):
    plt.figure(i+2)
    ax = plt.axes(projection = '3d')
    for j in range(0,5):
        if i*5+j <= arrays.shape[0]:
            Ylabel = arrays[i*5+j-1]
            Xlabel = range(0,len(arrays[i*5+j-1]))
            Xlabel = np.array(Xlabel)
            Zlabel = Ylabel
            ax.set_xlabel("iter")
            ax.set_ylabel("node")
            ax.set_zlabel("node")
            ax.scatter3D(Xlabel,Ylabel,Zlabel,marker='o',c=Linecolors[i*5+j-1],s=10)
            ax.plot3D(Xlabel,Ylabel,Zlabel,c=Linecolors[i*5+j-1], linewidth = 1)
plt.show()