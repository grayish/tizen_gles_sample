#ifndef __SHADOW_VIEW_H__
#define __SHADOW_VIEW_H__

#include "sample/sample_view.h"

class ShadowView : public SampleView {
private:
public:
	ShadowView(void *data);

	void OnInit() override;

};


#endif // __SHADOW_VIEW_H__
