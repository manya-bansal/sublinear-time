import random
import os
import argparse
import struct

def permute_unsigned(input_file, output_file, fraction):
    # Step 1: Read the contents of the input file as bytes
    with open(input_file, 'rb') as f:
        data = bytearray(f.read())  # Read the file as a mutable bytearray
    
    # Step 2: Calculate the number of unsigned 8-byte integers (64 bits) in the file
    num_bytes = len(data)
    num_integers = num_bytes // 8  # Assuming 8-byte unsigned integers
    
    # Step 3: Select the number of integers to permute
    num_to_permute = int(num_integers * fraction)
    
    # Step 4: Extract unsigned 8-byte integers from the bytearray
    unsigned_integers = [struct.unpack('<Q', data[i:i+8])[0] for i in range(0, num_bytes, 8)]
    
    # Step 5: Select random indices and permute the corresponding integers
    indices_to_permute = random.sample(range(num_integers), num_to_permute)
    selected_integers = [unsigned_integers[i] for i in indices_to_permute]
    
    random.shuffle(selected_integers)  # Shuffle the selected integers
    
    # Step 6: Replace the permuted integers in the original bytearray
    for idx, new_int in zip(indices_to_permute, selected_integers):
        # Pack the permuted integer back into 8 bytes and update the bytearray
        data[idx*8:(idx+1)*8] = struct.pack('<Q', new_int)
    
    # Step 7: Write the modified content to the output file
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
    parser.add_argument("file_size", type=int, help="Size of the file to generate in  bytes")
    parser.add_argument("fractions", nargs='+', type=float, help="A list of fractions of bytes to permute (e.g., 0.1 0.2 0.5)")

    # Parse the command-line arguments
    args = parser.parse_args()
    generate_random_file(args.base_file, args.file_size * 8)
     # Process each fraction and generate output files
    for fraction in args.fractions:
        diff_file = f"files/diff_{int(fraction * 100)}.bin"  # e.g., output_fraction_10.bin
        print(f"Processing with fraction {fraction}...")
        permute_unsigned(args.base_file, diff_file, fraction)
        print(f"Generated: {diff_file}")

# Run the script if executed as the main program
if __name__ == '__main__':
    main()