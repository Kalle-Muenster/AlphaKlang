#include "Sphere.h"


Sphere::Sphere(string meshFileName,string textureFileName,bool drawBackFaces)
{
	NoBackfaceCulling = drawBackFaces;
	InitializeObject(meshFileName,true);
	LoadTexture(textureFileName);

}

Sphere::Sphere(bool drawBackFaces)
{
	NoBackfaceCulling = drawBackFaces;
	InitializeObject("sphere_quads_high.obi");
}

Sphere::~Sphere(void)
{
}
