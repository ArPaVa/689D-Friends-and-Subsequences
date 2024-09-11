/*
You have two integer sequences, a and b, both of length n. For a pair of indices (l, r) (where 1 ≤ l ≤ r ≤ n):

- You can calculate the maximum value in sequence a from index l to r: max(a[l...r]).
- You can calculate the minimum value in sequence b from index l to r: min(b[l...r]).

You need to find how many times these two values are equal across all possible (l, r) pairs.

# Input
1. An integer n (1 ≤ n ≤ 200,000) indicating the length of the sequences.
2. A line containing n integers representing sequence a: a1, a2, ..., an (−10^9 ≤ ai ≤ 10^9).
3. A line containing n integers representing sequence b: b1, b2, ..., bn (−10^9 ≤ bi ≤ 10^9).

# Output
- Print a single integer: the count of pairs (l, r) such that max(a[l...r]) == min(b[l...r]).
*/
#include <bits/stdc++.h> 
using namespace std; 

const int MAX_SIZE = 200005;  // limit for array size 
struct Node {
    long long maxValue;
    long long minValue;

    // Default constructor initializes max and min to extreme values
    Node() { 
        maxValue=-1e16; 
        minValue=1e16; 
    }
    // Parameterized constructor
    Node(long long a, long long b){
        maxValue = a;
        minValue = b;
    }
};
Node join(Node node1, Node node2) {
    // Combines two Node instances by taking the maximum of their maxValue fields and the minimum of their minValue fields.
    return Node(max(node1.maxValue, node2.maxValue), min(node1.minValue, node2.minValue));
}
struct SegmentTree {
    long long lowIndex[MAX_SIZE<<2], highIndex[MAX_SIZE<<2], arrA[MAX_SIZE], arrB[MAX_SIZE];
    Node treeValues[MAX_SIZE<<2];

    void buildSegmentTree(long long nodeIndex, long long left, long long right) {   
        // Recursively constructs the segment tree. If the specified range is a leaf (i.e., left == right), it initializes the segment with 
        // the corresponding values from arrA and arrB. Otherwise, it splits the range and builds left and right subtrees and combines their results.
        lowIndex[nodeIndex] = left;
        highIndex[nodeIndex] = right;
        if(left == right) { // Leaf node
            treeValues[nodeIndex] = Node(arrA[left], arrB[left]);
            return;
        }
        long long mid = (left + right) >> 1; // (left + right) / 2 // Find mid-point
        long long leftChildIndex = nodeIndex << 1; // nodeIndex * 2
        long long rightChildIndex = leftChildIndex | 1;      // if (leftChildIndex % 2 == 0) { rightChildIndex = leftChildIndex + 1; } else {rightChildIndex = leftChildIndex; }
        
        // Recursively build the left and right subtrees
        buildSegmentTree(leftChildIndex, left, mid);
        buildSegmentTree(rightChildIndex, mid + 1, right);   

        // calculate the max and the min from the two child nodes
        treeValues[nodeIndex] = join(treeValues[leftChildIndex], treeValues[rightChildIndex]);
        return;
    }
    // Method for range query to get max and min within a range
    Node rangeQuery(long long nodeIndex, long long left, long long right) {
        // Allows for querying the maximum and minimum values over a specified range [left, right].
        // Checks if the current segment is out of the specified range and handles three main cases: fully included, partially included, or completely outside.
        
        // Get the current segment's bounds
        long long currentLeft = lowIndex[nodeIndex], currentRight = highIndex[nodeIndex];

        // If current segment is out of range
        if (currentRight < left || currentLeft > right)
            return Node();
        // If segment is completely within the specified range
        if (left <= currentLeft && currentRight <= right)
            return treeValues[nodeIndex];

        long long mid = (currentLeft + currentRight) >> 1; // (currentLeft + currentRight) / 2
        long long leftChildIndex = nodeIndex << 1;         // nodeIndex * 2
        long long rightChildIndex = leftChildIndex | 1;      // if (leftChildIndex % 2 == 0) { rightChildIndex = leftChildIndex + 1; } else {rightChildIndex = leftChildIndex; }
        
        // Recursively query the appropriate segment
        if (mid < left)
            return rangeQuery(rightChildIndex, left, right); // Query right child
        if (mid >= right)
            return rangeQuery(leftChildIndex, left, right); // Query left child
        
        // If it's partially in both, combine results from both sides
        return join(rangeQuery(leftChildIndex, left, right), rangeQuery(rightChildIndex, left, right));
    } 
    long long process(long long maxIndex){
        long long left = 1, right = maxIndex; // Initialize bounds for binary search
        long long mid, result = 1e16, result2 = maxIndex + 1;
        
        while(left <= right){
            mid = (left + right) >> 1; // (left + right) / 2
            Node current = rangeQuery(1, mid, maxIndex);
            if (current.maxValue > current.minValue)
                left = mid + 1; // Move right if max > min
            else {
                if (current.maxValue == current.minValue)
                    result = mid;
                right = mid - 1; // Move left if max < min
            }
        }
        if (result == 1e16)
            return 0;
        // Reset bounds for the second pass
        left = 1, right = maxIndex;

        while(left <= right){
            mid = (left + right) >> 1; // (left + right) / 2
            Node current = rangeQuery(1, mid, maxIndex);
            if (current.maxValue >= current.minValue)
                left = mid + 1; // Move right if max >= min
            else {
                result2 = mid;
                right = mid - 1; // Move left max < min
            }
        }
        return result2 - result;
    }
} segmentTree;

int main() {
    int n; // 1 <= n <= 200000
    long long result = 0;
    std::cin >> n;
    for (int i = 1; i <= n; i++)
        std::cin >> segmentTree.arrA[i]; // -10^9 <= a[i] <= 10^9
    for (int i = 1; i <= n; i++)
        std::cin >> segmentTree.arrB[i]; // -10^9 <= b[i] <= 10^9
    segmentTree.buildSegmentTree(1, 1, n);
    for (int maxIndex = 1; maxIndex <= n; maxIndex++)
        result += segmentTree.process(maxIndex);
    cout << result << endl;
    return 0; 
}
