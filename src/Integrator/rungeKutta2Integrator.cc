#include "integrator.hh"

template <int dim>
class RungeKutta2Integrator : public Integrator<dim> {
protected:

public:
    RungeKutta2Integrator(Physics<dim>* physics, double dtmin, std::vector<Boundaries<dim>*> boundaries) : 
        Integrator<dim>(physics,dtmin,boundaries) {}

    ~RungeKutta2Integrator() {}

    virtual void
    Step() override {
        double dt = this->dt;
        


        double time = this->time;
        
        Physics<dim>* physics = this->physics;

        physics->PreStepInitialize();

        std::vector<Boundaries<dim>*> boundaries = this->boundaries;
        if(boundaries.size() > 0)
            for(Boundaries<dim>* bounds : boundaries)
                bounds->ApplyBoundaries(); 


        State<dim>* state = physics->getState();
        State<dim> interim(state->size());
        State<dim> k1(state->size());
        State<dim> k2(state->size());

        k1.ghost(state);
        k2.ghost(state);

        physics->EvaluateDerivatives(state,k1,time,0);

        interim.ghost(state);
        interim+=*state;
        interim+=k1*dt; 

        physics->EvaluateDerivatives(&interim,k2,time,dt);

        State<dim> newState(state->size());
        newState.ghost(state);
        newState+=*state;

        k1 += k2;
        k1 *= 0.5*dt;
        newState += k1;

        physics->FinalizeStep(&newState);

        if(boundaries.size() > 0)
            for(Boundaries<dim>* bounds : boundaries)
                bounds->ApplyBoundaries();

        this->time += this->dt;
        this->cycle += 1;

        double newdt = physics->EstimateTimestep();
        this->dt = std::max(newdt,this->dtmin);
    }
};
