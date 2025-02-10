import subprocess
import hashlib
import time
import re

class ProgramInfo:
    ITERATIONS = 1000
    VALGRIND_MEM_PATTERN = re.compile(r"total heap usage: ([\d,]+) allocs, ([\d,]+) frees, ([\d,]+) bytes allocated")
    VALGRIND_LEAK_PATTERN = re.compile(r"All heap blocks were freed -- no leaks are possible")
    VALGRIND_ERROR_PATTERN = re.compile(r"0 errors from 0 contexts")

    def __init__(self, command, path, exact_pattern_match):
        self.name = command
        self.determineExecutionTime(command, path, exact_pattern_match)
        self.checkValgrind(command, path, exact_pattern_match)
        self.hashOutput(command, path, exact_pattern_match)

    def determineExecutionTime(self, command, path, exact_pattern_match):
        m1_execution_times = []
        for _ in range(self.ITERATIONS):
            start_time = time.perf_counter()
            subprocess.run([command, path], capture_output=True)
            m1_execution_times.append(time.perf_counter() - start_time)
        self.m1_execution_time_avg = (sum(m1_execution_times) / len(m1_execution_times)) * 1000000

        m2_execution_times = []
        for _ in range(self.ITERATIONS):
            start_time = time.perf_counter()
            subprocess.run([command, path, exact_pattern_match], capture_output=True)
            m2_execution_times.append(time.perf_counter() - start_time)
        self.m2_execution_time_avg = (sum(m2_execution_times) / len(m2_execution_times)) * 1000000

    def checkValgrind(self, command, path, exact_pattern_match):
        m1_process = subprocess.run(["valgrind", command, path], capture_output=True, text=True)
        m2_process = subprocess.run(["valgrind", command, path, exact_pattern_match], capture_output=True, text=True)

        self.m1_no_leak = (self.VALGRIND_LEAK_PATTERN.search(m1_process.stderr) is not None)
        self.m2_no_leak = (self.VALGRIND_LEAK_PATTERN.search(m2_process.stderr) is not None)

        self.m1_no_error = (self.VALGRIND_ERROR_PATTERN.search(m1_process.stderr) is not None)
        self.m2_no_error = (self.VALGRIND_ERROR_PATTERN.search(m2_process.stderr) is not None)

        self.m1_no_error = (self.VALGRIND_ERROR_PATTERN.search(m1_process.stderr) is not None)
        self.m2_no_error = (self.VALGRIND_ERROR_PATTERN.search(m2_process.stderr) is not None)

        m1_heap_blocks_text = re.search(self.VALGRIND_MEM_PATTERN, m1_process.stderr)
        m2_heap_blocks_text = re.search(self.VALGRIND_MEM_PATTERN, m2_process.stderr)

        self.m1_blocks_allocated, self.m2_blocks_allocated, self.m1_bytes_allocated, self.m2_bytes_allocated = (
            (m1_heap_blocks_text.group(1).replace(',', '') if m1_heap_blocks_text else None),
            (m2_heap_blocks_text.group(1).replace(',', '') if m2_heap_blocks_text else None),
            (m1_heap_blocks_text.group(3).replace(',', '') if m1_heap_blocks_text else None),
            (m2_heap_blocks_text.group(3).replace(',', '') if m2_heap_blocks_text else None),
        )

        self.m1_bytes_allocated = int(self.m1_bytes_allocated) if self.m1_bytes_allocated else None
        self.m2_bytes_allocated = int(self.m2_bytes_allocated) if self.m2_bytes_allocated else None
        self.m1_blocks_allocated = int(self.m1_blocks_allocated) if self.m1_blocks_allocated else None
        self.m2_blocks_allocated = int(self.m2_blocks_allocated) if self.m2_blocks_allocated else None

    def hashOutput(self, command, path, exact_pattern_match):
        m1_process = subprocess.run([command, path], capture_output=True, text=True)
        self.m1_sha256 = hashlib.sha256(m1_process.stdout.encode()).hexdigest()

        m2_process = subprocess.run([command, path, exact_pattern_match], capture_output=True, text=True)
        self.m2_sha256 = hashlib.sha256(m2_process.stdout.encode()).hexdigest()

