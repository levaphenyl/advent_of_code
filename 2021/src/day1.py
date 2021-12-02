import pandas as pd


def count_uphill(depth_df):
    higher = depth_df.diff() > 0
    return higher.sum().values[0]


def part1(path):
    inputdf = pd.read_csv(path, names=["depth"])
    ret = count_uphill(inputdf)
    print("Answer for part 1: {}.".format(ret))


def windowed_uphill(depth_df):
    aggregate = depth_df.rolling(window=3).sum()
    return count_uphill(aggregate)


def part2(path):
    inputdf = pd.read_csv(path, names=["depth"])
    ret = windowed_uphill(inputdf)
    print("Answer for part 2: {}.".format(ret))


if __name__ == "__main__":
    test_report = [ 199, 200, 208, 210, 200, 207, 240, 269, 260, 263, ]
    test_report = pd.DataFrame(test_report, columns=["depth",])
    assert count_uphill(test_report) == 7
    assert windowed_uphill(test_report) == 5

    input_path = "data/input-day1.txt"
    part1(input_path)
    part2(input_path)

