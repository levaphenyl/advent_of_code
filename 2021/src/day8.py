SEGMAP = {
    0: "abcefg",
    1: "cf",
    2: "acdeg",
    3: "acdfg",
    4: "bcdf",
    5: "abdfg",
    6: "abdefg",
    7: "acf",
    8: "abcdefg",
    9: "abcdfg",
}


def count_segments():
    count = { k: len(v) for k, v in SEGMAP.items() }
    reverse_cnt = {}
    for k, v in count.items():
        if v in reverse_cnt:
            reverse_cnt[v].append(k)
        else:
            reverse_cnt[v] = [ k, ]

    return reverse_cnt


def digit_diff(segments1, segments2):
    diffchars = [ c for c in segments1 if c not in segments2 ]
    return "".join(diffchars)


def print_digit_diff():
    for k1, v1 in SEGMAP.items():
        for k2, v2 in SEGMAP.items():
            if len(v1) > len(v2):
                t = digit_diff(v1, v2)
                print("{}-{}: {}".format(k1, k2, t))


def replace_unique_values(values):
    segcount = count_segments()
    replaced = []
    for val in values:
        rep = [ segcount[len(d)][0] if len(segcount[len(d)]) == 1 else d
                for d in val ]
        replaced.append(rep)

    return replaced


def decode(patterns, values):
    # Digits with unique number of segments: 1, 4, 7, 8.
    # Segment difference 7 - 1 = a
    # Segment difference 9 - 3 = b
    # Segment difference 8 - 6 = c
    # Segment difference 8 - 0 = d
    # Segment difference 8 - 9 = e
    # Knowing c and 1 => f
    # Knowing all others => g
    ret = []
    for i in range(len(values)):
        pattern = patterns[i]
        value = values[i]
        decode_map = {}
        for p in patterns:
            if len(p) == 2:
                one = p
            elif len(p) == 3:
                seven = p
            elif len(p) == 4:
                four = p
            elif len(p) == 7:
                eight = p

    print(count_segments())
    print_digit_diff()


def count_digits(values):
    is_digit = [ isinstance(d, int) for val in values for d in val ]
    return sum(is_digit)


def load_data(path):
    with open(path, "r") as f:
        rows = f.readlines()

    input = [ r.strip().split(' | ') for r in rows ]
    return [ r[0].split() for r in input ], [ r[1].split() for r in input ]


def print_results(values, part):
    if part == 1:
        replaced = replace_unique_values(values)
    elif part == 2:
        pass

    ret = count_digits(replaced)
    print("Answer for part {}: {}.".format(part, ret))


if __name__ == "__main__":
    test_patterns = [
        [ "be", "cfbegad", "cbdgef", "fgaecd", "cgeb", "fdcge", "agebfd", "fecdb", "fabcd", "edb", ],
        [ "edbfga", "begcd", "cbg", "gc", "gcadebf", "fbgde", "acbgfd", "abcde", "gfcbed", "gfec", ],
        [ "fgaebd", "cg", "bdaec", "gdafb", "agbcfd", "gdcbef", "bgcad", "gfac", "gcb", "cdgabef", ],
        [ "fbegcd", "cbd", "adcefb", "dageb", "afcb", "bc", "aefdc", "ecdab", "fgdeca", "fcdbega", ],
        [ "aecbfdg", "fbg", "gf", "bafeg", "dbefa", "fcge", "gcbea", "fcaegb", "dgceab", "fcbdga", ],
        [ "fgeab", "ca", "afcebg", "bdacfeg", "cfaedg", "gcfdb", "baec", "bfadeg", "bafgc", "acf", ],
        [ "dbcfg", "fgd", "bdegcaf", "fgec", "aegbdf", "ecdfab", "fbedc", "dacgb", "gdcebf", "gf", ],
        [ "bdfegc", "cbegaf", "gecbf", "dfcage", "bdacg", "ed", "bedf", "ced", "adcbefg", "gebcd", ],
        [ "egadfb", "cdbfeg", "cegd", "fecab", "cgb", "gbdefca", "cg", "fgcdab", "egfdb", "bfceg", ],
        [ "gcafb", "gcf", "dcaebfg", "ecagb", "gf", "abcdeg", "gaef", "cafbge", "fdbac", "fegbdc", ],
    ]
    test_values = [
        [ "fdgacbe", "cefdb", "cefbgd", "gcbe" ],
        [ "fcgedb", "cgb", "dgebacf", "gc" ],
        [ "cg", "cg", "fdcagb", "cbg" ],
        [ "efabcd", "cedba", "gadfec", "cb" ],
        [ "gecf", "egdcabf", "bgf", "bfgea" ],
        [ "gebdcfa", "ecba", "ca", "fadegcb" ],
        [ "cefg", "dcbef", "fcge", "gbcadfe" ],
        [ "ed", "bcgafe", "cdgba", "cbgef" ],
        [ "gbdfcae", "bgc", "cg", "cgb" ],
        [ "fgae", "cfgab", "fg", "bagce" ],
    ]
    test_res2 = [ 8394, 9781, 1197, 9361, 4873, 8418, 4548, 1625, 8717, 4315, ]
    assert count_digits(replace_unique_values(test_values)) == 26
    decode(test_patterns, test_values)

    input_path = "data/input-day8.txt"
    input_patn, input_vals = load_data(input_path)
    print_results(input_vals, 1)

