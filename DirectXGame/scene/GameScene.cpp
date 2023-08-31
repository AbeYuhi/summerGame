#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// OBJファイル読み込み
	skydomeModel_.reset(Model::CreateFromOBJ("Skydome", true));
	groundModel_.reset(Model::CreateFromOBJ("Ground", true));
	playerHeadModel_.reset(Model::CreateFromOBJ("PHead", true));
	playerBodyModel_.reset(Model::CreateFromOBJ("PBody", true));
	playerLeftArmModel_.reset(Model::CreateFromOBJ("PLeftArm", true));
	playerRightArmModel_.reset(Model::CreateFromOBJ("PRightArm", true));
	playerWeaponModel_.reset(Model::CreateFromOBJ("Hummer", true));
	enemyBodyModel_.reset(Model::CreateFromOBJ("Enemy", true));
	cubeModel_.reset(Model::Create());
	uint32_t gameTitleSprite = TextureManager::Load("GameTitle.png");
	uint32_t gameOverSprite = TextureManager::Load("GameOver.png");
	uint32_t gameClearSprite = TextureManager::Load("GameClear.png");
	uint32_t descriptionSprite = TextureManager::Load("description.png");

	gameTitleSprite_.reset(Sprite::Create(gameTitleSprite, {640, 360}, {1, 1, 1, 1}, {0.5, 0.5}));
	gameOverSprite_.reset(Sprite::Create(gameOverSprite, {640, 360}, {1, 1, 1, 1}, {0.5, 0.5}));
	gameClearSprite_.reset(Sprite::Create(gameClearSprite, {640, 360}, {1, 1, 1, 1}, {0.5, 0.5}));
	descriptionSprite_.reset(Sprite::Create(descriptionSprite, {640, 360}, {1, 1, 1, 1}, {0.5, 0.5}));

	cubeTransform_.Initialize();
	cubeTransform_.scale_.x = 5;
	cubeTransform_.scale_.y = 5;
	cubeTransform_.scale_.z = 5;
	cubeTransform_.translation_.y = 5;
	cubeTransform_.UpdateMatrix();

	TitleInitialize();
	
	gameStatus_ = Title;
}

void GameScene::TitleInitialize() {
	// カメラ関連
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// インスタンス化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());
}

void GameScene::GameInitialize() {
	enemys_.clear();

	// カメラ関連
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// インスタンス化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	player_ = std::make_unique<Player>();
	std::vector<Model*> playerModels = {
	    playerHeadModel_.get(), playerBodyModel_.get(), playerLeftArmModel_.get(),
	    playerRightArmModel_.get(), playerWeaponModel_.get()};
	player_->Initialize(playerModels);

	// セッター
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	frame_ = 0;
}

void GameScene::OverInitialize() {
	// カメラ関連
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// インスタンス化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

}

void GameScene::EndInitialize() {
	// カメラ関連
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// インスタンス化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

}

void GameScene::Update() {
	Input::GetInstance()->GetJoystickStatePrevious(0, joyState);

	switch (gameStatus_) {
	case GameScene::Title:
		TitleUpdate();
		break;
	case GameScene::InGame:
		InGameUpdate();
		break;
	case GameScene::GameOver:
		OverUpdate();
		break;
	case GameScene::GameClear:
		ClearUpdate();
		break;
	default:
		break;
	}

	preJoyState = joyState;
}

void GameScene::TitleUpdate() {
	if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)) {
		// ゲーム開始
		if (joyState.Gamepad.bRightTrigger && !preJoyState.Gamepad.bRightTrigger) {
			gameStatus_ = InGame;
			GameInitialize();
		}
	}
}

void GameScene::InGameUpdate() {
	frame_++;
	followCamera_->Update();

	skydome_->Update();
	ground_->Update();
	player_->Update();

	if (frame_ % 60 == 0) {
		PopEnemy();
	}

	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			return true;
		}
		return false;
	});

	for (auto enemy : enemys_) {
		enemy->Update();

		float length = Length({0, 0, 0}, enemy->GetPos());

		if (length <= 8) {
			OverInitialize();
			gameStatus_ = GameOver;
		}
	}

	if (frame_ >= 60 * 60) {
		EndInitialize();
		gameStatus_ = GameClear;
	}
}

void GameScene::PopEnemy() {
	Enemy* enemy = new Enemy();
	std::vector<Model*> enemyModels = {enemyBodyModel_.get()};
	enemy->Initialize(enemyModels);
	player_->SetEnemy(enemy);
	enemys_.push_back(enemy);
}

void GameScene::ClearUpdate() {
	if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)) {
		// ゲーム開始
		if (joyState.Gamepad.bRightTrigger && !preJoyState.Gamepad.bRightTrigger) {
			gameStatus_ = Title;
			TitleInitialize();
		}
	}
}

void GameScene::OverUpdate() {
	if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)) {
		// ゲーム開始
		if (joyState.Gamepad.bRightTrigger && !preJoyState.Gamepad.bRightTrigger) {
			gameStatus_ = Title;
			TitleInitialize();
		}
	}
}

void GameScene::Draw() {
	switch (gameStatus_) {
	case GameScene::Title:
		TitleDraw();
		break;
	case GameScene::InGame:
		InGameDraw();
		break;
	case GameScene::GameOver:
		OverDraw();
		break;
	case GameScene::GameClear:
		ClearDraw();
		break;
	default:
		break;
	}
}

void GameScene::TitleDraw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	ground_->Draw(followCamera_.get());
	skydome_->Draw(followCamera_.get());

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	gameTitleSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::InGameDraw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	ground_->Draw(followCamera_.get());
	skydome_->Draw(followCamera_.get());
	cubeModel_->Draw(cubeTransform_, followCamera_->GetViewProjection());
	player_->Draw(followCamera_.get());
	for (auto enemy : enemys_) {
		enemy->Draw(followCamera_.get());
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	descriptionSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::ClearDraw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	ground_->Draw(followCamera_.get());
	skydome_->Draw(followCamera_.get());

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	gameClearSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::OverDraw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	ground_->Draw(followCamera_.get());
	skydome_->Draw(followCamera_.get());

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	gameOverSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}