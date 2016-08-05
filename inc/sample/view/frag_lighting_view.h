#ifndef __FRAG_LIGHTING_VIEW_H__
#define __FRAG_LIGHTING_VIEW_H__

#include "sample/sample_view.h"

class FragLightingView : public SampleView {
public:
	FragLightingView(void *data);

	void OnInit() override;
};


#endif //__FRAG_LIGHTING_VIEW_H__


