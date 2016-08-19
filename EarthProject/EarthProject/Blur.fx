#define RADIUS 7
#define KERNAL (RADIUS * 2 + 1) 

cbuffer cbPerObject
{
	float4x4 World;
	float4x4 View;
	float4x4 Proj;
};

float weights[KERNAL];
float oX[KERNAL];
float oY[KERNAL];

Texture2D Tex;

SamplerState TextureSampler
{
	Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
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
	Output.Pos = mul(Output.Pos, Proj);

	
	Output.Tex = vsIn.Tex;

	return Output;

}

float4 PSW(PS_IN psIn) : SV_TARGET
{
	float4 finCol = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	for(int i = 0; i < KERNAL; ++i)
	{
		finCol += Tex.Sample(TextureSampler, psIn.Tex + float2(oX[i], oY[i]) ) * weights[i];	
	} 
	
	
	float4 overlay = Tex.Sample(TextureSampler, psIn.Tex);
	//creates smooth bloom effect
	return  (finCol + overlay)*.83f;
}



float4 PS2(PS_IN psIn) : SV_TARGET
{
	float4 finCol = float4(0.0f, 0.0f, 0.0f, 1.0f);	
	for(int i = 0; i < KERNAL; ++i)
	{
		finCol += Tex.Sample(TextureSampler, psIn.Tex + float2(oX[i], oY[i]) ) * weights[i];			
	} 
	
	return finCol;
}


technique10 Blur
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, VS() ) );
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSW() ) );
		
	}
	pass pass1
	{
		SetVertexShader(CompileShader(vs_4_0, VS() ) );
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSW() ) );
		
	}



}


technique10 JustBlur
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, VS() ) );
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS2() ) );
		
	}
	pass pass1
	{
		SetVertexShader(CompileShader(vs_4_0, VS() ) );
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS2() ) );
		
	}



}