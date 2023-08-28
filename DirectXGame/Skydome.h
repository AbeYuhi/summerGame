#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "FollowCamera.h"

class Skydome
{
public: 
	Skydome();
	~Skydome();

	void Initialize(Model* model);

	void Update();

	void Draw(FollowCamera* camera);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

};

