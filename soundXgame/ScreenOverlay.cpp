#include "ScreenOverlay.h"
#include "Utility.h"

ScreenOverlay::ScreenOverlay(void)
{
	InitializeObject();

}

void
ScreenOverlay::Initialize(char* fileName)
{
	LoadeImage(fileName);
}

ScreenOverlay::~ScreenOverlay(void)
{

}

void
ScreenOverlay::LoadeImage(char* filename)
{
//	texture.format = GL_RGBA8;
	char fName[64];
	strcpy(&fName[0],filename);
	int i = -1;
	while((++i<64)&&(fName[i]!='_'));
	if(i<64)
	sscanf(&fName[i],"%ix%i.%c%c%c",&texture.w,&texture.h);
	
	
	glPixelStorei(GL_UNPACK_ROW_LENGTH, texture.w);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //mipmaps, clamping...

	//entweder so
	//with The Loader!
	Loader* loader = new Loader(filename);   //...A
	this->texture.w = loader->width();
	this->texture.h = loader->height();
	this->texture.pData = loader->data;
	//oder so...
	//with glm imgload...
	texture.Loade(filename,texture.w,texture.h,Texture::Format::RGBA);  //...B


	
	// create a RGBA color texture
	glGenTextures(1, &texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	glTexImage2D(GL_TEXTURE_2D, 0, texture.format, loader->width(), loader->height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texture.pData);	// A !
	glTexImage2D(GL_TEXTURE_2D, 0,texture.format, texture.w, texture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL); // B !
  //

	delete loader;
}

void 
ScreenOverlay::draw(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	glDrawBuffer(GL_BACK);
	//glBindBuffer(GL_ARRAY_BUFFER, texture.ID);
//	glPixelTransferf(GL_MAP_COLOR,GL_PIXEL_MAP_S_TO_S);

	glutGet(GL_CURRENT_RASTER_INDEX);
	  // instead of GL_BLEND these values can be used:  GL_MODULATE , GL_DECAL , GL_REPLACE 

//	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);  /*GL_TEXTURE_MIN_FILTER */
																/*GL_TEXTURE_MAG_FILTER*/
																/*GL_TEXTURE_WRAP_S*/
																/*GL_TEXTURE_WRAP_T*/
//	glPixelZoom(scale.x,scale.y);  

	glBegin(GL_POINTS);
	
	//Sets the Curent Drawing-Position:
	glRasterPos2f(0,0);
	glDrawPixels(texture.w,texture.h, GL_RGBA, GL_UNSIGNED_BYTE,texture.pData);
	glEnd();
	
}