/**
 * 自定义ADT - 伸展树，一种二叉搜索树，保证M个对树的操作的总时间复杂度在O(MlogN)
 * 二叉搜索树的定义：二叉搜索树是有特殊性质的二叉树
 * 特殊性质：对于树中的每个节点X，左子树上的每个节点都比节点X小，右子树上的每个节点都比节点X大。即，中序遍历下，节点元素是有序的。
 * 伸展的定义，插入或查找时，对目标节点进行若干此旋转操作，将目标节点变换的根节点
 */

#pragma once

namespace Joy
{
	/// <summary>
	/// 伸展树实现
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T> class JoySplayTree
	{
	public:
		JoySplayTree()
			: m_Root(nullptr)
		{
		}

		JoySplayTree(const JoySplayTree& rhs)
		{
		}

		~JoySplayTree()
		{
			MakeEmpty(m_Root);
		}

	public:
		/// <summary>
		/// 清空树
		/// </summary>
		void MakeEmpty()
		{
			MakeEmpty(m_Root);
		}

		/// <summary>
		/// 查询伸展树是否包含某元素
		/// </summary>
		/// <param name="element"></param>
		/// <returns></returns>
		bool Contain(const T& element) const
		{
			return Contain(element, m_Root);
		}

		/// <summary>
		/// 向伸展树插入一个新的元素节点
		/// </summary>
		/// <param name="element"></param>
		void Insert(const T& element)
		{
			Insert(element, m_Root);
		}

	private:
		/// <summary>
		/// 伸展树的节点数据结构，伸展树不需要记录节点的深度信息和平衡信息
		/// </summary>
		struct SplayTreeNode
		{
			T elementData;
			SplayTreeNode* pLeftChild{ nullptr };
			SplayTreeNode* pRightChild{ nullptr };

			SplayTreeNode(const T& element, SplayTreeNode* pLeftNode = nullptr, SplayTreeNode* pRightNode = nullptr)
				: elementData(element), pLeftChild(pLeftNode), pRightChild(pRightNode)
			{
			}
		};

	private:
		/// <summary>
		/// 清空树
		/// </summary>
		/// <param name="pNode"></param>
		void MakeEmpty(SplayTreeNode*& pNode)
		{
			if (pNode == nullptr) { return; }

			MakeEmpty(pNode->pRightChild);
			MakeEmpty(pNode->pLeftChild);

			delete pNode;
			pNode = nullptr;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		/// <returns></returns>
		bool Contain(const T& element, SplayTreeNode* pNode)
		{
			// TODO 查询伸展树是否包含某元素
			return false;
		}

		/// <summary>
		/// 向伸展树插入一个新的元素节点，递归实现
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		void Insert(const T& element, SplayTreeNode* pNode)
		{
			// TODO 插入一个新的节点
		}

	private:
		/// <summary>
		/// 伸展树根节点
		/// </summary>
		/// <typeparam name="T"></typeparam>
		SplayTreeNode* m_Root{ nullptr };
	};
}
