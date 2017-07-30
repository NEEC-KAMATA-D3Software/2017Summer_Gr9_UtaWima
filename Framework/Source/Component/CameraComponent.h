#pragma once
#include"UpdateComponent.h"
#include<Source\Device\Camera\Camera.h>
#include<functional>
#include<memory>
#include<list>
#include<Source\Resource\Texture\IMultiRenderTarget.h>
#include<Source\Resource\Texture\RenderTarget.h>

/**
* @file	CameraComponent.h
* @brief	Entityをカメラにするクラス
* @authro	高須優輝
* @date	2017/04/12
*/

namespace framework {
	class MSAAMultiRenderTarget;
	class MultiRenderTarget;
	class DepthTarget;
	class Texture2D;
	class MSAADepthTarget;
	class DeferredLighting;
	class PostEffectPass;
}

namespace component {

	class CameraComponent;
	class PostEffectComponent;

	using WeakCamera = std::weak_ptr<CameraComponent>;
	using WeakEntity = std::weak_ptr<framework::Entity>;

	class CameraComponent : public framework::UpdateComponent
	{
	public:
		CameraComponent();
		~CameraComponent();

		/**
		* @brief		初期化
		*/
		virtual void init()override;

		/**
		* @brief		アクティブにする
		*/
		virtual void active() override;

		/**
		* @brief		ディアクティブにする
		*/
		virtual void deActive() override;


		// UpdateComponent を介して継承されました
		virtual void update() override;

		/**
		* @brief		ビュー変換行列に変換して取得
		*/
		virtual util::Mat4 toViewMatrix();

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param)override;

		/**
		* @brief					ライティングバッファに描画する
		*/
		void doLighting();

		/**
		* @brief					エフェクト処理をする
		*/
		void doEffect();

		/**
		* @brief					メンバレンダーターゲットに書き込み開始
		*/
		void beginToBuffer(std::shared_ptr<framework::DepthTarget>& inDepth);

		/**
		* @brief					メンバーレンダーターゲットの書き込み終了
		*/
		void endToBuffer();

		/**
		* @brief					gBufferの内容を描画
		* @param drawLocation	　　何番目のロケーションか
		*/
		void gBufferDraw(int drawLocation);

		/**
		* @brief					lightingBufferの内容を描画
		* @param drawLocation	　　何番目のロケーションか
		*/
		void lightingDraw(int drawLocation);

		/**
		* @brief					ポストエフェクトのかかった最終結果描画
		*/
		void finalBufferDraw();

		//テスト

		CComPtr<ID3D11ShaderResourceView> getShaderVeiw();

	public:
		/**
		* @brief		すべてのカメラを取得する
		*/
		static std::vector<WeakCamera> allCameras();

		/**
		* @brief		すべてのカメラを取得する
		* @return	　　カメラを持っているエンティティの名前でカメラを検索する 見つからなけらばnullptr
		* @detail		取得は一番最初に見つかったものに限るので複数ついているものはfindCamerasを使う
		*/
		static WeakCamera findCamera(const std::string& cameraEntityName);

		/**
		* @brief		すべてのカメラを取得する
		* @return	　　カメラを持っているエンティティの名前でカメラを検索する 見つからなけらばnullptr
		* @detail		取得は一番最初に見つかったものに限るので複数ついているものはfindCamerasを使う
		*/
		static std::vector<WeakCamera> findCameras(const std::string& cameraEntityName);

		/**
		* @brief		現在のメインカメラを取得する
		*/
		static WeakCamera getMainCamera();

	private:

		void lookSet(const std::vector<std::string>& param);

		void renderTargetNumSet(const std::vector<std::string>& param);

		/**
		* @brief		 自身のエンティティーについているポストエフェクトを探す
		*/
		void findPostEffect();

		/**
		* @brief		 すべてのカメラコンポーネントに対して関数ポインタで操作ができる
		* @param cameraAction  カメラを操作する関数ポインタ
		*/
		static void cameraLoop(std::function<void(WeakCamera)> cameraAction);

		/**
		* @brief		 カメラを持っているエンティティーを検索して操作するループ
		* @param action  Entityにたいする関数ポインタ
		*/
		static void haveCameraEntityLoop(std::function<void(WeakEntity)> entityAction);


	private:

		framework::Camera m_Camera;

		//!注視点オブジェクト
		std::weak_ptr<framework::Entity> m_LookEntity;

		std::shared_ptr<framework::MultiRenderTarget> m_pGBuffer;

		std::shared_ptr<framework::MultiRenderTarget> m_pLBuffer;

		//!ポストエフェクトが
		std::shared_ptr<framework::RenderTarget> m_pPostRender;

		std::list<std::weak_ptr<PostEffectComponent>> m_PostEfect;

		std::unique_ptr<framework::DeferredLighting> m_pLighting;

		std::unique_ptr<framework::PostEffectPass> m_pPass;

		static WeakCamera m_MainCamera;
	};



}