don't really want to think much about it, gonna just copy sbd1 project
and call it a day

Index file has indexes of first elements of pages in main_tape.

Every page in main_tape has it's own overflow page.

Every record can point to key that should be after it
(we call it overflow).

When we insert, we search for the predecessor of searched key.
If we can't insert it directly after predecessor, then we insert it in **overflow chain**.
When we find it impossible to insert in overflow chain, we reorganise the file.

**Overflow chain** has records and pointers to next overflow pointer. The overflow pointer in overflow page can be only within the same overflow page. We treat is somewhat like linked list.


My idea for this is creating three files:
- index.txt
- main_tape.txt
- overflow.txt

In `index.txt`:

| 4 Bytes | 4 Bytes |
| :-: | :-: |
| Key (int) | Page (int) |

In `main_tape.txt`:

| 4 Bytes | 4 Bytes | 4 Bytes | 4 Byte |
| :-: | :-: | :-: | :-: |
| Key (int) | Base (int) | Height (int) | OverflowPointer (int) |

In `overflow.txt`:

| 4 Bytes | 4 Bytes | 4 Bytes | 4 Byte |
| :-: | :-: | :-: | :-: |
| Key (int) | Base (int) | Height (int) | OverflowPointer (int) |
