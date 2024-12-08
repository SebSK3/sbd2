import random
def generate_non_repeating_numbers(start, end):
    # Create a list of numbers from start to end
    numbers = list(range(start, end + 1))
    # Shuffle the list to make it non-repeating
    random.shuffle(numbers)
    return numbers

# Generate numbers from 1 to 1000
non_repeating_numbers = generate_non_repeating_numbers(1, 1000)

for i in non_repeating_numbers:
    print("insert " + str(i) + " " + str(i) + " " + str(i))