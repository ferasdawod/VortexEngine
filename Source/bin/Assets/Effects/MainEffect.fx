// lighting structures

static const int MAX_DIRECTIONAL = 3;
static const int MAX_POINT = 3;
static const int MAX_SPOT = 3;

// update these from the effect class dude
static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;

struct DirectionalLight
{
	float3		DiffuseColor;
	float		Strength;

	float3		Direction;
	float		ShadowStrength;
};

struct PointLight
{
	float3		DiffuseColor;
	float		Strength;

	float3		Position;
	float		Falloff;

	float		Range;
	float3		_pad;
};

struct SpotLight
{
	float3		DiffuseColor;
	float		Strength;

	float3		Position;
	float		ConeAngle;

	float3		Direction;
	float		Falloff;
};

struct Material
{
	float4	DiffuseColor;
	float3	SpecularColor;
	float	SpecularPower;
	float4	AmbientColor;

	bool	IsLit;
	bool	IsTextured;
	bool	ReceiveShadows;
	bool	HasSpecularMap;

	bool	HasNormalMap;
	float3	_pad;

	matrix	TextureMatrix;
};

Texture2D diffuseTexture	: register(t0);
SamplerState diffuseSampler : register(s0);

Texture2D shadowMapTexture	: register(t1);
SamplerState shadowSampler	: register(s1);

Texture2D specularTexture	: register(t2);
Texture2D normalmapTexture	: register(t3);

SamplerComparisonState samShadow
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;

	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComparisonFunc = LESS_EQUAL;
};

cbuffer MaterialBuffer	: register(b0)
{
	Material	ObjectMaterial;
}

cbuffer MatricesBuffer : register(b1)
{
	matrix		WorldMatrix;
	matrix		WorldInvTransMatrix;
}

cbuffer PerFrameBuffer : register(b2)
{
	matrix				ViewProjectionMatrix;
	matrix				LightViewProjectionMatrix;
	
	DirectionalLight	DirectionalLights[MAX_DIRECTIONAL];
	PointLight			PointLights[MAX_POINT];
	SpotLight			SpotLights[MAX_SPOT];
	
	float4				AmbientColor;
	
	float3				CameraPositionW;
	float				_pad0;

	int					DirectionalCount;
	int					PointCount;
	int					SpotCount;
	int					_pad1;
}

cbuffer SettingsBuffer : register(b3)
{
	bool	TexturingEnabled;
	bool	LightingEnabled;
	bool	ShadowsEnabled;
	bool	SoftShadowsEnabled;
}

struct VS_Input
{
	float4 PosL	: POSITION;
	float4 Col	: COLOR;
	float2 Tex	: TEXCOORD0;
	float3 NorL	: NORMAL;
	float3 TanL	: TANGANT;
};

struct VS_Output
{
	float4 PosH				: SV_POSITION;
	float4 Col				: COLOR;
	float2 Tex				: TEXCOORD0;
	float3 NorW				: NORMAL;
	float3 TanW				: TANGANT;
	float4 PosHL			: TEXCOORD1;
	float4 PosW				: TEXCOORD2;
};

struct PS_Output
{
	float4	OutColor : SV_Target;
};

