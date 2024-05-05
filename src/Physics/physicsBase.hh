#ifndef PHYSICSBASE_HH
#define PHYSICSBASE_HH

#include "../DataBase/nodeList.hh"
#include "../DataBase/dataBase.hh"
#include "../Math/vectorMath.hh"
#include "../Type/physicalConstants.hh"
#include "../Integrator/integrator.hh"

namespace Physics {
template <int dim>
class PhysicsBase {
protected:
    DataBase* dataBase;
    PhysicalConstants& constants;
public:
    PhysicsBase(DataBase* dataBase, PhysicalConstants& constants) : 
        dataBase(dataBase), 
        constants(constants) {
        for (auto nodeList : dataBase->nodeLists) {
            VerifyFields(nodeList);
        }
    }

    virtual ~PhysicsBase() {}

    // virtual void
    // UpdateDerivatives(const double dt) const = 0;

    virtual void
    VerifyFields(NodeList* nodeList) {
        int numNodes = nodeList->size();
        if (nodeList->mass() == nullptr)
            nodeList->insertField<double>("mass"); // Add the mass field to the nodeList

        if (nodeList->velocity<dim>() == nullptr) 
            nodeList->insertField<Lin::Vector<dim>>("velocity"); // Add the velocity field to the nodeList

        if (nodeList->position<dim>() == nullptr) 
            nodeList->insertField<Lin::Vector<dim>>("position"); // Add the position field to the nodeList

    }
};

}
#endif //PHYSICS_HH