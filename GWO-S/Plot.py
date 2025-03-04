from functools import reduce
from posixpath import split
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os
import numpy as np
 
Path = "./"
PreFitness = "PreFitness.data"
 
Fitness = os.path.join(Path,PreFitness)
 
with open(PreFitness,"r+",encoding="utf-8") as f:
    PreFitnessFile = [i[:-1].split("\t") for i in f.readlines()]
     
PreFitnessFile = np.array(PreFitnessFile)
 
PreFitnessFile = PreFitnessFile.astype("float32")
print(PreFitnessFile)
 
plt.figure(1);
ylabel = PreFitnessFile[:,0]
print(len(ylabel))
xlabel = np.arange(0,len(ylabel),1)
plt.ticklabel_format(style="sci",axis='both')
plt.title("Real Fitness")
#y = ['b','c','g','k','m','r','w','y']
y = np.arctan2(xlabel,ylabel)
#y = np.random.randint(low=0,high=255,size=(Xlabel.shape[0]))
#y = y.astype("float32")
print(y)
plt.scatter(xlabel,ylabel,marker='o',c=y,s=10)
plt.plot(xlabel,ylabel,'blue',linewidth = 1)
 
 
 
Zlabel = PreFitnessFile[:,0]
Xlabel = np.arange(0,len(Zlabel),1)
Ylabel = np.arange(0,len(Zlabel),1)
 
y = np.arctan2(Ylabel,Xlabel)
y = y/2
fig = plt.figure(2)
ax1 = plt.axes(projection='3d')
ax1.scatter3D(Ylabel,Xlabel,Zlabel,c=y,s=10) #camp 映射
ax1.set_xlabel("X position")
ax1.set_ylabel("Y position")
ax1.set_zlabel("Z position")
ax1.plot3D(Ylabel,Xlabel,Zlabel,'blue',linewidth=1)
plt.show();
