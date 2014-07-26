
#include "CSceneMng.h"
#include "CMainScene.h"

static CSceneMng * g_pMng = nullptr;

CSceneMng::CSceneMng()
:m_pMainScene(nullptr) {

}

CSceneMng::~CSceneMng() {

}

CSceneMng * CSceneMng::sharedCSceneMng() {
	if (g_pMng)
		return g_pMng;

	g_pMng = new CSceneMng;
	return g_pMng;
}

CMainScene* CSceneMng::getMainScene() {
	if (m_pMainScene)
		return m_pMainScene;
	
	m_pMainScene = CMainScene::create();
	return m_pMainScene;
}




