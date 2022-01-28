#include "tgaimage.h"
#include "viki_renderer/Model.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

Model *model = nullptr;
const int width = 800;
const int height = 800;

/// <summary>
/// In this mthd we use a linear weight interpolations with range 0 - 1 with steps of 0.1.but this 
/// causes breaks and all///
/// But the pixels or steps is known. Which is the number of pixels right ? Think about it
/// the number input is nothing but x,y so if we just move from 1 to next as they are pixels 
/// that;s all the  steps.
/// </summary>
/// <param name="x0"></param>
/// <param name="y0"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="image"></param>
/// <param name="color"></param>
void line1(int x0 , int y0 , int x1 , int y1 , TGAImage &image , TGAColor color)
{
	for (float t=0;t<=1;t+=0.01)
	{
	//	std::cout << t<<"\n";
		int lineX = x0 + (x1 - x0) * t;
		int lineY = y0 +  (y1 - y0)*t;
		std::cout << lineX <<"," << lineY << "\n";
		image.set(lineX,lineY,color);
	}
}

void line2(int x0 ,int y0 , int x1 , int y1 , TGAImage &image , TGAColor color)
{
	float t = 0;
	for (int x = x0 ;x <= x1;x++)
	{
		t = (x-x0) / (float)(x1 - x0);
		int lineY = y0 + (y1 - y0) * t;
		image.set(x,lineY,color);
	}
}

void line3(int x0 , int y0 , int x1 , int y1 , TGAImage &image , TGAColor color)
{
	//Check if which to use as steep X or Y by finding which changes really fast.
	//Reason we do this is because if x move fast (low lenght) but y moves slow (more lenght) we 
	//are bound to X to control Y and we now have fewer points. To overcome this we 
	// a simple swap and this fixes it !
	bool stepWithX = true;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		stepWithX = false;
		std::swap(x0, y0);
		std::swap(x1 , y1);
	}

	//Check if Xo is greater than x1 then we swap so it always draws from lower to higher !
	//And since X will be proper by now we can just check that !
	if (x0>x1)
	{
		std::swap(x0, x1);
		std::swap(y0 , y1);
	}
	float t = 0;
	for (int x = x0; x <= x1; x++)
	{
		t = (x - x0) / (float)(x1 - x0);
		int lineY = y0 + (y1 - y0) * t;

		if(stepWithX)
			image.set(x, lineY, color);
		else
		image.set(lineY,x,  color);

	}
}



int main(int argc, char** argv) {
//	TGAImage image(100, 100, TGAImage::RGB);
	 TGAImage image(width, height, TGAImage::RGB);
	/*
	//image.set(52, 41, red);
	//line2(13, 20, 80, 40, image, white);
	//line2(20, 13, 40, 80, image, red);
	for (int i = 0; i < 1000000; i++)
	{

		line3(13, 20, 80, 40, image, white);
		line3(20, 13, 40, 80, image, red);
		line3(80, 40, 13, 20, image, red);
	}
	
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	*/
	
	
//Model loading
	model = new Model("obj/model.obj");

	
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.) * width / 2.;
			int y0 = (v0.y + 1.) * height / 2.;
			int x1 = (v1.x + 1.) * width / 2.;
			int y1 = (v1.y + 1.) * height / 2.;
			line3(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output_Model.tga");
	delete model;

	return 0;
}

