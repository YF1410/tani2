#include "FBX.hlsli"

Texture2D<float4> baseTex : register(t0); //0番スロットに設定されたテクスチャ
Texture2D<float4> metalnessTex : register(t1); //1番スロットに設定されたテクスチャ
Texture2D<float4> normalTex : register(t2); //2番スロットに設定されたテクスチャ
Texture2D<float4> roughnessTex : register(t3); //3番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      //0番スロットに設定されたサンプラー

//円周率
static const float PI = 3.141592654f;
//反射点の法線ベクトル
static float3 N;
//アルベド
static float3 s_baseColor;
//金属度
static float s_metalness;
//粗さ
static float s_roughness;

//Schlickによる近似
//f0とf90の値を(1 - cosine)の5乗でlerpする
//f0 : 光が垂直に入射したときの反射率
//f90 : 光が平行に入射したときの反射率
//cosine : 2ベクトルのなす角のコサイン(内積値)
float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

//UE4のGGX分布
//alpha : roughnessの2乗
//NdotH : 法線とハーフベクトルの内積
float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

//ディズニーのフレネル計算
float3 DisneyFresnel(float LdotH)
{
	//F項(フレネル:Fresnel)
	//輝度
	float luminance = 0.3f * s_baseColor.r + 0.6f * s_baseColor.g + 0.1f * s_baseColor.b;
	//色合い
	float3 tintColor = s_baseColor / luminance;
	//非金属の鏡面反射色の計算
	float3 nonMetalColor = specular * 0.08f * tintColor;
	//metalnessによる色補間 金属の場合はベースカラー
	float3 specularColor = lerp(nonMetalColor, s_baseColor, s_metalness);
	//NdotHの割合でSchlickFresnel補間
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

//UE4のSmithモデル
float GeometricSmith(float cosine)
{
	float k = (s_roughness + 1.0f);

	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}

//鏡面反射の計算
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	//D項(分布:Distribution)
	float Ds = DistributionGGX(s_roughness * s_roughness, NdotH);

	//F項(フレネル:Fresnel)
	float3 Fs = DisneyFresnel(LdotH);

	//G項(幾何減衰:Geometry attenuation)
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	//m項(分母)
	float m = 4.0f * NdotL * NdotV;

	//合成して鏡面反射の色を得る
	return Ds * Fs * Gs / m;
}

float3 BRDF(float3 L, float3 V)
{
	//法線とライト方向の内積
	float NdotL = dot(N, L);
	//法線とカメラ方向の内積
	float NdotV = dot(N, V);
	//ライト方向とカメラ方向の中間(ハーフベクトル)
	float3 H = normalize(L + V);
	//法線とハーフベクトルの内積
	float NdotH = dot(N, H);
	//ライトとハーフベクトルの内積
	float LdotH = dot(L, H);

	//どちらかが90度以上であれば真っ黒を返す
	if (NdotL < 0.0f || NdotV < 0.0f)
	{
		return float3(0, 0, 0);
	}
	//拡散反射率
	float diffuseReflectance = 1.0f / PI;

	//入射角が90度の場合の拡散反射率
	float energyBias = 0.5f * s_roughness;
	float Fd90 = energyBias + 2.0f * LdotH * LdotH * s_roughness;
	//入っていく時の拡散反射率
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	//出ていく時の拡散反射率
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	//入って出ていくまでの拡散反射率
	float energyFacter = lerp(1.0f, 1.0f / 1.51f, s_roughness);
	float Fd = FL * FV * energyFacter;

	//拡散反射項
	float3 diffuseColor = diffuseReflectance * Fd * s_baseColor * (1 - s_metalness);

	//鏡面反射項
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH);

	//拡散反射と鏡面反射の合計で色が決まる
	return diffuseColor + specularColor;
}

float4 main(VSOutput input) : SV_TARGET
{
	//面の情報をstatic変数に代入し、関数から参照出来るようにする
	N = input.normal;
	//パラメータをテクスチャのRGBから取得
	s_baseColor = baseColor + baseTex.Sample(smp, input.uv).rgb;
	s_metalness = metalness + metalnessTex.Sample(smp, input.uv).r;
	s_roughness = roughness + roughnessTex.Sample(smp, input.uv).r;
	//最終的なRGB
	float3 finalRGB = float3(0, 0, 0);
	//頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	
	//平行光源
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (!dirLights[i].active)
		{
			continue;
		}
		//BRDFの結果とライト色の合成
		finalRGB += BRDF(dirLights[i].lightv, eyedir) * dirLights[i].lightcolor;
	}
	
	return float4(finalRGB, 1);
}