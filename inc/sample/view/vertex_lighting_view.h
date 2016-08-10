#ifndef __VERTEX_LIGHTING_VIEW_H__
#define __VERTEX_LIGHTING_VIEW_H__

#include "sample/sample_view.h"

class VertexLightingView : public SampleView {
public:
	/**
	 * @brief Constructor for the class VertexLightingView
	 *
	 * @param[in] data Data for GLView
	 */
	VertexLightingView(void *data);

	/**
	 * @brief Init function for the class VertexLightingView
	 */
	void OnInit() override;

};


#endif //__VERTEX_LIGHTING_VIEW_H__


