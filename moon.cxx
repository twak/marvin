#include "moon.h"


/**
From: http://users.ox.ac.uk/~orie1381/code.html#bmp
*/

int moon::LoadBitmap(char * szFileName, GLuint texid, int width, int height)
// Creates Texture From A Bitmap File
{
unsigned char tex[texWidth*texHeight*3 + 18*3]; //array to hold data input
unsigned char tex2[texWidth*texHeight*3]; //array to hold texture data
unsigned char temp; //swap variable
int num; //loop variable
FILE * f; //open the bitmap for reading

if ((f=fopen(szFileName,"r"))== NULL)
  {
    printf("its all broken");
    return 1;
  }



fread(tex,width*height*3 + 18*3,1,f); //read data from bitmap

fclose(f); //close the file

for (num = 18*3-1; num < width*height*3 +18*3; num = num + 1)
{
//remove the first 18 pixels, RGB code = 3 bytes
tex2[num - 18*3] = tex[num];
}



for (num = 0; num < width*height*3; num = num + 3)
{
//bitmap backwards so change BGR code to RGB code
//by swaping the every third bit with evert first.
temp = tex2[num];
tex2[num] = tex2[num+2];
tex2[num +2] = temp;
}


glBindTexture(GL_TEXTURE_2D, texid); //select ID to store the texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0,3, width ,height, 0,GL_RGB, GL_UNSIGNED_BYTE, tex2);
//produce the texture
return 0;
}
