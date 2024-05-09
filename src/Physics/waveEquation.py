from PYB11Generator import *
from physics import *

@PYB11template("dim")
class WaveEquation(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               grid="Mesh::Grid<%(dim)s>*"):
        return
    # @PYB11virtual
    # def UpdateDerivatives(self,dt="const double"):
    #     return "void"

WaveEquation1d = PYB11TemplateClass(WaveEquation,
                              template_parameters = ("1"),
                              cppname = "WaveEquation<1>",
                              pyname = "WaveEquation1d",
                              docext = " (1D).")
WaveEquation2d = PYB11TemplateClass(WaveEquation,
                              template_parameters = ("2"),
                              cppname = "WaveEquation<2>",
                              pyname = "WaveEquation2d",
                              docext = " (2D).")
WaveEquation3d = PYB11TemplateClass(WaveEquation,
                              template_parameters = ("3"),
                              cppname = "WaveEquation<3>",
                              pyname = "WaveEquation3d",
                              docext = " (3D).") 