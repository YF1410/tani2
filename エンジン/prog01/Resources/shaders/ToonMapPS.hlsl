#include "FBX.hlsli"

Texture2D<float4> toonMap : register(t3);  //toonシェーダー用のテクスチャー
Texture2D<float4> albedoTexture : register(t0);  //モデルのテクスチャー
SamplerState smp : register(s0);      //0番スロットに設定されたサンプラー
SamplerState smp2 : register(s1);      //0番スロットに設定されたサンプラー


float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcolor = albedoTexture.Sample(smp, input.uv);
	float4 shadecolor = { 0,0,0,0 };
	float4 tooncolor = { 0,0,0,0 };

	// 平行光源
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (!dirLights[i].active)
		{
			continue;
		}
		//Lambert反射
		float3 light = normalize(dirLights[i].lightv); //ライトの向き
		float diffuse = saturate(dot(light, input.normal));
		float brightness = diffuse + 0.3f;

		brightness = saturate(brightness);

		// 全て加算する
		tooncolor = toonMap.Sample(smp2, float2(brightness, 0.0f));
		shadecolor += float4(brightness, brightness, brightness, 1.0f) * tooncolor;
	}

	return shadecolor * texcolor;
}