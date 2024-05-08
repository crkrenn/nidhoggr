#include "physics.hh"
#include <iostream>

template <int dim>
class ConstantGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> gravityVector;
    Field<Lin::Vector<dim>> dydt;
    Field<Lin::Vector<dim>> dvdt;
public:
    ConstantGravity() {}

    ConstantGravity(NodeList* nodeList, PhysicalConstants& constants, Lin::Vector<dim>& gravityVector) : 
        Physics<dim>(nodeList,constants),
        gravityVector(gravityVector) {

        int numNodes = nodeList->size();
        if (nodeList->getField<Lin::Vector<dim>>("acceleration") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("acceleration");
        for (int i=0; i<numNodes; ++i)
            nodeList->getField<Lin::Vector<dim>>("acceleration")->setValue(i,gravityVector);
        
        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        this->derivFields.push_back(position);
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        this->derivFields.push_back(velocity);
    }

    ~ConstantGravity() {}

    virtual void
    EvaluateDerivatives(const Field<Lin::Vector<dim>>* initialState, Field<Lin::Vector<dim>>& deriv, const double t) override {
        // compute accelerations
        NodeList* nodeList = this->nodeList;
        // for (std::string fieldName : nodeList->fieldNames()) {
        //     std::cout << fieldName << std::endl;
        // }
        int numNodes = nodeList->size();
        if(initialState->getNameString() == "position") {
            Field<Lin::Vector<dim>> *velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
            Field<Lin::Vector<dim>> *acceleration = nodeList->getField<Lin::Vector<dim>>("acceleration");
            Field<Lin::Vector<dim>> dxdt;
            dxdt = *velocity + (*acceleration)*t;
            deriv.copyValues(dxdt);
        }
        else if(initialState->getNameString() == "velocity") {
            Field<Lin::Vector<dim>>* acceleration = nodeList->getField<Lin::Vector<dim>>("acceleration");
            deriv.copyValues(acceleration);
        }
        
    }

    Field<Lin::Vector<dim>> 
    derivative(double t, const Field<Lin::Vector<dim>>& y, std::string yp) {
        // Example differential equation: dy/dt = y'
        // in this case, i don't use the field "y" to calculate y'
        Field<Lin::Vector<dim>> dydt("dydt-"+yp);
        NodeList* nodeList = this->nodeList;
        for (size_t i = 0; i < y.size(); ++i) {
            dydt.addValue(nodeList->getField<Lin::Vector<dim>>(yp)[i]);
        }
        return dydt;
    }
};
