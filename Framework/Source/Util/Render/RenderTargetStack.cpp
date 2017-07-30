#include"RenderTargetStack.h"
#include<Source\Util\Render\DXFunc.h>

namespace util {

	RenderTargetStack::RenderTargetStack(int numView)
	{
		m_numView = numView;
		util::getContext()->OMGetRenderTargets(numView, &m_TargetStack, &m_DepthStack);
		util::getContext()->OMSetRenderTargets(0, NULL, NULL);
	}

	RenderTargetStack::~RenderTargetStack()
	{
		//メンバのポインタが空だったら処理しない
		if (m_TargetStack == nullptr && m_DepthStack == nullptr)return;

		util::getContext()->OMSetRenderTargets(m_numView, &m_TargetStack, m_DepthStack);
	}

}