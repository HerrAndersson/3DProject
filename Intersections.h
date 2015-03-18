#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H
#include "Math.h"

#include "Object.h"
#include <iostream>
using namespace std;

struct Ray
{
	XMFLOAT3 origin;
	XMFLOAT3 direction;

	Ray(XMFLOAT3 origin, XMFLOAT3 direction)
	{
		this->origin = origin;
		this->direction = direction;
	}
};

struct Sphere
{
	XMFLOAT3 center;
	float radius;

	Sphere(XMFLOAT3 center, float radius)
	{
		this->center = center;
		this->radius = radius;
	}
};

struct Triangle
{
	XMFLOAT3 p0, p1, p2;

	Triangle(XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2)
	{
		this->p0 = p0;
		this->p1 = p1;
		this->p2 = p2;
	}
};

static bool RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
{
	bool hit = false;

	// Calculate the a, b, and c coefficients.
	float a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	float b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	float c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// Find the discriminant.
	float discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant > 0.0f)
	{
		hit = true;
		double hitPoint;

		double t0 = b*(-1) - sqrt(discriminant);
		double t1 = b*(-1) + sqrt(discriminant);

		if (t0 < t1)
			hitPoint = t0;
		else
			hitPoint = t1;

		cout << "HitPoint " << hitPoint << endl;
	}

	return hit;
}


static bool RayVsSphere2(Ray& ray, Sphere& sphere)
{
	bool hit = false;
	double hitPoint;

	double r = sphere.radius;
	XMFLOAT3 c = sphere.center;
	XMFLOAT3 o = ray.origin;
	XMFLOAT3 d = ray.direction;

	XMFLOAT3 a(o.x - c.x, o.y - c.y, o.z - c.z);

	double b = (d.x * a.x) + (d.y * a.y) + (d.z * a.z);
	double cf = (a.x * a.x) + (a.y * a.y) + (a.z * a.z);

	cout << b*b - cf << endl;

	if (b*b - cf > 0.0)
	{
		double t0 = b*(-1) - sqrt(b*b - cf);
		double t1 = b*(-1) + sqrt(b*b - cf);

		if (t0 < t1)
		{
			hitPoint = t0;
		}
		else
		{
			hitPoint = t1;
		}

		hit = true;
	}

	return hit;
}


static bool RayVsSphere(Ray& ray, Sphere& sphere)
{
	bool hit = false;

	XMFLOAT3 v = Subtract3(ray.origin, sphere.center);

	float b = DotProduct3(ray.direction, v);
	float c = DotProduct3(v, v) - sphere.radius*sphere.radius;

	float discriminant = b*b - c;

	//Discriminant has to be > 0 to avoid complex numbers
	cout << discriminant << endl;
	if (discriminant > 0.0f)
	{
		//Calculate both points of intersection.
		float returnValue = -1.0f;
		float t1 = -b + sqrt(discriminant);
		float t2 = -b - sqrt(discriminant);

		if (t1 < t2 && t1 > 0)
			returnValue = t1;
		else
			returnValue = t2;

		hit = true;
	}

	return hit;
}

static bool RayVsTriangle(Ray& ray, Triangle& triangle)
{
	float ALMOST_INFINITY = 10000000000000000.0f;
	float EXTREMELY_SMALL = 0.00000000000000001f;

	bool hit = false;

	XMFLOAT3 e1 = Subtract3(triangle.p1, triangle.p0);
	XMFLOAT3 e2 = Subtract3(triangle.p2, triangle.p0);

	XMFLOAT3 m = CrossProduct3(ray.direction, e2);

	//e1*(d x e2) = determinant
	float det = DotProduct3(e1, m);

	if (det < -EXTREMELY_SMALL || det > EXTREMELY_SMALL)
	{
		float f = 1 / det;

		XMFLOAT3 s = Subtract3(ray.origin, triangle.p0);

		// 1/det * det(s,d,e2)
		float u = f * DotProduct3(s, m);

		if (u > 0.0f)
		{
			XMFLOAT3 n = CrossProduct3(s, e1);

			// 1/det * det(s,d,e1)
			float v = f * DotProduct3(ray.direction, n);

			if (v > 0.0f && u + v < 1.0f)
			{
				// 1/det * det(d,e1,e2)
				float w = f * DotProduct3(e2, n);

				//Intersects at (u,v,w)
				hit = true;
			}
		}
	}

	return hit;
}

#endif
