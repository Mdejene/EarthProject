


cbuffer cbPerObject
{
	float4x4 World;
	float4x4 View;
	float4x4 OrthoProj;
	float DT;
};


Texture2D TEX;
SamplerState TextureSampler
{
	Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};


struct VS_IN2
{
	float3 PosL: POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_IN2
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float2 Tex : TEXCOORD0;
};



struct VS_IN
{
	float4 Pos: POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

PS_IN VS(VS_IN vsIn)
{
	
	PS_IN Output;
	vsIn.Pos.w = 1.0f;

	Output.Pos = mul(vsIn.Pos, World);
	Output.Pos = mul(Output.Pos, View);
	Output.Pos = mul(Output.Pos, OrthoProj);

	
	Output.Tex = vsIn.Tex;

	return Output;

}

float4 PS(PS_IN psIn) : SV_TARGET
{
	

	float4 finCol = TEX.Sample(TextureSampler, psIn.Tex);
	
	return finCol;
}

PS_IN2 VS2(VS_IN2 vsIn)
{
	
	PS_IN2 output;
	
	float4x4 a = mul(View, OrthoProj);
	float4x4 newA = mul(World, a);
	output.PosH = mul(float4(vsIn.PosL, 1.0f), newA);
	output.PosW = mul(float4(vsIn.PosL, 1.0f), newA);
	output.Tex = vsIn.Tex;
	
	return output;

}

float4 PS2(PS_IN2 psIn) : SV_TARGET
{
	

	float4 finCol = float4(0, 0, 0, 1);
	
	
   finCol = float4(0, 1, 0, 1);
	


	return finCol;
}


technique10 RenderTexture
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));  
		    
	}
}

technique10 NormalView
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, VS2()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS2()));  
		  
	}
}