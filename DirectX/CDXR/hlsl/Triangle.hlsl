struct PSInput {
	float4 position : SV_POSITION;
};

struct PSOutput {
	float4 color : SV_TARGET0;
};

cbuffer Constants : register(b0) {
	int frameIdx;
	int colorIdx;
	float offsX;
	float offsY;
}

static const float4 colors[2] = {
	{ 0.5, 0, 0, 1 },
	{ 0.7, 0, 0.6, 1 },
};

PSOutput PSMain(PSInput input) {
	return PSOutput(colors[colorIdx]);
}
