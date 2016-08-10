#ifndef __MRT_VIEW_H__
#define __MRT_VIEW_H__

#include "sample/sample_view.h"

class MrtView : public SampleView {
public:
	/**
	 * @brief Constructor for the class MrtView
	 *
	 * @param[in] data Data for GLView
	 */
	MrtView(void *data);

	/**
	 * @brief Init function for the class MrtView
	 */
	virtual void OnInit() override;

};

#endif //__MRT_VIEW_H__
