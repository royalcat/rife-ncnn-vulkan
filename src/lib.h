#include <vector>

#include "benchmark.h"
#include "cpu.h"
#include "filesystem_utils.h"
#include "gpu.h"
#include "platform.h"
#include "rife.h"

#if _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default"))) __attribute__((used))
#endif

extern "C" {
struct RifeParameters {
  const char* model;

  const int* gpuids;
  const int gpucount;

  const int* job_proc;
  const int job_proc_size;

  const int verbose;
  const int tta_mode;
  const int uhd_mode;
};

EXPORT static std::vector<RIFE*> rife;

EXPORT extern int init(RifeParameters params);

EXPORT extern int process(const unsigned char* pixels0,
                          const unsigned char* pixels1,
                          unsigned char* outpixels, int width, int height,
                          int pixels_type, float timestep, int gpuindex);
}