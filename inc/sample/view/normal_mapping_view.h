#ifndef __NORMAL_MAPPING_VIEW_H__
#define __NORMAL_MAPPING_VIEW_H__

#include "sample/sample_view.h"

class NormalMappingView : public SampleView {
public:
	NormalMappingView(void *data);

	virtual void OnInit() override;

};

#endif //__NORMAL_MAPPING_VIEW_H__


