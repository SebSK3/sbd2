don't really want to think much about it, gonna just copy sbd1 project
and call it a day

My way to handle overflow area:

Three files:
- index.txt
- main_tape.txt
- overflow.txt

In `index.txt`:

| 4 Bytes | 4 Bytes |
| :-: | :-: |
| Key (int) | Page (int) |

In `main_tape.txt`:

| 4 Bytes | 4 Bytes | 4 Bytes | 1 Byte |
| :-: | :-: | :-: | :-: | :-: |
| Key (int) | Base (int) | Height (int) | HasOverflow (bool) |

In `overflow.txt`:

| 4 Bytes | 4 Bytes | 4 Bytes |
| :-: | :-: | :-: |
| Key (int) | Base (int) | Height (int) |


What happens if `HasOverflow` is set to 1 (true), then we 


