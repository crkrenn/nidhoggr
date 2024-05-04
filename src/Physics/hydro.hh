#ifndef HYDRO_HH
#define HYDRO_HH

#include "physics.hh"

namespace Physics {
template <int dim>
class Hydro : public Physics<dim> {
protected:
    NodeList* nodeList;
public:

    Hydro() {}

    Hydro(NodeList* nodeListPtr) : Physics<dim>(nodeListPtr), nodeList(nodeListPtr) {

    }

    virtual ~Hydro() {}
};
}

#endif