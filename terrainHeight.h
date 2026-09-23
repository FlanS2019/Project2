#pragma once
#include <cmath>

constexpr float kTerrainAmplitude1 = 1.2f;
constexpr float kTerrainAmplitude2 = 0.6f;
constexpr float kTerrainFreqX1 = 0.15f;
constexpr float kTerrainFreqZ1 = 0.2f;
constexpr float kTerrainFreqX2 = 0.35f;
constexpr float kTerrainFreqZ2 = 0.28f;

// ワールド座標(x, z)における地形の高さ(Y)を返す
// 地面のメッシュ作成と、キャラクター/オブジェクトの高さ合わせの両方から使う共通関数
inline float GetTerrainHeight(float x, float z)
{
	return kTerrainAmplitude1 * sinf(x * kTerrainFreqX1) * cosf(z * kTerrainFreqZ1)
		+ kTerrainAmplitude2 * sinf(x * kTerrainFreqX2 + z * kTerrainFreqZ2);
}