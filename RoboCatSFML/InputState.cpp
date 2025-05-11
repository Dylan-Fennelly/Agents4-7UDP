#include "RoboCatPCH.hpp"

namespace
{
	void WriteSignedBinaryValue(OutputMemoryBitStream& inOutputStream, float inValue)
	{
		bool isNonZero = (inValue != 0.f);
		inOutputStream.Write(isNonZero);
		if (isNonZero)
		{
			inOutputStream.Write(inValue > 0.f);
		}
	}

	void ReadSignedBinaryValue(InputMemoryBitStream& inInputStream, float& outValue)
	{
		bool isNonZero;
		inInputStream.Read(isNonZero);
		if (isNonZero)
		{
			bool isPositive;
			inInputStream.Read(isPositive);
			outValue = isPositive ? 1.f : -1.f;
		}
		else
		{
			outValue = 0.f;
		}
	}

	void WriteQuantizedAngle(OutputMemoryBitStream& outStream, float angle)
	{
		// Ensure the angle is in [0, 360)
		angle = fmodf(angle + 360.f, 360.f);

		// Convert to [0, 255]
		uint8_t quantized = static_cast<uint8_t>((angle / 360.f) * 255.f);

		outStream.Write(quantized);
	}
	void ReadQuantizedAngle(InputMemoryBitStream& inStream, float& angle)
	{
		uint8_t quantized;
		inStream.Read(quantized);
		// Convert back to [0, 360)
		angle = (static_cast<float>(quantized) / 255.f) * 360.f;
	}
}

bool InputState::Write(OutputMemoryBitStream& inOutputStream) const
{
	uint8_t inputMask = 0;

	if (mDesiredForwardAmount != 0.f) inputMask |= (1 << 0);
	if (mDesiredBackAmount != 0.f) inputMask |= (1 << 1);
	if (mDesiredLeftAmount != 0.f) inputMask |= (1 << 2);
	if (mDesiredRightAmount != 0.f) inputMask |= (1 << 3);


	inOutputStream.Write(inputMask,4); // only 4 bits
	//WriteSignedBinaryValue(inOutputStream, mDesiredRightAmount);
	//WriteSignedBinaryValue(inOutputStream, mDesiredLeftAmount);
	//WriteSignedBinaryValue(inOutputStream, mDesiredForwardAmount);
	//WriteSignedBinaryValue(inOutputStream, mDesiredBackAmount);
	inOutputStream.Write(mIsShooting);  // 1 bit

	// Only encode quantised float as byte — don't re-quantise
	uint8_t quantized = static_cast<uint8_t>((mDesiredRotation / 360.f) * 255.f);
	inOutputStream.Write(quantized);

	return false;
}

bool InputState::Read(InputMemoryBitStream& inInputStream)
{
	uint8_t inputMask;
	inInputStream.Read(inputMask,4);
	//Output the bit mask values
	//std::cout << "bit1" << (inputMask & (1 << 0)) << " "
	//	<< "bit2" << (inputMask & (1 << 1)) << " "
	//	<< "bit3" << (inputMask & (1 << 2)) << " "
	//	<< "bit4" << (inputMask & (1 << 3)) << std::endl;

	mDesiredForwardAmount = (inputMask & (1 << 0)) ? 1.f : 0.f;
	mDesiredBackAmount = (inputMask & (1 << 1)) ? 1.f : 0.f;
	mDesiredLeftAmount = (inputMask & (1 << 2)) ? 1.f : 0.f;
	mDesiredRightAmount = (inputMask & (1 << 3)) ? 1.f : 0.f;

	//ReadSignedBinaryValue(inInputStream, mDesiredRightAmount);
	//ReadSignedBinaryValue(inInputStream, mDesiredLeftAmount);
	//ReadSignedBinaryValue(inInputStream, mDesiredForwardAmount);
	//ReadSignedBinaryValue(inInputStream, mDesiredBackAmount);

	inInputStream.Read(mIsShooting);

	uint8_t quantized;
	inInputStream.Read(quantized);
	mDesiredRotation = (quantized / 255.f) * 360.f;

	return true;
}