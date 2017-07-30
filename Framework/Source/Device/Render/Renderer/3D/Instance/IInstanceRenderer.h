#pragma once
#include<list>
#include<memory>
#include<vector>
#include<Source\Device\Render\Renderer\3D\RenderDefine.h>

/**
* @file	    IInstanceRenderer.h
* @brief	インスタンス描画クラスがすべて実装している
* @authro	高須優輝
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
		* @brief					初期化
		*/
		virtual void init() {};

		virtual void setCubMap(const std::vector<std::shared_ptr<framework::CubeRenderTrget>>& cubMap) {};

		/**
		* @brief				１つのキューブマップをすべてに反映させる
		* @param instanceNum　　描画するインスタンスの数
		* @param cubMap			反映させたいキューブテクスチャ
		*/
		virtual void setCubMap(int instanceNum, const std::shared_ptr<framework::CubeTexture>& cubMap) {};


		/**
		* @brief				マテリアルのパラメータセット
		* @param param			マテリアルパラメータ
		*/
		virtual void setMaterialParam(MaterialParam& param) {};

		/**
		* @brief					描画コールバック
		* @param drawTransList		描画に使うトランスフォームのリスト
		*/
		void draw(std::list<util::Transform*> drawTransList, int offset = 0){
			drawLoop(drawTransList, offset);
		};

		virtual void clear();

	protected:
		/**
		* @brief				すべて描画するまで再帰的に繰り返す
		* @param drawTransList	描画するトランスフォームリスト
		* @param offset			offsetは関数内でdrawの引数のものをそのまま入れる
		* @param drawAction		drawNumを使って処理しなければならない関数のポインタ
		*/
		void drawLoop(const std::list<util::Transform*>& drawTransList,int offset ) {
			//ワールド変換行列を定数バッファに詰め替える
			auto it = drawTransList.begin();
			//offset分イテレータを進めておく
			for (int i = 0; i != offset; ++i) {
				it++;
			}

			int drawNum;
			const int transListSize = drawTransList.size();
			for (drawNum = 0; drawNum != MAX_DRAW_NUM; ++drawNum) {
				//トランスリストより多くは描画できないようにする
				if (transListSize <= drawNum + offset)break;
				instanceAction(drawNum + offset,(*it));
				it++;
			}

			//パイプラインの設定
			setPipeline(drawNum);
			//draw回数が上限超えていればこう一度drawcallする
			drawComeback(drawTransList, drawNum, offset);
		}

		/**
		* @brief				すべて描画するまで再帰的に繰り返す
		* @param drawTransList	drawの引数をそのまま入れる
		* @param drawNum		描画数を数えてその数を入れる
		* @param offset			drawの引数をそのまま入れる
		*/
		void drawComeback(const std::list<util::Transform*>& drawTransList,int drawNum ,int offset) {
			auto&& offset_ = drawNum + offset;
			if ((drawTransList.size()) - offset_> 0) {
				draw(drawTransList, offset_);
			}
		}

		/**
		* @brief				インスタンスごとに情報を取り出すためのコールバック
		* @param instanceNum	何番目のインスタンスか
		* @param trans			そのループ描画するトランスフォーム
		*/
		virtual void instanceAction(int instanceNum, util::Transform* trans) = 0;

		/**
		* @brief			パイプラインの設定
		* @param drawNum	描画する数
		*/
		virtual void setPipeline(int drawNum) = 0;

	private:

	};
}