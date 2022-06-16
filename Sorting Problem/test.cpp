#include<iostream>
using namespace std;

//Function to calculate a random integer from a given range
int rand_range(int x,int y){
    return x+rand()%(y-x+1);
}

//To print the arrays
void print(double* arr,int n){
    for(int i=0;i<n;i++)cout<<*(arr+i)<<" ";
    cout<<"\n";
}

//Quick-sort
pair<int ,int> partition(double* arr, int st, int ed){
    double pv=arr[ed];                                                  //Select the last element as pivot
    int left=st,right=st,up=ed;                                         //initialize two-pointer from st and upper bound at ed the window size is 0 at the beginning(left==right)
    while(right<=up){                                                   //until our right pointer doesn't cross upper bound we set
        if(arr[right]<pv)swap(arr[right++],arr[left++]);                //if less than pivot we swap the value with left pointer and slide our window forward
        else if(arr[right]>pv)swap(arr[up--],arr[right]);               //if greater than pivot we swap the value with upper pointer and reduce the upperbound
        else right++;                                                   //if equal to our pivot we increase our window size, increment right pointer
    }
    return make_pair(left,right);                                       //return left and right pointer where elements below left are less than pivot and elements>=right are greater than pivot
}
void quicksort(double* arr, int st, int ed){
    if(st<ed){                                                          //ensures there are atleast 2 elements in our selected range
        pair<int,int>p = partition(arr,st,ed);                          //get the partition points
        quicksort(arr,st,p.first-1);                                    //recursive call on left batch of pivot
        quicksort(arr,p.second,ed);                                     //recursive call on right batch of pivot
    }
}

//Randomized-Quicksort
pair<int,int> rpartition(double* arr, int st, int ed){
    int r=rand_range(st,ed);                                            //Selecting a random index to be used as pivot, rest is same as quicksort above
    double pv=arr[r];
    int left=st,right=st,up=ed;
    while(right<=up){
        if(arr[right]<pv)swap(arr[right++],arr[left++]);
        else if(arr[right]>pv)swap(arr[up--],arr[right]);
        else right++;
    }
    return make_pair(left,right);
}
void rquicksort(double* arr, int st, int ed){
    if(st<ed){
        pair<int,int>p=rpartition(arr,st,ed);
        rquicksort(arr,st,p.first-1);
        rquicksort(arr,p.second,ed);
    }
}

//Mergesort(iterative-without extra copying)
void merge(double* arr, double* arr1, long long st, long long mid, long long ed){
    long long x=st,y=mid+1,i=st;                                        //we consider the two ranges [st,mid] and [mid+1,ed] as two parts which we want to merge
    while(x<=mid && y<=ed){                                             //when there are elements in both ranges
        if(arr1[x]<=arr1[y])arr[i++]=arr1[x++];                         //if smaller element is in first range we copy it to arr from arr1 and increase the first array pointer
        else arr[i++]=arr1[y++];                                        //else we copy the element from second to arr and increase the second array pointer
    }
    while(x<=mid)arr[i++]=arr1[x++];                                    //we copy remaining elements as it is.
    while(y<=ed)arr[i++]=arr1[y++];
}
void mergesort(double* arr, double* arr1, long long n){
    bool flag=true;                                                     //use a boolean variable flag to copy alternatively
    long long i=2;                                                      //initialise minimum size of block which we break into 2 for merging
    while(i<=2*n){                                                      //until the block size greater than 2 times the size of array
        for(long long j=0;j<n;j+=i){                                    //take each block and apply merging on breaking at the middle
            if(j+i/2-1>=n)break;                                        //if the array range is smaller than half of the block size we break out og the loop
            if(flag)merge(arr,arr1,j,j+i/2-1,min(j+i-1,n-1));           //if flag==true we perform copying to arr from arr1
            else merge(arr1,arr,j,j+i/2-1,min(j+i-1,n-1));              //else we copy to arr1 from arr
        }
        i*=2;                                                           //we double the block size
        flag=!flag;                                                     //toggles the flag
    }
}

int main(){
    //We create arrays of fixed size and initialise values into them for each iteration randomly
    double arr[100];
    double arr1[100];
    //Setting up seed for random number generator
    srand(time(0));
    //variable that is sum of all runtimes
    long long tot=0;
    //Iterate for 25000times
    for(int p=0;p<25000;p++){
        //initializing the array with random values in [0,1]
        for(int i=0;i<100;i++){
            //Rand_max is the maximum value that rand can produce
            arr[i]=(double)rand()/RAND_MAX;
            arr1[i]=arr[i];
        }

        //record the clock before calling sorting method
        auto start = std::chrono::high_resolution_clock::now();
        //method-call
        mergesort(arr,arr1,100);
        //record the clock and subtract the first clock to get approximate runtime
        auto elapsed = std::chrono::high_resolution_clock::now() - start;

        //extracting timelapse in microseconds
        long long t1=std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        tot+=t1;
    }
    //Print the average runtime
    cout<<"Avg Runtime: "<<(float) tot/25000<<"\n";
}