#include <iostream>
#include <vector>
#include "Node.h"
#include "AstarNode.h"
#include <cmath>
#include <float.h>
using namespace std;

static vector<AstarNode> openNodesList;
static vector<AstarNode> closeNodesList;
vector<vector<Node>> nodeMap;
static shared_ptr<Node> starNode;
static shared_ptr<Node> endNode;
shared_ptr<AstarNode> curCheckNode;

void CalculatePath(int map[][5], int width, int height, int resultX[], int resultY[]);
float DisOfNode(shared_ptr<Node> node1, shared_ptr<Node> node2);
void CheckNeighborNode(shared_ptr<AstarNode> centerNode, int width, int height);
shared_ptr<AstarNode> UpdateOpenCloseNodeList();

void CalculatePath(int map[][5], int width, int height, int resultX[], int resultY[]) {
	openNodesList.clear();
	closeNodesList.clear();

	int count = 0;

	for (int i = 0; i < width; i++)
	{
		vector<Node> nodeRow;
		nodeMap.push_back(nodeRow);
		for (int j = 0; j < height; j++) {
			int type = map[i][j];
			Node curNode = *new Node(type, i, j);
			nodeMap[i].push_back(curNode);
			if (type == 1) {
				starNode = make_shared<Node>(type, i, j);
			}
			else if (type == 2) {
				endNode = make_shared<Node>(type, i, j);
			}
		}
	}

	AstarNode startAstarNode((float)0, DisOfNode(starNode, endNode), nullptr, starNode->x, starNode->y);
	AstarNode endAstarNode(DisOfNode(starNode, endNode), 0, nullptr, endNode->x, endNode->y);

	closeNodesList.push_back(startAstarNode);
	curCheckNode = make_shared<Node>(startAstarNode);

	while (curCheckNode != nullptr && (curCheckNode->x != endAstarNode.x || curCheckNode->y != endAstarNode.y))
	{
		CheckNeighborNode(curCheckNode, width, height);
		curCheckNode = UpdateOpenCloseNodeList();
	}
	cout << "closeNodesList" << endl;
	for (auto itBegin = closeNodesList.begin(); itBegin != closeNodesList.end(); itBegin++) {
		cout << (*itBegin).x << "," << (*itBegin).y << endl;
	}

	//结果
	int resultCount = 0;
	if (closeNodesList.back().x == endNode->x && closeNodesList.back().y == endNode->y) {
		shared_ptr<AstarNode> curNode = closeNodesList.back().parentNode;
		while (curNode->x != starNode->x || curNode->y != starNode->y)
		{
			resultX[resultCount] = curNode->x;
			resultY[resultCount] = curNode->y;
			resultCount++;
			curNode = curNode->parentNode;
		}
	}
}

