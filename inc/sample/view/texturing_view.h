#ifndef __TEXTURING_VIEW_H__
#define __TEXTURING_VIEW_H__

#include "sample/sample_view.h"

class TexturingView : public SampleView {
public:
	explicit TexturingView(void *data);

	void OnInit() override;
};


#endif //__TEXTURING_VIEW_H__


