#pragma once

#include "RayTracer.h"

CRT_BEGIN

class Animation {
public:
	Animation(RayTracer& rayTracer, const std::string& output = kDefaultFilepath)
	: rayTracer_(rayTracer), output_(output) {}

	const std::string& output() const { return output_; }
	void output(const std::string& output) { output_ = output; frame_ = 0; }

	unsigned int frame() const { return frame_; }
	void frame(unsigned int frame) { frame_ = frame; }

	bool skipSaveFrame() const { return skipSaveFrame_; }
	void skipSaveFrame(bool skipSaveFrame) { skipSaveFrame_ = skipSaveFrame; }

	void staticSequence(unsigned int frames);

	void craneReveal(float dz, float dy, float tilt, const Vec3& lookAt, unsigned int frames);

	void orbit(float pan, float tilt, unsigned int frames); 
	void zigZagOrbit(float pan, float tilt, unsigned int steps, unsigned int frames); 
	
	void vertigo(float dz, float fov, unsigned int frames); 

	void modifyLighting(float intensity, const std::vector<unsigned int>& indexes, unsigned int frames);
	void modifyLighting(float intensity, unsigned int frames);

	void disco(float speed, const std::vector<unsigned int>& meshIndexes, unsigned int frames);

public:
	static constexpr char const* kDefaultFilepath = "image";
	
private:
	void saveFrame() const;

private:
	RayTracer& rayTracer_;

	std::string output_;
	unsigned int frame_= 0;

	bool skipSaveFrame_ = false;
};

CRT_END
