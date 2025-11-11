#ifndef IO_MUX_DEFINE_H__
#define IO_MUX_DEFINE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef char*(*ProcessRequestFunction)(const char* request, size_t req_size, size_t* res_size);

#ifdef __cplusplus
}
#endif

#endif
