from PYB11Generator import *

@PYB11template("dim")
class Physics:
    def pyinit(self,nodeListPtr="NodeList*",constants="PhysicalConstants&"):
        return