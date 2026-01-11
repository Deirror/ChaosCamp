#pragma once

#include "core.h"

// The two functions fill the vertexData with:
// - A hypnotic ribbon spiral
// - "DEIRRORz X" 

// Just a note: AI is used for the algorithms.

inline void Build_DEIRRORz_X_Triangles(std::array<Vertex, 1758>& vertexData) {

	int index = 1440; // Offset from the ribbon spiral coords.

	auto quad = [&](float x0, float y0, float x1, float y1) {

		vertexData[index] = { x0, y0, -1.f }; index++;
		vertexData[index] = { x1, y1, -1.f }; index++;
		vertexData[index] = { x1, y0, -1.f }; index++;

		vertexData[index] = { x0, y0, -1.f }; index++;
		vertexData[index] = { x1, y1, -1.f }; index++;
		vertexData[index] = { x0, y1, -1.f }; index++;
	};

	auto tri = [&](float ax, float ay, float bx, float by, float cx, float cy) {

		vertexData[index] = { ax, ay, -1.f }; index++;
		vertexData[index] = { bx, by, -1.f }; index++;
		vertexData[index] = { cx, cy, -1.f }; index++;
	};

	auto diag = [&](float x0, float y0, float x1, float y1, float th) {

		float dx = x1 - x0, dy = y1 - y0;
		float len = std::sqrt(dx * dx + dy * dy);
		if (len < 1e-6f) return;

		dx /= len; dy /= len;
		float px = -dy * (th * 0.5f);
		float py = dx * (th * 0.5f);

		float aX = x0 - px, aY = y0 - py;
		float bX = x0 + px, bY = y0 + py;
		float cX = x1 + px, cY = y1 + py;
		float dX = x1 - px, dY = y1 - py;

		tri(aX, aY, bX, bY, cX, cY);
		tri(aX, aY, cX, cY, dX, dY);
	};

	// Layout / font metrics
	const float startX = -0.92f;
	const float baseY = -0.15f;

	const float W = 0.15f;  // glyph width
	const float H = 0.32f;  // glyph height
	const float T = 0.05f;  // stroke thickness
	const float GAP = 0.03f;  // spacing

	float ox = startX;

	// ---- D ----
	quad(ox + 0.00f, baseY + 0.00f, ox + T, baseY + H);
	quad(ox + 0.00f, baseY + H - T, ox + W, baseY + H);
	quad(ox + 0.00f, baseY + 0.00f, ox + W, baseY + T);
	quad(ox + W - T, baseY + 0.00f, ox + W, baseY + H);
	ox += W + GAP;

	// ---- E ----
	quad(ox + 0.00f, baseY + 0.00f, ox + T, baseY + H);
	quad(ox + 0.00f, baseY + H - T, ox + W, baseY + H);
	quad(ox + 0.00f, baseY + 0.5f * H - 0.5f * T, ox + W * 0.80f, baseY + 0.5f * H + 0.5f * T);
	quad(ox + 0.00f, baseY + 0.00f, ox + W, baseY + T);
	quad(ox + W - T, baseY + 0.55f * H, ox + W, baseY + H);
	ox += W + GAP;

	// ---- I ----
	quad(ox + 0.50f * W - 0.50f * T, baseY + 0.00f,
		ox + 0.50f * W + 0.50f * T, baseY + H);
	ox += W + GAP;

	// ---- R ----
	auto drawR = [&](float x) {

		quad(x + 0.00f, baseY + 0.00f, x + T, baseY + H);     // left
		quad(x + 0.00f, baseY + H - T, x + W, baseY + H);     // top

		const float midY = baseY + 0.56f * H;
		quad(x + 0.00f, midY - 0.5f * T, x + W * 0.78f, midY + 0.5f * T); // mid

		quad(x + W - T, midY, x + W, baseY + H);     // right upper

		// diagonal leg
		const float sx = x + 0.42f * W;
		const float sy = midY - 0.15f * H;
		const float ex = x + W;
		const float ey = baseY;
		diag(sx, sy, ex, ey, T);
	};

	// ---- R ----
	drawR(ox); ox += W + GAP;
	// ---- R ----
	drawR(ox); ox += W + GAP;

	// ---- O ----
	quad(ox + 0.00f, baseY + 0.00f, ox + T, baseY + H);
	quad(ox + W - T, baseY + 0.00f, ox + W, baseY + H);
	quad(ox + 0.00f, baseY + H - T, ox + W, baseY + H);
	quad(ox + 0.00f, baseY + 0.00f, ox + W, baseY + T);
	ox += W + GAP;

	// ---- R ----
	drawR(ox); ox += W + GAP;

	{
		const float zW = W * 0.85f;
		const float zH = H * 0.70f;
		const float zX = ox;
		const float zY = baseY;            // baseline
		const float zT = T * 0.85f;

		quad(zX + 0.00f, zY + zH - zT, zX + zW, zY + zH);
		quad(zX + 0.00f, zY + 0.00f, zX + zW, zY + zT);

		const int   segs = 12;
		const float x0 = zX + zW * 0.92f;
		const float y0 = zY + zH * 0.82f;
		const float x1 = zX + zW * 0.08f;
		const float y1 = zY + zH * 0.18f;

		for (int s = 0; s < segs; ++s)
		{
			float a0 = (float)s / (float)segs;
			float a1 = (float)(s + 1) / (float)segs;

			float bow0 = 0.06f * zH * std::sin(a0 * 3.1415926f);
			float bow1 = 0.06f * zH * std::sin(a1 * 3.1415926f);

			float sx = x0 + (x1 - x0) * a0;
			float sy = y0 + (y1 - y0) * a0 + bow0;

			float ex = x0 + (x1 - x0) * a1;
			float ey = y0 + (y1 - y0) * a1 + bow1;

			diag(sx, sy, ex, ey, zT);
		}

		quad(zX + zW * 0.80f, zY + zH - zT * 1.10f, zX + zW, zY + zH);        // top-right cap
		quad(zX + 0.00f, zY + 0.00f, zX + zW * 0.20f, zY + zT * 1.10f);  // bottom-left cap

		ox += zW;
	}

	// ---- space ----
	ox += W * 0.55f;

	// ---- X ----
	auto drawX = [&](float x) {

		const float xL = x;
		const float xR = x + W;
		const float yB = baseY;
		const float yT = baseY + H;

		for (int s = 0; s < 4; ++s) {

			float a0 = (float)s / 4.0f;
			float a1 = (float)(s + 1) / 4.0f;

			// bottom-left -> top-right
			diag(xL + (xR - xL) * a0, yB + (yT - yB) * a0,
				xL + (xR - xL) * a1, yB + (yT - yB) * a1, T);

			// top-left -> bottom-right
			diag(xL + (xR - xL) * a0, yT - (yT - yB) * a0,
				xL + (xR - xL) * a1, yT - (yT - yB) * a1, T);
		}
	};

	drawX(ox);
}

