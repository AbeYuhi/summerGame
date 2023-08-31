#include "Enemy.h"
#include "Vector3_Math.hpp"
#include "GameScene.h"

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(const std::vector<Model*>& models) {
	Entity::Initialize(models);

	worldTransform_.translation_.z = 50;
	worldTransform_.translation_.x = 50;
	//int rotattion = rand() % 360;


	spownPos = worldTransform_.translation_;
	t = 0;
	isDead_ = false;
}

void Enemy::Update() {
	t += 0.005f;
	// スピード
	const float speed = 0.2f;
	// 移動量
	Vector3 pos = Leap(spownPos, {0, 0, 0}, t);
	Vector3 move = pos - RePos;
	// 移動量を速さに反映
	move = Normalize(move) * speed;

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