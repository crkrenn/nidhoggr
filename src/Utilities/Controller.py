from Integrator import *

class Controller:
    def __init__(self,integrator,periodicWork=[],statStep=1):
        self.integrator = integrator
        self.periodicWork = periodicWork
        self.statStep = statStep # use this to override how frequently we print to the screen
    def Step(self,nsteps=1):
        for i in range(nsteps):
            self.integrator.Step()
            cycle = self.integrator.Cycle()
            time = self.integrator.Time()
            dt = self.integrator.dt
            if self.integrator.Cycle() % self.statStep == 0:
                print("Cycle: %04d"%cycle,
                    " Time: %03.3e"%time,
                    " dt: %03.3e"%dt)
            if len(self.periodicWork) > 0:
                for work in self.periodicWork:
                    if cycle % work.cycle == 0:
                        work(cycle,time,dt)