#ifndef __NOISE_H__
#define __NOISE_H__

#include "tex_prop.h"

class TexNoise : public TexProp {
public:
	GLubyte *mNoiseBuffer;

	TexNoise(const unsigned int size, float freq, const std::string& name);

	virtual ~TexNoise();

	void Create3DNoiseTexture(int textureSize, float frequency, GLubyte *targetBuffer);

};


#endif //__NOISE_H__
