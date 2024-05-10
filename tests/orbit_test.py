from nidhoggr import *

class dumpState:
    def __init__(self,nodeList,workCycle=1,G=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.dump = []
        self.energy = []
        self.G = G
    def __call__(self,cycle,time,dt):
        v2 = self.nodeList.getFieldVector2d("velocity")[0].mag2
        ke = 0.5*v2
        r = self.nodeList.getFieldVector2d("position")[0].magnitude
        pe = self.G/r
        self.dump.append((self.nodeList.getFieldVector2d("position")[0].x,self.nodeList.getFieldVector2d("position")[0].y))
        self.energy.append((time,ke-pe))

if __name__ == "__main__":
    myNodeList = NodeList(1)

    constants = PhysicalConstants(6.378e+6,     # earth mass in kg
                                  5.972e+24,    # earth radius in m
                                  1.0,          # s
                                  1.0, 
                                  1.0) 
    loc = Vector2d(0, 0)

    sourceGrav = PointSourceGravity2d(nodeList=myNodeList,
                                      constants=constants,
                                      pointSourceLocation=loc,
                                      pointSourceMass=1)
    integrator = RungeKutta4Integrator2d(physics=sourceGrav,
                                         dtmin=0.01)
  
    pos = myNodeList.getFieldVector2d("position")[0]
    pos.x = -2.0

    v0 = -0.8*8.7298e-4
    velocity = myNodeList.getFieldVector2d("velocity")
    velocity[0].y = v0

    dump = dumpState(myNodeList,workCycle=10000,G=constants.G)
    periodicWork = [dump]

    controller = Controller(integrator=integrator,periodicWork=periodicWork,statStep=10000)

    print("G =",constants.G)
    controller.Step(1600000)

    # now plot the orbit
    
    import matplotlib.pyplot as plt
    import numpy as np
    from math import sqrt,atan2,cos

    x_values, y_values = zip(*dump.dump)

    plt.plot(x_values, y_values, 'o')  
    plt.plot(loc.x,loc.y,"o",color="red")

    r0 = 2.0
    t0 = atan2(0,-2)
    e = 1-(r0*(v0)**2/constants.G) # earth mass = 1 here
    a = r0/(1-e*cos(t0))

    def theta(t):
        return t0 + t/r0

    def r(t):
        return a*(1.0-e**2)/(1.0+e*cos(theta(t)))


    theta_vec = np.vectorize(theta)
    r_vec = np.vectorize(r)
    # Generate an array of time values
    t_values = np.linspace(0, 100, 1000)  # Example: time from 0 to 10 with 100 points

    # Calculate x(t) and y(t) for each time value
    xs = r_vec(t_values) * np.cos(theta_vec(t_values))
    ys = r_vec(t_values) * np.sin(theta_vec(t_values))

    plt.plot(xs,ys)

    plt.xlabel('x [R_E]')
    plt.ylabel('y [R_E]')
    plt.title('Plot of (x, y)')

    plt.grid(True)
    plt.show()

    x_values, y_values = zip(*dump.energy)
    plt.plot(x_values, y_values)
    plt.title('Total Energy')
    plt.xlabel('time [s]')
    plt.ylabel('Specific Energy')
    plt.grid(True)
    plt.show()