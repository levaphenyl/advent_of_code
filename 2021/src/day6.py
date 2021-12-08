import pandas as pd


def model_fish_state(initial, n_iter):
    # NB: this implementation is taking exponential memory!
    new_state = initial.copy()
    for _ in range(n_iter):
        new_state = new_state - 1
        n_new = (new_state < 0).sum()
        if n_new > 0:
            new_fishes = pd.Series([ 8 for _ in range(n_new) ])
            new_state = new_state.append(new_fishes, ignore_index=True)
            new_state.rename("fish_state", inplace=True)
            new_state.replace(-1, 6, inplace=True)

    return new_state


def model_fish_state_efficiently(initial, n_iter):
    state_map = initial.groupby(initial).count()
    state_map = state_map.reindex(range(-1, 9), fill_value=0)
    for _ in range(n_iter):
        state_map = state_map.shift(periods=-1, fill_value=0)
        state_map[6] += state_map[-1]
        state_map[8] += state_map[-1]
        state_map[-1] = 0

    return state_map


def load_data(path):
    with open(path, "r") as f:
        rows = f.readlines()

    input = rows[0].strip()
    input = [ int(c) for c in input.split(',') ]
    input = pd.Series(input, name="fish_state",dtype="int32")
    return input


def print_results(initial, part):
    if part == 1:
        end_state = model_fish_state(initial, 80)
        ret = len(end_state)
    elif part == 2:
        end_state = model_fish_state_efficiently(initial, 256)
        ret = end_state.sum()

    print("Answer for part {}: {}.".format(part, ret))


if __name__ == "__main__":
    test_pop = pd.Series([ 3, 4, 3, 1, 2, ], name="fish_state", dtype="int32")
    test_res1 = pd.Series([ 6, 0, 6, 4, 5, 6, 0, 1, 1, 2, 6, 0, 1, 1, 1, 2,
                               2, 3, 3, 4, 6, 7, 8, 8, 8, 8, ],
                               name="fish_state", dtype="int32")
    assert (model_fish_state(test_pop, 18) == test_res1).all()
    assert model_fish_state_efficiently(test_pop, 256).sum() == 26984457539

    input_path = "data/input-day6.txt"
    input_pop = load_data(input_path)
    print_results(input_pop, 1)
    print_results(input_pop, 2)

