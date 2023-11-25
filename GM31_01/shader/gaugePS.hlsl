
#include "common.hlsl"

static const float pi = 3.1415926535f;

//#define SQUARE
#define CIRCLE

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	/*
	color.x = Current HP
	color.y = Max HP
	color.z = Difference HP
	*/

#ifdef SQUARE
	outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);

	//Square Gauge
	float gauge = In.TexCoord * color.y;
	outDiffuse.rgb += color2.rgb * step(color.z, gauge);
	outDiffuse.rgb += color.rgb * (1.0f - step(color.x, gauge));
	outDiffuse.rgb += color4.rgb * (1.0f - step(color.z, gauge)) * step(color.x, gauge);
#endif

#ifdef CIRCLE
	outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float2 uv = In.TexCoord - 0.5f;

	float angle = atan2(uv.x, uv.y);
	angle += pi;

	float gauge = frac(angle * 0.5f / pi) * color.y;
	outDiffuse.rgb += color2.rgb * step(color.z, gauge);
	outDiffuse.rgb += color.rgb * (1.0f - step(color.x, gauge));
	outDiffuse.rgb += color4.rgb * (1.0f - step(color.z, gauge)) * step(color.x, gauge);

	float dist = length(uv) / 0.5f;
	outDiffuse.a = (1.0f - step(1.0, dist)) * step(1.0f / color.w, dist);
#endif
}

int step(float a, float b)
{
	if (b >= a) { return 1; }
	return 0;
}