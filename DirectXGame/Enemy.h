#pragma once
#include "Entity.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "FollowCamera.h"
#include <math.h>
#include <vector>

class Enemy : public Entity {
public:
	Enemy();
	~Enemy();

	enum EnemyModels { Body };

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(FollowCamera* camera) override;

	inline float GetT() { return t; }
	inline Vector3 GetPos() { return worldTransform_.translation_; }
	inline void SetIsDead(bool isDead) { isDead_ = isDead; }
	inline bool GetIsDead() { return isDead_; }

private:
	float t = 0;

	Vector3 spownPos;
	Vector3 RePos;
	bool isDead_;
};
