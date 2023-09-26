/**
 * @file main.cc
 * @author Mason Coles (mascoles@iu.edu)
 * @brief Assn #1 : Selection Sort
 * @date 2023-09-06
 * 
 * @note max value  : 18,446,744,073,709,551,615
 * @note max length : 255
 * 
 */

#include <iostream>

void SelectionSort(uint64_t* arr, int n)
{
    int min; // holds the index of the min
    for (int i = 0; i < n; i++)
    {
        // assume min is the current #
        min = i;
        for (int j = i + 1; j < n; j++)
            // for all remaining #s, if the # is lower than
            // current min, reassign the min to the new value
            if (arr[j] > arr[min])  min =  j;

        // if the min wasn't the current #, swap the two
        if (min != i)
        {
            uint64_t cur = arr[i];
            arr[i] = arr[min];
            arr[min] = cur;
        }
    }
}

void PrintArray(uint64_t* arr, int n)
{   // I think this function is pretty self-explanatory
    for (int i = 0; i < n; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

int main(const int argc, const char** argv)
{
    // check arg count
    if (argc < 3 || argc > 256)
    {
        std::cerr << "Too few arguments!\n";
        exit(1);
    }

    // convert args to uint64_t[]
    uint64_t inVars[argc - 1];
    for (int i = 1; i < argc; i++)
    {
        inVars[i - 1] = atoi(argv[i]);
    }

    // sort the array
    SelectionSort(&inVars[0], argc - 1);
    // print the sorted array
    PrintArray(&inVars[0], argc - 1);

    return 0;
}