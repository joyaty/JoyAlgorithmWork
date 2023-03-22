/**
 * 自定义ADT - 伸展树，一种二叉搜索树，保证M个对树的操作的总时间复杂度在O(MlogN)
 * 二叉搜索树的定义：二叉搜索树是有特殊性质的二叉树
 * 特殊性质：对于树中的每个节点X，左子树上的每个节点都比节点X小，右子树上的每个节点都比节点X大。即，中序遍历下，节点元素是有序的。
 * 伸展的定义，插入或查找时，对目标节点进行若干次旋转操作，将目标节点变换的根节点
 */

#pragma once

#include <iostream>

namespace Joy
{
	/// <summary>
	/// 自顶向下的伸展树实现
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T> class JoySplayTree
	{
	private:
		/// <summary>
		/// 伸展树的节点数据结构，伸展树不需要记录节点的深度信息和平衡信息
		/// </summary>
		struct SplayTreeNode
		{
			T elementData{};
			SplayTreeNode* pLeftChild{ nullptr };
			SplayTreeNode* pRightChild{ nullptr };

			SplayTreeNode() = default;
			SplayTreeNode(const T& element, SplayTreeNode* pLeftNode = nullptr, SplayTreeNode* pRightNode = nullptr)
				: elementData(element), pLeftChild(pLeftNode), pRightChild(pRightNode)
			{
			}
		};
	public:
		JoySplayTree()
		{
			m_NullNode = new SplayTreeNode();
			m_NullNode->pLeftChild = m_NullNode->pRightChild = m_NullNode;
			m_Root = m_NullNode;
		}

		JoySplayTree(const JoySplayTree& rhs)
			: JoySplayTree()
		{
			operator=(rhs);
		}

		JoySplayTree(const T* pHeader, int count)
			: JoySplayTree()
		{
			for (int i = 0; i < count; ++i)
			{
				SimpleBuild(*(pHeader + i), m_Root);
			}
		}

		~JoySplayTree()
		{
			MakeEmpty(m_Root);
			delete m_NullNode;
			m_NullNode = nullptr;
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
		/// 伸展树是否为空
		/// </summary>
		void IsEmpty() const
		{
			return m_Root == m_NullNode;
		}

		/// <summary>
		/// 打印树结构
		/// </summary>
		void PrintTree() const
		{
			std::cout << "PreOrder =================" << std::endl;
			PrintTreePreOrder(m_Root);
			std::cout << "InOrder ==================" << std::endl;
			PrintTreeInOrder(m_Root);
			std::cout << "PostOrder ================" << std::endl;
			PrintTreePostOrder(m_Root);
		}

		/// <summary>
		/// 查找最大元素
		/// </summary>
		/// <returns></returns>
		const T& FindMax()
		{
			SplayTreeNode* pNode = FindMax(m_Root);
			if (pNode != m_NullNode)
			{
				Splay(pNode->elementData, m_Root);
			}
			return pNode->elementData;
		}

		/// <summary>
		/// 查找最小元素
		/// </summary>
		/// <returns></returns>
		const T& FindMin()
		{
			SplayTreeNode* pNode = FindMin(m_Root);
			if (pNode != m_NullNode)
			{
				Splay(pNode->elementData, m_Root);
			}
			return pNode->elementData;
		}

		/// <summary>
		/// 查询伸展树是否包含某元素
		/// </summary>
		/// <param name="element"></param>
		/// <returns></returns>
		bool Contain(const T& element)
		{
			Splay(element, m_Root);
			return m_Root->elementData == element;
		}

		/// <summary>
		/// 向伸展树插入一个新的元素节点
		/// </summary>
		/// <param name="element"></param>
		void Insert(const T& element)
		{
			// 空树，直接在根节点插入
			if (m_Root == m_NullNode)
			{
				m_Root = new SplayTreeNode(element, m_NullNode, m_NullNode);
				return;
			}
			// 伸展操作，
			Splay(element, m_Root);
			if (m_Root->elementData < element)
			{
				// 新根节点比插入元素小，新根节点为插入节点的左子树，新根节点的右子树为新插入节点的右子树
				SplayTreeNode* pNewNode = new SplayTreeNode(element, m_Root, m_Root->pRightChild);
				// 移除旧根节点的右子树关联
				m_Root->pRightChild = m_NullNode;
				// 新插入节点变为新根节点
				m_Root = pNewNode;
			}
			else if (element < m_Root->elementData)
			{
				// 新根节点比插入元素大，新根节点为插入节点的右子树，新根节点的左子树为新插入节点的左子树
				SplayTreeNode* pNewNode = new SplayTreeNode(element, m_Root->pLeftChild, m_Root);
				// 移除旧根节点的左子树关联
				m_Root->pLeftChild = m_NullNode;
				// 新插入节点变为新根节点
				m_Root = pNewNode;
			}
			else
			{
				// Do nothing，元素已在数结构中
			}
		}

		/// <summary>
		/// 删除伸展树的一个节点
		/// </summary>
		/// <param name="element"></param>
		void Remove(const T& element)
		{
			// 空树，不处理
			if (m_Root == m_NullNode) { return; }
			// 伸展操作，将要删除的节点伸展到根节点
			Splay(element, m_Root);
			// 元素不存在
			if (m_Root->elementData != element) { return; }
			SplayTreeNode* pNewRoot;
			if (m_Root->pLeftChild == m_NullNode)
			{
				// 左子树为空，直接令新根节点为右子树
				pNewRoot = m_Root->pRightChild;
			}
			else if (m_Root->pRightChild == m_NullNode)
			{
				// 右子树为空，直接令新根节点为左子树
				pNewRoot = m_Root->pLeftChild;
			}
			else
			{
				// 根节点左右子树都不为空
				pNewRoot = m_Root->pLeftChild;
				// 再次伸展，找到左子树下最大的节点，作为新根节点
				Splay(element, pNewRoot);
				// 根节点左子树的最大节点，右子树必然为空，令其链接根节点右子树
				pNewRoot->pRightChild = m_Root->pRightChild;
			}
			// 删除目标根节点
			delete m_Root;
			// 新根节点
			m_Root = pNewRoot;
		}

	public:
		/// <summary>
		/// 重写赋值操作符，执行深拷贝
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		const JoySplayTree& operator=(const JoySplayTree& rhs)
		{
			MakeEmpty(m_Root);
			m_Root = Clone(rhs.m_Root, rhs.m_NullNode);
			return *this;
		}

	private:
		/// <summary>
		/// 清空树
		/// </summary>
		/// <param name="pNode"></param>
		void MakeEmpty(SplayTreeNode*& pNode)
		{
			if (pNode == m_NullNode) { return; }

			MakeEmpty(pNode->pRightChild);
			MakeEmpty(pNode->pLeftChild);

			delete pNode;
			pNode = nullptr;
		}

		/// <summary>
		/// 前序遍历打印
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePreOrder(SplayTreeNode* pNode) const
		{
			if (pNode == m_NullNode) { return; }
			std::cout << pNode->elementData << std::endl;
			PrintTreePreOrder(pNode->pLeftChild);
			PrintTreePreOrder(pNode->pRightChild);
		}

		/// <summary>
		/// 中序遍历打印
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreeInOrder(SplayTreeNode* pNode) const
		{
			if (pNode == m_NullNode) { return; }
			PrintTreeInOrder(pNode->pLeftChild);
			std::cout << pNode->elementData << std::endl;
			PrintTreeInOrder(pNode->pRightChild);
		}

		/// <summary>
		/// 后序遍历打印
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePostOrder(SplayTreeNode* pNode) const
		{
			if (pNode == m_NullNode) { return; }
			PrintTreePostOrder(pNode->pLeftChild);
			PrintTreePostOrder(pNode->pRightChild);
			std::cout << pNode->elementData << std::endl;
		}

		/// <summary>
		/// 构建一棵初始结构的二叉搜索树，不考虑伸展
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		void SimpleBuild(const T& element, SplayTreeNode*& pNode)
		{
			if (pNode == m_NullNode)
			{
				pNode = new SplayTreeNode(element, m_NullNode, m_NullNode);
			}
			else if (pNode->elementData < element)
			{
				SimpleBuild(element, pNode->pRightChild);
			}
			else if (element < pNode->elementData)
			{
				SimpleBuild(element, pNode->pLeftChild);
			}
		}

		SplayTreeNode* Clone(SplayTreeNode* pCopyNode, SplayTreeNode* nullNode)
		{
			if (pCopyNode == nullNode) { return m_NullNode; }
			return new SplayTreeNode(pCopyNode->elementData, Clone(pCopyNode->pLeftChild, nullNode), Clone(pCopyNode->pRightChild, nullNode));
		}

		/// <summary>
		/// 常规查找二叉搜索树最大节点
		/// </summary>
		/// <param name="pNode"></param>
		/// <returns></returns>
		SplayTreeNode* FindMax(SplayTreeNode* pNode)
		{
			if (pNode->pRightChild == m_NullNode)
			{
				return pNode;
			}
			return FindMax(pNode->pRightChild);
		}

		/// <summary>
		/// 常规查找二叉搜索树最小节点
		/// </summary>
		/// <param name="pNode"></param>
		/// <returns></returns>
		SplayTreeNode* FindMin(SplayTreeNode* pNode)
		{
			if (pNode->pLeftChild == m_NullNode)
			{
				return pNode;
			}
			return FindMin(pNode->pLeftChild);
		}

		/// <summary>
		/// 目标在左儿子下，右旋
		/// </summary>
		/// <param name="pNode"></param>
		void RotateWithLeftChild(SplayTreeNode*& pNode)
		{
			SplayTreeNode* pTemp = pNode->pLeftChild;
			pNode->pLeftChild = pTemp->pRightChild;
			pTemp->pRightChild = pNode;
			pNode = pTemp;
		}

		/// <summary>
		/// 目标在右儿子下，左旋
		/// </summary>
		/// <param name="pNode"></param>
		void RotateWithRightChild(SplayTreeNode*& pNode)
		{
			SplayTreeNode* pTemp = pNode->pRightChild;
			pNode->pRightChild = pTemp->pLeftChild;
			pTemp->pLeftChild = pNode;
			pNode = pTemp;
		}

		/// <summary>
		/// 伸展操作
		/// </summary>
		/// <param name="element"></param>
		/// <param name="pNode"></param>
		void Splay(const T& element, SplayTreeNode*& pNode)
		{
			// 左树的最大节点
			SplayTreeNode* pLeftMax;
			// 右树的最小节点
			SplayTreeNode* pRightMin;
			// 临时树节点，连接左树和右树的根节点，用于最后树合并，额外的O(1)的辅助空间
			static SplayTreeNode header;
			// 左树和右树初始为空树
			header.pLeftChild = header.pRightChild = m_NullNode;
			pLeftMax = pRightMin = &header;
			// 空节点的ElementData设置为目标数据，确保叶子节点能正确索引
			m_NullNode->elementData = element;
			// 自顶向下伸展
			while (true)
			{
				if (element < pNode->elementData)
				{
					if (element < pNode->pLeftChild->elementData)
					{
						// 左儿子的左子树下，一字型旋转(右旋)
						RotateWithLeftChild(pNode);
					}
					if (pNode->pLeftChild == m_NullNode)
					{
						// 到了叶子节点，结束伸展
						break;
					}
					// 当前节点挂载在右树最小节点的左儿子上
					pRightMin->pLeftChild = pNode;
					// 更新右树的最小节点
					pRightMin = pRightMin->pLeftChild;
					// 当前节点更新到左子节点
					pNode = pNode->pLeftChild;
				}
				else if (pNode->elementData < element)
				{
					if (pNode->pRightChild->elementData < element)
					{
						// 右儿子的右子树下，一字型旋转(左旋)
						RotateWithRightChild(pNode);
					}
					if (pNode->pRightChild == m_NullNode)
					{
						// 到了叶子节点，结束伸展
						break;
					}
					// 当前节点挂载在右树最小节点的左儿子上
					pLeftMax->pRightChild = pNode;
					// 更新右树的最小节点
					pLeftMax = pLeftMax->pRightChild;
					// 当前节点更新到左子节点
					pNode = pNode->pRightChild;
				}
				else
				{
					// 匹配，结束
					break;
				}
			}
			// 当前节点的左右儿子，分别挂载在左树的最大节点和右树的最小节点上
			pRightMin->pLeftChild = pNode->pRightChild;
			pLeftMax->pRightChild = pNode->pLeftChild;
			// 伸展结束，目标节点转换到了伸展树的根节点上，合并左树中树和右树，注意左树在header的右儿子上，右树在header的左儿子上
			pNode->pRightChild = header.pLeftChild;
			pNode->pLeftChild = header.pRightChild;
		}

	private:
		/// <summary>
		/// 伸展树根节点
		/// </summary>
		/// <typeparam name="T"></typeparam>
		SplayTreeNode* m_Root{ nullptr };

		/// <summary>
		/// 空节点，指代nullptr，所有叶子节点都应该指向m_NullNode，定义此节点可以简化程序，避免过多的nullptr异常处理，也方便按引用返回不存在的节点
		/// </summary>
		SplayTreeNode* m_NullNode{ nullptr };
	};
}
