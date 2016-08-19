 struct Light
{
	float3 pos;
	float3 dir;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 att;
	float  spotPower;
	float  range;
};
cbuffer cbPerFrame
{	
	float3 gEyePosW;
	Light gLight;
	float gCloudRot;
	
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP;
};

Texture2D EarthTex;
Texture2D EarthMapTex;
Texture2D EarthMaskTex;
Texture2D EarthAtmos;
Texture2D EarthBumpTex;
Texture2D EarthCloudTex;
Texture2D EarthLightTex;


cbuffer ScatteringParameters {

	uniform int nSamples = 2;
	uniform float fSamples = 2.0f;	

	uniform float3 InvWaveLength = {5.602, 9.478, 19.646};
	uniform float OuterRadius =100.0f*1.125;
	uniform float OuterRadiusSquared = 100.0f*1.125* 100.0f *1.125f;

	uniform float InnerRadius =  100.0f;
	uniform float InnerRadiusSquared =  100.0f* 100.0f;

	uniform float KrESun = .0375;
	uniform float KmESun = .0225;
	uniform float Kr4PI = .0314;
	uniform float Km4PI = .0188;
	uniform float Scale = .08f;
	uniform float ScaleOverScaleDepth = 0.22f;
	uniform float InvScaleDepth = 4;
	uniform float ScaleDepth = .25;
	uniform float G = -.98;
	uniform float GSquared = -.98*-.98;
};
// float OutRadius;
// float InnerRadius;
// float KrSun;
// float Kr4Pi;
// float KmSun;
// float Km4Pi;
// float Scale;
// float ScaleDepth;
// float ScaleOverScaleDepth;
// float Samples;
// float G;
// float G2;
// float3 InvWaveLength;
 SamplerState linearSampler
 {
  	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap; 
};

SamplerState BumpSample
{
	Filter = MIN_MAG_MIP_POINT; 
};




float scale(float fCos)
{
	float x = 1.0 - fCos;
	return ScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}
float getRayleighPhase(float fCos2)
{
	//return 1.0f;
	return 0.75 * (1.0f+fCos2);
}
float getNearIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B - sqrt(fDet));
}

struct VS_IN
{
	float3 posL    : POSITION;
	float3 normalL : NORMAL;
	float4 diffuse : DIFFUSE;
	float4 spec    : SPECULAR;
	float2 texCoord: TEXCOORD;
};

struct VS_OUT
{
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
    float4 diffuse : DIFFUSE;
    float4 spec    : SPECULAR;
	float4 texCoord: TEXCOORD;
	float4 col: TEXCOORD1;
	float4 col2: TEXCOORD2;
	float4 texC: TEXCOORD3;
};


