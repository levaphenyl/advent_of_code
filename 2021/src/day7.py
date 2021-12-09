import pandas as pd


def compute_distances(locations):
    # Euclidean distance in 1-D space is equivalent to the absolute value of
    # the difference.
    dist_mat = { i: [ abs(l - p) for l in locations ]
                 for i, p in enumerate(range(max(locations) + 1)) }
    dist_mat = pd.DataFrame(dist_mat, dtype="int32")
    return dist_mat


def compute_noneuclidean(locations):
    # The cost at step i is computed from the previous step: c_i = c_{i-1} + 1.
    # It is an arithmetic progression, thus: c_n = c_0 + n * inc.
    dist_mat = { i: [ sum(range(abs(l - p) + 1)) for l in locations ]
                 for i, p in enumerate(range(max(locations) + 1)) }
    dist_mat = pd.DataFrame(dist_mat, dtype="int32")
    return dist_mat


def load_data(path):
    with open(path, "r") as f:
        rows = f.readlines()

    input = rows[0].strip()
    input = [ int(c) for c in input.split(',') ]
    return input


def print_results(input, part):
    if part == 1:
        dist_mat = compute_distances(input)
    elif part == 2:
        dist_mat = compute_noneuclidean(input)

    ret = dist_mat.sum().min()
    print("Answer for part {}: {}.".format(part, ret))


if __name__ == "__main__":
    test_input = [ 16, 1, 2, 0, 4, 2, 7, 1, 2, 14, ]
    assert compute_distances(test_input).sum().min() == 37
    assert compute_noneuclidean(test_input).sum().min() == 168

    input_path = "data/input-day7.txt"
    input_pop = load_data(input_path)
    print_results(input_pop, 1)
    print_results(input_pop, 2)

