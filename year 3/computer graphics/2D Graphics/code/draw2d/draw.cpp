#include "draw.hpp"

#include <algorithm>

#include <cmath>
#include <cstdlib>

#include "surface.hpp"

bool is_offscreen(Surface& surface, Vec2f start, Vec2f end) {
	return ( start.x < 0 && end.x < 0 ) ||
		   ( start.y < 0 && end.y < 0 )	||
	       ( start.x > surface.get_width()  && end.x   > surface.get_width() ) ||
		   ( start.y > surface.get_height() && end.y > surface.get_height() );
}

void clip_line( Surface& aSurface, Vec2f& aBegin, Vec2f& aEnd ) {
	/*
	 *	Clipping
	 *	If t_x < 0 then the intersection is before P_x, otherwise we need to adjust using:
	 *	L(t) = P + td
	 *	Likewise if t_xw > 1 then the intersection is after the end of our line...
	 *	
	 *	NOTE
	 *	Notice we only check if aBegin < 0 and aEnd > width or height.
	 *	Therefore we want aBegin to be the smaller value.
	 *	the first if statement handles this.
	 * 
	 *	This is my original clipping  algorithm. It uses the parametric line equation
	 *	to work out the intersections of x and y independently. This method fails when
	 *	both x and y are out of bounds...
	 *
	 * */

	// We want to be drawing left to right for this to work
	if (aEnd.x < aBegin.x || aEnd.y < aBegin.y) {
		std::swap(aBegin, aEnd);
	}

	Vec2f diff = aEnd - aBegin; 

	// Calculate the value of t_x0 and t_xw
	float t_x0 = -aBegin.x / diff.x;							   // t_x0 = -P_x/d_x
	float t_xw = (aSurface.get_width() - aBegin.x) / diff.x;	   // t_xw = (width - P_x) / d_x

	if (t_x0 >= 0 && t_x0 <= 1) {
		// Change both x and y
		aBegin.x = aBegin.x + t_x0 * diff.x;
		aBegin.y = aBegin.y + t_x0 * diff.y;

	} else if (t_xw >= 0 && t_xw <= 1) {
		aEnd.x = aBegin.x + t_xw * diff.x;
		aEnd.y = aBegin.y + t_xw * diff.y;
	}

	// Repeat for y
	// Calculate the value of t_y0 and t_yw
	float t_y0 = (-aBegin.y) / diff.y;							// t_y0 = -P_y/d_y
	float t_yh = (aSurface.get_height() - aBegin.y) / diff.y;	// t_yh = (height - P_y) / d_y

	if (t_y0 >= 0 && t_y0 <= 1) {
		aBegin.y = aBegin.y + t_y0 * diff.y;
		aBegin.x = aBegin.x + t_y0 * diff.x;
	} else if (t_yh >= 0 && t_yh <= 1) {
		aEnd.y = aBegin.y + t_yh * diff.y;
		aEnd.x = aBegin.x + t_yh * diff.x;
	}

	// Clamp to width-1 and height-1
	aBegin.x = aBegin.x >= aSurface.get_width() ? aSurface.get_width() - 1 : aBegin.x;
	aBegin.y = aBegin.y >= aSurface.get_height() ? aSurface.get_height() - 1 : aBegin.y;

	aEnd.x = aEnd.x >= aSurface.get_width() ? aSurface.get_width() - 1 : aEnd.x;
	aEnd.y = aEnd.y >= aSurface.get_height() ? aSurface.get_height() - 1 : aEnd.y;
}

