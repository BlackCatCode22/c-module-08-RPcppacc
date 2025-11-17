#include <iostream> // This header is needed for input/output, like printing to the console (std::cout)

/**
 * @brief A helper function to print the elements of an integer array.
 * @param arr The array to be printed.
 * @param size The number of elements in the array.
 */
void printArray(int arr[], int size) {
    // Loop through each element from index 0 up to (but not including) size
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " "; // Print the element followed by a space
    }
    std::cout << std::endl; // Print a newline character at the end
}

// The main function is the entry point of every C++ program
int main() {
    // 1. Initialize the Array
    // This is the array of ten numbers you want to sort
    int arr[] = {2, 3, 6, 10, 4, 1, 5, 8, 9, 7};

    // 2. Determine the Size of the Array
    // sizeof(arr) gives the total size of the array in bytes (e.g., 10 ints * 4 bytes/int = 40 bytes)
    // sizeof(arr[0]) gives the size of a single element (e.g., 4 bytes for one int)
    // Dividing them gives the number of elements in the array (40 / 4 = 10)
    int n = sizeof(arr) / sizeof(arr[0]);

    // Print the original, unsorted array
    std::cout << "Unsorted array: ";
    printArray(arr, n);

    // --- Bubble Sort Algorithm ---

    // 3. The Outer Loop (Controls the number of passes)
    // For a list of 'n' items, we need at most 'n - 1' passes
    // to sort it. 'i' tracks the number of elements that are
    // already sorted and "bubbled" to the end.
    for (int i = 0; i < n - 1; i++) {

        // 4. The Inner Loop (Performs comparisons and swaps)
        // This loop compares adjacent elements 'j' and 'j + 1'.
        //
        // Optimization: 'n - i - 1'
        // - We subtract 'i' because after 'i' passes, the last 'i'
        //   elements are already in their correct, final sorted position.
        //   There is no need to check them again.
        // - We subtract '1' because we are comparing 'j' with 'j + 1',
        //   so this loop must stop one index early to prevent
        //   'j + 1' from going out of the array's bounds.
        for (int j = 0; j < n - i - 1; j++) {

            // 5. The Comparison
            // Check if the current element is GREATER than the next one.
            // If it is, they are in the wrong order.
            if (arr[j] > arr[j + 1]) {

                // 6. The Swap
                // We use a 'temp' (temporary) variable to hold one
                // value while we move the other, so no data is lost.

                // Store the left value
                int temp = arr[j];

                // Overwrite the left value with the right value
                arr[j] = arr[j + 1];

                // Overwrite the right value with the stored (original left) value
                arr[j + 1] = temp;
            }
        }
        // Optional: Uncomment the lines below to see the
        // state of the array after each pass of the outer loop.
        // std::cout << "After pass " << i + 1 << ": ";
        // printArray(arr, n);
    }

    // --- End of Bubble Sort ---

    // Print the final, sorted array
    std::cout << "Sorted array:   ";
    printArray(arr, n);

    return 0; // Tell the operating system the program finished successfully
}