#include <iostream>
#include <vector>

int partition(std::vector<int> &arr, int l, int r)
{
    int v = arr[l];
    int j = l;
    for (int i = l + 1; i <= r; i++)
    {
        if (arr[i] < v)
        {
            j++;
            std::cout << "swap value: " << arr[i] << " " << arr[j]
                      << ", swap index: " << i << " " << j << std::endl;
            std::swap(arr[i], arr[j]);
            
            for(int m: arr){
                std::cout << m << " ";
            }
            std::cout << std::endl;
        }
    }
    //{3, 4, 5, 1, 2}
    std::cout << "swap value: " << arr[l] << " " << arr[j]
              << ", swap index: " << l << " " << j << std::endl;
    std::cout << "j: " << j << std::endl;
    std::swap(arr[l], arr[j]);
    
    return j;
}

void quicksort(std::vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int p = partition(arr, l, r);
        quicksort(arr, l, p - 1);
        quicksort(arr, p + 1, r);
    }
}

int main(int argc, char const *argv[])
{
    /* code */
    std::vector<int> arr = {3, 4, 5, 1, 2};

    quicksort(arr, 0, arr.size() - 1);

    for (int i : arr)
    {
        std::cout << i << " ";
    }

    return 0;
}
