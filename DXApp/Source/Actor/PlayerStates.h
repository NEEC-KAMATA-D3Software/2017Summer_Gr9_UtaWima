#pragma once
#include<Source\State\State.h>

enum class PlayerState
{
	idle,//�~�܂��Ă�����
	move,//�����Ă�����
	attack,//�U�����Ă�����
	damage,//�_���[�W���
	step,//�X�e�b�v���
	slide,//�X���C�_�[���
};

struct PlayerDesc
{
	int hp;

};