float magnitude(float3 vec)
{
	return sqrt((vec.x*vec.x) + (vec.y * vec.y) + (vec.z*vec.z));
}
VS_OUT VS(VS_IN vIn)
{
	float camHeight = magnitude(gEyePosW);
	float camHeight2 = camHeight*camHeight;
	//float outRadius2 = OutRadius*OutRadius;

	float3 lightDirection = (gLight.pos/ magnitude(gLight.pos));
	float3 position = vIn.posL ;
	
	float3 ray = position - gEyePosW;
	float far = length(ray);
	ray /= far;

	float near = getNearIntersection(gEyePosW.xyz, ray.xyz, camHeight2, OuterRadiusSquared);
	
  
	float3 rayStart = gEyePosW +ray * near ;
	 far -= near;

	 float startDepth = exp((InnerRadius - OuterRadius) / ScaleDepth);
	 float cameraAngle = dot(-ray, position) / length(position);
	 float lightAngle= dot(lightDirection, position) / length(position);
	 float cameraScale = scale(cameraAngle);
	 float lightScale = scale(lightAngle);
	 float camOffset = startDepth * cameraScale;
	 
	 float sampleLength = far / fSamples;
	 float scaledLength = sampleLength * Scale;
	 float3 sampleRay = ray * sampleLength;
	 float3 samplePoint = rayStart + sampleRay * 0.5f;
	 float temp = lightScale + cameraScale;
	 float3 frontColor = float3(0.0f, 0.0f, 0.0f);
	 float3 attenuate;
	 for(int i = 0; i < fSamples; i++) 
	 {
		float height = length(samplePoint);
		float depth = exp(ScaleOverScaleDepth * ( InnerRadius - height ));		
		float scatter = depth * temp - camOffset;
		attenuate = exp(-scatter * (InvWaveLength * Kr4PI + Km4PI));		
		frontColor += attenuate * (depth * scaledLength);		
		samplePoint += sampleRay;
	}

	VS_OUT vOut;
	
	// Transform to world space space.
	vOut.posW    = mul(float4(vIn.posL, 1.0f), gWVP);
	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);

	// Output vertex attributes for interpolation across triangle.
	vOut.diffuse = vIn.diffuse;
	vOut.spec    = vIn.spec;
	vOut.texCoord = float4(vIn.texCoord.x, vIn.texCoord.y, 1, 1);
	vOut.col =  float4(frontColor * (float3(InvWaveLength.x,InvWaveLength.y +.1, InvWaveLength.z -3) *KrESun + KmESun), 1);	
	vOut.col2 =  float4(attenuate, 1);
	vOut.texC =  float4(gEyePosW - position, 1);
	return vOut; 
}
VS_OUT VS2(VS_IN vIn)
{
	float camHeight = magnitude(gEyePosW);
	float camHeight2 = camHeight*camHeight;
	//float outRadius2 = OutRadius*OutRadius;

	float3 lightDirection = (gLight.pos/ magnitude(gLight.pos));
	float3 position = vIn.posL ;
	
	float3 ray = position - gEyePosW;
	float far = length(ray);
	ray /= far;

	float near = getNearIntersection(gEyePosW.xyz, ray.xyz, camHeight2, OuterRadiusSquared);
	
  
	float3 rayStart = gEyePosW +ray * near ;
	 far -= near;

	 float startDepth = exp((InnerRadius - OuterRadius) / ScaleDepth);
	 float cameraAngle = dot(-ray, position) / length(position);
	 float lightAngle= dot(lightDirection, position) / length(position);
	 float cameraScale = scale(cameraAngle);
	 float lightScale = scale(lightAngle);
	 float camOffset = startDepth * cameraScale;
	 
	 float sampleLength = far / fSamples;
	 float scaledLength = sampleLength * Scale;
	 float3 sampleRay = ray * sampleLength;
	 float3 samplePoint = rayStart + sampleRay * 0.5f;
	 float temp = lightScale + cameraScale;
	 float3 frontColor = float3(0.0f, 0.0f, 0.0f);
	 float3 attenuate;
	 for(int i = 0; i < fSamples; i++) 
	 {
		float height = length(samplePoint);
		float depth = exp(ScaleOverScaleDepth * ( InnerRadius - height ));		
		float scatter = depth * temp - camOffset;
		attenuate = exp(-scatter * (InvWaveLength * Kr4PI + Km4PI));		
		frontColor += attenuate * (depth * scaledLength);		
		samplePoint += sampleRay;
	}

	VS_OUT vOut;
	
	// Transform to world space space.
	vOut.posW    = mul(float4(vIn.posL+gLight.pos, 1.0f), gWVP);
	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL+gLight.pos, 1.0f), gWVP);

	// Output vertex attributes for interpolation across triangle.
	vOut.diffuse = vIn.diffuse;
	vOut.spec    = vIn.spec;
	vOut.texCoord = float4(vIn.texCoord.x, vIn.texCoord.y, 1, 1);
	vOut.col =  float4(frontColor * (float3(InvWaveLength.x,InvWaveLength.y +.1, InvWaveLength.z -3) *KrESun + KmESun), 1);	
	vOut.col2 =  float4(attenuate, 1);
	vOut.texC =  float4(gEyePosW - position, 1);
	return vOut; 
} 
float4 PS(VS_OUT pIn) : SV_Target
{
	// Interpolating normal can make it not be of unit length so normalize it.
    pIn.normalW = normalize(pIn.normalW);
   
   
  
	float2 bmpTex = float2(pIn.texCoord.x + sin(pIn.texCoord.y*20+gCloudRot)/50, pIn.texCoord.y + cos(pIn.texCoord.x * 20 + gCloudRot) /50);
    
	float4 Bump = EarthBumpTex.Sample(BumpSample, pIn.texCoord  );
	float4 mainTex = EarthTex.Sample(linearSampler, pIn.texCoord );
	float4 AtmosTex = EarthAtmos.Sample(linearSampler, pIn.texCoord);
	float4 mapTex = EarthMapTex.Sample(linearSampler, pIn.texCoord);
	float4 mask = EarthMaskTex.Sample(BumpSample, pIn.texCoord  );

	float4 newMap = mapTex * mask;
	float4 LightTex = EarthLightTex.Sample(linearSampler, pIn.texCoord);
	//cos(LightTex.y*0.1f + (gCloudRot*20) ) *100;

	float2 cloudTexCoord;
	if(gCloudRot > 0)
	cloudTexCoord = float2(pIn.texCoord.x + gCloudRot/100, bmpTex.y);
	else
	cloudTexCoord = float2(pIn.texCoord.x,pIn.texCoord.y);

	

	float4 CloudTex = EarthCloudTex.Sample(linearSampler, cloudTexCoord);
	
	float3 norm = normalize(pIn.normalW);
	Bump.a *= 256; //upscale the Bump.a to usuable value

    
	float3 cam = normalize(gEyePosW);
	//float4 rim = pow(1- dot( norm, cam ),3.0f) * float4(0.3, 0.3f, 1,1.0f) *1.10f;
	//float4 rim = pow(1- dot( norm, cam ),1.5f) * float4(InvWaveLength,1) *1.3f;

	
	float4 finalColor =  ( ((AtmosTex)*(mainTex ))+(newMap) )+ (CloudTex) + (LightTex) ;

	float3 lightDirection = normalize(gLight.pos);
	float fCos = dot(lightDirection, pIn.texC) / length(pIn.texC);
	float fCos2 = fCos * fCos;
	float4 finCol = (getRayleighPhase(fCos2) * pIn.col) + (getMiePhase(fCos, fCos2, G, GSquared)* pIn.col2 );
	
	//finCol=  float4(aS.col, 1) + getMiePhase(fCos, fCos2, G, GSquared) * float4(aS.col2, 1);
	finCol +=.2;
    return  (pIn.col + finalColor * pIn.col2) ;
}

