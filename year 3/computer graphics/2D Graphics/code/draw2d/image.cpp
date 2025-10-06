#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include "surface-ex.hpp"
#include "surface.hpp"

#include "../support/error.hpp"

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"%s\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

void clip_blit_box( Surface& aSurface, int& minX, int& minY, int& maxX, int& maxY ) {
	if (minX < 0) minX = 0; 
	if (minY < 0) minY = 0; 

	if (maxX >= static_cast<int>(aSurface.get_width()))  
		maxX = static_cast<int>(aSurface.get_width()-1);

	if (maxY >= static_cast<int>(aSurface.get_height())) 
		maxY = static_cast<int>(aSurface.get_height()-1);
}

void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	int minX = aPosition.x - aImage.get_width()/2;
	int minY = aPosition.y - aImage.get_height()/2;
	int maxX = minX + aImage.get_width();
	int maxY = minY + aImage.get_height();

	// === CULLING ===
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

	// We need 2 sets of iterators. 
	//		1 for the source file which is from 0 -> image.width (i_src, j_src)
	//		1 for the dest file which is through the bounding box (i_dest, j_dest)
	ColorU8_sRGB_Alpha pixel;
	for ( size_t j_dest = static_cast<size_t>(minY), j_src = minY_src; j_dest < static_cast<size_t>(maxY); ++j_dest, ++j_src ) {
		for ( size_t i_dest = static_cast<size_t>(minX), i_src = minX_src; i_dest < static_cast<size_t>(maxX); ++i_dest, ++i_src ) {
			pixel = aImage.get_pixel(i_src, j_src);

			if (pixel.a >= 128) {
				aSurface.set_pixel_srgb(i_dest, j_dest, {pixel.r, pixel.g, pixel.b});
			}
		}
	}
}

namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
