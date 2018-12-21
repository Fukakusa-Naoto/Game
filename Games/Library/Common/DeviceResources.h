//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		DeviceResources.h
//!
//! @summary	Direct3D 11デバイスとスワップチェーンの管理クラスのヘッダファイル
//!
//! @date		2018.09.18
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once


// ヘッダファイルの読み込み =======================================================
// <プリコンパイル済みヘッダファイル>
#include "../../pch.h"

// <自作ヘッダファイル>
#include "../System/SingletonBase.h"


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Common
	{
		// インタフェースの定義 ===================================================
		// Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
		// デバイスが失われたり作成されたことを通知されるDeviceResourcesを所有するアプリケーションのインタフェースを提供します。
		interface IDeviceNotify
		{
			virtual void OnDeviceLost() = 0;
			virtual void OnDeviceRestored() = 0;
		};


		// クラスの定義 ==========================================================
		// すべてのDirectXデバイスリソースを制御するクラス
		class DeviceResources : public System::SingletonBase<DeviceResources>
		{

			// <フレンドの登録>
			friend class System::SingletonBase<DeviceResources>;


			// <メンバ定数>
		public:
			static const unsigned int c_AllowTearing = 0x1;
			static const unsigned int c_EnableHDR = 0x2;


			// <メンバ変数>
		private:
			// Direct3D objects.
			// Direct3Dオブジェクト
			Microsoft::WRL::ComPtr<IDXGIFactory2>               m_dxgiFactory;
			Microsoft::WRL::ComPtr<ID3D11Device1>               m_d3dDevice;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext1>        m_d3dContext;
			Microsoft::WRL::ComPtr<IDXGISwapChain1>             m_swapChain;
			Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation>   m_d3dAnnotation;

			// Direct3D rendering objects. Required for 3D.
			// Direct3Dレンダリングオブジェクト。 3Dには必須です
			Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_renderTarget;
			Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthStencil;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_d3dRenderTargetView;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_d3dDepthStencilView;
			D3D11_VIEWPORT                                  m_screenViewport;

			// Direct3D properties.
			// Direct3Dのプロパティ
			DXGI_FORMAT                                     m_backBufferFormat;
			DXGI_FORMAT                                     m_depthBufferFormat;
			UINT                                            m_backBufferCount;
			D3D_FEATURE_LEVEL                               m_d3dMinFeatureLevel;

			// キャッシュされたデバイスのプロパティ
			HWND                                            m_window;
			D3D_FEATURE_LEVEL                               m_d3dFeatureLevel;
			RECT                                            m_outputSize;

			// HDRサポート
			DXGI_COLOR_SPACE_TYPE                           m_colorSpace;

			// DeviceResourcesオプション（上記のフラグを参照）
			unsigned int                                    m_options;

			// The IDeviceNotify can be held directly as it owns the DeviceResources.
			// IDeviceNotifyは、DeviceResourcesを所有しているので直接保持することができます。
			IDeviceNotify*                                  m_deviceNotify;

			// コモンステート
			std::unique_ptr<DirectX::CommonStates> m_commonStates;


			// <コンストラクタ>
		private:
			DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
				DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT,
				UINT backBufferCount = 2,
				D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_10_0,
				unsigned int flags = 0);


			// <メンバ関数>
		public:
			void CreateDeviceResources();
			void CreateWindowSizeDependentResources();
			void SetWindow(HWND window, int width, int height);
			bool WindowSizeChanged(int width, int height);
			void HandleDeviceLost();
			void RegisterDeviceNotify(IDeviceNotify* deviceNotify) { m_deviceNotify = deviceNotify; }
			void Present();

		private:
			void CreateFactory();
			void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);
			void UpdateColorSpace();


			// <ゲッター関数>
		public:
			RECT					GetOutputSize() const { return m_outputSize; }
			ID3D11Device1*          GetD3DDevice() const { return m_d3dDevice.Get(); }
			ID3D11DeviceContext1*   GetD3DDeviceContext() const { return m_d3dContext.Get(); }
			IDXGISwapChain1*        GetSwapChain() const { return m_swapChain.Get(); }
			D3D_FEATURE_LEVEL       GetDeviceFeatureLevel() const { return m_d3dFeatureLevel; }
			ID3D11Texture2D*        GetRenderTarget() const { return m_renderTarget.Get(); }
			ID3D11Texture2D*        GetDepthStencil() const { return m_depthStencil.Get(); }
			ID3D11RenderTargetView*	GetRenderTargetView() const { return m_d3dRenderTargetView.Get(); }
			ID3D11DepthStencilView* GetDepthStencilView() const { return m_d3dDepthStencilView.Get(); }
			DXGI_FORMAT             GetBackBufferFormat() const { return m_backBufferFormat; }
			DXGI_FORMAT             GetDepthBufferFormat() const { return m_depthBufferFormat; }
			D3D11_VIEWPORT          GetScreenViewport() const { return m_screenViewport; }
			UINT                    GetBackBufferCount() const { return m_backBufferCount; }
			DXGI_COLOR_SPACE_TYPE   GetColorSpace() const { return m_colorSpace; }
			unsigned int            GetDeviceOptions() const { return m_options; }
			inline DirectX::CommonStates*	GetCommonStates() const { return m_commonStates.get(); }


			// Performance events
			// パフォーマンスイベント
			void PIXBeginEvent(_In_z_ const wchar_t* name)
			{
				m_d3dAnnotation->BeginEvent(name);
			}

			void PIXEndEvent()
			{
				m_d3dAnnotation->EndEvent();
			}

			void PIXSetMarker(_In_z_ const wchar_t* name)
			{
				m_d3dAnnotation->SetMarker(name);
			}

		};
	}
}