/// <summary>
/// 检查周围邻居节点
/// </summary>
/// <param name="centerNode"></param>
void CheckNeighborNode(shared_ptr<AstarNode> centerNode, int width, int height) {
	int centerX = (*centerNode).x;
	int centerY = (*centerNode).y;
	bool isTopObstacle = false;
	bool isBottomObstacle = false;
	bool isLeftObstacle = false;
	bool isRightObstacle = false;

	//下
	if (centerY + 1 < height) {
		auto it = nodeMap[centerX].begin() + centerY + 1;
		shared_ptr<Node> bottom = make_shared<Node>(*it);
		if (bottom->type != 3) {
			float disFromStart = centerNode->g + 1;
			float disFromEnd = DisOfNode(bottom, endNode);
			AstarNode aStarNodeBottom(disFromStart, disFromEnd, centerNode, centerX, centerY + 1);
			openNodesList.push_back(aStarNodeBottom);
		}
		else
		{
			isTopObstacle = true;
		}
	}

	//上
	if (centerY - 1 >= 0) {
		auto it = nodeMap[centerX].begin() + centerY - 1;
		shared_ptr<Node> top = make_shared<Node>(*it);;
		if (top->type != 3) {
			float disFromStart = centerNode->g + 1;
			float disFromEnd = DisOfNode(top, endNode);
			AstarNode aStarNodeTop(disFromStart, disFromEnd, centerNode, centerX, centerY - 1);
			openNodesList.push_back(aStarNodeTop);
		}
		else
		{
			isBottomObstacle = true;
		}
	}

	//左
	if (centerX - 1 >= 0) {
		auto it = nodeMap[centerX-1].begin() + centerY;
		shared_ptr<Node> left = make_shared<Node>(*it);
		if (left->type != 3) {
			float disFromStart = centerNode->g + 1;
			float disFromEnd = DisOfNode(left, endNode);
			AstarNode aStarNodeLeft(disFromStart, disFromEnd, centerNode, centerX - 1, centerY);
			openNodesList.push_back(aStarNodeLeft);
		}
		else
		{
			isLeftObstacle = true;
		}
	}

	//右
	if (centerX + 1 < width) {
		auto it = nodeMap[centerX + 1].begin() + centerY;
		shared_ptr<Node> right = make_shared<Node>(*it);
		if (right->type != 3) {
			float disFromStart = centerNode->g + 1;
			float disFromEnd = DisOfNode(right, endNode);
			AstarNode aStarNodeRight(disFromStart, disFromEnd, centerNode, centerX + 1, centerY);
			openNodesList.push_back(aStarNodeRight);
		}
		else
		{
			isRightObstacle = true;
		}
	}

	//左下
	if (centerX - 1 >= 0 && centerY + 1 < height && !(isTopObstacle && isLeftObstacle)) {
		auto it = nodeMap[centerX - 1].begin() + centerY + 1;
		shared_ptr<Node> leftBottom = make_shared<Node>(*it);
		if (leftBottom->type != 3) {
			float disFromStart = centerNode->g + 1.4f;
			float disFromEnd = DisOfNode(leftBottom, endNode);
			AstarNode aStarNodeLeftBottom(disFromStart, disFromEnd, centerNode, centerX - 1, centerY + 1);
			openNodesList.push_back(aStarNodeLeftBottom);
		}
	}

	//左上
	if (centerX - 1 >= 0 && centerY - 1 >= 0 && !(isBottomObstacle && isLeftObstacle)) {
		auto it = nodeMap[centerX - 1].begin() + centerY - 1;
		shared_ptr<Node> leftTop = make_shared<Node>(*it);
		if (leftTop->type != 3) {
			float disFromStart = centerNode->g + 1.4f;
			float disFromEnd = DisOfNode(leftTop, endNode);
			AstarNode aStarNodeLeftTop(disFromStart, disFromEnd, centerNode, centerX - 1, centerY - 1);
			openNodesList.push_back(aStarNodeLeftTop);
		}
	}

	//右下
	if (centerX + 1 < width && centerY + 1 < height && !(isTopObstacle && isRightObstacle)) {
		auto it = nodeMap[centerX + 1].begin() + centerY + 1;
		shared_ptr<Node> rightBottom = make_shared<Node>(*it);
		if (rightBottom->type != 3) {
			float disFromStart = centerNode->g + 1.4f;
			float disFromEnd = DisOfNode(rightBottom, endNode);
			AstarNode aStarNodeRightBottom(disFromStart, disFromEnd, centerNode, centerX + 1, centerY + 1);
			openNodesList.push_back(aStarNodeRightBottom);
		}
	}

	//右上
	if (centerX + 1 < width && centerY - 1 >= 0 && !(isBottomObstacle && isRightObstacle)) {
		auto it = nodeMap[centerX + 1].begin() + centerY - 1;
		shared_ptr<Node> rightTop = make_shared<Node>(*it);
		if (rightTop->type != 3) {
			float disFromStart = centerNode->g + 1.4f;
			float disFromEnd = DisOfNode(rightTop, endNode);
			AstarNode aStarNodeLeftTop(disFromStart, disFromEnd, centerNode, centerX + 1, centerY - 1);
			openNodesList.push_back(aStarNodeLeftTop);
		}
	}
}

/// <summary>
/// 更新关闭列表和开启列表
/// </summary>
/// <param name="curCheckNode"></param>
shared_ptr<AstarNode> UpdateOpenCloseNodeList() {
	shared_ptr<AstarNode> mincostNode;
	float curCost = FLT_MAX;
	//选择开启列表中消耗最小的点移动到关闭列表，要是为Null,curCheckNode = null
	for (auto itBegin = openNodesList.begin(); itBegin != openNodesList.end(); itBegin++) {
		if ((*itBegin).GetCost() < curCost) {
			curCost = (*itBegin).GetCost();
			mincostNode = make_shared<AstarNode>(*itBegin);
		}
	}

	for (auto it = openNodesList.begin(); it != openNodesList.end(); it++) {
		if ((*it).x == mincostNode->x && (*it).y == mincostNode->y) {
			it = openNodesList.erase(it);
			break;
		}
	}
	closeNodesList.push_back(*mincostNode);
	return mincostNode;
}

/// <summary>
/// 两个节点的距离
/// </summary>
/// <param name="node1"></param>
/// <param name="node2"></param>
/// <returns></returns>
float DisOfNode(shared_ptr<Node> node1, shared_ptr<Node> node2) {
	float result = abs(node1->x - node2->x) + abs(node1->y - node2->y);
	return result;
}

int main()
{
	int map[5][5] = { {0,0,0,0,0},
					  {0,1,0,0,0},
					  {0,3,3,3,0},
					  {0,0,0,3,0},
					  {0,2,0,0,0} };
	int resultX[25] = {0};
	int resultY[25] = { 0};
	CalculatePath(map,5,5, resultX, resultY);
	for (int i = 0; i < 5; i++) {
		for(int j = 0; j < 5; j++) {
			cout << nodeMap[i][j].type<<" ";
		}
		cout << endl;
	}
	cout << "结果" << endl;
	for (int i = 0; i < 25; i++) {
		if (resultX[i] == 0 && resultY[i] == 0) {
			continue;
		}
		cout << "(" << resultX[i] << "," << resultY[i] << ")"<<endl;
	}
}