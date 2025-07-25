#include "RayTracer.h"

#include <thread>
#include <queue>
#include <stack>
#include <mutex> 

#include "Intersection.h"
#include "PathRay.h"

CRT_BEGIN

RayTracer::RayTracer(Scene* scene, RenderMode renderMode, AccelerationType accelerationType) 
	: shader_(scene, this), scene_(scene), renderMode_(renderMode) {
	CRT_ENSURE(scene != nullptr, "Scene is null pointer");
	setTraceRayFunc(accelerationType);
	spp_ = scene_->settings().raySettings.spp;
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
		traceRayFunc_ = [this](const SceneRay& ray) {
			return traceRayLinear(ray);
		};
		break;
	case AccelerationType::AABB:
		traceRayFunc_ = [this](const SceneRay& ray) {
			return traceRayAABB(ray);
		};
		break;
	case AccelerationType::KDTree:
		traceRayFunc_ = [this](const SceneRay& ray) {
			return traceRayKDTree(ray);
		};
		break;
	default:
		traceRayFunc_ = [this](const SceneRay& ray) {
			return traceRayLinear(ray);
		};
		break;
	}
}

void RayTracer::traceRays(ImageBuffer& imageBuffer, const Interval& interval) const {
	const Resolution& resolution = scene_->settings().imageSettings.resolution;
	const Camera& camera = scene_->camera();

	int spp = std::max(1, spp_);
	bool ssaaOn = (spp_ > 1);

	for (int y = interval.start.y; y < interval.end.y; ++y) {
		for (int x = interval.start.x; x < interval.end.x; ++x) {
			Vec3 accumulatedColor;

			for (int s = 0; s < spp; ++s) {
				float dx = (ssaaOn) ? math::randf() : 0.5f;
				float dy = (ssaaOn) ? math::randf() : 0.5f;

				float u = (static_cast<float>(x) + dx) / (resolution.width() - 1);
				float v = (static_cast<float>(y) + dy) / (resolution.height() - 1);

				Ray ray = camera.getRay(u, v);
				HitRecord hitRecord = traceRayFunc_(ray);
				PathRay pathRay(ray);

				accumulatedColor += shader_.shade(pathRay, hitRecord);
			}

			accumulatedColor /= float(spp);
			Color finalColor = fromVec3ToColor(accumulatedColor * 255.f);
			imageBuffer.set(x, y, finalColor);
		}
	}
}

