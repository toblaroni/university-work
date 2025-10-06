#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE( "Point Ordering" )
{
	Surface surface( 640, 480 );
	surface.clear();

	auto const width  = surface.get_width();
	auto const height = surface.get_height();
	
	SECTION( "Simple Horizontal" ) 
	{
		draw_line_solid(
			surface,
			{ 120.f, 240.f },
			{ 520.f, 240.f },
			{ 255, 255, 255 }
		);

		auto const pc1 = max_row_pixel_count( surface );

		surface.clear();

		draw_line_solid(
			surface,
			{ 520.f, 240.f },
			{ 120.f, 240.f },
			{ 255, 255, 255 }
		);

		auto const pc2 = max_row_pixel_count( surface );

		REQUIRE( pc1 == pc2 );
	}

	SECTION( "Simple Vertical" ) 
	{
		draw_line_solid(
			surface,
			{ width/2.f, height / 4.f},
			{ width/2.f, (height*3.f) / 4.f },
			{ 255, 255, 255 }
		);

		auto const pc1 = max_col_pixel_count( surface );

		surface.clear();

		draw_line_solid(
			surface,
			{ width/2.f, (height*3.f) / 4.f },
			{ width/2.f, height / 4.f },
			{ 255, 255, 255 }
		);

		auto const pc2 = max_col_pixel_count( surface );

		REQUIRE( pc1 == pc2 );
	}
}
