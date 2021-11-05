#include <Windows.h>
#include <cpfspd/cpfspd.h>
#include <stdio.h>
#include <v-silicon/v-silicon.h>
#include <zstack/zstack.h>

int main(int argc, char *argv) {
  int c;
  pT_status status;
  pT_header header;

  status = p_read_header("d:\\1.yuv", &header);
  printf("status = %d\n", status);

  p_print_header(&header, stdout);

  return 0;
}
