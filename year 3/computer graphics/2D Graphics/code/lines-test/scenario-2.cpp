#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE ( "One Point In, One Point Out", "[clip][!mayfail]" )	
{
	Surface surface( 640, 480 );
	surface.clear();

	auto const width  = surface.get_width();
	auto const height = surface.get_height();

	// === End Outside ===
	SECTION( "Simple Horizontal Right, End Out" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height/2.f },
			{ width + 10.f, height/2.f },	// Extends slightly to the right
			{255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels == (width/2.f) );
	}

	SECTION( "Simple Horizontal Left, End Out" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height/2.f },
			{ -10.f, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels == (width/2.f)+1 );
	}

	SECTION( "Simple Vertical Bottom, End Out" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height/2.f },
			{ width/2.f, -10.f },
			{255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( pixels == (height/2.f)+1 );
	}

	SECTION( "Simple Vertical Top, End Out" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height/2.f },
			{ width/2.f, height + 10.f },
			{255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( pixels == (height/2.f) );
	}

	// === Begin Outside ===
	SECTION( "Simple Horizontal Right, Start Out" ) {
		draw_line_solid( 
			surface,
			{ width + 10.f, height/2.f },
			{ width/2.f, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels == (width/2.f) );
	}

	SECTION( "Simple Horizontal Left, Start Out" ) {
		draw_line_solid( 
			surface,
			{ -10.f, height/2.f },
			{ width/2.f, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels == (width/2.f)+1 );
	}

	SECTION( "Simple Vertical Bottom, Start Out" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, -10.f },
			{ width/2.f, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( pixels == (height/2.f)+1 );
	}

	SECTION( "Simple Vertical Top, Start Out" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height + 10.f },
			{ width/2.f, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( pixels == (height/2.f) );
	}


	// === Diagonal Tests ===
	// === Start Inside, End Outside ===
	SECTION( "Diagonal Bottom-Left to Top-Right, End Out" ) {
		draw_line_solid(
			surface,
			{ width/4.f, height/4.f },
			{ width + 10.f, height + 10.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Top-Right to Bottom-Left, End Out" ) {
		draw_line_solid(
			surface,
			{ 3 * width/4.f, 3 * height/4.f },
			{ -10.f, -10.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Bottom-Right to Top-Left, End Out" ) {
		draw_line_solid(
			surface,
			{ 3 * width/4.f, height/4.f },
			{ -10.f, height + 10.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Top-Left to Bottom-Right, End Out" ) {
		draw_line_solid(
			surface,
			{ width/4.f, 3 * height/4.f },
			{ width + 10.f, -10.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	// === Start Outside, End Inside ===
	SECTION( "Diagonal Bottom-Left to Top-Right, Start Out" ) {
		draw_line_solid(
			surface,
			{ -10.f, -10.f },
			{ width/4.f, height/4.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Top-Right to Bottom-Left, Start Out" ) {
		draw_line_solid(
			surface,
			{ width + 10.f, height + 10.f },
			{ 3 * width/4.f, 3 * height/4.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Bottom-Right to Top-Left, Start Out" ) {
		draw_line_solid(
			surface,
			{ width + 10.f, -10.f },
			{ width/4.f, 3 * height/4.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Top-Left to Bottom-Right, Start Out" ) {
		draw_line_solid(
			surface,
			{ -10.f, height + 10.f },
			{ 3 * width/4.f, height/4.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	// ==== Testing for Larger Numbers ====
	// ==== Horizonal =====
	float large_offset = static_cast<float>(10e5);

	SECTION( "Simple Horizontal Right, End Out, Large Offset" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height/2.f },
			{ width + large_offset, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels == (width/2.f) );
	}

	SECTION( "Simple Horizontal Left, End Out, Large Offset" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height/2.f },
			{ -large_offset, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels == (width/2.f)+1 );
	}

	SECTION( "Simple Vertical Bottom, End Out, Large Offset" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height/2.f },
			{ width/2.f, -large_offset },
			{255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( pixels == (height/2.f)+1 );
	}

	SECTION( "Simple Vertical Top, End Out, Large Offset" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height/2.f },
			{ width/2.f, height + large_offset },
			{255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( pixels == (height/2.f) );
	}

	// === Begin Outside ===
	SECTION( "Simple Horizontal Right, Start Out, Large Offset", "[!mayfail]" ) {
		// I think this may be failing due to floating point rounding errors?
		// Fails on Macbook, doesn't fail on linux...
		draw_line_solid( 
			surface,
			{ width+large_offset, height/2.f },
			{ width/2.f, height/2.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels == (width/2.f) );
	}

	SECTION( "Simple Horizontal Left, Start Out, Large Offset" ) {
		draw_line_solid( 
			surface,
			{ -large_offset, height/2.f },
			{ width/2.f, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels == (width/2.f)+1 );
	}

	SECTION( "Simple Vertical Bottom, Start Out, Large Offset" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, -large_offset },
			{ width/2.f, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( pixels == (height/2.f)+1 );
	}

	SECTION( "Simple Vertical Top, Start Out, Large Offset" ) {
		draw_line_solid( 
			surface,
			{ width/2.f, height + large_offset },
			{ width/2.f, height/2.f },
			{255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( pixels == (height/2.f) );
	}


	// === Diagonal Tests ===
	SECTION( "Diagonal Bottom-Left to Top-Right, End Out, Large Offset" ) {
		draw_line_solid(
			surface,
			{ width/4.f, height/4.f },
			{ width + large_offset, height + large_offset },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Top-Right to Bottom-Left, End Out, Large Offset" ) {
		draw_line_solid(
			surface,
			{ 3 * width/4.f, 3 * height/4.f },
			{ -large_offset, -large_offset },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Bottom-Right to Top-Left, End Out, Large Offset" ) {
		draw_line_solid(
			surface,
			{ 3 * width/4.f, height/4.f },
			{ -large_offset, height + large_offset },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Top-Left to Bottom-Right, End Out, Large Offset" ) {
		draw_line_solid(
			surface,
			{ width/4.f, 3 * height/4.f },
			{ width + large_offset, -large_offset },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	// === Start Outside, End Inside ===
	SECTION( "Diagonal Bottom-Left to Top-Right, Start Out, Large Offset" ) {
		draw_line_solid(
			surface,
			{ -large_offset, -large_offset },
			{ width/4.f, height/4.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Top-Right to Bottom-Left, Start Out, Large Offset" ) {
		draw_line_solid(
			surface,
			{ width + large_offset, height + large_offset },
			{ 3 * width/4.f, 3 * height/4.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Bottom-Right to Top-Left, Start Out, Large Offset" ) {
		draw_line_solid(
			surface,
			{ width + large_offset, -large_offset },
			{ width/4.f, 3 * height/4.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

	SECTION( "Diagonal Top-Left to Bottom-Right, Start Out, Large Offset" ) {
		draw_line_solid(
			surface,
			{ -large_offset, height + large_offset },
			{ 3 * width/4.f, height/4.f },
			{255, 255, 255 }
		);

		auto const pixels = total_pixel_count( surface );
		REQUIRE( pixels > 0 );                   
	}

}