float4 PS2(VS_OUT pIn) : SV_Target
{
    return  float4(1, 1, 1, 1) ;
}


technique10 EarthTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );		
    }
	

}


struct V
{
	float3 pos : POS;
};

struct AtmosScatter
{	
	float4 pos : SV_POSITION;
	float3 col : COLOR0;
	float3 col2 : COLOR1;
	float3 texC : TEX;
};

AtmosScatter  VSAtmos(float3 v : POS, uniform float inScale)
{
	float camHeight = magnitude(gEyePosW);
	float camHeight2 = camHeight*camHeight;
	//float outRadius2 = OutRadius*OutRadius;

	float3 lightDirection = gLight.pos/magnitude(gLight.pos);
	float3 position = v ;
	float3 ray = position - gEyePosW;
	float far = length(ray);
	ray /= far;
	  
	float3 rayStart = gEyePosW ;
	 float fheight = length(gEyePosW);	
	 float startDepth = exp (ScaleOverScaleDepth * (InnerRadius - camHeight));
	 float startAngle = dot(ray, rayStart) / camHeight;
	
	 float startOffset = startDepth * scale(startAngle);
	 
	 float sampleLength = far / fSamples;
	 float scaledLength = sampleLength * Scale;
	 float3 sampleRay = ray * sampleLength;
	 float3 samplePoint = rayStart + sampleRay * 0.5f;
	
	 float3 frontColor = float3(0.0f, 0.0f, 0.0f);
	 
	 for(int i = 0; i < nSamples; i++) 
	 {
		float height = length(samplePoint);
		float depth = exp(ScaleOverScaleDepth * ( InnerRadius - height ));
		float lightAngle = dot(lightDirection, samplePoint) / height;
		float cameraAngle = dot(ray, samplePoint) / height;
		float scatter = startOffset + depth * (scale(lightAngle) - scale(cameraAngle));
		float3 attenuate = exp(-scatter * ((InvWaveLength * Kr4PI + Km4PI)));
		
		frontColor += attenuate * (depth * scaledLength);
		
		samplePoint += sampleRay;
	}

	AtmosScatter output;
	output.pos = mul(float4(v, 1.0f), gWVP );
	if(camHeight > OuterRadius+10)
	{
		output.col = float4(0, 0, 0, 1);
		output.col2= float4(0, 0, 0, 1);
		output.texC= float3(0, 0, 0);
	}
	else
	{
		output.col = frontColor * (InvWaveLength *KrESun);	
		output.col2= frontColor* KmESun;
		output.texC= gEyePosW - v;
	}
	

	return output;

}

