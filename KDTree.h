#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

struct CPoint {
	CPoint() {};

	CPoint(const vector<double> newPoint, const size_t num) :
		point(newPoint), numberOfPoint(num) {};

	vector<double> point;
	size_t numberOfPoint;
};

struct KDNode {
	KDNode() :
		left(NULL), right(NULL) {};

	KDNode(const CPoint& newBottom, const CPoint& newTop) :
		bottom(newBottom), top(newTop), left(NULL), right(NULL) {};

	KDNode(KDNode* newLeft, KDNode* newRight) :
		left(newLeft), right(newRight) {}

	vector<CPoint> points;  // if we have more than one point in leaf
	CPoint bottom;  //    i can make rectangle
	CPoint top;     //	if i know these points
	KDNode* left;
	KDNode* right;
};

void ReadPoints(vector<CPoint>& points) {
	size_t n, d;
	cin >> n >> d;
	for (size_t i = 0; i < n; ++i) {
		CPoint currPoint;
		for (size_t j = 0; j < d; ++j) {
			double coordinate;
			cin >> coordinate;
			currPoint.point.push_back(coordinate);
		}
		currPoint.numberOfPoint = i;
		points.push_back(currPoint);
	}
}

double GetDistanceBetweenPoints(const CPoint& first, const CPoint& second) {
	double result = 0;
	for (size_t i = 0; i < first.point.size(); ++i) {
		result += (first.point[i] - second.point[i]) * (first.point[i] - second.point[i]);
	}
	return sqrt(result);
}

CPoint GetPoint(const char mode, const vector<CPoint>& points) {
	CPoint result = points[0];
	for (size_t i = 1; i < points.size(); ++i) {
		CPoint currPoint = points[i];
		for (size_t j = 0; j < currPoint.point.size(); ++j) {
			if (currPoint.point[j] < result.point[j]) {
				if (mode == 'b') {
					result.point[j] = currPoint.point[j];
				}
			}
			else {
				if (mode == 't') {
					result.point[j] = currPoint.point[j];
				}
			}
		}
	}
	return result;
}

size_t FindLongestEdgeDimension(const CPoint& bottom, const CPoint& top) {
	size_t dim = 0;
	double length = top.point[0] - bottom.point[0];
	for (size_t i = 1; i < top.point.size(); ++i) {
		if (top.point[i] - bottom.point[i] > length) {
			length = top.point[i] - bottom.point[i];
			dim = i;
		}
	}
	return dim;
}

void GetPointParts(vector<CPoint>& leftPoints, vector<CPoint>& rightPoints, const vector<CPoint>& points, const KDNode* currNode) {
	CPoint bottom = currNode->bottom;
	CPoint top = currNode->top;
	// we should choose from points- what we take in one side and what in the other
	size_t dim = FindLongestEdgeDimension(bottom, top);
	vector<pair<double, size_t > > orderedPoints;
	for (size_t i = 0; i < points.size(); ++i) {
		orderedPoints.push_back(make_pair(points[i].point[dim], i)); // we will sort on this dimension ,
	}									                            // to make division more efficent
	sort(orderedPoints.begin(), orderedPoints.end());
	size_t num = points.size() / 2;
	for (size_t i = 0; i < num; ++i) {                // first half will be in left points
		CPoint newPoint = points[orderedPoints[i].second];
		leftPoints.push_back(newPoint);
	}
	for (size_t i = num; i < points.size(); ++i) {     // second half will be in right points
		CPoint newPoint = points[orderedPoints[i].second];
		rightPoints.push_back(newPoint);
	}
}

void PrintPoint(const CPoint& point) {
	for (size_t i = 0; i < point.point.size(); ++i) {
		cout << point.point[i] << " ";
	}
	cout << endl;
}

