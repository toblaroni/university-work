#include <benchmark/benchmark.h>

#include "../draw2d/draw.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace
{
	// This is a placeholder. Replace this with yor own code. Refer to
	// blit-benchmark/main.cpp for a more complete example. 
	void dda( benchmark::State& aState )
	{
		auto const width  = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f begin = { static_cast<float>(aState.range(2)), static_cast<float>(aState.range(3)) };
		Vec2f end   = { static_cast<float>(aState.range(4)), static_cast<float>(aState.range(5)) };

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
            draw_line_solid( surface, begin, end, {255, 255, 255} );

			benchmark::ClobberMemory(); 
		}

        // Lets do something similar to the blitting bytes processed approximation
        // We can calculate the length of the line using pythagoras, and use that to see
        // How many bytes were written...
        Vec2f d = end - begin;
        float line_length = std::sqrt( std::pow(d.x, 2) + std::pow(d.y, 2) );

        aState.SetBytesProcessed( line_length * 4 * aState.iterations() );  // 4 bytes per pixel
	}

	void bresenhams( benchmark::State& aState )
	{
		auto const width  = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f begin = { static_cast<float>(aState.range(2)), static_cast<float>(aState.range(3)) };
		Vec2f end   = { static_cast<float>(aState.range(4)), static_cast<float>(aState.range(5)) };

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
            draw_ex_line_solid( surface, begin, end, {255, 255, 255} );

			benchmark::ClobberMemory(); 
		}

        Vec2f d = end - begin;
        float line_length = std::sqrt( std::pow(d.x, 2) + std::pow(d.y, 2) );

        aState.SetBytesProcessed( line_length * 4 * aState.iterations() );  // 4 bytes per pixel
	}
}

// Horizontal Line
BENCHMARK( dda )
    ->Args({320, 240, 10, 120, 310, 120})       // Horizontal line
    ->Args({1280, 720, 10, 360, 1270, 360})
    ->Args({1920, 1080, 10, 540, 1910, 540})  
    ->Args({7680, 4320, 10, 2160, 7670, 2160})

    ->Args({320, 240, 160, 10, 160, 230})		// Vertical line
    ->Args({1280, 720, 640, 10, 640, 710})
    ->Args({1920, 1080, 960, 10, 960, 1070})
    ->Args({7680, 4320, 3840, 10, 3840, 4310})

    ->Args({320, 240, 10, 10, 310, 230})		// Diagonal (positive gradient)
    ->Args({1280, 720, 10, 10, 1270, 710})   
    ->Args({1920, 1080, 10, 10, 1910, 1070}) 
    ->Args({7680, 4320, 10, 10, 7670, 4310})

    ->Args({320, 240, 10, 230, 310, 10})		// Diagonal (negative gradient)
    ->Args({1280, 720, 10, 710, 1270, 10})
    ->Args({1920, 1080, 10, 1070, 1910, 10})
    ->Args({7680, 4320, 10, 4310, 7670, 10});

BENCHMARK( bresenhams )
    ->Args({320, 240, 10, 120, 310, 120})       // Horizontal line
    ->Args({1280, 720, 10, 360, 1270, 360})
    ->Args({1920, 1080, 10, 540, 1910, 540})  
    ->Args({7680, 4320, 10, 2160, 7670, 2160})

    ->Args({320, 240, 160, 10, 160, 230})		// Vertical line
    ->Args({1280, 720, 640, 10, 640, 710})
    ->Args({1920, 1080, 960, 10, 960, 1070})
    ->Args({7680, 4320, 3840, 10, 3840, 4310})

    ->Args({320, 240, 10, 10, 310, 230})		// Diagonal (positive gradient)
    ->Args({1280, 720, 10, 10, 1270, 710})   
    ->Args({1920, 1080, 10, 10, 1910, 1070}) 
    ->Args({7680, 4320, 10, 10, 7670, 4310})

    ->Args({320, 240, 10, 230, 310, 10})		// Diagonal (negative gradient)
    ->Args({1280, 720, 10, 710, 1270, 10})
    ->Args({1920, 1080, 10, 1070, 1910, 10})
    ->Args({7680, 4320, 10, 4310, 7670, 10});

BENCHMARK_MAIN();