def comparePrograms(prog1, prog2):
    print()
    print("OUTPUT:")
    if prog1.m1_sha256 == prog2.m1_sha256:
        print("\tMode 1: The programs had identical outputs")
    else:
        print("\tMode 1: The programs had different outputs")
    if prog1.m2_sha256 == prog2.m2_sha256:
        print("\tMode 2: The programs had identical outputs")
    else:
        print("\tMode 2: The programs had different outputs")

    print()
    print("EXECUTION TIME:")
    print("\tProgram 1:")
    print(f"\t\tMode 1: {prog1.m1_execution_time_avg} microseconds (average across {prog1.ITERATIONS} iterations)")
    print(f"\t\tMode 2: {prog1.m2_execution_time_avg} microseconds (average across {prog1.ITERATIONS} iterations)")
    print("\tProgram 2:")
    print(f"\t\tMode 1: {prog2.m1_execution_time_avg} microseconds (average across {prog2.ITERATIONS} iterations)")
    print(f"\t\tMode 2: {prog2.m2_execution_time_avg} microseconds (average across {prog2.ITERATIONS} iterations)")
    print("\tDifference:")
    if prog1.m1_execution_time_avg < prog2.m1_execution_time_avg:
        print(f"\t\tMode 1: {prog1.name} was faster than {prog2.name} by {prog2.m1_execution_time_avg - prog1.m1_execution_time_avg} microseconds")
    elif prog2.m1_execution_time_avg < prog1.m1_execution_time_avg:
        print(f"\t\tMode 1: {prog2.name} was faster than {prog1.name} by {prog1.m1_execution_time_avg - prog2.m1_execution_time_avg} microseconds")
    else:
        print("\t\tMode 1: The programs had identical execution times")
    if prog1.m2_execution_time_avg < prog2.m2_execution_time_avg:
        print(f"\t\tMode 2: {prog1.name} was faster than {prog2.name} by {prog2.m2_execution_time_avg - prog1.m2_execution_time_avg} microseconds")
    elif prog2.m2_execution_time_avg < prog1.m2_execution_time_avg:
        print(f"\t\tMode 2: {prog2.name} was faster than {prog1.name} by {prog1.m2_execution_time_avg - prog2.m2_execution_time_avg} microseconds")
    else:
        print("\t\tMode 2: The programs had identical execution times")

    print()
    print("MEMORY USAGE:")
    print("\tProgram 1:")
    print(f"\t\tMode 1: {prog1.m1_blocks_allocated} blocks allocated ({prog1.m1_bytes_allocated} bytes)")
    print(f"\t\tMode 2: {prog1.m2_blocks_allocated} blocks allocated ({prog1.m2_bytes_allocated} bytes)")
    print("\tProgram 2:")
    print(f"\t\tMode 1: {prog2.m1_blocks_allocated} blocks allocated ({prog2.m1_bytes_allocated} bytes)")
    print(f"\t\tMode 2: {prog2.m2_blocks_allocated} blocks allocated ({prog2.m2_bytes_allocated} bytes)")
    print("\tDifference:")
    print(f"\t\tMode 1: {prog1.name} used {prog2.m1_blocks_allocated - prog1.m1_blocks_allocated} fewer blocks ({prog2.m1_bytes_allocated - prog1.m1_bytes_allocated} less bytes) than {prog2.name}")
    print(f"\t\tMode 1: {prog1.name} used {prog2.m2_blocks_allocated - prog1.m2_blocks_allocated} fewer blocks ({prog2.m2_bytes_allocated - prog1.m2_bytes_allocated} less bytes) than {prog2.name}")

ls2 = ProgramInfo("./ls2", ".", "main")
ls2Sol = ProgramInfo("./ls2Sol", ".", "main")
comparePrograms(ls2, ls2Sol)
