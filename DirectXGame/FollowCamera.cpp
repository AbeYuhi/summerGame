#include "FollowCamera.h"
#include "Vector3_Math.hpp"

FollowCamera::FollowCamera() {}
FollowCamera::~FollowCamera() {}

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
	viewProjection_.translation_.y = 5.0f;

	//行列の更新
	viewProjection_.matView = Inverse(MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, viewProjection_.rotation_, viewProjection_.translation_));
	viewProjection_.TransferMatrix();
}

void FollowCamera::Update() {

	viewProjection_.rotation_.x = 1.0f / 4.0f * M_PI;

	if (target_) {
		Vector3 offset = {0.0f, 10.0f, -100.0f};

		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

		offset = TransformNormal(offset, rotateMatrix);
		
		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}

	//行列の更新
	viewProjection_.matView = Inverse(MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, viewProjection_.rotation_, viewProjection_.translation_));
	viewProjection_.TransferMatrix();
}