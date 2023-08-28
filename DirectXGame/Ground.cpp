#include "Ground.h"

Ground::Ground() {}
Ground::~Ground() {}

void Ground::Initialize(Model* model) {
	worldTransform_.Initialize();
	model_ = model;

	worldTransform_.UpdateMatrix();
}

void Ground::Update() {

}

void Ground::Draw(FollowCamera* camera) {
	model_->Draw(worldTransform_, camera->GetViewProjection());
}