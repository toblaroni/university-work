#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE( "Precision Testing" )
{
	/*
	 *	 ========== Precision Testing ==========
	 *
	 *	The aim of these tests are to see if my half-plane test
	 *	implementation correctly identifies pixels that are in
	 *	the triangle vs those that aren't.
	 *	
	 * */


	// Square to make it easier to test individual pixels on the border
	Surface surface( 100, 100 );
	surface.clear();

	SECTION( "Very Small triangle" ) {
		// Should be 3 pixels
		// Important to note we treat the pixel coordinates as the centers
		Vec2f p0 = {49.5f, 49.5f};
		Vec2f p1 = {51.5f, 49.5f};
		Vec2f p2 = {49.5f, 51.5f}; 

		ColorU8_sRGB c = {255, 255, 255};

		draw_triangle_solid(surface, p0, p1, p2, c);

		int p_count = count_nonzero_pixels( surface );
		REQUIRE( p_count == 3 );
	}
}