float4 PSAtmos(AtmosScatter aS) : SV_TARGET
{
	float3 lightDirection = gLight.pos/ magnitude(gLight.pos);
	float fCos = dot(lightDirection, aS.texC) / length(aS.texC);
	float fCos2 = fCos * fCos;
	float4 finCol = float4((getRayleighPhase(fCos2) * aS.col) + (getMiePhase(fCos, fCos2, G, GSquared)* aS.col2 ), 1);
	float4 me = float4(aS.col + getMiePhase(fCos, fCos2, G, GSquared)* aS.col2, 1);
	//finCol=  float4(aS.col, 1) + getMiePhase(fCos, fCos2, G, GSquared) * float4(aS.col2, 1);
	return  1-exp(-2 * me) ; 

}


AtmosScatter  SkyFromAtmos(float3 v : POS, uniform float inScale)
{
	float camHeight = magnitude(gEyePosW);
	float camHeight2 = camHeight*camHeight;
	//float outRadius2 = OutRadius*OutRadius;

	float3 lightDirection = gLight.pos/magnitude(gLight.pos);
	float3 position = v ;
	float3 ray = position - gEyePosW;
	float far = length(ray);
	ray /= far;
	
	

	float3 rayStart = gEyePosW;
	
	//float startAngle = dot(ray, rayStart) / height;
	float startDepth = exp (InnerRadius -camHeight) / ScaleDepth;
	float cameraAngle = dot(-ray, position) / length(position);
	float lightAngle = dot(lightDirection, position) / length(position);
	float cameraScale = scale(cameraAngle);
	float lightScale = scale(lightAngle);
	float cameraOffset = startDepth*cameraScale;
	 float temp = (lightScale + cameraScale);
	 
	 float sampleLength = far / fSamples;
	 float scaledLength = sampleLength * Scale;
	 float3 sampleRay = ray * sampleLength;
	 float3 samplePoint = rayStart + sampleRay * 0.5f;
	
	 float3 frontColor = float3(0.0f, 0.0f, 0.0f);
	 float3 attenuate;
	 for(int i = 0; i < nSamples; i++) 
	 {
		float height = length(samplePoint);
		float depth = exp(ScaleOverScaleDepth * ( InnerRadius - height ));		
		float scatter = depth * temp - cameraOffset;
		attenuate = exp(-scatter * ((InvWaveLength * Kr4PI + Km4PI)));
		
		frontColor += attenuate * (depth * scaledLength);
		
		samplePoint += sampleRay;
	}

	AtmosScatter output;
	output.pos = mul(float4(v*inScale, 1.0f), gWVP );
	
	output.col = frontColor * (InvWaveLength *KrESun + KmESun);	
	output.col2= float4(attenuate, 1);
	output.texC= gEyePosW - v;

	return output;

}

float4 SkyFromAtmosPs(AtmosScatter aS) : SV_TARGET
{
	float3 lightDirection = gLight.pos / magnitude(gLight.pos);
	float fCos = dot(lightDirection, aS.texC) / length(aS.texC);
	float fCos2 = fCos * fCos;
	float4 finCol = float4((getRayleighPhase(fCos2) * aS.col) + (getMiePhase(fCos, fCos2, G, GSquared)* aS.col2 ), 1);
	float4 me = float4(aS.col + getMiePhase(fCos, fCos2, G, GSquared) * aS.col2, 1);
	//finCol=  float4(aS.col, 1) + getMiePhase(fCos, fCos2, G, GSquared) * float4(aS.col2, 1);
	
	float3 col = float3(135,206,235).xyz / 255;
	//col.b *= 5;
	//col.rg *= 2;
	return float4(aS.col + 0.15 * aS.col2 *col , 0.100f) ; 

}

technique10 AtmosTech
{

	 pass P0
    {
       SetVertexShader( CompileShader( vs_4_0, SkyFromAtmos(0.89f) ) );
       SetGeometryShader( NULL );
       SetPixelShader( CompileShader( ps_4_0, SkyFromAtmosPs() ) );
	
    }
	 pass P1
    {
       SetVertexShader( CompileShader( vs_4_0, VSAtmos(2) ) );
       SetGeometryShader( NULL );
       SetPixelShader( CompileShader( ps_4_0, PSAtmos() ) );
	
    }
	
    
 

}

