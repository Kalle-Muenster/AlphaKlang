#include "ScreenOverlay.h"
#include "Utility.h"
#include "IGObject.h"

ScreenOverlay::ScreenOverlay(void)
{
	InitializeObject();
	
}

void
ScreenOverlay::Initialize(char* fileName)
{
	transform.scale = Vector3(1,1,1);
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
		sscanf(&fName[i],"_%ix%i.",&texture.w,&texture.h);
	fName[i]='\0';
	SetName(&fName[0]);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, texture.w);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);	   // instead of GL_BLEND these values can be used:  GL_MODULATE , GL_DECAL , GL_REPLACE 
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //mipmaps, clamping...

	//entweder so
	//with The Loader!
	//Loader* loader = new Loader(filename);   //...A
	//this->texture.w = loader->width();
	//this->texture.h = loader->height();
	//this->texture.pData = loader->data;

	//oder so...
	//with glm imgload...
	texture.Loade(filename,texture.w,texture.h,Texture::Format::RGBA);  //...B


	
	// create a RGBA color texture
	glGenTextures(1, &texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
//	glTexImage2D(GL_TEXTURE_2D, 0, texture.format, loader->width(), loader->height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texture.pData);	// A !
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, texture.w, texture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL); // B !

	if(texture.w > texture.h)
		scale(transform.scale.x,transform.scale.y*((float)texture.h/texture.w),transform.scale.z);
	else if(texture.w < texture.h)
		scale(transform.scale.x*((float)texture.w/texture.h),transform.scale.y,transform.scale.z);
}

Vector3
ScreenOverlay::scale(float x, float y, float z)
{
	transform.scale.x = x;
	transform.scale.y = y;
	transform.scale.z = z;
	printf("overlay: scale: %f,%f,%f\n",this->getTransform()->scale.x,this->getTransform()->scale.y,this->getTransform()->scale.z);
	return transform.scale;
}

void 
ScreenOverlay::draw(void)
{

	glEnable(GL_COLOR);
//	glMatrixMode(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	glDrawBuffer(GL_BACK);
//	glBindBuffer(GL_ARRAY_BUFFER, texture.ID);
//	glPixelTransferf(GL_MAP_COLOR,GL_PIXEL_MAP_S_TO_S);

//	glutGet(GL_CURRENT_RASTER_INDEX);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);  /*GL_TEXTURE_MIN_FILTER */
																/*GL_TEXTURE_MAG_FILTER*/
																/*GL_TEXTURE_WRAP_S*/
																/*GL_TEXTURE_WRAP_T*/
//	glPixelZoom(scale.x,scale.y);  
	glPushMatrix();
	//	glMatrixMode(GL_MODELVIEW);
	//	glMatrixMode(GL_PROJECTION);
		
	 // glLoadIdentity();

	GLfloat vport[4];
	glGetFloatv(GL_VIEWPORT,&vport[0]);
	printf("viewport: %f,%f,%f,%f\n",vport[0],vport[1],vport[2],vport[3]);
	glWindowPos2f(vport[0],vport[1]);
	glGetFloatv(GL_CURRENT_RASTER_POSITION,&vport[0]);
	printf("rasterpositions: %f,%f,%f,%f\n",vport[0],vport[1],vport[2],vport[3]);
	glBegin(GL_POINTS);
	
	//Sets the Curent Drawing-Position:
	glRasterPos2f(0,0);
	glDrawPixels(texture.w,texture.h, GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,texture.pData);
	glEnd();
//	 glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_COLOR);
	//	glMatrixMode(GL_PROJECTION);
}