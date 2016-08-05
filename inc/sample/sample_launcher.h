#ifndef __SAMPLE_LAUNCHER_H__
#define __SAMPLE_LAUNCHER_H__

#include "sample_view.h"

class SampleLauncher {
private:
	static SampleLauncher *instance;

	SampleView *curView;

private:
	Evas_Object *mParent;

	/**
    * @brief Constructor for the class SampleLauncher
    */
	SampleLauncher();

	/**
	* @brief Destructor for the class SampleLauncher
	*/
	~SampleLauncher();


public:
	/**
	* @brief Return singleton instance
	*
	* @return a pointer for singleton SampleLauncher object
	*/
	static SampleLauncher *GetInstance();

	/**
	* @brief Initialize mView with child of SampleView
	* @param[in] data Appdata
	*
	* @return a pointer of the glview
	*/
	Evas_Object *InitSampleView(void *data);

	/**
	* @brief Get evas_window
	*
	* @return a pointer of the glview
	*/
	Evas_Object *GetParent() const;

	/**
	* @brief Delete SampleView and itself
	*/
	void Release();
};

inline Evas_Object *SampleLauncher::GetParent() const {
	return mParent;
}

#endif //__SAMPLE_LAUNCHER_H__


