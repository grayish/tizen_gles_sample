#ifndef __MRT_VIEW_H__
#define __MRT_VIEW_H__

#include "sample/sample_view.h"

class MrtView : public SampleView {
public:
	MrtView(void *data);

	virtual void OnInit() override;

};

#endif //__MRT_VIEW_H__
