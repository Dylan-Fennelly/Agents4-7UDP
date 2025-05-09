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
}

bool InputState::Write(OutputMemoryBitStream& inOutputStream) const
{
	WriteSignedBinaryValue(inOutputStream, mDesiredRightAmount);
	WriteSignedBinaryValue(inOutputStream, mDesiredLeftAmount);
	WriteSignedBinaryValue(inOutputStream, mDesiredForwardAmount);
	WriteSignedBinaryValue(inOutputStream, mDesiredBackAmount);
	inOutputStream.Write(mIsShooting);
	inOutputStream.Write(mDesiredRotation);
	return false;
}

bool InputState::Read(InputMemoryBitStream& inInputStream)
{
	ReadSignedBinaryValue(inInputStream, mDesiredRightAmount);
	ReadSignedBinaryValue(inInputStream, mDesiredLeftAmount);
	ReadSignedBinaryValue(inInputStream, mDesiredForwardAmount);
	ReadSignedBinaryValue(inInputStream, mDesiredBackAmount);
	inInputStream.Read(mIsShooting);
	inInputStream.Read(mDesiredRotation);
	return true;
}