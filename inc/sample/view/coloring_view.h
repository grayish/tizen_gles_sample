#ifndef __COLORING_VIEW_H__
#define __COLORING_VIEW_H__

#include "sample/sample_view.h"

class ColoringView : public SampleView {
public:
	/**
	 * @brief Constructor for the class ColoringView
	 *
	 * @param[in] data Data for GLView
	 */
	explicit ColoringView(void *data);

	/**
	 * @brief Init function for the class ColoringView
	 */
	void OnInit() override;
};


#endif //__COLORING_VIEW_H__
