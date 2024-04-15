#include "../DataBase/fieldList.cc"
#include "../DataBase/nodeList.cc"
#include "../Math/vector_math.cc"

namespace Hydro {
template <int dim>
class Hydro {
protected:
    std::vector<FieldListBase*> hydroFieldLists;
    NodeList* nodeList;
public:
    FieldList<double> density;
    FieldList<VectorMath::Vector<dim>> momentum;
    FieldList<double> specific_energy;

    Hydro() {}

    Hydro(int numNodes, NodeList* nodeListPtr) : nodeList(nodeListPtr) {
        density = FieldList<double>("density",numNodes);
        momentum = FieldList<VectorMath::Vector<dim>>("momentum",numNodes);
        specific_energy = FieldList<double>("specific_energy",numNodes);

        nodeList->addFieldList(&density);
        nodeList->addFieldList(&momentum);
        nodeList->addFieldList(&specific_energy);
    }

    virtual ~Hydro() {}

    virtual std::vector<FieldListBase*> getHydroFieldLists() { return hydroFieldLists; }
};
}
