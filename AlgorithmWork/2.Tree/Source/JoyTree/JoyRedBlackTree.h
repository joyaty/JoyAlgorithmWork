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
		/// 红黑树节点颜色枚举定义
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
			EnumColor colorTag{ EnumColor::Black };

			RedBlackNode(const T& element, RedBlackNode* pLeftNode = nullptr, RedBlackNode* pRightNode = nullptr, EnumColor color = EnumColor::BLACK)
				: elementData(element), pLeftChild(pLeftNode), pRightChild(pRightChild), colorTag(color)
			{
			}
		};

	public:
		JoyRedBlackTree()
			: m_Root(nullptr)
		{
		}

		JoyRedBlackTree(const JoyRedBlackTree& rhs)
		{
		}

		explicit JoyRedBlackTree(const T& element)
			: m_Root(new RedBlackNode(element, nullptr, nullptr, EnumColor::BLACK))
		{
		}

		~JoyRedBlackTree()
		{
		}

	public:
		void PrintTree() const
		{
			std::cout << "PreOrder  ================" << std::endl;
			PrintTreePreOrder(m_Root);
			std::cout << "InOrder   ================" << std::endl;
			PrintTreeInOrder(m_Root);
			std::cout << "PostOrder ================" << std::endl;
			PrintTreePostOrder(m_Root);
		}

		void MakeEmpty()
		{
		}

		/// <summary>
		/// 向红黑树新增一个元素节点
		/// </summary>
		/// <param name="element"></param>
		void Insert(const T& element)
		{
		}

		/// <summary>
		/// 从红黑树删除指定的元素节点
		/// </summary>
		/// <param name="element"></param>
		void Remove(const T& element)
		{
		}
	
	private:
		/// <summary>
		/// 前序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePreOrder(RedBlackNode* pNode)
		{

		}
		/// <summary>
		/// 中序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreeInOrder(RedBlackNode* pNode)
		{
		}
		/// <summary>
		/// 后序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePostOrder(RedBlackNode* pNode)
		{
		}

		/// <summary>
		/// 清空树
		/// </summary>
		/// <param name="pNode"></param>
		void MakeEmpty(RedBlackNode* pNode)
		{
		}

		/// <summary>
		/// 向红黑树插入一个元素，并维持红黑树平衡
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		void Insert(const T& element, RedBlackNode*& pNode)
		{
			if (pNode == nullptr)
			{
				pNode = new RedBlackNode(element, nullptr, nullptr, EnumColor::RED);
			}
		}

		/// <summary>
		/// 删除红黑树的一个元素节点，并维持红黑树平衡
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		void Remove(const T& element, RedBlackNode*& pNode)
		{
			// 不存在要删除的元素节点，直接返回，不处理
			if (pNode == nullptr) { return; }
		}

	private:
		/// <summary>
		/// 红黑树根节点
		/// </summary>
		RedBlackNode* m_Root{ nullptr };
	};
}
