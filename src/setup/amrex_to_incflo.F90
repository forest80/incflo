module amrex_to_incflo_module
! _________________________________________________________________

   use amrex_fort_module, only : rt => amrex_real
   use iso_c_binding , only: c_int, c_char
   implicit none
contains
!**************************************************************************!
! Take constants from incflo and make them available in the Fortran module !
! "constant", so that they can be accessed from f90 functions.             !
!**************************************************************************!
   subroutine incflo_get_data(gravity_in, ro_0_in, mu_in, n_in, tau_0_in, papa_reg_in, eta_0_in) &
      bind(C, name="incflo_get_data")

      use constant , only: gravity, ro_0, mu, n, tau_0, papa_reg, eta_0
      use get_data_module, only: get_data
      use param, only: is_undefined

      implicit none

      real(rt),   intent(in ) :: gravity_in(3)
      real(rt),   intent(in ) :: ro_0_in, mu_in, n_in, tau_0_in, papa_reg_in, eta_0_in

      call get_data()

      gravity(:) = gravity_in(:)
      ro_0 = ro_0_in
      mu = mu_in
      n = n_in
      tau_0 = tau_0_in
      papa_reg = papa_reg_in
      eta_0 = eta_0_in

   end subroutine incflo_get_data

end module amrex_to_incflo_module
