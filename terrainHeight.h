#pragma once
#include <cmath>

// 丘の凸凹用パラメータ
constexpr float kTerrainAmplitude1 = 1.2f;
constexpr float kTerrainAmplitude2 = 0.6f;
constexpr float kTerrainFreqX1 = 0.15f;
constexpr float kTerrainFreqZ1 = 0.2f;
constexpr float kTerrainFreqX2 = 0.35f;
constexpr float kTerrainFreqZ2 = 0.28f;

// 川用パラメータ
constexpr float kRiverWidth = 6.0f;   // 川の幅
constexpr float kRiverDepth = 2.5f;   // 川底の深さ
constexpr float kRiverBend = 15.0f;  // 蛇行の振れ幅
constexpr float kRiverBendFreq = 0.05f;  // 蛇行の周期
constexpr float kWaterLevel = -1.6f;  // 水面の高さ（ここは固定）

// 川の中心線のX座標（Z座標に応じて蛇行する）
inline float GetRiverCenterX(float z)
{
	return kRiverBend * sinf(z * kRiverBendFreq);
}

// 川を考慮しない、丘だけの高さ
inline float GetHillHeight(float x, float z)
{
	return kTerrainAmplitude1 * sinf(x * kTerrainFreqX1) * cosf(z * kTerrainFreqZ1)
		+ kTerrainAmplitude2 * sinf(x * kTerrainFreqX2 + z * kTerrainFreqZ2);
}

// ワールド座標(x, z)における地形の高さ(Y)。川の部分は掘り下げる
inline float GetTerrainHeight(float x, float z)
{
	float hill = GetHillHeight(x, z);

	float dist = fabsf(x - GetRiverCenterX(z));
	float t = dist / kRiverWidth;
	if (t > 1.0f) t = 1.0f;
	float smooth = t * t * (3.0f - 2.0f * t); // 中心=0, 縁=1でなめらかに変化
	float riverFactor = 1.0f - smooth;        // 中心=1, 縁=0

	return hill - kRiverDepth * riverFactor;
}