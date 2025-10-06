#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE ( "Fully Offscreen", "culling" ) 

{
	Surface surface (640, 480);
	surface.clear();

	float small_offset = 100.f;
	float large_offset = 10e5;

	auto const width  = static_cast<float>(surface.get_width());
	auto const height = static_cast<float>(surface.get_height());

	ColorF c = { 1.f, 1.f, 1.f };

	SECTION( "Above screen" ) {
		draw_triangle_interp(
			surface,
			{ width/2.f, height + small_offset },
			{ width/2.f + 50.f, height + small_offset },
			{ width/2.f, height + small_offset + 50.f },
			c, c, c
		);

		REQUIRE ( count_nonzero_pixels( surface ) == 0 );

	}

	SECTION("Left of screen") {
		draw_triangle_interp(
			surface,
			{ -small_offset, height / 2.f },
			{ -small_offset + 50.f, height / 2.f },
			{ -small_offset, height / 2.f + 50.f },
			c, c, c
		);

		REQUIRE(count_nonzero_pixels(surface) == 0);
	}


    SECTION("Below screen") {
        draw_triangle_interp(
            surface,
            { width / 2.f, -small_offset },
            { width / 2.f + 50.f, -small_offset },
            { width / 2.f, -small_offset + 50.f },
            c, c, c
        );

        REQUIRE(count_nonzero_pixels(surface) == 0);
    }


    SECTION("Right of screen") {
        draw_triangle_interp(
            surface,
            { width + small_offset, height / 2.f },
            { width + small_offset, height / 2.f + 50.f },
            { width + small_offset - 50.f, height / 2.f },
            c, c, c
        );
        REQUIRE(count_nonzero_pixels(surface) == 0);
    }


    // === Same but use large offset ===
    SECTION("Above screen with Larg Offset") {
        draw_triangle_interp(
            surface,
            { width / 2.f, height + large_offset },
            { width / 2.f + 50.f, height + large_offset },
            { width / 2.f, height + large_offset + 50.f },
            c, c, c
        );

        REQUIRE(count_nonzero_pixels(surface) == 0);
    }

    SECTION("Left of screen with Larg Offset") {
        draw_triangle_interp(
            surface,
            { -large_offset, height / 2.f },
            { -large_offset + 50.f, height / 2.f },
            { -large_offset, height / 2.f + 50.f },
            c, c, c
        );

        REQUIRE(count_nonzero_pixels(surface) == 0);
    }

    SECTION("Below screen with Larg Offset") {
        draw_triangle_interp(
            surface,
            { width / 2.f, -large_offset },
            { width / 2.f + 50.f, -large_offset },
            { width / 2.f, -large_offset + 50.f },
            c, c, c
        );

        REQUIRE(count_nonzero_pixels(surface) == 0);
    }

    SECTION("Right of screen with Larg Offset") {
        draw_triangle_interp(
            surface,
            { width + large_offset, height / 2.f },
            { width + large_offset, height / 2.f + 50.f },
            { width + large_offset - 50.f, height / 2.f },
            c, c, c
        );

        REQUIRE(count_nonzero_pixels(surface) == 0);
    }
}

