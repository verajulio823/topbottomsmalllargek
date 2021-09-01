
#include <iostream>;
#include <math.h>;
#include <vector>;
#include <queue>;
#include <algorithm>;
#include <ctime>;
#include <fstream>;
#include <string>;

using namespace std;

#define DESCENDING 0
#define ASCENDING 1
#define QUICK_SORT 0
#define QUICK_SELECT 1
#define HEAP 2


void print_array(vector<int> v);
void array_pivot(vector<int> arr, int pivot, vector<int>& vleft, vector<int>& vright);
vector<int> create_array(int size);
vector<int> quick_sort(vector<int> arr);
vector<int> concat_array(vector<int> a, vector<int>b, vector<int> c);
int topk_element(vector<int>values, int k, int option );
int bottomk_element(vector<int>values, int k, int option );
vector<int> nlargest(vector<int> arr, int k, int option);
vector<int> nsmallest(vector<int> arr, int k, int option);
int select_k(vector<int> arr, int begin, int end, int k, int order);
vector<int> array_pivot(vector<int> arr, int n0, int n1, int pivot_val, bool isleft);
int partition(int pivot, vector<int> &arr, int  low, int  high, int order);
int heap_k(vector<int> arr, int k);
vector<int> vheap_k(vector<int> arr, int k);
vector<int> heap2vector(priority_queue<int> pq);


int main() {
	
	unsigned t0, t1;
	ofstream mycsv;
	mycsv.open("timescomplexity.csv");
	vector<int> arr;
	
	vector<string> vapproach = { "quicksort", "quick_select","heap" };
	vector<string> vmeth = { "bottomkelement", "topkelement","nsmallest", "nlargest"};

	for (int meth = 0; meth < vmeth.size(); meth++) {

		for (int approach = 0; approach < vapproach.size(); approach++) {

			for (int i = 2; i < 16; i++) {
				t0 = clock();

				int size_array = pow(2, i);
				arr = create_array(size_array);

				if(meth==0)
					int rpta = bottomk_element(arr, i / 2, approach);
				if(meth==1)
					int rpta = topk_element(arr, i / 2, approach);
				if(meth==2)
					vector<int> rpta = nsmallest(arr, i / 2, approach);
				if(meth==3)
					vector<int> rpta = nlargest(arr, i / 2, approach);
				
				t1 = clock();
				double time = (double(t1 - t0) / CLOCKS_PER_SEC);
				cout << "Execution Time for 2^" << i << " = " << size_array << " datos: " << time << endl;
				mycsv <<vmeth[meth] <<"," << vapproach[approach] << "," << i << "," << size_array << "," << time << endl;


			}
		}
	
	}

	

	

	mycsv.close();

	return 0;
}


vector<int> nlargest(vector<int> arr, int k, int option) {
	int kn = arr.size() - (k);
	if (option == QUICK_SORT) {
		vector<int> vsort = quick_sort(arr);
		vector<int> vresult{ vsort.begin()+kn, vsort.end()};
		return vresult;
	}
	if (option == QUICK_SELECT) {
		vector<int> vselect;
		int val;
		for (int i = arr.size()-1; i >= kn; i--) {
			val = select_k(arr, 0, arr.size() - 1, i, DESCENDING);
			vselect.push_back(val);
		}
		return  vselect;
	}

	if (option == HEAP) {
		return vheap_k(arr, (kn+1) % (arr.size() + 1));
	}
}

vector<int> nsmallest(vector<int> arr, int k, int option) {
	if (option == QUICK_SORT) {
		vector<int> vsort = quick_sort(arr);
		vector<int> vresult{ vsort.begin(), vsort.begin() + k };
		return vresult;
	}
	if (option == QUICK_SELECT) {
		vector<int> vselect;
		int val;
		for (int i = 0; i < k; i++) {
			val=select_k(arr, 0, arr.size() - 1, i, DESCENDING);
			vselect.push_back(val);
		}
		return  vselect;
	}

	if (option == HEAP) {
		return vheap_k(arr, k % (arr.size() + 1));
	}
}

int topk_element(vector<int>values, int k, int option) {
	int kn = values.size() - (k);

	if (option == QUICK_SORT) {
		vector<int> vsort = quick_sort(values);
		return vsort[kn];
	}
	if (option == QUICK_SELECT) {
		return select_k(values, 0, values.size() - 1, kn, DESCENDING);
	}

	if (option == HEAP) {
		return heap_k(values, (kn+1)%(values.size()+1));
	}
}

