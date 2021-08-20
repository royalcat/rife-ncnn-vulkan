
#define EXPORT extern "C" __attribute__((visibility("default"))) __attribute__((used))

extern "C" struct RifeParameters
{
    const char* model;

    const int* gpuids;
    const int gpucount;

    const int* job_proc;
    const int job_proc_size;

    const bool verbose;
    const bool tta_mode;
    const bool uhd_mode;
};

EXPORT int init(RifeParameters params);

EXPORT int process(const unsigned char* pixels0,
                   const unsigned char* pixels1,
                   unsigned char* outpixels,
                   int width, int height,
                   int pixels_type,
                   float timestep,
                   int gpuindex);
