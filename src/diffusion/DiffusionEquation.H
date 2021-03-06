#ifndef DIFFUSION_EQUATION_H_
#define DIFFUSION_EQUATION_H_

#include <AMReX_AmrCore.H>
#include <AMReX_BC_TYPES.H>
#include <AMReX_IArrayBox.H>
#include <AMReX_MLMG.H>
#include <AMReX_MLEBABecLap.H>

// TODO: DOCUMENTATION

class DiffusionEquation
{
public: 
    // Constructor, with initialisation
    DiffusionEquation(amrex::AmrCore* _amrcore, 
                      amrex::Vector<std::unique_ptr<amrex::EBFArrayBoxFactory>>* _ebfactory,
                      amrex::Vector<std::unique_ptr<amrex::IArrayBox>>& bc_ilo, 
                      amrex::Vector<std::unique_ptr<amrex::IArrayBox>>& bc_ihi, 
                      amrex::Vector<std::unique_ptr<amrex::IArrayBox>>& bc_jlo, 
                      amrex::Vector<std::unique_ptr<amrex::IArrayBox>>& bc_jhi, 
                      amrex::Vector<std::unique_ptr<amrex::IArrayBox>>& bc_klo, 
                      amrex::Vector<std::unique_ptr<amrex::IArrayBox>>& bc_khi,
                      int _nghost);

    // Destructor
    ~DiffusionEquation();

    // Read input from ParmParse (solver settings)
    void readParameters();

    // Update internals if AmrCore changes (e.g. after regrid)
    void updateInternals(amrex::AmrCore* amrcore_in, 
                         amrex::Vector<std::unique_ptr<amrex::EBFArrayBoxFactory>>* ebfactory_in);

     // Update the matrix based on the current state of the simulation
    void setCurrentState(const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& eta, 
                         const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& ro, 
                         amrex::Real dt);

    // Set user-supplied solver settings (must be done every time step)
    void setSolverSettings(amrex::MLMG& solver);

    // Solve the diffusion equation, update vel
    void solve(amrex::Vector<std::unique_ptr<amrex::MultiFab>>& vel, 
               const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& ro, 
               int dir);

private:
    // Update the variable coefficients in the Poisson operator, called from setCurrentState()
    void updateCoefficients(const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& eta);

    // AmrCore data 
    amrex::AmrCore* amrcore;
	amrex::Vector<std::unique_ptr<amrex::EBFArrayBoxFactory>>* ebfactory;
    int nghost; 

    // Internal data used in the matrix solve
    amrex::Vector<amrex::Vector<std::unique_ptr<amrex::MultiFab>>> b;
    amrex::Vector<std::unique_ptr<amrex::MultiFab>> phi;
    amrex::Vector<std::unique_ptr<amrex::MultiFab>> rhs;
    amrex::MLEBABecLap matrix;

    // Boundary conditions
    int bc_lo[3], bc_hi[3];

    // DiffusionEquation verbosity
	int verbose = 0;

	// Options to control MLMG behavior
	int mg_verbose = 0;
	int mg_cg_verbose = 0;
	int mg_max_iter = 100;
	int mg_cg_maxiter = 100;
	int mg_max_fmg_iter = 0;
    amrex::Real mg_rtol = 1.0e-11;
    amrex::Real mg_atol = 1.0e-14;
    std::string bottom_solver_type = "bicgstab";
};


#endif
