from PYB11Generator import *
from boundaries import *

@PYB11template("dim")
class GridBoundaries(Boundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*",physics="Physics<%(dim)s>*"):
        return