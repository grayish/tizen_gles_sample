#ifndef __TEXTURE_ARRAY_VIEW_H__
#define __TEXTURE_ARRAY_VIEW_H__

#include "sample/sample_view.h"

class TextureArrayView : public SampleView {
public:
	explicit TextureArrayView(void *data);

	void OnInit() override;
};


#endif //__TEXTURE_ARRAY_VIEW_H__


