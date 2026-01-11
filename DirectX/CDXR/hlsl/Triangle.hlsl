struct PSInput {
	float4 position : SV_POSITION;
	nointerpolation uint partID : PART_ID;
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
	{ 0, 0, 0, 1 },
	{ 1, 1, 1, 1 },
};

PSOutput PSMain(PSInput input) {

	int partColorIdx = (colorIdx + input.partID) % 2;
	return PSOutput(colors[partColorIdx]);
}
