#include <cstdlib>
#include <cstdio>
#include <vector>

#include "tinyexr.h"

void
SaveAsPFM(const char* filename, int width, int height, float* data)
{
  FILE* fp = fopen(filename, "wb");
  if (!fp) {
    fprintf(stderr, "failed to write a PFM file.\n");
    return;
  }

  fprintf(fp, "PF\n");
  fprintf(fp, "%d %d\n", width, height);
  fprintf(fp, "-1\n"); // -1: little endian, 1: big endian

  // RGBA -> RGB
  std::vector<float> rgb(width*height*3);

  for (size_t i = 0; i < width * height; i++) {
    rgb[3*i+0] = data[4*i+0];
    rgb[3*i+1] = data[4*i+1];
    rgb[3*i+2] = data[4*i+2];
  }
  
  fwrite(&rgb.at(0), sizeof(float), width * height * 3, fp);

  fclose(fp);
}

int
main(int argc, char** argv)
{
#if 1
  const char* err;
  float* out;
  int width;
  int height;

  if (argc < 2) {
    fprintf(stderr, "Needs input.exr.\n");
    exit(-1);
  }
  
  int ret = LoadEXR(&out, &width, &height, argv[1], &err);
  if (ret != 0) {
    fprintf(stderr, "Load EXR err: %s\n", err);
    return ret;
  }

  printf("EXR: %d x %d\n", width, height);

  SaveAsPFM("output.pfm", width, height, out);

  printf("Saved pfm file.\n");

  free(out);
#else // @todo
  const char* err;
  float** out;
  int num_parts;
  int width;
  int height;

  if (argc < 2) {
    fprintf(stderr, "Needs input.exr.\n");
    exit(-1);
  }
  
  int ret = LoadMultiPartEXR(&out, &num_parts, &width, &height, argv[1], &err);
  if (ret != 0) {
    fprintf(stderr, "Load EXR err: %s\n", err);
    return ret;
  }

#endif

  return ret;
}
