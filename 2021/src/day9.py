import numpy as np
import pandas as pd


def find_local_minima(heightmap, maxh=9):
    down = heightmap.shift(periods=1, axis=0, fill_value=maxh)
    up = heightmap.shift(periods=-1, axis=0, fill_value=maxh)
    right = heightmap.shift(periods=1, axis=1, fill_value=maxh)
    left = heightmap.shift(periods=-1, axis=1, fill_value=maxh)
    is_min = np.logical_and(heightmap < down, heightmap < up)
    is_min = np.logical_and(is_min, heightmap < right)
    is_min = np.logical_and(is_min, heightmap < left)
    return is_min


def find_basin_sizes(heightmap, maxh=9):
    is_basin = heightmap < maxh
    is_min = find_local_minima(heightmap, maxh)
    bsizes = []
    def _recurse_basins(k, l, past=[]):
        try:
            if is_basin.at[k,l]:
                past.append((k, l))
                ret = 1
                if (k-1, l) not in past:
                    ret += _recurse_basins(k-1, l, past)
                if (k+1, l) not in past:
                    ret += _recurse_basins(k+1, l, past)
                if (k, l-1) not in past:
                    ret += _recurse_basins(k, l-1, past)
                if (k, l+1) not in past:
                    ret += _recurse_basins(k, l+1, past)
                return ret
            else:
                return 0
        except KeyError:
            return 0


    for i, row in enumerate(is_min.values):
        for j, cell in enumerate(row):
            if cell:
                # Found a local minimum.
                # Start recursion to find the basin size.
                bsizes.append(_recurse_basins(i, j))

    return bsizes


def load_data(path):
    with open(path, "r") as f:
        rows = f.readlines()

    input = [ [ int(c) for c in r.strip() ] for r in rows ]
    return pd.DataFrame(input, dtype="int32")


def print_results(input, part):
    if part == 1:
        is_min = find_local_minima(input)
        ret = input[is_min].sum().sum() + is_min.sum().sum()
    elif part == 2:
        bsizes = find_basin_sizes(input)
        ret = np.prod(sorted(bsizes)[-3:])

    print("Answer for part {}: {}.".format(part, ret))


if __name__ == "__main__":
    test_input = pd.DataFrame([
        [ 2, 1, 9, 9, 9, 4, 3, 2, 1, 0 ],
        [ 3, 9, 8, 7, 8, 9, 4, 9, 2, 1 ],
        [ 9, 8, 5, 6, 7, 8, 9, 8, 9, 2 ],
        [ 8, 7, 6, 7, 8, 9, 6, 7, 8, 9 ],
        [ 9, 8, 9, 9, 9, 6, 5, 6, 7, 8 ],
    ], dtype="int32")
    assert test_input[find_local_minima(test_input)].sum().sum() == 11
    assert find_basin_sizes(test_input) == [ 3, 9, 14, 9, ]

    input_path = "data/input-day9.txt"
    input_data = load_data(input_path)
    print_results(input_data, 1)
    print_results(input_data, 2)

