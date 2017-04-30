#include "bitmap.h"

#include <cstring>

namespace le {

namespace {

void WriteBitmap(const std::string& filename, int w, int h, const unsigned char* img) {
  
  FILE *f;
  int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int

  unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
  unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
  unsigned char bmppad[3] = {0,0,0};

  bmpfileheader[ 2] = (unsigned char)(filesize    );
  bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
  bmpfileheader[ 4] = (unsigned char)(filesize>>16);
  bmpfileheader[ 5] = (unsigned char)(filesize>>24);

  bmpinfoheader[ 4] = (unsigned char)(       w    );
  bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
  bmpinfoheader[ 6] = (unsigned char)(       w>>16);
  bmpinfoheader[ 7] = (unsigned char)(       w>>24);
  bmpinfoheader[ 8] = (unsigned char)(       h    );
  bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
  bmpinfoheader[10] = (unsigned char)(       h>>16);
  bmpinfoheader[11] = (unsigned char)(       h>>24);

  f = fopen(filename.c_str(),"wb");
  fwrite(bmpfileheader,1,14,f);
  fwrite(bmpinfoheader,1,40,f);
  for(int i=0; i<h; i++)
  {
    fwrite(img+(w*(h-i-1)*3),3,w,f);
    fwrite(bmppad,1,(4-(w*3)%4)%4,f);
  }
  fclose(f);
}

}


Bitmap::Bitmap(int width, int height, int depth)
    : width_(width),
      height_(height),
      depth_(depth)
{
  data_ = new unsigned char[width * height * depth];
  std::memset(data_, 0xFF, width*height*depth);
}

Bitmap::~Bitmap()
{

}

void Bitmap::SetPixel(int x, int y, Color color)
{
  if( x > -1 && y > -1 && x < GetWidth() && y < GetHeight() ) {
    auto a = GetA(color);
    auto r = GetR(color);
    auto g = GetG(color);
    auto b = GetB(color);

    auto origin_b = data_[(y * width_ + x) * depth_ + 0];
    auto origin_g = data_[(y * width_ + x) * depth_ + 1];
    auto origin_r = data_[(y * width_ + x) * depth_ + 2];

    r = r * a / 255 + origin_r * (255 - a) / 255;
    g = g * a / 255 + origin_g * (255 - a) / 255;
    b = b * a / 255 + origin_b * (255 - a) / 255;
    
    data_[(y * width_ + x) * depth_ + 0] = b;
    data_[(y * width_ + x) * depth_ + 1] = g;
    data_[(y * width_ + x) * depth_ + 2] = r;
    // std::memcpy(&data_[ (y * width_ + x) * depth_ ],&color + 1, sizeof(color) - 1);
  }
}

void Bitmap::WriteBitmapFile(const std::string& filename)
{
  WriteBitmap( filename, width_, height_, data_ );
}


}  // le
