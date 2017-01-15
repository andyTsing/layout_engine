#ifndef LE_CONV_UTIL_H_
#define LE_CONV_UTIL_H_

namespace le {

namespace conv_util {

int Twip2Pixel(int twip) {
  return twip / 15;
}

int PixelToTwip(int pixel) {
  return pixel * 15;
}

}

}  // le

#endif /* LE_CONV_UTIL_H_ */
