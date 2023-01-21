#include "image.h"
#include "parser.h"

void Scatter(uint64_t** sand_pile, uint16_t i, uint16_t j) {
  sand_pile[i][j] -= 4;
  sand_pile[i - 1][j] += 1;
  sand_pile[i + 1][j] += 1;
  sand_pile[i][j - 1] += 1;
  sand_pile[i][j + 1] += 1;
}

void Overwriting(uint64_t** sand_pile,
                 uint64_t** new_sand_pile,
                 uint16_t i_add,
                 uint16_t j_add,
                 uint16_t y_max,
                 uint16_t y_min,
                 uint16_t x_max,
                 uint16_t x_min) {
  for (int i = y_min; i <= y_max; i++) {
    for (int j = x_min; j <= x_max; j++) {
      new_sand_pile[i + i_add][j + j_add] = sand_pile[i][j];
    }
  }
}

int main(int argc, const char** argv) {
  Arguments arg = FillArguments(argc, argv);

  ///задаю крайние значения матрицы в которой находится песок
  uint16_t y_max = arg.length - 1;
  uint16_t x_max = arg.width - 1;
  uint16_t y_min = 0;
  uint16_t x_min = 0;

  ///размер полной матрицы
  uint16_t len;
  len = arg.length;
  uint16_t wid;
  wid = arg.width;

  uint64_t cnt_iteration;
  cnt_iteration = 0;

  auto** sand_pile = new uint64_t* [len];
  for (int i = 0; i < len; i++) sand_pile[i] = new uint64_t[wid]();
  auto** sand_pile2 =new uint64_t* [len];
  for (int i = 0; i < len; i++) sand_pile2[i] = new uint64_t[wid]();

  FileReader(sand_pile, sand_pile2, arg.input);

  bool change_flag = true;
  while (change_flag) {
    change_flag = false;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    uint16_t new_len;
    new_len = len;
    uint16_t new_wid;
    new_wid = wid;

    if (x_min == 0) left = true;
    if (x_max == wid - 1) right = true;
    if (y_min == 0) up = true;
    if (y_max == len - 1) down = true;

    if (up and down) new_len *= 3;
    else if (up or down) new_len *= 2;
    if (left and right) new_wid *= 3;
    else if (left or right) new_wid *= 2;

    if (up or down or left or right) {
      auto** new_sand_pile = new uint64_t* [new_len];
      for (int i = 0; i < new_len; i++) new_sand_pile[i] = new uint64_t[new_wid]();
      auto** new_sand_pile2 = new uint64_t* [new_len];
      for (int i = 0; i < new_len; i++) new_sand_pile2[i] = new uint64_t[new_wid]();

      if (up and left) {
        Overwriting(sand_pile, new_sand_pile, len, wid, y_max, y_min, x_max, x_min);
        Overwriting(sand_pile, new_sand_pile2, len, wid, y_max, y_min, x_max, x_min);
      } else if (up) {
        Overwriting(sand_pile, new_sand_pile, len, 0, y_max, y_min, x_max, x_min);
        Overwriting(sand_pile, new_sand_pile2, len, 0, y_max, y_min, x_max, x_min);
      } else if (left) {
        Overwriting(sand_pile, new_sand_pile, 0, wid, y_max, y_min, x_max, x_min);
        Overwriting(sand_pile, new_sand_pile2, 0, wid, y_max, y_min, x_max, x_min);
      } else {
        Overwriting(sand_pile, new_sand_pile, 0, 0, y_max, y_min, x_max, x_min);
        Overwriting(sand_pile, new_sand_pile2, 0, 0, y_max, y_min, x_max, x_min);
      }
      if (up) {
        y_min += len;
        y_max += len;
      }
      if (left) {
        x_min += wid;
        x_max += wid;
      }

      for (int i = 0; i < len; i++) {
        for (int j = 0; j < wid; j++) {
          if (sand_pile[i][j] >= 4) {
            change_flag = true;
            if (up and left) {
              Scatter(new_sand_pile, i + len, j + wid);
              Scatter(new_sand_pile2, i + len, j + wid);
            } else if (up) {
              Scatter(new_sand_pile, i + len, j);
              Scatter(new_sand_pile2, i + len, j);
            } else if (left) {
              Scatter(new_sand_pile, i, j + wid);
              Scatter(new_sand_pile2, i, j + wid);
            } else {
              Scatter(new_sand_pile, i, j);
              Scatter(new_sand_pile2, i, j);
            }
          }
        }
      }
      for (int i = 0; i < len; i++) delete[] sand_pile[i];
      delete[] sand_pile;
      sand_pile = new_sand_pile;
      for (int i = 0; i < len; i++) delete[] sand_pile2[i];
      delete[] sand_pile2;
      sand_pile2 = new_sand_pile2;
    } else {
      for (int i = y_min; i <= y_max; i++) {
        for (int j = x_min; j <= x_max; j++) {
          if (sand_pile2[i][j] >= 4) {
            change_flag = true;
            Scatter(sand_pile, i, j);
            if (i <= y_min) up = true;
            if (i >= y_max) down = true;
            if (j >= x_max) right = true;
            if (j <= x_min) left = true;
          }
        }
      }
      for (int i = y_min; i <= y_max; i++) {
        for (int j = x_min; j <= x_max; j++) {
          sand_pile2[i][j] = sand_pile[i][j];
        }
      }
      if (up) y_min--;
      if (down) y_max++;
      if (left) x_min--;
      if (right) x_max++;
    }
    len = new_len;
    wid = new_wid;

    cnt_iteration++;
    if (cnt_iteration >= arg.max_iter or !change_flag) {
      BmpPrinter(y_min, x_min, y_max, x_max, sand_pile, arg.output + "\\lastimage.bmp");
    } else if (arg.freq != 0) {
      if (cnt_iteration % arg.freq == 0) {
        BmpPrinter(y_min,
                   x_min,
                   y_max,
                   x_max,
                   sand_pile,
                   arg.output + "\\image" + std::to_string(cnt_iteration / arg.freq) + ".bmp");
      }
    }
  }
  for (int i = 0; i < len; i++) delete[] sand_pile[i];
  delete[] sand_pile;
  for (int i = 0; i < len; i++) delete[] sand_pile2[i];
  delete[] sand_pile2;

  return 0;
}