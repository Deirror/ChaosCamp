struct VSInput {
	float2 position : POSITION;
};

struct VSOutput {
	float4 position : SV_POSITION;
};

VSOutput VSMain(VSInput input) {
	return VSOutput(float4(input.position, 0, 1));
}
