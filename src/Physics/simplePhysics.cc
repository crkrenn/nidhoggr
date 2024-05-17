#include "physics.hh"
#include <iostream>

template <int dim>
class SimplePhysics : public Physics<dim> {
protected:

public:
    SimplePhysics() {}

    SimplePhysics(NodeList* nodeList, 
                        PhysicalConstants& constants) : 
        Physics<dim>(nodeList,constants) {

        int numNodes = nodeList->size();
        if (nodeList->getField<double>("y") == nullptr)
            nodeList->insertField<double>("y");
        
        Field<double>* y = nodeList->getField<double>("y");
        State<dim>* state = &this->state;        
        state->template addField<double>(y);
    }

    ~SimplePhysics() {}

    virtual void
    PreStepInitialize() override {
        // NodeList* nodeList = this->nodeList;
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        // extremely simple ode, y(t) = t^2  -> y'=2*t
        
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        Field<double>* y        = initialState->template getField<double>("y");
        Field<double>* dydt     = deriv.template getField<double>("y");

        #pragma omp parllel for
        for (int i=0; i<numNodes ; ++i) {
            dydt->setValue(i,2.0*(time+dt));
        }
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;

        Field<double>* sy       = state.template getField<double>("y");
        Field<double>* fy        = finalState->template getField<double>("y");
        Field<double>* y        = nodeList->template getField<double>("y");

        sy->copyValues(fy);
        y->copyValues(fy);
    }
};
