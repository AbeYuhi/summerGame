#pragma once
#include <math.h>
#include <vector>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "FollowCamera.h"
#define M_PI 3.14f

class Entity
{
public:
	Entity();
	~Entity();

	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(FollowCamera* camera);

public: //ゲッターセッター
	inline WorldTransform& GetWorldTransform() { return worldTransform_; }
	inline void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

protected:
	const ViewProjection* viewProjection_;
	WorldTransform worldTransform_;
	std::vector<Model*> models_;
	const int32_t* frame_;
};

