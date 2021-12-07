import numpy as np
import pandas as pd


def valrange(x1, x2):
    if x1 < x2:
        return range(x1, x2 + 1)
    elif x1 > x2:
        return range(x2, x1 + 1)
    else:
        return range(x1, x1 + 1)


def hv_diagram(segments):
    xymax = segments.max().max() + 1
    # Select only vertical or horizontal segments.
    select = np.logical_or( segments["x1"] == segments["x2"],
                            segments["y1"] == segments["y2"])
    segments = segments[select]
    diagram = pd.DataFrame(np.zeros((xymax, xymax)), dtype="int32")
    for x1, y1, x2, y2 in segments.values:
        if y1 == y2:
            for x in valrange(x1, x2):
                diagram.at[y1,x] += 1
        else:
            for y in valrange(y1, y2):
                diagram.at[y,x1] += 1

    return diagram


def load_data(path):
    with open(path, "r") as f:
        rows = f.readlines()

    rows = [ r.strip().replace(" -> ", ',') for r in rows ]
    rows = [ [ int(c) for c in r.split(',') ] for r in rows ]
    segments = pd.DataFrame(    rows, columns=[ "x1", "y1", "x2", "y2", ],
                                dtype="int32",)
    return segments


def part1(segments):
    diagram = hv_diagram(segments)
    ret = (diagram > 1).sum().sum()
    print("Answer for part 1: {}.".format(ret))


def affine_params(x1, x2, y1, y2):
    # Create a linear model of the form y = a*x + b.
    assert x1 != x2
    if x1 < x2:
        xa, xb, ya, yb = (x1, x2, y1, y2)
    else:
        xa, xb, ya, yb = (x2, x1, y2, y1)

    slope = (yb - ya) / (xb - xa)
    offset = (ya * xb - yb * xa) / (xb - xa)
    return slope, offset


def hvd_diagram(segments):
    xymax = segments.max().max() + 1
    diagram = pd.DataFrame(np.zeros((xymax, xymax)), dtype="int32")
    for x1, y1, x2, y2 in segments.values:
        if x1 == x2:
            for y in valrange(y1, y2):
                diagram.at[y,x1] += 1
        else:
            a, b = affine_params(x1, x2, y1, y2)
            for x in valrange(x1, x2):
                y = int(a * x + b)
                diagram.at[y,x] += 1

    return diagram


def part2(segments):
    diagram = hvd_diagram(segments)
    ret = (diagram > 1).sum().sum()
    print("Answer for part 2: {}.".format(ret))


if __name__ == "__main__":
    test_seg = pd.DataFrame([
            [ 0, 9, 5, 9 ],
            [ 8, 0, 0, 8 ],
            [ 9, 4, 3, 4 ],
            [ 2, 2, 2, 1 ],
            [ 7, 0, 7, 4 ],
            [ 6, 4, 2, 0 ],
            [ 0, 9, 2, 9 ],
            [ 3, 4, 1, 4 ],
            [ 0, 0, 8, 8 ],
            [ 5, 5, 8, 2 ], ],
        columns=[ "x1", "y1", "x2", "y2", ],
        dtype="int32",
    )
    test_res_hv = pd.DataFrame([
        [ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, ],
        [ 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, ],
        [ 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, ],
        [ 0, 1, 1, 2, 1, 1, 1, 2, 1, 1, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
        [ 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, ], ],
        dtype="int32",
    )
    test_res_hvd = pd.DataFrame([
        [ 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, ],
        [ 0, 1, 1, 1, 0, 0, 0, 2, 0, 0, ],
        [ 0, 0, 2, 0, 1, 0, 1, 1, 1, 0, ],
        [ 0, 0, 0, 1, 0, 2, 0, 2, 0, 0, ],
        [ 0, 1, 1, 2, 3, 1, 3, 2, 1, 1, ],
        [ 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, ],
        [ 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, ],
        [ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, ],
        [ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, ],
        [ 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, ], ],
        dtype="int32",
    )
    assert (hv_diagram(test_seg) == test_res_hv).all().all()
    assert (hvd_diagram(test_seg) == test_res_hvd).all().all()

    input_path = "data/input-day5.txt"
    input_seg = load_data(input_path)
    part1(input_seg)
    part2(input_seg)

