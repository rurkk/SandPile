#include <cinttypes>
#include <iostream>
#include <vector>

struct BmpColor {
  uint8_t color;
  BmpColor(uint8_t color = 0);
};

class BmpImage {
 public:
  BmpImage(int width, int height);

  BmpColor GetColor(int x, int y) const;
  void SetColor(const BmpColor& color, int x, int y);

  void Export(const char* path) const;
 private:
  int width;
  int length;
  std::vector<BmpColor> colors;
};

void BmpPrinter(uint16_t y_min,
                uint16_t x_min,
                uint16_t y_max,
                uint16_t x_max,
                uint64_t** sand_pile,
                const std::string& directory_path);