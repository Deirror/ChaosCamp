struct PSInput {
	float4 position : SV_POSITION;
};

struct PSOutput {
	float4 color : SV_TARGET0;
};

PSOutput PSMain(PSInput input) {
	return PSOutput(float4(1, 0, 1, 1));
}
