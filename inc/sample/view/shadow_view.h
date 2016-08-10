#ifndef __SHADOW_VIEW_H__
#define __SHADOW_VIEW_H__

#include "sample/sample_view.h"

class ShadowView : public SampleView {
private:
public:
	/**
	 * @brief Constructor for the class ShadowView
	 *
	 * @param[in] data Data for GLView
	 */
	ShadowView(void *data);

	/**
	 * @brief Init function for the class ShadowView
	 */
	void OnInit() override;

};


#endif // __SHADOW_VIEW_H__
