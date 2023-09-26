import matplotlib.pyplot as plt
import numpy as np
import csv
from time import sleep
import sys

x = np.linspace(-100, 100, 2)

fig, ax = plt.subplots()
ax.set_aspect(1)
plt.xlim(-100, 100)
plt.ylim(-100, 100)

def plot_object_circle(data):
    circle = plt.Circle(( float(data[0]), float(data[1]) ), int(data[2]), color='blue')
    ax.add_patch(circle)

def plot_object_rect(data):
    ax.plot([ 
        float(data[0]), 
        float(data[2]), 
        float(data[4]), 
        float(data[6]), 
        float(data[0]) 
        ], [     
        float(data[1]), 
        float(data[3]), 
        float(data[5]), 
        float(data[7]), 
        float(data[1])
        ])
    ax.fill_between([ 
        float(data[0]), 
        float(data[2]), 
        float(data[4]), 
        float(data[6]), 
        float(data[0]) 
        ], [     
        float(data[1]), 
        float(data[3]), 
        float(data[5]), 
        float(data[7]), 
        float(data[1])
        ], facecolor='black')

def plot_angle(data):
    #ax.plot([
    #    0,
    #    float(data[0]) * 100, 
    #    float(data[2]) * 100,
    #    0
    #], [
    #    0,
    #    float(data[1]) * 100, 
    #    float(data[3]) * 100,
    #    0
    #], color='blue')
    ax.fill_between([
        0,
        float(data[0]) * 100, 
        float(data[2]) * 100,
        0
    ], [
        0,
        float(data[1]) * 100, 
        float(data[3]) * 100,
        0
    ], facecolor='blue')

def plot_vec(data):
    ax.plot([
        0,
        float(data[0]) * 100, 
        float(data[2]) * 100,
        0
    ], [
        0,
        float(data[1]) * 100, 
        float(data[3]) * 100,
        0
    ])#, color='blue')


with open('./bin/data.csv', 'r') as f:
    reader = csv.reader(f)

    for row in reader:
        match row[0]:
            case "obj":
                # plot_object_circle(row[1:])
                plot_object_rect(row[4:])
            case "ang":
                plot_angle(row[1:])
            case "vec":
                plot_vec(row[1:])
        #ax.plot([ 
        #    float(row[0]), 
        #    float(row[2]), 
        #    float(row[4]), 
        #    float(row[6]), 
        #    float(row[0]) 
        #    ], [     
        #    float(row[1]), 
        #    float(row[3]), 
        #    float(row[5]), 
        #    float(row[7]), 
        #    float(row[1])
        #    ])

        #if bool(row[8]): ax.plot([0, float(row[int(row[10])*2])], [0, float(row[int(row[10])*2+1])])
        #if bool(row[9]): ax.plot([0, 0], [float(row[int(row[11])*2]), float(row[int(row[11])*2+1])])

plt.show()