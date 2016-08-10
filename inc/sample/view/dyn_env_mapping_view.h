#ifndef __DYN_ENV_MAPPING_VIEW_H__
#define __DYN_ENV_MAPPING_VIEW_H__

#include "sample/sample_view.h"

class DynEnvMappingView : public SampleView {
public:
	/**
	 * @brief Constructor for the class DynEnvMappingView
	 *
	 * @param[in] data Data for GLView
	 */
	DynEnvMappingView(void *data);

	/**
	 * @brief Init function for the class DynEnvMappingView
	 */
	void OnInit() override;

};

#endif //__ENV_MAPPING_VIEW_H__


