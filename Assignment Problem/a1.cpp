#include<iostream>
#include<queue>
using namespace std;
void print(int* arr,int n){
    for(int i=0;i<n;i++)cout<<i<<" : "<<arr[i]<<"\n";
}
void assign(){
    int wpl[4][4] = {{2,3,1,0},{2,1,3,0},{3,1,0,2},{1,2,3,0}};
    int mpl[4][4] = {{2,3,0,1},{3,1,0,2},{1,3,2,0},{0,3,2,1}};
    int n=4;
    int* top=new int[n];
    int* couples=new int[n];
    queue<int> unmarried;
    for(int i=0;i<n;i++){
        unmarried.push(i);
        couples[i]=-1;
        top[i]=0;
    }
    while(!unmarried.empty()){
        int man=unmarried.front();
        int woman=mpl[man][top[man]++];
        if(couples[woman]!=-1){
            int husband=couples[woman];
            bool highpriority;
            for(int i=0;i<n;i++){
                if(wpl[woman][i]==man){
                    highpriority=true;
                    break;
                }
                if(wpl[woman][i]==husband){
                    highpriority=false;
                    break;
                }
            }
            if(highpriority){
                couples[woman]=unmarried.front();
                unmarried.pop();
                unmarried.push(husband);
            }
        }
        else{
            couples[woman]=unmarried.front();
            unmarried.pop();
        }
    }
    print(couples,4);
}

int main(){
    assign();
}