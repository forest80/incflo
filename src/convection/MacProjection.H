#ifndef MAC_PROJECTION_H_
#define MAC_PROJECTION_H_

#include <iostream>
#include <memory>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <AMReX.H>
#include <AMReX_AmrCore.H>
#include <AMReX_ArrayLim.H>
#include <AMReX_BLProfiler.H>
#include <AMReX_IArrayBox.H>
#include <AMReX_LO_BCTYPES.H>
#include <AMReX_MultiFab.H>
#include <AMReX_Vector.H>

class MacProjection
{
public:
	MacProjection(amrex::AmrCore* a_amrcore,
				  int a_nghost,
				  amrex::Vector<std::unique_ptr<amrex::EBFArrayBoxFactory>>* a_ebfactory);

	~MacProjection();

	void set_bcs(amrex::IArrayBox* a_bc_ilo,
				 amrex::IArrayBox* a_bc_ihi,
				 amrex::IArrayBox* a_bc_jlo,
				 amrex::IArrayBox* a_bc_jhi,
				 amrex::IArrayBox* a_bc_klo,
				 amrex::IArrayBox* a_bc_khi);

	void apply_projection(amrex::Vector<std::unique_ptr<amrex::MultiFab>>& u,
						  amrex::Vector<std::unique_ptr<amrex::MultiFab>>& v,
						  amrex::Vector<std::unique_ptr<amrex::MultiFab>>& w,
						  const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& ro);

	void update_internals();

	void set_velocity_bcs(int lev,
						  amrex::Vector<std::unique_ptr<amrex::MultiFab>>& u,
						  amrex::Vector<std::unique_ptr<amrex::MultiFab>>& v,
						  amrex::Vector<std::unique_ptr<amrex::MultiFab>>& w);

private:
	// Unit vectors in Cartesian space
	static amrex::IntVect e_x;
	static amrex::IntVect e_y;
	static amrex::IntVect e_z;

	int m_nghost;

	amrex::AmrCore* m_amrcore;
	amrex::Vector<std::unique_ptr<amrex::EBFArrayBoxFactory>>* m_ebfactory;

	// BCs
	amrex::IArrayBox* m_bc_ilo;
	amrex::IArrayBox* m_bc_ihi;
	amrex::IArrayBox* m_bc_jlo;
	amrex::IArrayBox* m_bc_jhi;
	amrex::IArrayBox* m_bc_klo;
	amrex::IArrayBox* m_bc_khi;

	amrex::Vector<std::unique_ptr<amrex::MultiFab>> m_diveu;
	amrex::Vector<std::unique_ptr<amrex::MultiFab>> m_phi;
	amrex::Vector<amrex::Vector<std::unique_ptr<amrex::MultiFab>>> m_b;

	//
	// Stuff for linear solver
	//
	// LinOpBCType Definitions are in amrex/Src/Boundary/AMReX_LO_BCTYPES.H
	std::array<amrex::LinOpBCType, AMREX_SPACEDIM> m_lobc;
	std::array<amrex::LinOpBCType, AMREX_SPACEDIM> m_hibc;

	bool verbose = false;

    // What solver to use as the bottom solver in the MLMG solves.
    std::string bottom_solver_type;

	int m_mg_verbose = 0;
	amrex::Real m_mg_rtol = 1.0e-11;

	void read_inputs();

	void set_ccmf_bcs(int lev, amrex::MultiFab& mf);

	void compute_b_coeff(const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& u,
						 const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& v,
						 const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& w,
						 const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& ro,
						 int lev);

	//
	// Stuff to compute norms of EB-MultiFabs
	//
	amrex::Real norm0(const amrex::Vector<std::unique_ptr<amrex::MultiFab>>& mf, int lev);
};

#endif