KDNode* BuildKDTree(const vector<CPoint>& points) {
	CPoint bottom = GetPoint('b', points);
	CPoint top = GetPoint('t', points);
	KDNode* node = new KDNode(bottom, top);
	if (points.size() == 1) {   // ONE points in each rectangle
		node->points = points;
		return node;
	}
	vector<CPoint> leftPoints;
	vector<CPoint> rightPoints;
	GetPointParts(leftPoints, rightPoints, points, node);
	node->left = BuildKDTree(leftPoints);
	node->right = BuildKDTree(rightPoints);
	return node;
}

CPoint getMidPoint(const KDNode* node) {
	CPoint bottom = node->bottom;
	CPoint top = node->top;
	CPoint mid;
	for (size_t i = 0; i < bottom.point.size(); ++i) {
		mid.point.push_back((bottom.point[i] + top.point[i]) / 2);
	}
	return mid;
}

void GetFirstDistance(const KDNode* node, const CPoint& aim, pair<size_t, double>& point) { 
	if (node->left == NULL && node->right == NULL) { // we need just only some point from this rectangle
		pair<size_t, double> currPoint;
		currPoint.first = node->points[0].numberOfPoint;
		currPoint.second = GetDistanceBetweenPoints(aim, node->points[0]);
		point = currPoint;
		return;
	}
	CPoint leftPoint = getMidPoint(node->left);
	CPoint rightPoint = getMidPoint(node->right);
	if (GetDistanceBetweenPoints(leftPoint, aim) < GetDistanceBetweenPoints(rightPoint, aim)) {
		GetFirstDistance(node->left, aim, point);
	}
	else {
		GetFirstDistance(node->right, aim, point);
	}
}

bool RectanglesIntersect(const KDNode* rectangle, const CPoint& center, const double radius) {  // intersection rectangle with center 
	CPoint bottom = rectangle->bottom;                                                          // and radius around it
	CPoint top = rectangle->top;
	for (size_t i = 0; i < bottom.point.size(); ++i) {
		if (bottom.point[i] <= center.point[i] - radius && center.point[i] - radius <= top.point[i] ||
			bottom.point[i] <= center.point[i] + radius && center.point[i] + radius <= top.point[i]) {
			return true;
		}
	}
	return false;
}

void FindClosestPoint(const KDNode* node, const CPoint& aim, pair<size_t, double>& point) {
	if (node->left == NULL && node->right == NULL) {     // our aim must be there
		for (size_t i = 0; i < node->points.size(); ++i){
			double currDistance = GetDistanceBetweenPoints(node->points[i], aim);  
			if (currDistance < point.second) {
				point.first = node->points[0].numberOfPoint;
				point.second = currDistance;
			}
		}
		return;
	}
	if (RectanglesIntersect(node->left, aim, point.second)) {  // if our rectangle with center in aim intersects left rectangle, we should check it
		FindClosestPoint(node->left, aim, point);
	}
	if (RectanglesIntersect(node->right, aim, point.second)) {  // if our rectangle with center in aim intersects right rectangle, we should check it
		FindClosestPoint(node->right, aim, point);
	}
}

int QueryProcessing(const vector<CPoint>& points, const KDNode* root, CPoint & newPoint, int n) { // второй параметр - веткор рутов
	size_t dim = points[0].point.size();
	//cin >> n;
	for (size_t i = 0; i < n; ++i) {
		//size_t k;
		//cin >> k;
		/*CPoint newPoint;
		for (size_t j = 0; j < dim; ++j) {
			double coordinate;
			cin >> coordinate;
			newPoint.point.push_back(coordinate);
		}*/
		//for (size_t iter = 0; iter < k; ++iter) {
			pair<size_t, double> currPoint; // <number of point, distance to this point>
			/*первый параметр iter позиция в векторе рутов*/GetFirstDistance(root, newPoint, currPoint); // currPoint has some distance (? shortest)
			FindClosestPoint(root, newPoint, currPoint); // currPoint is now what we need
			return currPoint.first;
		//}
	}
}
