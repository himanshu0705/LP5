#include <iostream>
#include <omp.h>

using namespace std;

void SequentialBubble(int *, int);
void ParallelBubble(int *, int);
void swap(int &, int &);
void mergesort(int a[], int i, int j);
void merge(int a[], int i1, int j1, int i2, int j2);

void SequentialBubble(int *a, int n)
{
    int swapped;
    for(int i=0; i<n; i++)
    {
        swapped = 0;
        for(int j=0; j<n-1; j++)
        {
            if(a[j]>a[j+1])
            {
                swap(a[j],a[j+1]);
                swapped = 1;
            }
        }
        if(!swapped)
        {
            break;
        }
    }
}

void ParallelBubble(int *a, int n)
{
    int swapped;
    for(int i=0; i<n; i++)
    {
        swapped = 0;
        int first=i%2;
        #pragma omp parallel for shared(a,first)
        for(int j=0; j<n-1; j++)
        {
            if(a[j]>a[j+1])
            {
                swap(a[j],a[j+1]);
                swapped = 1;
            }
        }
        if(!swapped)
        {
            break;
        }
    }
}

void swap(int &a, int &b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void mergesort(int a[], int i, int j)
{
    int mid;
    if(i<j)
    {
        mid = (i+j)/2;
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergesort(a,i,mid);
            }
            #pragma omp section
            {
                mergesort(a,mid+1,j);
            }
        }
        merge(a,i,mid,mid+1,j);
    }
}

void merge(int a[], int i1, int j1, int i2, int j2)
{
    int temp[1000];
    int i,j,k;
    i = i1;
    j = i2;
    k = 0;

    while(i<=j1 && j<=j2)
    {
        if(a[i]<a[j])
        {
            temp[k++] = a[i++];
        }
        else
        {
            temp[k++] = a[j++];
        }
    }

    while(i<=j1)
    {
        temp[k++] = a[i++];
    }

    while(j<=j2)
    {
        temp[k++] = a[j++];
    }

    for(i=i1,j=0; i<=j2; i++,j++)
    {
        a[i] = temp[j];
    }
}

int main()
{
    int *a,n;
    cout<<"\nEnter total number of elements : ";
    cin>>n;
    a = new int[n];
    cout<<"\nEnter Elements : \n";
    for(int i=0; i<n; i++)
    {
        cin>>a[i];
    }
    cout<<"\n\nSequential Bubble Sort";
    double start = omp_get_wtime();
    SequentialBubble(a,n);
    double end = omp_get_wtime();
    cout<<"\nSorted Elements : ";
    for(int i=0 ;i<n; i++)
    {
        cout<<"\t"<<a[i];
    }
    cout<<"\nTime taken by Sequential Bubble Sort : "<<end - start<<" sec\n";
    
    int *b = new int[n];
    copy(a, a+n, b);
    cout<<"\n\nParallel Bubble Sort";
    double start_time = omp_get_wtime();
    ParallelBubble(b,n);
    double end_time = omp_get_wtime();
    cout<<"\nSorted Elements : ";
    for(int i=0 ;i<n; i++)
    {
        cout<<"\t"<<b[i];
    }
    cout<<"\nTime taken by Parallel Bubble Sort : "<<end_time - start_time<<" sec\n";
    
    int *c = new int[n];
    copy(a, a+n, c);
    cout<<"\n\nSequential Merge Sort";
    double starttime = omp_get_wtime();
    mergesort(c,0,n-1);
    double endtime = omp_get_wtime();
    cout<<"\nSorted Elements : ";
    for(int i=0 ;i<n; i++)
    {
        cout<<"\t"<<c[i];
    }
    cout<<"\nTime taken by Sequential Merge Sort : "<<endtime - starttime<<" sec\n";
    
    int *d = new int[n];
    copy(a, a+n, d);
    cout<<"\n\nParallel Merge Sort";
    double start_tm = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        {
            mergesort(d,0,n-1);
        }
    }
    double end_tm = omp_get_wtime();
    cout<<"\nSorted Elements : ";
    for(int i=0 ;i<n; i++)
    {
        cout<<"\t"<<d[i];
    }
    cout<<"\nTime taken by Parallel Merge Sort : "<<end_tm - start_tm<<" sec\n";
    delete[] a,b,c,d;
    return 0;
}