bool liang_barsky( Surface& aSurface, Vec2f& aBegin, Vec2f& aEnd ) {
	/*
	 *	  ============= Liang-Barsky Clipping =============
	 *	https://www.geeksforgeeks.org/liang-barsky-algorithm/
	 *
	 * */

	float dx = aEnd.x - aBegin.x;
	float dy = aEnd.y - aBegin.y;

	// p values represent directional indicators
	// AKA then lines slope relative to each boundary
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

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{

	/*
	 *		========== DDA ==========
	 *
	 *		https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)
	 *		https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/
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

	if (!liang_barsky(aSurface, aBegin, aEnd))
		return;

	// Calculate difference between start and finish
	Vec2f diff = aEnd - aBegin;

	// We use the large absolute value of the difference as the steps
	float stepNum = std::max(fabs(diff.x), fabs(diff.y));

	// Calculate how much to increment each iteration
	// Don't need to worry about division by zero because 
	// We've already handled the case where aBegin == aEnd
	float incX = diff.x / stepNum;
	float incY = diff.y / stepNum;

	float x = aBegin.x,
		  y = aBegin.y;

	for (int i = 0; i <= stepNum; ++i, x += incX, y += incY) {
		// Round so that each step fills a pixel
		aSurface.set_pixel_srgb(
			static_cast<uint32_t>(std::round(x)),
			static_cast<uint32_t>(std::round(y)), 
			aColor
		);
	}
}

void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	draw_line_solid(aSurface, aP0, aP1, aColor);
	draw_line_solid(aSurface, aP1, aP2, aColor);
	draw_line_solid(aSurface, aP2, aP0, aColor);
}

void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor ) {
	ColorF c = linear_from_srgb(aColor);
	draw_triangle_interp(aSurface, aP0, aP1, aP2, c, c, c);
}


bool is_inside_half_plane(Vec2f n, Vec2f X, Vec2f P) {
	/*
	 *		=== Half-Plane test ===
	 *		F(X) = n x (X - P)
	 *		n = { -d.y, d.x }
	 *
	 *		if F(X) > 0 then X is above line
	 *			< 0 then X is below line
	 *			= 0 then X is on line
	 *
	 *		NOTE 
	 *		We assume the points are given in CCW.
	 * */

	return dot(n, X-P) >= 0;
}

float triangle_area(Vec2f aP0, Vec2f aP1, Vec2f aP2) {
	// Calculate the area of a triangle given 3 points 
	// Equation from lecture 4 slides
	Vec2f v = aP2 - aP0;
	Vec2f u = aP1 - aP0;

	return std::fabs((u.x*v.y - u.y*v.x) / 2.f);
}

ColorF barycentric_interp( ColorF aC0, ColorF aC1, ColorF aC2, float alpha, float beta, float gamma) {
	return {
		(alpha * aC0.r + beta*aC1.r + gamma*aC2.r),
		(alpha * aC0.g + beta*aC1.g + gamma*aC2.g),
		(alpha * aC0.b + beta*aC1.b + gamma*aC2.b)
	};
}

void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	/* 
	 *	======================== Triangle Drawing ========================
	 *
	 *		Uses half-plane test to see if point is in triangle.
	 *		Uses barycentric interpolation to interpolate.
	 *		We assume that the points are given to us in CCW order.
	 *		
	 */


	// All colours are the same
	bool is_solid = aC0.r == aC1.r && aC0.g == aC1.g && aC0.b == aC1.b && 
						 aC1.r == aC2.r && aC1.g == aC2.g && aC1.b == aC2.b;

	// Calculate the bounding box of the triangle
	// If we clip the bounding box we don't have to test each pixel later
	int minX = static_cast<int>(std::floor(std::min({aP0.x, aP1.x, aP2.x })));
	int minY = static_cast<int>(std::floor(std::min({aP0.y, aP1.y, aP2.y })));

	int maxX = static_cast<int>(std::ceil(std::max({aP0.x, aP1.x, aP2.x })));
	int maxY = static_cast<int>(std::ceil(std::max({aP0.y, aP1.y, aP2.y })));

	minX = std::max(0, minX);
	minY = std::max(0, minY);

	maxX = std::min(static_cast<int>(aSurface.get_width()-1), maxX);
	maxY = std::min(static_cast<int>(aSurface.get_height()-1), maxY);

	Vec2f d0 = aP1 - aP0;
	Vec2f d1 = aP2 - aP1;
	Vec2f d2 = aP0 - aP2;

	// n is perpendicular to d
	Vec2f n0 = { -d0.y, d0.x };
	Vec2f n1 = { -d1.y, d1.x };
	Vec2f n2 = { -d2.y, d2.x };

	float total_area = triangle_area(aP0, aP1, aP2);

	if (total_area == 0.f) return;

	for (int j = minY; j <= maxY; j++) {
		for (int i = minX; i <= maxX; i++) {
			Vec2f X = { static_cast<float>(i), static_cast<float>(j) };

			if (is_inside_half_plane(n0, X, aP0) &&
				 is_inside_half_plane(n1, X, aP1) &&
				 is_inside_half_plane(n2, X, aP2)) {

				if (is_solid) {
					aSurface.set_pixel_srgb(X.x, X.y, linear_to_srgb(aC0));
					continue;
				}

				// Calculate barycentric coordinates
				float alpha = triangle_area(aP1, aP2, X) / total_area;		// BCU / ABC
				float beta  = triangle_area(aP0, X, aP2) / total_area;		// AUC / ABC
				float gamma = triangle_area(aP0, aP1, X) / total_area;		// ABU / ABC

				ColorF c = barycentric_interp(aC0, aC1, aC2, alpha, beta, gamma);

				aSurface.set_pixel_srgb(X.x, X.y, linear_to_srgb(c));
			}
		}
	}
}


// --- DON'T NEED FOR CWK1 ---
void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}
