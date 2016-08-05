#ifndef __INSTANCE_VIEW_H__
#define __INSTANCE_VIEW_H__

#include "sample/sample_view.h"

class InstanceView : public SampleView {
public:
	InstanceView(void *data);

	void OnInit() override;

};


#endif //__INSTANCE_VIEW_H__
