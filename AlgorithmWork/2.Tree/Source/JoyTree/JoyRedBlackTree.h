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
	/// 自定义自顶向下红黑树的实现                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
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
			EnumColor colorTag{ EnumColor::BLACK };

			RedBlackNode()
			{
			}

			RedBlackNode(const T& element, RedBlackNode* pLeftNode = nullptr, RedBlackNode* pRightNode = nullptr, EnumColor color = EnumColor::BLACK)
				: elementData(element), pLeftChild(pLeftNode), pRightChild(pRightNode), colorTag(color)
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
			operator=(rhs);
		}

		~JoyRedBlackTree()
		{
			MakeEmpty();
			delete m_NullNode;
			m_NullNode = nullptr;
		}

	public:
		/// <summary>
		/// 打印树
		/// </summary>
		void PrintTree() const
		{
			std::cout << "PreOrder  ================" << std::endl;
			PrintTreePreOrder(m_Root);
			std::cout << "InOrder   ================" << std::endl;
			PrintTreeInOrder(m_Root);
			std::cout << "PostOrder ================" << std::endl;
			PrintTreePostOrder(m_Root);
		}

		/// <summary>
		/// 是否为空树
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() const
		{
			return m_Root == m_NullNode;
		}

		/// <summary>
		/// 清空树
		/// </summary>
		void MakeEmpty()
		{
			MakeEmpty(m_Root);
		}

		/// <summary>
		/// 查找树最小值
		/// </summary>
		/// <returns></returns>
		const T& FindMin() const
		{

		}
		/// <summary>
		/// 查找树最大值
		/// </summary>
		/// <returns></returns>
		const T& FindMax() const
		{
		}

		/// <summary>
		/// 是否包含值
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		bool Contain(const T& value) const
		{
		}

		/// <summary>
		/// 向红黑树新增一个元素节点，自顶向下插入
		/// </summary>
		/// <param name="element"></param>
		void Insert(const T& element)
		{
			if (m_Root == m_NullNode)
			{
				// 空树，直接放在根节点
				m_Root = new RedBlackNode(element, m_NullNode, m_NullNode, EnumColor::BLACK);
				return;
			}
			m_Current = m_Parent = m_Grand = m_Great = m_Root;
			// 空节点元素标记为目标值，确保可以找到插入位置
			m_NullNode->elementData = element;
			while (m_Current->elementData != element)
			{
				m_Great = m_Grand;
				m_Grand = m_Parent;
				m_Parent = m_Current;
				m_Current = element < m_Current->elementData ? m_Current->pLeftChild : m_Current->pRightChild;
				// 当前节点的左右子节点皆为红色，需要翻转当前节点与子节点颜色，若父节点同为红色，需要再旋转
				if (m_Current->pLeftChild->colorTag == EnumColor::RED && m_Current->pRightChild->colorTag == EnumColor::RED)
				{
					HandleReorient(element);
				}
			}
			// 当前节点不为空节点，说明元素已在红黑树中，不处理插入
			if (m_Current != m_NullNode) { return; }
			// 位置在叶子(空)节点
			m_Current = new RedBlackNode(element, m_NullNode, m_NullNode, EnumColor::RED);
			if (element < m_Parent->elementData)
			{
				m_Parent->pLeftChild = m_Current;
			}
			else
			{
				m_Parent->pRightChild = m_Current;
			}
			// 插入新节点，检查红黑树，确认是否要翻转和旋转树结构
			HandleReorient(element);
		}

		/// <summary>
		/// 从红黑树删除指定的元素节点
		/// </summary>
		/// <param name="element"></param>
		void Remove(const T& element)
		{
		}

	public:
		/// <summary>
		/// 复制操作符
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		const JoyRedBlackTree& operator= (const JoyRedBlackTree& rhs)
		{
			if (this != &rhs)
			{
				MakeEmpty();
				m_Root = Clone(rhs->m_Root);
			}
			return *this;
		}
	
	private:
		/// <summary>
		/// 前序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePreOrder(RedBlackNode* pNode) const
		{
			if (pNode == m_NullNode) { return; }
			std::cout << pNode->elementData << " " << static_cast<int>(pNode->colorTag) << std::endl;
			PrintTreePreOrder(pNode->pLeftChild);
			PrintTreePreOrder(pNode->pRightChild);
		}
		/// <summary>
		/// 中序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreeInOrder(RedBlackNode* pNode) const
		{
			if (pNode == m_NullNode) { return; }
			PrintTreeInOrder(pNode->pLeftChild);
			std::cout << pNode->elementData << " " << static_cast<int>(pNode->colorTag) << std::endl;
			PrintTreeInOrder(pNode->pRightChild);
		}
		/// <summary>
		/// 后序遍历打印树
		/// </summary>
		/// <param name="pNode"></param>
		void PrintTreePostOrder(RedBlackNode* pNode) const
		{
			if (pNode == m_NullNode) { return; }
			PrintTreePostOrder(pNode->pLeftChild);
			PrintTreePostOrder(pNode->pRightChild);
			std::cout << pNode->elementData << " " << static_cast<int>(pNode->colorTag) << std::endl;
		}

		/// <summary>
		/// 克隆一个节点
		/// </summary>
		/// <param name="pNode"></param>
		/// <param name="pNullNode"></param>
		/// <returns></returns>
		RedBlackNode* Clone(const RedBlackNode* pNode) const
		{
			// 说明是空节点
			if (pNode == pNode->pLeftChild) { return m_NullNode; }
			return new RedBlackNode(pNode->elementData, Clone(pNode->pLeftChild), Clone(pNode->pRightChild), pNode->colorTag);
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
		/// 处理红黑树的旋转和颜色翻转
		/// </summary>
		/// <param name="value"></param>
		void HandleReorient(const T& value)
		{
			// 当前节点的左右两个子节点都是红色，或当前节点是新插入节点，则颜色翻转，当前节点为红色，子节点为黑色。
			m_Current->colorTag = EnumColor::RED;
			m_Current->pLeftChild->colorTag = EnumColor::BLACK;
			m_Current->pRightChild->colorTag = EnumColor::BLACK;
			// 当前节点的父节点是红色，需要旋转
			if (m_Parent->colorTag == EnumColor::RED)
			{
				// 旋转的祖父节点需要标记为红色
				m_Grand->colorTag = EnumColor::RED;
				// 当前节点与父节点到祖父节点的方向不同，需要双旋转
				if ((value < m_Parent->elementData) != (m_Parent->elementData < m_Grand->elementData))
				{
					// 当前节点与父节点做一次单旋，旋转的子树根节点的父节点是祖父节点。注意，此时，m_Parent更新为旋转子树的新根节点，这里新根节点为m_Current指向的节点
					m_Parent = Rotate(value, m_Grand);
				}
				// 父节点和祖父节点做一次单旋，旋转的子树的根节点的父节点是曾祖父节点。注意，此时，m_Current更新为旋转子树的新根节点，这里新根节点为m_Parent指向的节点
				m_Current = Rotate(value, m_Great);
				// 单旋转下，m_Current指向旧的m_Parent。双旋转下，m_Current与旧m_Current相同
				m_Current->colorTag = EnumColor::BLACK;
			}
			// 根节点保证为黑色
			m_Root->colorTag = EnumColor::BLACK;
		}

		/// <summary>
		/// 一次单旋转操作，返回旋转子树的新根节点
		/// 参数为目标值和旋转子树根节点的父节点，根据这两个参数可以方便的判断应该的旋转方向。
		/// </summary>
		/// <param name="value">目标值</param>
		/// <param name="theParent">旋转子树根节点的父节点</param>
		/// <returns></returns>
		RedBlackNode* Rotate(const T& value, RedBlackNode* theParent)
		{
			if (value < theParent->elementData)
			{
				value < theParent->pLeftChild->elementData
					? RotateWithLeftChild(theParent->pLeftChild) 
					: RotateWithRightChild(theParent->pLeftChild);
				return theParent->pLeftChild;
			}
			else
			{
				value < theParent->pRightChild->elementData
					? RotateWithLeftChild(theParent->pRightChild)
					: RotateWithRightChild(theParent->pRightChild);
				return theParent->pRightChild;
			}
		}

		/// <summary>
		/// 单旋转右旋
		/// </summary>
		/// <param name="pNode"></param>
		void RotateWithLeftChild(RedBlackNode*& pNode)
		{
			RedBlackNode* pTempNode = pNode->pLeftChild;
			pNode->pLeftChild = pTempNode->pRightChild;
			pTempNode->pRightChild = pNode;
			pNode = pTempNode;
		}

		/// <summary>
		/// 单旋转左旋
		/// </summary>
		/// <param name="pNode"></param>
		void RotateWithRightChild(RedBlackNode*& pNode)
		{
			RedBlackNode* pTempNode = pNode->pRightChild;
			pNode->pRightChild = pTempNode->pLeftChild;
			pTempNode->pLeftChild = pNode;
			pNode = pTempNode;
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

		/// <summary>
		/// 当前遍历(插入)的节点
		/// </summary>
		RedBlackNode* m_Current{ nullptr };
		/// <summary>
		/// 当前遍历(插入)的父节点
		/// </summary>
		RedBlackNode* m_Parent{ nullptr };
		/// <summary>
		/// 当前遍历(插入)的祖父节点
		/// </summary>
		RedBlackNode* m_Grand{ nullptr };
		/// <summary>
		/// 当前遍历(插入)的曾祖父节点
		/// </summary>
		RedBlackNode* m_Great{ nullptr };

	};
}
