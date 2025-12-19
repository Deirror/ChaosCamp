struct VSInput {
	float2 position : POSITION;
};

struct VSOutput {
	float4 position : SV_POSITION;
};

cbuffer Constants : register(b0) {
	int frameIdx;
	int colorIdx;
	float offsX;
	float offsY;
}

VSOutput VSMain(VSInput input) {
	VSOutput result;
	result.position = float4(0, 0, 0, 1);

	static const float step = 0.001;

	float deg = (float)frameIdx * step;
	float sine = sin(deg);
	float cosine = cos(deg);

	result.position.x = input.position.x * cosine - input.position.y * sine;
	result.position.y = input.position.x * sine + input.position.y * cosine;

	result.position.x += offsX;
	result.position.y += offsY;

	return result;
}
