/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

//
//  RoboMath.cpp
//  RoboCat
//
//  Created by Joshua Glazer on 6/13/15.
//  Copyright (c) 2015 com.JoshuaGlazer.Book. All rights reserved.
//

#include "RoboCatPCH.hpp"

#include <random>

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);
std::mt19937 sRandomEngine{ std::random_device{}() };


float RoboMath::GetRandomFloat()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution< float > dis(0.f, 1.f);
	return dis(gen);
}

int RoboMath::GetRandomInt(int inMin, int inMax)
{
	std::uniform_int_distribution<int> dist(inMin, inMax);
	return dist(sRandomEngine);
}

Vector3 RoboMath::GetRandomVector(const Vector3& inMin, const Vector3& inMax)
{
	Vector3 r = Vector3(GetRandomFloat(), GetRandomFloat(), GetRandomFloat());
	return inMin + (inMax - inMin) * r;
}
