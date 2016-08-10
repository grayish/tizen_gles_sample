#ifndef __NORMAL_MAPPING_VIEW_H__
#define __NORMAL_MAPPING_VIEW_H__

#include "sample/sample_view.h"

class NormalMappingView : public SampleView {
public:
	/**
	 * @brief Constructor for the class NormalMappingView
	 *
	 * @param[in] data Data for GLView
	 */
	NormalMappingView(void *data);

	/**
	 * @brief Init function for the class NormalMappingView
	 */
	virtual void OnInit() override;

};

#endif //__NORMAL_MAPPING_VIEW_H__


