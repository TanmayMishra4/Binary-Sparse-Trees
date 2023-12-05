Overview:
The Extension uses Hash Map as the data structure. For the hashing function, I have used a fairly simple algorithm
that takes the remainder with the number of buckets to get the bucket to put the number in.
I have used chaining method in Hash Map with an improvement that the Hash Map instead of using Linked Lists uses
Binary Search Trees (BST). This brings the worst case time complexity down by a significant factor.
Every bucket in the Hash Map holds a pointer to a BST for that bucket. We can then use BSTs to our advantage.


Time Complexity Ananlysis:
               Average Case         Worst Case
Insertion        O(Log N)            O(N)
Search           O(Log N)            O(N)
Max Index        O(Log N)            O(N)
Delete           O(Log N)            O(N)
For Each         O(N)                O(N)

N -> The total number of values stored in the data structure.
All logarithms are in base 2.
This is better than traditional Hash Map since in hash maps most of the times due to collisions the time complexity would be O(N)
for insert, search, delete and max index whereas in this case it is only when the values in the BST are sorted which is unlikely
since the hashing function shuffles the values.
This Data Structure is better than using a traditional BST because for larger values of N, the BST can get very large and the values
may appear in sorted order which can harm the complexity. This is taken care of by the hashing function in my case.


Space Complexity Analysis:
               Average Case         Worst Case
Insertion        O(Log N)            O(N)
Search           O(Log N)            O(N)
Max Index        O(1)                O(1)
Delete           O(Log N)            O(N)
For Each         O(Log N)            O(N)

The Overall Space taken up by the Hash Map is of the order of O(N) since we only allocate space for the values being used
by the user.


Comparison of Run Time and Memory usage:
Run Time Analysis
                   BSA          Hash Map with BST      input values
factorial        O(Log N)            O(N)                   
sieve            O(Log N)            O(N)
fibmemo          O(1)                O(1)
factorial        O(Log N)            O(N)
sieve            O(Log N)            O(N)
fibmemo
