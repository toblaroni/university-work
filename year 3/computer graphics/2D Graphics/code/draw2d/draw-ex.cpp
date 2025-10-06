#include "draw-ex.hpp"

#include <algorithm>

#include <cstring> // for std::memcpy()

#include "image.hpp"
#include "surface-ex.hpp"


bool liang_barsky( SurfaceEx& aSurface, Vec2f& aBegin, Vec2f& aEnd ) {
	/*
	 *	  ============= Liang-Barsky Clipping =============
	 *	https://www.geeksforgeeks.org/liang-barsky-algorithm/
	 *
	 * */

	float dx = aEnd.x - aBegin.x;
	float dy = aEnd.y - aBegin.y;

	// p values represent directional indicators
	// AKA then lines slop relative to each boundary
	float p[4] = {-dx, dx, -dy, dy};

	// q values represent the distance from the line's start to the boundary.
	float q[4] = { 
		aBegin.x,							
		(aSurface.get_width()-1) - aBegin.x,	// -1 from width to avoid index overflow
		aBegin.y,
		(aSurface.get_height()-1) - aBegin.y	// Same for height
	};
	
	float t_start = 0.f,
		  t_end   = 1.f;

	float t;
	for (int i = 0; i < 4; i++) {
		if (p[i] == 0) {	
			if (q[i] < 0) 
				return false;	// The line is parallel (p = 0) and outside so we don't render
		} else {
			t = q[i] / p[i];	// This t value represents the intersection with the boundaries
	
			if (p[i] < 0) {
				// Line enters boundary
				if (t > t_start) {	// The start of the line is before the boundary
					t_start = t;	
				}
			} else {
				// Line exits a boundary
				if (t < t_end) {
					t_end = t;
				}
			}
		}
	}

	if (t_start > t_end)	// Is the starting point further along than the exit point
		return false;

	// Clamp values to [0,1]
	t_start = std::max(0.0f, t_start);
	t_end = std::min(1.0f, t_end);

	// Adjust start and end points using t_start and t_end
	aEnd.x   = aBegin.x + t_end * dx;
	aEnd.y   = aBegin.y + t_end * dy;
	aBegin.x = aBegin.x + t_start * dx;
	aBegin.y = aBegin.y + t_start * dy;

	return true;
}

void draw_ex_line_solid( SurfaceEx& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	/*
	 *	========== Bresenham's Line Drawing Algorithm ==========
	 *
	 *	https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
	 *	https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	 *
	 * */

	aBegin = { std::round(aBegin.x), std::round(aBegin.y) };
	aEnd   = { std::round(aEnd.x), std::round(aEnd.y) };

	if (aBegin.x == aEnd.x && aBegin.y == aEnd.y) {
		// Just draw a pixel if they're the same
		aSurface.set_pixel_srgb(
			static_cast<uint32_t>(aBegin.x), 
			static_cast<uint32_t>(aBegin.y), 
			aColor
		);

		return;
	}

	int dx = static_cast<int>(std::round(aEnd.x - aBegin.x));
	int dy = static_cast<int>(std::round(aEnd.y - aBegin.y));

	// We need to handle situations where m > 1
	// This could be positive or negative
	bool steep_line = abs(dy) > abs(dx);

	// Invert the axis for steep lines
	if (steep_line) {
		std::swap(aBegin.x, aBegin.y);
		std::swap(aEnd.x, aEnd.y);
		std::swap(dx, dy);
	}

	// Make sure we're drawing from left to right
	if (aBegin.x > aEnd.x) {
		std::swap(aBegin, aEnd);
		dx = -dx;
		dy = -dy;
	}

	// Use same clipping algorithm
	if (!liang_barsky(aSurface, aBegin, aEnd))
		return;

	// This is our decision variable, it is equivalent to f(x+1, y+1/2)
	int D = 2*dy - dx;
	int y_inc = aBegin.y < aEnd.y ? 1 : -1;

	for (int x = aBegin.x, y = aBegin.y; x <= aEnd.x; x++) {
		// Here we switch the x and y axis if the line is steep. (refer to the Wikipedia).
		if (steep_line) {
			aSurface.set_pixel_srgb(y, x, aColor);		// Swap back when rendering
		} else {
			aSurface.set_pixel_srgb(x, y, aColor);
		}

		// Update D
		if (D > 0) {	// Above the line
			y += y_inc;
			D -= 2*abs(dx);
		}
		
		D += 2*abs(dy);
	}
}

