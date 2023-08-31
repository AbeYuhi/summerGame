#include "Enemy.h"
#include "Vector3_Math.hpp"
#include "GameScene.h"

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(const std::vector<Model*>& models) {
	Entity::Initialize(models);
	Vector3 center = {0, 0, 75};
	float rotation = float(rand() % 614);
	rotation /= 100;

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(0.0f);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotation);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(0.0f);

	Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(rotateXMatrix, rotateYMatrix, rotateZMatrix);

	center = Transform(center, rotateMatrix);
	worldTransform_.translation_ += center;

	spownPos = worldTransform_.translation_;
	t = 0;
	isDead_ = false;
}

void Enemy::Update() {
	t += 0.004f;
	// 移動量
	Vector3 pos = Leap(spownPos, {0, 0, 0}, t);
	Vector3 move = pos - RePos;
	// 移動量を速さに反映
	move = Normalize(move);

	// 回転
	worldTransform_.rotation_.y = atan2f(move.x, move.z);

	// 移動
	worldTransform_.translation_ = pos;

	worldTransform_.UpdateMatrix();

	RePos = move;
}

void Enemy::Draw(FollowCamera* camera) {
	this->models_[EnemyModels::Body]->Draw(worldTransform_, camera->GetViewProjection());
}