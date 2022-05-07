#include "FBX.hlsli"

Texture2D<float4> toonMap : register(t3);  //toon�V�F�[�_�[�p�̃e�N�X�`���[
Texture2D<float4> albedoTexture : register(t0);  //���f���̃e�N�X�`���[
SamplerState smp : register(s0);      //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp2 : register(s1);      //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcolor = albedoTexture.Sample(smp, input.uv);
	float4 shadecolor = { 0,0,0,0 };
	float4 tooncolor = { 0,0,0,0 };

	// ���s����
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (!dirLights[i].active)
		{
			continue;
		}
		//Lambert����
		float3 light = normalize(dirLights[i].lightv); //���C�g�̌���
		float diffuse = saturate(dot(light, input.normal));
		float brightness = diffuse + 0.3f;

		brightness = saturate(brightness);

		// �S�ĉ��Z����
		tooncolor = toonMap.Sample(smp2, float2(brightness, 0.0f));
		shadecolor += float4(brightness, brightness, brightness, 1.0f) * tooncolor;
	}

	return shadecolor * texcolor;
}