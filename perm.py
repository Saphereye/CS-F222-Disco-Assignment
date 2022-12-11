def next_permutation(arr: list[int]):
    # Find the longest decreasing suffix
    i = len(arr) - 1
    while i > 0 and arr[i - 1] >= arr[i]:
        i -= 1
    if i <= 0:
        return []  # The array is already at the last permutation

    # Find the rightmost element that is greater than the pivot
    j = len(arr) - 1
    while arr[j] <= arr[i - 1]:
        j -= 1

    # Swap the pivot with the rightmost element that is greater than the pivot
    arr[i - 1], arr[j] = arr[j], arr[i - 1]

    # Reverse the suffix
    arr[i : ] = arr[ : i - 1 : -1]

    return arr

arr = [1, 2, 3, 4]
while(arr != []):
    print(arr)
    arr = next_permutation(arr)
