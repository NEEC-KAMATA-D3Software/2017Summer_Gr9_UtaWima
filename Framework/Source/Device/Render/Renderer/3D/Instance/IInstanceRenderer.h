#pragma once
#include<list>
#include<memory>
#include<vector>
#include<Source\Device\Render\Renderer\3D\RenderDefine.h>

/**
* @file	    IInstanceRenderer.h
* @brief	�C���X�^���X�`��N���X�����ׂĎ������Ă���
* @authro	���{�D�P
* @date	2017/04/01
*/

namespace util {
	struct Transform;
}

namespace framework {

	struct MaterialParam;
	class IMaterial;
	class Texture2D;
	class CubeTexture;
	class CubeRenderTrget;
	class CubeDepthTarget;

	class IInstanceRenderer
	{
	public:
		IInstanceRenderer() {};
		~IInstanceRenderer() {};

		/**
		* @brief					������
		*/
		virtual void init() {};

		virtual void setCubMap(const std::vector<std::shared_ptr<framework::CubeRenderTrget>>& cubMap) {};

		/**
		* @brief				�P�̃L���[�u�}�b�v�����ׂĂɔ��f������
		* @param instanceNum�@�@�`�悷��C���X�^���X�̐�
		* @param cubMap			���f���������L���[�u�e�N�X�`��
		*/
		virtual void setCubMap(int instanceNum, const std::shared_ptr<framework::CubeTexture>& cubMap) {};


		/**
		* @brief				�}�e���A���̃p�����[�^�Z�b�g
		* @param param			�}�e���A���p�����[�^
		*/
		virtual void setMaterialParam(MaterialParam& param) {};

		/**
		* @brief					�`��R�[���o�b�N
		* @param drawTransList		�`��Ɏg���g�����X�t�H�[���̃��X�g
		*/
		void draw(std::list<util::Transform*> drawTransList, int offset = 0){
			drawLoop(drawTransList, offset);
		};

		virtual void clear();

	protected:
		/**
		* @brief				���ׂĕ`�悷��܂ōċA�I�ɌJ��Ԃ�
		* @param drawTransList	�`�悷��g�����X�t�H�[�����X�g
		* @param offset			offset�͊֐�����draw�̈����̂��̂����̂܂ܓ����
		* @param drawAction		drawNum���g���ď������Ȃ���΂Ȃ�Ȃ��֐��̃|�C���^
		*/
		void drawLoop(const std::list<util::Transform*>& drawTransList,int offset ) {
			//���[���h�ϊ��s���萔�o�b�t�@�ɋl�ߑւ���
			auto it = drawTransList.begin();
			//offset���C�e���[�^��i�߂Ă���
			for (int i = 0; i != offset; ++i) {
				it++;
			}

			int drawNum;
			const int transListSize = drawTransList.size();
			for (drawNum = 0; drawNum != MAX_DRAW_NUM; ++drawNum) {
				//�g�����X���X�g��葽���͕`��ł��Ȃ��悤�ɂ���
				if (transListSize <= drawNum + offset)break;
				instanceAction(drawNum + offset,(*it));
				it++;
			}

			//�p�C�v���C���̐ݒ�
			setPipeline(drawNum);
			//draw�񐔂���������Ă���΂�����xdrawcall����
			drawComeback(drawTransList, drawNum, offset);
		}

		/**
		* @brief				���ׂĕ`�悷��܂ōċA�I�ɌJ��Ԃ�
		* @param drawTransList	draw�̈��������̂܂ܓ����
		* @param drawNum		�`�搔�𐔂��Ă��̐�������
		* @param offset			draw�̈��������̂܂ܓ����
		*/
		void drawComeback(const std::list<util::Transform*>& drawTransList,int drawNum ,int offset) {
			auto&& offset_ = drawNum + offset;
			if ((drawTransList.size()) - offset_> 0) {
				draw(drawTransList, offset_);
			}
		}

		/**
		* @brief				�C���X�^���X���Ƃɏ������o�����߂̃R�[���o�b�N
		* @param instanceNum	���Ԗڂ̃C���X�^���X��
		* @param trans			���̃��[�v�`�悷��g�����X�t�H�[��
		*/
		virtual void instanceAction(int instanceNum, util::Transform* trans) = 0;

		/**
		* @brief			�p�C�v���C���̐ݒ�
		* @param drawNum	�`�悷�鐔
		*/
		virtual void setPipeline(int drawNum) = 0;

	private:

	};
}