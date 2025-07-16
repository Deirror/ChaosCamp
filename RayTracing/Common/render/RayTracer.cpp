#include "RayTracer.h"

#include <thread>
#include <queue>
#include <mutex> 

#include "Intersection.h"
#include "PathRay.h"

CRT_BEGIN

RayTracer::RayTracer(const Scene* scene, RenderMode renderMode, 
	AccelerationType accelerationType, 
	unsigned short maxDepth) 
	: shader_(scene, this, maxDepth), scene_(scene), renderMode_(renderMode) {
	CRT_ENSURE(scene != nullptr, "Scene is null pointer");
	setTraceRayFunc(accelerationType);
}

ImageBuffer RayTracer::render() const {
	ImageBuffer buffer(scene_->settings().imageSettings.resolution);

	switch (renderMode_) {
	case RenderMode::Linear:
		renderLinear(buffer);
		break;
	case RenderMode::RegionBands:
		renderRegionBands(buffer);
		break;
	case RenderMode::RegionGrids:
		renderRegionGrids(buffer);
		break;
	case RenderMode::Buckets:
		renderBuckets(buffer);
		break;
	}

	return buffer;
}

void RayTracer::setTraceRayFunc(AccelerationType accelerationType) {
	switch (accelerationType) {
	case AccelerationType::Linear:
		traceRayFunc_ = [this](const Ray& ray) {
			return traceRayLinear(ray);
		};
		break;
	case AccelerationType::AABB:
		traceRayFunc_ = [this](const Ray& ray) {
			return traceRayAABB(ray);
		};
		break;
	default:
		traceRayFunc_ = [this](const Ray& ray) {
			return traceRayLinear(ray);
		};
		break;
	}
}

void RayTracer::traceRays(ImageBuffer& imageBuffer, const Interval& interval) const {
	const Resolution resolution = scene_->settings().imageSettings.resolution;
	const Camera& camera = scene_->camera();

	for (unsigned int y = interval.start.y; y < interval.end.y; ++y) {
		for (unsigned int x = interval.start.x; x < interval.end.x; ++x) {
			float s = static_cast<float>(x) / (resolution.width() - 1);
			float t = static_cast<float>(y) / (resolution.height() - 1);
			Ray ray = camera.getRay(s, t);

			HitRecord hitRecord = traceRayFunc_(ray);
			PathRay pathRay(ray);
			Color color = shader_.shade(pathRay, hitRecord);
			imageBuffer.set(x, y, color);
		}
	}
}

float RayTracer::updateHitRecord(const Ray& ray, const SceneTriangle& sceneTriangle, HitRecord& hitRecord, float closestT) const {
	HitRecord tempHit;
	const Triangle& triangle = scene_->triangle(sceneTriangle.triangleIndex);

	bool cullBackFace = (ray.rayType() == RayType::Camera);

	if (intersect(triangle, ray, tempHit, cullBackFace) && tempHit.t < closestT) {
		closestT = tempHit.t;
		hitRecord = tempHit;
		hitRecord.meshIndex = sceneTriangle.meshIndex;
		hitRecord.triangleIndex = sceneTriangle.triangleIndex;
		hitRecord.materialIndex = scene_->mesh(sceneTriangle.meshIndex).materialIndex();
		hitRecord.textureIndex = scene_->textureIndex(scene_->material(hitRecord.materialIndex).albedoTextureName());

		if (scene_->totalUvsCount() > 0) {
			unsigned int meshTriangleIdx = scene_->meshLocalTriangleIndex(sceneTriangle.meshIndex, sceneTriangle.triangleIndex);
			const Mesh& mesh = scene_->mesh(sceneTriangle.meshIndex);
			const TriangleIndices& triIndices = mesh.triangleIndices(meshTriangleIdx);
			const Vec3& v0uv = mesh.uv(triIndices.v0);
			const Vec3& v1uv = mesh.uv(triIndices.v1);
			const Vec3& v2uv = mesh.uv(triIndices.v2);
			hitRecord.puv = v1uv * hitRecord.barycentricCoords.x() +
				v2uv * hitRecord.barycentricCoords.y() +
				v0uv * hitRecord.barycentricCoords.z();
		}
	}

	return closestT;
}

HitRecord RayTracer::traceRayLinear(const Ray& ray) const {
	float closestT = FLT_MAX;
	crt::HitRecord finalHitRecord;

	for (const auto& sceneTriangle : scene_->sceneTriangles()) {
		closestT = updateHitRecord(ray, sceneTriangle, finalHitRecord, closestT);
	}

	return finalHitRecord;
}

