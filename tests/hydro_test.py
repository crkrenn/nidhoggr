from nidhoggr import *
from Animation import *

# class dumpState:
#     def __init__(self,nodeList,workCycle=1):
#         self.nodeList = nodeList
#         self.cycle = workCycle
#         self.dump = []
#     def __call__(self,cycle,time,dt):
#         y = self.nodeList.getFieldDouble("y")[0]
#         self.dump.append([time,y])

if __name__ == "__main__":
    animate = False
    
    nx = 10
    ny = 10
    dx = 1
    dy = 1


    myGrid = Grid2d(nx,ny,dx,dy)

    print("grid size:",myGrid.size())
    
    myNodeList = NodeList(nx*ny)

    cycles = 2
    dtmin = 0.01

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    #constants = PhysicalConstants(1.0, 1.0, 1.0)
    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(5.0/3.0,constants)
    print(eos,"gamma =",eos.gamma)


    hydro = GridHydroHLL2d(myNodeList,constants,eos,myGrid)
    
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)
    
    integrator = Integrator2d([hydro],dtmin=dtmin)


    density = myNodeList.getFieldDouble("density")
    energy  = myNodeList.getFieldDouble("specificInternalEnergy")
    for i in range (nx*ny):
        density.setValue(i,1.0)
        energy.setValue(i,1.0)

    i = int(nx/2)
    j = int(ny/2)
    idx = myGrid.index(i,j,0)
    energy.setValue(idx,15.0)

    # dump = dumpState(myNodeList,workCycle=1)

    controller = Controller(integrator=integrator,periodicWork=[],statStep=1)
    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (nx,ny)
        update_method = AnimationUpdateMethod2d(call=hydro.getCell2d,
                                                stepper=controller.Step,
                                                title=title,
                                                fieldName="pressure")
        AnimateGrid2d(bounds,update_method,extremis=[-5,5],cmap='viridis')
    else:
        controller.Step(cycles)
    
    # import matplotlib.pyplot as plt
    # import numpy as np

    # def theta(t):
    #     return t**2

    # theta_vec = np.vectorize(theta)
    # t_values = np.linspace(0, cycles*dtmin, cycles*10)

    # x_values, y_values = zip(*dump.dump)

    # xs = t_values
    # ys = theta_vec(t_values)

    # plt.plot(x_values, y_values, 'o')  
    # plt.plot(xs,ys)

    # plt.xlabel('t')
    # plt.ylabel('y')

    # plt.grid(True)
    # plt.show()