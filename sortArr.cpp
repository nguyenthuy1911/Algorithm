//Ascending array sort algorithm

#include<iostream>
using namespace std;

//swap a,b
void swap(int *a, int *b){
	int c = *a;
		*a = *b;
		*b = c;
}

//selection sort
void selectionSort(int arr[], int n){
	for( int i=0; i<n-1; i++){
		int min_index = i;
		for (int j=i+1; j<n; j++){
			if (arr[j] < arr[min_index]){
				min_index = j;
			}
		}
		swap(&arr[i], &arr[min_index]);
	}	
}

//bubble sort
void bubbleSort(int arr[], int n){
	bool haveSwap;
	for(int i=0; i<n-1; i++){
		haveSwap = false;
		for (int j=0; j<n-1-i; j++){
			if (arr[j] > arr[j+1]){
				swap(&arr[j], &arr[j+1]);
				haveSwap = true;
			}
		}
		//sorted array
		if (haveSwap == false){
			break;
		}
	}
}

//insertion sort
void insertionSort(int arr[], int n){
	for ( int i=1; i<n; i++){
		 int key = arr[i];	
		 int j = i-1;
		 
		 while(j>=0 && arr[j]>key){
		 	arr[j+1] = arr[j];
		 	j--;
		 }
		 arr[j+1] = key;	
	}
}

//quick sort
	//array segment
int partition(int arr[], int low, int high){
	int privot = arr[high];	//choose privot as the last element of the array
	int left = low;
	int right = high - 1;
	while(true){
		while(left<=right && arr[left]<privot){
			left++;
		}
		while(left<=right && arr[right]>privot){
			right--;
		}
		if (left>=right) break;
		if (arr[left]>privot) swap(&arr[left], &arr[right]);
		right--;
		left++;
	}	
	swap(&arr[left], &arr[high]);
	return left;
}

int quickSort(int arr[], int low, int high){
	if (low < high){
		int center = partition(arr, low, high);
		quickSort(arr, low, center -1);
		quickSort(arr, center+1, high);
	}
}

//heap sort
	//create heap for a binary tree
void adjust(int arr[], int n, int i){
	int root = i;
	int left = 2*i + 1;
	int right = 2*i + 2;
	if (left<n && arr[left]>arr[root]){
		root = left;
	}
	if (right<n && arr[right]>arr[root]){
		root = right;
	}
	if (root !=i){
		swap(&arr[i], &arr[root]);
		adjust(arr, n, root);
	}
}

void heapSort(int arr[], int n){
	for (int i=(n/2)-1; i>=0; i--){
		adjust(arr, n, i);
	}
	for(int i=n-1; i>=0; i--){
		swap(&arr[0], &arr[i]);
		adjust(arr, i, 0);
	}
}

//merge sort
	//merge 2 array
void merge (int arr[], int l, int m, int r){
	int n1 = m - l + 1;		
	int n2 = r - m;		
	int L[n1],R[n2];	
	for(int i=0; i<n1; i++){
		L[i] = arr[l+i];
	}
	for(int i=0; i<n2; i++){
		R[i] = arr[m+1+i];
	}
	int i=0, j=0, k=l;
	// merge arr L arr R
	while (i<n1 && j<n2){
		if (L[i]>R[j]) {
			arr[k] = R[j];
			k++;
			j++;
		}
		else {
			arr[k] = L[i];
			k++;
			i++;
		}
	}
	
	while (i<n1){
		arr[k] = L[i];
		i++;
		k++;
	}
	
	while (j<n2){
		arr[k] = R[j];
		j++;
		k++;
	}
}	

void mergeSort(int arr[], int l, int r){
	if (l<r){
		int m = (r-l)/2+l; 	
		mergeSort(arr, l, m);
		mergeSort(arr, m+1, r);
		merge(arr, l, m, r);
	}
}

int main(){
	int arr[]={1,5,2,2,9,1,-1,-12,2,6,81,-2,91,92,93};
	int length = sizeof(arr)/sizeof(int);
	//selectionSort(arr, length);
	//bubbleSort(arr, length);
	//insertionSort(arr, length);
	//quickSort(arr,0,length-1);
	//heapSort(arr,length);
	mergeSort(arr, 0, length);
	for ( int i=0; i<length; i++){
		cout<<arr[i]<<" ";
	}
	return 0;
}

