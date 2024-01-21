#pragma once

#include <cstdint>
#include <unordered_map>
#include "GameObject.h"

namespace Joy
{
	namespace Gameplay
	{
		class LinkingContext
		{
		public:
			LinkingContext()
				: m_MapGameObjectToNetworkIDS({})
				, m_MapNetworkIDToGameObjects({})
			{
			}

		public:
			void AddGameObject(uint32_t nNetworkID, GameObject* pGameObject)
			{
				m_MapNetworkIDToGameObjects[nNetworkID] = pGameObject;
				m_MapGameObjectToNetworkIDS[pGameObject] = nNetworkID;
			}

			void RemoveGameObject(uint32_t nNetworkID)
			{
				GameObject* pRemoveObj = m_MapNetworkIDToGameObjects[nNetworkID];
				m_MapGameObjectToNetworkIDS.erase(pRemoveObj);
				m_MapNetworkIDToGameObjects.erase(nNetworkID);
			}

			void RemoveGameObject(GameObject* pGameObject)
			{
				uint32_t nNetworkID = m_MapGameObjectToNetworkIDS[pGameObject];
				m_MapNetworkIDToGameObjects.erase(nNetworkID);
				m_MapGameObjectToNetworkIDS.erase(pGameObject);
			}


		private:
			std::unordered_map<uint32_t, GameObject*> m_MapNetworkIDToGameObjects;
			std::unordered_map<GameObject*, uint32_t> m_MapGameObjectToNetworkIDS;
		};
	}
}