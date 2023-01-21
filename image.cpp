#include "image.h"
#include <iostream>
#include <fstream>

BmpColor::BmpColor(uint8_t color) : color(color) {}

BmpImage::BmpImage(int width, int height)
    : width(width), length(height), colors(std::vector<BmpColor>(width * height)) {}

BmpColor BmpImage::GetColor(int x, int y) const {
  return colors[y * width + x];
}

void BmpImage::SetColor(const BmpColor& color, int x, int y) {
  colors[y * width + x].color = color.color;
}

void BmpImage::Export(const char* path) const {
  std::ofstream f;
  f.open(path, std::ios::out | std::ios::binary);

  if (!f.is_open()) {
    std::cout << "file cant be open\n";
    return;
  }

  unsigned char bmpPad[3] = {0, 0, 0};
  const int kPaddingAmount = ((4 - ((width + 1) / 2) % 4) % 4);
  const int kFileHeaderSize = 14;
  const int kInformationHeaderSize = 40;
  const int kColorsAmount = 5;
  const uint32_t kFileSize =
      kFileHeaderSize + kInformationHeaderSize + (width + 1) / 2 * length + kPaddingAmount * length + kColorsAmount * 4;
  const int kPlanes = 1;
  const int kBitPerPixel = 4;

  unsigned char fileHeader[kFileHeaderSize]{};
  ///FileType
  fileHeader[0] = 'B';
  fileHeader[1] = 'M';
  ///FileSize
  fileHeader[2] = kFileSize;
  fileHeader[3] = kFileSize >> 8;
  fileHeader[4] = kFileSize >> 16;
  fileHeader[5] = kFileSize >> 24;
  ///PixelDataOffset
  fileHeader[10] = kFileHeaderSize + kInformationHeaderSize + kColorsAmount * 4;

  unsigned char informationHeader[kInformationHeaderSize]{};
  informationHeader[0] = kInformationHeaderSize;
  ///Image Width
  informationHeader[4] = width;
  informationHeader[5] = width >> 8;
  informationHeader[6] = width >> 16;
  informationHeader[7] = width >> 24;
  ///Image Height
  informationHeader[8] = length;
  informationHeader[9] = length >> 8;
  informationHeader[10] = length >> 16;
  informationHeader[11] = length >> 24;

  informationHeader[12] = kPlanes;
  informationHeader[14] = kBitPerPixel;
  informationHeader[32] = kColorsAmount;

  ///my colors and 0 after each color
  unsigned char colorPallet[] = {255, 255, 255, 0, 120, 214, 0, 0, 200, 0,
                                 119, 0, 0, 255, 223, 0, 0, 0, 0, 0};

  f.write(reinterpret_cast<char*>(fileHeader), kFileHeaderSize);
  f.write(reinterpret_cast<char*>(informationHeader), kInformationHeaderSize);
  f.write(reinterpret_cast<char*>(colorPallet), kColorsAmount * 4);

  for (int y = 0; y < length; y++) {
    unsigned char two_colors = 0;
    for (int x = 0; x < width; x++) {
      unsigned char one_color = static_cast<unsigned char>(GetColor(x, y).color);
      if (x % 2 == 0) two_colors = one_color << 4;
      else {
        two_colors += one_color;
        f.write((const char*) &two_colors, 1);
        two_colors = 0;
      }
    }
    if (width % 2 == 1) {
      f.write((const char*) &two_colors, 1);
    }
    f.write(reinterpret_cast<char*>(bmpPad), kPaddingAmount);
  }
  f.close();
  std::cout << "BMP " << path << " created\n";
}

void BmpPrinter(uint16_t y_min,
                uint16_t x_min,
                uint16_t y_max,
                uint16_t x_max,
                uint64_t** sand_pile,
                const std::string& directory_path) {
  BmpColor kWhite = BmpColor(0);
  BmpColor kGreen = BmpColor(1);
  BmpColor kPurple = BmpColor(2);
  BmpColor kGold = BmpColor(3);
  BmpColor kBlack = BmpColor(4);
  BmpImage image(x_max - x_min + 1, y_max - y_min + 1);
  for (int y = y_min; y <= y_max; y++) {
    for (int x = x_min; x <= x_max; x++) {
      if (sand_pile[y][x] == 0) image.SetColor(kWhite, x - x_min, y - y_min);
      else if (sand_pile[y][x] == 1) image.SetColor(kGreen, x - x_min, y - y_min);
      else if (sand_pile[y][x] == 2) image.SetColor(kPurple, x - x_min, y - y_min);
      else if (sand_pile[y][x] == 3) image.SetColor(kGold, x - x_min, y - y_min);
      else image.SetColor(kBlack, x - x_min, y - y_min);
    }
  }
  image.Export(directory_path.c_str());
}

