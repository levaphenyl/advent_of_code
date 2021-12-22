import pandas as pd


def iter_energy(grid):
    grid = grid + 1
    blasting = []
    for i in grid.index:
        for j in grid.columns:
            if grid.at[i,j] > 9:
                blasting.append((i, j))

    while len(blasting):
        i, j = blasting.pop()
        for k in [i - 1, i, i + 1]:
            for l in [j - 1, j, j + 1]:
                if k in grid.index and l in grid.columns and grid.at[k,l] <= 9:
                    grid.at[k,l] += 1
                    if grid.at[k,l] > 9:
                        blasting.append((k, l))

    grid[grid > 9] = 0
    return grid


def count_flashes(grid, n_iter=100):
    ret = 0
    for _ in range(n_iter):
        grid = iter_energy(grid)
        ret += (grid == 0).sum().sum()

    return ret


def find_sync(grid, n_iter=10000):
    for i in range(n_iter):
        grid = iter_energy(grid)
        if (grid == 0).all().all():
            break

    return i + 1


def load_input(path):
    with open(path, 'r') as f:
        rows = f.readlines()

    ret = [ [ int(c) for c in r.strip() ] for r in rows ]
    ret = pd.DataFrame(ret, dtype="int32")
    return ret


def print_results(input, part):
    if part == 1:
        ret = count_flashes(input)
    elif part == 2:
        ret = find_sync(input)

    print("Answer for part {} is {}".format(part, ret))


if __name__ == "__main__":
    test_input = pd.DataFrame([
        [ 5, 4, 8, 3, 1, 4, 3, 2, 2, 3, ],
        [ 2, 7, 4, 5, 8, 5, 4, 7, 1, 1, ],
        [ 5, 2, 6, 4, 5, 5, 6, 1, 7, 3, ],
        [ 6, 1, 4, 1, 3, 3, 6, 1, 4, 6, ],
        [ 6, 3, 5, 7, 3, 8, 5, 4, 7, 8, ],
        [ 4, 1, 6, 7, 5, 2, 4, 6, 4, 5, ],
        [ 2, 1, 7, 6, 8, 4, 1, 7, 2, 1, ],
        [ 6, 8, 8, 2, 8, 8, 1, 1, 3, 4, ],
        [ 4, 8, 4, 6, 8, 4, 8, 5, 5, 4, ],
        [ 5, 2, 8, 3, 7, 5, 1, 5, 2, 6, ],
    ], dtype="int32")
    assert count_flashes(test_input) == 1656
    assert find_sync(test_input) == 195

    input_path = "data/input-day11.txt"
    input_data = load_input(input_path)
    print_results(input_data, 1)
    print_results(input_data, 2)

