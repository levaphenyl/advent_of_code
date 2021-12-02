import pandas as pd


def move(directions):
    x = 0 # horizontal counter
    z = 0 # vertical counter, pointing downwards
    for d_str in directions:
        hint, val = d_str.split()
        val = int(val)
        if "forward" in hint:
            x += val
        elif "up" in hint:
            z -= val
        elif "down" in hint:
            z += val
        else:
            raise ValueError("day2.move: Unknown direction hint: {}".format(hint))

    return x, z


def load_data(path):
    with open(path, "r") as f:
        directions = f.readlines()

    directions = [ s.strip() for s in directions if len(s) > 1]
    return directions


def part1(path):
    directions = load_data(path)
    x, z = move(directions)
    ret = x * z
    print("Answer for part 1: {}.".format(ret))


def move_with_aim(directions):
    x = 0 # horizontal counter
    z = 0 # vertical counter, pointing downwards
    aim = 0 # the submarine is not translating vertically!
    for d_str in directions:
        hint, val = d_str.split()
        val = int(val)
        if "forward" in hint:
            x += val
            z += aim * val
        elif "up" in hint:
            aim -= val
        elif "down" in hint:
            aim += val
        else:
            raise ValueError("day2.move_with_aim: Unknown direction hint: {}".format(hint))

    return x, z


def part2(path):
    directions = load_data(path)
    x, z = move_with_aim(directions)
    ret = x * z
    print("Answer for part 2: {}.".format(ret))


if __name__ == "__main__":
    test_move = [ "forward 5", "down 5", "forward 8", "up 3", "down 8", "forward 2", ]
    assert move(test_move) == (15, 10)
    assert move_with_aim(test_move) == (15, 60)

    input_path = "data/input-day2.txt"
    part1(input_path)
    part2(input_path)

