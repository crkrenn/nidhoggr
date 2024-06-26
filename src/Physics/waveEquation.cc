#include "physics.hh"
#include "../Mesh/grid.hh"
#include <iostream>

template <int dim>
class WaveEquation : public Physics<dim> {
protected:
    Mesh::Grid<dim>* grid;
    double C;
    double dtmin;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;
    
    WaveEquation() {}

    WaveEquation(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<dim>* grid, double C) : 
        Physics<dim>(nodeList,constants),
        grid(grid), C(C) {
        if (nodeList->getField<double>("phi") == nullptr)
            nodeList->insertField<double>("phi");
        if (nodeList->getField<double>("xi") == nullptr)
            nodeList->insertField<double>("xi");
        
        /* 
        This sets the nodeList positions field to whatever is inside Grid positions. This should ideally
        happen with any physics package that uses a mesh, so this is a bit clunky to have here.
        */
        grid->assignPositions(nodeList);
        
        State<dim>* state = &this->state;
        ScalarField* xi = nodeList->getField<double>("xi");
        state->template addField<double>(xi);
        ScalarField* phi = nodeList->getField<double>("phi");
        state->template addField<double>(phi);
    }

    ~WaveEquation() {}

    virtual void
    PreStepInitialize() override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;
        state.updateFields(nodeList);
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        
        ScalarField* xi = initialState->template getField<double>("xi");
        ScalarField* phi = initialState->template getField<double>("phi");

        ScalarField* dxi = deriv.template getField<double>("xi");
        ScalarField* dphi = deriv.template getField<double>("phi");
        
        #pragma omp parallel for
        for (int i=0; i<numNodes; ++i) {
            std::vector<int> neighbors = grid->getNeighboringCells(i);
            double laplace2 = -4*phi->getValue(i);
            for (auto idx : neighbors) {
                laplace2 += phi->getValue(idx);
            }                
            laplace2 = laplace2/pow(grid->dx,2.0);
            dxi->setValue(i,laplace2*C*C); 
            dphi->setValue(i,dt*dxi->getValue(i)+xi->getValue(i));         
        }
    }

    double
    getCell(int i,int j, std::string fieldName="phi") {
        int idx = grid->index(j,i,0);
        NodeList* nodeList = this->nodeList;
        ScalarField* phi = nodeList->getField<double>(fieldName);
        return phi->getValue(idx);
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        ScalarField* fxi = finalState->template getField<double>("xi");
        ScalarField* fphi = finalState->template getField<double>("phi");

        ScalarField* xi = nodeList->getField<double>("xi");
        ScalarField* phi = nodeList->getField<double>("phi");

        xi->copyValues(fxi);
        phi->copyValues(fphi);
    }

    virtual double 
    EstimateTimestep() const override { 
        double dx = grid->dx;
        double cfl = 0.1;
        return cfl/C*dx;
    }


// d^2 phi / dt^2 = c^2 del^2 phi

// IIRC, which solution you get all comes down to initial conditions
// and boundary conditions.

// RHS is second spatial derivative.  Easy.  From Taylor,
// phi_{i+1,j} = phi_ij + dx * phi,x_ij + dx^2/2 phi,,x_ij + ... // where ,,x means d^2/dx^2
// phi_{i-1,j} = phi_ij - dx * phi,x_ij + dx^2/2 phi,,x_ij + ...
// phi_{i,j+1} = phi_ij + dy * phi,y_ij + dy^2/2 phi,,y_ij + ... // where ,,y means d^2/dy^2
// phi_{i,j-1} = phi_ij - dy * phi,y_ij + dy^2/2 phi,,y_ij + ...
// --------- = -----------------------------------
//           = 4 * phi_ij          + 2 * dx^2/2 phi,,x_ij + 2 * dy^2/2 phi,,y_ij + ...
//           = 4 * phi_ij          + dx^2 phi,,x_ij + dy^2 phi,,y_ij + ...
//           = 4 * phi_ij          + dx^2 (phi,,x_ij + phi,,y_ij) + ... // assume dx=dy
//           = 4 * phi_ij          + dx^2 del^2(phi_ij) + ... // assume dx=dy

// del^2 phi_ij = (-4*phi_{i,j} + phi_{i+1,j} + phi_{i-1,j} phi_{i,j+1} + phi_{i,j-1})/dx^2

};
