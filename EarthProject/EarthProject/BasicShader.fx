
cbuffer cbPerObject
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};


struct VS_IN
{
	float3 PosL: POSITION0;
	float2 Tex : TEXCOORD0;	
	float3 Normal: NORMAL0;
};

struct PS_IN
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal: NORMAL0;	
};

Texture2D Test;

SamplerState TextureSampler
{
	Filter = ANISOTROPIC;
    AddressU = Clamp;
    AddressV = Clamp;
};

PS_IN VS(VS_IN vsIn)
{
	PS_IN output;
		
	float4x4 mat = mul(view, proj);
	float4x4 wvp = mul(world, mat);

	output.PosH = mul(float4(vsIn.PosL, 1.0f), wvp);
	output.PosW = mul(float4(vsIn.PosL, 1.0f), wvp);
	

	output.Tex = vsIn.Tex;
	output.Normal = mul(float4(vsIn.Normal, 1.0f), world);
	
	return output;
}

float4 PS(PS_IN psOut) : SV_TARGET
{
	psOut.Normal = normalize(psOut.Normal);
	float4 lightCol = float4(0.1f, 0.1f, 0.1f, 1);
	float4 amb = float4(.4, .4, .4, .5);
	float lightInt = 0;
	float3 lightDir = float3(0, 0,-1 ) ;
	float3 lightDir1 = float3(0, 1, 0);
	float3 lightDir2 = float3(-1, 0, 0);
	float3 lightDir3 = float3(0, 0, -1);
	lightInt = dot(psOut.Normal, normalize(-lightDir));
	float power = log10(exp2(lightInt));
	float4 rim = pow(1- dot( psOut.Normal, lightDir ),2.750f) * normalize(float4(0.3, 0.4f, 0.4,1.0f) *1.10f);
	float4 a =  (lightCol+amb) * lightInt ;
	
	float4 t = Test.Sample(TextureSampler,psOut.Tex);	
	
	
	return  t*lightInt ;
}



technique10 BasicShader
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));  
			    
	}
}