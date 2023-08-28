#pragma once
#include <math.h>
#include <vector>
#include <optional>
#include "WinApp.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "Entity.h"
#include "GlobalVariables.h"
#include "FollowCamera.h"
#define M_PI 3.14f

class Player : public Entity
{
public:
	Player() = default;
	~Player() = default;

	void Initialize(const std::vector<Model*>& models) override;

	void ApplyGlobalVariables();

	void Update() override;

	void Draw(FollowCamera* camera) override;

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

	void InitializeAttackGimmick();
	void UpdateAttackGimmick();

	void BehaviorRootUpdata();

	void BehaviorRootInitialize();

	void BehaviorAttackUpdata();

	void BehaviorAttackInitialize();

	enum PlayerModelIndex {
		kModelIndexHead,
		kModelIndexBody,
		kModelIndexLArm,
		kModelIndexRArm,
		kModelIndexWeapon,
	};

	enum class Behavior {
		kRoot,
		kAttack
	};

private:
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformLeftArm_;
	WorldTransform worldTransformRightArm_;
	WorldTransform worldTransformWeapon_;

	//浮遊ギミックの媒介変数
	int cycle_;
	float floatingParameter_;
	float swingWidth_;

	//攻撃ギミックの媒介変数
	int attackCycle_;
	float attackParameter_;

	//
	Vector3 keepMove_;

	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = Behavior::kRoot;
};

