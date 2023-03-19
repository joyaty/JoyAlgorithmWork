/**
 * 自定义ADT - 红黑树，一种AVL树的特化变种
 * 红黑树的性质：
 * 1.树中的任意节点，要么是红色的，要么是黑色的
 * 1.根节点是黑色的
 * 3.若一个节点是红色，则子节点必须是黑色的
 * 4.从一个节点到一个NULL指针的节点的每条路径都必须包含相同数量的黑色节点
 */

#pragma once

#include <iostream>

namespace Joy
{
	/// <summary>
	/// 自定义红黑树的实现
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T> class JoyRedBlackTree
	{
	public:
		/// <summary>
		/// 红黑节点枚举定义
		/// </summary>
		enum class EnumColor : unsigned char
		{
			BLACK,
			RED,
		};

	private:
		/// <summary>
		/// 红黑树的节点数据结构
		/// </summary>
		struct RedBlackNode
		{
			T elementData;
			RedBlackNode* pLeftChild{ nullptr };
			RedBlackNode* pRightChild{ nullptr };
			EnumColor colorFlag{ EnumColor::Black };

			RedBlackNode(const T& element, RedBlackNode* pLeftNode = nullptr, RedBlackNode* pRightNode = nullptr, EnumColor color = EnumColor::BLACK)
				: elementData(element), pLeftChild(pLeftNode), pRightChild(pRightChild), colorFlag(color)
			{
			}
		};

	private:
		RedBlackNode* m_Root{ nullptr };
	};
}
