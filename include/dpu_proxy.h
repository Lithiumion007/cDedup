#include <doca_argp.h>
#include <doca_error.h>
#include <doca_dev.h>
#include <doca_sha.h>
#include <doca_log.h>
#include <doca_buf.h>
#include <doca_buf_inventory.h>
#include <doca_ctx.h>
#include "common.h"
#include <stdio.h>
#include <pthread.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <stdlib.h>

#define SLEEP_IN_NANOS (10 * 1000) /* Sample the job every 10 microseconds  */

void free_cb(void *addr, size_t len, void *opaque)
{
    (void)len;
    (void)opaque;
    free(addr);
}

class DPU_PROXY{
public:
    void doca_SHA1(){
        doca_workq_submit(state.workq, &sha_job.base);

        while ((result = doca_workq_progress_retrieve(state.workq, &event, DOCA_WORKQ_RETRIEVE_FLAGS_NONE)) ==
            DOCA_ERROR_AGAIN) {
            /* Wait for the job to complete */
            ts.tv_sec = 0;
            ts.tv_nsec = SLEEP_IN_NANOS;
            nanosleep(&ts, &ts);
        }
    }

    doca_error_t doca_sha_init(const unsigned char* src_buf, size_t src_len, 
				  const unsigned char* dst_buf, size_t dst_len){
        result = doca_sha_create(&sha_ctx);
        if (result != DOCA_SUCCESS) {
            printf("Unable to create sha engine: %s", doca_get_error_string(result));
            return result;
        }
        state.ctx = doca_sha_as_ctx(sha_ctx);

        result = open_doca_device_with_capabilities(&job_sha_hardware_is_supported, &state.dev);
        if (result != DOCA_SUCCESS) {
            result = open_doca_device_with_capabilities(&job_sha_software_is_supported, &state.dev);
            if (result != DOCA_SUCCESS) {
                printf("Failed to find device for SHA job");
                result = doca_sha_destroy(sha_ctx);
                return result;
            }
            printf("SHA engine is not enabled, using openssl instead");
	    }

	    init_core_objects(&state, workq_depth, max_bufs);

	    // destination memory (sha result) init
	    doca_mmap_set_memrange(state.dst_mmap, (void*)dst_buf, dst_len);
	    doca_mmap_set_free_cb(state.dst_mmap, &free_cb, NULL);
	    doca_mmap_start(state.dst_mmap);
	    doca_mmap_set_permissions(state.dst_mmap, DOCA_ACCESS_LOCAL_READ_WRITE);

	    // source memory init
	    doca_mmap_set_memrange(state.src_mmap, (void*)src_buf, src_len);
	    doca_mmap_start(state.src_mmap);
	    doca_mmap_set_permissions(state.src_mmap, DOCA_ACCESS_LOCAL_READ_WRITE);

	    src_doca_buf = (struct doca_buf *)malloc(sizeof(struct doca_buf *));
	    dst_doca_buf = (struct doca_buf *)malloc(sizeof(struct doca_buf *));

	    doca_buf_inventory_buf_by_addr(state.buf_inv, state.src_mmap, (void*)src_buf, src_len, &src_doca_buf);
	    doca_buf_set_data(src_doca_buf, (void*)src_buf, src_len);

        doca_buf_inventory_buf_by_addr(state.buf_inv, state.dst_mmap, (void*)dst_buf,
                                        DOCA_SHA1_BYTE_COUNT, &dst_doca_buf);

        this->sha_job.base.type = DOCA_SHA_JOB_SHA1;
        this->sha_job.base.flags = DOCA_JOB_FLAGS_NONE;
        this->sha_job.base.ctx = this->state.ctx;
        this->sha_job.base.user_data.u64 = DOCA_SHA_JOB_SHA1;
        this->sha_job.resp_buf = this->dst_doca_buf;
        this->sha_job.req_buf = this->src_doca_buf;
        this->sha_job.flags = DOCA_SHA_JOB_FLAGS_NONE;
    }


private:

    static doca_error_t job_sha_hardware_is_supported(struct doca_devinfo *devinfo)
    {
        doca_error_t result;
        result = doca_sha_job_get_supported(devinfo, DOCA_SHA_JOB_SHA512);
        if (result != DOCA_SUCCESS)
            return result;
        return doca_sha_get_hardware_supported(devinfo);
    }

    static doca_error_t job_sha_software_is_supported(struct doca_devinfo *devinfo)
    {
        return doca_sha_job_get_supported(devinfo, DOCA_SHA_JOB_SHA512);
    }

    struct program_core_objects state = {0};
    struct doca_sha *sha_ctx;
    doca_error_t result;
    struct doca_event event = {0};
    struct timespec ts;
    uint8_t *resp_head;
    uint32_t workq_depth = 1;	
    uint32_t max_bufs = 2;
    struct doca_buf *src_doca_buf;
    struct doca_buf *dst_doca_buf;
    struct doca_sha_job sha_job;
};