void clip_blit_box( SurfaceEx& aSurface, int& minX, int& minY, int& maxX, int& maxY ) {
	if (minX < 0) minX = 0; 
	if (minY < 0) minY = 0; 

	if (maxX >= static_cast<int>(aSurface.get_width()))  
			maxX = static_cast<int>(aSurface.get_width()-1);

	if (maxY >= static_cast<int>(aSurface.get_height())) 
			maxY = static_cast<int>(aSurface.get_height()-1);
}

void blit_ex_solid( SurfaceEx& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{	
	/*
	 *	Same as blit_masked but without the alpha pixel check
	 * */
	int minX = aPosition.x - aImage.get_width()/2;
	int minY = aPosition.y - aImage.get_height()/2;
	int maxX = minX + aImage.get_width();
	int maxY = minY + aImage.get_height();

	// If image is completely off screen, return
	if (maxX < 0 || maxY < 0)
		return;
	if (minX >= static_cast<int>(aSurface.get_width()) || minY >= static_cast<int>(aSurface.get_height()))
		return;

	// Calculate source indices
	// (offset the source)
	uint32_t minX_src = minX < 0 ? -minX : 0;	
	uint32_t minY_src = minY < 0 ? -minY : 0;	

	// Adjust bounding box depending on the surface dimensions
	clip_blit_box(aSurface, minX, minY, maxX, maxY);

	ColorU8_sRGB_Alpha pixel;
	for ( size_t j_dest = static_cast<size_t>(minY), j_src = minY_src; j_dest < static_cast<size_t>(maxY); ++j_dest, ++j_src ) {
		for ( size_t i_dest = static_cast<size_t>(minX), i_src = minX_src; i_dest < static_cast<size_t>(maxX); ++i_dest, ++i_src ) {
			pixel = aImage.get_pixel(i_src, j_src);

			aSurface.set_pixel_srgb(i_dest, j_dest, {pixel.r, pixel.g, pixel.b});
		}
	}
}

void blit_ex_memcpy( SurfaceEx& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	int minX = aPosition.x - aImage.get_width()/2;
	int minY = aPosition.y - aImage.get_height()/2;
	int maxX = minX + aImage.get_width();
	int maxY = minY + aImage.get_height();

	// If image is completely off screen, return
	if (maxX < 0 || maxY < 0)
		return;
	if (minX >= static_cast<int>(aSurface.get_width()) || minY >= static_cast<int>(aSurface.get_height()))
		return;

	// Calculate source indices
	// (offset the source)
	uint32_t minX_src = minX < 0 ? -minX : 0;	
	uint32_t minY_src = minY < 0 ? -minY : 0;	

	// Adjust bounding box depending on the surface dimensions
	clip_blit_box(aSurface, minX, minY, maxX, maxY);

	// Width of each line we want to copy
	//	-> It's the width of the bounding box
	std::size_t cpy_size = maxX - minX;

	// Only need to loop through y values since we're row-major
	//	-> AKA rows are in contiguous addresses
	for ( size_t j_dest = static_cast<size_t>(minY), j_src = minY_src; j_dest < static_cast<size_t>(maxY); ++j_dest, ++j_src ) {
		ImageRGBA::Index index_src  = aImage.get_linear_index(static_cast<std::uint32_t>(minX_src), static_cast<std::uint32_t>(j_src));
		ImageRGBA::Index index_dest = aSurface.get_linear_index(static_cast<std::uint32_t>(minX), static_cast<std::uint32_t>(j_dest));

		std::uint8_t const* ptr_src = aImage.get_image_ptr() + index_src;
		std::uint8_t *ptr_dest		= aSurface.get_surface_ptr() + index_dest;

		std::memcpy( (void*)ptr_dest, ptr_src, sizeof(uint32_t)*cpy_size );
	}
}

