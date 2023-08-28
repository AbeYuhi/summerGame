#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#define M_PI 3.14f

class FollowCamera
{
public:
	FollowCamera();
	~FollowCamera();

	void Initialize();

	void Update();

public: //ゲッターセッター
	inline void SetTarget(const WorldTransform* target) { target_ = target; }
	inline ViewProjection &GetViewProjection() { return viewProjection_; }
private:
	ViewProjection viewProjection_;
	//追従対象
	const WorldTransform* target_ = nullptr;
};
