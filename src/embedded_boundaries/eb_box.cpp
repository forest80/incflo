#include <AMReX_EB2.H>
#include <AMReX_EB2_IF_Cylinder.H>
#include <AMReX_EB2_IF_Intersection.H>
#include <AMReX_EB2_IF_Plane.H>
#include <AMReX_EB2_IF_Union.H>

#include <AMReX_ParmParse.H>

//#include <AMReX_VisMF.H>  // amrex::VisMF::Write(MultiFab)
//#include <sstream>

#include <algorithm>
#include <embedded_boundaries_F.H>
#include <incflo.H>

/****************************************************************************
 * Function to create a simple rectangular box with EB walls.               *
 *                                                                          *
 ****************************************************************************/
void incflo::make_eb_box()
{
	ParmParse pp("box");

	int max_level_here = 0;

	/****************************************************************************
     *                                                                          *
     * Build standard EB Factories                                              *
     *                                                                          *
     ****************************************************************************/

	// set up ebfactory

	EBSupport m_eb_support_level = EBSupport::full;

	int max_coarsening_level = 100;

	amrex::Print() << " " << std::endl;
	amrex::Print() << "Now making the ebfactories ..." << std::endl;

	if(geom[0].isAllPeriodic())
	{
		make_eb_regular();
	}
	else
	{
		/************************************************************************
         *                                                                      *
         * Define Box geometry:                                                 *
         *        -> box.{Lo,Hi} vector storing box lo/hi                       *
         *        -> box.offset  vector storing box offset                      *
         * NOTE: walls are placed _outside_ domain for periodic directions.     *
         *                                                                      *
         ************************************************************************/

		Vector<Real> boxLo(3), boxHi(3);
		Real offset = 1.0e-15;

		for(int i = 0; i < 3; i++)
		{
			boxLo[i] = geom[0].ProbLo(i);
			boxHi[i] = geom[0].ProbHi(i);
		}

		pp.queryarr("Lo", boxLo, 0, 3);
		pp.queryarr("Hi", boxHi, 0, 3);

		pp.query("offset", offset);

		Real xlo = boxLo[0] + offset;
		Real xhi = boxHi[0] - offset;

		// This ensures that the walls won't even touch the ghost cells. By
		// putting them one domain width away
		if(geom[0].isPeriodic(0))
		{
			xlo = 2.0 * geom[0].ProbLo(0) - geom[0].ProbHi(0);
			xhi = 2.0 * geom[0].ProbHi(0) - geom[0].ProbLo(0);
		}

		Real ylo = boxLo[1] + offset;
		Real yhi = boxHi[1] - offset;

		// This ensures that the walls won't even touch the ghost cells. By
		// putting them one domain width away
		if(geom[0].isPeriodic(1))
		{
			ylo = 2.0 * geom[0].ProbLo(1) - geom[0].ProbHi(1);
			yhi = 2.0 * geom[0].ProbHi(1) - geom[0].ProbLo(1);
		}

		Real zlo = boxLo[2] + offset;
		Real zhi = boxHi[2] - offset;

		// This ensures that the walls won't even touch the ghost cells. By
		// putting them one domain width away
		if(geom[0].isPeriodic(2))
		{
			zlo = 2.0 * geom[0].ProbLo(2) - geom[0].ProbHi(2);
			zhi = 2.0 * geom[0].ProbHi(2) - geom[0].ProbLo(2);
		}

		Array<Real, 3> point_lox{xlo, 0.0, 0.0};
		Array<Real, 3> normal_lox{-1.0, 0.0, 0.0};
		Array<Real, 3> point_hix{xhi, 0.0, 0.0};
		Array<Real, 3> normal_hix{1.0, 0.0, 0.0};

		Array<Real, 3> point_loy{0.0, ylo, 0.0};
		Array<Real, 3> normal_loy{0.0, -1.0, 0.0};
		Array<Real, 3> point_hiy{0.0, yhi, 0.0};
		Array<Real, 3> normal_hiy{0.0, 1.0, 0.0};

		Array<Real, 3> point_loz{0.0, 0.0, zlo};
		Array<Real, 3> normal_loz{0.0, 0.0, -1.0};
		Array<Real, 3> point_hiz{0.0, 0.0, zhi};
		Array<Real, 3> normal_hiz{0.0, 0.0, 1.0};

		EB2::PlaneIF plane_lox(point_lox, normal_lox);
		EB2::PlaneIF plane_hix(point_hix, normal_hix);

		EB2::PlaneIF plane_loy(point_loy, normal_loy);
		EB2::PlaneIF plane_hiy(point_hiy, normal_hiy);

		EB2::PlaneIF plane_loz(point_loz, normal_loz);
		EB2::PlaneIF plane_hiz(point_hiz, normal_hiz);

		auto gshop = EB2::makeShop(
			EB2::makeUnion(plane_lox, plane_hix, plane_loy, plane_hiy, plane_loz, plane_hiz));

		EB2::Build(gshop, geom.back(), max_level_here, max_level_here + max_coarsening_level);

		const EB2::IndexSpace& eb_is = EB2::IndexSpace::top();

        for(int lev = 0; lev <= max_level; lev++)
        {
            eb_level_fluid = &eb_is.getLevel(geom[lev]);

            ebfactory[lev].reset(new EBFArrayBoxFactory(
                *eb_level_fluid,
                geom[lev],
                grids[lev],
                dmap[lev],
                {m_eb_basic_grow_cells, m_eb_volume_grow_cells, m_eb_full_grow_cells},
                m_eb_support_level));
        }

		amrex::Print() << "Done making the ebfactories ..." << std::endl;
		amrex::Print() << " " << std::endl;
	}
}
