// MIT License

// Copyright (c) 2012 DK22Pac
// Copyright (c) 2017 FYP

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "pch.h"
#include "texture_manager.h"


RwTexture* createRwTextureFromRwImage(RwImage* image)
{
	RwInt32 width, height, depth, flags;
	RwImageFindRasterFormat(image, 4, &width, &height, &depth, &flags);
	RwRaster* raster = RwRasterCreate(width, height, depth, flags);
	RwRasterSetFromImage(raster, image);
	RwImageDestroy(image);
	RwTexture* texture = RwTextureCreate(raster);
	return texture;
}

void setRwTextureNameFromFilename(RwTexture* texture, const fs::path& path)
{
	path.stem().string().copy(texture->name, sizeof(texture->name) - 1);
}

RwTexture* texture_manager::loadTextureFromPngFile(fs::path path)
{
	RwImage* image = RtPNGImageRead(path.string().c_str());
	if (!image)
		return nullptr;
	RwTexture* texture = createRwTextureFromRwImage(image);
	setRwTextureNameFromFilename(texture, path);
	return texture;
}

RwTexture* texture_manager::loadMaskedTextureFromBmpFile(fs::path imagePath, fs::path maskPath)
{
	RwImage* image = RtBMPImageRead(imagePath.string().c_str());
	if (!image)
		return nullptr;
	if (!maskPath.empty())
	{
		RwImage* mask = RtBMPImageRead(maskPath.string().c_str());
		if (mask)
		{
			RwImageMakeMask(mask);
			RwImageApplyMask(image, mask);
			RwImageDestroy(mask);
		}
	}
	RwTexture* texture = createRwTextureFromRwImage(image);
	setRwTextureNameFromFilename(texture, imagePath);
	return texture;
}

RwTexture* texture_manager::loadTextureFromDdsFile(fs::path path)
{
	fs::path pathWithoutExt = path.parent_path() / path.stem();
	RwTexture* texture = RwD3D9DDSTextureRead(pathWithoutExt.string().c_str(), nullptr);
	if (!texture)
		return nullptr;
	setRwTextureNameFromFilename(texture, path);
	return texture;
}

void texture_manager::unloadTexture(RwTexture* texture)
{
	if (!texture)
		return;
	RwTextureDestroy(texture);
}
