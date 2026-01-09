RayTracingAccelerationStructure sceneBVH : register(t0);
RWTexture2D<float4> texFrame : register(u0);

[shader("raygeneration")]
void rayGen()
{
    uint2 pixel = DispatchRaysIndex().xy;
    uint2 resolution = DispatchRaysDimensions().xy;

    if (pixel.x >= resolution.x || pixel.y >= resolution.y) {
        return;
    }

    float2 uv = (float2(pixel) + 0.5) / float2(resolution);

    float2 screen = float2(
        uv.x * 2.0 - 1.0,
        1.0 - uv.y * 2.0
    );

    screen.x *= float(resolution.x) / float(resolution.y);

    RayDesc ray;
    ray.Origin = float3(0, 0, 0);
    ray.Direction = normalize(float3(screen.x, screen.y, -1));
    ray.TMin = 0.001;
    ray.TMax = 10000.0;

    RayPayload payload;
    payload.pixelColor = float4(0, 0, 0, 1);

    TraceRay(
       sceneBVH, RAY_FLAG_NONE, 0xFF, 0, 1, 0, cameraRay, payload
    );

    texFrame[pixel] = payload.pixelColor;
}

[shader("miss")]
void miss(inout RayPayload payload) {
	payload.pixelColor = float4(0.1, 0.2, 0.2, 1);
}

[shader("closesthit")]
void closestHit(inout RayPayload payload, BuiltInTriangleIntersectionAttributes attr) {
	payload.pixelColor = float4(0.7, 0.2, 0.6, 1);
}
