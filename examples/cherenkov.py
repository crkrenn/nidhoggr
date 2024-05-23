from nidhoggr import *
from Animation import *
from math import sin,cos

class oscillate:
    def __init__(self,nodeList,grid,cs,width,height,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.cs = cs
        self.width = width
        self.height = height
        self.phi = myNodeList.getFieldDouble("phi")
    def __call__(self,cycle,time,dt):
        a = 5*(cos(time/0.5))
        v = min(max(0.01*time*self.cs,0.5*self.cs),2.0*self.cs)
        posx = v*time
        if posx <= self.width:
            i = int(posx)
            j = int(self.height/2)
            idx = self.grid.index(i,j,0)
            self.phi.setValue(idx,a)
        # self.phi.setValue(5050,-val)
        # self.phi.setValue(2025,val)
        # self.phi.setValue(8075,val)

      

if __name__ == "__main__":
    animate = True
    
    constants = PhysicalConstants(1,1,1.0,1.0,1.0) 
    nx = 200
    ny = 200
    cs = 1.0

    myNodeList = NodeList(nx*ny)
    
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    waveEqn = WaveEquation2d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,C=cs)

    print(waveEqn)

    pm = PacmanGridBoundaries2d(grid=grid,physics=waveEqn)
    pbounds = []

    integrator = RungeKutta2Integrator2d(physics=waveEqn,
                              dtmin=0.05,
                              boundaries=pbounds)
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(nodeList=myNodeList,grid=grid,cs=cs,width=nx,height=ny,workCycle=1)
    periodicWork = [osc]

    controller = Controller(integrator=integrator,
                            statStep=10,
                            periodicWork=periodicWork)

    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (nx,ny)
        update_method = AnimationUpdateMethod2d(call=waveEqn.getCell2d,
                                                stepper=controller.Step,
                                                title=title)
        AnimateGrid2d(bounds,update_method,extremis=[-1,1],cmap="gist_rainbow")
    else:
        controller.Step(50000)
