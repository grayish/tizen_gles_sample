#ifndef __LOADER_INTERFACE_H__
#define __LOADER_INTERFACE_H__

#include <Elementary.h>

#ifdef __cplusplus
extern "C"
{
#endif
/**
* @brief Initialize mView with child of SampleView and get mView object
* @param[in] data Appdata
*
* @return a pointer of the glview
*/
Evas_Object *get_sample_view(void *data);


/**
* @brief Delete SampleView and SampleLauncher
*/
void release_loader();

#ifdef __cplusplus
}
#endif

#endif //__LOADER_INTERFACE_H__


