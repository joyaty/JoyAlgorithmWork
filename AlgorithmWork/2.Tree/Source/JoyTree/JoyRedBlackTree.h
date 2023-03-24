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
			T elementData{};
			RedBlackNode* pLeftChild{ nullptr };
			RedBlackNode* pRightChild{ nullptr };
			EnumColor colorTag{ EnumColor::Black };

			RedBlackNode()
			{
			}

			RedBlackNode(const T& element, RedBlackNode* pLeftNode = nullptr, RedBlackNode* pRightNode = nullptr, EnumColor color = EnumColor::BLACK)
				: elementData(element), pLeftChild(pLeftNode), pRightChild(pRightChild), colorTag(color)
			{
			}
		};

	public:
		JoyRedBlackTree()
			: m_Root(nullptr)
		{
			m_NullNode = new RedBlackNode();
			m_NullNode->pLeftChild = m_NullNode->pRightChild = m_NullNode;
			m_Root = m_NullNode;
		}

		JoyRedBlackTree(const JoyRedBlackTree& rhs)
			: JoyRedBlackTree()
		{
		}

		explicit JoyRedBlackTree(const T& element)
			:JoyRedBlackTree()
		{
			m_Root = new RedBlackNode(element, m_NullNode, m_NullNode);
		}

		~JoyRedBlackTree()
		{
			MakeEmpty();
			delete m_NullNode;
			m_NullNode = nullptr;
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
			MakeEmpty(m_Root);
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

	public:
		const JoyRedBlackTree& operator= (const JoyRedBlackTree& rhs)
		{
			MakeEmpty();
			m_Root = Clone(rhs.m_Root);
			return *this;
		}
	
	private:
		/// <summary>
		/// 前序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePreOrder(RedBlackNode* pNode)
		{
			if (pNode == m_NullNode) { return; }
			std::cout << pNode->elementData << std::endl;
			PrintTreePreOrder(pNode->pLeftChild);
			PrintTreePreOrder(pNode->pRightChild);
		}
		/// <summary>
		/// 中序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreeInOrder(RedBlackNode* pNode)
		{
			if (pNode == m_NullNode) { return; }
			PrintTreeInOrder(pNode->pLeftChild);
			std::cout << pNode->elementData << std::endl;
			PrintTreeInOrder(pNode->pRightChild);
		}
		/// <summary>
		/// 后序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePostOrder(RedBlackNode* pNode)
		{
			if (pNode == m_NullNode) { return; }
			PrintTreePostOrder(pNode->pLeftChild);
			PrintTreePostOrder(pNode->pRightChild);
			std::cout << pNode->elementData << std::endl;
		}

		/// <summary>
		/// 清空树
		/// </summary>
		/// <param name="pNode"></param>
		void MakeEmpty(RedBlackNode*& pNode)
		{
			if (pNode == m_NullNode) { return; }
			MakeEmpty(pNode->pLeftChild);
			MakeEmpty(pNode->pRightChild);

			delete pNode;
			pNode = nullptr;
		}

		/// <summary>
		/// 向红黑树插入一个元素，并维持红黑树平衡
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		void Insert(const T& element, RedBlackNode*& pNode)
		{
			if (pNode == m_NullNode)
			{
				pNode = new RedBlackNode(element, m_NullNode, m_NullNode, EnumColor::RED);
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
			if (pNode == m_NullNode) { return; }
		}

		/// <summary>
		/// 克隆一个节点
		/// </summary>
		/// <param name="pNode"></param>
		/// <param name="pNullNode"></param>
		/// <returns></returns>
		RedBlackNode* Clone(const RedBlackNode* pNode)
		{
			// 说明是空节点
			if (pNode == pNode->pLeftChild) { return m_NullNode; }
			return new RedBlackNode(pNode->elementData, Clone(pNode->pLeftChild), Clone(pNode->pRightChild));
		}
		
	private:
		/// <summary>
		/// 红黑树根节点
		/// </summary>
		RedBlackNode* m_Root{ nullptr };

		/// <summary>
		/// 空节点，指代nullptr，所有叶子节点都应该指向m_NullNode，定义此节点可以简化程序，避免过多的nullptr异常处理，也方便按引用返回不存在的节点
		/// </summary>
		RedBlackNode* m_NullNode{ nullptr };
	};
}
