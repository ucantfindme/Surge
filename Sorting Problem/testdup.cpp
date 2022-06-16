#include<iostream>
using namespace std;
//Function to calculate a random integer from a given range
int rand_range(int x,int y){
    return x+rand()%(y-x+1);
}
//Quick-sort
int partition(int* arr, int st, int ed){
    double pv=arr[ed];
    int pvi=st;
    for(int i=st;i<ed;i++){
        if(arr[i]<=pv){
            swap(arr[i],arr[pvi]);
            pvi++;
        }
    }
    swap(arr[pvi],arr[ed]);
    return pvi;
}
void quicksort(int* arr, int st, int ed){
    if(st<ed){
        int pvt=partition(arr,st,ed);
        quicksort(arr,st,pvt-1);
        quicksort(arr,pvt+1,ed);
    }
}
pair<int ,int> partition3(int* arr, int st, int ed){
    double pv=arr[ed];                                                  //Select the last element as pivot
    int left=st,right=st,up=ed;                                         //initialize two-pointer from st and upper bound at ed the window size is 0 at the beginning(left==right)
    while(right<=up){                                                   //until our right pointer doesn't cross upper bound we set
        if(arr[right]<pv)swap(arr[right++],arr[left++]);                //if less than pivot we swap the value with left pointer and slide our window forward
        else if(arr[right]>pv)swap(arr[up--],arr[right]);               //if greater than pivot we swap the value with upper pointer and reduce the upperbound
        else right++;                                                   //if equal to our pivot we increase our window size, increment right pointer
    }
    return make_pair(left,right);                                       //return left and right pointer where elements below left are less than pivot and elements>=right are greater than pivot
}
void quicksort3(int* arr, int st, int ed){
    if(st<ed){                                                          //ensures there are atleast 2 elements in our selected range
        pair<int,int>p = partition3(arr,st,ed);                          //get the partition points
        quicksort3(arr,st,p.first-1);                                    //recursive call on left batch of pivot
        quicksort3(arr,p.second,ed);                                     //recursive call on right batch of pivot
    }
}
//To print the arrays
void print(double* arr,int n){
    for(int i=0;i<n;i++)cout<<*(arr+i)<<" ";
    cout<<"\n";
}
//Same main is used to calculate runtimes, number of comparisons, cases where one algo perform better than other etc..
int main(){
    int arr1[100000];
    int arr2[100000];
    srand(time(0));
    long long tot1=0,tot2=0;
    for(int p=0;p<250;p++){
        for(int i=0;i<100000;i++){
            arr1[i]=rand_range(0,90000);
            arr2[i]=arr1[i];
        }
        auto start = std::chrono::high_resolution_clock::now();
        quicksort(arr1,0,99999);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long long t1=std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        tot1+=t1;

        start = std::chrono::high_resolution_clock::now();
        quicksort3(arr2,0,99999);
        elapsed = std::chrono::high_resolution_clock::now() - start;
        t1=std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        tot2+=t1;
    }
    cout<<"Avg Runtime of 2-way partition: "<<(float) tot1/250<<"\n";
    cout<<"Avg Runtime of 3-way partition: "<<(float) tot2/250<<"\n";
}