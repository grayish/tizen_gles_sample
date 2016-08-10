#ifndef __ENV_MAPPING_VIEW_H__
#define __ENV_MAPPING_VIEW_H__

#include "sample/sample_view.h"

class EnvMappingView : public SampleView {
public:
	/**
	 * @brief Constructor for the class EnvMappingView
	 *
	 * @param[in] data Data for GLView
	 */
	EnvMappingView(void *data);

	/**
	 * @brief Init function for the class EnvMappingView
	 */
	void OnInit() override;

};

#endif //__ENV_MAPPING_VIEW_H__


