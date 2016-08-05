#ifndef __ENV_MAPPING_VIEW_H__
#define __ENV_MAPPING_VIEW_H__

#include "sample/sample_view.h"

class EnvMappingView : public SampleView {
public:
	EnvMappingView(void *data);

	void OnInit() override;

};

#endif //__ENV_MAPPING_VIEW_H__


