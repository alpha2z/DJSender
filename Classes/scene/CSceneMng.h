

#ifndef __CSCENEMNG_H__
#define __CSCENEMNG_H__

class CMainScene;
class CSceneMng
{
public:
	CSceneMng(void);
	~CSceneMng();

	static CSceneMng * sharedCSceneMng();

	CMainScene* getMainScene();

private:
	CMainScene * m_pMainScene;

};

#endif//__CSCENEMNG_H__


