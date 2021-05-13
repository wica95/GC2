Texture2D shaderTexture;
SamplerState SampleType;
Texture2D reflectionTexture;
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ReflectionBuffer
{
	matrix reflectionMatrix;
};

struct VS_Input
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};
struct PS_Input
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 reflectionPosition : TEXCOORD1;
};



PS_Input VS_Main(VS_Input input)
{
	PS_Input output;
	matrix reflectProjectWorld;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
	
		// Create the reflection projection world matrix.
		reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
	reflectProjectWorld = mul(worldMatrix, reflectProjectWorld);
	
		// Calculate the input position against the reflectProjectWorld matrix.
		output.reflectionPosition = mul(input.position, reflectProjectWorld);

	return output;
}





float4 PS_Main(PS_Input input) : SV_TARGET
{
	float4 textureColor;
	float2 reflectTexCoord;
	float4 reflectionColor;
	float4 color;


	// Sample the texture pixel at this location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
// Calculate the projected reflection texture coordinates.
reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

// Sample the texture pixel from the reflection texture using the projected texture coordinates.
reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);

// Do a linear interpolation between the two textures for a blend effect.
color = lerp(textureColor, reflectionColor, 0.15f);

return color;
}
