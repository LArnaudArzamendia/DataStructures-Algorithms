import numpy as np
import random
import string
import sys

def gen_codes(N, out_path):
    A = np.random.randint(0,10, (N,4))  # incluye 9
    codes = [
        f"{d[0]}{d[1]}{d[2]}{d[3]}{random.choice(string.ascii_uppercase)}{random.choice(string.ascii_uppercase)}"
        for d in A
    ]
    with open(out_path, "w+") as f:
        for c in codes:
            f.write(c + "\n")
    print(f"done: {out_path} ({N} códigos)")

if __name__ == "__main__":
    # Uso: python generate_codes.py 500000 codes_500K.txt
    N = int(sys.argv[1]) if len(sys.argv) > 1 else 500000
    out = sys.argv[2] if len(sys.argv) > 2 else "codes_500K.txt"
    gen_codes(N, out)