import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import sys
sys.path.append("../build/src/Math")
sys.path.append("../build/src/DataBase")
sys.path.append("../build/src/Mesh")
sys.path.append("../build/src/Physics")

from Mesh import *
from DataBase import *
from LinearAlgebra import *
from RPS import *

x = 100
y = 100
grid = Grid2d(n, x, y,1,1,1)

print(grid.position(1))

rps = RockPaperScissors(grid,0.8,0.4)

#grid.A = 0.8
#grid.D = 0.4

fig, ax = plt.subplots()

def update(frame):
    rps.update()
    ax.clear()
    # Generate RGB values for each cell
    rgb_grid = np.zeros((x, y, 3))
    for i in range(x):
        for j in range(y):
            cell = rps.getCell(i, j)
            print(cell)
            rgb_grid[i, j][0] = cell.x
            rgb_grid[i, j][1] = cell.y
            rgb_grid[i, j][2] = cell.z
    # Plot the grid
    ax.imshow(rgb_grid, origin='lower', extent=[0, x, 0, y], interpolation='nearest')

#ani = FuncAnimation(fig, update, frames=100, interval=50)
#plt.show()
