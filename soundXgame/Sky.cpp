#include <GL\glew.h>
#include "Sky.h"


Sky::Sky(void) : r(0), g(0), b(0)
{
}


Sky::~Sky(void)
{
}

void Sky::Draw(void)
{
	Update();

	glClearColor(r/255,g/255,b/255,1.);
}

void Sky::Update(void)
{
	float add = 0.3f;

	if(r < 255 && g == 0)
	{
		r += add;
		if(r > 255)
			r = 255;
	}
	else 
	{
		if(g < 255 && r == 255 && b == 0)
		{
			g += add;
			if(g > 255)
				g = 255;
		}
		else 
		{
			if (r > 0 && g == 255)
			{
				r -= add;
				if(r < 0)
					r = 0;
			}
			else 
			{
				if (b < 255 && r == 0)
				{
					b += add;
					if(b > 255)
						b = 255;
				}
				else 
				{
					if (g > 0)
					{
						g -= add;
						if(g < 0)
							g = 0;
					}
					else 
					{
						
						if (b > 0)
						{
							b -= add;
							if(b < 0)
								b = 0;
						}
						else
						{
							// end -> starting from scratch 
						}
					}
				} 
			} 
		}
	}
}
