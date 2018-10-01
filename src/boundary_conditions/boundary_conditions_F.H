#include <AMReX_REAL.H>
#include <AMReX_BLFort.H>
#include <AMReX_SPACE.H>

#ifdef __cplusplus
extern "C"
{
#endif
    void incflo_set_cyclic(int* cyc_x, int* cyc_y, int* cyc_z);

    void set_bc_type(const int* bc_ilo, const int* bc_ihi,
                     const int* bc_jlo, const int* bc_jhi,
                     const int* bc_klo, const int* bc_khi,
                     const int* domlo , const int* domhi,
                     amrex::Real* dx, amrex::Real* dy, amrex::Real* dz,
                     amrex::Real* xlen, amrex::Real* ylen, amrex::Real* zlen,
                     const int* ng);

    void set_bc0(const int* slo, const int* shi,
                 amrex::Real* ro, amrex::Real* mu,  amrex::Real* lambda,
                 const int* bc_ilo, const int* bc_ihi,
                 const int* bc_jlo, const int* bc_jhi,
                 const int* bc_klo, const int* bc_khi,
                 const int* domlo , const int* domhi,
                 const int* ng);

    void set_velocity_bcs(amrex::Real* vel, 
                          const int* ulo, const int* uhi,
                          const int* bc_ilo, const int* bc_ihi,
                          const int* bc_jlo, const int* bc_jhi,
                          const int* bc_klo, const int* bc_khi,
                          const int* domlo , const int* domhi,
                          const int* ng, const int* extrap_dir_bcs);

    void fill_bc0(amrex::Real* s, const int* slo, const int* shi,
                  const int* bc_ilo, const int* bc_ihi,
                  const int* bc_jlo, const int* bc_jhi,
                  const int* bc_klo, const int* bc_khi,
                  const int* domlo , const int* domhi,
                  const int* ng);

    void extrap_pressure_to_ghost_cells (
       amrex::Real* p, const int* slo, const int* shi,
       const int* bc_ilo, const int* bc_ihi,
       const int* bc_jlo, const int* bc_jhi,
       const int* bc_klo, const int* bc_khi,
       const int* domlo , const int* domhi,
       const int* nghost);

    void set_scalar_bcs (
       amrex::Real* ro,
       amrex::Real* mu,    amrex::Real* lambda,
       const int* slo, const int* shi,
       const int* bc_ilo, const int* bc_ihi,
       const int* bc_jlo, const int* bc_jhi,
       const int* bc_klo, const int* bc_khi,
       const int* domlo , const int* domhi,
       const int* nghost);

   
#ifdef __cplusplus
}
#endif