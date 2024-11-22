import subprocess
import statistics
import argparse

def run_benchmark(command, num_runs):
    """
    Run a command multiple times and collect its execution times.
    
    Args:
        command (list): Command to run as a list of strings
        num_runs (int): Number of times to run the command
    
    Returns:
        tuple: (mean time, standard deviation of times)
    """
    times = []
    
    for _ in range(num_runs):
        # Run the command and capture output
        result = subprocess.run(command, capture_output=True, text=True)
        
        # Parse the final CPU time from the last line
        last_line = result.stdout.strip().split('\n')[-1]
        time_str = last_line.split(':')[-1].strip().split()[0]
        times.append(float(time_str))
    
    # Calculate statistics
    mean_time = statistics.mean(times)
    std_dev = statistics.stdev(times) if num_runs > 1 else 0
    
    return mean_time, std_dev

def main():
    # Set up argument parsing
    parser = argparse.ArgumentParser(description='Benchmark a command multiple times')
    parser.add_argument('-n', '--num-runs', type=int, default=10, 
                        help='Number of times to run the command (default: 10)')
    parser.add_argument('-c', '--command', nargs='+', 
                        default=['./build/SimpleGR', '-f', 'benchmarks/adaptec1.simple.gr'],
                        help='Command to benchmark')
    
    # Parse arguments
    args = parser.parse_args()
    
    # Run benchmark
    mean_time, std_dev = run_benchmark(args.command, args.num_runs)
    
    # Print results
    print(f"Benchmark Results (n = {args.num_runs}):")
    print(f"Mean Execution Time: {mean_time:.6f} seconds")
    print(f"Standard Deviation: {std_dev:.6f} seconds")

if __name__ == '__main__':
    main()
