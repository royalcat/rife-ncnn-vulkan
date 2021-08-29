#include "lib.h"

#include <vector>

#include "benchmark.h"
#include "cpu.h"
#include "filesystem_utils.h"
#include "gpu.h"
#include "platform.h"
#include "rife.h"

EXPORT int init(RifeParameters params) {
    std::string model(params.model);
    bool rife_v2 = false;
    if (model.find(PATHSTR("rife-v2")) != path_t::npos) {
        // fine
        rife_v2 = true;
    } else if (model.find(PATHSTR("rife-v3")) != path_t::npos) {
        // fine
        rife_v2 = true;
    } else if (model.find(PATHSTR("rife")) != path_t::npos) {
        // fine
    } else {
        fprintf(stderr, "unknown model dir type\n");
        return -1;
    }
    path_t modeldir = sanitize_dirpath(model);

#if _WIN32
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif
    ncnn::create_gpu_instance();

    std::vector<int> gpuid;
    if (params.gpucount > 0) {
        gpuid = std::vector<int>(params.gpuids, params.gpuids + params.gpucount);
    } else {
        gpuid.push_back(ncnn::get_default_gpu_index());
    }

    const int use_gpu_count = (int)gpuid.size();

    std::vector<int> jobs_proc;
    if (params.gpucount > 0) {
        jobs_proc = std::vector<int>(params.job_proc, params.job_proc + params.job_proc_size);
    } else {
        jobs_proc.resize(use_gpu_count, 2);
    }

    rife = std::vector<RIFE*>(use_gpu_count);
    for (int i = 0; i < use_gpu_count; i++) {
        int num_threads = gpuid[i] == -1 ? jobs_proc[i] : 1;

        rife[i] = new RIFE(gpuid[i], params.tta_mode, params.uhd_mode, num_threads, rife_v2);

        rife[i]->load(modeldir);
    }

    return rife.size();
}

EXPORT int process(const unsigned char* pixels0,
                   const unsigned char* pixels1,
                   unsigned char* outpixels,
                   int width, int height,
                   int pixels_type,
                   float timestep,
                   int gpuindex) {
    if (gpuindex > rife.size() - 1) {
        return -1;
    }

    return rife[gpuindex]->process(pixels0, pixels1, width, height, timestep, outpixels);
}