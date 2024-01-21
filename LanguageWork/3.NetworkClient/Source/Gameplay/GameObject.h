#pragma once

#include <cstdint>

namespace Joy
{
	namespace Gameplay
	{
		class GameObject
		{
		public:
			GameObject()
				: m_NetworkID(++s_NetworkIDCount)
				, m_PositionX(0.f)
				, m_PositionY(0.f)
			{
			}

			GameObject(float fPosX, float fPosY)
				: m_NetworkID(++s_NetworkIDCount)
				, m_PositionX(fPosX)
				, m_PositionY(fPosY)
			{
			}

			virtual ~GameObject()
			{
			}

		private:
			/// <summary>
			/// GameObject唯一标识ID
			/// </summary>
			uint32_t m_NetworkID;

			float m_PositionX;
			float m_PositionY;

		private:
			/// <summary>
			/// NetworkID分配数
			/// </summary>
			static uint32_t s_NetworkIDCount;
		};
	}
}