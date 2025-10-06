#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE( "Connecting Lines" )
{	
	Surface surface( 640, 480 );
	surface.clear();

	auto const width  = surface.get_width();
	auto const height = surface.get_height();

	SECTION( "Simple 2 Line, Horizontal" ) {
		draw_line_solid(
			surface,
			{ 0, height / 2.f },
			{ width / 2.f, height / 2.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ width / 2.f, height / 2.f },
			{ static_cast<float>(width), height / 2.f },
			{ 255, 255, 255 }
		);

		// This ensures there's no gaps
		REQUIRE ( max_row_pixel_count( surface ) == width );
	}

	SECTION( "Simple 2 Line, Vertical" ) {
		draw_line_solid(
			surface,
			{ width / 2.f, 0.f },
			{ width / 2.f, height / 2.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ width / 2.f, height / 2.f },
			{ width / 2.f, static_cast<float>(height) },
			{ 255, 255, 255 }
		);

		REQUIRE ( max_col_pixel_count( surface ) == height );
	}

	SECTION( "Simple 2 Line, Diagonal" ) {
		draw_line_solid(
			surface,
			{ 0.f, 0.f },
			{ width / 2.f, height / 2.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ width / 2.f, height / 2.f },
			{ static_cast<float>(width), static_cast<float>(height) },
			{ 255, 255, 255 }
		);

		// There should only be two points (start and finish) with a neighbour count of 1.
		REQUIRE( count_pixel_neighbours( surface )[1] == 2 );
	}

	SECTION( "3 Line Test" ) {
		// N Shaped Line
		draw_line_solid(
			surface,
			{ 30.f, 20.f },
			{ 30.f, height - 20.f },
			{ 255, 255 ,255 }
		);

		draw_line_solid(
			surface,
			{ 30.f, height - 20.f },
			{ width - 30.f, 20.f },
			{ 255, 255 ,255 }
		);

		draw_line_solid(
			surface,
			{ width - 30.f, 20.f },
			{ width - 30.f, height - 20.f },
			{ 255, 255 ,255 }
		);

		REQUIRE( count_pixel_neighbours( surface )[1] == 2 );
	}

	SECTION( "10 Line Zig Zag" ) {
		draw_line_solid(
			surface,
			{ 10.f, height - 20.f },
			{ 30.f, height - 40.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 30.f, height - 40.f },
			{ 50.f, height - 20.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 50.f, height - 20.f },
			{ 70.f, height - 40.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 70.f, height - 40.f },
			{ 90.f, height - 20.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 90.f, height - 20.f },
			{ 110.f, height - 40.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 110.f, height - 40.f },
			{ 130.f, height - 20.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 130.f, height - 20.f },
			{ 150.f, height - 40.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 150.f, height - 40.f },
			{ 170.f, height - 20.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 170.f, height - 20.f },
			{ 190.f, height - 40.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 190.f, height - 40.f },
			{ 210.f, height - 20.f },
			{ 255, 255, 255 }
		);

		REQUIRE( count_pixel_neighbours( surface )[1] == 2 );
	}

	SECTION( "10 line zig-zag smaller" ) {
		draw_line_solid(
			surface,
			{ 10.f, height - 10.f },
			{ 15.f, height - 15.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 15.f, height - 15.f },
			{ 20.f, height - 10.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 20.f, height - 10.f },
			{ 25.f, height - 15.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 25.f, height - 15.f },
			{ 30.f, height - 10.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 30.f, height - 10.f },
			{ 35.f, height - 15.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 35.f, height - 15.f },
			{ 40.f, height - 10.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 40.f, height - 10.f },
			{ 45.f, height - 15.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 45.f, height - 15.f },
			{ 50.f, height - 10.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 50.f, height - 10.f },
			{ 55.f, height - 15.f },
			{ 255, 255, 255 }
		);

		draw_line_solid(
			surface,
			{ 55.f, height - 15.f },
			{ 60.f, height - 10.f },
			{ 255, 255, 255 }
		);

		REQUIRE( count_pixel_neighbours( surface )[1] == 2 );
	}
}
