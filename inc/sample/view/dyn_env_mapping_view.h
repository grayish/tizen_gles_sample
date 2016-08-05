#ifndef __DYN_ENV_MAPPING_VIEW_H__
#define __DYN_ENV_MAPPING_VIEW_H__

#include "sample/sample_view.h"

class DynEnvMappingView : public SampleView {
public:
	DynEnvMappingView(void *data);

	void OnInit() override;

};

#endif //__ENV_MAPPING_VIEW_H__


