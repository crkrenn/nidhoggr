import sys
sys.path.append("Math")
sys.path.append("Field")

from LinearAlgebra import *
from Field import *

if __name__ == "__main__":
    vec1 = Vector3d(1.0,2.0,3.0)
    vec2 = Vector3d(2.0,3.0,4.0)

    myField = Field("position",vec1)

    print("a =",vec1)
    print("b =",vec2)
    print("a*b =",vec1*vec2)
    print("axb =",vec1.cross(vec2))
    print("a+b =",vec1+vec2)
    print("a*2 =",vec2*2.0)
    print("2*a =",2.0*vec2)
    print("|a| =",vec2.magnitude)
