#include <cstdlib> // for exit()
#include "TextureLoaderKtx.h"
#include "ServiceLocator.h"

namespace ncine {

///////////////////////////////////////////////////////////
// STATIC DEFINITIONS
///////////////////////////////////////////////////////////

uint8_t TextureLoaderKtx::fileIdentifier_[] =
{0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A}; // "«KTX 11»\r\n\x1A\n"};

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

TextureLoaderKtx::TextureLoaderKtx(const char *filename)
	: ITextureLoader(filename)
{
	init();
}

TextureLoaderKtx::TextureLoaderKtx(IFile *fileHandle)
	: ITextureLoader(fileHandle)
{
	init();
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void TextureLoaderKtx::init()
{
	KtxHeader header;

	fileHandle_->open(IFile::MODE_READ | IFile::MODE_BINARY);
	readHeader(header);
	parseFormat(header);
}

void TextureLoaderKtx::readHeader(KtxHeader &header)
{
	bool checkPassed = true;

	// KTX header is 64 bytes long
	fileHandle_->read(&header, 64);

	for (int i = 0; i < KtxIdentifierLength; i++)
	{
		if (header.identifier[i] != fileIdentifier_[i])
		{
			checkPassed = false;
		}
	}

	// Checking for the header identifier
	if (checkPassed)
	{
		if (header.endianess == 0x01020304)
		{
			LOGF("File endianess doesn't match machine one");
			exit(EXIT_FAILURE);
		}

		headerSize_ = 64 + IFile::int32FromLE(header.bytesOfKeyValueData);
		width_ = IFile::int32FromLE(header.pixelWidth);
		height_ = IFile::int32FromLE(header.pixelHeight);
		mipMapCount_ = header.numberOfMipmapLevels;
	}
	else
	{
		LOGF("Not a KTX file");
		exit(EXIT_FAILURE);
	}
}

void TextureLoaderKtx::parseFormat(const KtxHeader& header)
{
	GLenum internalFormat = IFile::int32FromLE(header.glInternalFormat);
	GLenum type = IFile::int32FromLE(header.glType);

	loadPixels(internalFormat, type);

	if (mipMapCount_ > 1)
	{
		LOGI_X("MIP Maps: %d", mipMapCount_);
		mipDataOffsets_ = new long[mipMapCount_];
		mipDataSizes_ = new long[mipMapCount_];
		long int dataSizesSum = TextureFormat::calculateMipSizes(internalFormat, width_, height_, mipMapCount_, mipDataOffsets_, mipDataSizes_);

		// HACK: accounting for "UInt32 imageSize;" on top of each MIP level
		for (int i = 0; i < mipMapCount_; i++)
		{
			mipDataOffsets_[i] += 4 * (i + 1);
		}
		dataSizesSum += 4 * mipMapCount_;

		if (dataSizesSum != dataSize_)
		{
			LOGW_X("The sum of MIP maps size (%ld) is different than texture total data (%ld)", dataSizesSum, dataSize_);
		}
	}
}

}