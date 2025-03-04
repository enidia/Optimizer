from functools import reduce
from posixpath import split
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os
import numpy as np
 
Path = "./Result"
Fitness = "Fitness.data"
Grid = "Grid.data"
Position = "Position.data"
PreFitness = "PreFitness.data"
 
Fitness = os.path.join(Path,Fitness)
Grid = os.path.join(Path,Grid);
Position = os.path.join(Path,Position)
PreFitness = os.path.join(Path,PreFitness)
 
# with open(Fitness,"r+",encoding="utf-8") as f:
#     FitnessFile = [i[:-1].split("\t") for i in f.readlines()]
# with open(Grid,"r+",encoding="utf-8") as f:
#     GridFile = [i[:-1].split("\t") for i in f.readlines()]
# with open(Position,"r+",encoding="utf-8") as f:
#     PositionFile = [i[:-1].split("\t") for i in f.readlines()]
with open(PreFitness,"r+",encoding="utf-8") as f:
    PreFitnessFile = [i[:-1].split("\t") for i in f.readlines()]
     
# FitnessFile = np.array(FitnessFile)
# GridFile = np.array(GridFile)
# PositionFile = np.array(PositionFile)
PreFitnessFile = np.array(PreFitnessFile)
 
# FitnessFile = FitnessFile.astype("float32")
# GridFile = GridFile.astype("float32")
# PositionFile = PositionFile.astype("float32")
PreFitnessFile = PreFitnessFile.astype("float32")
print(PreFitnessFile)
# Xlabel = FitnessFile[:,0]
# Ylabel = FitnessFile[:,1]
 
# plt.ticklabel_format(style="sci",axis='both')
# plt.title("Fitness in Bees Algorithm")
# plt.scatter(Xlabel,Ylabel)
# plt.show()
 
# Xlabel = GridFile[:,0]
# Ylabel = GridFile[:,1]
# plt.ticklabel_format(style="sci",axis='both')
# plt.title("Grid in Algorithm")
# plt.scatter(Xlabel,Ylabel)
# plt.show()
 
# Xlabel = PositionFile[:,0]
# Ylabel = PositionFile[:,1]
# plt.ticklabel_format(style="sci",axis='both')
# plt.title("X position")
# plt.scatter(Xlabel,Ylabel)
# plt.show()
if PreFitnessFile.shape[1] == 2:
    Xlabel = PreFitnessFile[:,0]
    Ylabel = PreFitnessFile[:,1]
    plt.ticklabel_format(style="sci",axis='both')
    plt.title("Real Fitness")
    #y = ['b','c','g','k','m','r','w','y']
    y = np.arctan2(Ylabel,Xlabel)
    #y = np.random.randint(low=0,high=255,size=(Xlabel.shape[0]))
    #y = y.astype("float32")
    print(y)
    plt.scatter(Xlabel,Ylabel,marker='o',c=y)
    # plt.plot(Xlabel,Ylabel,'gray')
    plt.show()
if PreFitnessFile.shape[1] ==3:
    Xlabel = PreFitnessFile[:,0]
    Ylabel = PreFitnessFile[:,1]
    Zlabel = PreFitnessFile[:,2]
    y = np.arctan2(Ylabel,Xlabel)
    fig = plt.figure()
    ax1 = plt.axes(projection='3d')
    ax1.scatter3D(Xlabel,Ylabel,Zlabel,c=y) #camp 映射
    #ax1.plot3D(Xlabel,Ylabel,Zlabel,'gray')
    plt.show();