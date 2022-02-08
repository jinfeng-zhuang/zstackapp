#include <zstack/zstack.h>

#include "main.h"
#include <math.h>

struct application app;

int _sum(int* input, int x, int y) {
  int i, j;

  int sum = 0;


  for (j = 0; j < 4; j++) {
    for (i = 0; i < 4; i++) {
      sum += input[j * 4 + i] * cos(x * PI * (i + 0.5) / 4) * cos(y * PI * (j + 0.5) / 4);
    }
  }

  return sum;
}

void DCT_4x4(int* input)
{
  int i, j;
  int sum;
  float scalei, scalej;

  for (j = 0; j < 4; j++) {
    for (i = 0; i < 4; i++) {
      if (i == 0)
        scalei = 1.0 / 4;
      else
        scalei = 1.0 / 2;

      if (j == 0)
        scalej = 1 / 4;
      else
        scalej = 1 / 2;

      sum = _sum(input, i, j) * scalei;

      printf("%-5d ", sum);
    }

    printf("\n");
  }
}

int demo[] = {
  -115, -115, -114, -114, -113, -113, -111, -110,
  -110, -109, -105, -103, -109, -107, -101, -99
};

int main(int argc, char *argv[]) {
  if (param_parser(argc, argv, &app) == -1) {
    print_usage();
    return -1;
  }

  log_init(app.param.log_config);

  DCT_4x4(demo);

  return 0;
}
