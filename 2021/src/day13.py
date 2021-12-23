import pandas as pd


def fold(map2d, hv_line):
    # Use linear symmetry to find the new point coordinates.
    # Only support horizontal and vertical lines as axis.
    axis, cut_val = hv_line
    if axis == 'x':
        folded = map2d.loc[:,:cut_val-1].copy()
        for x in map2d.columns[cut_val+1:]:
            for y in map2d.index:
                if map2d.at[y,x]:
                    xs = 2 * cut_val - x
                    folded.at[y,xs] = True

    else:
        folded = map2d.loc[:cut_val-1,:].copy()
        for x in map2d.columns:
            for y in map2d.index[cut_val+1:]:
                if map2d.at[y,x]:
                    ys = 2 * cut_val - y
                    folded.at[ys,x] = True

    return folded


def pts2map(coords):
    xmax = max([ c[0] for c in coords ])
    ymax = max([ c[1] for c in coords ])
    map2d = pd.DataFrame(False, index=range(ymax+1), columns=range(xmax+1), dtype=bool)
    for x, y in coords:
        map2d.at[y, x] = True

    return map2d


def load_input(path):
    with open(path, 'r') as f:
        rows = f.readlines()

    rows = [ r.strip() for r in rows if len(r) > 1 ]
    coords = []
    axes = []
    for r in rows:
        if r.startswith('fold'):
            dir, val = r[11:].split('=')
            axes.append((dir, int(val)))
        else:
            coords.append([ int(i) for i in r.split(',') ])

    ret = pts2map(coords)
    return ret, axes


def print_results(space, axes, part):
    if part == 1:
        ret = fold(space, axes[0]).sum().sum()
    elif part == 2:
        ret = space
        for ax in axes:
            ret = fold(ret, ax)

        ret = ret.replace({ True: '+', False: '', })

    with pd.option_context("display.max_rows", None, "display.max_columns", None):
        print("Answer for part {} is {}".format(part, ret))


if __name__ == "__main__":
    test_input = [
        [ 6, 10, ], [ 0, 14, ], [ 9, 10, ], [ 0, 3, ], [ 10, 4, ],
        [ 4, 11, ], [ 6, 0, ], [ 6, 12, ], [ 4, 1, ], [ 0, 13, ],
        [ 10, 12, ], [ 3, 4, ], [ 3, 0, ], [ 8, 4, ], [ 1, 10, ],
        [ 2, 14, ], [ 8, 10, ], [ 9, 0, ],
    ]
    test_space = pd.DataFrame([
        [ 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, ],
        [ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, ],
        [ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, ],
        [ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, ],
    ], dtype=bool)
    assert (pts2map(test_input) == test_space).all().all()
    test_folds = [ ('y', 7), ('x', 5), ]
    assert fold(test_space, test_folds[0]).sum().sum() == 17

    input_path = "data/input-day13.txt"
    input_space, input_axes = load_input(input_path)
    print_results(input_space, input_axes, 1)
    print_results(input_space, input_axes, 2)