float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH)
{
	float depth = shadowPosH.z;
	depth = saturate(depth);

	const float dx = SMAP_DX;

	float percentLit = 0.0f;
	const float2 offsets[9] =
	{
		float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
	};

	for (int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow,
												   shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.0f;
}

float3 NormalSampleToWorld(float3 normalSample, float3 normalW, float3 tanW)
{
	// move from [0,1] to [-1,1]
	float3 normalT = 2.0f * normalSample - 1.0f;

	float3 N = normalW;
	float3 T = normalize(tanW - dot(tanW, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);
	
	float3 normal = mul(normalT, TBN);
	return normal;
}

VS_Output VS_Main(VS_Input inputVert)
{
	VS_Output retVert = (VS_Output)0;

	float4 worldPos = mul(inputVert.PosL, WorldMatrix);
	retVert.PosH = mul(worldPos, ViewProjectionMatrix);

	retVert.Col = inputVert.Col;
	retVert.Tex = mul(float4(inputVert.Tex, 0.0f, 1.0f), ObjectMaterial.TextureMatrix).xy;
	retVert.NorW = mul(inputVert.NorL, (float3x3)WorldInvTransMatrix);
	retVert.TanW = mul(inputVert.TanL, (float3x3)WorldMatrix);

	retVert.PosHL = mul(worldPos ,LightViewProjectionMatrix);
	retVert.PosW = worldPos;

	return retVert;
}

PS_Output PS_Main(VS_Output inputFrag)
{
	PS_Output ps_out = (PS_Output)0;

	// start with the vertex color and apply texturing if needed
	float4 color = inputFrag.Col;

	if (ObjectMaterial.IsTextured && TexturingEnabled)
	{
		color *= diffuseTexture.Sample(diffuseSampler, inputFrag.Tex);
	}


	[flatten]
	if (ObjectMaterial.IsLit && LightingEnabled)
	{
		float3 lighting = (float3)0.0f;

		float3 ambient = (float3)0.0f;
		float3 diffuse = (float3)0.0f;
		float3 specular = (float3)0.0f;

		ambient += AmbientColor.xyz * ObjectMaterial.AmbientColor.xyz;

		float3 normal = normalize(inputFrag.NorW);
		if (ObjectMaterial.HasNormalMap && TexturingEnabled)
		{
			float3 normalSample = normalmapTexture.Sample(diffuseSampler, inputFrag.Tex).rgb;
			normal = NormalSampleToWorld(normalSample, normal, inputFrag.TanW);
		}

		float specularFactor = 1.0f;
		if (ObjectMaterial.HasSpecularMap && TexturingEnabled)
		{
			specularFactor = specularTexture.Sample(diffuseSampler, inputFrag.Tex).r;
		}

		float3 view = normalize(CameraPositionW - inputFrag.PosW.xyz);

		float shadowFactor = 1.0f;
		
		// if the object needs shadows then do it
		if (ShadowsEnabled && ObjectMaterial.ReceiveShadows)
		{
			// divide to get back in homogenous clip space
			inputFrag.PosHL.xyz /= inputFrag.PosHL.w;
			// move from [-1,1] to [0,1]
			inputFrag.PosHL.x = (inputFrag.PosHL.x + 1) / 2;
			inputFrag.PosHL.y = (inputFrag.PosHL.y + 1) / 2;
			// the texture origin is in the top left corner
			inputFrag.PosHL.y = 1.0f - inputFrag.PosHL.y;

			// only if the vertex is in the shadow map do shadow mapping stuff
			if (inputFrag.PosHL.x > 0 && inputFrag.PosHL.x < 1 && inputFrag.PosHL.y > 0 && inputFrag.PosHL.y < 1)
			{
				if (SoftShadowsEnabled)
				{
					// SOFT SHADOWS
					shadowFactor = CalcShadowFactor(samShadow, shadowMapTexture, inputFrag.PosHL);
				}
				else
				{
					// HARD SHADOWS
					// get the depth and clamp it so anything beyond the shadow map is always lit
					float depth = inputFrag.PosHL.z;
					depth = saturate(depth);

					float depthSample = shadowMapTexture.Sample(shadowSampler, inputFrag.PosHL.xy).r;
					// if the depth in the texture is lower ( closer ) then our depth then we must be in shadow
					// and we will only receive ambient color
					if (depthSample < depth)
					{
						shadowFactor = 1.0f - DirectionalLights[0].ShadowStrength;
					}
				}
			}
		}

		// Directional Lights
		[unroll]
		for (int i = 0; i < DirectionalCount; ++i)
		{
			float3 lightDir = normalize(DirectionalLights[i].Direction);
			float3 refl = reflect(lightDir, normal);

			// diffuse stuff
			diffuse += saturate(dot(-lightDir, normal)) * shadowFactor *
				DirectionalLights[i].DiffuseColor * DirectionalLights[i].Strength;
			// specular stuff
			specular += pow(max(dot(refl, view), 0.0f), max(ObjectMaterial.SpecularPower, 0.0001f))
				* ObjectMaterial.SpecularColor * shadowFactor * specularFactor * DirectionalLights[i].Strength;;
		}

		// Point Lights
		[unroll]
		for (int j = 0; j < PointCount; ++j)
		{
			float3 lightDir = PointLights[j].Position - inputFrag.PosW.xyz;
			float d = length(lightDir);

			if (d < PointLights[j].Range)
			{
				lightDir /= d;

				float3 refl = reflect(-lightDir, normal);

				// diffuse stuff
				float diffuseAmount = saturate(dot(lightDir, normal));
				float attenuation = 1.0f - pow(clamp(d / PointLights[j].Range, 0.0f, 1.0f), PointLights[j].Falloff);
				
				diffuse += diffuseAmount * attenuation * PointLights[j].DiffuseColor * PointLights[j].Strength;

				// specular stuff
				specular += pow(max(dot(refl, view), 0.0f), max(ObjectMaterial.SpecularPower, 0.0001f)) *
					ObjectMaterial.SpecularColor * attenuation * PointLights[j].Strength * specularFactor;
			}
		}

		// SpotLights
		[unroll]
		for (int k = 0; k < SpotCount; ++k)
		{
			float3 lightDir = normalize(SpotLights[k].Position - inputFrag.PosW.xyz);
			float3 refl = reflect(-lightDir, normal);

			// diffuse stuff
			float diffuseAmount = saturate(dot(normal, lightDir));
			float d = dot(-lightDir, normalize(SpotLights[k].Direction));
			float a = cos(SpotLights[k].ConeAngle);

			// TODO add range for spotlights
			float attenuation = 0.0f;

			if (a < d)
				attenuation = 1.0f - pow(clamp(a / d, 0, 1), SpotLights[k].Falloff);

			diffuse += diffuseAmount * attenuation * SpotLights[k].DiffuseColor * SpotLights[k].Strength;

			// specular stuff
			specular += pow(saturate(dot(refl, view)), max(ObjectMaterial.SpecularPower, 0.0001f)) * 
				ObjectMaterial.SpecularColor * attenuation * SpotLights[k].Strength * specularFactor;
		}

		lighting = color.xyz * ObjectMaterial.DiffuseColor.xyz * saturate(ambient + diffuse) + saturate(specular);

		float3 output = saturate(lighting);
		ps_out.OutColor = float4(output, ObjectMaterial.DiffuseColor.a);
	}
	else
		ps_out.OutColor = color * ObjectMaterial.DiffuseColor;

	return ps_out;
}

technique11 MainTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Main()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Main()));
	}
}

struct VS_Output_SM
{
	float4 PosH : SV_POSITION;
};

VS_Output_SM VS_Main_SM(VS_Input inputVert)
{
	VS_Output_SM retVert = (VS_Output_SM)0;

	float4 worldPosition = mul(inputVert.PosL, WorldMatrix);
	retVert.PosH = mul(worldPosition, LightViewProjectionMatrix);

	return retVert;
}

RasterizerState Depth
{
	// If the depth buffer currently bound to the output-merger stage
	// has a UNORM format or no depth buffer is bound the bias value
	// is calculated like this:
	//
	// Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
	//
	// where r is the minimum representable value > 0 in the
	// depth-buffer format converted to float32.
	// [/End MSDN]
	//
	// For a 24-bit depth buffer, r = 1 / 2^24.
	//
	// Example: DepthBias = 100000 ==> Actual DepthBias = 100000/2^24 = .006
	// These values are highly scene dependent, and you will need
	// to experiment with these values for your scene to find the
	// best values.

	DepthBias = 100000;
	DepthBiasClamp = 0.0f;
	SlopeScaledDepthBias = 1.0f;
};

technique11 ShadowPassTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Main_SM()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);

		SetRasterizerState(Depth);
	}
}