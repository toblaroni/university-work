#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE ( "Both Points Out", "[clip][!mayfail]" ) {
    Surface surface( 640, 480 );
    surface.clear();

    auto const width  = surface.get_width();
    auto const height = surface.get_height();

    float small_offset = 10.f;

    // === Simple Vertical ===
    SECTION( "Vertical Top to Bottom" ) {
        draw_line_solid(
            surface,
            { width / 2.f, height + small_offset },
            { width / 2.f, -small_offset },
            {255, 255, 255 }
        );

        auto const pixels = max_col_pixel_count( surface );
        REQUIRE( pixels == height );
    }

    SECTION( "Vertical Bottom to Top" ) {
        draw_line_solid(
            surface,
            { width / 2.f, -small_offset },
            { width / 2.f, height + small_offset },
            {255, 255, 255 }
        );

        auto const pixels = max_col_pixel_count( surface );
        REQUIRE( pixels == height );
    }

    // === Simple Horizontal ===
    SECTION( "Horizontal Left to Right" ) {
        draw_line_solid(
            surface,
            { -small_offset, height / 2.f },
            { width + small_offset, height / 2.f },
            {255, 255, 255 }
        );

        auto const pixels = max_row_pixel_count( surface );
        REQUIRE( pixels == width );
    }

    SECTION( "Horizontal Right to Left" ) {
        draw_line_solid(
            surface,
            { width + small_offset, height / 2.f },
            { -small_offset, height / 2.f },
            {255, 255, 255 }
        );

        auto const pixels = max_row_pixel_count( surface );
        REQUIRE( pixels == width );
    }

    // === Completely Out Off-Screen ===
    // === Top, Bottom, Left, Right Edge cases ===
    SECTION( "Above Window" ) {
        draw_line_solid(
            surface,
            { width / 4.f, height + small_offset },
            { 3 * width / 4.f, height + small_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels == 0 );
    }

    SECTION( "Below Window" ) {
        draw_line_solid(
            surface,
            { width / 4.f, -small_offset },
            { 3 * width / 4.f, -small_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels == 0 );
    }

    SECTION( "Left of Window" ) {
        draw_line_solid(
            surface,
            { -small_offset, height / 4.f },
            { -small_offset, 3 * height / 4.f },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels == 0 );
    }

    SECTION( "Right of Window" ) {
        draw_line_solid(
            surface,
            { width + small_offset, height / 4.f },
            { width + small_offset, 3 * height / 4.f },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels == 0 );
    }

    // === Diagonal Outside Corners ===
    SECTION( "Top-Left to Bottom-Right Diagonal Outside" ) {
        draw_line_solid(
            surface,
            { -small_offset, height + small_offset },
            { width + small_offset, -small_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels > 0 ); // Make sure at least some pixels are drawn
    }

    SECTION( "Top-Right to Bottom-Left Diagonal Outside" ) {
        draw_line_solid(
            surface,
            { width + small_offset, height + small_offset },
            { -small_offset, -small_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels > 0 );
    }

    SECTION( "Bottom-Left to Top-Right Diagonal Outside" ) {
        draw_line_solid(
            surface,
            { -small_offset, -small_offset },
            { width + small_offset, height + small_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels > 0 );
    }

    SECTION( "Bottom-Right to Top-Left Diagonal Outside" ) {
        draw_line_solid(
            surface,
            { width + small_offset, -small_offset },
            { -small_offset, height + small_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels > 0 );
    }


    // === Bigger Offset ===
	float large_offset = static_cast<float>(10e5);

    // === Simple Vertical ===
    SECTION( "Vertical Top to Bottom, Large Offset", "![mayfail]" ) {
        draw_line_solid(
            surface,
            { width / 2.f, height + large_offset },
            { width / 2.f, -large_offset },
            {255, 255, 255 }
        );

        auto const pixels = max_col_pixel_count( surface );
        REQUIRE( pixels == height );
    }

    SECTION( "Vertical Bottom to Top, Large Offset" ) {
        draw_line_solid(
            surface,
            { width / 2.f, -large_offset },
            { width / 2.f, height + large_offset },
            {255, 255, 255 }
        );

        auto const pixels = max_col_pixel_count( surface );
        REQUIRE( pixels == height );
    }

    // === Simple Horizontal ===
    SECTION( "Horizontal Left to Right, Large Offset" ) {
        draw_line_solid(
            surface,
            { -large_offset, height / 2.f },
            { width + large_offset, height / 2.f },
            {255, 255, 255 }
        );

        auto const pixels = max_row_pixel_count( surface );
        REQUIRE( pixels == width );
    }

    SECTION( "Horizontal Right to Left, Large Offset", "[!mayfail]" ) {
        draw_line_solid(
            surface,
            { width + large_offset, height / 2.f },
            { -large_offset, height / 2.f },
            {255, 255, 255 }
        );

        auto const pixels = max_row_pixel_count( surface );
        REQUIRE( pixels == width );
    }

    // === Completely Out Off-Screen ===
    // === Top, Bottom, Left, Right Edge cases ===
    SECTION( "Above Window, Large Offset" ) {
        draw_line_solid(
            surface,
            { width / 4.f, height + large_offset },
            { 3 * width / 4.f, height + large_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels == 0 );
    }

    SECTION( "Below Window, Large Offset" ) {
        draw_line_solid(
            surface,
            { width / 4.f, -large_offset },
            { 3 * width / 4.f, -large_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels == 0 );
    }

    SECTION( "Left of Window, Large Offset" ) {
        draw_line_solid(
            surface,
            { -large_offset, height / 4.f },
            { -large_offset, 3 * height / 4.f },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels == 0 );
    }

    SECTION( "Right of Window, Large Offset" ) {
        draw_line_solid(
            surface,
            { width + large_offset, height / 4.f },
            { width + large_offset, 3 * height / 4.f },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels == 0 );
    }

    // === Diagonal Outside Corners ===
    SECTION( "Top-Left to Bottom-Right Diagonal Outside, Large Offset" ) {
        draw_line_solid(
            surface,
            { -large_offset, height + large_offset },
            { width + large_offset, -large_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels > 0 ); // Make sure at least some pixels are drawn
    }

    SECTION( "Top-Right to Bottom-Left Diagonal Outside, Large Offset" ) {
        draw_line_solid(
            surface,
            { width + large_offset, height + large_offset },
            { -large_offset, -large_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels > 0 );
    }

    SECTION( "Bottom-Left to Top-Right Diagonal Outside, Large Offset" ) {
        draw_line_solid(
            surface,
            { -large_offset, -large_offset },
            { width + large_offset, height + large_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels > 0 );
    }

    SECTION( "Bottom-Right to Top-Left Diagonal Outside, Large Offset" ) {
        draw_line_solid(
            surface,
            { width + large_offset, -large_offset },
            { -large_offset, height + large_offset },
            {255, 255, 255 }
        );

        auto const pixels = total_pixel_count( surface );
        REQUIRE( pixels > 0 );
    }
}