float RayTracer::updateHitRecord(const SceneRay& sceneRay, const SceneTriangle& sceneTriangle, HitRecord& hitRecord, float closestT) const {
	HitRecord tempHit;
	const Triangle& triangle = scene_->triangle(sceneTriangle.triangleIndex);
	
	bool materialBackFaceCulling = scene_->material(scene_->mesh(sceneTriangle.meshIndex).materialIndex()).backFaceCulling();
	bool cullBackFace = (sceneRay.type() == RayType::Camera) || materialBackFaceCulling;

	if (intersect(triangle, sceneRay, tempHit, cullBackFace) && tempHit.t < closestT) {
		closestT = tempHit.t;
		hitRecord = tempHit;
		hitRecord.meshIndex = sceneTriangle.meshIndex;
		hitRecord.triangleIndex = sceneTriangle.triangleIndex;

		hitRecord.materialIndex = scene_->mesh(sceneTriangle.meshIndex).materialIndex();
		hitRecord.textureIndex = scene_->textureIndex(scene_->material(hitRecord.materialIndex).albedoTextureName());

		TextureType textureType = scene_->texture(hitRecord.textureIndex).textureType();
		if (textureType == TextureType::Zebra || textureType == TextureType::Checker || textureType == TextureType::Bitmap) {
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

HitRecord RayTracer::traceRayLinear(const SceneRay& ray) const {
	float closestT = FLT_MAX;
	HitRecord finalHitRecord;

	for (const auto& sceneTriangle : scene_->sceneTriangles()) {
		closestT = updateHitRecord(ray, sceneTriangle, finalHitRecord, closestT);
	}

	return finalHitRecord;
}

HitRecord RayTracer::traceRayAABB(const SceneRay& ray) const {
	float closestT = FLT_MAX;
	HitRecord finalHitRecord;

	const AABB& topLevelAABB = scene_->sceneAABB();
	if (!intersect(topLevelAABB, ray)) {
		return finalHitRecord;
	}

	for (int meshIdx = 0; meshIdx < scene_->meshes().size(); ++meshIdx) {
		const auto& mesh = scene_->mesh(meshIdx);

		if (!intersect(mesh.aabb(), ray)) {
			continue;
		}

		size_t startTriangle = scene_->meshGlobalTriangleIndex(meshIdx, 0);
		size_t endTriangle = startTriangle + mesh.trianglesCount();

		for (size_t i = startTriangle; i < endTriangle; ++i) {
			closestT = updateHitRecord(ray, scene_->sceneTriangle(static_cast<int>(i)), finalHitRecord, closestT);
		}
	}

	return finalHitRecord;
}

HitRecord RayTracer::traceRayKDTree(const SceneRay& ray) const {
	const KDTree& kdTree = scene_->kdTree();
	const std::vector<KDTreeNode>& nodes = kdTree.nodes();

	float closestT = FLT_MAX;
	HitRecord finalHitRecord;

	std::stack<unsigned int> nodeIndices;
	nodeIndices.push(0);

	while (nodeIndices.size() > 0) {
		unsigned int nodeIdx = nodeIndices.top();
		nodeIndices.pop();

		const KDTreeNode& node = nodes[nodeIdx];
		if (intersect(node.aabb, ray)) {
			if (node.triangleIndices.size() > 0) {
				for (auto triangleIdx : node.triangleIndices) {
					closestT = updateHitRecord(ray, scene_->sceneTriangle(triangleIdx), finalHitRecord, closestT);
				}
			}
			else {
				if (node.left != -1) {
					nodeIndices.push(node.left);
				}
				if (node.right != -1) {
					nodeIndices.push(node.right);
				}
			}
		}
	}

	return finalHitRecord;
}

void RayTracer::renderLinear(ImageBuffer& imageBuffer) const {
	CRT_ENSURE(renderMode_ == RenderMode::Linear, "Render mode is not linear");
	const Resolution& resolution = scene_->settings().imageSettings.resolution;
	Interval interval(0, 0, resolution.width(), resolution.height());
	traceRays(imageBuffer, interval);
}

void RayTracer::renderRegionGrids(ImageBuffer& imageBuffer) const {
	CRT_ENSURE(renderMode_ == RenderMode::RegionGrids, "Render mode is not region grids");

	const Resolution& resolution = scene_->settings().imageSettings.resolution;
	unsigned int threadsCount = std::thread::hardware_concurrency();

	int cols = static_cast<int>(std::ceil(std::sqrt(threadsCount)));
	int rows = static_cast<int>(std::ceil(float(threadsCount) / cols));

	int regionWidth = resolution.width() / cols;
	int regionHeight = resolution.height() / rows;

	std::vector<Interval> regions;

	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			int startX = col * regionWidth;
			int startY = row * regionHeight;
			int width = (col == cols - 1) ? (resolution.width() - startX) : regionWidth;
			int height = (row == rows - 1) ? (resolution.height() - startY) : regionHeight;

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

	const Resolution& resolution = scene_->settings().imageSettings.resolution;

	unsigned int threadCount = std::thread::hardware_concurrency();
	int bandHeight = resolution.height() / threadCount;

	std::vector<Interval> bands;

	for (unsigned int i = 0; i < threadCount; ++i) {
		int startRow = i * bandHeight;
		int endRow = (i == threadCount - 1) ? resolution.height() : (i + 1) * bandHeight;

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

	const Resolution& resolution = scene_->settings().imageSettings.resolution;
	int bucketSize = scene_->settings().imageSettings.bucketSize;
	CRT_ENSURE(bucketSize > 0, "Bucket size is invalid");

	std::queue<Interval> workQueue;
	std::mutex queueMutex;

	for (int y = 0; y < resolution.height(); y += bucketSize) {
		for (int x = 0; x < resolution.width(); x += bucketSize) {
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
