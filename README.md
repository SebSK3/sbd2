don't really want to think much about it, gonna just copy sbd1 project
and call it a day

Index file has indexes of first elements of pages in main_tape.

Every page in main_tape has it's own overflow page.

Every record can point to key that should be after it
(we call it overflow).

When we insert, we search for the predecessor of searched key.
If we can't insert it directly after predecessor, then we insert it in **overflow chain**.
When we find it impossible to insert in overflow chain (overflow tape is full), we reorganise the file.

**Overflow chain** has records and pointers to next overflow pointer. We treat it somewhat like linked list.


My idea for this is creating three files:
- index.txt
- main_tape.txt
- overflow.txt

In `index.txt`:

| 4 Bytes | 4 Bytes |
| :-: | :-: |
| Key (int) | Page (int) |

In `main_tape.txt`:

| 4 Bytes | X Bytes | 4 Bytes |
| :-: | :-: | :-: |
| Key (int) | YOUR DATA | OverflowPointer (int) |

In `overflow.txt`:

| 4 Bytes | X Bytes | 4 Bytes |
| :-: | :-: | :-: |
| Key (int) | YOUR DATA | OverflowPointer (int) |

Below is example, where page size is of 3 records. This example can't really happen
in real scenario, but it is there to visualize there being no restrictions
on pointer placement.

Index file:

| File offset | Page | Key |
|-------------|------|-----|
| #1          | 1    | 1   |
| #2          | 2    | 8   |
| #2          | 3    | 16   |


Main Tape file:

| File offset | Key | Data | Pointer |
|-------------|:---:|:----:|:-------:|
| Page 1      |     |      |         |
| #1           |  1  |  ... |   7      |
| #2           |  5  |  ... |  EMPTY       |
| #3           |  6  |  ... |    EMPTY     |
| Page 2      |     |      |         |
| #4           | 8   | ...  |  EMPTY       |
| #5           | 11  | ...  |   EMPTY      |
| #6           | EMPTY  | ...  |    EMPTY     |
| Page 3      |     |      |         |
| #7           | 16   | ...  |    EMPTY     |
| #8           | 19  | ...  |  EMPTY       |
| #9           | 20  | ...  |    EMPTY     |

Overflow file:

| File offset | Key | Data | Pointer |
|-------------|:---:|:----:|:-------:|
| Page 1      |     |      |         |
| #1           |  EMPTY  |  ... |    EMPTY     |
| #2           |  EMPTY  |  ... |   EMPTY      |
| #3           |  EMPTY  |  ... |   EMPTY      |
| Page 2      |     |      |         |
| #4           | 3   | ...  |    8     |
| #5           | EMPTY  | ...  |  EMPTY       |
| #6           | EMPTY  | ...  |     EMPTY    |
| Page 3      |     |      |         |
| #7           | 2   | ...  |   4      |
| #8           | EMPTY  | ...  |   EMPTY      |
| #9           | 4  | ...  |    EMPTY     |

The situtation in overflow file cannot happen (you will fill it from #1 increasing). The situation presented is for visualisation, of how you need to have implemented jumping around.