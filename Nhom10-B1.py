import math
import time
import random
from multiprocessing import Pool, cpu_count
from concurrent.futures import ThreadPoolExecutor

# Sinh danh sách số ngẫu nhiên
def gen(test_size):
    return [random.randint(10**9, 10**10) for _ in range(test_size)]

def is_prime_sequential(x):
    if x <= 1:
        return False
    if x <= 3:
        return True
    if x % 2 == 0 or x % 3 == 0:
        return False
    for i in range(5, int(math.sqrt(x)) + 1, 6):
        if x % i == 0 or x % (i + 2) == 0:
            return False
    return True

def check_range(args):
    x, start, end = args
    for i in range(start, end, 6):
        if x % i == 0 or x % (i + 2) == 0:
            return False
    return True

def is_prime_parallel(x):
    if x <= 1:
        return False
    if x <= 3:
        return True
    if x % 2 == 0 or x % 3 == 0:
        return False

    sqrt_x = int(math.sqrt(x)) + 1
    step = 1000  # Kích thước mỗi nhóm kiểm tra

    ranges = [(x, i, min(i + step, sqrt_x)) for i in range(5, sqrt_x, step)]

    with Pool(cpu_count()) as pool:
        results = pool.map(check_range, ranges)
        return all(results)

# Hàm chạy thử nghiệm
def main():
    test_cases = [1000000000000091, 10000000000000099, 100000000000000049]
    for x in test_cases:
        print(f"X = {x}")

        # Thực hiện tuần tự
        start_time = time.time()
        result_seq = is_prime_sequential(x)
        seq_time = time.time() - start_time
        print(f"Tuan tu: {result_seq}, Time: {seq_time:.6f}s")

        # Thực hiện song song
        start_time = time.time()
        result_par = is_prime_parallel(x)
        par_time = time.time() - start_time
        print(f"Song song: {result_par}, Time: {par_time:.6f}s")


    test_sizes = [20, 50, 100, 200]
    for test_size in test_sizes:
        print(f"Số lượng testcase = {test_size}")
        
        # Sinh danh sách số
        tests = gen(test_size)

        # Kiểm tra tuần tự
        start_time = time.time()
        results_seq = [is_prime_sequential(x) for x in tests]
        seq_time = time.time() - start_time
        print(f"Tuần tự: {seq_time:.6f}s")

        # Kiểm tra song song (toàn bộ danh sách)
        start_time = time.time()
        with ThreadPoolExecutor() as executor:
            results_par = list(executor.map(is_prime_parallel, tests))
        par_time = time.time() - start_time
        print(f"Song song: {par_time:.6f}s")

if __name__ == "__main__":
    main()