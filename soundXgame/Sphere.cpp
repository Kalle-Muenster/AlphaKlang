#include "Sphere.h"


//Sphere::Sphere(void)
//{
//	
//}

Sphere::Sphere(bool drawBackFaces)
{
	NoBackfaceCulling = drawBackFaces;
	InitializeObject("sphere_quads_high.obi");
}

Sphere::~Sphere(void)
{
}
