#ifndef __FRAG_LIGHTING_VIEW_H__
#define __FRAG_LIGHTING_VIEW_H__

#include "sample/sample_view.h"

class FragLightingView : public SampleView {
public:
	/**
	 * @brief Constructor for the class FragLightingView
	 *
	 * @param[in] data Data for GLView
	 */
	FragLightingView(void *data);

	/**
	 * @brief Init function for the class FragLightingView
	 */
	void OnInit() override;
};


#endif //__FRAG_LIGHTING_VIEW_H__


