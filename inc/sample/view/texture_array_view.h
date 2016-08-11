#ifndef __TEXTURE_ARRAY_VIEW_H__
#define __TEXTURE_ARRAY_VIEW_H__

#include "sample/sample_view.h"

class TextureArrayView : public SampleView {
public:
	/**
	 * @brief Constructor for the class TextureArrayView
	 *
	 * @param[in] data Data for GLView
	 */
	explicit TextureArrayView(void *data);

	/**
	 * @brief Init function for the class TextureArrayView
	 */
	void OnInit() override;

	void OnStep() override;
};


#endif //__TEXTURE_ARRAY_VIEW_H__


