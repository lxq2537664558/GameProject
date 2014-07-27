﻿#include "stdafx.h"
#include "SceneManager.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketDef/PacketHeader.h"
#include "DataBuffer/BufferHelper.h"



CSceneManager::CSceneManager()
{

}

CSceneManager::~CSceneManager()
{
	SceneMap::iterator itor = m_mapSceneList.begin();
	for(; itor != m_mapSceneList.end(); ++itor)
	{
		CScene *pScene = itor->second;
		if(pScene != NULL)
		{
			pScene->Uninit();
			delete pScene;
			pScene = NULL;
		}
	}
}

BOOL CSceneManager::Init(UINT32 dwReserved)
{
	CCommonCmdHandler::Init(dwReserved);

	return TRUE;
}

BOOL CSceneManager::Uninit()
{
	CCommonCmdHandler::Uninit();

	return TRUE;
}

BOOL CSceneManager::CreateScene( UINT32 dwSceneID )
{
	CScene *pScene = new CScene;

	if(!pScene->Init(dwSceneID))
	{
		delete pScene;
		return FALSE;
	}

	m_mapSceneList.insert(std::make_pair(dwSceneID, pScene));

	return TRUE;
}

BOOL CSceneManager::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	BOOL bHandled = TRUE;

	switch(wCommandID)
	{

	default:
		{
			bHandled = FALSE;
		}
		break;
	}

	if(bHandled) //消息己经被处理
	{
		return TRUE;
	}

	CommandHeader *pHeader = pBufferHelper->GetCommandHeader();
	if(pHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CScene *pScene = GetSceneByID(pHeader->dwSceneID);
	if(pScene == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	pScene->OnCommandHandle(wCommandID, u64ConnID, pBufferHelper);
		
	return TRUE;
}

CScene* CSceneManager::GetSceneByID( UINT32 dwSceneID )
{
	SceneMap::iterator itor = m_mapSceneList.find(dwSceneID);
	if(itor != m_mapSceneList.end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL CSceneManager::OnUpdate( UINT32 dwTick )
{
	for(SceneMap::iterator itor = m_mapSceneList.begin(); itor != m_mapSceneList.end(); ++itor)
	{
		CScene *pScene = itor->second;

		pScene->OnUpdate(dwTick);
	}

	return TRUE;
}