inline void BuildSpiralRibbonTriangles(std::array<Vertex, 1758>& vertexData) {
	
	static constexpr int kSegments = 240;          
	static constexpr int kVertPerSeg = 6;            
	static constexpr int kVertexCount = kSegments * kVertPerSeg;

	int index = 0;

	auto put = [&](float x, float y) {
		vertexData[index] = { x, y, -1.005f, 1 };
		index++;
	};

	for (int i = 0; i < kSegments; ++i)
	{
		const int j = (i + 1) % kSegments;

		const float a0 = i * 0.35f;
		const float a1 = j * 0.35f;

		const float t0 = (float)i / (float)(kSegments - 1);
		const float t1 = (float)j / (float)(kSegments - 1);

		const float r0 = 0.15f + 0.70f * t0 + 0.05f * std::sin(i * 0.40f);
		const float r1 = 0.15f + 0.70f * t1 + 0.05f * std::sin(j * 0.40f);

		const float w0 = 0.025f + 0.020f * (0.5f + 0.5f * std::sin(i * 0.55f));
		const float w1 = 0.025f + 0.020f * (0.5f + 0.5f * std::sin(j * 0.55f));

		const float cx0 = r0 * std::cos(a0);
		const float cy0 = r0 * std::sin(a0);

		const float cx1 = r1 * std::cos(a1);
		const float cy1 = r1 * std::sin(a1);

		const float nx0 = -std::sin(a0);
		const float ny0 = std::cos(a0);

		const float nx1 = -std::sin(a1);
		const float ny1 = std::cos(a1);

		const float ix0 = cx0 - nx0 * w0;
		const float iy0 = cy0 - ny0 * w0;
		const float ox0 = cx0 + nx0 * w0;
		const float oy0 = cy0 + ny0 * w0;

		const float ix1 = cx1 - nx1 * w1;
		const float iy1 = cy1 - ny1 * w1;
		const float ox1 = cx1 + nx1 * w1;
		const float oy1 = cy1 + ny1 * w1;

		put(ix0, iy0);
		put(ox0, oy0);
		put(ox1, oy1);

		put(ix0, iy0);
		put(ox1, oy1);
		put(ix1, iy1);
	}
}

