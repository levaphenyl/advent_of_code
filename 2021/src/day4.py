import numpy as np
import pandas as pd


def search_bingo(boards, draws):
    drawn = [ draws[i] for i in range(4) ]
    for d in draws[4:]:
        drawn.append(d)
        for b in boards:
            for c in b.columns:
                if b[c].isin(drawn).all():
                    return drawn, b

            for i in b.index:
                if b.loc[i,:].isin(drawn).all():
                    return drawn, b

    raise RuntimeError("day4.search_bingo: No bingo found in the boards.")


def load_data(path):
    with open(path, "r") as f:
        rows = f.readlines()

    draws = [ int(i) for i in rows[0].split(',') ]
    boards = []
    curr_brd = []
    for r in rows[1:]:
        if len(r) > 1:
            curr_brd.append([ int(i) for i in r.split() ])

        if len(curr_brd) == 5:
            boards.append(pd.DataFrame(curr_brd))
            curr_brd = []

    return draws, boards


def part1(draws, boards):
    drawn, winning = search_bingo(boards, draws)
    unmarked = winning[np.logical_not(winning.isin(drawn))].sum().sum()
    ret = unmarked * drawn[-1]
    print("Answer for part 1: {}.".format(ret))


def lose_bingo(boards, draws):
    drawn = [ draws[i] for i in range(4) ]
    for d in draws[4:]:
        drawn.append(d)
        new_boards = []
        for j, b in enumerate(boards):
            is_winning = False
            for c in b.columns:
                if b[c].isin(drawn).all():
                    is_winning = True

            for i in b.index:
                if b.loc[i,:].isin(drawn).all():
                    is_winning = True

            if len(boards) == 1 and is_winning:
                return drawn, b
            elif not is_winning:
                new_boards.append(b)

        boards = new_boards

    raise RuntimeError("day4.lose_bingo: No bingo found in the boards.")


def part2(draws, boards):
    drawn, losing = lose_bingo(boards, draws)
    print(drawn)
    print(losing)
    unmarked = losing[np.logical_not(losing.isin(drawn))].sum().sum()
    ret = unmarked * drawn[-1]
    print("Answer for part 2: {}.".format(ret))


if __name__ == "__main__":
    test_draws = [ 7, 4, 9, 5, 11, 17, 23, 2, 0, 14, 21, 24, 10, 16, 13, 6, 15 ]
    test_boards = [
            pd.DataFrame([  [ 22, 13, 17, 11, 0, ],
                            [ 8, 2, 23, 4, 24 ],
                            [ 21, 9, 14, 16, 7 ],
                            [ 6, 10, 3, 18, 5 ],
                            [ 1, 12, 20, 15, 19 ], ]),
            pd.DataFrame([  [ 3, 15, 0, 2, 22 ],
                            [ 9, 18, 13, 17, 5 ],
                            [ 19, 8, 7, 25, 23 ],
                            [ 20, 11, 10, 24, 4 ],
                            [ 14, 21, 16, 12, 6 ], ]),
            pd.DataFrame([  [ 14, 21, 17, 24, 4 ],
                            [ 10, 16, 15, 9, 19 ],
                            [ 18, 8, 23, 26, 20 ],
                            [ 22, 11, 13, 6, 5 ],
                            [ 2, 0, 12, 3, 7 ], ]),
    ]
    assert search_bingo(test_boards, test_draws) == (test_draws[:12], test_boards[2])
    assert lose_bingo(test_boards, test_draws) == (test_draws[:15], test_boards[1])

    input_path = "data/input-day4.txt"
    input_draws, input_boards = load_data(input_path)
    part1(input_draws, input_boards)
    part2(input_draws, input_boards)