int bottomk_element(vector<int>values, int k, int option) {
	if (option == QUICK_SORT) {
		vector<int> vsort = quick_sort(values);

		return vsort[k - 1];
	}
	if (option == QUICK_SELECT) {
		return select_k(values, 0, values.size() - 1, k - 1, DESCENDING);
	}

	if (option == HEAP) {
		return heap_k(values, k % (values.size()+1));
	}
}

vector<int> vheap_k(vector<int> arr, int k) {
	priority_queue<int> heap;
	for (int i = 0; i < arr.size(); i++) {
		heap.push(arr[i]);
		if (heap.size() > k) {
			heap.pop();
		}
	}

	return heap2vector(heap);
	//return heap.top();
}

vector<int> heap2vector(priority_queue<int> pq) {
	vector<int> v;
	while (!pq.empty())
	{
		v.push_back(pq.top());
		pq.pop();
	}
	reverse(v.begin(), v.end());
	return v;
}

int heap_k(vector<int> arr, int k) {
	priority_queue<int> heap;
	for (int i = 0; i < arr.size(); i++) {
		heap.push(arr[i]);
		if (heap.size() > k) {
			heap.pop();
		}
	}
	return heap.top();
}

int select_k(vector<int> arr, int begin, int end, int k, int order) {
	if (begin == end)
		return arr[begin];
	else {
		int pivot = begin + rand() % (end-begin+1);
		pivot = partition(pivot, arr, begin, end, order);
		if (k == pivot)
			return arr[k];
		if (k < pivot)
			return select_k(arr, begin, pivot - 1, k, order);
		else
			return select_k(arr, pivot + 1, end, k, order);
	}

}

int partition(int pivot, vector<int> &arr, int  low, int  high, int order) {
	int pivot_val = arr[pivot];
	int temp = arr[pivot];
	arr[pivot] = arr[low];
	arr[low] = temp;
	vector<int> left = array_pivot(arr, low + 1, high + 1, pivot_val, true);
	vector<int> right = array_pivot(arr, low + 1, high + 1, pivot_val, false);

	for (int i = 0; i < left.size(); i++) 
		arr[low + i] = left[i];
	
	int new_pivot = low + left.size();
	arr[new_pivot] = pivot_val;
	
	for (int i = 0; i < right.size(); i++)
		arr[new_pivot+1 +i] = right[i];

	return new_pivot;
}

vector<int> array_pivot(vector<int> arr, int n0, int n1, int pivot_val, bool isleft) {
	vector<int> result;
	for (int i = n0; i < n1; i++) {
		if (isleft) {
			if (arr[i] < pivot_val) {
				result.push_back(arr[i]);
			}
		}
		else {
			if (arr[i] >= pivot_val) {
				result.push_back(arr[i]);
			}
		
		}
		
	}
	return result;
}

void print_array(vector<int> v) {
	for (int i = 0; i < v.size(); i++) {
		cout << v[i]<<" ";
	}
	cout<< endl;

}

vector<int> quick_sort(vector<int> arr) {
	int pivot;
	vector<int> vleft, vright, vmid;

	if (arr.size() < 1)
		return arr;
	
	pivot = rand() % arr.size();
	array_pivot(arr, pivot, vleft, vright);
	vmid.push_back(arr[pivot]);

	return concat_array(quick_sort(vleft) , vmid ,quick_sort(vright));

}

vector<int> concat_array(vector<int> a, vector<int>b, vector<int> c) {
	int i;
	for (i = 0; i < b.size(); i++) {
		a.push_back(b[i]);
	}
	for (i = 0; i < c.size(); i++) {
		a.push_back(c[i]);
	}
	return a;
}

void array_pivot(vector<int> arr, int pivot, vector<int>& vleft, vector<int>& vright) {
	for (int i = 0; i < arr.size(); i++) {
		if (pivot != i) {

				if (arr[i] < arr[pivot])
					vleft.push_back(arr[i]);
			
				if (arr[i] >= arr[pivot])
					vright.push_back(arr[i]);
		}
	}
	
}

vector<int> create_array(int size) {
	vector<int> v;
	for (int i = 0; i < size; i++) {
		v.push_back( rand() % size);
	}
	return v;
}