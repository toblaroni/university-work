#include <benchmark/benchmark.h>

#include <algorithm>

#include <cassert>

#include "../draw2d/image.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

static const char* images[2] = { "assets/earth.png", "assets/earth_128.png" };

namespace
{
	void default_blit_earth( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( images[aState.range(2)] );
		assert( source );

		for( auto _ : aState )
		{
			// Make sure as much as possible of the image is visible. The
			// sample image here is 1k*1k. Blitting to center = (500,500) makes
			// sure that as much as possible of the image ends up in the target
			// surface.
			blit_masked( surface, *source, { surface.get_width() / 2.f, surface.get_height() / 2.f } );		// Blit to the center of the surface

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory(); 
		}

		// The following enables the benchmarking library to print information
		// about the memory bandwidth. The total number of bytes processed is
		// *approximatively* two times the total number of bytes in the blit,
		// accounding for both reading and writing. ("Approximatively" since
		// not all pixels are written.)
		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
	}


	void blit_ex_solid_benchmark( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( images[aState.range(2)] );
		assert( source );

		for( auto _ : aState )
		{
			blit_ex_solid( surface, *source, { surface.get_width() / 2.f, surface.get_height() / 2.f } );
			benchmark::ClobberMemory(); 
		}

		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2 * maxBlitX * maxBlitY * 4 * aState.iterations() );
	}

	void blit_ex_memcpy_benchmark( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( images[aState.range(2)] );
		assert( source );

		for( auto _ : aState )
		{
			blit_ex_memcpy( surface, *source, { surface.get_width() / 2.f, surface.get_height() / 2.f } );
			benchmark::ClobberMemory(); 
		}

		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2 * maxBlitX * maxBlitY * 4 * aState.iterations() );
	}
}

// 0 means big image, 1 means small...
BENCHMARK( default_blit_earth ) 
	->Args( { 320, 240, 0 } )
	->Args( { 1280, 720, 0 } )
	->Args( { 1920, 1080, 0 } )
	->Args( { 7680, 4320, 0 } )
	->Args( { 320, 240, 1 } )
	->Args( { 1280, 720, 1 } )
	->Args( { 1920, 1080, 1 } )
	->Args( { 7680, 4320, 1 } );

BENCHMARK( blit_ex_solid_benchmark )
	->Args( { 320, 240, 0 } )
	->Args( { 1280, 720, 0 } )
	->Args( { 1920, 1080, 0 } )
	->Args( { 7680, 4320, 0 } )
	->Args( { 320, 240, 1 } )
	->Args( { 1280, 720, 1 } )
	->Args( { 1920, 1080, 1 } )
	->Args( { 7680, 4320, 1 } );

BENCHMARK( blit_ex_memcpy_benchmark )
	->Args( { 320, 240, 0 } )
	->Args( { 1280, 720, 0 } )
	->Args( { 1920, 1080, 0  } )
	->Args( { 7680, 4320, 0  } )
	->Args( { 320, 240, 1 } )
	->Args( { 1280, 720, 1 } )
	->Args( { 1920, 1080, 1 } )
	->Args( { 7680, 4320, 1 } );

BENCHMARK_MAIN();
