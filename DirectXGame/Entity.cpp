#include "Entity.h"
#include "GameScene.h"

Entity::Entity(){}
Entity::~Entity(){}

void Entity::Initialize(const std::vector<Model*>& models) {
	models_ = models;
	worldTransform_.Initialize();
}

void Entity::Update() {


	worldTransform_.UpdateMatrix();
}

void Entity::Draw(FollowCamera* camera) {
	for (auto& model : models_) {
		model->Draw(worldTransform_, camera->GetViewProjection());
	}
}