/*****************************************************************************

        main.cpp
        Copyright (c) 2005 Laurent de Soras

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
	#pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
	#pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	"EPSPlot.h"
#include	"Gridaxis.h"

#if defined (_MSC_VER)
#include	<crtdbg.h>
#include	<new.h>
#endif	// _MSC_VER

#include	<iostream>
#include	<new>

#include	<cassert>
#include	<cmath>



/*\\\ FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



static void	PLIT_plot_figure_01 ();
static void	PLIT_plot_figure_02 ();



// Draws three sines
static void	PLIT_plot_figure_01 ()
{
	long				nbr_points = 100;

	const double	width  = 6 * 72;
	const double	height = 4 * 72;
	const double	margin_x = 0.5 * 72;
	const double	margin_y = 0.5 * 72;
	EPSPlot			plot (
		"plit_plot_test_01.eps",
		0, 0,
		width + margin_x * 2,
		height + margin_y * 2
	);
	Gridaxis			grid;
	grid.set_size (width, height);
	grid.set_plot (plot, margin_x, margin_y);
	grid.set_grid (true);
	grid.use_axis (Gridaxis::Direction_H).set_scale (0.0, nbr_points - 1.0);
	grid.use_axis (Gridaxis::Direction_H).set_tick_dist (10);
	grid.use_axis (Gridaxis::Direction_H).activate_tick (true);
	grid.use_axis (Gridaxis::Direction_V).set_scale (-1, 1);
	grid.use_axis (Gridaxis::Direction_V).set_tick_dist (0.25);
	grid.use_axis (Gridaxis::Direction_V).activate_tick (true);
	grid.render_background ();

	const long		nbr_curves = 3;
	const int		rgb [nbr_curves] [3] =
	{
		{ 0, 0, 0 },
		{ 0, 0, 1 },
		{ 1, 0, 0 }
	};
	const char *	leg_0_arr [nbr_curves] =
	{
		"Alpha", "Beta", "Gamma"
	};

	plot.setFontSize (8);

	for (int curve = 0; curve < nbr_curves; ++curve)
	{
		plot.setLineWidth (0.75);
		plot.setRGBColor (
			rgb [curve] [0],
			rgb [curve] [1],
			rgb [curve] [2]
		);

		std::vector <double>	x_arr (nbr_points);
		std::vector <double>	y_arr (nbr_points);
		for (long pos = 0; pos < nbr_points; ++pos)
		{
			using namespace std;

			x_arr [pos] = pos;
			y_arr [pos] = sin (pos * 2 * 3.1415927 * (curve + 1) / nbr_points);
		}

		grid.render_curve (&x_arr [0], &y_arr [0], nbr_points);
		if (curve == 0)
		{
			grid.render_point_set (&x_arr [0], &y_arr [0], nbr_points);

			plot.setLineWidth (0.5);
			grid.use_prim ().draw_arrow (
				0.70,
				0.25,
				grid.use_axis (Gridaxis::Direction_H).conv_val_to_pos (x_arr [65]),
				grid.use_axis (Gridaxis::Direction_V).conv_val_to_pos (y_arr [65])
			);
			grid.put_annotation_pos (0.70, 0.25, "Blah");
		}

		grid.use_prim ().print_legend (leg_0_arr [curve], curve);
	}
}



// Draws a second-order low-pass filter response curve
static void	PLIT_plot_figure_02 ()
{
	long				nbr_points = 1000;
	double			min_f = 20;
	double			max_f = 20000;

	const double	width  = 6 * 72;
	const double	height = 4 * 72;
	const double	margin_x = 0.5 * 72;
	const double	margin_y = 0.5 * 72;
	EPSPlot			plot (
		"plit_plot_test_02.eps",
		0, 0,
		width + margin_x * 2,
		height + margin_y * 2
	);
	Gridaxis			grid;
	grid.set_size (width, height);
	grid.set_plot (plot, margin_x, margin_y);
	grid.set_grid (true);
	grid.use_axis (Gridaxis::Direction_H).set_scale (min_f, max_f);
	grid.use_axis (Gridaxis::Direction_H).set_log_scale (true);
	grid.use_axis (Gridaxis::Direction_H).set_tick_org (1);
	grid.use_axis (Gridaxis::Direction_H).activate_tick (true);
	grid.use_axis (Gridaxis::Direction_H).set_name ("Frequency (Hz)");
	grid.use_axis (Gridaxis::Direction_V).set_scale (-50, 30);
	grid.use_axis (Gridaxis::Direction_V).set_log_scale (false);
	grid.use_axis (Gridaxis::Direction_V).set_tick_org (0);
	grid.use_axis (Gridaxis::Direction_V).set_tick_dist (20);
	grid.use_axis (Gridaxis::Direction_V).activate_tick (true);
	grid.use_axis (Gridaxis::Direction_V).set_name ("Amplitude (dB)");
	grid.render_background ();

	plot.setFontSize (8);
	plot.setLineWidth (0.75);
	plot.setRGBColor (0, 0, 1);

	std::vector <double>	x_arr (nbr_points);
	std::vector <double>	y_arr (nbr_points);
	const double	a = 0.1;
	const double	f0 = 1000;
	for (long pos = 0; pos < nbr_points; ++pos)
	{
		using namespace std;

		const double	f1 = min_f * exp (log (max_f / min_f) * pos / nbr_points);
		x_arr [pos] = f1;

		const double	f = f1 / f0;
		const double	r = 1 - f * f;
		const double	i = a * f;
		const double	g = 1 / sqrt (r*r + i*i);
		y_arr [pos] = 20 * log10 (g);
	}

	grid.render_curve (&x_arr [0], &y_arr [0], nbr_points);
}



#if defined (_MSC_VER)
static int __cdecl	PLIT_new_handler_cb (size_t dummy)
{
	throw std::bad_alloc ();
	return (0);
}
#endif	// _MSC_VER



#if defined (_MSC_VER) && ! defined (NDEBUG)
static int	__cdecl	PLIT_debug_alloc_hook_cb (int alloc_type, void *user_data_ptr, size_t size, int block_type, long request_nbr, const unsigned char *filename_0, int line_nbr)
{
	if (block_type != _CRT_BLOCK)	// Ignore CRT blocks to prevent infinite recursion
	{
		switch (alloc_type)
		{
		case	_HOOK_ALLOC:
		case	_HOOK_REALLOC:
		case	_HOOK_FREE:

			// Put some debug code here

			break;

		default:
			assert (false);	// Undefined allocation type
			break;
		}
	}

	return (1);
}
#endif



#if defined (_MSC_VER) && ! defined (NDEBUG)
static int	__cdecl	PLIT_debug_report_hook_cb (int report_type, char *user_msg_0, int *ret_val_ptr)
{
	*ret_val_ptr = 0;	// 1 to override the CRT default reporting mode

	switch (report_type)
	{
	case	_CRT_WARN:
	case	_CRT_ERROR:
	case	_CRT_ASSERT:

// Put some debug code here

		break;
	}

	return (*ret_val_ptr);
}
#endif



static void	PLIT_main_prog_init ()
{
#if defined (_MSC_VER)
	::_set_new_handler (::PLIT_new_handler_cb);
#endif	// _MSC_VER

#if defined (_MSC_VER) && ! defined (NDEBUG)
	{
		const int	mode =   (1 * _CRTDBG_MODE_DEBUG)
						       | (1 * _CRTDBG_MODE_WNDW);
		::_CrtSetReportMode (_CRT_WARN, mode);
		::_CrtSetReportMode (_CRT_ERROR, mode);
		::_CrtSetReportMode (_CRT_ASSERT, mode);

		const int	old_flags = ::_CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
		::_CrtSetDbgFlag (  old_flags
		                  | (1 * _CRTDBG_LEAK_CHECK_DF)
		                  | (1 * _CRTDBG_CHECK_ALWAYS_DF));
		::_CrtSetBreakAlloc (-1);	// Specify here a memory bloc number
		::_CrtSetAllocHook (PLIT_debug_alloc_hook_cb);
		::_CrtSetReportHook (PLIT_debug_report_hook_cb);

		// Speed up I/O but breaks C stdio compatibility
//		std::cout.sync_with_stdio (false);
//		std::cin.sync_with_stdio (false);
//		std::cerr.sync_with_stdio (false);
//		std::clog.sync_with_stdio (false);
	}
#endif	// _MSC_VER, NDEBUG
}



static void	PLIT_main_prog_end ()
{
#if defined (_MSC_VER) && ! defined (NDEBUG)
	{
		const int	mode =   (1 * _CRTDBG_MODE_DEBUG)
						       | (0 * _CRTDBG_MODE_WNDW);
		::_CrtSetReportMode (_CRT_WARN, mode);
		::_CrtSetReportMode (_CRT_ERROR, mode);
		::_CrtSetReportMode (_CRT_ASSERT, mode);

		::_CrtMemState	mem_state;
		::_CrtMemCheckpoint (&mem_state);
		::_CrtMemDumpStatistics (&mem_state);
	}
#endif	// _MSC_VER, NDEBUG
}



int main (int argc, char *argv [])
{
	PLIT_main_prog_init ();

	try
	{
		using namespace std;

		PLIT_plot_figure_01 ();
		PLIT_plot_figure_02 ();
	}

	catch (std::exception &e)
	{
		std::cout << "*** main() : Exception (std::exception) : " << e.what () << std::endl;
		throw;
	}

	catch (...)
	{
		std::cout << "*** main() : Undefined exception" << std::endl;
		throw;
	}

	PLIT_main_prog_end ();

	return (0);
}



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
