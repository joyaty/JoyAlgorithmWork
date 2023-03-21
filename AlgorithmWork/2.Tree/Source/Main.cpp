
#include <iostream>
#include "JoyTree/JoyBinarySearchTree.h"
#include "JoyTree/JoyAVLTree.h"
#include "JoyTree/JoySplayTree.h"
#include "JoyTree/JoyRedBlackTree.h"

/// <summary>
/// 测试二叉搜索树
/// </summary>
void TestJoyBinarySearchTree()
{
	std::cout << "Initialize BinarySearchTree" << std::endl;
	Joy::BinarySearchTree<int> tree;
	tree.Insert(6);
	tree.Insert(4);
	tree.Insert(1);
	tree.Insert(3);
	tree.Insert(5);
	tree.Insert(8);
	tree.Insert(7);
	tree.Insert(10);
	tree.PrintTree();

	std::cout << "Clone BinarySearchTree:" << std::endl;
	Joy::BinarySearchTree<int> copyTree(tree);
	copyTree.PrintTree();

	std::cout << "Add new TreeNode: 9" << std::endl;
	tree.Insert(9);
	tree.PrintTree();

	std::cout << "FindMin: " << tree.FindMin() << std::endl;
	std::cout << "FindMax: " << tree.FindMax() << std::endl;
	std::cout << "Contain: 2 -> is " << tree.Contain(2) << std::endl;
	std::cout << "Contain: 7 -> is " << tree.Contain(7) << std::endl;

	std::cout << "Remove TreeNode: 10" << std::endl;
	tree.Remove(10);
	tree.PrintTree();

	std::cout << "Remove TreeNode: 7" << std::endl;
	tree.Remove(7);
	tree.PrintTree();

	std::cout << "Remove TreeNode: 4" << std::endl;
	tree.Remove(4);
	tree.PrintTree();

	std::cout << "Print Copy BinarySearchTree:" << std::endl;
	copyTree.PrintTree();
	std::cout << "Remove CopyTree: 6" << std::endl;
	copyTree.Remove(6);
	copyTree.PrintTree();
}

/// <summary>
/// 测试自平衡的二叉搜索树
/// </summary>
void TestAVLTree()
{
	std::cout << "Initialize AVLTree" << std::endl;
	Joy::JoyAVLTree<int> tree;
	tree.Insert(3);
	tree.Insert(2);
	tree.Insert(1);
	tree.PrintTree();
	Joy::JoyAVLTree<int> copyTree = tree;
	std::cout << "After insert 4" << std::endl;
	tree.Insert(4);
	tree.PrintTree();
	std::cout << "After insert 5" << std::endl;
	tree.Insert(5);
	tree.PrintTree();
	std::cout << "After insert 6" << std::endl;
	tree.Insert(6);
	tree.PrintTree();
	std::cout << "After insert 7" << std::endl;
	tree.Insert(7);
	tree.PrintTree();
	std::cout << "After insert 16" << std::endl;
	tree.Insert(16);
	tree.PrintTree();
	std::cout << "After insert 15" << std::endl;
	tree.Insert(15);
	tree.PrintTree();
	std::cout << "After insert 14" << std::endl;
	tree.Insert(14);
	tree.PrintTree();
	std::cout << "After insert 13" << std::endl;
	tree.Insert(13);
	tree.PrintTree();
	std::cout << "After insert 12" << std::endl;
	tree.Insert(12);
	tree.PrintTree();
	std::cout << "After insert 11" << std::endl;
	tree.Insert(11);
	tree.PrintTree();
	std::cout << "After insert 10" << std::endl;
	tree.Insert(10);
	tree.PrintTree();
	std::cout << "After insert 8" << std::endl;
	tree.Insert(8);
	tree.PrintTree();
	std::cout << "After insert 9" << std::endl;
	tree.Insert(9);
	tree.PrintTree();

	std::cout << "Print copy tree" << std::endl;
	copyTree.PrintTree();
}

/// <summary>
/// 测试伸展树
/// </summary>
void TestSplayTree()
{
	std::cout << "Initialize SplayTree" << std::endl;
	int elements[] = { 12, 5, 25, 20, 30, 15, 24, 13, 18, 16 };
	int count = sizeof(elements) / sizeof(int);
	Joy::JoySplayTree<int> tree(elements, count);
	tree.PrintTree();
	std::cout << "Contain 19 ? "<< tree.Contain(19) << std::endl;
	tree.PrintTree();
	std::cout << "Contain 18 ? " << tree.Contain(18) << std::endl;
	tree.PrintTree();
	std::cout << "Max: " << tree.FindMax() << std::endl;
	tree.PrintTree();
	std::cout << "Min: " << tree.FindMin() << std::endl;
	tree.PrintTree();

	std::cout << "Initialize Empty SplayTree" << std::endl;
	Joy::JoySplayTree<int> emptyTree;
	std::cout << "Max: " << emptyTree.FindMax() << std::endl;
	emptyTree.PrintTree();
	std::cout << "Min: " << emptyTree.FindMin() << std::endl;
	emptyTree.PrintTree();
}

/// <summary>
/// 测试红黑树
/// </summary>
void TestRedBlackTree()
{
	// TODO
}

int main()
{
	// TestJoyBinarySearchTree();
	// TestAVLTree();
	TestSplayTree();
	std::cin.get();
}