HitRecord RayTracer::traceRayAABB(const Ray& ray) const {
	float closestT = FLT_MAX;
	HitRecord finalHitRecord;

	for (unsigned int meshIdx = 0; meshIdx < scene_->meshes().size(); ++meshIdx) {
		const auto& mesh = scene_->mesh(meshIdx);

		if (!intersect(mesh.aabb(), ray)) {
			continue;
		}

		unsigned int startTriangle = scene_->meshGlobalTriangleIndex(meshIdx, 0);
		unsigned int endTriangle = startTriangle + mesh.trianglesCount();

		for (unsigned int i = startTriangle; i < endTriangle; ++i) {
			closestT = updateHitRecord(ray, scene_->sceneTriangle(i), finalHitRecord, closestT);
		}
	}

	return finalHitRecord;
}

void RayTracer::renderLinear(ImageBuffer& imageBuffer) const {
	CRT_ENSURE(renderMode_ == RenderMode::Linear, "Render mode is not linear");
	Resolution resolution = scene_->settings().imageSettings.resolution;
	Interval interval(0, 0, resolution.width(), resolution.height());
	traceRays(imageBuffer, interval);
}

void RayTracer::renderRegionGrids(ImageBuffer& imageBuffer) const {
	CRT_ENSURE(renderMode_ == RenderMode::RegionGrids, "Render mode is not region grids");

	const Resolution resolution = scene_->settings().imageSettings.resolution;
	unsigned int threadsCount = std::thread::hardware_concurrency();

	unsigned int cols = static_cast<unsigned int>(std::ceil(std::sqrt(threadsCount)));
	unsigned int rows = static_cast<unsigned int>(std::ceil(float(threadsCount) / cols));

	unsigned int regionWidth = resolution.width() / cols;
	unsigned int regionHeight = resolution.height() / rows;

	std::vector<Interval> regions;

	for (unsigned int row = 0; row < rows; ++row) {
		for (unsigned int col = 0; col < cols; ++col) {
			unsigned int startX = col * regionWidth;
			unsigned int startY = row * regionHeight;
			unsigned int width = (col == cols - 1) ? (resolution.width() - startX) : regionWidth;
			unsigned int height = (row == rows - 1) ? (resolution.height() - startY) : regionHeight;

			regions.emplace_back(startX, startY, startX + width, startY + height);
		}
	}

	std::vector<std::thread> threads;

	for (unsigned int i = 0; i < threadsCount; ++i) {
		threads.emplace_back([&, region = regions[i]] () {
			traceRays(imageBuffer, region);
		});
	}

	for (auto& t : threads) { 
		t.join(); 
	}
}

void RayTracer::renderRegionBands(ImageBuffer& imageBuffer) const {
	CRT_ENSURE(renderMode_ == RenderMode::RegionBands, "Render mode is not region bands");

	const Resolution resolution = scene_->settings().imageSettings.resolution;

	unsigned int threadCount = std::thread::hardware_concurrency();
	unsigned int bandHeight = resolution.height() / threadCount;

	std::vector<Interval> bands;

	for (unsigned int i = 0; i < threadCount; ++i) {
		unsigned int startRow = i * bandHeight;
		unsigned int endRow = (i == threadCount - 1) ? resolution.height() : (i + 1) * bandHeight;

		bands.emplace_back(0, startRow, resolution.width(), endRow);
	}

	std::vector<std::thread> threads;

	for (unsigned int i = 0; i < threadCount; ++i) {
		threads.emplace_back([&, band = bands[i]] () {
			traceRays(imageBuffer, band);
		});
	}

	for (auto& t : threads) {
		t.join();
	}
}

void RayTracer::renderBuckets(ImageBuffer& buffer) const {
	CRT_ENSURE(renderMode_ == RenderMode::Buckets, "Render mode is not buckets");

	const Resolution resolution = scene_->settings().imageSettings.resolution;
	unsigned short bucketSize = scene_->settings().imageSettings.bucketSize;

	std::queue<Interval> workQueue;
	std::mutex queueMutex;

	for (unsigned short y = 0; y < resolution.height(); y += bucketSize) {
		for (unsigned short x = 0; x < resolution.width(); x += bucketSize) {
			workQueue.emplace(x, y, x + bucketSize, y + bucketSize);
		}
	}

	std::vector<std::thread> workers;
	unsigned int threadsCount = std::thread::hardware_concurrency();

	auto worker = [&]() {
		while (true) {
			Interval bucket;
			{
				std::lock_guard<std::mutex> lock(queueMutex);
				if (workQueue.empty()) break;
				bucket = workQueue.front();
				workQueue.pop();
			}

			traceRays(buffer, bucket);
		}
	};

	for (unsigned int i = 0; i < threadsCount; ++i) {
		workers.emplace_back(worker);
	}

	for (auto& t : workers) { 
		t.join(); 
	}
}

CRT_END
