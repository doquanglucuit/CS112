import random
import time
from multiprocessing import Pool, cpu_count

def generate_matrix(rows, cols):
    return [[random.randint(-5, 5) for _ in range(cols)] for _ in range(rows)]

# Hàm nhân ma trận tuần tự
def multiply_matrices_sequential(A, B):
    rows_A, cols_A = len(A), len(A[0])
    rows_B, cols_B = len(B), len(B[0])

    if cols_A != rows_B:
        print("Không thể nhân vì kích thước của ma trận không khớp")
        return []

    C = [[0 for _ in range(cols_B)] for _ in range(rows_A)]
    for i in range(rows_A):
        for j in range(cols_B):
            for k in range(cols_A):
                C[i][j] += A[i][k] * B[k][j]
    return C

def compute_chunk(args):
    A, B, start_row, end_row = args
    cols_B = len(B[0])
    cols_A = len(A[0])
    C_chunk = []
    for i in range(start_row, end_row):
        row = []
        for j in range(cols_B):
            value = sum(A[i][k] * B[k][j] for k in range(cols_A))
            row.append(value)
        C_chunk.append(row)
    return C_chunk

def multiply_matrices_parallel(A, B):
    rows_A, cols_A = len(A), len(A[0])
    rows_B, cols_B = len(B), len(B[0])

    if cols_A != rows_B:
        print("Không thể nhân vì kích thước của ma trận không khớp")
        return []

    num_processes = cpu_count()
    chunk_size = rows_A // num_processes
    ranges = [(A, B, i, min(i + chunk_size, rows_A)) for i in range(0, rows_A, chunk_size)]

    with Pool(num_processes) as pool:
        results = pool.map(compute_chunk, ranges)

    return [row for chunk in results for row in chunk]

# Hàm so sánh và đo thời gian
def main():
    size = 400
    A = generate_matrix(size, size)
    B = generate_matrix(size, size)

    print("Với ma trận kích thước 400 x 400:")

    # Thực hiện tuần tự
    start_time = time.time()
    result_seq = multiply_matrices_sequential(A, B)
    seq_time = time.time() - start_time

    print(f"Tuần tự: {seq_time:.6f}s")

    # Thực hiện song song
    start_time = time.time()
    result_par = multiply_matrices_parallel(A, B)
    par_time = time.time() - start_time

    print(f"Song song: {par_time:.6f}s")

    test_cases = [50, 100, 300, 500]
    for size in test_cases:
        A = generate_matrix(size, size)
        B = generate_matrix(size, size)

        print(f"Với ma trận kích thước{size} x {size}:")

        # Thực hiện tuần tự
        start_time = time.time()
        results = multiply_matrices_sequential(A, B)
        seq_time = time.time() - start_time
        print(f"Tuần tự: {seq_time:.6f}s")

        # Thực hiện song song
        start_time = time.time()
        results = multiply_matrices_parallel(A, B)
        par_time = time.time() - start_time
        print(f"Song song: {par_time:.6f}s")

if __name__ == "__main__":
    main()