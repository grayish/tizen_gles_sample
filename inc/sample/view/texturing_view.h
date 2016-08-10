#ifndef __TEXTURING_VIEW_H__
#define __TEXTURING_VIEW_H__

#include "sample/sample_view.h"

class TexturingView : public SampleView {
public:
	/**
	 * @brief Constructor for the class TexturingView
	 *
	 * @param[in] data Data for GLView
	 */
	explicit TexturingView(void *data);

	/**
	 * @brief Init function for the class TexturingView
	 */
	void OnInit() override;
};


#endif //__TEXTURING_VIEW_H__


