#ifndef __INSTANCE_VIEW_H__
#define __INSTANCE_VIEW_H__

#include "sample/sample_view.h"

class InstanceView : public SampleView {
public:
	/**
	 * @brief Constructor for the class InstanceView
	 *
	 * @param[in] data Data for GLView
	 */
	InstanceView(void *data);

	/**
	 * @brief Init function for the class InstanceView
	 */
	void OnInit() override;

};


#endif //__INSTANCE_VIEW_H__
