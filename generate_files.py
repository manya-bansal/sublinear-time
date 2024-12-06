import random
import os
import argparse

def permute_bytes(input_file, output_file, fraction):
    # Step 1: Read the contents of the input file
    with open(input_file, 'rb') as f:
        data = bytearray(f.read())  # Read the file as a mutable bytearray
    
    # Step 2: Calculate the number of bytes to permute
    num_bytes = len(data)
    num_to_permute = int(num_bytes * fraction)
    
    # Step 3: Select random indices and permute the corresponding bytes
    indices_to_permute = random.sample(range(num_bytes), num_to_permute)
    selected_bytes = [data[i] for i in indices_to_permute]
    
    random.shuffle(selected_bytes)  # Shuffle the selected bytes
    
    # Step 4: Replace the permuted bytes in the original bytearray
    for idx, byte in zip(indices_to_permute, selected_bytes):
        data[idx] = byte
    
    # Step 5: Write the modified content to the output file
    with open(output_file, 'wb') as f:
        f.write(data)



def generate_random_file(file_name, num_bytes):
    # Open the file in write binary mode
    with open(file_name, 'wb') as f:
        # Write num_bytes of random data to the file
        f.write(os.urandom(num_bytes))  # os.urandom generates random bytes


def main():
    # Set up argument parsing
    parser = argparse.ArgumentParser(description="Permute a fraction of the bytes in a file.")
    parser.add_argument("base_file", help="The input file to permute")
    parser.add_argument("file_size", type=int, help="Size of the file to generate in bytes")
    parser.add_argument("fractions", nargs='+', type=float, help="A list of fractions of bytes to permute (e.g., 0.1 0.2 0.5)")

    # Parse the command-line arguments
    args = parser.parse_args()
    generate_random_file(args.base_file, args.file_size)
     # Process each fraction and generate output files
    for fraction in args.fractions:
        diff_file = f"files/diff_{int(fraction * 100)}.bin"  # e.g., output_fraction_10.bin
        print(f"Processing with fraction {fraction}...")
        permute_bytes(args.base_file, diff_file, fraction)
        print(f"Generated: {diff_file}")

# Run the script if executed as the main program
if __name__ == '__main__':
    main()