#pragma once

class Collider
{
public:
	Collider() {}
	virtual ~Collider() {}
	virtual void update() = 0;
};