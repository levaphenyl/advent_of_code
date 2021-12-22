import numpy as np


BRACKET_MAP = { '(': ')', '<': '>', '[': ']', '{': '}', }


def check_syntax(line):
    opening = line[0]
    while len(line) > 1:
        next_chr = line[1]
        if next_chr in BRACKET_MAP:
            line = check_syntax(line[1:])
        elif next_chr != BRACKET_MAP[opening]:
            raise SyntaxError("check_syntax: expected {}, got {}.".format(BRACKET_MAP[opening], next_chr))
        else:
            return line[1:]

    raise IndexError("check_syntax: Line incomplete.")


def complete_line(line):
    opening = [ line[0], ]
    for c in line[1:]:
        if c in BRACKET_MAP:
            opening.append(c)
        else:
            opening.pop()

    ret = ''.join([ BRACKET_MAP[c] for c in opening[::-1] ])
    return ret


def check_first_corrupt(lines):
    score_table = { ')': 3, ']': 57, '}': 1197, '>': 25137, }
    ret = 0
    for l in lines:
        try:
            check_syntax(l)
        except SyntaxError as err:
            faulty_chr = err.__str__()[-2]
            ret += score_table[faulty_chr]
        except IndexError:
            pass

    return ret


def score_complete(lines):
    score_table = { ')': 1, ']': 2, '}': 3, '>': 4, }
    ret = []
    incomplete = []
    for l in lines:
        try:
            check_syntax(l)
        except IndexError:
            pass
        except SyntaxError:
            continue

        incomplete.append(l)

    for l in incomplete:
        score = 0
        line_end = complete_line(l)
        for c in line_end:
            score = score * 5 + score_table[c]

        ret.append(score)

    ret = np.median(ret)
    return ret


def load_input(path):
    with open(path, 'r') as f:
        rows = f.readlines()

    ret = [ r.strip() for r in rows ]
    return ret


def print_results(input, part):
    if part == 1:
        ret = check_first_corrupt(input)
    elif part == 2:
        ret = score_complete(input)

    print("Answer for part {} is {}".format(part, ret))


if __name__ == "__main__":
    test_input = [
        "[({(<(())[]>[[{[]{<()<>>",
        "[(()[<>])]({[<{<<[]>>(",
        "{([(<{}[<>[]}>{[]{[(<()>",
        "(((({<>}<{<{<>}{[]{[]{}",
        "[[<[([]))<([[{}[[()]]]",
        "[{[{({}]{}}([{[{{{}}([]",
        "{<[[]]>}<{[{[{[]{()[[[]",
        "[<(<(<(<{}))><([]([]()",
        "<{([([[(<>()){}]>(<<{{",
        "<{([{{}}[<[[[<>{}]]]>[]]",
    ]
    assert check_first_corrupt(test_input) == 26397
    assert score_complete(test_input) == 288957

    input_path = "data/input-day10.txt"
    input_data = load_input(input_path)
    print_results(input_data, 1)
    print_results(input_data, 2)

