#pragma once
#include<Source\State\State.h>

enum class PlayerState
{
	idle,//止まっている状態
	move,//動いている状態
	attack,//攻撃している状態
	damage,//ダメージ状態
	step,//ステップ状態
	slide,//スライダー状態
};

struct PlayerDesc
{
	int hp;

};